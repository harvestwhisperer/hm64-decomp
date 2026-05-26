//! Cooperative chunked Save All: planner, job state, runner tick. See
//! docs/superpowers/specs/2026-05-01-sprite-editor-save-all-progress-design.md
//! for the full design.

use std::cell::Cell;
use std::collections::HashMap;

use hm64_sprite_editor::cmd::DirtyMask;
use hm64_sprite_editor::ui::slint_generated::MainWindow;
use slint::{ComponentHandle, Weak};

use crate::wiring::AppState;

#[derive(Debug, Clone)]
pub struct SaveTask {
    pub label: String,
    pub domain: DirtyMask,
    pub kind: SaveTaskKind,
}

#[derive(Debug, Clone)]
pub enum SaveTaskKind {
    PaletteFile { index: u32 },
    TextureCi { index: u32 },
    TexturePng { index: u32 },
    TextureCleanup,
    AnimationFile { index: u32 },
    AnimationCleanup,
    ScriptFile,
    ManifestFile,
}

pub struct SaveAllJob {
    pub queue: Vec<SaveTask>,
    pub cursor: usize,
    pub cancel: Cell<bool>,
    pub domain_remaining: HashMap<DirtyMask, usize>,
}

/// Install a pre-built plan as a SaveAllJob and kick off the runner. Caller
/// supplies the plan (use `plan_save_all` for the global Save All button or
/// one of the `plan_*_only` planners for per-tab Save buttons) and the status
/// text to show when the plan is empty (e.g. "Palettes clean").
///
/// No-op if a save job is already running.
pub fn start_job(
    state: &AppState,
    window_weak: &Weak<MainWindow>,
    plan: Vec<SaveTask>,
    empty_status: &str,
) {
    let Some(w) = window_weak.upgrade() else { return };

    // Belt-and-suspenders: refuse to start a second job while one is running.
    if w.get_save_running() {
        return;
    }

    if plan.is_empty() {
        w.set_status_text(empty_status.into());
        return;
    }

    let mut domain_remaining: HashMap<DirtyMask, usize> = HashMap::new();
    for task in &plan {
        *domain_remaining.entry(task.domain).or_insert(0) += 1;
    }

    let total = plan.len() as i32;
    let job = SaveAllJob {
        queue: plan,
        cursor: 0,
        cancel: Cell::new(false),
        domain_remaining,
    };
    *state.save_job.borrow_mut() = Some(job);

    w.set_save_running(true);
    w.set_save_total(total);
    w.set_save_done(0);
    w.set_save_current("Preparing…".into());

    // Post the first tick. The runner re-arms itself.
    schedule_tick(state.clone(), window_weak.clone());
}

/// Set the cancel flag on the current job (if any). Idempotent; safe to call
/// when no job is running.
pub fn request_cancel(state: &AppState) {
    if let Some(job) = state.save_job.borrow().as_ref() {
        job.cancel.set(true);
    }
}

fn schedule_tick(state: AppState, window_weak: Weak<MainWindow>) {
    // Non-zero duration so each tick yields to a real paint cycle. Duration::ZERO
    // can fire within the same event-loop iteration that scheduled it, which
    // collapses the whole save into one synchronous burst with no paints in
    // between — the modal would never appear.
    slint::Timer::single_shot(std::time::Duration::from_millis(1), move || {
        tick(state, window_weak);
    });
}

