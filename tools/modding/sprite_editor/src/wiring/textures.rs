//! Textures tab callbacks: list selection, usage navigation, palette
//! reassignment, save, import/replace from indexed PNG, and delete with
//! confirmation modal.

use hm64_sprite_editor::state::editor::Editor;
use hm64_sprite_editor::ui::palettes_vm;
use hm64_sprite_editor::ui::slint_generated::MainWindow;
use hm64_sprite_editor::ui::textures_vm;
use slint::ComponentHandle;

use crate::wiring::helpers::{publish_dirty, toast};
use crate::wiring::palettes::{publish_palette_cells, publish_palette_names, publish_palette_rows};
use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_texture_clicked(move |idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let Some(tex) = s.textures.get(&(idx as u32)) else { return };
            w.set_selected_texture(idx);
            w.set_selected_texture_row(texture_row_for_index(Some(s), idx));
            refresh_texture_inspector(&w, s, tex);

            let usage_index =
                textures_vm::build_texture_usage_index(s, ed.loaded_animation_data.as_ref());
            let empty = textures_vm::TextureUsage::default();
            let usage = usage_index.get(&(idx as u32)).unwrap_or(&empty);
            w.set_usage_animations(textures_vm::animation_usage_rows(usage));
            w.set_usage_script_entries(textures_vm::script_usage_rows(usage));
            w.set_usage_show_scripts(matches!(
                s.manifest.kind,
                hm64_sprite_editor::model::SpriteKind::Entity
            ));
        });
    }

    {
        let editor = state.editor.clone();
        let weak = window.as_weak();
        window.on_usage_animation_clicked(move |anim_idx| {
            if let Some(w) = weak.upgrade() {
                w.set_current_tab(2);
                w.set_selected_anim(anim_idx);
                let ed = editor.borrow();
                w.set_selected_anim_row(anim_row_for_index(ed.current_sprite(), anim_idx));
            }
        });
    }

    {
        let editor = state.editor.clone();
        let weak = window.as_weak();
        window.on_usage_script_entry_clicked(move |script_name, entry_idx| {
            if let Some(w) = weak.upgrade() {
                w.set_current_tab(3);
                // Map script name to list index using the loaded animation data.
                let ed = editor.borrow();
                if let Some(hm64_sprite_editor::model::AnimationData::Entity { scripts }) =
                    ed.loaded_animation_data.as_ref()
                {
                    if let Some(script_list_idx) =
                        scripts.iter().position(|s| s.name == script_name.as_str())
                    {
                        w.set_script_selected_script_index(script_list_idx as i32);
                    }
                }
                w.set_script_selected_entry(entry_idx);
            }
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let window_weak = window.as_weak();
        window.on_texture_palette_changed(move |new_pal_row| {
            use hm64_sprite_editor::cmd::texture_cmds::AssignTextureToPalette;
            let Some(w) = window_weak.upgrade() else { return };
            let tex_idx = w.get_selected_texture();
            if tex_idx < 0 {
                return;
            }
            let new_pal_id = {
                let ed = editor.borrow();
                let Some(s) = ed.current_sprite() else { return };
                let Some(id) = palettes_vm::palette_id_at_row(s, new_pal_row) else {
                    w.set_status_text(format!("Palette row {} out of range", new_pal_row).into());
                    return;
                };
                id
            };
            // Guard: skip if palette is already assigned (avoids spurious dirty on init).
            {
                let ed = editor.borrow();
                if ed
                    .current_sprite()
                    .and_then(|s| s.textures.get(&(tex_idx as u32)))
                    .map(|t| t.palette_index)
                    == Some(new_pal_id)
                {
                    return;
                }
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(cmd) = AssignTextureToPalette::new(tex_idx as u32, new_pal_id, &ed) else {
                    return;
                };
                ed.run(Box::new(cmd))
            };
            if let Err(e) = result {
                w.set_status_text(format!("Palette assign failed: {e}").into());
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            let ed = editor.borrow();
            if let Some(s) = ed.current_sprite() {
                if let Some(tex) = s.textures.get(&(tex_idx as u32)) {
                    refresh_texture_inspector(&w, s, tex);
                }
                publish_texture_thumbs_inner(&w, s);
                // All textures re-rendered synchronously — cancel deferred queue.
                pending_tex.borrow_mut().clear();
            }
            publish_dirty(&w, &ed);
        });
    }

    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_save_textures(move || {
            let plan = crate::wiring::save_all::plan_texture_only(&app.editor.borrow());
            crate::wiring::save_all::start_job(&app, &window_weak, plan, "Textures clean");
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_save_selected_texture(move || {
            use hm64_sprite_editor::export::{
                manifest_export::write_manifest,
                texture_export::{write_texture_ci, write_texture_png},
            };
            let Some(w) = window_weak.upgrade() else { return };
            let tex_idx = w.get_selected_texture();
            if tex_idx < 0 {
                w.set_status_text("No texture selected".into());
                return;
            }
            let ed = editor.borrow();
            let Some(s) = ed.loaded_sprite.as_ref() else { return };
            let Some(t) = s.textures.get(&(tex_idx as u32)) else {
                w.set_status_text(format!("Texture #{tex_idx} not loaded").into());
                return;
            };
            let dir = s.dir.clone();
            // .ci first so that if anything fails the indexed PNG never gets
            // out of sync with the underlying pixel data on disk.
            if let Err(e) = write_texture_ci(&dir, t) {
                w.set_status_text("Save failed".into());
                toast(&window_weak, format!("Save texture #{tex_idx} failed: {e}"));
                return;
            }
            if let Some(pal) = s.palettes.get(&t.palette_index) {
                if let Err(e) = write_texture_png(&dir, t, pal) {
                    w.set_status_text("PNG save failed".into());
                    toast(&window_weak, format!("PNG save failed: {e}"));
                    return;
                }
            }
            // Manifest may have been mutated in-memory by texture-replace flows
            // (palette assignment, dimensions). Persist it so the assembler
            // sees a manifest consistent with the .ci we just wrote. Other
            // textures' .ci on disk may still be stale relative to the manifest;
            // the * dirty indicator stays on Save All until the user flushes
            // them, since per-domain dirty tracking can't tell us which texture
            // indices still have unsaved pixel changes.
            if let Err(e) = write_manifest(&dir, &s.manifest) {
                w.set_status_text("Manifest save failed".into());
                toast(&window_weak, format!("Manifest save failed: {e}"));
                return;
            }
            publish_dirty(&w, &ed);
            w.set_status_text(format!("Saved texture #{tex_idx}").into());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_import_texture(move || {
            use hm64_sprite_editor::cmd::palette_cmds::CreatePalette;
            use hm64_sprite_editor::cmd::texture_cmds::CreateTexture;
            use hm64_sprite_editor::io::png_import::import_indexed_png;
            use hm64_sprite_editor::model::find_matching_palette;
            let Some(w) = window_weak.upgrade() else { return };
            let path = rfd::FileDialog::new()
                .add_filter("Indexed PNG", &["png"])
                .set_title("Import indexed PNG as new texture")
                .pick_file();
            let Some(path) = path else { return };
            let imp = match import_indexed_png(&path) {
                Ok(i) => i,
                Err(e) => {
                    w.set_status_text(format!("Import failed: {e}").into());
                    return;
                }
            };

            // Decide IDs and whether a new palette is needed.
            let (new_tex_id, new_pal_id, need_create_palette) = {
                let ed = editor.borrow();
                let Some(s) = ed.current_sprite() else {
                    w.set_status_text("No sprite loaded".into());
                    return;
                };
                let new_tex_id = s.textures.keys().max().map(|m| m + 1).unwrap_or(0);
                match find_matching_palette(&imp.palette, imp.format, &s.palettes) {
                    Some(existing) => (new_tex_id, existing, false),
                    None => {
                        let new_pal_id = s.palettes.keys().max().map(|m| m + 1).unwrap_or(0);
                        (new_tex_id, new_pal_id, true)
                    }
                }
            };

            // Stage commands. Palette creation first (if needed), then texture.
            if need_create_palette {
                use hm64_sprite_editor::export::palette_export::{
                    write_palette_json, write_palette_pal,
                };
                let cmd = CreatePalette::new(new_pal_id, imp.format, imp.palette.clone());
                if let Err(e) = editor.borrow_mut().run(Box::new(cmd)) {
                    w.set_status_text(format!("Create palette failed: {e}").into());
                    return;
                }
                // Persist the new palette file immediately. The texture save
                // path writes a manifest that references this palette by index,
                // so the file must exist on disk before any subsequent save —
                // otherwise a Save-Textures-only click leaves the asset in an
                // inconsistent state where manifest.json references palettes/NN.pal
                // that does not exist, producing wrong-palette artifacts at runtime.
                let to_write = {
                    let ed = editor.borrow();
                    ed.current_sprite().and_then(|s| {
                        s.palettes.get(&new_pal_id).map(|p| (s.dir.clone(), p.clone()))
                    })
                };
                if let Some((dir, pal)) = to_write {
                    if let Err(e) = write_palette_json(&dir, &pal)
                        .and_then(|_| write_palette_pal(&dir, &pal))
                    {
                        // Roll back CreatePalette so in-memory state matches disk.
                        let _ = editor.borrow_mut().undo();
                        w.set_status_text(format!("Persist palette failed: {e}").into());
                        return;
                    }
                }
            }
            let cmd = CreateTexture::new(
                new_tex_id,
                imp.width,
                imp.height,
                imp.format,
                new_pal_id,
                imp.pixels,
            );
            if let Err(e) = editor.borrow_mut().run(Box::new(cmd)) {
                w.set_status_text(format!("Create texture #{new_tex_id} failed: {e}").into());
                return;
            }

            // Reached only after both commands succeeded; the early-returns above bypass this.
            anims_vm.borrow_mut().invalidate_cache();

            // Republish all affected views.
            let ed = editor.borrow();
            publish_texture_thumbs(&w, &ed);
            publish_palette_rows(&w, &ed);
            publish_palette_names(&w, &ed);
            let cur_pal = w.get_selected_palette();
            publish_palette_cells(&w, &ed, cur_pal);

            // Select the new texture and populate the inspector.
            if let Some(s) = ed.current_sprite() {
                if let Some(tex) = s.textures.get(&new_tex_id) {
                    w.set_selected_texture(new_tex_id as i32);
                    w.set_selected_texture_row(texture_row_for_index(Some(s), new_tex_id as i32));
                    refresh_texture_inspector(&w, s, tex);
                }
            }
            publish_dirty(&w, &ed);
            if need_create_palette {
                w.set_status_text(
                    format!("Imported texture #{new_tex_id} (created palette #{new_pal_id})")
                        .into(),
                );
            } else {
                w.set_status_text(
                    format!("Imported texture #{new_tex_id} (reused palette #{new_pal_id})")
                        .into(),
                );
            }
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_replace_texture(move || {
            use hm64_sprite_editor::cmd::palette_cmds::CreatePalette;
            use hm64_sprite_editor::cmd::texture_cmds::ReplaceTextureFromPng;
            use hm64_sprite_editor::io::png_import::import_indexed_png;
            use hm64_sprite_editor::model::find_matching_palette;
            let Some(w) = window_weak.upgrade() else { return };
            let tex_idx = w.get_selected_texture();
            if tex_idx < 0 {
                return;
            }
            let path = rfd::FileDialog::new()
                .add_filter("Indexed PNG", &["png"])
                .set_title(format!("Replace texture #{tex_idx} from PNG"))
                .pick_file();
            let Some(path) = path else { return };
            let imp = match import_indexed_png(&path) {
                Ok(i) => i,
                Err(e) => {
                    w.set_status_text(format!("Import failed: {e}").into());
                    return;
                }
            };

            // Validate format and decide palette target.
            let (new_pal_id, need_create_palette) = {
                let ed = editor.borrow();
                let Some(s) = ed.current_sprite() else {
                    w.set_status_text("No sprite loaded".into());
                    return;
                };
                let Some(t) = s.textures.get(&(tex_idx as u32)) else {
                    w.set_status_text(format!("Texture #{tex_idx} not loaded").into());
                    return;
                };
                if imp.format != t.format {
                    w.set_status_text(
                        format!(
                            "PNG format {:?} doesn't match texture #{} ({:?}). Quantize externally and re-import.",
                            imp.format, tex_idx, t.format
                        )
                        .into(),
                    );
                    return;
                }
                match find_matching_palette(&imp.palette, imp.format, &s.palettes) {
                    Some(id) => (id, false),
                    None => {
                        let id = s.palettes.keys().max().map(|m| m + 1).unwrap_or(0);
                        (id, true)
                    }
                }
            };
            let tex_id = tex_idx as u32;

            if need_create_palette {
                use hm64_sprite_editor::export::palette_export::{
                    write_palette_json, write_palette_pal,
                };
                let cmd = CreatePalette::new(new_pal_id, imp.format, imp.palette.clone());
                if let Err(e) = editor.borrow_mut().run(Box::new(cmd)) {
                    w.set_status_text(format!("Create palette failed: {e}").into());
                    return;
                }
                // Persist the new palette file immediately. See on_import_texture
                // above for rationale: the texture save path will later write a
                // manifest that references this palette by index, so the file
                // must exist on disk before that happens.
                let to_write = {
                    let ed = editor.borrow();
                    ed.current_sprite().and_then(|s| {
                        s.palettes.get(&new_pal_id).map(|p| (s.dir.clone(), p.clone()))
                    })
                };
                if let Some((dir, pal)) = to_write {
                    if let Err(e) = write_palette_json(&dir, &pal)
                        .and_then(|_| write_palette_pal(&dir, &pal))
                    {
                        let _ = editor.borrow_mut().undo();
                        w.set_status_text(format!("Persist palette failed: {e}").into());
                        return;
                    }
                }
            }
            // ReplaceTextureFromPng::new(...) reads old_* from current editor state.
            let cmd = {
                let ed = editor.borrow();
                let Some(cmd) = ReplaceTextureFromPng::new(
                    tex_id,
                    imp.pixels,
                    (imp.width, imp.height),
                    new_pal_id,
                    &ed,
                ) else {
                    w.set_status_text(format!("Texture #{tex_idx} not loaded").into());
                    return;
                };
                cmd
            };
            if let Err(e) = editor.borrow_mut().run(Box::new(cmd)) {
                w.set_status_text(format!("Replace failed: {e}").into());
                return;
            }

            // Reached only after both commands succeeded; the early-returns above bypass this.
            anims_vm.borrow_mut().invalidate_cache();
            let ed = editor.borrow();
            publish_texture_thumbs(&w, &ed);
            publish_palette_rows(&w, &ed);
            publish_palette_names(&w, &ed);
            let cur_pal = w.get_selected_palette();
            publish_palette_cells(&w, &ed, cur_pal);
            if let Some(s) = ed.current_sprite() {
                if let Some(tex) = s.textures.get(&tex_id) {
                    refresh_texture_inspector(&w, s, tex);
                }
            }
            publish_dirty(&w, &ed);
            if need_create_palette {
                w.set_status_text(
                    format!("Replaced texture #{tex_idx} (created palette #{new_pal_id})").into(),
                );
            } else {
                w.set_status_text(
                    format!("Replaced texture #{tex_idx} (reused palette #{new_pal_id})").into(),
                );
            }
        });
    }

    {
        let editor = state.editor.clone();
        let pending_delete_tex = state.pending_delete_tex.clone();
        let window_weak = window.as_weak();
        window.on_delete_texture(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let tex_idx = w.get_selected_texture();
            if tex_idx < 0 {
                return;
            }
            let body = {
                let ed = editor.borrow();
                let Some(s) = ed.current_sprite() else { return };
                let usage_index =
                    textures_vm::build_texture_usage_index(s, ed.loaded_animation_data.as_ref());
                let empty = textures_vm::TextureUsage::default();
                let usage = usage_index.get(&(tex_idx as u32)).unwrap_or(&empty);
                let frame_count: usize =
                    usage.animations.iter().map(|a| a.frame_count as usize).sum();
                let anim_count = usage.animations.len();
                if frame_count == 0 {
                    format!("Delete texture #{tex_idx}? You can undo this with Ctrl+Z.")
                } else {
                    format!(
                        "Delete texture #{tex_idx}? It is referenced by {frame_count} animation frame(s) across {anim_count} animation(s). The data slot will remain (gap), and references will continue to resolve to a zero-byte placeholder. You can undo this with Ctrl+Z."
                    )
                }
            };
            *pending_delete_tex.borrow_mut() = Some(tex_idx as u32);
            w.set_tex_confirm_title("Delete texture".into());
            w.set_tex_confirm_body(body.into());
            w.set_tex_confirm_visible(true);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_delete_tex = state.pending_delete_tex.clone();
        let window_weak = window.as_weak();
        window.on_tex_confirm_ok(move || {
            use hm64_sprite_editor::cmd::texture_cmds::DeleteTextureData;
            let Some(w) = window_weak.upgrade() else { return };
            let Some(tex_idx) = pending_delete_tex.borrow_mut().take() else {
                w.set_tex_confirm_visible(false);
                return;
            };
            let result = editor.borrow_mut().run(Box::new(DeleteTextureData::new(tex_idx)));
            w.set_tex_confirm_visible(false);
            if let Err(e) = result {
                w.set_status_text(format!("Delete failed: {e}").into());
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            let ed = editor.borrow();
            publish_texture_thumbs(&w, &ed);
            clear_texture_inspector(&w);
            let cur_pal = w.get_selected_palette();
            publish_palette_cells(&w, &ed, cur_pal);
            publish_dirty(&w, &ed);
            w.set_status_text(format!("Deleted texture #{tex_idx}").into());
        });
    }

    {
        let pending_delete_tex = state.pending_delete_tex.clone();
        let window_weak = window.as_weak();
        window.on_tex_confirm_cancel(move || {
            let Some(w) = window_weak.upgrade() else { return };
            *pending_delete_tex.borrow_mut() = None;
            w.set_tex_confirm_visible(false);
        });
    }
}

// ---------------------------------------------------------------------------
// Helpers (relocated from main.rs)
// ---------------------------------------------------------------------------

pub(crate) fn clear_texture_inspector(w: &MainWindow) {
    w.set_selected_texture(-1);
    w.set_selected_texture_row(-1);
    w.set_texture_preview(slint::Image::default());
    w.set_texture_info("Click a texture".into());
    w.set_texture_palette_index(-1);
}

pub(crate) fn texture_row_for_index(
    s: Option<&hm64_sprite_editor::model::SpriteAsset>,
    sparse_id: i32,
) -> i32 {
    if sparse_id < 0 {
        return -1;
    }
    s.and_then(|s| s.textures.keys().position(|&k| k == sparse_id as u32))
        .map(|p| p as i32)
        .unwrap_or(-1)
}

pub(crate) fn anim_row_for_index(
    s: Option<&hm64_sprite_editor::model::SpriteAsset>,
    sparse_id: i32,
) -> i32 {
    if sparse_id < 0 {
        return -1;
    }
    s.and_then(|s| s.animations.keys().position(|&k| k == sparse_id as u32))
        .map(|p| p as i32)
        .unwrap_or(-1)
}

pub(crate) fn refresh_texture_inspector(
    w: &MainWindow,
    s: &hm64_sprite_editor::model::SpriteAsset,
    tex: &hm64_sprite_editor::model::Texture,
) {
    w.set_texture_preview(textures_vm::render_texture_using_manifest_palette(s, tex));
    w.set_texture_info(
        format!(
            "#{}  {}×{}  {:?}  palette {}",
            tex.index, tex.width, tex.height, tex.format, tex.palette_index
        )
        .into(),
    );
    let row = palettes_vm::palette_row_for_id(s, tex.palette_index).unwrap_or(-1);
    w.set_texture_palette_index(row);
}

pub(crate) fn publish_texture_thumbs(w: &MainWindow, ed: &Editor) {
    if let Some(s) = ed.current_sprite() {
        publish_texture_thumbs_inner(w, s);
    }
}

pub(crate) fn publish_texture_thumbs_inner(w: &MainWindow, s: &hm64_sprite_editor::model::SpriteAsset) {
    w.set_texture_thumbs(textures_vm::texture_thumbs(s));
}

