//! Slint-generated component and struct types.
//!
//! `slint::include_modules!()` expands to a module containing every named
//! Slint component (`MainWindow`) and `struct` declared in `ui/*.slint`.
//! Centralising it here lets view-models in `crate::ui::*_vm` build those
//! struct types directly, instead of forcing the conversion into `main.rs`.

slint::include_modules!();
