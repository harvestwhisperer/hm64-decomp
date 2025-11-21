GAME_INSTRUMENTS = {
    0: "Upright Bass",
    1: "Accordion",
    2: "Steel Guitar",
    3: "Kick Drum", # F#2 absolute pitch, mapped to F#1
    4: "Bongos",
    5: "Bongos 2",
    6: "Gong",
    7: "Hi-Hat Pedal",
    8: "Horn secion",           
    9: "Low Piano (F#2)",          # Low range piano; F#2 absolute pitch, mapped to F#1
    10: "High Piano(A5)",           # High range piano; A5 absolute pitch, mapped to A4
    11: "Flute",
    12: "Ride Cymbal",
    13: "Snare",
    14: "Mid strings", # A4 absolute pitch, mapped to A3
    15: "Trumpet",
    16: "High strings", # F5 absolute pitch, mapped to F4
    17: "Side Stick",
    18: "Instr_12",
    19: "Instr_13",
    20: "Instr_14",
    21: "Instr_15",
    22: "Banjo",
    23: "Clarinet",
    24: "Harmonica",
    25: "Mid Piano (F#4)",          # Mid range piano; F#4 asbolute pitch, mapped to F#3
    26: "Steel Drums",
    27: "Tambourine",
    28: "Clave",
    29: "Muted E. Guitar",
    30: "Pizzicato Strings",
    31: "Pan Flute",
    32: "Fiddle",
    33: "Vibraphone",
    34: "Guiro",
    35: "Guiro 2",
    36: "Sleigh Bells",
    37: "Cowbell",
    38: "Hi-Hat Closed",
    39: "Hi-Hat Open",
    40: "Organ 2",
    41: "Cello",
    42: "Oboe",
    43: "Organ",
    44: "Instr_2C",
    45: "Timpani",
    46: "Brass Section",  # Trumpets/trombones
    47: "Low Strings", # C#3 absolute patch, mapped to C#2
    48: "Organ 2",
    49: "Church Organ",
    50: "Church Organ 2",
    51: "Ocarina",
    52: "Choir Aahs",
    53: "Harp",
    54: "Tubular Bells",
    55: "Instr_37",
    56: "Instr_38",
    57: "Electric Bass (Pick)",
    58: "Saxophone",
    59: "Glockenspiel",
    60: "Music Box",
    61: "Choir Oohs",
}

# Instrument upper limits (in MIDI note numbers after transposition down 12 semitones)
# These are the maximum notes each instrument can play
# Format: instrument_num: upper_limit_note
INSTRUMENT_UPPER_LIMITS = {
    0: 41,   # Upright bass -> F2
    1: 77,   # Accordion -> F5
    2: 59,   # Harpsichord/guitar -> B3
    3: 43,   # Kick drum -> G2
    9: 42,   # Low piano -> F#2
    10: 79,  # High piano -> G5
    11: 79,  # Flute -> G5
    14: 74,  # Mid strings -> D5
    16: 79,  # High strings -> G5
    22: 74,  # Banjo -> D5
    23: 65,  # Clarinet -> F4
    25: 54,  # Mid piano -> F#3
    30: 74,  # Pizzicato strings -> D5
    31: 74,  # Pan flute -> D5
    32: 79,  # Violin -> G5
    41: 62,  # Cello -> D4
    42: 74,  # Oboe -> D5
    47: 53,  # Low strings -> F3
    52: 81,  # Choir aahs -> A4
    53: 77,  # Harp -> F5
    57: 38,  # Electric bass -> D2
    61: 79,  # Choir oohs -> G5
}

