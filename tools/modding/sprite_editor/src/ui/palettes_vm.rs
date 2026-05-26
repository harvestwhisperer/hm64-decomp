//! Renders palettes as small swatch images for Slint display.

use slint::{Image, ModelRc, Rgba8Pixel, SharedPixelBuffer, VecModel};

use crate::model::{Palette, SpriteAsset, TextureFormat};
use crate::ui::slint_generated::{ColorCell, PaletteRow, PaletteTextureThumb};
use crate::ui::textures_vm;

/// Render the palette as a grid of colored cells. For CI4, 16 cells in a
/// single row. For CI8, 16 cells × 16 rows.
pub fn render_palette_grid(palette: &Palette, cell_px: u32) -> Image {
    let cols = 16u32;
    let rows = (palette.colors.len() as u32).div_ceil(cols);
    let w = cols * cell_px;
    let h = rows * cell_px;
    let mut buf = SharedPixelBuffer::<Rgba8Pixel>::new(w, h);
    let pixels: &mut [Rgba8Pixel] = buf.make_mut_slice();

    for (i, c) in palette.colors.iter().enumerate() {
        let col = i as u32 % cols;
        let row = i as u32 / cols;
        for dy in 0..cell_px {
            for dx in 0..cell_px {
                let y = row * cell_px + dy;
                let x = col * cell_px + dx;
                let p = &mut pixels[(y * w + x) as usize];
                *p = Rgba8Pixel { r: c.r, g: c.g, b: c.b, a: c.a };
            }
        }
    }
    Image::from_rgba8(buf)
}

/// Pretty descriptor, e.g. `"Palette 3 (CI8, 256 colors)"`.
pub fn describe(palette: &Palette) -> String {
    let format = match palette.format {
        TextureFormat::Ci4 => "CI4",
        TextureFormat::Ci8 => "CI8",
    };
    format!("Palette {} ({}, {} colors)", palette.index, format, palette.colors.len())
}

/// Translate a ComboBox row position to the palette ID at that row.
pub fn palette_id_at_row(s: &SpriteAsset, row: i32) -> Option<u32> {
    if row < 0 {
        return None;
    }
    s.palettes.keys().nth(row as usize).copied()
}

/// Translate a palette ID to its ComboBox row position.
pub fn palette_row_for_id(s: &SpriteAsset, id: u32) -> Option<i32> {
    s.palettes.keys().position(|&k| k == id).map(|p| p as i32)
}

// ---------------------------------------------------------------------------
// Slint property bridges
// ---------------------------------------------------------------------------

pub fn palette_rows(asset: &SpriteAsset) -> ModelRc<PaletteRow> {
    let rows: Vec<PaletteRow> = asset
        .palettes
        .values()
        .map(|p| PaletteRow {
            index: p.index as i32,
            label: describe(p).into(),
            swatch: render_palette_grid(p, 1),
        })
        .collect();
    ModelRc::new(VecModel::from(rows))
}

pub fn color_cells(palette: &Palette) -> ModelRc<ColorCell> {
    let cells: Vec<ColorCell> = palette
        .colors
        .iter()
        .map(|c| ColorCell { r: c.r as i32, g: c.g as i32, b: c.b as i32, a: c.a as i32 })
        .collect();
    ModelRc::new(VecModel::from(cells))
}

pub fn texture_thumbs_for_palette(asset: &SpriteAsset, pal_idx: u32) -> ModelRc<PaletteTextureThumb> {
    let thumbs: Vec<PaletteTextureThumb> = asset
        .textures
        .values()
        .filter(|t| t.palette_index == pal_idx)
        .map(|t| PaletteTextureThumb {
            index: t.index as i32,
            image: textures_vm::render_texture_using_manifest_palette(asset, t),
        })
        .collect();
    ModelRc::new(VecModel::from(thumbs))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{
        Manifest, Palette, Rgba, SpriteAsset, SpriteKind, SpriteType, TextureFormat,
    };
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn sprite_with_palette_ids(ids: &[u32]) -> SpriteAsset {
        let mut palettes = BTreeMap::new();
        for &id in ids {
            palettes.insert(
                id,
                Palette {
                    index: id,
                    format: TextureFormat::Ci4,
                    colors: vec![Rgba::default(); 16],
                },
            );
        }
        SpriteAsset {
            dir: PathBuf::new(),
            manifest: Manifest {
                label: "x".into(),
                subdir: "x".into(),
                kind: SpriteKind::Unknown,
                sprite_type: SpriteType::Type1,
                sprite_count: 0,
                spritesheet_index_count: 0,
                palette_count: ids.len() as u32,
                palette_index_count: 0,
                animation_count: 0,
                animation_index: vec![],
                palette_index: vec![],
                palettes: Default::default(),
                sprite_to_palette: vec![],
                palette_index_end_bytes: vec![],
                spritesheet_index_end_bytes: vec![],
                s2p_padding: vec![],
                texture_end_padding: vec![],
                asset_offsets: vec![],
                textures: vec![],
            },
            textures: BTreeMap::new(),
            palettes,
            animations: BTreeMap::new(),
        }
    }

    #[test]
    fn id_at_row_handles_dense_and_sparse() {
        let dense = sprite_with_palette_ids(&[0, 1, 2]);
        assert_eq!(palette_id_at_row(&dense, 0), Some(0));
        assert_eq!(palette_id_at_row(&dense, 2), Some(2));
        assert_eq!(palette_id_at_row(&dense, 3), None);

        let sparse = sprite_with_palette_ids(&[0, 1, 5, 10]);
        assert_eq!(palette_id_at_row(&sparse, 0), Some(0));
        assert_eq!(palette_id_at_row(&sparse, 2), Some(5));
        assert_eq!(palette_id_at_row(&sparse, 3), Some(10));

        assert_eq!(palette_id_at_row(&sparse, -1), None);
    }

    #[test]
    fn row_for_id_handles_dense_and_sparse() {
        let sparse = sprite_with_palette_ids(&[0, 1, 5, 10]);
        assert_eq!(palette_row_for_id(&sparse, 0), Some(0));
        assert_eq!(palette_row_for_id(&sparse, 5), Some(2));
        assert_eq!(palette_row_for_id(&sparse, 10), Some(3));
        assert_eq!(palette_row_for_id(&sparse, 99), None);
    }
}
