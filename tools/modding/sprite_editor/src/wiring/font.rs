//! Font workflow + new-font wizard callbacks. The font tab shares the
//! color picker overlay with the palette tab via the `picker_font_mode`
//! flag (set here, consumed by `apply_picker_rgb` in main.rs).

use hm64_sprite_editor::ui::slint_generated::{
    FontPaletteCellVm, FontPaletteRowVm, MainWindow,
};
use slint::ComponentHandle;

use crate::wiring::sprite_browser::append_font_tree_rows;
use crate::wiring::AppState;

/// Pixel scale at which the font contact sheet is rendered. Both the renderer
/// and the click hit-test must use the same value, so they share this constant.
const CONTACT_SHEET_SCALE: u32 = 3;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_font_contact_sheet_clicked(move |x, y| {
            let Some(w) = window_weak.upgrade() else { return };
            let maybe_idx = {
                let ed = editor.borrow();
                let Some(asset) = ed.current_font() else { return };
                hm64_sprite_editor::ui::font_glyph_grid_vm::glyph_at(
                    asset, x as u32, y as u32, CONTACT_SHEET_SCALE,
                )
            };
            if let Some(idx) = maybe_idx {
                w.set_font_selected_glyph(idx as i32);
                w.set_font_selected_label(format!("{idx}").into());
                refresh_font_views(&editor.borrow(), &w);
            }
        });
    }
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_font_glyph_pixel_clicked(move |x, y, button| {
            let Some(w) = window_weak.upgrade() else { return };
            let scale = 20u32;
            let Some(pi) = hm64_sprite_editor::ui::font_glyph_grid_vm::pixel_at(
                x as u32,
                y as u32,
                scale,
            ) else {
                return;
            };
            let glyph_idx = w.get_font_selected_glyph() as u32;
            let value = if button == 1 { 0 } else { w.get_font_active_color_index() as u8 };
            {
                let mut ed = editor.borrow_mut();
                if let Some(cmd) = hm64_sprite_editor::cmd::font_pixel_cmds::SetGlyphPixel::new(
                    glyph_idx, pi, value, &ed,
                ) {
                    let _ = ed.run(Box::new(cmd));
                }
            }
            refresh_font_views(&editor.borrow(), &w);
        });
    }
    {
        // Re-render the contact sheet and per-glyph editor when the user picks
        // a different palette from the dropdown. The Slint <=> binding has
        // already updated `font-selected-palette`; we just need to repaint.
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_font_palette_changed(move |_idx| {
            let Some(w) = window_weak.upgrade() else { return };
            refresh_font_views(&editor.borrow(), &w);
        });
    }
    {
        // The color radio's visual highlight comes from the <=> binding on
        // `font-active-color-index`; this handler just records that an event
        // fired (kept for symmetry with the palette callback and as a hook
        // point if we ever need side effects).
        let window_weak = window.as_weak();
        window.on_font_color_radio_changed(move |idx| {
            if let Some(w) = window_weak.upgrade() {
                w.set_font_active_color_index(idx);
            }
        });
    }
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_font_palette_cell_clicked(move |pal_idx, color_idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let ed = editor.borrow();
            let Some(asset) = ed.current_font() else { return };
            let Some(pal) = asset.palettes.get(&(pal_idx as u32)) else { return };
            let c = pal.colors[color_idx as usize];
            drop(ed);
            w.set_picker_font_mode(true);
            w.set_picker_font_palette_index(pal_idx);
            w.set_picker_font_color_index(color_idx);
            w.set_picker_pre_r(c.r as i32);
            w.set_picker_pre_g(c.g as i32);
            w.set_picker_pre_b(c.b as i32);
            w.set_picker_pre_a(c.a as i32);
            w.set_picker_r(c.r as i32);
            w.set_picker_g(c.g as i32);
            w.set_picker_b(c.b as i32);
            w.set_picker_a(c.a as i32);
            w.set_picker_visible(true);
        });
    }

    // -------------------------------------------------------------------------
    // New-font wizard handlers
    // -------------------------------------------------------------------------
    {
        let window_weak = window.as_weak();
        window.on_new_font_wizard_open(move || {
            let Some(w) = window_weak.upgrade() else { return };
            w.set_new_font_name("".into());
            w.set_new_font_glyph_count(700);
            w.set_new_font_initial_palette_index(0);
            w.set_new_font_error("".into());
            w.set_new_font_wizard_visible(true);
        });
    }
    {
        let window_weak = window.as_weak();
        window.on_new_font_wizard_cancel(move || {
            if let Some(w) = window_weak.upgrade() {
                w.set_new_font_wizard_visible(false);
            }
        });
    }
    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_new_font_wizard_create(move || {
            let Some(w) = window_weak.upgrade() else { return };
            // Resolve asset root from the editor (mirrors cfg.asset_root).
            let asset_root = {
                let ed = app.editor.borrow();
                match ed.asset_root.clone() {
                    Some(r) => r,
                    None => {
                        w.set_new_font_error("Open an asset root first".into());
                        return;
                    }
                }
            };
            let form = hm64_sprite_editor::ui::new_font_wizard_vm::WizardForm {
                name: w.get_new_font_name().to_string(),
                glyph_count: w.get_new_font_glyph_count() as u32,
                initial_palette: match w.get_new_font_initial_palette_index() {
                    0 => hm64_sprite_editor::ui::new_font_wizard_vm::InitialPalette::Black,
                    1 => hm64_sprite_editor::ui::new_font_wizard_vm::InitialPalette::Greyscale,
                    _ => hm64_sprite_editor::ui::new_font_wizard_vm::InitialPalette::CopyDefault,
                },
            };
            // For CopyDefault, load the existing default font's palette 1.
            let default_pal: Option<hm64_sprite_editor::model::FontPalette> =
                match form.initial_palette {
                    hm64_sprite_editor::ui::new_font_wizard_vm::InitialPalette::CopyDefault => {
                        let p = asset_root.join("assets/font");
                        hm64_sprite_editor::io::font_loader::load_font_asset(&p)
                            .ok()
                            .and_then(|a| a.palettes.get(&1).cloned())
                    }
                    _ => None,
                };
            let asset = match hm64_sprite_editor::ui::new_font_wizard_vm::build_asset(
                &asset_root,
                &form,
                default_pal.as_ref(),
            ) {
                Ok(a) => a,
                Err(e) => {
                    w.set_new_font_error(format!("{e}").into());
                    return;
                }
            };
            if let Err(e) = hm64_sprite_editor::ui::new_font_wizard_vm::commit(&asset) {
                w.set_new_font_error(format!("{e}").into());
                return;
            }
            // Refresh font rows now that a new font directory exists.
            *app.font_rows.borrow_mut() = discover_font_rows(&asset_root);
            // Load the new font.
            {
                let mut ed = app.editor.borrow_mut();
                if let Err(e) = ed.load_font(&asset.dir) {
                    w.set_new_font_error(format!("{e}").into());
                    return;
                }
            }
            append_font_tree_rows(
                &app.font_rows.borrow(),
                &app.rows_model,
                app.font_root_expanded.get(),
            );
            w.set_font_active(true);
            w.set_new_font_wizard_visible(false);
            refresh_font_views(&app.editor.borrow(), &w);
        });
    }
}

