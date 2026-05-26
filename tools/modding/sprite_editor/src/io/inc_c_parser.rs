//! Parser for `src/data/animationScripts/*.inc.c` files.
//!
//! Recognized lines (inside the array body):
//! - `PACK_ANIM_DATA(<u16>, ANIM_TYPE_<kind>, <0|FLIP_HORIZONTAL>),`
//! - blank lines (silently stripped)
//! - bare `0` / `0,` lines (counted as trailing zeros so the exporter can
//!   reproduce the same trailing-zero count)
//!
//! Anything else inside the array body is a parse error with line number.
//!
//! Not every file under `src/data/animationScripts/` is an animation script.
//! `characterAvatars.inc.c` defines a `u8 characterAvatarCodesToAnimations[]`
//! mapping array (not a `u16 *AnimationScripts[]`) and lives there only
//! because of ROM alignment. This parser rejects it with an `IncCParse` error
//! at the header; callers that walk the directory should filter it out.

use std::fs;
use std::path::{Path, PathBuf};
use std::sync::OnceLock;

use regex::Regex;

use crate::io::{IoError, IoResult};
use crate::model::{EntityAnimation, EntityScriptDoc, JsonAnimType};

fn entry_regex() -> &'static Regex {
    static RE: OnceLock<Regex> = OnceLock::new();
    RE.get_or_init(|| {
        Regex::new(
            r"^\s*PACK_ANIM_DATA\(\s*(\d+)\s*,\s*(ANIM_TYPE_[A-Z_]+)\s*,\s*(0|FLIP_HORIZONTAL)\s*\)\s*,?\s*$",
        )
        .unwrap()
    })
}

fn header_regex() -> &'static Regex {
    static RE: OnceLock<Regex> = OnceLock::new();
    RE.get_or_init(|| Regex::new(r"^\s*u16\s+(\w+AnimationScripts)\[\]\s*=\s*\{\s*$").unwrap())
}

