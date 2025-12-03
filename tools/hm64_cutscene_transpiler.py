#!/usr/bin/env python3
"""
HM64 Cutscene DSL to GNU as Transpiler

Converts cutscene DSL source to GNU assembler syntax (.s files) that can be
assembled and linked with the main game. This allows:

Output is data directives (.byte, .half, .word) that preserve exact layout.
"""

import re
import sys
import argparse
from dataclasses import dataclass, field
from typing import List, Dict, Optional, Tuple, TextIO
from enum import IntEnum
from pathlib import Path


# =============================================================================
# OPCODE DEFINITIONS (from decompiled game)
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
    SET_ASSET_ROTATION = 26
    SETUP_MAP_ASSET = 27
    ENTITY_MOVE_AND_ANIMATE = 28
    SET_MAP_ROTATION = 29
    SET_CAMERA_TRACKING_FLAG = 30
    SET_MOVEMENT_INFO = 31
    INIT_MESSAGE_BOX_TYPE1 = 32
    WAIT_MESSAGE_BOX_CLOSED = 33
    SET_MESSAGE_BOX_VIEW_POSITION = 34
    RESET_MESSAGE_BOX_AVATAR = 35
    ENTITY_MOVE_AND_ANIMATE_2 = 36
    SET_ENTITY_ANIMATION = 37
    SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE = 38
    SET_CALLBACK_ROUTINE = 39
    PAUSE_ENTITY = 40
    TOGGLE_PAUSE_ENTITY = 41
    FLIP_ENTITY_DIRECTION = 42
    PAUSE_ENTITIES = 43
    TOGGLE_PAUSE_ENTITIES = 44
    FLIP_ENTITY_ANIMATION = 45
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
    WAIT_FOR_DIALOGUE_INPUT = 63
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


# =============================================================================
# COMMAND SPECSs
# =============================================================================
# Format: 'CMD_NAME': (opcode, total_size, [(param_name, param_type), ...])
#
# Parameter types:
#   'u8', 's8'      - 8-bit unsigned/signed
#   'u16', 's16'    - 16-bit unsigned/signed  
#   'u32', 's32'    - 32-bit unsigned/signed
#   'rel16'         - 16-bit relative offset (label - .)
#   'addr32'        - 32-bit absolute address (linker-resolved symbol)
#   'pad8', 'pad16' - Padding bytes (emit zeros)

