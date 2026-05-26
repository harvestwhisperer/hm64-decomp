//! Animation panel view-model: owns per-animation playback state and
//! rebuilds Slint thumbnail + detail images every tick.

use std::collections::HashMap;
use std::rc::Rc;

use slint::{Image, ModelRc, SharedString, VecModel};

use crate::model::{Animation, SpriteAsset};
use crate::state::playback::{
    compose_frame_h_flipped, compute_animation_bounds, tick, FrameCache, PlayingAnim,
};
use crate::ui::slint_generated::{FrameRow, LayerRow, PickerCell, PickerRow};

pub struct AnimationsVm {
    pub anims: HashMap<u32, PlayingAnim>,
    pub cache: FrameCache,
    /// Pivot-space bounding box per animation, computed lazily and cleared on
    /// any palette/pixel/animation edit.
    pub bounds: HashMap<u32, (i32, i32, i32, i32)>,
    /// Per `(anim_idx, frame_idx)` editor expansion state. Cleared on
    /// `reset_for_sprite`. Frames default to collapsed; entries here
    /// override per-frame.
    pub frame_expanded: HashMap<(u32, usize), bool>,
}

impl Default for AnimationsVm {
    fn default() -> Self {
        Self::new()
    }
}

impl AnimationsVm {
    pub fn new() -> Self {
        Self {
            anims: HashMap::new(),
            cache: FrameCache::default(),
            bounds: HashMap::new(),
            frame_expanded: HashMap::new(),
        }
    }

    pub fn reset_for_sprite(&mut self, asset: &SpriteAsset) {
        self.anims.clear();
        self.cache.clear();
        self.bounds.clear();
        self.frame_expanded.clear();
        for &idx in asset.animations.keys() {
            self.anims.insert(idx, PlayingAnim::new(idx));
        }
    }

    pub fn reset_empty(&mut self) {
        self.anims.clear();
        self.cache.clear();
        self.bounds.clear();
        self.frame_expanded.clear();
    }

    /// Clear the frame cache and bounds, and force every animation to
    /// re-emit its image on the next tick. Call this whenever palette,
    /// pixel data, or animation metadata changes.
    pub fn invalidate_cache(&mut self) {
        self.cache.clear();
        self.bounds.clear();
        for a in self.anims.values_mut() {
            a.last_emitted_frame = None;
        }
    }

    /// Resolve the cached pivot-space bounds for an animation, computing
    /// them on first access. Returns `None` if no layer references a known
    /// texture.
    pub fn ensure_bounds(
        &mut self,
        asset: &SpriteAsset,
        animation_index: u32,
    ) -> Option<(i32, i32, i32, i32)> {
        if let Some(b) = self.bounds.get(&animation_index) {
            return Some(*b);
        }
        let anim = asset.animations.get(&animation_index)?;
        let b = compute_animation_bounds(asset, anim)?;
        self.bounds.insert(animation_index, b);
        Some(b)
    }

    /// Render the current frame of a single animation as a composed image.
    /// Used for immediate UI updates (e.g. on click) without waiting for the
    /// next 60 Hz advance tick.
    pub fn current_frame_image(
        &mut self,
        asset: &SpriteAsset,
        animation_index: u32,
    ) -> Option<Image> {
        let frame_idx = self.anims.get(&animation_index)?.frame_idx;
        let anim = asset.animations.get(&animation_index)?;
        let frame = anim.frames.get(frame_idx)?;
        let bounds = self.ensure_bounds(asset, animation_index)?;
        Some(self.cache.get_or_render(asset, animation_index, frame_idx, frame, bounds))
    }

