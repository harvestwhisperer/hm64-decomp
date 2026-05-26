//! Reader for `animations/NN.json`.

use std::fs;
use std::path::Path;

use crate::io::{IoError, IoResult};
use crate::model::Animation;

pub fn read_animation(path: &Path) -> IoResult<Animation> {
    let bytes = fs::read(path).map_err(|e| IoError::read(path, e))?;
    let a: Animation = serde_json::from_slice(&bytes).map_err(|e| IoError::json(path, e))?;
    if a.frames.len() as u32 != a.frame_count {
        return Err(IoError::invalid(
            path,
            format!("frame_count {} does not match frames.len() {}", a.frame_count, a.frames.len()),
        ));
    }
    Ok(a)
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    const SINGLE_FRAME: &str = r#"{
  "frame_count": 1,
  "unused": 0,
  "frames": [
    {
      "duration": 1,
      "has_audio": 0,
      "sprites": [
        { "spritesheet_index": 0, "anchor_x": 0, "anchor_y": -16 }
      ]
    }
  ],
  "index": 0
}"#;

    #[test]
    fn reads_single_frame_animation() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("00.json");
        fs::write(&p, SINGLE_FRAME).unwrap();
        let a = read_animation(&p).unwrap();
        assert_eq!(a.frame_count, 1);
        assert_eq!(a.frames.len(), 1);
        assert_eq!(a.frames[0].sprites[0].anchor_y, -16);
    }

    #[test]
    fn rejects_frame_count_mismatch() {
        let mismatched = r#"{
  "frame_count": 5,
  "unused": 0,
  "frames": [],
  "index": 0
}"#;
        let dir = tempdir().unwrap();
        let p = dir.path().join("00.json");
        fs::write(&p, mismatched).unwrap();
        let err = read_animation(&p).unwrap_err();
        assert!(matches!(err, IoError::InvalidData { .. }), "got: {err:?}");
    }
}
