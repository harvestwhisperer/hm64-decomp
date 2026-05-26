//! Animation playback engine. One shared frame cache + one shared timer
//! drive every visible animation. Each registered animation advances its
//! own frame cursor based on per-frame `duration` values, and composes all
//! sprite layers within a frame onto a stable canvas using anchor offsets.
//!
//! Composition follows `tools/libhm64/animations/gif_builder.py` for anchor
//! placement (each layer's anchor maps to (0, 0) in pivot space, and the
//! canvas spans the global bounding box across every layer of every frame in
//! the animation). It diverges from `gif_builder.py`'s implementation on
//! layer order: this module paints `sprites[0]` LAST (on top), matching the
//! comment "bitmap[0] ends up on top" in that file. The Python loop
//! contradicts its own comment and paints `sprites[N-1]` on top -- a known
//! issue not yet fixed there.

use std::collections::HashMap;
use std::time::{Duration, Instant};

use slint::{Image, Rgba8Pixel, SharedPixelBuffer};

use crate::model::{Animation, AnimationFrame, Palette, Rgba, SpriteAsset, Texture, TextureFormat};

/// Milliseconds per animation tick. Calibrated during manual playback review
/// against in-game footage. Initial estimate: ~33 ms = 30 fps.
pub const TICK_MS: u32 = 33;

/// Runtime state for one animation registered for playback.
#[derive(Debug)]
pub struct PlayingAnim {
    pub animation_index: u32,
    pub frame_idx: usize,
    pub frame_elapsed_ms: u32,
    pub playing: bool,
    pub h_flip: bool,
    pub speed: f32,
    /// Last frame_idx pushed to the UI. `None` = not yet emitted.
    /// Only emit a new image when this changes so we don't re-set_row_data
    /// every tick for animations whose current frame hasn't changed.
    pub last_emitted_frame: Option<usize>,
}

impl PlayingAnim {
    pub fn new(animation_index: u32) -> Self {
        Self {
            animation_index,
            frame_idx: 0,
            frame_elapsed_ms: 0,
            playing: true,
            h_flip: false,
            speed: 1.0,
            last_emitted_frame: None,
        }
    }
}

/// One source texture decoded into native-size RGBA pixels.
struct LayerPixels {
    width: u32,
    height: u32,
    pixels: Vec<Rgba8Pixel>,
}

/// Cache of decoded layers + composed frame images. Stays valid as long as
/// the palette, pixel data, and animation metadata are unchanged; commands
/// that touch any of these must clear the cache.
///
/// The `layers` map decodes each source texture exactly once -- shared across
/// every frame and every animation that references that spritesheet_index.
/// `composed` then caches per-frame composites; first-paint of a frame only
/// pays for blits, not palette decoding.
#[derive(Default)]
pub struct FrameCache {
    composed: HashMap<(u32, usize), Image>,
    layers: HashMap<u32, LayerPixels>,
}

impl FrameCache {
    pub fn get_or_render(
        &mut self,
        asset: &SpriteAsset,
        animation_index: u32,
        frame_idx: usize,
        frame: &AnimationFrame,
        bounds: (i32, i32, i32, i32),
    ) -> Image {
        if let Some(img) = self.composed.get(&(animation_index, frame_idx)) {
            return img.clone();
        }
        let img = compose_frame(self, asset, frame, bounds);
        self.composed.insert((animation_index, frame_idx), img.clone());
        img
    }

    pub fn clear(&mut self) {
        self.composed.clear();
        self.layers.clear();
    }

    fn ensure_layer(&mut self, asset: &SpriteAsset, spritesheet_index: u32) -> Option<&LayerPixels> {
        if !self.layers.contains_key(&spritesheet_index) {
            let tex = asset.textures.get(&spritesheet_index)?;
            let palette = asset.palettes.get(&tex.palette_index)?;
            self.layers.insert(spritesheet_index, decode_layer(tex, palette));
        }
        self.layers.get(&spritesheet_index)
    }
}

/// Pivot-space rectangle for one sprite layer, treating the anchor as the
/// origin (0, 0). Mirrors `compute_sprite_rect` in `gif_builder.py`.
pub fn compute_sprite_rect(
    width: u16,
    height: u16,
    anchor_x: i16,
    anchor_y: i16,
) -> (i32, i32, i32, i32) {
    let w = width as i32;
    let h = height as i32;
    let left = anchor_x as i32 - w / 2;
    let top = anchor_y as i32 - h / 2;
    (left, top, left + w, top + h)
}

