#ifndef _ENTITY_BUFFERS_H_
#define _ENTITY_BUFFERS_H_

// ============================================================================
// Base memory regions
// ============================================================================

#define NPC_LARGE_REGION_1_COUNT        8       // Festival slots 1-8
#define NPC_LARGE_REGION_2_COUNT        13      // Festival slots 9-21
#define SMALL_SPRITE_REGION_COUNT       20      // Festival small sprites

#define ENTITY_VRAM_START             0x8026F000

// Region 1: Large spritesheet NPCs
#define ENTITY_REGION_NPC_LARGE_1_BASE  ENTITY_VRAM_START
#define ENTITY_REGION_NPC_LARGE_1_END   (ENTITY_REGION_NPC_LARGE_1_BASE + NPC_LARGE_SLOT_SIZE * NPC_LARGE_REGION_1_COUNT)

// Region 2: Large spritesheet NPCs
#define ENTITY_REGION_NPC_LARGE_2_BASE  0x80294250

// Region 3: Small sprites
#define ENTITY_REGION_SMALL_SPRITE_BASE 0x802BBE50

#define ENTITY_REGION_COW_FESTIVAL_NPC_BASE  0x80278D50  // Cow festival large NPCs (slots 6-10)

// ============================================================================
// Large Spritesheet NPCs
// ============================================================================

#define NPC_LARGE_SIZE_TEX              0xC80
#define NPC_LARGE_SIZE_PALETTE          0xA80
#define NPC_LARGE_SIZE_ANIM             0xE00
#define NPC_LARGE_SIZE_SPRITESHEET      0x100
#define NPC_LARGE_SIZE_TEX_LOOKUP       0x280

#define NPC_LARGE_OFFSET_TEX2           (NPC_LARGE_SIZE_TEX)
#define NPC_LARGE_OFFSET_PALETTE        (NPC_LARGE_OFFSET_TEX2 + NPC_LARGE_SIZE_TEX)
#define NPC_LARGE_OFFSET_ANIM           (NPC_LARGE_OFFSET_PALETTE + NPC_LARGE_SIZE_PALETTE)
#define NPC_LARGE_OFFSET_SPRITESHEET    (NPC_LARGE_OFFSET_ANIM + NPC_LARGE_SIZE_ANIM)
#define NPC_LARGE_OFFSET_TEX_LOOKUP     (NPC_LARGE_OFFSET_SPRITESHEET + NPC_LARGE_SIZE_SPRITESHEET)

#define NPC_LARGE_SLOT_SIZE             (NPC_LARGE_OFFSET_TEX_LOOKUP + NPC_LARGE_SIZE_TEX_LOOKUP)

// Typed accessor macros (includes cast and calculates address)
#define NPC_LARGE_TEX1(base, n)         ((u8*)((base) + NPC_LARGE_SLOT_SIZE * (n)))
#define NPC_LARGE_TEX2(base, n)         ((u8*)((base) + NPC_LARGE_SLOT_SIZE * (n) + NPC_LARGE_OFFSET_TEX2))
#define NPC_LARGE_PALETTE(base, n)      ((u16*)((base) + NPC_LARGE_SLOT_SIZE * (n) + NPC_LARGE_OFFSET_PALETTE))
#define NPC_LARGE_ANIM(base, n)         ((AnimationFrameMetadata*)((base) + NPC_LARGE_SLOT_SIZE * (n) + NPC_LARGE_OFFSET_ANIM))
#define NPC_LARGE_SPRITESHEET(base, n)  ((u32*)((base) + NPC_LARGE_SLOT_SIZE * (n) + NPC_LARGE_OFFSET_SPRITESHEET))
#define NPC_LARGE_TEX_LOOKUP(base, n)   ((u32*)((base) + NPC_LARGE_SLOT_SIZE * (n) + NPC_LARGE_OFFSET_TEX_LOOKUP))

// ============================================================================
// Small Sprites
// ============================================================================

#define SMALL_SPRITE_SIZE_TEX           0x400

#define SMALL_SPRITE_OFFSET_TEX2        (SMALL_SPRITE_SIZE_TEX)

