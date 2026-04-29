"""SFX opcode mnemonics. Reuses the music sequence opcode table.

The full table lives at tools/modding/music/constants.py:OpCode. We re-export
the subset actually used by SFX micro-sequences here so the SFX module has no
dependency on tools/modding/.
"""

# Opcode constants (from tools/modding/music/constants.py:OpCode).
# Only the subset used by SFX is defined here; add more as needed.
END_OF_TRACK   = 0x80
SET_INSTRUMENT = 0x81
ADSR_SET       = 0x84
PAN            = 0x9C
NOTE_VOL       = 0xA6

# Mnemonic -> opcode byte. Used by parser.
MNEMONIC_TO_OPCODE = {
    "END":            END_OF_TRACK,
    "SET_INSTRUMENT": SET_INSTRUMENT,
    "ADSR_SET":       ADSR_SET,
    "PAN":            PAN,
    "NOTE_VOL":       NOTE_VOL,
}

# Opcode byte -> mnemonic. Used by disassembler.
OPCODE_TO_MNEMONIC = {v: k for k, v in MNEMONIC_TO_OPCODE.items()}

# Number of single-byte arguments each opcode takes (the args following the opcode byte).
# NOTE: program number for SET_INSTRUMENT is VLQ-encoded (1 or 2 bytes); handled separately.
# NOTE: notes are not opcodes - pitch byte < 0x80 marks a note event with a length arg.
# ADSR_SET takes 7 args: env_speed, init_vol, attack_speed, peak_vol, decay_speed,
# sustain_vol, release_speed (see lib/libmus/src/player_commands.c:Fdefa).
OPCODE_ARG_COUNT = {
    END_OF_TRACK:   0,
    ADSR_SET:       7,
    PAN:            1,
    NOTE_VOL:       1,
}

# The fixed ADSR envelope used by every non-sample SFX in the original sfx.c.
# 7 bytes: speed=1, init=0x73, attack=1, peak=0x73, decay=1, sustain=0x73, release=2.
# The disassembler treats any deviation from this as an "outlier" (forces opcode form).
CANONICAL_ADSR = bytes([0x01, 0x73, 0x01, 0x73, 0x01, 0x73, 0x02])
