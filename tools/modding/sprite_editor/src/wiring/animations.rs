//! Animations tab callbacks: gallery selection, playback transport,
//! per-frame edits (duration, audio, layer texture/anchor), structural
//! edits (frame add/remove/move, layer add/remove), and animation
//! lifecycle (new, save, clone, delete, expand-toggle).

use hm64_sprite_editor::state::editor::Editor;
use hm64_sprite_editor::ui::animations_vm::AnimationsVm;
use hm64_sprite_editor::ui::slint_generated::{AnimThumb, FrameRow, MainWindow, PickerRow};
use slint::ComponentHandle;
use slint::Model;

use crate::wiring::helpers::publish_dirty;
use crate::wiring::textures::anim_row_for_index;
use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_clicked(move |idx| {
            let Some(w) = window_weak.upgrade() else { return };
            w.set_selected_anim(idx);
            let ed = editor.borrow();
            w.set_selected_anim_row(anim_row_for_index(ed.current_sprite(), idx));
            let Some(s) = ed.current_sprite() else { return };
            let Some(data) = s.animations.get(&(idx as u32)) else { return };
            w.set_anim_detail_info(
                format!("anim {} · {} frames", data.index, data.frame_count).into(),
            );
            w.set_anim_detail_frame_count(data.frames.len().max(1) as i32);
            {
                let mut vm = anims_vm.borrow_mut();
                let frame_idx = vm.anims.get(&(idx as u32)).map(|a| a.frame_idx).unwrap_or(0);
                w.set_anim_detail_frame(frame_idx as i32);
                if let Some(img) = vm.current_frame_image(s, idx as u32) {
                    w.set_anim_detail_image(img);
                }
                vm.frame_expanded.retain(|(a, _), _| *a != idx as u32);
            }
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_play_pause(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let idx = w.get_selected_anim();
            if idx < 0 {
                return;
            }
            let mut vm = anims_vm.borrow_mut();
            let is_playing = vm.anims.get(&(idx as u32)).map(|a| a.playing).unwrap_or(false);
            vm.set_playing(idx as u32, !is_playing);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_prev_frame(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let idx = w.get_selected_anim();
            if idx < 0 {
                return;
            }
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let mut vm = anims_vm.borrow_mut();
            if let Some(img) = vm.step_frame(s, idx as u32, -1) {
                w.set_anim_detail_image(img);
            }
            if let Some(a) = vm.anims.get(&(idx as u32)) {
                w.set_anim_detail_frame(a.frame_idx as i32);
            }
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_next_frame(move || {
            let Some(w) = window_weak.upgrade() else { return };
            let idx = w.get_selected_anim();
            if idx < 0 {
                return;
            }
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let mut vm = anims_vm.borrow_mut();
            if let Some(img) = vm.step_frame(s, idx as u32, 1) {
                w.set_anim_detail_image(img);
            }
            if let Some(a) = vm.anims.get(&(idx as u32)) {
                w.set_anim_detail_frame(a.frame_idx as i32);
            }
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_seek(move |v| {
            let Some(w) = window_weak.upgrade() else { return };
            let idx = w.get_selected_anim();
            if idx < 0 {
                return;
            }
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let Some(data) = s.animations.get(&(idx as u32)) else { return };
            let frame_count = data.frames.len();
            if frame_count == 0 {
                return;
            }
            let target = (v.round() as i32).clamp(0, frame_count as i32 - 1) as usize;
            let mut vm = anims_vm.borrow_mut();
            vm.set_playing(idx as u32, false);
            vm.seek(idx as u32, target);
            if let Some(img) = vm.current_frame_image(s, idx as u32) {
                w.set_anim_detail_image(img);
            }
            w.set_anim_detail_frame(target as i32);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let anim_row_map = state.anim_row_map.clone();
        let window_weak = window.as_weak();
        window.on_anim_new(move || {
            use hm64_sprite_editor::cmd::animation_cmds::CreateAnimation;
            use hm64_sprite_editor::model::Animation;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let new_idx = match ed.current_sprite() {
                Some(s) => s.animations.keys().max().map(|m| m + 1).unwrap_or(0),
                None => {
                    w.set_status_text("No sprite loaded".into());
                    return;
                }
            };
            let anim = Animation { frame_count: 0, unused: 0, index: new_idx, frames: vec![] };
            let cmd = match CreateAnimation::new(new_idx, anim, &ed) {
                Some(c) => Box::new(c),
                None => {
                    w.set_status_text("Could not create animation".into());
                    return;
                }
            };
            if let Err(e) = ed.run(cmd) {
                w.set_status_text(format!("Create failed: {e}").into());
                return;
            }
            // Update VM playback table + gallery thumb list.
            anims_vm.borrow_mut().anims.insert(
                new_idx,
                hm64_sprite_editor::state::playback::PlayingAnim::new(new_idx),
            );
            let thumbs_model = w.get_anim_thumbs();
            let model_any = thumbs_model.as_any();
            if let Some(vm) = model_any.downcast_ref::<slint::VecModel<AnimThumb>>() {
                let new_row = vm.row_count();
                vm.push(AnimThumb {
                    index: new_idx as i32,
                    label: hm64_sprite_editor::ui::animations_vm::describe_animation(
                        &ed.current_sprite().unwrap().animations[&new_idx],
                    )
                    .into(),
                    image: slint::Image::default(),
                });
                anim_row_map.borrow_mut().insert(new_idx, new_row);
            }
            w.set_selected_anim(new_idx as i32);
            w.set_selected_anim_row(anim_row_for_index(ed.current_sprite(), new_idx as i32));
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
            w.set_status_text(format!("Created animation #{new_idx}").into());
        });
    }

    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_anim_save(move || {
            let plan = crate::wiring::save_all::plan_animation_only(&app.editor.borrow());
            crate::wiring::save_all::start_job(&app, &window_weak, plan, "Animations clean");
        });
    }

    // Field-edit handlers
    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_duration_changed(move |frame_idx, value| {
            use hm64_sprite_editor::cmd::animation_cmds::SetFrameDuration;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let new = (value.max(0) as u32).min(u16::MAX as u32) as u16;
            let Some(cmd) = SetFrameDuration::new(anim_idx as u32, frame_idx as usize, new, &ed)
            else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_audio_toggled(move |frame_idx, on| {
            use hm64_sprite_editor::cmd::animation_cmds::SetFrameHasAudio;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let new: u8 = if on { 1 } else { 0 };
            let Some(cmd) = SetFrameHasAudio::new(anim_idx as u32, frame_idx as usize, new, &ed)
            else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_layer_texture_changed(move |frame_idx, layer_idx, combo_idx| {
            use hm64_sprite_editor::cmd::animation_cmds::SetLayerTexture;
            use hm64_sprite_editor::ui::animations_vm::build_texture_options;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let Some(s) = ed.current_sprite() else { return };
            let (_labels, keys) = build_texture_options(s);
            let Some(&texture_key) = keys.get(combo_idx as usize) else { return };
            let Some(cmd) = SetLayerTexture::new(
                anim_idx as u32,
                frame_idx as usize,
                layer_idx as usize,
                texture_key,
                &ed,
            ) else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_layer_anchor_changed(move |frame_idx, layer_idx, x, y| {
            use hm64_sprite_editor::cmd::animation_cmds::SetLayerAnchor;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let nx = x.clamp(i16::MIN as i32, i16::MAX as i32) as i16;
            let ny = y.clamp(i16::MIN as i32, i16::MAX as i32) as i16;
            let Some(cmd) = SetLayerAnchor::new(
                anim_idx as u32,
                frame_idx as usize,
                layer_idx as usize,
                (nx, ny),
                &ed,
            ) else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    // Structural-edit handlers
    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_add(move |at| {
            use hm64_sprite_editor::cmd::animation_cmds::AddFrame;
            use hm64_sprite_editor::model::AnimationFrame;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let new_frame = AnimationFrame { duration: 2, has_audio: 0, sprites: vec![] };
            let Some(cmd) = AddFrame::new(anim_idx as u32, at as usize, new_frame, &ed) else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_remove(move |at| {
            use hm64_sprite_editor::cmd::animation_cmds::RemoveFrame;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let Some(cmd) = RemoveFrame::new(anim_idx as u32, at as usize, &ed) else { return };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_move(move |from, to| {
            use hm64_sprite_editor::cmd::animation_cmds::MoveFrame;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let Some(cmd) = MoveFrame::new(anim_idx as u32, from as usize, to as usize, &ed) else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_layer_add(move |frame_idx| {
            use hm64_sprite_editor::cmd::animation_cmds::AddLayer;
            use hm64_sprite_editor::model::FrameSprite;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            // Insert at end. Reading the current length requires a borrow.
            let at = match ed.current_sprite() {
                Some(s) => s
                    .animations
                    .get(&(anim_idx as u32))
                    .and_then(|a| a.frames.get(frame_idx as usize))
                    .map(|f| f.sprites.len())
                    .unwrap_or(0),
                None => 0,
            };
            let layer = FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 };
            let Some(cmd) = AddLayer::new(anim_idx as u32, frame_idx as usize, at, layer, &ed)
            else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_layer_remove(move |frame_idx, layer_idx| {
            use hm64_sprite_editor::cmd::animation_cmds::RemoveLayer;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let Some(cmd) = RemoveLayer::new(
                anim_idx as u32,
                frame_idx as usize,
                layer_idx as usize,
                &ed,
            ) else {
                return;
            };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().invalidate_cache();
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }

    // Lifecycle-edit handlers (clone, delete) and frame-expand toggle
    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let anim_row_map = state.anim_row_map.clone();
        let window_weak = window.as_weak();
        window.on_anim_clone(move || {
            use hm64_sprite_editor::cmd::animation_cmds::CloneAnimation;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let new_idx = match ed.current_sprite() {
                Some(s) => s.animations.keys().max().map(|m| m + 1).unwrap_or(0),
                None => return,
            };
            let Some(cmd) = CloneAnimation::new(anim_idx as u32, new_idx, &ed) else { return };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            // Update VM + thumb list.
            anims_vm.borrow_mut().anims.insert(
                new_idx,
                hm64_sprite_editor::state::playback::PlayingAnim::new(new_idx),
            );
            let thumbs_model = w.get_anim_thumbs();
            let model_any = thumbs_model.as_any();
            if let Some(vm) = model_any.downcast_ref::<slint::VecModel<AnimThumb>>() {
                let new_row = vm.row_count();
                vm.push(AnimThumb {
                    index: new_idx as i32,
                    label: hm64_sprite_editor::ui::animations_vm::describe_animation(
                        &ed.current_sprite().unwrap().animations[&new_idx],
                    )
                    .into(),
                    image: slint::Image::default(),
                });
                anim_row_map.borrow_mut().insert(new_idx, new_row);
            }
            w.set_selected_anim(new_idx as i32);
            w.set_selected_anim_row(anim_row_for_index(ed.current_sprite(), new_idx as i32));
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
            w.set_status_text(format!("Cloned to animation #{new_idx}").into());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let anim_row_map = state.anim_row_map.clone();
        let window_weak = window.as_weak();
        window.on_anim_delete(move || {
            use hm64_sprite_editor::cmd::animation_cmds::DeleteAnimation;
            let Some(w) = window_weak.upgrade() else { return };
            let mut ed = editor.borrow_mut();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let Some(cmd) = DeleteAnimation::new(anim_idx as u32, &ed) else { return };
            if ed.run(Box::new(cmd)).is_err() {
                return;
            }
            anims_vm.borrow_mut().anims.remove(&(anim_idx as u32));
            anims_vm.borrow_mut().bounds.remove(&(anim_idx as u32));
            anims_vm.borrow_mut().frame_expanded.retain(|(a, _), _| *a != anim_idx as u32);
            anims_vm.borrow_mut().invalidate_cache();
            // Rebuild the gallery thumb list and row map from scratch — easier
            // than splicing because the BTreeMap order shifts on remove.
            let Some(s) = ed.current_sprite() else { return };
            let thumbs: Vec<AnimThumb> = s
                .animations
                .values()
                .map(|a| AnimThumb {
                    index: a.index as i32,
                    label: hm64_sprite_editor::ui::animations_vm::describe_animation(a).into(),
                    image: slint::Image::default(),
                })
                .collect();
            w.set_anim_thumbs(std::rc::Rc::new(slint::VecModel::from(thumbs)).into());
            *anim_row_map.borrow_mut() =
                s.animations.keys().enumerate().map(|(row, &idx)| (idx, row)).collect();
            // Move selection to the highest remaining index, or -1.
            let new_sel: i32 = s.animations.keys().max().map(|&k| k as i32).unwrap_or(-1);
            w.set_selected_anim(new_sel);
            w.set_selected_anim_row(anim_row_for_index(Some(s), new_sel));
            if new_sel < 0 {
                w.set_anim_detail_image(slint::Image::default());
                w.set_anim_detail_info("".into());
                w.set_anim_detail_frame_count(1);
                w.set_anim_detail_frame(0);
            } else if let Some(data) = s.animations.get(&(new_sel as u32)) {
                w.set_anim_detail_info(
                    format!("anim {} · {} frames", data.index, data.frame_count).into(),
                );
                w.set_anim_detail_frame_count(data.frames.len().max(1) as i32);
                // The image will refresh on the next 60 Hz tick from the
                // playback advance loop within ~16 ms.
            }
            publish_dirty(&w, &ed);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
            w.set_status_text(format!("Deleted animation #{anim_idx}").into());
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_anim_frame_expand_toggled(move |frame_idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let ed = editor.borrow();
            let anim_idx = w.get_selected_anim();
            if anim_idx < 0 {
                return;
            }
            let key = (anim_idx as u32, frame_idx as usize);
            let mut vm = anims_vm.borrow_mut();
            let cur = vm.frame_expanded.get(&key).copied().unwrap_or(false);
            vm.frame_expanded.insert(key, !cur);
            drop(vm);
            rebuild_anim_editor_rows(&w, &ed, &anims_vm.borrow());
        });
    }
}

// ---------------------------------------------------------------------------
// Helpers (relocated from main.rs)
// ---------------------------------------------------------------------------

pub(crate) fn rebuild_anim_editor_rows(
    w: &MainWindow,
    ed: &Editor,
    anims_vm: &AnimationsVm,
) {
    const PICKER_COLUMNS: usize = 4;
    let selected = w.get_selected_anim();
    let Some(s) = ed.current_sprite() else {
        w.set_anim_editor_frames(std::rc::Rc::new(slint::VecModel::<FrameRow>::default()).into());
        w.set_anim_texture_picker_rows(
            std::rc::Rc::new(slint::VecModel::<PickerRow>::default()).into(),
        );
        return;
    };
    let (_labels, keys) = hm64_sprite_editor::ui::animations_vm::build_texture_options(s);
    w.set_anim_texture_picker_rows(hm64_sprite_editor::ui::animations_vm::texture_picker_rows(
        s,
        PICKER_COLUMNS,
    ));

    if selected < 0 {
        w.set_anim_editor_frames(std::rc::Rc::new(slint::VecModel::<FrameRow>::default()).into());
        return;
    }
    let anim_idx = selected as u32;
    w.set_anim_editor_frames(hm64_sprite_editor::ui::animations_vm::editor_frame_rows(
        s,
        anim_idx,
        &anims_vm.frame_expanded,
        &keys,
    ));
}

