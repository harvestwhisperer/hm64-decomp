//! New-Sprite wizard callbacks. Drives the modal that creates a fresh
//! sprite directory under `<asset_root>/<category>/<name>/` and seeds it
//! from optional indexed PNGs.

use hm64_sprite_editor::ui::slint_generated::MainWindow;
use hm64_sprite_editor::ui::sprite_browser_vm;
use hm64_sprite_editor::ui::wizard_vm::WizardState;
use slint::ComponentHandle;

use crate::wiring::sprite_browser::{append_font_tree_rows, publish_tree_rows};
use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let window_weak = window.as_weak();
        let wizard = state.wizard.clone();
        window.on_wizard_open(move || {
            *wizard.borrow_mut() = WizardState::default();
            if let Some(w) = window_weak.upgrade() {
                w.set_wizard_visible(true);
                w.set_wizard_msg("".into());
                w.set_wizard_files_label("".into());
                w.set_wizard_name("".into());
                w.set_wizard_category(0);
                w.set_wizard_kind(0);
                w.set_wizard_type(0);
                w.set_wizard_seed(false);
            }
        });
    }
    {
        let window_weak = window.as_weak();
        window.on_wizard_cancel(move || {
            if let Some(w) = window_weak.upgrade() {
                w.set_wizard_visible(false);
            }
        });
    }
    {
        let wizard = state.wizard.clone();
        window.on_wizard_name_edited(move |t| {
            wizard.borrow_mut().name = t.to_string();
        });
    }
    {
        let wizard = state.wizard.clone();
        window.on_wizard_category_changed(move |v| {
            wizard.borrow_mut().category_index = v;
        });
    }
    {
        let wizard = state.wizard.clone();
        window.on_wizard_kind_changed(move |v| {
            wizard.borrow_mut().kind_index = v;
        });
    }
    {
        let wizard = state.wizard.clone();
        window.on_wizard_type_changed(move |v| {
            wizard.borrow_mut().type_index = v;
        });
    }
    {
        let wizard = state.wizard.clone();
        let window_weak = window.as_weak();
        window.on_wizard_seed_toggled(move |b| {
            wizard.borrow_mut().seed_from_png = b;
            if let Some(w) = window_weak.upgrade() {
                w.set_wizard_seed(b);
            }
        });
    }
    {
        let wizard = state.wizard.clone();
        let window_weak = window.as_weak();
        window.on_wizard_pick_pngs(move || {
            if let Some(w) = window_weak.upgrade() {
                w.set_wizard_files_label("opening picker…".into());
            }
            let Some(paths) =
                rfd::FileDialog::new().add_filter("Indexed PNG", &["png"]).pick_files()
            else {
                if let Some(w) = window_weak.upgrade() {
                    w.set_wizard_files_label("picker returned no files".into());
                }
                return;
            };
            let label = format!("{} file(s) picked", paths.len());
            wizard.borrow_mut().picked_pngs = paths;
            if let Some(w) = window_weak.upgrade() {
                w.set_wizard_files_label(label.into());
            }
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_wizard_finish(move || {
            use hm64_sprite_editor::export::scaffold::{create_new_sprite, NewSpriteSpec};
            use hm64_sprite_editor::io::png_import::import_indexed_png;
            let Some(w) = window_weak.upgrade() else {
                return;
            };
            let asset_root = {
                let ed = app.editor.borrow();
                ed.asset_root.clone()
            };
            let Some(asset_root) = asset_root else {
                w.set_wizard_msg("No asset root configured".into());
                return;
            };
            let wiz = app.wizard.borrow();
            if let Err(e) = wiz.validate(&asset_root) {
                w.set_wizard_msg(e.into());
                return;
            }
            let imports = if wiz.seed_from_png {
                let mut imps = Vec::new();
                for p in &wiz.picked_pngs {
                    match import_indexed_png(p) {
                        Ok(i) => imps.push(i),
                        Err(e) => {
                            w.set_wizard_msg(format!("{}: {e}", p.display()).into());
                            return;
                        }
                    }
                }
                imps
            } else {
                Vec::new()
            };
            let spec = NewSpriteSpec {
                asset_root: asset_root.clone(),
                category: wiz.category().into(),
                name: wiz.name.clone(),
                kind: wiz.kind(),
                sprite_type: wiz.sprite_type(),
                imports,
            };
            drop(wiz);
            match create_new_sprite(&spec) {
                Ok(created) => {
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
                    w.set_wizard_visible(false);
                    let m = &created.sprite.manifest;
                    w.set_status_text(
                        format!(
                            "New sprite created: {} textures, {} palettes",
                            m.sprite_count, m.palette_count
                        )
                        .into(),
                    );
                }
                Err(e) => w.set_wizard_msg(format!("Create failed: {e}").into()),
            }
        });
    }
}