# GM to Game instrument mapping
# Each GM program maps to: (game_inst, range_min, range_max)
# If a single instrument covers multiple ranges, split into multiple entries
GM_TO_GAME = {
    # Piano family (GM 0-7)
    # Ranges: Patch 9 (0-54), Patch 25 (55-66), Patch 10 (67-127)
    0: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Acoustic Grand Piano -> Low Piano/Mid Piano/High Piano
    1: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Bright Acoustic Piano
    2: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Electric Grand Piano
    3: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Honky-tonk Piano
    4: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Electric Piano 1
    5: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Electric Piano 2
    6: [(2, 0, 127)],                               # Harpsichord
    7: [(9, 0, 54), (25, 55, 66), (10, 67, 127)],   # Clavinet -> Piano

    # Chromatic Percussion (GM 8-15)
    8: [(59, 0, 127)],   # Celesta -> Glockenspiel
    9: [(59, 0, 127)],   # Glockenspiel
    10: [(60, 0, 127)],  # Music Box
    11: [(33, 0, 127)],  # Vibraphone
    12: [(33, 0, 127)],  # Marimba
    13: [(33, 0, 127)],  # Xylophone
    14: [(54, 0, 127)],  # Tubular Bells
    15: [(22, 0, 127)],  # Dulcimer -> Banjo

    # Organ (GM 16-23)
    16: [(40, 0, 127)],  # Drawbar Organ
    17: [(40, 0, 127)],  # Percussive Organ
    18: [(43, 0, 127)],  # Rock Organ -> Organ 2
    19: [(49, 0, 127)],  # Church Organ
    20: [(1, 0, 127)],  # Reed Organ --> Accordion
    21: [(1, 0, 127)],   # Accordion
    22: [(24, 0, 127)],  # Harmonica
    23: [(1, 0, 127)],   # Tango Accordion -> Accordion

    # Guitar (GM 24-31)
    24: [(2, 0, 127)],   # Acoustic Guitar (nylon) -> Steel Guitar
    25: [(2, 0, 127)],   # Acoustic Guitar (steel)
    26: [(29, 0, 127)],  # Electric Guitar (jazz) -> Muted E. Guitar
    27: [(29, 0, 127)],  # Electric Guitar (clean) -> Muted E. Guitar
    28: [(29, 0, 127)],  # Electric Guitar (muted)
    29: [(29, 0, 127)],  # Overdriven Guitar -> Muted E. Guitar
    30: [(29, 0, 127)],  # Distortion Guitar -> Muted E. Guitar
    31: [(24, 0, 127)],  # Guitar harmonics -> Harmonica

    # Bass (GM 32-39)
    32: [(0, 0, 127)],   # Acoustic Bass -> Upright Bass
    33: [(57, 0, 127)],  # Electric Bass (finger) -> Electric Bass (Pick)
    34: [(57, 0, 127)],  # Electric Bass (pick)
    35: [(0, 0, 127)],   # Fretless Bass -> Upright Bass
    36: [(0, 0, 127)],   # Slap Bass 1 -> Upright Bass
    37: [(0, 0, 127)],   # Slap Bass 2 -> Upright Bass
    38: [(57, 0, 127)],  # Synth Bass 1 -> Electric Bass
    39: [(57, 0, 127)],  # Synth Bass 2 -> Electric Bass

    # Strings (GM 40-47)
    40: [(32, 0, 127)],  # Violin -> Fiddle
    41: [(32, 0, 127)],  # Viola -> Fiddle
    42: [(41, 0, 127)],  # Cello
    43: [(0, 0, 127)],   # Contrabass -> Upright Bass
    44: [(14, 0, 127)],  # Tremolo Strings -> Strings
    45: [(30, 0, 127)],  # Pizzicato Strings
    46: [(53, 0, 127)],  # Orchestral Harp -> Harp
    47: [(45, 0, 127)],  # Timpani

    # Ensemble (GM 48-55)
    # String ensembles split by range: Low strings (0-45), Mid strings (46-60), High strings (61-127)
    48: [(47, 0, 45), (14, 46, 60), (16, 61, 127)],  # String Ensemble 1 -> Low/Mid/High Strings
    49: [(47, 0, 45), (14, 46, 60), (16, 61, 127)],  # String Ensemble 2 -> Low/Mid/High Strings
    50: [(47, 0, 45), (14, 46, 60), (16, 61, 127)],  # SynthStrings 1 -> Low/Mid/High Strings
    51: [(47, 0, 45), (14, 46, 60), (16, 61, 127)],  # SynthStrings 2 -> Low/Mid/High Strings
    52: [(52, 0, 127)],  # Choir Aahs
    53: [(61, 0, 127)],  # Voice Oohs -> Choir Oohs
    54: [(52, 0, 127)],  # Synth Voice -> Choir Aahs
    55: [(46, 0, 127)],  # Orchestra Hit -> Brass Section

    # Brass (GM 56-63)
    56: [(15, 0, 127)],  # Trumpet
    57: [(15, 0, 127)],  # Trombone -> Trumpet
    58: [(15, 0, 127)],  # Tuba -> Trumpet
    59: [(15, 0, 127)],  # Muted Trumpet
    60: [(15, 0, 127)],  # French Horn -> Trumpet
    61: [(46, 0, 127)],  # Brass Section
    62: [(8, 0, 127)],   # SynthBrass 1 -> Brass Section 2
    63: [(8, 0, 127)],   # SynthBrass 2 -> Brass Section 2

    # Reed (GM 64-71)
    64: [(58, 0, 127)],  # Soprano Sax -> Saxophone
    65: [(58, 0, 127)],  # Alto Sax -> Saxophone
    66: [(58, 0, 127)],  # Tenor Sax -> Saxophone
    67: [(58, 0, 127)],  # Baritone Sax -> Saxophone
    68: [(23, 0, 127)],  # Oboe -> Clarinet
    69: [(23, 0, 127)],  # English Horn -> Clarinet
    70: [(42, 0, 127)],  # Bassoon
    71: [(23, 0, 127)],  # Clarinet

    # Pipe (GM 72-79)
    72: [(51, 0, 127)],  # Piccolo -> Ocarina
    73: [(11, 0, 127)],  # Flute
    74: [(11, 0, 127)],  # Recorder -> Flute
    75: [(31, 0, 127)],  # Pan Flute
    76: [(11, 0, 127)],  # Blown Bottle -> Flute
    77: [(11, 0, 127)],  # Shakuhachi -> Flute
    78: [(11, 0, 127)],  # Whistle -> Flute
    79: [(51, 0, 127)],  # Ocarina

    # Synth Lead (GM 80-87) - use closest melodic instruments
    80: [(11, 0, 127)],  # Lead 1 (square) -> Flute
    81: [(58, 0, 127)],  # Lead 2 (sawtooth) -> Saxophone
    82: [(23, 0, 127)],  # Lead 3 (calliope) -> Clarinet
    83: [(11, 0, 127)],  # Lead 4 (chiff) -> Flute
    84: [(15, 0, 127)],  # Lead 5 (charang) -> Trumpet
    85: [(52, 0, 127)],  # Lead 6 (voice) -> Choir Aahs
    86: [(14, 0, 127)],  # Lead 7 (fifths) -> Strings
    87: [(46, 0, 127)],  # Lead 8 (bass + lead) -> Brass Section

    # Synth Pad (GM 88-95)
    88: [(14, 0, 127)],  # Pad 1 (new age) -> Strings
    89: [(52, 0, 127)],  # Pad 2 (warm) -> Choir Aahs
    90: [(49, 0, 127)],  # Pad 3 (polysynth) -> Church Organ
    91: [(52, 0, 127)],  # Pad 4 (choir) -> Choir Aahs
    92: [(16, 0, 127)],  # Pad 5 (bowed) -> Strings 2
    93: [(14, 0, 127)],  # Pad 6 (metallic) -> Strings
    94: [(52, 0, 127)],  # Pad 7 (halo) -> Choir Aahs
    95: [(46, 0, 127)],  # Pad 8 (sweep) -> Brass Section

    # Synth Effects (GM 96-103)
    96: [(36, 0, 127)],  # FX 1 (rain) -> Sleigh Bells
    97: [(14, 0, 127)],  # FX 2 (soundtrack) -> Strings
    98: [(26, 0, 127)],  # FX 3 (crystal) -> Steel Drums
    99: [(52, 0, 127)],  # FX 4 (atmosphere) -> Choir Aahs
    100: [(36, 0, 127)], # FX 5 (brightness) -> Sleigh Bells
    101: [(6, 0, 127)],  # FX 6 (goblins) -> Gong
    102: [(6, 0, 127)],  # FX 7 (echoes) -> Gong
    103: [(14, 0, 127)], # FX 8 (sci-fi) -> Strings

    # Ethnic (GM 104-111)
    104: [(22, 0, 127)], # Sitar -> Banjo
    105: [(22, 0, 127)], # Banjo
    106: [(22, 0, 127)], # Shamisen -> Banjo
    107: [(22, 0, 127)], # Koto -> Banjo
    108: [(24, 0, 127)], # Kalimba -> Harmonica
    109: [(31, 0, 127)], # Bag pipe -> Pan Flute
    110: [(32, 0, 127)], # Fiddle
    111: [(11, 0, 127)], # Shanai -> Flute

    # Percussive (GM 112-119)
    112: [(54, 0, 127)], # Tinkle Bell -> Tubular Bells
    113: [(36, 0, 127)], # Agogo -> Sleigh Bells
    114: [(26, 0, 127)], # Steel Drums
    115: [(4, 0, 127)],  # Woodblock -> Bongos
    116: [(45, 0, 127)], # Taiko Drum -> Timpani
    117: [(45, 0, 127)], # Melodic Tom -> Timpani
    118: [(45, 0, 127)], # Synth Drum -> Timpani
    119: [(12, 0, 127)], # Reverse Cymbal -> Ride Cymbal

    # Sound Effects (GM 120-127)
    120: [(34, 0, 127)], # Guitar Fret Noise -> Guiro
    121: [(6, 0, 127)],  # Breath Noise -> Gong
    122: [(6, 0, 127)],  # Seashore -> Gong
    123: [(52, 0, 127)], # Bird Tweet -> Choir Aahs
    124: [(6, 0, 127)],  # Telephone Ring -> Gong
    125: [(6, 0, 127)],  # Helicopter -> Gong
    126: [(36, 0, 127)], # Applause -> Sleigh Bells
    127: [(6, 0, 127)],  # Gunshot -> Gong
}

