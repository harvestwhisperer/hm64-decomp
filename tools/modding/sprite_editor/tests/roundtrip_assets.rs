//! Walks `assets/sprites/**/` and asserts every manifest + palette +
//! animation + texture file reads successfully via `load_sprite_asset`.

use std::path::{Path, PathBuf};

fn repo_root() -> Option<PathBuf> {
    let manifest_dir = env!("CARGO_MANIFEST_DIR");
    Some(PathBuf::from(manifest_dir).ancestors().nth(3)?.to_path_buf())
}

fn walk_sprites(root: &Path) -> Vec<PathBuf> {
    let mut out = Vec::new();
    let Ok(entries) = std::fs::read_dir(root) else {
        return out;
    };
    for cat in entries.flatten() {
        let cat_path = cat.path();
        if !cat_path.is_dir() {
            continue;
        }
        // Category: entitySprites, mapObjects, effects, overlayScreens
        // Each contains sprite dirs directly or nested subdirs.
        walk_inner(&cat_path, &mut out);
    }
    out
}

fn walk_inner(dir: &Path, out: &mut Vec<PathBuf>) {
    if dir.join("manifest.json").is_file() {
        out.push(dir.to_path_buf());
        return;
    }
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for e in entries.flatten() {
        let p = e.path();
        if p.is_dir() {
            walk_inner(&p, out);
        }
    }
}

#[test]
fn all_sprites_load() {
    let Some(root) = repo_root() else {
        eprintln!("skipping: repo root not detected");
        return;
    };
    let sprites_root = root.join("assets/sprites");
    if !sprites_root.is_dir() {
        eprintln!("skipping: assets/sprites not present — run `make extract-sprites`");
        return;
    }

    let sprite_dirs = walk_sprites(&sprites_root);
    if sprite_dirs.is_empty() {
        eprintln!(
            "skipping: no sprite directories with manifest.json found under {} \
             (extracted sprites are gitignored; run from a checkout where they are extracted)",
            sprites_root.display()
        );
        return;
    }

    let mut failures = Vec::new();
    for dir in &sprite_dirs {
        match hm64_sprite_editor::io::sprite_loader::load_sprite_asset(dir) {
            Ok(_) => {}
            Err(e) => failures.push(format!("{}: {e}", dir.display())),
        }
    }

    if !failures.is_empty() {
        panic!(
            "load failures ({} of {}):\n{}",
            failures.len(),
            sprite_dirs.len(),
            failures.join("\n")
        );
    }
    eprintln!("loaded {} sprites", sprite_dirs.len());
}

#[test]
fn delete_texture_data_round_trip_on_player() {
    use hm64_sprite_editor::cmd::texture_cmds::DeleteTextureData;
    use hm64_sprite_editor::export::{
        manifest_export::write_manifest,
        texture_export::{write_texture_ci, write_texture_png},
    };
    use hm64_sprite_editor::io::sprite_loader::load_sprite_asset;
    use hm64_sprite_editor::state::editor::Editor;

    let Some(root) = repo_root() else { return; };
    let player_dir = root.join("assets/sprites/entitySprites/player");
    if !player_dir.is_dir() {
        eprintln!("skipping: assets/sprites/entitySprites/player not extracted");
        return;
    }
    if !player_dir.join("textures/290.ci").exists() {
        eprintln!("skipping: player texture 290 missing — run make extract-sprites");
        return;
    }

    // Stage a copy under tempdir so we don't mutate the working tree.
    let tmp = tempfile::tempdir().expect("tempdir");
    let staged = tmp.path().join("player");
    copy_dir_recursive(&player_dir, &staged).expect("copy player dir");

    let asset = load_sprite_asset(&staged).expect("load");
    let original_sprite_count = asset.manifest.sprite_count;
    let original_index_count = asset.manifest.spritesheet_index_count;

    let mut editor = Editor::new();
    editor.loaded_sprite = Some(asset);
    editor.run(Box::new(DeleteTextureData::new(290))).expect("apply");

    // Save: write each remaining texture, write manifest, clean stale files.
    let s = editor.loaded_sprite.as_ref().unwrap();
    for t in s.textures.values() {
        write_texture_ci(&staged, t).expect("ci");
        if let Some(pal) = s.palettes.get(&t.palette_index) {
            write_texture_png(&staged, t, pal).expect("png");
        }
    }
    write_manifest(&staged, &s.manifest).expect("manifest");
    let live: std::collections::HashSet<u32> = s.textures.keys().copied().collect();
    for entry in std::fs::read_dir(staged.join("textures")).unwrap().flatten() {
        let p = entry.path();
        let Some(stem) = p.file_stem().and_then(|s| s.to_str()) else { continue; };
        let Ok(idx) = stem.parse::<u32>() else { continue; };
        if !live.contains(&idx) {
            let ext = p.extension().and_then(|e| e.to_str()).unwrap_or("");
            if ext == "ci" || ext == "png" {
                std::fs::remove_file(&p).ok();
            }
        }
    }

    // On-disk asserts.
    assert!(!staged.join("textures/290.ci").exists());
    assert!(!staged.join("textures/290.png").exists());

    // Re-parse and assert manifest shape.
    let after = load_sprite_asset(&staged).expect("reload");
    assert!(!after.manifest.textures.iter().any(|m| m.index == 290));
    assert_eq!(after.manifest.sprite_count, original_sprite_count);
    assert_eq!(after.manifest.spritesheet_index_count, original_index_count);
    assert_eq!(after.manifest.sprite_to_palette[290], 0);
}

fn copy_dir_recursive(src: &Path, dst: &Path) -> std::io::Result<()> {
    std::fs::create_dir_all(dst)?;
    for entry in std::fs::read_dir(src)? {
        let entry = entry?;
        let from = entry.path();
        let to = dst.join(entry.file_name());
        if entry.file_type()?.is_dir() {
            copy_dir_recursive(&from, &to)?;
        } else {
            std::fs::copy(&from, &to)?;
        }
    }
    Ok(())
}
