//! Reader for `fontTexture.ci2` files: raw packed CI2 glyph data.
//! File size must equal `glyph_count * BYTES_PER_GLYPH`.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::{extract_unused_bytes, unpack_glyph, FontGlyph, GlyphUnusedBytes, BYTES_PER_GLYPH};

pub fn read_font_texture(path: &Path, glyph_count: u32) -> IoResult<Vec<FontGlyph>> {
    let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
    let expected = glyph_count as usize * BYTES_PER_GLYPH;
    if bytes.len() != expected {
        return Err(IoError::invalid(
            path,
            format!(
                "expected {expected} bytes for {glyph_count} glyphs, got {}",
                bytes.len()
            ),
        ));
    }
    let mut glyphs = Vec::with_capacity(glyph_count as usize);
    for i in 0..glyph_count {
        let base = i as usize * BYTES_PER_GLYPH;
        let mut packed = [0u8; BYTES_PER_GLYPH];
        packed.copy_from_slice(&bytes[base..base + BYTES_PER_GLYPH]);
        glyphs.push(FontGlyph {
            index: i,
            pixels: unpack_glyph(&packed),
            unused: extract_unused_bytes(&packed),
        });
    }
    Ok(glyphs)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use tempfile::tempdir;

    #[test]
    fn reads_two_glyphs() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("fontTexture.ci2");
        // Two zeroed glyphs.
        fs::write(&p, vec![0u8; 2 * BYTES_PER_GLYPH]).unwrap();
        let glyphs = read_font_texture(&p, 2).unwrap();
        assert_eq!(glyphs.len(), 2);
        assert_eq!(glyphs[0].index, 0);
        assert_eq!(glyphs[1].index, 1);
        assert_eq!(glyphs[0].pixels, [0u8; 14 * 14]);
    }

    #[test]
    fn rejects_size_mismatch() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("fontTexture.ci2");
        // Claim 3 glyphs but provide bytes for 2.
        fs::write(&p, vec![0u8; 2 * BYTES_PER_GLYPH]).unwrap();
        let err = read_font_texture(&p, 3).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }), "got: {err:?}");
        assert!(format!("{err}").contains("expected"), "rendered: {err}");
    }
}
