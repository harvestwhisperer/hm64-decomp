//! Writer for `src/data/animation/characterAvatars/characterAvatars.inc.c`.
//!
//! This file is the avatar counterpart to `inc_c_export`, which only handles
//! `u16 *AnimationScripts[]` arrays. The avatar table is a single
//! `u8 characterAvatarCodesToAnimations[]` whose values are each entry's
//! `metadata_index` (one byte per slot).
//!
//! Canonical form:
//! - LF newlines only (never CRLF), matching the entity inc.c emitter.
//! - Synthesized include block:
//!   `#include "common.h"\n\n#include "characterAvatars.h"\n\n`.
//! - Header: `u8 characterAvatarCodesToAnimations[] = {\n`.
//! - Body lines: tab-indented, 16 values per line, decimal with `, ` between
//!   values; every line except the last ends with a trailing `,`.
//! - Closing `};\n`.
//!
//! Returns an error if any `metadata_index` exceeds `u8::MAX` — the runtime
//! table is `u8`, so a wider value cannot round-trip.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::AvatarTable;

const VALUES_PER_LINE: usize = 16;

#[derive(Debug, thiserror::Error)]
pub enum AvatarIncCError {
    #[error("avatar entry {index} has metadata_index {value} which exceeds u8::MAX (255)")]
    ValueTooLarge { index: usize, value: u16 },
}

/// Emit the canonical text for `table` as a `characterAvatars.inc.c` body.
///
/// Errors if any `metadata_index` does not fit in a `u8`.
pub fn emit_avatar_inc_c(table: &AvatarTable) -> Result<String, AvatarIncCError> {
    let mut bytes: Vec<u8> = Vec::with_capacity(table.entries.len());
    for (i, e) in table.entries.iter().enumerate() {
        if e.metadata_index > u8::MAX as u16 {
            return Err(AvatarIncCError::ValueTooLarge {
                index: i,
                value: e.metadata_index,
            });
        }
        bytes.push(e.metadata_index as u8);
    }

    let mut out = String::with_capacity(96 + bytes.len() * 5);
    out.push_str("#include \"common.h\"\n\n");
    out.push_str("#include \"characterAvatars.h\"\n\n");
    out.push_str("u8 characterAvatarCodesToAnimations[] = {\n");

    if !bytes.is_empty() {
        for (i, v) in bytes.iter().enumerate() {
            let col = i % VALUES_PER_LINE;
            if col == 0 {
                out.push('\t');
            }
            out.push_str(&v.to_string());

            let is_last = i + 1 == bytes.len();
            let end_of_line = col + 1 == VALUES_PER_LINE;
            if is_last {
                out.push('\n');
            } else if end_of_line {
                out.push_str(",\n");
            } else {
                out.push_str(", ");
            }
        }
    }

    out.push_str("};\n");
    Ok(out)
}

