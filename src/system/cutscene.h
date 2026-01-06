#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "common.h"

#define MAX_BYTECODE_EXECUTORS 42

#define CUTSCENE_ASSET_ACTIVE 1
#define CUTSCENE_SPRITE_ASSET 2
#define CUTSCENE_ANIMATION_LOOP 4
#define CUTSCENE_ENTITY_ASSET 8
#define CUTSCENE_MAP_ASSET 0x10
#define CUTSCENE_PAUSE_EXECUTION 0x20

#define CUTSCENE_ASSET_BEHAVIOR_TRACK_ENTITY 1
#define CUTSCENE_ASSET_BEHAVIOR_WANDER 2
#define CUTSCENE_ASSET_BEHAVIOR_DIRECTION_MOVEMENT 4
#define CUTSCENE_ASSET_BEHAVIOR_DIRECT_MOVEMENT 8
#define CUTSCENE_ASSET_BEHAVIOR_ALTERNATE_ANIMATIONS 0x10

/* general */

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
    s16 offset;
    u16 unused;
} CutsceneSpawnExecutorCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
    s16 offset;
} CutsceneSetOtherExecutorPtrCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
} CutscenePauseExecutorCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
} CutsceneTogglePauseExecutorCmd;

typedef struct {
    u16 functionIndex;
} CutscenePauseAllChildExecutorsCmd;

typedef struct {
    u16 functionIndex;
} CutsceneTogglePauseAllChildrenCmd;

typedef struct {
    u16 functionIndex;
    u16 executorIndex;
} CutsceneDeactivateExecutorCmd;

typedef struct {
    u16 functionIndex;
} CutsceneDeactivateSelfCmd;

typedef struct {
    u16 functionIndex;
    u8 behaviorFlags;
} CutsceneSetBehaviorFlagsCmd;

typedef struct {
    u16 functionIndex;
    u16 useAltAnimations;
} CutsceneSetAltAnimationFlagCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 romAddrStart;
    u32 romAddrEnd;
    void *vaddr;
} CutsceneDMACmd;

typedef struct {
    u16 functionIndex;
    s16 offset;
} CutsceneSetAnimDataPtrCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 cutsceneIndex;
    u32* cutsceneIndexPtr;
} CutsceneSetU32ValueCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16 unk_4;
    s16 unk_8;
} CutsceneUnknownCmd;

/* gameStatus */

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
} CutsceneClearSpecialBitCmd;

typedef struct {
    u8 variablePtr;
    u8 unk_1;
    u8 unk_2;
    u8 value;
} CutsceneSetU8ValueCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16* unk_4;
} CutsceneSetU16ValueCmd;

typedef struct {
    u16 functionIndex;
    s16 value;
    s16 max;
    u8* valuePtr;
} CutsceneU8UpdateCmd;

typedef struct {
    u16 functionIndex;
    s16 value;
    u16 max;
    u16* valuePtr;
} CutsceneU16UpdateCmd;

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

typedef struct {
    u16 functionIndex;
    u8 r, g, b, a;
} CutsceneSetRgbaCmd;

typedef struct {
    u16 functionIndex;
    u8 r, g, b, a;
    s16 rate;
} CutsceneUpdateRGBACmd;

typedef struct {
    u16 functionIndex;
    u8 r, g, b, a;
    s16 rate;
} CutsceneUpdateGlobalRgbaCmd;

/* audio */

typedef struct {
    u16 functionIndex;
    u16 sequenceIndex;
    u8 *sequenceStart;
    u8 *sequenceEnd;
} CutsceneSetAudioSequenceCmd;

typedef struct {
    u16 functionIndex;
    u16 sequenceIndex;
} CutsceneIdleWhileSequencePlayingCmd;

typedef struct {
    u16 functionIndex;
    u16 sequenceIndex;
    u16 speed;
} CutsceneStopSequenceWithFadeCmd;

typedef struct {
    u16 functionIndex;
    u16 sequenceIndex;
    u16 targetVolume;
    s16 volume;
} CutsceneSetAudioSequenceVolumeCmd;

typedef struct {
    u16 functionIndex;
    u16 sfxIndex;
    u16 volume;
} CutsceneSetSfxCmd;

/* controller */

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneBranchOnCurrentButtonCmd;

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneBranchOnButtonPressedCmd;

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneBranchOnButtonRepeatCmd;

/* dialogue */

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
} CutsceneWaitMessageBoxReadyCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;
} CutsceneInitMessageBoxType1Cmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    u16 textAddressesIndex;
    u16 textIndex;
} CutsceneInitMessageBoxType2Cmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    u8 r;
    u8 b;
    u8 g;
    u8 a;
    s32 flags;
} CutsceneUpdateMessageBoxRgbaCmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u8 dialogueWindowIndex;
} CutsceneSetMessageBoxAssetsCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
    s16 x;
    s16 y;
    s16 z;
} CutsceneSetMessageBoxPositionCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
} CutsceneResetMessageBoxAvatarCmd;

typedef struct {
    u16 functionIndex;
    u16 messageBoxIndex;
} CutsceneWaitMessageBoxClosedCmd;

typedef struct {
    u16 functionIndex;
    u16 dialoguesIndex;
} CutsceneWaitForDialogueInputCmd;

