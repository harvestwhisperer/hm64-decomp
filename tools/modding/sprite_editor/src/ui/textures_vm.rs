//! Renders CI textures into `slint::Image` for display.

use slint::{Image, Model, ModelRc, Rgba8Pixel, SharedPixelBuffer, SharedString, VecModel};

use crate::model::{Palette, Rgba, SpriteAsset, Texture, TextureFormat};
use crate::ui::slint_generated::{AnimationUsageRow, ScriptEntryUsageRow, TextureThumb};

pub fn render_texture_with_palette(tex: &Texture, palette: &Palette) -> Image {
    render_impl(tex, palette, false)
}

pub fn render_texture_with_palette_h_flip(tex: &Texture, palette: &Palette) -> Image {
    render_impl(tex, palette, true)
}

pub fn render_texture_using_manifest_palette(asset: &SpriteAsset, tex: &Texture) -> Image {
    match asset.palettes.get(&tex.palette_index) {
        Some(p) => render_impl(tex, p, false),
        None => Image::default(),
    }
}

pub fn render_texture_using_manifest_palette_h_flip(asset: &SpriteAsset, tex: &Texture) -> Image {
    match asset.palettes.get(&tex.palette_index) {
        Some(p) => render_impl(tex, p, true),
        None => Image::default(),
    }
}

fn render_impl(tex: &Texture, palette: &Palette, h_flip: bool) -> Image {
    let w = tex.width as u32;
    let h = tex.height as u32;
    let mut buf = SharedPixelBuffer::<Rgba8Pixel>::new(w, h);
    let pixels: &mut [Rgba8Pixel] = buf.make_mut_slice();

    let get_color = |idx: u8| -> Rgba {
        palette.colors.get(idx as usize).copied().unwrap_or(Rgba::new(255, 0, 255, 255))
    };

    match tex.format {
        TextureFormat::Ci8 => {
            for (p, &idx) in pixels.iter_mut().zip(tex.pixels.iter()) {
                let c = get_color(idx);
                *p = Rgba8Pixel { r: c.r, g: c.g, b: c.b, a: c.a };
            }
        }
        TextureFormat::Ci4 => {
            for (i, p) in pixels.iter_mut().enumerate() {
                let byte = tex.pixels[i / 2];
                let nibble = if i % 2 == 0 { (byte >> 4) & 0x0F } else { byte & 0x0F };
                let c = get_color(nibble);
                *p = Rgba8Pixel { r: c.r, g: c.g, b: c.b, a: c.a };
            }
        }
    }

    if h_flip && w > 0 {
        for row in pixels.chunks_mut(w as usize) {
            row.reverse();
        }
    }

    Image::from_rgba8(buf)
}

// ---------------------------------------------------------------------------
// Texture usage index
// ---------------------------------------------------------------------------

use std::collections::BTreeMap;

use crate::model::{AnimationData, EntityScriptDoc};

#[derive(Debug, Clone)]
pub struct AnimationUsage {
    pub animation_index: u32,
    pub frame_count: u32,
}

#[derive(Debug, Clone)]
pub struct ScriptEntryUsage {
    pub script_name: String,
    pub entry_index: u32,
    pub label: String,
    pub animation_index: u32,
}

#[derive(Debug, Default, Clone)]
pub struct TextureUsage {
    pub animations: Vec<AnimationUsage>,
    pub script_entries: Vec<ScriptEntryUsage>,
}

