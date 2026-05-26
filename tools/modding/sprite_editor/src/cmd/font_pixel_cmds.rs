//! Commands that mutate font glyph pixels.

use crate::cmd::{CmdError, Command, DirtyMask, MergeResult};
use crate::model::GLYPH_PIXELS;
use crate::state::editor::Editor;

#[derive(Debug)]
pub struct SetGlyphPixel {
    pub glyph_index: u32,
    pub pixel_index: u16,
    pub new: u8,
    old: u8,
    applied_at: u128,
}

impl SetGlyphPixel {
    pub fn new(glyph_index: u32, pixel_index: u16, new: u8, editor: &Editor) -> Option<Self> {
        if new > 3 || (pixel_index as usize) >= GLYPH_PIXELS {
            return None;
        }
        let asset = editor.current_font()?;
        let glyph = asset.glyphs.get(glyph_index as usize)?;
        let old = glyph.pixels[pixel_index as usize];
        Some(Self { glyph_index, pixel_index, new, old, applied_at: 0 })
    }
}

impl Command for SetGlyphPixel {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let asset = state
            .current_font_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no font loaded".into()))?;
        let glyph = asset.glyphs.get_mut(self.glyph_index as usize).ok_or_else(|| {
            CmdError::InvalidTarget(format!("glyph {} out of range", self.glyph_index))
        })?;
        glyph.pixels[self.pixel_index as usize] = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let asset = state
            .current_font_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no font loaded".into()))?;
        let glyph = asset.glyphs.get_mut(self.glyph_index as usize).ok_or_else(|| {
            CmdError::InvalidTarget("glyph missing".into())
        })?;
        glyph.pixels[self.pixel_index as usize] = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set glyph {} pixel {} = {}", self.glyph_index, self.pixel_index, self.new)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::FONT_TEXTURE
    }
    fn try_merge(&mut self, other: &dyn Command) -> MergeResult {
        let Some(o) = (other as &dyn std::any::Any).downcast_ref::<SetGlyphPixel>() else {
            return MergeResult::Separate;
        };
        if o.glyph_index == self.glyph_index && o.pixel_index == self.pixel_index {
            self.new = o.new;
            MergeResult::Merged
        } else {
            MergeResult::Separate
        }
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct FillGlyph {
    pub glyph_index: u32,
    pub new: u8,
    old: Option<[u8; GLYPH_PIXELS]>,
    applied_at: u128,
}

impl FillGlyph {
    pub fn new(glyph_index: u32, new: u8) -> Option<Self> {
        if new > 3 {
            return None;
        }
        Some(Self { glyph_index, new, old: None, applied_at: 0 })
    }
}

impl Command for FillGlyph {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let asset = state
            .current_font_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no font loaded".into()))?;
        let glyph = asset.glyphs.get_mut(self.glyph_index as usize).ok_or_else(|| {
            CmdError::InvalidTarget(format!("glyph {} out of range", self.glyph_index))
        })?;
        self.old = Some(glyph.pixels);
        glyph.pixels = [self.new; GLYPH_PIXELS];
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let asset = state
            .current_font_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no font loaded".into()))?;
        let glyph = asset.glyphs.get_mut(self.glyph_index as usize).ok_or_else(|| {
            CmdError::InvalidTarget("glyph missing".into())
        })?;
        let prev = self.old.ok_or_else(|| CmdError::Invariant("revert before apply".into()))?;
        glyph.pixels = prev;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Fill glyph {} with {}", self.glyph_index, self.new)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::FONT_TEXTURE
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::cmd::history::History;
    use crate::model::{FontAsset, FontGlyph, FontPalette, GlyphUnusedBytes, Rgba};
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn editor_with_font() -> Editor {
        let glyphs: Vec<FontGlyph> = (0..4)
            .map(|i| FontGlyph { index: i, pixels: [0; 14 * 14], unused: GlyphUnusedBytes::default() })
            .collect();
        let mut palettes = BTreeMap::new();
        palettes.insert(1, FontPalette { index: 1, colors: [Rgba::default(); 16] });
        let asset = FontAsset {
            dir: PathBuf::new(),
            name: "test".into(),
            glyphs,
            palettes,
        };
        let mut e = Editor::new();
        e.loaded_font = Some(asset);
        e.active = crate::model::ActiveAsset::Font;
        e
    }

    #[test]
    fn set_pixel_apply_revert() {
        let mut e = editor_with_font();
        let mut h = History::default();
        let cmd = SetGlyphPixel::new(1, 5, 3, &e).unwrap();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(e.loaded_font.as_ref().unwrap().glyphs[1].pixels[5], 3);
        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_font.as_ref().unwrap().glyphs[1].pixels[5], 0);
    }

    #[test]
    fn rejects_out_of_range_value() {
        let e = editor_with_font();
        assert!(SetGlyphPixel::new(0, 0, 4, &e).is_none());
    }

    #[test]
    fn merges_same_pixel_target() {
        let mut e = editor_with_font();
        let mut h = History::default();
        h.execute(Box::new(SetGlyphPixel::new(1, 5, 1, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetGlyphPixel::new(1, 5, 2, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetGlyphPixel::new(1, 5, 3, &e).unwrap()), &mut e).unwrap();
        // Three pushes on same target → one entry on undo stack.
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 1);
    }

    #[test]
    fn does_not_merge_different_pixel_targets() {
        let mut e = editor_with_font();
        let mut h = History::default();
        h.execute(Box::new(SetGlyphPixel::new(1, 5, 1, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetGlyphPixel::new(1, 6, 2, &e).unwrap()), &mut e).unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 2);
    }

    #[test]
    fn fill_glyph_apply_revert() {
        let mut e = editor_with_font();
        let mut h = History::default();
        h.execute(Box::new(FillGlyph::new(2, 3).unwrap()), &mut e).unwrap();
        for &p in e.loaded_font.as_ref().unwrap().glyphs[2].pixels.iter() {
            assert_eq!(p, 3);
        }
        h.undo(&mut e).unwrap();
        for &p in e.loaded_font.as_ref().unwrap().glyphs[2].pixels.iter() {
            assert_eq!(p, 0);
        }
    }
}
