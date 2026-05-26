//! Per-sprite binary writer. Inverse of `src/extract/sprite.rs`. Mirrors
//! `tools/libhm64/sprites/assembler.py`.

use std::collections::HashMap;
use std::path::{Path, PathBuf};

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::io::binary::{pack_s16_le, pack_u16_be, pack_u16_le, pack_u32_be};
use crate::model::{Animation, Palette, SpriteAsset, SpriteType, TextureFormat};

/// Returns the list of bin files written.
pub fn export_bins(sprite: &SpriteAsset, out_dir: &Path) -> ExportResult<Vec<PathBuf>> {
    std::fs::create_dir_all(out_dir).map_err(|e| ExportError::write(out_dir, e))?;
    match sprite.manifest.sprite_type {
        SpriteType::Type1 => export_type1(sprite, out_dir),
        SpriteType::Type2 => export_type2(sprite, out_dir),
    }
}

fn export_type1(sprite: &SpriteAsset, out_dir: &Path) -> ExportResult<Vec<PathBuf>> {
    let label = &sprite.manifest.label;
    let bundle = build_type1_bundle(sprite)?;

    let texture_path = out_dir.join(format!("{label}Texture.bin"));
    let assets_path = out_dir.join(format!("{label}AssetsIndex.bin"));
    let spritesheet_path = out_dir.join(format!("{label}SpritesheetIndex.bin"));

    atomic_write(&texture_path, &bundle.texture).map_err(|e| ExportError::write(&texture_path, e))?;
    atomic_write(&assets_path, &bundle.assets_index)
        .map_err(|e| ExportError::write(&assets_path, e))?;
    atomic_write(&spritesheet_path, &bundle.spritesheet_index)
        .map_err(|e| ExportError::write(&spritesheet_path, e))?;

    Ok(vec![texture_path, assets_path, spritesheet_path])
}

fn export_type2(sprite: &SpriteAsset, out_dir: &Path) -> ExportResult<Vec<PathBuf>> {
    let label = &sprite.manifest.label;
    let bundle = build_type2_bundle(sprite)?;

    let texture_path = out_dir.join(format!("{label}Texture.bin"));
    let assets_path = out_dir.join(format!("{label}AssetsIndex.bin"));

    atomic_write(&texture_path, &bundle.texture).map_err(|e| ExportError::write(&texture_path, e))?;
    atomic_write(&assets_path, &bundle.assets_index)
        .map_err(|e| ExportError::write(&assets_path, e))?;

    Ok(vec![texture_path, assets_path])
}

struct Bundle {
    texture: Vec<u8>,
    assets_index: Vec<u8>,
    /// Empty for type-2 sprites — the spritesheet index is embedded in `texture`.
    spritesheet_index: Vec<u8>,
}

// ---- Section builders ----

fn build_spritesheet_data(sprite: &SpriteAsset) -> (Vec<u8>, Vec<u32>) {
    let sprite_count = sprite.manifest.sprite_count as usize;
    let mut data: Vec<u8> = Vec::new();
    let mut offsets: Vec<u32> = Vec::with_capacity(sprite_count + 1);

    for sprite_idx in 0..sprite_count {
        offsets.push(data.len() as u32);
        let Some(tex) = sprite.textures.get(&(sprite_idx as u32)) else {
            continue;
        };

        // 8-byte texture header: "46d" + format_flag + width(s16 LE) + height(s16 LE)
        data.extend_from_slice(b"\x34\x36\x64");
        data.push(match tex.format {
            TextureFormat::Ci4 => 0x10,
            TextureFormat::Ci8 => 0x00,
        });
        data.extend_from_slice(&pack_s16_le(tex.width as i16));
        data.extend_from_slice(&pack_s16_le(tex.height as i16));
        data.extend_from_slice(&tex.pixels);
    }
    offsets.push(data.len() as u32);
    (data, offsets)
}

