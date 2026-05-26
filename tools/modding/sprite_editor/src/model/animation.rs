//! Animation metadata — one `Animation` per `animations/NN.json` file.

use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Animation {
    /// Entry count reported in the JSON. Equal to `frames.len()` on disk.
    pub frame_count: u32,
    /// An unused/padding field preserved verbatim for round-trip fidelity.
    pub unused: u32,
    pub frames: Vec<AnimationFrame>,
    pub index: u32,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct AnimationFrame {
    /// Duration in game-ticks. Consumed by `state::playback` with a
    /// `TICK_MS` constant calibrated during M2.
    pub duration: u16,
    /// On disk this is a 0/1 integer; kept as u8 to preserve the exact value.
    pub has_audio: u8,
    pub sprites: Vec<FrameSprite>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct FrameSprite {
    pub spritesheet_index: u32,
    pub anchor_x: i16,
    pub anchor_y: i16,
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn minimum_animation_round_trip() {
        let a = Animation {
            frame_count: 1,
            unused: 0,
            frames: vec![AnimationFrame {
                duration: 1,
                has_audio: 0,
                sprites: vec![FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: -16 }],
            }],
            index: 0,
        };
        let s = serde_json::to_string(&a).unwrap();
        let back: Animation = serde_json::from_str(&s).unwrap();
        assert_eq!(a, back);
    }

    #[test]
    fn preserves_unused_field_nonzero() {
        // We don't know the semantics of `unused`; preserve it byte-identical.
        let a = Animation { frame_count: 0, unused: 0xDEAD_BEEF, frames: vec![], index: 0 };
        let s = serde_json::to_string(&a).unwrap();
        let back: Animation = serde_json::from_str(&s).unwrap();
        assert_eq!(back.unused, 0xDEAD_BEEF);
    }
}