/// Global pivot-space bounding box across every layer of every frame in an
/// animation. Returns `None` if no layer references a known texture.
pub fn compute_animation_bounds(
    asset: &SpriteAsset,
    animation: &Animation,
) -> Option<(i32, i32, i32, i32)> {
    let mut min_left = i32::MAX;
    let mut min_top = i32::MAX;
    let mut max_right = i32::MIN;
    let mut max_bot = i32::MIN;
    let mut found = false;
    for frame in &animation.frames {
        for sprite in &frame.sprites {
            let Some(tex) = asset.textures.get(&sprite.spritesheet_index) else { continue };
            let (l, t, r, b) =
                compute_sprite_rect(tex.width, tex.height, sprite.anchor_x, sprite.anchor_y);
            min_left = min_left.min(l);
            min_top = min_top.min(t);
            max_right = max_right.max(r);
            max_bot = max_bot.max(b);
            found = true;
        }
    }
    if found {
        Some((min_left, min_top, max_right, max_bot))
    } else {
        None
    }
}

/// Compose all sprite layers of `frame` onto a canvas sized to `bounds`,
/// each layer placed by its anchor. The render order is REVERSED relative to
/// the JSON ordering: `sprites[N-1]` paints first (background) and `sprites[0]`
/// paints last (on top), matching the comment in `gif_builder.py`
/// ("bitmap[0] ends up on top"). Source textures are decoded through the
/// cache so repeated composes amortize palette decoding.
pub fn compose_frame(
    cache: &mut FrameCache,
    asset: &SpriteAsset,
    frame: &AnimationFrame,
    bounds: (i32, i32, i32, i32),
) -> Image {
    match compose_frame_buffer(cache, asset, frame, bounds) {
        Some(buf) => Image::from_rgba8(buf),
        None => Image::default(),
    }
}

/// Same as [`compose_frame`] but returns the composed canvas mirrored
/// horizontally. Used by the Script-tab preview when the entry's `flip` flag
/// is set, so the displayed image matches what the game would draw and stays
/// consistent with the gallery (which composes all layers).
///
/// This is intentionally uncached — only one preview is on screen at a time,
/// and the unflipped composite already lives in [`FrameCache`].
pub fn compose_frame_h_flipped(
    cache: &mut FrameCache,
    asset: &SpriteAsset,
    frame: &AnimationFrame,
    bounds: (i32, i32, i32, i32),
) -> Image {
    match compose_frame_buffer(cache, asset, frame, bounds) {
        Some(buf) => Image::from_rgba8(flip_buffer_horizontal(buf)),
        None => Image::default(),
    }
}

fn flip_buffer_horizontal(
    mut buf: SharedPixelBuffer<Rgba8Pixel>,
) -> SharedPixelBuffer<Rgba8Pixel> {
    let w = buf.width() as usize;
    let h = buf.height() as usize;
    let pixels = buf.make_mut_slice();
    for y in 0..h {
        let row = &mut pixels[y * w..(y + 1) * w];
        row.reverse();
    }
    buf
}

fn compose_frame_buffer(
    cache: &mut FrameCache,
    asset: &SpriteAsset,
    frame: &AnimationFrame,
    bounds: (i32, i32, i32, i32),
) -> Option<SharedPixelBuffer<Rgba8Pixel>> {
    let (min_left, min_top, max_right, max_bot) = bounds;
    let canvas_w = (max_right - min_left).max(0) as u32;
    let canvas_h = (max_bot - min_top).max(0) as u32;
    if canvas_w == 0 || canvas_h == 0 {
        return None;
    }
    let mut buf = SharedPixelBuffer::<Rgba8Pixel>::new(canvas_w, canvas_h);
    let pixels = buf.make_mut_slice();

    for sprite in frame.sprites.iter().rev() {
        let anchor_x = sprite.anchor_x;
        let anchor_y = sprite.anchor_y;
        let Some(layer) = cache.ensure_layer(asset, sprite.spritesheet_index) else { continue };
        let (left, top, _, _) =
            compute_sprite_rect(layer.width as u16, layer.height as u16, anchor_x, anchor_y);
        let dx = left - min_left;
        let dy = top - min_top;
        let tw = layer.width;
        let th = layer.height;
        let canvas_w_i = canvas_w as i32;
        let canvas_h_i = canvas_h as i32;
        for ty in 0..th {
            let cy = dy + ty as i32;
            if cy < 0 || cy >= canvas_h_i {
                continue;
            }
            let row_base = (ty * tw) as usize;
            let canvas_row_base = (cy as u32 * canvas_w) as usize;
            for tx in 0..tw {
                let cx = dx + tx as i32;
                if cx < 0 || cx >= canvas_w_i {
                    continue;
                }
                let src = layer.pixels[row_base + tx as usize];
                if src.a == 0 {
                    continue;
                }
                pixels[canvas_row_base + cx as usize] = src;
            }
        }
    }

    Some(buf)
}