/// Serialize and atomically write `table` to `path`.
pub fn write_avatar_inc_c(path: &Path, table: &AvatarTable) -> ExportResult<()> {
    let bytes = match emit_avatar_inc_c(table) {
        Ok(s) => s.into_bytes(),
        Err(e) => {
            return Err(ExportError::write(
                path,
                std::io::Error::new(std::io::ErrorKind::InvalidData, e.to_string()),
            ));
        }
    };
    atomic_write(path, &bytes).map_err(|e| ExportError::write(path, e))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::AvatarEntry;

    fn entry(idx: u16) -> AvatarEntry {
        AvatarEntry {
            character: String::new(),
            expression: String::new(),
            metadata_index: idx,
        }
    }

    fn table_from(values: &[u16]) -> AvatarTable {
        AvatarTable {
            kind: "avatar".into(),
            entries: values.iter().map(|v| entry(*v)).collect(),
        }
    }

    /// The canonical 143-entry table that ships with the original ROM.
    /// Source: `src/data/animation/characterAvatars/characterAvatars.inc.c`.
    fn canonical_values() -> [u16; 143] {
        [
            7, 11, 9, 12, 13, 15, 17, 16, 19, 20, 1, 3, 2, 6, 4, 27, 28, 30, 31, 29, 21, 24, 23,
            25, 134, 69, 70, 49, 50, 51, 43, 44, 45, 46, 47, 48, 40, 41, 42, 80, 81, 82, 71, 72,
            73, 74, 75, 76, 77, 78, 79, 55, 56, 52, 53, 54, 83, 84, 85, 89, 88, 90, 91, 130, 131,
            98, 99, 100, 86, 87, 67, 68, 96, 97, 132, 133, 94, 95, 92, 93, 128, 129, 101, 102, 59,
            60, 65, 66, 61, 62, 57, 58, 105, 106, 63, 64, 114, 115, 127, 126, 33, 34, 35, 36, 37,
            116, 117, 38, 39, 122, 120, 121, 107, 108, 109, 103, 112, 113, 123, 124, 125, 118, 119,
            110, 111, 104, 8, 10, 14, 18, 5, 32, 22, 26, 0, 135, 136, 137, 138, 139, 140, 141, 142,
        ]
    }

    #[test]
    fn emits_canonical_character_avatars_inc_c() {
        let table = table_from(&canonical_values());
        let out = emit_avatar_inc_c(&table).unwrap();
        let expected = "\
#include \"common.h\"

#include \"characterAvatars.h\"

u8 characterAvatarCodesToAnimations[] = {
\t7, 11, 9, 12, 13, 15, 17, 16, 19, 20, 1, 3, 2, 6, 4, 27,
\t28, 30, 31, 29, 21, 24, 23, 25, 134, 69, 70, 49, 50, 51, 43, 44,
\t45, 46, 47, 48, 40, 41, 42, 80, 81, 82, 71, 72, 73, 74, 75, 76,
\t77, 78, 79, 55, 56, 52, 53, 54, 83, 84, 85, 89, 88, 90, 91, 130,
\t131, 98, 99, 100, 86, 87, 67, 68, 96, 97, 132, 133, 94, 95, 92, 93,
\t128, 129, 101, 102, 59, 60, 65, 66, 61, 62, 57, 58, 105, 106, 63, 64,
\t114, 115, 127, 126, 33, 34, 35, 36, 37, 116, 117, 38, 39, 122, 120, 121,
\t107, 108, 109, 103, 112, 113, 123, 124, 125, 118, 119, 110, 111, 104, 8, 10,
\t14, 18, 5, 32, 22, 26, 0, 135, 136, 137, 138, 139, 140, 141, 142
};
";
        assert_eq!(out, expected);
    }

    #[test]
    fn empty_table_emits_only_header_and_close() {
        let t = table_from(&[]);
        let out = emit_avatar_inc_c(&t).unwrap();
        assert_eq!(
            out,
            "#include \"common.h\"\n\n#include \"characterAvatars.h\"\n\nu8 characterAvatarCodesToAnimations[] = {\n};\n"
        );
    }

    #[test]
    fn single_entry_has_no_trailing_comma() {
        let t = table_from(&[42]);
        let out = emit_avatar_inc_c(&t).unwrap();
        assert!(out.contains("\n\t42\n};\n"), "got: {out}");
    }

    #[test]
    fn exactly_sixteen_entries_fit_on_one_line_no_trailing_comma() {
        let vals: Vec<u16> = (0..16).collect();
        let t = table_from(&vals);
        let out = emit_avatar_inc_c(&t).unwrap();
        // One line of 16 values, last value has no comma after it.
        assert!(out.contains("\t0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15\n};\n"),
            "got: {out}");
    }

    #[test]
    fn seventeen_entries_wrap_with_trailing_comma_on_first_line() {
        let vals: Vec<u16> = (0..17).collect();
        let t = table_from(&vals);
        let out = emit_avatar_inc_c(&t).unwrap();
        assert!(out.contains("\t0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,\n\t16\n};\n"),
            "got: {out}");
    }

    #[test]
    fn rejects_value_above_u8_max() {
        let t = table_from(&[256]);
        let err = emit_avatar_inc_c(&t).unwrap_err();
        match err {
            AvatarIncCError::ValueTooLarge { index, value } => {
                assert_eq!(index, 0);
                assert_eq!(value, 256);
            }
        }
    }

    #[test]
    fn write_avatar_inc_c_round_trips_via_disk() {
        let dir = tempfile::tempdir().unwrap();
        let path = dir.path().join("characterAvatars.inc.c");
        let table = table_from(&canonical_values());
        write_avatar_inc_c(&path, &table).unwrap();
        let written = std::fs::read_to_string(&path).unwrap();
        assert_eq!(written, emit_avatar_inc_c(&table).unwrap());
    }
}
