//! Commands that mutate palette data.

use crate::cmd::{CmdError, Command, DirtyMask, MergeResult};
use crate::model::Rgba;
use crate::model::{Palette, TextureFormat};
use crate::state::editor::Editor;

#[derive(Debug)]
pub struct SetPaletteColor {
    pub palette_index: u32,
    pub color_index: u32,
    pub old: Rgba,
    pub new: Rgba,
    applied_at: u128,
}

impl SetPaletteColor {
    pub fn new(palette_index: u32, color_index: u32, new: Rgba, editor: &Editor) -> Option<Self> {
        let p = editor.current_sprite()?.palettes.get(&palette_index)?;
        let old = *p.colors.get(color_index as usize)?;
        Some(Self { palette_index, color_index, old, new, applied_at: 0 })
    }
}

impl Command for SetPaletteColor {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        let p = s.palettes.get_mut(&self.palette_index).ok_or_else(|| {
            CmdError::InvalidTarget(format!("palette {} missing", self.palette_index))
        })?;
        let cell = p.colors.get_mut(self.color_index as usize).ok_or_else(|| {
            CmdError::InvalidTarget(format!("color {} out of range", self.color_index))
        })?;
        *cell = self.new;
        Ok(())
    }

    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        let p = s
            .palettes
            .get_mut(&self.palette_index)
            .ok_or_else(|| CmdError::InvalidTarget("palette missing".into()))?;
        let cell = p
            .colors
            .get_mut(self.color_index as usize)
            .ok_or_else(|| CmdError::InvalidTarget("color out of range".into()))?;
        *cell = self.old;
        Ok(())
    }

    fn describe(&self) -> String {
        format!("Set palette {} color {}", self.palette_index, self.color_index)
    }

    fn affects(&self) -> DirtyMask {
        DirtyMask::PALETTE
    }

    fn try_merge(&mut self, other: &dyn Command) -> MergeResult {
        let Some(o) = (other as &dyn std::any::Any).downcast_ref::<SetPaletteColor>() else {
            return MergeResult::Separate;
        };
        if o.palette_index == self.palette_index && o.color_index == self.color_index {
            self.new = o.new;
            MergeResult::Merged
        } else {
            MergeResult::Separate
        }
    }

    fn set_applied_at(&mut self, millis: u128) {
        self.applied_at = millis;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

/// Per-apply snapshot of the original-ROM palette layout fields, captured the
/// first time a `CreatePalette` runs against a sprite that's still in the
/// preservation path. Used to switch the sprite to the fresh-sprite assembly
/// path so the new palette actually makes it into the texture binary.
#[derive(Debug, Default, Clone)]
struct PreservationSnapshot {
    palette_index: Vec<u32>,
    palette_index_count: u32,
    palette_index_end_bytes: Vec<u8>,
}

#[derive(Debug)]
pub struct CreatePalette {
    pub palette_index: u32,
    pub format: TextureFormat,
    pub initial_colors: Vec<Rgba>,
    /// `Some` when `apply` cleared preservation-path fields so `revert` can
    /// restore them; `None` when the sprite was already in fresh-sprite mode.
    preservation_snapshot: Option<PreservationSnapshot>,
    applied_at: u128,
}

impl CreatePalette {
    pub fn new(palette_index: u32, format: TextureFormat, initial_colors: Vec<Rgba>) -> Self {
        assert_eq!(
            initial_colors.len(),
            format.palette_len(),
            "initial_colors length must match format"
        );
        Self {
            palette_index,
            format,
            initial_colors,
            preservation_snapshot: None,
            applied_at: 0,
        }
    }
}

impl Command for CreatePalette {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if s.palettes.contains_key(&self.palette_index) {
            return Err(CmdError::Invariant(format!(
                "palette {} already exists",
                self.palette_index
            )));
        }
        s.palettes.insert(
            self.palette_index,
            Palette {
                index: self.palette_index,
                format: self.format,
                colors: self.initial_colors.clone(),
            },
        );
        s.manifest.palette_count = s.palettes.len() as u32;
        // Original-ROM sprites carry a `palette_index` Vec<u32> describing the
        // ROM's exact byte layout of the palette section. The assembler's
        // preservation path iterates that vector and silently drops any palette
        // beyond it — so on the first new palette we switch the sprite to the
        // fresh-sprite path (which packages every palette referenced by
        // `sprite_to_palette`). The snapshot is restored by `revert`.
        if !s.manifest.palette_index.is_empty()
            || s.manifest.palette_index_count != 0
            || !s.manifest.palette_index_end_bytes.is_empty()
        {
            self.preservation_snapshot = Some(PreservationSnapshot {
                palette_index: std::mem::take(&mut s.manifest.palette_index),
                palette_index_count: std::mem::take(&mut s.manifest.palette_index_count),
                palette_index_end_bytes: std::mem::take(&mut s.manifest.palette_index_end_bytes),
            });
        }
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        s.palettes.remove(&self.palette_index);
        s.manifest.palette_count = s.palettes.len() as u32;
        if let Some(snap) = self.preservation_snapshot.take() {
            s.manifest.palette_index = snap.palette_index;
            s.manifest.palette_index_count = snap.palette_index_count;
            s.manifest.palette_index_end_bytes = snap.palette_index_end_bytes;
        }
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Create palette {}", self.palette_index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::PALETTE
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct DeletePalette {
    pub palette_index: u32,
    old: Option<Palette>,
    applied_at: u128,
}

impl DeletePalette {
    pub fn new(palette_index: u32) -> Self {
        Self { palette_index, old: None, applied_at: 0 }
    }
}

impl Command for DeletePalette {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        // Refuse if any texture still references it.
        let in_use: Vec<u32> = s
            .textures
            .values()
            .filter(|t| t.palette_index == self.palette_index)
            .map(|t| t.index)
            .collect();
        if !in_use.is_empty() {
            return Err(CmdError::Invariant(format!(
                "palette {} still referenced by textures {:?}",
                self.palette_index, in_use
            )));
        }
        self.old = s.palettes.remove(&self.palette_index);
        if self.old.is_none() {
            return Err(CmdError::InvalidTarget(format!(
                "palette {} not found",
                self.palette_index
            )));
        }
        s.manifest.palette_count = s.palettes.len() as u32;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if let Some(p) = self.old.take() {
            s.palettes.insert(self.palette_index, p);
            s.manifest.palette_count = s.palettes.len() as u32;
        }
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Delete palette {}", self.palette_index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::PALETTE
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
    use crate::model::{Manifest, Palette, Rgba, SpriteAsset, SpriteType, TextureFormat};
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn fake_editor_with_palette() -> Editor {
        let manifest = Manifest {
            label: "x".into(),
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
                colors: vec![Rgba::new(10, 20, 30, 255); 16],
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
        e
    }

    #[test]
    fn apply_revert_round_trip() {
        let mut e = fake_editor_with_palette();
        let before = e.loaded_sprite.clone().unwrap();
        let cmd = SetPaletteColor::new(0, 3, Rgba::new(1, 2, 3, 4), &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().palettes[&0].colors[3], Rgba::new(1, 2, 3, 4));
        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap(), &before);
        h.redo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().palettes[&0].colors[3], Rgba::new(1, 2, 3, 4));
    }

    #[test]
    fn consecutive_same_target_merges() {
        let mut e = fake_editor_with_palette();
        let mut h = History::default();
        h.execute(
            Box::new(SetPaletteColor::new(0, 3, Rgba::new(1, 0, 0, 255), &e).unwrap()),
            &mut e,
        )
        .unwrap();
        h.execute(
            Box::new(SetPaletteColor::new(0, 3, Rgba::new(2, 0, 0, 255), &e).unwrap()),
            &mut e,
        )
        .unwrap();
        // Expect only one entry on the undo stack because they merged.
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 1);
    }

    #[test]
    fn create_and_delete_round_trip() {
        let mut e = fake_editor_with_palette();
        let mut h = History::default();
        let colors = vec![Rgba::new(255, 0, 0, 255); 16];
        h.execute(Box::new(CreatePalette::new(1, TextureFormat::Ci4, colors)), &mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().palettes.contains_key(&1));
        h.undo(&mut e).unwrap();
        assert!(!e.loaded_sprite.as_ref().unwrap().palettes.contains_key(&1));
    }

    #[test]
    fn create_palette_clears_preservation_fields_and_revert_restores_them() {
        // Original-ROM sprites have palette_index populated. Without the clear,
        // the assembler's preservation path drops any palette beyond the
        // original count.
        let mut e = fake_editor_with_palette();
        {
            let m = &mut e.loaded_sprite.as_mut().unwrap().manifest;
            m.palette_index = vec![0x100, 0x200, 0x300];
            m.palette_index_count = 3;
            m.palette_index_end_bytes = vec![0xAA, 0xBB];
        }

        let mut h = History::default();
        let colors = vec![Rgba::new(255, 0, 0, 255); 16];
        h.execute(Box::new(CreatePalette::new(1, TextureFormat::Ci4, colors)), &mut e).unwrap();

        let m = &e.loaded_sprite.as_ref().unwrap().manifest;
        assert!(m.palette_index.is_empty(), "palette_index should be cleared");
        assert_eq!(m.palette_index_count, 0);
        assert!(m.palette_index_end_bytes.is_empty());

        h.undo(&mut e).unwrap();
        let m = &e.loaded_sprite.as_ref().unwrap().manifest;
        assert_eq!(m.palette_index, vec![0x100, 0x200, 0x300]);
        assert_eq!(m.palette_index_count, 3);
        assert_eq!(m.palette_index_end_bytes, vec![0xAA, 0xBB]);
    }

    #[test]
    fn create_palette_no_op_on_already_fresh_sprite() {
        // If the sprite is already in the fresh-sprite path, CreatePalette must
        // not invent a snapshot — revert would otherwise overwrite empty fields
        // with empty fields, which is fine, but we also want the snapshot to be
        // None so subsequent CreatePalettes don't accidentally restore stale
        // state.
        let mut e = fake_editor_with_palette();
        // fake_editor_with_palette already has all preservation fields empty.
        let mut h = History::default();
        let colors = vec![Rgba::new(255, 0, 0, 255); 16];
        h.execute(Box::new(CreatePalette::new(1, TextureFormat::Ci4, colors)), &mut e).unwrap();
        let m = &e.loaded_sprite.as_ref().unwrap().manifest;
        assert!(m.palette_index.is_empty());
        h.undo(&mut e).unwrap();
        let m = &e.loaded_sprite.as_ref().unwrap().manifest;
        assert!(m.palette_index.is_empty());
    }

    #[test]
    fn delete_refuses_when_palette_in_use() {
        use crate::model::{Texture, TextureFormat};
        let mut e = fake_editor_with_palette();
        e.loaded_sprite.as_mut().unwrap().textures.insert(
            0,
            Texture {
                index: 0,
                width: 1,
                height: 1,
                format: TextureFormat::Ci8,
                palette_index: 0,
                pixels: vec![0],
            },
        );
        let mut cmd = DeletePalette::new(0);
        let err = cmd.apply(&mut e).unwrap_err();
        assert!(matches!(err, CmdError::Invariant(_)), "got: {err:?}");
    }
}
