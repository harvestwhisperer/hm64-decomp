//! Writer for sprite-paired `.h` macro headers.
//!
//! Mirrors the deleted `macros_export.rs` (phase 2b.1) but rebuilt against
//! the JSON-source-of-truth model. Each `AnimationData` variant gets its own
//! prefix derivation and per-entry naming convention; see the per-variant
//! comments below for details.
//!
//! Output style:
//!
//! ```c
//! #ifndef _BUNNY_H_
//! #define _BUNNY_H_
//!
//! #define BUNNY_ANIMATION_IDLE   0
//! #define BUNNY_ANIMATION_WALK   1
//!
//! #endif
//! ```
//!
//! - Guard derived from `path.file_stem()` SCREAMING_SNAKE_CASE'd, sandwiched
//!   `_<STEM>_H_`.
//! - All `#define` names left-aligned to the longest, single space, then the
//!   integer index.
//! - LF newlines.
//! - Empty labels are skipped silently. If every label is empty (or there are
//!   no entries at all), the file still gets the guard + `#endif` block.

use std::path::Path;

use crate::export::{atomic_write, ExportError, ExportResult};
use crate::model::{AnimationData, DIRECTIONAL_GROUP_SIZE, DIRECTION_NAMES};

/// Convert a camelCase or PascalCase identifier into SCREAMING_SNAKE_CASE.
/// Splits on each uppercase boundary; runs of uppercase characters are kept
/// together (so e.g. `holdableItems` → `HOLDABLE_ITEMS`, `RGB` stays `RGB`).
/// Non-alphanumeric characters are passed through unchanged after upcasing
/// (callers shouldn't pass them, but it's harmless).
pub fn to_screaming_snake(s: &str) -> String {
    let mut out = String::with_capacity(s.len() + 4);
    let chars: Vec<char> = s.chars().collect();
    for (i, &c) in chars.iter().enumerate() {
        let prev_lower = i > 0 && chars[i - 1].is_ascii_lowercase();
        let prev_alpha = i > 0 && chars[i - 1].is_ascii_alphabetic();
        if c.is_ascii_uppercase() && i > 0 && prev_lower {
            out.push('_');
        } else if c.is_ascii_digit() && i > 0 && prev_alpha {
            // Treat digit boundary like a word break (e.g. `script1` → `SCRIPT_1`).
            // Actually skip this — keep `script1` → `SCRIPT1` to match the legacy
            // emitter; uncomment if a digit boundary becomes a problem.
        }
        out.push(c.to_ascii_uppercase());
    }
    out
}

fn guard_name(path: &Path) -> String {
    let stem = path
        .file_stem()
        .map(|s| s.to_string_lossy().to_string())
        .unwrap_or_else(|| "FILE".to_string());
    format!("_{}_H_", to_screaming_snake(&stem))
}

/// Pair of (define name, integer index). Sorted by index when emitted.
struct Define {
    name: String,
    value: usize,
}

