//! End-to-end test: scaffold a parallel Avatar sprite, load it, run the
//! Add/Remove commands through the Editor (apply + undo + redo), and confirm
//! the on-disk JSON round-trips.

use hm64_sprite_editor::cmd::animation_data::{AddAvatarEntry, RemoveAvatarEntry};
use hm64_sprite_editor::export::animation_data_json::save_animation_data;
use hm64_sprite_editor::export::scaffold::{create_new_sprite, NewSpriteSpec};
use hm64_sprite_editor::io::animation_data_json::load;
use hm64_sprite_editor::model::{AnimationData, AvatarEntry, SpriteKind, SpriteType};
use hm64_sprite_editor::state::editor::Editor;
use tempfile::tempdir;

#[test]
fn scaffold_then_add_remove_undo_redo_round_trips() {
    let dir = tempdir().unwrap();
    let root = dir.path().to_path_buf();

    let spec = NewSpriteSpec {
        asset_root: root.clone(),
        category: "overlayScreens".into(),
        name: "modAvatars".into(),
        kind: SpriteKind::Avatar,
        sprite_type: SpriteType::Type1,
        imports: vec![],
    };
    let created = create_new_sprite(&spec).unwrap();
    let sprite_dir = created.sprite.dir.clone();

    // Load animation data from disk.
    let data = load(&sprite_dir, SpriteKind::Avatar).unwrap().unwrap();
    let table = match &data {
        AnimationData::Avatar(t) => t.clone(),
        _ => panic!("expected Avatar"),
    };
    assert!(table.entries.is_empty(), "fresh avatar must start with no entries");

    // Build an Editor around the loaded data.
    let mut ed = Editor::new();
    ed.loaded_animation_data = Some(data);

    // Add an entry.
    let entry = AvatarEntry {
        character: "MARIA".into(),
        expression: "HAPPY".into(),
        metadata_index: 7,
    };
    let add = AddAvatarEntry::new(0, entry.clone(), &ed).unwrap();
    ed.run(Box::new(add)).unwrap();
    assert_eq!(avatar_len(&ed), 1);

    // Undo, then redo.
    assert!(ed.undo().unwrap().is_some());
    assert_eq!(avatar_len(&ed), 0);
    assert!(ed.redo().unwrap().is_some());
    assert_eq!(avatar_len(&ed), 1);

    // Remove the entry.
    let rem = RemoveAvatarEntry::new(0, &ed).unwrap();
    ed.run(Box::new(rem)).unwrap();
    assert_eq!(avatar_len(&ed), 0);

    // Undo the remove — entry comes back identical.
    assert!(ed.undo().unwrap().is_some());
    assert_eq!(avatar_len(&ed), 1);
    match ed.loaded_animation_data.as_ref().unwrap() {
        AnimationData::Avatar(t) => assert_eq!(t.entries[0], entry),
        _ => panic!("expected Avatar"),
    }

    // Save to disk and reload — JSON round-trips with the single entry.
    save_animation_data(&sprite_dir, ed.loaded_animation_data.as_ref().unwrap()).unwrap();
    let reloaded = load(&sprite_dir, SpriteKind::Avatar).unwrap().unwrap();
    match reloaded {
        AnimationData::Avatar(t) => {
            assert_eq!(t.entries.len(), 1);
            assert_eq!(t.entries[0], entry);
        }
        _ => panic!("expected Avatar"),
    }
}

fn avatar_len(ed: &Editor) -> usize {
    match ed.loaded_animation_data.as_ref().unwrap() {
        AnimationData::Avatar(t) => t.entries.len(),
        _ => panic!("expected Avatar"),
    }
}
