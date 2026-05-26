//! RGBA5551 palette decoding. Mirrors
//! `tools/libhm64/sprites/textures.py::extract_palette`.

use crate::model::Rgba;

#[derive(Debug, thiserror::Error)]
pub enum PaletteError {
    #[error("offset {offset}: out of range (rom len {len})")]
    OutOfRange { offset: usize, len: usize },
}

#[derive(Debug, Clone, PartialEq)]
pub struct DecodedPalette {
    pub colors: Vec<Rgba>,
}

/// Extract one palette block from ROM. The block starts with a 4-byte header
/// and ends with a 4-byte 0xFFFFFFFF sentinel; this returns just the inner
/// RGBA5551 bytes decoded into `Rgba` values.
///
/// `palette_section` is the absolute ROM offset of the palette index region
/// (`addr_base + asset_offsets[1]`).
pub fn decode_palette(
    rom: &[u8],
    palette_section: u32,
    palette_offsets: &[u32],
    pal_idx: usize,
) -> Option<DecodedPalette> {
    if pal_idx + 1 >= palette_offsets.len() {
        return None;
    }
    let offset = palette_offsets[pal_idx];
    let next_offset = palette_offsets[pal_idx + 1];
    if next_offset == 0 || offset == next_offset {
        return None;
    }
    let block_start = palette_section.checked_add(offset)?;
    let block_end = palette_section.checked_add(next_offset)?;
    let start = block_start as usize + 4; // skip 4-byte header
    let end = (block_end as usize).checked_sub(4)?; // skip 4-byte sentinel
    if end <= start {
        return None;
    }
    let pal_size = end - start;
    if pal_size > 512 {
        return None;
    }

    let raw = rom.get(start..end)?;
    let mut colors = Vec::with_capacity(raw.len() / 2);
    for chunk in raw.chunks_exact(2) {
        let v = u16::from_be_bytes([chunk[0], chunk[1]]);
        colors.push(Rgba::from_rgba5551(v));
    }
    Some(DecodedPalette { colors })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn decodes_two_colors() {
        // palette_section = 0
        // palette_offsets = [8, 24]: pal block from offset 8 to 24
        //   bytes 8..12: 4-byte header
        //   bytes 12..16: 2 colors (4 bytes RGBA5551 BE)
        //   bytes 16..20: trailing bytes (more colors or part of next block)
        //   bytes 20..24: 4-byte sentinel
        let mut rom = Vec::new();
        rom.extend_from_slice(&[0u8; 8]);             // index space
        rom.extend_from_slice(b"\x34\x36\x64\x00");   // header
        rom.extend_from_slice(&[0x00, 0x01]);         // RGBA5551 = 0x0001 (black, alpha=1)
        rom.extend_from_slice(&[0xFF, 0xFF]);         // RGBA5551 = 0xFFFF (white)
        rom.extend_from_slice(&[0x00; 4]);            // padding
        rom.extend_from_slice(&[0xFF; 4]);            // sentinel

        let pal = decode_palette(&rom, 0, &[8, 24], 0).unwrap();
        assert_eq!(pal.colors.len(), 4); // 8 bytes / 2 = 4
        assert_eq!(pal.colors[0].a, 255);
        assert_eq!(pal.colors[1], Rgba::new(255, 255, 255, 255));
    }

    #[test]
    fn returns_none_for_empty_slot() {
        let rom = vec![0u8; 32];
        assert!(decode_palette(&rom, 0, &[0, 0], 0).is_none());
    }
}
