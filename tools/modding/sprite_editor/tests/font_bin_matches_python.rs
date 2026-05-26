//! Byte-equality regression: our Rust font-bin export matches the Python
//! assembler. Skips silently if `assets/font/` isn't extracted or if `python3`
//! isn't on PATH.

use std::collections::BTreeSet;
use std::path::Path;
use std::process::Command;

use hm64_sprite_editor::export::font_bin_export::{write_font_bins, BinExportOpts};
use hm64_sprite_editor::io::font_loader::load_font_asset;

fn repo_root() -> std::path::PathBuf {
    // CARGO_MANIFEST_DIR = tools/modding/sprite_editor
    let manifest_dir = env!("CARGO_MANIFEST_DIR");
    Path::new(manifest_dir).ancestors().nth(3).unwrap().to_path_buf()
}

fn run_python_assembler(repo: &Path, out_dir: &Path) -> Option<()> {
    // python3 -m libhm64.fonts.assembler --output-dir <out_dir>
    let status = Command::new("python3")
        .arg("-m")
        .arg("libhm64.fonts.assembler")
        .arg("--output-dir")
        .arg(out_dir)
        .env("PYTHONPATH", repo.join("tools"))
        .current_dir(repo)
        .status()
        .ok()?;
    if !status.success() {
        return None;
    }
    Some(())
}

#[test]
fn rust_bins_match_python_bins() {
    let repo = repo_root();
    let assets_font = repo.join("assets/font");
    if !assets_font.join("fontTexture.ci2").is_file() {
        eprintln!("skipping: assets/font not extracted");
        return;
    }
    if Command::new("python3").arg("--version").status().is_err() {
        eprintln!("skipping: python3 not on PATH");
        return;
    }

    let asset = load_font_asset(&assets_font).expect("load font");

    let tmp_rust = tempfile::tempdir().unwrap();
    let mut palettes = BTreeSet::new();
    palettes.insert(1u32);
    palettes.insert(2u32);
    palettes.insert(3u32);
    write_font_bins(
        tmp_rust.path(),
        &asset,
        &BinExportOpts { texture: true, palettes },
    )
    .expect("rust write");

    let tmp_py = tempfile::tempdir().unwrap();
    if run_python_assembler(&repo, tmp_py.path()).is_none() {
        eprintln!("skipping: python assembler did not run cleanly");
        return;
    }

    for name in ["fontTexture.bin", "fontPalette1.bin", "fontPalette2.bin", "fontPalette3.bin"] {
        let rust = std::fs::read(tmp_rust.path().join(name)).expect(name);
        let py = std::fs::read(tmp_py.path().join(name)).expect(name);
        assert_eq!(rust, py, "{name} differs between Rust and Python");
    }
}
