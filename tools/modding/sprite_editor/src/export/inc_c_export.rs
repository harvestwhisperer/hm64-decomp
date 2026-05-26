//! Writer for `src/data/animationScripts/*.inc.c` files.
//!
//! Renders an [`EntityScriptDoc`] into the canonical textual form understood
//! by `crate::io::inc_c_parser`. Output is deterministic and lossless for any
//! script that was produced by that parser, modulo blank lines (which are
//! stripped on parse).
//!
//! Canonical form:
//! - LF newlines only (never CRLF).
//! - Tab indent (single `\t`) on every entry.
//! - Synthesized include block: `#include "common.h"\n\n#include "animationScripts.h"\n\n`.
//! - Header: `u16 <name>AnimationScripts[] = {\n`.
//! - Entries render as `PACK_ANIM_DATA(<u16>, ANIM_TYPE_<kind>, <flip>),`
//!   with exactly one space after each comma. `<flip>` is the literal
//!   `FLIP_HORIZONTAL` when set, else the literal `0`.
//! - Trailing zeros (preserved from the parsed `trailing_zeros` count) are
//!   emitted as `\t0,\n` per zero except the last, which is `\t0\n`, then a
//!   single blank line before `};`.
//! - Closing `};\n`.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{EntityScriptDoc, JsonAnimType};

fn anim_type_c_ident(t: JsonAnimType) -> &'static str {
    match t {
        JsonAnimType::OneShot => "ANIM_TYPE_ONE_SHOT",
        JsonAnimType::Loop => "ANIM_TYPE_LOOP",
        JsonAnimType::DestroyOnEnd => "ANIM_TYPE_DESTROY_ON_END",
    }
}

/// Emit a deterministic textual representation of `doc` matching the
/// canonical `.inc.c` layout.
pub fn emit_inc_c(doc: &EntityScriptDoc) -> String {
    let mut out = String::with_capacity(128 + doc.animations.len() * 48);

    out.push_str("#include \"common.h\"\n\n");
    out.push_str("#include \"animationScripts.h\"\n\n");

    out.push_str("u16 ");
    out.push_str(&doc.name);
    out.push_str("AnimationScripts[] = {\n");

    for a in &doc.animations {
        out.push('\t');
        out.push_str("PACK_ANIM_DATA(");
        out.push_str(&a.metadata_index.to_string());
        out.push_str(", ");
        out.push_str(anim_type_c_ident(a.anim_type));
        out.push_str(", ");
        out.push_str(if a.flip { "FLIP_HORIZONTAL" } else { "0" });
        out.push_str("),\n");
    }

    if doc.trailing_zeros > 0 {
        for i in 0..doc.trailing_zeros {
            if i + 1 < doc.trailing_zeros {
                out.push_str("\t0,\n");
            } else {
                out.push_str("\t0\n");
            }
        }
        out.push('\n');
    }
    out.push_str("};\n");
    out
}

/// Serialize and atomically write `doc` to `path`.
pub fn write_inc_c(path: &Path, doc: &EntityScriptDoc) -> ExportResult<()> {
    let bytes = emit_inc_c(doc).into_bytes();
    atomic_write(path, &bytes).map_err(|e| ExportError::write(path, e))
}

#[cfg(test)]
mod tests {
    use std::path::PathBuf;

    use super::*;
    use crate::io::inc_c_parser::parse_inc_c;

    fn fixture(name: &str) -> PathBuf {
        PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/inc_c").join(name)
    }

    fn assert_round_trip(name: &str) {
        let path = fixture(name);
        let original = std::fs::read(&path).expect("read fixture");
        let doc = parse_inc_c(&path).expect("parse fixture");
        let emitted = emit_inc_c(&doc);
        let emitted_bytes = emitted.as_bytes();
        if emitted_bytes != original.as_slice() {
            let mismatch = original
                .iter()
                .zip(emitted_bytes.iter())
                .position(|(a, b)| a != b)
                .unwrap_or(original.len().min(emitted_bytes.len()));
            panic!(
                "fixture {name}: emit != original (first diff at byte {mismatch}; \
                 original_len={}, emitted_len={})",
                original.len(),
                emitted_bytes.len(),
            );
        }
    }

    #[test]
    fn round_trip_minimal() {
        assert_round_trip("minimal.inc.c");
    }

    #[test]
    fn round_trip_all_anim_types() {
        assert_round_trip("all_anim_types.inc.c");
    }

    #[test]
    fn write_inc_c_produces_same_bytes_as_emit() {
        let dir = tempfile::tempdir().unwrap();
        let path = dir.path().join("out.inc.c");
        let doc = parse_inc_c(&fixture("minimal.inc.c")).unwrap();
        write_inc_c(&path, &doc).unwrap();
        let written = std::fs::read(&path).unwrap();
        assert_eq!(written, emit_inc_c(&doc).into_bytes());
    }

    #[test]
    fn empty_animations_emits_canonical_header_and_close() {
        use crate::model::EntityScriptDoc;
        let doc = EntityScriptDoc {
            kind: "entity".into(),
            sprite: "foo".into(),
            animations: vec![],
            trailing_zeros: 0,
            name: "foo".into(),
        };
        let out = emit_inc_c(&doc);
        assert_eq!(
            out,
            "#include \"common.h\"\n\n#include \"animationScripts.h\"\n\nu16 fooAnimationScripts[] = {\n};\n"
        );
    }
}
