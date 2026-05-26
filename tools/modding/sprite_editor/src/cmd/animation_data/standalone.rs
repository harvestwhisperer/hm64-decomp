//! Commands targeting `AnimationData::Standalone` (per-sprite label tables
//! without a per-entry character/expression split).

use once_cell::unsync::OnceCell;

use crate::cmd::animation_data::{standalone_label_mut, standalone_labels_mut, try_merge_set};
use crate::cmd::{CmdError, Command, DirtyMask};
use crate::model::{AnimationData, StandaloneLabel};
use crate::state::editor::Editor;

#[derive(Debug)]
pub struct SetStandaloneLabel {
    pub entry_idx: usize,
    pub new: String,
    pub old: String,
    applied_at: u128,
}

impl SetStandaloneLabel {
    pub fn new(entry_idx: usize, new: String, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Standalone(s) => s.labels.get(entry_idx)?.label.clone(),
            _ => return None,
        };
        Some(Self { entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetStandaloneLabel {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let l = standalone_label_mut(state, self.entry_idx)?;
        l.label = self.new.clone();
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let l = standalone_label_mut(state, self.entry_idx)?;
        l.label = self.old.clone();
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set standalone entry[{}] label", self.entry_idx)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetStandaloneLabel, [entry_idx], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct SetStandaloneOffset {
    pub entry_idx: usize,
    pub new: u16,
    pub old: u16,
    applied_at: u128,
}

impl SetStandaloneOffset {
    pub fn new(entry_idx: usize, new: u16, editor: &Editor) -> Option<Self> {
        let old = match editor.loaded_animation_data.as_ref()? {
            AnimationData::Standalone(s) => s.labels.get(entry_idx)?.metadata_index,
            _ => return None,
        };
        Some(Self { entry_idx, new, old, applied_at: 0 })
    }
}

impl Command for SetStandaloneOffset {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let l = standalone_label_mut(state, self.entry_idx)?;
        l.metadata_index = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let l = standalone_label_mut(state, self.entry_idx)?;
        l.metadata_index = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set standalone entry[{}] offset", self.entry_idx)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION_DATA
    }
    try_merge_set!(SetStandaloneOffset, [entry_idx], new);
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct AddStandaloneLabel {
    pub at: usize,
    pub entry: StandaloneLabel,
    applied_at: u128,
}

impl AddStandaloneLabel {
    pub fn new(at: usize, entry: StandaloneLabel, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Standalone(s) => {
                if at > s.labels.len() {
                    return None;
                }
            }
            _ => return None,
        }
        Some(Self { at, entry, applied_at: 0 })
    }
}

impl Command for AddStandaloneLabel {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let labels = standalone_labels_mut(state)?;
        if self.at > labels.len() {
            return Err(CmdError::InvalidTarget(format!(
                "insertion index {} > len {}",
                self.at,
                labels.len()
            )));
        }
        labels.insert(self.at, self.entry.clone());
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let labels = standalone_labels_mut(state)?;
        if self.at >= labels.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        labels.remove(self.at);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Add standalone label at entry[{}]", self.at)
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
pub struct RemoveStandaloneLabel {
    pub at: usize,
    pub removed: OnceCell<StandaloneLabel>,
    applied_at: u128,
}

impl RemoveStandaloneLabel {
    pub fn new(at: usize, editor: &Editor) -> Option<Self> {
        match editor.loaded_animation_data.as_ref()? {
            AnimationData::Standalone(s) => {
                let _ = s.labels.get(at)?;
            }
            _ => return None,
        }
        Some(Self { at, removed: OnceCell::new(), applied_at: 0 })
    }
}

impl Command for RemoveStandaloneLabel {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let labels = standalone_labels_mut(state)?;
        if self.at >= labels.len() {
            return Err(CmdError::InvalidTarget(format!(
                "remove index {} >= len {}",
                self.at,
                labels.len()
            )));
        }
        let removed = labels.remove(self.at);
        let _ = self.removed.set(removed);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let removed = self
            .removed
            .get()
            .ok_or_else(|| CmdError::Invariant("revert before apply".into()))?
            .clone();
        let labels = standalone_labels_mut(state)?;
        if self.at > labels.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        labels.insert(self.at, removed);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Remove standalone label entry[{}]", self.at)
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
    use crate::cmd::animation_data::test_fixtures::{avatar_editor, standalone_editor};
    use crate::cmd::history::History;

    #[test]
    fn set_standalone_label_apply_revert() {
        let mut e = standalone_editor();
        let mut cmd = SetStandaloneLabel::new(0, "RENAMED".into(), &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Standalone(s) => assert_eq!(s.labels[0].label, "RENAMED"),
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Standalone(s) => assert_eq!(s.labels[0].label, "FIRST"),
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn set_standalone_offset_merges_same_entry() {
        let mut e = standalone_editor();
        let mut h = History::default();
        h.execute(Box::new(SetStandaloneOffset::new(0, 1, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetStandaloneOffset::new(0, 2, &e).unwrap()), &mut e).unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 1);
    }

    #[test]
    fn add_standalone_label_apply_revert() {
        let mut e = standalone_editor();
        let entry = StandaloneLabel { metadata_index: 99, label: "NEW".into() };
        let mut cmd = AddStandaloneLabel::new(2, entry, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Standalone(s) => {
                assert_eq!(s.labels.len(), 3);
                assert_eq!(s.labels[2].label, "NEW");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Standalone(s) => assert_eq!(s.labels.len(), 2),
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn remove_standalone_label_apply_revert() {
        let mut e = standalone_editor();
        let mut cmd = RemoveStandaloneLabel::new(0, &e).unwrap();
        cmd.apply(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Standalone(s) => {
                assert_eq!(s.labels.len(), 1);
                assert_eq!(s.labels[0].label, "SECOND");
            }
            _ => panic!("wrong variant"),
        }
        cmd.revert(&mut e).unwrap();
        match e.loaded_animation_data.as_ref().unwrap() {
            AnimationData::Standalone(s) => {
                assert_eq!(s.labels.len(), 2);
                assert_eq!(s.labels[0].label, "FIRST");
            }
            _ => panic!("wrong variant"),
        }
    }

    #[test]
    fn standalone_new_returns_none_for_wrong_variant() {
        let e = avatar_editor();
        assert!(SetStandaloneLabel::new(0, "X".into(), &e).is_none());
        assert!(SetStandaloneOffset::new(0, 0, &e).is_none());
        let entry = StandaloneLabel { metadata_index: 0, label: "X".into() };
        assert!(AddStandaloneLabel::new(0, entry, &e).is_none());
        assert!(RemoveStandaloneLabel::new(0, &e).is_none());
    }
}