fn decode_layer(tex: &Texture, palette: &Palette) -> LayerPixels {
    let w = tex.width as u32;
    let h = tex.height as u32;
    let mut pixels = Vec::with_capacity((w * h) as usize);
    let lookup = |i: u8| -> Rgba {
        palette.colors.get(i as usize).copied().unwrap_or(Rgba::new(255, 0, 255, 255))
    };
    match tex.format {
        TextureFormat::Ci8 => {
            for &idx in tex.pixels.iter().take((w * h) as usize) {
                let c = lookup(idx);
                pixels.push(Rgba8Pixel { r: c.r, g: c.g, b: c.b, a: c.a });
            }
        }
        TextureFormat::Ci4 => {
            for i in 0..(w * h) as usize {
                let byte = tex.pixels.get(i / 2).copied().unwrap_or(0);
                let nibble = if i % 2 == 0 { (byte >> 4) & 0x0F } else { byte & 0x0F };
                let c = lookup(nibble);
                pixels.push(Rgba8Pixel { r: c.r, g: c.g, b: c.b, a: c.a });
            }
        }
    }
    LayerPixels { width: w, height: h, pixels }
}

/// Advances `anim` by `delta_ms`, returning the current `frame_idx`.
pub fn tick(anim: &mut PlayingAnim, data: &Animation, delta_ms: u32) -> Option<usize> {
    if data.frames.is_empty() {
        return None;
    }
    if !anim.playing {
        return Some(anim.frame_idx);
    }
    let scaled = (delta_ms as f32 * anim.speed).round() as u32;
    anim.frame_elapsed_ms += scaled;
    // Cap iterations at frames.len(): a duration-0 frame keeps frame_ms at 0,
    // and the inequality guard never breaks while elapsed > 0 — that hangs the
    // UI thread. Cycling once through every frame in a single tick is already
    // pathological, so bail and reset elapsed to keep playback moving.
    let max_iterations = data.frames.len();
    for _ in 0..max_iterations {
        let Some(frame) = data.frames.get(anim.frame_idx) else { break };
        let frame_ms = frame.duration as u32 * TICK_MS;
        if anim.frame_elapsed_ms < frame_ms {
            return Some(anim.frame_idx);
        }
        anim.frame_elapsed_ms = anim.frame_elapsed_ms.saturating_sub(frame_ms);
        anim.frame_idx += 1;
        if anim.frame_idx >= data.frames.len() {
            anim.frame_idx = 0;
        }
    }
    // Cap reached → discard residual elapsed so we stay in sync next tick.
    anim.frame_elapsed_ms = 0;
    Some(anim.frame_idx)
}

/// Helper for the Slint 60 Hz timer: returns a `Duration` since `last`.
pub fn elapsed_ms(last: Instant) -> (Instant, u32) {
    let now = Instant::now();
    let ms = (now - last).as_millis().min(1000) as u32;
    (now, ms)
}

