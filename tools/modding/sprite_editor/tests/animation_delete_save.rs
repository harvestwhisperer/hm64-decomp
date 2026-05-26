//! End-to-end: pre-create two animation files on disk, delete one
//! through the editor, save, and confirm only the kept one remains.

use std::fs;
use std::path::PathBuf;

use hm64_sprite_editor::cmd::animation_cmds::DeleteAnimation;
use hm64_sprite_editor::cmd::DirtyMask;
use hm64_sprite_editor::export::animation_export::{delete_animation_file, write_animation};
use hm64_sprite_editor::export::manifest_export::write_manifest;
use hm64_sprite_editor::model::{
    Animation, AnimationFrame, FrameSprite, Manifest, SpriteKind, SpriteType, TextureFormat,
    TextureMeta,
};
use hm64_sprite_editor::state::editor::Editor;

fn write_minimal_sprite_with_two_anims(root: &std::path::Path) -> PathBuf {
    let dir = root.join("assets/sprites/entitySprites/test");
    fs::create_dir_all(dir.join("textures")).unwrap();
    fs::create_dir_all(dir.join("palettes")).unwrap();
    fs::create_dir_all(dir.join("animations")).unwrap();

    // Palette JSON in the format the loader expects.
    let palette_json = serde_json::json!({
        "color_count": 16,
        "format": "ci4",
        "colors": (0u32..16).map(|i| serde_json::json!({
            "index": i, "hex": "#000000", "alpha": 255, "rgba": [0, 0, 0, 255]
        })).collect::<Vec<_>>(),
    });
    fs::write(
        dir.join("palettes/00.json"),
        serde_json::to_string_pretty(&palette_json).unwrap(),
    )
    .unwrap();
    fs::write(dir.join("palettes/00.pal"), vec![0u8; 32]).unwrap();
    fs::write(dir.join("textures/000.ci"), vec![0u8; 8]).unwrap();

    // Manifest: two animations, use the actual writer for correct JSON shape.
    let manifest = Manifest {
        label: "test".into(),
        subdir: "entitySprites".into(),
        kind: SpriteKind::Entity,
        sprite_type: SpriteType::Type1,
        sprite_count: 1,
        spritesheet_index_count: 1,
        palette_count: 1,
        palette_index_count: 1,
        animation_count: 2,
        animation_index: vec![0, 1],
        palette_index: vec![0],
        palettes: Default::default(),
        sprite_to_palette: vec![],
        palette_index_end_bytes: vec![],
        spritesheet_index_end_bytes: vec![],
        s2p_padding: vec![],
        texture_end_padding: vec![],
        asset_offsets: vec![],
        textures: vec![TextureMeta {
            index: 0,
            width: 4,
            height: 4,
            format: TextureFormat::Ci4,
            palette_index: 0,
        }],
    };
    write_manifest(&dir, &manifest).unwrap();

    let make_anim = |idx: u32| Animation {
        frame_count: 1,
        unused: 0,
        index: idx,
        frames: vec![AnimationFrame {
            duration: 1,
            has_audio: 0,
            sprites: vec![FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 }],
        }],
    };
    write_animation(&dir, &make_anim(0)).unwrap();
    write_animation(&dir, &make_anim(1)).unwrap();

    dir
}

#[test]
fn delete_then_save_removes_orphan_file() {
    let tmp = tempfile::tempdir().unwrap();
    let sprite_dir = write_minimal_sprite_with_two_anims(tmp.path());
    assert!(sprite_dir.join("animations/00.json").is_file());
    assert!(sprite_dir.join("animations/01.json").is_file());

    let mut editor = Editor::new();
    editor.load_sprite(&sprite_dir).unwrap();
    assert_eq!(editor.current_sprite().unwrap().animations.len(), 2);

    let cmd = DeleteAnimation::new(0, &editor).unwrap();
    editor.run(Box::new(cmd)).unwrap();
    assert!(editor.dirty.animation());

    // Save: write live animations, delete orphans (anim 0).
    let s = editor.loaded_sprite.as_ref().unwrap();
    let dir = s.dir.clone();
    let live: std::collections::HashSet<u32> = s.animations.keys().copied().collect();
    for a in s.animations.values() {
        write_animation(&dir, a).unwrap();
    }
    let entries: Vec<_> = fs::read_dir(dir.join("animations"))
        .unwrap()
        .filter_map(|e| e.ok())
        .filter(|e| e.file_name().to_str().map_or(false, |n| n.ends_with(".json")))
        .collect();
    for entry in entries {
        let name = entry.file_name();
        let stem = &name.to_str().unwrap()[..name.to_str().unwrap().len() - 5];
        let idx: u32 = stem.parse().unwrap();
        if !live.contains(&idx) {
            delete_animation_file(&dir, idx).unwrap();
        }
    }
    editor.mark_saved(DirtyMask::ANIMATION);

    assert!(!sprite_dir.join("animations/00.json").exists(), "orphan must be removed");
    assert!(sprite_dir.join("animations/01.json").is_file(), "live file must remain");

    // Reload to confirm only one animation present.
    let mut editor2 = Editor::new();
    editor2.load_sprite(&sprite_dir).unwrap();
    let s = editor2.current_sprite().unwrap();
    assert_eq!(s.animations.len(), 1);
    assert!(s.animations.contains_key(&1));
}
