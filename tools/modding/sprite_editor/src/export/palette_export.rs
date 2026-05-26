//! Writer for `palettes/NN.json` + `palettes/NN.pal`.

use std::path::Path;

use serde::Serialize;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::Palette;

#[derive(Serialize)]
struct PaletteJson<'a> {
    color_count: usize,
    format: &'a crate::model::TextureFormat,
    colors: Vec<PaletteColorJson>,
}

#[derive(Serialize)]
struct PaletteColorJson {
    index: usize,
    hex: String,
    alpha: u8,
    rgba: [u8; 4],
}

pub fn write_palette_json(sprite_dir: &Path, palette: &Palette) -> ExportResult<()> {
    let path = sprite_dir.join("palettes").join(format!("{:02}.json", palette.index));
    let colors = palette
        .colors
        .iter()
        .enumerate()
        .map(|(i, c)| PaletteColorJson {
            index: i,
            hex: format!("#{:02X}{:02X}{:02X}", c.r, c.g, c.b),
            alpha: c.a,
            rgba: [c.r, c.g, c.b, c.a],
        })
        .collect::<Vec<_>>();
    let j = PaletteJson { color_count: palette.colors.len(), format: &palette.format, colors };
    let text = serde_json::to_string_pretty(&j).map_err(|e| ExportError::serialize(&path, e))?;
    // Match existing files: pretty-print uses 2-space indent by default.
    atomic_write(&path, text.as_bytes()).map_err(|e| ExportError::write(&path, e))
}

pub fn write_palette_pal(sprite_dir: &Path, palette: &Palette) -> ExportResult<()> {
    let path = sprite_dir.join("palettes").join(format!("{:02}.pal", palette.index));
    let mut bytes = Vec::with_capacity(palette.colors.len() * 2);
    for c in &palette.colors {
        let v = c.to_rgba5551();
        bytes.push((v >> 8) as u8);
        bytes.push((v & 0xFF) as u8);
    }
    atomic_write(&path, &bytes).map_err(|e| ExportError::write(&path, e))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{Rgba, TextureFormat};
    use tempfile::tempdir;

    fn sample() -> Palette {
        Palette {
            index: 0,
            format: TextureFormat::Ci4,
            colors: (0..16).map(|i| Rgba::new(i * 16, 0, 0, 255)).collect(),
        }
    }

    #[test]
    fn writes_palette_json_with_hex_and_rgba() {
        let dir = tempdir().unwrap();
        write_palette_json(dir.path(), &sample()).unwrap();
        let text = std::fs::read_to_string(dir.path().join("palettes/00.json")).unwrap();
        assert!(text.contains("\"color_count\": 16"));
        assert!(text.contains("\"format\": \"ci4\""));
        assert!(text.contains("\"hex\": \"#000000\""));
    }

    #[test]
    fn writes_palette_pal_as_big_endian_rgba5551() {
        let dir = tempdir().unwrap();
        write_palette_pal(dir.path(), &sample()).unwrap();
        let bytes = std::fs::read(dir.path().join("palettes/00.pal")).unwrap();
        assert_eq!(bytes.len(), 32);
        // First color (0,0,0,255) = rgba5551 0x0001 → big-endian 00 01
        assert_eq!(bytes[0], 0x00);
        assert_eq!(bytes[1], 0x01);
    }
}