/// Build `texture_index → TextureUsage` for every texture in the sprite.
/// Textures that aren't referenced anywhere appear with empty `Vec`s, so
/// callers can distinguish "loaded sprite, unused texture" from "no data".
pub fn build_texture_usage_index(
    sprite: &SpriteAsset,
    animation_data: Option<&AnimationData>,
) -> BTreeMap<u32, TextureUsage> {
    let mut out: BTreeMap<u32, TextureUsage> = sprite
        .textures
        .keys()
        .map(|&i| (i, TextureUsage::default()))
        .collect();

    // Spritesheet index → texture index. The manifest's textures already
    // expose `index` and the spritesheet mapping is 1:1 for type-1 sprites.
    // For type-2 sprites, multiple spritesheet slots may share one texture,
    // so we walk the manifest's spritesheet table instead of assuming identity.
    let s2t: BTreeMap<u32, u32> = build_spritesheet_to_texture(sprite);

    for (&anim_idx, anim) in &sprite.animations {
        let usage = AnimationUsage {
            animation_index: anim_idx,
            frame_count: anim.frames.len() as u32,
        };
        let mut touched: std::collections::BTreeSet<u32> = Default::default();
        for frame in &anim.frames {
            for sprite_ref in &frame.sprites {
                if let Some(&tex_idx) = s2t.get(&sprite_ref.spritesheet_index) {
                    touched.insert(tex_idx);
                }
            }
        }
        for tex_idx in touched {
            if let Some(u) = out.get_mut(&tex_idx) {
                u.animations.push(usage.clone());
            }
        }
    }

    if let Some(AnimationData::Entity { scripts }) = animation_data {
        for script in scripts {
            for (entry_index, entry) in script.animations.iter().enumerate() {
                // metadata_index is u16 in the model; cast for the u32-keyed map.
                let target_anim = entry.metadata_index as u32;
                let Some(anim) = sprite.animations.get(&target_anim) else { continue };
                let mut touched: std::collections::BTreeSet<u32> = Default::default();
                for frame in &anim.frames {
                    for sprite_ref in &frame.sprites {
                        if let Some(&tex_idx) = s2t.get(&sprite_ref.spritesheet_index) {
                            touched.insert(tex_idx);
                        }
                    }
                }
                for tex_idx in touched {
                    if let Some(u) = out.get_mut(&tex_idx) {
                        u.script_entries.push(ScriptEntryUsage {
                            script_name: script.name.clone(),
                            entry_index: entry_index as u32,
                            label: entry.label.clone(),
                            animation_index: target_anim,
                        });
                    }
                }
            }
        }
    }

    out
}

fn build_spritesheet_to_texture(sprite: &SpriteAsset) -> BTreeMap<u32, u32> {
    // The manifest's `textures` list already has per-texture `index`
    // (spritesheet-slot index for that texture). Walk it once. If your
    // model exposes a different field name, adjust here.
    let mut m = BTreeMap::new();
    for meta in &sprite.manifest.textures {
        m.insert(meta.index, meta.index); // Default identity for type-1.
    }
    // Type-2 sprites may have multiple spritesheet slots per texture; if
    // the manifest exposes that, extend `m` with the extra slot→texture
    // edges here. Today the editor's loader treats them 1:1; this comment
    // is a load-bearing reminder for the type-2 extractor task in Group F.
    m
}

// ---------------------------------------------------------------------------
// Slint property bridges — convert `TextureUsage` and `Texture` collections
// into the Slint-generated row/thumb structs the UI binds to.
// ---------------------------------------------------------------------------

pub fn animation_usage_rows(usage: &TextureUsage) -> ModelRc<AnimationUsageRow> {
    let rows: Vec<AnimationUsageRow> = usage
        .animations
        .iter()
        .map(|a| AnimationUsageRow {
            animation_index: a.animation_index as i32,
            frame_count: a.frame_count as i32,
            label: SharedString::from(format!(
                "Animation {} ({} frames)",
                a.animation_index, a.frame_count
            )),
        })
        .collect();
    ModelRc::new(VecModel::from(rows))
}

pub fn script_usage_rows(usage: &TextureUsage) -> ModelRc<ScriptEntryUsageRow> {
    let rows: Vec<ScriptEntryUsageRow> = usage
        .script_entries
        .iter()
        .map(|e| ScriptEntryUsageRow {
            script_name: SharedString::from(e.script_name.clone()),
            entry_index: e.entry_index as i32,
            animation_index: e.animation_index as i32,
            label: SharedString::from(e.label.clone()),
        })
        .collect();
    ModelRc::new(VecModel::from(rows))
}

/// Build a `TextureThumb` row for a single texture using the sprite's
/// manifest-default palette. Returns a row with an empty image when the
/// texture's referenced palette is missing.
pub fn texture_thumb(asset: &SpriteAsset, tex: &Texture) -> TextureThumb {
    TextureThumb {
        index: tex.index as i32,
        width: tex.width as i32,
        height: tex.height as i32,
        palette_index: tex.palette_index as i32,
        image: render_texture_using_manifest_palette(asset, tex),
    }
}

