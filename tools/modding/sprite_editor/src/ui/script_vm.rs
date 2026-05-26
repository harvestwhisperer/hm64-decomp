//! Script tab view-model: exposes animation-data entries to the UI.

use std::collections::HashSet;

use slint::SharedString;

use crate::model::{AnimationData, JsonAnimType, DIRECTIONAL_GROUP_SIZE, DIRECTION_NAMES};
use crate::ui::slint_generated::ScriptEntryRow;

/// Display-kind tag for the Script panel, exposed as an `int` to Slint.
///
///   0 = entity (per-script list, optional dropdown)
///   1 = avatar (single 143-entry table)
///   2 = standalone (single label list)
///   3 = none (nothing loaded)
pub const DISPLAY_KIND_ENTITY: i32 = 0;
pub const DISPLAY_KIND_AVATAR: i32 = 1;
pub const DISPLAY_KIND_STANDALONE: i32 = 2;
pub const DISPLAY_KIND_NONE: i32 = 3;

#[derive(Debug, Clone)]
pub struct EntryRow {
    pub entry_index: u32,
    pub is_break: bool,
    pub label: String,
    pub offset: u16,
    pub type_short: &'static str,
    pub flip: bool,
    /// True for entity entries whose `EntityAnimation.directional` is set;
    /// always false for avatar / standalone rows.
    pub directional: bool,
    /// Synthetic clickable row that introduces a directional group. When set,
    /// `entry_index` is the base entry-index of the group, `group_collapsed`
    /// indicates current expand state, and `group_size` is the number of
    /// child entries (always 8 today). The data fields above are unused on
    /// header rows.
    pub is_group_header: bool,
    pub group_collapsed: bool,
    pub group_size: u32,
}

impl From<EntryRow> for ScriptEntryRow {
    fn from(r: EntryRow) -> Self {
        ScriptEntryRow {
            entry_index: r.entry_index as i32,
            is_break: r.is_break,
            label: r.label.into(),
            offset: r.offset as i32,
            type_short: SharedString::from(r.type_short),
            flip: r.flip,
            directional: r.directional,
            is_group_header: r.is_group_header,
            group_collapsed: r.group_collapsed,
            group_size: r.group_size as i32,
        }
    }
}

fn empty_entry_row(entry_index: u32) -> EntryRow {
    EntryRow {
        entry_index,
        is_break: false,
        label: String::new(),
        offset: 0,
        type_short: "",
        flip: false,
        directional: false,
        is_group_header: false,
        group_collapsed: false,
        group_size: 0,
    }
}

/// Joins an avatar entry's `character` and `expression` for display in the
/// row list. Empty parts collapse so the label never has a leading or
/// trailing underscore.
pub fn avatar_row_label(character: &str, expression: &str) -> String {
    match (character.is_empty(), expression.is_empty()) {
        (true, true) => String::new(),
        (true, false) => expression.to_string(),
        (false, true) => character.to_string(),
        (false, false) => format!("{character}_{expression}"),
    }
}

fn short_type_json(t: &JsonAnimType) -> &'static str {
    match t {
        JsonAnimType::OneShot => "ONCE",
        JsonAnimType::Loop => "LOOP",
        JsonAnimType::DestroyOnEnd => "DEST",
    }
}

