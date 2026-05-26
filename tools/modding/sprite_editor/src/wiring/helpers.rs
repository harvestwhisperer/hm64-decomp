//! Small helpers shared across multiple wiring modules.

use hm64_sprite_editor::state::editor::Editor;
use hm64_sprite_editor::ui::slint_generated::MainWindow;

/// Show a transient toast at the bottom of the window. Auto-hides after 4 s.
pub fn toast(window: &slint::Weak<MainWindow>, msg: impl Into<String>) {
    let Some(w) = window.upgrade() else { return };
    w.set_toast_text(msg.into().into());
    w.set_toast_visible(true);
    let window = window.clone();
    slint::Timer::single_shot(std::time::Duration::from_secs(4), move || {
        if let Some(w) = window.upgrade() {
            w.set_toast_visible(false);
        }
    });
}

/// Publish the per-domain dirty bits + undo/redo availability after any
/// command runs.
pub fn publish_dirty(w: &MainWindow, ed: &Editor) {
    w.set_palette_dirty(ed.dirty.palette());
    w.set_texture_dirty(ed.dirty.texture());
    w.set_script_animation_data_dirty(ed.dirty.animation_data());
    w.set_anim_dirty(ed.dirty.animation());
    w.set_can_undo(ed.history.can_undo());
    w.set_can_redo(ed.history.can_redo());
}