fn tick(state: AppState, window_weak: Weak<MainWindow>) {
    let Some(w) = window_weak.upgrade() else {
        // Window gone; drop the job and bail. No status text to set.
        *state.save_job.borrow_mut() = None;
        return;
    };

    // Pull the cancel flag *before* doing any work this tick.
    let cancelled = state
        .save_job
        .borrow()
        .as_ref()
        .map(|j| j.cancel.get())
        .unwrap_or(true);
    if cancelled {
        finalize(&state, &w, FinalizeKind::Cancelled);
        return;
    }

    // Read the next task. We clone the SaveTask out of the queue so we can
    // drop the borrow before calling `editor.borrow_mut()` inside execute.
    let task = match state.save_job.borrow().as_ref() {
        Some(job) if job.cursor < job.queue.len() => Some(job.queue[job.cursor].clone()),
        _ => None,
    };
    let Some(task) = task else {
        finalize(&state, &w, FinalizeKind::Success);
        return;
    };

    w.set_save_current(task.label.clone().into());

    let outcome = execute_task(&state, &task);

    match outcome {
        Ok(()) => {
            // Bookkeeping inside one short borrow. start_job populates
            // domain_remaining from the same plan that becomes job.queue, so
            // every executed task's domain MUST already have an entry —
            // .expect() surfaces invariant violations instead of silently
            // marking a domain clean prematurely.
            let domain_finished = {
                let mut job_ref = state.save_job.borrow_mut();
                let job = job_ref.as_mut().expect("job present");
                job.cursor += 1;
                let entry = job.domain_remaining.get_mut(&task.domain).expect(
                    "domain_remaining invariant: every queued task's domain was counted in start_job",
                );
                if *entry > 0 {
                    *entry -= 1;
                }
                *entry == 0
            };
            if domain_finished {
                state.editor.borrow_mut().mark_saved(task.domain);
            }
            w.set_save_done(state.save_job.borrow().as_ref().map(|j| j.cursor as i32).unwrap_or(0));

            // Done?
            let finished = state
                .save_job
                .borrow()
                .as_ref()
                .map(|j| j.cursor >= j.queue.len())
                .unwrap_or(true);
            if finished {
                finalize(&state, &w, FinalizeKind::Success);
            } else {
                schedule_tick(state, window_weak);
            }
        }
        Err(err) => {
            finalize(&state, &w, FinalizeKind::Error { task_label: task.label, err });
        }
    }
}

enum FinalizeKind {
    Success,
    Cancelled,
    Error { task_label: String, err: String },
}

fn finalize(state: &AppState, w: &MainWindow, kind: FinalizeKind) {
    // Read counters before dropping the job.
    let (done, total) = state
        .save_job
        .borrow()
        .as_ref()
        .map(|j| (j.cursor, j.queue.len()))
        .unwrap_or((0, 0));

    *state.save_job.borrow_mut() = None;
    w.set_save_running(false);
    w.set_save_current("".into());

    // Refresh dirty indicators in the Slint UI to reflect any partial mark_saved
    // calls that happened during the run.
    crate::wiring::helpers::publish_dirty(w, &state.editor.borrow());

    match kind {
        FinalizeKind::Success => {
            w.set_status_text(format!("Saved ({total} files)").into());
        }
        FinalizeKind::Cancelled => {
            w.set_status_text(format!("Cancelled — saved {done} of {total} files").into());
        }
        FinalizeKind::Error { task_label, err } => {
            w.set_status_text(format!("Save failed at {task_label}").into());
            crate::wiring::helpers::toast(
                &w.as_weak(),
                format!("Save failed at {task_label}: {err}"),
            );
        }
    }
}

fn execute_task(state: &AppState, task: &SaveTask) -> Result<(), String> {
    use hm64_sprite_editor::export::{
        animation_data_json::save_animation_data,
        animation_export::{delete_animation_file, write_animation},
        manifest_export::write_manifest,
        palette_export::{write_palette_json, write_palette_pal},
        texture_export::{write_texture_ci, write_texture_png},
    };

    let ed = state.editor.borrow();
    let Some(s) = ed.loaded_sprite.as_ref() else {
        return Err("no sprite loaded".into());
    };
    let dir = s.dir.clone();

    match &task.kind {
        SaveTaskKind::PaletteFile { index } => {
            let p = s.palettes.get(index).ok_or("palette gone")?;
            write_palette_json(&dir, p).map_err(|e| e.to_string())?;
            write_palette_pal(&dir, p).map_err(|e| e.to_string())?;
            Ok(())
        }
        SaveTaskKind::TextureCi { index } => {
            let t = s.textures.get(index).ok_or("texture gone")?;
            write_texture_ci(&dir, t).map_err(|e| e.to_string())
        }
        SaveTaskKind::TexturePng { index } => {
            let t = s.textures.get(index).ok_or("texture gone")?;
            let pal = s.palettes.get(&t.palette_index).ok_or("texture's palette missing")?;
            write_texture_png(&dir, t, pal).map_err(|e| e.to_string())
        }
        SaveTaskKind::TextureCleanup => {
            // Best-effort: mirror the existing post-save loop. Failures are
            // non-fatal — log and continue.
            let live: std::collections::HashSet<u32> = s.textures.keys().copied().collect();
            let textures_dir = dir.join("textures");
            if let Ok(entries) = std::fs::read_dir(&textures_dir) {
                for entry in entries.flatten() {
                    let p = entry.path();
                    let Some(stem) = p.file_stem().and_then(|s| s.to_str()) else { continue };
                    let Ok(idx) = stem.parse::<u32>() else { continue };
                    if live.contains(&idx) {
                        continue;
                    }
                    let ext = p.extension().and_then(|e| e.to_str()).unwrap_or("");
                    if ext != "ci" && ext != "png" {
                        continue;
                    }
                    let _ = std::fs::remove_file(&p);
                }
            }
            Ok(())
        }
        SaveTaskKind::AnimationFile { index } => {
            let a = s.animations.get(index).ok_or("animation gone")?;
            write_animation(&dir, a).map_err(|e| e.to_string())
        }
        SaveTaskKind::AnimationCleanup => {
            let live: std::collections::HashSet<u32> = s.animations.keys().copied().collect();
            let anim_dir = dir.join("animations");
            if anim_dir.is_dir() {
                if let Ok(entries) = std::fs::read_dir(&anim_dir) {
                    for entry in entries.flatten() {
                        let name = entry.file_name();
                        let Some(name_str) = name.to_str() else { continue };
                        if !name_str.ends_with(".json") {
                            continue;
                        }
                        let stem = &name_str[..name_str.len() - 5];
                        let Ok(idx) = stem.parse::<u32>() else { continue };
                        if !live.contains(&idx) {
                            let _ = delete_animation_file(&dir, idx);
                        }
                    }
                }
            }
            Ok(())
        }
        SaveTaskKind::ScriptFile => {
            let data = ed.loaded_animation_data.as_ref().ok_or("no script loaded")?;
            save_animation_data(&dir, data).map_err(|e| e.to_string())
        }
        SaveTaskKind::ManifestFile => write_manifest(&dir, &s.manifest).map_err(|e| e.to_string()),
    }
}

