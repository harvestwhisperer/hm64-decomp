#include "common.h"

#include "system/dialogue.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/volume.h"

// bss
extern u16 D_80204BF0[256];
extern DialogueBox dialogueBoxes[MAX_DIALOGUE_BOXES];
extern GameVariableString gameVariableStrings[64];
extern Vtx D_801C6230[2][2048][4];
// byteswapped font texture buffer
extern FontData D_801706C0[2][256];

// rodata
extern u32 D_8011EE10[8];
extern u8 D_8011EE30[16];
extern u8 D_8011EE90[8];
extern Gfx D_8011EEB0;
extern Gfx D_8011EEB8;
extern Gfx D_8011EEC0;

// forward delcarations
bool func_8003F024(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
bool func_8003E77C(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
bool func_8003FDB0(u16);      
u8 func_800403F0(u16 index, u16 arg1);
void func_80040628(u16, u8); 
u32 func_80041850(u16 index);
u32 func_80041B28(u16 index, u16 offset);
void func_80041B80(u16, DialogueBoxFont*, u8*);

//INCLUDE_ASM(const s32, "system/message", func_8003D970);

void func_8003D970(void) {

    u16 i = 0;

    for (i = 0; i < MAX_DIALOGUE_BOXES; i++) {

        dialogueBoxes[i].flags = 0;
        
        dialogueBoxes[i].dialogueIndex = 0;
        
        dialogueBoxes[i].unk_90 = 0;
        dialogueBoxes[i].unk_91 = 0;
        
        dialogueBoxes[i].unk_7C = 0;

        dialogueBoxes[i].buttonSfxCounter = 0;
        dialogueBoxes[i].flag = 0;
        dialogueBoxes[i].margins = 0;
        dialogueBoxes[i].maxLinesInBox = 0;

        dialogueBoxes[i].unk_9B = 0;
        dialogueBoxes[i].unk_9C = 0;

        dialogueBoxes[i].unk_70 = 0xFF;
        dialogueBoxes[i].unk_74 = 0xFF;
        dialogueBoxes[i].unk_78 = 0xFF;

        dialogueBoxes[i].unk_80 = 0;
        dialogueBoxes[i].unk_82 = 0;
        dialogueBoxes[i].unk_84 = 0;
        dialogueBoxes[i].unk_86 = 0;
        dialogueBoxes[i].dialogueInfoIndex = 0;

        dialogueBoxes[i].fontContext.unk_60 = 0;
        dialogueBoxes[i].fontContext.unk_61 = 0;

        dialogueBoxes[i].shrink.x = 0;
        dialogueBoxes[i].shrink.y = 0;
        dialogueBoxes[i].shrink.z = 0;

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

bool func_8003DBE8(u16 dialogueBoxIndex, u8* textBufferAddr) {
 
    bool set = FALSE;
    
    if (dialogueBoxIndex < MAX_DIALOGUE_BOXES) {

        if (!(dialogueBoxes[dialogueBoxIndex].flags & ACTIVE)) {

            dialogueBoxes[dialogueBoxIndex].unk_90 = 0;
            dialogueBoxes[dialogueBoxIndex].unk_91 = 0;
 
            dialogueBoxes[dialogueBoxIndex].buttonSfxCounter = 0;
            dialogueBoxes[dialogueBoxIndex].flag = 0;
            dialogueBoxes[dialogueBoxIndex].margins = 0;
            dialogueBoxes[dialogueBoxIndex].maxLinesInBox = 0;

            dialogueBoxes[dialogueBoxIndex].unk_70 = 0xFF;
            dialogueBoxes[dialogueBoxIndex].unk_74 = 0xFF;
            dialogueBoxes[dialogueBoxIndex].unk_78 = 0xFF;

            dialogueBoxes[dialogueBoxIndex].textBufferBase = textBufferAddr;
            
            dialogueBoxes[dialogueBoxIndex].flags = 1;

            func_800267A4((Volume*)&dialogueBoxes[dialogueBoxIndex].volume, 0, 0);
            func_8003F024(dialogueBoxIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            
            set = TRUE;        

        }
        
    }
    
    return set;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003DD14);

bool func_8003DD14(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {
            
            dialogueBoxes[index].flags &= ~1;
            
            if (dialogueBoxes[index].flags & HAS_DIALOGUE_WINDOW) {
                func_8002BAD8(dialogueWindows[dialogueBoxes[index].dialogueWindowIndex].spriteIndex);
            }
            
            if (dialogueBoxes[index].flags & HAS_CHARACTER_AVATAR) {
                func_8002BAD8(characterAvatars[dialogueBoxes[index].characterAvatarIndex].spriteIndex);
            }
            
            result = TRUE;

        }
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", initializeDialogueBox);

bool initializeDialogueBox(u16 dialogueBoxIndex, u16 dialogueInfoIndex, u16 dialogueIndex, u32 flag) {

    bool result = FALSE;

    u16 tempDialogueIndex;
    u32 textAddress;

    tempDialogueIndex = dialogueIndex;
    
    if (dialogueBoxIndex < MAX_DIALOGUE_BOXES) {
        
        if (dialogueBoxes[dialogueBoxIndex].flags & ACTIVE) {

            dialogueBoxes[dialogueBoxIndex].dialogueIndex = dialogueIndex;
            dialogueBoxes[dialogueBoxIndex].dialogueInfoIndex = dialogueInfoIndex;
            
            nuPiReadRom(dialogueInfo[dialogueInfoIndex].romIndexStart, dialogueInfo[dialogueInfoIndex].index, dialogueInfo[dialogueInfoIndex].romIndexEnd - dialogueInfo[dialogueInfoIndex].romIndexStart);

            textAddress = func_80041B28(dialogueBoxIndex, dialogueBoxes[dialogueBoxIndex].dialogueIndex);
            nuPiReadRom(textAddress, dialogueBoxes[dialogueBoxIndex].textBufferBase, func_80041B28(dialogueBoxIndex, dialogueBoxes[dialogueBoxIndex].dialogueIndex + 1) - textAddress);
            
            dialogueBoxes[dialogueBoxIndex].flag = func_800403F0(dialogueBoxIndex, tempDialogueIndex);
            dialogueBoxes[dialogueBoxIndex].unk_90 = 0;
            dialogueBoxes[dialogueBoxIndex].unk_91 = 0;
            dialogueBoxes[dialogueBoxIndex].buttonSfxCounter = 0;
            dialogueBoxes[dialogueBoxIndex].margins = 0;
            dialogueBoxes[dialogueBoxIndex].maxLinesInBox = 0;
            dialogueBoxes[dialogueBoxIndex].unk_9D = 0;

            dialogueBoxes[dialogueBoxIndex].currentCharPtr = dialogueBoxes[dialogueBoxIndex].textBufferBase;

            dialogueBoxes[dialogueBoxIndex].flags &= ~4;
            dialogueBoxes[dialogueBoxIndex].flags &= ~0x4000;
            dialogueBoxes[dialogueBoxIndex].flags |= INITIALIZED;

            dialogueBoxes[dialogueBoxIndex].flags &= ~0x8000;
            dialogueBoxes[dialogueBoxIndex].flags &= ~0x40000;
            dialogueBoxes[dialogueBoxIndex].flags &= ~0x80000;
            dialogueBoxes[dialogueBoxIndex].flags &= ~0x100000;
            dialogueBoxes[dialogueBoxIndex].flags &= ~0x20000;

            if (flag == 0x8000) {
                dialogueBoxes[dialogueBoxIndex].flags |= 0x8000;
            }
            
            if (flag == 0x40000) {
                dialogueBoxes[dialogueBoxIndex].flags |= flag;
            }

            if (flag == 0x80000) {
                dialogueBoxes[dialogueBoxIndex].flags |= flag;
            }

            if (flag == 0x100000) {
                dialogueBoxes[dialogueBoxIndex].flags |= flag;
            }

            if (dialogueBoxes[dialogueBoxIndex].flags & HAS_OVERLAY_ICON) {
                dmaSprite(overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].spriteIndex, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].romTextureStart, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].romTextureEnd, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].romIndexStart, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].romIndexEnd, 0, 0, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].vaddrTextureStart, NULL, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].vaddrTextureEnd, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].vaddrIndexStart, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].vaddrIndexEnd, overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].unk_20, 0, 0);
                func_8002CB24(overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].spriteIndex, TRUE);
                func_8002C914(overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C680(overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].spriteIndex, 2, 2);
                func_8002C7EC(overlayIcons[dialogueBoxes[dialogueBoxIndex].overlayIconIndex].spriteIndex, 3);
            }

            if (dialogueBoxes[dialogueBoxIndex].flags & HAS_CHARACTER_AVATAR) {
                dmaSprite(characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].spriteIndex, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].romTextureStart, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].romTextureEnd, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].romAssetIndexStart, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].romAssetIndexEnd, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].romSpritesheetIndexStart, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].romSpritesheetIndexEnd, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].vaddrTexture, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].vaddrSpritesheet, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].vaddrPalette, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].vaddrAnimation, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].vaddrSpriteToPalette, characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].vaddrSpritesheetIndex, 1, 0);
                func_8002CB24(characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].spriteIndex, TRUE);
                func_8002C914(characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C680(characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].spriteIndex, 2, 2);
                func_8002C7EC(characterAvatars[dialogueBoxes[dialogueBoxIndex].characterAvatarIndex].spriteIndex, 3);
            }

            if (dialogueBoxes[dialogueBoxIndex].flags & HAS_DIALOGUE_WINDOW) {

                dmaSprite(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].romTextureStart, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].romTextureEnd, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].romIndexStart, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].romIndexEnd, 0, 0, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].vaddrTextureStart, NULL, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].vaddrTextureEnd, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].vaddrIndexStart, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].vaddrIndexEnd, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].unk_20, 0, 0);
                func_8002CB24(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);
                func_8002C914(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C680(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, 2, 2);
                func_8002C7EC(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, 3);
                func_8002B80C(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteOffset, dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].flag);
                setSpriteShrinkFactor(dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].spriteIndex, dialogueBoxes[dialogueBoxIndex].shrink.x + dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].coordinates.x, dialogueBoxes[dialogueBoxIndex].shrink.y + dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].coordinates.y, (dialogueBoxes[dialogueBoxIndex].shrink.z + dialogueWindows[dialogueBoxes[dialogueBoxIndex].dialogueWindowIndex].coordinates.z) - 2.0f);
            
            }

            func_8003E77C(dialogueBoxIndex, 0xFF, 0xFF, 0xFF, 0xFF);

            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003E77C);