fn pack_palette_block(palette: &Palette) -> Vec<u8> {
    let mut buf = Vec::with_capacity(4 + palette.colors.len() * 2 + 4);
    // 4-byte header: "46d" + format_flag.
    buf.extend_from_slice(b"\x34\x36\x64");
    buf.push(match palette.format {
        TextureFormat::Ci4 => 0x10,
        TextureFormat::Ci8 => 0x00,
    });
    // RGBA5551 big-endian, one u16 per color.
    for c in &palette.colors {
        let v = c.to_rgba5551();
        buf.push((v >> 8) as u8);
        buf.push((v & 0xFF) as u8);
    }
    // Sentinel.
    buf.extend_from_slice(b"\xff\xff\xff\xff");
    buf
}

fn build_palette_section(sprite: &SpriteAsset) -> ExportResult<(Vec<u8>, Vec<u8>)> {
    let manifest = &sprite.manifest;

    if !manifest.palette_index.is_empty() {
        // Preservation path: keep the original ROM's palette index layout.
        let palette_index_count = if manifest.palette_index_count > 0 {
            manifest.palette_index_count as usize
        } else {
            manifest.palette_index.len()
        };
        let palette_index_size = palette_index_count * 4;
        let mut palette_data: Vec<u8> = Vec::new();
        let mut written_offsets: HashMap<u32, u32> = HashMap::new();

        for pal_idx in 0..manifest.palette_index.len().saturating_sub(1) {
            let orig_offset = manifest.palette_index[pal_idx];
            let next_offset = manifest.palette_index[pal_idx + 1];

            if next_offset == 0 || orig_offset == next_offset {
                // Reused / empty slot: just register where it would have been.
                written_offsets
                    .insert(orig_offset, palette_data.len() as u32 + palette_index_size as u32);
                continue;
            }

            let Some(palette) = sprite.palettes.get(&(pal_idx as u32)) else {
                written_offsets
                    .insert(orig_offset, palette_data.len() as u32 + palette_index_size as u32);
                continue;
            };
            let new_offset = palette_data.len() as u32 + palette_index_size as u32;
            written_offsets.insert(orig_offset, new_offset);
            palette_data.extend_from_slice(&pack_palette_block(palette));
        }

        let end_offset = palette_data.len() as u32 + palette_index_size as u32;
        let last_orig_offset = *manifest.palette_index.last().unwrap();
        if last_orig_offset != 0 {
            written_offsets.insert(last_orig_offset, end_offset);
        }

        let mut palette_index: Vec<u8> = Vec::with_capacity(palette_index_size);
        for i in 0..palette_index_count {
            if i < manifest.palette_index.len() {
                let orig_offset = manifest.palette_index[i];
                if orig_offset == 0 {
                    palette_index.extend_from_slice(&pack_u32_be(0));
                } else {
                    let new_offset = *written_offsets.get(&orig_offset).unwrap_or(&end_offset);
                    palette_index.extend_from_slice(&pack_u32_be(new_offset));
                }
            } else if i == palette_index_count - 1 && !manifest.palette_index_end_bytes.is_empty() {
                palette_index.extend_from_slice(&manifest.palette_index_end_bytes);
            } else {
                palette_index.extend_from_slice(&pack_u32_be(0));
            }
        }
        Ok((palette_index, palette_data))
    } else {
        // Fresh-sprite path: sequential layout.
        let mut palette_data: Vec<u8> = Vec::new();
        let mut palette_offsets: Vec<u32> = Vec::new();

        let unique_palettes: Vec<u8> = {
            let mut s = manifest.sprite_to_palette.clone();
            s.sort_unstable();
            s.dedup();
            s
        };
        let palette_count = unique_palettes.last().map(|&m| m as usize + 1).unwrap_or(0);

        for pal_idx in 0..palette_count {
            palette_offsets.push(palette_data.len() as u32);
            if let Some(palette) = sprite.palettes.get(&(pal_idx as u32)) {
                palette_data.extend_from_slice(&pack_palette_block(palette));
            }
        }
        palette_offsets.push(palette_data.len() as u32);

        let palette_index_size = palette_offsets.len() * 4;
        let mut palette_index: Vec<u8> = Vec::with_capacity(palette_index_size);
        for offset in &palette_offsets {
            palette_index.extend_from_slice(&pack_u32_be(*offset + palette_index_size as u32));
        }
        Ok((palette_index, palette_data))
    }
}

