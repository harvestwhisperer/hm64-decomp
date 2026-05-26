//! Commands that mutate per-animation metadata in
//! `loaded_sprite.animations` (a `BTreeMap<u32, Animation>`). Distinct
//! from `animation_data/`, which edits the script-side
//! `EntityAnimation` entries.

use once_cell::unsync::OnceCell;

use crate::cmd::{CmdError, Command, DirtyMask, MergeResult};
use crate::model::{Animation, AnimationFrame, FrameSprite};
use crate::state::editor::Editor;

/// Borrow the animation at `idx` mut, or return a typed CmdError.
fn anim_mut<'a>(state: &'a mut Editor, idx: u32) -> Result<&'a mut Animation, CmdError> {
    let s = state
        .current_sprite_mut()
        .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
    s.animations
        .get_mut(&idx)
        .ok_or_else(|| CmdError::InvalidTarget(format!("animation {idx} missing")))
}

// ---------- SetFrameDuration ----------

#[derive(Debug)]
pub struct SetFrameDuration {
    pub anim_idx: u32,
    pub frame_idx: usize,
    pub old: u16,
    pub new: u16,
    applied_at: u128,
}

impl SetFrameDuration {
    pub fn new(anim_idx: u32, frame_idx: usize, new: u16, editor: &Editor) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let old = a.frames.get(frame_idx)?.duration;
        Some(Self { anim_idx, frame_idx, old, new, applied_at: 0 })
    }
}

impl Command for SetFrameDuration {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("frame {} out of range", self.frame_idx))
        })?;
        f.duration = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: frame {} out of range", self.frame_idx))
        })?;
        f.duration = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set anim {} frame {} duration", self.anim_idx, self.frame_idx)
    }
    fn affects(&self) -> DirtyMask {
        DirtyMask::ANIMATION
    }
    fn try_merge(&mut self, other: &dyn Command) -> MergeResult {
        let Some(o) = (other as &dyn std::any::Any).downcast_ref::<SetFrameDuration>() else {
            return MergeResult::Separate;
        };
        if o.anim_idx == self.anim_idx && o.frame_idx == self.frame_idx {
            self.new = o.new;
            MergeResult::Merged
        } else {
            MergeResult::Separate
        }
    }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- SetFrameHasAudio ----------

#[derive(Debug)]
pub struct SetFrameHasAudio {
    pub anim_idx: u32,
    pub frame_idx: usize,
    pub old: u8,
    pub new: u8,
    applied_at: u128,
}

impl SetFrameHasAudio {
    pub fn new(anim_idx: u32, frame_idx: usize, new: u8, editor: &Editor) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let old = a.frames.get(frame_idx)?.has_audio;
        Some(Self { anim_idx, frame_idx, old, new, applied_at: 0 })
    }
}

impl Command for SetFrameHasAudio {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("frame {} out of range", self.frame_idx))
        })?;
        f.has_audio = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: frame {} out of range", self.frame_idx))
        })?;
        f.has_audio = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set anim {} frame {} has_audio", self.anim_idx, self.frame_idx)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- SetLayerTexture ----------

#[derive(Debug)]
pub struct SetLayerTexture {
    pub anim_idx: u32,
    pub frame_idx: usize,
    pub layer_idx: usize,
    pub old: u32,
    pub new: u32,
    applied_at: u128,
}

impl SetLayerTexture {
    pub fn new(
        anim_idx: u32,
        frame_idx: usize,
        layer_idx: usize,
        new: u32,
        editor: &Editor,
    ) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let f = a.frames.get(frame_idx)?;
        let old = f.sprites.get(layer_idx)?.spritesheet_index;
        Some(Self { anim_idx, frame_idx, layer_idx, old, new, applied_at: 0 })
    }
}