bool func_8003E77C(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) { 

        if (dialogueBoxes[index].flags & ACTIVE) {
            
            dialogueBoxes[index].unk_14.r = (dialogueBoxes[index].unk_4.r * arg1) / 255.0f;
            dialogueBoxes[index].unk_14.g = (dialogueBoxes[index].unk_4.g * arg2) / 255.0f;
            dialogueBoxes[index].unk_14.b = (dialogueBoxes[index].unk_4.b * arg3) / 255.0f;
            dialogueBoxes[index].unk_14.a = (dialogueBoxes[index].unk_4.a * arg4) / 255.0f;
    
            dialogueBoxes[index].unk_24.r = (dialogueBoxes[index].unk_4.r * arg1) / 255.0f;
            dialogueBoxes[index].unk_24.g = (dialogueBoxes[index].unk_4.g * arg2) / 255.0f;
            dialogueBoxes[index].unk_24.b = (dialogueBoxes[index].unk_4.b * arg3) / 255.0f;
            dialogueBoxes[index].unk_24.a = (dialogueBoxes[index].unk_4.a * arg4) / 255.0f;
    
            result = TRUE;
     
        }
        
    }

    return result;
    
}

// rgba
INCLUDE_ASM(const s32, "system/message", func_8003E910);

INCLUDE_ASM(const s32, "system/message", func_8003EA1C);

