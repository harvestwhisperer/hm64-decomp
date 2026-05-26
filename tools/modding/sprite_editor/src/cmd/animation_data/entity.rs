//! Commands targeting `AnimationData::Entity` — per-entry value edits, list
//! mutations, and the directional-group toggle.

use once_cell::unsync::OnceCell;

use crate::cmd::animation_data::{entity_anim_mut, entity_script_mut, try_merge_set};
use crate::cmd::{CmdError, Command, DirtyMask};
use crate::model::{directional_group_base, AnimationData, EntityAnimation, JsonAnimType};
use crate::state::editor::Editor;

/// Sets the label on one entity-script entry, OR on all 8 entries of a
/// directional group when the target entry is part of one. Editing any entry
/// of a group rewrites the whole group so JSON serialization stays
/// consistent — the group has a single conceptual label, even though the
/// JSON model still stores it per-entry.
#[derive(Debug)]
pub struct SetEntityAnimationLabel {
    pub script_idx: usize,
    /// Always non-empty. Either a single index (non-grouped entry) or all 8
    /// indices of a directional group.
    pub entry_indices: Vec<usize>,
    pub new: String,
    pub old: Vec<String>,
    applied_at: u128,
}

impl SetEntityAnimationLabel {
    pub fn new(script_idx: usize, entry_idx: usize, new: String, editor: &Editor) -> Option<Self> {
        let (entry_indices, old) = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                let doc = scripts.get(script_idx)?;
                let _ = doc.animations.get(entry_idx)?;
                let indices: Vec<usize> =
                    if let Some(base) = directional_group_base(doc, entry_idx) {
                        (base..base + crate::model::DIRECTIONAL_GROUP_SIZE).collect()
                    } else {
                        vec![entry_idx]
                    };
                let labels: Vec<String> =
                    indices.iter().map(|&i| doc.animations[i].label.clone()).collect();
                (indices, labels)
            }
            _ => return None,
        };
        Some(Self { script_idx, entry_indices, new, old, applied_at: 0 })
    }
}

