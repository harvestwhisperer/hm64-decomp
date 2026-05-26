//! CI4/CI8 texture decoding from raw ROM bytes. Mirrors
//! `tools/libhm64/sprites/textures.py::extract_texture`.

use crate::io::binary::read_s16_le;
use crate::model::TextureFormat;

#[derive(Debug, thiserror::Error)]
pub enum TextureError {
    #[error("offset {offset}: out of range (rom len {len})")]
    OutOfRange { offset: usize, len: usize },
}

#[derive(Debug, Clone, PartialEq)]
pub struct DecodedTexture {
    pub width: i16,
    pub height: i16,
    pub format: TextureFormat,
    /// Raw pixel bytes between header end and the next sprite's offset.
    pub pixels: Vec<u8>,
}

/// Decode the texture at `spritesheet_offsets[sprite_idx]`. Returns `None`
/// when the slot is empty (offset == next_offset, or out of range).
///
/// `addr_base` is the absolute ROM offset where the sprite's texture binary
/// starts; offsets are relative to that.
pub fn decode_texture(
    rom: &[u8],
    addr_base: u32,
    spritesheet_offsets: &[u32],
    sprite_idx: usize,
) -> Option<DecodedTexture> {
    if sprite_idx + 1 >= spritesheet_offsets.len() {
        return None;
    }
    let offset = spritesheet_offsets[sprite_idx];
    let next_offset = spritesheet_offsets[sprite_idx + 1];
    if offset == next_offset {
        return None;
    }
    let sprite_addr = (addr_base + offset) as usize;

    // Header: 8 bytes
    //   bytes 0..3: "46d" magic (validated by extractor; not enforced here)
    //   byte 3: format flag (0x10 = ci4, else ci8)
    //   bytes 4..6: width (s16 LE)
    //   bytes 6..8: height (s16 LE)
    let flags = rom.get(sprite_addr + 3).copied()?;
    let width = read_s16_le(rom, sprite_addr + 4)?;
    let height = read_s16_le(rom, sprite_addr + 6)?;
    let format = if flags == 0x10 { TextureFormat::Ci4 } else { TextureFormat::Ci8 };

    let data_start = sprite_addr + 8;
    let data_end = (addr_base + next_offset) as usize;
    if data_end <= data_start {
        return Some(DecodedTexture { width, height, format, pixels: Vec::new() });
    }
    let pixels = rom.get(data_start..data_end)?.to_vec();
    Some(DecodedTexture { width, height, format, pixels })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn decodes_synthetic_ci4_texture() {
        // Construct a minimal ROM with one CI4 4x4 texture:
        //   addr_base = 0
        //   spritesheet_offsets = [0, 24] (8-byte header + 16 bytes pixels)
        //   bytes 0..3 = "46d", byte 3 = 0x10 (ci4)
        //   bytes 4..6 = width=4 LE, bytes 6..8 = height=4 LE
        //   bytes 8..24 = 16 bytes pixel data
        let mut rom = Vec::with_capacity(24);
        rom.extend_from_slice(b"\x34\x36\x64");
        rom.push(0x10);
        rom.extend_from_slice(&4i16.to_le_bytes());
        rom.extend_from_slice(&4i16.to_le_bytes());
        rom.extend_from_slice(&[0xAA; 16]);

        let dec = decode_texture(&rom, 0, &[0, 24], 0).unwrap();
        assert_eq!(dec.width, 4);
        assert_eq!(dec.height, 4);
        assert_eq!(dec.format, TextureFormat::Ci4);
        assert_eq!(dec.pixels, vec![0xAA; 16]);
    }

    #[test]
    fn returns_none_for_empty_slot() {
        let rom = vec![0u8; 32];
        assert!(decode_texture(&rom, 0, &[0, 0], 0).is_none());
    }
}
