"""
Cutscene bytecode opcode definitions for HM64.

Shared between the extractor and transpiler.
"""

from enum import IntEnum
from typing import Dict, List, Tuple


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
    ENTITY_WALK = 28
    SET_MAP_ROTATION = 29
    SET_BEHAVIOR_FLAGS = 30
    SET_ENTITY_WANDER = 31
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
    SET_HOLDING_ANIMATION_FLAG = 68
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
    STOP_SEQUENCE_WITH_FADE = 88
    SET_AUDIO_SEQUENCE_VOLUME = 89
    SET_SFX = 90
    IDLE_WHILE_SEQUENCE_PLAYING = 91
    UPDATE_MESSAGE_BOX_RGBA = 92
    WAIT_MESSAGE_BOX_READY = 93
    SET_SPRITE_BILINEAR = 94
    SET_MAP_ADDITION = 95
    SET_MAP_GROUND_OBJECT = 96
    SET_MESSAGE_INTERPOLATION = 97


# =============================================================================
# COMMAND SPECS - Single source of truth for both ASM-to-DSL and transpiler
# =============================================================================
# Format: opcode -> (name, size, [(param_name, param_type), ...])
# Types: 'u8', 'u16', 'u32', 's8', 's16', 's32', 'rel16', 'addr32', 'pad8', 'pad16', 'rom_start32', 'rom_end32'

