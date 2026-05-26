//! Import an indexed PNG: extract its palette, convert indices to CI4 or
//! CI8 pixel data. The `image` crate decodes indexed PNGs as RGBA by
//! default; we use a lower-level decode path to preserve palette indices.

use std::fs::File;
use std::io::BufReader;
use std::path::Path;

use png::{ColorType, Decoder, Transformations};

use crate::io::{IoError, IoResult};
use crate::model::{Rgba, TextureFormat};

pub struct ImportedIndexed {
    pub width: u16,
    pub height: u16,
    pub format: TextureFormat,
    pub pixels: Vec<u8>,    // raw indices (CI8) or packed nibbles (CI4)
    pub palette: Vec<Rgba>, // 16 (CI4) or 256 (CI8) entries
}

pub fn import_indexed_png(path: &Path) -> IoResult<ImportedIndexed> {
    let file = File::open(path).map_err(|e| IoError::read(path, e))?;
    let mut decoder = Decoder::new(BufReader::new(file));
    // We want raw palette indices — suppress automatic expansion to RGB.
    decoder.set_transformations(Transformations::IDENTITY);
    let mut reader =
        decoder.read_info().map_err(|e| IoError::binary(path, 0, format!("png header: {e}")))?;
    let info = reader.info().clone();

    if info.color_type != ColorType::Indexed {
        return Err(IoError::invalid(
            path,
            format!("not an indexed PNG (color_type: {:?})", info.color_type),
        ));
    }

    let mut buf = vec![0u8; reader.output_buffer_size()];
    let out_info = reader
        .next_frame(&mut buf)
        .map_err(|e| IoError::binary(path, 0, format!("png decode: {e}")))?;
    buf.truncate(out_info.buffer_size());

    // PLTE chunk: RGB triplets. tRNS: optional alpha per palette entry.
    let palette_rgb = info
        .palette
        .as_deref()
        .ok_or_else(|| IoError::invalid(path, "indexed PNG missing PLTE chunk"))?;
    let alpha = info.trns.as_deref();

    let mut palette = Vec::with_capacity(palette_rgb.len() / 3);
    for (i, chunk) in palette_rgb.chunks(3).enumerate() {
        let a = alpha.and_then(|t| t.get(i).copied()).unwrap_or(255);
        palette.push(Rgba::new(chunk[0], chunk[1], chunk[2], a));
    }

    let bit_depth = info.bit_depth as u8;
    let (format, pixels) = match bit_depth {
        8 => {
            // Already 1 byte per pixel.
            if palette.len() > 256 {
                return Err(IoError::invalid(path, "palette exceeds 256 entries"));
            }
            // Pad palette to 256 if needed.
            palette.resize(256, Rgba::default());
            (TextureFormat::Ci8, buf)
        }
        4 => {
            // PNG stores 4-bit indexed data as 2 pixels per byte with the
            // high nibble first — same packing as CI4. No repack needed.
            palette.resize(16, Rgba::default());
            (TextureFormat::Ci4, buf)
        }
        other => {
            return Err(IoError::invalid(
                path,
                format!("unsupported bit depth {other} (only 4 and 8 are handled)"),
            ));
        }
    };

    Ok(ImportedIndexed {
        width: info.width as u16,
        height: info.height as u16,
        format,
        pixels,
        palette,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn imports_ci8_grayscale_indexed_png() {
        // Fabricate a minimal indexed PNG by going through the png crate's
        // encoder directly so we get a PLTE chunk.
        let dir = tempdir().unwrap();
        let p = dir.path().join("t.png");
        let w = 4u32;
        let h = 4u32;
        let palette: Vec<u8> = (0..256).flat_map(|i| [i as u8, i as u8, i as u8]).collect();
        let pixels: Vec<u8> = (0..(w * h) as u8).collect();
        let file = File::create(&p).unwrap();
        let mut encoder = png::Encoder::new(file, w, h);
        encoder.set_color(ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(palette.clone());
        let mut writer = encoder.write_header().unwrap();
        writer.write_image_data(&pixels).unwrap();
        drop(writer);

        let imp = import_indexed_png(&p).unwrap();
        assert_eq!(imp.width, 4);
        assert_eq!(imp.height, 4);
        assert_eq!(imp.format, TextureFormat::Ci8);
        assert_eq!(imp.palette.len(), 256);
        assert_eq!(imp.pixels.len(), 16);
    }

    #[test]
    fn imports_ci4_indexed_png() {
        // Fabricate a minimal 4-bit indexed PNG (16-color palette).
        let dir = tempdir().unwrap();
        let p = dir.path().join("t4.png");
        let w = 4u32;
        let h = 4u32;
        // 16-color palette — each entry is a distinct gray.
        let palette: Vec<u8> = (0..16u8).flat_map(|i| [i * 16, i * 16, i * 16]).collect();
        assert_eq!(palette.len(), 16 * 3);
        // 4x4 image at 4bpp = 8 bytes. Pack high nibble first.
        // Row 0: indices 0,1,2,3 -> bytes 0x01, 0x23
        // Row 1: indices 4,5,6,7 -> bytes 0x45, 0x67
        // Row 2: indices 8,9,10,11 -> bytes 0x89, 0xAB
        // Row 3: indices 12,13,14,15 -> bytes 0xCD, 0xEF
        let pixels: Vec<u8> = vec![0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF];
        let file = File::create(&p).unwrap();
        let mut encoder = png::Encoder::new(file, w, h);
        encoder.set_color(ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Four);
        encoder.set_palette(palette.clone());
        let mut writer = encoder.write_header().unwrap();
        writer.write_image_data(&pixels).unwrap();
        drop(writer);

        let imp = import_indexed_png(&p).unwrap();
        assert_eq!(imp.width, 4);
        assert_eq!(imp.height, 4);
        assert_eq!(imp.format, TextureFormat::Ci4);
        assert_eq!(imp.palette.len(), 16);
    }
}
