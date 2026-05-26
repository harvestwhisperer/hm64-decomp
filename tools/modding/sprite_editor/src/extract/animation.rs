//! Animation metadata block decoding. Mirrors
//! `tools/libhm64/animations/metadata.py::read_metadata` (simple format).

use crate::io::binary::{read_s16_le, read_u16_le};
use crate::model::{Animation, AnimationFrame, FrameSprite};

#[derive(Debug, thiserror::Error)]
pub enum AnimationError {
    #[error("offset {offset}: out of range (rom len {len})")]
    OutOfRange { offset: usize, len: usize },
}

/// Decode the animation block at `anim_base + anim_offsets[anim_idx]`.
/// Returns `None` if the slot is empty or the index is the trailing sentinel.
pub fn decode_animation(
    rom: &[u8],
    anim_base: u32,
    anim_offsets: &[u32],
    anim_idx: usize,
) -> Option<Animation> {
    if anim_idx + 1 >= anim_offsets.len() {
        return None;
    }
    let offset = anim_offsets[anim_idx];
    let next_offset = anim_offsets[anim_idx + 1];
    if offset == next_offset {
        return None;
    }
    let anim_addr = (anim_base + offset) as usize;

    // Header: bytes 0..4 = "46d\0" magic, 4..6 = num_frames LE, 6..8 = unused LE
    let num_frames = read_u16_le(rom, anim_addr + 4)? as u32;
    let unused = read_u16_le(rom, anim_addr + 6)? as u32;

    let mut p = anim_addr + 8;
    let mut frames = Vec::with_capacity(num_frames as usize);
    for _ in 0..num_frames {
        let sprite_count = read_u16_le(rom, p)?;
        let frame_duration = *rom.get(p + 2)?;
        let has_audio = *rom.get(p + 3)?;
        p += 4;

        let mut sprites = Vec::with_capacity(sprite_count as usize);
        for _ in 0..sprite_count {
            let spritesheet_index = read_u16_le(rom, p)? as u32;
            // p+2..p+4 is the unused u16
            let anchor_x = read_s16_le(rom, p + 4)?;
            let anchor_y = read_s16_le(rom, p + 6)?;
            sprites.push(FrameSprite { spritesheet_index, anchor_x, anchor_y });
            p += 8;
        }
        frames.push(AnimationFrame {
            duration: frame_duration as u16,
            has_audio,
            sprites,
        });
    }

    Some(Animation { frame_count: num_frames, unused, frames, index: anim_idx as u32 })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn decodes_one_frame_one_sprite() {
        // anim_base = 0, anim_offsets = [0, 24]
        //   bytes 0..4: "46d\0"
        //   bytes 4..6: num_frames = 1 (LE)
        //   bytes 6..8: unused = 0 (LE)
        //   bytes 8..12: per-frame header (sprite_count=1 LE, dur=4, audio=0)
        //   bytes 12..20: per-sprite (idx=5 LE, unused=0 LE, ax=-3 LE, ay=10 LE)
        let mut rom = Vec::new();
        rom.extend_from_slice(b"\x34\x36\x64\x00");
        rom.extend_from_slice(&1u16.to_le_bytes());
        rom.extend_from_slice(&0u16.to_le_bytes());
        rom.extend_from_slice(&1u16.to_le_bytes());
        rom.push(4);
        rom.push(0);
        rom.extend_from_slice(&5u16.to_le_bytes());
        rom.extend_from_slice(&0u16.to_le_bytes());
        rom.extend_from_slice(&(-3i16).to_le_bytes());
        rom.extend_from_slice(&10i16.to_le_bytes());
        // Pad with extra so anim_offsets[1]=24 is in range
        rom.resize(rom.len() + 8, 0);

        let a = decode_animation(&rom, 0, &[0, 24], 0).unwrap();
        assert_eq!(a.frame_count, 1);
        assert_eq!(a.frames.len(), 1);
        assert_eq!(a.frames[0].duration, 4);
        assert_eq!(a.frames[0].has_audio, 0);
        assert_eq!(a.frames[0].sprites.len(), 1);
        let s = &a.frames[0].sprites[0];
        assert_eq!(s.spritesheet_index, 5);
        assert_eq!(s.anchor_x, -3);
        assert_eq!(s.anchor_y, 10);
    }

    #[test]
    fn returns_none_for_empty_slot() {
        let rom = vec![0u8; 64];
        assert!(decode_animation(&rom, 0, &[0, 0], 0).is_none());
    }
}
