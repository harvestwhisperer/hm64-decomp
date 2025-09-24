#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "common.h"

#define MAX_BYTECODE_EXECUTORS 42

// TODO: figure out and add flag macros
#define CUTSCENE_ASSET_ACTIVE 1
#define CUTSCENE_SPRITE_ASSET 2
#define CUTSCENE_ENTITY_ASSET 8
#define CUTSCENE_MAP_ASSET 0x10
#define CUTSCENE_PAUSE_EXECUTION 0x20

/* general */

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
    s16 offset;
    u16 unused;
} CutsceneExecutorInitializationCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
    s16 offset;
} CutsceneExecutorSetPtrCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
} CutsceneExecutorPauseExecutorCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
} CutsceneExecutorTogglePauseExecutorCmd;

typedef struct {
    u16 functionIndex;
} cutsceneExecutorPauseAllChildExecutorsCmd;

typedef struct {
    u16 functionIndex;
} cutsceneExecutorTogglePauseAllChildExecutorsCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
} CutsceneDeactivateExecutorCmd;

typedef struct {
    u16 functionIndex;
} CutsceneDeactivateSelfCmd;

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
    s16 offset;
    u16 unused;
} CutsceneIndexCheckCmd;

typedef struct {
    u16 functionIndex;
    s16 offset;
} CutsceneJumpCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 cutsceneIndex;
    u32* cutsceneIndexPtr;
} CutsceneUpdateCutsceneIndexCmd;

// func_80049E84
typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16 unk_4;
    s16 unk_8;
} CutsceneUnknownCmd;

/* gameStatus */

// func_80049064
typedef struct {
    u16 functionIndex;
    u16 bit;
    u32* bitfield;
} CutsceneBranchOnSpecialBitCmd;

typedef struct {
    u16 functionIndex;
    u16 bit;
    s32* bitfield;
} CutsceneSetSpecialBitCmd;

typedef struct {
    u16 index;
    u16 bit;
    u32* bitfield;
} CutsceneToggleSpecialBitToggleCmd;

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
    u16 songIndex;
    u8 *songStart;
    u8 *songEnd;
} CutsceneAudioSetSongCmd;

typedef struct {
    u16 functionIndex;
    u16 songIndex;
} CutsceneAudioIdleWhilePlayingCmd;

typedef struct {
    u16 functionIndex;
    u16 songIndex;
    u16 speed;
} CutsceneAudioStopSongWithFadeOutCmd;

typedef struct {
    u16 functionIndex;
    u16 songIndex;
    u16 targetVolume;
    s16 volume;
} CutsceneAudioSetSongVolumeCmd;

typedef struct {
    u16 functionIndex;
    u16 sfxIndex;
    u16 volume;
} CutsceneAudioSetSfxCmd;

/* controller */

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneBranchOnCurrentButton;

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneBranchOnButtonPressed;

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneBranchOnButtonRepeat;

/* dialogue */

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
} CutsceneMessageBoxIndexCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;
} CutsceneMessageBoxInitializeType1Cmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;
} CutsceneMessageBoxInitializeType2Cmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    u8 r;
    u8 b;
    u8 g;
    u8 a;
    s32 flags;
} CutsceneMessageBoxRGBACmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u8 dialogueWindowIndex;
} CutsceneMessageBoxSetupCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    s16 x;
    s16 y;
    s16 z;
} CutsceneMessageBoxViewSpaceCmd;

// func_80049A28
typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
} CutsceneUnknownMessageBoxCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
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
    u16 dialogueBytecodeAddressIndex;
    u16 dialogueIndex;
} CutsceneDialogueInitializeCmd;

/* entity */

typedef struct {
    u16 functionIndex;
    u16 entityAssetIndex;
    u16 flags;
} CutsceneEntitySetShadowFlagsCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutsceneEntitySetDirectionCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u16 animationIndex1;
    u16 animationIndex2;
    u16 animationIndex3;
    u16 animationIndex4;
    u16 animationIndex5;
    u16 animationIndex6;
} CutsceneEntitySetAnimationsCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u16 entityAssetIndex;
    u16 flag;
} CutsceneEntityLoadCmd;

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
    u16 flag;
} CutsceneSetEntityUnknownFlagCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u8 targetDirecton;
} CutsceneEntityBranchOnEntityDirectionCmd;

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
    u8 x;
    u8 z;
} CutsceneMapSetMapAdditionsCmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u8 x;
    u8 z;
} CutsceneMapSetGroundObjectCmd;

/* mapController */

typedef struct {
    u16 functionIndex;
    u16 mapDataIndex;
    u16 mainMapIndex;
} CutsceneMapControllerCmd;

typedef struct {
    u16 functionIndex;
    u16 mapDataIndex;
    u16 mainMapIndex;
} CutsceneMapControllerLoadMapCmd;

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

/* message */

// func_8004CCF0
typedef struct {
    u16 functionIndex;
    s16 rate;
    u8 messageBoxIndex;
    u8 flag;
} CutsceneMessageSetInterpolationRate;

/* sprite */

typedef struct {
    u16 functionIndex;
    u16 paletteIndex;
} CutsceneSpriteSetPaletteCmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
} CutsceneSpriteCmd1;

typedef struct {
    u16 functionIndex;
    u16 renderingLayer;
} CutsceneSpriteSetRenderingLayerCmd;

typedef struct {
    u16 functionIndex;
    u16 useBilinearFilterng;
} CutsceneSpriteSetBilinearFilteringCmd;

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
    u32 unk_8;
    u32 unk_C;
} CutsceneUnknownCmd7;

// 0x801808B0
typedef struct {
    void *bytecodePtr;
    void *subroutineBytecodePtr;
    void *returnPtr;
    Vec3f targetPosition;
    u32 callbackBytecodePtr; // when branching based on button interaction with entity
    u16 entityCollidedWithIndex;
    u16 targetButtonMask;
    u16 animationIndex1;
    u16 animationIndex2;
    u16 animationIndex3;
    u16 animationIndex4;
    u16 animationIndex5;
    u16 animationIndex6;
    u16 collisionWidth;
    u16 collisionHeight;
    u16 cameraFlags; // 0x30
    Vec3f coordinates; // 0x34
    Vec3f scale; // 0x40
    Vec3f frameDelta; // 0x4C
    Vec3f movementVector;
    u16 assetIndex; // sprite or map context index
    u16 waitFrames;
    u8 animationFrameCounter;
    u8 entityDirectionOrMapRotation;
    u8 movementDistance;
    u16 flags; // 0x6C
} CutsceneExecutor;

// can't get a consistent match across files for this variable; some functions need array/struct loading, which breaks the match in other functions
extern u32 gCutsceneFlags;
extern u32 cutsceneFlagsHack[2];

extern void initializeCutsceneExecutors(void);
extern void updateCutsceneExecutors(void);
extern bool spawnCutsceneExecutor(u16 index, void *bytecodePtr);
extern void deactivateCutsceneExecutors(void);   
extern void func_80046C98(void);     
extern void func_80046CF4(void);    
extern inline int func_80046D50(int adjustment, int value, int max);

#endif