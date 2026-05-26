//! Wiring layer: registers Slint callbacks against the `MainWindow`. Each
//! submodule owns one user-facing domain (palettes, textures, animations,
//! script, font, …) and exposes a single `install` function. `main.rs`
//! creates the `AppState`, calls every `install`, then enters the Slint
//! event loop.
//!
//! The split exists so that `main.rs` doesn't grow to thousands of lines of
//! near-identical `{ let editor = editor.clone(); window.on_xxx(...) }`
//! blocks. Helper functions specific to a domain (e.g. `publish_palette_rows`)
//! live in that domain's wiring module; helpers used across domains live in
//! `helpers.rs`.

use std::cell::{Cell, RefCell};
use std::collections::{HashMap, HashSet, VecDeque};
use std::path::PathBuf;
use std::rc::Rc;

use hm64_sprite_editor::state::editor::Editor;
use hm64_sprite_editor::ui::animations_vm::AnimationsVm;
use hm64_sprite_editor::ui::slint_generated::TreeRow;
use hm64_sprite_editor::ui::sprite_browser_vm;
use hm64_sprite_editor::ui::wizard_vm::WizardState;
use slint::VecModel;

pub mod animations;
pub mod app_actions;
pub mod font;
pub mod helpers;
pub mod new_sprite_wizard;
pub mod palettes;
pub mod playback_timer;
pub mod save_all;
pub mod script;
pub mod sprite_browser;
pub mod textures;
pub mod window_chrome;

/// HSV state for the color picker overlay. Kept across slider drags so the
/// hue and saturation aren't lost when the user dials value down to zero.
#[derive(Debug, Default)]
pub struct PickerState {
    pub hue: f32,
    pub sat: f32,
    pub val: f32,
    pub alpha: f32,
}

impl PickerState {
    pub fn new() -> Self {
        Self { hue: 0.0, sat: 0.0, val: 0.0, alpha: 1.0 }
    }
}

/// In-flight sprite drag in the left browser tree. Created by
/// `try_start_drag`, geometry queried by `drag_move`, consumed by
/// `commit_drag` / `cancel_drag`. `None` when no drag is active.
///
/// `source_abs_y` is intentionally *not* stored — `drag_move` receives the
/// source row's CURRENT absolute-Y on every call so geometry stays correct
/// when auto-scroll moves the source row on screen.
#[derive(Debug, Clone)]
pub struct SpriteDragState {
    pub source_sprite_index: i32,
    pub source_row_index: usize,
    pub source_category: String,
    pub source_group: Option<String>,
}

/// Bag of every `Rc<RefCell<…>>` (and equivalents) shared between Slint
/// callbacks. Cloning is cheap — every field is an `Rc`. Callback-install
/// functions take `&AppState`; each individual closure does
/// `let state = state.clone();` to capture its own owned handle by move.
#[derive(Clone)]
pub struct AppState {
    pub editor: Rc<RefCell<Editor>>,
    pub anims_vm: Rc<RefCell<AnimationsVm>>,
    pub picker_state: Rc<RefCell<PickerState>>,
    pub pending_delete_tex: Rc<RefCell<Option<u32>>>,
    pub tree: Rc<RefCell<sprite_browser_vm::SpriteTree>>,
    pub expanded: Rc<RefCell<HashSet<sprite_browser_vm::NodeKey>>>,
    pub browser_filter: Rc<RefCell<String>>,
    pub rows_model: Rc<VecModel<TreeRow>>,
    pub pending_tex: Rc<RefCell<VecDeque<(u32, usize)>>>,
    pub anim_row_map: Rc<RefCell<HashMap<u32, usize>>>,
    pub wizard: Rc<RefCell<WizardState>>,
    pub font_rows: Rc<RefCell<Vec<(String, PathBuf)>>>,
    pub font_root_expanded: Rc<Cell<bool>>,
    pub save_job: Rc<RefCell<Option<save_all::SaveAllJob>>>,
    pub sprite_drag: Rc<RefCell<Option<SpriteDragState>>>,
    pub new_folder: Rc<RefCell<NewFolderState>>,
}

/// In-flight new-subfolder dialog state. The dialog is opened from the "+"
/// button on a category header; `category` is fixed at open time and `name`
/// tracks the LineEdit.
#[derive(Debug, Default, Clone)]
pub struct NewFolderState {
    pub category: String,
    pub name: String,
}

impl AppState {
    pub fn new() -> Self {
        Self {
            editor: Rc::new(RefCell::new(Editor::new())),
            anims_vm: Rc::new(RefCell::new(AnimationsVm::new())),
            picker_state: Rc::new(RefCell::new(PickerState::new())),
            pending_delete_tex: Rc::new(RefCell::new(None)),
            tree: Rc::new(RefCell::new(sprite_browser_vm::SpriteTree::default())),
            expanded: Rc::new(RefCell::new(HashSet::new())),
            browser_filter: Rc::new(RefCell::new(String::new())),
            rows_model: Rc::new(VecModel::default()),
            pending_tex: Rc::new(RefCell::new(VecDeque::new())),
            anim_row_map: Rc::new(RefCell::new(HashMap::new())),
            wizard: Rc::new(RefCell::new(WizardState::default())),
            font_rows: Rc::new(RefCell::new(Vec::new())),
            font_root_expanded: Rc::new(Cell::new(true)),
            save_job: Rc::new(RefCell::new(None)),
            sprite_drag: Rc::new(RefCell::new(None)),
            new_folder: Rc::new(RefCell::new(NewFolderState::default())),
        }
    }
}
