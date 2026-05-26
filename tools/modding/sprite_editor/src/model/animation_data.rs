//! Animation-data JSON model.
//!
//! These types mirror the per-sprite JSON files emitted by the Python
//! animation pipeline (`tools/libhm64/animations/extractor.py` /
//! `exporter.py`):
//!
//!   - `assets/sprites/<subdir>/<sprite>/animationScripts/<name>.json`     (entity, 1+ files)
//!   - `assets/sprites/overlayScreens/characterAvatars/characterAvatars.json` (avatar, 1 file)
//!   - `assets/sprites/<subdir>/<sprite>/animationLabels.json`             (standalone, 0..1 file)
//!
//! Phase 2b.1 makes these the single source of truth for the editor.

use serde::{Deserialize, Serialize};

/// Animation-type tag as written in the JSON files (SCREAMING_SNAKE_CASE).
#[derive(Copy, Clone, Debug, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "SCREAMING_SNAKE_CASE")]
pub enum JsonAnimType {
    OneShot,
    Loop,
    DestroyOnEnd,
}

/// Number of entries in an entity's directional animation group.
/// `setEntityDirectionalAnimation(base)` plays `base + direction` where
/// direction is the macro from `graphic.h`, so a group is exactly 8 entries.
pub const DIRECTIONAL_GROUP_SIZE: usize = 8;

