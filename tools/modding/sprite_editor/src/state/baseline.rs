//! Per-domain baseline tracking so undo/redo can correctly flip dirty bits.
//!
//! Each command executed by `History` is assigned a monotonic sequence ID.
//! At save time we snapshot the set of sequence IDs (filtered to commands
//! that touch the saved domain) onto `BaselineTracker`. A domain is dirty
//! iff the *current* set of undo-stack seqs affecting that domain differs
//! from its baseline set. This handles all four transition patterns:
//! edit→save→undo (re-dirty), edit→undo (back to clean), edit→undo→redo
//! (clean again), and edit→undo→new-command (dirty, even though the undo
//! stack depth matches the baseline count).
//!
//! Sequence IDs are never reused, so commands popped off the history by
//! capacity pruning still contribute correctly via `masks`, which we keep
//! for the session lifetime of the current sprite.
//!
//! Baselines are keyed by single-bit `DirtyMask` values rather than enumerated
//! per-field. Adding a domain is a single new bit constant in `DirtyMask`; no
//! changes are required here.

use std::collections::{HashMap, HashSet};

use crate::cmd::DirtyMask;

#[derive(Debug, Default)]
pub struct BaselineTracker {
    /// Per-seq mask captured at `record` time. Survives history pruning so
    /// dropped seqs that resurface (e.g. via a redo path that the History
    /// would never produce — defensive) still resolve correctly.
    masks: HashMap<u64, DirtyMask>,
    /// Clean-state baseline per single-bit domain. Absent key = empty set
    /// (the load-time default).
    baselines: HashMap<DirtyMask, HashSet<u64>>,
}

impl BaselineTracker {
    pub fn record(&mut self, seq: u64, affects: DirtyMask) {
        self.masks.insert(seq, affects);
    }

    /// Snapshot the seqs currently on the undo stack that touch each domain
    /// in `domain`, marking those domains clean. Multi-bit `domain` values
    /// snapshot every bit independently — used by `save_font`, which clears
    /// FONT_TEXTURE | FONT_PALETTE in one call.
    pub fn set_clean_for(&mut self, domain: DirtyMask, current_undo_seqs: &[u64]) {
        for bit in domain.iter_bits() {
            self.baselines
                .insert(bit, self.filter_affecting(current_undo_seqs, bit));
        }
    }

    /// Returns whether the `domain` (must be a single bit) is dirty given the
    /// current undo-stack seqs.
    pub fn dirty_for(&self, domain: DirtyMask, current_undo_seqs: &[u64]) -> bool {
        debug_assert!(
            domain.bits().count_ones() == 1,
            "dirty_for requires a single-bit DirtyMask, got {:?}", domain
        );
        let current = self.filter_affecting(current_undo_seqs, domain);
        match self.baselines.get(&domain) {
            Some(baseline) => &current != baseline,
            None => !current.is_empty(),
        }
    }

    /// Builds the full per-domain dirty mask in one pass. Equivalent to
    /// calling `dirty_for` for each domain bit but avoids repeating the
    /// per-domain filter walks at every call site.
    pub fn dirty_mask(&self, current_undo_seqs: &[u64]) -> DirtyMask {
        let mut out = DirtyMask::empty();
        for bit in DirtyMask::all().iter_bits() {
            if self.dirty_for(bit, current_undo_seqs) {
                out.insert(bit);
            }
        }
        out
    }

    pub fn reset(&mut self) {
        *self = Self::default();
    }

    fn filter_affecting(&self, seqs: &[u64], domain: DirtyMask) -> HashSet<u64> {
        seqs.iter()
            .copied()
            .filter(|s| self.masks.get(s).is_some_and(|m| m.intersects(domain)))
            .collect()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn initially_all_clean() {
        let b = BaselineTracker::default();
        assert!(!b.dirty_for(DirtyMask::PALETTE, &[]));
        assert!(!b.dirty_for(DirtyMask::ANIMATION_DATA, &[]));
        assert!(!b.dirty_for(DirtyMask::ANIMATION, &[]));
    }

    #[test]
    fn new_command_marks_dirty() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        assert!(b.dirty_for(DirtyMask::PALETTE, &[1]));
        // Other domains untouched.
        assert!(!b.dirty_for(DirtyMask::ANIMATION_DATA, &[1]));
    }

