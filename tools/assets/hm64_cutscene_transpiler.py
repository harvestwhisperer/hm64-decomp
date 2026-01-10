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
from pathlib import Path

# Import shared command specs from utilities
from hm64_cutscene_utilities import CutsceneOpcode, COMMAND_SPECS_BY_NAME


# Command specs are imported from shared utilities
# COMMAND_SPECS_BY_NAME format: 'CMD_NAME' or 'NAME' -> (opcode, size, [(param_name, param_type), ...])
COMMAND_SPECS = COMMAND_SPECS_BY_NAME

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
    'ENTITY_ASSET_PLAYER_DOG': 64,
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
    'TEXT_VINEYARD_CUTSCENES': 61,
    'TEXT_DIALOGUE_62': 62,
    'TEXT_DIALOGUE_63': 63,
    'TEXT_DIALOGUE_64': 64,
    'TEXT_EGG_FESTIVAL': 65,
    'TEXT_BEACH_CUTSCENES': 66,
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
    'FESTIVALS_TEXT_INDEX': 4,
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
    'VINEYARD_CUTSCENES_TEXT_INDEX': 61,
    'DIALOGUE_62_TEXT_INDEX': 62,
    'DIALOGUE_63_TEXT_INDEX': 63,
    'DIALOGUE_64_TEXT_INDEX': 64,
    'EGG_FESTIVAL_TEXT_INDEX': 65,
    'BEACH_CUTSCENES_TEXT_INDEX': 66,
    'VEGETABLE_FESTIVAL_TEXT_INDEX': 67,
    'BABY_TEXT_INDEX': 68,
    'MRS_MANA_AND_JOHN_TEXT_INDEX': 69,
    'ADDITIONAL_NPCS_TEXT_INDEX': 70,
    'NPC_BABY_TEXT_INDEX': 71,
    'HOW_TO_PLAY_TEXT_INDEX': 72,

    'TRUE': 1,
    'FALSE': 0,

    'CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY': 1,

}

# =============================================================================
# STRUCT TYPE DEFINITIONS
# =============================================================================

