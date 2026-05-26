//! Animation script decoding. Reads packed `u16` entries from ROM and
//! produces an `EntityScriptDoc` ready to round-trip through the editor.
//!
//! The packed u16 layout (see `src/data/animation/entityAnimationScripts/entityAnimationScripts.h`):
//!   bits 0..12 (mask 0x1FFF): metadata_index
//!   bits 13..14 (mask 0x6000): anim_type — 0x0000 ONE_SHOT, 0x2000 LOOP, 0x4000 DESTROY_ON_END
//!   bit 15     (mask 0x8000): FLIP_HORIZONTAL

use crate::io::binary::read_u16_be;
use crate::io::labels_csv::LabelTable;
use crate::model::{EntityAnimation, EntityScriptDoc, JsonAnimType};

#[derive(Debug, thiserror::Error)]
pub enum ScriptError {
    #[error("offset {offset}: out of range (rom len {len})")]
    OutOfRange { offset: usize, len: usize },
    #[error("invalid anim_type bits 0x{bits:04X} at entry offset {offset}")]
    BadAnimType { offset: usize, bits: u16 },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ScriptAddress {
    pub start: u32,
    pub end: u32,
    pub label: String,
}

pub fn parse_script_addresses(text: &str) -> Vec<ScriptAddress> {
    let mut out = Vec::new();
    for line in text.lines() {
        if line.trim().is_empty() {
            continue;
        }
        let cols: Vec<&str> = line.split(',').map(|s| s.trim()).collect();
        if cols.len() != 3 {
            continue;
        }
        let (Ok(start), Ok(end)) =
            (u32::from_str_radix(cols[0], 16), u32::from_str_radix(cols[1], 16))
        else {
            continue;
        };
        out.push(ScriptAddress { start, end, label: cols[2].to_string() });
    }
    out
}

pub fn embedded_script_addresses() -> Vec<ScriptAddress> {
    parse_script_addresses(include_str!(
        "../../../../libhm64/data/animation_scripts_addresses.csv"
    ))
}

fn decode_entry(raw: u16, offset: usize) -> Result<(u16, JsonAnimType, bool), ScriptError> {
    let metadata_index = raw & 0x1FFF;
    let flip = (raw & 0x8000) != 0;
    let anim_type = match raw & 0x6000 {
        0x0000 => JsonAnimType::OneShot,
        0x2000 => JsonAnimType::Loop,
        0x4000 => JsonAnimType::DestroyOnEnd,
        bits => return Err(ScriptError::BadAnimType { offset, bits }),
    };
    Ok((metadata_index, anim_type, flip))
}

pub fn decode_script(
    rom: &[u8],
    start: u32,
    end: u32,
    sprite_label: &str,
    script_name: &str,
    labels: &LabelTable,
) -> Result<EntityScriptDoc, ScriptError> {
    let start = start as usize;
    let end = end as usize;
    if end > rom.len() || start > end {
        return Err(ScriptError::OutOfRange { offset: start, len: rom.len() });
    }

    let mut animations = Vec::new();
    let entry_size = 2;
    let mut cursor = start;
    let mut entry_idx = 0u32;
    while cursor + entry_size <= end {
        let raw = read_u16_be(rom, cursor)
            .ok_or(ScriptError::OutOfRange { offset: cursor, len: rom.len() })?;
        if raw == 0 {
            // Bare `0` entries are trailing padding once we stop seeing real
            // entries; counted at the end of the loop.
            cursor += entry_size;
            continue;
        }
        let (metadata_index, anim_type, flip) = decode_entry(raw, cursor)?;
        let lookup = labels.lookup(sprite_label, entry_idx);
        let label =
            lookup.and_then(|e| e.label.clone()).unwrap_or_else(|| entry_idx.to_string());
        let directional =
            lookup.and_then(|e| e.group_role.map(|_| true)).unwrap_or(false);
        animations.push(EntityAnimation {
            label,
            metadata_index,
            anim_type,
            flip,
            directional,
        });
        entry_idx += 1;
        cursor += entry_size;
    }

    // Count trailing zeros: consecutive zero u16 entries at the end.
    let mut trailing_zeros = 0u32;
    let mut probe = end;
    while probe >= start + entry_size {
        let raw = read_u16_be(rom, probe - entry_size).unwrap_or(1);
        if raw == 0 {
            trailing_zeros += 1;
            probe -= entry_size;
        } else {
            break;
        }
    }

    Ok(EntityScriptDoc {
        kind: "entity".into(),
        sprite: sprite_label.into(),
        name: script_name.into(),
        animations,
        trailing_zeros,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn decodes_pack_round_trip() {
        // Build raw bytes for [PACK(0, LOOP, 0), PACK(15, ONE_SHOT, FLIP), 0, 0]
        let entries: [u16; 4] = [
            0u16 | 0x2000,        // metadata=0, LOOP
            15u16 | 0x0000 | 0x8000, // metadata=15, ONE_SHOT, FLIP
            0,
            0,
        ];
        let mut rom = Vec::new();
        for e in entries {
            rom.extend_from_slice(&e.to_be_bytes());
        }

        let labels = LabelTable::default();
        let doc =
            decode_script(&rom, 0, rom.len() as u32, "test", "test", &labels).unwrap();
        assert_eq!(doc.animations.len(), 2);
        assert_eq!(doc.animations[0].metadata_index, 0);
        assert_eq!(doc.animations[0].anim_type, JsonAnimType::Loop);
        assert!(!doc.animations[0].flip);
        assert_eq!(doc.animations[1].metadata_index, 15);
        assert_eq!(doc.animations[1].anim_type, JsonAnimType::OneShot);
        assert!(doc.animations[1].flip);
        assert_eq!(doc.trailing_zeros, 2);
    }

    #[test]
    fn parses_script_addresses_csv() {
        let csv = "F4B50,F50D0,player\nF50D0,F51D0,maria\n";
        let rows = parse_script_addresses(csv);
        assert_eq!(rows.len(), 2);
        assert_eq!(rows[0].start, 0xF4B50);
        assert_eq!(rows[0].label, "player");
    }

    #[test]
    fn embedded_script_addresses_parses() {
        let rows = embedded_script_addresses();
        assert!(!rows.is_empty());
        assert!(rows.iter().any(|r| r.label == "player"));
    }
}
