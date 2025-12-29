#!/usr/bin/env python3

import struct
import argparse
import sys
import csv
from pathlib import Path
from dataclasses import dataclass, field
from typing import List, Tuple, Optional, Dict, Set
from enum import IntEnum
from collections import deque

try:
    from rom_segment_symbols import ROM_SEGMENT_START_SYMBOLS, ROM_SEGMENT_END_SYMBOLS
except ImportError:
        ROM_SEGMENT_START_SYMBOLS = {}
        ROM_SEGMENT_END_SYMBOLS = {}

ROM_PATH = Path("../baserom.us.z64")

rom = None

def set_rom():
    global rom
    rom = memoryview(ROM_PATH.read_bytes())

# =============================================================================
# CONSTANTS & ENUMS
# =============================================================================

class CutsceneOpcode(IntEnum):
    """Cutscene bytecode opcodes (indices into cutsceneCommandHandlers array)"""
    SET_ANIM_DATA_PTR_WITH_FLAG = 0
    SET_ANIM_DATA_PTR = 1
    SET_COORDINATES = 2
    SET_FRAME_DELTA = 3
    SET_WAIT_FRAMES = 4
    DEACTIVATE_SELF = 5
    EXECUTE_SUBROUTINE = 6
    RETURN_FROM_SUBROUTINE = 7
    BRANCH_ON_CURRENT_BUTTON = 8
    BRANCH_ON_BUTTON_PRESSED = 9
    BRANCH_ON_BUTTON_REPEAT = 10
    SPAWN_EXECUTOR = 11
    SET_OTHER_EXECUTOR_PTR = 12
    DEACTIVATE_EXECUTOR = 13
    DMA_SPRITE = 14
    SET_ENTITY_ANIMATIONS = 15
    DO_DMA = 16
    SET_U8_VALUE = 17
    SET_U16_VALUE = 18
    SET_U32_VALUE = 19
    BRANCH_U8_VAR_WITHIN_RANGE = 20
    BRANCH_U16_VAR_WITHIN_RANGE = 21
    BRANCH_U32_VAR_WITHIN_RANGE = 22
    SET_SPECIAL_BIT = 23
    TOGGLE_SPECIAL_BIT = 24
    BRANCH_ON_SPECIAL_BIT = 25
    SET_ENTITY_OR_MAP_ROTATION = 26
    SETUP_MAP_ASSET = 27
    ENTITY_MOVE_AND_ANIMATE = 28
    SET_MAP_ROTATION = 29
    SET_CAMERA_TRACKING_FLAG = 30
    SET_MOVEMENT_INFO = 31
    INIT_MESSAGE_BOX_TYPE1 = 32
    WAIT_MESSAGE_BOX = 33
    SET_MESSAGE_BOX_POSITION = 34
    CLEAR_MESSAGE_BOX_FLAG = 35
    ENTITY_RUN = 36
    SET_ENTITY_ANIMATION = 37
    SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE = 38
    SET_CALLBACK_ROUTINE = 39
    PAUSE_ENTITY = 40
    TOGGLE_PAUSE_ENTITY = 41
    FLIP_ENTITY_DIRECTION = 42
    SET_ENTITY_FLAG_1 = 43
    SET_ENTITY_FLAG_2 = 44
    SET_ENTITY_FLAG_3 = 45
    SET_ENTITY_NON_COLLIDABLE = 46
    SETUP_ENTITY = 47
    SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG = 48
    LOAD_MAP = 49
    SET_ENTITY_MAP_SPACE_INDEPENDENT = 50
    SET_RGBA = 51
    UPDATE_RGBA = 52
    UPDATE_U8_VALUE = 53
    UPDATE_U16_VALUE = 54
    UPDATE_U32_VALUE = 55
    DEACTIVATE_MAP_OBJECTS = 56
    UPDATE_GLOBAL_RGBA = 57
    DEACTIVATE_SPRITES = 58
    DEACTIVATE_MAP_CONTROLLERS = 59
    WAIT_RGBA_FINISHED = 60
    CHECK_ENTITY_COLLISION = 61
    INIT_DIALOGUE_SESSION = 62
    WAIT_FOR_DIALOGUE = 63
    BRANCH_ON_DIALOGUE = 64
    WAIT_ENTITY_ANIMATION = 65
    SET_MESSAGE_BOX_ASSETS = 66
    SET_ENTITY_TRACKING_TARGET = 67
    UPDATE_CAMERA_FLAGS = 68
    WAIT_MAP_LOAD = 69
    BRANCH_ON_ENTITY_DIR = 70
    SET_ENTITY_PHYSICS_FLAGS = 71
    SET_ENTITY_PALETTE = 72
    SET_ENTITY_DIMENSIONS = 73
    SET_SHADOW_FLAGS = 74
    SET_SPRITE_SCALE = 75
    SET_SPRITE_RENDERING_LAYER = 76
    INIT_MESSAGE_BOX_TYPE2 = 77
    INIT_MAP_ADDITION = 78
    BRANCH_ON_RANDOM = 79
    BRANCH_ON_U16_PTR_RANGE = 80
    PAUSE_EXECUTOR = 81
    TOGGLE_PAUSE_EXECUTOR = 82
    PAUSE_ALL_CHILD_EXECUTORS = 83
    TOGGLE_PAUSE_ALL_CHILDREN = 84
    SET_SPRITE_PALETTE = 85
    BRANCH_ON_U8_PTR_RANGE = 86
    SET_AUDIO_SEQUENCE = 87
    STOP_SONG_WITH_FADE = 88
    SET_AUDIO_SEQUENCE_VOLUME = 89
    SET_SFX = 90
    IDLE_WHILE_SONG_PLAYING = 91
    UPDATE_MESSAGE_BOX_RGBA = 92
    WAIT_MESSAGE_BOX_READY = 93
    SET_SPRITE_BILINEAR = 94
    SET_MAP_ADDITION = 95
    SET_MAP_GROUND_OBJECT = 96
    SET_MESSAGE_INTERPOLATION = 97

# Maps Opcode ID -> (Opcode Name, List of parameter types for ASM generation)
# Types: 'u8', 'u16', 'u32', 's8', 's16', 's32', 'rel16' (label - .), 'addr32' (symbol), 'pad8', 'pad16'
ASM_COMMAND_SPECS = {
    0: ("CMD_SET_ANIM_DATA_PTR_WITH_FLAG", ['rel16']),
    1: ("CMD_SET_ANIM_DATA_PTR", ['rel16']),
    2: ("CMD_SET_COORDINATES", ['s16', 's16', 's16']),
    3: ("CMD_SET_FRAME_DELTA", ['s8', 's8', 's8', 'u8', 'pad16']),
    4: ("CMD_SET_WAIT_FRAMES", ['u16']),
    5: ("CMD_DEACTIVATE_SELF", ['pad16']),
    6: ("CMD_EXECUTE_SUBROUTINE", ['rel16']),
    7: ("CMD_RETURN_FROM_SUBROUTINE", ['pad16']),
    8: ("CMD_BRANCH_ON_CURRENT_BUTTON", ['u16', 'u32', 'rel16', 'pad16']),
    9: ("CMD_BRANCH_ON_BUTTON_PRESSED", ['u16', 'u32', 'rel16', 'pad16']),
    10: ("CMD_BRANCH_ON_BUTTON_REPEAT", ['u16', 'u32', 'rel16', 'pad16']),
    11: ("CMD_SPAWN_EXECUTOR", ['u16', 'rel16', 'pad16']),
    12: ("CMD_SET_OTHER_EXECUTOR_PTR", ['u16', 'rel16', 'pad16']),
    13: ("CMD_DEACTIVATE_EXECUTOR", ['u16']),
    14: ("CMD_DMA_SPRITE", ['u16', 'u16', 'pad16', 'rom_start32', 'rom_end32', 'rom_start32', 'rom_end32', 'rom_start32', 'rom_end32', 'addr32', 'addr32', 'addr32', 'addr32', 'addr32', 'addr32']),
    15: ("CMD_SET_ENTITY_ANIMATIONS", ['u16', 'u16', 'u16', 'u16', 'u16', 'u16', 'u16']),
    16: ("CMD_DO_DMA", ['pad16', 'rom_start32', 'rom_end32', 'addr32']),
    17: ("CMD_SET_U8_VALUE", ['u8', 'pad8', 'addr32']), # func_80048B90
    18: ("CMD_SET_U16_VALUE", ['u16', 'addr32']), # func_80048BEC
    19: ("CMD_SET_U32_VALUEE", ['pad16', 'addr32', 'u32']), 
    20: ("CMD_BRANCH_U8_VAR_WITHIN_RANGE", ['u8', 'u8', 'addr32', 'rel16', 'pad16']),  # func_80048CA4
    21: ("CMD_BRANCH_U16_VAR_WITHIN_RANGE", ['u16', 'u16', 'pad16', 'addr32', 'rel16', 'pad16']), # func_80048DA8
    22: ("CMD_BRANCH_U32_VAR_WITHIN_RANGE", ['pad16', 'u32', 'u32', 'addr32', 'rel16', 'pad16']), # func_80048E98
    23: ("CMD_SET_SPECIAL_BIT", ['u16', 'addr32']),
    24: ("CMD_TOGGLE_SPECIAL_BIT", ['u16', 'addr32']),
    25: ("CMD_BRANCH_ON_SPECIAL_BIT", ['u16', 'addr32', 'rel16', 'pad16']),
    26: ("CMD_SET_ASSET_ROTATION", ['u8', 'pad8']), # func_8004910C
    27: ("CMD_SETUP_MAP_ASSET", ['u16']), # func_80049228
    28: ("CMD_ENTITY_MOVE_AND_ANIMATE", ['u16', 'u8', 'u8', 'pad16']), # func_80049350
    29: ("CMD_SET_MAP_ROTATION_WITH_FLAG", ['u16', 'u8', 'u8', 'pad16']), # func_800495F0
    30: ("CMD_SET_CAMERA_TRACKING_FLAG", ['u8', 'pad8']), # func_8004969C
    31: ("CMD_SET_MOVEMENT_INFO", ['pad8', 'u8', 'u16', 'u16']),
    32: ("CMD_INIT_MESSAGE_BOX_TYPE1", ['u16', 'u16', 'u16']),
    33: ("CMD_WAIT_MESSAGE_BOX_CLOSED", ['u16']), # func_800498B0
    34: ("CMD_SET_MESSAGE_BOX_VIEW_POSITION", ['u16', 's16', 's16', 's16', 'pad16']),
    35: ("CMD_RESET_MESSAGE_BOX_AVATAR", ['u16']), # func_80049A28
    36: ("CMD_ENTITY_MOVE_AND_ANIMATE_2", ['u16', 'pad16', 'u8', 'u8']), # func_80049AC4
    37: ("CMD_SET_ENTITY_ANIMATION", ['u16']),
    38: ("CMD_SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE", ['u16']),
    39: ("CMD_SET_CALLBACK_ROUTINE", ['u16', 'u16', 'rel16']),
    40: ("CMD_PAUSE_ENTITY", ['u16']),
    41: ("CMD_TOGGLE_PAUSE_ENTITY", ['u16']),
    42: ("CMD_FLIP_ENTITY_DIRECTION", ['u16']),
    43: ("CMD_PAUSE_ENTITIES", ['pad16']),
    44: ("CMD_TOGGLE_PAUSE_ENTITIES", ['pad16']), # func_8004A0F4
    45: ("CMD_FLIP_ENTITY_ANIMATION", ['pad16']), # func_8004A140
    46: ("CMD_SET_ENTITY_NON_COLLIDABLE", ['pad16']), 
    47: ("CMD_SETUP_ENTITY", ['u16', 'u16', 'u16']),
    48: ("CMD_SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG", ['u16']), # func_8004A320
    49: ("CMD_LOAD_MAP", ['u16', 'u16', 'pad16']),
    50: ("CMD_SET_ENTITY_MAP_SPACE_INDEPENDENT", ['u16']),
    51: ("CMD_SET_RGBA", ['u8', 'u8', 'u8', 'u8', 'pad16']),
    52: ("CMD_UPDATE_RGBA", ['u8', 'u8', 'u8', 'u8', 's16']),
    53: ("CMD_UPDATE_U8_VALUE", ['pad8', 's8', 'pad8', 'u8', 'pad16', 'addr32']),
    54: ("CMD_UPDATE_U16_VALUE", ['s16', 'u16', 'pad16', 'addr32']),
    55: ("CMD_UPDATE_U32_VALUE", ['pad16', 's32', 's32', 'addr32']),
    56: ("CMD_DEACTIVATE_MAP_OBJECTS", ['u16']), # func_8004ACE4
    57: ("CMD_UPDATE_GLOBAL_RGBA", ['u8', 'u8', 'u8', 'u8', 's16']),
    58: ("CMD_DEACTIVATE_SPRITES", ['pad16']),
    59: ("CMD_DEACTIVATE_MAP_CONTROLLERS", ['pad16']),
    60: ("CMD_WAIT_RGBA_FINISHED", ['pad16']), # func_8004B0E8
    61: ("CMD_CHECK_ENTITY_COLLISION", ['u16', 'rel16', 'pad16']),
    62: ("CMD_INIT_DIALOGUE_SESSION", ['u16', 'u16', 'u16']),
    63: ("CMD_WAIT_FOR_DIALOGUE_INPUT", ['u16']),
    64: ("CMD_BRANCH_ON_DIALOGUE", ['u16', 'u16', 'pad16', 'rel16', 'pad16']), # func_8004B538
    65: ("CMD_WAIT_ENTITY_ANIMATION", ['pad16']), # func_8004B5F0
    66: ("CMD_SET_MESSAGE_BOX_ASSETS", ['u16', 'u8', 'u8', 'u8', 'pad8']),
    67: ("CMD_SET_ENTITY_TRACKING_TARGET", ['u16', 's16', 's16', 's16', 'u8', 'pad8']),
    68: ("CMD_UPDATE_CAMERA_FLAGS", ['u16']), # func_8004B920
    69: ("CMD_WAIT_MAP_LOAD", ['u16']), # func_8004B9A0
    70: ("CMD_BRANCH_ON_ENTITY_DIR", ['u16', 'u8', 'pad8', 'rel16']),
    71: ("CMD_SET_ENTITY_PHYSICS_FLAGS", ['u16']),
    72: ("CMD_SET_ENTITY_PALETTE", ['u16']),
    73: ("CMD_SET_ENTITY_DIMENSIONS", ['u8', 'u8']),
    74: ("CMD_SET_SHADOW_FLAGS", ['u16', 'u16', 'pad16']), # func_8004BE14
    75: ("CMD_SET_SPRITE_SCALE", ['u16', 'u16', 'u16']),
    76: ("CMD_SET_SPRITE_RENDERING_LAYER", ['u16']),
    77: ("CMD_INIT_MESSAGE_BOX_TYPE2", ['u16', 'u16', 'u16']),
    78: ("CMD_INIT_MAP_ADDITION", ['u16', 'u16', 'pad16']), # func_8004C0D0
    79: ("CMD_BRANCH_ON_RANDOM", ['u16', 'rel16', 'pad16']), # func_8004C148
    80: ("CMD_BRANCH_IF_U16_PTR_IN_RANGE", ['pad16', 'addr32', 'addr32', 'addr32', 'rel16', 'pad16']), # func_8004C258
    81: ("CMD_PAUSE_EXECUTOR", ['u16']),
    82: ("CMD_TOGGLE_PAUSE_EXECUTOR", ['u16']),
    83: ("CMD_PAUSE_ALL_CHILD_EXECUTORS", ['pad16']),
    84: ("CMD_TOGGLE_PAUSE_ALL_CHILDREN", ['pad16']),
    85: ("CMD_SET_SPRITE_PALETTE", ['u16']),
    86: ("CMD_BRANCH_IF_U8_PTR_RANGE", ['u8', 'u8', 'addr32', 'addr32', 'addr32', 'rel16', 'pad16']), # func_8004C5D8
    87: ("CMD_SET_AUDIO_SEQUENCE", ['u16', 'rom_start32', 'rom_end32']),
    88: ("CMD_STOP_SONG_WITH_FADE", ['u16', 'u16', 'pad16']),
    89: ('CMD_SET_AUDIO_SEQUENCE_VOLUME', ['u16', 'u16', 's16']),
    90: ("CMD_SET_SFX", ['u16', 'u16', 'pad16']),
    91: ("CMD_IDLE_WHILE_SONG_PLAYING", ['u16']),
    92: ("CMD_UPDATE_MESSAGE_BOX_RGBA", ['u16', 'u8', 'u8', 'u8', 'u8', 'u16', 'pad16']),
    93: ("CMD_WAIT_MESSAGE_BOX_READY", ['u16']), # func_8004CA80
    94: ("CMD_SET_SPRITE_BILINEAR", ['u16']),
    95: ("CMD_SET_MAP_ADDITION", ['u16', 'u8', 'u8', 'pad16']),
    96: ("CMD_SET_MAP_GROUND_OBJECT", ['u16', 'u16', 'u16']),
    97: ("CMD_SET_MESSAGE_INTERPOLATION", ['s16', 's16', 's16']), # func_8004CCF0
}

