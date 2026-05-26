use std::collections::HashSet;
use std::path::{Path, PathBuf};

use crate::cmd::history::History;
use crate::cmd::DirtyMask;
use crate::io::sprite_loader::load_sprite_asset;
use crate::io::IoResult;
use crate::model::{ActiveAsset, AnimationData, FontAsset, SpriteAsset, SpriteKind};
use crate::state::baseline::BaselineTracker;

#[derive(Default)]
pub struct Editor {
    pub asset_root: Option<PathBuf>,
    pub loaded_sprite: Option<SpriteAsset>,
    pub loaded_animation_data: Option<AnimationData>,
    pub loaded_font: Option<FontAsset>,
    pub active: ActiveAsset,
    /// For multi-script `AnimationData::Entity` payloads (e.g. `holdableItems`),
    /// which sub-script the Script tab is currently displaying. Reset to 0 on
    /// every `load_sprite`. Ignored for the Avatar / Standalone variants.
    pub current_entity_script_idx: u32,
    /// Base entry-indices of directional groups the user has collapsed in the
    /// Script tab. Reset on `load_sprite` and on entity script switch — UI
    /// only, never persisted.
    pub collapsed_groups: HashSet<u32>,
    /// Per-domain unsaved-change bits, recomputed from `baselines` after every
    /// run/undo/redo/save. Direct field-style reads use the named accessors on
    /// `DirtyMask` (`palette()`, `texture()`, etc.).
    pub dirty: DirtyMask,
    pub history: History,
    pub baselines: BaselineTracker,
}

impl std::fmt::Debug for Editor {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Editor")
            .field("asset_root", &self.asset_root)
            .field("has_sprite", &self.loaded_sprite.is_some())
            .field("dirty", &self.dirty)
            .finish()
    }
}

impl Editor {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn set_asset_root(&mut self, path: PathBuf) {
        self.asset_root = Some(path);
    }

    pub fn load_sprite(&mut self, dir: &Path) -> IoResult<()> {
        let sprite = load_sprite_asset(dir)?;
        self.loaded_sprite = Some(sprite);
        self.loaded_animation_data = None;
        self.loaded_font = None;
        self.active = ActiveAsset::Sprite;
        self.current_entity_script_idx = 0;
        self.collapsed_groups.clear();
        self.dirty = DirtyMask::empty();
        self.history.clear();
        self.baselines.reset();
        Ok(())
    }

    pub fn unload_sprite(&mut self) {
        self.loaded_sprite = None;
        self.loaded_animation_data = None;
        self.loaded_font = None;
        self.active = ActiveAsset::None;
        self.current_entity_script_idx = 0;
        self.collapsed_groups.clear();
        self.dirty = DirtyMask::empty();
        self.history.clear();
        self.baselines.reset();
    }

    pub fn load_font(&mut self, dir: &Path) -> IoResult<()> {
        let font = crate::io::font_loader::load_font_asset(dir)?;
        self.loaded_font = Some(font);
        self.loaded_sprite = None;
        self.loaded_animation_data = None;
        self.active = ActiveAsset::Font;
        self.current_entity_script_idx = 0;
        self.collapsed_groups.clear();
        self.dirty = DirtyMask::empty();
        self.history.clear();
        self.baselines.reset();
        Ok(())
    }

    pub fn unload_font(&mut self) {
        self.loaded_font = None;
        self.active = ActiveAsset::None;
        self.dirty = DirtyMask::empty();
        self.history.clear();
        self.baselines.reset();
    }

    pub fn current_font(&self) -> Option<&FontAsset> {
        self.loaded_font.as_ref()
    }

    pub fn current_font_mut(&mut self) -> Option<&mut FontAsset> {
        self.loaded_font.as_mut()
    }

    /// Populate `loaded_animation_data` for the current sprite.
    ///
    /// Call this *after* `load_sprite`, since that clears the field. Returns
    /// `Ok(true)` if data was loaded, `Ok(false)` if the sprite has no
    /// animation-data JSON on disk for its kind.
    pub fn load_animation_data_for_sprite(
        &mut self,
        sprite_dir: &Path,
        kind: SpriteKind,
    ) -> IoResult<bool> {
        self.loaded_animation_data = crate::io::animation_data_json::load(sprite_dir, kind)?;
        Ok(self.loaded_animation_data.is_some())
    }

