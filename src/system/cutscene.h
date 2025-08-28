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
    u8 r;
    u8 b;
    u8 g;
    u8 a;
    s32 flags;
} CutsceneDialogueBoxRGBACmd;

/* entity */

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u16 flags;
} CutsceneCharacterSpriteCmd;

/* map */

typedef struct {
    u16 functionIndex;
    u16 mapAdditionIndex;
    u16 flag;
} CutsceneMapAdditionsCmd;

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

/* mapContext */

typedef struct {
    u16 functionIndex;
    u16 mapModelContextIndex;
    u16 mainMapIndex;
} CutsceneMapModelContextCmd;

typedef struct {
    u16 functionIndex;
    u16 index;
} CutsceneMapModelContextCmd2;

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

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
    u16 unk_C;
    u16 unk_E;
    u16 unk_10;
} CutsceneSpriteCmd3;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u16 characterIndex;
    u16 flag;
} CutsceneSpriteCmd4;

/* spriteDMAManager */

typedef struct  {
    // u16 functionIndex;
    // u16 spriteIndex
    // u16 unk_4
    u16 assetType;
    u32 romTextureStart; 
    u32 romTextureEnd;
    u32 romAssetIndexStart;
    u32 romAssetIndexEnd;
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
    u32 romAssetIndexStart;
    u32 romAssetIndexEnd;
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
    u32* unk_8;
} CutsceneUnknownCmd6;

// 0x801808B0
typedef struct {
    void *bytecodePtr;
    void *unk_4;
    void *unk_8; // return ptr
    Vec3f unk_C;
    u32 unk_18; // sets cutscenePointer
    u16 unk_1C; // sprite info, entities.unk_58
    u16 unk_1E; // sprite info,  entities.unk_5A
    u16 unk_20; // sprite animation
    u16 unk_22; // sprite animation
    u16 unk_24; // sprite animation
    u16 unk_26; // sprite animation
    u16 unk_28; // sprite animation
    u16 unk_2A; // sprite animation
    u32 unk_2C;
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