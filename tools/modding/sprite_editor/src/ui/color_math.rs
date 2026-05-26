//! Pure RGB/HSV/hex helpers for the palette color picker. No dependencies on
//! the model, Slint, or editor state.

/// Convert an 8-bit RGB triple to HSV. Hue is returned in [0, 360); saturation
/// and value are in [0, 1]. When saturation is 0 (greyscale), hue is returned
/// as 0 — callers that want to preserve a "last non-zero hue" must track that
/// separately.
pub fn rgb_to_hsv(r: u8, g: u8, b: u8) -> (f32, f32, f32) {
    let rf = r as f32 / 255.0;
    let gf = g as f32 / 255.0;
    let bf = b as f32 / 255.0;
    let max = rf.max(gf).max(bf);
    let min = rf.min(gf).min(bf);
    let delta = max - min;

    let hue = if delta == 0.0 {
        0.0
    } else if max == rf {
        60.0 * (((gf - bf) / delta).rem_euclid(6.0))
    } else if max == gf {
        60.0 * (((bf - rf) / delta) + 2.0)
    } else {
        60.0 * (((rf - gf) / delta) + 4.0)
    };
    let hue = if hue < 0.0 { hue + 360.0 } else { hue };

    let sat = if max == 0.0 { 0.0 } else { delta / max };
    (hue, sat, max)
}

/// Convert HSV (hue in [0, 360), S/V in [0, 1]) to an 8-bit RGB triple.
pub fn hsv_to_rgb(hue: f32, sat: f32, val: f32) -> (u8, u8, u8) {
    let h = hue.rem_euclid(360.0);
    let c = val * sat;
    let x = c * (1.0 - ((h / 60.0) % 2.0 - 1.0).abs());
    let m = val - c;
    let (r1, g1, b1) = match h {
        h if h < 60.0 => (c, x, 0.0),
        h if h < 120.0 => (x, c, 0.0),
        h if h < 180.0 => (0.0, c, x),
        h if h < 240.0 => (0.0, x, c),
        h if h < 300.0 => (x, 0.0, c),
        _ => (c, 0.0, x),
    };
    let to_u8 = |v: f32| ((v + m) * 255.0).round().clamp(0.0, 255.0) as u8;
    (to_u8(r1), to_u8(g1), to_u8(b1))
}

/// Parse `#RRGGBB` or `#RRGGBBAA` into `(r, g, b, a)`. Returns `None` for
/// anything else (including missing `#`, wrong length, or non-hex chars).
pub fn parse_hex(s: &str) -> Option<(u8, u8, u8, u8)> {
    let s = s.trim();
    let s = s.strip_prefix('#')?;
    let bytes: Vec<u8> = (0..s.len())
        .step_by(2)
        .map(|i| u8::from_str_radix(s.get(i..i + 2)?, 16).ok())
        .collect::<Option<Vec<u8>>>()?;
    match bytes.as_slice() {
        [r, g, b] => Some((*r, *g, *b, 255)),
        [r, g, b, a] => Some((*r, *g, *b, *a)),
        _ => None,
    }
}

/// Format `(r, g, b, a)` as `#RRGGBB` when alpha is 255, otherwise `#RRGGBBAA`.
pub fn format_hex(r: u8, g: u8, b: u8, a: u8) -> String {
    if a == 255 {
        format!("#{r:02X}{g:02X}{b:02X}")
    } else {
        format!("#{r:02X}{g:02X}{b:02X}{a:02X}")
    }
}
