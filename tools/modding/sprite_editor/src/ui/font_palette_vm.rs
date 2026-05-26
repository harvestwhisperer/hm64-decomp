//! View-model: turn a font's palettes into rows of (palette_index, [4 cells])
//! that the Slint `font_palettes_panel` can bind. Only the 4 live colors are
//! exposed; dead bytes are not.

use crate::model::{FontAsset, Rgba, PALETTE_LIVE_COLORS};

#[derive(Debug, Clone, PartialEq)]
pub struct FontPaletteRowVm {
    pub palette_index: u32,
    pub label: String,
    pub cells: [Rgba; PALETTE_LIVE_COLORS],
}

pub fn build_rows(asset: &FontAsset) -> Vec<FontPaletteRowVm> {
    let mut rows = Vec::with_capacity(asset.palettes.len());
    for (index, pal) in asset.palettes.iter() {
        let cells = [pal.colors[0], pal.colors[1], pal.colors[2], pal.colors[3]];
        rows.push(FontPaletteRowVm {
            palette_index: *index,
            label: format!("Palette {index}"),
            cells,
        });
    }
    rows
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{FontAsset, FontGlyph, FontPalette, GlyphUnusedBytes};
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    #[test]
    fn build_rows_skips_missing_indices_and_orders_ascending() {
        let mut palettes = BTreeMap::new();
        palettes.insert(3, FontPalette { index: 3, colors: [Rgba::new(3, 0, 0, 255); 16] });
        palettes.insert(1, FontPalette { index: 1, colors: [Rgba::new(1, 0, 0, 255); 16] });
        let asset = FontAsset {
            dir: PathBuf::new(),
            name: "x".into(),
            glyphs: vec![FontGlyph { index: 0, pixels: [0; 14 * 14], unused: GlyphUnusedBytes::default() }],
            palettes,
        };
        let rows = build_rows(&asset);
        assert_eq!(rows.len(), 2);
        // BTreeMap iterates ascending → 1 then 3.
        assert_eq!(rows[0].palette_index, 1);
        assert_eq!(rows[0].cells[0].r, 1);
        assert_eq!(rows[1].palette_index, 3);
        assert_eq!(rows[1].cells[0].r, 3);
    }

    #[test]
    fn build_rows_exposes_only_first_four_colors() {
        let mut palettes = BTreeMap::new();
        let mut colors = [Rgba::new(0, 0, 0, 255); 16];
        colors[7] = Rgba::new(255, 255, 255, 255); // dead byte
        palettes.insert(1, FontPalette { index: 1, colors });
        let asset = FontAsset {
            dir: PathBuf::new(),
            name: "x".into(),
            glyphs: vec![FontGlyph { index: 0, pixels: [0; 14 * 14], unused: GlyphUnusedBytes::default() }],
            palettes,
        };
        let rows = build_rows(&asset);
        // White at index 7 must not leak into the cells array.
        for cell in rows[0].cells.iter() {
            assert_ne!(*cell, Rgba::new(255, 255, 255, 255));
        }
    }
}