GM_DRUM_TO_GAME = {
    35: 3,   # Acoustic Bass Drum -> Kick Drum
    36: 3,   # Bass Drum 1 -> Kick Drum
    37: 17,  # Side Stick
    38: 13,  # Acoustic Snare -> Snare
    39: 28,  # Hand Clap -> Clave
    40: 13,  # Electric Snare -> Snare
    41: 45,  # Low Floor Tom -> Timpani
    42: 38,  # Closed Hi Hat -> Hi-Hat Closed
    43: 45,  # High Floor Tom -> Timpani
    44: 7,   # Pedal Hi-Hat -> Hi-Hat Pedal
    45: 45,  # Low Tom -> Timpani
    46: 39,  # Open Hi-Hat -> Hi-Hat Open
    47: 45,  # Low-Mid Tom -> Timpani
    48: 45,  # Hi-Mid Tom -> Timpani
    49: 12,  # Crash Cymbal 1 -> Ride Cymbal
    50: 45,  # High Tom -> Timpani
    51: 12,  # Ride Cymbal 1
    52: 12,  # Chinese Cymbal -> Ride Cymbal
    53: 12,  # Ride Bell -> Ride Cymbal
    54: 27,  # Tambourine
    55: 12,  # Splash Cymbal -> Ride Cymbal
    56: 37,  # Cowbell
    57: 12,  # Crash Cymbal 2 -> Ride Cymbal
    58: 33,  # Vibraslap -> Vibraphone
    59: 12,  # Ride Cymbal 2
    60: 4,   # Hi Bongo -> Bongos
    61: 5,   # Low Bongo -> Bongos 2
    62: 4,   # Mute Hi Conga -> Bongos
    63: 4,   # Open Hi Conga -> Bongos
    64: 5,   # Low Conga -> Bongos 2
    65: 45,  # High Timbale -> Timpani
    66: 45,  # Low Timbale -> Timpani
    67: 36,  # High Agogo -> Sleigh Bells
    68: 36,  # Low Agogo -> Sleigh Bells
    69: 34,  # Cabasa -> Guiro
    70: 34,  # Maracas -> Guiro
    71: 11,  # Short Whistle -> Flute
    72: 11,  # Long Whistle -> Flute
    73: 35,  # Short Guiro -> Guiro 2
    74: 34,  # Long Guiro -> Guiro
    75: 28,  # Claves -> Clave
    76: 4,   # Hi Wood Block -> Bongos
    77: 5,   # Low Wood Block -> Bongos 2
    78: 28,  # Mute Cuica -> Clave
    79: 28,  # Open Cuica -> Clave
    80: 36,  # Mute Triangle -> Sleigh Bells
    81: 36,  # Open Triangle -> Sleigh Bells
}

