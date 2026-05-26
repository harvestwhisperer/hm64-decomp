//! Writer for `manifest.json`.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::Manifest;

pub fn write_manifest(sprite_dir: &Path, manifest: &Manifest) -> ExportResult<()> {
    let path = sprite_dir.join("manifest.json");
    let text =
        serde_json::to_string_pretty(manifest).map_err(|e| ExportError::serialize(&path, e))?;
    atomic_write(&path, text.as_bytes()).map_err(|e| ExportError::write(&path, e))
}
