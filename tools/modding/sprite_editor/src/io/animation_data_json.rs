//! Loader for animation-data JSON files keyed by sprite kind.
//!
//! Each `SpriteKind` maps to a different on-disk layout:
//!
//!   - `Entity`     → `<sprite_dir>/animationScripts/*.json` (1+ files, sorted)
//!   - `Avatar`     → `<sprite_dir>/characterAvatars.json`   (1 file)
//!   - `Standalone` → `<sprite_dir>/animationLabels.json`    (0..1 file)
//!   - `Unknown`    → no data
//!
//! All cases return `Ok(None)` when the expected files are missing rather
//! than erroring out, since not every sprite has animation data on disk yet.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::{
    AnimationData, AvatarTable, EntityScriptDoc, SpriteKind, StandaloneLabels,
};

pub fn load(sprite_dir: &Path, kind: SpriteKind) -> IoResult<Option<AnimationData>> {
    match kind {
        SpriteKind::Entity => load_entity(sprite_dir),
        SpriteKind::Avatar => load_avatar(sprite_dir),
        SpriteKind::Standalone => load_standalone(sprite_dir),
        SpriteKind::Unknown => Ok(None),
    }
}

fn load_entity(sprite_dir: &Path) -> IoResult<Option<AnimationData>> {
    let scripts_dir = sprite_dir.join("animationScripts");
    if !scripts_dir.is_dir() {
        return Ok(None);
    }

    let read_dir = fs::read_dir(&scripts_dir).map_err(|e| IoError::read(&scripts_dir, e))?;
    let mut json_paths: Vec<_> = read_dir
        .filter_map(|entry| entry.ok())
        .map(|entry| entry.path())
        .filter(|p| p.extension().and_then(|e| e.to_str()) == Some("json"))
        .collect();
    json_paths.sort();

    if json_paths.is_empty() {
        return Ok(None);
    }

    let mut scripts = Vec::with_capacity(json_paths.len());
    for path in &json_paths {
        let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
        let mut doc: EntityScriptDoc =
            serde_json::from_slice(&bytes).map_err(|e| IoError::json(path, e))?;
        // Derive the script `name` from the JSON filename stem (e.g.
        // `bunny.json` → `"bunny"`); the JSON itself doesn't carry this.
        doc.name = path
            .file_stem()
            .and_then(|s| s.to_str())
            .unwrap_or("")
            .to_string();
        scripts.push(doc);
    }
    Ok(Some(AnimationData::Entity { scripts }))
}

fn load_avatar(sprite_dir: &Path) -> IoResult<Option<AnimationData>> {
    let path = sprite_dir.join("characterAvatars.json");
    if !path.is_file() {
        return Ok(None);
    }
    let bytes = fs::read(&path).map_err(|e| IoError::read(&path, e))?;
    let table: AvatarTable =
        serde_json::from_slice(&bytes).map_err(|e| IoError::json(&path, e))?;
    Ok(Some(AnimationData::Avatar(table)))
}

