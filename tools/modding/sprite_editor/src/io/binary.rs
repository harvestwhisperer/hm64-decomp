//! Byte packing/unpacking primitives used by the bin exporter and ROM
//! extractor. Mirrors the helper functions in `tools/libhm64/sprites/assembler.py`.
//!
//! The HM64 sprite format mixes endianness:
//!   - u32 / u16 in headers and offset tables: big-endian
//!   - s16 / u16 in animation metadata (durations, anchors): little-endian

pub fn pack_u32_be(v: u32) -> [u8; 4] {
    v.to_be_bytes()
}

pub fn pack_u16_be(v: u16) -> [u8; 2] {
    v.to_be_bytes()
}

pub fn pack_u16_le(v: u16) -> [u8; 2] {
    v.to_le_bytes()
}

pub fn pack_s16_le(v: i16) -> [u8; 2] {
    v.to_le_bytes()
}

pub fn read_u32_be(bytes: &[u8], offset: usize) -> Option<u32> {
    let slice: &[u8; 4] = bytes.get(offset..offset + 4)?.try_into().ok()?;
    Some(u32::from_be_bytes(*slice))
}

pub fn read_u16_be(bytes: &[u8], offset: usize) -> Option<u16> {
    let slice: &[u8; 2] = bytes.get(offset..offset + 2)?.try_into().ok()?;
    Some(u16::from_be_bytes(*slice))
}

pub fn read_u16_le(bytes: &[u8], offset: usize) -> Option<u16> {
    let slice: &[u8; 2] = bytes.get(offset..offset + 2)?.try_into().ok()?;
    Some(u16::from_le_bytes(*slice))
}

pub fn read_s16_le(bytes: &[u8], offset: usize) -> Option<i16> {
    let slice: &[u8; 2] = bytes.get(offset..offset + 2)?.try_into().ok()?;
    Some(i16::from_le_bytes(*slice))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn pack_u32_be_matches_known_bytes() {
        assert_eq!(pack_u32_be(0x12345678), [0x12, 0x34, 0x56, 0x78]);
    }

    #[test]
    fn pack_u16_le_matches_known_bytes() {
        assert_eq!(pack_u16_le(0x1234), [0x34, 0x12]);
    }

    #[test]
    fn pack_s16_le_round_trip() {
        assert_eq!(read_s16_le(&pack_s16_le(-42), 0), Some(-42));
    }

    #[test]
    fn read_returns_none_at_short_input() {
        assert_eq!(read_u32_be(&[1, 2, 3], 0), None);
        assert_eq!(read_u32_be(&[1, 2, 3, 4], 1), None);
    }
}
