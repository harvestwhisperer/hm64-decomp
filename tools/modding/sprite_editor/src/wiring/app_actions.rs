//! Top-level application actions: asset-root selection, save-all,
//! export-bins, delete-sprite (with confirm modal), ROM extraction, and
//! undo/redo.

use hm64_sprite_editor::ui::slint_generated::MainWindow;
use hm64_sprite_editor::ui::sprite_browser_vm;
use slint::ComponentHandle;

use crate::wiring::font::discover_font_rows;
use crate::wiring::helpers::{publish_dirty, toast};
use crate::wiring::palettes::{publish_palette_cells, publish_palette_rows};
use crate::wiring::script::publish_script_panel;
use crate::wiring::sprite_browser::{append_font_tree_rows, clear_sprite_tabs, publish_tree_rows};
use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_open_asset_root(move || {
            let Some(path) = rfd::FileDialog::new()
                .set_title("Select the decomp repo root (containing assets/sprites)")
                .pick_folder()
            else {
                return;
            };
            if !path.join("assets/sprites").is_dir() {
                if let Some(w) = window_weak.upgrade() {
                    w.set_status_text("Chosen folder has no assets/sprites subdirectory".into());
                }
                return;
            }
            app.editor.borrow_mut().set_asset_root(path.clone());
            let mut cfg = hm64_sprite_editor::state::config::load();
            cfg.asset_root = Some(path.clone());
            let _ = hm64_sprite_editor::state::config::save(&cfg);

            *app.tree.borrow_mut() = sprite_browser_vm::discover(&path);
            // Switching asset roots: restore persisted expanded set if any,
            // else default (all categories expanded).
            *app.expanded.borrow_mut() = match cfg.browser_expanded.as_ref() {
                Some(keys) => keys.iter().filter_map(|s| sprite_browser_vm::NodeKey::decode(s)).collect(),
                None => sprite_browser_vm::default_expanded(&app.tree.borrow()),
            };
            *app.font_rows.borrow_mut() = discover_font_rows(&path);
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

            if let Some(w) = window_weak.upgrade() {
                w.set_asset_root_loaded(true);
                w.set_selected_sprite_index(-1);
                w.set_status_text("Asset root loaded".into());
            }
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_save_all(move || {
            let Some(w) = window_weak.upgrade() else { return };
            // Font mode keeps its synchronous path — single call, no modal needed.
            if app.editor.borrow().active == hm64_sprite_editor::model::ActiveAsset::Font {
                match app.editor.borrow_mut().save_font() {
                    Ok(()) => w.set_status_text("Font saved".into()),
                    Err(e) => w.set_status_text(format!("Save failed: {e}").into()),
                }
                return;
            }
            let plan = crate::wiring::save_all::plan_save_all(&app.editor.borrow());
            crate::wiring::save_all::start_job(&app, &window_weak, plan, "Nothing to save");
        });
    }
    {
        let app = state.clone();
        window.on_save_cancel(move || {
            crate::wiring::save_all::request_cancel(&app);
        });
    }
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_export_bins(move || {
            let Some(w) = window_weak.upgrade() else { return };

            // Font mode: export font bins.
            if editor.borrow().active == hm64_sprite_editor::model::ActiveAsset::Font {
                let ed = editor.borrow();
                let Some(asset) = ed.current_font() else { return };
                let Some(out_dir) = rfd::FileDialog::new().pick_folder() else { return };
                let mut palettes = std::collections::BTreeSet::new();
                for k in asset.palettes.keys() {
                    palettes.insert(*k);
                }
                let opts = hm64_sprite_editor::export::font_bin_export::BinExportOpts {
                    texture: true,
                    palettes,
                };
                match hm64_sprite_editor::export::font_bin_export::write_font_bins(
                    &out_dir, asset, &opts,
                ) {
                    Ok(()) => {
                        w.set_status_text(format!("Bins exported to {}", out_dir.display()).into())
                    }
                    Err(e) => w.set_status_text(format!("Bin export failed: {e}").into()),
                }
                return;
            }

            let ed = editor.borrow();
            let Some(sprite) = ed.current_sprite() else {
                w.set_status_text("No sprite loaded".into());
                return;
            };

            let mut dialog = rfd::FileDialog::new();
            let cfg = hm64_sprite_editor::state::config::load();
            if let Some(prev) = cfg.last_bin_export_dir.as_ref() {
                dialog = dialog.set_directory(prev);
            }
            let Some(out_dir) = dialog.pick_folder() else { return };

            match hm64_sprite_editor::export::sprite_bin::export_bins(sprite, &out_dir) {
                Ok(paths) => {
                    let mut cfg = hm64_sprite_editor::state::config::load();
                    cfg.last_bin_export_dir = Some(out_dir.clone());
                    let _ = hm64_sprite_editor::state::config::save(&cfg);
                    w.set_status_text(
                        format!("Wrote {} bins to {}", paths.len(), out_dir.display()).into(),
                    );
                }
                Err(e) => {
                    w.set_status_text("Export bins failed".into());
                    toast(&window_weak, format!("Export bins failed: {e}"));
                }
            }
        });
    }
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_delete_sprite(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let (sprite_dir, sprite_label) = {
                let ed = editor.borrow();
                match ed.current_sprite() {
                    Some(s) => (s.dir.clone(), s.manifest.label.clone()),
                    None => {
                        w.set_status_text("No sprite loaded".into());
                        return;
                    }
                }
            };
            w.set_delete_confirm_name(sprite_label.clone().into());
            w.set_delete_confirm_path(sprite_dir.display().to_string().into());
            w.set_delete_confirm_visible(true);
        });
    }
    {
        let window_weak = window.as_weak();
        window.on_delete_confirm_cancel(move || {
            if let Some(w) = window_weak.upgrade() {
                w.set_delete_confirm_visible(false);
            }
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_delete_confirm_accept(move || {
            let Some(w) = window_weak.upgrade() else { return };
            w.set_delete_confirm_visible(false);
            let (sprite_dir, sprite_label) = {
                let ed = app.editor.borrow();
                match ed.current_sprite() {
                    Some(s) => (s.dir.clone(), s.manifest.label.clone()),
                    None => {
                        w.set_status_text("No sprite loaded".into());
                        return;
                    }
                }
            };
            if let Err(e) = std::fs::remove_dir_all(&sprite_dir) {
                w.set_status_text("Delete failed".into());
                toast(&window_weak, format!("Delete failed: {e}"));
                return;
            }
            app.editor.borrow_mut().unload_sprite();
            clear_sprite_tabs(&w, &app);
            let asset_root = app.editor.borrow().asset_root.clone();
            if let Some(root) = asset_root {
                *app.tree.borrow_mut() = sprite_browser_vm::discover(&root);
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
            }
            w.set_selected_sprite_index(-1);
            w.set_status_text(format!("Deleted '{sprite_label}'").into());
        });
    }
    {
        let window_weak = window.as_weak();
        window.on_start_extract(move || {
            let Some(w) = window_weak.upgrade() else { return };

            let cfg = hm64_sprite_editor::state::config::load();

            let mut rom_dialog = rfd::FileDialog::new().add_filter("N64 ROM", &["z64"]);
            if let Some(prev) = cfg.last_rom_path.as_ref() {
                if let Some(parent) = prev.parent() {
                    rom_dialog = rom_dialog.set_directory(parent);
                }
            }
            let Some(rom_path) = rom_dialog.pick_file() else { return };

            let mut ws_dialog = rfd::FileDialog::new();
            if let Some(prev) = cfg.last_extract_workspace.as_ref() {
                ws_dialog = ws_dialog.set_directory(prev);
            }
            let Some(ws) = ws_dialog.pick_folder() else { return };

            // Persist user choices for next session.
            let mut cfg = hm64_sprite_editor::state::config::load();
            cfg.last_rom_path = Some(rom_path.clone());
            cfg.last_extract_workspace = Some(ws.clone());
            let _ = hm64_sprite_editor::state::config::save(&cfg);

            w.set_extract_running(true);
            w.set_extract_current("Loading ROM…".into());
            w.set_extract_done(0);
            w.set_extract_total(0);

            let weak = window_weak.clone();
            std::thread::spawn(move || {
                let weak_progress = weak.clone();
                let result =
                    hm64_sprite_editor::extract::extract_us(&rom_path, &ws, move |p| {
                        let weak = weak_progress.clone();
                        let _ = slint::invoke_from_event_loop(move || {
                            let Some(w) = weak.upgrade() else { return };
                            match p {
                                hm64_sprite_editor::extract::Progress::Started {
                                    total_sprites,
                                } => {
                                    w.set_extract_total(total_sprites as i32);
                                    w.set_extract_done(0);
                                }
                                hm64_sprite_editor::extract::Progress::Sprite {
                                    name,
                                    done,
                                    total,
                                } => {
                                    w.set_extract_current(name.into());
                                    w.set_extract_done(done as i32);
                                    w.set_extract_total(total as i32);
                                }
                                hm64_sprite_editor::extract::Progress::Finished {
                                    sprites_extracted,
                                    warnings,
                                } => {
                                    w.set_extract_running(false);
                                    w.set_status_text(
                                        format!(
                                            "Extracted {sprites_extracted} sprites ({warnings} warnings)"
                                        )
                                        .into(),
                                    );
                                }
                                hm64_sprite_editor::extract::Progress::Phase { name } => {
                                    w.set_extract_current(format!("Extracting {name}…").into());
                                }
                            }
                        });
                    });
                if let Err(e) = result {
                    let weak = weak.clone();
                    let _ = slint::invoke_from_event_loop(move || {
                        let Some(w) = weak.upgrade() else { return };
                        w.set_extract_running(false);
                        w.set_status_text(format!("Extraction failed: {e}").into());
                    });
                }
            });
        });
    }
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_undo(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let changed = match editor.borrow_mut().undo() {
                Ok(Some(_)) => {
                    w.set_status_text("Undo".into());
                    true
                }
                Ok(None) => {
                    w.set_status_text("Nothing to undo".into());
                    false
                }
                Err(e) => {
                    w.set_status_text(format!("Undo error: {e}").into());
                    false
                }
            };
            let ed = editor.borrow();
            if changed {
                publish_palette_rows(&w, &ed);
                w.set_picker_visible(false);
                let sel = w.get_selected_palette();
                publish_palette_cells(&w, &ed, sel);
                publish_script_panel(&w, &ed);
            }
            publish_dirty(&w, &ed);
        });
    }
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_redo(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let changed = match editor.borrow_mut().redo() {
                Ok(Some(_)) => {
                    w.set_status_text("Redo".into());
                    true
                }
                Ok(None) => {
                    w.set_status_text("Nothing to redo".into());
                    false
                }
                Err(e) => {
                    w.set_status_text(format!("Redo error: {e}").into());
                    false
                }
            };
            let ed = editor.borrow();
            if changed {
                publish_palette_rows(&w, &ed);
                w.set_picker_visible(false);
                let sel = w.get_selected_palette();
                publish_palette_cells(&w, &ed, sel);
                publish_script_panel(&w, &ed);
            }
            publish_dirty(&w, &ed);
        });
    }
}