/// Convenience: how long to sleep between ticks.
pub const FRAME_INTERVAL: Duration = Duration::from_millis(16);

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{
        AnimationFrame, FrameSprite, Manifest, Palette, SpriteAsset, SpriteKind, SpriteType,
        Texture, TextureFormat,
    };
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn frame(dur: u16, sprite: u32) -> AnimationFrame {
        AnimationFrame {
            duration: dur,
            has_audio: 0,
            sprites: vec![FrameSprite { spritesheet_index: sprite, anchor_x: 0, anchor_y: 0 }],
        }
    }

    #[test]
    fn tick_advances_after_frame_duration_elapses() {
        let data = Animation {
            frame_count: 2,
            unused: 0,
            index: 0,
            frames: vec![frame(3, 10), frame(3, 11)], // each 3 ticks ≈ 99ms
        };
        let mut a = PlayingAnim::new(0);

        assert_eq!(tick(&mut a, &data, 50), Some(0));
        assert_eq!(a.frame_idx, 0);

        assert_eq!(tick(&mut a, &data, 50), Some(1));
        assert_eq!(a.frame_idx, 1);
    }

    #[test]
    fn tick_loops_back_to_start() {
        let data = Animation {
            frame_count: 2,
            unused: 0,
            index: 0,
            frames: vec![frame(1, 10), frame(1, 11)],
        };
        let mut a = PlayingAnim::new(0);
        tick(&mut a, &data, 200); // way past both frames
        assert!(a.frame_idx < 2);
    }

    #[test]
    fn tick_when_paused_returns_current_frame() {
        let data = Animation { frame_count: 1, unused: 0, index: 0, frames: vec![frame(1, 42)] };
        let mut a = PlayingAnim::new(0);
        a.playing = false;
        assert_eq!(tick(&mut a, &data, 1000), Some(0));
    }

    #[test]
    fn tick_empty_animation_returns_none() {
        let data = Animation { frame_count: 0, unused: 0, index: 0, frames: vec![] };
        let mut a = PlayingAnim::new(0);
        assert_eq!(tick(&mut a, &data, 50), None);
    }

    #[test]
    fn tick_zero_duration_frames_do_not_hang() {
        // Without the iteration cap, this would loop forever: every frame_ms
        // is 0, so the inequality guard never breaks while elapsed > 0.
        let data = Animation {
            frame_count: 3,
            unused: 0,
            index: 0,
            frames: vec![frame(0, 10), frame(0, 11), frame(0, 12)],
        };
        let mut a = PlayingAnim::new(0);
        // Test must complete; if tick hangs the harness will time out.
        let result = tick(&mut a, &data, 100);
        assert!(result.is_some());
        assert!(a.frame_idx < 3);
        // Residual elapsed is discarded so the next tick starts fresh.
        assert_eq!(a.frame_elapsed_ms, 0);
    }

    #[test]
    fn tick_mixed_zero_and_nonzero_durations_progress() {
        // 0-duration frames should be skipped instantly; the nonzero one is
        // what actually consumes time.
        let data = Animation {
            frame_count: 3,
            unused: 0,
            index: 0,
            frames: vec![frame(0, 10), frame(2, 11), frame(0, 12)],
        };
        let mut a = PlayingAnim::new(0);
        // 50 ms < 2 ticks (= 66 ms), but the first 0-duration frame skips
        // forward so we land on frame 1 with elapsed carried over.
        let r = tick(&mut a, &data, 50);
        assert_eq!(r, Some(1));
        assert_eq!(a.frame_idx, 1);
    }

    fn solid_palette(color: Rgba) -> Palette {
        let mut colors = vec![Rgba::new(0, 0, 0, 0); 256]; // index 0 = transparent
        colors[1] = color;
        Palette { index: 0, format: TextureFormat::Ci8, colors }
    }

    /// Build a 2x2 CI8 texture with `idx` in every pixel.
    fn solid_texture(index: u32, idx: u8) -> Texture {
        Texture {
            index,
            width: 2,
            height: 2,
            format: TextureFormat::Ci8,
            palette_index: 0,
            pixels: vec![idx; 4],
        }
    }

    fn empty_manifest() -> Manifest {
        Manifest {
            label: "test".into(),
            subdir: "test".into(),
            kind: SpriteKind::Unknown,
            sprite_type: SpriteType::Type1,
            sprite_count: 0,
            spritesheet_index_count: 0,
            palette_count: 0,
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
        }
    }

    fn asset_with(textures: Vec<Texture>, palettes: Vec<Palette>) -> SpriteAsset {
        SpriteAsset {
            dir: PathBuf::new(),
            manifest: empty_manifest(),
            textures: textures.into_iter().map(|t| (t.index, t)).collect::<BTreeMap<_, _>>(),
            palettes: palettes.into_iter().map(|p| (p.index, p)).collect::<BTreeMap<_, _>>(),
            animations: BTreeMap::new(),
        }
    }

    #[test]
    fn compute_sprite_rect_centers_on_anchor() {
        // 4x4 texture, anchor at (10, 20): left = 10 - 2 = 8, top = 20 - 2 = 18
        assert_eq!(compute_sprite_rect(4, 4, 10, 20), (8, 18, 12, 22));
    }

    #[test]
    fn animation_bounds_spans_all_layers_and_frames() {
        // Two frames, each with one 2x2 layer. Anchors shift by (10, 0) between
        // frames, so the global bbox should span 12 wide, 2 tall.
        let asset = asset_with(
            vec![solid_texture(0, 1)],
            vec![solid_palette(Rgba::new(255, 0, 0, 255))],
        );
        let anim = Animation {
            frame_count: 2,
            unused: 0,
            index: 0,
            frames: vec![
                AnimationFrame {
                    duration: 1,
                    has_audio: 0,
                    sprites: vec![FrameSprite {
                        spritesheet_index: 0,
                        anchor_x: 0,
                        anchor_y: 0,
                    }],
                },
                AnimationFrame {
                    duration: 1,
                    has_audio: 0,
                    sprites: vec![FrameSprite {
                        spritesheet_index: 0,
                        anchor_x: 10,
                        anchor_y: 0,
                    }],
                },
            ],
        };
        // Frame 0 rect = (-1, -1, 1, 1). Frame 1 rect = (9, -1, 11, 1).
        // Union = (-1, -1, 11, 1).
        assert_eq!(compute_animation_bounds(&asset, &anim), Some((-1, -1, 11, 1)));
    }

    #[test]
    fn animation_bounds_none_when_no_known_texture() {
        let asset = asset_with(vec![], vec![]);
        let anim = Animation {
            frame_count: 1,
            unused: 0,
            index: 0,
            frames: vec![AnimationFrame {
                duration: 1,
                has_audio: 0,
                sprites: vec![FrameSprite { spritesheet_index: 99, anchor_x: 0, anchor_y: 0 }],
            }],
        };
        assert_eq!(compute_animation_bounds(&asset, &anim), None);
    }

    fn solid_2x2_texture(index: u32, palette_index: u32) -> Texture {
        Texture {
            index,
            width: 2,
            height: 2,
            format: TextureFormat::Ci8,
            palette_index,
            pixels: vec![1; 4],
        }
    }

    fn one_color_palette(index: u32, color: Rgba) -> Palette {
        let mut colors = vec![Rgba::new(0, 0, 0, 0); 256];
        colors[1] = color;
        Palette { index, format: TextureFormat::Ci8, colors }
    }

    #[test]
    fn compose_frame_blits_two_layers_at_their_anchors() {
        // Two 2x2 textures with distinct colors. Layer A anchored at (0, 0),
        // layer B anchored at (4, 0). Global bbox = (-1, -1, 5, 1) -> a 6x2
        // canvas. Layer A occupies columns 0..2, layer B columns 4..6, with
        // a transparent gap in the middle.
        let red = Rgba::new(255, 0, 0, 255);
        let green = Rgba::new(0, 255, 0, 255);
        let asset = asset_with(
            vec![solid_2x2_texture(0, 0), solid_2x2_texture(1, 1)],
            vec![one_color_palette(0, red), one_color_palette(1, green)],
        );
        let frame = AnimationFrame {
            duration: 1,
            has_audio: 0,
            sprites: vec![
                FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 },
                FrameSprite { spritesheet_index: 1, anchor_x: 4, anchor_y: 0 },
            ],
        };
        let bounds = (-1i32, -1i32, 5i32, 1i32);
        let mut cache = FrameCache::default();
        let buf = compose_frame_buffer(&mut cache, &asset, &frame, bounds).unwrap();
        assert_eq!(buf.width(), 6);
        assert_eq!(buf.height(), 2);
        let pixels = buf.as_slice();
        // Column 0 (top-left of canvas) should be red (layer A).
        assert_eq!(pixels[0], Rgba8Pixel { r: 255, g: 0, b: 0, a: 255 });
        // Column 4 (start of layer B) should be green.
        assert_eq!(pixels[4], Rgba8Pixel { r: 0, g: 255, b: 0, a: 255 });
        // Column 2 (between the two layers) should be transparent.
        assert_eq!(pixels[2], Rgba8Pixel { r: 0, g: 0, b: 0, a: 0 });
    }

    #[test]
    fn compose_frame_first_layer_paints_on_top() {
        // Two 2x2 textures sharing the same anchor, so they fully overlap.
        // sprites[0] = red, sprites[1] = blue. Result must be entirely red:
        // sprites[0] is rendered LAST (on top), matching the game's bitmap[0]
        // ordering convention.
        let red = Rgba::new(255, 0, 0, 255);
        let blue = Rgba::new(0, 0, 255, 255);
        let asset = asset_with(
            vec![solid_2x2_texture(0, 0), solid_2x2_texture(1, 1)],
            vec![one_color_palette(0, red), one_color_palette(1, blue)],
        );
        let frame = AnimationFrame {
            duration: 1,
            has_audio: 0,
            sprites: vec![
                FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 },
                FrameSprite { spritesheet_index: 1, anchor_x: 0, anchor_y: 0 },
            ],
        };
        let bounds = compute_animation_bounds(
            &asset,
            &Animation { frame_count: 1, unused: 0, index: 0, frames: vec![frame.clone()] },
        )
        .unwrap();
        let mut cache = FrameCache::default();
        let buf = compose_frame_buffer(&mut cache, &asset, &frame, bounds).unwrap();
        assert_eq!(buf.width(), 2);
        assert_eq!(buf.height(), 2);
        for &p in buf.as_slice() {
            assert_eq!(p, Rgba8Pixel { r: 255, g: 0, b: 0, a: 255 });
        }
    }

    #[test]
    fn compose_frame_h_flipped_mirrors_columns() {
        // Two 2x2 layers side by side: red at anchor (0,0), green at anchor
        // (4,0). Unflipped row 0 is [R, R, _, _, G, G]; flipped is the
        // reverse: [G, G, _, _, R, R]. This pins down that the post-compose
        // mirror happens across the *whole canvas*, not per-layer — which is
        // what the game's flip flag does and what makes the script preview
        // match the gallery (the gallery also flips the composed result via
        // shader mirroring at display time).
        let red = Rgba::new(255, 0, 0, 255);
        let green = Rgba::new(0, 255, 0, 255);
        let asset = asset_with(
            vec![solid_2x2_texture(0, 0), solid_2x2_texture(1, 1)],
            vec![one_color_palette(0, red), one_color_palette(1, green)],
        );
        let frame = AnimationFrame {
            duration: 1,
            has_audio: 0,
            sprites: vec![
                FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 },
                FrameSprite { spritesheet_index: 1, anchor_x: 4, anchor_y: 0 },
            ],
        };
        let bounds = (-1i32, -1i32, 5i32, 1i32);
        let mut cache = FrameCache::default();
        let buf = compose_frame_buffer(&mut cache, &asset, &frame, bounds).unwrap();
        let flipped = flip_buffer_horizontal(buf);
        let pixels = flipped.as_slice();
        // Row 0: column 0 should now be green (was at column 4).
        assert_eq!(pixels[0], Rgba8Pixel { r: 0, g: 255, b: 0, a: 255 });
        // Row 0: column 5 should now be red (was at column 0).
        assert_eq!(pixels[5], Rgba8Pixel { r: 255, g: 0, b: 0, a: 255 });
    }

    #[test]
    fn frame_cache_decodes_each_texture_once() {
        // Two animations sharing a texture. After composing both, the layer
        // cache should hold exactly one decoded entry for the shared texture.
        let red = Rgba::new(255, 0, 0, 255);
        let asset = asset_with(vec![solid_2x2_texture(0, 0)], vec![one_color_palette(0, red)]);
        let frame_a = AnimationFrame {
            duration: 1,
            has_audio: 0,
            sprites: vec![FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 }],
        };
        let frame_b = AnimationFrame {
            duration: 1,
            has_audio: 0,
            sprites: vec![FrameSprite { spritesheet_index: 0, anchor_x: 5, anchor_y: 5 }],
        };
        let bounds = (-1i32, -1i32, 6i32, 6i32);
        let mut cache = FrameCache::default();
        let _ = cache.get_or_render(&asset, 0, 0, &frame_a, bounds);
        let _ = cache.get_or_render(&asset, 1, 0, &frame_b, bounds);
        assert_eq!(cache.layers.len(), 1, "shared texture should decode exactly once");
        assert_eq!(cache.composed.len(), 2, "two distinct frames -> two cached composites");
    }
}
