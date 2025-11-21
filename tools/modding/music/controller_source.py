from dataclasses import dataclass, field
from typing import List
from typedefs import ControllerEvent, ControllerType, NoteEvent, NoteType

@dataclass
class ControllerSource:
    type: ControllerType
    events: List[ControllerEvent] = field(default_factory=list)
    owner_track_id: int = -1
    owner_track_name: str = ""

    def convert_clock_base(self, from_tpqn: int, total_ticks: int):
        """Convert MIDI ticks to N64 48 TPQN"""
        divisor = 48.0 / from_tpqn
        i = 0
        prev_duration = 0

        while i < len(self.events):
            # Calculate duration until next event
            if i < len(self.events) - 1:
                d_duration = int((self.events[i + 1].ticks - self.events[i].ticks) * divisor)
            else:
                d_duration = int((total_ticks - self.events[i].ticks) * divisor)

            # Scale the tick time
            self.events[i].ticks = int(self.events[i].ticks * divisor)

            # Handle collisions (events landing on same tick)
            if i > 0 and self.events[i].ticks == self.events[i - 1].ticks:
                # Keep the event with longer duration
                if d_duration > prev_duration:
                    self.events.pop(i - 1)
                    prev_duration = d_duration
                else:
                    self.events.pop(i)
            else:
                prev_duration = d_duration
                i += 1
    
    def trim_events(self, total_ticks: int):
        """Remove events past the end of sequence"""
        while self.events and self.events[-1].ticks >= total_ticks:
            self.events.pop()

    def optimize(self, track_notes: List[NoteEvent]):
        """Remove redundant controller events and those during rests"""
        if not self.events:
            return

        # Phase 1: Remove events during rests
        note_idx = 0
        event_idx = 0
        last_type = NoteType.NOTE

        while event_idx < len(self.events):
            passed_note = False
            # Find which note we're at
            while (note_idx < len(track_notes) and
                   track_notes[note_idx].ticks <= self.events[event_idx].ticks):
                if track_notes[note_idx].type == NoteType.NOTE:
                    passed_note = True
                note_idx += 1

            note_idx = max(0, note_idx - 1)
            # If we didn't pass a note and previous was a rest, remove previous event
            if not passed_note and last_type == NoteType.REST and event_idx > 0:
                self.events.pop(event_idx - 1)
                event_idx -= 1
            
            event_idx += 1
            if note_idx < len(track_notes):
                last_type = track_notes[note_idx].type

        # Phase 2: Remove consecutive duplicate values
        if not self.events: return
        
        last_value = self.events[0].value
        event_idx = 1
        while event_idx < len(self.events):
            if abs(self.events[event_idx].value - last_value) < 0.001:
                self.events.pop(event_idx)
            else:
                last_value = self.events[event_idx].value
                event_idx += 1

    def filter_to_note_ranges(self, track_notes: List[NoteEvent]):
        """Remove pitch bend events that don't overlap with actual notes"""
        if not self.events or not track_notes: return

        note_ranges = []
        for i, note in enumerate(track_notes):
            if note.type == NoteType.NOTE:
                start = note.ticks
                end = track_notes[i + 1].ticks if i < len(track_notes) - 1 else start + 1
                note_ranges.append((start, end))

        if not note_ranges:
            self.events.clear()
            return

        filtered = []
        for event in self.events:
            for start, end in note_ranges:
                if start <= event.ticks < end:
                    filtered.append(event)
                    break
        self.events = filtered