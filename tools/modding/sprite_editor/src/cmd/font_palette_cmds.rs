//! Commands that mutate font palette colors. Only colors 0..=3 (the live ones
//! the CI2 unpacker addresses) are editable; attempts to set indices 4..=15
//! are rejected at construction.

use crate::cmd::{CmdError, Command, DirtyMask, MergeResult};
use crate::model::{Rgba, PALETTE_LIVE_COLORS};
use crate::state::editor::Editor;

#[derive(Debug)]
pub struct SetFontPaletteColor {
    pub palette_index: u32,
    pub color_index: u8,
    pub new: Rgba,
    old: Rgba,
    applied_at: u128,
}

impl SetFontPaletteColor {
    pub fn new(palette_index: u32, color_index: u8, new: Rgba, editor: &Editor) -> Option<Self> {
        if (color_index as usize) >= PALETTE_LIVE_COLORS {
            return None;
        }
        let asset = editor.current_font()?;
        let pal = asset.palettes.get(&palette_index)?;
        let old = pal.colors[color_index as usize];
        Some(Self { palette_index, color_index, new, old, applied_at: 0 })
    }
}

impl Command for SetFontPaletteColor {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let asset = state
            .current_font_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no font loaded".into()))?;
        let pal = asset.palettes.get_mut(&self.palette_index).ok_or_else(|| {
            CmdError::InvalidTarget(format!("palette {} missing", self.palette_index))
        })?;
        pal.colors[self.color_index as usize] = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let asset = state
            .current_font_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no font loaded".into()))?;
        let pal = asset
            .palettes
            .get_mut(&self.palette_index)
            .ok_or_else(|| CmdError::InvalidTarget("palette missing".into()))?;
        pal.colors[self.color_index as usize] = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set font palette {} color {}", self.palette_index, self.color_index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::FONT_PALETTE
    }
    fn try_merge(&mut self, other: &dyn Command) -> MergeResult {
        let Some(o) = (other as &dyn std::any::Any).downcast_ref::<SetFontPaletteColor>() else {
            return MergeResult::Separate;
        };
        if o.palette_index == self.palette_index && o.color_index == self.color_index {
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::cmd::history::History;
    use crate::model::{FontAsset, FontGlyph, FontPalette, GlyphUnusedBytes};
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn editor_with_font() -> Editor {
        let glyphs = vec![FontGlyph { index: 0, pixels: [0; 14 * 14], unused: GlyphUnusedBytes::default() }];
        let mut palettes = BTreeMap::new();
        palettes.insert(1, FontPalette { index: 1, colors: [Rgba::new(0, 0, 0, 255); 16] });
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
    fn apply_revert_round_trip() {
        let mut e = editor_with_font();
        let mut h = History::default();
        let cmd = SetFontPaletteColor::new(1, 2, Rgba::new(255, 0, 0, 255), &e).unwrap();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(
            e.loaded_font.as_ref().unwrap().palettes[&1].colors[2],
            Rgba::new(255, 0, 0, 255)
        );
        h.undo(&mut e).unwrap();
        assert_eq!(
            e.loaded_font.as_ref().unwrap().palettes[&1].colors[2],
            Rgba::new(0, 0, 0, 255)
        );
    }

    #[test]
    fn rejects_dead_byte_indices() {
        let e = editor_with_font();
        for ci in 4..=15 {
            assert!(
                SetFontPaletteColor::new(1, ci, Rgba::new(255, 0, 0, 255), &e).is_none(),
                "index {ci} should be rejected"
            );
        }
    }

    #[test]
    fn merges_same_target() {
        let mut e = editor_with_font();
        let mut h = History::default();
        h.execute(Box::new(SetFontPaletteColor::new(1, 0, Rgba::new(1, 0, 0, 255), &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetFontPaletteColor::new(1, 0, Rgba::new(2, 0, 0, 255), &e).unwrap()), &mut e).unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() { count += 1; }
        assert_eq!(count, 1);
    }
}