    /// Same as [`current_frame_image`] but mirrors the result horizontally.
    /// Used by the Script-tab preview when the entry's flip flag is set, so
    /// the preview composes all layers (matching the Animations gallery)
    /// rather than rendering only the first sprite layer.
    pub fn current_frame_image_h_flipped(
        &mut self,
        asset: &SpriteAsset,
        animation_index: u32,
    ) -> Option<Image> {
        let frame_idx = self.anims.get(&animation_index)?.frame_idx;
        let anim = asset.animations.get(&animation_index)?;
        let frame = anim.frames.get(frame_idx)?;
        let bounds = self.ensure_bounds(asset, animation_index)?;
        Some(compose_frame_h_flipped(&mut self.cache, asset, frame, bounds))
    }

    pub fn advance(&mut self, asset: &SpriteAsset, delta_ms: u32) -> Vec<(u32, Image)> {
        let mut out = Vec::with_capacity(self.anims.len());
        let indices: Vec<u32> = self.anims.keys().copied().collect();
        for idx in indices {
            let Some(data) = asset.animations.get(&idx) else { continue };
            let Some(a) = self.anims.get_mut(&idx) else { continue };
            let Some(frame_idx) = tick(a, data, delta_ms) else { continue };
            // Only push an update when the displayed frame actually changed.
            // This prevents spamming set_row_data every 16 ms for all
            // animations (especially costly for large sprites like cow with
            // 166 simultaneous animations).
            if a.last_emitted_frame == Some(frame_idx) {
                continue;
            }
            a.last_emitted_frame = Some(frame_idx);
            let Some(frame) = data.frames.get(frame_idx) else { continue };
            let Some(bounds) = self.ensure_bounds(asset, idx) else { continue };
            let img = self.cache.get_or_render(asset, idx, frame_idx, frame, bounds);
            out.push((idx, img));
        }
        out
    }

    pub fn set_speed(&mut self, idx: u32, speed: f32) {
        if let Some(a) = self.anims.get_mut(&idx) {
            a.speed = speed;
        }
    }

    pub fn set_playing(&mut self, idx: u32, playing: bool) {
        if let Some(a) = self.anims.get_mut(&idx) {
            a.playing = playing;
        }
    }

    pub fn seek(&mut self, idx: u32, frame: usize) {
        if let Some(a) = self.anims.get_mut(&idx) {
            a.frame_idx = frame;
            a.frame_elapsed_ms = 0;
        }
    }

    /// Pause `animation_index` and step its frame cursor by `delta` (wrapping
    /// around the frame count). Returns the composed image of the new frame
    /// for the caller to push to the UI immediately.
    pub fn step_frame(
        &mut self,
        asset: &SpriteAsset,
        animation_index: u32,
        delta: i32,
    ) -> Option<Image> {
        let frame_count = asset.animations.get(&animation_index)?.frames.len();
        if frame_count == 0 {
            return None;
        }
        let a = self.anims.get_mut(&animation_index)?;
        a.playing = false;
        let cur = a.frame_idx as i32;
        let next = (cur + delta).rem_euclid(frame_count as i32) as usize;
        a.frame_idx = next;
        a.frame_elapsed_ms = 0;
        self.current_frame_image(asset, animation_index)
    }
}

/// Summary info for the animation gallery thumbnails.
pub fn describe_animation(anim: &Animation) -> String {
    format!(
        "#{} · {} frame{}",
        anim.index,
        anim.frame_count,
        if anim.frame_count == 1 { "" } else { "s" }
    )
}

/// Describe the current sprite's textures as a parallel `(labels, keys)`
/// pair for the layer picker. Iteration order is `BTreeMap` key order —
/// sparse texture maps preserve their actual indices in the keys vec.
/// `combo_index` returned by [`build_editor_frames`] indexes into both.
pub fn build_texture_options(asset: &SpriteAsset) -> (Vec<String>, Vec<u32>) {
    let mut labels = Vec::with_capacity(asset.textures.len());
    let mut keys = Vec::with_capacity(asset.textures.len());
    for (&idx, t) in &asset.textures {
        labels.push(format!("#{} ({}×{})", idx, t.width, t.height));
        keys.push(idx);
    }
    (labels, keys)
}