impl Command for SetLayerTexture {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("frame {} out of range", self.frame_idx))
        })?;
        let layer = f.sprites.get_mut(self.layer_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("layer {} out of range", self.layer_idx))
        })?;
        layer.spritesheet_index = self.new;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: frame {} out of range", self.frame_idx))
        })?;
        let layer = f.sprites.get_mut(self.layer_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: layer {} out of range", self.layer_idx))
        })?;
        layer.spritesheet_index = self.old;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set anim {} frame {} layer {} texture",
            self.anim_idx, self.frame_idx, self.layer_idx)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn try_merge(&mut self, other: &dyn Command) -> MergeResult {
        let Some(o) = (other as &dyn std::any::Any).downcast_ref::<SetLayerTexture>() else {
            return MergeResult::Separate;
        };
        if o.anim_idx == self.anim_idx
            && o.frame_idx == self.frame_idx
            && o.layer_idx == self.layer_idx
        {
            self.new = o.new;
            MergeResult::Merged
        } else {
            MergeResult::Separate
        }
    }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- SetLayerAnchor ----------

#[derive(Debug)]
pub struct SetLayerAnchor {
    pub anim_idx: u32,
    pub frame_idx: usize,
    pub layer_idx: usize,
    pub old: (i16, i16),
    pub new: (i16, i16),
    applied_at: u128,
}

impl SetLayerAnchor {
    pub fn new(
        anim_idx: u32,
        frame_idx: usize,
        layer_idx: usize,
        new: (i16, i16),
        editor: &Editor,
    ) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let f = a.frames.get(frame_idx)?;
        let s = f.sprites.get(layer_idx)?;
        Some(Self {
            anim_idx, frame_idx, layer_idx,
            old: (s.anchor_x, s.anchor_y),
            new,
            applied_at: 0,
        })
    }
}

impl Command for SetLayerAnchor {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("frame {} out of range", self.frame_idx))
        })?;
        let layer = f.sprites.get_mut(self.layer_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("layer {} out of range", self.layer_idx))
        })?;
        layer.anchor_x = self.new.0;
        layer.anchor_y = self.new.1;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: frame {} out of range", self.frame_idx))
        })?;
        let layer = f.sprites.get_mut(self.layer_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: layer {} out of range", self.layer_idx))
        })?;
        layer.anchor_x = self.old.0;
        layer.anchor_y = self.old.1;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Set anim {} frame {} layer {} anchor",
            self.anim_idx, self.frame_idx, self.layer_idx)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn try_merge(&mut self, other: &dyn Command) -> MergeResult {
        let Some(o) = (other as &dyn std::any::Any).downcast_ref::<SetLayerAnchor>() else {
            return MergeResult::Separate;
        };
        if o.anim_idx == self.anim_idx
            && o.frame_idx == self.frame_idx
            && o.layer_idx == self.layer_idx
        {
            self.new = o.new;
            MergeResult::Merged
        } else {
            MergeResult::Separate
        }
    }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- AddFrame ----------

#[derive(Debug)]
pub struct AddFrame {
    pub anim_idx: u32,
    pub at: usize,
    pub frame: AnimationFrame,
    applied_at: u128,
}

impl AddFrame {
    pub fn new(anim_idx: u32, at: usize, frame: AnimationFrame, editor: &Editor) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        if at > a.frames.len() {
            return None;
        }
        Some(Self { anim_idx, at, frame, applied_at: 0 })
    }
}

impl Command for AddFrame {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        if self.at > a.frames.len() {
            return Err(CmdError::InvalidTarget(format!(
                "insert at {} > len {}", self.at, a.frames.len()
            )));
        }
        a.frames.insert(self.at, self.frame.clone());
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        if self.at >= a.frames.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        a.frames.remove(self.at);
        Ok(())
    }
    fn describe(&self) -> String { format!("Add frame at anim {} pos {}", self.anim_idx, self.at) }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- RemoveFrame ----------

#[derive(Debug)]
pub struct RemoveFrame {
    pub anim_idx: u32,
    pub at: usize,
    pub removed: OnceCell<AnimationFrame>,
    applied_at: u128,
}

impl RemoveFrame {
    pub fn new(anim_idx: u32, at: usize, editor: &Editor) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let _ = a.frames.get(at)?;
        Some(Self { anim_idx, at, removed: OnceCell::new(), applied_at: 0 })
    }
}

