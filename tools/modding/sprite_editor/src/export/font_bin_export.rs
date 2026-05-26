//! Producer for `.bin` artifacts that match `tools/libhm64/fonts/assembler.py`
//! byte-for-byte. These bins are the "developer handoff" output a modder
//! would give to someone wiring a new font into the matching build.

use std::collections::BTreeSet;
use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{pack_glyph_full, FontAsset, FontPalette, BYTES_PER_GLYPH};

const PALETTE_HEADER: [u8; 4] = [0x34, 0x36, 0x64, 0x10];
const ALIGN: usize = 16;

#[derive(Debug, Clone, Default)]
pub struct BinExportOpts {
    pub texture: bool,
    pub palettes: BTreeSet<u32>,
}

pub fn write_texture_bin(out_path: &Path, asset: &FontAsset) -> ExportResult<()> {
    let mut bytes = Vec::with_capacity(asset.glyphs.len() * BYTES_PER_GLYPH);
    for glyph in asset.glyphs.iter() {
        bytes.extend_from_slice(&pack_glyph_full(&glyph.pixels, &glyph.unused));
    }
    atomic_write(out_path, &bytes).map_err(|e| ExportError::write(out_path, e))
}

pub fn write_palette_bin(out_path: &Path, palette: &FontPalette) -> ExportResult<()> {
    let mut bytes = Vec::with_capacity(ALIGN);
    bytes.extend_from_slice(&PALETTE_HEADER);
    for color in palette.colors.iter() {
        bytes.extend_from_slice(&color.to_rgba5551().to_be_bytes());
    }
    let pad = (ALIGN - (bytes.len() % ALIGN)) % ALIGN;
    bytes.extend(std::iter::repeat(0u8).take(pad));
    debug_assert_eq!(bytes.len() % ALIGN, 0);
    atomic_write(out_path, &bytes).map_err(|e| ExportError::write(out_path, e))
}

pub fn write_font_bins(out_dir: &Path, asset: &FontAsset, opts: &BinExportOpts) -> ExportResult<()> {
    if opts.texture {
        write_texture_bin(&out_dir.join("fontTexture.bin"), asset)?;
    }
    for n in opts.palettes.iter() {
        let pal = asset.palettes.get(n).ok_or_else(|| {
            ExportError::invalid(out_dir, format!("font has no palette {n}"))
        })?;
        write_palette_bin(&out_dir.join(format!("fontPalette{n}.bin")), pal)?;
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{FontGlyph, GlyphUnusedBytes, Rgba};
    use std::collections::BTreeMap;
    use std::path::PathBuf;
    use tempfile::tempdir;

    fn one_glyph_asset() -> FontAsset {
        let glyph = FontGlyph { index: 0, pixels: [0u8; 14 * 14], unused: GlyphUnusedBytes::default() };
        let mut pals = BTreeMap::new();
        pals.insert(1, FontPalette { index: 1, colors: [Rgba::new(0, 0, 0, 255); 16] });
        FontAsset {
            dir: PathBuf::new(),
            name: "x".into(),
            glyphs: vec![glyph],
            palettes: pals,
        }
    }

    #[test]
    fn texture_bin_has_correct_size() {
        let dir = tempdir().unwrap();
        let asset = one_glyph_asset();
        write_texture_bin(&dir.path().join("fontTexture.bin"), &asset).unwrap();
        let b = std::fs::read(dir.path().join("fontTexture.bin")).unwrap();
        assert_eq!(b.len(), BYTES_PER_GLYPH);
    }

    #[test]
    fn palette_bin_starts_with_46d_header() {
        let dir = tempdir().unwrap();
        let asset = one_glyph_asset();
        let pal = asset.palettes.get(&1).unwrap();
        write_palette_bin(&dir.path().join("fontPalette1.bin"), pal).unwrap();
        let b = std::fs::read(dir.path().join("fontPalette1.bin")).unwrap();
        assert_eq!(&b[..4], &[0x34, 0x36, 0x64, 0x10]);
    }

    #[test]
    fn palette_bin_padded_to_16_bytes() {
        let dir = tempdir().unwrap();
        let asset = one_glyph_asset();
        let pal = asset.palettes.get(&1).unwrap();
        write_palette_bin(&dir.path().join("fontPalette1.bin"), pal).unwrap();
        let b = std::fs::read(dir.path().join("fontPalette1.bin")).unwrap();
        // 4 header + 32 data = 36, padded to 48.
        assert_eq!(b.len(), 48);
        assert_eq!(&b[36..], &[0u8; 12]);
    }

    #[test]
    fn write_font_bins_dispatches_per_opts() {
        let dir = tempdir().unwrap();
        let asset = one_glyph_asset();
        let mut palettes = BTreeSet::new();
        palettes.insert(1);
        write_font_bins(
            dir.path(),
            &asset,
            &BinExportOpts { texture: true, palettes: palettes.clone() },
        )
        .unwrap();
        assert!(dir.path().join("fontTexture.bin").exists());
        assert!(dir.path().join("fontPalette1.bin").exists());
        assert!(!dir.path().join("fontPalette2.bin").exists());
    }

    #[test]
    fn write_font_bins_errors_on_missing_palette() {
        let dir = tempdir().unwrap();
        let asset = one_glyph_asset();
        let mut palettes = BTreeSet::new();
        palettes.insert(2); // asset has only palette 1
        let err = write_font_bins(
            dir.path(),
            &asset,
            &BinExportOpts { texture: false, palettes },
        )
        .unwrap_err();
        assert!(matches!(err, ExportError::Invalid { .. }));
    }
}