//INCLUDE_ASM(const s32, "system/message", func_8003EE7C);

bool func_8003EE7C(u16 index, u8 arg1, s16 arg2) {

    bool result;
    s16 temp;
    f32 tempF;

    temp = getAbsoluteValue(arg2);

    result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {

            dialogueBoxes[index].unk_24.a = (dialogueBoxes[index].unk_4.a * arg1) / 255.0f;

            dialogueBoxes[index].flags &= ~(0x200000);

            if (dialogueBoxes[index].unk_24.a < dialogueBoxes[index].unk_14.a) {
                tempF = dialogueBoxes[index].unk_14.a - dialogueBoxes[index].unk_24.a;
            } else {
                tempF = dialogueBoxes[index].unk_24.a - dialogueBoxes[index].unk_14.a;
            }

            dialogueBoxes[index].unk_34[3] = (tempF * temp) / dialogueBoxes[index].unk_4.a;

            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003EFD8);

bool func_8003EFD8(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {
            result = (dialogueBoxes[index].flags >> 0x15) & 1;
        }
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003F024);

bool func_8003F024(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {

            dialogueBoxes[index].unk_4.r = r;
            dialogueBoxes[index].unk_4.g = g;
            dialogueBoxes[index].unk_4.b = b;
            dialogueBoxes[index].unk_4.a = a;

            result = TRUE;
            
        }
        
    }

    return result;
    
}

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

//INCLUDE_ASM(const s32, "system/message", setDialogueInfo);

bool setDialogueInfo(u16 dialogueIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 index) {

    bool result = FALSE;

    if (dialogueIndex < MAX_DIALOGUE_BANKS) {

        dialogueInfo[dialogueIndex].romIndexStart = romIndexStart;
        dialogueInfo[dialogueIndex].romIndexEnd = romIndexEnd;
        dialogueInfo[dialogueIndex].index = index;
        dialogueInfo[dialogueIndex].romTextStart = romTextStart;
        
        result = TRUE;
        
    }
    
    return result;
    
}

INCLUDE_ASM(const s32, "system/message", func_8003F360);

//INCLUDE_ASM(const s32, "system/message", func_8003F464);

