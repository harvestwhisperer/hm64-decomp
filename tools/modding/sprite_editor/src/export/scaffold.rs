//! Create a brand-new sprite asset folder on disk, optionally seeded from
//! imported PNG data. Does not modify `spec` or `animationScripts.c` (that's
//! the modder's responsibility).

use std::collections::BTreeMap;
use std::fs;
use std::path::PathBuf;

use crate::export::animation_data_json::{write_avatar_table, write_standalone_labels};
use crate::export::manifest_export::write_manifest;
use crate::export::palette_export::{write_palette_json, write_palette_pal};
use crate::export::texture_export::{write_texture_ci, write_texture_png};
use crate::export::{ExportError, ExportResult};
use crate::io::png_import::ImportedIndexed;
use crate::model::{
    Animation, AnimationFrame, AvatarTable, FrameSprite, Manifest, Palette, SpriteAsset,
    SpriteType, StandaloneLabels, Texture, TextureMeta,
};

pub struct NewSpriteSpec {
    pub asset_root: PathBuf,
    pub category: String, // "entitySprites" etc.
    pub name: String,     // folder + script base name
    pub kind: crate::model::SpriteKind,
    pub sprite_type: SpriteType,
    pub imports: Vec<ImportedIndexed>, // may be empty -> empty sprite
}

pub struct CreatedSprite {
    pub sprite: SpriteAsset,
}

pub fn create_new_sprite(spec: &NewSpriteSpec) -> ExportResult<CreatedSprite> {
    let sprite_dir = spec.asset_root.join("assets/sprites").join(&spec.category).join(&spec.name);
    if sprite_dir.exists() {
        return Err(ExportError::write(
            &sprite_dir,
            std::io::Error::new(
                std::io::ErrorKind::AlreadyExists,
                format!("sprite dir {} already exists", sprite_dir.display()),
            ),
        ));
    }

    match spec.kind {
        crate::model::SpriteKind::Entity | crate::model::SpriteKind::Unknown => {
            scaffold_entity(spec, &sprite_dir)
        }
        crate::model::SpriteKind::Avatar => scaffold_avatar(spec, &sprite_dir),
        crate::model::SpriteKind::Standalone => scaffold_standalone(spec, &sprite_dir),
    }
}