    #[test]
    fn set_clean_after_save_clears_dirty() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        b.set_clean_for(DirtyMask::PALETTE, &[1]);
        assert!(!b.dirty_for(DirtyMask::PALETTE, &[1]));
    }

    #[test]
    fn undo_back_to_baseline_is_clean() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        // Never saved. Baseline is empty. Undo pops seq 1 → current = []. Empty == empty.
        assert!(!b.dirty_for(DirtyMask::PALETTE, &[]));
    }

    #[test]
    fn undo_past_save_then_new_command_still_dirty() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        // Save at seq=1. Then undo pops seq 1 → current seqs = []; a new
        // command takes seq=2 on the next push.
        b.set_clean_for(DirtyMask::PALETTE, &[1]);
        b.record(2, DirtyMask::PALETTE);
        // Current = {2}, baseline = {1}. Dirty.
        assert!(b.dirty_for(DirtyMask::PALETTE, &[2]));
    }

    #[test]
    fn undo_past_save_only_is_dirty() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        b.set_clean_for(DirtyMask::PALETTE, &[1]);
        // Undo pops 1 → current = []. Baseline = {1}. Dirty.
        assert!(b.dirty_for(DirtyMask::PALETTE, &[]));
    }

    #[test]
    fn per_domain_isolation() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        b.record(2, DirtyMask::ANIMATION_DATA);
        // Save palette only.
        b.set_clean_for(DirtyMask::PALETTE, &[1, 2]);
        // Palette clean, animation_data still dirty.
        assert!(!b.dirty_for(DirtyMask::PALETTE, &[1, 2]));
        assert!(b.dirty_for(DirtyMask::ANIMATION_DATA, &[1, 2]));
        // ANIMATION domain is independent: a palette-only save must not touch it.
        b.record(3, DirtyMask::ANIMATION);
        assert!(b.dirty_for(DirtyMask::ANIMATION, &[1, 2, 3]));
    }

    #[test]
    fn reset_clears_all() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        b.set_clean_for(DirtyMask::PALETTE, &[1]);
        b.reset();
        assert!(!b.dirty_for(DirtyMask::PALETTE, &[1]));
    }

    #[test]
    fn animation_domain_round_trips() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::ANIMATION);
        // After recording, the domain should be dirty.
        assert!(b.dirty_for(DirtyMask::ANIMATION, &[1]));
        // After saving, it should be clean.
        b.set_clean_for(DirtyMask::ANIMATION, &[1]);
        assert!(!b.dirty_for(DirtyMask::ANIMATION, &[1]));
    }

    #[test]
    fn font_texture_and_palette_isolate_correctly() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::FONT_TEXTURE);
        b.record(2, DirtyMask::FONT_PALETTE);
        assert!(b.dirty_for(DirtyMask::FONT_TEXTURE, &[1, 2]));
        assert!(b.dirty_for(DirtyMask::FONT_PALETTE, &[1, 2]));
        assert!(!b.dirty_for(DirtyMask::PALETTE, &[1, 2]));
        b.set_clean_for(DirtyMask::FONT_TEXTURE, &[1, 2]);
        assert!(!b.dirty_for(DirtyMask::FONT_TEXTURE, &[1, 2]));
        // Font-palette still dirty because we only saved font-texture.
        assert!(b.dirty_for(DirtyMask::FONT_PALETTE, &[1, 2]));
    }

    #[test]
    fn dirty_mask_aggregates_per_domain() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::PALETTE);
        b.record(2, DirtyMask::ANIMATION);
        let m = b.dirty_mask(&[1, 2]);
        assert!(m.palette());
        assert!(m.animation());
        assert!(!m.texture());
    }

    #[test]
    fn set_clean_handles_multi_bit_domain() {
        let mut b = BaselineTracker::default();
        b.record(1, DirtyMask::FONT_TEXTURE);
        b.record(2, DirtyMask::FONT_PALETTE);
        // save_font passes both bits at once.
        b.set_clean_for(
            DirtyMask::FONT_TEXTURE | DirtyMask::FONT_PALETTE,
            &[1, 2],
        );
        assert!(!b.dirty_for(DirtyMask::FONT_TEXTURE, &[1, 2]));
        assert!(!b.dirty_for(DirtyMask::FONT_PALETTE, &[1, 2]));
    }
}
