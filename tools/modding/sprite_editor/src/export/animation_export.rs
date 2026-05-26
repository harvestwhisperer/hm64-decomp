//! Writer for `animations/NN.json`. Atomic temp+rename, two-digit
//! zero-padded filename to match the existing on-disk convention.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::Animation;

pub fn write_animation(sprite_dir: &Path, anim: &Animation) -> ExportResult<()> {
    let path = sprite_dir.join("animations").join(format!("{:02}.json", anim.index));
    // Spec §2: frame_count auto-syncs to frames.len() at save time.
    let synced = Animation { frame_count: anim.frames.len() as u32, ..anim.clone() };
    let text = serde_json::to_string_pretty(&synced)
        .map_err(|e| ExportError::serialize(&path, e))?;
    atomic_write(&path, text.as_bytes()).map_err(|e| ExportError::write(&path, e))
}

pub fn delete_animation_file(sprite_dir: &Path, idx: u32) -> ExportResult<()> {
    let path = sprite_dir.join("animations").join(format!("{:02}.json", idx));
    match std::fs::remove_file(&path) {
        Ok(()) => Ok(()),
        Err(e) if e.kind() == std::io::ErrorKind::NotFound => Ok(()),
        Err(e) => Err(ExportError::write(&path, e)),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{Animation, AnimationFrame, FrameSprite};
    use tempfile::tempdir;

    fn sample(index: u32) -> Animation {
        Animation {
            frame_count: 1,
            unused: 0,
            index,
            frames: vec![AnimationFrame {
                duration: 4,
                has_audio: 0,
                sprites: vec![FrameSprite { spritesheet_index: 7, anchor_x: -3, anchor_y: 5 }],
            }],
        }
    }

    #[test]
    fn writes_two_digit_padded_filename() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("animations")).unwrap();
        write_animation(dir.path(), &sample(3)).unwrap();
        assert!(dir.path().join("animations/03.json").is_file());
    }

    #[test]
    fn writes_pretty_json_with_all_fields() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("animations")).unwrap();
        write_animation(dir.path(), &sample(0)).unwrap();
        let text = std::fs::read_to_string(dir.path().join("animations/00.json")).unwrap();
        assert!(text.contains("\"frame_count\": 1"));
        assert!(text.contains("\"unused\": 0"));
        assert!(text.contains("\"index\": 0"));
        assert!(text.contains("\"duration\": 4"));
        assert!(text.contains("\"spritesheet_index\": 7"));
        assert!(text.contains("\"anchor_x\": -3"));
        assert!(text.contains("\"anchor_y\": 5"));
    }

    #[test]
    fn round_trips_serialize_then_deserialize() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("animations")).unwrap();
        let original = sample(11);
        write_animation(dir.path(), &original).unwrap();
        let text = std::fs::read_to_string(dir.path().join("animations/11.json")).unwrap();
        let back: Animation = serde_json::from_str(&text).unwrap();
        assert_eq!(back, original);
    }

    #[test]
    fn delete_removes_existing_file() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("animations")).unwrap();
        write_animation(dir.path(), &sample(5)).unwrap();
        assert!(dir.path().join("animations/05.json").is_file());
        delete_animation_file(dir.path(), 5).unwrap();
        assert!(!dir.path().join("animations/05.json").exists());
    }

    #[test]
    fn delete_silently_succeeds_on_missing() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("animations")).unwrap();
        // No file exists at index 9 — should be Ok(()), not an error.
        delete_animation_file(dir.path(), 9).unwrap();
    }

    #[test]
    fn auto_syncs_frame_count_to_frames_len() {
        let dir = tempdir().unwrap();
        std::fs::create_dir_all(dir.path().join("animations")).unwrap();
        // Construct an Animation with a deliberately wrong frame_count.
        let anim = Animation {
            frame_count: 99,  // Lies — actually has 1 frame.
            unused: 0,
            index: 0,
            frames: vec![AnimationFrame {
                duration: 1, has_audio: 0,
                sprites: vec![FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 }],
            }],
        };
        write_animation(dir.path(), &anim).unwrap();
        let text = std::fs::read_to_string(dir.path().join("animations/00.json")).unwrap();
        // Saved file must show the corrected count.
        assert!(text.contains("\"frame_count\": 1"), "frame_count must auto-sync to frames.len()");
        assert!(!text.contains("\"frame_count\": 99"), "the stale count must not survive");
    }
}
