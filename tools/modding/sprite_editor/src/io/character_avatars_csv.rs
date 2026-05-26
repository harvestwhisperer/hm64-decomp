//! Parser for `tools/libhm64/data/character_avatars.csv`.
//!
//! Three columns: `index,character,expression`. Provides the canonical
//! `(character, expression)` labels keyed by ROM-order avatar index — used to
//! seed `characterAvatars.json` when scaffolding from baserom and to keep the
//! sprite editor's defaults in sync with the libhm64 text transpiler.

use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AvatarLabel {
    pub character: String,
    pub expression: String,
}

#[derive(Debug, Default)]
pub struct AvatarLabelTable {
    entries: HashMap<u32, AvatarLabel>,
}

impl AvatarLabelTable {
    pub fn lookup(&self, index: u32) -> Option<&AvatarLabel> {
        self.entries.get(&index)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

#[derive(Debug, thiserror::Error)]
pub enum AvatarCsvError {
    #[error("line {line}: expected 3 columns, got {got}")]
    BadColumnCount { line: usize, got: usize },
    #[error("line {line}: invalid index `{value}`")]
    BadIndex { line: usize, value: String },
    #[error("duplicate index {index} at lines {first} and {second}")]
    Duplicate { index: u32, first: usize, second: usize },
}

pub fn parse(text: &str) -> Result<AvatarLabelTable, AvatarCsvError> {
    let mut table = AvatarLabelTable::default();
    let mut origins: HashMap<u32, usize> = HashMap::new();

    for (i, line) in text.lines().enumerate() {
        let line_num = i + 1;
        if i == 0 || line.trim().is_empty() {
            continue;
        }

        let cols: Vec<&str> = line.split(',').collect();
        if cols.len() != 3 {
            return Err(AvatarCsvError::BadColumnCount { line: line_num, got: cols.len() });
        }

        let index = cols[0]
            .trim()
            .parse::<u32>()
            .map_err(|_| AvatarCsvError::BadIndex { line: line_num, value: cols[0].to_string() })?;

        if let Some(&first) = origins.get(&index) {
            return Err(AvatarCsvError::Duplicate { index, first, second: line_num });
        }
        origins.insert(index, line_num);

        table.entries.insert(
            index,
            AvatarLabel {
                character: cols[1].trim().to_string(),
                expression: cols[2].trim().to_string(),
            },
        );
    }

    Ok(table)
}

/// Embed the sprite editor's own bundled CSV at build time.
pub fn embedded_table() -> Result<AvatarLabelTable, AvatarCsvError> {
    parse(include_str!("../../data/character_avatars.csv"))
}

#[cfg(test)]
mod tests {
    use super::*;

    const HEADER: &str = "index,character,expression\n";

    #[test]
    fn parses_empty_csv() {
        let t = parse(HEADER).unwrap();
        assert!(t.is_empty());
    }

    #[test]
    fn parses_full_row() {
        let csv = format!("{HEADER}0,MARIA,1\n");
        let t = parse(&csv).unwrap();
        let e = t.lookup(0).unwrap();
        assert_eq!(e.character, "MARIA");
        assert_eq!(e.expression, "1");
    }

    #[test]
    fn rejects_bad_column_count() {
        let csv = format!("{HEADER}0,MARIA\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, AvatarCsvError::BadColumnCount { line: 2, got: 2 }));
    }

    #[test]
    fn rejects_bad_index() {
        let csv = format!("{HEADER}abc,MARIA,1\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, AvatarCsvError::BadIndex { line: 2, .. }));
    }

    #[test]
    fn rejects_duplicate_index() {
        let csv = format!("{HEADER}0,MARIA,1\n0,POPURI,2\n");
        let err = parse(&csv).unwrap_err();
        assert!(matches!(err, AvatarCsvError::Duplicate { index: 0, .. }));
    }

    #[test]
    fn skips_blank_lines() {
        let csv = format!("{HEADER}\n0,MARIA,1\n\n");
        let t = parse(&csv).unwrap();
        assert_eq!(t.len(), 1);
    }

    #[test]
    fn embedded_table_parses_cleanly_and_covers_143_entries() {
        let t = embedded_table().expect("embedded CSV must parse");
        assert_eq!(t.len(), 143);
        // Spot-check a few canonical entries that the libhm64 text transpiler
        // and existing assets/text references depend on.
        assert_eq!(t.lookup(0).unwrap().character, "MARIA");
        assert_eq!(t.lookup(0).unwrap().expression, "1");
        assert_eq!(t.lookup(63).unwrap().character, "HARVEST_GODDESS");
        assert_eq!(t.lookup(63).unwrap().expression, "1");
        assert_eq!(t.lookup(142).unwrap().character, "CLIFF");
        assert_eq!(t.lookup(142).unwrap().expression, "4");
    }

    #[test]
    fn bundled_csv_matches_libhm64_canonical() {
        let bundled = include_str!("../../data/character_avatars.csv");
        let libhm64_path =
            std::path::Path::new(env!("CARGO_MANIFEST_DIR")).join("../../libhm64/data/character_avatars.csv");
        let libhm64 = match std::fs::read_to_string(&libhm64_path) {
            Ok(s) => s,
            Err(_) => return, // standalone build — no sibling to compare against
        };
        assert_eq!(
            bundled, libhm64,
            "sprite editor's bundled character_avatars.csv has drifted from {}",
            libhm64_path.display()
        );
    }
}
