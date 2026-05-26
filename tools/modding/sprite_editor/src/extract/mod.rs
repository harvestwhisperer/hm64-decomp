//! Native Rust ROM extractor for the HM64 US sprite domain.
//!
//! Extracts to the same `assets/sprites/<subdir>/<name>/...` layout that
//! `make extract-sprites` produces, so an extracted workspace is loadable
//! by the rest of the editor without any change.

pub mod addresses;
pub mod animation;
pub mod font;
pub mod labels;
pub mod palette;
pub mod rom;
pub mod script;
pub mod sprite;
pub mod texture;

use std::path::Path;

#[derive(Debug, thiserror::Error)]
pub enum ExtractError {
    #[error("rom: {0}")]
    Rom(#[from] rom::RomError),
    #[error("addresses: {0}")]
    Addresses(#[from] addresses::AddressesError),
    #[error("labels: {0}")]
    Labels(#[from] crate::io::labels_csv::LabelCsvError),
    #[error("texture: {0}")]
    Texture(#[from] texture::TextureError),
    #[error("palette: {0}")]
    Palette(#[from] palette::PaletteError),
    #[error("animation: {0}")]
    Animation(#[from] animation::AnimationError),
    #[error("script: {0}")]
    Script(#[from] script::ScriptError),
    #[error("export: {0}")]
    Export(#[from] crate::export::ExportError),
    #[error("io: {0}")]
    Io(#[from] std::io::Error),
    #[error("cancelled")]
    Cancelled,
}

pub type ExtractResult<T> = Result<T, ExtractError>;

#[derive(Debug, Clone)]
pub enum Progress {
    Started { total_sprites: usize },
    Sprite { name: String, done: usize, total: usize },
    Phase { name: String },
    Finished { sprites_extracted: usize, warnings: usize },
}

pub fn extract_us(
    rom_path: &Path,
    workspace_dir: &Path,
    mut progress: impl FnMut(Progress) + Send + 'static,
) -> ExtractResult<()> {
    let r = rom::load_us(rom_path)?;
    let address_rows = addresses::embedded()?;
    let labels = labels::embedded()?;
    let script_rows = script::embedded_script_addresses();

    std::fs::create_dir_all(workspace_dir.join("assets/sprites"))?;

    let total = address_rows.len();
    progress(Progress::Started { total_sprites: total });

    let mut warnings = 0usize;
    for (i, info) in address_rows.iter().enumerate() {
        progress(Progress::Sprite { name: info.label.clone(), done: i, total });

        let sprite_dir = match sprite::extract_sprite(&r.bytes, info, workspace_dir) {
            Ok(p) => p,
            Err(e) => {
                eprintln!("warning: failed to extract {}: {e}", info.label);
                warnings += 1;
                continue;
            }
        };

        // Entity sprites: also extract the animation script bytes from ROM
        // and write per-script JSON. The python extractor leaves this to splat;
        // we do it inline so the standalone extractor produces a complete
        // workspace.
        if info.kind == "entity" {
            if let Some(script_addr) = script_rows.iter().find(|s| s.label == info.label) {
                match script::decode_script(
                    &r.bytes,
                    script_addr.start,
                    script_addr.end,
                    &info.label,
                    &info.label,
                    &labels,
                ) {
                    Ok(doc) => {
                        let scripts_dir = sprite_dir.join("animationScripts");
                        if let Err(e) = std::fs::create_dir_all(&scripts_dir) {
                            eprintln!("warning: failed to create {scripts_dir:?}: {e}");
                            warnings += 1;
                            continue;
                        }
                        let path = scripts_dir.join(format!("{}.json", info.label));
                        match serde_json::to_string_pretty(&doc) {
                            Ok(mut text) => {
                                text.push('\n');
                                if let Err(e) =
                                    crate::export::atomic_write(&path, text.as_bytes())
                                {
                                    eprintln!("warning: failed to write {path:?}: {e}");
                                    warnings += 1;
                                }
                            }
                            Err(e) => {
                                eprintln!("warning: failed to serialize script {}: {e}", info.label);
                                warnings += 1;
                            }
                        }
                    }
                    Err(e) => {
                        eprintln!("warning: failed to decode script {}: {e}", info.label);
                        warnings += 1;
                    }
                }
            }
        }
    }

    progress(Progress::Phase { name: "font".into() });
    if let Err(e) = font::extract_default_font_us(&r.bytes, workspace_dir) {
        eprintln!("warning: font extraction failed: {e}");
        warnings += 1;
    }

    progress(Progress::Finished {
        sprites_extracted: total - warnings,
        warnings,
    });
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn extract_us_walks_all_addresses() {
        let manifest_dir = env!("CARGO_MANIFEST_DIR");
        let root = std::path::Path::new(manifest_dir).ancestors().nth(3).unwrap();
        let rom = root.join("baserom.us.z64");
        if !rom.exists() {
            eprintln!("skipping: ROM not present");
            return;
        }

        let tmp = tempdir().unwrap();
        let result = extract_us(&rom, tmp.path(), |_p| {});
        result.expect("extract_us should succeed");
        assert!(tmp.path().join("assets/sprites").is_dir());
        // At least one sprite should be present.
        assert!(tmp.path().join("assets/sprites/entitySprites/animals/bird/manifest.json").exists());
        assert!(tmp.path().join("assets/font/fontTexture.ci2").is_file());
        assert!(tmp.path().join("assets/font/fontPalette1.pal").is_file());
        assert!(tmp.path().join("assets/font/fontPalette2.pal").is_file());
        assert!(tmp.path().join("assets/font/fontPalette3.pal").is_file());
    }
}
