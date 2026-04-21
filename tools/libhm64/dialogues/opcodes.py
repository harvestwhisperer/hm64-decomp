"""
Dialogue bytecode opcode definitions for HM64.

Shared between the extractor and transpiler.
"""

import re
from enum import IntEnum
from pathlib import Path
from typing import Dict, List, Tuple


class DialogueOpcode(IntEnum):
    """Dialogue bytecode opcodes"""
    SHOW_TEXT = 0
    DIALOGUE_VARIABLE_BRANCH = 1
    UPDATE_DIALOGUE_VARIABLE = 2
    SET_DIALOGUE_VARIABLE = 3
    SPECIAL_DIALOGUE_BIT_BRANCH = 4
    SET_SPECIAL_DIALOGUE_BIT = 5
    CLEAR_SPECIAL_DIALOGUE_BIT = 6
    RANDOM_BRANCH = 7
    JUMP_TO_DIALOGUE = 8
    UNUSED = 9
    SHOW_SELECTION_MENU = 10
    HANDLE_MENU_SELECTION_BRANCH = 11
    END_DIALOGUE = 12


# =============================================================================
# COMMAND SPECIFICATIONS
# =============================================================================
# Format: 'CMD_NAME': (opcode, total_size, [(param_name, param_type), ...])
#
# Parameter types for dialogue bytecode (all little-endian):
#   'u8'    - 8-bit unsigned
#   's8'    - 8-bit signed
#   'u16'   - 16-bit unsigned (little-endian)
#   's16'   - 16-bit signed (little-endian)
#   'rel16' - 16-bit segment index (for branching to other segments)
#
# Note: Dialogue bytecode uses 1-byte opcodes, not 2-byte like cutscenes

COMMAND_SPECS: Dict[str, Tuple[int, int, List[Tuple[str, str]]]] = {
    'SHOW_TEXT': (
        DialogueOpcode.SHOW_TEXT, 3,
        [('text_index', 'u16')]
    ),
    'DIALOGUE_VARIABLE_BRANCH': (
        DialogueOpcode.DIALOGUE_VARIABLE_BRANCH, 10,
        [('var_index', 'u8'), ('min_val', 'u16'), ('max_val', 'u16'),
         ('true_text', 'u16'), ('false_segment', 'rel16')]
    ),
    'UPDATE_DIALOGUE_VARIABLE': (
        DialogueOpcode.UPDATE_DIALOGUE_VARIABLE, 4,
        [('var_index', 'u8'), ('adjustment', 's16')]
    ),
    'SET_DIALOGUE_VARIABLE': (
        DialogueOpcode.SET_DIALOGUE_VARIABLE, 4,
        [('var_index', 'u8'), ('value', 'u16')]
    ),
    'SPECIAL_DIALOGUE_BIT_BRANCH': (
        DialogueOpcode.SPECIAL_DIALOGUE_BIT_BRANCH, 7,
        [('bit_index', 'u16'), ('true_text', 'u16'), ('false_segment', 'rel16')]
    ),
    'SET_SPECIAL_DIALOGUE_BIT': (
        DialogueOpcode.SET_SPECIAL_DIALOGUE_BIT, 3,
        [('bit_index', 'u16')]
    ),
    'CLEAR_SPECIAL_DIALOGUE_BIT': (
        DialogueOpcode.CLEAR_SPECIAL_DIALOGUE_BIT, 3,
        [('bit_index', 'u16')]
    ),
    'RANDOM_BRANCH': (
        DialogueOpcode.RANDOM_BRANCH, 7,
        [('min_val', 'u8'), ('max_val', 'u8'),
         ('true_text', 'u16'), ('false_segment', 'rel16')]
    ),
    'JUMP_TO_DIALOGUE': (
        DialogueOpcode.JUMP_TO_DIALOGUE, 3,
        [('segment', 'rel16')]
    ),
    'UNUSED': (
        DialogueOpcode.UNUSED, 1,
        []
    ),
    'SHOW_SELECTION_MENU': (
        DialogueOpcode.SHOW_SELECTION_MENU, 4,
        [('text_offset', 'u16'), ('unused_field', 'u8')]
    ),
    'HANDLE_MENU_SELECTION_BRANCH': (
        DialogueOpcode.HANDLE_MENU_SELECTION_BRANCH, 4,
        [('unk_18', 'u8'), ('branching_dialogue', 'rel16')]
    ),
    'END_DIALOGUE': (
        DialogueOpcode.END_DIALOGUE, 1,
        []
    ),
}


# =============================================================================
# BUILT-IN CONSTANTS
# =============================================================================