# Struct member definitions: (offset, type) or (offset, type, count) for arrays
# For 2D arrays: (offset, type, dim1, dim2)
STRUCT_TYPES = {
    # Player struct at 0x80189060
    'Player': {
        'currentStamina': (0x00, 'u8'),
        'name': (0x01, 'u8', 6),
        'toolHeldCounter': (0x08, 'u16'),
        'toolUseCounters': (0x0A, 'u16', 5),
        'currentToolLevel': (0x14, 'u8'),
        'toolSlots': (0x15, 'u8', 8),
        'currentTool': (0x1D, 'u8'),
        'bottleContents': (0x1E, 'u8'),
        'toolLevels': (0x1F, 'u8', 5),
        'belongingsSlots': (0x24, 'u8', 8),
        'heldItem': (0x2C, 'u8'),
        'itemInfoIndex': (0x2D, 'u8'),
        'keyItemSlots': (0x2E, 'u8', 24),
        'unk_46': (0x46, 'u8'),
        'unk_47': (0x47, 'u8'),
        'coordinates': (0x48, 'f32', 3),
        'movementVector': (0x54, 'f32', 3),
        'velocity': (0x60, 'f32'),
        'actionHandler': (0x64, 'u16'),
        'animationHandler': (0x66, 'u16'),
        'actionPhaseFrameCounter': (0x68, 'u16'),
        'actionPhase': (0x6A, 'u8'),
        'shopItemIndex': (0x6B, 'u8'),
        'heldAnimalIndex': (0x6C, 'u8'),
        'direction': (0x6D, 'u8'),
        'savedDirection': (0x6E, 'u8'),
        'actionTimer': (0x6F, 'u8'),
        'groundObjectIndex': (0x70, 'u8'),
        'fatigueCounter': (0x71, 'u8'),
        'staminaExhaustionLevel': (0x72, 'u8'),
        'fatigueThreshold': (0x73, 'u8'),
        'fishingSpotType': (0x74, 'u8'),
        'unused': (0x75, 'u8'),
        'flags': (0x76, 'u16'),
    },

    # Dog struct at 0x801886B0
    # Note: 1 byte padding after name for Vec3f alignment
    'Dog': {
        'affection': (0x00, 'u8'),
        'name': (0x01, 'u8', 6),
        'coordinates': (0x08, 'f32', 3),
        'entityIndex': (0x14, 'u16'),
        'location': (0x16, 'u8'),
        'actionState': (0x17, 'u8'),
        'direction': (0x18, 'u8'),
        'speed': (0x19, 'u8'),
        'stateTimer': (0x1A, 'u8'),
        'unk_1B': (0x1B, 'u8'),
        'bestRacePlacement': (0x1C, 'u8'),
        'flags': (0x1E, 'u16'),
    },

    # Horse struct at 0x8016FDD0
    # Note: 1 byte padding after name for Vec3f alignment
    'Horse': {
        'affection': (0x00, 'u8'),
        'name': (0x01, 'u8', 6),
        'coordinates': (0x08, 'f32', 3),
        'entityIndex': (0x14, 'u16'),
        'location': (0x16, 'u8'),
        'actionState': (0x17, 'u8'),
        'direction': (0x18, 'u8'),
        'speed': (0x19, 'u8'),
        'stateTimer': (0x1A, 'u8'),
        'unk_1B': (0x1B, 'u8'),
        'grown': (0x1C, 'u8'),
        'age': (0x1D, 'u8'),
        'bestRacePlacement': (0x1E, 'u8'),
        'unk_1F': (0x1F, 'u8'),
        'flags': (0x20, 'u16'),
    },

    # RacingContext struct at 0x801C3DA0
    'RacingContext': {
        'displayOddsTop': (0x00, 'u16', 3),
        # oddsAndNames union at 0x06 - support both views
        'oddsAndNames': (0x06, 'u8', 6),  # raw bytes view
        'oddsAndNames.displayOddsBottom': (0x06, 'u16', 3),  # u16[3] view
        'oddsAndNames.racerNameLookup': (0x06, 'u8', 1, 6),  # u8[1][6] view
        'racerNames': (0x0C, 'u8', 6, 6),
        'playerBets': (0x30, 'u8', 3, 6),
        'racerOdds': (0x42, 'u8', 3, 6),
        'racerNameIndices': (0x54, 'u8', 6, 3),
        'finishOrder': (0x66, 'u8', 6),
        'currentRaceIndex': (0x6C, 'u8'),
        'playerRaceNumber': (0x6D, 'u8'),
        'unk_6E': (0x6E, 'u8'),
        'betPlacedFlags': (0x6F, 'u8', 3),
    },
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

    # Buffer base addresses
    BUFFER_BASES = {
        1: 0x802F4000,
        2: 0x802F5000,
    }

    def __init__(self, bank_name: str = "cutscene_bank"):
        self.bank_name = bank_name
        self.parser = DSLParser()
        self.output_lines: List[str] = []
        self.errors: List[str] = []
        self.warnings: List[str] = []
        self.current_line: int = 0
        self.arrays: Dict[str, int] = {}  # array_name -> element_size in bytes
        self.variables: List[Dict] = []   # [{name, type, values, line_num}]
        self.buffer_base: Optional[int] = None  # Buffer base address from .buffer directive
        self.locals: Dict[str, Dict] = {}  # name -> {offset, type, size} for buffer-local variables
        self.next_local_offset: int = 0  # Next available offset for auto-allocated buffer-local vars
        self.structs: Dict[str, str] = {}  # instance_name -> type_name for struct member access
        
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
        self.buffer_base = None
        self.locals = {}
        self.next_local_offset = 0
        self.structs = {}

        lines = self.parser.parse_file(source)

        # First pass: collect variables, arrays, buffer directive, and locals
        code_lines = []

        for line in lines:
            self.current_line = line.line_num
            if line.is_directive and line.directive_name == '.buffer':
                self._collect_buffer(line)
            elif line.is_directive and line.directive_name == '.var':
                self._collect_variable(line)
            elif line.is_directive and line.directive_name == '.array':
                self._collect_array(line)
            elif line.is_directive and line.directive_name == '.local':
                self._collect_local(line)
            elif line.is_directive and line.directive_name == '.struct':
                self._collect_struct(line)
            else:
                code_lines.append(line)
        
        self._emit_header()

        if self.variables:
            self._emit_variables()

        if self.locals:
            self._emit_locals()

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
                size = self.ARRAY_TYPE_SIZES.get(var_type, 1)

                # If buffer is set, treat as buffer-local variable (resolve to buffer_base + offset)
                if self.buffer_base is not None:
                    # Add to locals for address resolution
                    self.locals[var_name] = {
                        'offset': self.next_local_offset,
                        'type': var_type,
                        'size': size,
                        'line_num': line.line_num
                    }
                    self.next_local_offset += size
                else:
                    # No buffer set - treat as linker-resolved symbol
                    self.variables.append({
                        'name': var_name,
                        'type': var_type,
                        'values': values,
                        'line_num': line.line_num
                    })

                # Also register as array for array indexing calculations
                self.arrays[var_name] = size

            else:
                self.errors.append(f"Line {line.line_num}: Unknown variable type '{var_type}'")
        else:
            self.errors.append(f"Line {line.line_num}: .var requires name and type (e.g., .var myCounter, u8, 0)")

    def _collect_buffer(self, line: SourceLine):
        """Collect buffer directive - sets the base address for buffer references"""

        args = line.directive_args

        if len(args) >= 1:
            buffer_arg = args[0].strip()

            # Parse buffer number (1 or 2) or direct hex address
            if buffer_arg.isdigit():
                buffer_num = int(buffer_arg)
                if buffer_num in self.BUFFER_BASES:
                    self.buffer_base = self.BUFFER_BASES[buffer_num]
                else:
                    self.errors.append(f"Line {line.line_num}: Invalid buffer number '{buffer_num}' (use 1 or 2)")
            elif buffer_arg.startswith('0x') or buffer_arg.startswith('0X'):
                self.buffer_base = int(buffer_arg, 16)
            else:
                self.errors.append(f"Line {line.line_num}: Invalid buffer argument '{buffer_arg}' (use 1, 2, or hex address)")
        else:
            self.errors.append(f"Line {line.line_num}: .buffer requires buffer number (1 or 2) or hex address")

    def _collect_local(self, line: SourceLine):
        """Collect a local buffer variable declaration

        Syntax: .local name, type [, initial_value]
        Example: .local flags, u32
                 .local counter, u16, 0
                 .local byte_val, u8, 255

        The name can then be used in commands and will resolve to buffer_base + offset.
        Offsets are auto-calculated. Space is reserved in the output (without a label).
        """

        args = line.directive_args

        if len(args) >= 2:
            var_name = args[0]
            var_type = args[1].lower()

            # Parse initial value (optional, default 0)
            initial_value = 0
            if len(args) >= 3:
                value_str = args[2]
                if value_str.startswith('0x') or value_str.startswith('0X'):
                    initial_value = int(value_str, 16)
                elif value_str.startswith('-'):
                    initial_value = int(value_str)
                elif value_str.isdigit():
                    initial_value = int(value_str)
                else:
                    self.errors.append(f"Line {line.line_num}: Invalid initial value '{value_str}' in .local")
                    return

            # Get size from type
            if var_type in self.ARRAY_TYPE_SIZES:
                size = self.ARRAY_TYPE_SIZES[var_type]
            else:
                self.errors.append(f"Line {line.line_num}: Unknown type '{var_type}' in .local")
                return

            # Store the local variable with auto-calculated offset
            self.locals[var_name] = {
                'offset': self.next_local_offset,
                'type': var_type,
                'size': size,
                'value': initial_value,
                'line_num': line.line_num
            }
            self.next_local_offset += size
        else:
            self.errors.append(f"Line {line.line_num}: .local requires name and type (e.g., .local flags, u32)")

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

    def _collect_struct(self, line: SourceLine):
        """Collect a struct instance declaration

        Syntax: .struct instanceName, TypeName
        Example: .struct gPlayer, Player
                 .struct horseInfo, Horse

        The instance name can then be used with member access syntax:
        e.g., gPlayer.currentStamina, horseInfo.name[2]
        """

        args = line.directive_args

        if len(args) >= 2:
            instance_name = args[0]
            type_name = args[1]

            if type_name in STRUCT_TYPES:
                self.structs[instance_name] = type_name
            else:
                self.errors.append(f"Line {line.line_num}: Unknown struct type '{type_name}'")
        else:
            self.errors.append(f"Line {line.line_num}: .struct requires instance and type (e.g., .struct gPlayer, Player)")

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

    def _emit_locals(self):
        """Emit local buffer variable space (without labels)

        Local variables are emitted as anonymous data at the start of the section.
        They're sorted by offset to ensure proper layout, with initial values.
        Aligned to 4 bytes before code starts.
        """

        self._emit("# --- Local Buffer Space ---")

        # Sort locals by offset
        sorted_locals = sorted(self.locals.items(), key=lambda x: x[1]['offset'])

        current_offset = 0
        for name, info in sorted_locals:
            offset = info['offset']
            size = info['size']
            var_type = info['type']
            value = info.get('value', 0)

            # Emit padding if there's a gap
            if offset > current_offset:
                gap = offset - current_offset
                self._emit(f"    .space  {gap}          # padding")
                current_offset = offset

            # Emit the variable with its initial value
            directive = self.VAR_TYPE_DIRECTIVES[var_type]
            self._emit(f"    {directive}   {value}          # {name} @ buffer+0x{offset:X}")
            current_offset += size

        self._emit()
        # Align to 4 bytes before code starts
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
            .L_foo  -> .L_foo (already has prefix, unchanged)
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
            target = self._transform_label(target)
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

        elif directive == '.buffer':
            # Handled in first pass by _collect_buffer - should not reach here
            pass

        elif directive == '.local':
            # Handled in first pass by _collect_local - should not reach here
            pass

        elif directive == '.struct':
            # Handled in first pass by _collect_struct - should not reach here
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

            elif spec_type in ('rom_start32', 'rom_end32'):
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
        Buffer references (buffer, buffer+0x4) are resolved to absolute addresses.
        """

        s = s.strip()

        # Check for local buffer variable
        if s in self.locals:
            return self._resolve_local_var(s)

        # Check for buffer reference (buffer or buffer+offset)
        if s == 'buffer' or s.startswith('buffer+'):
            return self._resolve_buffer_ref(s)

        # Check for struct member access: instance.member or instance.member[index]
        # Must check before array indexing since structs can have array members
        if '.' in s and not s.startswith('.'):
            result = self._resolve_struct_access(s)
            if result is not None:
                return result

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

    def _resolve_buffer_ref(self, s: str) -> str:
        """
        Resolve buffer reference: buffer or buffer+offset
        Returns: absolute address as hex string

        Example: buffer -> 0x802F4000 (if buffer 1)
        Example: buffer+0x4 -> 0x802F4004
        Example: buffer+4 -> 0x802F4004
        """

        if self.buffer_base is None:
            self.errors.append(f"Line {self.current_line}: 'buffer' used but no .buffer directive found")
            return "0x00000000"

        if s == 'buffer':
            return f"0x{self.buffer_base:08X}"

        # Parse buffer+offset
        if s.startswith('buffer+'):
            offset_str = s[7:].strip()  # Remove 'buffer+'

            # Parse offset (hex or decimal)
            if offset_str.startswith('0x') or offset_str.startswith('0X'):
                offset = int(offset_str, 16)
            elif offset_str.isdigit():
                offset = int(offset_str)
            else:
                self.errors.append(f"Line {self.current_line}: Invalid buffer offset '{offset_str}'")
                return f"0x{self.buffer_base:08X}"

            return f"0x{self.buffer_base + offset:08X}"

        # Shouldn't reach here
        return s

    def _resolve_local_var(self, name: str) -> str:
        """
        Resolve a local buffer variable to its absolute address.

        Example: flags (declared as .local flags, u32, 0x00 with .buffer 1)
                 -> 0x802F4000
        """

        if self.buffer_base is None:
            self.errors.append(f"Line {self.current_line}: Local variable '{name}' used but no .buffer directive found")
            return "0x00000000"

        local_info = self.locals[name]
        addr = self.buffer_base + local_info['offset']
        return f"0x{addr:08X}"

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

    def _resolve_struct_access(self, s: str) -> Optional[str]:
        """
        Resolve struct member access: instance.member or instance.member[index]
        Returns: resolved expression, or None if not a struct access

        Examples:
            gPlayer.currentStamina -> gPlayer + 0
            gPlayer.name[2] -> gPlayer + 3  (offset 1 + 2*1)
            gRacingContext.racerOdds[1][2] -> gRacingContext + 0x44
        """

        # Split on first dot to get instance name
        dot_idx = s.index('.')
        instance = s[:dot_idx].strip()
        rest = s[dot_idx + 1:].strip()

        # Check if this is a known struct instance
        if instance not in self.structs:
            # Not a registered struct - return None to try other resolution
            return None

        type_name = self.structs[instance]
        struct_def = STRUCT_TYPES[type_name]

        # Parse member name and optional indices
        # Handle cases like: member, member[0], member[0][1]
        member_name = rest
        indices = []

        # Extract all bracketed indices
        while '[' in member_name:
            bracket_idx = member_name.index('[')
            end_bracket = member_name.index(']')
            index_expr = member_name[bracket_idx + 1:end_bracket]
            indices.append(self._resolve_index(index_expr))
            # Remove the index from member name and continue
            member_name = member_name[:bracket_idx] + member_name[end_bracket + 1:]

        member_name = member_name.strip()

        if member_name not in struct_def:
            self.errors.append(f"Line {self.current_line}: Unknown member '{member_name}' in struct type '{type_name}'")
            return s

        member_info = struct_def[member_name]
        base_offset = member_info[0]
        elem_type = member_info[1]

        # Get element size - handle f32 specially
        if elem_type == 'f32':
            elem_size = 4
        else:
            elem_size = self.ARRAY_TYPE_SIZES.get(elem_type, 1)

        # Calculate total offset with indices
        total_offset = base_offset

        if indices:
            # Get array dimensions from member_info (after offset and type)
            if len(member_info) > 2:
                dims = member_info[2:]  # Tuple of dimensions

                # For multi-dimensional arrays, calculate offset
                # dims = (6, 3) means arr[6][3], so arr[i][j] = base + (i * 3 + j) * elem_size
                if len(indices) == 1:
                    # Single index into array
                    total_offset += indices[0] * elem_size
                elif len(indices) == 2 and len(dims) >= 2:
                    # 2D array access: arr[i][j] = base + (i * inner_dim + j) * elem_size
                    inner_dim = dims[1] if len(dims) >= 2 else 1
                    total_offset += (indices[0] * inner_dim + indices[1]) * elem_size
                else:
                    # General case for more dimensions
                    stride = elem_size
                    for i in range(len(indices) - 1, -1, -1):
                        total_offset += indices[i] * stride
                        if i > 0 and i - 1 < len(dims) - 1:
                            stride *= dims[len(dims) - len(indices) + i]
            else:
                # Indexing a scalar member - emit warning
                self.warnings.append(f"Line {self.current_line}: Indexing non-array member '{member_name}'")
                total_offset += indices[0] * elem_size

        # Return linker expression
        if total_offset == 0:
            return instance
        else:
            return f"{instance} + {total_offset}"

    def _resolve_index(self, index_expr: str) -> int:
        """Resolve an index expression to an integer value"""

        index_expr = index_expr.strip()

        if index_expr in BUILTIN_CONSTANTS:
            return BUILTIN_CONSTANTS[index_expr]
        elif index_expr.startswith('0x') or index_expr.startswith('0X'):
            return int(index_expr, 16)
        elif index_expr.startswith('-'):
            return int(index_expr)
        elif index_expr.isdigit():
            return int(index_expr)
        else:
            self.warnings.append(f"Line {self.current_line}: Unknown index '{index_expr}', using 0")
            return 0


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

    # Support reading from stdin with '-'
    if args.input == '-':
        source = sys.stdin.read()
    else:
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