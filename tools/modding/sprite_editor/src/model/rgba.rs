//! 32-bit RGBA color type, plus lossy conversions to/from N64 RGBA5551.
//!
//! RGBA5551 layout (big-endian u16):
//!     RRRRR GGGGG BBBBB A
//! Each channel has 5 bits except alpha (1 bit). We scale to 8-bit channels
//! by left-shifting by 3 (r/g/b) and expanding alpha to 0 or 255.

use serde::{Deserialize, Serialize};

#[derive(Copy, Clone, Debug, Default, PartialEq, Eq, Serialize, Deserialize)]
pub struct Rgba {
    pub r: u8,
    pub g: u8,
    pub b: u8,
    pub a: u8,
}

impl Rgba {
    pub const fn new(r: u8, g: u8, b: u8, a: u8) -> Self {
        Self { r, g, b, a }
    }

    /// Decode a 16-bit RGBA5551 value into 8-bit-per-channel RGBA.
    ///
    /// The bottom 3 bits of each color channel are filled by replicating the
    /// top 3 bits (standard 5-to-8 bit expansion used by most N64 tooling).
    pub const fn from_rgba5551(v: u16) -> Self {
        let r5 = ((v >> 11) & 0x1F) as u8;
        let g5 = ((v >> 6) & 0x1F) as u8;
        let b5 = ((v >> 1) & 0x1F) as u8;
        let a1 = (v & 0x01) as u8;
        Self {
            r: (r5 << 3) | (r5 >> 2),
            g: (g5 << 3) | (g5 >> 2),
            b: (b5 << 3) | (b5 >> 2),
            a: if a1 == 0 { 0 } else { 255 },
        }
    }

    /// Encode to RGBA5551. Alpha is thresholded at 128.
    pub const fn to_rgba5551(self) -> u16 {
        let r5 = (self.r >> 3) as u16;
        let g5 = (self.g >> 3) as u16;
        let b5 = (self.b >> 3) as u16;
        let a1 = if self.a >= 128 { 1 } else { 0 };
        (r5 << 11) | (g5 << 6) | (b5 << 1) | a1
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn decode_all_channels_max() {
        // 0xFFFF = 11111 11111 11111 1 → RGBA (255, 255, 255, 255)
        let c = Rgba::from_rgba5551(0xFFFF);
        assert_eq!(c, Rgba::new(255, 255, 255, 255));
    }

    #[test]
    fn decode_all_zero() {
        let c = Rgba::from_rgba5551(0x0000);
        assert_eq!(c, Rgba::new(0, 0, 0, 0));
    }

    #[test]
    fn decode_red_only() {
        // 0xF801 = 11111 00000 00000 1
        let c = Rgba::from_rgba5551(0xF801);
        assert_eq!(c, Rgba::new(255, 0, 0, 255));
    }

    #[test]
    fn encode_all_channels_max() {
        assert_eq!(Rgba::new(255, 255, 255, 255).to_rgba5551(), 0xFFFF);
    }

    #[test]
    fn encode_round_trip_preserves_representable_values() {
        // Values representable in 5 bits (i.e., multiples of 8 with the 3-bit
        // replication pattern) round-trip exactly.
        for v in 0..=0xFFFFu16 {
            let decoded = Rgba::from_rgba5551(v);
            assert_eq!(decoded.to_rgba5551(), v, "round-trip failed at 0x{v:04X}");
        }
    }

    #[test]
    fn alpha_threshold_at_128() {
        assert_eq!(Rgba::new(0, 0, 0, 127).to_rgba5551() & 1, 0);
        assert_eq!(Rgba::new(0, 0, 0, 128).to_rgba5551() & 1, 1);
    }
}
