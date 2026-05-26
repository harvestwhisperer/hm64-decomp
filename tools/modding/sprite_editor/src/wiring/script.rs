//! Script tab callbacks: entry selection (with arrow-key navigation), entry
//! preview playback transport, the structural editing actions (add / remove /
//! clone / move), per-field commits (offset / type / flip / label /
//! character / expression / directional), save, and inc.c / .h export.

use hm64_sprite_editor::state::editor::Editor;
use hm64_sprite_editor::ui::animations_vm::AnimationsVm;
use hm64_sprite_editor::ui::script_vm;
use hm64_sprite_editor::ui::slint_generated::{MainWindow, ScriptEntryRow};
use slint::{ComponentHandle, Model, SharedString};

use crate::wiring::helpers::publish_dirty;
use crate::wiring::AppState;

pub fn install(window: &MainWindow, state: &AppState) {
    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_script_entry_clicked(move |idx| {
            let Some(w) = window_weak.upgrade() else { return };
            let ed = editor.borrow();
            let entry_count = animation_data_entry_count(&ed) as i32;
            if idx < 0 || idx >= entry_count {
                w.set_script_selected_entry(-1);
                w.set_script_can_edit_entry(false);
                return;
            }
            w.set_script_selected_entry(idx);
            publish_selection_editor_fields(&w, &ed, idx);
            // Sync the preview slider with the shared playback state so it
            // doesn't briefly read 0 while waiting for the next 60 Hz tick.
            let offset = w.get_script_editor_offset() as u32;
            if let Some(a) = anims_vm.borrow().anims.get(&offset) {
                w.set_script_preview_frame(a.frame_idx as i32);
            } else {
                w.set_script_preview_frame(0);
            }
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_selected(move |idx| {
            let Some(w) = window_weak.upgrade() else { return };
            if idx < 0 {
                return;
            }
            {
                let mut ed = editor.borrow_mut();
                ed.current_entity_script_idx = idx as u32;
                // Group-collapse state is keyed by entry-index alone, so it
                // doesn't transfer meaningfully between scripts.
                ed.collapsed_groups.clear();
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_group_toggle(move |base_entry_index| {
            let Some(w) = window_weak.upgrade() else { return };
            if base_entry_index < 0 {
                return;
            }
            {
                let mut ed = editor.borrow_mut();
                let key = base_entry_index as u32;
                if !ed.collapsed_groups.remove(&key) {
                    ed.collapsed_groups.insert(key);
                }
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
        });
    }

    // Script-tab entry preview playback controls. They drive the same
    // `anims_vm` state as the Animations tab; the entry's metadata_offset is
    // the animation index.
    {
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_script_preview_play_pause(move || {
            let Some(w) = window_weak.upgrade() else { return };
            if !w.get_script_can_edit_entry() {
                return;
            }
            let idx = w.get_script_editor_offset() as u32;
            let mut vm = anims_vm.borrow_mut();
            let is_playing = vm.anims.get(&idx).map(|a| a.playing).unwrap_or(false);
            vm.set_playing(idx, !is_playing);
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_script_preview_prev_frame(move || {
            let Some(w) = window_weak.upgrade() else { return };
            if !w.get_script_can_edit_entry() {
                return;
            }
            let idx = w.get_script_editor_offset() as u32;
            let h_flip = w.get_script_editor_flip();
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let mut vm = anims_vm.borrow_mut();
            if vm.step_frame(s, idx, -1).is_some() {
                if let Some(img) = render_script_preview_for(s, &mut vm, idx, h_flip) {
                    w.set_script_entry_preview(img);
                }
            }
            if let Some(a) = vm.anims.get(&idx) {
                w.set_script_preview_frame(a.frame_idx as i32);
            }
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_script_preview_next_frame(move || {
            let Some(w) = window_weak.upgrade() else { return };
            if !w.get_script_can_edit_entry() {
                return;
            }
            let idx = w.get_script_editor_offset() as u32;
            let h_flip = w.get_script_editor_flip();
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let mut vm = anims_vm.borrow_mut();
            if vm.step_frame(s, idx, 1).is_some() {
                if let Some(img) = render_script_preview_for(s, &mut vm, idx, h_flip) {
                    w.set_script_entry_preview(img);
                }
            }
            if let Some(a) = vm.anims.get(&idx) {
                w.set_script_preview_frame(a.frame_idx as i32);
            }
        });
    }

    {
        let editor = state.editor.clone();
        let anims_vm = state.anims_vm.clone();
        let window_weak = window.as_weak();
        window.on_script_preview_frame_seek(move |v| {
            let Some(w) = window_weak.upgrade() else { return };
            if !w.get_script_can_edit_entry() {
                return;
            }
            let idx = w.get_script_editor_offset() as u32;
            let h_flip = w.get_script_editor_flip();
            let ed = editor.borrow();
            let Some(s) = ed.current_sprite() else { return };
            let Some(data) = s.animations.get(&idx) else { return };
            let frame_count = data.frames.len();
            if frame_count == 0 {
                return;
            }
            let target = (v.round() as i32).clamp(0, frame_count as i32 - 1) as usize;
            let mut vm = anims_vm.borrow_mut();
            vm.set_playing(idx, false);
            vm.seek(idx, target);
            if let Some(img) = render_script_preview_for(s, &mut vm, idx, h_flip) {
                w.set_script_entry_preview(img);
            }
            w.set_script_preview_frame(target as i32);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_import(move || {
            use hm64_sprite_editor::io::inc_c_parser::parse_inc_c;
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let path = rfd::FileDialog::new()
                .add_filter("Animation script", &["c"])
                .set_title("Import animation script (.inc.c)")
                .pick_file();
            let Some(path) = path else { return };
            let doc = match parse_inc_c(&path) {
                Ok(d) => d,
                Err(e) => {
                    w.set_status_text(format!("Import failed: {e}").into());
                    return;
                }
            };
            let mut ed = editor.borrow_mut();
            ed.loaded_animation_data = Some(AnimationData::Entity { scripts: vec![doc] });
            ed.current_entity_script_idx = 0;
            drop(ed);
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            w.set_status_text("Script imported".into());
        });
    }

    // Editing callbacks. Each callback dispatches by `loaded_animation_data`
    // variant; commands set `DirtyMask::ANIMATION_DATA` and merge across rapid
    // edits to a single (script_idx, entry_idx, field) target.
    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_add_entry(move || {
            use hm64_sprite_editor::cmd::animation_data::{
                AddAvatarEntry, AddEntityAnimation, AddStandaloneLabel,
            };
            use hm64_sprite_editor::model::{
                AnimationData, AvatarEntry, EntityAnimation, JsonAnimType, StandaloneLabel,
            };
            let Some(w) = window_weak.upgrade() else { return };
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else {
                    drop(ed);
                    w.set_status_text("No script loaded".into());
                    return;
                };
                let sel = w.get_script_selected_entry();
                match data {
                    AnimationData::Entity { scripts } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let len = scripts.get(script_idx).map(|d| d.animations.len()).unwrap_or(0);
                        let at = if sel >= 0 && (sel as usize) < len {
                            (sel as usize) + 1
                        } else {
                            len
                        };
                        let entry = EntityAnimation {
                            label: String::new(),
                            metadata_index: 0,
                            anim_type: JsonAnimType::Loop,
                            flip: false,
                            directional: false,
                        };
                        let Some(cmd) = AddEntityAnimation::new(script_idx, at, entry, &ed) else {
                            drop(ed);
                            w.set_status_text("Add entry: out of range".into());
                            return;
                        };
                        let r = ed.run(Box::new(cmd));
                        Some((r, at as i32))
                    }
                    AnimationData::Standalone(s) => {
                        let len = s.labels.len();
                        let at = if sel >= 0 && (sel as usize) < len {
                            (sel as usize) + 1
                        } else {
                            len
                        };
                        let entry = StandaloneLabel { metadata_index: 0, label: String::new() };
                        let Some(cmd) = AddStandaloneLabel::new(at, entry, &ed) else {
                            drop(ed);
                            w.set_status_text("Add entry: out of range".into());
                            return;
                        };
                        let r = ed.run(Box::new(cmd));
                        Some((r, at as i32))
                    }
                    AnimationData::Avatar(t) => {
                        let len = t.entries.len();
                        let at = if sel >= 0 && (sel as usize) < len {
                            (sel as usize) + 1
                        } else {
                            len
                        };
                        let entry = AvatarEntry {
                            character: String::new(),
                            expression: String::new(),
                            metadata_index: 0,
                        };
                        let Some(cmd) = AddAvatarEntry::new(at, entry, &ed) else {
                            drop(ed);
                            w.set_status_text("Add entry: out of range".into());
                            return;
                        };
                        let r = ed.run(Box::new(cmd));
                        Some((r, at as i32))
                    }
                }
            };
            let Some((result, new_idx)) = result else { return };
            if let Err(e) = result {
                w.set_status_text(format!("Add entry failed: {e}").into());
                return;
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            // Select the new row.
            w.set_script_selected_entry(new_idx);
            publish_selection_editor_fields(&w, &ed, new_idx);
            w.set_status_text("Entry added".into());
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_remove_entry(move || {
            use hm64_sprite_editor::cmd::animation_data::{
                RemoveAvatarEntry, RemoveEntityAnimation, RemoveStandaloneLabel,
            };
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) = RemoveEntityAnimation::new(script_idx, sel as usize, &ed)
                        else {
                            drop(ed);
                            w.set_status_text("Remove entry: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Standalone(_) => {
                        let Some(cmd) = RemoveStandaloneLabel::new(sel as usize, &ed) else {
                            drop(ed);
                            w.set_status_text("Remove entry: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Avatar(_) => {
                        let Some(cmd) = RemoveAvatarEntry::new(sel as usize, &ed) else {
                            drop(ed);
                            w.set_status_text("Remove entry: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Remove entry failed: {e}").into());
                return;
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            // Selection now invalid — clear it.
            w.set_script_selected_entry(-1);
            w.set_script_can_edit_entry(false);
            w.set_status_text("Entry removed".into());
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_clone_entry(move || {
            use hm64_sprite_editor::cmd::animation_data::{CloneAvatarEntry, CloneEntityAnimation};
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) = CloneEntityAnimation::new(script_idx, sel as usize, &ed)
                        else {
                            drop(ed);
                            w.set_status_text("Clone entry: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Avatar(_) => {
                        let Some(cmd) = CloneAvatarEntry::new(sel as usize, &ed) else {
                            drop(ed);
                            w.set_status_text("Clone entry: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Standalone(_) => {
                        drop(ed);
                        w.set_status_text("Use Add for new standalone labels".into());
                        return;
                    }
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Clone entry failed: {e}").into());
                return;
            }
            let new_idx = sel + 1;
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            w.set_script_selected_entry(new_idx);
            publish_selection_editor_fields(&w, &ed, new_idx);
            w.set_status_text("Entry cloned".into());
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_move_up(move || {
            use hm64_sprite_editor::cmd::animation_data::{MoveAvatarEntry, MoveEntityAnimation};
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel <= 0 {
                return;
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) = MoveEntityAnimation::new(
                            script_idx,
                            sel as usize,
                            (sel - 1) as usize,
                            &ed,
                        ) else {
                            drop(ed);
                            w.set_status_text("Move up: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Avatar(_) => {
                        let Some(cmd) = MoveAvatarEntry::new(
                            sel as usize,
                            (sel - 1) as usize,
                            &ed,
                        ) else {
                            drop(ed);
                            w.set_status_text("Move up: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    _ => {
                        drop(ed);
                        w.set_status_text("Move not supported for this kind".into());
                        return;
                    }
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Move up failed: {e}").into());
                return;
            }
            let new_idx = sel - 1;
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            w.set_script_selected_entry(new_idx);
            publish_selection_editor_fields(&w, &ed, new_idx);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_move_down(move || {
            use hm64_sprite_editor::cmd::animation_data::{MoveAvatarEntry, MoveEntityAnimation};
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { scripts } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let len = scripts.get(script_idx).map(|d| d.animations.len()).unwrap_or(0);
                        if (sel as usize) + 1 >= len {
                            return;
                        }
                        let Some(cmd) = MoveEntityAnimation::new(
                            script_idx,
                            sel as usize,
                            (sel + 1) as usize,
                            &ed,
                        ) else {
                            drop(ed);
                            w.set_status_text("Move down: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Avatar(t) => {
                        let len = t.entries.len();
                        if (sel as usize) + 1 >= len {
                            return;
                        }
                        let Some(cmd) = MoveAvatarEntry::new(
                            sel as usize,
                            (sel + 1) as usize,
                            &ed,
                        ) else {
                            drop(ed);
                            w.set_status_text("Move down: out of range".into());
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    _ => {
                        drop(ed);
                        w.set_status_text("Move not supported for this kind".into());
                        return;
                    }
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Move down failed: {e}").into());
                return;
            }
            let new_idx = sel + 1;
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            w.set_script_selected_entry(new_idx);
            publish_selection_editor_fields(&w, &ed, new_idx);
        });
    }

    {
        let app = state.clone();
        let window_weak = window.as_weak();
        window.on_script_save(move || {
            let plan = crate::wiring::save_all::plan_script_only(&app.editor.borrow());
            crate::wiring::save_all::start_job(&app, &window_weak, plan, "Script clean");
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_offset(move |v| {
            use hm64_sprite_editor::cmd::animation_data::{
                SetAvatarOffset, SetEntityAnimationOffset, SetStandaloneOffset,
            };
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let new_value = v.clamp(0, u16::MAX as i32) as u16;
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) = SetEntityAnimationOffset::new(
                            script_idx,
                            sel as usize,
                            new_value,
                            &ed,
                        ) else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Avatar(_) => {
                        let Some(cmd) = SetAvatarOffset::new(sel as usize, new_value, &ed) else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Standalone(_) => {
                        let Some(cmd) = SetStandaloneOffset::new(sel as usize, new_value, &ed)
                        else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Edit offset failed: {e}").into());
                return;
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            publish_selection_editor_fields(&w, &ed, sel);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_anim_type(move |v| {
            use hm64_sprite_editor::cmd::animation_data::SetEntityAnimationType;
            use hm64_sprite_editor::model::{AnimationData, JsonAnimType};
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let new_type = match v {
                0 => JsonAnimType::OneShot,
                1 => JsonAnimType::Loop,
                2 => JsonAnimType::DestroyOnEnd,
                _ => return,
            };
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) = SetEntityAnimationType::new(
                            script_idx,
                            sel as usize,
                            new_type,
                            &ed,
                        ) else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    _ => return,
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Edit type failed: {e}").into());
                return;
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            publish_selection_editor_fields(&w, &ed, sel);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_flip(move |v| {
            use hm64_sprite_editor::cmd::animation_data::SetEntityAnimationFlip;
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) =
                            SetEntityAnimationFlip::new(script_idx, sel as usize, v, &ed)
                        else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    _ => return,
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Edit flip failed: {e}").into());
                return;
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            publish_selection_editor_fields(&w, &ed, sel);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_label(move |v| {
            use hm64_sprite_editor::cmd::animation_data::{
                SetEntityAnimationLabel, SetStandaloneLabel,
            };
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let new_value = v.to_string();
            let new_value_for_patch = new_value.clone();
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Entity { .. } => {
                        let script_idx = ed.current_entity_script_idx as usize;
                        let Some(cmd) = SetEntityAnimationLabel::new(
                            script_idx,
                            sel as usize,
                            new_value,
                            &ed,
                        ) else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Standalone(_) => {
                        let Some(cmd) = SetStandaloneLabel::new(sel as usize, new_value, &ed)
                        else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    AnimationData::Avatar(_) => return,
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Edit label failed: {e}").into());
                return;
            }
            // In-place row patch only — calling publish_script_panel /
            // publish_selection_editor_fields here disrupts the LineEdit's
            // focus and restarts the entry preview on every keystroke.
            let ed = editor.borrow();
            patch_script_row_label_in_place(&w, &ed, sel as usize, &new_value_for_patch);
            publish_dirty(&w, &ed);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_character(move |v| {
            use hm64_sprite_editor::cmd::animation_data::SetAvatarCharacter;
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let new_value = v.to_string();
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Avatar(_) => {
                        let Some(cmd) = SetAvatarCharacter::new(sel as usize, new_value, &ed)
                        else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    _ => return,
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Edit character failed: {e}").into());
                return;
            }
            // See commit-label: avoid publish_script_panel /
            // publish_selection_editor_fields per keystroke.
            let ed = editor.borrow();
            patch_script_row_avatar_in_place(&w, &ed, sel as usize);
            publish_dirty(&w, &ed);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_expression(move |v| {
            use hm64_sprite_editor::cmd::animation_data::SetAvatarExpression;
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let new_value = v.to_string();
            let result = {
                let mut ed = editor.borrow_mut();
                let Some(data) = ed.loaded_animation_data.as_ref() else { return };
                match data {
                    AnimationData::Avatar(_) => {
                        let Some(cmd) = SetAvatarExpression::new(sel as usize, new_value, &ed)
                        else {
                            return;
                        };
                        ed.run(Box::new(cmd))
                    }
                    _ => return,
                }
            };
            if let Err(e) = result {
                w.set_status_text(format!("Edit expression failed: {e}").into());
                return;
            }
            // See commit-label: avoid publish_script_panel /
            // publish_selection_editor_fields per keystroke.
            let ed = editor.borrow();
            patch_script_row_avatar_in_place(&w, &ed, sel as usize);
            publish_dirty(&w, &ed);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_commit_directional(move |v| {
            use hm64_sprite_editor::cmd::animation_data::ToggleEntityDirectionalGroup;
            let Some(w) = window_weak.upgrade() else { return };
            let sel = w.get_script_selected_entry();
            if sel < 0 {
                return;
            }
            let result = {
                let mut ed = editor.borrow_mut();
                let script_idx = ed.current_entity_script_idx as usize;
                let Some(cmd) =
                    ToggleEntityDirectionalGroup::new(script_idx, sel as usize, v, &ed)
                else {
                    drop(ed);
                    w.set_status_text(
                        if v {
                            "Directional toggle: need 8 free entries from this position"
                        } else {
                            "Directional toggle: entry is not part of a group"
                        }
                        .into(),
                    );
                    return;
                };
                ed.run(Box::new(cmd))
            };
            if let Err(e) = result {
                w.set_status_text(format!("Directional toggle failed: {e}").into());
                return;
            }
            let ed = editor.borrow();
            publish_script_panel(&w, &ed);
            publish_dirty(&w, &ed);
            publish_selection_editor_fields(&w, &ed, sel);
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_export_inc_c(move || {
            use hm64_sprite_editor::export::avatar_inc_c_export::write_avatar_inc_c;
            use hm64_sprite_editor::export::inc_c_export::write_inc_c;
            use hm64_sprite_editor::model::AnimationData;
            let Some(w) = window_weak.upgrade() else { return };

            // Snapshot the data + sprite_dir once. The avatar and entity
            // branches each get the slice they need to write without holding
            // the editor borrow across the file dialog.
            enum ExportPayload {
                Entity {
                    sprite_dir: Option<std::path::PathBuf>,
                    doc: hm64_sprite_editor::model::EntityScriptDoc,
                },
                Avatar {
                    sprite_dir: Option<std::path::PathBuf>,
                    table: hm64_sprite_editor::model::AvatarTable,
                },
            }
            let payload = {
                let ed = editor.borrow();
                let Some(data) = ed.loaded_animation_data.as_ref() else {
                    drop(ed);
                    w.set_status_text("Export inc.c: no script loaded".into());
                    return;
                };
                let sprite_dir = ed.current_sprite().map(|s| s.dir.clone());
                match data {
                    AnimationData::Entity { scripts } => {
                        let idx = ed.current_entity_script_idx as usize;
                        let Some(doc) = scripts.get(idx) else {
                            drop(ed);
                            w.set_status_text("Export inc.c: script index out of range".into());
                            return;
                        };
                        ExportPayload::Entity { sprite_dir, doc: doc.clone() }
                    }
                    AnimationData::Avatar(t) => {
                        ExportPayload::Avatar { sprite_dir, table: t.clone() }
                    }
                    AnimationData::Standalone(_) => {
                        drop(ed);
                        w.set_status_text(
                            "Export inc.c: standalone sprites have no inc.c".into(),
                        );
                        return;
                    }
                }
            };

            match payload {
                ExportPayload::Entity { sprite_dir, doc } => {
                    let default_name = if doc.name.is_empty() {
                        "script.inc.c".to_string()
                    } else {
                        format!("{}.inc.c", doc.name)
                    };
                    let mut dialog = rfd::FileDialog::new()
                        .add_filter("C source", &["c"])
                        .set_title("Export animation script (.inc.c)")
                        .set_file_name(&default_name);
                    if let Some(dir) = sprite_dir.as_ref() {
                        dialog = dialog.set_directory(dir);
                    }
                    let Some(path) = dialog.save_file() else { return };
                    if let Err(e) = write_inc_c(&path, &doc) {
                        w.set_status_text(format!("Export inc.c failed: {e}").into());
                        return;
                    }
                    w.set_status_text("Exported inc.c".into());
                }
                ExportPayload::Avatar { sprite_dir, table } => {
                    let mut dialog = rfd::FileDialog::new()
                        .add_filter("C source", &["c"])
                        .set_title("Export character avatars (.inc.c)")
                        .set_file_name("characterAvatars.inc.c");
                    if let Some(dir) = sprite_dir.as_ref() {
                        dialog = dialog.set_directory(dir);
                    }
                    let Some(path) = dialog.save_file() else { return };
                    if let Err(e) = write_avatar_inc_c(&path, &table) {
                        w.set_status_text(format!("Export inc.c failed: {e}").into());
                        return;
                    }
                    w.set_status_text("Exported characterAvatars.inc.c".into());
                }
            }
        });
    }

    {
        let editor = state.editor.clone();
        let window_weak = window.as_weak();
        window.on_script_export_h(move || {
            use hm64_sprite_editor::export::macros_h_export::write_macros_h;
            let Some(w) = window_weak.upgrade() else { return };
            let (sprite_dir, sprite_label, data) = {
                let ed = editor.borrow();
                let Some(data) = ed.loaded_animation_data.as_ref().cloned() else {
                    drop(ed);
                    w.set_status_text("Export .h: no script loaded".into());
                    return;
                };
                let (sprite_dir, sprite_label) = match ed.current_sprite() {
                    Some(s) => (Some(s.dir.clone()), s.manifest.label.clone()),
                    None => (None, String::new()),
                };
                (sprite_dir, sprite_label, data)
            };
            let default_name = if sprite_label.is_empty() {
                "macros.h".to_string()
            } else {
                format!("{sprite_label}.h")
            };
            let mut dialog = rfd::FileDialog::new()
                .add_filter("Header", &["h"])
                .set_title("Export macro header (.h)")
                .set_file_name(&default_name);
            if let Some(dir) = sprite_dir.as_ref() {
                dialog = dialog.set_directory(dir);
            }
            let Some(path) = dialog.save_file() else { return };
            if let Err(e) = write_macros_h(&path, &data, &sprite_label) {
                w.set_status_text(format!("Export .h failed: {e}").into());
                return;
            }
            w.set_status_text("Exported .h".into());
        });
    }

    {
        let window_weak = window.as_weak();
        window.on_script_arrow(move |dir| {
            let Some(w) = window_weak.upgrade() else { return };
            let rows = w.get_script_rows();
            let n = rows.row_count();
            if n == 0 {
                return;
            }
            let step = dir.signum();
            if step == 0 {
                return;
            }
            let cur_entry = w.get_script_selected_entry();
            // Find the row position of the currently-selected entry. Group-header
            // rows share their entry-index with the first entry below them, so we
            // explicitly skip them when locating the current row.
            let cur_row: i32 = if cur_entry < 0 {
                if step > 0 {
                    -1
                } else {
                    n as i32
                }
            } else {
                let mut found = -1i32;
                for i in 0..n {
                    if let Some(row) = rows.row_data(i) {
                        if !row.is_group_header && row.entry_index == cur_entry {
                            found = i as i32;
                            break;
                        }
                    }
                }
                found
            };
            // Step in row-position space, skipping group-header rows; clamp at boundaries.
            let mut pos = cur_row + step;
            while pos >= 0 && (pos as usize) < n {
                if let Some(row) = rows.row_data(pos as usize) {
                    if !row.is_group_header {
                        w.invoke_script_entry_clicked(row.entry_index);
                        return;
                    }
                }
                pos += step;
            }
            // Boundary — stay put.
        });
    }
}

// ---------------------------------------------------------------------------
// Helpers (relocated from main.rs)
// ---------------------------------------------------------------------------

pub(crate) fn publish_script_panel(w: &MainWindow, ed: &Editor) {
    use hm64_sprite_editor::model::AnimationData;

    // Single source of truth: `loaded_animation_data`. Inc.c imports populate
    // the same slot (wrapped as an entity script), so there's no separate
    // inc.c-flow branch anymore.

    if let Some(data) = ed.loaded_animation_data.as_ref() {
        let kind = script_vm::display_kind_for_animation_data(data);
        let options = script_vm::script_options_for_animation_data(data);
        let selected_idx = match data {
            AnimationData::Entity { scripts } => {
                let idx = ed.current_entity_script_idx as usize;
                if idx < scripts.len() { idx } else { 0 }
            }
            _ => 0,
        };
        let header = script_vm::header_for_animation_data(data, selected_idx);
        let rows: Vec<ScriptEntryRow> =
            script_vm::rows_from_animation_data(data, selected_idx, &ed.collapsed_groups)
                .into_iter()
                .map(ScriptEntryRow::from)
                .collect();
        let opts: Vec<SharedString> = options.into_iter().map(SharedString::from).collect();

        w.set_script_header(header.into());
        // Mutate the existing rows VecModel in place when possible. Replacing
        // the Rc<VecModel> resets the ListView's scroll position; in-place
        // updates (set_row_data / push / remove) preserve it, which matters
        // for actions like group expand/collapse and per-keystroke label
        // edits where the user expects to stay where they were looking.
        publish_script_rows_in_place(w, rows);
        w.set_script_options(std::rc::Rc::new(slint::VecModel::from(opts)).into());
        w.set_script_selected_script_index(selected_idx as i32);
        w.set_script_display_kind(kind);
        w.set_script_show_type_flip(kind == script_vm::DISPLAY_KIND_ENTITY);
        w.set_script_editing_allowed(true);
        w.set_script_allow_add_remove(true);

        let sel = w.get_script_selected_entry();
        let entry_count = animation_data_entry_count(ed) as i32;
        if sel >= 0 && sel < entry_count {
            publish_selection_editor_fields(w, ed, sel);
        } else {
            w.set_script_selected_entry(-1);
            w.set_script_can_edit_entry(false);
            w.set_script_editor_offset(0);
            w.set_script_editor_anim_type(0);
            w.set_script_editor_flip(false);
            w.set_script_editor_label("".into());
            w.set_script_editor_character("".into());
            w.set_script_editor_expression("".into());
            w.set_script_editor_directional(false);
            w.set_script_editor_directional_allowed(false);
            w.set_script_entry_preview(slint::Image::default());
        }
        return;
    }

    // Nothing loaded.
    w.set_script_header("(no script)".into());
    w.set_script_rows(std::rc::Rc::new(slint::VecModel::<ScriptEntryRow>::default()).into());
    w.set_script_options(std::rc::Rc::new(slint::VecModel::<SharedString>::default()).into());
    w.set_script_selected_script_index(0);
    w.set_script_display_kind(script_vm::DISPLAY_KIND_NONE);
    w.set_script_show_type_flip(true);
    w.set_script_editing_allowed(false);
    w.set_script_allow_add_remove(true);
    w.set_script_can_edit_entry(false);
    w.set_script_selected_entry(-1);
    w.set_script_editor_offset(0);
    w.set_script_editor_anim_type(0);
    w.set_script_editor_flip(false);
    w.set_script_editor_label("".into());
    w.set_script_editor_character("".into());
    w.set_script_editor_expression("".into());
    w.set_script_editor_directional(false);
    w.set_script_editor_directional_allowed(false);
}

/// Number of selectable entries for the currently-loaded animation data.
/// 0 if nothing is loaded.
pub(crate) fn animation_data_entry_count(ed: &Editor) -> usize {
    use hm64_sprite_editor::model::AnimationData;
    match ed.loaded_animation_data.as_ref() {
        Some(AnimationData::Entity { scripts }) => scripts
            .get(ed.current_entity_script_idx as usize)
            .map(|d| d.animations.len())
            .unwrap_or(0),
        Some(AnimationData::Avatar(t)) => t.entries.len(),
        Some(AnimationData::Standalone(s)) => s.labels.len(),
        None => 0,
    }
}

/// Publish the editor-* fields from `entry_idx` (assumed to be in range).
pub(crate) fn publish_selection_editor_fields(w: &MainWindow, ed: &Editor, entry_idx: i32) {
    use hm64_sprite_editor::model::AnimationData;
    let Some(data) = ed.loaded_animation_data.as_ref() else {
        return;
    };
    let idx = entry_idx as usize;
    match data {
        AnimationData::Entity { scripts } => {
            let Some(doc) = scripts.get(ed.current_entity_script_idx as usize) else { return };
            let Some(anim) = doc.animations.get(idx) else { return };
            w.set_script_can_edit_entry(true);
            w.set_script_editor_offset(anim.metadata_index as i32);
            w.set_script_editor_anim_type(json_anim_type_to_combo_idx(anim.anim_type));
            w.set_script_editor_flip(anim.flip);
            // When the selected entry belongs to a directional group, the
            // editor edits the group's *single* label (taken from the base
            // entry); committing rewrites all 8 entries.
            let label = match hm64_sprite_editor::model::directional_group_base(doc, idx) {
                Some(base) => doc.animations[base].label.clone(),
                None => anim.label.clone(),
            };
            w.set_script_editor_label(label.into());
            w.set_script_editor_character("".into());
            w.set_script_editor_expression("".into());
            w.set_script_editor_directional(anim.directional);
            // The Directional checkbox is enabled when toggling it in the
            // direction implied by the current state would be valid:
            //   - currently grouped → allow disband
            //   - currently free    → allow forming a fresh group from here
            let allowed = if anim.directional {
                hm64_sprite_editor::model::directional_group_base(doc, idx).is_some()
            } else {
                hm64_sprite_editor::model::can_form_directional_group(doc, idx)
            };
            w.set_script_editor_directional_allowed(allowed);
            // Preview pipeline is unchanged from 2a; re-publish it.
            republish_entry_preview(w, ed, anim.metadata_index, anim.flip);
            return;
        }
        AnimationData::Avatar(t) => {
            let Some(e) = t.entries.get(idx) else { return };
            w.set_script_can_edit_entry(true);
            w.set_script_editor_offset(e.metadata_index as i32);
            w.set_script_editor_anim_type(0);
            w.set_script_editor_flip(false);
            w.set_script_editor_label("".into());
            w.set_script_editor_character(e.character.clone().into());
            w.set_script_editor_expression(e.expression.clone().into());
            w.set_script_editor_directional(false);
            w.set_script_editor_directional_allowed(false);
            republish_entry_preview(w, ed, e.metadata_index, false);
            return;
        }
        AnimationData::Standalone(s) => {
            let Some(l) = s.labels.get(idx) else { return };
            w.set_script_can_edit_entry(true);
            w.set_script_editor_offset(l.metadata_index as i32);
            w.set_script_editor_anim_type(0);
            w.set_script_editor_flip(false);
            w.set_script_editor_label(l.label.clone().into());
            w.set_script_editor_character("".into());
            w.set_script_editor_expression("".into());
            w.set_script_editor_directional(false);
            w.set_script_editor_directional_allowed(false);
            republish_entry_preview(w, ed, l.metadata_index, false);
            return;
        }
    }
}

/// In-place label patch for the script-tab entry list, used by the three
/// per-keystroke text-edit handlers (label / character / expression). Skips
/// the full `publish_script_panel` + `publish_selection_editor_fields` pair
/// so the in-progress LineEdit keeps its focus and the entry preview
/// doesn't restart on every character.
///
/// For directional groups, only the group header row carries the label
/// (child rows show direction suffixes from `DIRECTION_NAMES`), so the
/// patch targets the header at `entry_index == base`.
pub(crate) fn patch_script_row_label_in_place(w: &MainWindow, ed: &Editor, sel: usize, new_label: &str) {
    use hm64_sprite_editor::model::{directional_group_base, AnimationData};
    let Some(data) = ed.loaded_animation_data.as_ref() else { return };
    let (target_entry_index, want_header) = match data {
        AnimationData::Entity { scripts } => {
            let Some(doc) = scripts.get(ed.current_entity_script_idx as usize) else { return };
            match directional_group_base(doc, sel) {
                Some(base) => (base as i32, true),
                None => (sel as i32, false),
            }
        }
        AnimationData::Standalone(_) => (sel as i32, false),
        AnimationData::Avatar(_) => return,
    };
    let rows_rc = w.get_script_rows();
    let Some(rows) =
        rows_rc.as_any().downcast_ref::<slint::VecModel<ScriptEntryRow>>()
    else {
        return;
    };
    let n = rows.row_count();
    for i in 0..n {
        let Some(mut row) = rows.row_data(i) else { continue };
        if row.is_break {
            continue;
        }
        if row.entry_index == target_entry_index && row.is_group_header == want_header {
            row.label = SharedString::from(new_label);
            rows.set_row_data(i, row);
            break;
        }
    }
}

/// Replace the script-tab `ListView`'s row data without changing the
/// `Rc<VecModel>` identity. Slint's `ListView` resets its scroll position
/// whenever the model property is reassigned, so structural changes that
/// reach `publish_script_panel` (group expand/collapse, directional toggle,
/// add / remove / clone / move) need to mutate the existing model rather
/// than replace it. Falls back to a fresh model on the very first publish
/// (when the property still holds a non-`VecModel` default).
pub(crate) fn publish_script_rows_in_place(w: &MainWindow, new_rows: Vec<ScriptEntryRow>) {
    let rows_rc = w.get_script_rows();
    if let Some(rows) = rows_rc.as_any().downcast_ref::<slint::VecModel<ScriptEntryRow>>() {
        let cur_n = rows.row_count();
        let new_n = new_rows.len();
        let copy_n = cur_n.min(new_n);
        let mut iter = new_rows.into_iter();
        for i in 0..copy_n {
            // copy_n <= new_n, so the iterator is guaranteed to yield here.
            rows.set_row_data(i, iter.next().unwrap());
        }
        // Trim any extra trailing rows from the existing model. Removing from
        // the back avoids reshuffling earlier indices.
        for i in (new_n..cur_n).rev() {
            rows.remove(i);
        }
        // Append any rows the new layout has past the old length.
        for row in iter {
            rows.push(row);
        }
        return;
    }
    w.set_script_rows(std::rc::Rc::new(slint::VecModel::from(new_rows)).into());
}

/// Avatar-tab analogue of `patch_script_row_label_in_place`. Recomputes the
/// row label from the current `character` + `expression` model values and
/// updates the matching row in place.
pub(crate) fn patch_script_row_avatar_in_place(w: &MainWindow, ed: &Editor, sel: usize) {
    use hm64_sprite_editor::model::AnimationData;
    let Some(AnimationData::Avatar(t)) = ed.loaded_animation_data.as_ref() else { return };
    let Some(entry) = t.entries.get(sel) else { return };
    let new_label = script_vm::avatar_row_label(&entry.character, &entry.expression);
    let rows_rc = w.get_script_rows();
    let Some(rows) =
        rows_rc.as_any().downcast_ref::<slint::VecModel<ScriptEntryRow>>()
    else {
        return;
    };
    let target = sel as i32;
    let n = rows.row_count();
    for i in 0..n {
        let Some(mut row) = rows.row_data(i) else { continue };
        if row.is_break || row.is_group_header {
            continue;
        }
        if row.entry_index == target {
            row.label = SharedString::from(new_label);
            rows.set_row_data(i, row);
            break;
        }
    }
}

pub(crate) fn json_anim_type_to_combo_idx(t: hm64_sprite_editor::model::JsonAnimType) -> i32 {
    use hm64_sprite_editor::model::JsonAnimType;
    match t {
        JsonAnimType::OneShot => 0,
        JsonAnimType::Loop => 1,
        JsonAnimType::DestroyOnEnd => 2,
    }
}

/// Render the current preview image for a script-tab entry. Goes through the
/// shared playback compose pipeline (same as the Animations tab gallery), so
/// multi-layer animations show every layer instead of just the first sprite.
pub(crate) fn render_script_preview_for(
    s: &hm64_sprite_editor::model::SpriteAsset,
    anims_vm: &mut AnimationsVm,
    idx: u32,
    h_flip: bool,
) -> Option<slint::Image> {
    if h_flip {
        anims_vm.current_frame_image_h_flipped(s, idx)
    } else {
        anims_vm.current_frame_image(s, idx)
    }
}

pub(crate) fn republish_entry_preview(w: &MainWindow, ed: &Editor, metadata_index: u16, h_flip: bool) {
    use hm64_sprite_editor::state::playback::{
        compose_frame, compose_frame_h_flipped, compute_animation_bounds, FrameCache,
    };

    let frame_count = ed
        .current_sprite()
        .and_then(|s| s.animations.get(&(metadata_index as u32)))
        .map(|a| a.frames.len())
        .unwrap_or(1)
        .max(1) as i32;
    w.set_script_preview_frame_count(frame_count);

    // Compose every layer of the first frame, matching the Animations gallery.
    // The earlier "render only the first sprite layer" shortcut dropped layers
    // for composed animations (e.g. standalone albumPhotos), making the
    // Scripts-tab preview disagree with the Animations-tab thumbnail.
    //
    // A transient FrameCache is fine here: this only runs on entry click /
    // commit, and the advance-loop path uses the long-lived `anims_vm` cache
    // anyway. Decoding a single sprite's palette is microseconds.
    let preview = ed
        .current_sprite()
        .and_then(|s| {
            let anim = s.animations.get(&(metadata_index as u32))?;
            let frame = anim.frames.first()?;
            let bounds = compute_animation_bounds(s, anim)?;
            let mut cache = FrameCache::default();
            Some(if h_flip {
                compose_frame_h_flipped(&mut cache, s, frame, bounds)
            } else {
                compose_frame(&mut cache, s, frame, bounds)
            })
        })
        .unwrap_or_default();
    w.set_script_entry_preview(preview);
}

