//! Per-sprite extraction. Mirrors the byte-format part of
//! `tools/libhm64/sprites/extractor.py::extract_sprite`, then writes the
//! workspace files via existing writers.
//!
//! The Python extractor also emits indexed PNGs per texture for human
//! preview; we skip that here. The editor doesn't need PNGs to load a
//! sprite — they can be regenerated later by a separate command.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};

use crate::export::manifest_export::write_manifest;
use crate::export::palette_export::{write_palette_json, write_palette_pal};
use crate::export::texture_export::write_texture_ci;
use crate::export::{atomic_write, ExportError};
use crate::extract::{
    addresses::{self, AddressVariant, SpriteAddress},
    animation::decode_animation,
    palette::decode_palette,
    texture::decode_texture,
    ExtractError, ExtractResult,
};
use crate::model::{
    Animation, Manifest, Palette, PaletteMeta, SpriteKind, SpriteType, Texture, TextureFormat,
    TextureMeta,
};

/// Extract one sprite from the ROM into `workspace/<subdir>/<label>/`.
/// Returns the sprite directory path.
pub fn extract_sprite(
    rom: &[u8],
    info: &SpriteAddress,
    workspace: &Path,
) -> ExtractResult<PathBuf> {
    let sprite_dir = workspace.join("assets/sprites").join(&info.subdir).join(&info.label);
    fs::create_dir_all(sprite_dir.join("textures"))?;
    fs::create_dir_all(sprite_dir.join("palettes"))?;
    fs::create_dir_all(sprite_dir.join("animations"))?;

    let asset_offsets = addresses::read_asset_offsets(rom, info.addr_index);
    let spritesheet_offsets = addresses::spritesheet_offsets(rom, info);
    if spritesheet_offsets.len() < 2 {
        return Err(ExtractError::Io(std::io::Error::other(format!(
            "no sprites in {}",
            info.label
        ))));
    }

    // Sprite count: spritesheet_index entries minus the trailing sentinel,
    // then trim trailing zeros.
    let spritesheet_index_count = spritesheet_offsets.len();
    let mut sprite_count = spritesheet_index_count.saturating_sub(1);
    while sprite_count > 0 && spritesheet_offsets[sprite_count] == 0 {
        sprite_count -= 1;
    }

    // Capture last spritesheet index entry's raw bytes (4 bytes).
    let last_entry_addr = match &info.variant {
        AddressVariant::Type1 { spritesheet_index_base, .. } => {
            *spritesheet_index_base + (spritesheet_index_count as u32 - 1) * 4
        }
        AddressVariant::Type2 { .. } => {
            info.addr_base + (spritesheet_index_count as u32 - 1) * 4
        }
    };
    let spritesheet_index_end_bytes = read_4_bytes(rom, last_entry_addr).to_vec();

    // Palette section: if asset_offsets[1] is 0 or invalid, this sprite has
    // no palettes — extract textures only and return.
    if asset_offsets[1] == 0 {
        return finalize_simple(
            rom,
            info,
            &sprite_dir,
            &spritesheet_offsets,
            sprite_count,
            spritesheet_index_count as u32,
            spritesheet_index_end_bytes,
            &asset_offsets,
        );
    }

    let first_pal_offset = read_u32_be(rom, info.addr_base + asset_offsets[1]);
    if first_pal_offset == 0 || first_pal_offset > 0x1000 || first_pal_offset % 4 != 0 {
        return finalize_simple(
            rom,
            info,
            &sprite_dir,
            &spritesheet_offsets,
            sprite_count,
            spritesheet_index_count as u32,
            spritesheet_index_end_bytes,
            &asset_offsets,
        );
    }
    let pal_count = (first_pal_offset / 4) as usize;
    if pal_count > 256 {
        return finalize_simple(
            rom,
            info,
            &sprite_dir,
            &spritesheet_offsets,
            sprite_count,
            spritesheet_index_count as u32,
            spritesheet_index_end_bytes,
            &asset_offsets,
        );
    }
    let palette_offsets_raw = addresses::palette_offsets(rom, info.addr_base, &asset_offsets);

    // Last palette index entry (4 raw bytes) for round-trip preservation.
    let palette_section = info.addr_base + asset_offsets[1];
    let last_pal_entry_addr = palette_section + (pal_count as u32 - 1) * 4;
    let palette_index_end_bytes = read_4_bytes(rom, last_pal_entry_addr).to_vec();

    // Trim sprite_count by the s2p section size (matches python).
    let s2p_section_size = asset_offsets[4].saturating_sub(asset_offsets[3]);
    if s2p_section_size > 4 {
        let actual_sprite_count = (s2p_section_size - 4) as usize;
        if actual_sprite_count < sprite_count {
            sprite_count = actual_sprite_count;
        }
    }

    let sprite_to_palette =
        addresses::sprite_to_palette_map(rom, info.addr_base, &asset_offsets, sprite_count);

    // S2P padding bytes from end of s2p data to end of section.
    let s2p_section_start = (info.addr_base + asset_offsets[3]) as usize;
    let s2p_data_size = 4 + sprite_count;
    let s2p_padding_size = s2p_section_size as usize - s2p_data_size.min(s2p_section_size as usize);
    let s2p_padding: Vec<u8> = if s2p_padding_size > 0 {
        rom.get(s2p_section_start + s2p_data_size..s2p_section_start + s2p_section_size as usize)
            .map(|s| s.to_vec())
            .unwrap_or_default()
    } else {
        Vec::new()
    };

    let anim_offsets = addresses::animation_offsets(rom, info.addr_base, &asset_offsets);

    // Decode and write palettes.
    let mut palettes: BTreeMap<u32, Palette> = BTreeMap::new();
    let mut palettes_meta: std::collections::BTreeMap<String, PaletteMeta> =
        std::collections::BTreeMap::new();

    for pal_idx in 0..palette_offsets_raw.len().saturating_sub(1) {
        let Some(decoded) = decode_palette(rom, palette_section, &palette_offsets_raw, pal_idx)
        else {
            continue;
        };
        let format = if decoded.colors.len() <= 16 { TextureFormat::Ci4 } else { TextureFormat::Ci8 };
        let palette = Palette { index: pal_idx as u32, format, colors: decoded.colors };
        write_palette_pal(&sprite_dir, &palette).map_err(map_export)?;
        write_palette_json(&sprite_dir, &palette).map_err(map_export)?;
        palettes_meta.insert(
            pal_idx.to_string(),
            PaletteMeta { color_count: palette.colors.len() as u32, format },
        );
        palettes.insert(pal_idx as u32, palette);
    }

    // Decode and write textures.
    let mut texture_metas: Vec<TextureMeta> = Vec::new();
    for sprite_idx in 0..sprite_count {
        let Some(dec) =
            decode_texture(rom, info.addr_base, &spritesheet_offsets, sprite_idx)
        else {
            continue;
        };
        if dec.width <= 0 || dec.height <= 0 || dec.pixels.is_empty() {
            continue;
        }
        let pal_idx_for_sprite =
            sprite_to_palette.get(sprite_idx).copied().unwrap_or(0) as u32;
        let texture = Texture {
            index: sprite_idx as u32,
            width: dec.width as u16,
            height: dec.height as u16,
            format: dec.format,
            palette_index: pal_idx_for_sprite,
            pixels: dec.pixels,
        };
        write_texture_ci(&sprite_dir, &texture).map_err(map_export)?;
        texture_metas.push(TextureMeta {
            index: texture.index,
            width: texture.width,
            height: texture.height,
            format: texture.format,
            palette_index: pal_idx_for_sprite,
        });
    }

    // Decode and write animations.
    let mut animations: Vec<Animation> = Vec::new();
    for anim_idx in 0..anim_offsets.len().saturating_sub(1) {
        let anim_base = info.addr_base + asset_offsets[2];
        if let Some(anim) = decode_animation(rom, anim_base, &anim_offsets, anim_idx) {
            animations.push(anim);
        }
    }
    for anim in &animations {
        let path = sprite_dir.join("animations").join(format!("{:02}.json", anim.index));
        let mut text = serde_json::to_string_pretty(anim)
            .map_err(|e| ExportError::serialize(&path, e))
            .map_err(map_export)?;
        text.push('\n');
        atomic_write(&path, text.as_bytes())
            .map_err(|e| ExportError::write(&path, e))
            .map_err(map_export)?;
    }

    // Texture end padding: bytes between asset_offsets[4] and addr_index.
    let texture_segment_size = info.addr_index.saturating_sub(info.addr_base);
    let texture_end_offset = asset_offsets[4];
    let texture_end_padding: Vec<u8> = if texture_segment_size > texture_end_offset {
        rom.get(
            (info.addr_base + texture_end_offset) as usize
                ..(info.addr_base + texture_segment_size) as usize,
        )
        .map(|s| s.to_vec())
        .unwrap_or_default()
    } else {
        Vec::new()
    };

    let manifest = Manifest {
        label: info.label.clone(),
        subdir: info.subdir.clone(),
        kind: parse_kind(&info.kind),
        sprite_type: match info.variant {
            AddressVariant::Type1 { .. } => SpriteType::Type1,
            AddressVariant::Type2 { .. } => SpriteType::Type2,
        },
        sprite_count: sprite_count as u32,
        spritesheet_index_count: spritesheet_index_count as u32,
        palette_count: palettes_meta.len() as u32,
        palette_index_count: pal_count as u32,
        animation_count: animations.len() as u32,
        animation_index: anim_offsets,
        palette_index: palette_offsets_raw,
        palettes: palettes_meta,
        sprite_to_palette,
        palette_index_end_bytes,
        spritesheet_index_end_bytes,
        s2p_padding,
        texture_end_padding,
        asset_offsets: asset_offsets.to_vec(),
        textures: texture_metas,
    };

    write_manifest(&sprite_dir, &manifest).map_err(map_export)?;
    Ok(sprite_dir)
}

