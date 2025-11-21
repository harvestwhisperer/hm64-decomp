from enum import IntEnum

# Timing
TICKS_PER_QUARTER_N64 = 48
TICKS_PER_QUARTER_MIDI_DEFAULT = 480

# Note Values
NOTE_REST = 0x60
MAX_NOTE_VAL = 0x5F

# Sequence command codes
class OpCode(IntEnum):
    END_OF_TRACK       = 0x80
    SET_INSTRUMENT     = 0x81
    SET_SLIDE          = 0x82
    ADSR_SET           = 0x84
    SET_TEMPO          = 0x85
    SET_RELEASE_TIME   = 0x86
    RELEASE_TO_FULL    = 0x87
    VIBRATO_UP         = 0x88
    VIBRATO_DOWN       = 0x89
    SOLO               = 0x8A
    SET_DEFINED_LENGTH = 0x8B
    TRANSPOSE          = 0x8D
    DETUNE             = 0x8F
    ADSR_INDEX         = 0x90
    ENV_TRIGGER_OFF    = 0x91
    ENV_TRIGGER_ON     = 0x92
    SAMPLE_TRIGGER_OFF = 0x93
    SAMPLE_TRIGGER_ON  = 0x94
    LOOP_START         = 0x95
    LOOP_BACK          = 0x96
    WOBBLE             = 0x97
    VELOCITY_ENABLE    = 0x99
    VELOCITY_DEFINE    = 0x9A
    SET_REPEAT_VEL     = 0x9B
    PAN                = 0x9C
    DRUM_MAP_ON        = 0x9E
    DRUM_MAP_OFF       = 0x9F
    EFFECT             = 0xA2
    SET_MIN_MAX_TRANS  = 0xA3
    SET_MIN_MAX_VOL    = 0xA4
    SET_MIN_MAX_PAN    = 0xA5
    NOTE_VOL           = 0xA6
    LOAD_TRACK         = 0xA7
    PITCH_BEND         = 0xA8
    STEREO_SWEEP       = 0xA9
    MASTER_EFFECT      = 0xAA
    MASTER_SYNC        = 0xAB
    REENABLE_LENGTH    = 0xAC