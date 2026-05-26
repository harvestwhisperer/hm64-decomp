//! Reader for `assets/sprites/<subdir>/<name>/manifest.json`.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::Manifest;

pub fn read_manifest(path: &Path) -> IoResult<Manifest> {
    let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
    serde_json::from_slice(&bytes).map_err(|e| IoError::json(path, e))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{SpriteType, TextureFormat};
    use tempfile::tempdir;

    const MINIMAL: &str = r#"{
  "label": "player",
  "subdir": "entitySprites",
  "type": "type-1",
  "sprite_count": 1,
  "spritesheet_index_count": 1,
  "palette_count": 1,
  "palette_index_count": 1,
  "animation_count": 1,
  "animation_index": [0],
  "palette_index": [0],
  "textures": [
    { "index": 0, "width": 32, "height": 32, "format": "ci8", "palette_index": 0 }
  ]
}"#;

    #[test]
    fn reads_minimal_manifest() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("manifest.json");
        fs::write(&p, MINIMAL).unwrap();
        let m = read_manifest(&p).unwrap();
        assert_eq!(m.label, "player");
        assert_eq!(m.subdir, "entitySprites");
        assert_eq!(m.sprite_type, SpriteType::Type1);
        assert_eq!(m.textures.len(), 1);
        assert_eq!(m.textures[0].format, TextureFormat::Ci8);
    }

    #[test]
    fn returns_io_error_for_missing_file() {
        let err = read_manifest(Path::new("/nonexistent/manifest.json")).unwrap_err();
        assert!(matches!(err, IoError::Read { .. }), "got: {err:?}");
    }

    #[test]
    fn returns_json_error_for_malformed() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("manifest.json");
        fs::write(&p, "not json").unwrap();
        let err = read_manifest(&p).unwrap_err();
        assert!(matches!(err, IoError::Json { .. }), "got: {err:?}");
    }
}
