//! Pure-Rust domain types for sprite assets, animations, and scripts.
//!
//! This module has zero dependencies on Slint, I/O, or the command layer.
//! All types are serde-derivable for straightforward JSON I/O in the sibling
//! `io` and `export` modules.

pub mod animation;
pub mod animation_data;
pub mod font;
pub mod palette_eq;
pub mod rgba;
pub mod sprite;
pub mod texture_format;

pub use animation::{Animation, AnimationFrame, FrameSprite};
pub use animation_data::{
    can_form_directional_group, directional_group_base, AnimationData, AvatarEntry, AvatarTable,
    EntityAnimation, EntityScriptDoc, JsonAnimType, StandaloneLabel, StandaloneLabels,
    DIRECTIONAL_GROUP_SIZE, DIRECTION_NAMES,
};
pub use font::{
    extract_unused_bytes, pack_glyph, pack_glyph_full, swap_bottom, swap_top, unpack_glyph,
    unpack_glyph_at, unswap_bottom, unswap_top, ActiveAsset, FontAsset, FontGlyph,
    FontPalette, GlyphUnusedBytes, BYTES_PER_GLYPH, GLYPH_HEIGHT, GLYPH_PIXELS, GLYPH_WIDTH,
    PALETTE_LIVE_COLORS, PALETTE_TOTAL_COLORS,
};
pub use palette_eq::find_matching_palette;
pub use rgba::Rgba;
pub use sprite::{Manifest, Palette, PaletteMeta, SpriteAsset, SpriteKind, Texture, TextureMeta};
pub use texture_format::{SpriteType, TextureFormat};
