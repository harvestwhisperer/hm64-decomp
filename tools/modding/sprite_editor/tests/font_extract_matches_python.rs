//! Byte-equality regression: the Rust font extractor produces files identical
//! to `make extract-fonts` (the python pipeline). Skips when the ROM or
//! python is unavailable.

use std::path::Path;
use std::process::Command;

use hm64_sprite_editor::extract::font::extract_default_font_us;

fn repo_root() -> std::path::PathBuf {
    let manifest_dir = env!("CARGO_MANIFEST_DIR");
    Path::new(manifest_dir).ancestors().nth(3).unwrap().to_path_buf()
}

#[test]
fn rust_extractor_matches_python_extractor() {
    let repo = repo_root();
    let rom = repo.join("baserom.us.z64");
    if !rom.is_file() {
        eprintln!("skipping: baserom.us.z64 not present");
        return;
    }
    if Command::new("python3").arg("--version").status().is_err() {
        eprintln!("skipping: python3 not on PATH");
        return;
    }

    let rom_bytes = std::fs::read(&rom).expect("read rom");

    let tmp_rust = tempfile::tempdir().unwrap();
    extract_default_font_us(&rom_bytes, tmp_rust.path()).expect("rust extract");

    let tmp_py = tempfile::tempdir().unwrap();
    let py_out = tmp_py.path().join("assets/font");
    std::fs::create_dir_all(&py_out).unwrap();
    let status = Command::new("python3")
        .arg("-m")
        .arg("libhm64.fonts.extractor")
        .arg("--output-dir")
        .arg(&py_out)
        .arg("--rom")
        .arg(&rom)
        .env("PYTHONPATH", repo.join("tools"))
        .current_dir(&repo)
        .status();
    let Ok(status) = status else {
        eprintln!("skipping: python extractor failed to launch");
        return;
    };
    if !status.success() {
        eprintln!("skipping: python extractor exited non-zero");
        return;
    }

    for name in ["fontTexture.ci2", "fontPalette1.pal", "fontPalette2.pal", "fontPalette3.pal"] {
        let rust = std::fs::read(tmp_rust.path().join("assets/font").join(name)).expect(name);
        let py = std::fs::read(py_out.join(name)).expect(name);
        assert_eq!(rust, py, "{name} differs between Rust and Python");
    }
}
