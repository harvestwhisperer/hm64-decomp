//! Reader for `.pal` palette binaries (RGBA5551 big-endian, 2 bytes/color).
//! Mirrors the writer at `src/export/palette_export.rs::write_palette_pal`.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::Rgba;

pub fn import_pal(path: &Path) -> IoResult<Vec<Rgba>> {
    let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
    if bytes.len() % 2 != 0 {
        return Err(IoError::invalid(path, format!("odd byte count {} (expected even)", bytes.len())));
    }
    let mut colors = Vec::with_capacity(bytes.len() / 2);
    for chunk in bytes.chunks_exact(2) {
        let v = u16::from_be_bytes([chunk[0], chunk[1]]);
        colors.push(Rgba::from_rgba5551(v));
    }
    Ok(colors)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use tempfile::tempdir;

    #[test]
    fn reads_16_color_pal() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("p.pal");
        // 16 colors × 2 bytes = 32 bytes. First color is black (0x0001 = R=0,G=0,B=0,A=1).
        let mut bytes = vec![0u8; 32];
        bytes[1] = 0x01;
        fs::write(&p, &bytes).unwrap();
        let colors = import_pal(&p).unwrap();
        assert_eq!(colors.len(), 16);
        assert_eq!(colors[0].a, 255);
    }

    #[test]
    fn rejects_odd_size() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("bad.pal");
        fs::write(&p, [0u8; 5]).unwrap();
        let err = import_pal(&p).unwrap_err();
        assert!(format!("{err}").contains("odd byte count"));
    }

    #[test]
    fn round_trips_through_writer() {
        // Build a palette, write via existing writer, read back via this reader.
        use crate::export::palette_export::write_palette_pal;
        use crate::model::{Palette, TextureFormat};

        let dir = tempdir().unwrap();
        let pal = Palette {
            index: 0,
            format: TextureFormat::Ci4,
            colors: (0..16).map(|i| Rgba::new(i * 16, i * 8, 255 - i * 16, 255)).collect(),
        };
        write_palette_pal(dir.path(), &pal).unwrap();
        let colors = import_pal(&dir.path().join("palettes/00.pal")).unwrap();
        assert_eq!(colors.len(), 16);
        // RGBA5551 is lossy; exact-byte equality after the round-trip would
        // require quantizing the source palette. Confirm length + alpha bit.
        for (orig, round) in pal.colors.iter().zip(colors.iter()) {
            assert_eq!(orig.a == 255, round.a == 255, "alpha bit must round-trip");
        }
    }
}