/// Build display rows for a JSON-loaded `AnimationData` value.
///
/// `selected_script_idx` only matters for the `Entity` variant; it's ignored
/// for `Avatar` and `Standalone`. Out-of-range indices yield no rows.
///
/// `collapsed_groups` is a set of base entry-indices for directional groups
/// the user has collapsed; only consulted for `Entity` data.
pub fn rows_from_animation_data(
    data: &AnimationData,
    selected_script_idx: usize,
    collapsed_groups: &HashSet<u32>,
) -> Vec<EntryRow> {
    match data {
        AnimationData::Entity { scripts } => {
            let Some(doc) = scripts.get(selected_script_idx) else {
                return Vec::new();
            };
            let n = doc.animations.len();
            let mut rows = Vec::with_capacity(n);
            let mut i = 0;
            while i < n {
                let base = i as u32;
                let group_size = DIRECTIONAL_GROUP_SIZE;
                // Groups can start at any index — detect by looking ahead
                // for 8 consecutive directional entries from `i`.
                let is_group_start = doc.animations[i].directional
                    && i + group_size <= n
                    && (i..i + group_size).all(|j| doc.animations[j].directional);
                if is_group_start {
                    let collapsed = collapsed_groups.contains(&base);
                    // Header row carries the group's label (= base entry's
                    // label) so the Slint header can display it alongside
                    // the entry-range summary.
                    let group_label = doc.animations[i].label.clone();
                    rows.push(EntryRow {
                        is_group_header: true,
                        group_collapsed: collapsed,
                        group_size: DIRECTIONAL_GROUP_SIZE as u32,
                        entry_index: base,
                        label: group_label,
                        ..empty_entry_row(base)
                    });
                    if !collapsed {
                        for j in i..i + group_size {
                            let a = &doc.animations[j];
                            rows.push(EntryRow {
                                entry_index: j as u32,
                                is_break: false,
                                // Child rows in a group display only the
                                // direction suffix (e.g. "SW"). The group
                                // label appears once on the header row.
                                label: DIRECTION_NAMES[j - i].to_string(),
                                offset: a.metadata_index,
                                type_short: short_type_json(&a.anim_type),
                                flip: a.flip,
                                directional: a.directional,
                                is_group_header: false,
                                group_collapsed: false,
                                group_size: 0,
                            });
                        }
                    }
                    i += group_size;
                } else {
                    let a = &doc.animations[i];
                    rows.push(EntryRow {
                        entry_index: i as u32,
                        is_break: false,
                        label: a.label.clone(),
                        offset: a.metadata_index,
                        type_short: short_type_json(&a.anim_type),
                        flip: a.flip,
                        directional: a.directional,
                        is_group_header: false,
                        group_collapsed: false,
                        group_size: 0,
                    });
                    i += 1;
                }
            }
            rows
        }
        AnimationData::Avatar(t) => t
            .entries
            .iter()
            .enumerate()
            .map(|(i, e)| EntryRow {
                entry_index: i as u32,
                is_break: false,
                label: avatar_row_label(&e.character, &e.expression),
                offset: e.metadata_index,
                type_short: "",
                flip: false,
                directional: false,
                is_group_header: false,
                group_collapsed: false,
                group_size: 0,
            })
            .collect(),
        AnimationData::Standalone(s) => s
            .labels
            .iter()
            .enumerate()
            .map(|(i, l)| EntryRow {
                entry_index: i as u32,
                is_break: false,
                label: l.label.clone(),
                offset: l.metadata_index,
                type_short: "",
                flip: false,
                directional: false,
                is_group_header: false,
                group_collapsed: false,
                group_size: 0,
            })
            .collect(),
    }
}

/// Title string for the Script tab GroupBox when driven by JSON data.
pub fn header_for_animation_data(data: &AnimationData, selected_script_idx: usize) -> String {
    match data {
        AnimationData::Entity { scripts } => {
            let name = scripts
                .get(selected_script_idx)
                .map(|d| d.name.as_str())
                .unwrap_or("");
            if name.is_empty() {
                "Entity".to_string()
            } else {
                format!("Entity: {name}")
            }
        }
        AnimationData::Avatar(t) => format!("Avatar table ({} entries)", t.entries.len()),
        AnimationData::Standalone(s) => format!("Standalone labels ({})", s.sprite),
    }
}

/// List of script names for the dropdown selector. Empty unless `data` is an
/// `Entity` with more than one script.
pub fn script_options_for_animation_data(data: &AnimationData) -> Vec<String> {
    match data {
        AnimationData::Entity { scripts } if scripts.len() > 1 => {
            scripts.iter().map(|d| d.name.clone()).collect()
        }
        _ => Vec::new(),
    }
}

