//! Parser for `tools/modding/sprite_editor/data/animation_labels.csv`.
//!
//! Sparse format: only rows with meaningful content (label, group, or both)
//! appear. Used by the ROM extractor to seed inline labels in per-script JSON.

use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LabelEntry {
    pub label: Option<String>,
    pub group: Option<String>,
    pub group_role: Option<GroupRole>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum GroupRole {
    S, Sw, W, Nw, N, Ne, E, Se,
}

impl GroupRole {
    pub fn parse(s: &str) -> Option<Self> {
        match s {
            "S" => Some(Self::S),
            "SW" => Some(Self::Sw),
            "W" => Some(Self::W),
            "NW" => Some(Self::Nw),
            "N" => Some(Self::N),
            "NE" => Some(Self::Ne),
            "E" => Some(Self::E),
            "SE" => Some(Self::Se),
            _ => None,
        }
    }

    pub fn as_str(self) -> &'static str {
        match self {
            Self::S => "S",
            Self::Sw => "SW",
            Self::W => "W",
            Self::Nw => "NW",
            Self::N => "N",
            Self::Ne => "NE",
            Self::E => "E",
            Self::Se => "SE",
        }
    }
}

/// Lookup table from (sprite, index) → label entry.
#[derive(Debug, Default)]
pub struct LabelTable {
    entries: HashMap<(String, u32), LabelEntry>,
}

impl LabelTable {
    pub fn lookup(&self, sprite: &str, index: u32) -> Option<&LabelEntry> {
        self.entries.get(&(sprite.to_string(), index))
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

#[derive(Debug, thiserror::Error)]
pub enum LabelCsvError {
    #[error("line {line}: expected 5 columns, got {got}")]
    BadColumnCount { line: usize, got: usize },
    #[error("line {line}: invalid index `{value}`")]
    BadIndex { line: usize, value: String },
    #[error("line {line}: empty sprite name")]
    EmptySprite { line: usize },
    #[error("line {line}: invalid group_role `{value}` (expected S/SW/W/NW/N/NE/E/SE)")]
    BadGroupRole { line: usize, value: String },
    #[error("line {line}: group and group_role must both be present or both empty")]
    MismatchedGroup { line: usize },
    #[error("duplicate (sprite={sprite}, index={index}) at lines {first} and {second}")]
    Duplicate { sprite: String, index: u32, first: usize, second: usize },
}

pub fn parse(text: &str) -> Result<LabelTable, LabelCsvError> {
    let mut table = LabelTable::default();
    let mut origins: HashMap<(String, u32), usize> = HashMap::new();

    for (i, line) in text.lines().enumerate() {
        let line_num = i + 1;
        // Skip header and blank lines.
        if i == 0 || line.trim().is_empty() {
            continue;
        }

        let cols: Vec<&str> = line.split(',').collect();
        if cols.len() != 5 {
            return Err(LabelCsvError::BadColumnCount { line: line_num, got: cols.len() });
        }

        let sprite = cols[0].trim();
        if sprite.is_empty() {
            return Err(LabelCsvError::EmptySprite { line: line_num });
        }

        let index = cols[1]
            .trim()
            .parse::<u32>()
            .map_err(|_| LabelCsvError::BadIndex { line: line_num, value: cols[1].to_string() })?;

        let label = cols[2].trim();
        let group = cols[3].trim();
        let role = cols[4].trim();

        if (group.is_empty()) != (role.is_empty()) {
            return Err(LabelCsvError::MismatchedGroup { line: line_num });
        }

        let group_role = if role.is_empty() {
            None
        } else {
            Some(GroupRole::parse(role).ok_or_else(|| LabelCsvError::BadGroupRole {
                line: line_num,
                value: role.to_string(),
            })?)
        };

        let entry = LabelEntry {
            label: if label.is_empty() { None } else { Some(label.to_string()) },
            group: if group.is_empty() { None } else { Some(group.to_string()) },
            group_role,
        };

        let key = (sprite.to_string(), index);
        if let Some(&first) = origins.get(&key) {
            return Err(LabelCsvError::Duplicate {
                sprite: sprite.to_string(),
                index,
                first,
                second: line_num,
            });
        }
        origins.insert(key.clone(), line_num);
        table.entries.insert(key, entry);
    }

    Ok(table)
}

/// Embed the repo's CSV at build time so the standalone-without-decomp use
/// case works without external file dependencies.
pub fn embedded_table() -> Result<LabelTable, LabelCsvError> {
    parse(include_str!("../../data/animation_labels.csv"))
}

#[cfg(test)]
mod tests {
    use super::*;

    const HEADER: &str = "sprite,index,label,group,group_role\n";

    #[test]
    fn parses_empty_csv() {
        let t = parse(HEADER).unwrap();
        assert!(t.is_empty());
    }

    #[test]
    fn parses_label_only_row() {
        let csv = format!("{HEADER}cat,5,CAT_ANIM_SLEEPING,,\n");
        let t = parse(&csv).unwrap();
        let e = t.lookup("cat", 5).unwrap();
        assert_eq!(e.label.as_deref(), Some("CAT_ANIM_SLEEPING"));
        assert!(e.group.is_none());
        assert!(e.group_role.is_none());
    }

    #[test]
    fn parses_full_row() {
        let csv = format!("{HEADER}player,0,PLAYER_ANIM_STANDING_S,STANDING,S\n");
        let t = parse(&csv).unwrap();
        let e = t.lookup("player", 0).unwrap();
        assert_eq!(e.label.as_deref(), Some("PLAYER_ANIM_STANDING_S"));
        assert_eq!(e.group.as_deref(), Some("STANDING"));
        assert_eq!(e.group_role, Some(GroupRole::S));
    }

    #[test]
    fn parses_group_only_row() {
        let csv = format!("{HEADER}player,8,,WALKING,SW\n");
        let t = parse(&csv).unwrap();
        let e = t.lookup("player", 8).unwrap();
        assert!(e.label.is_none());
        assert_eq!(e.group.as_deref(), Some("WALKING"));
        assert_eq!(e.group_role, Some(GroupRole::Sw));
    }

    #[test]
    fn rejects_mismatched_group() {
        let csv = format!("{HEADER}player,0,X,STANDING,\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, LabelCsvError::MismatchedGroup { line: 2 }));
    }

    #[test]
    fn rejects_bad_group_role() {
        let csv = format!("{HEADER}player,0,X,STANDING,DOWN\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, LabelCsvError::BadGroupRole { line: 2, .. }));
    }

    #[test]
    fn rejects_duplicate_keys() {
        let csv = format!("{HEADER}player,0,A,,\nplayer,0,B,,\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, LabelCsvError::Duplicate { .. }));
    }

    #[test]
    fn rejects_bad_column_count() {
        let csv = format!("{HEADER}player,0,A,B\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, LabelCsvError::BadColumnCount { line: 2, got: 4 }));
    }

    #[test]
    fn rejects_bad_index() {
        let csv = format!("{HEADER}player,abc,A,,\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, LabelCsvError::BadIndex { line: 2, .. }));
    }

    #[test]
    fn skips_blank_lines() {
        let csv = format!("{HEADER}\ncat,5,X,,\n\n");
        let t = parse(&csv).unwrap();
        assert_eq!(t.len(), 1);
    }

    #[test]
    fn embedded_table_parses_cleanly() {
        // The repo CSV must always be valid.
        embedded_table().expect("embedded CSV must parse");
    }
}