// ---------- per-domain task emitters (shared by all planners) ----------

fn append_palette_tasks(
    tasks: &mut Vec<SaveTask>,
    sprite: &hm64_sprite_editor::model::SpriteAsset,
) {
    for p in sprite.palettes.values() {
        tasks.push(SaveTask {
            label: format!("palettes/{:02}.json", p.index),
            domain: DirtyMask::PALETTE,
            kind: SaveTaskKind::PaletteFile { index: p.index },
        });
    }
}

fn append_texture_tasks(
    tasks: &mut Vec<SaveTask>,
    sprite: &hm64_sprite_editor::model::SpriteAsset,
) {
    for t in sprite.textures.values() {
        tasks.push(SaveTask {
            label: format!("textures/{:03}.ci", t.index),
            domain: DirtyMask::TEXTURE,
            kind: SaveTaskKind::TextureCi { index: t.index },
        });
        tasks.push(SaveTask {
            label: format!("textures/{:03}.png", t.index),
            domain: DirtyMask::TEXTURE,
            kind: SaveTaskKind::TexturePng { index: t.index },
        });
    }
    tasks.push(SaveTask {
        label: "cleanup stale texture files".into(),
        domain: DirtyMask::TEXTURE,
        kind: SaveTaskKind::TextureCleanup,
    });
}

fn append_animation_tasks(
    tasks: &mut Vec<SaveTask>,
    sprite: &hm64_sprite_editor::model::SpriteAsset,
) {
    for a in sprite.animations.values() {
        tasks.push(SaveTask {
            label: format!("animations/{:02}.json", a.index),
            domain: DirtyMask::ANIMATION,
            kind: SaveTaskKind::AnimationFile { index: a.index },
        });
    }
    tasks.push(SaveTask {
        label: "cleanup orphan animation files".into(),
        domain: DirtyMask::ANIMATION,
        kind: SaveTaskKind::AnimationCleanup,
    });
}

fn append_script_task(tasks: &mut Vec<SaveTask>) {
    tasks.push(SaveTask {
        label: "animation script".into(),
        domain: DirtyMask::ANIMATION_DATA,
        kind: SaveTaskKind::ScriptFile,
    });
}

fn append_manifest(tasks: &mut Vec<SaveTask>, domain: DirtyMask) {
    tasks.push(SaveTask {
        label: "manifest.json".into(),
        domain,
        kind: SaveTaskKind::ManifestFile,
    });
}

// ---------- planners ----------

