//! Reader for `fontPaletteN.pal` files: 32 bytes of 16 RGBA5551 colors.
//! The trailing 12 colors past index 3 are dead bytes in the runtime but
//! preserved here so unmodified palettes round-trip byte-identical.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::{FontPalette, Rgba, PALETTE_TOTAL_COLORS};

/// Expected file size as written by the python extractor.
pub const PAL_BYTE_LEN: usize = PALETTE_TOTAL_COLORS * 2;

pub fn read_font_palette(path: &Path, index: u32) -> IoResult<FontPalette> {
    let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
    if bytes.len() != PAL_BYTE_LEN {
        return Err(IoError::invalid(
            path,
            format!("expected {PAL_BYTE_LEN} bytes, got {}", bytes.len()),
        ));
    }
    let mut colors = [Rgba::default(); PALETTE_TOTAL_COLORS];
    for (i, chunk) in bytes.chunks_exact(2).enumerate() {
        let v = u16::from_be_bytes([chunk[0], chunk[1]]);
        colors[i] = Rgba::from_rgba5551(v);
    }
    Ok(FontPalette { index, colors })
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use tempfile::tempdir;

    #[test]
    fn reads_well_formed_palette() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("fontPalette1.pal");
        // 16 colors, all 0xFFFF (white, alpha=1).
        let bytes = vec![0xFFu8; PAL_BYTE_LEN];
        fs::write(&p, &bytes).unwrap();
        let pal = read_font_palette(&p, 1).unwrap();
        assert_eq!(pal.index, 1);
        assert_eq!(pal.colors[0], Rgba::new(255, 255, 255, 255));
        assert_eq!(pal.colors[15], Rgba::new(255, 255, 255, 255));
    }

    #[test]
    fn rejects_short_file() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("p.pal");
        fs::write(&p, [0u8; 4]).unwrap();
        let err = read_font_palette(&p, 1).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }), "got: {err:?}");
        assert!(format!("{err}").contains("expected 32 bytes"), "rendered: {err}");
    }

    #[test]
    fn rejects_long_file() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("p.pal");
        fs::write(&p, [0u8; 64]).unwrap();
        let err = read_font_palette(&p, 1).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }), "got: {err:?}");
        assert!(format!("{err}").contains("expected 32 bytes"), "rendered: {err}");
    }

    #[test]
    fn preserves_dead_byte_colors() {
        // The reader must place each color at byte_offset = color_index * 2
        // for every slot, including the dead range 4..16. Probe a live slot
        // (0), the live/dead boundary (4), and a deep-dead slot (7) to catch
        // any alignment regression.
        let dir = tempdir().unwrap();
        let p = dir.path().join("p.pal");
        let mut bytes = vec![0u8; PAL_BYTE_LEN];
        // Color 0 = 0xABCD (live slot).
        bytes[0] = 0xAB;
        bytes[1] = 0xCD;
        // Color 4 = 0x5678 (first dead slot).
        bytes[8] = 0x56;
        bytes[9] = 0x78;
        // Color 7 = 0x1234 (interior dead slot).
        bytes[14] = 0x12;
        bytes[15] = 0x34;
        fs::write(&p, &bytes).unwrap();
        let pal = read_font_palette(&p, 2).unwrap();
        assert_eq!(pal.colors[0], Rgba::from_rgba5551(0xABCD));
        assert_eq!(pal.colors[4], Rgba::from_rgba5551(0x5678));
        assert_eq!(pal.colors[7], Rgba::from_rgba5551(0x1234));
    }
}