impl Command for RemoveFrame {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        if self.at >= a.frames.len() {
            return Err(CmdError::InvalidTarget(format!(
                "remove {} >= len {}", self.at, a.frames.len()
            )));
        }
        let f = a.frames.remove(self.at);
        let _ = self.removed.set(f);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let frame = self.removed.get()
            .ok_or_else(|| CmdError::Invariant("revert before apply".into()))?
            .clone();
        let a = anim_mut(state, self.anim_idx)?;
        if self.at > a.frames.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        a.frames.insert(self.at, frame);
        Ok(())
    }
    fn describe(&self) -> String { format!("Remove frame anim {} pos {}", self.anim_idx, self.at) }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- MoveFrame ----------

#[derive(Debug)]
pub struct MoveFrame {
    pub anim_idx: u32,
    pub from: usize,
    pub to: usize,
    applied_at: u128,
}

impl MoveFrame {
    pub fn new(anim_idx: u32, from: usize, to: usize, editor: &Editor) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        if from >= a.frames.len() || to >= a.frames.len() {
            return None;
        }
        Some(Self { anim_idx, from, to, applied_at: 0 })
    }
}

impl Command for MoveFrame {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        if self.from == self.to {
            return Ok(());
        }
        let a = anim_mut(state, self.anim_idx)?;
        if self.from >= a.frames.len() || self.to >= a.frames.len() {
            return Err(CmdError::InvalidTarget(
                format!("move {} -> {} out of range (len {})",
                    self.from, self.to, a.frames.len())
            ));
        }
        let f = a.frames.remove(self.from);
        a.frames.insert(self.to, f);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        if self.from == self.to {
            return Ok(());
        }
        let a = anim_mut(state, self.anim_idx)?;
        if self.from >= a.frames.len() || self.to >= a.frames.len() {
            return Err(CmdError::Invariant("revert: index out of range".into()));
        }
        let f = a.frames.remove(self.to);
        a.frames.insert(self.from, f);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Move frame anim {} {} -> {}", self.anim_idx, self.from, self.to)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- AddLayer ----------

#[derive(Debug)]
pub struct AddLayer {
    pub anim_idx: u32,
    pub frame_idx: usize,
    pub at: usize,
    pub layer: FrameSprite,
    applied_at: u128,
}

impl AddLayer {
    pub fn new(
        anim_idx: u32,
        frame_idx: usize,
        at: usize,
        layer: FrameSprite,
        editor: &Editor,
    ) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let f = a.frames.get(frame_idx)?;
        if at > f.sprites.len() {
            return None;
        }
        Some(Self { anim_idx, frame_idx, at, layer, applied_at: 0 })
    }
}

impl Command for AddLayer {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("frame {} out of range", self.frame_idx))
        })?;
        if self.at > f.sprites.len() {
            return Err(CmdError::InvalidTarget(format!(
                "insert layer at {} > len {}", self.at, f.sprites.len()
            )));
        }
        f.sprites.insert(self.at, self.layer.clone());
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: frame {} out of range", self.frame_idx))
        })?;
        if self.at >= f.sprites.len() {
            return Err(CmdError::Invariant("revert: layer index out of range".into()));
        }
        f.sprites.remove(self.at);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Add layer anim {} frame {} pos {}", self.anim_idx, self.frame_idx, self.at)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- RemoveLayer ----------

#[derive(Debug)]
pub struct RemoveLayer {
    pub anim_idx: u32,
    pub frame_idx: usize,
    pub at: usize,
    pub removed: OnceCell<FrameSprite>,
    applied_at: u128,
}

impl RemoveLayer {
    pub fn new(anim_idx: u32, frame_idx: usize, at: usize, editor: &Editor) -> Option<Self> {
        let a = editor.current_sprite()?.animations.get(&anim_idx)?;
        let f = a.frames.get(frame_idx)?;
        let _ = f.sprites.get(at)?;
        Some(Self { anim_idx, frame_idx, at, removed: OnceCell::new(), applied_at: 0 })
    }
}