# Instrument Families
PIANO_FAMILY = {9, 10, 25}
STRING_FAMILY = {14, 16, 47}

# Default ADSR Table
# The engine uses 7 bytes per envelope, indexed by 0x90 command.
# Index 0: Melodic/Sustained
# Index 1: Percussive/Short
# Index 2: Piano/Bells
ADSR_TABLE = b"".join([
        bytes([0x07, 0x64, 0x06, 0x73, 0x6D, 0x40, 0x03]),  # 0: Piano
        bytes([0x07, 0x73, 0x01, 0x73, 0x04, 0x5C, 0x02]),  # 1: Bass/general 
        bytes([0x01, 0x73, 0x01, 0x73, 0x01, 0x5A, 0x02]),  # 2: Strings

        # FIXME:
        # Currently using bass envelope for winds (temporary - has fast attack/decay):
        bytes([0x07, 0x73, 0x01, 0x73, 0x04, 0x5C, 0x02]),  # 3: Clarinet/Brass/Winds

        bytes([0x01, 0x5F, 0x01, 0x69, 0x01, 0x5F, 0x01]),  # 4: Tambourine/percussion
        bytes([0x01, 0x73, 0x01, 0x73, 0x17, 0x00, 0x01]),  # 5: Kick Drum
        bytes([0x01, 0x7E, 0x01, 0x7F, 0x3F, 0x00, 0x10]),  # 6: Kick Drum variant/percussion

        bytes([0x07, 0x73, 0x01, 0x73, 0x04, 0x5C, 0x02]),  # 7: Reserved (bass envelope)
        bytes([0x07, 0x73, 0x01, 0x73, 0x04, 0x5C, 0x02]),  # 8: Reserved (bass envelope)
        bytes([0x07, 0x73, 0x01, 0x73, 0x04, 0x5C, 0x02]),  # 9: Reserved (bass envelope)
])

