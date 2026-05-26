//! View-model: render a font's contact sheet and per-glyph editor pixmap as
//! Slint `Image`s. All conversions are offline pixel buffers; no I/O.

use slint::{Image, Rgba8Pixel, SharedPixelBuffer};

use crate::model::{FontAsset, FontPalette, Rgba, GLYPH_HEIGHT, GLYPH_PIXELS, GLYPH_WIDTH};

pub const CONTACT_SHEET_COLS: u32 = 16;

/// Render the entire font's glyphs to a single RGBA pixel buffer at `scale`x.
/// Returns a buffer of size `(cols * GLYPH_WIDTH * scale, rows * GLYPH_HEIGHT * scale)`.
pub fn render_contact_sheet(asset: &FontAsset, palette_index: u32, scale: u32) -> Image {
    let rows = (asset.glyphs.len() as u32).div_ceil(CONTACT_SHEET_COLS);
    let pixel_w = CONTACT_SHEET_COLS * GLYPH_WIDTH as u32 * scale;
    let pixel_h = rows * GLYPH_HEIGHT as u32 * scale;
    let mut buf = SharedPixelBuffer::<Rgba8Pixel>::new(pixel_w, pixel_h);

    let palette_colors = palette_or_fallback(asset, palette_index);

    {
        let dst = buf.make_mut_slice();
        for (i, glyph) in asset.glyphs.iter().enumerate() {
            let cell_x = (i as u32 % CONTACT_SHEET_COLS) * GLYPH_WIDTH as u32 * scale;
            let cell_y = (i as u32 / CONTACT_SHEET_COLS) * GLYPH_HEIGHT as u32 * scale;
            blit_glyph(dst, pixel_w, &glyph.pixels, &palette_colors, cell_x, cell_y, scale);
        }
    }

    Image::from_rgba8(buf)
}

/// Render one glyph at `scale`x for the per-glyph editor pixmap.
pub fn render_glyph(pixels: &[u8; GLYPH_PIXELS], palette: &[Rgba; 4], scale: u32) -> Image {
    let pixel_w = GLYPH_WIDTH as u32 * scale;
    let pixel_h = GLYPH_HEIGHT as u32 * scale;
    let mut buf = SharedPixelBuffer::<Rgba8Pixel>::new(pixel_w, pixel_h);
    {
        let dst = buf.make_mut_slice();
        blit_glyph(dst, pixel_w, pixels, palette, 0, 0, scale);
    }
    Image::from_rgba8(buf)
}

/// Translate a click in the contact-sheet image's pixel space to a glyph
/// index. Returns None if the click falls outside the populated area.
pub fn glyph_at(asset: &FontAsset, x_px: u32, y_px: u32, scale: u32) -> Option<u32> {
    let col = x_px / (GLYPH_WIDTH as u32 * scale);
    let row = y_px / (GLYPH_HEIGHT as u32 * scale);
    if col >= CONTACT_SHEET_COLS {
        return None;
    }
    let idx = row * CONTACT_SHEET_COLS + col;
    if (idx as usize) < asset.glyphs.len() { Some(idx) } else { None }
}

/// Translate a click in the per-glyph editor's pixel space to a flat pixel
/// index 0..196.
pub fn pixel_at(x_px: u32, y_px: u32, scale: u32) -> Option<u16> {
    let col = x_px / scale;
    let row = y_px / scale;
    if col >= GLYPH_WIDTH as u32 || row >= GLYPH_HEIGHT as u32 {
        return None;
    }
    Some((row * GLYPH_WIDTH as u32 + col) as u16)
}

fn palette_or_fallback(asset: &FontAsset, palette_index: u32) -> [Rgba; 4] {
    if let Some(p) = asset.palettes.get(&palette_index) {
        first_four(p)
    } else if let Some((_, p)) = asset.palettes.iter().next() {
        first_four(p)
    } else {
        [Rgba::default(); 4]
    }
}

fn first_four(p: &FontPalette) -> [Rgba; 4] {
    [p.colors[0], p.colors[1], p.colors[2], p.colors[3]]
}

fn blit_glyph(
    dst: &mut [Rgba8Pixel],
    dst_stride: u32,
    pixels: &[u8; GLYPH_PIXELS],
    palette: &[Rgba; 4],
    x0: u32,
    y0: u32,
    scale: u32,
) {
    for gy in 0..GLYPH_HEIGHT as u32 {
        for gx in 0..GLYPH_WIDTH as u32 {
            let pi = pixels[(gy * GLYPH_WIDTH as u32 + gx) as usize] as usize;
            let c = palette[pi.min(3)];
            let pix = Rgba8Pixel { r: c.r, g: c.g, b: c.b, a: c.a };
            for sy in 0..scale {
                for sx in 0..scale {
                    let x = x0 + gx * scale + sx;
                    let y = y0 + gy * scale + sy;
                    let idx = (y * dst_stride + x) as usize;
                    dst[idx] = pix;
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{FontAsset, FontGlyph, FontPalette, GlyphUnusedBytes};
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn one_glyph_asset() -> FontAsset {
        let glyph = FontGlyph { index: 0, pixels: [3u8; GLYPH_PIXELS], unused: GlyphUnusedBytes::default() };
        let mut palettes = BTreeMap::new();
        palettes.insert(1, FontPalette { index: 1, colors: [Rgba::new(0, 0, 0, 255); 16] });
        FontAsset {
            dir: PathBuf::new(),
            name: "x".into(),
            glyphs: vec![glyph],
            palettes,
        }
    }

    #[test]
    fn glyph_at_inside_first_cell() {
        let asset = one_glyph_asset();
        assert_eq!(glyph_at(&asset, 0, 0, 1), Some(0));
        assert_eq!(glyph_at(&asset, 13, 13, 1), Some(0));
    }

    #[test]
    fn glyph_at_outside_returns_none() {
        let asset = one_glyph_asset();
        // Single-glyph asset → only cell (0,0) is valid.
        assert_eq!(glyph_at(&asset, 14, 0, 1), None);
    }

    #[test]
    fn pixel_at_inside_glyph() {
        assert_eq!(pixel_at(0, 0, 1), Some(0));
        assert_eq!(pixel_at(13, 13, 1), Some(13 * 14 + 13));
    }

    #[test]
    fn pixel_at_outside_glyph() {
        assert_eq!(pixel_at(14, 0, 1), None);
        assert_eq!(pixel_at(0, 14, 1), None);
    }

    #[test]
    fn render_contact_sheet_produces_image_of_expected_size() {
        let asset = one_glyph_asset();
        let img = render_contact_sheet(&asset, 1, 2);
        let (w, h) = (img.size().width, img.size().height);
        // 1 glyph → 1 row × 16 cols. At scale 2: 14*2*16 = 448, 14*2*1 = 28.
        assert_eq!(w, 448);
        assert_eq!(h, 28);
    }
}