/// Build the ordered list of file writes needed to fully save the loaded
/// sprite. Skips any domain whose dirty bit is clear. Returns an empty `Vec`
/// when nothing is dirty or no sprite is loaded.
pub fn plan_save_all(editor: &hm64_sprite_editor::state::editor::Editor) -> Vec<SaveTask> {
    let Some(sprite) = editor.loaded_sprite.as_ref() else {
        return Vec::new();
    };
    let mut tasks = Vec::new();

    if editor.dirty.palette() {
        append_palette_tasks(&mut tasks, sprite);
    }
    if editor.dirty.texture() {
        append_texture_tasks(&mut tasks, sprite);
    }
    if editor.dirty.animation() {
        append_animation_tasks(&mut tasks, sprite);
    }
    if editor.dirty.animation_data() {
        append_script_task(&mut tasks);
    }

    // Manifest: emit once at the end if either PALETTE or TEXTURE is dirty.
    // Domain assignment matches today's two-handler split: TEXTURE wins when
    // both are dirty (it's the larger scope), else PALETTE.
    let manifest_domain = if editor.dirty.texture() {
        Some(DirtyMask::TEXTURE)
    } else if editor.dirty.palette() {
        Some(DirtyMask::PALETTE)
    } else {
        None
    };
    if let Some(domain) = manifest_domain {
        append_manifest(&mut tasks, domain);
    }

    tasks
}

/// Single-domain plan: palettes only (plus a manifest write, since today's
/// `on_save_palettes` writes the manifest as part of palette save).
pub fn plan_palette_only(editor: &hm64_sprite_editor::state::editor::Editor) -> Vec<SaveTask> {
    let Some(sprite) = editor.loaded_sprite.as_ref() else {
        return Vec::new();
    };
    let mut tasks = Vec::new();
    if editor.dirty.palette() {
        append_palette_tasks(&mut tasks, sprite);
        append_manifest(&mut tasks, DirtyMask::PALETTE);
    }
    tasks
}

/// Single-domain plan: textures only (plus cleanup + manifest, mirroring
/// today's `on_save_textures`).
pub fn plan_texture_only(editor: &hm64_sprite_editor::state::editor::Editor) -> Vec<SaveTask> {
    let Some(sprite) = editor.loaded_sprite.as_ref() else {
        return Vec::new();
    };
    let mut tasks = Vec::new();
    if editor.dirty.texture() {
        append_texture_tasks(&mut tasks, sprite);
        append_manifest(&mut tasks, DirtyMask::TEXTURE);
    }
    tasks
}

/// Single-domain plan: animations only (plus cleanup, no manifest — animation
/// save doesn't touch the manifest today).
pub fn plan_animation_only(editor: &hm64_sprite_editor::state::editor::Editor) -> Vec<SaveTask> {
    let Some(sprite) = editor.loaded_sprite.as_ref() else {
        return Vec::new();
    };
    let mut tasks = Vec::new();
    if editor.dirty.animation() {
        append_animation_tasks(&mut tasks, sprite);
    }
    tasks
}