COMMAND_SPECS: Dict[int, Tuple[str, int, List[Tuple[str, str]]]] = {
    0: ("CMD_SET_ANIM_DATA_PTR_WITH_FLAG", 4, [('target', 'rel16')]),
    1: ("CMD_SET_ANIM_DATA_PTR", 4, [('target', 'rel16')]),
    2: ("CMD_SET_COORDINATES", 8, [('x', 's16'), ('y', 's16'), ('z', 's16')]),
    3: ("CMD_SET_FRAME_DELTA", 8, [('x', 's8'), ('y', 's8'), ('z', 's8'), ('wait', 'u8'), ('pad', 'pad16')]),
    4: ("CMD_SET_WAIT_FRAMES", 4, [('frames', 'u16')]),
    5: ("CMD_DEACTIVATE_SELF", 4, [('pad', 'pad16')]),
    6: ("CMD_EXECUTE_SUBROUTINE", 4, [('target', 'rel16')]),
    7: ("CMD_RETURN_FROM_SUBROUTINE", 4, [('pad', 'pad16')]),
    8: ("CMD_BRANCH_ON_CURRENT_BUTTON", 12, [('entity', 'u16'), ('button', 'u32'), ('target', 'rel16'), ('pad', 'pad16')]),
    9: ("CMD_BRANCH_ON_BUTTON_PRESSED", 12, [('entity', 'u16'), ('button', 'u32'), ('target', 'rel16'), ('pad', 'pad16')]),
    10: ("CMD_BRANCH_ON_BUTTON_REPEAT", 12, [('entity', 'u16'), ('button', 'u32'), ('target', 'rel16'), ('pad', 'pad16')]),
    11: ("CMD_SPAWN_EXECUTOR", 8, [('executor', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]),
    12: ("CMD_SET_OTHER_EXECUTOR_PTR", 8, [('executor', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]),
    13: ("CMD_DEACTIVATE_EXECUTOR", 4, [('executor', 'u16')]),
    14: ("CMD_DMA_SPRITE", 56, [('sprite', 'u16'), ('asset_type', 'u16'), ('pad', 'pad16'),
         ('rom_tex_start', 'rom_start32'), ('rom_tex_end', 'rom_end32'),
         ('rom_assets_start', 'rom_start32'), ('rom_assets_end', 'rom_end32'),
         ('rom_sheet_start', 'rom_start32'), ('rom_sheet_end', 'rom_end32'),
         ('tex1_vaddr', 'addr32'), ('tex2_vaddr', 'addr32'),
         ('palette_vaddr', 'addr32'), ('anim_vaddr', 'addr32'),
         ('s2p_vaddr', 'addr32'), ('sheet_idx_vaddr', 'addr32')]),
    15: ("CMD_SET_ENTITY_ANIMATIONS", 16, [('entity', 'u16'), ('a0', 'u16'), ('a1', 'u16'), ('a2', 'u16'),
         ('a3', 'u16'), ('a4', 'u16'), ('a5', 'u16')]),
    16: ("CMD_DO_DMA", 16, [('pad', 'pad16'), ('start', 'rom_start32'), ('end', 'rom_end32'), ('dest', 'addr32')]),
    17: ("CMD_SET_U8_VALUE", 8, [('pad', 'pad8'), ('value', 'u8'), ('address', 'addr32')]),
    18: ("CMD_SET_U16_VALUE", 8, [('value', 'u16'), ('address', 'addr32')]),
    19: ("CMD_SET_U32_VALUE", 12, [('pad', 'pad16'), ('address', 'addr32'), ('value', 'u32')]),
    20: ("CMD_BRANCH_U8_VAR_WITHIN_RANGE", 12, [('min', 'u8'), ('max', 'u8'), ('address', 'addr32'), ('target', 'rel16'), ('pad', 'pad16')]),
    21: ("CMD_BRANCH_U16_VAR_WITHIN_RANGE", 16, [('min', 'u16'), ('max', 'u16'), ('pad', 'pad16'), ('address', 'addr32'), ('target', 'rel16'), ('pad2', 'pad16')]),
    22: ("CMD_BRANCH_U32_VAR_WITHIN_RANGE", 20, [('pad', 'pad16'), ('min', 'u32'), ('max', 'u32'), ('address', 'addr32'), ('target', 'rel16'), ('pad2', 'pad16')]),
    23: ("CMD_SET_SPECIAL_BIT", 8, [('bit', 'u16'), ('address', 'addr32')]),
    24: ("CMD_TOGGLE_SPECIAL_BIT", 8, [('bit', 'u16'), ('address', 'addr32')]),
    25: ("CMD_BRANCH_ON_SPECIAL_BIT", 12, [('bit', 'u16'), ('address', 'addr32'), ('target', 'rel16'), ('pad', 'pad16')]),
    26: ("CMD_SET_ASSET_ROTATION", 4, [('direction', 'u8'), ('pad', 'pad8')]),
    27: ("CMD_SETUP_MAP_ASSET", 4, [('mapIndex', 'u16')]),
    28: ("CMD_ENTITY_WALK", 8, [('distance', 'u16'), ('direction', 'u8'), ('speed', 'u8'), ('pad', 'pad16')]),
    29: ("CMD_SET_MAP_ROTATION", 8, [('mapIndex', 'u16'), ('arg1', 'u8'), ('rotation', 'u8'), ('pad', 'pad16')]),
    30: ("CMD_SET_BEHAVIOR_FLAGS", 4, [('flags', 'u8'), ('pad', 'pad8')]),
    31: ("CMD_SET_ENTITY_WANDER", 8, [('movement_distance', 'u8'), ('pad', 'pad8'), ('collision_width', 'u16'), ('collision_height', 'u16')]),
    32: ("CMD_INIT_MESSAGE_BOX_TYPE1", 8, [('box', 'u16'), ('bank', 'u16'), ('index', 'u16')]),
    33: ("CMD_WAIT_MESSAGE_BOX_CLOSED", 4, [('box', 'u16')]),
    34: ("CMD_SET_MESSAGE_BOX_VIEW_POSITION", 12, [('box', 'u16'), ('x', 's16'), ('y', 's16'), ('z', 's16'), ('pad', 'pad16')]),
    35: ("CMD_RESET_MESSAGE_BOX_AVATAR", 4, [('box', 'u16')]),
    36: ("CMD_ENTITY_RUN", 8, [('distance', 'u16'), ('direction', 'u8'), ('speed', 'u8'), ('pad', 'pad16')]),
    37: ("CMD_SET_ENTITY_ANIMATION", 4, [('anim', 'u16')]),
    38: ("CMD_SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE", 4, [('anim', 'u16')]),
    39: ("CMD_SET_CALLBACK_ROUTINE", 8, [('entity', 'u16'), ('button', 'u16'), ('target', 'rel16')]),
    40: ("CMD_PAUSE_ENTITY", 4, [('entity', 'u16')]),
    41: ("CMD_TOGGLE_PAUSE_ENTITY", 4, [('entity', 'u16')]),
    42: ("CMD_FLIP_ENTITY_DIRECTION", 4, [('entity', 'u16')]),
    43: ("CMD_PAUSE_ENTITIES", 4, [('pad', 'pad16')]),
    44: ("CMD_TOGGLE_PAUSE_ENTITIES", 4, [('pad', 'pad16')]),
    45: ("CMD_FLIP_ENTITY_ANIMATION", 4, [('pad', 'pad16')]),
    46: ("CMD_SET_ENTITY_NON_COLLIDABLE", 4, [('pad', 'pad16')]),
    47: ("CMD_SETUP_ENTITY", 8, [('entity', 'u16'), ('asset', 'u16'), ('variant', 'u16')]),
    48: ("CMD_SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG", 4, [('flag', 'u16')]),
    49: ("CMD_LOAD_MAP", 8, [('mapControllerIndex', 'u16'), ('mapIndex', 'u16'), ('pad', 'pad16')]),
    50: ("CMD_SET_ENTITY_MAP_SPACE_INDEPENDENT", 4, [('flag', 'u16')]),
    51: ("CMD_SET_RGBA", 8, [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('pad', 'pad16')]),
    52: ("CMD_UPDATE_RGBA", 8, [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('speed', 's16')]),
    53: ("CMD_UPDATE_U8_VALUE", 12, [('delta', 's16'), ('pad2', 'pad8'), ('max', 'u8'), ('pad3', 'pad16'), ('address', 'addr32')]),
    54: ("CMD_UPDATE_U16_VALUE", 12, [('delta', 's16'), ('max', 'u16'), ('pad', 'pad16'), ('address', 'addr32')]),
    55: ("CMD_UPDATE_U32_VALUE", 16, [('pad', 'pad16'), ('delta', 's32'), ('max', 's32'), ('address', 'addr32')]),
    56: ("CMD_DEACTIVATE_MAP_OBJECTS", 4, [('mapIndex', 'u16')]),
    57: ("CMD_UPDATE_GLOBAL_RGBA", 8, [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('speed', 's16')]),
    58: ("CMD_DEACTIVATE_SPRITES", 4, [('pad', 'pad16')]),
    59: ("CMD_DEACTIVATE_MAP_CONTROLLERS", 4, [('pad', 'pad16')]),
    60: ("CMD_WAIT_RGBA_FINISHED", 4, [('pad', 'pad16')]),
    61: ("CMD_CHECK_ENTITY_COLLISION", 8, [('entity', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]),
    62: ("CMD_INIT_DIALOGUE_SESSION", 8, [('box', 'u16'), ('bank', 'u16'), ('index', 'u16')]),
    63: ("CMD_WAIT_FOR_DIALOGUE_INPUT", 4, [('box', 'u16')]),
    64: ("CMD_BRANCH_ON_DIALOGUE", 12, [('dialogue_index', 'u16'), ('unk_17', 'u16'), ('pad', 'pad16'), ('target', 'rel16'), ('pad2', 'pad16')]),
    65: ("CMD_WAIT_ENTITY_ANIMATION", 4, [('pad', 'pad16')]),
    66: ("CMD_SET_MESSAGE_BOX_ASSETS", 8, [('spriteIndex', 'u16'), ('dialogueWindowIndex', 'u8'), ('overlayIconIndex', 'u8'), ('characterAvatarIndex', 'u8'), ('pad', 'pad8')]),
    67: ("CMD_SET_ENTITY_TRACKING_TARGET", 12, [('target_sprite', 'u16'), ('x', 's16'), ('y', 's16'), ('z', 's16'), ('tracking_mode', 'u8'), ('pad', 'pad8')]),
    68: ("CMD_SET_HOLDING_ANIMATION_FLAG", 4, [('useAltAnimations', 'u16')]),
    69: ("CMD_WAIT_MAP_LOAD", 4, [('map', 'u16')]),
    70: ("CMD_BRANCH_ON_ENTITY_DIR", 8, [('entity', 'u16'), ('direction', 'u8'), ('pad', 'pad8'), ('target', 'rel16')]),
    71: ("CMD_SET_ENTITY_PHYSICS_FLAGS", 4, [('flags', 'u16')]),
    72: ("CMD_SET_ENTITY_PALETTE", 4, [('palette', 'u16')]),
    73: ("CMD_SET_ENTITY_DIMENSIONS", 4, [('width', 'u8'), ('height', 'u8')]),
    74: ("CMD_SET_SHADOW_FLAGS", 8, [('entity', 'u16'), ('flags', 'u16'), ('pad', 'pad16')]),
    75: ("CMD_SET_SPRITE_SCALE", 8, [('x', 'u16'), ('y', 'u16'), ('z', 'u16')]),
    76: ("CMD_SET_SPRITE_RENDERING_LAYER", 4, [('layer', 'u16')]),
    77: ("CMD_INIT_MESSAGE_BOX_TYPE2", 8, [('box', 'u16'), ('bank', 'u16'), ('index', 'u16')]),
    78: ("CMD_INIT_MAP_ADDITION", 8, [('mapAdditionIndex', 'u16'), ('flag', 'u16'), ('pad', 'pad16')]),
    79: ("CMD_BRANCH_ON_RANDOM", 8, [('min', 'u16'), ('max', 'u16'), ('target', 'rel16')]),
    80: ("CMD_BRANCH_IF_U16_PTR_IN_RANGE", 20, [('pad', 'pad16'), ('ptr', 'addr32'), ('min_ptr', 'addr32'), ('max_ptr', 'addr32'), ('target', 'rel16'), ('pad2', 'pad16')]),
    81: ("CMD_PAUSE_EXECUTOR", 4, [('executor', 'u16')]),
    82: ("CMD_TOGGLE_PAUSE_EXECUTOR", 4, [('executor', 'u16')]),
    83: ("CMD_PAUSE_ALL_CHILD_EXECUTORS", 4, [('pad', 'pad16')]),
    84: ("CMD_TOGGLE_PAUSE_ALL_CHILDREN", 4, [('pad', 'pad16')]),
    85: ("CMD_SET_SPRITE_PALETTE", 4, [('palette', 'u16')]),
    86: ("CMD_BRANCH_IF_U8_PTR_RANGE", 20, [('pad1', 'pad16'), ('ptr', 'addr32'), ('min_ptr', 'addr32'), ('max_ptr', 'addr32'), ('target', 'rel16'), ('pad2', 'pad16')]),
    87: ("CMD_SET_AUDIO_SEQUENCE", 12, [('channel', 'u16'), ('start', 'rom_start32'), ('end', 'rom_end32')]),
    88: ("CMD_STOP_SEQUENCE_WITH_FADE", 8, [('channel', 'u16'), ('speed', 'u16'), ('pad', 'pad16')]),
    89: ("CMD_SET_AUDIO_SEQUENCE_VOLUME", 8, [('songIndex', 'u16'), ('targetVolume', 'u16'), ('volume', 's16')]),
    90: ("CMD_SET_SFX", 8, [('sfx', 'u16'), ('volume', 'u16'), ('pad', 'pad16')]),
    91: ("CMD_IDLE_WHILE_SEQUENCE_PLAYING", 4, [('channel', 'u16')]),
    92: ("CMD_UPDATE_MESSAGE_BOX_RGBA", 12, [('box', 'u16'), ('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('speed', 'u16'), ('pad', 'pad16')]),
    93: ("CMD_WAIT_MESSAGE_BOX_READY", 4, [('box', 'u16')]),
    94: ("CMD_SET_SPRITE_BILINEAR", 4, [('flag', 'u16')]),
    95: ("CMD_SET_MAP_ADDITION", 8, [('a', 'u16'), ('b', 'u8'), ('c', 'u8'), ('pad', 'pad16')]),
    96: ("CMD_SET_MAP_GROUND_OBJECT", 8, [('a', 'u16'), ('b', 'u16'), ('c', 'u16')]),
    97: ("CMD_SET_MESSAGE_INTERPOLATION", 8, [('rate', 's16'), ('message_box_index', 's16'), ('flag', 'u8'), ('pad', 'pad8')]),
}


# Build name-indexed COMMAND_SPECS_BY_NAME for transpiler use
COMMAND_SPECS_BY_NAME: Dict[str, Tuple[int, int, List[Tuple[str, str]]]] = {
    name: (opcode, size, params)
    for opcode, (name, size, params) in COMMAND_SPECS.items()
}

# Also index without CMD_ prefix
for opcode, (name, size, params) in COMMAND_SPECS.items():
    if name.startswith('CMD_'):
        COMMAND_SPECS_BY_NAME[name[4:]] = (opcode, size, params)

# Format: opcode -> (name, [types])
ASM_COMMAND_SPECS: Dict[int, Tuple[str, List[str]]] = {
    opcode: (name, [p[1] for p in params])
    for opcode, (name, size, params) in COMMAND_SPECS.items()
}