#define SMALL_SPRITE_SLOT_SIZE          (SMALL_SPRITE_SIZE_TEX * 2)

#define SMALL_SPRITE_TEX1(base, n)      ((u8*)((base) + SMALL_SPRITE_SLOT_SIZE * (n)))
#define SMALL_SPRITE_TEX2(base, n)      ((u8*)((base) + SMALL_SPRITE_SLOT_SIZE * (n) + SMALL_SPRITE_OFFSET_TEX2))

// ============================================================================
// Standard NPCs
// ============================================================================

#define STANDARD_NPC_SIZE_TEX           0xA88
#define STANDARD_NPC_OFFSET_TEX2        (STANDARD_NPC_SIZE_TEX)
#define STANDARD_NPC_SLOT_SIZE          0x1510  // Includes alignment padding

#define STANDARD_NPC_TEX1(base, n)      ((u8*)((base) + STANDARD_NPC_SLOT_SIZE * (n)))
#define STANDARD_NPC_TEX2(base, n)      ((u8*)((base) + STANDARD_NPC_SLOT_SIZE * (n) + STANDARD_NPC_OFFSET_TEX2))

// ============================================================================
// Chickens (Slots 2-7, shared metadata)
// ============================================================================

#define CHICKEN_SIZE_TEX                0x480
#define CHICKEN_OFFSET_TEX2             (CHICKEN_SIZE_TEX)
#define CHICKEN_SLOT_SIZE               0x900

#define CHICKEN_TEX1(base, n)           ((u8*)((base) + CHICKEN_SLOT_SIZE * (n)))
#define CHICKEN_TEX2(base, n)           ((u8*)((base) + CHICKEN_SLOT_SIZE * (n) + CHICKEN_OFFSET_TEX2))

// ============================================================================
// Chicks (Slots 8-13, shared metadata)
// ============================================================================

#define CHICK_SIZE_TEX                  0x90
#define CHICK_OFFSET_TEX2               (CHICK_SIZE_TEX)
#define CHICK_SLOT_SIZE                 0x120

#define CHICK_TEX1(base, n)             ((u8*)((base) + CHICK_SLOT_SIZE * (n)))
#define CHICK_TEX2(base, n)             ((u8*)((base) + CHICK_SLOT_SIZE * (n) + CHICK_OFFSET_TEX2))

// ============================================================================
// Items (Slots 40-48, shared metadata)
// ============================================================================

#define ITEM_SIZE_TEX                   0xE00
#define ITEM_OFFSET_TEX2                (ITEM_SIZE_TEX)
#define ITEM_SLOT_SIZE                  0x1C00

#define ITEM_TEX1(base, n)              ((u8*)((base) + ITEM_SLOT_SIZE * (n)))
#define ITEM_TEX2(base, n)              ((u8*)((base) + ITEM_SLOT_SIZE * (n) + ITEM_OFFSET_TEX2))

// ============================================================================
// Other Entity Types
// ============================================================================

#define DOG_SIZE_TEX                    0x908
#define COW_SLOT_SIZE                   0x1510

// ============================================================================
// Entity Buffer Region Base Addresses
// ============================================================================

#define ENTITY_REGION_SLOT_1_BASE           ENTITY_VRAM_START
#define ENTITY_REGION_CHICKEN_BASE          0x80272800  // Slots 2-7 (Chickens)
#define ENTITY_REGION_CHICK_BASE            0x80277300  // Slots 8-13 (Chicks)
#define ENTITY_REGION_SLOT_39_BASE          0x8028B930  // Slot 39 base (separate)
#define ENTITY_REGION_SLOTS_40_48_BASE      ENTITY_REGION_NPC_LARGE_2_BASE

// ============================================================================
// Shared Metadata Buffer Groups
// ============================================================================

// Group A: Slots 2-7 (Chickens)
#define ENTITY_SLOTS_2_7_PALETTE                0x80275E00
#define ENTITY_SLOTS_2_7_ANIM_METADATA          0x80275F00
#define ENTITY_SLOTS_2_7_SPRITESHEET_INDEX      0x80276700
#define ENTITY_SLOTS_2_7_TEXTURE_TO_PALETTE_LOOKUP         0x80276800