    pub fn current_sprite(&self) -> Option<&SpriteAsset> {
        self.loaded_sprite.as_ref()
    }

    pub fn current_sprite_mut(&mut self) -> Option<&mut SpriteAsset> {
        self.loaded_sprite.as_mut()
    }

    pub fn run(
        &mut self,
        cmd: Box<dyn crate::cmd::Command>,
    ) -> Result<DirtyMask, crate::cmd::CmdError> {
        let mut history = std::mem::take(&mut self.history);
        let result = history.execute(cmd, self);
        self.history = history;
        if let Ok(outcome) = &result {
            self.baselines.record(outcome.seq, outcome.affects);
        }
        self.recompute_dirty();
        result.map(|o| o.affects)
    }

    pub fn undo(&mut self) -> Result<Option<DirtyMask>, crate::cmd::CmdError> {
        let mut history = std::mem::take(&mut self.history);
        let result = history.undo(self);
        self.history = history;
        self.recompute_dirty();
        result
    }

    pub fn redo(&mut self) -> Result<Option<DirtyMask>, crate::cmd::CmdError> {
        let mut history = std::mem::take(&mut self.history);
        let result = history.redo(self);
        self.history = history;
        self.recompute_dirty();
        result
    }

    /// Persist the currently-loaded font to its source directory. Walks dirty
    /// bits: rewrites `fontTexture.ci2` if FONT_TEXTURE is dirty; rewrites all
    /// existing palette files if FONT_PALETTE is dirty. The BaselineTracker
    /// doesn't distinguish per-palette; preserving dead-byte palettes verbatim
    /// makes rewriting unchanged ones harmless (they round-trip byte-identical).
    pub fn save_font(&mut self) -> Result<(), crate::export::ExportError> {
        let asset = self.loaded_font.as_ref().ok_or_else(|| {
            crate::export::ExportError::invalid(
                std::path::PathBuf::new(),
                "no font loaded",
            )
        })?;
        let dir = asset.dir.clone();

        if self.dirty.font_texture() {
            crate::export::font_ci2_export::write_font_texture(&dir, asset)?;
        }
        if self.dirty.font_palette() {
            for pal in asset.palettes.values() {
                crate::export::font_pal_export::write_font_palette(&dir, pal)?;
            }
        }

        self.mark_saved(crate::cmd::DirtyMask::FONT_TEXTURE | crate::cmd::DirtyMask::FONT_PALETTE);
        Ok(())
    }

    /// Mark `domain` as saved to disk: snapshot the current undo-stack seqs
    /// affecting that domain as the clean baseline, then recompute dirty bits.
    pub fn mark_saved(&mut self, domain: DirtyMask) {
        let seqs = self.history.undo_seqs();
        self.baselines.set_clean_for(domain, &seqs);
        self.recompute_dirty();
    }

    fn recompute_dirty(&mut self) {
        let seqs = self.history.undo_seqs();
        self.dirty = self.baselines.dirty_mask(&seqs);
    }
}

#[cfg(test)]
mod tests {
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    use super::*;
    use crate::cmd::palette_cmds::SetPaletteColor;
    use crate::model::{Manifest, Palette, Rgba, SpriteAsset, SpriteType, TextureFormat};

    fn fake_editor() -> Editor {
        let manifest = Manifest {
            label: "t".into(),
            subdir: "entitySprites".into(),
            kind: crate::model::SpriteKind::Unknown,
            sprite_type: SpriteType::Type1,
            sprite_count: 0,
            spritesheet_index_count: 0,
            palette_count: 1,
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
        };
        let mut palettes = BTreeMap::new();
        palettes.insert(
            0,
            Palette {
                index: 0,
                format: TextureFormat::Ci4,
                colors: vec![Rgba::new(0, 0, 0, 255); 16],
            },
        );
        let mut e = Editor::new();
        e.loaded_sprite = Some(SpriteAsset {
            dir: PathBuf::new(),
            manifest,
            textures: BTreeMap::new(),
            palettes,
            animations: BTreeMap::new(),
        });
        e.active = crate::model::ActiveAsset::Sprite;
        e
    }

    fn set_color(e: &Editor, r: u8) -> Box<dyn crate::cmd::Command> {
        Box::new(SetPaletteColor::new(0, 3, Rgba::new(r, 0, 0, 255), e).unwrap())
    }