/// Plain-Rust cell type for the texture picker popup grid. `combo_index`
/// matches the position used by [`build_editor_frames`] / `texture_keys`.
#[derive(Debug, Clone)]
pub struct PickerCellData {
    pub combo_index: i32,
    pub label: String,
    pub image: Image,
}

/// Build picker rows for the popup, chunked into rows of `columns`. Image
/// rendering uses the texture's manifest-assigned palette (same as the
/// Textures tab thumbnails).
pub fn build_texture_picker_rows(
    asset: &SpriteAsset,
    columns: usize,
) -> Vec<Vec<PickerCellData>> {
    use crate::ui::textures_vm::render_texture_using_manifest_palette;
    assert!(columns > 0, "columns must be > 0");
    let mut cells: Vec<PickerCellData> = Vec::with_capacity(asset.textures.len());
    for (i, (_idx, t)) in asset.textures.iter().enumerate() {
        cells.push(PickerCellData {
            combo_index: i as i32,
            label: format!("#{} ({}×{})", t.index, t.width, t.height),
            image: render_texture_using_manifest_palette(asset, t),
        });
    }
    cells.chunks(columns).map(|c| c.to_vec()).collect()
}

/// Plain-Rust row types for the editor section. The `editor_frame_rows`
/// bridge below converts these into the Slint-generated `LayerRow` /
/// `FrameRow` structs the UI binds to.
#[derive(Debug, Clone, PartialEq)]
pub struct LayerRowData {
    pub layer_index: i32,
    /// Combo position of this layer's texture in the texture-keys / picker
    /// row order (BTreeMap iteration). -1 if the texture no longer exists.
    pub texture_combo_index: i32,
    /// Display label for the trigger button, e.g. "#0 (32×16)".
    /// Empty string when the layer's texture no longer exists.
    pub texture_label: String,
    pub anchor_x: i32,
    pub anchor_y: i32,
}

#[derive(Debug, Clone, PartialEq)]
pub struct FrameRowData {
    pub frame_index: i32,
    pub duration: i32,
    pub has_audio: bool,
    pub layers: Vec<LayerRowData>,
    pub expanded: bool,
}

/// Build the editor row data for `animation_index`. Frames default to
/// collapsed; entries in `expanded` override per-frame.
pub fn build_editor_frames(
    asset: &SpriteAsset,
    animation_index: u32,
    expanded: &HashMap<(u32, usize), bool>,
    texture_keys: &[u32],
) -> Vec<FrameRowData> {
    let Some(anim) = asset.animations.get(&animation_index) else {
        return Vec::new();
    };
    anim.frames
        .iter()
        .enumerate()
        .map(|(fi, f)| {
            let layers = f
                .sprites
                .iter()
                .enumerate()
                .map(|(li, s)| {
                    let combo = texture_keys
                        .iter()
                        .position(|&k| k == s.spritesheet_index)
                        .map(|p| p as i32)
                        .unwrap_or(-1);
                    let texture_label = match asset.textures.get(&s.spritesheet_index) {
                        Some(t) => format!("#{} ({}×{})", t.index, t.width, t.height),
                        None => String::new(),
                    };
                    LayerRowData {
                        layer_index: li as i32,
                        texture_combo_index: combo,
                        texture_label,
                        anchor_x: s.anchor_x as i32,
                        anchor_y: s.anchor_y as i32,
                    }
                })
                .collect();
            let expanded = expanded.get(&(animation_index, fi)).copied().unwrap_or(false);
            FrameRowData {
                frame_index: fi as i32,
                duration: f.duration as i32,
                has_audio: f.has_audio != 0,
                layers,
                expanded,
            }
        })
        .collect()
}

// ---------------------------------------------------------------------------
// Slint property bridges — convert the plain-Rust row data above into the
// Slint-generated structs the editor's `anim_editor_frames` / picker bind to.
// ---------------------------------------------------------------------------