fn scaffold_entity(spec: &NewSpriteSpec, sprite_dir: &std::path::Path) -> ExportResult<CreatedSprite> {
    for sub in ["textures", "palettes", "animations"] {
        fs::create_dir_all(sprite_dir.join(sub))
            .map_err(|e| ExportError::write(sprite_dir.join(sub), e))?;
    }

    // Build model objects from imports. Palettes are deduplicated within the
    // batch by byte equality (same format, same Vec<Rgba>); each unique
    // palette is assigned the next contiguous palette index.
    let mut textures: BTreeMap<u32, Texture> = BTreeMap::new();
    let mut palettes: BTreeMap<u32, Palette> = BTreeMap::new();
    let mut animations: BTreeMap<u32, Animation> = BTreeMap::new();
    let mut tex_metas: Vec<TextureMeta> = Vec::new();
    let mut sprite_to_palette: Vec<u8> = Vec::with_capacity(spec.imports.len());
    let mut next_pal_idx: u32 = 0;

    for (i, imp) in spec.imports.iter().enumerate() {
        let tex_idx = i as u32;
        let pal_idx = match crate::model::find_matching_palette(&imp.palette, imp.format, &palettes)
        {
            Some(existing) => existing,
            None => {
                let idx = next_pal_idx;
                next_pal_idx += 1;
                palettes.insert(
                    idx,
                    Palette { index: idx, format: imp.format, colors: imp.palette.clone() },
                );
                idx
            }
        };
        sprite_to_palette.push(pal_idx as u8);
        let tex = Texture {
            index: tex_idx,
            width: imp.width,
            height: imp.height,
            format: imp.format,
            palette_index: pal_idx,
            pixels: imp.pixels.clone(),
        };
        tex_metas.push(TextureMeta {
            index: tex_idx,
            width: imp.width,
            height: imp.height,
            format: imp.format,
            palette_index: pal_idx,
        });
        textures.insert(tex_idx, tex);
        animations.insert(
            tex_idx,
            Animation {
                frame_count: 1,
                unused: 0,
                index: tex_idx,
                frames: vec![AnimationFrame {
                    duration: 1,
                    has_audio: 0,
                    sprites: vec![FrameSprite {
                        spritesheet_index: tex_idx,
                        anchor_x: 0,
                        anchor_y: 0,
                    }],
                }],
            },
        );
    }

    let manifest = Manifest {
        label: spec.name.clone(),
        subdir: spec.category.clone(),
        kind: spec.kind,
        sprite_type: spec.sprite_type,
        sprite_count: textures.len() as u32,
        spritesheet_index_count: textures.len() as u32,
        palette_count: palettes.len() as u32,
        palette_index_count: palettes.len() as u32,
        animation_count: animations.len() as u32,
        animation_index: (0..animations.len() as u32).collect(),
        palette_index: (0..palettes.len() as u32).collect(),
        palettes: Default::default(),
        sprite_to_palette,
        palette_index_end_bytes: vec![],
        spritesheet_index_end_bytes: vec![],
        s2p_padding: vec![],
        texture_end_padding: vec![],
        asset_offsets: vec![],
        textures: tex_metas,
    };

    // Write all asset files.
    write_manifest(sprite_dir, &manifest)?;
    for p in palettes.values() {
        write_palette_json(sprite_dir, p)?;
        write_palette_pal(sprite_dir, p)?;
    }
    for t in textures.values() {
        write_texture_ci(sprite_dir, t)?;
        if let Some(pal) = palettes.get(&t.palette_index) {
            write_texture_png(sprite_dir, t, pal)?;
        }
    }
    for a in animations.values() {
        let path = sprite_dir.join("animations").join(format!("{:02}.json", a.index));
        let text = serde_json::to_string_pretty(a).map_err(|e| ExportError::serialize(&path, e))?;
        crate::export::atomic_write(&path, text.as_bytes())
            .map_err(|e| ExportError::write(&path, e))?;
    }

    Ok(CreatedSprite {
        sprite: SpriteAsset {
            dir: sprite_dir.to_path_buf(),
            manifest,
            textures,
            palettes,
            animations,
        },
    })
}

