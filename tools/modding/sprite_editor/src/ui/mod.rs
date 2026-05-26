//! Bridges between the Slint UI and the Rust editor state. Holds view-models
//! that translate model data into the simple property types Slint supports
//! (strings, numbers, brushes, `Image`, arrays of structs).

pub mod animations_vm;
pub mod color_math;
pub mod font_glyph_grid_vm;
pub mod font_palette_vm;
pub mod new_font_wizard_vm;
pub mod palettes_vm;
pub mod script_vm;
pub mod slint_generated;
pub mod sprite_browser_vm;
pub mod textures_vm;
pub mod wizard_vm;