// ---------------------------------------------------------------------------
// Helpers (relocated from main.rs)
// ---------------------------------------------------------------------------

/// Discover all loadable font directories under `<root>/assets/font/`.
/// Returns (display_name, dir_path) pairs.
pub(crate) fn discover_font_rows(root: &std::path::Path) -> Vec<(String, std::path::PathBuf)> {
    let mut rows: Vec<(String, std::path::PathBuf)> = Vec::new();
    let font_root = root.join("assets/font");
    if !font_root.is_dir() {
        return rows;
    }
    // The root font directory itself (default font).
    if font_root.join("fontTexture.ci2").is_file() {
        rows.push(("default".into(), font_root.clone()));
    }
    // Sub-directories (named font variants).
    if let Ok(rd) = std::fs::read_dir(&font_root) {
        let mut sub: Vec<(String, std::path::PathBuf)> = rd
            .flatten()
            .filter_map(|entry| {
                let path = entry.path();
                if path.is_dir() && path.join("fontTexture.ci2").is_file() {
                    let name = path.file_name().and_then(|n| n.to_str())?.to_string();
                    Some((name, path))
                } else {
                    None
                }
            })
            .collect();
        sub.sort_by(|a, b| a.0.cmp(&b.0));
        rows.extend(sub);
    }
    rows
}

