//! Texture mutation commands.

use crate::cmd::{CmdError, Command, DirtyMask};
use crate::model::{Texture, TextureFormat, TextureMeta};
use crate::state::editor::Editor;

#[derive(Debug)]
pub struct AssignTextureToPalette {
    pub texture_index: u32,
    pub old: u32,
    pub new: u32,
    applied_at: u128,
}

impl AssignTextureToPalette {
    pub fn new(texture_index: u32, new: u32, editor: &Editor) -> Option<Self> {
        let t = editor.current_sprite()?.textures.get(&texture_index)?;
        Some(Self { texture_index, old: t.palette_index, new, applied_at: 0 })
    }
}

impl Command for AssignTextureToPalette {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if !s.palettes.contains_key(&self.new) {
            return Err(CmdError::InvalidTarget(format!("palette {} missing", self.new)));
        }
        let t = s.textures.get_mut(&self.texture_index).ok_or_else(|| {
            CmdError::InvalidTarget(format!("texture {} missing", self.texture_index))
        })?;
        t.palette_index = self.new;
        // Keep manifest in sync.
        if let Some(tm) = s.manifest.textures.iter_mut().find(|m| m.index == self.texture_index) {
            tm.palette_index = self.new;
        }
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        let t = s.textures.get_mut(&self.texture_index).ok_or_else(|| {
            CmdError::InvalidTarget(format!("texture {} missing", self.texture_index))
        })?;
        t.palette_index = self.old;
        if let Some(tm) = s.manifest.textures.iter_mut().find(|m| m.index == self.texture_index) {
            tm.palette_index = self.old;
        }
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Assign texture {} → palette {} (was {})", self.texture_index, self.new, self.old)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::TEXTURE
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
struct CreateTextureSnapshot {
    old_sprite_count: u32,
    old_spritesheet_index_count: u32,
    /// `Some(prior_len)` when apply extended `sprite_to_palette`; `None` when it left it untouched
    /// (empty). Used by revert to truncate back to the prior length.
    prior_s2p_len: Option<usize>,
}

#[derive(Debug)]
pub struct CreateTexture {
    pub index: u32,
    pub width: u16,
    pub height: u16,
    pub format: TextureFormat,
    pub palette_index: u32,
    pub pixels: Vec<u8>,
    snapshot: Option<CreateTextureSnapshot>,
    applied_at: u128,
}

impl CreateTexture {
    pub fn new(
        index: u32,
        width: u16,
        height: u16,
        format: TextureFormat,
        palette_index: u32,
        pixels: Vec<u8>,
    ) -> Self {
        Self {
            index,
            width,
            height,
            format,
            palette_index,
            pixels,
            snapshot: None,
            applied_at: 0,
        }
    }
}

impl Command for CreateTexture {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if s.textures.contains_key(&self.index) {
            return Err(CmdError::Invariant(format!(
                "texture {} already exists",
                self.index
            )));
        }
        if !s.palettes.contains_key(&self.palette_index) {
            return Err(CmdError::InvalidTarget(format!(
                "palette {} missing",
                self.palette_index
            )));
        }
        if self.palette_index > u8::MAX as u32 {
            return Err(CmdError::Invariant(format!(
                "palette_index {} doesn't fit in sprite_to_palette byte",
                self.palette_index
            )));
        }
        let old_s2p_len = s.manifest.sprite_to_palette.len();
        let snap_placeholder_sprite_count = s.manifest.sprite_count;
        let snap_placeholder_spritesheet_count = s.manifest.spritesheet_index_count;
        s.textures.insert(
            self.index,
            Texture {
                index: self.index,
                width: self.width,
                height: self.height,
                format: self.format,
                palette_index: self.palette_index,
                pixels: self.pixels.clone(),
            },
        );
        // Insert TextureMeta in sort order.
        let pos = s.manifest.textures.partition_point(|m| m.index < self.index);
        s.manifest.textures.insert(
            pos,
            TextureMeta {
                index: self.index,
                width: self.width,
                height: self.height,
                format: self.format,
                palette_index: self.palette_index,
            },
        );
        // Extend sprite_to_palette only when it is already in use.
        let prior_s2p_len;
        if !s.manifest.sprite_to_palette.is_empty() {
            prior_s2p_len = Some(old_s2p_len);
            let needed = self.index as usize + 1;
            if s.manifest.sprite_to_palette.len() < needed {
                s.manifest.sprite_to_palette.resize(needed, 0);
            }
            s.manifest.sprite_to_palette[self.index as usize] = self.palette_index as u8;
        } else {
            prior_s2p_len = None;
        }
        let snap = CreateTextureSnapshot {
            old_sprite_count: snap_placeholder_sprite_count,
            old_spritesheet_index_count: snap_placeholder_spritesheet_count,
            prior_s2p_len,
        };
        // Extend counts to cover the new index.
        let needed_count = self.index + 1;
        if s.manifest.sprite_count < needed_count {
            s.manifest.sprite_count = needed_count;
        }
        if s.manifest.spritesheet_index_count < needed_count {
            s.manifest.spritesheet_index_count = needed_count;
        }
        self.snapshot = Some(snap);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let snap = self
            .snapshot
            .take()
            .ok_or_else(|| CmdError::Invariant("CreateTexture::revert with no snapshot".into()))?;
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        s.textures.remove(&self.index);
        s.manifest.textures.retain(|m| m.index != self.index);
        if let Some(prior_len) = snap.prior_s2p_len {
            s.manifest.sprite_to_palette.truncate(prior_len);
        }
        s.manifest.sprite_count = snap.old_sprite_count;
        s.manifest.spritesheet_index_count = snap.old_spritesheet_index_count;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Create texture {}", self.index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::TEXTURE
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct ReplaceTextureFromCi {
    pub texture_index: u32,
    pub old_pixels: Vec<u8>,
    pub new_pixels: Vec<u8>,
    pub old_wh: (u16, u16),
    pub new_wh: (u16, u16),
    applied_at: u128,
}

impl Command for ReplaceTextureFromCi {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite".into()))?;
        let t = s
            .textures
            .get_mut(&self.texture_index)
            .ok_or_else(|| CmdError::InvalidTarget("texture missing".into()))?;
        t.pixels = self.new_pixels.clone();
        t.width = self.new_wh.0;
        t.height = self.new_wh.1;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite".into()))?;
        let t = s
            .textures
            .get_mut(&self.texture_index)
            .ok_or_else(|| CmdError::InvalidTarget("texture missing".into()))?;
        t.pixels = self.old_pixels.clone();
        t.width = self.old_wh.0;
        t.height = self.old_wh.1;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Replace texture {} from CI", self.texture_index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::TEXTURE
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
struct DeletedTextureSnapshot {
    texture: Texture,
    meta: TextureMeta,
    meta_position: usize,
    s2p_byte: Option<u8>, // None when sprite_to_palette is empty or index is out of range
}

#[derive(Debug)]
pub struct DeleteTextureData {
    pub index: u32,
    snapshot: Option<DeletedTextureSnapshot>,
    applied_at: u128,
}

impl DeleteTextureData {
    pub fn new(index: u32) -> Self {
        Self { index, snapshot: None, applied_at: 0 }
    }
}

impl Command for DeleteTextureData {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        // Validate both lookups before mutating.
        if !s.textures.contains_key(&self.index) {
            return Err(CmdError::InvalidTarget(format!("texture {} not found", self.index)));
        }
        let meta_position = s
            .manifest
            .textures
            .iter()
            .position(|m| m.index == self.index)
            .ok_or_else(|| CmdError::Invariant(format!("manifest missing texture {}", self.index)))?;
        // Both confirmed present — now mutate.
        let texture = s.textures.remove(&self.index).expect("validated above");
        let meta = s.manifest.textures.remove(meta_position);
        let s2p_byte = if !s.manifest.sprite_to_palette.is_empty()
            && (self.index as usize) < s.manifest.sprite_to_palette.len()
        {
            let prev = s.manifest.sprite_to_palette[self.index as usize];
            s.manifest.sprite_to_palette[self.index as usize] = 0;
            Some(prev)
        } else {
            None
        };
        self.snapshot = Some(DeletedTextureSnapshot { texture, meta, meta_position, s2p_byte });
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let snap = self.snapshot.take().ok_or_else(|| {
            CmdError::Invariant("DeleteTextureData::revert with no snapshot".into())
        })?;
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        s.textures.insert(self.index, snap.texture);
        let pos = snap.meta_position.min(s.manifest.textures.len());
        s.manifest.textures.insert(pos, snap.meta);
        if let Some(prev) = snap.s2p_byte {
            debug_assert!(
                (self.index as usize) < s.manifest.sprite_to_palette.len(),
                "s2p shrunk between apply and revert for texture {}",
                self.index
            );
            if (self.index as usize) < s.manifest.sprite_to_palette.len() {
                s.manifest.sprite_to_palette[self.index as usize] = prev;
            }
        }
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Delete texture {} (gap)", self.index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::TEXTURE
    }
    fn set_applied_at(&mut self, m: u128) {
        self.applied_at = m;
    }
    fn applied_at(&self) -> u128 {
        self.applied_at
    }
}

#[derive(Debug)]
pub struct ReplaceTextureFromPng {
    index: u32,
    old_pixels: Vec<u8>,
    old_wh: (u16, u16),
    old_palette_index: u32,
    new_pixels: Vec<u8>,
    new_wh: (u16, u16),
    new_palette_index: u32,
    applied_at: u128,
}

impl ReplaceTextureFromPng {
    pub fn new(
        index: u32,
        new_pixels: Vec<u8>,
        new_wh: (u16, u16),
        new_palette_index: u32,
        editor: &Editor,
    ) -> Option<Self> {
        let t = editor.current_sprite()?.textures.get(&index)?;
        Some(Self {
            index,
            old_pixels: t.pixels.clone(),
            old_wh: (t.width, t.height),
            old_palette_index: t.palette_index,
            new_pixels,
            new_wh,
            new_palette_index,
            applied_at: 0,
        })
    }
}

impl Command for ReplaceTextureFromPng {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if !s.textures.contains_key(&self.index) {
            return Err(CmdError::InvalidTarget(format!("texture {} missing", self.index)));
        }
        let meta_position = s
            .manifest
            .textures
            .iter()
            .position(|m| m.index == self.index)
            .ok_or_else(|| CmdError::Invariant(format!("manifest missing texture {}", self.index)))?;
        // Both confirmed — now mutate.
        let t = s.textures.get_mut(&self.index).expect("validated above");
        t.pixels = self.new_pixels.clone();
        t.width = self.new_wh.0;
        t.height = self.new_wh.1;
        t.palette_index = self.new_palette_index;
        let m = &mut s.manifest.textures[meta_position];
        m.width = self.new_wh.0;
        m.height = self.new_wh.1;
        m.palette_index = self.new_palette_index;
        if !s.manifest.sprite_to_palette.is_empty() {
            debug_assert!(
                self.new_palette_index <= u8::MAX as u32,
                "palette_index {} doesn't fit in sprite_to_palette byte",
                self.new_palette_index
            );
            if let Some(slot) = s.manifest.sprite_to_palette.get_mut(self.index as usize) {
                *slot = self.new_palette_index as u8;
            } else {
                debug_assert!(
                    false,
                    "sprite_to_palette is non-empty but index {} is out of range",
                    self.index
                );
            }
        }
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state
            .current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if !s.textures.contains_key(&self.index) {
            return Err(CmdError::InvalidTarget(format!("texture {} missing", self.index)));
        }
        let meta_position = s
            .manifest
            .textures
            .iter()
            .position(|m| m.index == self.index)
            .ok_or_else(|| CmdError::Invariant(format!("manifest missing texture {}", self.index)))?;
        // Both confirmed — now mutate.
        let t = s.textures.get_mut(&self.index).expect("validated above");
        t.pixels = self.old_pixels.clone();
        t.width = self.old_wh.0;
        t.height = self.old_wh.1;
        t.palette_index = self.old_palette_index;
        let m = &mut s.manifest.textures[meta_position];
        m.width = self.old_wh.0;
        m.height = self.old_wh.1;
        m.palette_index = self.old_palette_index;
        if !s.manifest.sprite_to_palette.is_empty() {
            debug_assert!(
                self.old_palette_index <= u8::MAX as u32,
                "palette_index {} doesn't fit in sprite_to_palette byte",
                self.old_palette_index
            );
            if let Some(slot) = s.manifest.sprite_to_palette.get_mut(self.index as usize) {
                *slot = self.old_palette_index as u8;
            } else {
                debug_assert!(
                    false,
                    "sprite_to_palette is non-empty but index {} is out of range",
                    self.index
                );
            }
        }
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Replace texture {} from PNG", self.index)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::TEXTURE
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
    use crate::model::{
        Manifest, Palette, Rgba, SpriteAsset, SpriteType, Texture, TextureFormat, TextureMeta,
    };
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn fake_editor() -> Editor {
        let manifest = Manifest {
            label: "x".into(),
            subdir: "x".into(),
            kind: crate::model::SpriteKind::Unknown,
            sprite_type: SpriteType::Type1,
            sprite_count: 1,
            spritesheet_index_count: 1,
            palette_count: 2,
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
            textures: vec![TextureMeta {
                index: 0,
                width: 1,
                height: 1,
                format: TextureFormat::Ci8,
                palette_index: 0,
            }],
        };
        let mut palettes = BTreeMap::new();
        palettes.insert(
            0,
            Palette { index: 0, format: TextureFormat::Ci8, colors: vec![Rgba::default(); 256] },
        );
        palettes.insert(
            1,
            Palette { index: 1, format: TextureFormat::Ci8, colors: vec![Rgba::default(); 256] },
        );
        let mut textures = BTreeMap::new();
        textures.insert(
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
        let mut e = Editor::new();
        e.loaded_sprite = Some(SpriteAsset {
            dir: PathBuf::new(),
            manifest,
            textures,
            palettes,
            animations: BTreeMap::new(),
        });
        e
    }

    #[test]
    fn assign_round_trip_updates_manifest() {
        let mut e = fake_editor();
        let mut h = History::default();
        let cmd = AssignTextureToPalette::new(0, 1, &e).unwrap();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert_eq!(s.textures[&0].palette_index, 1);
        assert_eq!(s.manifest.textures[0].palette_index, 1);
        h.undo(&mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert_eq!(s.textures[&0].palette_index, 0);
        assert_eq!(s.manifest.textures[0].palette_index, 0);
    }

    #[test]
    fn assign_rejects_unknown_palette() {
        let mut e = fake_editor();
        let mut cmd = AssignTextureToPalette::new(0, 99, &e).unwrap();
        let err = cmd.apply(&mut e).unwrap_err();
        assert!(matches!(err, CmdError::InvalidTarget(_)));
    }

    #[test]
    fn create_texture_apply_inserts_into_textures_manifest_and_s2p() {
        let mut e = fake_editor();
        // fake_editor starts with one texture index=0, sprite_count=1, s2p empty.
        // Force s2p non-empty so we can verify it is extended.
        e.loaded_sprite.as_mut().unwrap().manifest.sprite_to_palette = vec![0u8];

        let mut h = History::default();
        h.execute(
            Box::new(CreateTexture::new(
                5, 8, 8, TextureFormat::Ci8, 1, vec![0u8; 64],
            )),
            &mut e,
        )
        .unwrap();

        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(s.textures.contains_key(&5));
        assert_eq!(s.textures[&5].width, 8);
        assert_eq!(s.textures[&5].palette_index, 1);
        // Manifest entry inserted in sort order: existing index 0 first, new index 5 second.
        assert_eq!(s.manifest.textures.len(), 2);
        assert_eq!(s.manifest.textures[0].index, 0);
        assert_eq!(s.manifest.textures[1].index, 5);
        // sprite_count and spritesheet_index_count extended to cover index 5.
        assert_eq!(s.manifest.sprite_count, 6);
        assert_eq!(s.manifest.spritesheet_index_count, 6);
        // s2p extended from len=1 to len=6, byte at [5] = 1, gap bytes at [1..5] = 0.
        assert_eq!(s.manifest.sprite_to_palette, vec![0, 0, 0, 0, 0, 1]);
    }

    #[test]
    fn create_texture_revert_restores_state() {
        let mut e = fake_editor();
        e.loaded_sprite.as_mut().unwrap().manifest.sprite_to_palette = vec![0u8];
        let before = e.loaded_sprite.clone().unwrap();

        let mut h = History::default();
        h.execute(
            Box::new(CreateTexture::new(
                5, 8, 8, TextureFormat::Ci8, 1, vec![0u8; 64],
            )),
            &mut e,
        )
        .unwrap();
        h.undo(&mut e).unwrap();

        assert_eq!(e.loaded_sprite.as_ref().unwrap(), &before);
    }

    #[test]
    fn create_texture_rejects_existing_index() {
        let mut e = fake_editor();
        let mut cmd = CreateTexture::new(0, 1, 1, TextureFormat::Ci8, 0, vec![0]);
        let err = cmd.apply(&mut e).unwrap_err();
        assert!(matches!(err, CmdError::Invariant(_)), "got: {err:?}");
    }

    #[test]
    fn create_texture_leaves_empty_s2p_untouched() {
        let mut e = fake_editor();
        // fake_editor's manifest.sprite_to_palette is already empty by default.
        assert!(e.loaded_sprite.as_ref().unwrap().manifest.sprite_to_palette.is_empty());

        let mut h = History::default();
        h.execute(
            Box::new(CreateTexture::new(
                5, 8, 8, TextureFormat::Ci8, 1, vec![0u8; 64],
            )),
            &mut e,
        )
        .unwrap();

        assert!(e.loaded_sprite.as_ref().unwrap().manifest.sprite_to_palette.is_empty());
        h.undo(&mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().manifest.sprite_to_palette.is_empty());
    }

    #[test]
    fn delete_texture_data_removes_texture_and_meta_zeros_s2p() {
        let mut e = fake_editor();
        // Add a second texture and put s2p in non-empty state.
        {
            let s = e.loaded_sprite.as_mut().unwrap();
            s.textures.insert(
                7,
                Texture {
                    index: 7,
                    width: 4,
                    height: 4,
                    format: TextureFormat::Ci8,
                    palette_index: 1,
                    pixels: vec![0; 16],
                },
            );
            s.manifest.textures.push(TextureMeta {
                index: 7,
                width: 4,
                height: 4,
                format: TextureFormat::Ci8,
                palette_index: 1,
            });
            s.manifest.sprite_count = 8;
            s.manifest.spritesheet_index_count = 8;
            s.manifest.sprite_to_palette = vec![0, 0, 0, 0, 0, 0, 0, 1];
        }

        let mut h = History::default();
        h.execute(Box::new(DeleteTextureData::new(7)), &mut e).unwrap();

        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(!s.textures.contains_key(&7));
        assert!(!s.manifest.textures.iter().any(|m| m.index == 7));
        // sprite_count / spritesheet_index_count are unchanged (gap mode preserves slot).
        assert_eq!(s.manifest.sprite_count, 8);
        assert_eq!(s.manifest.spritesheet_index_count, 8);
        // s2p[7] zeroed.
        assert_eq!(s.manifest.sprite_to_palette[7], 0);
        assert_eq!(s.manifest.sprite_to_palette.len(), 8);
    }

    #[test]
    fn delete_texture_data_revert_restores_everything() {
        let mut e = fake_editor();
        {
            let s = e.loaded_sprite.as_mut().unwrap();
            s.textures.insert(
                7,
                Texture {
                    index: 7,
                    width: 4,
                    height: 4,
                    format: TextureFormat::Ci8,
                    palette_index: 1,
                    pixels: vec![42; 16],
                },
            );
            s.manifest.textures.push(TextureMeta {
                index: 7,
                width: 4,
                height: 4,
                format: TextureFormat::Ci8,
                palette_index: 1,
            });
            s.manifest.sprite_count = 8;
            s.manifest.spritesheet_index_count = 8;
            s.manifest.sprite_to_palette = vec![0, 0, 0, 0, 0, 0, 0, 1];
        }
        let before = e.loaded_sprite.clone().unwrap();

        let mut h = History::default();
        h.execute(Box::new(DeleteTextureData::new(7)), &mut e).unwrap();
        h.undo(&mut e).unwrap();

        assert_eq!(e.loaded_sprite.as_ref().unwrap(), &before);
    }

    #[test]
    fn delete_texture_data_rejects_missing_index() {
        let mut e = fake_editor();
        let mut cmd = DeleteTextureData::new(99);
        let err = cmd.apply(&mut e).unwrap_err();
        assert!(matches!(err, CmdError::InvalidTarget(_)), "got: {err:?}");
    }

    #[test]
    fn replace_texture_from_png_apply_revert_round_trip() {
        let mut e = fake_editor();
        // fake_editor has texture 0 with palette 0, 1x1, pixels=[0].
        let before = e.loaded_sprite.clone().unwrap();

        let mut h = History::default();
        let cmd = ReplaceTextureFromPng::new(0, vec![1, 2, 3, 4], (2, 2), 1, &e).unwrap();
        h.execute(Box::new(cmd), &mut e).unwrap();

        let s = e.loaded_sprite.as_ref().unwrap();
        assert_eq!(s.textures[&0].pixels, vec![1, 2, 3, 4]);
        assert_eq!(s.textures[&0].width, 2);
        assert_eq!(s.textures[&0].height, 2);
        assert_eq!(s.textures[&0].palette_index, 1);
        // Manifest entry kept in sync.
        assert_eq!(s.manifest.textures[0].width, 2);
        assert_eq!(s.manifest.textures[0].palette_index, 1);

        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap(), &before);
    }

    #[test]
    fn replace_texture_from_png_syncs_sprite_to_palette() {
        let mut e = fake_editor();
        e.loaded_sprite.as_mut().unwrap().manifest.sprite_to_palette = vec![0u8];

        let mut h = History::default();
        let cmd = ReplaceTextureFromPng::new(0, vec![1, 2, 3, 4], (2, 2), 1, &e).unwrap();
        h.execute(Box::new(cmd), &mut e).unwrap();

        let s = e.loaded_sprite.as_ref().unwrap();
        assert_eq!(s.manifest.sprite_to_palette[0], 1);

        h.undo(&mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert_eq!(s.manifest.sprite_to_palette[0], 0);
    }
}
