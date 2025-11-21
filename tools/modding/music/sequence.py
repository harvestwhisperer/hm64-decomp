from dataclasses import dataclass, field
from typing import List

from typedefs import NoteEvent, NoteType, Track
from controller_source import ControllerSource

@dataclass
class Sequence:
    tracks: List[Track] = field(default_factory=list)
    sources: List[ControllerSource] = field(default_factory=list)
    ticks_per_quarter: int = 480
    total_ticks: int = 0
    tempo_source: int = -1

    def convert_clock_base(self):
        """Convert all timing from MIDI to N64 48 TPQN"""
        
        divisor = 48.0 / self.ticks_per_quarter

        # Convert track notes
        # Convert start AND end positions separately, then calculate duration
        # This prevents accumulated timing drift from independent truncation of durations
        for track in self.tracks:
            
            for note in track.notes:
            
                original_start = note.ticks
                original_end = note.ticks + note.duration_ticks

                # Convert both start and end times
                new_start = int(original_start * divisor)
                new_end = int(original_end * divisor)

                # Calculate duration from converted positions (maintains timing accuracy)
                note.ticks = new_start
                note.duration_ticks = new_end - new_start
                
                # Ensure note events have at least 1 tick duration to prevent zero-length notes
                if note.type == NoteType.NOTE and note.duration_ticks < 1:
                    note.duration_ticks = 1

        # Convert controller sources
        for source in self.sources:
            source.convert_clock_base(self.ticks_per_quarter, self.total_ticks)

        # Update sequence metadata
        self.total_ticks = int(self.total_ticks * divisor)
        self.ticks_per_quarter = 48
    
    def trim_events(self):
        """Remove events past sequence end"""
        for source in self.sources:
            source.trim_events(self.total_ticks)

    def trim_trailing_silence(self):
        """Recalculate total_ticks based on actual note/event end times"""

        actual_end_time = 0
        
        for track in self.tracks:
            for note in track.notes:
                if note.type == NoteType.NOTE:
                    note_end = note.ticks + note.duration_ticks
                    if note_end > actual_end_time:
                        actual_end_time = note_end
        
        # Also check controller event end times
        for source in self.sources:
            if source.events:
                actual_end_time = max(actual_end_time, source.events[-1].ticks)
        
        if actual_end_time > 0 and actual_end_time < self.total_ticks:
            self.total_ticks = actual_end_time

    def ensure_final_rests(self):
        """Ensure tracks end with a rest if they stop before sequence end."""
        
        for track in self.tracks:

            if not track.notes: continue
            
            last_event = track.notes[-1]

            if last_event.type == NoteType.NOTE:
                last_event_end = last_event.ticks + last_event.duration_ticks
            elif last_event.type == NoteType.REST:
                # rest already exists at the end
                # last_event_end = last_event.ticks
                continue
            else:
                last_event_end = last_event.ticks
            
            if last_event_end < self.total_ticks:

                track.notes.append(NoteEvent(type=NoteType.REST, ticks=last_event_end))
                
                # Update merged timeline if present
                if track.instrument_timeline:
                    # Copy the last instrument/effect settings for the rest
                    # Format: (time, instrument, effect, note, source_idx)
                    last_timeline_entry = track.instrument_timeline[-1]
                    
                    rest_timeline_entry = (
                        last_event_end,  # time
                        last_timeline_entry[1],  # instrument (same as last)
                        last_timeline_entry[2],  # effect (same as last)
                        0x60,  # note (0x60 = REST)
                        last_timeline_entry[4] if len(last_timeline_entry) > 4 else -1  # source_idx
                    )

                    track.instrument_timeline.append(rest_timeline_entry)
    
    def optimize_all(self):
        """Optimize all controller sources"""
        for track in self.tracks:
            if track.volume_source >= 0:
                self.sources[track.volume_source].optimize(track.notes)
            if track.pan_source >= 0:
                self.sources[track.pan_source].optimize(track.notes)
            if track.fine_pitch_source >= 0:
                # For pitch bend, first filter to note ranges
                self.sources[track.fine_pitch_source].filter_to_note_ranges(track.notes)
                self.sources[track.fine_pitch_source].optimize(track.notes)