/// Rebuild all font-mode view properties from the current editor state.
pub(crate) fn refresh_font_views(
    editor: &hm64_sprite_editor::state::editor::Editor,
    w: &MainWindow,
) {
    use hm64_sprite_editor::ui::{font_glyph_grid_vm as grid, font_palette_vm as pvm};

    let Some(asset) = editor.current_font() else {
        w.set_font_active(false);
        return;
    };

    // Palette options.
    let palette_keys: Vec<u32> = asset.palettes.keys().copied().collect();
    let palette_options: Vec<slint::SharedString> = palette_keys
        .iter()
        .map(|k| slint::SharedString::from(format!("Palette {k}")))
        .collect();
    w.set_font_palette_options(
        slint::ModelRc::new(slint::VecModel::from(palette_options)),
    );

    let selected_pal = w.get_font_selected_palette() as usize;
    let pal_index = palette_keys
        .get(selected_pal)
        .copied()
        .unwrap_or_else(|| palette_keys.first().copied().unwrap_or(1));

    // Contact sheet image. Scale 3 keeps the sheet readable on the larger
    // window — it must match `CONTACT_SHEET_SCALE` in the click handler so
    // hit-testing maps screen pixels back to glyph indices correctly.
    let sheet = grid::render_contact_sheet(asset, pal_index, CONTACT_SHEET_SCALE);
    let sheet_w = sheet.size().width as i32;
    let sheet_h = sheet.size().height as i32;
    w.set_font_contact_sheet(sheet);
    w.set_font_contact_sheet_width(sheet_w);
    w.set_font_contact_sheet_height(sheet_h);

    // Selected glyph editor image.
    let g = w.get_font_selected_glyph() as usize;
    let glyph_pixels = asset
        .glyphs
        .get(g)
        .map(|gl| gl.pixels)
        .unwrap_or([0u8; 14 * 14]);
    let live: [hm64_sprite_editor::model::Rgba; 4] = if let Some(p) = asset.palettes.get(&pal_index) {
        [p.colors[0], p.colors[1], p.colors[2], p.colors[3]]
    } else {
        [hm64_sprite_editor::model::Rgba::default(); 4]
    };
    let editor_img = grid::render_glyph(&glyph_pixels, &live, 20);
    w.set_font_glyph_editor(editor_img);

    // Palette rows.
    let rows = pvm::build_rows(asset);
    let slint_rows: Vec<FontPaletteRowVm> = rows
        .into_iter()
        .map(|r| FontPaletteRowVm {
            palette_index: r.palette_index as i32,
            label: r.label.into(),
            cell_0: rgba_to_cell_vm(r.cells[0]),
            cell_1: rgba_to_cell_vm(r.cells[1]),
            cell_2: rgba_to_cell_vm(r.cells[2]),
            cell_3: rgba_to_cell_vm(r.cells[3]),
        })
        .collect();
    w.set_font_palette_rows(slint::ModelRc::new(slint::VecModel::from(slint_rows)));
}

pub(crate) fn rgba_to_cell_vm(c: hm64_sprite_editor::model::Rgba) -> FontPaletteCellVm {
    FontPaletteCellVm {
        r: c.r as i32,
        g: c.g as i32,
        b: c.b as i32,
        a: c.a as i32,
    }
}