bool func_8003F464(u16 index, u8 arg1, u8 arg2, u8* fontTexturePtr, u16* fontPalettePtr) {
    
    u8 result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {
        
        if (dialogueBoxes[index].flags & ACTIVE) {
            
            dialogueBoxes[index].fontContext.unk_60 = arg1;
            dialogueBoxes[index].fontContext.unk_61 = arg2;
            dialogueBoxes[index].fontContext.fontTexturePtr = fontTexturePtr;
            dialogueBoxes[index].fontContext.fontPalettePtr = fontPalettePtr;
             
            result = TRUE;
            
        }
    }
    
    return result;
    
}


INCLUDE_ASM(const s32, "system/message", func_8003F4E0);

//INCLUDE_ASM(const s32, "system/message", func_8003F54C);

bool func_8003F54C(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {
            
            dialogueBoxes[index].shrink.x = x;
            dialogueBoxes[index].shrink.y = y;
            dialogueBoxes[index].shrink.z = z;
            
            result = TRUE;

        }

    }

    return result;

}

//INCLUDE_ASM(const s32, "system/message", func_8003F5D0);

bool func_8003F5D0(u16 index, u8 arg1, u8 arg2) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {

            dialogueBoxes[index].unk_92 = arg1;
            dialogueBoxes[index].unk_93 = arg2;
            
            result = TRUE;
            
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/message", func_8003F630);

bool func_8003F630(u16 index, u8 arg1, u8 arg2) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {

            dialogueBoxes[index].unk_9B = arg1;
            dialogueBoxes[index].unk_9C = arg2;
            
            result = TRUE;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", setDialogueBoxSpriteIndices);

bool setDialogueBoxSpriteIndices(u16 index, u8 dialogueWindowIndex, u8 overlayIconIndex, u8 characterAvatarIndex) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) { 
            
            dialogueBoxes[index].dialogueWindowIndex = dialogueWindowIndex;
            dialogueBoxes[index].overlayIconIndex = overlayIconIndex;
            dialogueBoxes[index].characterAvatarIndex = characterAvatarIndex;

            if (dialogueBoxes[index].dialogueWindowIndex != 0xFF) {
                dialogueBoxes[index].flags |= HAS_DIALOGUE_WINDOW;
            } else {
                dialogueBoxes[index].flags &= ~HAS_DIALOGUE_WINDOW;
            }

            if (dialogueBoxes[index].overlayIconIndex != 0xFF) {
                dialogueBoxes[index].flags |= HAS_OVERLAY_ICON;
            } else {
                dialogueBoxes[index].flags &= ~HAS_OVERLAY_ICON;
            }

            if (dialogueBoxes[index].characterAvatarIndex != 0xFF) {
                dialogueBoxes[index].flags |= HAS_CHARACTER_AVATAR;
            } else {
                dialogueBoxes[index].flags &= ~HAS_CHARACTER_AVATAR;
            }

            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003F80C);

bool func_8003F80C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < 3) {

        dialogueWindows[index].romTextureStart = romTextureStart;
        dialogueWindows[index].romTextureEnd = romTextureEnd;
        dialogueWindows[index].romIndexStart = romIndexStart;
        dialogueWindows[index].romIndexEnd = romIndexEnd;

        dialogueWindows[index].vaddrTextureStart = vaddrTextureStart;
        dialogueWindows[index].vaddrTextureEnd = vaddrTextureEnd;
        dialogueWindows[index].vaddrIndexStart = vaddrIndexStart;
        dialogueWindows[index].vaddrIndexEnd = vaddrIndexEnd;

        dialogueWindows[index].unk_20 = argA;

        dialogueWindows[index].spriteIndex = spriteIndex;
        dialogueWindows[index].spriteOffset = spriteOffset;

        dialogueWindows[index].flag = flag;

        dialogueWindows[index].coordinates.x = x;
        dialogueWindows[index].coordinates.y = y;
        dialogueWindows[index].coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003F910);

bool func_8003F910(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, 
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
    void* vaddrTexture, void* vaddrTexture2, 
    void* vaddrPalette, void* vaddrUnknownAsset, 
    void* vaddrUnknownAsset2, void* vaddrSpritesheetIndex, 
    f32 x, f32 y, f32 z) {

    bool result = FALSE;

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
        if (dialogueBoxes[index].flags & ACTIVE ) {
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

        if (dialogueBoxes[index].flags & ACTIVE ) {

            dialogueBoxes[index].unk_84 = arg1;

            result = TRUE;

        }
    }

    return result;
    
}


//INCLUDE_ASM(const s32, "system/message", setGameVariableString);

