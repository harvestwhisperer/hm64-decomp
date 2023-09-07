#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "common.h"

#define MAX_CUTSCENE_ASSETS 42

#define ACTIVE 1

typedef struct {
    u16 unk_0;
    short offset;
} CutscenePointersUpdate;

typedef struct {
    u32 unk_0;
    u32 romAddrStart;
    u32 romAddrEnd;
    void *vaddr;
} CutsceneDMA;

typedef struct {
    u16 unk_0;
    u16 index;
    u8 *songStart;
    u8 *songEnd;
} CutsceneSongInfo;

typedef struct {
    u16 unk_0;
    u16 index;
    u16 speed;
} CutsceneSongSpeedInfo;

typedef struct {
    u16 unk_0;
    u16 index;
    u16 maxVolume;
    s16 volume;
} CutsceneSongVolumeInfo;

typedef struct {
    u16 unk_0;
    u16 index;
    u16 volume;
} CutsceneSfxVolumeInfo;

typedef struct {
    u16 unk_0;
    u16 spriteIndex;
} CutsceneSpriteInfo;

typedef struct {
    u16 unk_0;
    u16 flag;
} CutsceneSpriteInfo2;

typedef struct {
    u16 unk_0;
    u16 index;
} CutsceneTileInfo;

typedef struct {
    u16 unk_0;
    u16 controllerIndex;
    u32 buttonMask;
} CutsceneButtonCheck;

typedef union {
    CutscenePointersUpdate pointerUpdate;
    CutsceneSpriteInfo spriteInfo;
    CutsceneSongInfo songInfo;
    CutsceneSongSpeedInfo songSpeed;
    CutsceneSongVolumeInfo songVolume;
    CutsceneSfxVolumeInfo sfxVolume;
    CutsceneTileInfo tileInfo;
    CutsceneButtonCheck buttonCheck;
} CutsceneContext;

typedef struct {
    // CutsceneContext *cutscenePointer;
    void *cutscenePointer;
    void *unk_4;
    void *unk_8;
    Vec3f unk_C;
    u16 sfxIndex; // 0x18
    u16 unk_1A;
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
    u16 spriteIndex; // sprite/tile index
    u16 unk_66;
    u8 unk_68;
    u8 unk_69;
    u8 unk_6A;
    u16 flags; // 0x6C
} CutsceneMap;

extern u32 gCutsceneFlags;

extern void initializeCutsceneMaps(void);
extern bool func_800469A8(u16 index, void *cutsceneMapPointer);
extern void func_80046BB8(void);   
extern void func_80046C98(void);     
extern void func_80046CF4(void);    
extern inline int func_80046D50(int adjustment, int value, int max);
extern void func_80046D78(void);

#endif