fn include_regex() -> &'static Regex {
    static RE: OnceLock<Regex> = OnceLock::new();
    RE.get_or_init(|| Regex::new(r#"^\s*#\s*include\s+"[^"]+"\s*$"#).unwrap())
}

fn parse_anim_type(s: &str) -> Option<JsonAnimType> {
    Some(match s {
        "ANIM_TYPE_ONE_SHOT" => JsonAnimType::OneShot,
        "ANIM_TYPE_LOOP" => JsonAnimType::Loop,
        "ANIM_TYPE_DESTROY_ON_END" => JsonAnimType::DestroyOnEnd,
        _ => return None,
    })
}

pub fn parse_inc_c(path: &Path) -> IoResult<EntityScriptDoc> {
    let text = fs::read_to_string(path).map_err(|e| IoError::read(path, e))?;
    parse_inc_c_text(path.to_path_buf(), &text)
}

pub fn parse_inc_c_text(source_file: PathBuf, text: &str) -> IoResult<EntityScriptDoc> {
    enum State {
        Prologue,
        Body,
        Done,
    }
    let mut state = State::Prologue;
    let mut array_ident: Option<String> = None;
    let mut animations: Vec<EntityAnimation> = Vec::new();
    let mut trailing_zeros: u32 = 0;

    for (idx, raw_line) in text.lines().enumerate() {
        let line_no = idx + 1;
        let trimmed = raw_line.trim();

        match state {
            State::Prologue => {
                if trimmed.is_empty() {
                    continue; // skip blank lines before the header
                }
                if include_regex().is_match(raw_line) {
                    // Includes are recognized but not preserved — the exporter
                    // synthesizes a canonical include block.
                    continue;
                }
                if let Some(caps) = header_regex().captures(raw_line) {
                    array_ident = Some(caps[1].to_string());
                    state = State::Body;
                    continue;
                }
                return Err(IoError::inc_c(
                    &source_file,
                    line_no,
                    format!("unexpected line in prologue: {raw_line:?}"),
                ));
            }
            State::Body => {
                if trimmed.is_empty() {
                    continue;
                }
                if trimmed == "};" {
                    state = State::Done;
                    continue;
                }
                // One or more bare `0` / `0,` lines are trailing padding.
                // Count them; the emitter reproduces the exact count.
                if trimmed == "0" || trimmed == "0," {
                    trailing_zeros += 1;
                    continue;
                }
                if let Some(caps) = entry_regex().captures(raw_line) {
                    let offset: u16 = caps[1].parse().map_err(|_| {
                        IoError::inc_c(&source_file, line_no, "metadata_offset not u16")
                    })?;
                    if offset > 8191 {
                        return Err(IoError::inc_c(
                            &source_file,
                            line_no,
                            format!("metadata_offset {offset} exceeds 8191"),
                        ));
                    }
                    let anim_type = parse_anim_type(&caps[2]).ok_or_else(|| {
                        IoError::inc_c(
                            &source_file,
                            line_no,
                            format!("unknown anim type {}", &caps[2]),
                        )
                    })?;
                    let flip = &caps[3] == "FLIP_HORIZONTAL";
                    let label = animations.len().to_string();
                    animations.push(EntityAnimation {
                        label,
                        metadata_index: offset,
                        anim_type,
                        flip,
                        directional: false,
                    });
                    continue;
                }
                return Err(IoError::inc_c(
                    &source_file,
                    line_no,
                    format!("unrecognized line in array body: {raw_line:?}"),
                ));
            }
            State::Done => {
                if trimmed.is_empty() || trimmed == "};" {
                    continue;
                }
                return Err(IoError::inc_c(
                    &source_file,
                    line_no,
                    format!("content after array close: {raw_line:?}"),
                ));
            }
        }
    }

    let array_ident =
        array_ident.ok_or_else(|| IoError::inc_c(&source_file, 0, "no script header found"))?;
    if !matches!(state, State::Done) {
        return Err(IoError::inc_c(&source_file, 0, "unterminated array (missing `};`)"));
    }

    // `<name>AnimationScripts` → `<name>`.
    let name = array_ident
        .strip_suffix("AnimationScripts")
        .unwrap_or(&array_ident)
        .to_string();

    Ok(EntityScriptDoc {
        kind: "entity".into(),
        // Sprite identifier defaults to the script name; the import flow can
        // refine it (e.g. `holdableItems`-style overrides) in a later phase.
        sprite: name.clone(),
        animations,
        trailing_zeros,
        name,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn fixture(name: &str) -> PathBuf {
        PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/inc_c").join(name)
    }

    #[test]
    fn parses_minimal() {
        let doc = parse_inc_c(&fixture("minimal.inc.c")).unwrap();
        assert_eq!(doc.name, "minimal");
        assert_eq!(doc.kind, "entity");
        assert_eq!(doc.sprite, "minimal");
        assert_eq!(doc.animations.len(), 1);
        let a = &doc.animations[0];
        assert_eq!(a.metadata_index, 0);
        assert_eq!(a.anim_type, JsonAnimType::Loop);
        assert!(!a.flip);
        assert!(!a.directional);
        assert_eq!(doc.trailing_zeros, 0);
    }

    #[test]
    fn parses_group_breaks_strips_blanks() {
        // Blank lines are silently stripped — only entries are preserved.
        let doc = parse_inc_c(&fixture("with_group_breaks.inc.c")).unwrap();
        assert_eq!(doc.name, "grouped");
        assert_eq!(doc.animations.len(), 4);
    }

    #[test]
    fn parses_all_anim_types() {
        let doc = parse_inc_c(&fixture("all_anim_types.inc.c")).unwrap();
        let types: Vec<JsonAnimType> = doc.animations.iter().map(|a| a.anim_type).collect();
        assert_eq!(
            types,
            vec![JsonAnimType::OneShot, JsonAnimType::Loop, JsonAnimType::DestroyOnEnd]
        );
        let last = &doc.animations[2];
        assert!(last.flip);
        assert_eq!(last.metadata_index, 8191);
    }

    #[test]
    fn rejects_garbage_in_body() {
        let txt = r#"#include "x.h"

u16 xAnimationScripts[] = {
	garbage_line,
};
"#;
        let err = parse_inc_c_text(PathBuf::from("x.inc.c"), txt).unwrap_err();
        assert!(matches!(err, IoError::IncCParse { line: 4, .. }), "got: {err:?}");
    }

    #[test]
    fn rejects_out_of_range_offset() {
        let txt = r#"#include "x.h"

u16 xAnimationScripts[] = {
	PACK_ANIM_DATA(9999, ANIM_TYPE_LOOP, 0),
};
"#;
        let err = parse_inc_c_text(PathBuf::from("x.inc.c"), txt).unwrap_err();
        assert!(matches!(err, IoError::IncCParse { .. }), "got: {err:?}");
    }

    #[test]
    fn name_strips_animation_scripts_suffix() {
        let txt = "u16 fooAnimationScripts[] = {\n};\n";
        let doc = parse_inc_c_text(PathBuf::from("foo.inc.c"), txt).unwrap();
        assert_eq!(doc.name, "foo");
    }

    #[test]
    fn entry_labels_default_to_index() {
        let txt = "u16 xAnimationScripts[] = {\n\tPACK_ANIM_DATA(0, ANIM_TYPE_LOOP, 0),\n\tPACK_ANIM_DATA(1, ANIM_TYPE_LOOP, 0),\n};\n";
        let doc = parse_inc_c_text(PathBuf::from("x.inc.c"), txt).unwrap();
        assert_eq!(doc.animations[0].label, "0");
        assert_eq!(doc.animations[1].label, "1");
    }
}
