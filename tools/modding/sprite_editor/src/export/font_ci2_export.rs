//! Writer for `fontTexture.ci2`: each glyph repacked through `pack_glyph`
//! into 64 bytes, concatenated. Round-trips with `io::font_ci2::read_font_texture`.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{pack_glyph_full, FontAsset, BYTES_PER_GLYPH};

pub fn write_font_texture(dir: &Path, asset: &FontAsset) -> ExportResult<()> {
    let path = dir.join("fontTexture.ci2");
    let mut bytes = Vec::with_capacity(asset.glyphs.len() * BYTES_PER_GLYPH);
    for glyph in asset.glyphs.iter() {
        bytes.extend_from_slice(&pack_glyph_full(&glyph.pixels, &glyph.unused));
    }
    atomic_write(&path, &bytes).map_err(|e| ExportError::write(&path, e))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::io::font_ci2::read_font_texture;
    use crate::model::{FontGlyph, GlyphUnusedBytes};
    use std::collections::BTreeMap;
    use std::path::PathBuf;
    use tempfile::tempdir;

    fn make_asset(num_glyphs: u32) -> FontAsset {
        let glyphs: Vec<FontGlyph> = (0..num_glyphs)
            .map(|i| {
                let mut pixels = [0u8; 14 * 14];
                for (j, p) in pixels.iter_mut().enumerate() {
                    *p = ((i as usize + j) % 4) as u8;
                }
                FontGlyph { index: i, pixels, unused: GlyphUnusedBytes::default() }
            })
            .collect();
        FontAsset {
            dir: PathBuf::new(),
            name: "test".into(),
            glyphs,
            palettes: BTreeMap::new(),
        }
    }

    #[test]
    fn round_trips_through_reader() {
        let dir = tempdir().unwrap();
        let asset = make_asset(5);
        write_font_texture(dir.path(), &asset).unwrap();
        let glyphs = read_font_texture(&dir.path().join("fontTexture.ci2"), 5).unwrap();
        assert_eq!(glyphs.len(), 5);
        for (orig, back) in asset.glyphs.iter().zip(glyphs.iter()) {
            assert_eq!(orig.pixels, back.pixels, "glyph {} pixels differ", orig.index);
        }
    }

    #[test]
    fn writes_correct_byte_count() {
        let dir = tempdir().unwrap();
        let asset = make_asset(700);
        write_font_texture(dir.path(), &asset).unwrap();
        let bytes = std::fs::read(dir.path().join("fontTexture.ci2")).unwrap();
        assert_eq!(bytes.len(), 700 * BYTES_PER_GLYPH);
    }
}
