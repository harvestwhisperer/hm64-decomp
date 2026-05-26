//! Palette tab callbacks: list selection, cell selection (which opens the
//! color picker overlay), the picker's slider/hex handlers, and the
//! create/import/delete/save commands.

use hm64_sprite_editor::state::editor::Editor;
use hm64_sprite_editor::ui::palettes_vm;
use hm64_sprite_editor::ui::slint_generated::{ColorCell, MainWindow, PaletteRow, PaletteTextureThumb};
use slint::{ComponentHandle, SharedString};

use crate::wiring::font::refresh_font_views;
use crate::wiring::helpers::{publish_dirty, toast};
use crate::wiring::textures::{publish_texture_thumbs, refresh_texture_inspector};
use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_palette_clicked(move |idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let ed = editor.borrow();
            if ed.current_sprite().and_then(|s| s.palettes.get(&(idx as u32))).is_none() {
                return;
            }
            w.set_selected_palette(idx);
            w.set_picker_visible(false);
            w.set_palette_selected_cell(-1);
            publish_palette_cells(&w, &ed, idx);
        });
    }

    {
        let editor = state.editor.clone();
        let picker_state = state.picker_state.clone();
        let window_weak = window.as_weak();
        window.on_cell_clicked(move |cell_idx| {
            use hm64_sprite_editor::ui::color_math;
            let Some(w) = window_weak.upgrade() else { return };
            let pal_idx = w.get_selected_palette();
            if pal_idx < 0 {
                return;
            }
            w.set_palette_selected_cell(cell_idx);
            let ed = editor.borrow();
            let Some(color) = ed
                .current_sprite()
                .and_then(|s| s.palettes.get(&(pal_idx as u32)))
                .and_then(|p| p.colors.get(cell_idx as usize).copied())
            else {
                w.set_picker_visible(false);
                return;
            };

            // Derive HSV; preserve prior hue when the new color is greyscale.
            let (h, s, v) = color_math::rgb_to_hsv(color.r, color.g, color.b);
            {
                let mut ps = picker_state.borrow_mut();
                if s > 0.0 {
                    ps.hue = h;
                }
                ps.sat = s;
                ps.val = v;
                ps.alpha = color.a as f32 / 255.0;
            }
            let ps = picker_state.borrow();

            // Position: anchor to the right of the clicked cell, flip if near
            // the panel's right edge. Panel-local coordinates within the grid
            // are `col * 24px + 8px` (grid offset inside its VerticalBox).
            let cols = w.get_palette_cell_cols().max(1);
            let col = cell_idx % cols;
            let row = cell_idx / cols;
            let cell_x_px = (col as f32) * 24.0;
            let cell_y_px = (row as f32) * 24.0;
            let popover_w_px = 280.0;
            let popover_h_px = 260.0;
            // Grid sits inside the right VerticalBox at x ≈ 268 (260 left-dock
            // + 8 spacing) + 4px panel padding = ~272. The right-pane grid
            // origin is panel padding (4) + info text (~20) + spacing (6) = ~30.
            let grid_origin_x = 272.0;
            let grid_origin_y = 30.0;
            // Floor for bottom-overflow nudge-up: must clear the LEFT-pane
            // two-row button stack (panel padding 4 + 32 + 32 + ~8 spacing +
            // 12 breathing ≈ 88), since a left-flipped popover would cross
            // into the left pane's vertical band.
            let safe_top_y = 88.0;
            let mut x = grid_origin_x + cell_x_px + 24.0 + 6.0;
            let mut y = grid_origin_y + cell_y_px;
            // Flip left if overflowing right.
            // `w.window().size()` is physical pixels; picker coords are logical
            // (Slint `Coord`). Divide by scale_factor to compare in the same
            // unit system — otherwise the guard fires ~2× early on HiDPI.
            let scale = w.window().scale_factor();
            let panel_w_est = w.window().size().width as f32 / scale - 280.0 - 24.0;
            if x + popover_w_px > panel_w_est {
                x = (grid_origin_x + cell_x_px - popover_w_px - 6.0).max(grid_origin_x);
            }
            // Nudge up if overflowing bottom — but never above the button row.
            let panel_h_est = w.window().size().height as f32 / scale - 60.0;
            if y + popover_h_px > panel_h_est {
                y = (panel_h_est - popover_h_px).max(safe_top_y);
            }

            let (hr, hg, hb) = color_math::hsv_to_rgb(ps.hue, 1.0, 1.0);
            w.set_picker_x(x);
            w.set_picker_y(y);
            w.set_picker_r(color.r as i32);
            w.set_picker_g(color.g as i32);
            w.set_picker_b(color.b as i32);
            w.set_picker_a(color.a as i32);
            w.set_picker_pre_r(color.r as i32);
            w.set_picker_pre_g(color.g as i32);
            w.set_picker_pre_b(color.b as i32);
            w.set_picker_pre_a(color.a as i32);
            w.set_picker_hue_color(slint::Color::from_rgb_u8(hr, hg, hb));
            w.set_picker_sat(ps.sat);
            w.set_picker_val(ps.val);
            w.set_picker_hue_frac(ps.hue / 360.0);
            w.set_picker_alpha_frac(ps.alpha);
            w.set_picker_hex_text(color_math::format_hex(color.r, color.g, color.b, color.a).into());
            w.set_picker_visible(true);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let window_weak = window.as_weak();
        window.on_palette_commit_color(move |r, g, b, a| {
            let Some(w) = window_weak.upgrade() else { return };
            apply_picker_rgb(
                &w,
                &editor,
                &anims_vm,
                &pending_tex,
                r as u8,
                g as u8,
                b as u8,
                a as u8,
            );
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let picker_state = state.picker_state.clone();
        let window_weak = window.as_weak();
        window.on_picker_sv_pointer(move |x_frac, y_frac| {
            use hm64_sprite_editor::ui::color_math;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ps = picker_state.borrow_mut();
            ps.sat = x_frac.clamp(0.0, 1.0);
            ps.val = (1.0 - y_frac).clamp(0.0, 1.0);
            let (r, g, b) = color_math::hsv_to_rgb(ps.hue, ps.sat, ps.val);
            let a = (ps.alpha * 255.0).round() as u8;
            w.set_picker_sat(ps.sat);
            w.set_picker_val(ps.val);
            drop(ps);
            apply_picker_rgb(&w, &editor, &anims_vm, &pending_tex, r, g, b, a);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let picker_state = state.picker_state.clone();
        let window_weak = window.as_weak();
        window.on_picker_hue_pointer(move |y_frac| {
            use hm64_sprite_editor::ui::color_math;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ps = picker_state.borrow_mut();
            ps.hue = (y_frac.clamp(0.0, 1.0)) * 360.0;
            let (r, g, b) = color_math::hsv_to_rgb(ps.hue, ps.sat, ps.val);
            let a = (ps.alpha * 255.0).round() as u8;
            let (hr, hg, hb) = color_math::hsv_to_rgb(ps.hue, 1.0, 1.0);
            w.set_picker_hue_frac(ps.hue / 360.0);
            w.set_picker_hue_color(slint::Color::from_rgb_u8(hr, hg, hb));
            drop(ps);
            apply_picker_rgb(&w, &editor, &anims_vm, &pending_tex, r, g, b, a);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let picker_state = state.picker_state.clone();
        let window_weak = window.as_weak();
        window.on_picker_alpha_pointer(move |y_frac| {
            let Some(w) = window_weak.upgrade() else { return };
            let mut ps = picker_state.borrow_mut();
            // y_frac 0 = top = fully opaque, y_frac 1 = bottom = transparent.
            ps.alpha = (1.0 - y_frac.clamp(0.0, 1.0)).clamp(0.0, 1.0);
            let a = (ps.alpha * 255.0).round() as u8;
            w.set_picker_alpha_frac(ps.alpha);
            let (r, g, b) =
                (w.get_picker_r() as u8, w.get_picker_g() as u8, w.get_picker_b() as u8);
            drop(ps);
            apply_picker_rgb(&w, &editor, &anims_vm, &pending_tex, r, g, b, a);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let picker_state = state.picker_state.clone();
        let window_weak = window.as_weak();
        window.on_picker_hex_accepted(move |text| {
            use hm64_sprite_editor::ui::color_math;
            let Some(w) = window_weak.upgrade() else { return };
            let Some((r, g, b, a)) = color_math::parse_hex(text.as_str()) else {
                // Restore canonical display; reject invalid input silently.
                let rr = w.get_picker_r() as u8;
                let gg = w.get_picker_g() as u8;
                let bb = w.get_picker_b() as u8;
                let aa = w.get_picker_a() as u8;
                w.set_picker_hex_text(color_math::format_hex(rr, gg, bb, aa).into());
                return;
            };
            let (h, s, v) = color_math::rgb_to_hsv(r, g, b);
            {
                let mut ps = picker_state.borrow_mut();
                if s > 0.0 {
                    ps.hue = h;
                }
                ps.sat = s;
                ps.val = v;
                ps.alpha = a as f32 / 255.0;
            }
            let ps = picker_state.borrow();
            let (hr, hg, hb) = color_math::hsv_to_rgb(ps.hue, 1.0, 1.0);
            w.set_picker_sat(ps.sat);
            w.set_picker_val(ps.val);
            w.set_picker_hue_frac(ps.hue / 360.0);
            w.set_picker_alpha_frac(ps.alpha);
            w.set_picker_hue_color(slint::Color::from_rgb_u8(hr, hg, hb));
            drop(ps);
            apply_picker_rgb(&w, &editor, &anims_vm, &pending_tex, r, g, b, a);
        });
    }

    {
        let window_weak = window.as_weak();
        window.on_picker_close(move || {
            let Some(w) = window_weak.upgrade() else { return };
            w.set_picker_visible(false);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_new_palette(move || {
            use hm64_sprite_editor::cmd::palette_cmds::CreatePalette;
            use hm64_sprite_editor::model::{Rgba, TextureFormat};
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let (new_idx, format) = match ed.current_sprite() {
                Some(s) => {
                    let next = s.palettes.keys().max().map(|m| m + 1).unwrap_or(0);
                    let fmt =
                        s.palettes.values().next().map(|p| p.format).unwrap_or(TextureFormat::Ci4);
                    (next, fmt)
                }
                None => {
                    w.set_status_text("No sprite loaded".into());
                    return;
                }
            };
            let colors = vec![Rgba::new(0, 0, 0, 255); format.palette_len()];
            if let Err(e) = ed.run(Box::new(CreatePalette::new(new_idx, format, colors))) {
                w.set_status_text(format!("Create palette failed: {e}").into());
                return;
            }
            drop(ed);
            let ed = editor.borrow();
            publish_palette_rows(&w, &ed);
            w.set_picker_visible(false);
            publish_palette_names(&w, &ed);
            w.set_selected_palette(new_idx as i32);
            publish_palette_cells(&w, &ed, new_idx as i32);
            publish_dirty(&w, &ed);
            w.set_status_text(format!("Created palette {new_idx}").into());
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_import_palette(move || {
            use hm64_sprite_editor::cmd::palette_cmds::CreatePalette;
            use hm64_sprite_editor::model::{find_matching_palette, TextureFormat};
            let Some(w) = window_weak.upgrade() else { return };
            let path = match rfd::FileDialog::new()
                .add_filter("Palette", &["pal", "png"])
                .pick_file()
            {
                Some(p) => p,
                None => return,
            };
            let colors = match path.extension().and_then(|e| e.to_str()) {
                Some("pal") => match hm64_sprite_editor::io::pal_import::import_pal(&path) {
                    Ok(c) => c,
                    Err(e) => {
                        w.set_status_text("Import failed".into());
                        toast(&window_weak, format!("Import failed: {e}"));
                        return;
                    }
                },
                Some("png") => {
                    match hm64_sprite_editor::io::png_import::import_indexed_png(&path) {
                        Ok(imp) => imp.palette,
                        Err(e) => {
                            w.set_status_text("Import failed".into());
                            toast(&window_weak, format!("Import failed: {e}"));
                            return;
                        }
                    }
                }
                _ => {
                    w.set_status_text("Unsupported file extension".into());
                    return;
                }
            };
            let format = match colors.len() {
                16 => TextureFormat::Ci4,
                256 => TextureFormat::Ci8,
                n => {
                    w.set_status_text(
                        format!(
                            "Expected 16 or 256 colors, got {n}. Quantize externally and re-import."
                        )
                        .into(),
                    );
                    return;
                }
            };
            let mut ed = editor.borrow_mut();
            let new_idx = match ed.current_sprite() {
                Some(s) => {
                    if let Some(existing) = find_matching_palette(&colors, format, &s.palettes) {
                        w.set_status_text(
                            format!("Identical to palette {existing} — not added").into(),
                        );
                        return;
                    }
                    s.palettes.keys().max().map(|m| m + 1).unwrap_or(0)
                }
                None => {
                    w.set_status_text("No sprite loaded".into());
                    return;
                }
            };
            if let Err(e) = ed.run(Box::new(CreatePalette::new(new_idx, format, colors))) {
                w.set_status_text(format!("Import palette failed: {e}").into());
                return;
            }
            drop(ed);
            let ed = editor.borrow();
            publish_palette_rows(&w, &ed);
            w.set_picker_visible(false);
            publish_palette_names(&w, &ed);
            w.set_selected_palette(new_idx as i32);
            publish_palette_cells(&w, &ed, new_idx as i32);
            publish_dirty(&w, &ed);
            w.set_status_text(format!("Imported palette {new_idx}").into());
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_delete_palette(move || {
            use hm64_sprite_editor::cmd::palette_cmds::DeletePalette;
            let Some(w) = window_weak.upgrade() else { return };
            let idx = w.get_selected_palette();
            if idx < 0 {
                return;
            }
            let mut ed = editor.borrow_mut();
            if let Err(e) = ed.run(Box::new(DeletePalette::new(idx as u32))) {
                w.set_status_text(format!("Delete palette failed: {e}").into());
                return;
            }
            drop(ed);
            let ed = editor.borrow();
            publish_palette_rows(&w, &ed);
            w.set_picker_visible(false);
            publish_palette_names(&w, &ed);
            w.set_selected_palette(-1);
            publish_palette_cells(&w, &ed, -1);
            publish_dirty(&w, &ed);
            w.set_status_text(format!("Deleted palette {idx}").into());
        });
    }

    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_save_palettes(move || {
            let plan = crate::wiring::save_all::plan_palette_only(&app.editor.borrow());
            crate::wiring::save_all::start_job(&app, &window_weak, plan, "Palettes clean");
        });
    }
}

// ---------------------------------------------------------------------------
// Helpers (relocated from main.rs)
// ---------------------------------------------------------------------------

pub(crate) fn publish_palette_rows(w: &MainWindow, ed: &Editor) {
    let Some(s) = ed.current_sprite() else {
        w.set_palette_rows(std::rc::Rc::new(slint::VecModel::<PaletteRow>::default()).into());
        return;
    };
    w.set_palette_rows(palettes_vm::palette_rows(s));
}

pub(crate) fn publish_palette_cells(w: &MainWindow, ed: &Editor, pal_idx: i32) {
    let empty_cells = || std::rc::Rc::new(slint::VecModel::<ColorCell>::default()).into();
    let Some(s) = ed.current_sprite() else {
        w.set_palette_cells(empty_cells());
        w.set_palette_selected_cell(-1);
        w.set_palette_can_create(false);
        w.set_palette_can_delete(false);
        w.set_palette_info("".into());
        return;
    };
    let Some(p) = (if pal_idx >= 0 { s.palettes.get(&(pal_idx as u32)) } else { None }) else {
        w.set_palette_cells(empty_cells());
        w.set_palette_selected_cell(-1);
        w.set_palette_can_create(true);
        w.set_palette_can_delete(false);
        w.set_palette_info("Click a palette".into());
        w.set_palette_textures(
            std::rc::Rc::new(slint::VecModel::<PaletteTextureThumb>::default()).into(),
        );
        return;
    };
    let cols = 16i32;
    let rows = ((p.colors.len() as i32) + cols - 1) / cols;
    w.set_palette_cell_cols(cols);
    w.set_palette_cell_rows(rows);
    w.set_palette_cells(palettes_vm::color_cells(p));
    w.set_palette_info(palettes_vm::describe(p).into());
    w.set_palette_can_create(true);
    w.set_palette_can_delete(true);
    w.set_palette_textures(palettes_vm::texture_thumbs_for_palette(s, pal_idx as u32));
}

/// Single source of truth for the Script panel: publishes header, rows,
/// macro prefix, and the editor fields for the currently-selected entry.
/// Preserves the current `selected-entry-index` when still in range.
pub(crate) fn publish_palette_names(w: &MainWindow, ed: &Editor) {
    let names: Vec<SharedString> = match ed.current_sprite() {
        Some(s) => s.palettes.keys().map(|i| format!("Palette {i}").into()).collect(),
        None => vec![],
    };
    w.set_palette_names(std::rc::Rc::new(slint::VecModel::from(names)).into());
}

pub(crate) fn apply_picker_rgb(
    w: &MainWindow,
    editor: &std::rc::Rc<std::cell::RefCell<hm64_sprite_editor::state::editor::Editor>>,
    anims_vm: &std::rc::Rc<std::cell::RefCell<hm64_sprite_editor::ui::animations_vm::AnimationsVm>>,
    pending_tex: &std::rc::Rc<std::cell::RefCell<std::collections::VecDeque<(u32, usize)>>>,
    r: u8, g: u8, b: u8, a: u8,
) {
    use hm64_sprite_editor::model::Rgba;

    // Font-mode: dispatch SetFontPaletteColor and return.
    if w.get_picker_font_mode() {
        let pal_idx = w.get_picker_font_palette_index() as u32;
        let color_idx = w.get_picker_font_color_index() as u8;
        let new = Rgba::new(r, g, b, a);
        let result = {
            let mut ed = editor.borrow_mut();
            if let Some(cmd) =
                hm64_sprite_editor::cmd::font_palette_cmds::SetFontPaletteColor::new(
                    pal_idx, color_idx, new, &ed,
                )
            {
                Some(ed.run(Box::new(cmd)))
            } else {
                None
            }
        };
        if let Some(Err(e)) = result {
            w.set_status_text(format!("Color edit failed: {e}").into());
            return;
        }
        w.set_picker_r(r as i32);
        w.set_picker_g(g as i32);
        w.set_picker_b(b as i32);
        w.set_picker_a(a as i32);
        w.set_picker_hex_text(
            hm64_sprite_editor::ui::color_math::format_hex(r, g, b, a).into(),
        );
        refresh_font_views(&editor.borrow(), w);
        return;
    }

    use hm64_sprite_editor::cmd::palette_cmds::SetPaletteColor;
    let pal_idx = w.get_selected_palette();
    let cell_idx = w.get_palette_selected_cell();
    if pal_idx < 0 || cell_idx < 0 { return; }
    let new_color = Rgba::new(r, g, b, a);
    let result = {
        let mut ed = editor.borrow_mut();
        let Some(cmd) =
            SetPaletteColor::new(pal_idx as u32, cell_idx as u32, new_color, &ed)
        else { return; };
        ed.run(Box::new(cmd))
    };
    if let Err(e) = result {
        w.set_status_text(format!("Color edit failed: {e}").into());
        return;
    }
    anims_vm.borrow_mut().invalidate_cache();
    let ed = editor.borrow();
    publish_palette_cells(w, &ed, pal_idx);
    publish_texture_thumbs(w, &ed);
    // Also refresh the big inspector preview for the currently selected
    // texture — publish_texture_thumbs only redraws the gallery thumbnails,
    // not the preview image, so without this the preview stays on stale
    // palette colors until the user clicks the texture again.
    let sel_tex = w.get_selected_texture();
    if sel_tex >= 0 {
        if let Some(s) = ed.current_sprite() {
            if let Some(tex) = s.textures.get(&(sel_tex as u32)) {
                refresh_texture_inspector(w, s, tex);
            }
        }
    }
    pending_tex.borrow_mut().clear();
    publish_dirty(w, &ed);

    w.set_picker_r(r as i32);
    w.set_picker_g(g as i32);
    w.set_picker_b(b as i32);
    w.set_picker_a(a as i32);
    w.set_picker_hex_text(
        hm64_sprite_editor::ui::color_math::format_hex(r, g, b, a).into(),
    );
}