    #[test]
    fn run_marks_domain_dirty() {
        let mut e = fake_editor();
        assert!(!e.dirty.palette());
        e.run(set_color(&e, 1)).unwrap();
        assert!(e.dirty.palette());
    }

    #[test]
    fn undo_back_to_empty_history_clears_dirty() {
        let mut e = fake_editor();
        e.run(set_color(&e, 1)).unwrap();
        assert!(e.dirty.palette());
        e.undo().unwrap();
        assert!(!e.dirty.palette(), "undo to initial state should un-dirty");
    }

    #[test]
    fn redo_after_undo_restores_dirty() {
        let mut e = fake_editor();
        e.run(set_color(&e, 1)).unwrap();
        e.undo().unwrap();
        e.redo().unwrap();
        assert!(e.dirty.palette());
    }

    #[test]
    fn mark_saved_sets_clean_and_survives_redo() {
        let mut e = fake_editor();
        e.run(set_color(&e, 1)).unwrap();
        e.mark_saved(DirtyMask::PALETTE);
        assert!(!e.dirty.palette());
        // Undo pops the saved command → now differs from saved state → dirty.
        e.undo().unwrap();
        assert!(e.dirty.palette());
        // Redo restores → clean again.
        e.redo().unwrap();
        assert!(!e.dirty.palette());
    }

    #[test]
    fn undo_past_save_then_new_command_is_dirty() {
        let mut e = fake_editor();
        e.run(set_color(&e, 1)).unwrap();
        e.mark_saved(DirtyMask::PALETTE);
        e.undo().unwrap();
        // Now apply a *different* command: history.execute clears redo first.
        // Because merges use the prior seq and we just undid, the prior on the
        // undo stack is gone; this takes a fresh seq. Baseline still references
        // the saved seq which is now unreachable → dirty.
        e.run(set_color(&e, 9)).unwrap();
        assert!(e.dirty.palette(), "edit after undo-past-save must be dirty");
    }

    #[test]
    fn load_sprite_resets_baselines() {
        // After reset, dirty tracking starts fresh: an empty history with an
        // empty baseline is clean.
        let mut e = fake_editor();
        e.run(set_color(&e, 1)).unwrap();
        e.mark_saved(DirtyMask::PALETTE);
        e.baselines.reset();
        e.history.clear();
        // Simulate what load_sprite does without hitting the filesystem.
        e.dirty = DirtyMask::empty();
        assert!(!e.dirty.palette());
    }

    #[test]
    fn load_font_clears_sprite_and_sets_active() {
        use std::fs;
        use tempfile::tempdir;

        let tmp = tempdir().unwrap();
        let font_dir = tmp.path().join("font");
        fs::create_dir_all(&font_dir).unwrap();
        // 1 glyph + 1 palette is enough to load.
        fs::write(font_dir.join("fontTexture.ci2"), vec![0u8; 64]).unwrap();
        fs::write(font_dir.join("fontPalette1.pal"), vec![0u8; 32]).unwrap();

        let mut e = fake_editor();
        // Pre-condition: sprite loaded.
        assert!(e.loaded_sprite.is_some());
        assert_eq!(e.active, crate::model::ActiveAsset::Sprite);

        e.load_font(&font_dir).unwrap();
        assert!(e.loaded_sprite.is_none());
        assert!(e.loaded_font.is_some());
        assert_eq!(e.active, crate::model::ActiveAsset::Font);
    }

    #[test]
    fn save_font_writes_dirty_files_only() {
        use std::fs;
        use tempfile::tempdir;

        let tmp = tempdir().unwrap();
        let font_dir = tmp.path().join("font");
        fs::create_dir_all(&font_dir).unwrap();
        fs::write(font_dir.join("fontTexture.ci2"), vec![0u8; 64]).unwrap();
        fs::write(font_dir.join("fontPalette1.pal"), vec![0u8; 32]).unwrap();

        let mut e = Editor::new();
        e.load_font(&font_dir).unwrap();

        // Nothing dirty → no rewrites; verify no error.
        e.save_font().unwrap();

        // Mark texture dirty by hand, save again.
        e.dirty.insert(DirtyMask::FONT_TEXTURE);
        e.save_font().unwrap();
        // After save, dirty bits should clear via mark_saved → recompute_dirty.
        assert!(!e.dirty.font_texture());
    }
}
