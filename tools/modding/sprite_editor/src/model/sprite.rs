//! Manifest, Texture, and Palette types for sprite assets.
//!
//! The `SpriteAsset` container lands in B5 once the `Animation` type exists.

use std::collections::BTreeMap;
use std::path::PathBuf;

use serde::{Deserialize, Serialize};

use super::Animation;
use crate::model::rgba::Rgba;
use crate::model::texture_format::{SpriteType, TextureFormat};

/// Classification of a sprite asset based on how its animation script (if any)
/// is exported. Mirrors `SpriteInfo.kind` in `libhm64/sprites/addresses.py`.
#[derive(Copy, Clone, Debug, PartialEq, Eq, Default, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum SpriteKind {
    Entity,
    Avatar,
    Standalone,
    #[default]
    Unknown,
}

/// Per-palette metadata recorded by the Python extractor and preserved here
/// so the Rust assembler can reproduce the original byte layout. Keyed in the
/// JSON by stringified palette index ("0", "1", …).
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct PaletteMeta {
    pub color_count: u32,
    pub format: TextureFormat,
}

/// `manifest.json` schema. Field order and names match the existing on-disk
/// format produced by `libhm64/sprites/extractor.py`.
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Manifest {
    pub label: String,
    pub subdir: String,
    #[serde(default)]
    pub kind: SpriteKind,
    #[serde(rename = "type")]
    pub sprite_type: SpriteType,
    pub sprite_count: u32,
    #[serde(default)]
    pub spritesheet_index_count: u32,
    pub palette_count: u32,
    #[serde(default)]
    pub palette_index_count: u32,
    pub animation_count: u32,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub animation_index: Vec<u32>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub palette_index: Vec<u32>,
    /// Per-palette metadata (color_count, format). Optional; defaults to empty
    /// for sprites authored from scratch.
    #[serde(default, skip_serializing_if = "BTreeMap::is_empty")]
    pub palettes: BTreeMap<String, PaletteMeta>,
    /// Sprite-to-palette mapping. One byte per sprite index. Populated for
    /// every sprite extracted from the ROM and for every sprite created by the
    /// wizard; the empty form only appears in defaults and in the defensive
    /// `finalize_simple` extraction fallback for malformed addresses. When
    /// non-empty, length must equal `textures.len()` and entry `i` must equal
    /// `textures[i].palette_index` — editors that mutate `textures` must keep
    /// this field in sync.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub sprite_to_palette: Vec<u8>,
    /// Tail bytes for the palette index when the original ROM stored a
    /// non-u32-aligned terminator.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub palette_index_end_bytes: Vec<u8>,
    /// Tail bytes for the spritesheet index, same purpose as above.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub spritesheet_index_end_bytes: Vec<u8>,
    /// Explicit padding bytes appended after the s2p section.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub s2p_padding: Vec<u8>,
    /// Trailing bytes appended after the texture binary's last section.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub texture_end_padding: Vec<u8>,
    /// Original ROM section offsets, used as a fallback when sizing s2p
    /// padding for sprites that pre-date `s2p_padding`.
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub asset_offsets: Vec<u32>,
    pub textures: Vec<TextureMeta>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct TextureMeta {
    pub index: u32,
    pub width: u16,
    pub height: u16,
    pub format: TextureFormat,
    #[serde(default)]
    pub palette_index: u32,
}

/// A CI texture: raw pixel indices plus metadata mirrored from `TextureMeta`.
#[derive(Debug, Clone, PartialEq)]
pub struct Texture {
    pub index: u32,
    pub width: u16,
    pub height: u16,
    pub format: TextureFormat,
    pub palette_index: u32,
    /// Raw palette indices. Length = w*h (CI8) or w*h/2 rounded up (CI4).
    pub pixels: Vec<u8>,
}

/// A color palette. Length is 16 (CI4) or 256 (CI8).
#[derive(Debug, Clone, PartialEq)]
pub struct Palette {
    pub index: u32,
    pub format: TextureFormat,
    pub colors: Vec<Rgba>,
}

/// In-memory representation of a single asset directory under
/// `assets/sprites/<subdir>/<name>/`.
#[derive(Debug, Clone, PartialEq)]
pub struct SpriteAsset {
    pub dir: PathBuf,
    pub manifest: Manifest,
    pub textures: BTreeMap<u32, Texture>,
    pub palettes: BTreeMap<u32, Palette>,
    pub animations: BTreeMap<u32, Animation>,
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn manifest_minimum_example_round_trip() {
        let m = Manifest {
            label: "player".into(),
            subdir: "entitySprites".into(),
            kind: SpriteKind::Entity,
            sprite_type: SpriteType::Type1,
            sprite_count: 2,
            spritesheet_index_count: 2,
            palette_count: 1,
            palette_index_count: 1,
            animation_count: 1,
            animation_index: vec![0],
            palette_index: vec![0],
            palettes: BTreeMap::new(),
            sprite_to_palette: vec![],
            palette_index_end_bytes: vec![],
            spritesheet_index_end_bytes: vec![],
            s2p_padding: vec![],
            texture_end_padding: vec![],
            asset_offsets: vec![],
            textures: vec![TextureMeta {
                index: 0,
                width: 32,
                height: 32,
                format: TextureFormat::Ci8,
                palette_index: 0,
            }],
        };
        let s = serde_json::to_string(&m).unwrap();
        let back: Manifest = serde_json::from_str(&s).unwrap();
        assert_eq!(m, back);
    }

    #[test]
    fn manifest_type_field_renamed() {
        let m = Manifest {
            label: "x".into(),
            subdir: "y".into(),
            kind: SpriteKind::Standalone,
            sprite_type: SpriteType::Type2,
            sprite_count: 0,
            spritesheet_index_count: 0,
            palette_count: 0,
            palette_index_count: 0,
            animation_count: 0,
            animation_index: vec![],
            palette_index: vec![],
            palettes: BTreeMap::new(),
            sprite_to_palette: vec![],
            palette_index_end_bytes: vec![],
            spritesheet_index_end_bytes: vec![],
            s2p_padding: vec![],
            texture_end_padding: vec![],
            asset_offsets: vec![],
            textures: vec![],
        };
        let s = serde_json::to_string(&m).unwrap();
        assert!(s.contains("\"type\":\"type-2\""), "got: {s}");
        assert!(s.contains("\"kind\":\"standalone\""), "got: {s}");
    }

    #[test]
    fn manifest_preserves_assembler_fields() {
        let json = r#"{
            "label": "bird",
            "subdir": "entitySprites/animals",
            "kind": "entity",
            "type": "type-1",
            "sprite_count": 2,
            "spritesheet_index_count": 4,
            "palette_count": 1,
            "palette_index_count": 2,
            "animation_count": 1,
            "animation_index": [0, 32],
            "palette_index": [16, 0],
            "palette_index_end_bytes": [0, 0, 0, 0],
            "s2p_padding": [255, 255],
            "texture_end_padding": [],
            "spritesheet_index_end_bytes": [0, 0, 0, 0],
            "asset_offsets": [0, 16, 32, 48, 64, 0, 0, 0],
            "palettes": { "0": { "color_count": 16, "format": "ci4" } },
            "sprite_to_palette": [0, 0],
            "textures": []
        }"#;
        let m: Manifest = serde_json::from_str(json).unwrap();
        assert_eq!(m.palette_index_end_bytes, vec![0, 0, 0, 0]);
        assert_eq!(m.s2p_padding, vec![255, 255]);
        assert_eq!(m.spritesheet_index_end_bytes, vec![0, 0, 0, 0]);
        assert_eq!(m.asset_offsets, vec![0, 16, 32, 48, 64, 0, 0, 0]);
        assert_eq!(m.sprite_to_palette, vec![0, 0]);
        assert_eq!(m.palettes.get("0").unwrap().color_count, 16);
        assert_eq!(m.palettes.get("0").unwrap().format, TextureFormat::Ci4);
    }

    #[test]
    fn manifest_with_kind_field_parses() {
        let json = r#"{
            "label": "bunny",
            "subdir": "entitySprites/animals",
            "kind": "entity",
            "type": "type-1",
            "sprite_count": 0,
            "palette_count": 0,
            "animation_count": 0,
            "textures": []
        }"#;
        let m: Manifest = serde_json::from_str(json).unwrap();
        assert_eq!(m.kind, SpriteKind::Entity);
        assert_eq!(m.label, "bunny");
    }

    #[test]
    fn manifest_without_kind_field_defaults_to_unknown() {
        // Transition compatibility: older manifests without "kind" should still parse.
        let json = r#"{
            "label": "x",
            "subdir": "y",
            "type": "type-2",
            "sprite_count": 0,
            "palette_count": 0,
            "animation_count": 0,
            "textures": []
        }"#;
        let m: Manifest = serde_json::from_str(json).unwrap();
        assert_eq!(m.kind, SpriteKind::Unknown);
    }
}
