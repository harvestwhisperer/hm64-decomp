//! Writer for `textures/NNN.ci` + regenerated `textures/NNN.png`.
//!
//! `.ci` files are raw pixel bytes with no header — width, height, and format
//! are stored in `manifest.json` and used by the Python assembler to rebuild
//! the ROM binary with the proper 8-byte per-texture header.
//!
//! `.png` files are written as indexed PNGs (PLTE + tRNS), bit-depth-matched to
//! the texture format (4 for CI4, 8 for CI8). This must match the format the
//! Python extractor produces (see `tools/libhm64/sprites/textures.py`) so the
//! editor's save path can never replace an extracted indexed PNG with a
//! true-color one — that would corrupt re-importability and silently break
//! `Replace Texture from PNG` when users pick the editor's own preview file.

use std::path::Path;

use png::{BitDepth, ColorType, Encoder};

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{Palette, Texture, TextureFormat};

pub fn write_texture_ci(sprite_dir: &Path, tex: &Texture) -> ExportResult<()> {
    let path = sprite_dir.join("textures").join(format!("{:03}.ci", tex.index));
    atomic_write(&path, &tex.pixels).map_err(|e| ExportError::write(&path, e))
}

pub fn write_texture_png(sprite_dir: &Path, tex: &Texture, palette: &Palette) -> ExportResult<()> {
    let path = sprite_dir.join("textures").join(format!("{:03}.png", tex.index));

    let bit_depth = match tex.format {
        TextureFormat::Ci4 => BitDepth::Four,
        TextureFormat::Ci8 => BitDepth::Eight,
    };

    // PLTE: flat RGB triplets, one per palette entry.
    let mut plte: Vec<u8> = Vec::with_capacity(palette.colors.len() * 3);
    for c in &palette.colors {
        plte.extend_from_slice(&[c.r, c.g, c.b]);
    }
    // tRNS: per-entry alpha. The png crate accepts trailing 0xFF entries
    // harmlessly and the importer reads them back, so emit one alpha byte per
    // palette entry to keep output deterministic.
    let trns: Vec<u8> = palette.colors.iter().map(|c| c.a).collect();

    let pixels_for_png = repack_pixels_for_png(tex)?;

    let mut buf: Vec<u8> = Vec::new();
    {
        let mut encoder = Encoder::new(&mut buf, tex.width as u32, tex.height as u32);
        encoder.set_color(ColorType::Indexed);
        encoder.set_depth(bit_depth);
        encoder.set_palette(plte);
        encoder.set_trns(trns);
        let mut writer = encoder
            .write_header()
            .map_err(|e| ExportError::write(&path, std::io::Error::other(e.to_string())))?;
        writer
            .write_image_data(&pixels_for_png)
            .map_err(|e| ExportError::write(&path, std::io::Error::other(e.to_string())))?;
    }

    atomic_write(&path, &buf).map_err(|e| ExportError::write(&path, e))
}

