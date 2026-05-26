//! ROM loading and IPL3 checksum validation.

use std::fs;
use std::path::Path;

const HM64_US_SIZE: u64 = 16 * 1024 * 1024;
// IPL3 checksum bytes at offset 0x10..0x18. Captured from the repo's
// `baserom.us.z64` (16-MiB cart image).
const HM64_US_CRC1: u32 = 0x98DF9DFC;
const HM64_US_CRC2: u32 = 0x6606C189;

#[derive(Debug, thiserror::Error)]
pub enum RomError {
    #[error("could not read ROM file: {0}")]
    Read(std::io::Error),
    #[error("file size {actual} bytes — expected {expected}")]
    BadSize { actual: u64, expected: u64 },
    #[error("not an HM64 US ROM (CRC mismatch)")]
    BadChecksum,
}

#[derive(Debug)]
pub struct Rom {
    pub bytes: Vec<u8>,
}

pub fn load_us(path: &Path) -> Result<Rom, RomError> {
    let metadata = fs::metadata(path).map_err(RomError::Read)?;
    let actual = metadata.len();
    if actual != HM64_US_SIZE {
        return Err(RomError::BadSize { actual, expected: HM64_US_SIZE });
    }
    let bytes = fs::read(path).map_err(RomError::Read)?;
    validate_us(&bytes)?;
    Ok(Rom { bytes })
}

fn validate_us(bytes: &[u8]) -> Result<(), RomError> {
    if bytes.len() < 0x18 {
        return Err(RomError::BadChecksum);
    }
    let crc1 = u32::from_be_bytes(bytes[0x10..0x14].try_into().unwrap());
    let crc2 = u32::from_be_bytes(bytes[0x14..0x18].try_into().unwrap());
    if crc1 != HM64_US_CRC1 || crc2 != HM64_US_CRC2 {
        return Err(RomError::BadChecksum);
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use tempfile::tempdir;

    #[test]
    fn rejects_wrong_size() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("small.z64");
        fs::write(&p, [0u8; 16]).unwrap();
        let err = load_us(&p).unwrap_err();
        assert!(matches!(err, RomError::BadSize { .. }));
    }

    #[test]
    fn rejects_bad_checksum_with_correct_size() {
        let dir = tempdir().unwrap();
        let p = dir.path().join("zeroes.z64");
        let buf = vec![0u8; HM64_US_SIZE as usize];
        fs::write(&p, &buf).unwrap();
        let err = load_us(&p).unwrap_err();
        assert!(matches!(err, RomError::BadChecksum));
    }
}
