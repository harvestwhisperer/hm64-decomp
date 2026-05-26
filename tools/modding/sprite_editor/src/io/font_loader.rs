//! Walks an `assets/font/...` directory and assembles a `FontAsset`.
//! Required: `fontTexture.ci2`. Optional: `fontPalette[123].pal`.
//! Files matching higher palette indices are silently ignored.

use std::collections::BTreeMap;
use std::path::Path;

use crate::io::{font_ci2, font_pal, IoError, IoResult};
use crate::model::{FontAsset, BYTES_PER_GLYPH};

/// Scan `dir`, build a `FontAsset`. Returns `IoError::InvalidData` if the
/// required texture file is missing or its byte count is not a positive
/// multiple of `BYTES_PER_GLYPH` (we infer glyph_count from file size).
pub fn load_font_asset(dir: &Path) -> IoResult<FontAsset> {
    let texture_path = dir.join("fontTexture.ci2");
    if !texture_path.is_file() {
        return Err(IoError::invalid(
            &texture_path,
            "fontTexture.ci2 missing — every font directory must include one",
        ));
    }

    // Infer glyph count from file size.
    let texture_bytes = std::fs::metadata(&texture_path)
        .map_err(|e| IoError::read(&texture_path, e))?
        .len() as usize;
    if texture_bytes == 0 || texture_bytes % BYTES_PER_GLYPH != 0 {
        return Err(IoError::invalid(
            &texture_path,
            format!(
                "size {texture_bytes} is not a positive multiple of {BYTES_PER_GLYPH}"
            ),
        ));
    }
    let glyph_count = (texture_bytes / BYTES_PER_GLYPH) as u32;
    let glyphs = font_ci2::read_font_texture(&texture_path, glyph_count)?;

    let mut palettes = BTreeMap::new();
    for n in 1u32..=3 {
        let path = dir.join(format!("fontPalette{n}.pal"));
        if path.is_file() {
            let pal = font_pal::read_font_palette(&path, n)?;
            palettes.insert(n, pal);
        }
    }

    let name = if dir.file_name().and_then(|n| n.to_str()) == Some("font") {
        "default".to_string()
    } else {
        dir.file_name()
            .and_then(|n| n.to_str())
            .unwrap_or("default")
            .to_string()
    };

    Ok(FontAsset {
        dir: dir.to_path_buf(),
        name,
        glyphs,
        palettes,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use tempfile::tempdir;

    fn write_zero_glyph_texture(dir: &Path, glyph_count: usize) {
        let p = dir.join("fontTexture.ci2");
        fs::write(p, vec![0u8; glyph_count * BYTES_PER_GLYPH]).unwrap();
    }

    fn write_zero_palette(dir: &Path, n: u32) {
        let p = dir.join(format!("fontPalette{n}.pal"));
        fs::write(p, vec![0u8; 32]).unwrap();
    }

    #[test]
    fn loads_default_layout() {
        let tmp = tempdir().unwrap();
        let font_dir = tmp.path().join("font");
        fs::create_dir_all(&font_dir).unwrap();
        write_zero_glyph_texture(&font_dir, 700);
        write_zero_palette(&font_dir, 1);
        write_zero_palette(&font_dir, 2);
        write_zero_palette(&font_dir, 3);

        let asset = load_font_asset(&font_dir).unwrap();
        assert_eq!(asset.name, "default");
        assert_eq!(asset.glyphs.len(), 700);
        assert_eq!(asset.palettes.len(), 3);
    }

    #[test]
    fn loads_subdir_with_partial_palettes() {
        let tmp = tempdir().unwrap();
        let font_dir = tmp.path().join("myFont");
        fs::create_dir_all(&font_dir).unwrap();
        write_zero_glyph_texture(&font_dir, 32);
        write_zero_palette(&font_dir, 1);
        // No palette 2 or 3.

        let asset = load_font_asset(&font_dir).unwrap();
        assert_eq!(asset.name, "myFont");
        assert_eq!(asset.glyphs.len(), 32);
        assert_eq!(asset.palettes.len(), 1);
        assert!(asset.palettes.contains_key(&1));
    }

    #[test]
    fn rejects_missing_texture() {
        let tmp = tempdir().unwrap();
        let err = load_font_asset(tmp.path()).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }));
        assert!(format!("{err}").contains("font"), "rendered: {err}");
    }

    #[test]
    fn rejects_misaligned_texture_size() {
        let tmp = tempdir().unwrap();
        let p = tmp.path().join("fontTexture.ci2");
        fs::write(&p, [0u8; 100]).unwrap();
        let err = load_font_asset(tmp.path()).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }));
        assert!(format!("{err}").contains("font"), "rendered: {err}");
    }
}