typedef struct {
    u16 functionIndex;
    u16 dialoguesIndex;
    u16 unk;
} CutsceneBranchOnDialogueCmd;

typedef struct {
    u16 functionIndex;
    u16 dialoguesIndex;
    u16 dialogueBytecodeAddressIndex;
    u16 dialogueIndex;
} CutsceneInitDialogueSessionCmd;

/* entity */

typedef struct {
    u16 functionIndex;
    u16 entityAssetIndex;
    u16 flags;
} CutsceneSetShadowFlagsCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutsceneFlipEntityDirectionCmd;

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
} CutsceneSetupEntityCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u16 collisionWidth;
    u16 collisionHeight;
} CutsceneCheckEntityCollisionCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutscenePauseEntityCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
} CutsceneTogglePauseEntityCmd;

typedef struct {
    u16 functionIndex;
    u16 animation;
} CutsceneEntitySetAnimationCmd;

typedef struct {
    u16 functionIndex;
    u16 animation;
} CutsceneEntitySetAnimationWithDirectionCmd;

typedef struct {
    u16 functionIndex;
    u8 direction;
} CutsceneSetAssetRotationCmd;

typedef struct {
    u16 functionIndex;
    u16 paletteIndex;
} CutsceneSetEntityPaletteCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
} CutsceneSetEntityPhysicsFlagsCmd;

typedef struct {
    u16 functionIndex;
    u16 flag;
} CutsceneSetEntityMapSpaceIndependentFlagCmd;

typedef struct {
    u16 functionIndex;
    u16 entityIndex;
    u8 targetDirecton;
} CutsceneBranchOnEntityDirectionCmd;

typedef struct {
    u16 functionIndex;
    u16 targetEntityIndex;
    s16 arg1;
    s16 arg2;
    s16 arg3;
    u8 trackingMode;
} CutsceneSetEntityTrackingTargetCmd;

/* map */

typedef struct {
    u16 functionIndex;
    u16 mapAdditionIndex;
    u16 flag;
} CutsceneInitMapAdditionCmd;

typedef struct {
    u16 functionIndex;
    u16 mapAdditionIndex;
    u8 x;
    u8 z;
} CutsceneSetMapAdditionCmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u8 x;
    u8 z;
} CutsceneSetMapGroundObjectCmd;

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
} CutsceneLoadMapCmd;

typedef struct {
    u16 functionIndex;
    u16 index;
} CutsceneMapControllerIndexCmd;

typedef struct {
    u16 functionIndex;
    u16 mapIndex;
} CutsceneDeactivateMapObjectsCmd;

typedef struct {
    u16 functionIndex;
    u16 mapIndex;
    u8 arg1;
    u8 rotation;
} CutsceneSetMapRotationCmd;

/* message */

typedef struct {
    u16 functionIndex;
    s16 rate;
    u8 messageBoxIndex;
    u8 flag;
} CutsceneSetMessageInterpolationCmd;

/* sprite */

typedef struct {
    u16 functionIndex;
    u16 paletteIndex;
} CutsceneSetSpritePaletteCmd;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
} CutsceneDmaSpriteIndexCmd;

typedef struct {
    u16 functionIndex;
    u16 renderingLayer;
} CutsceneSetSpriteRenderingLayerCmd;

typedef struct {
    u16 functionIndex;
    u16 useBilinearFilterng;
} CutsceneSetSpriteBilinearCmd;

typedef struct {
    u16 functionIndex;
    s16 x, y, z;
} CutsceneSetSpriteScaleCmd;

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

/* Branch on value */

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u16 unk_4;
} CutsceneBranchOnRandomCmd;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
} CutsceneBranchIfU8PtrInRangeCmd;

typedef struct {
    u16 functionIndex;
    u16* unk_4;
    u16* unk_8;
    u32 unk_C;
} CutsceneBranchIfU16PtrInRangeCmd;

typedef struct {
    u16 functionIndex;
    u8 min;
    u8 max;
    u8* variablePtr;
    s16 offset;
    u16 unused;
} CutsceneBranchU8VariableWithinRange;

typedef struct {
    u16 functionIndex;
    u16 min;
    u16 max;
    u16 unk_6;
    u16* variablePtr;
    s16 offset;
    u16 unused;
} CutsceneBranchU16VariableWithinRange;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 min;
    u32 max;
    u32* variablePtr;
} CutsceneBranchU32VariableWithinRange;

// 0x801808B0
typedef struct {
    void *bytecodePtr;
    void *animationDataPtr;
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
    u16 behaviorFlags;
    Vec3f coordinates;
    Vec3f scale;
    Vec3f frameDelta;
    Vec3f movementVector;
    u16 assetIndex; // sprite or map context index
    u16 waitFrames;
    u8 animationFrameCounter;
    u8 entityDirectionOrMapRotation;
    u8 movementDistance;
    u16 flags;
} CutsceneExecutor;

extern void initializeCutsceneExecutors(void);
extern void updateCutsceneExecutors(void);
extern bool spawnCutsceneExecutor(u16 index, void *bytecodePtr);
extern void deactivateCutsceneExecutors(void);   
extern void pauseAllCutsceneExecutors(void);     
extern void resumeCutsceneExecutors(void);    
extern inline int func_80046D50(int adjustment, int value, int max);

#endif