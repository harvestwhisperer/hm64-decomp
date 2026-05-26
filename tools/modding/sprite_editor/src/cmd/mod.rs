//! Command pattern: every user edit is a Command with apply/revert. The
//! `History` layer coalesces rapid-fire edits (color slider, label typing)
//! and caps stack depth.

pub mod animation_cmds;
pub mod animation_data;
pub mod font_palette_cmds;
pub mod font_pixel_cmds;
pub mod history;
pub mod palette_cmds;
pub mod texture_cmds;

use bitflags::bitflags;
use thiserror::Error;

use crate::state::editor::Editor;

bitflags! {
    /// Domain bits used in two roles:
    /// 1. **Per-command:** every `Command::affects()` returns the set of
    ///    domains it touches (queried by `BaselineTracker` to track save
    ///    state). Multi-bit values are the norm here.
    /// 2. **Editor-state:** `Editor::dirty` is a single `DirtyMask` that
    ///    answers "is X domain unsaved?" via the named accessors below.
    #[derive(Debug, Default, Clone, Copy, Hash, PartialEq, Eq)]
    pub struct DirtyMask: u8 {
        const PALETTE        = 0b00000001;
        const TEXTURE        = 0b00000010;
        const ANIMATION_DATA = 0b00000100;
        const ANIMATION      = 0b00001000;
        const FONT_TEXTURE   = 0b00010000;
        const FONT_PALETTE   = 0b00100000;
    }
}

impl DirtyMask {
    pub fn palette(self) -> bool { self.contains(Self::PALETTE) }
    pub fn texture(self) -> bool { self.contains(Self::TEXTURE) }
    pub fn animation_data(self) -> bool { self.contains(Self::ANIMATION_DATA) }
    pub fn animation(self) -> bool { self.contains(Self::ANIMATION) }
    pub fn font_texture(self) -> bool { self.contains(Self::FONT_TEXTURE) }
    pub fn font_palette(self) -> bool { self.contains(Self::FONT_PALETTE) }
    /// True iff any single domain bit is set. Equivalent to `!self.is_empty()`.
    pub fn any(self) -> bool { !self.is_empty() }

    /// Yields the single-bit submasks present in `self`. Used by
    /// `BaselineTracker` to dispatch over each affected domain without an
    /// explicit per-domain `if` ladder.
    pub fn iter_bits(self) -> impl Iterator<Item = DirtyMask> {
        const BITS: [DirtyMask; 6] = [
            DirtyMask::PALETTE,
            DirtyMask::TEXTURE,
            DirtyMask::ANIMATION_DATA,
            DirtyMask::ANIMATION,
            DirtyMask::FONT_TEXTURE,
            DirtyMask::FONT_PALETTE,
        ];
        BITS.into_iter().filter(move |b| self.contains(*b))
    }
}

#[derive(Debug, PartialEq, Eq)]
pub enum MergeResult {
    Merged,
    Separate,
}

#[derive(Debug, Error)]
pub enum CmdError {
    #[error("invalid target: {0}")]
    InvalidTarget(String),
    #[error("invariant violated: {0}")]
    Invariant(String),
}

/// Every editor mutation implements this. `apply` takes the state forward;
/// `revert` takes it back. Both must be total functions given the state
/// was at the immediately-prior checkpoint.
pub trait Command: std::fmt::Debug + Send + std::any::Any {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError>;
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError>;
    fn describe(&self) -> String;
    fn affects(&self) -> DirtyMask;
    /// Opportunistically merge `other` into `self`. Called on the most-
    /// recently-applied command before pushing a new one — if merging, the
    /// new command is discarded and self retains the *latest* `new` value.
    fn try_merge(&mut self, _other: &dyn Command) -> MergeResult {
        MergeResult::Separate
    }
    /// Milliseconds since apply. Used by the merge window (500 ms by
    /// default). Implementations can ignore this.
    fn set_applied_at(&mut self, _millis_since_epoch: u128) {}
    fn applied_at(&self) -> u128 {
        0
    }
}
