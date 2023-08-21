#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "common.h"
#include "audio.h"

typedef struct {
    void *unk_0;
    Vec4f unk_4;
    Vec4f unk_14;
    Vec4f unk_24;
    u32 unk_34[4];
    void *unk_44;
    void *unk_48;
    Vec3f unk_4C;
    void *unk_58;
    /* sfx struct */
    void *unk_5C;
    u8 unk_60;
    u8 unk_61;
    u16 unk_62;
    u16 unk_63;
    u16 frameCounter; // 0x64
    u16 unk_65;
    Volume volume; // 0x6C
    u16 unk_7C;
    u16 unk_7E;
    u16 unk_80;
    u16 unk_82;
    u16 unk_84;
    u16 unk_86;
    u16 unk_88;
    u32 unk_8C;
    u8 unk_90;
    u8 unk_91;
    u16 unk_92;
    u8 buttonSfxCounter; // 0x94
    u8 flag; // 0x95
    u8 margins; // 0x96
    u8 maxLinesInBox; // 0x97
    u8 currentLineFromTop; // 0x98
    u8 maxLinesInText; // 0x99
    u8 charPerLineCount; // 0x9A
    u8 currentLine; // 0x9B
    u8 unk_9C;
    u8 unk_9D;
    u8 unk_9E;
    u8 unk_9F;
    u8 unk_A0;
    u8 unk_A1;
    u16 unk_A2;
    u32 flags; // 0xA4
} DialogueBox;

#endif