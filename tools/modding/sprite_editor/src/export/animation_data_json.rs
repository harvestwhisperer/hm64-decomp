//! Writers for the per-sprite animation-data JSON files.
//!
//! Mirrors the Python extractor's output style: 2-space indent, trailing
//! newline. The exact filenames are:
//!
//!   - entity     → `<sprite_dir>/animationScripts/<doc.name>.json`
//!   - avatar     → `<sprite_dir>/characterAvatars.json`
//!   - standalone → `<sprite_dir>/animationLabels.json`
//!
//! For `AnimationData::Entity` the dispatcher writes *every* script in the
//! vec, since multi-script sprites (e.g. `holdableItems`) keep one JSON file
//! per script.

use std::fs;
use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{AnimationData, AvatarTable, EntityScriptDoc, StandaloneLabels};

/// Serialize `value` with `serde_json::to_string_pretty` and append a trailing
/// newline. Matches the Python extractor's per-file convention.
fn pretty_with_trailing_newline<T: serde::Serialize>(
    value: &T,
    path: &Path,
) -> ExportResult<Vec<u8>> {
    let mut text =
        serde_json::to_string_pretty(value).map_err(|e| ExportError::serialize(path, e))?;
    text.push('\n');
    Ok(text.into_bytes())
}

pub fn write_entity_script(sprite_dir: &Path, doc: &EntityScriptDoc) -> ExportResult<()> {
    let scripts_dir = sprite_dir.join("animationScripts");
    if !scripts_dir.exists() {
        fs::create_dir_all(&scripts_dir).map_err(|e| ExportError::write(&scripts_dir, e))?;
    }
    let filename = if doc.name.is_empty() {
        // Defensive fallback: shouldn't happen because the loader populates
        // `name` from the file stem, but if it does, derive a stable name from
        // the sprite field rather than emitting `.json`.
        format!("{}.json", if doc.sprite.is_empty() { "script" } else { doc.sprite.as_str() })
    } else {
        format!("{}.json", doc.name)
    };
    let path = scripts_dir.join(filename);
    let bytes = pretty_with_trailing_newline(doc, &path)?;
    atomic_write(&path, &bytes).map_err(|e| ExportError::write(&path, e))
}

pub fn write_avatar_table(sprite_dir: &Path, table: &AvatarTable) -> ExportResult<()> {
    let path = sprite_dir.join("characterAvatars.json");
    let bytes = pretty_with_trailing_newline(table, &path)?;
    atomic_write(&path, &bytes).map_err(|e| ExportError::write(&path, e))
}

pub fn write_standalone_labels(
    sprite_dir: &Path,
    labels: &StandaloneLabels,
) -> ExportResult<()> {
    let path = sprite_dir.join("animationLabels.json");
    let bytes = pretty_with_trailing_newline(labels, &path)?;
    atomic_write(&path, &bytes).map_err(|e| ExportError::write(&path, e))
}

