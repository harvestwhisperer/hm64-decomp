//! Exporters. One writer per dirty domain. Each writer takes a model
//! reference + a target path and writes the updated files atomically
//! (temp-file + rename).

pub mod animation_data_json;
pub mod animation_export;
pub mod avatar_inc_c_export;
pub mod font_bin_export;
pub mod font_ci2_export;
pub mod font_pal_export;
pub mod inc_c_export;
pub mod macros_h_export;
pub mod manifest_export;
pub mod palette_export;
pub mod scaffold;
pub mod sprite_bin;
pub mod texture_export;

use std::fs;
use std::io;
use std::path::{Path, PathBuf};

use thiserror::Error;

#[derive(Debug, Error)]
pub enum ExportError {
    #[error("i/o error writing {path}: {source}")]
    Write {
        path: PathBuf,
        #[source]
        source: io::Error,
    },
    #[error("serialization error for {path}: {source}")]
    Serialize {
        path: PathBuf,
        #[source]
        source: serde_json::Error,
    },
    #[error("invalid data for {path}: {message}")]
    Invalid { path: PathBuf, message: String },
}

impl ExportError {
    pub fn write(path: impl Into<PathBuf>, source: io::Error) -> Self {
        Self::Write { path: path.into(), source }
    }
    pub fn serialize(path: impl Into<PathBuf>, source: serde_json::Error) -> Self {
        Self::Serialize { path: path.into(), source }
    }
    pub fn invalid(path: impl Into<PathBuf>, message: impl Into<String>) -> Self {
        Self::Invalid { path: path.into(), message: message.into() }
    }
}

pub type ExportResult<T> = Result<T, ExportError>;

/// Atomic write: write a sibling temp file, then rename into place. Survives
/// mid-write crashes (partial file will be the temp file, not the target).
pub fn atomic_write(path: &Path, bytes: &[u8]) -> io::Result<()> {
    let parent = path
        .parent()
        .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidInput, "path has no parent"))?;
    fs::create_dir_all(parent)?;
    let tmp = parent.join(format!(".{}.tmp", path.file_name().unwrap().to_string_lossy()));
    fs::write(&tmp, bytes)?;
    fs::rename(&tmp, path)?;
    Ok(())
}
