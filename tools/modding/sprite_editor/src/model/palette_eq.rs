//! Byte-identical palette duplicate detection.
//!
//! Pure helper used by the three PNG/palette import flows to decide whether
//! an incoming palette is already present in the loaded sprite (or, for the
//! wizard, in the in-progress palette accumulator). Equality is exact:
//! same `TextureFormat`, same `Vec<Rgba>` (per-entry exact equality on all
//! four 8-bit channels, in the same order, including any padding entries).

use crate::model::{Palette, Rgba, TextureFormat};

/// Returns the lowest matching palette index in `haystack`, or `None` if no
/// palette in the haystack byte-equals `needle`.
///
/// Equality requires `needle_format == palette.format` and
/// `needle == palette.colors[..]`. Iteration order is the haystack iterator's
/// order; passing `&BTreeMap<u32, Palette>` yields ascending key order, so
/// "lowest-index wins" is the natural and stable result.
pub fn find_matching_palette<'a>(
    needle: &[Rgba],
    needle_format: TextureFormat,
    haystack: impl IntoIterator<Item = (&'a u32, &'a Palette)>,
) -> Option<u32> {
    for (idx, pal) in haystack {
        if pal.format == needle_format && pal.colors == needle {
            return Some(*idx);
        }
    }
    None
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::BTreeMap;

    fn pal(idx: u32, format: TextureFormat, colors: Vec<Rgba>) -> Palette {
        Palette { index: idx, format, colors }
    }

    fn ci8_grey(n: u8) -> Vec<Rgba> {
        let mut v: Vec<Rgba> = (0..n).map(|i| Rgba::new(i, i, i, 255)).collect();
        v.resize(256, Rgba::default());
        v
    }

    #[test]
    fn empty_haystack_returns_none() {
        let haystack: BTreeMap<u32, Palette> = BTreeMap::new();
        let needle = ci8_grey(4);
        assert_eq!(find_matching_palette(&needle, TextureFormat::Ci8, &haystack), None);
    }

    #[test]
    fn single_match_returns_index() {
        let mut haystack: BTreeMap<u32, Palette> = BTreeMap::new();
        haystack.insert(7, pal(7, TextureFormat::Ci8, ci8_grey(4)));
        let needle = ci8_grey(4);
        assert_eq!(find_matching_palette(&needle, TextureFormat::Ci8, &haystack), Some(7));
    }

    #[test]
    fn format_mismatch_returns_none() {
        // Same colors, different format → no match. (In practice CI4 has 16
        // entries vs CI8's 256, so the lengths also differ; this test asserts
        // we don't accidentally match by treating format as ignorable.)
        let mut ci4_colors: Vec<Rgba> = vec![Rgba::new(1, 2, 3, 255); 16];
        ci4_colors.resize(16, Rgba::default());
        let mut haystack: BTreeMap<u32, Palette> = BTreeMap::new();
        haystack.insert(0, pal(0, TextureFormat::Ci4, ci4_colors.clone()));
        // Build a CI8 needle that contains the same first 16 entries; format
        // differs, so the function must not match.
        let mut needle = ci4_colors.clone();
        needle.resize(256, Rgba::default());
        assert_eq!(find_matching_palette(&needle, TextureFormat::Ci8, &haystack), None);
    }

    #[test]
    fn different_colors_return_none() {
        let mut haystack: BTreeMap<u32, Palette> = BTreeMap::new();
        haystack.insert(0, pal(0, TextureFormat::Ci8, ci8_grey(4)));
        let needle = ci8_grey(5); // differs at index 4
        assert_eq!(find_matching_palette(&needle, TextureFormat::Ci8, &haystack), None);
    }

    #[test]
    fn multiple_matches_lowest_index_wins() {
        let mut haystack: BTreeMap<u32, Palette> = BTreeMap::new();
        haystack.insert(2, pal(2, TextureFormat::Ci8, ci8_grey(4)));
        haystack.insert(0, pal(0, TextureFormat::Ci8, ci8_grey(4)));
        haystack.insert(5, pal(5, TextureFormat::Ci8, ci8_grey(4)));
        let needle = ci8_grey(4);
        assert_eq!(find_matching_palette(&needle, TextureFormat::Ci8, &haystack), Some(0));
    }

    #[test]
    fn padding_byte_difference_returns_none() {
        // Two palettes whose meaningful prefixes match but whose padding
        // entries differ must NOT match. This guards against drift if a
        // future change to import_indexed_png shifts the padding default.
        let prefix: Vec<Rgba> = (0..4).map(|i| Rgba::new(i, i, i, 255)).collect();

        let mut padded_zero: Vec<Rgba> = prefix.clone();
        padded_zero.resize(256, Rgba::new(0, 0, 0, 0));

        let mut padded_opaque: Vec<Rgba> = prefix.clone();
        padded_opaque.resize(256, Rgba::new(0, 0, 0, 255));

        let mut haystack: BTreeMap<u32, Palette> = BTreeMap::new();
        haystack.insert(0, pal(0, TextureFormat::Ci8, padded_zero));
        assert_eq!(
            find_matching_palette(&padded_opaque, TextureFormat::Ci8, &haystack),
            None,
        );
    }
}
