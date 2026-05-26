//! Native Rust font extractor for the HM64 US ROM. Mirrors
//! `tools/libhm64/fonts/extractor.py` byte-for-byte.

use std::path::Path;

use crate::extract::ExtractResult;

const FONT_TEXTURE_ROM_START: usize = 0xE08870;
const FONT_TEXTURE_ROM_END: usize = 0xE13770;
const FONT_PALETTE_ROM_START: usize = 0xE13770;
/// Each palette block in ROM is 4-byte header + 32-byte data + 12-byte pad = 48 bytes.
const PALETTE_BLOCK_LEN: usize = 48;
const PALETTE_HEADER_LEN: usize = 4;
const PALETTE_DATA_LEN: usize = 32;

/// Write `<workspace>/assets/font/fontTexture.ci2` and `fontPalette[123].pal`.
/// `rom_bytes` is the loaded ROM image (from `extract::rom::load_us`).
pub fn extract_default_font_us(rom_bytes: &[u8], workspace_dir: &Path) -> ExtractResult<()> {
    let out_dir = workspace_dir.join("assets/font");
    std::fs::create_dir_all(&out_dir)?;

    // Texture: copy verbatim.
    let texture = &rom_bytes[FONT_TEXTURE_ROM_START..FONT_TEXTURE_ROM_END];
    let texture_path = out_dir.join("fontTexture.ci2");
    crate::export::atomic_write(&texture_path, texture)
        .map_err(|e| crate::export::ExportError::write(&texture_path, e))?;

    // Three palettes: skip 4-byte header, write 32-byte data section.
    for n in 1u32..=3 {
        let block_start = FONT_PALETTE_ROM_START + (n as usize - 1) * PALETTE_BLOCK_LEN;
        let data_start = block_start + PALETTE_HEADER_LEN;
        let data_end = data_start + PALETTE_DATA_LEN;
        let pal = &rom_bytes[data_start..data_end];
        let path = out_dir.join(format!("fontPalette{n}.pal"));
        crate::export::atomic_write(&path, pal)
            .map_err(|e| crate::export::ExportError::write(&path, e))?;
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn extract_writes_four_files() {
        // Synthetic ROM: enough bytes to cover all addresses.
        let mut rom = vec![0u8; FONT_PALETTE_ROM_START + 3 * PALETTE_BLOCK_LEN];
        // Stamp recognizable bytes in the texture region.
        rom[FONT_TEXTURE_ROM_START] = 0xAA;
        // Stamp recognizable bytes in palette 2 data section.
        let p2_data = FONT_PALETTE_ROM_START + PALETTE_BLOCK_LEN + PALETTE_HEADER_LEN;
        rom[p2_data] = 0xBB;

        let tmp = tempdir().unwrap();
        extract_default_font_us(&rom, tmp.path()).unwrap();

        let tex = std::fs::read(tmp.path().join("assets/font/fontTexture.ci2")).unwrap();
        assert_eq!(tex.len(), FONT_TEXTURE_ROM_END - FONT_TEXTURE_ROM_START);
        assert_eq!(tex[0], 0xAA);

        let p2 = std::fs::read(tmp.path().join("assets/font/fontPalette2.pal")).unwrap();
        assert_eq!(p2.len(), 32);
        assert_eq!(p2[0], 0xBB);
    }
}
