//! 60 Hz tick that drives:
//!   * deferred texture-thumbnail rendering (20 per tick to avoid freezing
//!     on first sprite load), and
//!   * animation playback for the gallery, the Animations-tab inspector,
//!     and the Script-tab entry preview.
//!
//! `slint::Timer` stops when its handle is dropped, so `install` returns
//! the `Timer` for the caller to bind for the lifetime of the event loop.

use std::cell::RefCell;
use std::rc::Rc;

use hm64_sprite_editor::ui::slint_generated::{AnimThumb, MainWindow, TextureThumb};
use hm64_sprite_editor::ui::textures_vm;
use slint::{ComponentHandle, Model, Timer, TimerMode};

use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) -> Timer {
    let timer = Timer::default();
    let last = Rc::new(RefCell::new(std::time::Instant::now()));
    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let pending_tex = state.pending_tex.clone();
        let anim_row_map = state.anim_row_map.clone();
        let last = last.clone();
        let window_weak = window.as_weak();
        timer.start(TimerMode::Repeated, std::time::Duration::from_millis(16), move || {
            let Some(w) = window_weak.upgrade() else { return };
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let (now, delta_ms) = hm64_sprite_editor::state::playback::elapsed_ms(*last.borrow());
            *last.borrow_mut() = now;

            // Batch-render up to 20 deferred texture thumbnails per tick so the
            // initial sprite load doesn't freeze the UI for large sprites.
            {
                let mut queue = pending_tex.borrow_mut();
                if !queue.is_empty() {
                    let n = 20.min(queue.len());
                    let batch: Vec<_> = queue.drain(..n).collect();
                    drop(queue); // release borrow before touching UI model
                    let tex_thumbs = w.get_texture_thumbs();
                    if let Some(vm) =
                        tex_thumbs.as_any().downcast_ref::<slint::VecModel<TextureThumb>>()
                    {
                        for (tex_idx, row_idx) in batch {
                            if let Some(tex) = s.textures.get(&tex_idx) {
                                if let Some(mut row) = vm.row_data(row_idx) {
                                    row.image =
                                        textures_vm::render_texture_using_manifest_palette(s, tex);
                                    vm.set_row_data(row_idx, row);
                                }
                            }
                        }
                    }
                }
            }

            let updates = anims_vm.borrow_mut().advance(s, delta_ms);
            // Update animation gallery using O(1) index map instead of linear scan.
            let thumbs_model = w.get_anim_thumbs();
            let model_any = thumbs_model.as_any();
            let Some(vec_model) = model_any.downcast_ref::<slint::VecModel<AnimThumb>>() else {
                return;
            };
            let row_map = anim_row_map.borrow();
            // Script panel preview state.
            let script_can_edit = w.get_script_can_edit_entry();
            let script_anim_idx = w.get_script_editor_offset() as u32;
            let script_h_flip = w.get_script_editor_flip();
            for (idx, img) in updates {
                if let Some(&row_idx) = row_map.get(&idx) {
                    let mut row = vec_model.row_data(row_idx).unwrap();
                    row.image = img.clone();
                    vec_model.set_row_data(row_idx, row);
                    if w.get_selected_anim() == idx as i32 {
                        w.set_anim_detail_image(img.clone());
                        if let Some(a) = anims_vm.borrow().anims.get(&idx) {
                            w.set_anim_detail_frame(a.frame_idx as i32);
                        }
                    }
                }
                // Keep the script panel entry preview in sync with the animation.
                if script_can_edit && idx == script_anim_idx {
                    let preview = if script_h_flip {
                        // Re-render the current frame with horizontal flip
                        // applied, going through the same compose pipeline as
                        // the unflipped path so multi-layer animations stay
                        // composed correctly.
                        anims_vm
                            .borrow_mut()
                            .current_frame_image_h_flipped(s, idx)
                            .unwrap_or_default()
                    } else {
                        img
                    };
                    w.set_script_entry_preview(preview);
                    if let Some(a) = anims_vm.borrow().anims.get(&idx) {
                        w.set_script_preview_frame(a.frame_idx as i32);
                    }
                }
            }
        });
    }
    timer
}
