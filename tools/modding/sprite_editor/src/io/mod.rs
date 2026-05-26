//! Filesystem readers. Each function in a submodule takes a path and returns
//! the corresponding model type, or a typed error.

pub mod animation_data_json;
pub mod animation_json;
pub mod binary;
pub mod character_avatars_csv;
pub mod font_ci2;
pub mod font_loader;
pub mod font_pal;
pub mod inc_c_parser;
pub mod labels_csv;
pub mod manifest;
pub mod pal_import;
pub mod palette_json;
pub mod png_import;
pub mod sprite_loader;
pub mod texture_ci;

use std::io;
use std::path::PathBuf;

use thiserror::Error;

#[derive(Debug, Error)]
pub enum IoError {
    #[error("i/o error reading {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: io::Error,
    },
    #[error("json parse error in {path}: {source}")]
    Json {
        path: PathBuf,
        #[source]
        source: serde_json::Error,
    },
    #[error("binary parse error in {path} at offset {offset}: {message}")]
    BinaryParse { path: PathBuf, offset: usize, message: String },
    #[error("inc.c parse error in {path} on line {line}: {message}")]
    IncCParse { path: PathBuf, line: usize, message: String },
    #[error("invalid data in {path}: {message}")]
    InvalidData { path: PathBuf, message: String },
}

impl IoError {
    pub fn read(path: impl Into<PathBuf>, source: io::Error) -> Self {
        Self::Read { path: path.into(), source }
    }
    pub fn json(path: impl Into<PathBuf>, source: serde_json::Error) -> Self {
        Self::Json { path: path.into(), source }
    }
    pub fn binary(path: impl Into<PathBuf>, offset: usize, message: impl Into<String>) -> Self {
        Self::BinaryParse { path: path.into(), offset, message: message.into() }
    }
    pub fn inc_c(path: impl Into<PathBuf>, line: usize, message: impl Into<String>) -> Self {
        Self::IncCParse { path: path.into(), line, message: message.into() }
    }
    pub fn invalid(path: impl Into<PathBuf>, message: impl Into<String>) -> Self {
        Self::InvalidData { path: path.into(), message: message.into() }
    }
}

pub type IoResult<T> = Result<T, IoError>;