// Group B: Slots 8-13, 39-48 (Small animals, items, shop items)
#define ENTITY_SLOTS_8_13_PALETTE               0x8028DD50
#define ENTITY_SLOTS_8_13_ANIM_METADATA         0x80290550
#define ENTITY_SLOTS_8_13_SPRITESHEET_INDEX     0x80293A50
#define ENTITY_SLOTS_8_13_TEXTURE_TO_PALETTE_LOOKUP        0x80293C50

// Group C: Slots 19, 21-28 (NPCs; also used by adult cows and horse)
#define ENTITY_SLOTS_21_28_PALETTE                0x802779C0
#define ENTITY_SLOTS_21_28_ANIM_METADATA          0x80277AC0
#define ENTITY_SLOTS_21_28_SPRITESHEET_INDEX      0x8027A6C0
#define ENTITY_SLOTS_21_28_TEXTURE_TO_PALETTE_LOOKUP         0x8027A8C0

// Group D: Slots 1 and 20 (Dog/Horse shared)
#define ENTITY_SLOTS_1_20_PALETTE               0x80270210
#define ENTITY_SLOTS_1_20_ANIM_METADATA         0x80270410
#define ENTITY_SLOTS_1_20_SPRITESHEET_INDEX     0x80272000
#define ENTITY_SLOTS_1_20_TEXTURE_TO_PALETTE_LOOKUP        0x80272200

#define ENTITY_CHICK_PALETTE                    0x80276B00
#define ENTITY_CHICK_ANIM_METADATA              0x80276C00
#define ENTITY_CHICK_SPRITESHEET_INDEX          0x80277000
#define ENTITY_CHICK_TEXTURE_TO_PALETTE_LOOKUP             0x80277100

#define ENTITY_BABY_COW_PALETTE                 0x802B1E40
#define ENTITY_BABY_COW_ANIM_METADATA           0x802B1F40
#define ENTITY_BABY_COW_SPRITESHEET_INDEX       0x802B2E40
#define ENTITY_BABY_COW_TEXTURE_TO_PALETTE_LOOKUP          0x802B2F40

#define ENTITY_CALF_PALETTE                     0x802B0940
#define ENTITY_CALF_ANIM_METADATA               0x802B0A40
#define ENTITY_CALF_SPRITESHEET_INDEX           0x802B1940
#define ENTITY_CALF_TEXTURE_TO_PALETTE_LOOKUP              0x802B1A40

#define ENTITY_PREGNANT_COW_PALETTE             0x802B3140
#define ENTITY_PREGNANT_COW_ANIM_METADATA       0x802B3240
#define ENTITY_PREGNANT_COW_SPRITESHEET_INDEX   0x802B4240
#define ENTITY_PREGNANT_COW_TEXTURE_TO_PALETTE_LOOKUP      0x802B4340

#define ENTITY_BABY_SHEEP_PALETTE               0x802B5A40
#define ENTITY_BABY_SHEEP_ANIM_METADATA         0x802B5B40
#define ENTITY_BABY_SHEEP_SPRITESHEET_INDEX     0x802B6240
#define ENTITY_BABY_SHEEP_TEXTURE_TO_PALETTE_LOOKUP        0x802B6340

#define ENTITY_ADULT_SHEEP_PALETTE              0x802B4540
#define ENTITY_ADULT_SHEEP_ANIM_METADATA        0x802B4640
#define ENTITY_ADULT_SHEEP_SPRITESHEET_INDEX    0x802B5640
#define ENTITY_ADULT_SHEEP_TEXTURE_TO_PALETTE_LOOKUP       0x802B5740

// ----------------------------------------------------------------------------
// Entity Slots 14-18 (Cows)
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_14_TEXTURE_1                0x8027ADC0
#define ENTITY_SLOT_14_TEXTURE_2                0x8027B848
#define ENTITY_SLOT_14_PALETTE                  0x80284130
#define ENTITY_SLOT_14_ANIM_METADATA            0x80284230
#define ENTITY_SLOT_14_SPRITESHEET_INDEX        0x802852B0
#define ENTITY_SLOT_14_TEXTURE_TO_PALETTE_LOOKUP           0x802854B0