/// Display-kind tag (see `DISPLAY_KIND_*` constants) for the given data.
pub fn display_kind_for_animation_data(data: &AnimationData) -> i32 {
    match data {
        AnimationData::Entity { .. } => DISPLAY_KIND_ENTITY,
        AnimationData::Avatar(_) => DISPLAY_KIND_AVATAR,
        AnimationData::Standalone(_) => DISPLAY_KIND_STANDALONE,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    use crate::model::{
        AvatarEntry, AvatarTable, EntityAnimation, EntityScriptDoc, StandaloneLabel,
        StandaloneLabels,
    };

    fn entity_data(scripts: &[(&str, &[(&str, u16, JsonAnimType, bool)])]) -> AnimationData {
        AnimationData::Entity {
            scripts: scripts
                .iter()
                .map(|(name, anims)| EntityScriptDoc {
                    kind: "entity".into(),
                    sprite: "sprite".into(),
                    animations: anims
                        .iter()
                        .map(|(label, idx, ty, flip)| EntityAnimation {
                            label: (*label).into(),
                            metadata_index: *idx,
                            anim_type: *ty,
                            flip: *flip,
                            directional: false,
                        })
                        .collect(),
                    trailing_zeros: 0,
                    name: (*name).into(),
                })
                .collect(),
        }
    }

    fn avatar_data(entries: &[(&str, &str, u16)]) -> AnimationData {
        AnimationData::Avatar(AvatarTable {
            kind: "avatar".into(),
            entries: entries
                .iter()
                .map(|(c, e, i)| AvatarEntry {
                    character: (*c).into(),
                    expression: (*e).into(),
                    metadata_index: *i,
                })
                .collect(),
        })
    }

    fn standalone_data(sprite: &str, labels: &[(u16, &str)]) -> AnimationData {
        AnimationData::Standalone(StandaloneLabels {
            kind: "standalone".into(),
            sprite: sprite.into(),
            labels: labels
                .iter()
                .map(|(i, l)| StandaloneLabel {
                    metadata_index: *i,
                    label: (*l).into(),
                })
                .collect(),
        })
    }

    #[test]
    fn rows_from_entity_uses_selected_script() {
        let d = entity_data(&[
            ("a", &[("0", 1, JsonAnimType::Loop, false)]),
            (
                "b",
                &[
                    ("x", 4, JsonAnimType::OneShot, true),
                    ("y", 5, JsonAnimType::DestroyOnEnd, false),
                ],
            ),
        ]);
        let rows0 = rows_from_animation_data(&d, 0, &HashSet::new());
        assert_eq!(rows0.len(), 1);
        assert_eq!(rows0[0].label, "0");
        assert_eq!(rows0[0].offset, 1);
        assert_eq!(rows0[0].type_short, "LOOP");
        let rows1 = rows_from_animation_data(&d, 1, &HashSet::new());
        assert_eq!(rows1.len(), 2);
        assert_eq!(rows1[0].type_short, "ONCE");
        assert!(rows1[0].flip);
        assert_eq!(rows1[1].type_short, "DEST");
    }

    #[test]
    fn rows_from_entity_oob_index_is_empty() {
        let d = entity_data(&[("a", &[("0", 1, JsonAnimType::Loop, false)])]);
        assert!(rows_from_animation_data(&d, 5, &HashSet::new()).is_empty());
    }

    #[test]
    fn rows_from_entity_carries_directional_flag() {
        // Two entries, one directional, one not.
        let d = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "s".into(),
                animations: vec![
                    EntityAnimation {
                        label: "a".into(),
                        metadata_index: 0,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: true,
                    },
                    EntityAnimation {
                        label: "b".into(),
                        metadata_index: 1,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: false,
                    },
                ],
                trailing_zeros: 0,
                name: "x".into(),
            }],
        };
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        assert!(rows[0].directional);
        assert!(!rows[1].directional);
    }

    #[test]
    fn rows_from_avatar_and_standalone_directional_always_false() {
        let av = avatar_data(&[("MARIA", "HAPPY", 0)]);
        assert!(!rows_from_animation_data(&av, 0, &HashSet::new())[0].directional);
        let st = standalone_data("s", &[(0, "x")]);
        assert!(!rows_from_animation_data(&st, 0, &HashSet::new())[0].directional);
    }

    #[test]
    fn rows_from_avatar_omits_type_and_flip() {
        let d = avatar_data(&[("", "0", 7), ("MARIA", "HAPPY", 11)]);
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        assert_eq!(rows.len(), 2);
        // empty character collapses; expression alone is the label.
        assert_eq!(rows[0].label, "0");
        assert_eq!(rows[1].label, "MARIA_HAPPY");
        assert_eq!(rows[1].offset, 11);
        assert_eq!(rows[1].type_short, "");
        assert!(!rows[1].flip);
    }

    #[test]
    fn rows_from_standalone_omits_type_and_flip() {
        let d = standalone_data("balloons", &[(0, "pop"), (3, "drift")]);
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        assert_eq!(rows.len(), 2);
        assert_eq!(rows[0].label, "pop");
        assert_eq!(rows[0].offset, 0);
        assert_eq!(rows[1].offset, 3);
        assert_eq!(rows[0].type_short, "");
    }

    #[test]
    fn header_entity_single_uses_script_name() {
        let d = entity_data(&[("bunny", &[])]);
        assert_eq!(header_for_animation_data(&d, 0), "Entity: bunny");
    }

    #[test]
    fn header_entity_multi_uses_selected_name() {
        let d = entity_data(&[("apple", &[]), ("zebra", &[])]);
        assert_eq!(header_for_animation_data(&d, 0), "Entity: apple");
        assert_eq!(header_for_animation_data(&d, 1), "Entity: zebra");
    }

    #[test]
    fn header_avatar_includes_count() {
        let d = avatar_data(&[("", "0", 0), ("", "1", 0), ("", "2", 0)]);
        assert_eq!(header_for_animation_data(&d, 0), "Avatar table (3 entries)");
    }

    #[test]
    fn header_standalone_includes_sprite() {
        let d = standalone_data("balloons", &[]);
        assert_eq!(
            header_for_animation_data(&d, 0),
            "Standalone labels (balloons)"
        );
    }

    #[test]
    fn script_options_only_for_multi_script_entity() {
        let single = entity_data(&[("bunny", &[])]);
        assert!(script_options_for_animation_data(&single).is_empty());

        let multi = entity_data(&[("apple", &[]), ("zebra", &[])]);
        assert_eq!(
            script_options_for_animation_data(&multi),
            vec!["apple".to_string(), "zebra".to_string()]
        );

        let av = avatar_data(&[("", "0", 0)]);
        assert!(script_options_for_animation_data(&av).is_empty());

        let st = standalone_data("balloons", &[]);
        assert!(script_options_for_animation_data(&st).is_empty());
    }

    #[test]
    fn display_kind_matches_variant() {
        assert_eq!(
            display_kind_for_animation_data(&entity_data(&[("a", &[])])),
            DISPLAY_KIND_ENTITY
        );
        assert_eq!(
            display_kind_for_animation_data(&avatar_data(&[])),
            DISPLAY_KIND_AVATAR
        );
        assert_eq!(
            display_kind_for_animation_data(&standalone_data("x", &[])),
            DISPLAY_KIND_STANDALONE
        );
    }

    fn entity_directional_8_then_2() -> AnimationData {
        AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "s".into(),
                animations: (0..10)
                    .map(|i| EntityAnimation {
                        label: i.to_string(),
                        metadata_index: i as u16,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: i < 8,
                    })
                    .collect(),
                trailing_zeros: 0,
                name: "x".into(),
            }],
        }
    }

    #[test]
    fn rows_from_entity_emits_group_header_for_8_aligned_directional_run() {
        let d = entity_directional_8_then_2();
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        // 1 header + 8 entries + 2 trailing non-directional = 11 rows
        assert_eq!(rows.len(), 11);
        assert!(rows[0].is_group_header);
        assert_eq!(rows[0].entry_index, 0);
        assert_eq!(rows[0].group_size, 8);
        assert!(!rows[0].group_collapsed);
        // Header carries the group's (= base entry's) label.
        assert_eq!(rows[0].label, "0");
        for j in 1..=8 {
            assert!(!rows[j].is_group_header);
            assert_eq!(rows[j].entry_index, (j - 1) as u32);
            // Child rows display direction suffixes (S..SE order; see DIRECTION_NAMES).
            assert_eq!(rows[j].label, DIRECTION_NAMES[j - 1]);
        }
        assert_eq!(rows[9].entry_index, 8);
        assert_eq!(rows[10].entry_index, 9);
    }

    #[test]
    fn rows_from_entity_collapses_group_when_in_set() {
        let d = entity_directional_8_then_2();
        let collapsed: HashSet<u32> = [0u32].into_iter().collect();
        let rows = rows_from_animation_data(&d, 0, &collapsed);
        // 1 header + 0 (collapsed) + 2 trailing = 3 rows
        assert_eq!(rows.len(), 3);
        assert!(rows[0].is_group_header);
        assert!(rows[0].group_collapsed);
        assert_eq!(rows[1].entry_index, 8);
        assert_eq!(rows[2].entry_index, 9);
    }

    #[test]
    fn rows_from_entity_no_group_header_when_run_is_short() {
        // 7 directional entries — short of 8, so no header.
        let d = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "s".into(),
                animations: (0..7)
                    .map(|i| EntityAnimation {
                        label: i.to_string(),
                        metadata_index: i as u16,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: true,
                    })
                    .collect(),
                trailing_zeros: 0,
                name: "x".into(),
            }],
        };
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        assert_eq!(rows.len(), 7);
        assert!(rows.iter().all(|r| !r.is_group_header));
    }

    #[test]
    fn rows_from_entity_groups_can_start_at_non_8_aligned_positions() {
        // Two non-directional entries, then 8 directional, then 2 more
        // non-directional. Group base is index 2.
        let d = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "s".into(),
                animations: (0..12)
                    .map(|i| EntityAnimation {
                        label: i.to_string(),
                        metadata_index: i as u16,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: (2..10).contains(&i),
                    })
                    .collect(),
                trailing_zeros: 0,
                name: "x".into(),
            }],
        };
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        // 2 leading + 1 header + 8 children + 2 trailing = 13 rows
        assert_eq!(rows.len(), 13);
        assert!(!rows[0].is_group_header);
        assert!(!rows[1].is_group_header);
        assert!(rows[2].is_group_header);
        assert_eq!(rows[2].entry_index, 2);
        assert_eq!(rows[2].label, "2");
        for j in 0..8 {
            assert_eq!(rows[3 + j].entry_index, (2 + j) as u32);
            assert_eq!(rows[3 + j].label, DIRECTION_NAMES[j]);
        }
        assert_eq!(rows[11].entry_index, 10);
        assert_eq!(rows[12].entry_index, 11);
    }

    #[test]
    fn rows_from_entity_two_back_to_back_groups() {
        // Indices [0..8] and [8..16] both directional → two groups, no
        // alignment requirement on the second one (it just happens to be at 8).
        let d = AnimationData::Entity {
            scripts: vec![EntityScriptDoc {
                kind: "entity".into(),
                sprite: "s".into(),
                animations: (0..16)
                    .map(|i| EntityAnimation {
                        label: i.to_string(),
                        metadata_index: i as u16,
                        anim_type: JsonAnimType::Loop,
                        flip: false,
                        directional: true,
                    })
                    .collect(),
                trailing_zeros: 0,
                name: "x".into(),
            }],
        };
        let rows = rows_from_animation_data(&d, 0, &HashSet::new());
        // 2 headers + 16 children = 18 rows
        assert_eq!(rows.len(), 18);
        assert!(rows[0].is_group_header && rows[0].entry_index == 0);
        assert!(rows[9].is_group_header && rows[9].entry_index == 8);
    }
}
