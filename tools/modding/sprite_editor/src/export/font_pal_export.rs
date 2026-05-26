//! Writer for `fontPaletteN.pal`: 32 bytes of RGBA5551 (big-endian).
//! Round-trips with `io::font_pal::read_font_palette`.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{FontPalette, PALETTE_TOTAL_COLORS};

pub fn write_font_palette(dir: &Path, palette: &FontPalette) -> ExportResult<()> {
    let path = dir.join(format!("fontPalette{}.pal", palette.index));
    let mut bytes = Vec::with_capacity(PALETTE_TOTAL_COLORS * 2);
    for color in palette.colors.iter() {
        let v = color.to_rgba5551();
        bytes.extend_from_slice(&v.to_be_bytes());
    }
    atomic_write(&path, &bytes).map_err(|e| ExportError::write(&path, e))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::io::font_pal::read_font_palette;
    use crate::model::Rgba;
    use tempfile::tempdir;

    #[test]
    fn round_trips_through_reader() {
        let dir = tempdir().unwrap();
        let mut colors = [Rgba::default(); PALETTE_TOTAL_COLORS];
        // Build colors via from_rgba5551 so they are representable and round-trip
        // cleanly through the 5-to-8-bit expansion used by the codec.
        for (i, c) in colors.iter_mut().enumerate() {
            let r5 = (i as u16) & 0x1F;
            let g5 = ((i as u16) * 2) & 0x1F;
            let b5 = (0x1F - i as u16) & 0x1F;
            let v: u16 = (r5 << 11) | (g5 << 6) | (b5 << 1) | 1;
            *c = Rgba::from_rgba5551(v);
        }
        let pal = FontPalette { index: 2, colors };

        write_font_palette(dir.path(), &pal).unwrap();

        let back = read_font_palette(&dir.path().join("fontPalette2.pal"), 2).unwrap();
        assert_eq!(back.colors, pal.colors);
    }

    #[test]
    fn writes_exactly_32_bytes() {
        let dir = tempdir().unwrap();
        let pal = FontPalette { index: 1, colors: [Rgba::default(); 16] };
        write_font_palette(dir.path(), &pal).unwrap();
        let bytes = std::fs::read(dir.path().join("fontPalette1.pal")).unwrap();
        assert_eq!(bytes.len(), 32);
    }
}
