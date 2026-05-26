//! Reader for `palettes/NN.json`.

use std::fs;
use std::path::Path;

use serde::Deserialize;

use crate::io::{IoError, IoResult};
use crate::model::{Palette, Rgba, TextureFormat};

#[derive(Deserialize)]
struct PaletteJson {
    #[allow(dead_code)]
    color_count: u32,
    format: TextureFormat,
    colors: Vec<PaletteColorJson>,
}

#[derive(Deserialize)]
struct PaletteColorJson {
    #[allow(dead_code)]
    index: u32,
    rgba: [u8; 4],
}

pub fn read_palette(path: &Path, index: u32) -> IoResult<Palette> {
    let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
    let parsed: PaletteJson = serde_json::from_slice(&bytes).map_err(|e| IoError::json(path, e))?;
    let colors = parsed
        .colors
        .into_iter()
        .map(|c| Rgba::new(c.rgba[0], c.rgba[1], c.rgba[2], c.rgba[3]))
        .collect::<Vec<_>>();
    let expected = parsed.format.palette_len();
    if colors.len() != expected {
        return Err(IoError::invalid(
            path,
            format!("palette has {} colors, expected {}", colors.len(), expected),
        ));
    }
    Ok(Palette { index, format: parsed.format, colors })
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    fn minimal_ci4() -> String {
        // 16 colors, all (0,0,0,0)
        let colors: Vec<String> = (0..16)
            .map(|i| {
                format!(r##"{{"index": {i}, "hex": "#000000", "alpha": 0, "rgba": [0,0,0,0]}}"##)
            })
            .collect();
        format!(r#"{{"color_count": 16, "format": "ci4", "colors": [{}]}}"#, colors.join(","))
    }

    #[test]
    fn reads_minimal_ci4_palette() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("00.json");
        fs::write(&p, minimal_ci4()).unwrap();
        let pal = read_palette(&p, 0).unwrap();
        assert_eq!(pal.format, TextureFormat::Ci4);
        assert_eq!(pal.colors.len(), 16);
        assert_eq!(pal.colors[0], Rgba::new(0, 0, 0, 0));
    }

    #[test]
    fn rejects_mismatched_color_count() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("00.json");
        // Says ci8 (expects 256) but only provides 1 color
        fs::write(
            &p,
            r##"{"color_count": 1, "format": "ci8", "colors": [{"index":0,"hex":"#0","alpha":0,"rgba":[0,0,0,0]}]}"##,
        )
        .unwrap();
        let err = read_palette(&p, 0).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }), "got: {err:?}");
    }
}
