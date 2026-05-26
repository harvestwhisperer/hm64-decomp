//! End-to-end: create an animation in-editor, edit it, save, reload,
//! and assert the persisted state matches.

use std::fs;
use std::path::PathBuf;

use hm64_sprite_editor::cmd::animation_cmds::{
    AddFrame, AddLayer, CreateAnimation, SetLayerAnchor, SetLayerTexture,
};
use hm64_sprite_editor::cmd::DirtyMask;
use hm64_sprite_editor::export::animation_export::write_animation;
use hm64_sprite_editor::export::manifest_export::write_manifest;
use hm64_sprite_editor::model::{
    Animation, AnimationFrame, FrameSprite, Manifest, SpriteKind, SpriteType, TextureFormat,
    TextureMeta,
};
use hm64_sprite_editor::state::editor::Editor;

fn write_minimal_sprite(root: &std::path::Path) -> PathBuf {
    let dir = root.join("assets/sprites/entitySprites/test");
    fs::create_dir_all(dir.join("textures")).unwrap();
    fs::create_dir_all(dir.join("palettes")).unwrap();
    fs::create_dir_all(dir.join("animations")).unwrap();

    // Palette JSON: written in the format the loader expects.
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

    // Texture: 4x4 CI4 raw pixels (w*h/2 = 8 bytes).
    fs::write(dir.join("textures/000.ci"), vec![0u8; 8]).unwrap();

    // Manifest: use the actual writer so the JSON shape exactly matches the loader.
    let manifest = Manifest {
        label: "test".into(),
        subdir: "entitySprites".into(),
        kind: SpriteKind::Entity,
        sprite_type: SpriteType::Type1,
        sprite_count: 1,
        spritesheet_index_count: 1,
        palette_count: 1,
        palette_index_count: 1,
        animation_count: 0,
        animation_index: vec![],
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

    dir
}

fn save_all_animations(editor: &mut Editor) {
    let s = editor.loaded_sprite.as_ref().unwrap();
    let dir = s.dir.clone();
    for a in s.animations.values() {
        write_animation(&dir, a).unwrap();
    }
    editor.mark_saved(DirtyMask::ANIMATION);
}

#[test]
fn create_edit_save_reload_round_trip() {
    let tmp = tempfile::tempdir().unwrap();
    let sprite_dir = write_minimal_sprite(tmp.path());

    // Load sprite.
    let mut editor = Editor::new();
    editor.load_sprite(&sprite_dir).unwrap();

    // CreateAnimation #0 (empty).
    let anim = Animation { frame_count: 0, unused: 0, index: 0, frames: vec![] };
    let cmd = CreateAnimation::new(0, anim, &editor).unwrap();
    editor.run(Box::new(cmd)).unwrap();

    // AddFrame (empty).
    let frame = AnimationFrame { duration: 5, has_audio: 0, sprites: vec![] };
    let cmd = AddFrame::new(0, 0, frame, &editor).unwrap();
    editor.run(Box::new(cmd)).unwrap();

    // AddLayer.
    let layer = FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 };
    let cmd = AddLayer::new(0, 0, 0, layer, &editor).unwrap();
    editor.run(Box::new(cmd)).unwrap();

    // SetLayerTexture (no-op since only texture is index 0; still exercises path).
    let cmd = SetLayerTexture::new(0, 0, 0, 0, &editor).unwrap();
    editor.run(Box::new(cmd)).unwrap();

    // SetLayerAnchor.
    let cmd = SetLayerAnchor::new(0, 0, 0, (-7, 11), &editor).unwrap();
    editor.run(Box::new(cmd)).unwrap();

    assert!(editor.dirty.animation(), "edits should mark animation dirty");

    // Save.
    save_all_animations(&mut editor);
    assert!(!editor.dirty.animation(), "after save the bit should clear");
    assert!(sprite_dir.join("animations/00.json").is_file());

    // Reload.
    let mut editor2 = Editor::new();
    editor2.load_sprite(&sprite_dir).unwrap();
    let s = editor2.current_sprite().unwrap();
    assert_eq!(s.animations.len(), 1);
    let a = &s.animations[&0];
    assert_eq!(a.index, 0);
    assert_eq!(a.frames.len(), 1);
    assert_eq!(a.frames[0].duration, 5);
    assert_eq!(a.frames[0].sprites.len(), 1);
    assert_eq!(a.frames[0].sprites[0].spritesheet_index, 0);
    assert_eq!(a.frames[0].sprites[0].anchor_x, -7);
    assert_eq!(a.frames[0].sprites[0].anchor_y, 11);
}