BUILTIN_CONSTANTS = {
    # Special values
    'NO_TEXT': 0xFFFF,
    'NO_BRANCH': 0xFFFF,

    # Dialogue variable indices (VAR_*) are sourced from
    # src/system/dialogue.h and merged in at module load time.

    # VAR_WEATHER values
    'SUNNY':   1,
    'RAIN':    2,
    'SNOW':    3,
    'TYPHOON': 5,

    # VAR_SEASON values
    'SPRING': 1,
    'SUMMER': 2,
    'AUTUMN': 3,
    'WINTER': 4,

    # VAR_ITEM_BEING_HELD categories
    'HELD_ITEM_TURNIP':                0x01,  # TURNIP (13)
    'HELD_ITEM_POTATO':                0x02,  # POTATO (14)
    'HELD_ITEM_CABBAGE':               0x03,  # CABBAGE (15)
    'HELD_ITEM_TOMATO':                0x04,  # TOMATO (16)
    'HELD_ITEM_CORN':                  0x05,  # CORN (17)
    'HELD_ITEM_EGGPLANT':              0x06,  # EGGPLANT (18)
    'HELD_ITEM_STRAWBERRY':            0x07,  # STRAWBERRY (19)
    'HELD_ITEM_EGG':                   0x08,  # EGG_HELD_ITEM (20)
    'HELD_ITEM_SMALL_MILK':            0x09,  # SMALL_MILK (21)
    'HELD_ITEM_MEDIUM_MILK':           0x0A,  # MEDIUM_MILK (22)
    'HELD_ITEM_LARGE_MILK':            0x0B,  # LARGE_MILK (23)
    'HELD_ITEM_GOLDEN_MILK':           0x0C,  # GOLDEN_MILK (24)
    'HELD_ITEM_MOONDROP_FLOWER':       0x0D,  # MOONDROP_FLOWER_HELD_ITEM (4)
    'HELD_ITEM_PINK_CAT_MINT_FLOWER':  0x0E,  # PINK_CAT_MINT_FLOWER_HELD_ITEM (5)
    'HELD_ITEM_CAKE':                  0x0F,  # CAKE (7)
    'HELD_ITEM_PIE':                   0x10,  # PIE (8)
    'HELD_ITEM_COOKIES':               0x11,  # COOKIES (9)
    'HELD_ITEM_WOOL':                  0x12,  # WOOL (25)
    'HELD_ITEM_HIGH_QUALITY_WOOL':     0x13,  # HIGH_QUALITY_WOOL (26)
    'HELD_ITEM_WILD_GRAPES':           0x14,  # WILD_GRAPES (27)
    'HELD_ITEM_VERYBERRY':             0x15,  # VERYBERRY (28)
    'HELD_ITEM_TROPICAL_FRUIT':        0x16,  # TROPICAL_FRUIT (29)
    'HELD_ITEM_WALNUT':                0x17,  # WALNUT (30)
    'HELD_ITEM_MUSHROOM':              0x18,  # MUSHROOM (31)
    'HELD_ITEM_BERRY_OF_THE_FULLMOON': 0x19,  # BERRY_OF_THE_FULLMOON (34)
    'HELD_ITEM_DUMPLING':              0x1A,  # DUMPLING (40)
    'HELD_ITEM_MEDICINAL_HERB':        0x1B,  # MEDICINAL_HERB (35)
    'HELD_ITEM_EDIBLE_HERB':           0x1C,  # EDIBLE_HERB (36)
    'HELD_ITEM_SMALL_FISH':            0x1D,  # SMALL_FISH (37)
    'HELD_ITEM_MEDIUM_FISH':           0x1E,  # MEDIUM_FISH (38)
    'HELD_ITEM_LARGE_FISH':            0x1F,  # LARGE_FISH (39)
    'HELD_ITEM_COTTON_CANDY':          0x20,  # COTTON_CANDY (41)
    'HELD_ITEM_FRIED_OCTUPUS':         0x21,  # FRIED_OCTUPUS (42)
    'HELD_ITEM_CHOCOLATE':             0x22,  # CHOCOLATE (45)
    'HELD_ITEM_CANDY':                 0x23,  # CANDY (44)
    # 0x24-0x28 (36-40): no items remap to these categories
    'HELD_ITEM_FODDER':                0x29,  # FODDER_HELD_ITEM (57)
    'HELD_ITEM_LOG':                   0x2A,  # LOG_HELD_ITEM (3)
    'HELD_ITEM_IRON_ORE':              0x2B,  # IRON_ORE (46)
    'HELD_ITEM_RARE_METAL':            0x2C,  # RARE_METAL (48)
    'HELD_ITEM_BLUE_ROCK':             0x2D,  # BLUE_ROCK (47)
    'HELD_ITEM_MOONLIGHT_STONE':       0x2E,  # MOONLIGHT_STONE (49)
    'HELD_ITEM_ROCK':                  0x2F,  # ROCK_HELD_ITEM (2)
    'HELD_ITEM_WEED':                  0x30,  # WEED_HELD_ITEM (1)
    'HELD_ITEM_POISONOUS_MUSHROOM':    0x31,  # POISONOUS_MUSHROOM (32)
    'HELD_ITEM_BLUE_FEATHER':          0x32,  # BLUE_FEATHER (10)
    'HELD_ITEM_DOG':                   0x33,  # DOG_HELD_ITEM (88) + 7 direction variants
    'HELD_ITEM_BABY':                  0x34,  # BABY_HELD_ITEM (186) + grown + 14 variants
    'HELD_ITEM_HORNED_BEETLE':         0x35,  # HORNED_BEETLE_HELD_ITEM (182)
    'HELD_ITEM_STAG_BEETLE':           0x36,  # STAG_BEETLE_HELD_ITEM (183)
    # 0x37 (55): no items remap to this category
    'HELD_ITEM_CICADA':                0x38,  # CICADA_HELD_ITEM (181)
    'HELD_ITEM_BUTTERFLY':             0x39,  # BUTTERFLY_HELD_ITEM (179)
    'HELD_ITEM_LADYBUG':               0x3A,  # LADYBUG_HELD_ITEM (180)
    'HELD_ITEM_RED_DRAGONFLY':         0x3B,  # RED_DRAGONFLY_HELD_ITEM (184)
    'HELD_ITEM_CRICKET':               0x3C,  # CRICKET_HELD_ITEM (185)
    'HELD_ITEM_PONTATA_ROOT':          0x3D,  # PONTATA_ROOT (50)
    'HELD_ITEM_CHICKEN':               0x3E,  # CHICKEN_HELD_ITEM (96) + 7 direction variants
    'HELD_ITEM_CHICK':                 0x3F,  # CHICK_HELD_ITEM (104) + 7 direction variants
    'HELD_ITEM_ANIMAL':                0x40,  # misc animals
    'HELD_ITEM_PUPPY':                 0x41,  # PUPPY_1_HELD_ITEM (123) + 7 direction variants
    'HELD_ITEM_DOG_2':                 0x42,  # DOG_2_HELD_ITEM (171) + 7 direction variants

    'TRUE': 1,
    'FALSE': 0,
}