#define ENTITY_SLOT_15_TEXTURE_1                0x8027C2D0
#define ENTITY_SLOT_15_TEXTURE_2                0x8027CD58
#define ENTITY_SLOT_15_PALETTE                  0x80285930
#define ENTITY_SLOT_15_ANIM_METADATA            0x80285A30
#define ENTITY_SLOT_15_SPRITESHEET_INDEX        0x80286AB0
#define ENTITY_SLOT_15_TEXTURE_TO_PALETTE_LOOKUP           0x80286CB0

#define ENTITY_SLOT_16_TEXTURE_1                0x8027D7E0
#define ENTITY_SLOT_16_TEXTURE_2                0x8027E268
#define ENTITY_SLOT_16_PALETTE                  0x80287130
#define ENTITY_SLOT_16_ANIM_METADATA            0x80287230
#define ENTITY_SLOT_16_SPRITESHEET_INDEX        0x802882B0
#define ENTITY_SLOT_16_TEXTURE_TO_PALETTE_LOOKUP           0x802884B0

#define ENTITY_SLOT_17_TEXTURE_1                0x8027ECF0
#define ENTITY_SLOT_17_TEXTURE_2                0x8027F778
#define ENTITY_SLOT_17_PALETTE                  0x80288930
#define ENTITY_SLOT_17_ANIM_METADATA            0x80288A30
#define ENTITY_SLOT_17_SPRITESHEET_INDEX        0x80289AB0
#define ENTITY_SLOT_17_TEXTURE_TO_PALETTE_LOOKUP           0x80289CB0

#define ENTITY_SLOT_18_TEXTURE_1                0x80280200
#define ENTITY_SLOT_18_TEXTURE_2                0x80280C88
#define ENTITY_SLOT_18_PALETTE                  0x8028A130
#define ENTITY_SLOT_18_ANIM_METADATA            0x8028A230
#define ENTITY_SLOT_18_SPRITESHEET_INDEX        0x8028B2B0
#define ENTITY_SLOT_18_TEXTURE_TO_PALETTE_LOOKUP           0x8028B4B0

// ----------------------------------------------------------------------------
// Entity Slot 19 (Horse)
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_19_TEXTURE_1                0x80281710
#define ENTITY_SLOT_19_TEXTURE_2                0x80282198

// ----------------------------------------------------------------------------
// Entity Slot 20 - Uses Slot 1/20 shared metadata
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_20_TEXTURE_1                0x80282C20
#define ENTITY_SLOT_20_TEXTURE_2                0x802836A8

// ----------------------------------------------------------------------------
// Entity Slots 21-28 (NPCs)
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_21_TEXTURE_1                0x802A60C0
#define ENTITY_SLOT_21_TEXTURE_2                0x802A6B48
#define ENTITY_SLOT_22_TEXTURE_1                0x802A75D0
#define ENTITY_SLOT_22_TEXTURE_2                0x802A8058
#define ENTITY_SLOT_23_TEXTURE_1                0x802A8AE0
#define ENTITY_SLOT_23_TEXTURE_2                0x802A9568
#define ENTITY_SLOT_24_TEXTURE_1                0x802A9FF0
#define ENTITY_SLOT_24_TEXTURE_2                0x802AAA78
#define ENTITY_SLOT_25_TEXTURE_1                0x802AB500
#define ENTITY_SLOT_25_TEXTURE_2                0x802ABF88
#define ENTITY_SLOT_26_TEXTURE_1                0x802ACA10
#define ENTITY_SLOT_26_TEXTURE_2                0x802AD498
#define ENTITY_SLOT_27_TEXTURE_1                0x802ADF20
#define ENTITY_SLOT_27_TEXTURE_2                0x802AE9A8
#define ENTITY_SLOT_28_TEXTURE_1                0x802AF430
#define ENTITY_SLOT_28_TEXTURE_2                0x802AFEB8

