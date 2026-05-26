//! Integration test: a 50-pixel paint drag on the same target collapses to
//! one undo step under the merge window.

use std::collections::BTreeMap;
use std::path::PathBuf;

use hm64_sprite_editor::cmd::font_palette_cmds::SetFontPaletteColor;
use hm64_sprite_editor::cmd::font_pixel_cmds::SetGlyphPixel;
use hm64_sprite_editor::model::{ActiveAsset, FontAsset, FontGlyph, FontPalette, GlyphUnusedBytes, Rgba};
use hm64_sprite_editor::state::editor::Editor;

fn editor_with_font() -> Editor {
    let glyphs = vec![FontGlyph { index: 0, pixels: [0; 14 * 14], unused: GlyphUnusedBytes::default() }];
    let mut palettes = BTreeMap::new();
    palettes.insert(1, FontPalette { index: 1, colors: [Rgba::default(); 16] });
    let asset = FontAsset {
        dir: PathBuf::new(),
        name: "test".into(),
        glyphs,
        palettes,
    };
    let mut e = Editor::new();
    e.loaded_font = Some(asset);
    e.active = ActiveAsset::Font;
    e
}

#[test]
fn pixel_drag_50_strokes_collapses_to_one_undo() {
    let mut e = editor_with_font();
    // 50 strokes on the same pixel target (pixel_index=10) within the merge window.
    for v in 0..50u8 {
        let cmd = SetGlyphPixel::new(0, 10, v % 4, &e).unwrap();
        e.run(Box::new(cmd)).unwrap();
    }
    // Undo all pushed commands, count steps.
    let mut steps = 0;
    while e.undo().unwrap().is_some() {
        steps += 1;
    }
    assert_eq!(steps, 1, "merged drag should be one undo step, got {steps}");
}

#[test]
fn palette_drag_collapses_to_one_undo() {
    let mut e = editor_with_font();
    for v in 0..50u8 {
        let cmd = SetFontPaletteColor::new(1, 0, Rgba::new(v * 5, 0, 0, 255), &e).unwrap();
        e.run(Box::new(cmd)).unwrap();
    }
    let mut steps = 0;
    while e.undo().unwrap().is_some() {
        steps += 1;
    }
    assert_eq!(steps, 1, "merged palette drag should be one undo step, got {steps}");
}