/// Save the entire animation-data payload for the sprite at `sprite_dir`.
/// For multi-script entity sprites this writes one file per script.
pub fn save_animation_data(sprite_dir: &Path, data: &AnimationData) -> ExportResult<()> {
    match data {
        AnimationData::Entity { scripts } => {
            for doc in scripts {
                write_entity_script(sprite_dir, doc)?;
            }
            Ok(())
        }
        AnimationData::Avatar(t) => write_avatar_table(sprite_dir, t),
        AnimationData::Standalone(s) => write_standalone_labels(sprite_dir, s),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::io::animation_data_json::load;
    use crate::model::{
        AvatarEntry, AvatarTable, EntityAnimation, EntityScriptDoc, JsonAnimType, SpriteKind,
        StandaloneLabel, StandaloneLabels,
    };
    use tempfile::tempdir;

    #[test]
    fn entity_round_trip_via_io_loader() {
        let dir = tempdir().unwrap();
        let doc = EntityScriptDoc {
            kind: "entity".into(),
            sprite: "bunny".into(),
            animations: vec![
                EntityAnimation {
                    label: "0".into(),
                    metadata_index: 14,
                    anim_type: JsonAnimType::Loop,
                    flip: false,
                    directional: false,
                },
                EntityAnimation {
                    label: "25".into(),
                    metadata_index: 0,
                    anim_type: JsonAnimType::OneShot,
                    flip: true,
                    directional: true,
                },
            ],
            trailing_zeros: 6,
            name: "main".into(),
        };
        let data = AnimationData::Entity { scripts: vec![doc.clone()] };
        save_animation_data(dir.path(), &data).unwrap();

        let loaded = load(dir.path(), SpriteKind::Entity).unwrap().unwrap();
        match loaded {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts.len(), 1);
                // The loader fills `name` from the filename, matching what we wrote.
                assert_eq!(scripts[0].name, "main");
                assert_eq!(scripts[0], doc);
            }
            _ => panic!("expected Entity"),
        }
    }

    #[test]
    fn entity_multi_script_writes_one_file_per_script() {
        let dir = tempdir().unwrap();
        let docs = vec![
            EntityScriptDoc {
                kind: "entity".into(),
                sprite: "holdableItems".into(),
                animations: vec![],
                trailing_zeros: 0,
                name: "apple".into(),
            },
            EntityScriptDoc {
                kind: "entity".into(),
                sprite: "holdableItems".into(),
                animations: vec![EntityAnimation {
                    label: "x".into(),
                    metadata_index: 1,
                    anim_type: JsonAnimType::Loop,
                    flip: false,
                    directional: false,
                }],
                trailing_zeros: 0,
                name: "banana".into(),
            },
        ];
        save_animation_data(dir.path(), &AnimationData::Entity { scripts: docs.clone() }).unwrap();

        let loaded = load(dir.path(), SpriteKind::Entity).unwrap().unwrap();
        match loaded {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts.len(), 2);
                assert_eq!(scripts[0].name, "apple");
                assert_eq!(scripts[1].name, "banana");
            }
            _ => panic!("expected Entity"),
        }
    }

    #[test]
    fn avatar_round_trip_via_io_loader() {
        let dir = tempdir().unwrap();
        let table = AvatarTable {
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
        };
        save_animation_data(dir.path(), &AnimationData::Avatar(table.clone())).unwrap();

        let loaded = load(dir.path(), SpriteKind::Avatar).unwrap().unwrap();
        match loaded {
            AnimationData::Avatar(t) => assert_eq!(t, table),
            _ => panic!("expected Avatar"),
        }
    }

    #[test]
    fn standalone_round_trip_via_io_loader() {
        let dir = tempdir().unwrap();
        let labels = StandaloneLabels {
            kind: "standalone".into(),
            sprite: "balloons".into(),
            labels: vec![
                StandaloneLabel { metadata_index: 0, label: "0".into() },
                StandaloneLabel { metadata_index: 4, label: "1".into() },
            ],
        };
        save_animation_data(dir.path(), &AnimationData::Standalone(labels.clone())).unwrap();

        let loaded = load(dir.path(), SpriteKind::Standalone).unwrap().unwrap();
        match loaded {
            AnimationData::Standalone(s) => assert_eq!(s, labels),
            _ => panic!("expected Standalone"),
        }
    }

    #[test]
    fn entity_creates_animation_scripts_dir_if_missing() {
        let dir = tempdir().unwrap();
        let doc = EntityScriptDoc {
            kind: "entity".into(),
            sprite: "fresh".into(),
            animations: vec![],
            trailing_zeros: 0,
            name: "fresh".into(),
        };
        // No subdir yet — writer must create it.
        assert!(!dir.path().join("animationScripts").exists());
        write_entity_script(dir.path(), &doc).unwrap();
        assert!(dir.path().join("animationScripts/fresh.json").is_file());
    }

    #[test]
    fn output_has_trailing_newline_and_two_space_indent() {
        let dir = tempdir().unwrap();
        let labels = StandaloneLabels {
            kind: "standalone".into(),
            sprite: "x".into(),
            labels: vec![StandaloneLabel { metadata_index: 0, label: "L".into() }],
        };
        save_animation_data(dir.path(), &AnimationData::Standalone(labels)).unwrap();
        let text =
            std::fs::read_to_string(dir.path().join("animationLabels.json")).unwrap();
        assert!(text.ends_with('\n'), "expected trailing newline, got: {text:?}");
        // serde_json::to_string_pretty defaults to 2-space indent; spot-check.
        assert!(text.contains("\n  \"kind\""));
    }
}