fn pack_animation_block(animation: &Animation) -> Vec<u8> {
    let mut buf = Vec::new();
    // 8-byte header: "46d\x00" + frame_count(u16 LE) + 2 bytes 0x0000.
    buf.extend_from_slice(b"\x34\x36\x64\x00");
    buf.extend_from_slice(&pack_u16_le(animation.frames.len() as u16));
    buf.extend_from_slice(b"\x00\x00");

    for frame in &animation.frames {
        // Per-frame: sprite_count(u16 LE), duration(u8), has_audio(u8).
        buf.extend_from_slice(&pack_u16_le(frame.sprites.len() as u16));
        buf.push(frame.duration as u8);
        buf.push(frame.has_audio);

        for sprite in &frame.sprites {
            // Per-sprite (8 bytes): spritesheet_index(u16 LE), 0(u16 LE), anchor_x(s16 LE), anchor_y(s16 LE).
            buf.extend_from_slice(&pack_u16_le(sprite.spritesheet_index as u16));
            buf.extend_from_slice(&pack_u16_le(0));
            buf.extend_from_slice(&pack_s16_le(sprite.anchor_x));
            buf.extend_from_slice(&pack_s16_le(sprite.anchor_y));
        }
    }
    buf
}

fn build_animation_section(sprite: &SpriteAsset) -> ExportResult<(Vec<u8>, Vec<u8>)> {
    let manifest = &sprite.manifest;

    if !manifest.animation_index.is_empty() {
        // Preservation path.
        let animation_index_size = manifest.animation_index.len() * 4;
        let mut animation_data: Vec<u8> = Vec::new();
        let mut written_offsets: HashMap<u32, u32> = HashMap::new();

        for (idx, &orig_offset) in manifest
            .animation_index
            .iter()
            .take(manifest.animation_index.len().saturating_sub(1))
            .enumerate()
        {
            if written_offsets.contains_key(&orig_offset) {
                continue;
            }
            if let Some(animation) = sprite.animations.get(&(idx as u32)) {
                let new_offset = animation_data.len() as u32 + animation_index_size as u32;
                written_offsets.insert(orig_offset, new_offset);
                animation_data.extend_from_slice(&pack_animation_block(animation));
            }
        }

        let end_offset = animation_data.len() as u32 + animation_index_size as u32;
        let last = *manifest.animation_index.last().unwrap();
        written_offsets.insert(last, end_offset);

        let mut animation_index: Vec<u8> = Vec::with_capacity(animation_index_size);
        for &orig_offset in &manifest.animation_index {
            let new_offset = *written_offsets.get(&orig_offset).unwrap_or(&end_offset);
            animation_index.extend_from_slice(&pack_u32_be(new_offset));
        }
        Ok((animation_index, animation_data))
    } else {
        // Fresh-sprite path.
        let mut animation_data: Vec<u8> = Vec::new();
        let mut animation_offsets: Vec<u32> = Vec::new();

        let mut keys: Vec<u32> = sprite.animations.keys().copied().collect();
        keys.sort_unstable();
        for k in keys {
            animation_offsets.push(animation_data.len() as u32);
            animation_data.extend_from_slice(&pack_animation_block(&sprite.animations[&k]));
        }
        animation_offsets.push(animation_data.len() as u32);

        let animation_index_size = animation_offsets.len() * 4;
        let mut animation_index: Vec<u8> = Vec::with_capacity(animation_index_size);
        for offset in &animation_offsets {
            animation_index.extend_from_slice(&pack_u32_be(*offset + animation_index_size as u32));
        }
        Ok((animation_index, animation_data))
    }
}

fn build_s2p_section(sprite: &SpriteAsset) -> Vec<u8> {
    let manifest = &sprite.manifest;
    let mut data: Vec<u8> = Vec::new();
    // 4-byte header: "46d\x00".
    data.extend_from_slice(b"\x34\x36\x64\x00");
    data.extend_from_slice(&manifest.sprite_to_palette);

    if !manifest.s2p_padding.is_empty() {
        data.extend_from_slice(&manifest.s2p_padding);
    } else if manifest.asset_offsets.len() >= 5 && manifest.asset_offsets[3] > 0 {
        // Match the original section size by padding with 0xFF.
        let original_s2p_size = manifest.asset_offsets[4].saturating_sub(manifest.asset_offsets[3])
            as usize;
        while data.len() < original_s2p_size {
            data.push(0xFF);
        }
    } else {
        // Align to 4 with 0xFF.
        while data.len() % 4 != 0 {
            data.push(0xFF);
        }
    }
    data
}