OPCODE_NAMES = {k: v[0] for k, v in ASM_COMMAND_SPECS.items()}

BUTTON_CONSTANTS = {
    0x0001: "BUTTON_C_RIGHT",
    0x0002: "BUTTON_C_LEFT",
    0x0004: "BUTTON_C_DOWN",
    0x0008: "BUTTON_C_UP",
    0x0010: "BUTTON_R",
    0x0020: "BUTTON_L",
    0x0100: "BUTTON_D_RIGHT",
    0x0200: "BUTTON_D_LEFT",
    0x0400: "BUTTON_D_DOWN",
    0x0800: "BUTTON_D_UP",
    0x1000: "BUTTON_START",
    0x2000: "BUTTON_Z",
    0x4000: "BUTTON_B",
    0x8000: "BUTTON_A",
    0x10000: "BUTTON_STICK_UP",
    0x20000: "BUTTON_STICK_DOWN",
    0x40000: "BUTTON_STICK_LEFT",
    0x80000: "BUTTON_STICK_RIGHT"
}

ENTITY_CONSTANTS = {
    0: "ENTITY_PLAYER",
}

ENTITY_ASSET_CONSTANTS = {
    0: "ENTITY_ASSET_PLAYER",
    1: "ENTITY_ASSET_MARIA",
    2: "ENTITY_ASSET_POPURI",
    3: "ENTITY_ASSET_ELLI",
    4: "ENTITY_ASSET_ANN",
    5: "ENTITY_ASSET_KAREN",
    6: "ENTITY_ASSET_BABY",
    7: "ENTITY_ASSET_HARRIS",
    8: "ENTITY_ASSET_GRAY",
    9: "ENTITY_ASSET_JEFF",
    10: "ENTITY_ASSET_CLIFF",
    11: "ENTITY_ASSET_KAI",
    12: "ENTITY_ASSET_MAYOR",
    13: "ENTITY_ASSET_MAYOR_WIFE",
    14: "ENTITY_ASSET_LILIA",
    15: "ENTITY_ASSET_BASIL",
    16: "ENTITY_ASSET_ELLEN",
    17: "ENTITY_ASSET_DOUG",
    18: "ENTITY_ASSET_GOTZ",
    19: "ENTITY_ASSET_SASHA",
    20: "ENTITY_ASSET_POTION_SHOP_DEALER",
    21: "ENTITY_ASSET_KENT",
    22: "ENTITY_ASSET_STU",
    23: "ENTITY_ASSET_MIDWIFE",
    24: "ENTITY_ASSET_MAY",
    25: "ENTITY_ASSET_RICK",
    26: "ENTITY_ASSET_PASTOR",
    27: "ENTITY_ASSET_SHIPPER",
    28: "ENTITY_ASSET_SAIBARA",
    29: "ENTITY_ASSET_DUKE",
    30: "ENTITY_ASSET_GREG",
    31: "ENTITY_ASSET_ASSISTANT_CARPENTER",
    32: "ENTITY_ASSET_MASTER_CARPENTER",
    33: "ENTITY_ASSET_HARVEST_SPRITE",
    34: "ENTITY_ASSET_BARLEY",
    35: "ENTITY_ASSET_SYDNEY",
    36: "ENTITY_ASSET_ENTOMOLOGIST",
    37: "ENTITY_ASSET_GOURMET_JUDGE",
    38: "ENTITY_ASSET_PHOTOGRAPHER",
    39: "ENTITY_ASSET_SHADY_SALESMAN",
    40: "ENTITY_ASSET_MRS_MANA_SON",
    41: "ENTITY_ASSET_MRS_MANA",
    42: "ENTITY_ASSET_OLD_MAN",
    43: "ENTITY_ASSET_OLD_WOMAN",
    44: "ENTITY_ASSET_TOURIST_COUPLE_MAN",
    45: "ENTITY_ASSET_TOURIST_COUPLE_WOMAN",
    46: "ENTITY_ASSET_FESTIVAL_GIRL1",
    47: "ENTITY_ASSET_FESTIVAL_GIRL2",
    48: "ENTITY_ASSET_FESTIVAL_GIRL3",
    49: "ENTITY_ASSET_HARVEST_GODDESS",
    50: "ENTITY_ASSET_SPIRIT_FESTIVAL_OCARINA_PERFORMER",
    51: "ENTITY_ASSET_KAPPA",
    52: "ENTITY_ASSET_PRINCESS",
    53: "ENTITY_ASSET_EMPTY_NPC_SPRITE",
    54: "ENTITY_ASSET_BUNNY",
    55: "ENTITY_ASSET_MOUSE",
    56: "ENTITY_ASSET_FOX",
    57: "ENTITY_ASSET_MONKEY",
    58: "ENTITY_ASSET_HAWK",
    59: "ENTITY_ASSET_MOLE",
    60: "ENTITY_ASSET_SQUIRREL",
    61: "ENTITY_ASSET_DOG_RACE",
    62: "ENTITY_ASSET_CAT",
    63: "ENTITY_ASSET_DOG",
    64: "ENTITY_ASSET_DOG_TITLE",
    65: "ENTITY_ASSET_HORSE_PONY",
    66: "ENTITY_ASSET_HORSE_GROWN",
    67: "ENTITY_ASSET_CHICK",
    68: "ENTITY_ASSET_CHICKEN",
    69: "ENTITY_ASSET_CALF",
    70: "ENTITY_ASSET_YOUNG_COW",
    71: "ENTITY_ASSET_COW",
    72: "ENTITY_ASSET_LAMB",
    73: "ENTITY_ASSET_SHEEP",
    74: "ENTITY_ASSET_FROG",
    75: "ENTITY_ASSET_CRAB",
    76: "ENTITY_ASSET_HORNED_BEETLE",
    77: "ENTITY_ASSET_STAG_BEETLE",
    78: "ENTITY_ASSET_BELL_RING_CRICKET",
    79: "ENTITY_ASSET_CICADA",
    80: "ENTITY_ASSET_BIRD",
    81: "ENTITY_ASSET_LADYBUG",
    82: "ENTITY_ASSET_DRAGONFLY",
    83: "ENTITY_ASSET_GRASSHOPPER",
    84: "ENTITY_ASSET_SNAKE",
    85: "ENTITY_ASSET_PURPLE_BUTTERFLY",
    86: "ENTITY_ASSET_DAD",
    87: "ENTITY_ASSET_COW_INDOORS",
    88: "ENTITY_ASSET_HORSE_UNBRIDLED",
    89: "ENTITY_ASSET_HORSE_RACER",
    90: "ENTITY_ASSET_EMPTY_ENTITY",
    91: "ENTITY_ASSET_HOLDABLE_ITEMS_1",
    92: "ENTITY_ASSET_EXTRA_FESTIVAL_ITEMS",
    93: "ENTITY_ASSET_HOLDABLE_ITEMS_2",
    94: "ENTITY_ASSET_HOLDABLE_ITEMS_3",
    95: "ENTITY_ASSET_TOOLS",
    96: "ENTITY_ASSET_HOLDABLE_ITEMS_4",
    97: "ENTITY_ASSET_MARIA_HARRIS_BABY",
    98: "ENTITY_ASSET_POPURI_GRAY_BABY",
    99: "ENTITY_ASSET_ELLI_JEFF_BABY",
    100: "ENTITY_ASSET_ANN_CLIFF_BABY",
    101: "ENTITY_ASSET_KAREN_KAI_BABY"
}

MAP_CONSTANTS = {
    0: "RANCH_SPRING", 
    1: "RANCH_SUMMER",
    2: "RANCH_AUTUMN",
    3: "RANCH_WINTER",
    4: "ANN_ROOM",
    5: "RANCH_BARN",
    6: "RANCH_STORE",
    7: "RANCH_HOUSE",
    8: "EMPTY_MAP_1",
    9: "BEACH_SPRING", 
    0xA: "BEACH_SUMMER",
    0xB: "BEACH_AUTUMN",
    0xC: "BEACH_WINTER",
    0xD: "RACE_TRACK_SPRING",
    0xE: "RACE_TRACK_SUMMER",
    0xF: "RACE_TRACK_AUTUMN",
    0x10: "RACE_TRACK_WINTER",
    0x11: "MOUNTAIN_1_SPRING",
    0x12: "MOUNTAIN_1_SUMMER",
    0x13: "MOUNTAIN_1_AUTUMN",
    0x14: "MOUNTAIN_1_WINTER",
    0x15: "MOUNTAIN_2_SPRING",
    0x16: "MOUNTAIN_2_SUMMER",
    0x17: "MOUNTAIN_2_AUTUMN",
    0x18: "MOUNTAIN_2_WINTER",
    0x19: "TOP_OF_MOUNTAIN_1_SPRING",
    0x1A: "TOP_OF_MOUNTAIN_1_SUMMER",
    0x1B: "TOP_OF_MOUNTAIN_1_AUTUMN",
    0x1C: "TOP_OF_MOUNTAIN_1_WINTER",
    0x1D: "MOON_MOUNTAIN_SPRING",
    0x1E: "MOON_MOUNTAIN_SUMMER",
    0x1F: "MOON_MOUNTAIN_AUTUMN",
    0x20: "MOON_MOUNTAIN_WINTER",
    0x21: "CARPENTER_HUT",
    0x22: "DUMPLING_HOUSE",
    0x23: "POND_SPRING",
    0x24: "POND_SUMMER",
    0x25: "POND_AUTUMN",
    0x26: "POND_WINTER",
    0x27: "ELLI_ROOM",
    0x28: "BAKERY",
    0x29: "VILLAGE_1_SPRING",
    0x2A: "VILLAGE_1_SUMMER",
    0x2B: "VILLAGE_1_AUTUMN",
    0x2C: "VILLAGE_1_WINTER",
    0x2D: "VILLAGE_2_SPRING",
    0x2E: "VILLAGE_2_SUMMER",
    0x2F: "VILLAGE_2_AUTUMN",
    0x30: "VILLAGE_2_WINTER",
    0x31: "POPURI_ROOM",
    0x32: "FLOWER_SHOP",
    0x33: "CHURCH",
    0x34: "SOUVENIR_SHOP",
    0x35: "SQUARE_SPRING",
    0x36: "SQUARE_SUMMER",
    0x37: "SQUARE_AUTUMN",
    0x38: "SQUARE_WINTER",
    0x39: "RICK_STORE",
    0x3A: "MIDWIFE_HOUSE",
    0x3B: "TAVERN",
    0x3C: "LIBRARY",
    0x3D: "MARIA_ROOM",
    0x3E: "MAYOR_HOUSE",
    0x3F: "POTION_SHOP_BEDROOM",
    0x40: "POTION_SHOP",
    0x41: "EMPTY_MAP_2",
    0x42: "HARVEST_SPRITE_CAVE",
    0x43: "CAVE",
    0x44: "MINE",
    0x45: "MINE_2",
    0x46: "KAREN_ROOM",
    0x47: "VINEYARD_SPRING",
    0x48: "VINEYARD_SUMMER",
    0x49: "VINEYARD_AUTUMN",
    0x4A: "VINEYARD_WINTER",
    0x4B: "VINEYARD_HOUSE",
    0x4C: "VINEYARD_CELLAR",
    0x4D: "VINEYARD_CELLAR_BASEMENT",
    0x4E: "ROAD_SPRING",
    0x4F: "ROAD_SUMMER",
    0x50: "ROAD_AUTUMN",
    0x51: "ROAD_WINTER",
    0x52: "FARM_SPRING",
    0x53: "FARM_SUMMER",
    0x54: "FARM_AUTUMN",
    0x55: "FARM_WINTER",
    0x56: "GREENHOUSE",
    0x57: "HOUSE",
    0x58: "BARN",
    0x59: "COOP",
    0x5A: "KITCHEN",
    0x5B: "BATHROOM"
}

DIRECTION_CONSTANTS = {
    0: "SOUTHWEST", 
    1: "WEST", 
    2: "NORTHWEST", 
    3: "NORTH",
    4: "NORTHEAST", 
    5: "EAST", 
    6: "SOUTHEAST", 
    7: "SOUTH"
}