/// Returns the texture's pixels in the row layout PNG expects.
///
/// HM64 stores CI4 as 2 pixels per byte sequentially with no row padding. PNG
/// requires each row to start on a byte boundary, so for odd widths we have to
/// repack to insert a padding nibble at the end of every row. CI8 and even-width
/// CI4 already match PNG's layout; we just clone in those cases.
fn repack_pixels_for_png(tex: &Texture) -> ExportResult<Vec<u8>> {
    if tex.format == TextureFormat::Ci8 || tex.width % 2 == 0 {
        return Ok(tex.pixels.clone());
    }
    let w = tex.width as usize;
    let h = tex.height as usize;
    // Unpack every nibble in HM64 order.
    let mut indices: Vec<u8> = Vec::with_capacity(tex.pixels.len() * 2);
    for byte in &tex.pixels {
        indices.push((byte >> 4) & 0x0F);
        indices.push(byte & 0x0F);
    }
    if indices.len() < w * h {
        return Err(ExportError::invalid(
            std::path::PathBuf::new(),
            format!("CI4 pixel buffer too short for {w}x{h} ({} bytes)", tex.pixels.len()),
        ));
    }
    let bytes_per_row = (w + 1) / 2;
    let mut out: Vec<u8> = Vec::with_capacity(bytes_per_row * h);
    for row in 0..h {
        let row_start = row * w;
        for byte_idx in 0..bytes_per_row {
            let col0 = byte_idx * 2;
            let col1 = col0 + 1;
            let p0 = indices[row_start + col0];
            // Pad the trailing nibble of an odd-width row with 0; reading the
            // next index would pull a pixel from the next row.
            let p1 = if col1 < w { indices[row_start + col1] } else { 0 };
            out.push((p0 << 4) | (p1 & 0x0F));
        }
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::io::png_import::import_indexed_png;
    use crate::model::Rgba;
    use tempfile::tempdir;

    fn ci8_palette() -> Palette {
        Palette {
            index: 0,
            format: TextureFormat::Ci8,
            colors: (0..256).map(|i| Rgba::new(i as u8, 0, 0, if i < 4 { 0 } else { 255 })).collect(),
        }
    }

    fn ci4_palette() -> Palette {
        Palette {
            index: 0,
            format: TextureFormat::Ci4,
            colors: (0..16).map(|i| Rgba::new(i * 16, 0, 0, 255)).collect(),
        }
    }

    #[test]
    fn writes_ci8_raw_pixels() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("textures")).unwrap();
        let t = Texture {
            index: 0,
            width: 2,
            height: 2,
            format: TextureFormat::Ci8,
            palette_index: 0,
            pixels: vec![0, 1, 2, 3],
        };
        write_texture_ci(dir.path(), &t).unwrap();
        let bytes = std::fs::read(dir.path().join("textures/000.ci")).unwrap();
        assert_eq!(bytes, vec![0, 1, 2, 3]);
    }

    #[test]
    fn writes_ci4_raw_pixels() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("textures")).unwrap();
        let t = Texture {
            index: 0,
            width: 4,
            height: 4,
            format: TextureFormat::Ci4,
            palette_index: 0,
            pixels: vec![0xAB; 8],
        };
        write_texture_ci(dir.path(), &t).unwrap();
        let bytes = std::fs::read(dir.path().join("textures/000.ci")).unwrap();
        assert_eq!(bytes, vec![0xAB; 8]);
    }

    #[test]
    fn ci8_png_round_trips_through_import() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("textures")).unwrap();
        let pal = ci8_palette();
        let t = Texture {
            index: 7,
            width: 2,
            height: 2,
            format: TextureFormat::Ci8,
            palette_index: 0,
            pixels: vec![10, 20, 30, 40],
        };
        write_texture_png(dir.path(), &t, &pal).unwrap();

        let imp = import_indexed_png(&dir.path().join("textures/007.png")).unwrap();
        assert_eq!(imp.format, TextureFormat::Ci8);
        assert_eq!(imp.width, 2);
        assert_eq!(imp.height, 2);
        assert_eq!(imp.pixels, vec![10, 20, 30, 40]);
        // First 256 entries (or however many we wrote) must round-trip exactly.
        for (i, c) in pal.colors.iter().enumerate() {
            assert_eq!(imp.palette[i], *c, "palette entry {i} differs");
        }
    }

    #[test]
    fn ci4_even_width_png_round_trips_through_import() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("textures")).unwrap();
        let pal = ci4_palette();
        // 4x2 image: 4 bytes (2 pixels per byte, 2 rows).
        // Row 0: 0x12, 0x34 → pixels [1, 2, 3, 4]
        // Row 1: 0x56, 0x78 → pixels [5, 6, 7, 8]
        let t = Texture {
            index: 3,
            width: 4,
            height: 2,
            format: TextureFormat::Ci4,
            palette_index: 0,
            pixels: vec![0x12, 0x34, 0x56, 0x78],
        };
        write_texture_png(dir.path(), &t, &pal).unwrap();

        let imp = import_indexed_png(&dir.path().join("textures/003.png")).unwrap();
        assert_eq!(imp.format, TextureFormat::Ci4);
        assert_eq!(imp.width, 4);
        assert_eq!(imp.height, 2);
        assert_eq!(imp.pixels, vec![0x12, 0x34, 0x56, 0x78]);
    }

    #[test]
    fn ci4_odd_width_png_round_trips_through_import() {
        // 3x2 image. HM64 packs as: byte0 = (0,0)(1,0), byte1 = (2,0)(0,1),
        // byte2 = (1,1)(2,1). PNG row layout differs (2 bytes per row, padding
        // nibble after the third pixel). repack_pixels_for_png must handle this.
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("textures")).unwrap();
        let pal = ci4_palette();
        let t = Texture {
            index: 1,
            width: 3,
            height: 2,
            format: TextureFormat::Ci4,
            palette_index: 0,
            // Pixel grid (row-major):
            //   row 0: idx 1, 2, 3
            //   row 1: idx 4, 5, 6
            // HM64 packing: 0x12, 0x34, 0x56
            pixels: vec![0x12, 0x34, 0x56],
        };
        write_texture_png(dir.path(), &t, &pal).unwrap();

        let imp = import_indexed_png(&dir.path().join("textures/001.png")).unwrap();
        assert_eq!(imp.format, TextureFormat::Ci4);
        assert_eq!(imp.width, 3);
        assert_eq!(imp.height, 2);
        // PNG packing: 4 bytes total (2 bytes per row × 2 rows).
        // Row 0: 0x12, 0x30 (third pixel idx 3 in high nibble, padding 0).
        // Row 1: 0x45, 0x60.
        assert_eq!(imp.pixels, vec![0x12, 0x30, 0x45, 0x60]);
    }

    #[test]
    fn png_is_actually_indexed_not_rgba() {
        // Regression: this writer used to produce RGBA PNGs, which broke
        // re-import (the importer rejects non-indexed PNGs) and corrupted the
        // extractor's indexed PNGs in place on save.
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("textures")).unwrap();
        let pal = ci8_palette();
        let t = Texture {
            index: 0,
            width: 1,
            height: 1,
            format: TextureFormat::Ci8,
            palette_index: 0,
            pixels: vec![5],
        };
        write_texture_png(dir.path(), &t, &pal).unwrap();
        // import_indexed_png errors on non-indexed PNGs — successful import is
        // the assertion.
        let imp = import_indexed_png(&dir.path().join("textures/000.png")).unwrap();
        assert_eq!(imp.format, TextureFormat::Ci8);
    }
}