// ----------------------------------------------------------------------------
// Entity Slots 21-28 (Cutscene NPCs)
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_21_CUTSCENE_TEXTURE_1       0x802A60C0
#define ENTITY_SLOT_21_CUTSCENE_TEXTURE_2       0x802A6D40
#define ENTITY_SLOT_21_CUTSCENE_PALETTE         0x802A79C0
#define ENTITY_SLOT_21_CUTSCENE_ANIM_METADATA   0x802A8440
#define ENTITY_SLOT_21_CUTSCENE_SPRITESHEET     0x802A9240
#define ENTITY_SLOT_21_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802A9340

#define ENTITY_SLOT_22_CUTSCENE_TEXTURE_1       0x802A95C0
#define ENTITY_SLOT_22_CUTSCENE_TEXTURE_2       0x802AA240
#define ENTITY_SLOT_22_CUTSCENE_PALETTE         0x802AAEC0
#define ENTITY_SLOT_22_CUTSCENE_ANIM_METADATA   0x802AB940
#define ENTITY_SLOT_22_CUTSCENE_SPRITESHEET     0x802AC740
#define ENTITY_SLOT_22_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802AC840

#define ENTITY_SLOT_23_CUTSCENE_TEXTURE_1       0x802ACAC0
#define ENTITY_SLOT_23_CUTSCENE_TEXTURE_2       0x802AD740
#define ENTITY_SLOT_23_CUTSCENE_PALETTE         0x802AE3C0
#define ENTITY_SLOT_23_CUTSCENE_ANIM_METADATA   0x802AEE40
#define ENTITY_SLOT_23_CUTSCENE_SPRITESHEET     0x802AFC40
#define ENTITY_SLOT_23_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802AFD40

#define ENTITY_SLOT_24_CUTSCENE_TEXTURE_1       0x802AFFC0
#define ENTITY_SLOT_24_CUTSCENE_TEXTURE_2       0x802B0C40
#define ENTITY_SLOT_24_CUTSCENE_PALETTE         0x802B18C0
#define ENTITY_SLOT_24_CUTSCENE_ANIM_METADATA   0x802B2340
#define ENTITY_SLOT_24_CUTSCENE_SPRITESHEET     0x802B3140
#define ENTITY_SLOT_24_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802B3240

#define ENTITY_SLOT_25_CUTSCENE_TEXTURE_1       0x802B34C0
#define ENTITY_SLOT_25_CUTSCENE_TEXTURE_2       0x802B4140
#define ENTITY_SLOT_25_CUTSCENE_PALETTE         0x802B4DC0
#define ENTITY_SLOT_25_CUTSCENE_ANIM_METADATA   0x802B5840
#define ENTITY_SLOT_25_CUTSCENE_SPRITESHEET     0x802B6640
#define ENTITY_SLOT_25_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802B6740

#define ENTITY_SLOT_26_CUTSCENE_TEXTURE_1       0x802B69C0
#define ENTITY_SLOT_26_CUTSCENE_TEXTURE_2       0x802B7640
#define ENTITY_SLOT_26_CUTSCENE_PALETTE         0x802B82C0
#define ENTITY_SLOT_26_CUTSCENE_ANIM_METADATA   0x802B8D40
#define ENTITY_SLOT_26_CUTSCENE_SPRITESHEET     0x802B9B40
#define ENTITY_SLOT_26_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802B9C40

#define ENTITY_SLOT_27_CUTSCENE_TEXTURE_1       0x802B9EC0
#define ENTITY_SLOT_27_CUTSCENE_TEXTURE_2       0x802BAB40
#define ENTITY_SLOT_27_CUTSCENE_PALETTE         0x802BB7C0
#define ENTITY_SLOT_27_CUTSCENE_ANIM_METADATA   0x802BC240
#define ENTITY_SLOT_27_CUTSCENE_SPRITESHEET     0x802BD040
#define ENTITY_SLOT_27_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802BD140