TEXT_BANK_CONSTANTS = {
    0: "DIALOGUE_0_TEXT_INDEX", 
    1: "LIBRARY_TEXT_INDEX", 
    2: "DIARY_TEXT_INDEX", 
    3: "DIALOGUE_3_TEXT_INDEX",
    4: "FESTIVAL_OVERLAY_SELECTIONS_TEXT_INDEX", 
    5: "LETTERS_TEXT_INDEX", 
    6: "SHOP_TEXT_INDEX",
    7: "ANIMAL_INTERACTIONS_TEXT_INDEX", 
    8: "TV_TEXT_INDEX", 
    9: "DIALOGUE_9_TEXT_INDEX",
    10: "NAMING_SCREEN_TEXT_INDEX", 
    11: "ELLI_TEXT_INDEX", 
    12: "KAI_TEXT_INDEX", 
    13: "KAREN_TEXT_INDEX",
    14: "GOTZ_TEXT_INDEX", 
    15: "SASHA_TEXT_INDEX", 
    16: "CLIFF_TEXT_INDEX", 
    17: "JEFF_TEXT_INDEX",
    18: "KENT_TEXT_INDEX", 
    19: "HARRIS_TEXT_INDEX", 
    20: "POPURI_TEXT_INDEX", 
    21: "MARIA_TEXT_INDEX",
    22: "MAY_TEXT_INDEX", 
    23: "ANN_TEXT_INDEX", 
    24: "DOUG_TEXT_INDEX", 
    25: "GRAY_TEXT_INDEX",
    26: "BASIL_TEXT_INDEX", 
    27: "LILIA_TEXT_INDEX", 
    28: "DUKE_TEXT_INDEX", 
    29: "SHIPPER_TEXT_INDEX",
    30: "HARVEST_SPRITES_TEXT_INDEX", 
    31: "ASSISTANT_CARPENTERS_TEXT_INDEX", 
    32: "MASTER_CARPENTER_TEXT_INDEX",
    33: "MAYOR_TEXT_INDEX", 
    34: "GREG_TEXT_INDEX", 
    35: "RICK_TEXT_INDEX", 
    36: "BARLEY_TEXT_INDEX",
    37: "SYDNEY_TEXT_INDEX", 
    38: "POTION_SHOP_DEALER_TEXT_INDEX", 
    39: "MAYOR_WIFE_TEXT_INDEX",
    40: "ELLEN_TEXT_INDEX", 
    41: "STU_TEXT_INDEX", 
    42: "MIDWIFE_TEXT_INDEX", 
    43: "PASTOR_TEXT_INDEX",
    44: "SAIBARA_TEXT_INDEX", 
    45: "CUTSCENES_1_TEXT_INDEX", 
    46: "FUNERAL_INTRO_TEXT_INDEX",
    47: "DIALOGUE_47_TEXT_INDEX", 
    48: "DIALOGUE_48_TEXT_INDEX", 
    49: "DIALOGUE_49_TEXT_INDEX",
    50: "DIALOGUE_50_TEXT_INDEX", 
    51: "DIALOGUE_51_TEXT_INDEX",
    52: "DOG_RACE_TEXT_INDEX",
    53: "DIALOGUE_53_TEXT_INDEX", 
    54: "DIALOGUE_54_TEXT_INDEX", 
    55: "DIALOGUE_55_TEXT_INDEX",
    56: "NEW_YEAR_FESTIVAL_TEXT_INDEX", 
    57: "SPIRIT_FESTIVAL_TEXT_INDEX", 
    58: "HORSE_RACE_TEXT_INDEX",
    59: "MARRIED_DIALOGUES_TEXT_INDEX", 
    60: "DIALOGUE_60_TEXT_INDEX", 
    61: "KAREN_CUTSCENES_1_TEXT_INDEX",
    62: "DIALOGUE_62_TEXT_INDEX", 
    63: "DIALOGUE_63_TEXT_INDEX", 
    64: "DIALOGUE_64_TEXT_INDEX",
    65: "EGG_FESTIVAL_TEXT_INDEX", 
    66: "KAREN_CUTSCENES_2_TEXT_INDEX", 
    67: "VEGETABLE_FESTIVAL_TEXT_INDEX",
    68: "BABY_TEXT_INDEX", 
    69: "MRS_MANA_AND_JOHN_TEXT_INDEX", 
    70: "ADDITIONAL_NPCS_TEXT_INDEX",
    71: "NPC_BABY_TEXT_INDEX", 
    72: "HOW_TO_PLAY_TEXT_INDEX"
}

# animation data with no references/jumps
# since it's only a few cases, it's easiest just to hardcode these to force the linear scanner 
# to parse these bytes as animation data instead of as commands
ORPHANED_ANIM_OFFSETS = {
    "bank21": [0x144E38],
    "bank25": [0x150114],
    "bank27": [0x159230, 0x15923C],
}

NPC_CONSTANTS = {
    0: "MARIA",
    1: "POPURI",
    2: "ELLI",
    3: "ANN",
    4: "KAREN",
    5: "BABY",
    6: "HARRIS",
    7: "GRAY",
    8: "JEFF",
    9: "CLIFF",
    0xA: "KAI",
    0xB: "MAYOR",
    0xC: "MAYOR_WIFE",
    0xD: "LILLIA",
    0xE: "BASIL",
    0xF: "ELLEN",
    0x10: "DOUG",
    0x11: "GOTZ",
    0x12: "SASHA",
    0x13: "POTION_SHOP_DEALER",
    0x14: "KENT",
    0x15: "STU",
    0x16: "MIDWIFE",
    0x17: "MAY",
    0x18: "RICK",
    0x19: "PASTOR",
    0x1A: "SHIPPER",
    0x1B: "SAIBARA",
    0x1C: "DUKE",
    0x1D: "GREG",
    0x1E: "CARPENTER_1",
    0x1F: "CARPENTER_2",
    0x20: "MASTER_CARPENTER",
    0x21: "HARVEST_SPRITE_1",
    0x22: "HARVEST_SPRITE_2",
    0x23: "HARVEST_SPRITE_3",
    0x24: "SYDNEY",
    0x25: "BARLEY",
    0x26: "GOURMET_JUDGE",
    0x27: "PHOTOGRAPHER",
    0x29: "MARIA_HARRIS_BABY",
    0x2A: "POPURI_GRAY_BABY",
    0x2B: "ELLI_JEFF_BABY",
    0x2C: "ANN_CLIFF_BABY",
    0x2D: "KAREN_KAI_BABY",
}

VARIABLE_ADDRESSES = {
    0x8017027F: "gMinutes",
    0x80182FB8: "gHappiness",
    0x80189060: "gPlayer", # really gPlayer.currentFatigue
    0x801891D4: "gCutsceneCompletionFlags",
    0x80189826: "cutsceneLevelInteractionIndex",
    0x80189E50: "gLumber",
    0x801C3B66: "gCutsceneIndex",
    0x801C3E2C: "interactedWithLevelInteractionIndex",
    0x801C3F88: "gTotalMilkShipped",
    0x801FB5CA: "gHour",
    0x801FC150: "spiritFestivalAssistant2",
    0x801FD60C: "gGold",
    0x801FD621: "spiritFestivalAssistant1",
    0x8020563F: "gMaximumStamina",
    0x80215EB8: "powerNutBits",
    0x802226E0: "playerIdleCounter",
    0x802373A9: "gWeather",
    0x802373C8: "gItemBeingHeld",
    0x802373E9: "gWife",
    0x80237412: "spiritFestivalAssistant3",
    0x80158260: "gDayOfMonth",
    0x801654F4: "D_801654F4",
    0x8016F8BC: "specialDialogueBits + 0xC",
    0x8016FDD0: "horseInfo",
    0x8016FE00: "gCutsceneFlags",
    0x8017026E: "D_8017026E",
    0x801806C0: "D_801806C0",
    0x80180710: "D_80180710",
    0x80180714: "fodderQuantity",
    0x80182DB1: "gSeason",
    0x801886A8: "D_801886A8",
    0x801886B0: "dogInfo",
    0x801886D0: "D_801886D0",
    0x801886D2: "D_801886D2",
    0x801886E0: "D_801886E0",
    0x80188F60: "D_80188F60",
    0x80188F68: "D_80188F68",
    0x80189134: "gBabyAge",
    0x80189824: "D_80189824",
    0x80189E52: "gAlcoholTolerance",
    0x8018A72C: "D_8018A72C",
    0x8018A72D: "D_8018A72C + 0x1",
    0x8018A72E: "D_8018A72C + 0x2",
    0x8018A72F: "D_8018A72C + 0x3",
    0x8018A730: "D_8018A72C + 0x4",
    0x8018A731: "D_8018A72C + 0x5",
    0x801C3B60: "D_801C3B60",  # needs declaration
    0x801C3B62: "D_801C3B62",
    0x801C3DA0: "overlayScreenStrings",
    0x801C3DA2: "overlayScreenStrings + 0x2",
    0x801C3DA4: "overlayScreenStrings + 0x4",
    0x801C3DA6: "overlayScreenStrings + 0x6",
    0x801C3DA8: "overlayScreenStrings + 0x8",
    0x801C3DAA: "overlayScreenStrings + 0xA",
    0x801C3E1C: "D_801C3E1C",
    0x801C3F44: "gFlowerFestivalGoddess",
    0x801C3F4C: "D_801C3F4C",
    0x801C3F78: "D_801C3F78",
    0x801C3F80: "D_801C3F80",
    0x801D62C4: "D_801D62C4",
    0x801D62C6: "D_801D62C6",
    0x801F6F30: "gYear",
    0x801FB5D0: "D_801FB5D0",
    0x801FB68C: "mountainConstructionWorkDays",
    0x801FB6FC: "D_801FB6FC",
    0x80204FB0: "gSumGirlsWithHighAffection",
    0x80204FF8: "gHarvestKing",
    0x80205209: "D_80205209",
    0x80205638: "D_80205638",
    0x8020563A: "gAverageFarmAnimalAffection",
    0x80215DF0: "D_80215DF0",
    0x80215EC0: "gElliGrievingCounter",
    0x8021E6D0: "D_8021E6D0",
    0x80237409: "gHarvestCoinFinder",
    0x80237414: "D_80237414",
    0x80237A00: "D_80237A00",

}

# Array variables: (base_address, element_size, element_count, var_name, index_constants)
ARRAY_VARIABLES = [
    (0x8013D1C8, 4, 0x80, "lifeEventBits", None),     
    (0x80189140, 4, 0x80, "dailyEventBits", None),    
    (0x801C3F90, 1, 0x30, "npcAffection", NPC_CONSTANTS), 
]


def format_variable_address(addr: int, use_labels: bool, asm_mode: bool = False) -> str:
    """Formats a variable address, optionally using variable names and array indexing"""

    if not use_labels:
        return f"0x{addr:X}"

    # Check if it's a simple variable
    if addr in VARIABLE_ADDRESSES:
        return VARIABLE_ADDRESSES[addr]

    # Check if it's part of an array
    for base_addr, elem_size, elem_count, var_name, index_constants in ARRAY_VARIABLES:
        end_addr = base_addr + (elem_size * elem_count)
        if base_addr <= addr < end_addr:
            offset = addr - base_addr

            if asm_mode:
                # ASM mode: use 'var + offset' syntax (valid in assembly)
                if offset == 0:
                    return var_name
                else:
                    return f"{var_name} + 0x{offset:X}"
            else:
                # Text mode: use 'var[index]' syntax (readable)
                if offset % elem_size == 0:  # Valid array access
                    index = offset // elem_size
                    if index_constants and index in index_constants:
                        return f"{var_name}[{index_constants[index]}]"
                    else:
                        return f"{var_name}[{index}]"
                else:
                    # Unaligned access - show as offset
                    return f"{var_name} + 0x{offset:X}"


    return f"0x{addr:X}"


def format_rom_address(addr: int, use_labels: bool, is_end_symbol: bool = False) -> str:
    """Formats a ROM address, optionally using segment symbols.

    Args:
        addr: The ROM address to format
        use_labels: Whether to use symbolic names
        is_end_symbol: If True, look up in ROM_SEGMENT_END_SYMBOLS instead of START
    """

    if not use_labels:
        return f"0x{addr:X}"

    # Choose the appropriate dictionary based on whether this is an end symbol
    symbols_dict = ROM_SEGMENT_END_SYMBOLS if is_end_symbol else ROM_SEGMENT_START_SYMBOLS

    # Check if it's a known ROM segment symbol
    if addr in symbols_dict:
        return symbols_dict[addr]


    return f"0x{addr:X}"

def format_map_name(idx: int, use_labels: bool) -> str:
    """Formats a map index using constants"""
    if use_labels and idx in MAP_CONSTANTS:
        return MAP_CONSTANTS[idx]
    return str(idx)

def format_direction(direction: int, use_labels: bool) -> str:
    """Formats a direction using constants"""
    if use_labels and direction in DIRECTION_CONSTANTS:
        return DIRECTION_CONSTANTS[direction]
    return str(direction)

def format_button_mask(mask: int) -> str:
    """Converts an integer mask to a string of OR'd constants"""
    
    if mask == 0:
        return "0"

    parts = []
    
    for flag, name in BUTTON_CONSTANTS.items():
        if (mask & flag) == flag:
            parts.append(name)
    
    if not parts:
        return f"0x{mask:X}"
    
    return " | ".join(parts)

def format_entity_index(idx: int, use_labels: bool) -> str:
    """Formats an entity index, optionally using constants"""
    
    if idx == 0xFFFF:
        return "0xFFFF"

    if use_labels and idx in ENTITY_CONSTANTS:
        return ENTITY_CONSTANTS[idx]
    
    return str(idx)

def format_entity_asset_index(idx: int, use_labels: bool) -> str:
    """Formats an entity index, optionally using constants"""
    
    if idx == 0xFFFF:
        return "0xFFFF"
    
    if use_labels and idx in ENTITY_ASSET_CONSTANTS:
        return ENTITY_ASSET_CONSTANTS[idx]
    
    return str(idx)

def format_text_bank_index(idx: int, use_labels: bool) -> str:
    """Formats a text bank index using constants"""

    if use_labels and idx in TEXT_BANK_CONSTANTS:
        return TEXT_BANK_CONSTANTS[idx]
        
    return str(idx)

def format_boolean(flag: int) -> str:
    if flag == 0:
        return "FALSE"
    elif flag == 1:
        return "TRUE"
    else:
        return f"{flag}"
        
def format_audio_address(addr: int, use_labels: bool) -> str:
    """Formats an audio sequence address, optionally using sequence name with suffix

    Args:
        addr: The address to format
        use_labels: Whether to use labels or hex
    """

    if not use_labels:
        return f"0x{addr:X}"

    # Check if this address is a known sequence start
    if addr in AUDIO_SEQUENCES:
        name = AUDIO_SEQUENCES[addr]
        return f"{name}"

    # Not a known audio address
    return f"0x{addr:X}"

# =============================================================================
# DATA STRUCTURES
# =============================================================================

@dataclass
class ParsedCommand:
    """Represents a parsed cutscene command"""

    offset: int
    opcode: int
    size: int
    name: str
    params: List[Tuple[str, any]]
    raw_bytes: bytes
    # For graph traversal
    spawn_target: Optional[Tuple[int, int]] = None  # (executor_index, absolute_offset)
    branch_target: Optional[int] = None  # absolute offset of branch target
    jump_target: Optional[int] = None  # absolute offset of subroutine target
    anim_target: Optional[int] = None # absolute offset of animation data
    extra_info: Optional[str] = None  # additional notes for comment
    branch_terminates: bool = False  # True if this subroutine call targets an infinite loop
    
    def to_macro(self) -> str:
        """Convert to C macro format"""

        param_strs = []
        
        for pname, pvalue in self.params:
            if isinstance(pvalue, int):
                if abs(pvalue) > 255:
                    param_strs.append(f"0x{pvalue & 0xFFFFFFFF:X}")
                else:
                    param_strs.append(str(pvalue))
            else:
                param_strs.append(str(pvalue))
        
        params_str = ", ".join(param_strs) if param_strs else ""
        return f"{self.name}({params_str})"