fn pack_assets_index_bytes(
    spritesheet_start: u32,
    palette_start: u32,
    animation_start: u32,
    s2p_start: u32,
    assets_index_start: u32,
) -> Vec<u8> {
    let mut buf = Vec::with_capacity(32);
    for v in [
        spritesheet_start,
        palette_start,
        animation_start,
        s2p_start,
        assets_index_start,
        0,
        0,
        0,
    ] {
        buf.extend_from_slice(&pack_u32_be(v));
    }
    buf
}

// ---- Type-1 bundle ----

fn build_type1_bundle(sprite: &SpriteAsset) -> ExportResult<Bundle> {
    let manifest = &sprite.manifest;
    let label = manifest.label.as_str();
    let sprite_count = manifest.sprite_count as usize;
    let spritesheet_index_count = if manifest.spritesheet_index_count > 0 {
        manifest.spritesheet_index_count as usize
    } else {
        sprite_count + 1
    };

    let (spritesheet_data, texture_offsets) = build_spritesheet_data(sprite);
    let (palette_index, palette_data) = build_palette_section(sprite)?;
    let (animation_index, animation_data) = build_animation_section(sprite)?;
    let s2p_data = build_s2p_section(sprite);

    // Layout offsets.
    let embedded_index_size = if label == "festivalFlowers" {
        spritesheet_index_count * 4
    } else {
        0
    };
    let mut current_offset = embedded_index_size as u32;
    let spritesheet_start = if label == "festivalFlowers" { 0 } else { current_offset };
    current_offset += spritesheet_data.len() as u32;

    let palette_section_start = current_offset;
    let mut palette_section = palette_index;
    palette_section.extend_from_slice(&palette_data);
    current_offset += palette_section.len() as u32;

    let animation_section_start = current_offset;
    let mut animation_section = animation_index;
    animation_section.extend_from_slice(&animation_data);
    let animation_section_end = animation_section_start + animation_section.len() as u32;
    if animation_section_end % 8 == 4 {
        animation_section.extend_from_slice(b"\xff\xff\xff\xff");
    }
    current_offset += animation_section.len() as u32;

    let s2p_start = current_offset;
    current_offset += s2p_data.len() as u32;

    let assets_index_start = current_offset;

    // Spritesheet index file (separate for type-1).
    let mut spritesheet_index: Vec<u8> = Vec::new();
    for i in 0..spritesheet_index_count {
        if i == spritesheet_index_count - 1 && !manifest.spritesheet_index_end_bytes.is_empty() {
            spritesheet_index.extend_from_slice(&manifest.spritesheet_index_end_bytes);
        } else if i < texture_offsets.len() {
            spritesheet_index.extend_from_slice(&pack_u32_be(texture_offsets[i]));
        } else {
            spritesheet_index.extend_from_slice(&pack_u32_be(0));
        }
    }
    while spritesheet_index.len() % 16 != 0 {
        spritesheet_index.extend_from_slice(&pack_u32_be(0));
    }

    // Texture binary.
    let mut texture: Vec<u8> = Vec::new();
    if label == "festivalFlowers" {
        for i in 0..spritesheet_index_count {
            if i < texture_offsets.len() {
                texture.extend_from_slice(&pack_u32_be(
                    texture_offsets[i] + embedded_index_size as u32,
                ));
            } else {
                texture.extend_from_slice(&pack_u32_be(0));
            }
        }
    }
    texture.extend_from_slice(&spritesheet_data);
    texture.extend_from_slice(&palette_section);
    texture.extend_from_slice(&animation_section);
    texture.extend_from_slice(&s2p_data);
    if !manifest.texture_end_padding.is_empty() {
        texture.extend_from_slice(&manifest.texture_end_padding);
    }

    let assets_index = pack_assets_index_bytes(
        spritesheet_start,
        palette_section_start,
        animation_section_start,
        s2p_start,
        assets_index_start,
    );

    Ok(Bundle { texture, assets_index, spritesheet_index })
}

