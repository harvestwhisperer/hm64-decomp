#include "common.h"

#include "system/dialogue.h"
#include "system/message.h"

// bss
extern u16 D_80204BF0[];
DialogueBox dialogueBoxes[MAX_DIALOGUE_BOXES];
extern GameVariableReference D_80189E58[64];

// forward delcarations
extern bool func_8003FDB0(u16);      
extern void func_80040628(u16, u8); 
extern u16 func_80041850(u16);

INCLUDE_ASM(const s32, "system/message", func_8003D970);

INCLUDE_ASM(const s32, "system/message", func_8003DBE8);

INCLUDE_ASM(const s32, "system/message", func_8003DD14);

INCLUDE_ASM(const s32, "system/message", func_8003DDF8);

INCLUDE_ASM(const s32, "system/message", func_8003E77C);

INCLUDE_ASM(const s32, "system/message", func_8003E910);

INCLUDE_ASM(const s32, "system/message", func_8003EA1C);

INCLUDE_ASM(const s32, "system/message", func_8003EE7C);
 
INCLUDE_ASM(const s32, "system/message", func_8003EFD8);

INCLUDE_ASM(const s32, "system/message", func_8003F024);

INCLUDE_ASM(const s32, "system/message", func_8003F0DC);

INCLUDE_ASM(const s32, "system/message", func_8003F130);

INCLUDE_ASM(const s32, "system/message", func_8003F28C);

//INCLUDE_ASM(const s32, "system/message", func_8003F30C);

bool func_8003F30C(u16 dialogueIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 textBufferVaddr) {

    bool result = 0;

    if (dialogueIndex < MAX_DIALOGUE_BANKS) {

        D_80183200[dialogueIndex].romIndexStart = romIndexStart;
        D_80183200[dialogueIndex].romIndexEnd = romIndexEnd;
        D_80183200[dialogueIndex].textBufferVaddr = textBufferVaddr;
        D_80183200[dialogueIndex].romTextStart = romTextStart;
        
        result = 1;
        
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003F360);

INCLUDE_ASM(const s32, "system/message", func_8003F464);

INCLUDE_ASM(const s32, "system/message", func_8003F4E0);

//INCLUDE_ASM(const s32, "system/message", func_8003F54C);

bool func_8003F54C(u16 index, f32 x, f32 y, f32 z) {

    bool result = 0;

    if (index < MAX_DIALOGUE_BOXES) {
        if (dialogueBoxes[index].flags & 1) {
            result = 1;
            dialogueBoxes[index].unk_4C.x = x;
            dialogueBoxes[index].unk_4C.y = y;
            dialogueBoxes[index].unk_4C.z = z;
        }
    }

    return result;
}

INCLUDE_ASM(const s32, "system/message", func_8003F5D0);

INCLUDE_ASM(const s32, "system/message", func_8003F630);

//INCLUDE_ASM(const s32, "system/message", func_8003F690);

bool func_8003F690(u16 index, u8 arg1, u8 arg2, u8 arg3) {

    bool result = 0;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & 1) { 
            
            dialogueBoxes[index].currentLineFromTop = arg1;
            dialogueBoxes[index].maxLinesInText = arg2;
            dialogueBoxes[index].charPerLineCount = arg3;

            if (dialogueBoxes[index].currentLineFromTop != 0xFF) {
                dialogueBoxes[index].flags |= 0x200;
            } else {
                dialogueBoxes[index].flags &= ~0x200;
            }

            if (dialogueBoxes[index].maxLinesInText != 0xFF) {
                dialogueBoxes[index].flags |= 0x400;
            } else {
                dialogueBoxes[index].flags &= ~0x400;
            }

            if (dialogueBoxes[index].charPerLineCount != 0xFF) {
                dialogueBoxes[index].flags |= 0x10000;
            } else {
                dialogueBoxes[index].flags &= ~0x10000;
            }

            result = 1;
        }
    }

    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003F80C);

INCLUDE_ASM(const s32, "system/message", func_8003F910);

//INCLUDE_ASM(const s32, "system/message", func_8003FA1C);

