//! Undo/redo stacks with a 500 ms merge window for rapid-fire edits.
//!
//! Each pushed command carries a monotonic `seq: u64` that persists as the
//! command moves between the undo and redo stacks. Sequences are consumed
//! by `BaselineTracker` in `state::baseline` to decide which domains are
//! currently dirty. Merged commands reuse the prior seq so merge chains
//! look like a single command to the dirty tracker.

use std::time::{SystemTime, UNIX_EPOCH};

use crate::cmd::{CmdError, Command, DirtyMask, MergeResult};
use crate::state::editor::Editor;

pub const DEFAULT_CAPACITY: usize = 200;
pub const MERGE_WINDOW_MS: u128 = 500;

type Entry = (u64, Box<dyn Command>);

pub struct History {
    undo: Vec<Entry>,
    redo: Vec<Entry>,
    next_seq: u64,
    capacity: usize,
}

pub struct ExecuteOutcome {
    pub seq: u64,
    pub affects: DirtyMask,
    pub merged: bool,
}

impl Default for History {
    fn default() -> Self {
        Self::new(DEFAULT_CAPACITY)
    }
}

impl History {
    pub fn new(capacity: usize) -> Self {
        Self { undo: Vec::new(), redo: Vec::new(), next_seq: 0, capacity }
    }

    pub fn can_undo(&self) -> bool {
        !self.undo.is_empty()
    }
    pub fn can_redo(&self) -> bool {
        !self.redo.is_empty()
    }

    pub fn undo_seqs(&self) -> Vec<u64> {
        self.undo.iter().map(|(s, _)| *s).collect()
    }

    pub fn execute(
        &mut self,
        mut cmd: Box<dyn Command>,
        editor: &mut Editor,
    ) -> Result<ExecuteOutcome, CmdError> {
        cmd.apply(editor)?;
        let now = now_ms();
        cmd.set_applied_at(now);
        let affects = cmd.affects();

        if let Some((top_seq, top)) = self.undo.last_mut() {
            if now - top.applied_at() < MERGE_WINDOW_MS
                && top.try_merge(cmd.as_ref()) == MergeResult::Merged
            {
                top.set_applied_at(now);
                let seq = *top_seq;
                self.redo.clear();
                return Ok(ExecuteOutcome { seq, affects, merged: true });
            }
        }

        let seq = self.next_seq;
        self.next_seq += 1;
        self.undo.push((seq, cmd));
        self.redo.clear();
        while self.undo.len() > self.capacity {
            self.undo.remove(0);
        }
        Ok(ExecuteOutcome { seq, affects, merged: false })
    }

    pub fn undo(&mut self, editor: &mut Editor) -> Result<Option<DirtyMask>, CmdError> {
        let Some((seq, mut cmd)) = self.undo.pop() else { return Ok(None) };
        cmd.revert(editor)?;
        let dirty = cmd.affects();
        self.redo.push((seq, cmd));
        Ok(Some(dirty))
    }

    pub fn redo(&mut self, editor: &mut Editor) -> Result<Option<DirtyMask>, CmdError> {
        let Some((seq, mut cmd)) = self.redo.pop() else { return Ok(None) };
        cmd.apply(editor)?;
        let dirty = cmd.affects();
        self.undo.push((seq, cmd));
        Ok(Some(dirty))
    }

    pub fn clear(&mut self) {
        self.undo.clear();
        self.redo.clear();
    }
}

fn now_ms() -> u128 {
    SystemTime::now().duration_since(UNIX_EPOCH).map(|d| d.as_millis()).unwrap_or(0)
}