impl Command for RemoveLayer {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("frame {} out of range", self.frame_idx))
        })?;
        if self.at >= f.sprites.len() {
            return Err(CmdError::InvalidTarget(format!(
                "remove layer {} >= len {}", self.at, f.sprites.len()
            )));
        }
        let layer = f.sprites.remove(self.at);
        let _ = self.removed.set(layer);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let layer = self.removed.get()
            .ok_or_else(|| CmdError::Invariant("revert before apply".into()))?
            .clone();
        let a = anim_mut(state, self.anim_idx)?;
        let f = a.frames.get_mut(self.frame_idx).ok_or_else(|| {
            CmdError::Invariant(format!("revert: frame {} out of range", self.frame_idx))
        })?;
        if self.at > f.sprites.len() {
            return Err(CmdError::Invariant("revert: layer index out of range".into()));
        }
        f.sprites.insert(self.at, layer);
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Remove layer anim {} frame {} pos {}", self.anim_idx, self.frame_idx, self.at)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- CreateAnimation ----------

#[derive(Debug)]
pub struct CreateAnimation {
    pub new_idx: u32,
    pub anim: Animation,
    applied_at: u128,
}

impl CreateAnimation {
    pub fn new(new_idx: u32, anim: Animation, editor: &Editor) -> Option<Self> {
        let s = editor.current_sprite()?;
        if s.animations.contains_key(&new_idx) {
            return None;
        }
        // Ensure the embedded index field matches, so JSON written later
        // reflects the actual key.
        let mut a = anim;
        a.index = new_idx;
        Some(Self { new_idx, anim: a, applied_at: 0 })
    }
}

impl Command for CreateAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state.current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if s.animations.contains_key(&self.new_idx) {
            return Err(CmdError::Invariant(format!("animation {} exists", self.new_idx)));
        }
        s.animations.insert(self.new_idx, self.anim.clone());
        s.manifest.animation_count = s.animations.len() as u32;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state.current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        s.animations.remove(&self.new_idx);
        s.manifest.animation_count = s.animations.len() as u32;
        Ok(())
    }
    fn describe(&self) -> String { format!("Create animation {}", self.new_idx) }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- DeleteAnimation ----------

#[derive(Debug)]
pub struct DeleteAnimation {
    pub anim_idx: u32,
    pub removed: OnceCell<Animation>,
    applied_at: u128,
}

impl DeleteAnimation {
    pub fn new(anim_idx: u32, editor: &Editor) -> Option<Self> {
        let _ = editor.current_sprite()?.animations.get(&anim_idx)?;
        Some(Self { anim_idx, removed: OnceCell::new(), applied_at: 0 })
    }
}

impl Command for DeleteAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state.current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        let removed = s.animations.remove(&self.anim_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("animation {} missing", self.anim_idx))
        })?;
        s.manifest.animation_count = s.animations.len() as u32;
        let _ = self.removed.set(removed);
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let anim = self.removed.get()
            .ok_or_else(|| CmdError::Invariant("revert before apply".into()))?
            .clone();
        let s = state.current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        s.animations.insert(self.anim_idx, anim);
        s.manifest.animation_count = s.animations.len() as u32;
        Ok(())
    }
    fn describe(&self) -> String { format!("Delete animation {}", self.anim_idx) }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

// ---------- CloneAnimation ----------

#[derive(Debug)]
pub struct CloneAnimation {
    pub src_idx: u32,
    pub new_idx: u32,
    applied_at: u128,
}

impl CloneAnimation {
    pub fn new(src_idx: u32, new_idx: u32, editor: &Editor) -> Option<Self> {
        let s = editor.current_sprite()?;
        let _ = s.animations.get(&src_idx)?;
        if s.animations.contains_key(&new_idx) {
            return None;
        }
        Some(Self { src_idx, new_idx, applied_at: 0 })
    }
}

