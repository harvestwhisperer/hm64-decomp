//! Sprite browser callbacks: filter input + the catch-all row-click
//! dispatcher (toggle expand, load font, load sprite). The row-click handler
//! is the entry point for sprite/font loading and so reaches into nearly
//! every other domain to refresh their views.

use hm64_sprite_editor::ui::animations_vm::describe_animation;
use hm64_sprite_editor::ui::slint_generated::{AnimThumb, MainWindow, TextureThumb, TreeRow};
use hm64_sprite_editor::ui::sprite_browser_vm;
use slint::{ComponentHandle, Model, SharedString, VecModel};

use crate::wiring::animations::rebuild_anim_editor_rows;
use crate::wiring::font::refresh_font_views;
use crate::wiring::helpers::{publish_dirty, toast};
use crate::wiring::palettes::{publish_palette_cells, publish_palette_names, publish_palette_rows};
use crate::wiring::script::publish_script_panel;
use crate::wiring::textures::clear_texture_inspector;
use crate::wiring::{AppState, NewFolderState, SpriteDragState};

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let app = state.clone();
        window.on_filter_changed(move |needle| {
            *app.browser_filter.borrow_mut() = needle.to_string();
            publish_tree_rows(
                &app.tree.borrow(),
                &app.expanded.borrow(),
                &app.browser_filter.borrow(),
                &app.rows_model,
            );
            // Filter forces all sprite categories expanded; mirror that for the
            // font row so a search can surface font matches.
            let force = !app.browser_filter.borrow().is_empty();
            append_font_tree_rows(
                &app.font_rows.borrow(),
                &app.rows_model,
                force || app.font_root_expanded.get(),
            );
        });
    }

    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_row_clicked(move |row_idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let Some(row) = app.rows_model.row_data(row_idx as usize) else { return };
            // Kind 0 = Category, 1 = Group, 2 = Sprite.
            if row.kind != 2 {
                // Toggle expand/collapse. Filter forces everything expanded, so
                // ignore caret clicks while filtering — clearing the filter
                // restores the persisted collapsed state.
                if !app.browser_filter.borrow().is_empty() {
                    return;
                }
                // Font category row uses a synthetic node_key that doesn't
                // decode into the sprite NodeKey enum; toggle its dedicated
                // state instead.
                if row.node_key.as_str() == "font:root" {
                    app.font_root_expanded.set(!app.font_root_expanded.get());
                    publish_tree_rows(
                        &app.tree.borrow(),
                        &app.expanded.borrow(),
                        &app.browser_filter.borrow(),
                        &app.rows_model,
                    );
                    append_font_tree_rows(
                        &app.font_rows.borrow(),
                        &app.rows_model,
                        app.font_root_expanded.get(),
                    );
                    return;
                }
                let Some(key) = sprite_browser_vm::NodeKey::decode(row.node_key.as_str()) else {
                    return;
                };
                {
                    let mut set = app.expanded.borrow_mut();
                    if set.contains(&key) {
                        set.remove(&key);
                    } else {
                        set.insert(key);
                    }
                    persist_expanded(&set);
                }
                publish_tree_rows(
                    &app.tree.borrow(),
                    &app.expanded.borrow(),
                    &app.browser_filter.borrow(),
                    &app.rows_model,
                );
                append_font_tree_rows(
                    &app.font_rows.borrow(),
                    &app.rows_model,
                    app.font_root_expanded.get(),
                );
                return;
            }

            // Font leaf rows have kind == 2 and sprite_index == -1; intercept by node_key.
            if let Some(path_str) = row.node_key.as_str().strip_prefix("font:") {
                if path_str != "root" {
                    let path = std::path::PathBuf::from(path_str);
                    let mut ed = app.editor.borrow_mut();
                    if let Err(e) = ed.load_font(&path) {
                        w.set_status_text(format!("Failed to load font: {e}").into());
                        return;
                    }
                    w.set_font_active(true);
                    drop(ed);
                    refresh_font_views(&app.editor.borrow(), &w);
                }
                return;
            }

            let sprite_index = row.sprite_index;
            if sprite_index < 0 {
                return;
            }

            if app.editor.borrow().dirty.any() {
                let answer = rfd::MessageDialog::new()
                    .set_title("Unsaved changes")
                    .set_description(
                        "Current sprite has unsaved changes.\nDiscard and load new sprite?",
                    )
                    .set_buttons(rfd::MessageButtons::OkCancel)
                    .show();
                if answer != rfd::MessageDialogResult::Ok {
                    return;
                }
            }

            w.set_selected_sprite_index(sprite_index);
            let dir_opt = {
                let t = app.tree.borrow();
                t.sprites.get(sprite_index as usize).map(|s| s.dir.clone())
            };
            let Some(dir) = dir_opt else { return };
            let load_result = app.editor.borrow_mut().load_sprite(&dir);
            match load_result {
                Ok(()) => {
                    let label = app
                        .editor
                        .borrow()
                        .current_sprite()
                        .map(|s| s.manifest.label.clone())
                        .unwrap_or_default();
                    w.set_status_text(format!("Loaded: {label}").into());
                    // font-active gates which TabWidget renders (see main_window.slint);
                    // without this, switching from a font to a sprite leaves the font tab visible.
                    w.set_font_active(false);
                    let sprite_dir_and_kind: Option<(
                        std::path::PathBuf,
                        hm64_sprite_editor::model::SpriteKind,
                    )> = {
                        let ed = app.editor.borrow();
                        if let Some(s) = ed.current_sprite() {
                            // Blank thumbs — images are filled lazily by the timer.
                            let thumbs: Vec<TextureThumb> = s
                                .textures
                                .values()
                                .map(|t| TextureThumb {
                                    index: t.index as i32,
                                    width: t.width as i32,
                                    height: t.height as i32,
                                    palette_index: t.palette_index as i32,
                                    image: slint::Image::default(),
                                })
                                .collect();
                            // Queue textures for deferred rendering (BTreeMap
                            // order = model row order, so row == enumerate index).
                            *app.pending_tex.borrow_mut() = s
                                .textures
                                .keys()
                                .enumerate()
                                .map(|(row, &idx)| (idx, row))
                                .collect();
                            w.set_texture_thumbs(
                                std::rc::Rc::new(slint::VecModel::from(thumbs)).into(),
                            );
                            clear_texture_inspector(&w);

                            publish_palette_rows(&w, &ed);
                            w.set_picker_visible(false);
                            publish_palette_names(&w, &ed);
                            w.set_selected_palette(-1);
                            publish_palette_cells(&w, &ed, -1);

                            app.anims_vm.borrow_mut().reset_for_sprite(s);
                            // Build O(1) anim-index → model-row map.
                            *app.anim_row_map.borrow_mut() = s
                                .animations
                                .keys()
                                .enumerate()
                                .map(|(row, &idx)| (idx, row))
                                .collect();
                            let thumbs: Vec<AnimThumb> = s
                                .animations
                                .values()
                                .map(|a| AnimThumb {
                                    index: a.index as i32,
                                    label: describe_animation(a).into(),
                                    image: slint::Image::default(),
                                })
                                .collect();
                            w.set_anim_thumbs(
                                std::rc::Rc::new(slint::VecModel::from(thumbs)).into(),
                            );
                            w.set_selected_anim(-1);
                            w.set_selected_anim_row(-1);
                            w.set_anim_detail_image(slint::Image::default());
                            w.set_anim_detail_info("".into());
                            rebuild_anim_editor_rows(&w, &ed, &app.anims_vm.borrow());
                        }
                        // Capture what we need for animation-data loading before ed drops.
                        ed.current_sprite().map(|s| (s.dir.clone(), s.manifest.kind))
                    }; // ed (Ref) dropped here — safe to borrow_mut below
                    if let Some((sprite_dir, kind)) = sprite_dir_and_kind {
                        let res = app
                            .editor
                            .borrow_mut()
                            .load_animation_data_for_sprite(&sprite_dir, kind);
                        match res {
                            Ok(_) => {}
                            Err(e) => {
                                w.set_status_text("Animation data load failed".into());
                                toast(&window_weak, format!("Animation data load error: {e}"));
                            }
                        }
                    }
                    // Reset selection state and then publish the Script panel.
                    w.set_script_selected_entry(-1);
                    w.set_script_can_edit_entry(false);
                    w.set_script_editor_offset(0);
                    w.set_script_editor_anim_type(0);
                    w.set_script_editor_flip(false);
                    w.set_script_editor_label("".into());
                    let ed = app.editor.borrow();
                    publish_script_panel(&w, &ed);
                    publish_dirty(&w, &ed);
                }
                Err(e) => {
                    w.set_status_text("Load failed".into());
                    toast(&window_weak, format!("Load failed: {e}"));
                }
            }
        });
    }

    // --- Sprite drag-and-drop -----------------------------------------------
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_try_start_drag(move |sprite_index, source_abs_y| -> bool {
            let Some(w) = window_weak.upgrade() else { return false };
            try_start_drag(&w, &app, sprite_index, source_abs_y)
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_drag_move(move |pointer_y, source_abs_y_now| {
            let Some(w) = window_weak.upgrade() else { return };
            drag_move(&w, &app, pointer_y, source_abs_y_now);
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_commit_drag(move || {
            let Some(w) = window_weak.upgrade() else { return };
            commit_drag(&w, &app, &window_weak);
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_cancel_drag(move || {
            if let Some(w) = window_weak.upgrade() {
                cancel_drag(&w, &app, &window_weak);
            }
        });
    }

    // --- New-subfolder dialog ----------------------------------------------
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_new_folder_clicked(move |category| {
            let Some(w) = window_weak.upgrade() else { return };
            *app.new_folder.borrow_mut() =
                NewFolderState { category: category.to_string(), name: String::new() };
            w.set_new_folder_category(category);
            w.set_new_folder_message("".into());
            w.set_new_folder_visible(true);
        });
    }
    {
        let app = state.clone();
        window.on_new_folder_name_edited(move |t| {
            app.new_folder.borrow_mut().name = t.to_string();
        });
    }
    {
        let window_weak = window.as_weak();
        window.on_new_folder_cancel(move || {
            if let Some(w) = window_weak.upgrade() {
                w.set_new_folder_visible(false);
                w.set_new_folder_message("".into());
            }
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_new_folder_accept(move || {
            let Some(w) = window_weak.upgrade() else { return };
            create_new_folder(&w, &app);
        });
    }
}

// ---------------------------------------------------------------------------
// Drag-and-drop implementation
// ---------------------------------------------------------------------------

fn try_start_drag(w: &MainWindow, app: &AppState, sprite_index: i32, _source_abs_y: f32) -> bool {
    if sprite_index < 0 {
        return false;
    }
    // Look up source sprite metadata. `subdir` from the manifest is the full
    // nested path (e.g. "entitySprites/npc" for grouped sprites), so we strip
    // off the group component to get the top-level category that matches the
    // Category-row `NodeKey`.
    let info = {
        let tree = app.tree.borrow();
        tree.sprites
            .get(sprite_index as usize)
            .map(|s| (s.dir.clone(), top_level_category(&s.subdir), s.group.clone()))
    };
    let Some((sprite_dir, category, group)) = info else { return false };

    // Block drag if the currently-loaded sprite is the one being dragged and
    // has unsaved changes. Other sprites' load state is irrelevant.
    let is_loaded_and_dirty = {
        let ed = app.editor.borrow();
        ed.current_sprite().map(|s| s.dir == sprite_dir).unwrap_or(false) && ed.dirty.any()
    };
    if is_loaded_and_dirty {
        w.set_status_text("Save changes before relocating this sprite".into());
        return false;
    }

    // Find the source's row index in the flattened rows model so geometry
    // calculations in `drag_move` have a fixed reference point.
    let source_row_index = (0..app.rows_model.row_count())
        .find(|&i| app.rows_model.row_data(i).map(|r| r.sprite_index) == Some(sprite_index));
    let Some(source_row_index) = source_row_index else { return false };

    *app.sprite_drag.borrow_mut() = Some(SpriteDragState {
        source_sprite_index: sprite_index,
        source_row_index,
        source_category: category,
        source_group: group,
    });
    w.set_drag_source_sprite_index(sprite_index);
    w.set_drop_target_row(-1);
    true
}

fn drag_move(w: &MainWindow, app: &AppState, pointer_y: f32, source_abs_y_now: f32) {
    let Some(drag) = app.sprite_drag.borrow().clone() else { return };
    let ui_scale = w.get_ui_scale();
    let row_height = |kind: i32| -> f32 {
        if kind == 2 { 42.0 * ui_scale } else { 24.0 * ui_scale }
    };

    // Use the CURRENT source row Y (passed fresh each call) instead of the
    // value captured at drag start, so geometry stays correct after auto-
    // scroll moves the source row on screen.
    let total = app.rows_model.row_count();
    let delta = pointer_y - source_abs_y_now;
    let target_row: Option<usize> = if delta >= 0.0 {
        let mut acc = 0.0_f32;
        let mut hit: Option<usize> = None;
        for j in drag.source_row_index..total {
            let kind = app.rows_model.row_data(j).map(|r| r.kind).unwrap_or(2);
            let h = row_height(kind);
            if delta < acc + h {
                hit = Some(j);
                break;
            }
            acc += h;
        }
        hit
    } else {
        let mut acc = 0.0_f32;
        let mut j = drag.source_row_index;
        let mut hit: Option<usize> = None;
        while j > 0 {
            j -= 1;
            let kind = app.rows_model.row_data(j).map(|r| r.kind).unwrap_or(2);
            acc += row_height(kind);
            if -delta <= acc {
                hit = Some(j);
                break;
            }
        }
        hit
    };

    let valid = target_row.and_then(|j| resolve_drop_target_row(app, j, &drag));
    w.set_drop_target_row(valid.map(|j| j as i32).unwrap_or(-1));
}

/// Resolve a row index to a valid drop target. Returns `Some(j)` if the row
/// is a valid drop target (header or sprite whose parent is a different
/// directory than the source's current parent, within the same category).
///
/// For sprite-row targets, we treat the drop as "into this sprite's parent"
/// — so dropping on any top-level sprite is the same as dropping on the
/// category header, and dropping on a grouped sprite is the same as
/// dropping on that group's header.
fn resolve_drop_target_row(
    app: &AppState,
    target_row: usize,
    drag: &SpriteDragState,
) -> Option<usize> {
    let r = app.rows_model.row_data(target_row)?;
    let (target_category, target_group) = parent_of_row(app, &r)?;
    if target_category != drag.source_category {
        return None;
    }
    if target_group.as_deref() == drag.source_group.as_deref() {
        return None; // same dir as source — no-op
    }
    Some(target_row)
}

/// Returns `(category, group)` for the directory a row LIVES IN.
/// - Category row: `(cat_name, None)` — its own children's parent is the
///   category root.
/// - Group row: `(category, Some(group_name))` — children live in the group.
/// - Sprite row: looks up the sprite and reads its category/group fields.
fn parent_of_row(
    app: &AppState,
    r: &hm64_sprite_editor::ui::slint_generated::TreeRow,
) -> Option<(String, Option<String>)> {
    if r.kind == 2 {
        if r.sprite_index < 0 {
            return None;
        }
        let tree = app.tree.borrow();
        let sprite = tree.sprites.get(r.sprite_index as usize)?;
        return Some((top_level_category(&sprite.subdir), sprite.group.clone()));
    }
    let key = sprite_browser_vm::NodeKey::decode(r.node_key.as_str())?;
    match key {
        sprite_browser_vm::NodeKey::Category(cat) => Some((cat, None)),
        sprite_browser_vm::NodeKey::Group { category, name } => Some((category, Some(name))),
    }
}

fn commit_drag(_w: &MainWindow, app: &AppState, window_weak: &slint::Weak<MainWindow>) {
    // Take the drag state synchronously (Rust state only — no Slint writes).
    // Setting Slint properties (drag-source-sprite-index, drop-target-row,
    // status-text, selected-sprite-index) from inside the source TouchArea's
    // `pointer-event(up)` handler triggers binding re-evaluation that
    // interferes with Slint's pointer-capture release. The capture stays
    // pinned to the original TouchArea, so subsequent presses on other rows
    // never fire `pointer-event(down)`. Defer all Slint writes (and the
    // filesystem operation) to the next event-loop tick so the up handler
    // returns cleanly.
    let drag = app.sprite_drag.borrow_mut().take();
    let app = app.clone();
    let window_weak = window_weak.clone();
    slint::Timer::single_shot(std::time::Duration::from_millis(1), move || {
        let Some(w) = window_weak.upgrade() else { return };
        let target_row = w.get_drop_target_row();
        w.set_drag_source_sprite_index(-1);
        w.set_drop_target_row(-1);
        w.set_drag_pointer_y(-1.0);

        let Some(drag) = drag else { return };
        if target_row < 0 {
            return;
        }

        let Some(asset_root) = app.editor.borrow().asset_root.clone() else { return };
        let sprites_root = asset_root.join("assets/sprites");

        let target_parent: Option<std::path::PathBuf> = app
            .rows_model
            .row_data(target_row as usize)
            .and_then(|r| parent_of_row(&app, &r))
            .and_then(|(cat, grp)| {
                if cat != drag.source_category {
                    return None;
                }
                if grp.as_deref() == drag.source_group.as_deref() {
                    return None;
                }
                Some(match grp {
                    Some(g) => sprites_root.join(cat).join(g),
                    None => sprites_root.join(cat),
                })
            });
        let Some(target_parent) = target_parent else { return };

        let source_dir = {
            let tree = app.tree.borrow();
            match tree.sprites.get(drag.source_sprite_index as usize) {
                Some(s) => s.dir.clone(),
                None => return,
            }
        };
        let Some(file_name) = source_dir.file_name().map(|n| n.to_owned()) else { return };
        let new_path = target_parent.join(&file_name);

        if new_path.exists() {
            w.set_status_text(
                "Move failed: target already contains a folder of that name".into(),
            );
            toast(&window_weak, format!("Cannot move: {} already exists", new_path.display()));
            return;
        }

        if let Err(e) = std::fs::rename(&source_dir, &new_path) {
            w.set_status_text("Move failed".into());
            toast(&window_weak, format!("Move failed: {e}"));
            return;
        }

        let was_loaded = app
            .editor
            .borrow()
            .current_sprite()
            .map(|s| s.dir == source_dir)
            .unwrap_or(false);
        if was_loaded {
            if let Some(sprite) = app.editor.borrow_mut().current_sprite_mut() {
                sprite.dir = new_path.clone();
            }
        }

        *app.tree.borrow_mut() = sprite_browser_vm::discover(&asset_root);
        publish_tree_rows(
            &app.tree.borrow(),
            &app.expanded.borrow(),
            &app.browser_filter.borrow(),
            &app.rows_model,
        );
        append_font_tree_rows(
            &app.font_rows.borrow(),
            &app.rows_model,
            app.font_root_expanded.get(),
        );

        let loaded_dir = app.editor.borrow().current_sprite().map(|s| s.dir.clone());
        let new_selected = loaded_dir
            .and_then(|d| {
                app.tree.borrow().sprites.iter().position(|s| s.dir == d).map(|i| i as i32)
            })
            .unwrap_or(-1);
        w.set_selected_sprite_index(new_selected);

        let label = file_name.to_string_lossy().into_owned();
        let dest = new_path
            .strip_prefix(&sprites_root)
            .unwrap_or(&new_path)
            .display()
            .to_string();
        w.set_status_text(format!("Moved '{label}' to {dest}").into());
    });
}

fn cancel_drag(_w: &MainWindow, app: &AppState, window_weak: &slint::Weak<MainWindow>) {
    // Same reasoning as `commit_drag`: defer Slint writes so the
    // pointer-event(up) handler returns before bindings re-evaluate.
    *app.sprite_drag.borrow_mut() = None;
    let window_weak = window_weak.clone();
    slint::Timer::single_shot(std::time::Duration::from_millis(1), move || {
        if let Some(w) = window_weak.upgrade() {
            w.set_drag_source_sprite_index(-1);
            w.set_drop_target_row(-1);
            w.set_drag_pointer_y(-1.0);
        }
    });
}

/// Extract the top-level sprite category from a manifest `subdir`. The
/// extractor writes the full nested path ("entitySprites/npc") for grouped
/// sprites, but `NodeKey::Category` only carries the top-level directory
/// name ("entitySprites"), so we split on '/' to compare them.
fn top_level_category(subdir: &str) -> String {
    subdir.split('/').next().unwrap_or(subdir).to_string()
}

/// Validate a new-subfolder name. Reject empty names and anything that
/// contains a path separator so the user can't accidentally escape the
/// category dir.
fn validate_folder_name(name: &str) -> Result<(), &'static str> {
    let trimmed = name.trim();
    if trimmed.is_empty() {
        return Err("Folder name cannot be empty");
    }
    if trimmed.contains('/') || trimmed.contains('\\') {
        return Err("Folder name cannot contain slashes");
    }
    if trimmed == "." || trimmed == ".." {
        return Err("Invalid folder name");
    }
    Ok(())
}

fn create_new_folder(w: &MainWindow, app: &AppState) {
    let (category, name) = {
        let nf = app.new_folder.borrow();
        (nf.category.clone(), nf.name.trim().to_string())
    };
    if let Err(e) = validate_folder_name(&name) {
        w.set_new_folder_message(e.into());
        return;
    }
    let Some(asset_root) = app.editor.borrow().asset_root.clone() else {
        w.set_new_folder_message("No asset root configured".into());
        return;
    };
    let dir = asset_root.join("assets/sprites").join(&category).join(&name);
    if dir.exists() {
        w.set_new_folder_message("A folder with that name already exists".into());
        return;
    }
    if let Err(e) = std::fs::create_dir_all(&dir) {
        w.set_new_folder_message(format!("Create failed: {e}").into());
        return;
    }

    // Refresh the tree so the new (empty) group appears.
    *app.tree.borrow_mut() = sprite_browser_vm::discover(&asset_root);
    publish_tree_rows(
        &app.tree.borrow(),
        &app.expanded.borrow(),
        &app.browser_filter.borrow(),
        &app.rows_model,
    );
    append_font_tree_rows(
        &app.font_rows.borrow(),
        &app.rows_model,
        app.font_root_expanded.get(),
    );

    w.set_new_folder_visible(false);
    w.set_new_folder_message("".into());
    w.set_status_text(format!("Created '{category}/{name}'").into());
}

// ---------------------------------------------------------------------------
// Helpers (relocated from main.rs)
// ---------------------------------------------------------------------------

pub(crate) fn publish_tree_rows(
    tree: &sprite_browser_vm::SpriteTree,
    expanded: &std::collections::HashSet<sprite_browser_vm::NodeKey>,
    filter: &str,
    model: &VecModel<TreeRow>,
) {
    let rows = sprite_browser_vm::flatten(tree, expanded, filter);
    model.set_vec(rows.iter().map(to_tree_row).collect::<Vec<_>>());
}

pub(crate) fn persist_expanded(set: &std::collections::HashSet<sprite_browser_vm::NodeKey>) {
    let mut cfg = hm64_sprite_editor::state::config::load();
    let mut keys: Vec<String> = set.iter().map(|k| k.encode()).collect();
    keys.sort();
    cfg.browser_expanded = Some(keys);
    let _ = hm64_sprite_editor::state::config::save(&cfg);
}

pub(crate) fn to_tree_row(r: &sprite_browser_vm::TreeRow) -> TreeRow {
    let key = match r.kind {
        sprite_browser_vm::TreeRowKind::Category => {
            sprite_browser_vm::NodeKey::Category(r.category.clone()).encode()
        }
        sprite_browser_vm::TreeRowKind::Group => sprite_browser_vm::NodeKey::Group {
            category: r.category.clone(),
            name: r.group.clone().unwrap_or_default(),
        }
        .encode(),
        sprite_browser_vm::TreeRowKind::Sprite => String::new(),
    };
    TreeRow {
        depth: r.depth as i32,
        kind: r.kind as i32,
        label: SharedString::from(r.label.as_str()),
        stats: SharedString::from(r.stats.as_str()),
        expanded: r.expanded,
        sprite_index: r.sprite_index,
        node_key: SharedString::from(key.as_str()),
    }
}

/// Reset every sprite-tab UI property to its no-sprite baseline. Mirrors the
/// publishes the row-click handler does on load, but for the empty-editor case.
/// Call after `Editor::unload_sprite()` so the tabs don't keep showing the
/// previously-loaded sprite's data.
pub(crate) fn clear_sprite_tabs(w: &MainWindow, app: &AppState) {
    w.set_texture_thumbs(std::rc::Rc::new(VecModel::<TextureThumb>::default()).into());
    app.pending_tex.borrow_mut().clear();
    clear_texture_inspector(w);

    let ed = app.editor.borrow();
    publish_palette_rows(w, &ed);
    publish_palette_names(w, &ed);
    w.set_selected_palette(-1);
    publish_palette_cells(w, &ed, -1);
    w.set_picker_visible(false);

    app.anims_vm.borrow_mut().reset_empty();
    app.anim_row_map.borrow_mut().clear();
    w.set_anim_thumbs(std::rc::Rc::new(VecModel::<AnimThumb>::default()).into());
    w.set_selected_anim(-1);
    w.set_selected_anim_row(-1);
    w.set_anim_detail_image(slint::Image::default());
    w.set_anim_detail_info("".into());
    rebuild_anim_editor_rows(w, &ed, &app.anims_vm.borrow());

    publish_script_panel(w, &ed);
    publish_dirty(w, &ed);
}

/// Append font `TreeRow`s to an existing model (called after `publish_tree_rows`).
pub(crate) fn append_font_tree_rows(
    font_rows: &[(String, std::path::PathBuf)],
    model: &VecModel<TreeRow>,
    expanded: bool,
) {
    if font_rows.is_empty() {
        return;
    }
    model.push(TreeRow {
        depth: 0,
        kind: 0, // Category
        label: SharedString::from("font"),
        stats: SharedString::from(format!("{} font(s)", font_rows.len())),
        expanded,
        sprite_index: -1,
        node_key: SharedString::from("font:root"),
    });
    if !expanded {
        return;
    }
    for (name, path) in font_rows {
        model.push(TreeRow {
            depth: 1,
            kind: 2, // Leaf (same as sprite leaf)
            label: SharedString::from(name.as_str()),
            stats: SharedString::from(""),
            expanded: false,
            sprite_index: -1,
            node_key: SharedString::from(format!("font:{}", path.to_string_lossy()).as_str()),
        });
    }
}