pub fn texture_thumbs(asset: &SpriteAsset) -> ModelRc<TextureThumb> {
    let thumbs: Vec<TextureThumb> = asset.textures.values().map(|t| texture_thumb(asset, t)).collect();
    ModelRc::new(VecModel::from(thumbs))
}

#[cfg(test)]
mod usage_tests {
    use super::*;
    use crate::model::{
        Animation, AnimationData, AnimationFrame, EntityAnimation, EntityScriptDoc, FrameSprite,
        JsonAnimType, Manifest, Palette, Rgba, SpriteAsset, SpriteKind, SpriteType, Texture,
        TextureFormat, TextureMeta,
    };
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn fixture(animations: Vec<(u32, Vec<u32>)>) -> SpriteAsset {
        // Build a sprite with N textures whose spritesheet_index == texture_index.
        let n_tex: u32 = 4;
        let mut textures = BTreeMap::new();
        let mut tex_metas = Vec::new();
        for i in 0..n_tex {
            tex_metas.push(TextureMeta {
                index: i,
                palette_index: 0,
                width: 8,
                height: 8,
                format: TextureFormat::Ci4,
            });
            textures.insert(
                i,
                Texture {
                    index: i,
                    palette_index: 0,
                    width: 8,
                    height: 8,
                    format: TextureFormat::Ci4,
                    pixels: vec![0; 32],
                },
            );
        }

        let mut anims = BTreeMap::new();
        for (idx, sprites) in animations {
            let frames: Vec<AnimationFrame> = sprites
                .into_iter()
                .map(|sheet_idx| AnimationFrame {
                    duration: 10,
                    has_audio: 0,
                    sprites: vec![FrameSprite {
                        spritesheet_index: sheet_idx,
                        anchor_x: 0,
                        anchor_y: 0,
                    }],
                })
                .collect();
            anims.insert(
                idx,
                Animation {
                    frame_count: frames.len() as u32,
                    unused: 0,
                    frames,
                    index: idx,
                },
            );
        }

        SpriteAsset {
            dir: PathBuf::new(),
            manifest: Manifest {
                label: "test".into(),
                subdir: "test".into(),
                kind: SpriteKind::Entity,
                sprite_type: SpriteType::Type1,
                sprite_count: n_tex,
                spritesheet_index_count: n_tex,
                palette_count: 1,
                palette_index_count: 1,
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
                textures: tex_metas,
            },
            textures,
            palettes: BTreeMap::from([(
                0,
                Palette {
                    index: 0,
                    format: TextureFormat::Ci4,
                    colors: vec![Rgba::default(); 16],
                },
            )]),
            animations: anims,
        }
    }

    #[test]
    fn empty_sprite_yields_empty_per_texture_usage() {
        let s = fixture(vec![]);
        let idx = build_texture_usage_index(&s, None);
        assert_eq!(idx.len(), 4);
        for (_, u) in &idx {
            assert!(u.animations.is_empty());
            assert!(u.script_entries.is_empty());
        }
    }

    #[test]
    fn single_animation_referencing_two_textures() {
        let s = fixture(vec![(0, vec![1, 2])]);
        let idx = build_texture_usage_index(&s, None);
        assert_eq!(idx[&1].animations.len(), 1);
        assert_eq!(idx[&2].animations.len(), 1);
        assert!(idx[&0].animations.is_empty());
        assert!(idx[&3].animations.is_empty());
    }

    #[test]
    fn animation_dedupes_repeated_texture_per_frame() {
        // Animation references texture 1 in two frames — should appear once.
        let s = fixture(vec![(0, vec![1, 1])]);
        let idx = build_texture_usage_index(&s, None);
        assert_eq!(idx[&1].animations.len(), 1);
    }

    #[test]
    fn entity_script_attaches_to_textures_via_metadata_index() {
        let s = fixture(vec![(5, vec![2])]);
        let data = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "test".into(),
                name: "test".into(),
                trailing_zeros: 0,
                animations: vec![EntityAnimation {
                    label: "TEST_ANIM_LABEL".into(),
                    metadata_index: 5,
                    anim_type: JsonAnimType::Loop,
                    flip: false,
                    directional: false,
                }],
            }],
        };
        let idx = build_texture_usage_index(&s, Some(&data));
        assert_eq!(idx[&2].script_entries.len(), 1);
        assert_eq!(idx[&2].script_entries[0].label, "TEST_ANIM_LABEL");
    }
}