fn collect_defines(data: &AnimationData, sprite_label: &str) -> Vec<Vec<Define>> {
    // Returns groups (one per script for entity, single group for others).
    match data {
        AnimationData::Entity { scripts } => scripts
            .iter()
            .map(|doc| {
                let prefix = format!("{}_ANIMATION", to_screaming_snake(&doc.name));
                let n = doc.animations.len();
                let mut defs: Vec<Define> = Vec::new();
                let mut i = 0;
                while i < n {
                    // Groups start wherever 8 consecutive entries are
                    // marked directional — no alignment requirement.
                    let is_group = doc.animations[i].directional
                        && i + DIRECTIONAL_GROUP_SIZE <= n
                        && (i..i + DIRECTIONAL_GROUP_SIZE)
                            .all(|j| doc.animations[j].directional);
                    if is_group {
                        // Group label = base entry's label. The group emits
                        // one `_DIRECTIONAL` macro (base index, used by
                        // setEntityDirectionalAnimation) plus 8 per-direction
                        // macros derived from the group label. Skip silently
                        // if the group has no label set.
                        let group_label = doc.animations[i].label.as_str();
                        if !group_label.is_empty() {
                            defs.push(Define {
                                name: format!("{prefix}_{group_label}_DIRECTIONAL"),
                                value: i,
                            });
                            for k in 0..DIRECTIONAL_GROUP_SIZE {
                                defs.push(Define {
                                    name: format!(
                                        "{prefix}_{group_label}_{}",
                                        DIRECTION_NAMES[k]
                                    ),
                                    value: i + k,
                                });
                            }
                        }
                        i += DIRECTIONAL_GROUP_SIZE;
                    } else {
                        let a = &doc.animations[i];
                        if !a.label.is_empty() {
                            defs.push(Define {
                                name: format!("{prefix}_{}", a.label),
                                value: i,
                            });
                        }
                        i += 1;
                    }
                }
                // Group entries arrive in source order; sort to keep the
                // file stable and grouped by ascending value.
                defs.sort_by_key(|d| d.value);
                defs
            })
            .collect(),
        AnimationData::Avatar(t) => {
            let prefix = "DIALOGUE_AVATARS";
            let defs: Vec<Define> = t
                .entries
                .iter()
                .enumerate()
                .filter_map(|(i, e)| {
                    if e.character.is_empty() || e.expression.is_empty() {
                        return None;
                    }
                    Some(Define {
                        name: format!("{prefix}_{}_{}", e.character, e.expression),
                        value: i,
                    })
                })
                .collect();
            vec![defs]
        }
        AnimationData::Standalone(s) => {
            let prefix = to_screaming_snake(sprite_label);
            let defs: Vec<Define> = s
                .labels
                .iter()
                .enumerate()
                .filter_map(|(i, l)| {
                    if l.label.is_empty() {
                        return None;
                    }
                    Some(Define {
                        name: format!("{prefix}_{}", l.label),
                        value: i,
                    })
                })
                .collect();
            vec![defs]
        }
    }
}

/// Lines emitted between the include guard and the macro block. Currently
/// only the Avatar variant uses this — `initialize.c` consumes the `extern`
/// declaration, so it must appear in the header regardless of whether any
/// names have been filled in yet.
fn prelude_lines(data: &AnimationData) -> &'static [&'static str] {
    match data {
        AnimationData::Avatar(_) => &["extern u8 characterAvatarCodesToAnimations[];", ""],
        AnimationData::Entity { .. } | AnimationData::Standalone(_) => &[],
    }
}

fn emit(data: &AnimationData, sprite_label: &str, path: &Path) -> String {
    let guard = guard_name(path);
    let groups = collect_defines(data, sprite_label);

    // Compute global longest name across all groups so all `#define` lines
    // line up regardless of which script they belong to.
    let longest = groups
        .iter()
        .flat_map(|g| g.iter())
        .map(|d| d.name.len())
        .max()
        .unwrap_or(0);

    let mut out = String::new();
    out.push_str("#ifndef ");
    out.push_str(&guard);
    out.push('\n');
    out.push_str("#define ");
    out.push_str(&guard);
    out.push('\n');
    out.push('\n');

    for line in prelude_lines(data) {
        out.push_str(line);
        out.push('\n');
    }

    let total_defines: usize = groups.iter().map(|g| g.len()).sum();
    if total_defines > 0 {
        let nonempty_groups: Vec<&Vec<Define>> =
            groups.iter().filter(|g| !g.is_empty()).collect();
        for (gi, group) in nonempty_groups.iter().enumerate() {
            for d in *group {
                let pad = longest.saturating_sub(d.name.len());
                out.push_str("#define ");
                out.push_str(&d.name);
                for _ in 0..pad {
                    out.push(' ');
                }
                out.push(' ');
                out.push_str(&d.value.to_string());
                out.push('\n');
            }
            if gi + 1 < nonempty_groups.len() {
                out.push('\n');
            }
        }
        out.push('\n');
    }

    out.push_str("#endif\n");
    out
}

