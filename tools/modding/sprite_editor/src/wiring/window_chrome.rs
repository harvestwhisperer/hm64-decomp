//! Window chrome: UI scale persistence + close-with-unsaved-changes guard.
//! Tiny but kept separate so the persistence + window resize logic doesn't
//! have to live in `main.rs`.

use hm64_sprite_editor::state::config;
use hm64_sprite_editor::ui::slint_generated::MainWindow;
use slint::{CloseRequestResponse, ComponentHandle};

use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let window_weak = window.as_weak();
        window.on_ui_scale_changed_by_index(move |idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let presets = config::UI_SCALE_PRESETS;
            let i = (idx as usize).min(presets.len().saturating_sub(1));
            let scale = presets.get(i).copied().unwrap_or(1.0);
            w.set_ui_scale(scale);
            w.set_ui_scale_index(i as i32);
            // Resize the window to match the new preferred size. Slint grows
            // the window when min-width climbs past the current size but
            // doesn't shrink it when min-width drops, so without this an
            // explicit downscale (e.g. 200 % → 75 %) would leave the window
            // huge with empty space. Numbers track the slint preferred-width
            // / preferred-height (1400 × 900 logical at scale 1.0).
            w.window().set_size(slint::LogicalSize::new(1400.0 * scale, 900.0 * scale));
            let mut cfg = config::load();
            cfg.ui_scale = scale;
            let _ = config::save(&cfg);
        });
    }

    // Block the OS window-close when the editor has unsaved changes. Mirrors
    // the discard prompt used when switching sprites (sprite_browser.rs).
    {
        let app = state.clone();
        window.window().on_close_requested(move || {
            if !app.editor.borrow().dirty.any() {
                return CloseRequestResponse::HideWindow;
            }
            let answer = rfd::MessageDialog::new()
                .set_title("Unsaved changes")
                .set_description("There are unsaved changes.\nDiscard and quit?")
                .set_buttons(rfd::MessageButtons::OkCancel)
                .show();
            if answer == rfd::MessageDialogResult::Ok {
                CloseRequestResponse::HideWindow
            } else {
                CloseRequestResponse::KeepWindowShown
            }
        });
    }
}
