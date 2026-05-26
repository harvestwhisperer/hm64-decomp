//! Commands that mutate `Editor::loaded_animation_data` (entity / avatar /
//! standalone). Submodules group commands by the variant they target; helpers
//! that locate the right slice of `loaded_animation_data` live here so each
//! submodule can `use super::*` instead of duplicating the variant matches.

pub mod avatar;
pub mod entity;
pub mod standalone;

pub use avatar::*;
pub use entity::*;
pub use standalone::*;

use crate::cmd::CmdError;
use crate::model::{AnimationData, AvatarEntry, EntityAnimation, StandaloneLabel};
use crate::state::editor::Editor;

/// Generates a `try_merge` impl for "Set <field>" commands that target a
/// single entry identified by one or more key fields. Two commands of the
/// same type merge when every key field matches; the more recent `new` value
/// wins. Only suitable for value-edit commands — insert/remove/move have
/// non-trivial merge semantics and stay hand-written.
macro_rules! try_merge_set {
    ($Self:ident, [$($key:ident),+ $(,)?], $field:ident) => {
        fn try_merge(
            &mut self,
            other: &dyn $crate::cmd::Command,
        ) -> $crate::cmd::MergeResult {
            let Some(o) = (other as &dyn std::any::Any).downcast_ref::<$Self>() else {
                return $crate::cmd::MergeResult::Separate;
            };
            if $(o.$key == self.$key)&&+ {
                self.$field = o.$field.clone();
                $crate::cmd::MergeResult::Merged
            } else {
                $crate::cmd::MergeResult::Separate
            }
        }
    };
}
pub(crate) use try_merge_set;

pub(crate) fn entity_anim_mut(
    state: &mut Editor,
    script_idx: usize,
    entry_idx: usize,
) -> Result<&mut EntityAnimation, CmdError> {
    let data = state
        .loaded_animation_data
        .as_mut()
        .ok_or_else(|| CmdError::InvalidTarget("no animation data loaded".into()))?;
    let scripts = match data {
        AnimationData::Entity { scripts } => scripts,
        _ => {
            return Err(CmdError::InvalidTarget(
                "loaded animation data is not an entity script".into(),
            ))
        }
    };
    let doc = scripts
        .get_mut(script_idx)
        .ok_or_else(|| CmdError::InvalidTarget(format!("script index {script_idx} out of range")))?;
    doc.animations
        .get_mut(entry_idx)
        .ok_or_else(|| CmdError::InvalidTarget(format!("entry index {entry_idx} out of range")))
}

pub(crate) fn entity_script_mut(
    state: &mut Editor,
    script_idx: usize,
) -> Result<&mut Vec<EntityAnimation>, CmdError> {
    let data = state
        .loaded_animation_data
        .as_mut()
        .ok_or_else(|| CmdError::InvalidTarget("no animation data loaded".into()))?;
    let scripts = match data {
        AnimationData::Entity { scripts } => scripts,
        _ => {
            return Err(CmdError::InvalidTarget(
                "loaded animation data is not an entity script".into(),
            ))
        }
    };
    let doc = scripts
        .get_mut(script_idx)
        .ok_or_else(|| CmdError::InvalidTarget(format!("script index {script_idx} out of range")))?;
    Ok(&mut doc.animations)
}

pub(crate) fn avatar_entries_mut(
    state: &mut Editor,
) -> Result<&mut Vec<AvatarEntry>, CmdError> {
    let data = state
        .loaded_animation_data
        .as_mut()
        .ok_or_else(|| CmdError::InvalidTarget("no animation data loaded".into()))?;
    let table = match data {
        AnimationData::Avatar(t) => t,
        _ => {
            return Err(CmdError::InvalidTarget(
                "loaded animation data is not an avatar table".into(),
            ))
        }
    };
    Ok(&mut table.entries)
}