fn picker_cell(c: PickerCellData) -> PickerCell {
    PickerCell { combo_index: c.combo_index, label: SharedString::from(c.label), image: c.image }
}

fn picker_row(cells: Vec<PickerCellData>) -> PickerRow {
    let cells: Vec<PickerCell> = cells.into_iter().map(picker_cell).collect();
    PickerRow { cells: Rc::new(VecModel::from(cells)).into() }
}

pub fn texture_picker_rows(asset: &SpriteAsset, columns: usize) -> ModelRc<PickerRow> {
    let rows: Vec<PickerRow> = build_texture_picker_rows(asset, columns)
        .into_iter()
        .map(picker_row)
        .collect();
    ModelRc::new(VecModel::from(rows))
}

fn layer_row(ld: LayerRowData) -> LayerRow {
    LayerRow {
        layer_index: ld.layer_index,
        texture_combo_index: ld.texture_combo_index,
        texture_label: SharedString::from(ld.texture_label),
        anchor_x: ld.anchor_x,
        anchor_y: ld.anchor_y,
    }
}

fn frame_row(fd: FrameRowData) -> FrameRow {
    let layers: Vec<LayerRow> = fd.layers.into_iter().map(layer_row).collect();
    FrameRow {
        frame_index: fd.frame_index,
        duration: fd.duration,
        has_audio: fd.has_audio,
        expanded: fd.expanded,
        layers: Rc::new(VecModel::from(layers)).into(),
    }
}

pub fn editor_frame_rows(
    asset: &SpriteAsset,
    animation_index: u32,
    expanded: &HashMap<(u32, usize), bool>,
    texture_keys: &[u32],
) -> ModelRc<FrameRow> {
    let rows: Vec<FrameRow> = build_editor_frames(asset, animation_index, expanded, texture_keys)
        .into_iter()
        .map(frame_row)
        .collect();
    ModelRc::new(VecModel::from(rows))
}

#[cfg(test)]
mod editor_tests {
    use super::*;
    use crate::model::{
        Animation, AnimationFrame, FrameSprite, Manifest, SpriteAsset, SpriteType, Texture,
        TextureFormat,
    };
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn fake_sprite() -> SpriteAsset {
        let manifest = Manifest {
            label: "x".into(),
            subdir: "entitySprites".into(),
            kind: crate::model::SpriteKind::Unknown,
            sprite_type: SpriteType::Type1,
            sprite_count: 0,
            spritesheet_index_count: 0,
            palette_count: 0,
            palette_index_count: 0,
            animation_count: 1,
            animation_index: vec![0],
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
        let mut textures = BTreeMap::new();
        textures.insert(0, Texture {
            index: 0, width: 32, height: 16, format: TextureFormat::Ci4,
            palette_index: 0, pixels: vec![],
        });
        // Sparse: skip 1.
        textures.insert(2, Texture {
            index: 2, width: 8, height: 8, format: TextureFormat::Ci4,
            palette_index: 0, pixels: vec![],
        });
        let mut animations = BTreeMap::new();
        animations.insert(0, Animation {
            frame_count: 2,
            unused: 0,
            index: 0,
            frames: vec![
                AnimationFrame {
                    duration: 4, has_audio: 1,
                    sprites: vec![
                        FrameSprite { spritesheet_index: 2, anchor_x: -3, anchor_y: 5 },
                    ],
                },
                AnimationFrame {
                    duration: 7, has_audio: 0,
                    sprites: vec![
                        FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 },
                        FrameSprite { spritesheet_index: 99, anchor_x: 1, anchor_y: 2 },
                    ],
                },
            ],
        });
        SpriteAsset {
            dir: PathBuf::new(),
            manifest,
            textures,
            palettes: BTreeMap::new(),
            animations,
        }
    }

    #[test]
    fn texture_options_preserve_sparse_keys() {
        let s = fake_sprite();
        let (labels, keys) = build_texture_options(&s);
        assert_eq!(labels, vec!["#0 (32×16)".to_string(), "#2 (8×8)".to_string()]);
        assert_eq!(keys, vec![0u32, 2]);
    }

