#include "common.h"

#include "system/dialogue.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/volume.h"

// bss
extern u16 D_80204BF0[0x100];
DialogueBox dialogueBoxes[MAX_DIALOGUE_BOXES];
extern GameVariableReference gameVariableStrings[64];

// forward delcarations
void func_8003F024(u16, u8, u8, u8, u8);
bool func_8003FDB0(u16);      
void func_80040628(u16, u8); 
u16 func_80041850(u16);

//INCLUDE_ASM(const s32, "system/message", func_8003D970);

void func_8003D970(void) {

    u16 i = 0;

    for (i = 0; i < MAX_DIALOGUE_BOXES; i++) {

        dialogueBoxes[i].flags = 0;
        
        dialogueBoxes[i].unk_7E = 0;
        
        dialogueBoxes[i].unk_90 = 0;
        dialogueBoxes[i].unk_91 = 0;
        
        dialogueBoxes[i].unk_7C = 0;

        dialogueBoxes[i].buttonSfxCounter = 0;
        dialogueBoxes[i].flag = 0;
        dialogueBoxes[i].margins = 0;
        dialogueBoxes[i].maxLinesInBox = 0;

        dialogueBoxes[i].currentLine = 0;
        dialogueBoxes[i].unk_9C = 0;

        dialogueBoxes[i].unk_70 = 0xFF;
        dialogueBoxes[i].unk_74 = 0xFF;
        dialogueBoxes[i].unk_78 = 0xFF;

        dialogueBoxes[i].unk_80 = 0;
        dialogueBoxes[i].unk_82 = 0;
        dialogueBoxes[i].unk_84 = 0;
        dialogueBoxes[i].unk_86 = 0;
        dialogueBoxes[i].unk_88 = 0;

        dialogueBoxes[i].unk_60 = 0;
        dialogueBoxes[i].unk_61 = 0;

        dialogueBoxes[i].unk_4C.x = 0;
        dialogueBoxes[i].unk_4C.y = 0;
        dialogueBoxes[i].unk_4C.z = 0;

        dialogueBoxes[i].unk_92 = 0;
        dialogueBoxes[i].unk_93 = 0;

        dialogueBoxes[i].unk_14.r = 255.0f;
        dialogueBoxes[i].unk_14.g = 255.0f;
        dialogueBoxes[i].unk_14.b = 255.0f;
        dialogueBoxes[i].unk_14.a = 255.0f;

        dialogueBoxes[i].unk_24.r = 255.0f;
        dialogueBoxes[i].unk_24.g = 255.0f;
        dialogueBoxes[i].unk_24.b = 255.0f;
        dialogueBoxes[i].unk_24.a = 255.0f;
       
        func_800267A4((Volume*)&dialogueBoxes[i].unk_64, 0, 0);
        func_800267A4((Volume*)&dialogueBoxes[i].volume, 0, 0);
         
        dialogueBoxes[i].unk_9D = 0;
        
    }

    for (i = 0; i < 3; i++) {
        
    }

    func_8003FD74();
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003DBE8);

bool func_8003DBE8(u16 dialogueBoxIndex, void* arg1) {
 
    bool set = FALSE;
    
    if (dialogueBoxIndex < 6) {

        if (!(dialogueBoxes[dialogueBoxIndex].flags & 1)) {

            dialogueBoxes[dialogueBoxIndex].unk_90 = 0;
            dialogueBoxes[dialogueBoxIndex].unk_91 = 0;
 
            dialogueBoxes[dialogueBoxIndex].buttonSfxCounter = 0;
            dialogueBoxes[dialogueBoxIndex].flag = 0;
            dialogueBoxes[dialogueBoxIndex].margins = 0;
            dialogueBoxes[dialogueBoxIndex].maxLinesInBox = 0;

            dialogueBoxes[dialogueBoxIndex].unk_70 = 0xFF;
            dialogueBoxes[dialogueBoxIndex].unk_74 = 0xFF;
            dialogueBoxes[dialogueBoxIndex].unk_78 = 0xFF;

            dialogueBoxes[dialogueBoxIndex].unk_0 = arg1;
            
            dialogueBoxes[dialogueBoxIndex].flags = 1;

            func_800267A4((Volume*)&dialogueBoxes[dialogueBoxIndex].volume, 0, 0);
            func_8003F024(dialogueBoxIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            
            set = TRUE;        

        }
        
    }
    
    return set;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003DD14);

// DMA dialogues
INCLUDE_ASM(const s32, "system/message", func_8003DDF8);

INCLUDE_ASM(const s32, "system/message", func_8003E77C);

INCLUDE_ASM(const s32, "system/message", func_8003E910);

INCLUDE_ASM(const s32, "system/message", func_8003EA1C);

INCLUDE_ASM(const s32, "system/message", func_8003EE7C);
 
INCLUDE_ASM(const s32, "system/message", func_8003EFD8);

INCLUDE_ASM(const s32, "system/message", func_8003F024);

//INCLUDE_ASM(const s32, "system/message", func_8003F0DC);

bool func_8003F0DC(void) {

    u16 i;
    bool result = FALSE;

    for (i = 0; i < MAX_DIALOGUE_BOXES; i++) {

        if (dialogueBoxes[i].flags & 4) {
            result = TRUE;
        }
        
    }

    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003F130);

INCLUDE_ASM(const s32, "system/message", func_8003F28C);

//INCLUDE_ASM(const s32, "system/message", func_8003F30C);

bool func_8003F30C(u16 dialogueIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 textBufferVaddr) {

    bool result = FALSE;

    if (dialogueIndex < MAX_DIALOGUE_BANKS) {

        dialogueInfo[dialogueIndex].romIndexStart = romIndexStart;
        dialogueInfo[dialogueIndex].romIndexEnd = romIndexEnd;
        dialogueInfo[dialogueIndex].textBufferVaddr = textBufferVaddr;
        dialogueInfo[dialogueIndex].romTextStart = romTextStart;
        
        result = TRUE;
        
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003F360);

INCLUDE_ASM(const s32, "system/message", func_8003F464);

INCLUDE_ASM(const s32, "system/message", func_8003F4E0);

//INCLUDE_ASM(const s32, "system/message", func_8003F54C);

bool func_8003F54C(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {
        if (dialogueBoxes[index].flags & 1) {
            
            dialogueBoxes[index].unk_4C.x = x;
            dialogueBoxes[index].unk_4C.y = y;
            dialogueBoxes[index].unk_4C.z = z;
            
            result = TRUE;

        }
    }

    return result;
}

INCLUDE_ASM(const s32, "system/message", func_8003F5D0);

INCLUDE_ASM(const s32, "system/message", func_8003F630);

//INCLUDE_ASM(const s32, "system/message", func_8003F690);

bool func_8003F690(u16 index, u8 arg1, u8 arg2, u8 arg3) {

    bool result = FALSE;
    
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

            result = TRUE;
        }
    }

    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003F80C);