impl Command for SetEntityAnimationLabel {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        for &idx in &self.entry_indices {
            let anim = entity_anim_mut(state, self.script_idx, idx)?;
            anim.label = self.new.clone();
        }
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        for (idx, old) in self.entry_indices.iter().zip(self.old.iter()) {
            let anim = entity_anim_mut(state, self.script_idx, *idx)?;
            anim.label = old.clone();
        }
        Ok(())
    }
    fn describe(&self) -> String {
        if self.entry_indices.len() == 1 {
            format!(
                "Set entity script[{}] entry[{}] label",
                self.script_idx, self.entry_indices[0]
            )
        } else {
            let base = self.entry_indices[0];
            format!(
                "Set entity script[{}] directional group [{}..{}] label",
                self.script_idx,
                base,
                base + self.entry_indices.len() - 1
            )
        }
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetEntityAnimationLabel, [script_idx, entry_indices], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct SetEntityAnimationOffset {
    pub script_idx: usize,
    pub entry_idx: usize,
    pub new: u16,
    pub old: u16,
    applied_at: u128,
}

impl SetEntityAnimationOffset {
    pub fn new(script_idx: usize, entry_idx: usize, new: u16, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                scripts.get(script_idx)?.animations.get(entry_idx)?.metadata_index
            }
            _ => return None,
        };
        Some(Self { script_idx, entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetEntityAnimationOffset {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = entity_anim_mut(state, self.script_idx, self.entry_idx)?;
        anim.metadata_index = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = entity_anim_mut(state, self.script_idx, self.entry_idx)?;
        anim.metadata_index = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Set entity script[{}] entry[{}] offset",
            self.script_idx, self.entry_idx
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetEntityAnimationOffset, [script_idx, entry_idx], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct SetEntityAnimationType {
    pub script_idx: usize,
    pub entry_idx: usize,
    pub new: JsonAnimType,
    pub old: JsonAnimType,
    applied_at: u128,
}

impl SetEntityAnimationType {
    pub fn new(
        script_idx: usize,
        entry_idx: usize,
        new: JsonAnimType,
        editor: &Editor,
    ) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                scripts.get(script_idx)?.animations.get(entry_idx)?.anim_type
            }
            _ => return None,
        };
        Some(Self { script_idx, entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetEntityAnimationType {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = entity_anim_mut(state, self.script_idx, self.entry_idx)?;
        anim.anim_type = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = entity_anim_mut(state, self.script_idx, self.entry_idx)?;
        anim.anim_type = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Set entity script[{}] entry[{}] type",
            self.script_idx, self.entry_idx
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct SetEntityAnimationFlip {
    pub script_idx: usize,
    pub entry_idx: usize,
    pub new: bool,
    pub old: bool,
    applied_at: u128,
}

impl SetEntityAnimationFlip {
    pub fn new(script_idx: usize, entry_idx: usize, new: bool, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                scripts.get(script_idx)?.animations.get(entry_idx)?.flip
            }
            _ => return None,
        };
        Some(Self { script_idx, entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetEntityAnimationFlip {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = entity_anim_mut(state, self.script_idx, self.entry_idx)?;
        anim.flip = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = entity_anim_mut(state, self.script_idx, self.entry_idx)?;
        anim.flip = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Set entity script[{}] entry[{}] flip",
            self.script_idx, self.entry_idx
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct AddEntityAnimation {
    pub script_idx: usize,
    pub at: usize,
    pub entry: EntityAnimation,
    applied_at: u128,
}

impl AddEntityAnimation {
    pub fn new(
        script_idx: usize,
        at: usize,
        entry: EntityAnimation,
        editor: &Editor,
    ) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                let doc = scripts.get(script_idx)?;
                if at > doc.animations.len() {
                    return None;
                }
            }
            _ => return None,
        }
        Some(Self { script_idx, at, entry, applied_at: 0 })
    }
}

impl Command for AddEntityAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        if self.at > anims.len() {
            return Err(CmdError::InvalidTarget(format!(
                "insertion index {} > len {}",
                self.at,
                anims.len()
            )));
        }
        anims.insert(self.at, self.entry.clone());
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        if self.at >= anims.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        anims.remove(self.at);
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Add entity animation at script[{}] entry[{}]",
            self.script_idx, self.at
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct RemoveEntityAnimation {
    pub script_idx: usize,
    pub at: usize,
    pub removed: OnceCell<EntityAnimation>,
    applied_at: u128,
}

impl RemoveEntityAnimation {
    pub fn new(script_idx: usize, at: usize, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                let doc = scripts.get(script_idx)?;
                let _ = doc.animations.get(at)?;
            }
            _ => return None,
        }
        Some(Self { script_idx, at, removed: OnceCell::new(), applied_at: 0 })
    }
}

impl Command for RemoveEntityAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        if self.at >= anims.len() {
            return Err(CmdError::InvalidTarget(format!(
                "remove index {} >= len {}",
                self.at,
                anims.len()
            )));
        }
        let removed = anims.remove(self.at);
        // OnceCell may already hold a value if we re-apply after revert: that's
        // fine, the entry is identical, so we ignore the set error.
        let _ = self.removed.set(removed);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let removed = self
            .removed
            .get()
            .ok_or_else(|| CmdError::Invariant("revert before apply".into()))?
            .clone();
        let anims = entity_script_mut(state, self.script_idx)?;
        if self.at > anims.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        anims.insert(self.at, removed);
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Remove entity animation script[{}] entry[{}]",
            self.script_idx, self.at
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct CloneEntityAnimation {
    pub script_idx: usize,
    pub src_idx: usize,
    cloned: OnceCell<EntityAnimation>,
    applied_at: u128,
}

impl CloneEntityAnimation {
    pub fn new(script_idx: usize, src_idx: usize, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                let doc = scripts.get(script_idx)?;
                let _ = doc.animations.get(src_idx)?;
            }
            _ => return None,
        }
        Some(Self {
            script_idx,
            src_idx,
            cloned: OnceCell::new(),
            applied_at: 0,
        })
    }

    /// Insertion index used both by `apply` and `revert`.
    fn insert_at(&self) -> usize {
        self.src_idx + 1
    }
}

impl Command for CloneEntityAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        let src = anims
            .get(self.src_idx)
            .ok_or_else(|| {
                CmdError::InvalidTarget(format!("src index {} out of range", self.src_idx))
            })?
            .clone();
        let _ = self.cloned.set(src.clone());
        anims.insert(self.insert_at(), src);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let at = self.insert_at();
        let anims = entity_script_mut(state, self.script_idx)?;
        if at >= anims.len() {
            return Err(CmdError::Invariant("revert: clone index out of range".into()));
        }
        anims.remove(at);
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Clone entity animation script[{}] entry[{}]",
            self.script_idx, self.src_idx
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct MoveEntityAnimation {
    pub script_idx: usize,
    pub from: usize,
    pub to: usize,
    applied_at: u128,
}

impl MoveEntityAnimation {
    pub fn new(script_idx: usize, from: usize, to: usize, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => {
                let doc = scripts.get(script_idx)?;
                let len = doc.animations.len();
                if from >= len || to >= len {
                    return None;
                }
            }
            _ => return None,
        }
        Some(Self { script_idx, from, to, applied_at: 0 })
    }
}

impl Command for MoveEntityAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        let len = anims.len();
        if self.from >= len || self.to >= len {
            return Err(CmdError::InvalidTarget(format!(
                "move from={} to={} len={}",
                self.from, self.to, len
            )));
        }
        if self.from == self.to {
            return Ok(());
        }
        let entry = anims.remove(self.from);
        anims.insert(self.to, entry);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        let len = anims.len();
        if self.from >= len || self.to >= len {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        if self.from == self.to {
            return Ok(());
        }
        let entry = anims.remove(self.to);
        anims.insert(self.from, entry);
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Move entity animation script[{}] {} → {}",
            self.script_idx, self.from, self.to
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

/// Toggle the `directional` flag on a contiguous run of 8 entity-animation
/// entries (the SW..S group consumed by `setEntityDirectionalAnimation`).
///
/// Groups are not 8-aligned — a group can start at any position. The two
/// directions of toggle are interpreted differently in `new()`:
///
///   - `new_value=true` (form a group): `entry_idx` is the proposed base.
///     Rejected if there aren't 8 entries from there, or if any entry in
///     `[entry_idx..entry_idx+8]` is already directional (would overlap an
///     existing group).
///   - `new_value=false` (disband a group): `entry_idx` may be any entry
///     inside an existing group; the actual base is recovered via
///     `directional_group_base`. Rejected if the entry isn't part of a
///     complete group.
#[derive(Debug)]
pub struct ToggleEntityDirectionalGroup {
    pub script_idx: usize,
    pub base_idx: usize,
    pub new_value: bool,
    pub old_values: [bool; 8],
    applied_at: u128,
}

impl ToggleEntityDirectionalGroup {
    pub fn new(
        script_idx: usize,
        entry_idx: usize,
        new_value: bool,
        editor: &Editor,
    ) -> Option<Self> {
        let scripts = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Entity { scripts } => scripts,
            _ => return None,
        };
        let doc = scripts.get(script_idx)?;
        let base_idx = if new_value {
            if !crate::model::can_form_directional_group(doc, entry_idx) {
                return None;
            }
            entry_idx
        } else {
            directional_group_base(doc, entry_idx)?
        };
        if base_idx + 8 > doc.animations.len() {
            return None;
        }
        let mut old_values = [false; 8];
        for (i, slot) in old_values.iter_mut().enumerate() {
            *slot = doc.animations[base_idx + i].directional;
        }
        Some(Self { script_idx, base_idx, new_value, old_values, applied_at: 0 })
    }
}

impl Command for ToggleEntityDirectionalGroup {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        if self.base_idx + 8 > anims.len() {
            return Err(CmdError::InvalidTarget(format!(
                "directional toggle: base {} + 8 > len {}",
                self.base_idx,
                anims.len()
            )));
        }
        for i in 0..8 {
            anims[self.base_idx + i].directional = self.new_value;
        }
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anims = entity_script_mut(state, self.script_idx)?;
        if self.base_idx + 8 > anims.len() {
            return Err(CmdError::Invariant(
                "revert: directional group out of range".into(),
            ));
        }
        for i in 0..8 {
            anims[self.base_idx + i].directional = self.old_values[i];
        }
        Ok(())
    }
    fn describe(&self) -> String {
        format!(
            "Toggle entity script[{}] directional [{}..{}] = {}",
            self.script_idx,
            self.base_idx,
            self.base_idx + 8,
            self.new_value
        )
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::cmd::animation_data::test_fixtures::{
        avatar_editor, entity_editor,
    };
    use crate::cmd::history::{History, MERGE_WINDOW_MS};
    use crate::model::EntityScriptDoc;

    fn entity_editor_with_directional_group() -> Editor {
        let mut e = Editor::new();
        e.loaded_animation_data = Some(AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "test".into(),
                animations: (0..8)
                    .map(|i| EntityAnimation {
                        label: i.to_string(),
                        metadata_index: i as u16,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: true,
                    })
                    .collect(),
                trailing_zeros: 0,
                name: "main".into(),
            }],
        });
        e
    }

    /// Single-script entity with 16 entries, all non-directional. Tests that
    /// need a pre-existing group toggle entries directly.
    fn directional_editor() -> Editor {
        let mut e = Editor::new();
        let anims: Vec<EntityAnimation> = (0..16)
            .map(|i| EntityAnimation {
                label: format!("L{i}"),
                metadata_index: i as u16,
                anim_type: JsonAnimType::Loop,
                flip: false,
                directional: false,
            })
            .collect();
        e.loaded_animation_data = Some(AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "test".into(),
                animations: anims,
                trailing_zeros: 0,
                name: "main".into(),
            }],
        });
        e
    }

    fn set_range_directional(e: &mut Editor, range: std::ops::Range<usize>, value: bool) {
        match e.loaded_animation_data.as_mut().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in range {
                    scripts[0].animations[i].directional = value;
                }
            }
            _ => panic!(),
        }
    }

    #[test]
    fn set_entity_label_apply_revert() {
        let mut e = entity_editor();
        let mut cmd = SetEntityAnimationLabel::new(0, 0, "NEW".into(), &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].label, "NEW");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].label, "0");
            }
            _ => panic!("wrong variant"),
        }
        // Re-apply produces the same end state.
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].label, "NEW");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_entity_label_describe_format() {
        let cmd = SetEntityAnimationLabel::new(2, 7, "X".into(), &entity_editor());
        // script idx 2 doesn't exist, so new() returns None.
        assert!(cmd.is_none());
        let cmd = SetEntityAnimationLabel::new(0, 1, "X".into(), &entity_editor()).unwrap();
        assert_eq!(cmd.describe(), "Set entity script[0] entry[1] label");
    }

    #[test]
    fn set_entity_label_rewrites_whole_directional_group() {
        let mut e = entity_editor_with_directional_group();
        // Targeting any entry in the group should rewrite all 8.
        let mut cmd = SetEntityAnimationLabel::new(0, 5, "WALKING".into(), &e).unwrap();
        assert_eq!(cmd.entry_indices, (0..8).collect::<Vec<_>>());
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for a in &scripts[0].animations {
                    assert_eq!(a.label, "WALKING");
                }
            }
            _ => panic!("wrong variant"),
        }
        // Revert restores each original label.
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for (i, a) in scripts[0].animations.iter().enumerate() {
                    assert_eq!(a.label, i.to_string());
                }
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_entity_label_describe_for_group() {
        let cmd =
            SetEntityAnimationLabel::new(0, 3, "X".into(), &entity_editor_with_directional_group())
                .unwrap();
        assert_eq!(
            cmd.describe(),
            "Set entity script[0] directional group [0..7] label"
        );
    }

    #[test]
    fn set_entity_offset_merges_same_target() {
        let mut e = entity_editor();
        let mut h = History::default();
        h.execute(
            Box::new(SetEntityAnimationOffset::new(0, 0, 5, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        h.execute(
            Box::new(SetEntityAnimationOffset::new(0, 0, 9, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        // Verify only one undo entry.
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 1);
        // After reverting the merged command, offset is back to original 0.
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].metadata_index, 0);
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_entity_offset_different_entry_does_not_merge() {
        let mut e = entity_editor();
        let mut h = History::default();
        h.execute(
            Box::new(SetEntityAnimationOffset::new(0, 0, 5, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        h.execute(
            Box::new(SetEntityAnimationOffset::new(0, 1, 9, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 2);
    }

    #[test]
    fn set_entity_offset_outside_merge_window_does_not_merge() {
        // Simulate two commands > MERGE_WINDOW_MS apart.
        let mut e = entity_editor();
        let mut h = History::default();
        let mut first = SetEntityAnimationOffset::new(0, 0, 5, &e).unwrap();
        first.apply(&mut e).unwrap();
        first.set_applied_at(0);
        // Push manually with a stale timestamp by using execute (which sets a
        // fresh timestamp). To test the boundary we use try_merge directly.
        let second = SetEntityAnimationOffset::new(0, 0, 9, &e).unwrap();
        // Same target merges via try_merge regardless of time — the time-window
        // filter is in `History::execute`. Verify via that path that two
        // commands more than MERGE_WINDOW_MS apart are kept separate.
        let _ = second; // unused: see end-to-end check below
        h.execute(
            Box::new(SetEntityAnimationOffset::new(0, 0, 5, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        // Sleep past the merge window then push another.
        std::thread::sleep(std::time::Duration::from_millis(
            (MERGE_WINDOW_MS as u64) + 50,
        ));
        h.execute(
            Box::new(SetEntityAnimationOffset::new(0, 0, 7, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 2);
    }

    #[test]
    fn set_entity_type_apply_revert() {
        let mut e = entity_editor();
        let mut cmd =
            SetEntityAnimationType::new(0, 0, JsonAnimType::DestroyOnEnd, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].anim_type, JsonAnimType::DestroyOnEnd);
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].anim_type, JsonAnimType::Loop);
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_entity_type_does_not_merge() {
        let mut e = entity_editor();
        let mut h = History::default();
        h.execute(
            Box::new(SetEntityAnimationType::new(0, 0, JsonAnimType::OneShot, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        h.execute(
            Box::new(SetEntityAnimationType::new(0, 0, JsonAnimType::DestroyOnEnd, &e).unwrap()),
            &mut e,
        )
        .unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 2);
    }

    #[test]
    fn set_entity_flip_apply_revert() {
        let mut e = entity_editor();
        let mut cmd = SetEntityAnimationFlip::new(0, 0, true, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert!(scripts[0].animations[0].flip);
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert!(!scripts[0].animations[0].flip);
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn add_entity_animation_apply_revert() {
        let mut e = entity_editor();
        let entry = EntityAnimation {
            label: "NEW".into(),
            metadata_index: 99,
            anim_type: JsonAnimType::Loop,
            flip: false,
            directional: false,
        };
        let mut cmd = AddEntityAnimation::new(0, 1, entry.clone(), &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 3);
                assert_eq!(scripts[0].animations[1].label, "NEW");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 2);
                assert_eq!(scripts[0].animations[1].label, "1");
            }
            _ => panic!("wrong variant"),
        }
        // Re-apply.
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 3);
                assert_eq!(scripts[0].animations[1].label, "NEW");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn remove_entity_animation_apply_revert() {
        let mut e = entity_editor();
        let mut cmd = RemoveEntityAnimation::new(0, 0, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 1);
                assert_eq!(scripts[0].animations[0].label, "1");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 2);
                assert_eq!(scripts[0].animations[0].label, "0");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn clone_entity_animation_round_trip() {
        let mut e = entity_editor();
        let mut cmd = CloneEntityAnimation::new(0, 0, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 3);
                // The clone goes to src+1 = 1.
                assert_eq!(scripts[0].animations[1].label, "0");
                assert_eq!(scripts[0].animations[2].label, "1");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations.len(), 2);
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn move_entity_animation_round_trip() {
        let mut e = entity_editor();
        let mut cmd = MoveEntityAnimation::new(0, 0, 1, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].label, "1");
                assert_eq!(scripts[0].animations[1].label, "0");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts[0].animations[0].label, "0");
                assert_eq!(scripts[0].animations[1].label, "1");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn entity_command_affects_animation_data() {
        let e = entity_editor();
        let cmd = SetEntityAnimationLabel::new(0, 0, "X".into(), &e).unwrap();
        assert_eq!(cmd.affects(), DirtyMask::ANIMATION_DATA);
    }

    #[test]
    fn directional_new_rejects_wrong_variant() {
        let e = avatar_editor();
        assert!(ToggleEntityDirectionalGroup::new(0, 0, true, &e).is_none());
    }

    #[test]
    fn directional_new_rejects_oob_script() {
        let e = directional_editor();
        assert!(ToggleEntityDirectionalGroup::new(99, 0, true, &e).is_none());
    }

    #[test]
    fn directional_new_form_allows_arbitrary_position() {
        // No alignment requirement: forming a group at base 3 is OK as long
        // as 8 entries are available and none of them are already directional.
        let e = directional_editor();
        assert!(ToggleEntityDirectionalGroup::new(0, 0, true, &e).is_some());
        assert!(ToggleEntityDirectionalGroup::new(0, 3, true, &e).is_some());
        assert!(ToggleEntityDirectionalGroup::new(0, 7, true, &e).is_some());
        assert!(ToggleEntityDirectionalGroup::new(0, 8, true, &e).is_some());
    }

    #[test]
    fn directional_new_form_rejects_too_few_entries_from_base() {
        let e = directional_editor();
        // 16 entries — base 9 leaves only 7 → reject; base 16 leaves 0.
        assert!(ToggleEntityDirectionalGroup::new(0, 9, true, &e).is_none());
        assert!(ToggleEntityDirectionalGroup::new(0, 16, true, &e).is_none());
        assert!(ToggleEntityDirectionalGroup::new(0, 24, true, &e).is_none());
    }

    #[test]
    fn directional_new_form_rejects_overlap_with_existing_group() {
        // Pre-form a group at [0..8]. Trying to form a new group anywhere
        // that overlaps must reject.
        let mut e = directional_editor();
        set_range_directional(&mut e, 0..8, true);
        assert!(ToggleEntityDirectionalGroup::new(0, 0, true, &e).is_none()); // exact overlap
        assert!(ToggleEntityDirectionalGroup::new(0, 4, true, &e).is_none()); // partial overlap
        assert!(ToggleEntityDirectionalGroup::new(0, 7, true, &e).is_none()); // 1-entry overlap
        // But a fresh, non-overlapping window is fine.
        assert!(ToggleEntityDirectionalGroup::new(0, 8, true, &e).is_some());
    }

    #[test]
    fn directional_new_disband_finds_base_from_any_entry_in_group() {
        // Group at [3..11]. Disband targeted from any inside index resolves
        // to base 3.
        let mut e = directional_editor();
        set_range_directional(&mut e, 3..11, true);
        let cmd = ToggleEntityDirectionalGroup::new(0, 7, false, &e).unwrap();
        assert_eq!(cmd.base_idx, 3);
        let cmd = ToggleEntityDirectionalGroup::new(0, 3, false, &e).unwrap();
        assert_eq!(cmd.base_idx, 3);
    }

    #[test]
    fn directional_new_disband_rejects_when_not_in_a_group() {
        let e = directional_editor();
        assert!(ToggleEntityDirectionalGroup::new(0, 0, false, &e).is_none());
    }

    #[test]
    fn directional_apply_revert_round_trip() {
        let mut e = directional_editor();
        let mut cmd = ToggleEntityDirectionalGroup::new(0, 0, true, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in 0..8 {
                    assert!(scripts[0].animations[i].directional, "slot {i} not set");
                }
                assert!(!scripts[0].animations[8].directional);
            }
            _ => panic!(),
        }

        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in 0..8 {
                    assert!(!scripts[0].animations[i].directional);
                }
            }
            _ => panic!(),
        }
    }

    #[test]
    fn directional_double_apply_idempotent() {
        let mut e = directional_editor();
        let mut cmd = ToggleEntityDirectionalGroup::new(0, 8, true, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in 8..16 {
                    assert!(scripts[0].animations[i].directional);
                }
                for i in 0..8 {
                    assert!(!scripts[0].animations[i].directional);
                }
            }
            _ => panic!(),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in 8..16 {
                    assert!(!scripts[0].animations[i].directional);
                }
            }
            _ => panic!(),
        }
    }

    #[test]
    fn directional_clear_value_works() {
        // Pre-populate slots 0..8 as directional, then toggle off.
        let mut e = directional_editor();
        set_range_directional(&mut e, 0..8, true);
        let mut cmd = ToggleEntityDirectionalGroup::new(0, 0, false, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in 0..8 {
                    assert!(!scripts[0].animations[i].directional);
                }
            }
            _ => panic!(),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Entity { scripts } => {
                for i in 0..8 {
                    assert!(scripts[0].animations[i].directional);
                }
            }
            _ => panic!(),
        }
    }
}
