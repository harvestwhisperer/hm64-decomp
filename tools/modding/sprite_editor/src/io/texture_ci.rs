//! Reader for `textures/NNN.ci` binary files.
//!
//! The extractor writes raw CI pixel bytes with no header — width, height, and
//! format come from the accompanying `manifest.json` via `TextureMeta`.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::{Texture, TextureFormat};

pub fn read_texture(
    path: &Path,
    index: u32,
    palette_index: u32,
    width: u16,
    height: u16,
    format: TextureFormat,
) -> IoResult<Texture> {
    let mut pixels = fs::read(path).map_err(|e| IoError::read(path, e))?;
    let expected = format.pixel_bytes(width, height);
    if pixels.len() < expected {
        return Err(IoError::invalid(
            path,
            format!(
                "pixel length {} does not match expected {} for {:?} {}x{}",
                pixels.len(),
                expected,
                format,
                width,
                height
            ),
        ));
    }
    // Some extracted CI files have trailing alignment padding — truncate to the
    // exact pixel data size so callers always see a predictable buffer.
    pixels.truncate(expected);
    Ok(Texture { index, width, height, format, palette_index, pixels })
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    fn write_raw(path: &Path, pixels: &[u8]) {
        fs::write(path, pixels).unwrap();
    }

    #[test]
    fn reads_ci8_texture() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("000.ci");
        write_raw(&p, &(0u8..16).collect::<Vec<u8>>());
        let t = read_texture(&p, 0, 3, 4, 4, TextureFormat::Ci8).unwrap();
        assert_eq!(t.width, 4);
        assert_eq!(t.height, 4);
        assert_eq!(t.format, TextureFormat::Ci8);
        assert_eq!(t.palette_index, 3);
        assert_eq!(t.pixels.len(), 16);
    }

    #[test]
    fn reads_ci4_texture() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("000.ci");
        // 8x8 CI4 = 32 bytes (2 pixels per byte)
        write_raw(&p, &[0xAA; 32]);
        let t = read_texture(&p, 5, 0, 8, 8, TextureFormat::Ci4).unwrap();
        assert_eq!(t.format, TextureFormat::Ci4);
        assert_eq!(t.pixels.len(), 32);
        assert_eq!(t.index, 5);
    }

    #[test]
    fn rejects_pixel_length_mismatch() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("000.ci");
        // Claims 4x4 CI8 (16 bytes) but only has 10
        write_raw(&p, &[0; 10]);
        let err = read_texture(&p, 0, 0, 4, 4, TextureFormat::Ci8).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }), "got: {err:?}");
    }
}
