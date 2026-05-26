//! Commands targeting `AnimationData::Avatar` (the character / expression /
//! metadata-offset table). Entries are mutable via Add / Remove / Clone / Move
//! commands.

use once_cell::unsync::OnceCell;

use crate::cmd::animation_data::{avatar_entries_mut, avatar_entry_mut, try_merge_set};
use crate::cmd::{CmdError, Command, DirtyMask};
use crate::model::{AnimationData, AvatarEntry};
use crate::state::editor::Editor;

#[derive(Debug)]
pub struct SetAvatarCharacter {
    pub entry_idx: usize,
    pub new: String,
    pub old: String,
    applied_at: u128,
}

impl SetAvatarCharacter {
    pub fn new(entry_idx: usize, new: String, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => t.entries.get(entry_idx)?.character.clone(),
            _ => return None,
        };
        Some(Self { entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetAvatarCharacter {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let e = avatar_entry_mut(state, self.entry_idx)?;
        e.character = self.new.clone();
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let e = avatar_entry_mut(state, self.entry_idx)?;
        e.character = self.old.clone();
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set avatar entry[{}] character", self.entry_idx)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetAvatarCharacter, [entry_idx], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct SetAvatarExpression {
    pub entry_idx: usize,
    pub new: String,
    pub old: String,
    applied_at: u128,
}

impl SetAvatarExpression {
    pub fn new(entry_idx: usize, new: String, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => t.entries.get(entry_idx)?.expression.clone(),
            _ => return None,
        };
        Some(Self { entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetAvatarExpression {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let e = avatar_entry_mut(state, self.entry_idx)?;
        e.expression = self.new.clone();
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let e = avatar_entry_mut(state, self.entry_idx)?;
        e.expression = self.old.clone();
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set avatar entry[{}] expression", self.entry_idx)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetAvatarExpression, [entry_idx], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct SetAvatarOffset {
    pub entry_idx: usize,
    pub new: u16,
    pub old: u16,
    applied_at: u128,
}

impl SetAvatarOffset {
    pub fn new(entry_idx: usize, new: u16, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => t.entries.get(entry_idx)?.metadata_index,
            _ => return None,
        };
        Some(Self { entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetAvatarOffset {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let e = avatar_entry_mut(state, self.entry_idx)?;
        e.metadata_index = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let e = avatar_entry_mut(state, self.entry_idx)?;
        e.metadata_index = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set avatar entry[{}] offset", self.entry_idx)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetAvatarOffset, [entry_idx], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct AddAvatarEntry {
    pub at: usize,
    pub entry: AvatarEntry,
    applied_at: u128,
}

impl AddAvatarEntry {
    pub fn new(at: usize, entry: AvatarEntry, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => {
                if at > t.entries.len() {
                    return None;
                }
            }
            _ => return None,
        }
        Some(Self { at, entry, applied_at: 0 })
    }
}

impl Command for AddAvatarEntry {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        if self.at > entries.len() {
            return Err(CmdError::InvalidTarget(format!(
                "insertion index {} > len {}",
                self.at,
                entries.len()
            )));
        }
        entries.insert(self.at, self.entry.clone());
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        if self.at >= entries.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        entries.remove(self.at);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Add avatar entry at[{}]", self.at)
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
pub struct RemoveAvatarEntry {
    pub at: usize,
    pub removed: OnceCell<AvatarEntry>,
    applied_at: u128,
}

impl RemoveAvatarEntry {
    pub fn new(at: usize, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => {
                let _ = t.entries.get(at)?;
            }
            _ => return None,
        }
        Some(Self { at, removed: OnceCell::new(), applied_at: 0 })
    }
}

impl Command for RemoveAvatarEntry {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        if self.at >= entries.len() {
            return Err(CmdError::InvalidTarget(format!(
                "remove index {} >= len {}",
                self.at,
                entries.len()
            )));
        }
        let removed = entries.remove(self.at);
        let _ = self.removed.set(removed);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let removed = self
            .removed
            .get()
            .ok_or_else(|| CmdError::Invariant("revert before apply".into()))?
            .clone();
        let entries = avatar_entries_mut(state)?;
        if self.at > entries.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        entries.insert(self.at, removed);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Remove avatar entry[{}]", self.at)
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
pub struct CloneAvatarEntry {
    pub src: usize,
    applied_at: u128,
}

impl CloneAvatarEntry {
    pub fn new(src: usize, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => {
                let _ = t.entries.get(src)?;
            }
            _ => return None,
        }
        Some(Self { src, applied_at: 0 })
    }
}

impl Command for CloneAvatarEntry {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        let src = self.src;
        let entry = entries
            .get(src)
            .ok_or_else(|| CmdError::InvalidTarget(format!("clone src {src} out of range")))?
            .clone();
        entries.insert(src + 1, entry);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        let idx = self.src + 1;
        if idx >= entries.len() {
            return Err(CmdError::Invariant("revert: clone slot out of range".into()));
        }
        entries.remove(idx);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Clone avatar entry[{}]", self.src)
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
pub struct MoveAvatarEntry {
    pub from: usize,
    pub to: usize,
    applied_at: u128,
}

impl MoveAvatarEntry {
    pub fn new(from: usize, to: usize, editor: &Editor) -> Option<Self> {
        if from == to {
            return None;
        }
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Avatar(t) => {
                let len = t.entries.len();
                if from >= len || to >= len {
                    return None;
                }
            }
            _ => return None,
        }
        Some(Self { from, to, applied_at: 0 })
    }
}

impl Command for MoveAvatarEntry {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        let len = entries.len();
        if self.from >= len || self.to >= len {
            return Err(CmdError::InvalidTarget(format!(
                "move from={} to={} len={}",
                self.from, self.to, len
            )));
        }
        let entry = entries.remove(self.from);
        entries.insert(self.to, entry);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let entries = avatar_entries_mut(state)?;
        let len = entries.len();
        if self.from >= len || self.to >= len {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        let entry = entries.remove(self.to);
        entries.insert(self.from, entry);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Move avatar entry {} -> {}", self.from, self.to)
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
    use crate::cmd::animation_data::test_fixtures::{avatar_editor, entity_editor};
    use crate::cmd::history::History;

    #[test]
    fn set_avatar_character_apply_revert() {
        let mut e = avatar_editor();
        let mut cmd = SetAvatarCharacter::new(0, "ANN".into(), &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries[0].character, "ANN"),
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries[0].character, "MARIA"),
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_avatar_character_merges_same_entry() {
        let mut e = avatar_editor();
        let mut h = History::default();
        h.execute(
            Box::new(SetAvatarCharacter::new(0, "A".into(), &e).unwrap()),
            &mut e,
        )
        .unwrap();
        h.execute(
            Box::new(SetAvatarCharacter::new(0, "B".into(), &e).unwrap()),
            &mut e,
        )
        .unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 1);
    }

    #[test]
    fn set_avatar_expression_apply_revert() {
        let mut e = avatar_editor();
        let mut cmd = SetAvatarExpression::new(1, "ANGRY".into(), &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries[1].expression, "ANGRY"),
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries[1].expression, "SAD"),
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_avatar_offset_apply_revert() {
        let mut e = avatar_editor();
        let mut cmd = SetAvatarOffset::new(0, 42, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries[0].metadata_index, 42),
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries[0].metadata_index, 7),
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn avatar_new_returns_none_for_wrong_variant() {
        let e = entity_editor();
        assert!(SetAvatarCharacter::new(0, "X".into(), &e).is_none());
        assert!(SetAvatarExpression::new(0, "X".into(), &e).is_none());
        assert!(SetAvatarOffset::new(0, 0, &e).is_none());
    }

    #[test]
    fn add_avatar_entry_apply_revert() {
        let mut e = avatar_editor();
        let entry = AvatarEntry {
            character: "POPURI".into(),
            expression: "SHY".into(),
            metadata_index: 99,
        };
        let mut cmd = AddAvatarEntry::new(2, entry.clone(), &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries.len(), 3);
                assert_eq!(t.entries[2], entry);
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => assert_eq!(t.entries.len(), 2),
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn add_avatar_entry_new_returns_none_for_wrong_variant() {
        let e = entity_editor();
        let entry = AvatarEntry { character: "X".into(), expression: "Y".into(), metadata_index: 0 };
        assert!(AddAvatarEntry::new(0, entry, &e).is_none());
    }

    #[test]
    fn add_avatar_entry_new_returns_none_for_oob_index() {
        let e = avatar_editor();
        let entry = AvatarEntry { character: "X".into(), expression: "Y".into(), metadata_index: 0 };
        // len is 2, so at=3 is out of range
        assert!(AddAvatarEntry::new(3, entry, &e).is_none());
    }

    #[test]
    fn remove_avatar_entry_apply_revert() {
        let mut e = avatar_editor();
        let mut cmd = RemoveAvatarEntry::new(0, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries.len(), 1);
                assert_eq!(t.entries[0].character, "ELLI");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries.len(), 2);
                assert_eq!(t.entries[0].character, "MARIA");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn remove_avatar_entry_new_returns_none_for_wrong_variant() {
        let e = entity_editor();
        assert!(RemoveAvatarEntry::new(0, &e).is_none());
    }

    #[test]
    fn remove_avatar_entry_new_returns_none_for_oob_index() {
        let e = avatar_editor();
        assert!(RemoveAvatarEntry::new(5, &e).is_none());
    }

    #[test]
    fn clone_avatar_entry_apply_revert() {
        let mut e = avatar_editor();
        let mut cmd = CloneAvatarEntry::new(0, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries.len(), 3);
                assert_eq!(t.entries[0].character, "MARIA");
                assert_eq!(t.entries[1].character, "MARIA"); // cloned at src+1
                assert_eq!(t.entries[2].character, "ELLI"); // shifted from old idx 1
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries.len(), 2);
                assert_eq!(t.entries[0].character, "MARIA");
                assert_eq!(t.entries[1].character, "ELLI");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn clone_avatar_entry_new_returns_none_for_wrong_variant() {
        let e = entity_editor();
        assert!(CloneAvatarEntry::new(0, &e).is_none());
    }

    #[test]
    fn clone_avatar_entry_new_returns_none_for_oob_index() {
        let e = avatar_editor();
        assert!(CloneAvatarEntry::new(5, &e).is_none());
    }

    #[test]
    fn move_avatar_entry_apply_revert_forward() {
        let mut e = avatar_editor();
        let mut cmd = MoveAvatarEntry::new(0, 1, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries[0].character, "ELLI");
                assert_eq!(t.entries[1].character, "MARIA");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries[0].character, "MARIA");
                assert_eq!(t.entries[1].character, "ELLI");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn move_avatar_entry_apply_revert_backward() {
        let mut e = avatar_editor();
        let mut cmd = MoveAvatarEntry::new(1, 0, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries[0].character, "ELLI");
                assert_eq!(t.entries[1].character, "MARIA");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Avatar(t) => {
                assert_eq!(t.entries[0].character, "MARIA");
                assert_eq!(t.entries[1].character, "ELLI");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn move_avatar_entry_new_rejects_same_index() {
        let e = avatar_editor();
        assert!(MoveAvatarEntry::new(0, 0, &e).is_none());
    }

    #[test]
    fn move_avatar_entry_new_returns_none_for_wrong_variant() {
        let e = entity_editor();
        assert!(MoveAvatarEntry::new(0, 1, &e).is_none());
    }

    #[test]
    fn move_avatar_entry_new_returns_none_for_oob_index() {
        let e = avatar_editor();
        assert!(MoveAvatarEntry::new(0, 5, &e).is_none());
        assert!(MoveAvatarEntry::new(5, 0, &e).is_none());
    }
}