/// Direction-suffix names for the 8 entries in a directional group, in
/// the order the game's runtime adds them to the base animation index.
/// Labels are **world compass directions** (what the entity is facing in
/// world coords), matching the macros in `src/system/graphic.h`. Note
/// that under the canonical SW-iso projection a slot's art may *visually*
/// appear pointing 45° away from its compass label (e.g. slot 0 / SW
/// looks "south-facing" on screen because the camera sits at world-SW).
pub const DIRECTION_NAMES: [&str; DIRECTIONAL_GROUP_SIZE] =
    ["S", "SW", "W", "NW", "N", "NE", "E", "SE"];

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct EntityAnimation {
    pub label: String,
    pub metadata_index: u16,
    pub anim_type: JsonAnimType,
    pub flip: bool,
    /// Whether this entry is part of an 8-script directional run (SW..S
    /// order; see `DIRECTION_NAMES`). Defaults to `false` for legacy JSON
    /// files that pre-date this field — the editor / exporter is responsible
    /// for backfilling it.
    #[serde(default)]
    pub directional: bool,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct EntityScriptDoc {
    /// The literal `kind` field from the JSON. Always `"entity"` for files in
    /// `animationScripts/`. Read-through; not validated here.
    pub kind: String,
    pub sprite: String,
    pub animations: Vec<EntityAnimation>,
    pub trailing_zeros: u32,
    /// Script identifier derived from the JSON filename stem
    /// (`<sprite_dir>/animationScripts/<name>.json`). Populated by the loader;
    /// not present in the JSON itself, so it is skipped during (de)serialize.
    #[serde(default, skip)]
    pub name: String,
}

/// Avatar-table entry (one of the 143 portrait slots).
///
/// Phase 2b.1 schema: `character` + `expression` replace the old `label` field.
/// Both default to empty so that legacy avatar JSON files (which only had
/// `label` + `metadata_index`) keep parsing — the legacy `label` value is
/// migrated into `expression` so existing data isn't lost.
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
#[serde(from = "AvatarEntryRaw")]
pub struct AvatarEntry {
    #[serde(default)]
    pub character: String,
    #[serde(default)]
    pub expression: String,
    pub metadata_index: u16,
}

#[derive(Deserialize)]
struct AvatarEntryRaw {
    #[serde(default)]
    character: String,
    #[serde(default)]
    expression: String,
    #[serde(default)]
    label: String,
    metadata_index: u16,
}

impl From<AvatarEntryRaw> for AvatarEntry {
    fn from(raw: AvatarEntryRaw) -> Self {
        let expression = if raw.expression.is_empty() && !raw.label.is_empty() {
            raw.label
        } else {
            raw.expression
        };
        AvatarEntry {
            character: raw.character,
            expression,
            metadata_index: raw.metadata_index,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct AvatarTable {
    /// The literal `kind` field from the JSON. Expected to be `"avatar"`.
    pub kind: String,
    pub entries: Vec<AvatarEntry>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct StandaloneLabel {
    pub metadata_index: u16,
    pub label: String,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct StandaloneLabels {
    /// The literal `kind` field from the JSON. Expected to be `"standalone"`.
    pub kind: String,
    pub sprite: String,
    pub labels: Vec<StandaloneLabel>,
}

/// Top-level container holding whichever flavor of animation data was loaded
/// for the current sprite.
#[derive(Debug, Clone, PartialEq)]
pub enum AnimationData {
    /// One or more `<sprite_dir>/animationScripts/*.json` files, sorted by
    /// filename. Empty `scripts` is not valid -- callers should return `None`
    /// instead.
    Entity { scripts: Vec<EntityScriptDoc> },
    Avatar(AvatarTable),
    Standalone(StandaloneLabels),
}

/// If `entry_idx` lies within an 8-entry run of directional entries in
/// `script`, returns the base index of that run. Groups can start at *any*
/// position — `setEntityDirectionalAnimation(base)` indexes from `base`,
/// not from an 8-aligned slot. Detection is left-to-right greedy: the first
/// directional entry whose 8 successors are all directional starts a group;
/// the next group can begin immediately after.
///
/// Returns `None` if `entry_idx` is out of range or isn't part of a complete
/// directional run.
pub fn directional_group_base(script: &EntityScriptDoc, entry_idx: usize) -> Option<usize> {
    let n = script.animations.len();
    if entry_idx >= n {
        return None;
    }
    let mut i = 0;
    while i < n {
        let is_group_start = script.animations[i].directional
            && i + DIRECTIONAL_GROUP_SIZE <= n
            && (i..i + DIRECTIONAL_GROUP_SIZE).all(|j| script.animations[j].directional);
        if is_group_start {
            if entry_idx >= i && entry_idx < i + DIRECTIONAL_GROUP_SIZE {
                return Some(i);
            }
            i += DIRECTIONAL_GROUP_SIZE;
        } else {
            i += 1;
        }
    }
    None
}

/// True when toggling the directional flag ON for an 8-entry run starting at
/// `base_idx` would not overlap any existing directional entry. Used to
/// validate group creation. Group disbanding (toggling OFF) uses
/// `directional_group_base` to locate the actual base instead.
pub fn can_form_directional_group(script: &EntityScriptDoc, base_idx: usize) -> bool {
    let n = script.animations.len();
    base_idx + DIRECTIONAL_GROUP_SIZE <= n
        && (base_idx..base_idx + DIRECTIONAL_GROUP_SIZE)
            .all(|j| !script.animations[j].directional)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn entity_doc_round_trips_real_fixture_slice() {
        let json = r#"{
  "kind": "entity",
  "sprite": "bunny",
  "animations": [
    { "label": "0", "metadata_index": 14, "anim_type": "LOOP", "flip": false, "directional": false },
    { "label": "25", "metadata_index": 0, "anim_type": "ONE_SHOT", "flip": true, "directional": true }
  ],
  "trailing_zeros": 6
}"#;
        let doc: EntityScriptDoc = serde_json::from_str(json).unwrap();
        assert_eq!(doc.kind, "entity");
        assert_eq!(doc.sprite, "bunny");
        assert_eq!(doc.animations.len(), 2);
        assert_eq!(doc.animations[0].metadata_index, 14);
        assert_eq!(doc.animations[0].anim_type, JsonAnimType::Loop);
        assert!(!doc.animations[0].flip);
        assert!(!doc.animations[0].directional);
        assert_eq!(doc.animations[1].anim_type, JsonAnimType::OneShot);
        assert!(doc.animations[1].flip);
        assert!(doc.animations[1].directional);
        assert_eq!(doc.trailing_zeros, 6);

        // Round-trip.
        let s = serde_json::to_string(&doc).unwrap();
        let back: EntityScriptDoc = serde_json::from_str(&s).unwrap();
        assert_eq!(doc, back);
    }

    #[test]
    fn entity_animation_directional_defaults_false_when_absent() {
        let json = r#"{ "label": "0", "metadata_index": 1, "anim_type": "LOOP", "flip": false }"#;
        let a: EntityAnimation = serde_json::from_str(json).unwrap();
        assert!(!a.directional);
    }

    #[test]
    fn entity_anim_type_destroy_on_end_serializes_screaming_snake() {
        let a = EntityAnimation {
            label: "x".into(),
            metadata_index: 1,
            anim_type: JsonAnimType::DestroyOnEnd,
            flip: false,
            directional: false,
        };
        let s = serde_json::to_string(&a).unwrap();
        assert!(s.contains("\"DESTROY_ON_END\""), "got: {s}");
    }

    #[test]
    fn avatar_doc_round_trips_real_fixture_slice() {
        let json = r#"{
  "kind": "avatar",
  "entries": [
    { "character": "MARIA", "expression": "HAPPY", "metadata_index": 7 },
    { "character": "ELLI", "expression": "SAD", "metadata_index": 11 }
  ]
}"#;
        let doc: AvatarTable = serde_json::from_str(json).unwrap();
        assert_eq!(doc.kind, "avatar");
        assert_eq!(doc.entries.len(), 2);
        assert_eq!(doc.entries[0].metadata_index, 7);
        assert_eq!(doc.entries[0].character, "MARIA");
        assert_eq!(doc.entries[0].expression, "HAPPY");
        assert_eq!(doc.entries[1].character, "ELLI");

        let s = serde_json::to_string(&doc).unwrap();
        let back: AvatarTable = serde_json::from_str(&s).unwrap();
        assert_eq!(doc, back);
    }

    #[test]
    fn avatar_entry_legacy_label_migrates_to_expression() {
        // Old schema: only `label` + `metadata_index`.
        let json = r#"{"label": "0", "metadata_index": 7}"#;
        let e: AvatarEntry = serde_json::from_str(json).unwrap();
        assert_eq!(e.character, "");
        assert_eq!(e.expression, "0");
        assert_eq!(e.metadata_index, 7);
    }

    #[test]
    fn avatar_entry_new_schema_parses() {
        let json = r#"{"character": "MARIA", "expression": "HAPPY", "metadata_index": 7}"#;
        let e: AvatarEntry = serde_json::from_str(json).unwrap();
        assert_eq!(e.character, "MARIA");
        assert_eq!(e.expression, "HAPPY");
        assert_eq!(e.metadata_index, 7);
    }

    #[test]
    fn avatar_entry_explicit_expression_wins_over_label() {
        // If both are present, expression takes precedence; label is ignored.
        let json = r#"{"label": "OLD", "expression": "NEW", "metadata_index": 1}"#;
        let e: AvatarEntry = serde_json::from_str(json).unwrap();
        assert_eq!(e.expression, "NEW");
    }

    #[test]
    fn standalone_doc_round_trips_real_fixture_slice() {
        let json = r#"{
  "kind": "standalone",
  "sprite": "balloons",
  "labels": [
    { "metadata_index": 0, "label": "0" }
  ]
}"#;
        let doc: StandaloneLabels = serde_json::from_str(json).unwrap();
        assert_eq!(doc.kind, "standalone");
        assert_eq!(doc.sprite, "balloons");
        assert_eq!(doc.labels.len(), 1);
        assert_eq!(doc.labels[0].metadata_index, 0);

        let s = serde_json::to_string(&doc).unwrap();
        let back: StandaloneLabels = serde_json::from_str(&s).unwrap();
        assert_eq!(doc, back);
    }
}
