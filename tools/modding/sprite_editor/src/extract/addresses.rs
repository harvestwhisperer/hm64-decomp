//! Embedded parser for `tools/libhm64/data/sprite_addresses.csv`. Frozen
//! original-ROM data — see memory note "frozen asset catalogues".
//!
//! The CSV has two row shapes — see `libhm64/sprites/addresses.py`:
//! - 7 columns (type-1): addr_base, addr_index, spritesheet_index_base,
//!   spritesheet_index_end, label, subdir, kind
//! - 6 columns (type-2): addr_base, addr_index, addr_extra, label, subdir, kind

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SpriteAddress {
    pub addr_base: u32,
    pub addr_index: u32,
    pub label: String,
    pub subdir: String,
    pub kind: String,
    pub variant: AddressVariant,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum AddressVariant {
    /// Type-1: spritesheet index is a separate region in ROM.
    Type1 { spritesheet_index_base: u32, spritesheet_index_end: u32 },
    /// Type-2: spritesheet index is embedded; `addr_extra` points to the next region.
    Type2 { addr_extra: u32 },
}

#[derive(Debug, thiserror::Error)]
pub enum AddressesError {
    #[error("line {line}: invalid hex `{value}` in column `{column}`")]
    BadHex { line: usize, value: String, column: &'static str },
}

pub fn parse(text: &str) -> Result<Vec<SpriteAddress>, AddressesError> {
    let mut out = Vec::new();
    for (i, line) in text.lines().enumerate() {
        if line.trim().is_empty() {
            continue;
        }
        let cols: Vec<String> = line.split(',').map(|s| s.trim().to_string()).collect();
        let line_no = i + 1;
        let parse_hex = |val: &str, col: &'static str| {
            u32::from_str_radix(val, 16).map_err(|_| AddressesError::BadHex {
                line: line_no,
                value: val.to_string(),
                column: col,
            })
        };
        let row = match cols.len() {
            7 => SpriteAddress {
                addr_base: parse_hex(&cols[0], "addr_base")?,
                addr_index: parse_hex(&cols[1], "addr_index")?,
                label: cols[4].clone(),
                subdir: cols[5].clone(),
                kind: cols[6].clone(),
                variant: AddressVariant::Type1 {
                    spritesheet_index_base: parse_hex(&cols[2], "spritesheet_index_base")?,
                    spritesheet_index_end: parse_hex(&cols[3], "spritesheet_index_end")?,
                },
            },
            6 => SpriteAddress {
                addr_base: parse_hex(&cols[0], "addr_base")?,
                addr_index: parse_hex(&cols[1], "addr_index")?,
                label: cols[3].clone(),
                subdir: cols[4].clone(),
                kind: cols[5].clone(),
                variant: AddressVariant::Type2 { addr_extra: parse_hex(&cols[2], "addr_extra")? },
            },
            // Match the Python loader: silently skip rows that aren't 6 or 7 columns.
            _ => continue,
        };
        out.push(row);
    }
    Ok(out)
}

pub fn embedded() -> Result<Vec<SpriteAddress>, AddressesError> {
    parse(include_str!("../../../../libhm64/data/sprite_addresses.csv"))
}

/// Read the 8 u32-BE asset offsets at `addr_index`. Mirrors
/// `addresses.py::get_asset_offsets`.
pub fn read_asset_offsets(rom: &[u8], addr_index: u32) -> [u32; 8] {
    let mut out = [0u32; 8];
    for (i, slot) in out.iter_mut().enumerate() {
        let off = addr_index as usize + i * 4;
        *slot = crate::io::binary::read_u32_be(rom, off).unwrap_or(0);
    }
    out
}

/// Build the spritesheet offset table for a sprite. Mirrors
/// `addresses.py::get_spritesheet_offsets`.
pub fn spritesheet_offsets(rom: &[u8], info: &SpriteAddress) -> Vec<u32> {
    match &info.variant {
        AddressVariant::Type1 { spritesheet_index_base, spritesheet_index_end } => {
            let count = (spritesheet_index_end - spritesheet_index_base) as usize / 4;
            let mut offsets: Vec<u32> = (0..count)
                .map(|i| {
                    let off = *spritesheet_index_base as usize + i * 4;
                    crate::io::binary::read_u32_be(rom, off).unwrap_or(0)
                })
                .collect();
            if info.label == "festivalFlowers" {
                let embedded_index_size = (count * 4) as u32;
                let last_nonzero = offsets.iter().rposition(|&v| v != 0).map(|p| p as i32).unwrap_or(-1);
                for (i, off) in offsets.iter_mut().enumerate() {
                    if (i as i32) <= last_nonzero {
                        *off += embedded_index_size;
                    } else {
                        *off = 0;
                    }
                }
            }
            offsets
        }
        AddressVariant::Type2 { .. } => {
            let base = info.addr_base as usize;
            let first_offset = crate::io::binary::read_u32_be(rom, base).unwrap_or(0);
            if first_offset == 0 || first_offset > 0x10000 || first_offset % 4 != 0 {
                return Vec::new();
            }
            let count = first_offset as usize / 4;
            if count > 1024 {
                return Vec::new();
            }
            (0..count)
                .map(|i| crate::io::binary::read_u32_be(rom, base + i * 4).unwrap_or(0))
                .collect()
        }
    }
}

/// Build the palette offset table.
pub fn palette_offsets(rom: &[u8], addr_base: u32, asset_offsets: &[u32; 8]) -> Vec<u32> {
    if asset_offsets[1] == 0 {
        return Vec::new();
    }
    let palette_section = (addr_base + asset_offsets[1]) as usize;
    let first_offset = crate::io::binary::read_u32_be(rom, palette_section).unwrap_or(0);
    if first_offset == 0 || first_offset > 0x1000 || first_offset % 4 != 0 {
        return Vec::new();
    }
    let count = first_offset as usize / 4;
    if count > 256 {
        return Vec::new();
    }
    (0..count)
        .map(|i| crate::io::binary::read_u32_be(rom, palette_section + i * 4).unwrap_or(0))
        .collect()
}

/// Build the animation offset table.
pub fn animation_offsets(rom: &[u8], addr_base: u32, asset_offsets: &[u32; 8]) -> Vec<u32> {
    let anim_section = (addr_base + asset_offsets[2]) as usize;
    let first_offset = crate::io::binary::read_u32_be(rom, anim_section).unwrap_or(0);
    if first_offset == 0 {
        return Vec::new();
    }
    let count = first_offset as usize / 4;
    (0..count)
        .map(|i| crate::io::binary::read_u32_be(rom, anim_section + i * 4).unwrap_or(0))
        .collect()
}

/// Read the sprite-to-palette mapping bytes (one u8 per sprite).
pub fn sprite_to_palette_map(
    rom: &[u8],
    addr_base: u32,
    asset_offsets: &[u32; 8],
    sprite_count: usize,
) -> Vec<u8> {
    let map_base = (addr_base + asset_offsets[3]) as usize + 4; // skip 4-byte header
    rom.get(map_base..map_base + sprite_count).map(|s| s.to_vec()).unwrap_or_default()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_type1_row() {
        let csv = "159330,2D0660,2D0680,2D15E0,player,entitySprites,entity\n";
        let rows = parse(csv).unwrap();
        assert_eq!(rows.len(), 1);
        assert_eq!(rows[0].addr_base, 0x159330);
        assert_eq!(rows[0].addr_index, 0x2D0660);
        assert_eq!(rows[0].label, "player");
        assert_eq!(rows[0].kind, "entity");
        assert!(matches!(rows[0].variant, AddressVariant::Type1 { .. }));
    }

    #[test]
    fn parses_type2_row() {
        let csv = "7FCD10,7FD1E0,7FD200,shadows,entitySprites,standalone\n";
        let rows = parse(csv).unwrap();
        assert_eq!(rows.len(), 1);
        assert_eq!(rows[0].label, "shadows");
        assert!(matches!(rows[0].variant, AddressVariant::Type2 { .. }));
    }

    #[test]
    fn rejects_bad_hex() {
        let csv = "ZZZZ,2D0660,2D0680,2D15E0,player,entitySprites,entity\n";
        let err = parse(csv).unwrap_err();
        assert!(matches!(err, AddressesError::BadHex { .. }));
    }

    #[test]
    fn embedded_csv_parses() {
        let rows = embedded().expect("embedded CSV must parse");
        assert!(!rows.is_empty());
        assert!(rows.iter().any(|r| r.label == "player"));
        assert!(rows.iter().any(|r| r.label == "shadows"));
    }
}