    #[test]
    fn editor_frames_map_textures_to_combo_positions() {
        let s = fake_sprite();
        let (_labels, keys) = build_texture_options(&s);
        let expanded = HashMap::new();
        let rows = build_editor_frames(&s, 0, &expanded, &keys);
        assert_eq!(rows.len(), 2);
        // Frame 0 references texture 2 -> combo position 1.
        assert_eq!(rows[0].layers[0].texture_combo_index, 1);
        assert_eq!(rows[0].layers[0].anchor_x, -3);
        assert_eq!(rows[0].layers[0].anchor_y, 5);
        assert!(rows[0].has_audio);
        // Frame 1 layer 0 references texture 0 -> combo position 0.
        assert_eq!(rows[1].layers[0].texture_combo_index, 0);
        // Frame 1 layer 1 references missing texture 99 -> -1.
        assert_eq!(rows[1].layers[1].texture_combo_index, -1);
    }

    #[test]
    fn editor_frames_populate_texture_label() {
        let s = fake_sprite();
        let (_labels, keys) = build_texture_options(&s);
        let rows = build_editor_frames(&s, 0, &HashMap::new(), &keys);
        // Frame 0 layer 0 -> texture #2 (8×8).
        assert_eq!(rows[0].layers[0].texture_label, "#2 (8×8)");
        // Frame 1 layer 0 -> texture #0 (32×16).
        assert_eq!(rows[1].layers[0].texture_label, "#0 (32×16)");
        // Frame 1 layer 1 references missing texture 99 -> empty label.
        assert_eq!(rows[1].layers[1].texture_label, "");
    }

    #[test]
    fn picker_rows_chunk_by_columns_and_carry_combo_index() {
        let s = fake_sprite();
        let rows = build_texture_picker_rows(&s, 4);
        // 2 textures, 4 cols -> 1 row of 2 cells.
        assert_eq!(rows.len(), 1);
        assert_eq!(rows[0].len(), 2);
        assert_eq!(rows[0][0].combo_index, 0);
        assert_eq!(rows[0][0].label, "#0 (32×16)");
        assert_eq!(rows[0][1].combo_index, 1);
        assert_eq!(rows[0][1].label, "#2 (8×8)");
    }

    #[test]
    fn picker_rows_chunk_with_partial_last_row() {
        let s = fake_sprite();
        let rows = build_texture_picker_rows(&s, 1);
        // 2 textures, 1 col -> 2 rows of 1 cell each.
        assert_eq!(rows.len(), 2);
        assert_eq!(rows[0].len(), 1);
        assert_eq!(rows[1].len(), 1);
        assert_eq!(rows[0][0].combo_index, 0);
        assert_eq!(rows[1][0].combo_index, 1);
    }

    #[test]
    fn editor_frames_default_to_collapsed() {
        let s = fake_sprite();
        let (_labels, keys) = build_texture_options(&s);
        let expanded = HashMap::new();
        let rows = build_editor_frames(&s, 0, &expanded, &keys);
        assert!(!rows[0].expanded);
        assert!(!rows[1].expanded);
    }

    #[test]
    fn editor_frames_explicit_expansion_overrides_default() {
        let s = fake_sprite();
        let (_labels, keys) = build_texture_options(&s);
        let mut expanded = HashMap::new();
        expanded.insert((0, 0), true);
        expanded.insert((0, 1), false);
        let rows = build_editor_frames(&s, 0, &expanded, &keys);
        assert!(rows[0].expanded);
        assert!(!rows[1].expanded);
    }

    #[test]
    fn editor_frames_returns_empty_for_unknown_anim() {
        let s = fake_sprite();
        let (_labels, keys) = build_texture_options(&s);
        let rows = build_editor_frames(&s, 99, &HashMap::new(), &keys);
        assert!(rows.is_empty());
    }
}