#define ENTITY_SLOT_28_CUTSCENE_TEXTURE_1       0x802BD3C0
#define ENTITY_SLOT_28_CUTSCENE_TEXTURE_2       0x802BE040
#define ENTITY_SLOT_28_CUTSCENE_PALETTE         0x802BECC0
#define ENTITY_SLOT_28_CUTSCENE_ANIM_METADATA   0x802BF740
#define ENTITY_SLOT_28_CUTSCENE_SPRITESHEET     0x802C0540
#define ENTITY_SLOT_28_CUTSCENE_TEXTURE_TO_PALETTE_LOOKUP  0x802C0640

// ----------------------------------------------------------------------------
// Entity Slots 29-37 (Large spritesheet NPCs)
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_29_TEXTURE_1                0x802CA7C0
#define ENTITY_SLOT_29_TEXTURE_2                0x802CB440
#define ENTITY_SLOT_29_PALETTE                  0x802CC0C0
#define ENTITY_SLOT_29_ANIM_METADATA            0x802CCB40
#define ENTITY_SLOT_29_SPRITESHEET_INDEX        0x802CD940
#define ENTITY_SLOT_29_TEXTURE_TO_PALETTE_LOOKUP           0x802CDA40

#define ENTITY_SLOT_30_TEXTURE_1                0x802CDCC0
#define ENTITY_SLOT_30_TEXTURE_2                0x802CE940
#define ENTITY_SLOT_30_PALETTE                  0x802CF5C0
#define ENTITY_SLOT_30_ANIM_METADATA            0x802D0040
#define ENTITY_SLOT_30_SPRITESHEET_INDEX        0x802D0E40
#define ENTITY_SLOT_30_TEXTURE_TO_PALETTE_LOOKUP           0x802D0F40

#define ENTITY_SLOT_31_TEXTURE_1                0x802D11C0
#define ENTITY_SLOT_31_TEXTURE_2                0x802D1E40
#define ENTITY_SLOT_31_PALETTE                  0x802D2AC0
#define ENTITY_SLOT_31_ANIM_METADATA            0x802D3540
#define ENTITY_SLOT_31_SPRITESHEET_INDEX        0x802D4340
#define ENTITY_SLOT_31_TEXTURE_TO_PALETTE_LOOKUP           0x802D4440

#define ENTITY_SLOT_32_TEXTURE_1                0x802D46C0
#define ENTITY_SLOT_32_TEXTURE_2                0x802D5340
#define ENTITY_SLOT_32_PALETTE                  0x802D5FC0
#define ENTITY_SLOT_32_ANIM_METADATA            0x802D6A40
#define ENTITY_SLOT_32_SPRITESHEET_INDEX        0x802D7840
#define ENTITY_SLOT_32_TEXTURE_TO_PALETTE_LOOKUP           0x802D7940

#define ENTITY_SLOT_33_TEXTURE_1                0x802D7BC0
#define ENTITY_SLOT_33_TEXTURE_2                0x802D8840
#define ENTITY_SLOT_33_PALETTE                  0x802D94C0
#define ENTITY_SLOT_33_ANIM_METADATA            0x802D9F40
#define ENTITY_SLOT_33_SPRITESHEET_INDEX        0x802DAD40
#define ENTITY_SLOT_33_TEXTURE_TO_PALETTE_LOOKUP           0x802DAE40

#define ENTITY_SLOT_34_TEXTURE_1                0x802DB0C0
#define ENTITY_SLOT_34_TEXTURE_2                0x802DBD40
#define ENTITY_SLOT_34_PALETTE                  0x802DC9C0
#define ENTITY_SLOT_34_ANIM_METADATA            0x802DD440
#define ENTITY_SLOT_34_SPRITESHEET_INDEX        0x802DE240
#define ENTITY_SLOT_34_TEXTURE_TO_PALETTE_LOOKUP           0x802DE340

#define ENTITY_SLOT_35_TEXTURE_1                0x802C08C0
#define ENTITY_SLOT_35_TEXTURE_2                0x802C1540
#define ENTITY_SLOT_35_PALETTE                  0x802C21C0
#define ENTITY_SLOT_35_ANIM_METADATA            0x802C2C40
#define ENTITY_SLOT_35_SPRITESHEET_INDEX        0x802C3A40
#define ENTITY_SLOT_35_TEXTURE_TO_PALETTE_LOOKUP           0x802C3B40