@dataclass
class ExecutorBlock:
    """Represents a block of commands for one executor"""

    executor_index: int
    start_offset: int
    commands: List[ParsedCommand] = field(default_factory=list)
    spawned_by: Optional[int] = None  # Offset of command that spawned this
    is_branch: bool = False 
    label: str = ""

@dataclass
class AnimationEntry:
    """Represents one frame or control code in an animation sequence"""
    
    offset: int
    raw_bytes: bytes
    is_control: bool = False
    is_terminator: bool = False
    
    # Standard entries
    anim_id: Optional[int] = None
    flag: Optional[int] = None
    vec: Optional[Tuple[int, int, int]] = None
    wait_frames: Optional[int] = None
    flip_flags: Optional[int] = None
    
    # For control entries (0xFFFE loop)
    loop_target: Optional[int] = None

    def to_string(self) -> str:
        if self.is_terminator:
            return "ANIM_END()"
        if self.is_control:
            return f"ANIM_LOOP(target=0x{self.loop_target:X})"
        return (f"ANIM_FRAME(id=0x{self.anim_id:04X}, flag=0x{self.flag:02X}, "
                f"vec=({self.vec[0]}, {self.vec[1]}, {self.vec[2]}), "
                f"wait={self.wait_frames}, flip=0x{self.flip_flags:X})")

@dataclass
class AnimationBlock:
    """Represents a sequence of animation data"""
    start_offset: int
    entries: List[AnimationEntry] = field(default_factory=list)

@dataclass
class EntryPointInfo:
    """Information about a single entry point"""
    offset: int
    array_index: int
    executor_index: int
    reachable_blocks: Set[int] = field(default_factory=set)  # Block offsets reachable from this entry

@dataclass
class CutsceneGraph:
    """Unified graph representation for single or multi-entry cutscenes"""

    # Metadata
    bank_start: int
    bank_end: int
    bank_name: str = "Unknown"
    
    # Storage
    entry_points: List[EntryPointInfo] = field(default_factory=list)
    blocks: Dict[int, ExecutorBlock] = field(default_factory=dict)
    anim_blocks: Dict[int, AnimationBlock] = field(default_factory=dict)
    execution_order: List[int] = field(default_factory=list) # Keeps linear order for simple single-entry cases
    
    # Analysis
    block_reachability: Dict[int, Set[int]] = field(default_factory=dict) # block_offset -> set of entry point indices

    def get_shared_blocks(self) -> List[int]:
        """Return offsets of blocks reachable from multiple entry points"""
        return sorted([o for o, entries in self.block_reachability.items() if len(entries) > 1])
    
    def get_unique_blocks(self, entry_idx: int) -> List[int]:
        """Return offsets reachable ONLY from this entry point"""
        return sorted([o for o, entries in self.block_reachability.items() if entries == {entry_idx}])

# =============================================================================
# PARSER
# =============================================================================