/// "Simple" path: no palette section. Write raw .ci textures and a minimal manifest.
fn finalize_simple(
    rom: &[u8],
    info: &SpriteAddress,
    sprite_dir: &Path,
    spritesheet_offsets: &[u32],
    sprite_count: usize,
    spritesheet_index_count: u32,
    spritesheet_index_end_bytes: Vec<u8>,
    asset_offsets: &[u32; 8],
) -> ExtractResult<PathBuf> {
    let mut texture_metas: Vec<TextureMeta> = Vec::new();
    for sprite_idx in 0..sprite_count {
        let Some(dec) = decode_texture(rom, info.addr_base, spritesheet_offsets, sprite_idx)
        else {
            continue;
        };
        if dec.width <= 0 || dec.height <= 0 || dec.pixels.is_empty() {
            continue;
        }
        let texture = Texture {
            index: sprite_idx as u32,
            width: dec.width as u16,
            height: dec.height as u16,
            format: dec.format,
            palette_index: 0,
            pixels: dec.pixels,
        };
        write_texture_ci(sprite_dir, &texture).map_err(map_export)?;
        texture_metas.push(TextureMeta {
            index: texture.index,
            width: texture.width,
            height: texture.height,
            format: texture.format,
            palette_index: 0,
        });
    }
    let manifest = Manifest {
        label: info.label.clone(),
        subdir: info.subdir.clone(),
        kind: parse_kind(&info.kind),
        sprite_type: match info.variant {
            AddressVariant::Type1 { .. } => SpriteType::Type1,
            AddressVariant::Type2 { .. } => SpriteType::Type2,
        },
        sprite_count: sprite_count as u32,
        spritesheet_index_count,
        palette_count: 0,
        palette_index_count: 0,
        animation_count: 0,
        animation_index: vec![],
        palette_index: vec![],
        palettes: Default::default(),
        sprite_to_palette: vec![],
        palette_index_end_bytes: vec![],
        spritesheet_index_end_bytes,
        s2p_padding: vec![],
        texture_end_padding: vec![],
        asset_offsets: asset_offsets.to_vec(),
        textures: texture_metas,
    };
    write_manifest(sprite_dir, &manifest).map_err(map_export)?;
    Ok(sprite_dir.to_path_buf())
}