pub(crate) fn avatar_entry_mut(
    state: &mut Editor,
    entry_idx: usize,
) -> Result<&mut AvatarEntry, CmdError> {
    let data = state
        .loaded_animation_data
        .as_mut()
        .ok_or_else(|| CmdError::InvalidTarget("no animation data loaded".into()))?;
    let table = match data {
        AnimationData::Avatar(t) => t,
        _ => {
            return Err(CmdError::InvalidTarget(
                "loaded animation data is not an avatar table".into(),
            ))
        }
    };
    table
        .entries
        .get_mut(entry_idx)
        .ok_or_else(|| CmdError::InvalidTarget(format!("avatar entry {entry_idx} out of range")))
}

pub(crate) fn standalone_labels_mut(
    state: &mut Editor,
) -> Result<&mut Vec<StandaloneLabel>, CmdError> {
    let data = state
        .loaded_animation_data
        .as_mut()
        .ok_or_else(|| CmdError::InvalidTarget("no animation data loaded".into()))?;
    let labels = match data {
        AnimationData::Standalone(s) => s,
        _ => {
            return Err(CmdError::InvalidTarget(
                "loaded animation data is not a standalone label table".into(),
            ))
        }
    };
    Ok(&mut labels.labels)
}

pub(crate) fn standalone_label_mut(
    state: &mut Editor,
    entry_idx: usize,
) -> Result<&mut StandaloneLabel, CmdError> {
    let labels = standalone_labels_mut(state)?;
    labels
        .get_mut(entry_idx)
        .ok_or_else(|| CmdError::InvalidTarget(format!("label index {entry_idx} out of range")))
}

#[cfg(test)]
pub(crate) mod test_fixtures {
    use crate::model::{
        AnimationData, AvatarEntry, AvatarTable, EntityAnimation, EntityScriptDoc, JsonAnimType,
        StandaloneLabel, StandaloneLabels,
    };
    use crate::state::editor::Editor;

    pub fn entity_editor() -> Editor {
        let mut e = Editor::new();
        e.loaded_animation_data = Some(AnimationData::Entity {
            scripts: vec![
                EntityScriptDoc {
                    kind: "entity".into(),
                    sprite: "test".into(),
                    animations: vec![
                        EntityAnimation {
                            label: "0".into(),
                            metadata_index: 0,
                            anim_type: JsonAnimType::Loop,
                            flip: false,
                            directional: false,
                        },
                        EntityAnimation {
                            label: "1".into(),
                            metadata_index: 1,
                            anim_type: JsonAnimType::OneShot,
                            flip: true,
                            directional: false,
                        },
                    ],
                    trailing_zeros: 0,
                    name: "main".into(),
                },
                EntityScriptDoc {
                    kind: "entity".into(),
                    sprite: "test".into(),
                    animations: vec![EntityAnimation {
                        label: "B0".into(),
                        metadata_index: 5,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: false,
                    }],
                    trailing_zeros: 0,
                    name: "second".into(),
                },
            ],
        });
        e.current_entity_script_idx = 0;
        e
    }

    pub fn avatar_editor() -> Editor {
        let mut e = Editor::new();
        e.loaded_animation_data = Some(AnimationData::Avatar(AvatarTable {
            kind: "avatar".into(),
            entries: vec![
                AvatarEntry {
                    character: "MARIA".into(),
                    expression: "HAPPY".into(),
                    metadata_index: 7,
                },
                AvatarEntry {
                    character: "ELLI".into(),
                    expression: "SAD".into(),
                    metadata_index: 11,
                },
            ],
        }));
        e
    }

    pub fn standalone_editor() -> Editor {
        let mut e = Editor::new();
        e.loaded_animation_data = Some(AnimationData::Standalone(StandaloneLabels {
            kind: "standalone".into(),
            sprite: "balloons".into(),
            labels: vec![
                StandaloneLabel { metadata_index: 0, label: "FIRST".into() },
                StandaloneLabel { metadata_index: 4, label: "SECOND".into() },
            ],
        }));
        e
    }
}
