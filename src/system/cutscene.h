#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "common.h"

#define MAX_BYTECODE_EXECUTORS 42

// TODO: figure out and add flag macros
#define ACTIVE 1
#define CHARACTER_SPRITE_ASSET 8
#define MAP_ASSET 0x10

/* general */

typedef struct {
    u16 functionIndex;
    u16 assetIndex;
    short offset; // offset from main cutscene object ptr
} CutsceneExecutorInitializationCmd;

typedef struct {
    u16 functionIndex;
    u16 CutsceneExecutorIndex;
} CutsceneAssetFlagsCmd;

typedef struct {
    u16 functionIndex;
    u8 cameraFlags;
} CutsceneCameraFlagsCmd;

// func_8004B920
typedef struct {
    u16 functionINdex;
    u16 unk_2;
} CutsceneCameraFlagsUpdateCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 romAddrStart;
    u32 romAddrEnd;
    void *vaddr;
} CutsceneDMACmd;

typedef struct {
    u16 functionIndex;
    u16 cutsceneIndex1;
    u16 cutsceneIndex2;
    u16 unk_6;
    u16* cutsceneIndexPtr;
    u16 unk_C;
    u16 padding;
} CutsceneIndexCheckCmd;

typedef struct {
    u16 functionIndex;
    short offset;
} CutsceneJumpCmd;

// func_80049E84
typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16 unk_4;
    s16 unk_8;
} CutsceneUnknownCmd;

// func_80049064
typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32* unk_4;
} CutsceneUnknown2Cmd;

/* gameStatus */

typedef struct {
    u16 index;
    u16 bit;
    u32* bitfield;
} CutsceneSpecialBitToggleCmd;

// func_8004AB04
typedef struct {
    u16 functionIndex;
    s16 value;
    s16 max;
    u8* valuePtr;
} CutsceneU8UpdateCmd;

// func_8004ABA8
typedef struct {
    u16 functionIndex;
    s16 value;
    u16 max;
    u16* valuePtr;
} CutsceneU16UpdateCmd;

// func_8004AC4C
typedef struct {
    u16 functionIndex;
    u32 value;
    u32 max;
    u32* valuePtr;
} CutsceneU32UpdateCmd;

typedef struct {
    u16 index;
    u16 frames;
} CutsceneWaitFramesCmd;

/* general graphics */

// func_8004A47C
typedef struct {
    u16 functionIndex;
    u8 r, g, b, a;
} CutsceneSetColorsCmd;

// func_8004A7A4
typedef struct {
    u16 functionIndex;
    u8 r, g, b, a;
    s16 rate;
} CutsceneUpdateRGBACmd;

// func_8004AD44
typedef struct {
    u16 functionIndex;
    u8 r, g, b, a;
    s16 rate;
} CutsceneUpdateRGBA2Cmd;

/* audio */

typedef struct {
    u16 functionIndex;
    u16 index;
    u8 *songStart;
    u8 *songEnd;
} CutsceneSongCmd;

typedef struct {
    u16 functionIndex;
    u16 index;
    u16 speed;
} CutsceneSongSpeedCmd;

typedef struct {
    u16 functionIndex;
    u16 index;
    u16 maxVolume;
    s16 volume;
} CutsceneSongVolumeCmd;

typedef struct {
    u16 functionIndex;
    u16 index;
    u16 volume;
} CutsceneSfxVolumeCmd;

/* controller */

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneButtonCmd;

/* dialogue */

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
} CutsceneDialogueBoxIndexCmd;

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
    u16 dialogueBytecodeUnk14;
    u16 dialogueStruct1Unk0;
} CutscenInitializeDialogueBoxCmd;

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
    u8 r;
    u8 b;
    u8 g;
    u8 a;
    s32 flags;
} CutsceneDialogueBoxRGBACmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u8 dialogueWindowIndex;
} CutsceneDialogueBoxSetupCmd;

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
    s16 x;
    s16 y;
    s16 z;
} CutsceneDialogueBoxViewSpaceCmd;

// func_80049A28
typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
} CutsceneUnknownDialogueBoxCmd;

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
} CutsceneDialogueWaitCmd;

// func_8004B498
typedef struct {
    u16 functionIndex;
    u16 dialoguesIndex;
} CutsceneDialoguesWaitCmd;

// func_8004B538
typedef struct {
    u16 functionIndex;
    u16 dialoguesIndex;
    u16 unk;
} CutsceneDialoguesUnknownCmd;

// func_8004B410
typedef struct {
    u16 functionIndex;
    u16 dialoguesIndex;
    u16 dialogueMapAddressIndex;
    u16 dialogueIndex;
} CutsceneDialogueSpritesDMACmd;

/* entity */

typedef struct {
    u16 functionIndex;
    u16 entityAssetIndex;
    u16 flags;
} CutsceneEntitySetShadowFlagsCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutsceneEntityCmd1;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
    u16 unk_C;
    u16 unk_E;
    u16 unk_10;
} CutsceneEntityCmd2;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u16 entityAssetIndex;
    u16 flag;
} CutsceneEntityCmd3;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u16 collisionWidth;
    u16 collisionHeight;
} CutsceneEntityCollisionCheckCmd;

// func_80049F40
typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutsceneEntityPauseCmd;

// func_80049FA0
typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutsceneEntityTogglePauseCmd;

// func_80049D64
typedef struct {
    u16 functionIndex;
    u16 animation;
} CutsceneEntitySetAnimationCmd;

// func_80049DF4
typedef struct {
    u16 functionIndex;
    u16 animation;
} CutsceneEntitySetAnimationWithDirectionCmd;

// func_8004910C
typedef struct {
    u16 functionIndex;
    u8 direction;
} CutsceneEntityDirectionCmd;

