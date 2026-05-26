//! Font asset model: glyphs, palettes, and CI2 byte packing.

use std::collections::BTreeMap;
use std::path::PathBuf;

use crate::model::Rgba;

pub const GLYPH_WIDTH: usize = 14;
pub const GLYPH_HEIGHT: usize = 14;
pub const GLYPH_PIXELS: usize = GLYPH_WIDTH * GLYPH_HEIGHT;
pub const BYTES_PER_GLYPH: usize = 64;
pub const PALETTE_LIVE_COLORS: usize = 4;
pub const PALETTE_TOTAL_COLORS: usize = 16;

#[derive(Debug, Clone, PartialEq)]
pub struct FontGlyph {
    pub index: u32,
    pub pixels: [u8; GLYPH_PIXELS],
    /// Runtime-ignored bytes preserved for byte-identical round-trip with the
    /// original ROM. `bytes_56_63` are the 8 padding bytes at the end of each
    /// 64-byte CI2 slot (only 56 bytes are used by 14 rows × 4 bytes).
    /// `row_byte0_low_nibbles[r]` is the bottom 4 bits of row `r`'s byte 0;
    /// `swap_top` reads only bits 7-4 of that byte, so bits 3-0 are ignored.
    pub unused: GlyphUnusedBytes,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct GlyphUnusedBytes {
    pub bytes_56_63: [u8; 8],
    pub row_byte0_low_nibbles: [u8; GLYPH_HEIGHT], // each value is 0..=15
}

impl Default for GlyphUnusedBytes {
    fn default() -> Self {
        Self { bytes_56_63: [0; 8], row_byte0_low_nibbles: [0; GLYPH_HEIGHT] }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct FontPalette {
    pub index: u32,
    pub colors: [Rgba; PALETTE_TOTAL_COLORS],
}

#[derive(Debug, Clone, PartialEq)]
pub struct FontAsset {
    pub dir: PathBuf,
    pub name: String,
    pub glyphs: Vec<FontGlyph>,
    pub palettes: BTreeMap<u32, FontPalette>,
}

#[derive(Copy, Clone, Debug, PartialEq, Eq, Default)]
pub enum ActiveAsset {
    #[default]
    None,
    Sprite,
    Font,
}

/// Extract two 2-bit values from the upper nibble of a packed byte.
/// Mirrors `swapTop` in `src/system/message.c`:
///   bits 7-6 → bits 5-4 (first pixel)
///   bits 5-4 → bits 1-0 (second pixel)
/// Output byte holds the two unpacked pixel values in those positions.
pub const fn swap_top(b: u8) -> u8 {
    ((b & 0xC0) >> 2) | ((b & 0x30) >> 4)
}

/// Extract two 2-bit values from the lower nibble of a packed byte.
/// Mirrors `swapBottom` in `src/system/message.c`:
///   bits 3-2 → bits 5-4 (first pixel)
///   bits 1-0 stay at bits 1-0 (second pixel)
pub const fn swap_bottom(b: u8) -> u8 {
    ((b & 0x0C) << 2) | (b & 0x03)
}

/// Inverse of `swap_top`: given an output byte that holds two pixels at bits
/// 5-4 and 1-0, repack into the upper-nibble form `swap_top` would produce.
pub const fn unswap_top(out: u8) -> u8 {
    ((out & 0x30) << 2) | ((out & 0x03) << 4)
}

/// Inverse of `swap_bottom`: given an output byte that holds two pixels at bits
/// 5-4 and 1-0, repack into the lower-nibble form `swap_bottom` would produce.
///   bits 5-4 → bits 3-2 (first pixel)
///   bits 1-0 stay at bits 1-0 (second pixel)
pub const fn unswap_bottom(out: u8) -> u8 {
    ((out & 0x30) >> 2) | (out & 0x03)
}

/// Unpack one glyph (64 packed bytes) into 196 palette indices 0..=3.
///
/// Mirrors `unpack_font_ci2_data` in `libhm64/fonts/png_exporter.py`. For each
/// row, we read 4 source bytes in reverse (offsets 3, 2, 1, 0 within the row)
/// and emit 7 pixel values (the 8th output slot is padding which the python
/// code zeros and we discard).
pub fn unpack_glyph(packed: &[u8; BYTES_PER_GLYPH]) -> [u8; GLYPH_PIXELS] {
    let mut out = [0u8; GLYPH_PIXELS];
    for row in 0..GLYPH_HEIGHT {
        let row_base = row * 4;
        let row_buf = [
            // Three bytes contribute two pixels each (top + bottom nibbles).
            swap_top(packed[row_base + 3]),
            swap_bottom(packed[row_base + 3]),
            swap_top(packed[row_base + 2]),
            swap_bottom(packed[row_base + 2]),
            swap_top(packed[row_base + 1]),
            swap_bottom(packed[row_base + 1]),
            // Source byte 0's bottom nibble is unused (would represent pixels
            // beyond column 13 in a 14-wide row).
            swap_top(packed[row_base]),
        ];
        let pixel_base = row * GLYPH_WIDTH;
        let mut col = 0;
        for &b in &row_buf {
            out[pixel_base + col]     = (b >> 4) & 0x03;
            out[pixel_base + col + 1] = b & 0x03;
            col += 2;
        }
        debug_assert_eq!(col, GLYPH_WIDTH);
    }
    out
}

/// Repack 196 palette indices 0..=3 into 64 packed bytes.
pub fn pack_glyph(pixels: &[u8; GLYPH_PIXELS]) -> [u8; BYTES_PER_GLYPH] {
    let mut out = [0u8; BYTES_PER_GLYPH];
    for row in 0..GLYPH_HEIGHT {
        let row_pixels = &pixels[row * GLYPH_WIDTH..(row + 1) * GLYPH_WIDTH];
        // Build 7 "output bytes" each carrying a (top, bottom) pixel pair at
        // bits 5-4 and 1-0. 7 pairs × 2 pixels = 14 = GLYPH_WIDTH exactly, so
        // every output byte holds two real pixels (no padding pair).
        let pair = |a: u8, b: u8| -> u8 { (a << 4) | b };
        let row_buf = [
            pair(row_pixels[0], row_pixels[1]),
            pair(row_pixels[2], row_pixels[3]),
            pair(row_pixels[4], row_pixels[5]),
            pair(row_pixels[6], row_pixels[7]),
            pair(row_pixels[8], row_pixels[9]),
            pair(row_pixels[10], row_pixels[11]),
            pair(row_pixels[12], row_pixels[13]),
        ];
        let row_base = row * 4;
        // Source byte 3 carries pairs 0 and 1.
        out[row_base + 3] = unswap_top(row_buf[0]) | unswap_bottom(row_buf[1]);
        // Source byte 2 carries pairs 2 and 3.
        out[row_base + 2] = unswap_top(row_buf[2]) | unswap_bottom(row_buf[3]);
        // Source byte 1 carries pairs 4 and 5.
        out[row_base + 1] = unswap_top(row_buf[4]) | unswap_bottom(row_buf[5]);
        // Source byte 0 carries pair 6 only (top).
        out[row_base + 0] = unswap_top(row_buf[6]);
    }
    out
}

/// Extract the runtime-ignored bytes from a packed CI2 glyph slot.
/// These bytes are preserved so that loading and saving the font produces
/// byte-identical output to the original ROM.
pub fn extract_unused_bytes(packed: &[u8; BYTES_PER_GLYPH]) -> GlyphUnusedBytes {
    let mut bytes_56_63 = [0u8; 8];
    bytes_56_63.copy_from_slice(&packed[56..64]);
    let mut row_byte0_low_nibbles = [0u8; GLYPH_HEIGHT];
    for row in 0..GLYPH_HEIGHT {
        row_byte0_low_nibbles[row] = packed[row * 4] & 0x0F;
    }
    GlyphUnusedBytes { bytes_56_63, row_byte0_low_nibbles }
}

/// Repack 196 palette indices plus the runtime-ignored `unused` bytes into 64
/// packed bytes, producing byte-identical output to the original ROM.
pub fn pack_glyph_full(pixels: &[u8; GLYPH_PIXELS], unused: &GlyphUnusedBytes) -> [u8; BYTES_PER_GLYPH] {
    let mut packed = pack_glyph(pixels);
    // Restore the runtime-ignored padding bytes at slots 56-63.
    packed[56..64].copy_from_slice(&unused.bytes_56_63);
    // Restore the lower nibble of each row's byte 0 (swap_top only reads the
    // upper nibble, so bits 3-0 were zeroed by pack_glyph).
    for row in 0..GLYPH_HEIGHT {
        packed[row * 4] = (packed[row * 4] & 0xF0) | (unused.row_byte0_low_nibbles[row] & 0x0F);
    }
    packed
}

/// Convenience: read glyph `glyph_index` from a multi-glyph buffer.
/// Caller must ensure `buffer.len() >= (glyph_index + 1) * BYTES_PER_GLYPH`.
pub fn unpack_glyph_at(buffer: &[u8], glyph_index: usize) -> [u8; GLYPH_PIXELS] {
    let base = glyph_index * BYTES_PER_GLYPH;
    let mut packed = [0u8; BYTES_PER_GLYPH];
    packed.copy_from_slice(&buffer[base..base + BYTES_PER_GLYPH]);
    unpack_glyph(&packed)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn swap_top_matches_python_reference() {
        // Python reference: ((b & 0xC0) >> 2) | ((b & 0x30) >> 4)
        for b in 0u8..=255 {
            let expected = ((b & 0xC0) >> 2) | ((b & 0x30) >> 4);
            assert_eq!(swap_top(b), expected, "byte 0x{b:02X}");
        }
    }

    #[test]
    fn swap_bottom_matches_python_reference() {
        for b in 0u8..=255 {
            let expected = ((b & 0x0C) << 2) | (b & 0x03);
            assert_eq!(swap_bottom(b), expected, "byte 0x{b:02X}");
        }
    }

    #[test]
    fn unswap_top_inverts_swap_top_for_top_nibble_inputs() {
        // swap_top only reads bits 7-4 of the input; bits 3-0 of the input do
        // not affect output, so we can only round-trip when those are zero.
        for b in (0u8..=255).filter(|b| b & 0x0F == 0) {
            assert_eq!(unswap_top(swap_top(b)), b, "byte 0x{b:02X}");
        }
    }

    #[test]
    fn unswap_bottom_inverts_swap_bottom_for_bottom_nibble_inputs() {
        for b in (0u8..=255).filter(|b| b & 0xF0 == 0) {
            assert_eq!(unswap_bottom(swap_bottom(b)), b, "byte 0x{b:02X}");
        }
    }

    #[test]
    fn unswap_round_trip_through_2bit_pixel_pair() {
        // Canonical unpacked byte layout: pixel pair stored at bits 5-4 and 1-0
        // (zeros at bits 7-6, 3-2). swap_top and swap_bottom both produce this
        // shape, so the round-trip property to verify is:
        //   swap_*(unswap_*(out)) == out
        // for every byte 'out' that has zeros at bits 7-6 and 3-2.
        for top in 0..=3u8 {
            for bottom in 0..=3u8 {
                let out_byte = (top << 4) | bottom; // bits 5-4 = top, 1-0 = bottom
                assert_eq!(swap_top(unswap_top(out_byte)), out_byte);
                assert_eq!(swap_bottom(unswap_bottom(out_byte)), out_byte);
            }
        }
    }

    #[test]
    fn unpack_glyph_matches_python_reference() {
        // All-0xFF source bytes mean every nibble's top 2 bits are 11, so every
        // extracted 2-bit pixel value is 0x03. unpack_glyph returns 196 split
        // pixel indices directly (the "split into individual pixels" step that
        // the Python reference does in a separate function is fused in here).
        let mut packed = [0u8; BYTES_PER_GLYPH];
        for row in 0..GLYPH_HEIGHT {
            // Row layout per python: bytes at source_base + 0..=3 (read in
            // reverse), then advance by 4. Set offsets 0..=3 of each row's
            // 4-byte chunk to 0xFF.
            let row_base = row * 4;
            for i in 0..4 {
                packed[row_base + i] = 0xFF;
            }
        }
        let pixels = unpack_glyph(&packed);
        for &p in pixels.iter() {
            assert_eq!(p, 0x03, "every pixel of an all-0xFF source should be 3");
        }
    }

    #[test]
    fn pack_unpack_round_trips() {
        // Random-looking pixel pattern, all values 0..=3.
        let mut pixels = [0u8; GLYPH_PIXELS];
        for (i, p) in pixels.iter_mut().enumerate() {
            *p = (i % 4) as u8;
        }
        let packed = pack_glyph(&pixels);
        let unpacked = unpack_glyph(&packed);
        assert_eq!(unpacked, pixels);
    }

    #[test]
    fn pack_unpack_round_trips_zeroed() {
        let pixels = [0u8; GLYPH_PIXELS];
        let packed = pack_glyph(&pixels);
        assert_eq!(packed, [0u8; BYTES_PER_GLYPH]);
        assert_eq!(unpack_glyph(&packed), pixels);
    }

    #[test]
    fn unpack_glyph_at_uses_correct_offset() {
        // Two glyphs back-to-back. Set glyph 1's first row to nonzero,
        // glyph 0's bytes all zero. unpack_glyph_at(1) must read glyph 1.
        let mut buf = vec![0u8; 2 * BYTES_PER_GLYPH];
        buf[BYTES_PER_GLYPH..BYTES_PER_GLYPH + 4].copy_from_slice(&[0xFF, 0xFF, 0xFF, 0xFF]);
        let g0 = unpack_glyph_at(&buf, 0);
        let g1 = unpack_glyph_at(&buf, 1);
        assert_eq!(g0, [0u8; GLYPH_PIXELS]);
        // First row of g1 should be all 0x03.
        for x in 0..GLYPH_WIDTH {
            assert_eq!(g1[x], 0x03);
        }
    }

    #[test]
    fn pack_glyph_full_preserves_unused_bytes() {
        // Build a packed buffer with arbitrary bytes in the runtime-ignored
        // regions, unpack, repack, and assert byte-identical result.
        let mut packed = [0u8; BYTES_PER_GLYPH];
        // Fill the unused padding region (bytes 56-63) with recognizable bytes.
        for (i, b) in packed[56..64].iter_mut().enumerate() {
            *b = (0xA0 + i) as u8;
        }
        // Set bits 3-0 of each row's byte 0 to a recognizable nibble.
        for row in 0..GLYPH_HEIGHT {
            packed[row * 4] |= row as u8 & 0x0F;
        }
        // Some pixel data so unpack/repack sets the upper bits.
        packed[3] = 0x33; // row 0, byte 3
        let pixels = unpack_glyph(&packed);
        let unused = extract_unused_bytes(&packed);
        let repacked = pack_glyph_full(&pixels, &unused);
        assert_eq!(repacked, packed, "unused bytes / nibbles must round-trip");
    }
}
