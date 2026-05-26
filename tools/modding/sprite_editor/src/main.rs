//! HM64 Sprite Editor — standalone desktop asset editor. `main()` does
//! initial state setup (asset-root discovery, UI scale apply) and then
//! delegates all callback wiring to the per-domain modules under `wiring/`.
//! Any non-trivial logic should live in those modules or in the existing
//! `model/` / `io/` / `cmd/` / `state/` / `ui/` library layers — not here.

mod wiring;

use hm64_sprite_editor::state::config;
use hm64_sprite_editor::ui::slint_generated::MainWindow;
use hm64_sprite_editor::ui::sprite_browser_vm;
use slint::ComponentHandle;

use wiring::font::discover_font_rows;
use wiring::sprite_browser::{append_font_tree_rows, publish_tree_rows};

fn main() -> Result<(), slint::PlatformError> {
    let window = MainWindow::new()?;

    let cfg = config::load();
    let state = wiring::AppState::new();

    let initial_root = cfg
        .asset_root
        .clone()
        .or_else(|| std::env::current_exe().ok().and_then(|p| config::auto_detect_asset_root(&p)));
    if let Some(root) = &initial_root {
        state.editor.borrow_mut().set_asset_root(root.clone());
        window.set_asset_root_loaded(true);
        *state.tree.borrow_mut() = sprite_browser_vm::discover(root);
        *state.expanded.borrow_mut() = match cfg.browser_expanded.as_ref() {
            Some(keys) => keys.iter().filter_map(|s| sprite_browser_vm::NodeKey::decode(s)).collect(),
            None => sprite_browser_vm::default_expanded(&state.tree.borrow()),
        };
        *state.font_rows.borrow_mut() = discover_font_rows(root);
        publish_tree_rows(
            &state.tree.borrow(),
            &state.expanded.borrow(),
            &state.browser_filter.borrow(),
            &state.rows_model,
        );
        append_font_tree_rows(
            &state.font_rows.borrow(),
            &state.rows_model,
            state.font_root_expanded.get(),
        );
    }

    window.set_tree_rows(state.rows_model.clone().into());
    window.set_selected_sprite_index(-1);

    // Apply persisted UI scale. Snap to a known preset so the dropdown's
    // current-index always matches the float value — and so a stale or hand-
    // edited scale value can't leave the UI in a half-applied state.
    {
        let scale = config::nearest_ui_scale_preset(cfg.ui_scale);
        let idx = config::UI_SCALE_PRESETS
            .iter()
            .position(|&v| (v - scale).abs() < f32::EPSILON)
            .unwrap_or(0) as i32;
        window.set_ui_scale(scale);
        window.set_ui_scale_index(idx);
    }

    wiring::window_chrome::install(&window, &state);
    wiring::sprite_browser::install(&window, &state);
    wiring::textures::install(&window, &state);
    wiring::palettes::install(&window, &state);
    wiring::animations::install(&window, &state);
    let _playback_timer = wiring::playback_timer::install(&window, &state);
    wiring::app_actions::install(&window, &state);
    wiring::font::install(&window, &state);
    wiring::script::install(&window, &state);
    wiring::new_sprite_wizard::install(&window, &state);

    window.set_status_text("Ready".into());
    window.run()
}