//INCLUDE_ASM(const s32, "system/message", func_8003F910);

bool func_8003F910(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u32 vaddrTextureStart, u32 vaddrTextureEnd, u32 vaddrIndexStart, u32 vaddrIndexEnd, 
    u32 argA, u16 offset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = 0;
    
    if (index < 2) {

        overlayIcons[index].romTextureStart = romTextureStart;
        overlayIcons[index].romTextureEnd = romTextureEnd;
        overlayIcons[index].romIndexStart = romIndexStart;
        overlayIcons[index].romIndexEnd = romIndexEnd;

        overlayIcons[index].vaddrTextureStart = vaddrTextureStart;
        overlayIcons[index].vaddrTextureEnd = vaddrTextureEnd;
        overlayIcons[index].vaddrIndexStart = vaddrIndexStart;
        overlayIcons[index].vaddrIndexEnd = vaddrIndexEnd;

        overlayIcons[index].unk_20 = argA;
        overlayIcons[index].spriteIndex = spriteIndex;

        overlayIcons[index].spriteOffset = offset;
        
        overlayIcons[index].flag = flag;

        overlayIcons[index].coordinates.x = x;
        overlayIcons[index].coordinates.y = y;
        overlayIcons[index].coordinates.z = z;
        
        result = 1;
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003FA1C);

bool func_8003FA1C(u16 index, u16 arg1, u32 romTextureStart, u32 romTextureEnd, 
    u32 romAssetIndexStart, u32 romAssetIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, 
    u32 vaddrTexture, u32 vaddrTexture2, 
    u32 vaddrPalette, u32 vaddrUnknownAsset, 
    u32 vaddrUnknownAsset2, u32 vaddrSpritesheetIndex, 
    f32 x, f32 y, f32 z) {

    bool result = FALSE;
\
    if (index == 0) {
        
        characterAvatars[index].romTextureStart = romTextureStart;
        characterAvatars[index].romTextureEnd = romTextureEnd;
        characterAvatars[index].romAssetIndexStart = romAssetIndexStart;
        characterAvatars[index].romAssetIndexEnd = romAssetIndexEnd;
        characterAvatars[index].romSpritesheetIndexStart = romSpritesheetIndexStart;
        characterAvatars[index].romSpritesheetIndexEnd = romSpritesheetIndexEnd;
 
        characterAvatars[index].vaddrTexture = vaddrTexture;
        characterAvatars[index].vaddrSpritesheet = vaddrTexture2;
        characterAvatars[index].vaddrPalette = vaddrPalette;
        characterAvatars[index].vaddrAnimation = vaddrUnknownAsset;
        characterAvatars[index].vaddrSpriteToPalette = vaddrUnknownAsset2;
        characterAvatars[index].vaddrSpritesheetIndex = vaddrSpritesheetIndex;

        characterAvatars[index].spriteIndex = arg1;

        characterAvatars[index].coordinates.x = x;
        characterAvatars[index].coordinates.y = y;
        characterAvatars[index].coordinates.z = z;

        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003FAE8);

void func_8003FAE8(void* arg0) {
    D_801891C8 = arg0;
}

//INCLUDE_ASM(const s32, "system/message", func_8003FAF8);

bool func_8003FAF8(u16 index, u16 arg1) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {
        if (dialogueBoxes[index].flags & 1 ) {
            dialogueBoxes[index].unk_86 = arg1;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003FB4C);

bool func_8003FB4C(u16 index, u16 arg1) {

    u8 result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & 1 ) {

            dialogueBoxes[index].unk_84 = arg1;

            result = TRUE;

        }
    }

    return result;
    
}


//INCLUDE_ASM(const s32, "system/message", func_8003FBA0);

bool func_8003FBA0(u16 arg0, u8* ptr, s8 length) {

    bool result = FALSE;

    if (arg0 < 0x40) {
        
        gameVariableStrings[arg0].ptr = ptr;
        gameVariableStrings[arg0].length = length;

        result = TRUE;

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

    u8 count = 0;

    if (index < MAX_DIALOGUE_BOXES) {
        if (dialogueBoxes[index].flags & 1 && dialogueBoxes[index].flags & 2) {
            count = dialogueBoxes[index].buttonSfxCounter != 0;
        }
    }

    return count;
    
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

// display lists, gsDPSetScissor
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

// main loop function
INCLUDE_ASM(const s32, "system/message", func_80042634);