class CutsceneBytecodeParser:
    """Parser for HM64 cutscene bytecode with graph traversal"""
    
    def __init__(self, data: bytes, base_offset: int = 0, enable_labels: bool = False):
        self.data = data
        self.base_offset = base_offset
        self.enable_labels = enable_labels
        self.pos = 0
        # Cache for subroutine return status (True = can return, False = infinite loop/terminal)
        self.subroutine_return_cache: Dict[int, bool] = {}
        
    def seek(self, offset: int):
        """Seek to absolute offset (relative to base_offset)"""
        self.pos = offset - self.base_offset
        
    def tell(self) -> int:
        """Get current absolute offset"""
        return self.base_offset + self.pos
        
    def read_u8(self) -> int:
        val = self.data[self.pos]
        self.pos += 1
        return val
    
    def read_s8(self) -> int:
        val = struct.unpack_from('b', self.data, self.pos)[0]
        self.pos += 1
        return val
    
    def read_u16(self) -> int:
        val = struct.unpack_from('>H', self.data, self.pos)[0]
        self.pos += 2
        return val
    
    def read_s16(self) -> int:
        val = struct.unpack_from('>h', self.data, self.pos)[0]
        self.pos += 2
        return val
    
    def read_u32(self) -> int:
        val = struct.unpack_from('>I', self.data, self.pos)[0]
        self.pos += 4
        return val
    
    def read_s32(self) -> int:
        val = struct.unpack_from('>i', self.data, self.pos)[0]
        self.pos += 4
        return val

    def _try_parse_padding_or_data(self) -> Optional[ParsedCommand]:
        """
        Checks for specific padding patterns or invalid opcodes that define data.
        Returns a ParsedCommand if padding/data is found and advances pos.
        Returns None if the next bytes look like a standard command.
        """

        if self.pos + 2 > len(self.data):
            return None

        try:
            peek_opcode = struct.unpack_from('>H', self.data, self.pos)[0]
        except struct.error:
            return None

        cmd_offset = self.base_offset + self.pos
        
        # Check for 4-byte padding patterns
        if self.pos + 4 <= len(self.data):

            try:
                next_word = struct.unpack_from('>H', self.data, self.pos + 2)[0]
                
                # Manually parse 8 consecutive 0s as padding for now
                # Also handle 00 00 03 E7, which occurs in two banks before first entry points (and doesn't occur as a valid command anywhere else)
                if peek_opcode == 0x0000 and (next_word == 0x0000 or next_word == 0x03E7):

                    raw_bytes = self.data[self.pos:self.pos+4]
                    self.pos += 4
                    
                    return ParsedCommand(
                        offset=cmd_offset, opcode=peek_opcode, size=4, 
                        name="PADDING", params=[], raw_bytes=raw_bytes
                    )

                # Manually check for animation data signatures
                # Check for FFFF 0000 or FE00 0000
                if (peek_opcode == 0xFFFF or peek_opcode == 0xFE00 or peek_opcode == 0xFF00) and next_word == 0x0000:

                    raw_bytes = self.data[self.pos:self.pos+4]
                    self.pos += 4
                    
                    return ParsedCommand(
                        offset=cmd_offset, opcode=peek_opcode, size=4, 
                        name=f"PADDING_{peek_opcode:04X}0000", params=[], raw_bytes=raw_bytes
                    )

            except struct.error:
                pass
        
        if peek_opcode < 98:
            return None

        # Fallback/treat as 2-byte data
        raw_bytes = self.data[self.pos:self.pos+2]
        self.pos += 2

        return ParsedCommand(
            offset=cmd_offset, opcode=peek_opcode, size=2, 
            name=f"DATA_{peek_opcode:04X}", params=[], raw_bytes=raw_bytes
        )

    def parse_command(self) -> Optional[ParsedCommand]:
        """Parse a single command at current position"""

        if self.pos >= len(self.data) - 1:
            return None

        def fmt_offset(val):
            return f"{val}"
            
        start_pos = self.pos
        cmd_offset = self.base_offset + start_pos
        
        # Initialize defaults
        opcode = 0
        name = "UNKNOWN"
        params = []
        spawn_target = None
        branch_target = None
        jump_target = None
        anim_target = None
        extra_info = None
        
        try:

            opcode = self.read_u16()
            
            if opcode > 97:
                return None
            
            name = OPCODE_NAMES.get(opcode, f"CMD_UNKNOWN_{opcode}")
            
            if opcode == 0:  # SET_ANIM_DATA_PTR_WITH_FLAG
                offset = self.read_s16()
                target = self.tell() + offset - 2
                params = [("offset", fmt_offset(offset))] 
                anim_target = target
                extra_info = "Sets Animation Data (With Flag)"

            elif opcode == 1:  # SET_ANIM_DATA_PTR
                offset = self.read_s16()
                target = self.tell() + offset - 2
                params = [("offset", fmt_offset(offset))]
                anim_target = target
                extra_info = "Sets Animation Data"

            elif opcode == 2:  # SET_COORDINATES
                x = self.read_s16()
                y = self.read_s16()
                z = self.read_s16()
                params = [("x", f"{x}"), ("y", f"{y}"), ("z", f"{z}")]

            elif opcode == 3:  # SET_FRAME_DELTA
                dx = self.read_s8()
                dy = self.read_s8()
                dz = self.read_s8()
                frames = self.read_u8()
                self.pos += 2 # Padding
                params = [("dx", dx), ("dy", dy), ("dz", dz), ("frames", frames)]

            elif opcode == 4:  # SET_WAIT_FRAMES
                frames = self.read_u16()
                params = [("frames", frames)]

            elif opcode == 5:  # DEACTIVATE_SELF
                # C code doesn't explicitly increment, but data includes alignment padding
                self.pos += 2 

            elif opcode == 6:  # EXECUTE_SUBROUTINE
                offset = self.read_s16()
                target = self.tell() + offset - 2
                params = [("offset", fmt_offset(offset))] 
                # Set branch_target for graph follower; add extra info for printer to track this is a call
                branch_target = target
                extra_info = "Call Subroutine"
            
            elif opcode == 7:  # RETURN_FROM_SUBROUTINE
                # Ends execution flow
                self.pos += 2 

            elif opcode in (8, 9, 10):  # BRANCH_ON_BUTTON types
                controller = self.read_u16()
                mask = self.read_u32()
                offset = self.read_s16()
                self.pos += 2 
                target = self.tell() - 4 + offset
                mask_str = format_button_mask(mask)
                params = [("controller", controller), ("mask", mask_str), ("offset", offset)]
                branch_target = target

            elif opcode == 11:  # SPAWN_EXECUTOR
                index = self.read_u16()
                offset = self.read_s16()
                self.pos += 2 # Padding
                target = self.tell() - 4 + offset 
                params = [("executor_index", index), ("offset", f"{offset}")]
                spawn_target = (index, target)

            elif opcode == 12:  # SET_OTHER_EXECUTOR_PTR
                index = self.read_u16()
                offset = self.read_s16()
                self.pos += 2 # Padding
                # Target is calculated from offset field position (+4 from command start)
                # After reading offset, we're at +6, so offset field was at +4 = tell() - 2
                target = self.tell() - 4 + offset  # -4 because we also skipped padding
                params = [("executor_index", index), ("offset", f"{offset}")]
                spawn_target = (index, target)  # Include in graph traversal

                
            elif opcode == 13:  # DEACTIVATE_EXECUTOR
                index = self.read_u16()
                params = [("executor_index", index)]

            elif opcode == 14:  # DMA_SPRITE
                sprite_idx = self.read_u16()
                asset_type = self.read_u16()
                self.pos += 2 # Padding
                
                rom_tex_start = self.read_u32()
                rom_tex_end = self.read_u32()
                rom_assets_start = self.read_u32()
                rom_assets_end = self.read_u32()
                rom_sheet_start = self.read_u32()
                rom_sheet_end = self.read_u32()
                
                tex1_vaddr = self.read_u32()
                tex2_vaddr = self.read_u32()
                palette_vaddr = self.read_u32()
                anim_vaddr = self.read_u32()
                s2p_vaddr = self.read_u32()
                sheet_idx_vaddr = self.read_u32()
                
                params = [
                    ("sprite", sprite_idx),
                    ("type", asset_type),
                    ("rom_tex_start", format_rom_address(rom_tex_start, self.enable_labels, is_end_symbol=False)),
                    ("rom_tex_end", format_rom_address(rom_tex_end, self.enable_labels, is_end_symbol=True)),
                    ("rom_assets_start", format_rom_address(rom_assets_start, self.enable_labels, is_end_symbol=False)),
                    ("rom_assets_end", format_rom_address(rom_assets_end, self.enable_labels, is_end_symbol=True)),
                    ("rom_sheet_start", format_rom_address(rom_sheet_start, self.enable_labels, is_end_symbol=False)),
                    ("rom_sheet_end", format_rom_address(rom_sheet_end, self.enable_labels, is_end_symbol=True)),
                    ("tex1", format_variable_address(tex1_vaddr, self.enable_labels)),
                    ("tex2", format_variable_address(tex2_vaddr, self.enable_labels)),
                    ("pal", format_variable_address(palette_vaddr, self.enable_labels)),
                    ("anim", format_variable_address(anim_vaddr, self.enable_labels)),
                    ("s2p", format_variable_address(s2p_vaddr, self.enable_labels)),
                    ("sheet_idx", format_variable_address(sheet_idx_vaddr, self.enable_labels))
                ]

            elif opcode == 15:  # SET_ENTITY_ANIMATIONS
                idx = self.read_u16()
                anims = [self.read_u16() for _ in range(6)]
                idx_str = format_entity_index(idx, self.enable_labels)
                params = [("entity", idx_str)] + [(f"anim{i+1}", a) for i, a in enumerate(anims)]

            elif opcode == 16:  # DO_DMA
                self.pos += 2 # Padding
                rom_start = self.read_u32()
                rom_end = self.read_u32()
                vaddr = self.read_u32()
                params = [
                    ("rom_start", format_rom_address(rom_start, self.enable_labels, is_end_symbol=False)),
                    ("rom_end", format_rom_address(rom_end, self.enable_labels, is_end_symbol=True)),
                    ("vaddr", format_variable_address(vaddr, self.enable_labels))
                ]

            elif opcode == 17:  # SET_U8_VALUE
                value = self.read_u8()
                self.pos += 1 # Padding
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("value", value), ("variable_ptr", ptr_str)]

            elif opcode == 18:  # SET_U16_VALUE
                val = self.read_u16()
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("value", val), ("ptr", ptr_str)]

            elif opcode == 19:  # SET_U32_VALUE
                self.pos += 2 # Padding
                val = self.read_u32()
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("value", val), ("ptr", ptr_str)]

            elif opcode == 20:  # BRANCH_IF_U8_WITHIN_RANGE
                idx1 = self.read_u8()
                idx2 = self.read_u8()
                ptr = self.read_u32()
                offset = self.read_s16()
                self.pos += 2 # Padding
                target = self.tell() - 4 + offset
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("idx1", idx1), ("idx2", idx2), ("ptr", ptr_str), ("offset", f"{offset}")]
                branch_target = target

            elif opcode == 21:  # BRANCH_IF_U16_WITHIN_RANGE
                idx1 = self.read_u16()
                idx2 = self.read_u16()
                self.pos += 2 # Padding
                ptr = self.read_u32()
                offset = self.read_s16()
                self.pos += 2 # Padding
                target = self.tell() - 4 + offset
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("idx1", idx1), ("idx2", idx2), ("ptr", ptr_str), ("offset", f"{offset}")]
                branch_target = target

            elif opcode == 22:  # BRANCH_IF_U32_WITHIN_RANGE
                self.pos += 2 # Padding
                val1 = self.read_u32()
                val2 = self.read_u32()
                ptr = self.read_u32()
                offset = self.read_s16()
                self.pos += 2 # Padding
                target = self.tell() - 4 + offset
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("min", val1), ("max", val2), ("ptr", ptr_str), ("offset", f"{offset}")]
                branch_target = target

            elif opcode == 23:  # SET_SPECIAL_BIT
                bit = self.read_u16()
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("bit", bit), ("ptr", ptr_str)]

            elif opcode == 24:  # TOGGLE_SPECIAL_BIT
                bit = self.read_u16()
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("bit", bit), ("ptr", ptr_str)]

            elif opcode == 25:  # BRANCH_ON_SPECIAL_BIT
                bit = self.read_u16()
                ptr = self.read_u32()
                offset = self.read_s16()
                self.pos += 2 # Padding
                target = self.tell() - 4 + offset
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("bit", bit), ("ptr", ptr_str), ("offset", f"{offset}")]
                branch_target = target

            elif opcode == 26:  # SET_ASSET_ROTATION
                val = self.read_u8() 
                self.pos += 1
                val_str = format_direction(val, self.enable_labels)
                params = [("direction", val_str)]

            elif opcode == 27: # SETUP_MAP_ASSET
                index = self.read_u16() # main map index, not level index
                params = [("index", index)]

            elif opcode in (28, 36): # ENTITY_MOVE
                frames = self.read_u16()
                direction = self.read_u8()
                distance = self.read_u8()
                self.pos += 2 # Padding
                dir_str = format_direction(direction, self.enable_labels)
                params = [("frames", frames), ("dir", dir_str), ("dist", distance)]

            elif opcode == 29: # SET_MAP_ROTATION_WITH_FLAG
                idx = self.read_u16()
                flag = self.read_u8() 
                rot = self.read_u8()
                self.pos += 2 # Padding
                rot_str = format_direction(rot, self.enable_labels)
                params = [("map_idx", idx), ("flag", f"0x{flag:X}"), ("rot", rot_str)]

            elif opcode == 30: # SET_CAMERA_TRACKING_FLAG
                flag = self.read_u8()
                self.pos += 1
                flag_str = format_boolean(flag)
                params = [("flag", flag_str)]

            elif opcode == 31: # SET_MOVEMENT_INFO
                dist = self.read_u8()
                self.pos += 1 # Padding
                col_w = self.read_u16()
                col_h = self.read_u16()
                params = [("dist", dist), ("w", col_w), ("h", col_h)]

            elif opcode in (32, 77):  # INIT_MESSAGE_BOX_TYPE1/2
                msg_box_idx = self.read_u16()
                text_addr_idx = self.read_u16()
                text_idx = self.read_u16()
                text_addr_str = format_text_bank_index(text_addr_idx, self.enable_labels)
                params = [
                    ("message_box", msg_box_idx), 
                    ("text_addresses", text_addr_str), 
                    ("text_index", f"{text_idx}")
                ]

            elif opcode in (33, 35, 63): # WAIT_MESSAGE/DIALOGUE
                idx = self.read_u16()
                params = [("index", idx)]

            elif opcode == 34: # SET_MESSAGE_BOX_VIEW_POS
                idx = self.read_u16()
                x = self.read_s16()
                y = self.read_s16()
                z = self.read_s16()
                self.pos += 2 # Padding
                params = [("box", idx), ("x", f"{x}"), ("y", f"{y}"), ("z", f"{z}")]

            elif opcode in (37, 38): # SET_ENTITY_ANIMATION
                anim = self.read_u16()
                params = [("anim", anim)]

            elif opcode == 39: # SET_CALLBACK_ROUTINE
                idx = self.read_u16()
                mask = self.read_u16()
                offset = self.read_s16()
                # 8 bytes, offset relative to offset field (current_pos - 2)
                target = self.tell() - 2 + offset
                mask_str = format_button_mask(mask)
                idx_str = format_entity_index(idx, self.enable_labels)
                params = [("idx", idx_str), ("mask", mask_str), ("offset", fmt_offset(offset))]
                branch_target = target
                extra_info = "Callback"

            elif opcode in (40, 41): # PAUSE/TOGGLE PAUSE ENTITY
                val = self.read_u16()
                val_str = format_entity_index(val, self.enable_labels)
                params = [("entity", val_str)]

            elif opcode == 42:  # FLIP_ENTITY_DIRECTION
                val = self.read_u16()
                val_str = format_entity_index(val, self.enable_labels)
                params = [("entity", val_str)]

            elif opcode in (43, 44): # PAUSE/UNPAUSE ENTITIES
                 self.pos += 2

            elif opcode == 45: # FLIP_ENTITY_ANIMATION
                self.pos += 2

            elif opcode == 46: # SET_ENTITY_NON_COLLIDABLE
                self.pos += 2

            elif opcode == 47: # SETUP_ENTITY
                idx = self.read_u16()
                asset = self.read_u16()
                flag = self.read_u16()
                idx_str = format_entity_index(idx, self.enable_labels)
                asset_str = format_entity_asset_index(asset, self.enable_labels)
                params = [("index", idx_str), ("asset", asset_str), ("flag", flag)]

            elif opcode == 48: # SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG
                val = self.read_u16()
                flag = format_boolean(val)
                params = [("flag", flag)]

            elif opcode == 49: # LOAD_MAP
                data_idx = self.read_u16()
                map_idx = self.read_u16()
                self.pos += 2 # Padding
                map_str = format_map_name(map_idx, self.enable_labels)
                params = [("data_idx", data_idx), ("map_idx", map_str)]

            elif opcode == 50: # SET_ENTITY_MAP_SPACE_INDEPENDENT
                self.pos += 2 # padding

            elif opcode == 51: # SET_RGBA
                r = self.read_u8()
                g = self.read_u8()
                b = self.read_u8()
                a = self.read_u8()
                self.pos += 2 # Padding
                params = [("r", r), ("g", g), ("b", b), ("a", a)]

            elif opcode in (52, 57): # UPDATE_RGBA / UPDATE_GLOBAL_RGBA
                r = self.read_u8()
                g = self.read_u8()
                b = self.read_u8()
                a = self.read_u8()
                rate = self.read_s16()
                params = [("r", r), ("g", g), ("b", b), ("a", a), ("rate", rate)]

            elif opcode == 53: # UPDATE_U8
                self.pos += 1
                val = self.read_s8()
                self.pos += 1
                max_val = self.read_u8()
                self.pos += 2 # padding/unused
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("val", val), ("max", f"{max_val}"), ("ptr", ptr_str)]

            elif opcode == 54: # UPDATE_U16
                val = self.read_s16()
                max_val = self.read_u16()
                self.pos += 2 # Padding
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("val", val), ("max", f"{max_val}"), ("ptr", ptr_str)]

            elif opcode == 55: # UPDATE_U32
                self.pos += 2 # Padding
                val = self.read_s32()
                max_val = self.read_s32()
                ptr = self.read_u32()
                ptr_str = format_variable_address(ptr, self.enable_labels)
                params = [("val", val), ("max", f"{max_val}"), ("ptr", ptr_str)]

            elif opcode == 56: # DEACTIVATE_MAP_OBJECTS
                val = self.read_u16()
                params = [("map_idx", val)]

            elif opcode in (58, 59, 72, 81, 82):
                val = self.read_u16()
                params = [("val", val)]

            elif opcode == 60: # WAIT_RGBA_FINISHED
                self.pos += 2 

            elif opcode == 61: # CHECK_ENTITY_COLLISION
                idx = self.read_u16()
                w = self.read_u16()
                h = self.read_u16()
                offset = self.read_s16()
                target = self.tell() - 2 + offset
                idx_str = format_entity_index(idx, self.enable_labels)
                params = [("index", idx_str), ("w", w), ("h", h), ("offset", f"{offset}")]
                branch_target = target

            elif opcode == 62:  # INIT_DIALOGUE_SESSION
                dialogue_idx = self.read_u16()
                bytecode_addr_idx = self.read_u16()
                dialogue_num = self.read_u16()
                params = [
                    ("dialogue_index", dialogue_idx), 
                    ("bytecode_addr", f"{bytecode_addr_idx}"), 
                    ("dialogue_num", f"{dialogue_num}")
                ]

            elif opcode == 64: # BRANCH_ON_DIALOGUE_UNK
                idx = self.read_u16()
                flag = self.read_u16()
                self.pos += 2 # Padding before offset
                offset = self.read_s16()
                self.pos += 2 # Padding after offset
                target = self.tell() - 4 + offset
                flag_str = format_boolean(flag)
                params = [("idx", idx), ("flag", flag_str), ("offset", f"{offset}")]
                branch_target = target

            elif opcode == 65: # WAIT_ENTITY_ANIMATION
                self.pos += 2

            elif opcode == 66: # SET_MESSAGE_BOX_ASSETS
                sprite = self.read_u16()
                dlg_win = self.read_u8()
                icon = self.read_u8()
                avatar = self.read_u8()
                self.pos += 1 
                params = [("sprite", sprite), ("win", dlg_win), ("icon", icon), ("avatar", avatar)]

            elif opcode == 67: # SET_ENTITY_TRACKING_TARGET
                target_entity_idx = self.read_u16()
                x = self.read_s16()
                y = self.read_s16()
                z = self.read_s16()
                tracking_mode = self.read_u8()
                self.pos += 1 # Padding
                target_idx_str = format_entity_index(target_entity_idx, self.enable_labels)
                params = [
                    ("target_sprite", target_idx_str), 
                    ("x", f"{x}"), 
                    ("y", f"{y}"), 
                    ("z", f"{z}"), 
                    ("tracking_mode", f"0x{tracking_mode:X}")
                ]

            elif opcode == 68: # UPDATE_CAMERA_FLAGS
                flag = self.read_u16()
                flag_str = format_boolean(flag)
                params = [("flag", flag_str)]

            elif opcode == 69: # WAIT_MAP_CONTROLLER
                 idx = self.read_u16()
                 params = [("index", idx)]

            elif opcode == 70: # BRANCH_ON_ENTITY_DIR
                 idx = self.read_u16()
                 direction = self.read_u8()
                 self.pos += 1
                 offset = self.read_s16()
                 target = self.tell() - 2 + offset
                 idx_str = format_entity_index(idx, self.enable_labels)
                 dir_str = format_direction(direction, self.enable_labels)
                 params = [("idx", idx_str), ("dir", dir_str), ("offset", f"{offset}")]
                 branch_target = target

            elif opcode == 71: # SET_ENTITY_PHYSICS_FLAGS
                flags = self.read_u16()
                params = [("flags", flags)]

            elif opcode == 73: # SET_ENTITY_DIMENSIONS
                 w = self.read_u8()
                 h = self.read_u8()
                 params = [("w", w), ("h", h)]

            elif opcode == 74: # SET_SHADOW_FLAGS
                 asset = self.read_u16()
                 flags = self.read_u16()
                 self.pos += 2 # Padding
                 params = [("asset", asset), ("flags", flags)]

            elif opcode == 75: # SET_SPRITE_SCALE
                 x = self.read_s16()
                 y = self.read_s16()
                 z = self.read_s16()
                 params = [("x", f"{x}"), ("y", f"{y}"), ("z", f"{z}")]

            elif opcode == 76: # SET_SPRITE_RENDERING_LAYER
                layer = self.read_u16()
                params = [("layer", layer)]

            elif opcode == 78: # INIT_MAP_ADDITION
                 idx = self.read_u16()
                 flag = self.read_u16()
                 self.pos += 2 # Padding
                 flag_str = format_boolean(flag)
                 params = [("idx", idx), ("flag", flag_str)]

            elif opcode == 79: # BRANCH_ON_RANDOM
                 unk2 = self.read_u16()
                 unk4 = self.read_u16()
                 offset = self.read_s16()
                 target = self.tell() - 2 + offset
                 params = [("unk2", unk2), ("unk4", unk4), ("offset", f"{offset}")]
                 branch_target = target

            elif opcode == 80: # BRANCH_IF_U16_PTR_IN_RANGE
                 self.pos += 2 # Padding
                 ptr1 = self.read_u32()
                 ptr2 = self.read_u32()
                 ptr3 = self.read_u32()
                 offset = self.read_s16()
                 self.pos += 2 # Padding
                 target = self.tell() - 4 + offset
                 params = [("ptr1", f"0x{ptr1:X}"), ("ptr2", f"0x{ptr2:X}"), ("val_ptr", f"0x{ptr3:X}"), ("offset", f"{offset}")]
                 branch_target = target

            elif opcode in (83, 84): # PAUSE/UNPAUSE_ALL_CHILDREN
                 self.pos += 2 # Padding

            elif opcode == 85: # SET_SPRITE_PALETTE
                palette_idx = self.read_u16()
                params = [("palette_index", palette_idx)]

            elif opcode == 86: # BRANCH_IF_U8_PTR_RANGE
                 self.pos += 2 # Padding
                 ptr1 = self.read_u32()
                 ptr2 = self.read_u32()
                 ptr3 = self.read_u32()
                 offset = self.read_s16()
                 self.pos += 2 # Padding
                 target = self.tell() - 4 + offset
                 params = [("ptr1", f"0x{ptr1:X}"), ("ptr2", f"0x{ptr2:X}"), ("val", f"0x{ptr3:X}"), ("offset", f"{offset}")]
                 branch_target = target

            elif opcode == 87: # SET_AUDIO_SEQUENCE
                idx = self.read_u16()
                start = self.read_u32()
                end = self.read_u32()
                params = [
                    ("idx", idx),
                    ("start", format_rom_address(start, self.enable_labels, is_end_symbol=False)),
                    ("end", format_rom_address(end, self.enable_labels, is_end_symbol=True))
                ]

            elif opcode == 88: # STOP_SONG
                idx = self.read_u16()
                speed = self.read_u16()
                self.pos += 2
                params = [("idx", idx), ("speed", speed)]

            elif opcode == 89: # SET_AUDIO_SEQUENCE_VOLUME
                idx = self.read_u16()
                vol_target = self.read_s16()
                vol_current = self.read_s16()
                params = [("idx", idx), ("target", vol_target), ("current", vol_current)]

            elif opcode == 90: # SET_SFX
                idx = self.read_u16()
                vol = self.read_u16()
                self.pos += 2 # Padding
                params = [("idx", idx), ("vol", vol)]

            elif opcode == 91: # IDLE_WHILE_SONG
                idx = self.read_u16()
                params = [("idx", idx)]

            elif opcode == 92: # UPDATE_MESSAGE_RGBA
                idx = self.read_u16()
                r = self.read_u8()
                g = self.read_u8()
                b = self.read_u8()
                a = self.read_u8()
                flags = self.read_u16()
                self.pos += 2 # Padding
                params = [("idx", idx), ("r", r), ("g", g), ("b", b), ("a", a), ("flags", flags)]

            elif opcode == 93: # WAIT_MESSAGE_BOX_READY
                idx = self.read_u16()
                self.pos += 2 # Logic check advances
                params = [("idx", idx)]

            elif opcode == 94: # SET_SPRITE_BILINEAR
                use_bilinear = self.read_u16()
                params = [("enable", use_bilinear)]

            elif opcode == 95: # SET_MAP_ADDITION
                idx = self.read_u16()
                x = self.read_u8()
                z = self.read_u8()
                self.pos += 2
                params = [("idx", idx), ("x", f"{x}"), ("z", f"{z}")]

            elif opcode == 96: # SET_MAP_GROUND_OBJECT
                sprite = self.read_u16()
                x = self.read_u8()
                z = self.read_u8()
                self.pos += 2
                params = [("sprite", sprite), ("x", f"{x}"), ("z", f"{z}")]

            elif opcode == 97: # SET_MESSAGE_INTERPOLATION
                rate = self.read_s16()
                idx = self.read_u8()
                flag = self.read_u8()
                self.pos += 2
                params = [("rate", rate), ("idx", idx), ("flag", flag)]

        except (struct.error, IndexError):
            return None
        
        size = self.pos - start_pos
        raw_bytes = self.data[start_pos:self.pos]
        
        return ParsedCommand(
            offset=cmd_offset,
            opcode=opcode,
            size=size,
            name=name,
            params=params,
            raw_bytes=raw_bytes,
            spawn_target=spawn_target,
            branch_target=branch_target,
            jump_target=jump_target,
            anim_target=anim_target,
            extra_info=extra_info,
            branch_terminates=False,
        )
    
    def parse_animation_sequence(self, start_offset: int) -> AnimationBlock:

        self.seek(start_offset)
        entries = []

        # FIXME: what is this? Why entities?
        MAX_ENTITIES = 500
        
        while self.pos < len(self.data) and len(entries) < MAX_ENTITIES:

            entry_offset = self.tell()

            if self.pos + 2 > len(self.data):
                break
            try:
                animation = self.read_u16()
            except struct.error:
                break
            
            if animation == 0xFFFF: # Terminator
                # Skip the 2-byte padding for 4-byte alignment
                if self.pos + 2 <= len(self.data):
                    self.pos += 2
                raw = self.data[self.pos-4:self.pos]
                entries.append(AnimationEntry(entry_offset, raw, is_terminator=True))
                break
            elif animation == 0xFFFE: # Loop
                if self.pos + 2 > len(self.data):
                    break
                loop_offset = self.read_s16()
                target = self.tell() + loop_offset - 2
                raw = self.data[self.pos-4:self.pos]
                entries.append(AnimationEntry(entry_offset, raw, is_control=True, loop_target=target))
                break
            else: # Standard Frame
                if self.pos + 6 > len(self.data):
                    break
                flag = self.read_u8()
                x = self.read_s8()
                y = self.read_s8()
                z = self.read_s8()
                count = self.read_u8()
                flip = self.read_u8()
                raw = self.data[self.pos-8:self.pos]
                entries.append(AnimationEntry(
                    offset=entry_offset, raw_bytes=raw, anim_id=animation, flag=flag,
                    vec=(x,y,z), wait_frames=count, flip_flags=flip
                ))

        return AnimationBlock(start_offset, entries)

    def check_subroutine_returns(self, start_offset: int, call_stack: Set[int] = None) -> bool:
        """
        Recursively checks if a subroutine ever returns
        """

        # Shouldn't happen
        if start_offset < self.base_offset or start_offset >= self.base_offset + len(self.data):
            return True

        # Check global cache first
        if call_stack is None:
            call_stack = set()
            
        # Cycle detection: A calls B calls A.
        # If we are already analyzing this offset in the recursion stack, treat this specific path as non-returning (loop)
        if start_offset in self.subroutine_return_cache:
            return self.subroutine_return_cache[start_offset]
            
        if start_offset in call_stack:
            return False
            
        # Add to stack for recursion
        call_stack.add(start_offset)
        
        # Save current position to restore later
        # Use direct position assignment to avoid re-calculation errors with seek()
        saved_pos = self.pos
        
        # Local BFS/Queue to traverse the graph of subroutine
        queue = deque([start_offset])
        visited = set()
        
        can_return = False
        ops_checked = 0
        # FIXME: necessary?
        MAX_OPS = 2000 # Safety limit to prevent analyzing massive chunks of unrelated data
        
        while queue and ops_checked < MAX_OPS:

            offset = queue.popleft()

            # Necessary?
            # Additional bounds check for queued items
            if offset < self.base_offset or offset >= self.base_offset + len(self.data):
                # unknown code; assume returns
                can_return = True 
                break

            if offset in visited:
                continue

            visited.add(offset)
            
            # Linear scan from this offset
            curr = offset
            self.seek(curr)
            block_ended = False
            
            while self.pos < len(self.data) and not block_ended:

                ops_checked += 1
                cmd = self.parse_command()

                if not cmd: 
                    block_ended = True
                    break
                    
                if cmd.opcode == 7: # RETURN_FROM_SUBROUTINE
                    can_return = True
                    block_ended = True
                    break
                if cmd.opcode == 5: # DEACTIVATE_SELF
                    block_ended = True
                    break
                if cmd.opcode == 6: # EXECUTE_SUBROUTINE
                    target = cmd.branch_target
                    # Recursively check the target
                    # If callee returns, execution can continue past this instruction
                    if self.check_subroutine_returns(target, call_stack):
                        pass
                    else:
                        # Callee never returns (infinite loop or deactivation)
                        block_ended = True
                        break

                targets = self._get_command_targets(cmd)
                
                # Filter targets:
                # - Include 'branch' types
                # - EXCLUDE Opcode 6 (EXECUTE_SUBROUTINE), which is handled recursively above
                branch_targets = [
                    t[0] for t in targets 
                    if t[2] == 'branch' and cmd.opcode != 6
                ]
                
                if branch_targets:
                    # Add branch targets to queue
                    for t in branch_targets:
                        if t not in visited:
                            queue.append(t)
                    # Assume fall-through is possible unless logic proves otherwise
                    pass

            if can_return:
                break
        
        # Restore state
        self.pos = saved_pos
        call_stack.remove(start_offset)

        # Cache result
        self.subroutine_return_cache[start_offset] = can_return

        return can_return
    
    def parse_executor(self, start_offset: int, max_commands: int = 500) -> List[ParsedCommand]:
        """Parse commands for a single executor starting at given offset"""

        self.seek(start_offset)

        commands = []
        count = 0
        
        while self.pos < len(self.data) - 1 and count < max_commands:

            # Check for padding via helper
            padding_cmd = self._try_parse_padding_or_data()

            if padding_cmd:
                commands.append(padding_cmd)
                count += 1
                continue
            
            cmd = self.parse_command()
            if cmd is None:
                break
            commands.append(cmd)
            count += 1
            
            if cmd.opcode == 5 or cmd.opcode == 7:
                break
            
            if cmd.opcode == 6 and cmd.branch_target is not None:
                if not self.check_subroutine_returns(cmd.branch_target):
                    cmd.branch_terminates = True
                    break
                
        return commands
    
    def parse_linear_scan(self, start_offset: int, end_offset: int = None, max_commands: int = 50000, known_anim_offsets: Set[int] = None) -> List[ParsedCommand]:
        """Parse all commands linearly without stopping at DEACTIVATE_SELF."""

        self.seek(start_offset)
        
        commands = []
        
        anim_targets = set(known_anim_offsets) if known_anim_offsets else set()
        
        if end_offset is not None:
            max_pos = end_offset - self.base_offset
        else:
            max_pos = len(self.data)

        # PRE-PASS: Scan for animation targets
        discovery_queue = deque()
        dry_run_count = 0

        while self.pos < max_pos - 1 and dry_run_count < max_commands:

            # Padding check for dry run
            if self.pos + 2 <= len(self.data):

                try:
                    peek = struct.unpack_from('>H', self.data, self.pos)[0]

                    if peek >= 98: 
                        if self.pos + 4 <= len(self.data):
                             try:
                                 nw = struct.unpack_from('>H', self.data, self.pos+2)[0]
                                 if (peek == 0xFFFF and nw == 0) or (peek == 0xFE00 and nw == 0):
                                     self.pos += 4
                                     dry_run_count += 1
                                     continue
                             except struct.error: pass

                        self.pos += 2
                        dry_run_count += 1

                        continue

                except struct.error:
                    break

            cmd = self.parse_command()

            if cmd:
                if cmd.anim_target is not None:
                    if self.base_offset <= cmd.anim_target < (self.base_offset + len(self.data)):
                        if cmd.anim_target not in anim_targets:
                            anim_targets.add(cmd.anim_target)
                            discovery_queue.append(cmd.anim_target)
            else:
                self.pos += 2

            dry_run_count += 1

        # Follow animation loops
        processed_anims = set()

        while discovery_queue:

            addr = discovery_queue.popleft()

            if addr in processed_anims: continue

            processed_anims.add(addr)

            if addr < self.base_offset or addr >= self.base_offset + len(self.data): continue

            try:
                block = self.parse_animation_sequence(addr)
                for entry in block.entries:
                    if entry.is_control and entry.loop_target is not None:
                        if entry.loop_target not in anim_targets:
                            if self.base_offset <= entry.loop_target < (self.base_offset + len(self.data)):
                                anim_targets.add(entry.loop_target)
                                discovery_queue.append(entry.loop_target)

            except Exception: pass

        # MAIN PASS
        self.seek(start_offset)

        count = 0

        while self.pos < max_pos - 1 and count < max_commands:
            curr_offset = self.tell()
            
            # 1. Animation Data
            if curr_offset in anim_targets:

                anim_block = self.parse_animation_sequence(curr_offset)

                for entry in anim_block.entries:
                    if entry.is_terminator:
                        name, params = "ANIM_END", []
                    elif entry.is_control:
                        name, params = "ANIM_LOOP", [("target", f"0x{entry.loop_target:X}")]
                    else:
                        name = "ANIM_FRAME"
                        params = [
                            ("id", f"id=0x{entry.anim_id:04X}"), ("flag", f"flag=0x{entry.flag:02X}"),
                            ("vec", f"vec=({entry.vec[0]}, {entry.vec[1]}, {entry.vec[2]})"),
                            ("wait", f"wait={entry.wait_frames}"), ("flip", f"flip=0x{entry.flip_flags:X}")
                        ]
                    commands.append(ParsedCommand(
                        offset=entry.offset, opcode=-1, size=len(entry.raw_bytes),
                        name=name, params=params, raw_bytes=entry.raw_bytes, extra_info="Animation Data"
                    ))

                continue

            # 2. Check for padding/data
            padding_cmd = self._try_parse_padding_or_data()

            if padding_cmd:
                commands.append(padding_cmd)
                count += 1
                continue
            
            # 3. Parse Regular Command
            cmd = self.parse_command()
            if cmd is None:
                if self.pos + 2 <= max_pos:
                    self.pos += 2
                else:
                    break
                continue
            
            if cmd.anim_target is not None:
                anim_targets.add(cmd.anim_target)

            commands.append(cmd)
            count += 1
                
        return commands

    def _get_command_targets(self, cmd: ParsedCommand) -> List[Tuple[int, int, str]]:

        targets = []

        if cmd.spawn_target:
            exec_idx, target = cmd.spawn_target
            targets.append((target, exec_idx, "spawn"))
        if cmd.branch_target is not None:
            targets.append((cmd.branch_target, 0, "branch"))
        return targets

    def _trace_execution(self, start_tuples: List[Tuple], 
                         visited_global: Set[int],
                         parse_context: Dict) -> Tuple[List[int], List[Tuple]]:

        queue = deque(start_tuples)
        anim_queue = deque()
        newly_parsed_offsets = []
        local_visited = set() 
        
        while queue:

            offset, exec_idx, spawned_by, is_branch, label = queue.popleft()
            
            if offset in local_visited: continue

            local_visited.add(offset)
            
            if offset < self.base_offset or offset >= self.base_offset + len(self.data):
                error_entry = (offset, exec_idx, spawned_by or 0, "branch" if is_branch else "spawn")
                parse_context['out_of_range'].append(error_entry)
                continue

            if offset not in parse_context['blocks']:
                commands = self.parse_executor(offset)
                if not commands: continue

                block = ExecutorBlock(
                    executor_index=exec_idx,
                    start_offset=offset,
                    commands=commands,
                    spawned_by=spawned_by,
                    is_branch=is_branch,
                    label=label,
                )
                parse_context['blocks'][offset] = block
                visited_global.add(offset)
            
            newly_parsed_offsets.append(offset)
            block = parse_context['blocks'][offset]
            
            for cmd in block.commands:
                targets = self._get_command_targets(cmd)
                for target_offset, target_exec_idx, type_str in targets:
                    next_exec_idx = target_exec_idx if type_str == "spawn" else exec_idx
                    if type_str == "spawn":
                        next_label = f"executor_{next_exec_idx}"
                        is_next_branch = False
                    elif type_str == "jump":
                        next_label = f"sub_{target_offset:X}"
                        is_next_branch = True
                    else:
                        next_label = f"branch_{target_offset:X}"
                        is_next_branch = True

                    if target_offset not in local_visited:
                        queue.append((target_offset, next_exec_idx, cmd.offset, is_next_branch, next_label))
                
                if cmd.anim_target is not None:
                     if cmd.anim_target not in parse_context['anim_blocks']:
                         anim_queue.append(cmd.anim_target)

        while anim_queue:
            anim_offset = anim_queue.popleft()
            if anim_offset in parse_context['anim_blocks']: continue
            if anim_offset < self.base_offset or anim_offset >= self.base_offset + len(self.data): continue
            anim_block = self.parse_animation_sequence(anim_offset)
            parse_context['anim_blocks'][anim_offset] = anim_block

        return newly_parsed_offsets

    def parse_graph_unified(self, entry_points: List[EntryPointInfo], follow_branches: bool = True) -> CutsceneGraph:
        """Parse graph with one or more entry points"""

        graph = CutsceneGraph(bank_start=self.base_offset, bank_end=self.base_offset + len(self.data))
        graph.entry_points = entry_points
        
        context = {
            'blocks': graph.blocks,
            'anim_blocks': graph.anim_blocks,
            'out_of_range': []
        }
        
        visited_global = set()

        for i, entry_info in enumerate(graph.entry_points):
            start_node = (entry_info.offset, entry_info.executor_index, None, False, "main")
            reachable_offsets = self._trace_execution([start_node], visited_global, context)
            
            if i == 0:
                graph.execution_order = reachable_offsets

            for offset in reachable_offsets:
                entry_info.reachable_blocks.add(offset)
                if offset not in graph.block_reachability:
                    graph.block_reachability[offset] = set()
                graph.block_reachability[offset].add(i)

        return graph

