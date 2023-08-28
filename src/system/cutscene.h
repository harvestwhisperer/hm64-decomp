#ifndef _CUTSCENE_H_
#define _CUTSCENE_H_

#include "common.h"

typedef struct {
    u16 *cutsceneMapPointer;
    short *unk_4;
    short *unk_8;
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
    u32 cameraFlags; // 0x30
    Vec3f coordiantes; // 0x34
    Vec3f scaling; // 0x40
    Vec3f offsets; // 0x4C
    Vec3f unk_58;
    u16 unk_64; // sprite/tile index
    u16 unk_66;
    u8 unk_68;
    u8 unk_69;
    u8 unk_6A;
    u16 flags; // 0x6C
} Cutscene;

#endif