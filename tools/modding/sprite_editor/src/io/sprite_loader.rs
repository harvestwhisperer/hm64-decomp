//! Top-level read path: assembles a full `SpriteAsset` from a directory.
//!
//! File I/O is parallelized across worker threads using `std::thread::scope`.
//! Without this, loading a large sprite (e.g. player with 1,560 textures +
//! 396 animations + 80 palettes = ~2,036 file reads) blocks the UI for
//! 10+ seconds on Windows.

use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use std::sync::Mutex;

use crate::io::{
    animation_json::read_animation, manifest::read_manifest, palette_json::read_palette,
    texture_ci::read_texture, IoError, IoResult,
};
use crate::model::{Animation, Manifest, Palette, SpriteAsset, Texture};

pub fn load_sprite_asset(dir: &Path) -> IoResult<SpriteAsset> {
    let manifest_path = dir.join("manifest.json");
    let manifest = read_manifest(&manifest_path)?;

    let textures_dir = dir.join("textures");
    let palettes_dir = dir.join("palettes");
    let animations_dir = dir.join("animations");

    // Enumerate palette / animation JSON filenames up front (small dir reads,
    // single-threaded). The per-file reads below are what we parallelize.
    let palette_indices = enumerate_json_indices(&palettes_dir)?;
    let animation_indices = enumerate_json_indices(&animations_dir)?;

    // Three parallel groups — textures dominate by count, so give them their
    // own parallel pool; palettes and animations are small enough to share a
    // pool but it's cleaner to run all three concurrently via `scope`.
    let (textures, palettes, animations) = std::thread::scope(|scope| {
        let tex_handle =
            scope.spawn(|| parallel_read_textures(&textures_dir, &manifest));
        let pal_handle =
            scope.spawn(|| parallel_read_palettes(&palettes_dir, &palette_indices));
        let anim_handle =
            scope.spawn(|| parallel_read_animations(&animations_dir, &animation_indices));

        let textures = tex_handle.join().expect("texture read panicked")?;
        let palettes = pal_handle.join().expect("palette read panicked")?;
        let animations = anim_handle.join().expect("animation read panicked")?;
        Ok::<_, IoError>((textures, palettes, animations))
    })?;

    Ok(SpriteAsset { dir: dir.to_path_buf(), manifest, textures, palettes, animations })
}

fn enumerate_json_indices(dir: &Path) -> IoResult<Vec<u32>> {
    if !dir.is_dir() {
        return Ok(Vec::new());
    }
    let mut out: Vec<u32> = std::fs::read_dir(dir)
        .into_iter()
        .flatten()
        .flatten()
        .filter_map(|e| {
            let p = e.path();
            if p.extension()?.to_str()? != "json" {
                return None;
            }
            p.file_stem()?.to_str()?.parse::<u32>().ok()
        })
        .collect();
    out.sort_unstable();
    Ok(out)
}

fn worker_count(items: usize) -> usize {
    let avail =
        std::thread::available_parallelism().map(|n| n.get()).unwrap_or(4).min(16);
    avail.min(items.max(1))
}

fn parallel_read_textures(
    dir: &Path,
    manifest: &Manifest,
) -> IoResult<BTreeMap<u32, Texture>> {
    let metas = &manifest.textures;
    if metas.is_empty() {
        return Ok(BTreeMap::new());
    }
    let n = worker_count(metas.len());
    let out: Mutex<BTreeMap<u32, Texture>> = Mutex::new(BTreeMap::new());
    let first_err: Mutex<Option<IoError>> = Mutex::new(None);

    std::thread::scope(|scope| {
        for chunk in metas.chunks(metas.len().div_ceil(n)) {
            let out = &out;
            let first_err = &first_err;
            scope.spawn(move || {
                for meta in chunk {
                    if first_err.lock().unwrap().is_some() {
                        return;
                    }
                    let ci_path = dir.join(format!("{:03}.ci", meta.index));
                    match read_texture(
                        &ci_path,
                        meta.index,
                        meta.palette_index,
                        meta.width,
                        meta.height,
                        meta.format,
                    ) {
                        Ok(t) => {
                            out.lock().unwrap().insert(meta.index, t);
                        }
                        Err(e) => {
                            let mut slot = first_err.lock().unwrap();
                            if slot.is_none() {
                                *slot = Some(e);
                            }
                            return;
                        }
                    }
                }
            });
        }
    });

    if let Some(e) = first_err.into_inner().unwrap() {
        return Err(e);
    }
    Ok(out.into_inner().unwrap())
}

fn parallel_read_palettes(
    dir: &Path,
    indices: &[u32],
) -> IoResult<BTreeMap<u32, Palette>> {
    parallel_read_indexed(dir, indices, |path, idx| read_palette(path, idx))
}

fn parallel_read_animations(
    dir: &Path,
    indices: &[u32],
) -> IoResult<BTreeMap<u32, Animation>> {
    parallel_read_indexed(dir, indices, |path, _idx| read_animation(path))
}

fn parallel_read_indexed<T, F>(
    dir: &Path,
    indices: &[u32],
    read_fn: F,
) -> IoResult<BTreeMap<u32, T>>
where
    T: Send + 'static,
    F: Fn(&Path, u32) -> IoResult<T> + Sync,
{
    if indices.is_empty() {
        return Ok(BTreeMap::new());
    }
    let n = worker_count(indices.len());
    let out: Mutex<BTreeMap<u32, T>> = Mutex::new(BTreeMap::new());
    let first_err: Mutex<Option<IoError>> = Mutex::new(None);

    std::thread::scope(|scope| {
        for chunk in indices.chunks(indices.len().div_ceil(n)) {
            let out = &out;
            let first_err = &first_err;
            let read_fn = &read_fn;
            scope.spawn(move || {
                for &i in chunk {
                    if first_err.lock().unwrap().is_some() {
                        return;
                    }
                    let path: PathBuf = dir.join(format!("{i:02}.json"));
                    match read_fn(&path, i) {
                        Ok(v) => {
                            out.lock().unwrap().insert(i, v);
                        }
                        Err(e) => {
                            let mut slot = first_err.lock().unwrap();
                            if slot.is_none() {
                                *slot = Some(e);
                            }
                            return;
                        }
                    }
                }
            });
        }
    });

    if let Some(e) = first_err.into_inner().unwrap() {
        return Err(e);
    }
    Ok(out.into_inner().unwrap())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::env;

    /// Skip if the repo's extracted asset tree isn't present (CI on a fresh
    /// checkout won't have it).
    fn repo_root() -> Option<std::path::PathBuf> {
        let manifest_dir = env!("CARGO_MANIFEST_DIR");
        // tools/modding/sprite_editor/ → repo root
        let root = Path::new(manifest_dir).ancestors().nth(3)?.to_path_buf();
        if root.join("assets/sprites").is_dir() {
            Some(root)
        } else {
            None
        }
    }

    #[test]
    fn loads_player_if_extracted() {
        let Some(root) = repo_root() else {
            eprintln!("skipping: assets/sprites not present");
            return;
        };
        let dir = root.join("assets/sprites/entitySprites/player");
        if !dir.is_dir() {
            eprintln!("skipping: player sprite not extracted");
            return;
        }
        let s = load_sprite_asset(&dir).unwrap();
        assert_eq!(s.manifest.label, "player");
        assert!(!s.textures.is_empty());
        assert!(!s.palettes.is_empty());
        assert!(!s.animations.is_empty());
    }
}