# =============================================================================
# FORMATTING
# =============================================================================

class AsmLabelManager:
    """Manages unique labels for the disassembler"""
    def __init__(self, prefix: str = ""):
        self.labels: Dict[int, str] = {}
        self.prefix = prefix
        self.counts = {"sub": 0, "branch": 0, "anim": 0, "data": 0, "exec": 0}

    def get_or_create(self, offset: int, type_hint: str = "branch", index_hint: int = None) -> str:
        if offset in self.labels:
            return self.labels[offset]
        
        if type_hint == "sub":
            name = f"{self.prefix}sub_{offset:X}"
        elif type_hint == "spawn":
            name = f"{self.prefix}executor_{index_hint}_{offset:X}"
        elif type_hint == "anim":
            name = f"{self.prefix}anim_{offset:X}"
        elif type_hint == "data":
            name = f"{self.prefix}data_{offset:X}"
        else:
            name = f"{self.prefix}branch_{offset:X}"
            
        self.labels[offset] = name
        return name

def format_asm_output(graph: CutsceneGraph, show_original_hex: bool = False) -> str:
    """
    Format CutsceneGraph as GNU Assembler (.s) data.
    Uses ASM_COMMAND_SPECS to properly format arguments as directives.
    """
    lines = []
    
    # Header
    lines.append(f"# {graph.bank_name} - Cutscene Data")
    lines.append(f"# Auto-generated Disassembly")
    lines.append("")
    lines.append(f'.section .data')
    lines.append(f'.balign 4')
    lines.append("")

    label_mgr = AsmLabelManager()

    # 1. Pre-scan for Labels
    # Iterate all blocks and commands to find targets
    all_offsets = sorted(list(set(graph.blocks.keys()) | set(graph.anim_blocks.keys())))
    
    # Add entry points as global labels
    for i, ep in enumerate(graph.entry_points):
        label_mgr.labels[ep.offset] = f"{graph.bank_name}_entry_{i}"

    # Scan commands for targets
    for block in graph.blocks.values():
        for cmd in block.commands:
            if cmd.spawn_target:
                exec_idx, target = cmd.spawn_target
                label_mgr.get_or_create(target, "spawn", exec_idx)
            if cmd.branch_target is not None:
                # Distinguish subs if opcode is 6
                hint = "sub" if cmd.opcode == 6 else "branch"
                label_mgr.get_or_create(cmd.branch_target, hint)
            if cmd.anim_target is not None:
                label_mgr.get_or_create(cmd.anim_target, "anim")

    # Scan anim blocks for loops
    for block in graph.anim_blocks.values():
        for entry in block.entries:
            if entry.is_control and entry.loop_target is not None:
                label_mgr.get_or_create(entry.loop_target, "anim")

    # 2. Iterate and Emit
    current_offset = graph.bank_start # Just tracking logic, though graph is sparse usually
    
    for offset in all_offsets:
        
        # Padding/Alignment Check? 
        # Since we are iterating strictly parsed blocks, if there are gaps, we might want to emit them?
        # For now, let's assume the graph covers the relevant data.
        
        # Emit Label if present
        if offset in label_mgr.labels:
            lines.append(f"{label_mgr.labels[offset]}:")
        
        if offset in graph.blocks:
            block = graph.blocks[offset]
            for cmd in block.commands:
                
                # Check if internal label needed (e.g. into middle of block - rare but possible)
                if cmd.offset != offset and cmd.offset in label_mgr.labels:
                    lines.append(f"{label_mgr.labels[cmd.offset]}:")

                line_parts = []
                
                # Special handling for Padding/Data pseudo-commands
                if cmd.name.startswith("PADDING") or cmd.name.startswith("DATA"):
                    # Emit raw bytes
                    byte_str = ", ".join([f"0x{b:02X}" for b in cmd.raw_bytes])
                    lines.append(f"    .byte {byte_str}")
                    continue

                # Standard Command
                # Emit Opcode
                comment = f" # {cmd.name}"
                lines.append(f"    .half 0x{cmd.opcode:04X}{comment}")
                
                specs = ASM_COMMAND_SPECS.get(cmd.opcode, (None, []))[1]
                
                # We need to pull data from raw_bytes based on specs
                # The ParsedCommand already has raw_bytes. 
                # We parse them strictly according to spec types for output.
                
                curr_byte_pos = 2 # Skip opcode
                
                for spec_type in specs:
                    if spec_type == 'pad8':
                        val = cmd.raw_bytes[curr_byte_pos]
                        lines.append(f"    .byte 0x{val:02X}")
                        curr_byte_pos += 1
                    elif spec_type == 'pad16':
                        val = struct.unpack_from('>H', cmd.raw_bytes, curr_byte_pos)[0]
                        lines.append(f"    .half 0x{val:04X}")
                        curr_byte_pos += 2
                    elif spec_type == 'u8' or spec_type == 's8':
                        val = cmd.raw_bytes[curr_byte_pos]
                        lines.append(f"    .byte 0x{val:02X}")
                        curr_byte_pos += 1
                    elif spec_type == 'u16' or spec_type == 's16':
                        val = struct.unpack_from('>H', cmd.raw_bytes, curr_byte_pos)[0]
                        lines.append(f"    .half 0x{val:04X}")
                        curr_byte_pos += 2
                    elif spec_type == 'u32' or spec_type == 's32':
                        val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                        lines.append(f"    .word 0x{val:08X}")
                        curr_byte_pos += 4
                    elif spec_type == 'rel16':
                        # This is the tricky one.
                        # Original bytes are an offset. We want "Label - ."
                        # The parse_command logic already calculated the absolute target.
                        # We need to map which parameter corresponds to this spec.
                        # Since specs are ordered, we can usually infer, OR rely on pre-calculated targets in ParsedCommand.
                        
                        # Fallback: Read the raw offset, check if it matches a known target
                        raw_offset = struct.unpack_from('>h', cmd.raw_bytes, curr_byte_pos)[0]
                        
                        # Logic to determine which target field to use (branch vs spawn vs callback)
                        # This assumes one rel16 per command generally, or specific order.
                        target_addr = None
                        
                        # Try to reconstruct absolute address
                        # Note: Different commands calculate relative offset from different base points
                        # Usually it is relative to the start of the *argument* or the *instruction*.
                        # Cutscene engine usually does: current_ptr + offset.
                        
                        # However, our parsed command object has 'branch_target' or 'spawn_target' populated.
                        if cmd.branch_target is not None:
                            target_addr = cmd.branch_target
                        elif cmd.spawn_target:
                            target_addr = cmd.spawn_target[1]
                        elif cmd.anim_target:
                            target_addr = cmd.anim_target
                        
                        # If we found a target address and we have a label for it
                        if target_addr is not None and target_addr in label_mgr.labels:
                            label = label_mgr.labels[target_addr]
                            lines.append(f"    .half {label} - .")
                        else:
                            # Fallback if logic fails or no label exists
                             lines.append(f"    .half {raw_offset}")
                        
                        curr_byte_pos += 2

                    elif spec_type == 'addr32':
                        val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                        # Use variable formatter but force label usage
                        sym = format_variable_address(val, use_labels=True, asm_mode=True)
                        lines.append(f"    .word {sym}")
                        curr_byte_pos += 4
                    elif spec_type == 'rom_start32':
                        val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                        sym = format_rom_address(val, use_labels=True, is_end_symbol=False)
                        lines.append(f"    .word {sym}")
                        curr_byte_pos += 4
                    elif spec_type == 'rom_end32':
                        val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                        sym = format_rom_address(val, use_labels=True, is_end_symbol=True)
                        lines.append(f"    .word {sym}")
                        curr_byte_pos += 4
                    else:
                        lines.append(f"    # ERROR: Unknown Type {spec_type}")

        elif offset in graph.anim_blocks:
            block = graph.anim_blocks[offset]
            for entry in block.entries:
                 if entry.offset in label_mgr.labels and entry.offset != offset:
                     lines.append(f"{label_mgr.labels[entry.offset]}:")
                 
                 # Format animation data manually
                 if entry.is_terminator:
                     lines.append("    .half 0xFFFF")
                     lines.append("    .half 0")
                 elif entry.is_control:
                     lines.append("    .half 0xFFFE")
                     tgt = entry.loop_target
                     if tgt in label_mgr.labels:
                         lines.append(f"    .half {label_mgr.labels[tgt]} - .")
                     else:
                         lines.append("    .half 0") # Should not happen
                 else:
                     lines.append(f"    .half 0x{entry.anim_id:04X}")
                     lines.append(f"    .byte 0x{entry.flag:02X}")
                     lines.append(f"    .byte {entry.vec[0]}, {entry.vec[1]}, {entry.vec[2]}")
                     lines.append(f"    .byte {entry.wait_frames}")
                     lines.append(f"    .byte 0x{entry.flip_flags:02X}")

    return "\n".join(lines)

