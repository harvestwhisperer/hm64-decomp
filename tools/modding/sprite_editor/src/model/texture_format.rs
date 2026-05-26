//! Texture format and sprite type enums, shared across model, I/O, and UI.

use serde::{Deserialize, Serialize};

#[derive(Copy, Clone, Debug, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "lowercase")]
pub enum TextureFormat {
    /// 4 bits per pixel, 16-color palette.
    Ci4,
    /// 8 bits per pixel, 256-color palette.
    Ci8,
}

impl TextureFormat {
    pub const fn palette_len(self) -> usize {
        match self {
            TextureFormat::Ci4 => 16,
            TextureFormat::Ci8 => 256,
        }
    }

    pub const fn pixel_bytes(self, width: u16, height: u16) -> usize {
        let pixels = width as usize * height as usize;
        match self {
            TextureFormat::Ci4 => pixels.div_ceil(2),
            TextureFormat::Ci8 => pixels,
        }
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "kebab-case")]
pub enum SpriteType {
    /// Sprites with a `spritesheet_index_base..end` range (address CSV type-1).
    #[serde(rename = "type-1")]
    Type1,
    /// Sprites with an `addr_extra` pointer (address CSV type-2).
    #[serde(rename = "type-2")]
    Type2,
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ci4_palette_len() {
        assert_eq!(TextureFormat::Ci4.palette_len(), 16);
    }

    #[test]
    fn ci8_palette_len() {
        assert_eq!(TextureFormat::Ci8.palette_len(), 256);
    }

    #[test]
    fn ci4_pixel_bytes_odd_count() {
        // 3x3 = 9 pixels → 5 bytes (rounded up)
        assert_eq!(TextureFormat::Ci4.pixel_bytes(3, 3), 5);
    }

    #[test]
    fn ci8_pixel_bytes() {
        assert_eq!(TextureFormat::Ci8.pixel_bytes(32, 32), 1024);
    }

    #[test]
    fn texture_format_serializes_lowercase() {
        assert_eq!(serde_json::to_string(&TextureFormat::Ci4).unwrap(), "\"ci4\"");
        assert_eq!(serde_json::to_string(&TextureFormat::Ci8).unwrap(), "\"ci8\"");
    }

    #[test]
    fn sprite_type_serializes_with_dash() {
        assert_eq!(serde_json::to_string(&SpriteType::Type1).unwrap(), "\"type-1\"");
        assert_eq!(serde_json::to_string(&SpriteType::Type2).unwrap(), "\"type-2\"");
    }
}
