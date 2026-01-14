"""
Dialogue bytecode opcode definitions for HM64.

Shared between the extractor and transpiler.
"""

from enum import IntEnum
from typing import Dict, List, Tuple


class DialogueOpcode(IntEnum):
    """Dialogue bytecode opcodes"""
    SHOW_TEXT = 0
    DIALOGUE_VARIABLE_BRANCH = 1
    UPDATE_DIALOGUE_VARIABLE = 2
    SET_DIALOGUE_VARIABLE = 3
    SPECIAL_DIALOGUE_BIT_BRANCH = 4
    SET_SPECIAL_DIALOGUE_BIT = 5
    TOGGLE_SPECIAL_DIALOGUE_BIT = 6
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
    'TOGGLE_SPECIAL_DIALOGUE_BIT': (
        DialogueOpcode.TOGGLE_SPECIAL_DIALOGUE_BIT, 3,
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

    # Dialogue variables
    'VAR_ALCOHOL_TOLERANCE': 0x00,
    'VAR_SEASON': 0x01,
    'VAR_HOUR': 0x02,
    'VAR_WEATHER': 0x03,
    'VAR_DAY_OF_WEEK': 0x04,
    'VAR_YEAR': 0x05,
    'VAR_ITEM_BEING_HELD': 0x06,

    'VAR_AFFECTION_MARIA': 0x07,
    'VAR_AFFECTION_POPURI': 0x08,
    'VAR_AFFECTION_ELLI': 0x09,
    'VAR_AFFECTION_ANN': 0x0A,
    'VAR_AFFECTION_KAREN': 0x0B,
    'VAR_AFFECTION_HARRIS': 0x0C,
    'VAR_AFFECTION_GRAY': 0x0D,
    'VAR_AFFECTION_JEFF': 0x0E,
    'VAR_AFFECTION_CLIFF': 0x0F,
    'VAR_AFFECTION_KAI': 0x10,
    'VAR_AFFECTION_MAYOR': 0x11,
    'VAR_AFFECTION_MAYOR_WIFE': 0x12,
    'VAR_AFFECTION_LILLIA': 0x13,
    'VAR_AFFECTION_BASIL': 0x14,
    'VAR_AFFECTION_ELLEN': 0x15,
    'VAR_AFFECTION_PASTOR': 0x16,
    'VAR_AFFECTION_RICK': 0x17,
    'VAR_AFFECTION_SAIBARA': 0x18,
    'VAR_AFFECTION_POTION_SHOP_DEALER': 0x19,
    'VAR_AFFECTION_KENT': 0x1A,
    'VAR_AFFECTION_STU': 0x1B,
    'VAR_AFFECTION_MIDWIFE': 0x1C,
    'VAR_AFFECTION_MAY': 0x1D,
    'VAR_AFFECTION_CARPENTER_1': 0x1E,
    'VAR_AFFECTION_CARPENTER_2': 0x1F,
    'VAR_AFFECTION_MASTER_CARPENTER': 0x20,
    'VAR_AFFECTION_HARVEST_SPRITE_1': 0x21,
    'VAR_AFFECTION_SYDNEY': 0x22,
    'VAR_AFFECTION_BARLEY': 0x23,
    'VAR_AFFECTION_GREG': 0x24,
    'VAR_AFFECTION_BABY': 0x25,

    'VAR_TOTAL_BACHELORETTE_AFFECTION': 0x26,
    'VAR_DAY_OF_MONTH': 0x27,
    'VAR_GOLD': 0x28,
    'VAR_TOTAL_GRASS_TILES': 0x29,
    'VAR_TOTAL_PINK_CAT_MINT_FLOWERS': 0x2A,

    'VAR_CROPS_SHIPPED_1': 0x2B,
    'VAR_CROPS_SHIPPED_2': 0x2C,
    'VAR_CROPS_SHIPPED_3': 0x2D,
    'VAR_CROPS_SHIPPED_4': 0x2E,
    'VAR_CROPS_SHIPPED_5': 0x2F,
    'VAR_CROPS_SHIPPED_6': 0x30,

    'VAR_AFFECTION_DOUG': 0x31,
    'VAR_AFFECTION_GOTZ': 0x32,
    'VAR_AFFECTION_SASHA': 0x33,
    'VAR_AFFECTION_SHIPPER': 0x34,
    'VAR_AFFECTION_DUKE': 0x35,

    'VAR_AFFECTION_HORSE': 0x36,
    'VAR_AFFECTION_FIRST_FARM_ANIMAL': 0x37,
    'VAR_AFFECTION_DOG': 0x38,

    'VAR_ELLI_GRIEVING_COUNTER': 0x39,

    'VAR_BABY_AGE': 0x3A,
    'VAR_WIFE_CONCEPTION_COUNTER': 0x3B,
    'VAR_WIFE_CONCEPTION_COUNTER_2': 0x3C,
    'VAR_BABY_AGE_2': 0x3D,
    'VAR_UNUSED_3E': 0x3E,
    'VAR_BABY_AGE_3': 0x3F,
    'VAR_UNUSED_40': 0x40,
    'VAR_BABY_AGE_4': 0x41,
    'VAR_BABY_AGE_5': 0x42,
    'VAR_UNUSED_43': 0x43,
    'VAR_UNUSED_44': 0x44,
    'VAR_WIFE_PREGNANCY_COUNTER': 0x45,
    'VAR_WIFE_PREGNANCY_COUNTER_2': 0x46,
    'VAR_UNUSED_47': 0x47,
    'VAR_UNUSED_48': 0x48,
    'VAR_WIFE_PREGNANCY_COUNTER_3': 0x49,
    'VAR_WIFE_PREGNANCY_COUNTER_4': 0x4A,
    'VAR_WIFE_PREGNANCY_COUNTER_5': 0x4B,

    'TRUE': 1,
    'FALSE': 0,
}
