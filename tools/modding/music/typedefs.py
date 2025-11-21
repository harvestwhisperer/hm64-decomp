from enum import Enum
from dataclasses import dataclass, field
from typing import List, Optional

@dataclass
class ADSREnvelope:
    """ADSR Envelope - 7 bytes"""
    rate: int
    start_level: int
    attack_rate: int
    peak_level: int
    decay_rate: int
    sustain_level: int
    release_rate: int

@dataclass
class NoteInfo:
    track: int
    start_time: int
    end_time: int
    note: int
    velocity: int
    instrument: int
    pan: int
    volume: int
    pitch_bend: int
    tempo: int

class NoteType(Enum):
    NOTE = "note"
    REST = "rest"

@dataclass
class NoteEvent:
    """A note or rest event"""
    type: NoteType
    ticks: int
    velocity: float = 0.0  # 0.0-1.0
    note: int = 0  # MIDI note number
    duration_ticks: int = 0  # Duration in ticks (for polyphony detection)

class ControllerType(Enum):
    VOLUME = "volume"
    PAN = "pan"
    FINE_PITCH = "fine_pitch"
    TEMPO = "tempo"

@dataclass
class ControllerEvent:
    """A single controller event with time and value"""
    ticks: int
    value: float  # Normalized 0.0-1.0

class ControllerSource:
    """Controller data stream (volume, pan, pitch bend, etc.)"""
    type: ControllerType
    events: List[ControllerEvent] = field(default_factory=list)
    owner_track_id: int = -1
    owner_track_name: str = ""
    
@dataclass
class TimeValue:
    value: int
    start_time: int
    end_time: int

@dataclass
class Track:
    name: str = ""
    instrument: int = -1
    notes: List[NoteEvent] = field(default_factory=list)
    volume_source: int = -1
    pan_source: int = -1
    fine_pitch_source: int = -1
    effect: int = 0
    # Custom fields for merged tracks / processing
    instrument_timeline: List = field(default_factory=list)
    program_changes: List = field(default_factory=list)
    gm_program: Optional[int] = None