impl Command for CloneAnimation {
    fn apply(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state.current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        let mut clone = s.animations.get(&self.src_idx).ok_or_else(|| {
            CmdError::InvalidTarget(format!("source animation {} missing", self.src_idx))
        })?.clone();
        clone.index = self.new_idx;
        if s.animations.contains_key(&self.new_idx) {
            return Err(CmdError::Invariant(format!(
                "destination animation {} already exists", self.new_idx
            )));
        }
        s.animations.insert(self.new_idx, clone);
        s.manifest.animation_count = s.animations.len() as u32;
        Ok(())
    }
    fn revert(&mut self, state: &mut Editor) -> Result<(), CmdError> {
        let s = state.current_sprite_mut()
            .ok_or_else(|| CmdError::InvalidTarget("no sprite loaded".into()))?;
        if s.animations.remove(&self.new_idx).is_none() {
            return Err(CmdError::Invariant(format!(
                "revert: cloned animation {} missing", self.new_idx
            )));
        }
        s.manifest.animation_count = s.animations.len() as u32;
        Ok(())
    }
    fn describe(&self) -> String {
        format!("Clone animation {} -> {}", self.src_idx, self.new_idx)
    }
    fn affects(&self) -> DirtyMask { DirtyMask::ANIMATION }
    fn set_applied_at(&mut self, m: u128) { self.applied_at = m; }
    fn applied_at(&self) -> u128 { self.applied_at }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::cmd::history::History;
    use crate::model::{
        Animation, AnimationFrame, FrameSprite, Manifest, SpriteAsset, SpriteType,
    };
    use std::collections::BTreeMap;
    use std::path::PathBuf;