// ---- Type-2 bundle ----

fn build_type2_bundle(sprite: &SpriteAsset) -> ExportResult<Bundle> {
    let manifest = &sprite.manifest;
    let sprite_count = manifest.sprite_count as usize;
    let spritesheet_index_count = if manifest.spritesheet_index_count > 0 {
        manifest.spritesheet_index_count as usize
    } else {
        sprite_count + 1
    };

    // Pack each texture (header + pixels) into its own buffer.
    let mut texture_data_list: Vec<Vec<u8>> = Vec::with_capacity(sprite_count);
    for sprite_idx in 0..sprite_count {
        let Some(tex) = sprite.textures.get(&(sprite_idx as u32)) else {
            texture_data_list.push(Vec::new());
            continue;
        };
        let mut buf = Vec::with_capacity(8 + tex.pixels.len());
        buf.extend_from_slice(b"\x34\x36\x64");
        buf.push(match tex.format {
            TextureFormat::Ci4 => 0x10,
            TextureFormat::Ci8 => 0x00,
        });
        buf.extend_from_slice(&pack_s16_le(tex.width as i16));
        buf.extend_from_slice(&pack_s16_le(tex.height as i16));
        buf.extend_from_slice(&tex.pixels);
        texture_data_list.push(buf);
    }

    // Index table sits at the start of the embedded spritesheet section.
    let index_table_size = spritesheet_index_count * 4;
    let mut current = index_table_size as u32;
    let mut texture_offsets: Vec<u32> = Vec::with_capacity(texture_data_list.len() + 1);
    for tex_data in &texture_data_list {
        texture_offsets.push(current);
        current += tex_data.len() as u32;
    }
    texture_offsets.push(current);

    // Build embedded spritesheet section: index table + concatenated texture buffers.
    let mut spritesheet_data: Vec<u8> = Vec::new();
    for i in 0..spritesheet_index_count {
        if i == spritesheet_index_count - 1 && !manifest.spritesheet_index_end_bytes.is_empty() {
            spritesheet_data.extend_from_slice(&manifest.spritesheet_index_end_bytes);
        } else if i < texture_offsets.len() {
            spritesheet_data.extend_from_slice(&pack_u32_be(texture_offsets[i]));
        } else {
            spritesheet_data.extend_from_slice(&pack_u32_be(0));
        }
    }
    for tex_data in &texture_data_list {
        spritesheet_data.extend_from_slice(tex_data);
    }

    let (palette_index, palette_data) = build_palette_section(sprite)?;
    let (animation_index, animation_data) = build_animation_section(sprite)?;
    let s2p_data = build_s2p_section(sprite);

    // Layout: spritesheet at 0, then palette/animation/s2p, then assets_index_start.
    let mut current_offset: u32 = 0;
    let spritesheet_start = current_offset;
    current_offset += spritesheet_data.len() as u32;

    let palette_section_start = current_offset;
    let mut palette_section = palette_index;
    palette_section.extend_from_slice(&palette_data);
    current_offset += palette_section.len() as u32;

    let animation_section_start = current_offset;
    let mut animation_section = animation_index;
    animation_section.extend_from_slice(&animation_data);
    let animation_section_end = animation_section_start + animation_section.len() as u32;
    if animation_section_end % 8 == 4 {
        animation_section.extend_from_slice(b"\xff\xff\xff\xff");
    }
    current_offset += animation_section.len() as u32;

    let s2p_start = current_offset;
    current_offset += s2p_data.len() as u32;
    let assets_index_start = current_offset;

    let mut texture: Vec<u8> = Vec::new();
    texture.extend_from_slice(&spritesheet_data);
    texture.extend_from_slice(&palette_section);
    texture.extend_from_slice(&animation_section);
    texture.extend_from_slice(&s2p_data);
    if !manifest.texture_end_padding.is_empty() {
        texture.extend_from_slice(&manifest.texture_end_padding);
    }

    let assets_index = pack_assets_index_bytes(
        spritesheet_start,
        palette_section_start,
        animation_section_start,
        s2p_start,
        assets_index_start,
    );

    Ok(Bundle { texture, assets_index, spritesheet_index: Vec::new() })
}
