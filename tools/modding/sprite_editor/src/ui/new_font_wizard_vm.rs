//! View-model + commit logic for the New Font Wizard.

use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

use crate::export::{font_ci2_export, font_pal_export, ExportError, ExportResult};
use crate::model::{FontAsset, FontGlyph, FontPalette, GlyphUnusedBytes, Rgba, GLYPH_PIXELS, PALETTE_TOTAL_COLORS};

pub const DEFAULT_GLYPH_COUNT: u32 = 700;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InitialPalette {
    Black,
    Greyscale,
    CopyDefault,
}

#[derive(Debug, Clone)]
pub struct WizardForm {
    pub name: String,
    pub glyph_count: u32,
    pub initial_palette: InitialPalette,
}

#[derive(Debug)]
pub enum WizardError {
    EmptyName,
    InvalidName(String),
    DirectoryExists(PathBuf),
    DefaultPaletteUnavailable,
    Export(ExportError),
}

impl std::fmt::Display for WizardError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::EmptyName => write!(f, "name is required"),
            Self::InvalidName(s) => write!(f, "invalid name: {s}"),
            Self::DirectoryExists(p) => write!(f, "directory already exists: {}", p.display()),
            Self::DefaultPaletteUnavailable => write!(f, "default font has no palette 1 to copy"),
            Self::Export(e) => write!(f, "export error: {e}"),
        }
    }
}

impl std::error::Error for WizardError {}

impl From<ExportError> for WizardError {
    fn from(e: ExportError) -> Self {
        Self::Export(e)
    }
}

/// Validate name: nonempty, no path separators, no leading dot.
pub fn validate_name(name: &str) -> Result<&str, WizardError> {
    if name.is_empty() {
        return Err(WizardError::EmptyName);
    }
    if name.contains(['/', '\\']) {
        return Err(WizardError::InvalidName("contains path separator".into()));
    }
    if name.starts_with('.') {
        return Err(WizardError::InvalidName("starts with dot".into()));
    }
    Ok(name)
}

/// Build a fresh `FontAsset` from form values without touching disk.
/// `default_palette` is required only when `initial_palette == CopyDefault`.
pub fn build_asset(
    asset_root: &Path,
    form: &WizardForm,
    default_palette: Option<&FontPalette>,
) -> Result<FontAsset, WizardError> {
    validate_name(&form.name)?;
    let dir = asset_root.join("assets/font").join(&form.name);
    if dir.exists() {
        return Err(WizardError::DirectoryExists(dir));
    }
    let glyphs: Vec<FontGlyph> = (0..form.glyph_count)
        .map(|i| FontGlyph { index: i, pixels: [0u8; GLYPH_PIXELS], unused: GlyphUnusedBytes::default() })
        .collect();

    let initial_colors = match form.initial_palette {
        InitialPalette::Black => [Rgba::default(); PALETTE_TOTAL_COLORS],
        InitialPalette::Greyscale => greyscale_palette(),
        InitialPalette::CopyDefault => {
            default_palette
                .ok_or(WizardError::DefaultPaletteUnavailable)?
                .colors
        }
    };
    let mut palettes = BTreeMap::new();
    palettes.insert(1, FontPalette { index: 1, colors: initial_colors });

    Ok(FontAsset { dir, name: form.name.clone(), glyphs, palettes })
}

/// Materialize the font on disk: create the directory, write fontTexture.ci2
/// and fontPalette1.pal.
pub fn commit(asset: &FontAsset) -> ExportResult<()> {
    std::fs::create_dir_all(&asset.dir)
        .map_err(|e| ExportError::write(&asset.dir, e))?;
    font_ci2_export::write_font_texture(&asset.dir, asset)?;
    if let Some(pal) = asset.palettes.get(&1) {
        font_pal_export::write_font_palette(&asset.dir, pal)?;
    }
    Ok(())
}

fn greyscale_palette() -> [Rgba; PALETTE_TOTAL_COLORS] {
    let mut p = [Rgba::default(); PALETTE_TOTAL_COLORS];
    // Live colors: black, dark grey, light grey, white.
    p[0] = Rgba::new(0, 0, 0, 0);
    p[1] = Rgba::new(85, 85, 85, 255);
    p[2] = Rgba::new(170, 170, 170, 255);
    p[3] = Rgba::new(255, 255, 255, 255);
    p
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn validate_name_accepts_simple() {
        assert!(validate_name("myFont").is_ok());
    }

    #[test]
    fn validate_name_rejects_empty_and_dot_and_slash() {
        assert!(validate_name("").is_err());
        assert!(validate_name(".hidden").is_err());
        assert!(validate_name("a/b").is_err());
        assert!(validate_name("a\\b").is_err());
    }

    #[test]
    fn build_asset_rejects_existing_dir() {
        let tmp = tempdir().unwrap();
        std::fs::create_dir_all(tmp.path().join("assets/font/myFont")).unwrap();
        let form = WizardForm {
            name: "myFont".into(),
            glyph_count: 4,
            initial_palette: InitialPalette::Black,
        };
        let err = build_asset(tmp.path(), &form, None).unwrap_err();
        assert!(matches!(err, WizardError::DirectoryExists(_)));
    }

    #[test]
    fn build_asset_with_greyscale_palette() {
        let tmp = tempdir().unwrap();
        let form = WizardForm {
            name: "f".into(),
            glyph_count: 4,
            initial_palette: InitialPalette::Greyscale,
        };
        let asset = build_asset(tmp.path(), &form, None).unwrap();
        assert_eq!(asset.glyphs.len(), 4);
        assert_eq!(asset.palettes[&1].colors[3], Rgba::new(255, 255, 255, 255));
    }

    #[test]
    fn commit_writes_files() {
        let tmp = tempdir().unwrap();
        let form = WizardForm {
            name: "f".into(),
            glyph_count: 1,
            initial_palette: InitialPalette::Black,
        };
        let asset = build_asset(tmp.path(), &form, None).unwrap();
        commit(&asset).unwrap();
        assert!(asset.dir.join("fontTexture.ci2").is_file());
        assert!(asset.dir.join("fontPalette1.pal").is_file());
    }
}