bool setGameVariableString(u16 index, u8* ptr, s8 length) {

    bool result = FALSE;

    if (index < MAX_GAME_VARIABLE_STRINGS) {
        
        gameVariableStrings[index].ptr = ptr;
        gameVariableStrings[index].length = length;

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003FBD8);

// FIXME: likely fake matches, but need do {} while (0) to get registers right
u8 func_8003FBD8(u16 index, u32 arg1, u8 arg2) {
    
    u32 buffer1[8];
    u8 buffer2[16];
    
    u8 length;
    u8 position;
    u8 set;
    
    u32 temp;
    u32 temp2;
    u32 temp3;
    
    memcpy(buffer1, D_8011EE10, 32);
    memcpy(buffer2, D_8011EE30, 10);
    
    temp3 = arg1;
    position = 0;
    set = 0;
    
    length = gameVariableStrings[index].length - 1;
    
    do {
        
loop_start:

        temp = buffer1[length];
        temp2 = temp3 / temp;
        temp3 %= temp;
        
        if (temp2 || (length < 1)) {
            set = 1;
            goto setChar;
        }
        
        if (set) goto setChar;
        
        do {
            
            if (length) goto setEndLetter;
            
        } while (0);
        
        do {
            
            do {
setChar:
                gameVariableStrings[index].ptr[position] = buffer2[temp2];
            } while (0);
            
            goto end_loop;
            
setEndLetter:
            if (arg2 == 1) {
                gameVariableStrings[index].ptr[position] = 0xEE;
            } else {
                gameVariableStrings[index].ptr[position] = 0xFF;
            }

end_loop:
            position++;

        } while (0);
        
        if (length--) goto loop_start;
        
        return 0;
        
    } while (1);
}


//INCLUDE_ASM(const s32, "system/message", func_8003FD74);

void func_8003FD74(void) {

    u16 i;

    for (i = 0; i < 256; i++) {
        D_80204BF0[i] = 0xEE;
    }
    
}
 
//INCLUDE_ASM(const s32, "system/message", func_8003FDB0);

bool func_8003FDB0(u16 index) {

    u16 i;
    u16 temp;
    u32 temp2;

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (dialogueBoxes[index].flags & ACTIVE) {

            temp = dialogueBoxes[index].unk_92 * dialogueBoxes[index].unk_93;

            if (((dialogueBoxes[index].flags >> 4) & 3) / 3) {
   
                temp += dialogueBoxes[index].unk_92;

            }

            for (i = 0; i < temp; i++) {

                D_80204BF0[dialogueBoxes[index].unk_80 + i] = 0xEE;

            }
        
            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003FE9C);

bool func_8003FE9C(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) { 

        if ((dialogueBoxes[index].flags & ACTIVE) && (dialogueBoxes[index].flags & INITIALIZED) && !(dialogueBoxes[index].flags & (0x40 | 0x80))) {

            if (dialogueBoxes[index].flag + 1 != dialogueBoxes[index].buttonSfxCounter + dialogueBoxes[index].unk_93) {

                func_800267A4((Volume*)&dialogueBoxes[index].volume, dialogueBoxes[index].unk_84, 0);

                dialogueBoxes[index].flags |= 0x40;
                dialogueBoxes[index].flags &= ~8;

                dialogueBoxes[index].maxLinesInBox++;
                
                func_80040628(index, dialogueBoxes[index].buttonSfxCounter + dialogueBoxes[index].unk_93 + 1);
                
                dialogueBoxes[index].currentCharPtr--;
                
                result = TRUE ;
                
            }
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8003FFF4);

bool func_8003FFF4(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) { 

        if ((dialogueBoxes[index].flags & ACTIVE) && (dialogueBoxes[index].flags & INITIALIZED) && !(dialogueBoxes[index].flags & (0x40 | 0x80))) {

            if (dialogueBoxes[index].buttonSfxCounter) {

                func_800267A4((Volume*)&dialogueBoxes[index].volume, dialogueBoxes[index].unk_84, 0);

                dialogueBoxes[index].flags |= 0x80;
                dialogueBoxes[index].flags &= ~8;

                dialogueBoxes[index].buttonSfxCounter--;
                dialogueBoxes[index].maxLinesInBox++;
                
                func_80040628(index, dialogueBoxes[index].buttonSfxCounter + dialogueBoxes[index].unk_93 + 1);
                
                dialogueBoxes[index].currentCharPtr--;
                
                result = TRUE;
                
            }
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/message", func_80040140);

bool func_80040140(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) { 

        if ((dialogueBoxes[index].flags & ACTIVE) && (dialogueBoxes[index].flags & INITIALIZED)) {
            result = dialogueBoxes[index].flag + 1 != dialogueBoxes[index].buttonSfxCounter + dialogueBoxes[index].unk_93;
        }
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_800401C8);
 
u8 func_800401C8(u16 index) {

    u8 count = 0;

    if (index < MAX_DIALOGUE_BOXES) {
        if (dialogueBoxes[index].flags & ACTIVE && dialogueBoxes[index].flags & INITIALIZED) {
            count = dialogueBoxes[index].buttonSfxCounter != 0;
        }
    }

    return count;
    
}

//INCLUDE_ASM(const s32, "system/message", func_8004022C);

void func_8004022C(u16 index) {

    func_800267B4((Volume*)&dialogueBoxes[index].volume);

    if ((u16)func_80026844((Volume*)&dialogueBoxes[index].volume) >= dialogueBoxes[index].fontContext.unk_61 + dialogueBoxes[index].unk_9C) {

        dialogueBoxes[index].flags &= ~0x40;
         
        func_800267A4((Volume*)&dialogueBoxes[index].volume, 0, 0);
 
        dialogueBoxes[index].buttonSfxCounter++;
        dialogueBoxes[index].maxLinesInBox--;
        
    }
       
}

//INCLUDE_ASM(const s32, "system/message", func_80040318);

void func_80040318(u16 index) {

    func_800267B4((Volume*)&dialogueBoxes[index].volume);

    if ((u16)func_80026844((Volume*)&dialogueBoxes[index].volume) >= dialogueBoxes[index].fontContext.unk_61 + dialogueBoxes[index].unk_9C) {

        dialogueBoxes[index].flags &= ~0x80;
        
        func_800267A4((Volume*)&dialogueBoxes[index].volume, 0, 0);

        dialogueBoxes[index].maxLinesInBox--;
        
    }

}

static const u32 D_8011EE10[8] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

//INCLUDE_RODATA(const s32, "system/message", D_8011EE10);

static const u8 D_8011EE30[16] = {
    222, 213, 214, 215, 
    216, 217, 218, 219,
    220, 221, 0, 0,
    0, 0, 0, 0 
};

//INCLUDE_RODATA(const s32, "system/message", D_8011EE30);

//INCLUDE_ASM(const s32, "system/message", func_800403F0);

u8 func_800403F0(u16 index, u16 arg1) {
    
    u8 result = 0;
    
    bool set = 0;
    
    u8 count2 = 0;
    u8 count = 0;
    
    u8 i;
    u8 temp;
    
    dialogueBoxes[index].unk_90 = 0;
    dialogueBoxes[index].unk_91 = 0;
    dialogueBoxes[index].unk_A0 = 0;
    dialogueBoxes[index].unk_A1 = 0;
    dialogueBoxes[index].currentCharPtr = dialogueBoxes[index].textBufferBase;
    
    do {
        switch ((u16)func_80041850(index)) {
            case 0:
                if (dialogueBoxes[index].unk_A0 < count2) {
                    dialogueBoxes[index].unk_A0 = count2;
                }
                count2 = 0;
                result += 1;
                count++;
                break;

            case 1:
                result += 1;
                count++;
                break;

            case 2:
                if (dialogueBoxes[index].unk_A0 < count2) {
                    dialogueBoxes[index].unk_A0 = count2;
                }
                set = 1;
                count++;
                break;

            case 7:
                temp = *dialogueBoxes[index].currentCharPtr;
                dialogueBoxes[index].currentCharPtr++;
                
                if (gameVariableStrings[temp].ptr[0] != 0xFF) {
                    
                    i = 0;
                    
                    while ((gameVariableStrings[temp].ptr[i] != 0xFF) && (i < gameVariableStrings[temp].length)) {
                        i++;
                        count2++;
                    }
                }
                break;

            case 3:
            case 9:
                dialogueBoxes[index].currentCharPtr++;
                break;

            default:
                count2++;
                break;
        }

    } while (!set);
    
    dialogueBoxes[index].unk_A1 = count;
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_80040628);

// FIXME: do {} while (0) switch statement
void func_80040628(u16 index, u8 arg1) {
    
    u8 i;
    u8 position;
    u8 done;
    
    u16 temp;
    u32 temp2;
    
    done = 0; 
    i = 0;
    
    dialogueBoxes[index].unk_90 = 0;
    dialogueBoxes[index].unk_91 = 0;
    dialogueBoxes[index].currentCharPtr = dialogueBoxes[index].textBufferBase;
    
    if (arg1) {
        
        do {

            if (done) {
                
                position++;
                
                if (gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].ptr[position] == 0xFF) {
                    
                    while ((gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].ptr[position] == 0xFF) && (position < gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].length)) {
                        position++;
                    }

                }
                
                temp = 0xEE;
                
                if (position == gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].length) {
                    done = 0;
                }
                
            } else {
                temp = func_80041850(index);
            }
            
            switch (temp) {
                
                do {
                    case 2:
                        i = arg1;
                        break;
                    case 0:
                    case 1:
                        i++;
                        break;
                    case 7:
                        position = 0;
                        done = 1;
                        dialogueBoxes[index].currentCharPtr++;
                        break;
                    case 3:
                    case 9:
                        dialogueBoxes[index].currentCharPtr++;
                        do {} while (0);
                        break;
                } while (0);
            
                default:
                    break;
            }

        } while (i < arg1);
    }
}

INCLUDE_ASM(const s32, "system/message", func_80040858);

INCLUDE_ASM(const s32, "system/message", func_80040C38);

//INCLUDE_ASM(const s32, "system/message", func_80041850);

u32 func_80041850(u16 index) {

    u8 buffer[2];
    u32 padding[4];

    u32 result;
    
    u8 temp;
    u32 temp2;
    
    memcpy(buffer, D_8011EE90, 8);

    if (!(dialogueBoxes[index].unk_91 & 7)) {
        dialogueBoxes[index].unk_90 = *dialogueBoxes[index].currentCharPtr;        
        dialogueBoxes[index].currentCharPtr++;
    }

    if (dialogueBoxes[index].unk_90 & buffer[dialogueBoxes[index].unk_91 & 7]) {
        
        temp = *dialogueBoxes[index].currentCharPtr;
        dialogueBoxes[index].currentCharPtr++;
        
        temp2 = *dialogueBoxes[index].currentCharPtr;
        dialogueBoxes[index].currentCharPtr++;
        
        result = (temp | (temp2 << 8));
        
    } else {
        result = *dialogueBoxes[index].currentCharPtr; 
        dialogueBoxes[index].currentCharPtr++;
    }

    dialogueBoxes[index].unk_91++;

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_800419C4);

u32 func_800419C4(u16 index) {

    u32 result;

    result = gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].ptr[dialogueBoxes[index].gameVariableStringLength];
    
    dialogueBoxes[index].gameVariableStringLength++;
    
     while (gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].ptr[dialogueBoxes[index].gameVariableStringLength] == 0xFF && dialogueBoxes[index].gameVariableStringLength < gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].length) {
        dialogueBoxes[index].gameVariableStringLength++;
    } 

    if (dialogueBoxes[index].gameVariableStringLength == gameVariableStrings[dialogueBoxes[index].gameVariableStringPosition].length) {
        dialogueBoxes[index].flags &= ~0x100;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/message", func_80041B28);

u32 func_80041B28(u16 index, u16 offset) {

    u32 result = dialogueInfo[dialogueBoxes[index].dialogueInfoIndex].romTextStart;
    
    result += dialogueInfo[dialogueBoxes[index].dialogueInfoIndex].index[offset];

    return result;
    
}

static inline void swapTop(u8* source, u8* dest) {

    u8 temp;
    
    // 0xC0 = 1100 0000
    // 0x30 = 0011 0000

    temp = ((*source) & 0xC0) >> 2;
    *dest = temp;
    *dest |= ((*source) & 0x30) >> 4;
    
}

static inline void swapBottom(u8* source, u8* dest) {

    u8 temp;

    // 0xC = 1100
    // 0x3 = 0011

    temp = ((*source) & 0xC) << 2;
    *dest = temp;
    *dest |= *source & 3;
    
}

//INCLUDE_ASM(const s32, "system/message", func_80041B80);

void func_80041B80(u16 arg0, DialogueBoxFont* fontContext, u8 arg2[]) {

    u8 i;

    u8 *arr;
    u8 *ptr;

    arr = (fontContext->fontTexturePtr+3);
    arr = &arr[arg0*0x40];
    
    ptr = arg2; 
    
    i = 0;
    
    if (fontContext->unk_60) {

        do {

            swapTop(arr, ptr);
            swapBottom(arr, ++ptr);
            arr--;

            swapTop(arr, ++ptr);
            swapBottom(arr, ++ptr);
            arr--;

            swapTop(arr, ++ptr);
            swapBottom(arr, ++ptr);
            arr--;

            swapTop(arr, ++ptr);
            
            arr += 7;
            ptr++;
            
            i++;
            
        } while (i < fontContext->unk_60);
    }
    
}

#ifdef PERMUTER
Gfx* func_80041CD8(Gfx* dl, DialogueBox* dialogueBox) {

    f32 ulx, uly;
    
    Gfx tempDl;

    *dl++ = D_8011EE98;

    *dl++ = D_8011EEA0;

    ulx = dialogueBox->fontContext.unk_60 >> 1;

    ulx = (((dialogueBox->shrink.x + 160.0f) - ulx) - ((dialogueBox->unk_92* dialogueBox->fontContext.unk_60) >> 1) + ulx) - ((dialogueBox->unk_92* dialogueBox->currentLine) >> 1);

    uly = dialogueBox->fontContext.unk_61 >> 1;
    
    uly = ((((120.0f - dialogueBox->shrink.y) - uly) - ((dialogueBox->unk_93 * dialogueBox->fontContext.unk_61) >> 1)) + uly) - ((dialogueBox->unk_93 * dialogueBox->unk_9C) >> 1);

    gDPSetScissor(&tempDl, G_SC_NON_INTERLACE, ulx, uly, 
        ulx + (dialogueBox->unk_92 * dialogueBox->fontContext.unk_60) + (dialogueBox->unk_92 * dialogueBox->currentLine), 
        uly + (dialogueBox->unk_93 * dialogueBox->fontContext.unk_61) + (dialogueBox->unk_93 * dialogueBox->unk_9C));

    *dl++ = tempDl;

    *dl++ = D_8011EEA8;

    return dl++;
    
}
#else
INCLUDE_ASM(const s32, "system/message", func_80041CD8);
#endif

#ifdef PERMUTER
Gfx* func_80042014(Gfx* dl, DialogueBox* dialogueBox, s32 arg2) {

    u32 count1;
    u16 count2;
    
    u32 temp;
    u16 temp2;
    u16 temp3;
    u32 tempFlags;

    FontBitmap sprite;
    Gfx tempDl[2];

    temp = dialogueBox->unk_92;
    
    count2 = 0;
    count1 = 0;

    if (dialogueBox->unk_92) {

        arg2 &= 0xFF;
        temp3 = arg2;

        do {

            count1 &= 0xFF;
            temp *= temp3;
            temp += count1;
            temp2 = (dialogueBox->unk_80 + temp);
            
            func_80041B80(
                D_80204BF0[dialogueBox->unk_80+temp2], 
                &dialogueBox->fontContext.fontTexturePtr, 
                D_801706C0[gDisplayContextIndex][dialogueBox->unk_80+temp2].timg
            );

            if (dialogueBox->flags & 2) {

                tempFlags = (0x10 | 0x40 | 0x100 | 0x400);
                
                func_800276AC(&D_801C6230[gDisplayContextIndex][dialogueBox->unk_80+temp2][0], 
                    dialogueBox->fontContext.unk_60,
                    dialogueBox->fontContext.unk_61,
                    dialogueBox->fontContext.unk_61,
                    0,
                    0,
                    0,
                    (s16)((dialogueBox->fontContext.unk_60 + dialogueBox->unk_9B)*count1) >> 1, 
                    0, 
                    tempFlags, 
                    dialogueBox->unk_14.r, 
                    dialogueBox->unk_14.g, 
                    dialogueBox->unk_14.b, 
                    dialogueBox->unk_14.a
                );
                
                sprite.timg = D_801706C0[gDisplayContextIndex][dialogueBox->unk_80 + temp2].timg;
                
                func_80026F30((Bitmap*)&sprite, dialogueBox->fontContext.fontPalettePtr);

                sprite.width = dialogueBox->fontContext.unk_60;
                sprite.height = dialogueBox->fontContext.unk_61;
                sprite.fmt = 2;
                sprite.pixelSize = 0;

                dl = func_80026F88(dl, (Bitmap*)&sprite, 0, sprite.height);

                gSPVertex(&tempDl[1], &D_801C6230[gDisplayContextIndex][dialogueBox->unk_80+temp2][0], 4, 0);

                *tempDl = *(tempDl+1);
                *dl++ = *tempDl;
                
                *dl++ = D_8011EEB0;
                *dl++ = D_8011EEB8;
                
            }
            
            count2++;
            count1++;
            
        } while ((u8)count1 < dialogueBox->unk_92);

    }

    *dl++ = D_8011EEB8;
    *dl++ = D_8011EEC0;

    dialogueBox->unk_82 += count2;
    
    return dl++;
    
}
#else
INCLUDE_ASM(const s32, "system/message", func_80042014);
#endif

INCLUDE_ASM(const s32, "system/message", func_800423F0);

static const u8 D_8011EE90[8] = { 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1, 0 };

//INCLUDE_RODATA(const s32, "system/message", D_8011EE90);

static const Gfx D_8011EE98 = gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);

//INCLUDE_RODATA(const s32, "system/message", D_8011EE98);

static const Gfx D_8011EEA0 = gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

//INCLUDE_RODATA(const s32, "system/message", D_8011EEA0);

static const Gfx D_8011EEA8 = gsDPSetTextureFilter(G_TF_BILERP);

//INCLUDE_RODATA(const s32, "system/message", D_8011EEA8);

static const Gfx D_8011EEB0 = gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0);

//INCLUDE_RODATA(const s32, "system/message", D_8011EEB0);

static const Gfx D_8011EEB8 = gsDPPipeSync();

//INCLUDE_RODATA(const s32, "system/message", D_8011EEB8);

static const Gfx D_8011EEC0 = gsSPEndDisplayList();

//INCLUDE_RODATA(const s32, "system/message", D_8011EEC0);

// main loop function
INCLUDE_ASM(const s32, "system/message", func_80042634);