    fn sample_frame(dur: u16) -> AnimationFrame {
        AnimationFrame {
            duration: dur,
            has_audio: 0,
            sprites: vec![FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 }],
        }
    }

    fn fake_editor_with_one_anim() -> Editor {
        let manifest = Manifest {
            label: "x".into(),
            subdir: "entitySprites".into(),
            kind: crate::model::SpriteKind::Unknown,
            sprite_type: SpriteType::Type1,
            sprite_count: 0,
            spritesheet_index_count: 0,
            palette_count: 0,
            palette_index_count: 0,
            animation_count: 1,
            animation_index: vec![0],
            palette_index: vec![],
            palettes: Default::default(),
            sprite_to_palette: vec![],
            palette_index_end_bytes: vec![],
            spritesheet_index_end_bytes: vec![],
            s2p_padding: vec![],
            texture_end_padding: vec![],
            asset_offsets: vec![],
            textures: vec![],
        };
        let mut animations = BTreeMap::new();
        animations.insert(
            0,
            Animation {
                frame_count: 2,
                unused: 0,
                index: 0,
                frames: vec![sample_frame(2), sample_frame(3)],
            },
        );
        let mut e = Editor::new();
        e.loaded_sprite = Some(SpriteAsset {
            dir: PathBuf::new(),
            manifest,
            textures: BTreeMap::new(),
            palettes: BTreeMap::new(),
            animations,
        });
        e
    }

    #[test]
    fn set_frame_duration_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let before = e.loaded_sprite.clone().unwrap();
        let cmd = SetFrameDuration::new(0, 0, 9, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].duration, 9);
        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap(), &before);
    }

    #[test]
    fn set_frame_duration_consecutive_same_target_merges() {
        let mut e = fake_editor_with_one_anim();
        let mut h = History::default();
        h.execute(Box::new(SetFrameDuration::new(0, 0, 5, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetFrameDuration::new(0, 0, 7, &e).unwrap()), &mut e).unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 1, "two same-target edits should merge into one history entry");
    }

    #[test]
    fn set_frame_duration_different_frames_do_not_merge() {
        let mut e = fake_editor_with_one_anim();
        let mut h = History::default();
        h.execute(Box::new(SetFrameDuration::new(0, 0, 5, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetFrameDuration::new(0, 1, 5, &e).unwrap()), &mut e).unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 2, "different frames must not merge");
    }

    #[test]
    fn set_frame_has_audio_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = SetFrameHasAudio::new(0, 0, 1, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].has_audio, 1);
        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].has_audio, 0);
    }

    #[test]
    fn set_frame_has_audio_does_not_merge() {
        let mut e = fake_editor_with_one_anim();
        let mut h = History::default();
        // Two same-target toggles back-to-back. Must NOT collapse, because
        // a checkbox toggle is discrete — collapsing would erase the
        // intermediate state from history.
        h.execute(Box::new(SetFrameHasAudio::new(0, 0, 1, &e).unwrap()), &mut e).unwrap();
        h.execute(Box::new(SetFrameHasAudio::new(0, 0, 0, &e).unwrap()), &mut e).unwrap();
        let mut count = 0;
        while h.undo(&mut e).unwrap().is_some() {
            count += 1;
        }
        assert_eq!(count, 2, "discrete toggles must not merge");
    }

    #[test]
    fn set_layer_texture_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = SetLayerTexture::new(0, 0, 0, 42, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(
            e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites[0].spritesheet_index,
            42
        );
        h.undo(&mut e).unwrap();
        assert_eq!(
            e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites[0].spritesheet_index,
            0
        );
    }

    #[test]
    fn set_layer_anchor_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = SetLayerAnchor::new(0, 0, 0, (-8, 12), &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let layer = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites[0];
        assert_eq!((layer.anchor_x, layer.anchor_y), (-8, 12));
        h.undo(&mut e).unwrap();
        let layer = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites[0];
        assert_eq!((layer.anchor_x, layer.anchor_y), (0, 0));
    }

    #[test]
    fn add_frame_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let new_frame = sample_frame(99);
        let cmd = AddFrame::new(0, 1, new_frame.clone(), &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let frames = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames;
        assert_eq!(frames.len(), 3);
        assert_eq!(frames[1].duration, 99);
        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames.len(), 2);
    }

    #[test]
    fn add_frame_out_of_range_returns_none() {
        let e = fake_editor_with_one_anim();
        // Anim 0 has 2 frames; positions 0..=2 are valid; 3 is out of range.
        assert!(AddFrame::new(0, 99, sample_frame(0), &e).is_none());
    }

    #[test]
    fn remove_frame_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = RemoveFrame::new(0, 0, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let frames = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames;
        assert_eq!(frames.len(), 1);
        assert_eq!(frames[0].duration, 3);
        h.undo(&mut e).unwrap();
        let frames = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames;
        assert_eq!(frames.len(), 2);
        assert_eq!(frames[0].duration, 2);
    }

    #[test]
    fn remove_frame_revert_before_apply_errors() {
        let mut e = fake_editor_with_one_anim();
        let mut cmd = RemoveFrame::new(0, 0, &e).unwrap();
        let err = cmd.revert(&mut e).unwrap_err();
        assert!(matches!(err, CmdError::Invariant(_)));
    }

    #[test]
    fn move_frame_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        // Initially: [dur=2, dur=3]. Move frame 0 -> 1. Result: [dur=3, dur=2].
        let cmd = MoveFrame::new(0, 0, 1, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let durs: Vec<u16> = e.loaded_sprite.as_ref().unwrap().animations[&0]
            .frames.iter().map(|f| f.duration).collect();
        assert_eq!(durs, vec![3, 2]);
        h.undo(&mut e).unwrap();
        let durs: Vec<u16> = e.loaded_sprite.as_ref().unwrap().animations[&0]
            .frames.iter().map(|f| f.duration).collect();
        assert_eq!(durs, vec![2, 3]);
    }

    #[test]
    fn move_frame_same_index_is_noop() {
        let mut e = fake_editor_with_one_anim();
        let before = e.loaded_sprite.clone();
        let cmd = MoveFrame::new(0, 1, 1, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert_eq!(e.loaded_sprite, before);
    }

    #[test]
    fn add_layer_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let new_layer = FrameSprite { spritesheet_index: 7, anchor_x: 1, anchor_y: 2 };
        let cmd = AddLayer::new(0, 0, 1, new_layer, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let layers = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites;
        assert_eq!(layers.len(), 2);
        assert_eq!(layers[1].spritesheet_index, 7);
        h.undo(&mut e).unwrap();
        assert_eq!(
            e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.len(),
            1
        );
    }

    #[test]
    fn remove_layer_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = RemoveLayer::new(0, 0, 0, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.is_empty());
        h.undo(&mut e).unwrap();
        assert_eq!(
            e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.len(),
            1
        );
    }

    #[test]
    fn remove_frame_undo_redo() {
        let mut e = fake_editor_with_one_anim();
        let cmd = RemoveFrame::new(0, 0, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        // After apply: 1 frame left (dur=3).
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames.len(), 1);
        h.undo(&mut e).unwrap();
        // After undo: 2 frames again, dur=2 first.
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames.len(), 2);
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].duration, 2);
        h.redo(&mut e).unwrap();
        // After redo: back to 1 frame (dur=3 — frame 1 is the survivor).
        let frames = &e.loaded_sprite.as_ref().unwrap().animations[&0].frames;
        assert_eq!(frames.len(), 1);
        assert_eq!(frames[0].duration, 3);
        // One more undo: back to 2 frames. This proves OnceCell still holds the
        // original removed value across the apply/revert/apply cycle.
        h.undo(&mut e).unwrap();
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames.len(), 2);
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].duration, 2);
    }

    #[test]
    fn remove_layer_undo_redo() {
        let mut e = fake_editor_with_one_anim();
        let cmd = RemoveLayer::new(0, 0, 0, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.is_empty());
        h.undo(&mut e).unwrap();
        assert_eq!(
            e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.len(), 1
        );
        h.redo(&mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.is_empty());
        // Second undo proves OnceCell survived the redo.
        h.undo(&mut e).unwrap();
        assert_eq!(
            e.loaded_sprite.as_ref().unwrap().animations[&0].frames[0].sprites.len(), 1
        );
    }

    #[test]
    fn remove_frame_out_of_range_returns_none() {
        let e = fake_editor_with_one_anim();
        // Anim 0 has 2 frames; index 2+ is out of range.
        assert!(RemoveFrame::new(0, 99, &e).is_none());
    }

    #[test]
    fn move_frame_out_of_range_returns_none() {
        let e = fake_editor_with_one_anim();
        // 2 frames, valid 0..=1. Either out-of-range index returns None.
        assert!(MoveFrame::new(0, 99, 0, &e).is_none());
        assert!(MoveFrame::new(0, 0, 99, &e).is_none());
    }

    #[test]
    fn add_layer_out_of_range_returns_none() {
        let e = fake_editor_with_one_anim();
        // Frame 0 has 1 sprite; insertion bound is at <= 1, so at = 99 is out of range.
        let layer = FrameSprite { spritesheet_index: 0, anchor_x: 0, anchor_y: 0 };
        assert!(AddLayer::new(0, 0, 99, layer, &e).is_none());
    }

    #[test]
    fn remove_layer_out_of_range_returns_none() {
        let e = fake_editor_with_one_anim();
        // 1 sprite, valid index 0; index 99 is out of range.
        assert!(RemoveLayer::new(0, 0, 99, &e).is_none());
    }

    #[test]
    fn create_animation_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let new_anim = Animation { frame_count: 0, unused: 0, index: 99, frames: vec![] };
        // Constructor overrides the index field with new_idx (5).
        let cmd = CreateAnimation::new(5, new_anim, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(s.animations.contains_key(&5));
        assert_eq!(s.animations[&5].index, 5);
        assert_eq!(s.manifest.animation_count, 2);
        h.undo(&mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(!s.animations.contains_key(&5));
        assert_eq!(s.manifest.animation_count, 1);
    }

    #[test]
    fn create_animation_collision_returns_none() {
        let e = fake_editor_with_one_anim();
        let new_anim = Animation { frame_count: 0, unused: 0, index: 0, frames: vec![] };
        // Index 0 already exists.
        assert!(CreateAnimation::new(0, new_anim, &e).is_none());
    }

    #[test]
    fn delete_animation_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = DeleteAnimation::new(0, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(!s.animations.contains_key(&0));
        assert_eq!(s.manifest.animation_count, 0);
        h.undo(&mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(s.animations.contains_key(&0));
        assert_eq!(s.animations[&0].frames.len(), 2);
        assert_eq!(s.manifest.animation_count, 1);
    }

    #[test]
    fn delete_animation_undo_redo() {
        // OnceCell-bearing command — exercise apply→undo→redo→undo to prove
        // the cell survives across the cycle.
        let mut e = fake_editor_with_one_anim();
        let cmd = DeleteAnimation::new(0, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert!(!e.loaded_sprite.as_ref().unwrap().animations.contains_key(&0));
        h.undo(&mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations.contains_key(&0));
        h.redo(&mut e).unwrap();
        assert!(!e.loaded_sprite.as_ref().unwrap().animations.contains_key(&0));
        h.undo(&mut e).unwrap();
        // Frames intact after the apply→undo→redo→undo cycle.
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&0].frames.len(), 2);
    }

    #[test]
    fn delete_animation_missing_returns_none() {
        let e = fake_editor_with_one_anim();
        // Anim 99 does not exist.
        assert!(DeleteAnimation::new(99, &e).is_none());
    }

    #[test]
    fn clone_animation_deep_copy() {
        let mut e = fake_editor_with_one_anim();
        let cmd = CloneAnimation::new(0, 7, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        let s = e.loaded_sprite.as_ref().unwrap();
        assert!(s.animations.contains_key(&7));
        assert_eq!(s.animations[&7].index, 7);
        // Source unchanged.
        assert_eq!(s.animations[&0].frames.len(), 2);
        // Clone is independent (mutate clone via a fresh edit; source must not change).
        let mut e2 = e;
        let edit = SetFrameDuration::new(7, 0, 88, &e2).unwrap();
        h.execute(Box::new(edit), &mut e2).unwrap();
        let s = e2.loaded_sprite.as_ref().unwrap();
        assert_eq!(s.animations[&7].frames[0].duration, 88);
        assert_eq!(s.animations[&0].frames[0].duration, 2);
    }

    #[test]
    fn clone_animation_collision_returns_none() {
        let e = fake_editor_with_one_anim();
        // Cloning to existing index 0 must return None.
        assert!(CloneAnimation::new(0, 0, &e).is_none());
    }

    #[test]
    fn clone_animation_missing_source_returns_none() {
        let e = fake_editor_with_one_anim();
        // Source 99 does not exist.
        assert!(CloneAnimation::new(99, 7, &e).is_none());
    }

    #[test]
    fn clone_animation_apply_revert() {
        let mut e = fake_editor_with_one_anim();
        let cmd = CloneAnimation::new(0, 7, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations.contains_key(&7));
        assert_eq!(e.loaded_sprite.as_ref().unwrap().manifest.animation_count, 2);
        h.undo(&mut e).unwrap();
        assert!(!e.loaded_sprite.as_ref().unwrap().animations.contains_key(&7));
        assert_eq!(e.loaded_sprite.as_ref().unwrap().manifest.animation_count, 1);
    }

    #[test]
    fn create_animation_undo_redo() {
        // Symmetric counterpart to delete_animation_undo_redo. Proves apply
        // is fully repeatable through the History layer.
        let mut e = fake_editor_with_one_anim();
        let new_anim = Animation { frame_count: 0, unused: 0, index: 5, frames: vec![] };
        let cmd = CreateAnimation::new(5, new_anim, &e).unwrap();
        let mut h = History::default();
        h.execute(Box::new(cmd), &mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations.contains_key(&5));
        h.undo(&mut e).unwrap();
        assert!(!e.loaded_sprite.as_ref().unwrap().animations.contains_key(&5));
        h.redo(&mut e).unwrap();
        assert!(e.loaded_sprite.as_ref().unwrap().animations.contains_key(&5));
        assert_eq!(e.loaded_sprite.as_ref().unwrap().animations[&5].index, 5);
    }
}