/// Write the macro `.h` file for `data` to `path`. `sprite_label` is used
/// only for the standalone variant's prefix (entity uses each script's
/// `name`; avatar uses a fixed `DIALOGUE_AVATARS`).
pub fn write_macros_h(path: &Path, data: &AnimationData, sprite_label: &str) -> ExportResult<()> {
    let bytes = emit(data, sprite_label, path).into_bytes();
    atomic_write(path, &bytes).map_err(|e| ExportError::write(path, e))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{
        AvatarEntry, AvatarTable, EntityAnimation, EntityScriptDoc, JsonAnimType,
        StandaloneLabel, StandaloneLabels,
    };
    use std::path::PathBuf;

    #[test]
    fn screaming_snake_basic() {
        assert_eq!(to_screaming_snake("bunny"), "BUNNY");
        assert_eq!(to_screaming_snake("expressionBubbles"), "EXPRESSION_BUBBLES");
        assert_eq!(to_screaming_snake("holdableItems"), "HOLDABLE_ITEMS");
        assert_eq!(to_screaming_snake(""), "");
    }

    #[test]
    fn screaming_snake_keeps_acronym_runs_together() {
        // `RGB` stays `RGB`; `expressionRGB` → `EXPRESSION_RGB`.
        assert_eq!(to_screaming_snake("RGB"), "RGB");
        assert_eq!(to_screaming_snake("expressionRGB"), "EXPRESSION_RGB");
    }

    #[test]
    fn entity_emits_all_scripts_skipping_empty_labels() {
        let data = AnimationData::Entity {
            scripts: vec![
                EntityScriptDoc {
                    kind: "entity".into(),
                    sprite: "bunny".into(),
                    animations: vec![
                        EntityAnimation {
                            label: "IDLE".into(),
                            metadata_index: 0,
                            anim_type: JsonAnimType::Loop,
                            flip: false,
                            directional: false,
                        },
                        // Empty label — skipped.
                        EntityAnimation {
                            label: "".into(),
                            metadata_index: 1,
                            anim_type: JsonAnimType::Loop,
                            flip: false,
                            directional: false,
                        },
                        EntityAnimation {
                            label: "WALK".into(),
                            metadata_index: 2,
                            anim_type: JsonAnimType::Loop,
                            flip: false,
                            directional: false,
                        },
                    ],
                    trailing_zeros: 0,
                    name: "bunny".into(),
                },
                EntityScriptDoc {
                    kind: "entity".into(),
                    sprite: "bunny".into(),
                    animations: vec![EntityAnimation {
                        label: "POP".into(),
                        metadata_index: 0,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: false,
                    }],
                    trailing_zeros: 0,
                    name: "expressionBubbles".into(),
                },
            ],
        };
        let path = PathBuf::from("bunny.h");
        let out = emit(&data, "bunny", &path);
        let expected = "\
#ifndef _BUNNY_H_
#define _BUNNY_H_

#define BUNNY_ANIMATION_IDLE             0
#define BUNNY_ANIMATION_WALK             2

#define EXPRESSION_BUBBLES_ANIMATION_POP 0

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn entity_emits_directional_group_with_directional_and_per_direction_macros() {
        // 8-aligned all-directional run with shared label "WALKING", followed
        // by a single non-directional entry. Should produce one
        // `_DIRECTIONAL` macro (base 0) plus 8 per-direction macros, then a
        // plain entry macro at index 8.
        let group_anim = |i: usize| EntityAnimation {
            label: "WALKING".into(),
            metadata_index: i as u16,
            anim_type: JsonAnimType::Loop,
            flip: false,
            directional: true,
        };
        let mut animations: Vec<EntityAnimation> = (0..8).map(group_anim).collect();
        animations.push(EntityAnimation {
            label: "IDLE".into(),
            metadata_index: 8,
            anim_type: JsonAnimType::Loop,
            flip: false,
            directional: false,
        });
        let data = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "player".into(),
                animations,
                trailing_zeros: 0,
                name: "player".into(),
            }],
        };
        let path = PathBuf::from("player.h");
        let out = emit(&data, "player", &path);
        let expected = "\
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_ANIMATION_WALKING_DIRECTIONAL 0
#define PLAYER_ANIMATION_WALKING_S           0
#define PLAYER_ANIMATION_WALKING_SW          1
#define PLAYER_ANIMATION_WALKING_W           2
#define PLAYER_ANIMATION_WALKING_NW          3
#define PLAYER_ANIMATION_WALKING_N           4
#define PLAYER_ANIMATION_WALKING_NE          5
#define PLAYER_ANIMATION_WALKING_E           6
#define PLAYER_ANIMATION_WALKING_SE          7
#define PLAYER_ANIMATION_IDLE                8

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn entity_skips_unlabeled_directional_group() {
        // Group with empty label produces no macros for the group at all.
        let group_anim = |i: usize| EntityAnimation {
            label: "".into(),
            metadata_index: i as u16,
            anim_type: JsonAnimType::Loop,
            flip: false,
            directional: true,
        };
        let data = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "x".into(),
                animations: (0..8).map(group_anim).collect(),
                trailing_zeros: 0,
                name: "x".into(),
            }],
        };
        let path = PathBuf::from("x.h");
        let out = emit(&data, "x", &path);
        let expected = "\
#ifndef _X_H_
#define _X_H_

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn avatar_emits_skipping_empty_character_or_expression() {
        let data = AnimationData::Avatar(AvatarTable {
            kind: "avatar".into(),
            entries: vec![
                AvatarEntry {
                    character: "MARIA".into(),
                    expression: "HAPPY".into(),
                    metadata_index: 0,
                },
                // Missing character — skipped.
                AvatarEntry {
                    character: "".into(),
                    expression: "SAD".into(),
                    metadata_index: 1,
                },
                AvatarEntry {
                    character: "ELLI".into(),
                    expression: "ANGRY".into(),
                    metadata_index: 2,
                },
            ],
        });
        let path = PathBuf::from("characterAvatars.h");
        let out = emit(&data, "characterAvatars", &path);
        let expected = "\
#ifndef _CHARACTER_AVATARS_H_
#define _CHARACTER_AVATARS_H_

extern u8 characterAvatarCodesToAnimations[];

#define DIALOGUE_AVATARS_MARIA_HAPPY 0
#define DIALOGUE_AVATARS_ELLI_ANGRY  2

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn avatar_with_no_named_entries_still_emits_extern() {
        // Mirrors the Python emitter: the `extern` declaration is required by
        // initialize.c regardless of whether any macros have been named yet.
        let data = AnimationData::Avatar(AvatarTable {
            kind: "avatar".into(),
            entries: vec![AvatarEntry {
                character: "".into(),
                expression: "".into(),
                metadata_index: 0,
            }],
        });
        let path = PathBuf::from("characterAvatars.h");
        let out = emit(&data, "characterAvatars", &path);
        let expected = "\
#ifndef _CHARACTER_AVATARS_H_
#define _CHARACTER_AVATARS_H_

extern u8 characterAvatarCodesToAnimations[];

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn standalone_emits_with_sprite_label_prefix_skipping_empty() {
        let data = AnimationData::Standalone(StandaloneLabels {
            kind: "standalone".into(),
            sprite: "balloons".into(),
            labels: vec![
                StandaloneLabel { metadata_index: 0, label: "POP".into() },
                StandaloneLabel { metadata_index: 1, label: "DRIFT".into() },
                StandaloneLabel { metadata_index: 2, label: "".into() }, // skipped
                StandaloneLabel { metadata_index: 3, label: "BURST".into() },
            ],
        });
        let path = PathBuf::from("balloons.h");
        let out = emit(&data, "balloons", &path);
        let expected = "\
#ifndef _BALLOONS_H_
#define _BALLOONS_H_

#define BALLOONS_POP   0
#define BALLOONS_DRIFT 1
#define BALLOONS_BURST 3

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn empty_defines_emits_only_guard_block() {
        // Every label empty → no defines.
        let data = AnimationData::Standalone(StandaloneLabels {
            kind: "standalone".into(),
            sprite: "x".into(),
            labels: vec![StandaloneLabel { metadata_index: 0, label: "".into() }],
        });
        let path = PathBuf::from("empty.h");
        let out = emit(&data, "x", &path);
        let expected = "\
#ifndef _EMPTY_H_
#define _EMPTY_H_

#endif
";
        assert_eq!(out, expected);
    }

    #[test]
    fn write_macros_h_round_trips_via_disk() {
        let dir = tempfile::tempdir().unwrap();
        let path = dir.path().join("bunny.h");
        let data = AnimationData::Standalone(StandaloneLabels {
            kind: "standalone".into(),
            sprite: "bunny".into(),
            labels: vec![StandaloneLabel { metadata_index: 0, label: "X".into() }],
        });
        write_macros_h(&path, &data, "bunny").unwrap();
        let s = std::fs::read_to_string(&path).unwrap();
        assert!(s.starts_with("#ifndef _BUNNY_H_\n"));
        assert!(s.ends_with("#endif\n"));
        assert!(s.contains("#define BUNNY_X 0\n"));
    }
}
