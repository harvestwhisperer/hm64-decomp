#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "common.h"

#define MAX_CUTSCENE_ASSETS 42

#define ACTIVE 1

typedef struct {
    u16 functionIndex;
    short offset;
} CutscenePointersUpdate;

typedef struct {
    u16 functionIndex;
    u16 unk_2;
    u32 romAddrStart;
    u32 romAddrEnd;
    void *vaddr;
} CutsceneDMA;

typedef struct {
    u16 functionIndex;
    u16 paletteIndex;
} CutscenePaletteInfo;

typedef struct {
    u16 functionIndex;
    u16 index;
    u8 *songStart;
    u8 *songEnd;
} CutsceneSongInfo;

typedef struct {
    u16 functionIndex;
    u16 index;
    u16 speed;
} CutsceneSongSpeedInfo;

typedef struct {
    u16 functionIndex;
    u16 index;
    u16 maxVolume;
    s16 volume;
} CutsceneSongVolumeInfo;

typedef struct {
    u16 functionIndex;
    u16 index;
    u16 volume;
} CutsceneSfxVolumeInfo;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
} CutsceneSpriteInfo;

typedef struct {
    u16 functionIndex;
    u16 flag;
} CutsceneSpriteInfo2;

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
} CutsceneSpriteInfo3;

typedef struct {
    u16 functionIndex;
    u16 index;
} CutsceneMapInfo;

typedef struct {
    u16 functionIndex;
    u16 value;
    u8 index1;
    u8 index2;
} CutsceneMapStruct6Info;

typedef struct {
    u16 functionIndex;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneButtonCheck;

typedef struct {
    u16 functionIndex;
    u16 cutsceneMapIndex;
} CutsceneAssetFlagsInfo;

typedef struct {
    u16 functionIndex;
    u16 structIndex;
    u16 flag;
} CutsceneMapStruct7Info;

typedef struct {
    u16 functionIndex;
    u16 spriteIndex;
    u16 flags;
} CutsceneCharacterSpriteInfo;

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
} CutsceneSpriteAddressInfo;

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
} CutsceneSpriteAddressInfo;
*/

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
} CutsceneDialogueBoxInfo;

typedef struct {
    u16 functionIndex;
    u16 dialogueBoxIndex;
    u8 r;
    u8 b;
    u8 g;
    u8 a;
    s32 flags;
} CutsceneDialogueBoxRGBAInfo;

typedef struct {
    u8 functionIndex;
    u8 unk_1;
    u8 unk_2;
    u8 nextFunctionIndex;
} CutsceneUnknownInfo;

typedef union {
    CutscenePointersUpdate pointerUpdate;
    CutsceneSpriteInfo spriteInfo;
    CutsceneSongInfo songInfo;
    CutsceneSongSpeedInfo songSpeed;
    CutsceneSongVolumeInfo songVolume;
    CutsceneSfxVolumeInfo sfxVolume;
    CutsceneMapInfo tileInfo;
    CutsceneButtonCheck buttonCheck;
} CutsceneContext;

// 0x801808B0
typedef struct {
    // CutsceneContext *cutscenePointer;
    void *cutscenePointer;
    void *unk_4;
    void *unk_8;
    Vec3f unk_C;
    u32 unk_18; // 0x18
    u16 unk_1C; // sprite info
    u16 unk_1E; // sprite info
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
    u16 spriteIndex; // sprite or map context index
    u16 unk_66; // counter for callback function loop
    u8 unk_68;
    u8 unk_69;
    u8 unk_6A;
    u16 flags; // 0x6C
} CutsceneMap;

// can't get a consistent match across files for this variable; some functions need array/struct loading, which breaks the match in other functions
extern u32 gCutsceneFlags;
extern u32 cutsceneFlagsHack[2];

extern void initializeCutsceneMaps(void);
extern void mainLoopCutsceneHandler(void);
extern bool func_800469A8(u16 index, void *cutsceneMapPointer);
extern void func_80046BB8(void);   
extern void func_80046C98(void);     
extern void func_80046CF4(void);    
extern inline int func_80046D50(int adjustment, int value, int max);

#endif