COMMAND_SPECS: Dict[str, Tuple[int, int, List[Tuple[str, str]]]] = {
    # -------------------------------------------------------------------------
    # Animation pointer commands (4 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_ANIM_DATA_PTR_WITH_FLAG': (
        CutsceneOpcode.SET_ANIM_DATA_PTR_WITH_FLAG, 4,
        [('target', 'rel16')]
    ),
    'CMD_SET_ANIM_DATA_PTR': (
        CutsceneOpcode.SET_ANIM_DATA_PTR, 4,
        [('target', 'rel16')]
    ),
    
    # -------------------------------------------------------------------------
    # Simple commands (4 bytes, no params or single u16)
    # -------------------------------------------------------------------------
    'CMD_DEACTIVATE_SELF': (
        CutsceneOpcode.DEACTIVATE_SELF, 4,
        [('pad', 'pad16')]
    ),
    'CMD_RETURN_FROM_SUBROUTINE': (
        CutsceneOpcode.RETURN_FROM_SUBROUTINE, 4,
        [('pad', 'pad16')]
    ),
    'CMD_PAUSE_ENTITIES': (
        CutsceneOpcode.PAUSE_ENTITIES, 4,
        [('pad', 'pad16')]
    ),
    'CMD_TOGGLE_PAUSE_ENTITIES': (
        CutsceneOpcode.TOGGLE_PAUSE_ENTITIES, 4,
        [('pad', 'pad16')]
    ),
    'CMD_PAUSE_ALL_CHILD_EXECUTORS': (
        CutsceneOpcode.PAUSE_ALL_CHILD_EXECUTORS, 4,
        [('pad', 'pad16')]
    ),
    'CMD_TOGGLE_PAUSE_ALL_CHILDREN': (
        CutsceneOpcode.TOGGLE_PAUSE_ALL_CHILDREN, 4,
        [('pad', 'pad16')]
    ),
    'CMD_WAIT_RGBA_FINISHED': (
        CutsceneOpcode.WAIT_RGBA_FINISHED, 4,
        [('pad', 'pad16')]
    ),
    'CMD_WAIT_ENTITY_ANIMATION': (
        CutsceneOpcode.WAIT_ENTITY_ANIMATION, 4,
        [('pad', 'pad16')]
    ),
    'CMD_DEACTIVATE_MAP_OBJECTS': (
        CutsceneOpcode.DEACTIVATE_MAP_OBJECTS, 4,
        [('pad', 'pad16')]
    ),
    'CMD_DEACTIVATE_SPRITES': (
        CutsceneOpcode.DEACTIVATE_SPRITES, 4,
        [('pad', 'pad16')]
    ),
    'CMD_DEACTIVATE_MAP_CONTROLLERS': (
        CutsceneOpcode.DEACTIVATE_MAP_CONTROLLERS, 4,
        [('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Single u16 parameter commands (4 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_WAIT_FRAMES': (
        CutsceneOpcode.SET_WAIT_FRAMES, 4,
        [('frames', 'u16')]
    ),
    'SET_CAMERA_TRACKING_FLAG': (
        CutsceneOpcode.SET_CAMERA_TRACKING_FLAG, 4,
        [('flag', 'u8'), ('pad', 'pad8')]
    ),
    'CMD_SET_ENTITY_ANIMATION': (
        CutsceneOpcode.SET_ENTITY_ANIMATION, 4,
        [('anim', 'u16')]
    ),
    'CMD_SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE': (
        CutsceneOpcode.SET_ENTITY_ANIMATION_WITH_DIRECTION_CHANGE, 4,
        [('anim', 'u16')]
    ),
    'CMD_PAUSE_ENTITY': (
        CutsceneOpcode.PAUSE_ENTITY, 4,
        [('entity', 'u16')]
    ),
    'CMD_TOGGLE_PAUSE_ENTITY': (
        CutsceneOpcode.TOGGLE_PAUSE_ENTITY, 4,
        [('entity', 'u16')]
    ),
    'CMD_FLIP_ENTITY_DIRECTION': (
        CutsceneOpcode.FLIP_ENTITY_DIRECTION, 4,
        [('entity', 'u16')]
    ),
    'CMD_FLIP_ENTITY_ANIMATION': (
        CutsceneOpcode.FLIP_ENTITY_ANIMATION, 4,
        [('_pad', 'pad16')]
    ),
    'CMD_SET_ENTITY_NON_COLLIDABLE': (
        CutsceneOpcode.SET_ENTITY_NON_COLLIDABLE, 4,
        [('_pad', 'pad16')]
    ),
    'CMD_SET_ASSET_ROTATION': (
        CutsceneOpcode.SET_ASSET_ROTATION, 4,
        [('direction', 'u8'), ('_pad', 'pad8')]
    ),
    'CMD_SET_ENTITY_PHYSICS_FLAGS': (
        CutsceneOpcode.SET_ENTITY_PHYSICS_FLAGS, 4,
        [('flags', 'u16')]
    ),
    'CMD_SET_SPRITE_PALETTE': (
        CutsceneOpcode.SET_SPRITE_PALETTE, 4,
        [('palette', 'u16')]
    ),
    'CMD_SET_SPRITE_RENDERING_LAYER': (
        CutsceneOpcode.SET_SPRITE_RENDERING_LAYER, 4,
        [('layer', 'u16')]
    ),
    'CMD_SET_SPRITE_BILINEAR': (
        CutsceneOpcode.SET_SPRITE_BILINEAR, 4,
        [('flag', 'u16')]
    ),
    'CMD_DEACTIVATE_EXECUTOR': (
        CutsceneOpcode.DEACTIVATE_EXECUTOR, 4,
        [('executor', 'u16')]
    ),
    'CMD_PAUSE_EXECUTOR': (
        CutsceneOpcode.PAUSE_EXECUTOR, 4,
        [('executor', 'u16')]
    ),
    'CMD_TOGGLE_PAUSE_EXECUTOR': (
        CutsceneOpcode.TOGGLE_PAUSE_EXECUTOR, 4,
        [('executor', 'u16')]
    ),
    'CMD_WAIT_MESSAGE_BOX_CLOSED': (
        CutsceneOpcode.WAIT_MESSAGE_BOX_CLOSED, 4,
        [('box', 'u16')]
    ),
    'CMD_RESET_MESSAGE_BOX_AVATAR': (
        CutsceneOpcode.RESET_MESSAGE_BOX_AVATAR, 4,
        [('box', 'u16')]
    ),
    'CMD_WAIT_FOR_DIALOGUE_INPUT': (
        CutsceneOpcode.WAIT_FOR_DIALOGUE_INPUT, 4,
        [('box', 'u16')]
    ),
    'CMD_WAIT_MESSAGE_BOX_READY': (
        CutsceneOpcode.WAIT_MESSAGE_BOX_READY, 4,
        [('box', 'u16')]
    ),
    'CMD_IDLE_WHILE_SONG_PLAYING': (
        CutsceneOpcode.IDLE_WHILE_SONG_PLAYING, 4,
        [('channel', 'u16')]
    ),
    'CMD_WAIT_MAP_LOAD': (
        CutsceneOpcode.WAIT_MAP_LOAD, 4,
        [('map', 'u16')]
    ),
    'CMD_LOAD_MAP': (
        CutsceneOpcode.LOAD_MAP, 4,
        [('map', 'u16')]
    ),
    'CMD_SET_FRAME_DELTA': (
        CutsceneOpcode.SET_FRAME_DELTA, 4,
        [('delta', 's16')]
    ),
    
    # -------------------------------------------------------------------------
    # Subroutine/branch with relative offset (4 bytes)
    # -------------------------------------------------------------------------
    'CMD_EXECUTE_SUBROUTINE': (
        CutsceneOpcode.EXECUTE_SUBROUTINE, 4,
        [('target', 'rel16')]
    ),
    
    # -------------------------------------------------------------------------
    # Spawn/set executor pointer (8 bytes)
    # Format: opcode(2) + executor_index(2) + relative_offset(2) + pad(2)
    # -------------------------------------------------------------------------
    'CMD_SPAWN_EXECUTOR': (
        CutsceneOpcode.SPAWN_EXECUTOR, 8,
        [('executor', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    'CMD_SET_OTHER_EXECUTOR_PTR': (
        CutsceneOpcode.SET_OTHER_EXECUTOR_PTR, 8,
        [('executor', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Coordinates (8 bytes)
    # Format: opcode(2) + x(2) + y(2) + z(2)
    # -------------------------------------------------------------------------
    'CMD_SET_COORDINATES': (
        CutsceneOpcode.SET_COORDINATES, 8,
        [('x', 's16'), ('y', 's16'), ('z', 's16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity setup (8 bytes)
    # Format: opcode(2) + entity(2) + asset(2) + variant(2)
    # -------------------------------------------------------------------------
    'CMD_SETUP_ENTITY': (
        CutsceneOpcode.SETUP_ENTITY, 8,
        [('entity', 'u16'), ('asset', 'u16'), ('variant', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Set special bit (8 bytes)
    # Format: opcode(2) + bit_index(2) + address(4)
    # -------------------------------------------------------------------------
    'CMD_SET_SPECIAL_BIT': (
        CutsceneOpcode.SET_SPECIAL_BIT, 8,
        [('bit', 'u16'), ('address', 'addr32')]
    ),
    'CMD_TOGGLE_SPECIAL_BIT': (
        CutsceneOpcode.TOGGLE_SPECIAL_BIT, 8,
        [('bit', 'u16'), ('address', 'addr32')]
    ),
    
    # -------------------------------------------------------------------------
    # Callback routine (8 bytes)
    # Format: opcode(2) + entity(2) + button(2) + relative_offset(2)
    # -------------------------------------------------------------------------
    'CMD_SET_CALLBACK_ROUTINE': (
        CutsceneOpcode.SET_CALLBACK_ROUTINE, 8,
        [('entity', 'u16'), ('button', 'u16'), ('target', 'rel16')]
    ),
    
    # -------------------------------------------------------------------------
    # Message box init (8 bytes)
    # Format: opcode(2) + box(2) + bank(2) + index(2)
    # -------------------------------------------------------------------------
    'CMD_INIT_MESSAGE_BOX_TYPE1': (
        CutsceneOpcode.INIT_MESSAGE_BOX_TYPE1, 8,
        [('box', 'u16'), ('bank', 'u16'), ('index', 'u16')]
    ),
    'CMD_INIT_MESSAGE_BOX_TYPE2': (
        CutsceneOpcode.INIT_MESSAGE_BOX_TYPE2, 8,
        [('box', 'u16'), ('bank', 'u16'), ('index', 'u16')]
    ),
    'CMD_INIT_DIALOGUE_SESSION': (
        CutsceneOpcode.INIT_DIALOGUE_SESSION, 8,
        [('box', 'u16'), ('bank', 'u16'), ('index', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Message box assets (8 bytes)
    # Format: opcode(2) + a(2) + b(2) + c(2)
    # -------------------------------------------------------------------------
    'CMD_SET_MESSAGE_BOX_ASSETS': (
        CutsceneOpcode.SET_MESSAGE_BOX_ASSETS, 8,
        [('spriteIndex', 'u16'), ('dialogueWindowIndex', 'u8'), ('overlayIconIndex', 'u8'), ('characterAvatarIndex', 'u8'), ('_pad', 'pad8')]
    ),
    
    # -------------------------------------------------------------------------
    # Message interpolation (8 bytes)
    # Format: opcode(2) + a(2) + b(2) + c(2)
    # -------------------------------------------------------------------------
    'CMD_SET_MESSAGE_INTERPOLATION': (
        CutsceneOpcode.SET_MESSAGE_INTERPOLATION, 8,
        [('a', 's16'), ('b', 's16'), ('c', 's16')]
    ),
    
    # -------------------------------------------------------------------------
    # RGBA commands (8 bytes)
    # Format: opcode(2) + r(1) + g(1) + b(1) + a(1) + speed(1) + pad(1)
    # -------------------------------------------------------------------------
    'CMD_SET_RGBA': (
        CutsceneOpcode.SET_RGBA, 8,
        [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('pad', 'pad16')]
    ),
    'CMD_UPDATE_RGBA': (
        CutsceneOpcode.UPDATE_RGBA, 8,
        [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('speed', 's16')]
    ),
    'CMD_UPDATE_GLOBAL_RGBA': (
        CutsceneOpcode.UPDATE_GLOBAL_RGBA, 8,
        [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('speed', 's16')]
    ),
    'CMD_UPDATE_MESSAGE_BOX_RGBA': (
        CutsceneOpcode.UPDATE_MESSAGE_BOX_RGBA, 8,
        [('r', 'u8'), ('g', 'u8'), ('b', 'u8'), ('a', 'u8'), ('speed', 'u8'), ('pad', 'pad8')]
    ),
    
    # -------------------------------------------------------------------------
    # Audio commands (8 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_AUDIO_SEQUENCE_VOLUME': (
        CutsceneOpcode.SET_AUDIO_SEQUENCE_VOLUME, 8,
        [('songIndex', 'u16'), ('targetVolume', 'u16'), ('volume', 's16')]
    ),
    'CMD_SET_SFX': (
        CutsceneOpcode.SET_SFX, 8,
        [('sfx', 'u16'), ('volume', 'u16'), ('pad1', 'pad16')]
    ),
    'CMD_STOP_SONG_WITH_FADE': (
        CutsceneOpcode.STOP_SONG_WITH_FADE, 8,
        [('channel', 'u16'), ('speed', 'u16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity move and animate (8 bytes)
    # Format: opcode(2) + distance(2) + pad(2) + direction(1) + speed(1)
    # -------------------------------------------------------------------------
    'CMD_ENTITY_MOVE_AND_ANIMATE': (
        CutsceneOpcode.ENTITY_MOVE_AND_ANIMATE, 8,
        [('distance', 'u16'), ('direction', 'u8'), ('speed', 'u8'), ('pad', 'pad16')]
    ),
    'CMD_ENTITY_MOVE_AND_ANIMATE_2': (
        CutsceneOpcode.ENTITY_MOVE_AND_ANIMATE_2, 8,
        [('distance', 'u16'), ('direction', 'u8'), ('speed', 'u8'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Sprite scale (8 bytes)
    # Format: opcode(2) + x(2) + y(2) + z(2)
    # -------------------------------------------------------------------------
    'CMD_SET_SPRITE_SCALE': (
        CutsceneOpcode.SET_SPRITE_SCALE, 8,
        [('x', 'u16'), ('y', 'u16'), ('z', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Set value commands (8 bytes)
    # Format: opcode(2) + pad(2) + address(4), value in next word varies
    # -------------------------------------------------------------------------
    'CMD_SET_U8_VALUE': (
        CutsceneOpcode.SET_U8_VALUE, 8,
        [('value', 'u8'), ('pad', 'pad8'), ('address', 'addr32')]
    ),
    'CMD_SET_U16_VALUE': (
        CutsceneOpcode.SET_U16_VALUE, 8,
        [('value', 'u16'), ('address', 'addr32')]
    ),
    'CMD_SET_U32_VALUE': (
        CutsceneOpcode.SET_U32_VALUE, 12,
        [('pad', 'pad16'), ('address', 'addr32'), ('value', 'u32')]
    ),
    
    # -------------------------------------------------------------------------
    # Update value commands (8-12 bytes)
    # -------------------------------------------------------------------------
    'CMD_UPDATE_U8_VALUE': (
        CutsceneOpcode.UPDATE_U8_VALUE, 12,
        [('_pad1', 'pad8'), ('delta', 's8'), ('_pad2', 'pad8'), ('max', 'u8'), ('_pad3', 'pad16'), ('address', 'addr32')]
    ),
    'CMD_UPDATE_U16_VALUE': (
        CutsceneOpcode.UPDATE_U16_VALUE, 12,
        [('delta', 's16'), ('max', 'u16'), ('pad', 'pad16'), ('address', 'addr32')]
    ),
    'CMD_UPDATE_U32_VALUE': (
        CutsceneOpcode.UPDATE_U32_VALUE, 16,
        [('pad', 'pad16'), ('delta', 's32'), ('max', 's32'), ('address', 'addr32')]
    ),
    
    # -------------------------------------------------------------------------
    # Message box view position (12 bytes)
    # Format: opcode(2) + box(2) + x(2) + y(2) + z(2) + pad(2)
    # -------------------------------------------------------------------------
    'CMD_SET_MESSAGE_BOX_VIEW_POSITION': (
        CutsceneOpcode.SET_MESSAGE_BOX_VIEW_POSITION, 12,
        [('box', 'u16'), ('x', 's16'), ('y', 's16'), ('z', 's16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Branch on special bit (12 bytes)
    # Format: opcode(2) + bit(2) + address(4) + offset(2) + pad(2)
    # -------------------------------------------------------------------------
    'CMD_BRANCH_ON_SPECIAL_BIT': (
        CutsceneOpcode.BRANCH_ON_SPECIAL_BIT, 12,
        [('bit', 'u16'), ('address', 'addr32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Audio set (12 bytes)
    # Format: opcode(2) + channel(2) + start(4) + end(4) - but we'll simplify
    # -------------------------------------------------------------------------
    'CMD_SET_AUDIO_SEQUENCE': (
        CutsceneOpcode.SET_AUDIO_SEQUENCE, 12,
        [('channel', 'u16'), ('start', 'u32'), ('end', 'u32')]
    ),
    
    # -------------------------------------------------------------------------
    # Branch commands (12-16 bytes)
    # -------------------------------------------------------------------------
    'CMD_BRANCH_U8_VAR_WITHIN_RANGE': (
        CutsceneOpcode.BRANCH_U8_VAR_WITHIN_RANGE, 12,
        [('min', 'u8'), ('max', 'u8'), ('address', 'addr32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    'CMD_BRANCH_U16_VAR_WITHIN_RANGE': (
        CutsceneOpcode.BRANCH_U16_VAR_WITHIN_RANGE, 16,
        [('min', 'u16'), ('max', 'u16'), ('pad', 'pad16'), ('address', 'addr32'), ('target', 'rel16'), ('pad2', 'pad16')]
    ),
    'CMD_BRANCH_U32_VAR_WITHIN_RANGE': (
        CutsceneOpcode.BRANCH_U32_VAR_WITHIN_RANGE, 20,
        [('min', 'u32'), ('max', 'u32'), ('address', 'addr32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    'CMD_BRANCH_ON_RANDOM': (
        CutsceneOpcode.BRANCH_ON_RANDOM, 8,
        [('chance', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    'CMD_BRANCH_ON_ENTITY_DIR': (
        CutsceneOpcode.BRANCH_ON_ENTITY_DIR, 8,
        [('entity', 'u16'), ('direction', 'u8'), ('pad', 'pad8'), ('target', 'rel16')]
    ),
    'CMD_BRANCH_ON_DIALOGUE': (
        CutsceneOpcode.BRANCH_ON_DIALOGUE, 8,
        [('choice', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Button branch commands (12 bytes)
    # Format: opcode(2) + entity(2) + button(4) + offset(2) + pad(2)
    # -------------------------------------------------------------------------
    'CMD_BRANCH_ON_BUTTON_PRESSED': (
        CutsceneOpcode.BRANCH_ON_BUTTON_PRESSED, 12,
        [('entity', 'u16'), ('button', 'u32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    'CMD_BRANCH_ON_BUTTON_REPEAT': (
        CutsceneOpcode.BRANCH_ON_BUTTON_REPEAT, 12,
        [('entity', 'u16'), ('button', 'u32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    'CMD_BRANCH_ON_CURRENT_BUTTON': (
        CutsceneOpcode.BRANCH_ON_CURRENT_BUTTON, 12,
        [('entity', 'u16'), ('button', 'u32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity animations (16 bytes)
    # Format: opcode(2) + entity(2) + anim0-5(2 each) = 14 + pad(2)
    # -------------------------------------------------------------------------
    'CMD_SET_ENTITY_ANIMATIONS': (
        CutsceneOpcode.SET_ENTITY_ANIMATIONS, 16,
        [('entity', 'u16'), ('a0', 'u16'), ('a1', 'u16'), ('a2', 'u16'), 
         ('a3', 'u16'), ('a4', 'u16'), ('a5', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Movement info (16 bytes) - complex, placeholder
    # -------------------------------------------------------------------------
    'CMD_SET_MOVEMENT_INFO': (
        CutsceneOpcode.SET_MOVEMENT_INFO, 16,
        [('a', 'u16'), ('b', 'u16'), ('c', 'u16'), ('d', 'u16'),
         ('e', 'u16'), ('f', 'u16'), ('g', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity tracking (8 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_ENTITY_TRACKING_TARGET': (
        CutsceneOpcode.SET_ENTITY_TRACKING_TARGET, 12,
        [('target_sprite', 'u16'), ('x', 's16'), ('y', 's16'), ('z', 's16'), ('tracking_mode', 'u8'), ('_pad', 'pad8')]
    ),
    
    # -------------------------------------------------------------------------
    # Map asset setup (variable, placeholder for now at 8 bytes)
    # -------------------------------------------------------------------------
    'CMD_SETUP_MAP_ASSET': (
        CutsceneOpcode.SETUP_MAP_ASSET, 8,
        [('asset', 'u16'), ('param', 'u16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity dimensions (8 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_ENTITY_DIMENSIONS': (
        CutsceneOpcode.SET_ENTITY_DIMENSIONS, 8,
        [('entity', 'u16'), ('width', 'u16'), ('height', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity palette (4 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_ENTITY_PALETTE': (
        CutsceneOpcode.SET_ENTITY_PALETTE, 4,
        [('palette', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Shadow flags (8 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_SHADOW_FLAGS': (
        CutsceneOpcode.SET_SHADOW_FLAGS, 8,
        [('entity', 'u16'), ('flags', 'u16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Map rotation (4 bytes)
    # -------------------------------------------------------------------------
    'CMD_SET_MAP_ROTATION': (
        CutsceneOpcode.SET_MAP_ROTATION, 4,
        [('rotation', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Check entity collision (8 bytes)
    # -------------------------------------------------------------------------
    'CMD_CHECK_ENTITY_COLLISION': (
        CutsceneOpcode.CHECK_ENTITY_COLLISION, 8,
        [('entity', 'u16'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Update camera flags (4 bytes)
    # -------------------------------------------------------------------------
    'CMD_UPDATE_CAMERA_FLAGS': (
        CutsceneOpcode.UPDATE_CAMERA_FLAGS, 4,
        [('flags', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Map additions (8 bytes)
    # -------------------------------------------------------------------------
    'CMD_INIT_MAP_ADDITION': (
        CutsceneOpcode.INIT_MAP_ADDITION, 8,
        [('a', 'u16'), ('b', 'u16'), ('c', 'u16')]
    ),
    'CMD_SET_MAP_ADDITION': (
        CutsceneOpcode.SET_MAP_ADDITION, 8,
        [('a', 'u16'), ('b', 'u16'), ('c', 'u16')]
    ),
    'CMD_SET_MAP_GROUND_OBJECT': (
        CutsceneOpcode.SET_MAP_GROUND_OBJECT, 8,
        [('a', 'u16'), ('b', 'u16'), ('c', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # Pointer range branches (12 bytes)
    # -------------------------------------------------------------------------
    'CMD_BRANCH_ON_U16_PTR_RANGE': (
        CutsceneOpcode.BRANCH_ON_U16_PTR_RANGE, 12,
        [('min', 'u16'), ('max', 'u16'), ('ptr', 'addr32'), ('target', 'rel16')]
    ),
    'CMD_BRANCH_ON_U8_PTR_RANGE': (
        CutsceneOpcode.BRANCH_ON_U8_PTR_RANGE, 12,
        [('min', 'u8'), ('max', 'u8'), ('ptr', 'addr32'), ('target', 'rel16'), ('pad', 'pad16')]
    ),
    
    # -------------------------------------------------------------------------
    # Entity flags (4 bytes each)  
    # These use the actual enum values from the game
    # -------------------------------------------------------------------------
    'CMD_SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG': (
        CutsceneOpcode.SET_ENTITY_MAP_SPACE_INDEPENDENT_FLAG, 4,
        [('flag', 'u16')]
    ),
    'CMD_SET_ENTITY_MAP_SPACE_INDEPENDENT': (
        CutsceneOpcode.SET_ENTITY_MAP_SPACE_INDEPENDENT, 4,
        [('flag', 'u16')]
    ),
    
    # -------------------------------------------------------------------------
    # DMA_SPRITE (56 bytes) - complex sprite DMA command
    # opcode(2) + sprite(2) + asset_type(2) + pad(2) + 12 x u32 addresses
    # -------------------------------------------------------------------------
    'CMD_DMA_SPRITE': (
        CutsceneOpcode.DMA_SPRITE, 56,
        [('sprite', 'u16'), ('asset_type', 'u16'), ('pad', 'pad16'),
         ('rom_tex_start', 'addr32'), ('rom_tex_end', 'addr32'),
         ('rom_assets_start', 'addr32'), ('rom_assets_end', 'addr32'),
         ('rom_sheet_start', 'addr32'), ('rom_sheet_end', 'addr32'),
         ('tex1_vaddr', 'addr32'), ('tex2_vaddr', 'addr32'),
         ('palette_vaddr', 'addr32'), ('anim_vaddr', 'addr32'),
         ('s2p_vaddr', 'addr32'), ('sheet_idx_vaddr', 'addr32')]
    ),
    
}

# =============================================================================
# BUILT-IN CONSTANTS
# =============================================================================

BUILTIN_CONSTANTS = {

    'ENTITY_PLAYER': 0,
    
    'SOUTHWEST': 0, 
    'WEST': 1, 
    'NORTHWEST': 2, 
    'NORTH': 3, 
    'NORTHEAST': 4, 
    'EAST': 5, 
    'SOUTHEAST': 6, 
    'SOUTH': 7, 

    "BUTTON_C_RIGHT": 0x0001,
    "BUTTON_C_LEFT": 0x0002,
    "BUTTON_C_DOWN": 0x0004,
    "BUTTON_C_UP": 0x0008,
    "BUTTON_R": 0x0010,
    "BUTTON_L": 0x0020,
    "BUTTON_D_RIGHT": 0x0100,
    "BUTTON_D_LEFT": 0x0200,
    "BUTTON_D_DOWN": 0x0400,
    "BUTTON_D_UP": 0x0800,
    "BUTTON_START": 0x1000,
    "BUTTON_Z": 0x2000,
    "BUTTON_B": 0x4000,
    "BUTTON_A": 0x8000,
    "BUTTON_STICK_UP": 0x10000,
    "BUTTON_STICK_DOWN": 0x20000,
    "BUTTON_STICK_LEFT": 0x40000,
    "BUTTON_STICK_RIGHT": 0x80000,

    'ENTITY_ASSET_PLAYER': 0,
    'ENTITY_ASSET_MARIA': 1,
    'ENTITY_ASSET_POPURI': 2,
    'ENTITY_ASSET_ELLI': 3,
    'ENTITY_ASSET_ANN': 4,
    'ENTITY_ASSET_KAREN': 5,
    'ENTITY_ASSET_BABY': 6,
    'ENTITY_ASSET_HARRIS': 7,
    'ENTITY_ASSET_GRAY': 8,
    'ENTITY_ASSET_JEFF': 9,
    'ENTITY_ASSET_CLIFF': 10,
    'ENTITY_ASSET_KAI': 11,
    'ENTITY_ASSET_MAYOR': 12,
    'ENTITY_ASSET_MAYOR_WIFE': 13,
    'ENTITY_ASSET_LILIA': 14,
    'ENTITY_ASSET_BASIL': 15,
    'ENTITY_ASSET_ELLEN': 16,
    'ENTITY_ASSET_DOUG': 17,
    'ENTITY_ASSET_GOTZ': 18,
    'ENTITY_ASSET_SASHA': 19,
    'ENTITY_ASSET_POTION_SHOP_DEALER': 20,
    'ENTITY_ASSET_KENT': 21,
    'ENTITY_ASSET_STU': 22,
    'ENTITY_ASSET_MIDWIFE': 23,
    'ENTITY_ASSET_MAY': 24,
    'ENTITY_ASSET_RICK': 25,
    'ENTITY_ASSET_PASTOR': 26,
    'ENTITY_ASSET_SHIPPER': 27,
    'ENTITY_ASSET_SAIBARA': 28,
    'ENTITY_ASSET_DUKE': 29,
    'ENTITY_ASSET_GREG': 30,
    'ENTITY_ASSET_ASSISTANT_CARPENTER': 31,
    'ENTITY_ASSET_MASTER_CARPENTER': 32,
    'ENTITY_ASSET_HARVEST_SPRITE': 33,
    'ENTITY_ASSET_BARLEY': 34,
    'ENTITY_ASSET_SYDNEY': 35,
    'ENTITY_ASSET_ENTOMOLOGIST': 36,
    'ENTITY_ASSET_GOURMET_JUDGE': 37,
    'ENTITY_ASSET_PHOTOGRAPHER': 38,
    'ENTITY_ASSET_SHADY_SALESMAN': 39,
    'ENTITY_ASSET_MRS_MANA_SON': 40,
    'ENTITY_ASSET_MRS_MANA': 41,
    'ENTITY_ASSET_OLD_MAN': 42,
    'ENTITY_ASSET_OLD_WOMAN': 43,
    'ENTITY_ASSET_TOURIST_COUPLE_MAN': 44,
    'ENTITY_ASSET_TOURIST_COUPLE_WOMAN': 45,
    'ENTITY_ASSET_FESTIVAL_GIRL1': 46,
    'ENTITY_ASSET_FESTIVAL_GIRL2': 47,
    'ENTITY_ASSET_FESTIVAL_GIRL3': 48,
    'ENTITY_ASSET_HARVEST_GODDESS': 49,
    'ENTITY_ASSET_SPIRIT_FESTIVAL_OCARINA_PERFORMER': 50,
    'ENTITY_ASSET_KAPPA': 51,
    'ENTITY_ASSET_PRINCESS': 52,
    'ENTITY_ASSET_EMPTY_NPC_SPRITE': 53,
    'ENTITY_ASSET_BUNNY': 54,
    'ENTITY_ASSET_MOUSE': 55,
    'ENTITY_ASSET_FOX': 56,
    'ENTITY_ASSET_MONKEY': 57,
    'ENTITY_ASSET_HAWK': 58,
    'ENTITY_ASSET_MOLE': 59,
    'ENTITY_ASSET_SQUIRREL': 60,
    'ENTITY_ASSET_DOG_RACE': 61,
    'ENTITY_ASSET_CAT': 62,
    'ENTITY_ASSET_DOG': 63,
    'ENTITY_ASSET_DOG_TITLE': 64,
    'ENTITY_ASSET_HORSE_PONY': 65,
    'ENTITY_ASSET_HORSE_GROWN': 66,
    'ENTITY_ASSET_CHICK': 67,
    'ENTITY_ASSET_CHICKEN': 68,
    'ENTITY_ASSET_CALF': 69,
    'ENTITY_ASSET_YOUNG_COW': 70,
    'ENTITY_ASSET_COW': 71,
    'ENTITY_ASSET_LAMB': 72,
    'ENTITY_ASSET_SHEEP': 73,
    'ENTITY_ASSET_FROG': 74,
    'ENTITY_ASSET_CRAB': 75,
    'ENTITY_ASSET_HORNED_BEETLE': 76,
    'ENTITY_ASSET_STAG_BEETLE': 77,
    'ENTITY_ASSET_BELL_RING_CRICKET': 78,
    'ENTITY_ASSET_CICADA': 79,
    'ENTITY_ASSET_BIRD': 80,
    'ENTITY_ASSET_LADYBUG': 81,
    'ENTITY_ASSET_DRAGONFLY': 82,
    'ENTITY_ASSET_GRASSHOPPER': 83,
    'ENTITY_ASSET_SNAKE': 84,
    'ENTITY_ASSET_PURPLE_BUTTERFLY': 85,
    'ENTITY_ASSET_DAD': 86,
    'ENTITY_ASSET_COW_INDOORS': 87,
    'ENTITY_ASSET_HORSE_UNBRIDLED': 88,
    'ENTITY_ASSET_HORSE_RACER': 89,
    'ENTITY_ASSET_EMPTY_ENTITY': 90,
    'ENTITY_ASSET_HOLDABLE_ITEMS_1': 91,
    'ENTITY_ASSET_EXTRA_FESTIVAL_ITEMS': 92,
    'ENTITY_ASSET_HOLDABLE_ITEMS_2': 93,
    'ENTITY_ASSET_HOLDABLE_ITEMS_3': 94,
    'ENTITY_ASSET_TOOLS': 95,
    'ENTITY_ASSET_HOLDABLE_ITEMS_4': 96,
    'ENTITY_ASSET_MARIA_HARRIS_BABY': 97,
    'ENTITY_ASSET_POPURI_GRAY_BABY': 98,
    'ENTITY_ASSET_ELLI_JEFF_BABY': 99,
    'ENTITY_ASSET_ANN_CLIFF_BABY': 100,
    'ENTITY_ASSET_KAREN_KAI_BABY': 101,
    
    'RANCH_SPRING': 0,
    'RANCH_SUMMER': 1,
    'RANCH_AUTUMN': 2,
    'RANCH_WINTER': 3,
    'ANN_ROOM': 4,
    'RANCH_BARN': 5,
    'RANCH_STORE': 6,
    'RANCH_HOUSE': 7,
    'EMPTY_1': 8,
    'BEACH_SPRING': 9,
    'BEACH_SUMMER': 0xA,
    'BEACH_AUTUMN': 0xB,
    'BEACH_WINTER': 0xC,
    'RACE_TRACK_SPRING': 0xD,
    'RACE_TRACK_SUMMER': 0xE,
    'RACE_TRACK_AUTUMN': 0xF,
    'RACE_TRACK_WINTER': 0x10,
    'MOUNTAIN_1_SPRING': 0x11,
    'MOUNTAIN_1_SUMMER': 0x12,
    'MOUNTAIN_1_AUTUMN': 0x13,
    'MOUNTAIN_1_WINTER': 0x14,
    'MOUNTAIN_2_SPRING': 0x15,
    'MOUNTAIN_2_SUMMER': 0x16,
    'MOUNTAIN_2_AUTUMN': 0x17,
    'MOUNTAIN_2_WINTER': 0x18,
    'TOP_OF_MOUNTAIN_1_SPRING': 0x19,
    'TOP_OF_MOUNTAIN_1_SUMMER': 0x1A,
    'TOP_OF_MOUNTAIN_1_AUTUMN': 0x1B,
    'TOP_OF_MOUNTAIN_1_WINTER': 0x1C,
    'MOON_MOUNTAIN_SPRING': 0x1D,
    'MOON_MOUNTAIN_SUMMER': 0x1E,
    'MOON_MOUNTAIN_AUTUMN': 0x1F,
    'MOON_MOUNTAIN_WINTER': 0x20,
    'CARPENTER_HUT': 0x21,
    'DUMPLING_HOUSE': 0x22,
    'POND_SPRING': 0x23,
    'POND_SUMMER': 0x24,
    'POND_AUTUMN': 0x25,
    'POND_WINTER': 0x26,
    'ELLI_ROOM': 0x27,
    'BAKERY': 0x28,
    'VILLAGE_1_SPRING': 0x29,
    'VILLAGE_1_SUMMER': 0x2A,
    'VILLAGE_1_AUTUMN': 0x2B,
    'VILLAGE_1_WINTER': 0x2C,
    'VILLAGE_2_SPRING': 0x2D,
    'VILLAGE_2_SUMMER': 0x2E,
    'VILLAGE_2_AUTUMN': 0x2F,
    'VILLAGE_2_WINTER': 0x30,
    'POPURI_ROOM': 0x31,
    'FLOWER_SHOP': 0x32,
    'CHURCH': 0x33,
    'SOUVENIR_SHOP': 0x34,
    'SQUARE_SPRING': 0x35,
    'SQUARE_SUMMER': 0x36,
    'SQUARE_AUTUMN': 0x37,
    'SQUARE_WINTER': 0x38,
    'RICK_STORE': 0x39,
    'MIDWIFE_HOUSE': 0x3A,
    'TAVERN': 0x3B,
    'LIBRARY': 0x3C,
    'MARIA_ROOM': 0x3D,
    'MAYOR_HOUSE': 0x3E,
    'POTION_SHOP_BEDROOM': 0x3F,
    'POTION_SHOP': 0x40,
    'EMPTY_2': 0x41,
    'HARVEST_SPRITE_CAVE': 0x42,
    'CAVE': 0x43,
    'MINE': 0x44,
    'MINE_2': 0x45,
    'KAREN_ROOM': 0x46,
    'VINEYARD_SPRING': 0x47,
    'VINEYARD_SUMMER': 0x48,
    'VINEYARD_AUTUMN': 0x49,
    'VINEYARD_WINTER': 0x4A,
    'VINEYARD_HOUSE': 0x4B,
    'VINEYARD_CELLAR': 0x4C,
    'VINEYARD_CELLAR_BASEMENT': 0x4D,
    'ROAD_SPRING': 0x4E,
    'ROAD_SUMMER': 0x4F,
    'ROAD_AUTUMN': 0x50,
    'ROAD_WINTER': 0x51,
    'FARM_SPRING': 0x52,
    'FARM_SUMMER': 0x53,
    'FARM_AUTUMN': 0x54,
    'FARM_WINTER': 0x55,
    'GREENHOUSE': 0x56,
    'HOUSE': 0x57,
    'BARN': 0x58,
    'COOP': 0x59,
    'KITCHEN': 0x5A,
    'BATHROOM': 0x5B,
    
    'TEXT_DIALOGUE_0': 0,
    'TEXT_LIBRARY': 1,
    'TEXT_DIARY': 2,
    'TEXT_DIALOGUE_3': 3,
    'TEXT_FESTIVAL_OVERLAY_SELECTIONS': 4,
    'TEXT_LETTERS': 5,
    'TEXT_SHOP': 6,
    'TEXT_ANIMAL_INTERACTIONS': 7,
    'TEXT_TV': 8,
    'TEXT_DIALOGUE_9': 9,
    'TEXT_NAMING_SCREEN': 10,
    'TEXT_ELLI': 11,
    'TEXT_KAI': 12,
    'TEXT_KAREN': 13,
    'TEXT_GOTZ': 14,
    'TEXT_SASHA': 15,
    'TEXT_CLIFF': 16,
    'TEXT_JEFF': 17,
    'TEXT_KENT': 18,
    'TEXT_HARRIS': 19,
    'TEXT_POPURI': 20,
    'TEXT_MARIA': 21,
    'TEXT_MAY': 22,
    'TEXT_ANN': 23,
    'TEXT_DOUG': 24,
    'TEXT_GRAY': 25,
    'TEXT_BASIL': 26,
    'TEXT_LILIA': 27,
    'TEXT_DUKE': 28,
    'TEXT_SHIPPER': 29,
    'TEXT_HARVEST_SPRITES': 30,
    'TEXT_ASSISTANT_CARPENTERS': 31,
    'TEXT_MASTER_CARPENTER': 32,
    'TEXT_MAYOR': 33,
    'TEXT_GREG': 34,
    'TEXT_RICK': 35,
    'TEXT_BARLEY': 36,
    'TEXT_SYDNEY': 37,
    'TEXT_POTION_SHOP_DEALER': 38,
    'TEXT_MAYOR_WIFE': 39,
    'TEXT_ELLEN': 40,
    'TEXT_STU': 41,
    'TEXT_MIDWIFE': 42,
    'TEXT_PASTOR': 43,
    'TEXT_SAIBARA': 44,
    'TEXT_CUTSCENES_1': 45,
    'TEXT_FUNERAL_INTRO': 46,
    'TEXT_DIALOGUE_47': 47,
    'TEXT_DIALOGUE_48': 48,
    'TEXT_DIALOGUE_49': 49,
    'TEXT_DIALOGUE_50': 50,
    'TEXT_DIALOGUE_51': 51,
    'TEXT_DOG_RACE': 52,
    'TEXT_DIALOGUE_53': 53,
    'TEXT_DIALOGUE_54': 54,
    'TEXT_DIALOGUE_55': 55,
    'TEXT_NEW_YEAR_FESTIVAL': 56,
    'TEXT_SPIRIT_FESTIVAL': 57,
    'TEXT_HORSE_RACE': 58,
    'TEXT_MARRIED_DIALOGUES': 59,
    'TEXT_DIALOGUE_60': 60,
    'TEXT_KAREN_CUTSCENES_1': 61,
    'TEXT_DIALOGUE_62': 62,
    'TEXT_DIALOGUE_63': 63,
    'TEXT_DIALOGUE_64': 64,
    'TEXT_EGG_FESTIVAL': 65,
    'TEXT_KAREN_CUTSCENES_2': 66,
    'TEXT_VEGETABLE_FESTIVAL': 67,
    'TEXT_BABY': 68,
    'TEXT_MRS_MANA_AND_JOHN': 69,
    'TEXT_ADDITIONAL_NPCS': 70,
    'TEXT_NPC_BABY': 71,
    'TEXT_HOW_TO_PLAY': 72,
    
    'NPC_MARIA': 0,
    'NPC_POPURI': 1,
    'NPC_ELLI': 2,
    'NPC_ANN': 3,
    'NPC_KAREN': 4,
    'NPC_BABY': 5,
    'NPC_HARRIS': 6,
    'NPC_GRAY': 7,
    'NPC_JEFF': 8,
    'NPC_CLIFF': 9,
    'NPC_KAI': 0xA,
    'NPC_MAYOR': 0xB,
    'NPC_MAYOR_WIFE': 0xC,
    'NPC_LILLIA': 0xD,
    'NPC_BASIL': 0xE,
    'NPC_ELLEN': 0xF,
    'NPC_DOUG': 0x10,
    'NPC_GOTZ': 0x11,
    'NPC_SASHA': 0x12,
    'NPC_POTION_SHOP_DEALER': 0x13,
    'NPC_KENT': 0x14,
    'NPC_STU': 0x15,
    'NPC_MIDWIFE': 0x16,
    'NPC_MAY': 0x17,
    'NPC_RICK': 0x18,
    'NPC_PASTOR': 0x19,
    'NPC_SHIPPER': 0x1A,
    'NPC_SAIBARA': 0x1B,
    'NPC_DUKE': 0x1C,
    'NPC_GREG': 0x1D,
    'NPC_CARPENTER_1': 0x1E,
    'NPC_CARPENTER_2': 0x1F,
    'NPC_MASTER_CARPENTER': 0x20,
    'NPC_HARVEST_SPRITE_1': 0x21,
    'NPC_HARVEST_SPRITE_2': 0x22,
    'NPC_HARVEST_SPRITE_3': 0x23,
    'NPC_SYDNEY': 0x24,
    'NPC_BARLEY': 0x25,
    'NPC_GOURMET_JUDGE': 0x26,
    'NPC_PHOTOGRAPHER': 0x27,
    'NPC_MARIA_HARRIS_BABY': 0x29,
    'NPC_POPURI_GRAY_BABY': 0x2A,
    'NPC_ELLI_JEFF_BABY': 0x2B,
    'NPC_ANN_CLIFF_BABY': 0x2C,
    'NPC_KAREN_KAI_BABY': 0x2D,

    # Short NPC aliases (for array indexing convenience)
    'MARIA': 0,
    'POPURI': 1,
    'ELLI': 2,
    'ANN': 3,
    'KAREN': 4,
    'BABY': 5,
    'HARRIS': 6,
    'GRAY': 7,
    'JEFF': 8,
    'CLIFF': 9,
    'KAI': 0xA,

    # Text bank indices
    'DIALOGUE_0_TEXT_INDEX': 0,
    'LIBRARY_TEXT_INDEX': 1,
    'DIARY_TEXT_INDEX': 2,
    'DIALOGUE_3_TEXT_INDEX': 3,
    'FESTIVAL_OVERLAY_SELECTIONS_TEXT_INDEX': 4,
    'LETTERS_TEXT_INDEX': 5,
    'SHOP_TEXT_INDEX': 6,
    'ANIMAL_INTERACTIONS_TEXT_INDEX': 7,
    'TV_TEXT_INDEX': 8,
    'DIALOGUE_9_TEXT_INDEX': 9,
    'NAMING_SCREEN_TEXT_INDEX': 10,
    'ELLI_TEXT_INDEX': 11,
    'KAI_TEXT_INDEX': 12,
    'KAREN_TEXT_INDEX': 13,
    'GOTZ_TEXT_INDEX': 14,
    'SASHA_TEXT_INDEX': 15,
    'CLIFF_TEXT_INDEX': 16,
    'JEFF_TEXT_INDEX': 17,
    'KENT_TEXT_INDEX': 18,
    'HARRIS_TEXT_INDEX': 19,
    'POPURI_TEXT_INDEX': 20,
    'MARIA_TEXT_INDEX': 21,
    'MAY_TEXT_INDEX': 22,
    'ANN_TEXT_INDEX': 23,
    'DOUG_TEXT_INDEX': 24,
    'GRAY_TEXT_INDEX': 25,
    'BASIL_TEXT_INDEX': 26,
    'LILIA_TEXT_INDEX': 27,
    'DUKE_TEXT_INDEX': 28,
    'SHIPPER_TEXT_INDEX': 29,
    'HARVEST_SPRITES_TEXT_INDEX': 30,
    'ASSISTANT_CARPENTERS_TEXT_INDEX': 31,
    'MASTER_CARPENTER_TEXT_INDEX': 32,
    'MAYOR_TEXT_INDEX': 33,
    'GREG_TEXT_INDEX': 34,
    'RICK_TEXT_INDEX': 35,
    'BARLEY_TEXT_INDEX': 36,
    'SYDNEY_TEXT_INDEX': 37,
    'POTION_SHOP_DEALER_TEXT_INDEX': 38,
    'MAYOR_WIFE_TEXT_INDEX': 39,
    'ELLEN_TEXT_INDEX': 40,
    'STU_TEXT_INDEX': 41,
    'MIDWIFE_TEXT_INDEX': 42,
    'PASTOR_TEXT_INDEX': 43,
    'SAIBARA_TEXT_INDEX': 44,
    'CUTSCENES_1_TEXT_INDEX': 45,
    'FUNERAL_INTRO_TEXT_INDEX': 46,
    'DIALOGUE_47_TEXT_INDEX': 47,
    'DIALOGUE_48_TEXT_INDEX': 48,
    'DIALOGUE_49_TEXT_INDEX': 49,
    'DIALOGUE_50_TEXT_INDEX': 50,
    'DIALOGUE_51_TEXT_INDEX': 51,
    'DOG_RACE_TEXT_INDEX': 52,
    'DIALOGUE_53_TEXT_INDEX': 53,
    'DIALOGUE_54_TEXT_INDEX': 54,
    'DIALOGUE_55_TEXT_INDEX': 55,
    'NEW_YEAR_FESTIVAL_TEXT_INDEX': 56,
    'SPIRIT_FESTIVAL_TEXT_INDEX': 57,
    'HORSE_RACE_TEXT_INDEX': 58,
    'MARRIED_DIALOGUES_TEXT_INDEX': 59,
    'DIALOGUE_60_TEXT_INDEX': 60,
    'KAREN_CUTSCENES_1_TEXT_INDEX': 61,
    'DIALOGUE_62_TEXT_INDEX': 62,
    'DIALOGUE_63_TEXT_INDEX': 63,
    'DIALOGUE_64_TEXT_INDEX': 64,
    'EGG_FESTIVAL_TEXT_INDEX': 65,
    'KAREN_CUTSCENES_2_TEXT_INDEX': 66,
    'VEGETABLE_FESTIVAL_TEXT_INDEX': 67,
    'BABY_TEXT_INDEX': 68,
    'MRS_MANA_AND_JOHN_TEXT_INDEX': 69,
    'ADDITIONAL_NPCS_TEXT_INDEX': 70,
    'NPC_BABY_TEXT_INDEX': 71,
    'HOW_TO_PLAY_TEXT_INDEX': 72,

    'TRUE': 1,
    'FALSE': 0

}

# =============================================================================
# PARSER
# =============================================================================

@dataclass
class SourceLine:
    """Represents a parsed source line"""
    line_num: int
    raw_text: str
    label: Optional[str] = None
    command: Optional[str] = None
    params: List[str] = field(default_factory=list)
    is_directive: bool = False
    directive_name: Optional[str] = None
    directive_args: List[str] = field(default_factory=list)
    comment: Optional[str] = None


class DSLParser:
    """Parses the cutscene DSL source"""
    
    def __init__(self):
        self.lines: List[SourceLine] = []
        
    def parse_file(self, source: str) -> List[SourceLine]:
        """Parse source text into list of SourceLine objects"""
        self.lines = []
        
        for line_num, raw_line in enumerate(source.split('\n'), 1):
            parsed = self._parse_line(line_num, raw_line)
            if parsed:
                self.lines.append(parsed)
                
        return self.lines
    
    def _parse_line(self, line_num: int, raw_line: str) -> Optional[SourceLine]:
        """Parse a single source line"""
        result = SourceLine(line_num=line_num, raw_text=raw_line)
        
        # Extract comment
        line = raw_line
        for comment_char in [';', '#', '//']:
            if comment_char in line:
                idx = line.index(comment_char)
                result.comment = line[idx:].strip()
                line = line[:idx]
                break
        
        line = line.strip()
        if not line:
            return result if result.comment else None
            
        # Check for label (ends with colon, starts with . or alphanumeric)
        if ':' in line:
            parts = line.split(':', 1)
            label_part = parts[0].strip()
            
            if label_part and (label_part.startswith('.') or label_part[0].isalpha() or label_part[0] == '_'):
                result.label = label_part
                line = parts[1].strip() if len(parts) > 1 else ''
        
        if not line:
            return result if result.label or result.comment else None
            
        # Check for directives
        if line.startswith('.'):
            result.is_directive = True
            parts = line.split(None, 1)
            result.directive_name = parts[0]
            if len(parts) > 1:
                result.directive_args = self._parse_params(parts[1])
            return result
            
        # Parse command
        match = re.match(r'(\w+)\s*(.*)', line)
        if match:
            result.command = match.group(1)
            if match.group(2):
                result.params = self._parse_params(match.group(2))
                
        return result
    
    def _parse_params(self, param_str: str) -> List[str]:
        """Parse comma-separated parameters, handling parentheses"""
        params = []
        current = ""
        paren_depth = 0
        
        for char in param_str:
            if char == '(':
                paren_depth += 1
                current += char
            elif char == ')':
                paren_depth -= 1
                current += char
            elif char == ',' and paren_depth == 0:
                params.append(current.strip())
                current = ""
            else:
                current += char
                
        if current.strip():
            params.append(current.strip())
            
        return params


# =============================================================================
# TRANSPILER
# =============================================================================

class CutsceneTranspiler:
    """Transpiles cutscene DSL to GNU as syntax"""
    
    def __init__(self, bank_name: str = "cutscene_bank"):
        self.bank_name = bank_name
        self.parser = DSLParser()
        self.output_lines: List[str] = []
        self.errors: List[str] = []
        self.warnings: List[str] = []
        self.current_line: int = 0
        self.arrays: Dict[str, int] = {}  # array_name -> element_size in bytes
        self.variables: List[Dict] = []   # [{name, type, values, line_num}]
        
    # Element sizes for array type specifiers
    ARRAY_TYPE_SIZES = {
        'u8': 1, 's8': 1, 'byte': 1,
        'u16': 2, 's16': 2, 'half': 2, 'short': 2,
        'u32': 4, 's32': 4, 'word': 4, 'long': 4,
    }
    
    # Type to assembler directive mapping
    VAR_TYPE_DIRECTIVES = {
        'u8': '.byte', 's8': '.byte', 'byte': '.byte',
        'u16': '.half', 's16': '.half', 'half': '.half', 'short': '.half',
        'u32': '.word', 's32': '.word', 'word': '.word', 'long': '.word',
    }
        
    def transpile(self, source: str) -> str:
        """Transpile DSL source to GNU as syntax"""

        self.output_lines = []
        self.errors = []
        self.warnings = []
        self.variables = []
        self.arrays = {}
        
        lines = self.parser.parse_file(source)
        
        # First pass: collect variables and arrays
        code_lines = []

        for line in lines:
            self.current_line = line.line_num
            if line.is_directive and line.directive_name == '.var':
                self._collect_variable(line)
            elif line.is_directive and line.directive_name == '.array':
                self._collect_array(line)
            else:
                code_lines.append(line)
        
        self._emit_header()
        
        if self.variables:
            self._emit_variables()
        
        # Process code lines
        for line in code_lines:
            self.current_line = line.line_num
            self._process_line(line)
            
        # Emit footer
        self._emit_footer()
        
        return '\n'.join(self.output_lines) + '\n'
    
    def _collect_variable(self, line: SourceLine):
        """Collect a variable declaration for later emission"""

        args = line.directive_args

        if len(args) >= 2:

            var_name = args[0]
            var_type = args[1].lower()

            # Remaining args are initial values (default to 0)
            if len(args) > 2:
                values = args[2:]
            else:
                values = ['0']
            
            if var_type in self.VAR_TYPE_DIRECTIVES:

                self.variables.append({
                    'name': var_name,
                    'type': var_type,
                    'values': values,
                    'line_num': line.line_num
                })
                
                # Also register as array if multiple values or for address resolution
                self.arrays[var_name] = self.ARRAY_TYPE_SIZES.get(var_type, 1)

            else:
                self.errors.append(f"Line {line.line_num}: Unknown variable type '{var_type}'")
        else:
            self.errors.append(f"Line {line.line_num}: .var requires name and type (e.g., .var myCounter, u8, 0)")
    
    def _collect_array(self, line: SourceLine):
        """Collect an array declaration"""

        args = line.directive_args
        
        if len(args) >= 2:
        
            array_name = args[0]
            element_type = args[1].lower()
        
            if element_type in self.ARRAY_TYPE_SIZES:
                self.arrays[array_name] = self.ARRAY_TYPE_SIZES[element_type]
            else:
                self.errors.append(f"Line {line.line_num}: Unknown array element type '{args[1]}'")
        
        else:
            self.errors.append(f"Line {line.line_num}: .array requires name and type (e.g., .array npcAffection, u8)")
    
    def _emit_variables(self):
        """Emit variable declarations at start of section"""
        
        self._emit("# --- Variable Space ---")
        
        for var in self.variables:
            directive = self.VAR_TYPE_DIRECTIVES[var['type']]
            resolved_values = ', '.join(self._resolve_value(v) for v in var['values'])
            self._emit(f"{var['name']}:")
            self._emit(f"    {directive}   {resolved_values}")
        
        self._emit()
        # Align before code starts
        self._emit(".balign 4")
        self._emit("# --- Code ---")
        self._emit()
    
    def _emit(self, text: str = ""):
        """Emit a line to output"""
        
        self.output_lines.append(text)
    
    # Reserved GNU as directives that cannot be used as labels
    RESERVED_DIRECTIVES = {
        '.end', '.data', '.text', '.section', '.global', '.globl',
        '.byte', '.half', '.word', '.long', '.short', '.hword',
        '.align', '.balign', '.p2align', '.include', '.incbin',
        '.if', '.else', '.endif', '.ifdef', '.ifndef', '.macro', '.endm',
        '.set', '.equ', '.equiv', '.comm', '.lcomm', '.space', '.skip',
        '.ascii', '.asciz', '.string', '.float', '.double',
        '.file', '.line', '.loc', '.type', '.size', '.weak', '.extern',
        '.org', '.fill', '.zero', '.rept', '.endr', '.irp', '.irpc',
    }
        
    def _transform_label(self, label: str) -> str:
        """
        Transform label names to avoid conflicts with GNU as reserved directives.
        
        Labels starting with '.' are transformed to use '.L_' prefix which is
        GNU's standard local label convention and won't conflict with directives.
        
        Examples:
            .entry  -> .L_entry
            .end    -> .L_end
            .loop   -> .L_loop
            main    -> main (unchanged, no conflict)
        """
        if label.startswith('.'):
            # Transform .name to .L_name to avoid directive conflicts
            return '.L_' + label[1:]
        return label
        
    def _emit_header(self):
        """Emit file header"""

        self._emit(f"# Auto-generated from cutscene DSL")
        self._emit(f"# Do not edit manually")
        self._emit()
        # self._emit(f'.section .{self.bank_name}, "a"')
        # Use .data section for linker compatibility with binary object replacement
        self._emit('.section .data')
        self._emit('.balign 4')
        self._emit()
        self._emit(f'.global {self.bank_name}_start')
        self._emit(f'{self.bank_name}_start:')
        self._emit()
        
    def _emit_footer(self):
        """Emit file footer"""

        self._emit()
        self._emit(f'.global {self.bank_name}_end')
        self._emit(f'{self.bank_name}_end:')
        self._emit()
        self._emit(f'.balign 4')
        
    def _process_line(self, line: SourceLine):
        """Process a single source line"""

        # Emit comment if present (as standalone or with content)
        if line.comment and not line.label and not line.command and not line.is_directive:

            # Convert comment to GNU as style (strip leading ; or // and use #)
            comment_text = line.comment

            for prefix in [';', '//', '#']:
                if comment_text.startswith(prefix):
                    comment_text = comment_text[len(prefix):].strip()
                    break
            self._emit(f"    # {comment_text}")
            return
            
        if line.label:
            self._emit()
            # Transform labels to avoid GNU as reserved directives
            # Labels starting with . are transformed to .L_ prefix
            # This avoids conflicts with .end, .data, .text, etc.
            label = self._transform_label(line.label)
            self._emit(f'{label}:')
            
        # Process directive
        if line.is_directive:
            self._process_directive(line)
            return
            
        # Process command
        if line.command:
            self._process_command(line)
            
    def _process_directive(self, line: SourceLine):
        """Process assembler directives"""

        directive = line.directive_name
        args = line.directive_args
        
        if directive == '.section':
            # Change section
            if args:
                self._emit(f'.section .{args[0]}, "a"')
                
        elif directive == '.align' or directive == '.balign':
            alignment = args[0] if args else '4'
            self._emit(f'    .balign {alignment}')
            
        elif directive in ('.byte', '.db'):
            values = ', '.join(self._resolve_value(a) for a in args)
            self._emit(f'    .byte   {values}')
            
        elif directive in ('.half', '.hword', '.short', '.word', '.dh', '.dw'):
            values = ', '.join(self._resolve_value(a) for a in args)
            self._emit(f'    .half   {values}')
            
        elif directive in ('.long', '.dl'):
            values = ', '.join(self._resolve_value(a) for a in args)
            self._emit(f'    .word   {values}')
            
        elif directive == '.anim_frame':
            self._emit_anim_frame(args)
            
        elif directive == '.anim_end':
            self._emit('    .half   0xFFFF              # ANIM_END')
            self._emit('    .half   0')
            
        elif directive == '.anim_loop':
            target = args[0] if args else '.'
            self._emit(f'    .half   0xFFFE              # ANIM_LOOP')
            self._emit(f'    .half   {target} - .')
            
        elif directive == '.include':
            # Pass through
            if args:
                self._emit(f'.include "{args[0]}"')
                
        elif directive == '.extern':
            # Declare external symbol
            if args:
                self._emit(f'.extern {args[0]}')
                
        elif directive == '.array':
            # Handled in first pass by _collect_array - should not reach here
            pass
        
        elif directive == '.var':
            # Handled in first pass by _collect_variable - should not reach here
            pass
                
        else:
            self.warnings.append(f"Line {line.line_num}: Unknown directive {directive}")
            
    def _emit_anim_frame(self, args: List[str]):
        """Emit animation frame data"""

        # Parse: id, flag, (x, y, z), wait, flip
        anim_id = self._resolve_value(args[0]) if len(args) > 0 else '0'
        flag = self._resolve_value(args[1]) if len(args) > 1 else '0xFE'
        
        # Parse vector
        vec = ['0', '0', '0']
        if len(args) > 2:
            vec_str = args[2]
            if vec_str.startswith('(') and vec_str.endswith(')'):
                vec_parts = vec_str[1:-1].split(',')
                vec = [self._resolve_value(p.strip()) for p in vec_parts[:3]]
        
        wait = self._resolve_value(args[3]) if len(args) > 3 else '255'
        flip = self._resolve_value(args[4]) if len(args) > 4 else '0'
        
        self._emit(f'    .half   {anim_id}             # anim_id')
        self._emit(f'    .byte   {flag}                # flag')
        self._emit(f'    .byte   {vec[0]}, {vec[1]}, {vec[2]}  # vec')
        self._emit(f'    .byte   {wait}                # wait')
        self._emit(f'    .byte   {flip}                # flip')
        
    def _process_command(self, line: SourceLine):
        """Process a command and emit GNU as data directives"""

        cmd = line.command
        params = line.params

        # Try to find command - support both with and without CMD_ prefix
        if cmd not in COMMAND_SPECS:

            # Try adding CMD_ prefix
            cmd_with_prefix = f'CMD_{cmd}'
            
            if cmd_with_prefix in COMMAND_SPECS:
                cmd = cmd_with_prefix
            else:
                self.errors.append(f"Line {line.line_num}: Unknown command '{line.command}'")
                return
            
        opcode, size, param_specs = COMMAND_SPECS[cmd]
        
        # Count expected parameters (excluding padding)
        expected_params = sum(1 for name, ptype in param_specs if not ptype.startswith('pad'))
        provided_params = len(params)
        
        if provided_params != expected_params:

            # Build helpful message showing expected parameter names
            param_names = [name for name, ptype in param_specs if not ptype.startswith('pad')]

            self.errors.append(
                f"Line {line.line_num}: {line.command} expects {expected_params} parameter(s) "
                f"({', '.join(param_names)}), but got {provided_params}"
            )

            return
        
        # Add comment with original command
        comment = f"# {line.command}"

        if params:
            comment += f"({', '.join(params)})"
            
        # Emit opcode
        self._emit(f'    .half   0x{opcode:04X}          {comment}')
        
        # Track bytes emitted (opcode is 2 bytes)
        bytes_emitted = 2
        param_idx = 0
        
        for spec_name, spec_type in param_specs:
            # Skip padding - emit zeros
            if spec_type == 'pad8':
                self._emit(f'    .byte   0')
                bytes_emitted += 1
                continue
            elif spec_type == 'pad16':
                self._emit(f'    .half   0')
                bytes_emitted += 2
                continue
                
            # Get parameter value
            if param_idx < len(params):
                param_value = params[param_idx]
            else:
                param_value = '0'
            param_idx += 1
            
            # Emit based on type
            if spec_type == 'u8':
                val = self._resolve_value(param_value)
                self._emit(f'    .byte   {val}')
                bytes_emitted += 1
                
            elif spec_type == 's8':
                val = self._resolve_value(param_value)
                self._emit(f'    .byte   {val}')
                bytes_emitted += 1
                
            elif spec_type == 'u16':
                val = self._resolve_value(param_value)
                self._emit(f'    .half   {val}')
                bytes_emitted += 2
                
            elif spec_type == 's16':
                val = self._resolve_value(param_value)
                self._emit(f'    .half   {val}')
                bytes_emitted += 2
                
            elif spec_type == 'u32':
                val = self._resolve_value(param_value)
                self._emit(f'    .word   {val}')
                bytes_emitted += 4
                
            elif spec_type == 's32':
                val = self._resolve_value(param_value)
                self._emit(f'    .word   {val}')
                bytes_emitted += 4
                
            elif spec_type == 'rel16':
                # Relative offset - assembler calculates
                # Transform label reference if needed
                target = self._resolve_value(param_value)
                self._emit(f'    .half   {target} - .')
                bytes_emitted += 2
                
            elif spec_type == 'addr32':
                # Absolute address - linker resolves
                symbol = self._resolve_value(param_value)
                self._emit(f'    .word   {symbol}')
                bytes_emitted += 4
                
            else:
                self.errors.append(f"Line {line.line_num}: Unknown param type '{spec_type}'")
                
        # Pad to expected size if needed
        while bytes_emitted < size:
            self._emit(f'    .byte   0')
            bytes_emitted += 1
            
    def _resolve_value(self, s: str) -> str:
        """
        Resolve a value - returns a string suitable for GNU as.
        Constants are resolved to numbers; symbols are passed through.
        Labels starting with . are transformed to avoid directive conflicts.
        Array indexing (e.g., npcAffection[MARIA]) is resolved to base + offset.
        """

        s = s.strip()
        
        # Check for array indexing syntax: arrayName[index]
        if '[' in s and s.endswith(']'):
            return self._resolve_array_access(s)
        
        # Check built-in constants
        if s in BUILTIN_CONSTANTS:
            return str(BUILTIN_CONSTANTS[s])
            
        # Check for hex
        if s.startswith('0x') or s.startswith('0X'):
            return s
            
        # Check for binary
        if s.startswith('0b') or s.startswith('0B'):
            return str(int(s, 2))
            
        # Check for negative number
        if s.startswith('-') and s[1:].isdigit():
            return s
            
        # Check for decimal
        if s.isdigit():
            return s
            
        # Otherwise, it's a symbol - transform if it's a local label
        # Labels starting with . need to be transformed to .L_ prefix
        if s.startswith('.'):
            return self._transform_label(s)
            
        return s
    
    def _resolve_array_access(self, s: str) -> str:
        """
        Resolve array access syntax: arrayName[index]
        Returns: arrayName + offset (for linker resolution)
        
        Example: npcAffection[MARIA] where MARIA=0, element_size=1
                 -> npcAffection + 0
        
        Example: npcAffection[POPURI] where POPURI=1, element_size=1
                 -> npcAffection + 1
        """

        bracket_idx = s.index('[')
        array_name = s[:bracket_idx].strip()
        index_expr = s[bracket_idx + 1:-1].strip()  # Remove [ and ]
        
        # Resolve the index expression (could be a constant like MARIA or a number)
        if index_expr in BUILTIN_CONSTANTS:
            index_val = BUILTIN_CONSTANTS[index_expr]
        elif index_expr.startswith('0x') or index_expr.startswith('0X'):
            index_val = int(index_expr, 16)
        elif index_expr.isdigit():
            index_val = int(index_expr)
        else:
            # Unknown index - emit warning and use 0
            self.warnings.append(f"Line {self.current_line}: Unknown array index '{index_expr}', using 0")
            index_val = 0
        
        # Look up element size for this array
        if array_name in self.arrays:
            element_size = self.arrays[array_name]
        else:
            # Array not declared - emit warning and assume u8 (1 byte)
            self.warnings.append(f"Line {self.current_line}: Array '{array_name}' not declared with .array directive, assuming u8")
            element_size = 1
        
        # Compute byte offset
        offset = index_val * element_size
        
        # Return expression for GNU as / linker
        if offset == 0:
            return array_name
        else:
            return f"{array_name} + {offset}"


# =============================================================================
# MAIN
# =============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='HM64 Cutscene DSL to GNU as Transpiler',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Example:
  %(prog)s cutsceneBank1.cutscene -o cutsceneBank1.s -n bank1
  
"""
    )
    
    parser.add_argument('input', help='Input DSL source file')
    parser.add_argument('-o', '--output', help='Output assembly file (default: stdout)')
    parser.add_argument('-n', '--name', default='cutscene_bank',
                        help='Bank name for section and symbols (default: cutscene_bank)')
    
    args = parser.parse_args()
    
    with open(args.input, 'r') as f:
        source = f.read()
    
    transpiler = CutsceneTranspiler(bank_name=args.name)
    output = transpiler.transpile(source)
    
    for warning in transpiler.warnings:
        print(f"WARNING: {warning}", file=sys.stderr)
        
    for error in transpiler.errors:
        print(f"ERROR: {error}", file=sys.stderr)
        
    if transpiler.errors:
        sys.exit(1)
        
    if args.output:
        with open(args.output, 'w') as f:
            f.write(output)
        print(f"Transpiled to {args.output}", file=sys.stderr)
    else:
        print(output)


if __name__ == '__main__':
    main()