typedef struct {
    u16 functionIndex;
    u16 paletteIndex;
} CutsceneEntitySetPaletteIndexCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
} CutsceneEntitySetFlagsCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u8 targetDirecton;
} CutscenEntityCheckDirectionCmd;

// func_8004B7B8
typedef struct {
    u16 functionIndex;
    u16 targetSpriteIndex;
    s16 arg1;
    s16 arg2;
    s16 arg3;
    u8 trackingMode;
} CutsceneEntityUnknownCmd;

/* map */

typedef struct {
    u16 functionIndex;
    u16 mapAdditionIndex;
    u16 flag;
} CutsceneMapAdditionsCmd;

// func_8004CB88
typedef struct {
    u16 functionIndex;
    u16 mapAdditionIndex;
    u8 arg2;
    u8 arg3;
} CutsceneMapSetMapAdditionsCmd;

typedef struct {
    u16 functionIndex;
    u16 value;
    u8 index1;
    u8 index2;
} CutsceneMapStruct6Cmd;

typedef struct {
    u16 functionIndex;
    u16 flag;
    s32* cutsceneFlagPointer;
} CutsceneMapFlagCmd;

/* mapControllr */

typedef struct {
    u16 functionIndex;
    u16 mapModelIndex;
    u16 mainMapIndex;
} CutsceneMapControllerCmd;

typedef struct {
    u16 functionIndex;
    u16 index;
} CutsceneMapControllerCmd2;

// func_8004ACE4
typedef struct {
    u16 functionIndex;
    u16 mapIndex;
} CutsceneMapControllerCmd3;

typedef struct {
    u16 functionIndex;
    u16 mapIndex;
    u8 arg1;
    u8 rotation;
} CutsceneMapControllerSetRotationFlagCmd;

/* sprite */

typedef struct {
    u16 functionIndex;
    u16 paletteIndex;
} CutscenePaletteCmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
} CutsceneSpriteCmd1;

typedef struct {
    u16 functionIndex;
    u16 flag;
} CutsceneSetBilinearFilteringCmd;

// func_8004BE88
typedef struct {
    u16 functionIndex;
    s16 x, y, z;
} CutsceneSpriteSetScaleCmd;

/* spriteDMAManager */

typedef struct  {
    // u16 functionIndex;
    // u16 spriteIndex
    // u16 unk_4
    u16 assetType;
    u32 romTextureStart; 
    u32 romTextureEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u32 romSpritesheetIndexStart; 
    u32 romSpritesheetIndexEnd; 
    u32 texture1Vaddr;
    u32 texture2Vaddr;
    u32 paletteVaddr;
    u32 animationVaddr;
    u32 spriteToPaletteVaddr;
    u32 spritesheetIndexVaddr;
} CutsceneSpriteDMAManagerCmd;

/*
typedef struct  {
    u16 functionIndex;
    u16 spriteIndex; 
    u16 assetType; 
    u32 romTextureStart; 
    u32 romTextureEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u32 romSpritesheetIndexStart; 
    u32 romSpritesheetIndexEnd; 
    u32 texture1Vaddr;
    u32 texture2Vaddr;
    u32 paletteVaddr;
    u32 animationVaddr;
    u32 spriteToPaletteVaddr;
    u32 spritesheetIndexVaddr;
} CutsceneSpriteDMAManagerCmd;
*/

typedef struct {
    u16 functionIndex;
    u16 renderingLayer;
} CutsceneSetSpriteRenderingLayerCmd;

/* unknown */

typedef struct {
    u8 functionIndex;
    u8 unk_1;
    u8 unk_2;
    u8 nextFunctionIndex;
} CutsceneUnknownCmd1;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16* unk_4;
} CutsceneUnknownCmd2;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 unk_4;
    u32 unk_8;
    u32* unk_C;
} CutsceneUnknownCmd3;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16 unk_4;
} CutsceneUnknownCmd4;

typedef struct {
    u16 functionIndex;
    u16* unk_4;
    u16* unk_8;
    u32 unk_C;
} CutsceneUnknownCmd5;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 unk_4;
    u32* unk_8;
} CutsceneUnknownCmd6;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
} CutsceneUnknownCmd7;

// 0x801808B0
typedef struct {
    void *bytecodePtr;
    void *unk_4;
    void *unk_8; // return ptr
    Vec3f unk_C;
    u32 unk_18; // sets cutscenePointer
    u16 unk_1C; // entities, entities.entityCollidedWithIndex
    u16 unk_1E; // entities,  entities.buttonPressed
    u16 unk_20; // sprite animation
    u16 unk_22; // sprite animation
    u16 unk_24; // sprite animation
    u16 unk_26; // sprite animation
    u16 unk_28; // sprite animation
    u16 unk_2A; // sprite animation
    u16 unk_2C;
    u16 unk_2E;
    u16 cameraFlags; // 0x30
    Vec3f coordinates; // 0x34
    Vec3f scaling; // 0x40
    Vec3f offsets; // 0x4C
    Vec3f unk_58;
    u16 assetIndex; // sprite or map context index
    u16 waitFrames; // is asset active, does callback function loop
    u8 unk_68;
    u8 unk_69;
    u8 unk_6A;
    u16 flags; // 0x6C
} CutsceneExecutor;

// can't get a consistent match across files for this variable; some functions need array/struct loading, which breaks the match in other functions
extern u32 gCutsceneFlags;
extern u32 cutsceneFlagsHack[2];

extern void initializeCutsceneExecutors(void);
extern void updateCutsceneExecutors(void);
extern bool spawnCutsceneExecutor(u16 index, void *bytecodePtr);
extern void func_80046BB8(void);   
extern void func_80046C98(void);     
extern void func_80046CF4(void);    
extern inline int func_80046D50(int adjustment, int value, int max);

#endif