fn load_standalone(sprite_dir: &Path) -> IoResult<Option<AnimationData>> {
    let path = sprite_dir.join("animationLabels.json");
    if !path.is_file() {
        return Ok(None);
    }
    let bytes = fs::read(&path).map_err(|e| IoError::read(&path, e))?;
    let labels: StandaloneLabels =
        serde_json::from_slice(&bytes).map_err(|e| IoError::json(&path, e))?;
    Ok(Some(AnimationData::Standalone(labels)))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::JsonAnimType;
    use std::fs;
    use tempfile::tempdir;

    #[test]
    fn unknown_kind_returns_none() {
        let dir = tempdir().unwrap();
        let res = load(dir.path(), SpriteKind::Unknown).unwrap();
        assert!(res.is_none());
    }

    #[test]
    fn entity_loads_multiple_sorted_scripts() {
        let dir = tempdir().unwrap();
        let scripts_dir = dir.path().join("animationScripts");
        fs::create_dir_all(&scripts_dir).unwrap();
        // Write in non-sorted order; loader must sort by filename.
        fs::write(
            scripts_dir.join("zebra.json"),
            r#"{"kind":"entity","sprite":"holdableItems","animations":[],"trailing_zeros":0}"#,
        )
        .unwrap();
        fs::write(
            scripts_dir.join("apple.json"),
            r#"{
  "kind": "entity",
  "sprite": "holdableItems",
  "animations": [
    {"label":"0","metadata_index":3,"anim_type":"LOOP","flip":false}
  ],
  "trailing_zeros": 2
}"#,
        )
        .unwrap();

        let data = load(dir.path(), SpriteKind::Entity).unwrap().unwrap();
        match data {
            AnimationData::Entity { scripts } => {
                assert_eq!(scripts.len(), 2);
                assert_eq!(scripts[0].sprite, "holdableItems");
                // Sorted by filename: apple before zebra.
                assert_eq!(scripts[0].name, "apple");
                assert_eq!(scripts[1].name, "zebra");
                assert_eq!(scripts[0].animations.len(), 1);
                assert_eq!(scripts[0].animations[0].metadata_index, 3);
                assert_eq!(scripts[0].animations[0].anim_type, JsonAnimType::Loop);
                assert_eq!(scripts[0].trailing_zeros, 2);
                assert_eq!(scripts[1].animations.len(), 0);
            }
            other => panic!("expected Entity variant, got {other:?}"),
        }
    }

    #[test]
    fn entity_missing_dir_returns_none() {
        let dir = tempdir().unwrap();
        let res = load(dir.path(), SpriteKind::Entity).unwrap();
        assert!(res.is_none());
    }

    #[test]
    fn entity_empty_dir_returns_none() {
        let dir = tempdir().unwrap();
        fs::create_dir_all(dir.path().join("animationScripts")).unwrap();
        let res = load(dir.path(), SpriteKind::Entity).unwrap();
        assert!(res.is_none());
    }

    #[test]
    fn avatar_loads_table() {
        let dir = tempdir().unwrap();
        fs::write(
            dir.path().join("characterAvatars.json"),
            r#"{
  "kind": "avatar",
  "entries": [
    {"label":"0","metadata_index":7},
    {"label":"1","metadata_index":11}
  ]
}"#,
        )
        .unwrap();
        let data = load(dir.path(), SpriteKind::Avatar).unwrap().unwrap();
        match data {
            AnimationData::Avatar(t) => {
                assert_eq!(t.kind, "avatar");
                assert_eq!(t.entries.len(), 2);
                assert_eq!(t.entries[1].metadata_index, 11);
            }
            other => panic!("expected Avatar variant, got {other:?}"),
        }
    }

    #[test]
    fn avatar_missing_file_returns_none() {
        let dir = tempdir().unwrap();
        let res = load(dir.path(), SpriteKind::Avatar).unwrap();
        assert!(res.is_none());
    }

    #[test]
    fn standalone_loads_labels() {
        let dir = tempdir().unwrap();
        fs::write(
            dir.path().join("animationLabels.json"),
            r#"{
  "kind": "standalone",
  "sprite": "balloons",
  "labels": [
    {"metadata_index":0,"label":"0"}
  ]
}"#,
        )
        .unwrap();
        let data = load(dir.path(), SpriteKind::Standalone).unwrap().unwrap();
        match data {
            AnimationData::Standalone(l) => {
                assert_eq!(l.sprite, "balloons");
                assert_eq!(l.labels.len(), 1);
                assert_eq!(l.labels[0].metadata_index, 0);
            }
            other => panic!("expected Standalone variant, got {other:?}"),
        }
    }

    #[test]
    fn standalone_missing_file_returns_none() {
        let dir = tempdir().unwrap();
        let res = load(dir.path(), SpriteKind::Standalone).unwrap();
        assert!(res.is_none());
    }

    #[test]
    fn entity_invalid_json_propagates_error() {
        let dir = tempdir().unwrap();
        let scripts_dir = dir.path().join("animationScripts");
        fs::create_dir_all(&scripts_dir).unwrap();
        fs::write(scripts_dir.join("bad.json"), b"not json").unwrap();
        let err = load(dir.path(), SpriteKind::Entity).unwrap_err();
        assert!(matches!(err, IoError::Json { .. }), "got {err:?}");
    }
}