fn empty_shell_manifest(spec: &NewSpriteSpec) -> Manifest {
    Manifest {
        label: spec.name.clone(),
        subdir: spec.category.clone(),
        kind: spec.kind,
        sprite_type: spec.sprite_type,
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

fn scaffold_avatar(spec: &NewSpriteSpec, sprite_dir: &std::path::Path) -> ExportResult<CreatedSprite> {
    fs::create_dir_all(sprite_dir).map_err(|e| ExportError::write(sprite_dir, e))?;
    let manifest = empty_shell_manifest(spec);
    write_manifest(sprite_dir, &manifest)?;
    let table = AvatarTable { kind: "avatar".into(), entries: vec![] };
    write_avatar_table(sprite_dir, &table)?;
    Ok(CreatedSprite {
        sprite: SpriteAsset {
            dir: sprite_dir.to_path_buf(),
            manifest,
            textures: BTreeMap::new(),
            palettes: BTreeMap::new(),
            animations: BTreeMap::new(),
        },
    })
}

fn scaffold_standalone(spec: &NewSpriteSpec, sprite_dir: &std::path::Path) -> ExportResult<CreatedSprite> {
    fs::create_dir_all(sprite_dir).map_err(|e| ExportError::write(sprite_dir, e))?;
    let manifest = empty_shell_manifest(spec);
    write_manifest(sprite_dir, &manifest)?;
    let labels = StandaloneLabels {
        kind: "standalone".into(),
        sprite: spec.name.clone(),
        labels: vec![],
    };
    write_standalone_labels(sprite_dir, &labels)?;
    Ok(CreatedSprite {
        sprite: SpriteAsset {
            dir: sprite_dir.to_path_buf(),
            manifest,
            textures: BTreeMap::new(),
            palettes: BTreeMap::new(),
            animations: BTreeMap::new(),
        },
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{Rgba, TextureFormat};
    use tempfile::tempdir;

    #[test]
    fn scaffolds_empty_sprite() {
        let dir = tempdir().unwrap();
        let root = dir.path().to_path_buf();
        let spec = NewSpriteSpec {
            asset_root: root.clone(),
            category: "entitySprites".into(),
            name: "testmob".into(),
            kind: crate::model::SpriteKind::Entity,
            sprite_type: SpriteType::Type1,
            imports: vec![],
        };
        let created = create_new_sprite(&spec).unwrap();
        assert_eq!(created.sprite.manifest.sprite_count, 0);
        assert!(root.join("assets/sprites/entitySprites/testmob/manifest.json").is_file());
    }

    #[test]
    fn scaffolds_two_png_imports_with_identical_palettes_dedup() {
        let dir = tempdir().unwrap();
        let root = dir.path().to_path_buf();

        // Both imports use Rgba::default() -> identical 256-entry zero palette.
        let make_import = || ImportedIndexed {
            width: 4,
            height: 4,
            format: TextureFormat::Ci8,
            pixels: vec![0u8; 16],
            palette: vec![Rgba::default(); 256],
        };

        let spec = NewSpriteSpec {
            asset_root: root.clone(),
            category: "entitySprites".into(),
            name: "twomobs".into(),
            kind: crate::model::SpriteKind::Entity,
            sprite_type: SpriteType::Type1,
            imports: vec![make_import(), make_import()],
        };
        let created = create_new_sprite(&spec).unwrap();

        assert_eq!(created.sprite.manifest.sprite_count, 2);
        assert_eq!(created.sprite.manifest.palette_count, 1, "palettes must dedup");
        assert_eq!(created.sprite.manifest.palette_index, vec![0]);
        assert_eq!(created.sprite.manifest.animation_count, 2);
        assert_eq!(created.sprite.manifest.sprite_to_palette, vec![0u8, 0u8]);
        assert_eq!(created.sprite.manifest.textures[0].palette_index, 0);
        assert_eq!(created.sprite.manifest.textures[1].palette_index, 0);

        let base = root.join("assets/sprites/entitySprites/twomobs");
        assert!(base.join("textures/000.ci").is_file());
        assert!(base.join("textures/001.ci").is_file());
        // Only the deduplicated palette is on disk.
        assert!(base.join("palettes/00.json").is_file());
        assert!(!base.join("palettes/01.json").exists(), "no second palette should be written");
        // Animations stay 1:1 with imports.
        assert!(base.join("animations/00.json").is_file());
        assert!(base.join("animations/01.json").is_file());
    }

    #[test]
    fn scaffolds_two_png_imports_with_distinct_palettes_no_dedup() {
        let dir = tempdir().unwrap();
        let root = dir.path().to_path_buf();

        // Two imports with distinct palettes (differ in entry 0).
        let mut pal_a = vec![Rgba::default(); 256];
        pal_a[0] = Rgba::new(255, 0, 0, 255);
        let mut pal_b = vec![Rgba::default(); 256];
        pal_b[0] = Rgba::new(0, 255, 0, 255);

        let spec = NewSpriteSpec {
            asset_root: root.clone(),
            category: "entitySprites".into(),
            name: "twodistinct".into(),
            kind: crate::model::SpriteKind::Entity,
            sprite_type: SpriteType::Type1,
            imports: vec![
                ImportedIndexed {
                    width: 4,
                    height: 4,
                    format: TextureFormat::Ci8,
                    pixels: vec![0u8; 16],
                    palette: pal_a,
                },
                ImportedIndexed {
                    width: 4,
                    height: 4,
                    format: TextureFormat::Ci8,
                    pixels: vec![0u8; 16],
                    palette: pal_b,
                },
            ],
        };
        let created = create_new_sprite(&spec).unwrap();

        assert_eq!(created.sprite.manifest.sprite_count, 2);
        assert_eq!(created.sprite.manifest.palette_count, 2);
        assert_eq!(created.sprite.manifest.palette_index, vec![0, 1]);
        assert_eq!(created.sprite.manifest.sprite_to_palette, vec![0u8, 1u8]);
        assert_eq!(created.sprite.manifest.textures[0].palette_index, 0);
        assert_eq!(created.sprite.manifest.textures[1].palette_index, 1);

        let base = root.join("assets/sprites/entitySprites/twodistinct");
        assert!(base.join("palettes/00.json").is_file());
        assert!(base.join("palettes/01.json").is_file());
    }

    #[test]
    fn scaffolds_avatar_sprite_writes_empty_character_avatars_json() {
        let dir = tempdir().unwrap();
        let root = dir.path().to_path_buf();
        let spec = NewSpriteSpec {
            asset_root: root.clone(),
            category: "overlayScreens".into(),
            name: "modAvatars".into(),
            kind: crate::model::SpriteKind::Avatar,
            sprite_type: SpriteType::Type1,
            imports: vec![],
        };
        let created = create_new_sprite(&spec).unwrap();

        assert_eq!(created.sprite.manifest.kind, crate::model::SpriteKind::Avatar);
        assert_eq!(created.sprite.manifest.sprite_count, 0);
        assert_eq!(created.sprite.manifest.palette_count, 0);
        assert_eq!(created.sprite.manifest.animation_count, 0);

        let base = root.join("assets/sprites/overlayScreens/modAvatars");
        let json_path = base.join("characterAvatars.json");
        assert!(json_path.is_file(), "characterAvatars.json should exist at sprite root");

        let text = std::fs::read_to_string(&json_path).unwrap();
        assert!(text.contains("\"kind\": \"avatar\""), "got: {text}");
        assert!(text.contains("\"entries\": []"), "got: {text}");

        assert!(!base.join("textures").exists(), "no textures dir for Avatar kind");
        assert!(!base.join("palettes").exists(), "no palettes dir for Avatar kind");
        assert!(!base.join("animations").exists(), "no animations dir for Avatar kind");
    }

    #[test]
    fn scaffolds_standalone_sprite_writes_empty_animation_labels_json() {
        let dir = tempdir().unwrap();
        let root = dir.path().to_path_buf();
        let spec = NewSpriteSpec {
            asset_root: root.clone(),
            category: "entitySprites".into(),
            name: "balloonsMod".into(),
            kind: crate::model::SpriteKind::Standalone,
            sprite_type: SpriteType::Type1,
            imports: vec![],
        };
        let created = create_new_sprite(&spec).unwrap();

        assert_eq!(created.sprite.manifest.kind, crate::model::SpriteKind::Standalone);

        let base = root.join("assets/sprites/entitySprites/balloonsMod");
        let json_path = base.join("animationLabels.json");
        assert!(json_path.is_file(), "animationLabels.json should exist at sprite root");

        let text = std::fs::read_to_string(&json_path).unwrap();
        assert!(text.contains("\"kind\": \"standalone\""), "got: {text}");
        assert!(text.contains("\"sprite\": \"balloonsMod\""), "got: {text}");
        assert!(text.contains("\"labels\": []"), "got: {text}");

        assert!(!base.join("textures").exists());
        assert!(!base.join("palettes").exists());
        assert!(!base.join("animations").exists());
    }

    #[test]
    fn scaffolds_three_png_imports_mixed_palettes() {
        let dir = tempdir().unwrap();
        let root = dir.path().to_path_buf();

        // Palettes A, B, A (third reuses first).
        let mut pal_a = vec![Rgba::default(); 256];
        pal_a[0] = Rgba::new(255, 0, 0, 255);
        let mut pal_b = vec![Rgba::default(); 256];
        pal_b[0] = Rgba::new(0, 255, 0, 255);

        let make = |palette: Vec<Rgba>| ImportedIndexed {
            width: 4,
            height: 4,
            format: TextureFormat::Ci8,
            pixels: vec![0u8; 16],
            palette,
        };

        let spec = NewSpriteSpec {
            asset_root: root.clone(),
            category: "entitySprites".into(),
            name: "threemixed".into(),
            kind: crate::model::SpriteKind::Entity,
            sprite_type: SpriteType::Type1,
            imports: vec![make(pal_a.clone()), make(pal_b), make(pal_a)],
        };
        let created = create_new_sprite(&spec).unwrap();

        assert_eq!(created.sprite.manifest.sprite_count, 3);
        assert_eq!(created.sprite.manifest.palette_count, 2);
        assert_eq!(created.sprite.manifest.palette_index, vec![0, 1]);
        assert_eq!(created.sprite.manifest.sprite_to_palette, vec![0u8, 1u8, 0u8]);
        assert_eq!(created.sprite.manifest.textures[0].palette_index, 0);
        assert_eq!(created.sprite.manifest.textures[1].palette_index, 1);
        assert_eq!(created.sprite.manifest.textures[2].palette_index, 0);
    }
}