def format_asm_linear(commands: List[ParsedCommand], bank_name: str) -> str:
    """
    Format a linear list of commands into GNU Assembler (.s) data.
    Preserves memory layout while resolving labels.
    """
    lines = []
    
    # Header
    lines.append(f"# {bank_name} - Cutscene Data (Linear Scan)")
    lines.append(f"# Auto-generated Disassembly")
    lines.append("")
    lines.append(f'.section .data')
    lines.append(f'.balign 4')
    lines.append(f'.global {bank_name}_start')
    lines.append(f'{bank_name}_start:')
    lines.append("")

    label_mgr = AsmLabelManager()
    
    # Pass 1: Collect Labels from all commands
    for cmd in commands:
        # Standard Targets
        if cmd.spawn_target:
             label_mgr.get_or_create(cmd.spawn_target[1], "spawn", cmd.spawn_target[0])
        if cmd.branch_target is not None:
             hint = "sub" if cmd.opcode == 6 else "branch"
             label_mgr.get_or_create(cmd.branch_target, hint)
        if cmd.anim_target is not None:
             label_mgr.get_or_create(cmd.anim_target, "anim")

        # Reconstruct ANIM_LOOP targets (opcode -1, name="ANIM_LOOP")
        if cmd.opcode == -1 and cmd.name == "ANIM_LOOP":
             # Raw Bytes: FE 00 [OFF_HI] [OFF_LO]
             # Target = Current_Pos + 2 (size of FE 00) + s16_val
             if len(cmd.raw_bytes) >= 4:
                 val = struct.unpack_from('>h', cmd.raw_bytes, 2)[0]
                 target = cmd.offset + 2 + val
                 label_mgr.get_or_create(target, "anim")

        # Pass 2: Emission
    for cmd in commands:
        
        # Emit Label if current offset is a known target
        if cmd.offset in label_mgr.labels:
            lines.append(f"{label_mgr.labels[cmd.offset]}:")
        
        # Handle Animation Pseudo-Commands
        if cmd.opcode == -1:
            if cmd.name == "ANIM_END":
                lines.append("    .half 0xFFFF")
                lines.append("    .half 0")
            elif cmd.name == "ANIM_LOOP":
                lines.append("    .half 0xFFFE")
                # Use raw offset for assemblability
                val = struct.unpack_from('>h', cmd.raw_bytes, 2)[0]
                lines.append(f"    .half {val}")
            else:
                 # ANIM_FRAME (Standard 8-byte frame)
                 if len(cmd.raw_bytes) == 8:
                     anim_id = struct.unpack_from('>H', cmd.raw_bytes, 0)[0]
                     flag = cmd.raw_bytes[2]
                     vec = struct.unpack_from('bbb', cmd.raw_bytes, 3)
                     wait = cmd.raw_bytes[6]
                     flip = cmd.raw_bytes[7]
                     lines.append(f"    .half 0x{anim_id:04X}")
                     lines.append(f"    .byte 0x{flag:02X}")
                     lines.append(f"    .byte {vec[0]}, {vec[1]}, {vec[2]}")
                     lines.append(f"    .byte {wait}")
                     lines.append(f"    .byte 0x{flip:02X}")
                 else:
                      # Fallback for weirdly sized frames
                      byte_str = ", ".join([f"0x{b:02X}" for b in cmd.raw_bytes])
                      lines.append(f"    .byte {byte_str}")
        
        # Handle Padding/Raw Data
        elif cmd.name.startswith("PADDING") or cmd.name.startswith("DATA"):
             byte_str = ", ".join([f"0x{b:02X}" for b in cmd.raw_bytes])
             lines.append(f"    .byte {byte_str}")
        
        # Handle Standard Commands
        else:
             comment = f" # {cmd.name}"
             lines.append(f"    .half 0x{cmd.opcode:04X}{comment}")
             
             specs = ASM_COMMAND_SPECS.get(cmd.opcode, (None, []))[1]
             curr_byte_pos = 2 # Skip opcode
             
             for spec_type in specs:
                if spec_type == 'pad8':
                    val = cmd.raw_bytes[curr_byte_pos]
                    lines.append(f"    .byte 0x{val:02X}")
                    curr_byte_pos += 1
                elif spec_type == 'pad16':
                    val = struct.unpack_from('>H', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .half 0x{val:04X}")
                    curr_byte_pos += 2
                elif spec_type == 'u8' or spec_type == 's8':
                    val = cmd.raw_bytes[curr_byte_pos]
                    lines.append(f"    .byte 0x{val:02X}")
                    curr_byte_pos += 1
                elif spec_type == 'u16' or spec_type == 's16':
                    val = struct.unpack_from('>H', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .half 0x{val:04X}")
                    curr_byte_pos += 2
                elif spec_type == 'u32' or spec_type == 's32':
                    val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .word 0x{val:08X}")
                    curr_byte_pos += 4
                elif spec_type == 'rel16':
                    # For ASM output, use raw offset values to ensure assemblability
                    # (label - . syntax requires labels to be defined, which isn't guaranteed
                    # for all branch targets in linear scan mode)
                    raw_offset = struct.unpack_from('>h', cmd.raw_bytes, curr_byte_pos)[0]
                    lines.append(f"    .half {raw_offset}")
                    curr_byte_pos += 2

                elif spec_type == 'addr32':
                    val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                    sym = format_variable_address(val, use_labels=True, asm_mode=True)
                    lines.append(f"    .word {sym}")
                    curr_byte_pos += 4
                elif spec_type == 'rom_start32':
                    val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                    sym = format_rom_address(val, use_labels=True, is_end_symbol=False)
                    lines.append(f"    .word {sym}")
                    curr_byte_pos += 4
                elif spec_type == 'rom_end32':
                    val = struct.unpack_from('>I', cmd.raw_bytes, curr_byte_pos)[0]
                    sym = format_rom_address(val, use_labels=True, is_end_symbol=True)
                    lines.append(f"    .word {sym}")
                    curr_byte_pos += 4
                else:
                    lines.append(f"    # ERROR: Unknown Type {spec_type}")

    lines.append("")
    lines.append(f'{bank_name}_end:')
    return "\n".join(lines)

def format_hex_bytes(raw_bytes: bytes, max_width: int = 30) -> str:
    """Format raw bytes as spaced hex string with optional truncation."""
    hex_str = raw_bytes.hex().upper()
    hex_spaced = ' '.join(hex_str[i:i+2] for i in range(0, len(hex_str), 2))
    if len(hex_spaced) > max_width:
        hex_spaced = hex_spaced[:max_width-3] + "..."
    return hex_spaced

def format_control_flow_annotation(cmd: ParsedCommand) -> Optional[str]:
    """Generate control flow annotation comment for a command."""

    if cmd.spawn_target:
        
        exec_idx, target = cmd.spawn_target
        
        if cmd.opcode == CutsceneOpcode.SPAWN_EXECUTOR: # SPAWN_EXECUTOR
            return f"// -> Spawns executor {exec_idx} @ 0x{target:X}"
        else:  # SET_OTHER_EXECUTOR_PTR (opcode 12)
            return f"// -> Sets executor {exec_idx} ptr to 0x{target:X}"

    elif cmd.branch_target is not None:

        if cmd.opcode == CutsceneOpcode.EXECUTE_SUBROUTINE:

            if cmd.branch_terminates:
                return f"// -> No return from jump or infinite loop @ 0x{cmd.branch_target:X}"
            else:
                return f"// -> Call subroutine @ 0x{cmd.branch_target:X}"

        else:
            return f"// -> Branch to 0x{cmd.branch_target:X}"

    elif cmd.jump_target is not None:
        return f"// -> Subroutine @ 0x{cmd.jump_target:X}"
    elif cmd.anim_target is not None:
        return f"// -> Animation @ 0x{cmd.anim_target:X}"

    return None

def format_command_line(cmd: ParsedCommand, show_hex: bool = True, 
                        hex_width: int = 30, show_annotations: bool = True) -> str:
    """Format a single command as an output line."""
    parts = [f"/* 0x{cmd.offset:06X} */"]
    
    if show_hex:
        hex_spaced = format_hex_bytes(cmd.raw_bytes, hex_width)
        parts.append(f"/* {hex_spaced:<{hex_width}} */")
    
    macro = cmd.to_macro()
    
    if show_annotations:
        annotation = format_control_flow_annotation(cmd)
        if annotation:
            macro += f"  {annotation}"
    
    parts.append(macro)
    return "  ".join(parts)

def format_block_generic(block: ExecutorBlock, reachability_note: str = None, show_hex: bool = True) -> str:
    """Unified block formatter."""
    lines = []
    
    if block.is_branch:
        header = f"// === BRANCH TARGET @ 0x{block.start_offset:06X} ==="
    else:
        header = f"// === EXECUTOR {block.executor_index} @ 0x{block.start_offset:06X} ({block.label}) ==="
    lines.append(header)
    
    if reachability_note:
        lines.append(f"// Reachable from: {reachability_note}")
    if block.spawned_by is not None:
        lines.append(f"// (spawned from 0x{block.spawned_by:06X})")
    
    for cmd in block.commands:
        lines.append(format_command_line(cmd, show_hex=show_hex, hex_width=30, show_annotations=True))
    
    return "\n".join(lines)

def format_cutscene_output(graph: CutsceneGraph, show_hex: bool = True) -> str:
    """Consolidated formatter for cutscene graphs"""

    lines = []
    
    # Header
    lines.append(f"// ============================================")
    lines.append(f"// {graph.bank_name.upper()}: 0x{graph.bank_start:06X} - 0x{graph.bank_end:06X}")
    lines.append(f"// Total bytes: {graph.bank_end - graph.bank_start} (0x{graph.bank_end - graph.bank_start:X})")
    lines.append(f"// Entry points: {len(graph.entry_points)}")
    lines.append(f"// Total blocks: {len(graph.blocks)}")
    lines.append(f"// Total anim blocks: {len(graph.anim_blocks)}")
    
    lines.append(f"// ============================================")
    lines.append("")
    
    # List entry points if multiple
    if len(graph.entry_points) > 0:
        lines.append("// Entry Points:")
        for i, ep in enumerate(graph.entry_points):
            unique_count = len(graph.get_unique_blocks(i))
            lines.append(f"//   [{i}] @ 0x{ep.offset:06X} (array_idx={ep.array_index}, exec={ep.executor_index:04X}) - {len(ep.reachable_blocks)} blocks ({unique_count} unique)")
        lines.append("")

    # Helper for reachability text
    def get_reachability_str(offset: int) -> Optional[str]:
        if len(graph.entry_points) <= 1: return None
        entry_indices = graph.block_reachability.get(offset, set())
        if len(entry_indices) == len(graph.entry_points):
            return "all entries"
        elif len(entry_indices) > 1:
            return "entries " + ", ".join(str(i) for i in sorted(entry_indices))
        elif len(entry_indices) == 1:
            return f"entry {list(entry_indices)[0]}"
        return "unreachable"

    # Helper to format a block (code or animation)
    def format_offset(offset: int) -> None:
        if offset in graph.blocks:
            block = graph.blocks[offset]
            reachability = get_reachability_str(offset)
            lines.append(format_block_generic(block, reachability, show_hex))
            lines.append("")
        elif offset in graph.anim_blocks:
            anim_block = graph.anim_blocks[offset]
            lines.append(f"// === ANIMATION DATA @ 0x{offset:06X} ===")
            for entry in anim_block.entries:
                parts = [f"/* 0x{entry.offset:06X} */"]
                if show_hex:
                    hex_spaced = format_hex_bytes(entry.raw_bytes, 30)
                    parts.append(f"/* {hex_spaced:<30} */")
                parts.append(entry.to_string())
                lines.append("  ".join(parts))
            lines.append("")

    # Organize blocks by section
    if len(graph.entry_points) > 1:
        # Multi-entry cutscene: show shared code first, then unique blocks per entry
        shared_blocks = graph.get_shared_blocks()

        # Show shared code section
        if shared_blocks:
            lines.append("// ============================================")
            lines.append(f"// SHARED CODE ({len(shared_blocks)} blocks)")
            lines.append("// ============================================")
            lines.append("")
            for offset in shared_blocks:
                format_offset(offset)

        # Show unique blocks for each entry point
        for i, ep in enumerate(graph.entry_points):
            unique_blocks = graph.get_unique_blocks(i)
            if unique_blocks:
                lines.append("// ============================================")
                lines.append(f"// ENTRY POINT {i} Blocks @ 0x{ep.offset:06X} ({len(unique_blocks)} blocks)")
                lines.append("// ============================================")
                lines.append("")
                for offset in unique_blocks:
                    format_offset(offset)
    else:
        # Single entry: just show all blocks in order
        all_offsets = set(graph.blocks.keys()) | set(graph.anim_blocks.keys())
        sorted_offsets = sorted(list(all_offsets))
        for offset in sorted_offsets:
            format_offset(offset)
    
    # Summary footer
    total_cmds = sum(len(b.commands) for b in graph.blocks.values())
    code_bytes = sum(sum(c.size for c in b.commands) for b in graph.blocks.values())
    anim_bytes = sum(sum(len(e.raw_bytes) for e in b.entries) for b in graph.anim_blocks.values())
    shared_blocks = graph.get_shared_blocks()

    lines.append(f"// ============================================")
    lines.append(f"// SUMMARY")
    lines.append(f"// ============================================")
    lines.append(f"// Total: {len(graph.blocks)} code blocks, {len(graph.anim_blocks)} anim blocks")
    lines.append(f"// Parsed: {total_cmds} commands, {code_bytes + anim_bytes} bytes ({code_bytes} code, {anim_bytes} anim)")

    if len(graph.entry_points) > 1:
        lines.append(f"// Shared code: {len(shared_blocks)} blocks")

    return "\n".join(lines)

def format_scan_output(commands: List[ParsedCommand], item: dict, 
                       show_hex: bool = True, verbose: bool = False) -> str:
    """Format linear scan output."""
    lines = []
    
    # Header
    lines.append(f"// SCAN: {item['name']} (0x{item['start']:X}-0x{item['end']:X})")
    lines.append(f"// Total bytes: {item['end'] - item['start']} (0x{item['end'] - item['start']:X})")
    lines.append("// ============================================")
    lines.append("")

    # Pre-calculate targets for annotations
    spawn_targets = {}
    branch_targets = set()
    if verbose:
        for cmd in commands:
            if cmd.spawn_target:
                spawn_targets[cmd.spawn_target[1]] = cmd.spawn_target[0]
            if cmd.branch_target is not None:
                branch_targets.add(cmd.branch_target)

    # Format commands
    for cmd in commands:
        # Section headers
        if verbose:
            if cmd.offset in spawn_targets:
                lines.append(f"\n// --- SPAWN TARGET: executor {spawn_targets[cmd.offset]} ---")
            elif cmd.offset in branch_targets:
                lines.append(f"\n// --- BRANCH TARGET ---")

        lines.append(format_command_line(cmd, show_hex=show_hex, hex_width=24, show_annotations=verbose))

        # Block separators
        if verbose and cmd.opcode in [5, 7]:  # DEACTIVATE_SELF, RETURN
            lines.append(f"// {'-'*60}")

    # Summary footer
    if verbose:
        valid_cmds = [c for c in commands if not c.name.startswith("DATA_") and not c.name.startswith("PADDING_")]
        data_cmds = [c for c in commands if c.name.startswith("DATA_")]
        lines.append(f"\n// Summary: {len(valid_cmds)} commands, {len(data_cmds)} data words")
        lines.append(f"// Spawn targets: {len(spawn_targets)}")
        lines.append(f"// Branch targets: {len(branch_targets)}")

    return "\n".join(lines)

# =============================================================================
# IO
# =============================================================================

def load_cutscene_addresses(csv_path: str) -> List[Tuple[int, int, str]]:
    banks = []
    with open(csv_path, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            if len(row) >= 3:
                start = int(row[0], 16)
                end = int(row[1], 16)
                name = row[2]
                banks.append((start, end, name))
    return banks

def load_entry_points(csv_path: str) -> Dict[str, List[Dict]]:

    entry_points = {}

    with open(csv_path, 'r') as f:

        reader = csv.DictReader(f)

        for row in reader:
            bank_name = row['bank_name']
            if bank_name not in entry_points:
                entry_points[bank_name] = []
            entry_points[bank_name].append({
                'array_index': int(row['array_index']),
                'rom_offset': int(row['entry_rom_offset'], 16),
                'executor_index': int(row['executor_index'], 16),
            })

    return entry_points

# =============================================================================
# WORK ITEM BUILDERS
# =============================================================================

def get_entry_points_for_bank(bank_name: str, start_offset: int, ep_map: Dict[str, List[Dict]]) -> List[EntryPointInfo]:
    """Get entry points for a bank, or default to start offset if not found."""

    if bank_name in ep_map:
        return [
            EntryPointInfo(
                offset=ep_data['rom_offset'],
                array_index=ep_data['array_index'],
                executor_index=ep_data['executor_index']
            )
            for ep_data in ep_map[bank_name]
        ]
    else:
        return [EntryPointInfo(offset=start_offset, array_index=0, executor_index=0)]


def build_work_items_from_csv(csv_path: str, entry_points_csv: str) -> List[Dict]:
    """Load all banks from CSV files and build work items."""

    banks = load_cutscene_addresses(csv_path)
    
    try:
        ep_map = load_entry_points(entry_points_csv)
    except Exception:
        ep_map = {}
    
    return [
        {
            'name': name,
            'start': start,
            'end': end,
            'entries': get_entry_points_for_bank(name, start, ep_map)
        }
        for start, end, name in banks
    ]


def build_work_item_from_offset(offset: int, length: int) -> Dict:
    """Build a single work item from offset and length."""

    start = offset
    end = offset + length
    entries = [EntryPointInfo(offset=start, array_index=0, executor_index=0)]
    
    return {
        'name': f"segment_{start:X}",
        'start': start,
        'end': end,
        'entries': entries
    }


def process_work_item(item: Dict, args, output_dir: Path) -> None:
    """Process a single work item: parse, format, and write output."""

    if rom is None:
        set_rom()
    
    data = rom[item['start']:item['end']]
    parser = CutsceneBytecodeParser(data, base_offset=item['start'], enable_labels=args.labels)
    
    # Determine output mode and format
    if args.asm:
        known_anims = set(ORPHANED_ANIM_OFFSETS.get(item['name'], []))
        commands = parser.parse_linear_scan(item['start'], item['end'], known_anim_offsets=known_anims)
        output_content = format_asm_linear(commands, item['name'])
        mode_suffix = "asm"
        ext = ".s"
    elif args.scan:
        known_anims = set(ORPHANED_ANIM_OFFSETS.get(item['name'], []))
        commands = parser.parse_linear_scan(item['start'], item['end'], known_anim_offsets=known_anims)
        output_content = format_scan_output(commands, item, show_hex=not args.no_hex, verbose=args.verbose_scan)
        mode_suffix = "linear"
        ext = ".txt"
    else:  # Graph mode (default)
        graph = parser.parse_graph_unified(item['entries'], follow_branches=True)
        graph.bank_name = item['name']
        output_content = format_cutscene_output(graph, show_hex=not args.no_hex)
        mode_suffix = "graph"
        ext = ".txt"
    
    # Write output
    # ASM mode uses simple names for build integration, other modes use descriptive suffixes
    if args.asm:
        out_file = output_dir / f"{item['name']}{ext}"
    else:
        out_file = output_dir / f"{item['name']}-{mode_suffix}{ext}"
    with open(out_file, 'w') as f:
        f.write(output_content)
    print(f"  Wrote {out_file.name}")

# =============================================================================
# MAIN
# =============================================================================

def main():

    parser = argparse.ArgumentParser(
        description='HM64 Cutscene Bytecode Parser (Unified)',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    # Inputs
    source_group = parser.add_mutually_exclusive_group(required=True)
    source_group.add_argument('--offset', '-o', type=lambda x: int(x, 0),
                        help='Offset in file to start parsing')
    source_group.add_argument('--all', '-a', action='store_true',
                        help='Parse all banks from CSV')
    source_group.add_argument('--bank', '-b', type=str,
                        help='Parse a single bank by name (e.g., cutsceneBank1)')

    # Helpers
    parser.add_argument('--length', '-l', type=lambda x: int(x, 0),
                        help='Length (required with --offset)')
    parser.add_argument('--csv', type=str, default='./cutscene_addresses.csv')
    parser.add_argument('--entry-points-csv', type=str, default='./cutscene_entry_points.csv')
    parser.add_argument('--output-dir', type=str, default='../assets/cutscenes')
    parser.add_argument('--labels', action='store_true', help='Use constant labels (for text output)')
    parser.add_argument('--no-hex', action='store_true', help='Hide hex bytes')
    parser.add_argument('--verbose-scan', action='store_true', help='Add control flow comments/separators to linear scan')
    
    # Processing mode
    mode_group = parser.add_mutually_exclusive_group()
    mode_group.add_argument('--scan', action='store_true', help='Linear scan (ignore control flow)')
    mode_group.add_argument('--graph', action='store_true', help='Graph traversal (default)')
    mode_group.add_argument('--asm', action='store_true', help='Disassemble to GNU ASM')

    args = parser.parse_args()

    # Build work items
    try:
        if args.all:
            work_items = build_work_items_from_csv(args.csv, args.entry_points_csv)
        elif args.bank:
            all_items = build_work_items_from_csv(args.csv, args.entry_points_csv)
            work_items = [item for item in all_items if item['name'] == args.bank]
            if not work_items:
                available = [item['name'] for item in all_items]
                print(f"Error: Bank '{args.bank}' not found. Available: {', '.join(available)}", file=sys.stderr)
                sys.exit(1)
        elif args.offset is not None:
            if args.length is None:
                print("Error: --length is required with --offset", file=sys.stderr)
                sys.exit(1)
            work_items = [build_work_item_from_offset(args.offset, args.length)]
    except Exception as e:
        print(f"Error loading CSVs: {e}", file=sys.stderr)
        sys.exit(1)

    # Process all work items
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    for item in work_items:
        try:
            process_work_item(item, args, output_dir)
        except Exception as e:
            raise e


if __name__ == '__main__':
    main()