def get_adsr_for_instrument(game_inst: int) -> int:

    # Explicit mappings
    adsr_0 = {1, 5, 11, 14, 16, 28, 32, 38, 40, 41, 46, 57, 58, 2, 22, 12}
    adsr_1 = {3, 4, 27, 37, 44, 55}
    adsr_2 = {9, 59, 23}

    if game_inst in adsr_2: return 2
    if game_inst in adsr_1: return 1
    if game_inst in adsr_0: return 0
    
    # Default fallback
    return 0

def get_game_instrument_from_gm(gm_program: int, midi_note: int) -> int:

    if gm_program not in GM_TO_GAME:
        # Default to piano for unknown instruments
        if midi_note < (60 - 24):
            return 9  # Low Piano
        elif midi_note < (60 - 1):
            return 25  # Mid Piano
        else:
            return 10  # High Piano

    mappings = GM_TO_GAME[gm_program]

    # Find the mapping that covers this note range
    for game_inst, range_min, range_max in mappings:
        if range_min <= midi_note <= range_max:
            return game_inst

    # Default to first mapping if note is out of range
    return mappings[0][0]

def get_game_drum_instrument(gm_drum_note):
    """
    Get the game instrument for a GM drum note.
    Returns game instrument number or None if not mapped.
    """
    return GM_DRUM_TO_GAME.get(gm_drum_note)

def get_best_instrument_for_note(current_instrument: int, note: int) -> int:
    """Switches between Low/Mid/High variants of Piano/Strings"""
    if current_instrument in PIANO_FAMILY:
        if note <= 42: return 9   # Low
        if note <= 54: return 25  # Mid
        return 10                 # High
    
    if current_instrument in STRING_FAMILY:
        if note <= 45: return 47  # Low
        if note <= 60: return 14  # Mid
        return 16                 # High
        
    return current_instrument

def is_multi_patch_instrument(instrument: int) -> bool:
    return instrument in PIANO_FAMILY or instrument in STRING_FAMILY