#define ENTITY_SLOT_36_TEXTURE_1                0x802C3DC0
#define ENTITY_SLOT_36_TEXTURE_2                0x802C4A40
#define ENTITY_SLOT_36_PALETTE                  0x802C56C0
#define ENTITY_SLOT_36_ANIM_METADATA            0x802C6140
#define ENTITY_SLOT_36_SPRITESHEET_INDEX        0x802C6F40
#define ENTITY_SLOT_36_TEXTURE_TO_PALETTE_LOOKUP           0x802C7040

#define ENTITY_SLOT_37_TEXTURE_1                0x802C72C0
#define ENTITY_SLOT_37_TEXTURE_2                0x802C7F40
#define ENTITY_SLOT_37_PALETTE                  0x802C8BC0
#define ENTITY_SLOT_37_ANIM_METADATA            0x802C9640
#define ENTITY_SLOT_37_SPRITESHEET_INDEX        0x802CA440
#define ENTITY_SLOT_37_TEXTURE_TO_PALETTE_LOOKUP           0x802CA540

// ----------------------------------------------------------------------------
// Entity Slot 38 (Adult horse)
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_38_TEXTURE_1                0x802B6640
#define ENTITY_SLOT_38_TEXTURE_2                0x802B7648
#define ENTITY_SLOT_38_PALETTE                  0x802B8650
#define ENTITY_SLOT_38_ANIM_METADATA            0x802B8F40
#define ENTITY_SLOT_38_SPRITESHEET_INDEX        0x802B9B40
#define ENTITY_SLOT_38_TEXTURE_TO_PALETTE_LOOKUP           0x802B9C40

// ============================================================================
// Festival cutscene shared data
// ============================================================================

#define ENTITY_SLOTS_FESTIVAL_PALETTE           0x802C8650
#define ENTITY_SLOTS_FESTIVAL_ANIM_METADATA     0x802CAE50
#define ENTITY_SLOTS_FESTIVAL_SPRITESHEET       0x802CE350
#define ENTITY_SLOTS_FESTIVAL_TEXTURE_TO_PALETTE_LOOKUP    0x802CE550

#define ENTITY_SLOTS_COW_FESTIVAL_1_5_PALETTE        0x80275950
#define ENTITY_SLOTS_COW_FESTIVAL_1_5_ANIM_METADATA  0x80275A50
#define ENTITY_SLOTS_COW_FESTIVAL_1_5_SPRITESHEET    0x80278650
#define ENTITY_SLOTS_COW_FESTIVAL_1_5_TEXTURE_TO_PALETTE_LOOKUP 0x80278850

// ----------------------------------------------------------------------------
// Festival Slots 40-44
// ----------------------------------------------------------------------------
#define ENTITY_SLOT_FESTIVAL_40_TEXTURE_1       0x802C4A50
#define ENTITY_SLOT_FESTIVAL_40_TEXTURE_2       0x802C4E50
#define ENTITY_SLOT_FESTIVAL_41_TEXTURE_1       0x802C5A50
#define ENTITY_SLOT_FESTIVAL_41_TEXTURE_2       0x802C5E50
#define ENTITY_SLOT_FESTIVAL_42_TEXTURE_1       0x802C6250
#define ENTITY_SLOT_FESTIVAL_42_TEXTURE_2       0x802C6650
#define ENTITY_SLOT_FESTIVAL_43_TEXTURE_1       0x802C6A50
#define ENTITY_SLOT_FESTIVAL_43_TEXTURE_2       0x802C7850

#define ENTITY_SLOT_FESTIVAL_44_TEXTURE_1       0x802CEB50
#define ENTITY_SLOT_FESTIVAL_44_TEXTURE_2       0x802D1050
#define ENTITY_SLOT_FESTIVAL_44_PALETTE         0x802D3550
#define ENTITY_SLOT_FESTIVAL_44_ANIM_METADATA   0x802D3750
#define ENTITY_SLOT_FESTIVAL_44_SPRITESHEET     0x802D3850
#define ENTITY_SLOT_FESTIVAL_44_TEXTURE_TO_PALETTE_LOOKUP  0x802D3650

#endif