bool func_8003FA1C(u16 index, u16 arg1, u32 romTextureStart, u32 romTextureEnd, 
    u32 romAssetIndexStart, u32 romAssetIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, 
    u32 vaddrTexture, u32 vaddrTexture2, 
    u32 vaddrPalette, u32 vaddrUnknownAsset, 
    u32 vaddrUnknownAsset2, u32 vaddrSpritesheetIndex, 
    f32 argE, f32 argF, f32 arg10) {

    bool result = 0;

    if (index == 0) {
        
        dialogueIcons[index].romTextureStart = romTextureStart;
        dialogueIcons[index].romTextureEnd = romTextureEnd;
        dialogueIcons[index].romAssetIndexStart = romAssetIndexStart;
        dialogueIcons[index].romAssetIndexEnd = romAssetIndexEnd;
        dialogueIcons[index].romSpritesheetIndexStart = romSpritesheetIndexStart;
        dialogueIcons[index].romSpritesheetIndexEnd = romSpritesheetIndexEnd;

        dialogueIcons[index].vaddrTexture = vaddrTexture;
        dialogueIcons[index].vaddrTexture2 = vaddrTexture2;
        dialogueIcons[index].vaddrPalette = vaddrPalette;
        dialogueIcons[index].vaddrUnknownAsset = vaddrUnknownAsset;
        dialogueIcons[index].vaddrUnknownAsset2 = vaddrUnknownAsset2;
        dialogueIcons[index].vaddrSpritesheetIndex = vaddrSpritesheetIndex;

        dialogueIcons[index].unk_3C = arg1;

        dialogueIcons[index].unk_30.x = argE;
        dialogueIcons[index].unk_30.y = argF;
        dialogueIcons[index].unk_30.z = arg10;

        result = 1;
        
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003FAE8);

INCLUDE_ASM(const s32, "system/message", func_8003FAF8);

INCLUDE_ASM(const s32, "system/message", func_8003FB4C);

//INCLUDE_ASM(const s32, "system/message", func_8003FBA0);

bool func_8003FBA0(u16 arg0, u8* arg1, s8 arg2) {

    bool result = 0;

    if (arg0 < 0x40) {
        result = 1;
        D_80189E58[arg0].unk_0 = arg1;
        D_80189E58[arg0].unk_4 = arg2;
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003FBD8);

//INCLUDE_ASM(const s32, "system/message", func_8003FD74);

void func_8003FD74(void) {

    u16 i;

    for (i = 0; i < 256; i++) {
        D_80204BF0[i] = 0xEE;
    }
    
}

INCLUDE_ASM(const s32, "system/message", func_8003FDB0);

INCLUDE_ASM(const s32, "system/message", func_8003FE9C);

INCLUDE_ASM(const s32, "system/message", func_8003FFF4);

INCLUDE_ASM(const s32, "system/message", func_80040140);

//INCLUDE_ASM(const s32, "system/message", func_800401C8);

u8 func_800401C8(u16 index) {

    u8 result = 0;

    if (index < MAX_DIALOGUE_BOXES) {
        if (dialogueBoxes[index].flags & 1 && dialogueBoxes[index].flags & 2) {
            result = dialogueBoxes[index].buttonSfxCounter != 0;
        }
    }

    return result;
}

INCLUDE_ASM(const s32, "system/message", func_8004022C);

INCLUDE_ASM(const s32, "system/message", func_80040318);

INCLUDE_RODATA(const s32, "system/message", D_8011EE10);

INCLUDE_RODATA(const s32, "system/message", D_8011EE30);

INCLUDE_ASM(const s32, "system/message", func_800403F0);

INCLUDE_ASM(const s32, "system/message", func_80040628);

INCLUDE_ASM(const s32, "system/message", func_80040858);

INCLUDE_ASM(const s32, "system/message", func_80040C38);

INCLUDE_ASM(const s32, "system/message", func_80041850);

INCLUDE_ASM(const s32, "system/message", func_800419C4);

INCLUDE_ASM(const s32, "system/message", func_80041B28);

// dialogue box rendering/printing
INCLUDE_ASM(const s32, "system/message", func_80041B80);

INCLUDE_ASM(const s32, "system/message", func_80041CD8);

// param 1: 0x80159510 + 0x10 * offset
// Gfx* func_80042014(Gfx*, DialogueBox*, u16);
INCLUDE_ASM(const s32, "system/message", func_80042014);

INCLUDE_ASM(const s32, "system/message", func_800423F0);

INCLUDE_RODATA(const s32, "system/message", D_8011EE90);
/*
dlabel D_8011EE90
.word 0x80402010
.word 0x08040201
*/

INCLUDE_RODATA(const s32, "system/message", D_8011EE98);

INCLUDE_RODATA(const s32, "system/message", D_8011EE9C);

INCLUDE_RODATA(const s32, "system/message", D_8011EEA0);

INCLUDE_RODATA(const s32, "system/message", D_8011EEA4);

INCLUDE_RODATA(const s32, "system/message", D_8011EEA8);

INCLUDE_RODATA(const s32, "system/message", D_8011EEAC);

INCLUDE_RODATA(const s32, "system/message", D_8011EEB0);

INCLUDE_RODATA(const s32, "system/message", D_8011EEB4);

INCLUDE_RODATA(const s32, "system/message", D_8011EEB8);

INCLUDE_RODATA(const s32, "system/message", D_8011EEBC);

INCLUDE_RODATA(const s32, "system/message", D_8011EEC0);

INCLUDE_RODATA(const s32, "system/message", D_8011EEC4);

INCLUDE_ASM(const s32, "system/message", func_80042634);