# =============================================================================
# Load macro values from C headers
# =============================================================================
# The C header is the single source of truth for named indices into
# specialDialogueBits and dialogue variables. We parse the `#define` block under the

_GAME_STATUS_HEADER = Path(__file__).resolve().parents[3] / "src" / "game" / "gameStatus.h"
_DIALOGUE_HEADER = Path(__file__).resolve().parents[3] / "src" / "system" / "dialogue.h"
_SPECIAL_BIT_START_SENTINEL = "/* special dialogue bits */"
_SPECIAL_BIT_END_SENTINEL = "/* end special dialogue bits */"
_DIALOGUE_VAR_START_SENTINEL = "/* dialogue variables */"
_DIALOGUE_VAR_END_SENTINEL = "/* end dialogue variables */"
_DEFINE_RE = re.compile(r'^\s*#define\s+(\w+)\s+(\S+)')


def _load_sentinel_define_block(header_path: Path, start_sentinel: str, end_sentinel: str) -> Dict[str, int]:
    """Parse `#define NAME VALUE` entries from a sentinel-delimited block of a C header.

    Blank lines inside the block do not truncate parsing; the block ends at the
    end-sentinel line.
    """
    if not header_path.is_file():
        return {}

    macros: Dict[str, int] = {}
    in_block = False

    for line in header_path.read_text(encoding='utf-8').splitlines():
        if not in_block:
            if start_sentinel in line:
                in_block = True
            continue

        if end_sentinel in line:
            break

        match = _DEFINE_RE.match(line)
        if not match:
            continue

        name, raw_value = match.group(1), match.group(2)
        try:
            macros[name] = int(raw_value, 0)
        except ValueError:
            continue

    return macros


def load_special_dialogue_bit_macros(header_path: Path = _GAME_STATUS_HEADER) -> Dict[str, int]:
    """Parse the special-dialogue-bit `#define` block from gameStatus.h."""
    return _load_sentinel_define_block(header_path, _SPECIAL_BIT_START_SENTINEL, _SPECIAL_BIT_END_SENTINEL)


def load_dialogue_variable_macros(header_path: Path = _DIALOGUE_HEADER) -> Dict[str, int]:
    """Parse the VAR_* dialogue variable `#define` block from system/dialogue.h."""
    return _load_sentinel_define_block(header_path, _DIALOGUE_VAR_START_SENTINEL, _DIALOGUE_VAR_END_SENTINEL)


BUILTIN_CONSTANTS.update(load_dialogue_variable_macros())
BUILTIN_CONSTANTS.update(load_special_dialogue_bit_macros())
