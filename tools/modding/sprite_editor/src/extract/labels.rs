//! Lookup wrapper around `crate::io::labels_csv` for use during script extraction.

use crate::io::labels_csv::{embedded_table, LabelCsvError, LabelTable};

pub fn embedded() -> Result<LabelTable, LabelCsvError> {
    embedded_table()
}