fn parse_kind(s: &str) -> SpriteKind {
    match s {
        "entity" => SpriteKind::Entity,
        "avatar" => SpriteKind::Avatar,
        "standalone" => SpriteKind::Standalone,
        _ => SpriteKind::Unknown,
    }
}

fn read_u32_be(rom: &[u8], offset: u32) -> u32 {
    crate::io::binary::read_u32_be(rom, offset as usize).unwrap_or(0)
}

fn read_4_bytes(rom: &[u8], offset: u32) -> [u8; 4] {
    let mut out = [0u8; 4];
    if let Some(slice) = rom.get(offset as usize..offset as usize + 4) {
        out.copy_from_slice(slice);
    }
    out
}

fn map_export(e: ExportError) -> ExtractError {
    ExtractError::Export(e)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::extract::rom;
    use crate::io::sprite_loader::load_sprite_asset;

    fn repo_root() -> Option<PathBuf> {
        let manifest_dir = env!("CARGO_MANIFEST_DIR");
        let root = std::path::Path::new(manifest_dir).ancestors().nth(3)?.to_path_buf();
        if root.join("baserom.us.z64").exists() {
            Some(root)
        } else {
            None
        }
    }

    /// Extract bird from the ROM into a tempdir, then load both that workspace
    /// and the existing python-extracted one and compare key fields.
    #[test]
    fn extract_bird_matches_python_workspace() {
        let Some(root) = repo_root() else {
            eprintln!("skipping: ROM not present");
            return;
        };
        let python_dir = root.join("assets/sprites/entitySprites/animals/bird");
        if !python_dir.is_dir() {
            eprintln!("skipping: python-extracted bird not present");
            return;
        }

        let r = rom::load_us(&root.join("baserom.us.z64")).unwrap();
        let addresses = addresses::embedded().unwrap();
        let bird = addresses.iter().find(|a| a.label == "bird").unwrap();

        let tmp = tempfile::tempdir().unwrap();
        let _dir = extract_sprite(&r.bytes, bird, tmp.path()).unwrap();

        let rust = load_sprite_asset(&tmp.path().join("assets/sprites/entitySprites/animals/bird"))
            .unwrap();
        let py = load_sprite_asset(&python_dir).unwrap();

        // Manifest fields that drive the bin output.
        assert_eq!(rust.manifest.label, py.manifest.label);
        assert_eq!(rust.manifest.subdir, py.manifest.subdir);
        assert_eq!(rust.manifest.kind, py.manifest.kind);
        assert_eq!(rust.manifest.sprite_type, py.manifest.sprite_type);
        assert_eq!(rust.manifest.sprite_count, py.manifest.sprite_count);
        assert_eq!(rust.manifest.spritesheet_index_count, py.manifest.spritesheet_index_count);
        assert_eq!(rust.manifest.palette_count, py.manifest.palette_count);
        assert_eq!(rust.manifest.palette_index_count, py.manifest.palette_index_count);
        assert_eq!(rust.manifest.animation_count, py.manifest.animation_count);
        assert_eq!(rust.manifest.animation_index, py.manifest.animation_index);
        assert_eq!(rust.manifest.palette_index, py.manifest.palette_index);
        assert_eq!(rust.manifest.sprite_to_palette, py.manifest.sprite_to_palette);
        assert_eq!(rust.manifest.s2p_padding, py.manifest.s2p_padding);
        assert_eq!(rust.manifest.palette_index_end_bytes, py.manifest.palette_index_end_bytes);
        assert_eq!(
            rust.manifest.spritesheet_index_end_bytes,
            py.manifest.spritesheet_index_end_bytes
        );
        assert_eq!(rust.manifest.texture_end_padding, py.manifest.texture_end_padding);
        assert_eq!(rust.manifest.asset_offsets, py.manifest.asset_offsets);

        // Texture pixels should be byte-identical to the python extractor.
        for (k, py_tex) in &py.textures {
            let rust_tex = rust.textures.get(k).unwrap_or_else(|| panic!("missing texture {k}"));
            assert_eq!(rust_tex.pixels, py_tex.pixels, "pixels differ for texture {k}");
            assert_eq!(rust_tex.width, py_tex.width);
            assert_eq!(rust_tex.height, py_tex.height);
            assert_eq!(rust_tex.format, py_tex.format);
        }
        // .pal bytes must match (decoded RGBA values diverge by a few bits due
        // to 5-to-8 expansion choice — bit replication vs simple shift — but
        // both encode back to the same RGBA5551 bytes).
        let bird_dir = python_dir.clone();
        for k in py.palettes.keys() {
            let rust_pal = std::fs::read(
                tmp.path()
                    .join("assets/sprites/entitySprites/animals/bird/palettes")
                    .join(format!("{k:02}.pal")),
            )
            .unwrap();
            let py_pal = std::fs::read(bird_dir.join("palettes").join(format!("{k:02}.pal"))).unwrap();
            assert_eq!(rust_pal, py_pal, ".pal bytes differ for palette {k}");
        }
        for (k, py_anim) in &py.animations {
            let rust_anim = rust.animations.get(k).unwrap_or_else(|| panic!("missing animation {k}"));
            assert_eq!(rust_anim.frame_count, py_anim.frame_count);
            assert_eq!(rust_anim.frames, py_anim.frames);
        }

        // Round-trip: pack bins from rust workspace and compare to bird golden.
        let golden_dir = root.join("tools/modding/sprite_editor/tests/fixtures/bird-bin");
        if golden_dir.is_dir() {
            let bin_dir = tmp.path().join("rebuilt-bins");
            std::fs::create_dir_all(&bin_dir).unwrap();
            let bin = crate::export::sprite_bin::export_bins(&rust, &bin_dir).unwrap();
            for path in &bin {
                let filename = path.file_name().unwrap().to_str().unwrap();
                let actual = std::fs::read(path).unwrap();
                let golden = std::fs::read(golden_dir.join(filename)).unwrap();
                assert_eq!(actual, golden, "rust-extracted bird produces wrong bin for {filename}");
            }
        }
    }

    /// Same parity check for a type-2 sprite (balloons).
    #[test]
    fn extract_balloons_matches_python_workspace() {
        let Some(root) = repo_root() else {
            eprintln!("skipping: ROM not present");
            return;
        };
        let python_dir = root.join("assets/sprites/effects/balloons");
        if !python_dir.is_dir() {
            eprintln!("skipping: python-extracted balloons not present");
            return;
        }

        let r = rom::load_us(&root.join("baserom.us.z64")).unwrap();
        let addresses = addresses::embedded().unwrap();
        let info = addresses.iter().find(|a| a.label == "balloons").unwrap();

        let tmp = tempfile::tempdir().unwrap();
        extract_sprite(&r.bytes, info, tmp.path()).unwrap();

        let rust =
            load_sprite_asset(&tmp.path().join("assets/sprites/effects/balloons")).unwrap();
        let py = load_sprite_asset(&python_dir).unwrap();

        assert_eq!(rust.manifest.sprite_type, SpriteType::Type2);
        assert_eq!(rust.manifest.sprite_count, py.manifest.sprite_count);
        assert_eq!(rust.manifest.sprite_to_palette, py.manifest.sprite_to_palette);

        let golden_dir = root.join("tools/modding/sprite_editor/tests/fixtures/balloons-bin");
        if golden_dir.is_dir() {
            let bin_dir = tmp.path().join("rebuilt-bins");
            std::fs::create_dir_all(&bin_dir).unwrap();
            let bin = crate::export::sprite_bin::export_bins(&rust, &bin_dir).unwrap();
            for path in &bin {
                let filename = path.file_name().unwrap().to_str().unwrap();
                let actual = std::fs::read(path).unwrap();
                let golden = std::fs::read(golden_dir.join(filename)).unwrap();
                assert_eq!(
                    actual, golden,
                    "rust-extracted balloons produces wrong bin for {filename}"
                );
            }
        }
    }
}