/// Single-domain plan: animation script only.
pub fn plan_script_only(editor: &hm64_sprite_editor::state::editor::Editor) -> Vec<SaveTask> {
    let mut tasks = Vec::new();
    if editor.dirty.animation_data() {
        append_script_task(&mut tasks);
    }
    tasks
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    use hm64_sprite_editor::cmd::{CmdError, Command, MergeResult};
    use hm64_sprite_editor::model::{
        ActiveAsset, Animation, AnimationFrame, FrameSprite, Manifest, Palette, Rgba,
        SpriteAsset, SpriteKind, SpriteType, Texture, TextureFormat,
    };
    use hm64_sprite_editor::state::editor::Editor;

    /// Stub Command that does nothing on apply/revert but reports `affects()`
    /// as the requested mask. Used solely to flip a dirty bit in tests.
    #[derive(Debug)]
    struct DirtyStub(DirtyMask);
    impl Command for DirtyStub {
        fn apply(&mut self, _: &mut Editor) -> Result<(), CmdError> { Ok(()) }
        fn revert(&mut self, _: &mut Editor) -> Result<(), CmdError> { Ok(()) }
        fn describe(&self) -> String { "dirty stub".into() }
        fn affects(&self) -> DirtyMask { self.0 }
        fn try_merge(&mut self, _: &dyn Command) -> MergeResult { MergeResult::Separate }
    }

    fn dirty(e: &mut Editor, mask: DirtyMask) {
        e.run(Box::new(DirtyStub(mask))).unwrap();
        // For every bit in `mask`, the matching dirty accessor must report true.
        if mask.contains(DirtyMask::PALETTE)        { assert!(e.dirty.palette()); }
        if mask.contains(DirtyMask::TEXTURE)        { assert!(e.dirty.texture()); }
        if mask.contains(DirtyMask::ANIMATION)      { assert!(e.dirty.animation()); }
        if mask.contains(DirtyMask::ANIMATION_DATA) { assert!(e.dirty.animation_data()); }
    }

    /// Mirror of `state::editor::tests::fake_editor` — fully-spelled Manifest
    /// (no Default impl available) and an empty-textures / empty-animations
    /// SpriteAsset. Tests extend this in their own helpers.
    fn fake_editor_empty() -> Editor {
        let manifest = Manifest {
            label: "t".into(),
            subdir: "entitySprites".into(),
            kind: SpriteKind::Unknown,
            sprite_type: SpriteType::Type1,
            sprite_count: 0,
            spritesheet_index_count: 0,
            palette_count: 1,
            palette_index_count: 0,
            animation_count: 0,
            animation_index: vec![],
            palette_index: vec![],
            palettes: Default::default(),
            sprite_to_palette: vec![],
            palette_index_end_bytes: vec![],
            spritesheet_index_end_bytes: vec![],
            s2p_padding: vec![],
            texture_end_padding: vec![],
            asset_offsets: vec![],
            textures: vec![],
        };
        let mut palettes = BTreeMap::new();
        palettes.insert(
            0,
            Palette {
                index: 0,
                format: TextureFormat::Ci4,
                colors: vec![Rgba::new(0, 0, 0, 255); 16],
            },
        );
        let mut e = Editor::new();
        e.loaded_sprite = Some(SpriteAsset {
            dir: PathBuf::from("/tmp/fake_sprite"),
            manifest,
            textures: BTreeMap::new(),
            palettes,
            animations: BTreeMap::new(),
        });
        e.active = ActiveAsset::Sprite;
        e
    }

    #[test]
    fn plan_returns_empty_when_no_domains_dirty() {
        let e = fake_editor_empty();
        let tasks = plan_save_all(&e);
        assert!(tasks.is_empty(), "clean sprite must produce zero tasks, got {:?}", tasks);
    }

    #[test]
    fn plan_emits_one_task_per_palette_when_palette_dirty() {
        let mut e = fake_editor_empty();
        dirty(&mut e, DirtyMask::PALETTE);
        let tasks = plan_save_all(&e);
        let palette_tasks: Vec<_> = tasks
            .iter()
            .filter(|t| matches!(t.kind, SaveTaskKind::PaletteFile { .. }))
            .collect();
        assert_eq!(palette_tasks.len(), 1, "one palette in fixture");
        assert_eq!(palette_tasks[0].domain, DirtyMask::PALETTE);
        assert!(matches!(
            palette_tasks[0].kind,
            SaveTaskKind::PaletteFile { index: 0 }
        ));
    }

    /// Extends `fake_editor_empty` with one Texture at index 0 bound to palette 0.
    fn fake_editor_with_texture() -> Editor {
        let mut e = fake_editor_empty();
        if let Some(s) = e.loaded_sprite.as_mut() {
            s.textures.insert(
                0,
                Texture {
                    index: 0,
                    width: 8,
                    height: 8,
                    format: TextureFormat::Ci4,
                    palette_index: 0,
                    pixels: vec![0u8; 8 * 8 / 2],
                },
            );
        }
        e
    }

    #[test]
    fn plan_emits_ci_then_png_per_texture_then_cleanup() {
        let mut e = fake_editor_with_texture();
        dirty(&mut e, DirtyMask::TEXTURE);
        let tasks = plan_save_all(&e);

        let kinds: Vec<&SaveTaskKind> = tasks.iter().map(|t| &t.kind).collect();

        // Find the texture run (CI, PNG, Cleanup, in that order, contiguous).
        let ci_pos = kinds
            .iter()
            .position(|k| matches!(k, SaveTaskKind::TextureCi { index: 0 }))
            .expect("CI task present");
        assert!(matches!(kinds[ci_pos + 1], SaveTaskKind::TexturePng { index: 0 }));
        assert!(matches!(kinds[ci_pos + 2], SaveTaskKind::TextureCleanup));

        for t in &tasks {
            if matches!(
                t.kind,
                SaveTaskKind::TextureCi { .. }
                    | SaveTaskKind::TexturePng { .. }
                    | SaveTaskKind::TextureCleanup
            ) {
                assert_eq!(t.domain, DirtyMask::TEXTURE);
            }
        }
    }

    /// Extends `fake_editor_with_texture` with one Animation at index 0.
    fn fake_editor_with_animation() -> Editor {
        let mut e = fake_editor_with_texture();
        if let Some(s) = e.loaded_sprite.as_mut() {
            s.animations.insert(
                0,
                Animation {
                    frame_count: 1,
                    unused: 0,
                    frames: vec![AnimationFrame {
                        duration: 4,
                        has_audio: 0,
                        sprites: vec![FrameSprite {
                            spritesheet_index: 0,
                            anchor_x: 0,
                            anchor_y: 0,
                        }],
                    }],
                    index: 0,
                },
            );
        }
        e
    }

    #[test]
    fn plan_emits_animation_files_then_cleanup() {
        let mut e = fake_editor_with_animation();
        dirty(&mut e, DirtyMask::ANIMATION);
        let tasks = plan_save_all(&e);

        let anim_pos = tasks
            .iter()
            .position(|t| matches!(t.kind, SaveTaskKind::AnimationFile { index: 0 }))
            .expect("animation task present");
        assert!(matches!(
            tasks[anim_pos + 1].kind,
            SaveTaskKind::AnimationCleanup
        ));
        assert_eq!(tasks[anim_pos].domain, DirtyMask::ANIMATION);
        assert_eq!(tasks[anim_pos + 1].domain, DirtyMask::ANIMATION);
    }

    #[test]
    fn plan_orders_palettes_before_textures() {
        let mut e = fake_editor_with_texture();
        dirty(&mut e, DirtyMask::PALETTE);
        dirty(&mut e, DirtyMask::TEXTURE);
        let tasks = plan_save_all(&e);

        let first_palette = tasks
            .iter()
            .position(|t| matches!(t.kind, SaveTaskKind::PaletteFile { .. }))
            .expect("palette present");
        let first_texture = tasks
            .iter()
            .position(|t| matches!(t.kind, SaveTaskKind::TextureCi { .. }))
            .expect("texture present");
        assert!(
            first_palette < first_texture,
            "palette tasks must come before texture tasks"
        );
    }

    #[test]
    fn plan_emits_script_task_when_animation_data_dirty() {
        let mut e = fake_editor_with_animation();
        dirty(&mut e, DirtyMask::ANIMATION_DATA);
        let tasks = plan_save_all(&e);
        assert!(
            tasks.iter().any(|t| matches!(t.kind, SaveTaskKind::ScriptFile)),
            "script task must be present when ANIMATION_DATA dirty"
        );
    }

    #[test]
    fn manifest_belongs_to_texture_when_texture_dirty() {
        let mut e = fake_editor_with_texture();
        dirty(&mut e, DirtyMask::PALETTE);
        dirty(&mut e, DirtyMask::TEXTURE);
        let tasks = plan_save_all(&e);
        let manifest = tasks
            .iter()
            .find(|t| matches!(t.kind, SaveTaskKind::ManifestFile))
            .expect("manifest task present");
        assert_eq!(manifest.domain, DirtyMask::TEXTURE);
    }

    #[test]
    fn manifest_belongs_to_texture_when_only_texture_dirty() {
        let mut e = fake_editor_with_texture();
        dirty(&mut e, DirtyMask::TEXTURE);
        let tasks = plan_save_all(&e);
        let manifest = tasks
            .iter()
            .find(|t| matches!(t.kind, SaveTaskKind::ManifestFile))
            .expect("manifest task present");
        assert_eq!(manifest.domain, DirtyMask::TEXTURE);
    }

    #[test]
    fn manifest_belongs_to_palette_when_only_palette_dirty() {
        let mut e = fake_editor_with_texture();
        dirty(&mut e, DirtyMask::PALETTE);
        let tasks = plan_save_all(&e);
        let manifest = tasks
            .iter()
            .find(|t| matches!(t.kind, SaveTaskKind::ManifestFile))
            .expect("manifest task present");
        assert_eq!(manifest.domain, DirtyMask::PALETTE);
    }

    #[test]
    fn no_manifest_task_when_neither_palette_nor_texture_dirty() {
        let mut e = fake_editor_with_animation();
        dirty(&mut e, DirtyMask::ANIMATION);
        let tasks = plan_save_all(&e);
        assert!(
            !tasks.iter().any(|t| matches!(t.kind, SaveTaskKind::ManifestFile)),
            "no manifest write when only ANIMATION dirty"
        );
    }
}
