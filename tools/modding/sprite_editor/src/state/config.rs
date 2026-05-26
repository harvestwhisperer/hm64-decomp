//! Persisted user config: asset-root path and recent-files list.

use std::fs;
use std::path::{Path, PathBuf};

use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct UserConfig {
    pub asset_root: Option<PathBuf>,
    #[serde(default)]
    pub browser_expanded: Option<Vec<String>>,
    #[serde(default)]
    pub last_bin_export_dir: Option<PathBuf>,
    #[serde(default)]
    pub last_rom_path: Option<PathBuf>,
    #[serde(default)]
    pub last_extract_workspace: Option<PathBuf>,
    /// User-selected UI scale factor. Multiplies the window's default font
    /// size and other length-typed values that opt in to scaling. Clamped to
    /// [0.75, 2.5] on read; values outside that range probably indicate a
    /// hand-edited config and are not worth honoring.
    #[serde(default = "default_ui_scale")]
    pub ui_scale: f32,
}

fn default_ui_scale() -> f32 {
    1.0
}

impl Default for UserConfig {
    fn default() -> Self {
        Self {
            asset_root: None,
            browser_expanded: None,
            last_bin_export_dir: None,
            last_rom_path: None,
            last_extract_workspace: None,
            ui_scale: default_ui_scale(),
        }
    }
}

/// Allowed UI-scale presets, exposed in the toolbar dropdown. Values are
/// stored as multipliers (1.0 = 100 %). Sub-100 % options exist for users on
/// small screens or who prefer denser text; the labels in the slint dropdown
/// must stay 1-to-1 with this list.
pub const UI_SCALE_PRESETS: &[f32] = &[0.75, 0.9, 1.0, 1.25, 1.5, 1.75, 2.0];

/// Snap an arbitrary scale value to the nearest preset, falling back to 1.0
/// when the input is non-finite or wildly out of range.
pub fn nearest_ui_scale_preset(value: f32) -> f32 {
    if !value.is_finite() {
        return 1.0;
    }
    let clamped = value.clamp(0.5, 2.5);
    UI_SCALE_PRESETS
        .iter()
        .copied()
        .min_by(|a, b| {
            (a - clamped)
                .abs()
                .partial_cmp(&(b - clamped).abs())
                .unwrap_or(std::cmp::Ordering::Equal)
        })
        .unwrap_or(1.0)
}

pub fn config_path() -> Option<PathBuf> {
    let dir = dirs::config_dir()?.join("hm64-sprite-editor");
    Some(dir.join("config.json"))
}

pub fn load() -> UserConfig {
    let Some(path) = config_path() else {
        return UserConfig::default();
    };
    let Ok(bytes) = fs::read(&path) else {
        return UserConfig::default();
    };
    serde_json::from_slice(&bytes).unwrap_or_default()
}

pub fn save(cfg: &UserConfig) -> std::io::Result<()> {
    let Some(path) = config_path() else {
        return Ok(()); // Non-fatal: just skip.
    };
    let text = serde_json::to_string_pretty(cfg).unwrap();
    crate::export::atomic_write(&path, text.as_bytes())
}

/// Walk up from `start` looking for a directory containing `assets/sprites`.
pub fn auto_detect_asset_root(start: &Path) -> Option<PathBuf> {
    for anc in start.ancestors() {
        if anc.join("assets/sprites").is_dir() {
            return Some(anc.to_path_buf());
        }
    }
    None
}
