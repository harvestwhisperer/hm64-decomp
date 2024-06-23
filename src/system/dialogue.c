#include "common.h"

#include "system/controller.h" 
#include "system/dialogue.h"
#include "system/message.h"
#include "system/sprite.h"

#include "game.h"

// bss
// struct/array
extern u32 D_8013CE0C;
extern u8 D_8013CE10;

// game state updated by/talked about in dialogues
extern DialogueVariable dialogueVariables[46];

extern u32* specialDialogueBitsPointer;

// forward declarations
extern u32 func_80043C98(u16, u16);
extern void func_80043B84(u16);
extern void func_80044684(u16 index);
extern void func_800449C4(u16 index);
extern void func_80044BF4(u16 index);
extern void func_80044D78(u16);
extern void func_80045260(u16);

//INCLUDE_ASM(const s32, "system/dialogue", func_80042F60);

void func_80042F60(void) {

    u16 i;
 
    for (i = 0; i < 1; i++) {
        dialogues[i].struct5.dialogueIndex = 0;
        dialogues[i].struct5.dialogueAddressIndex = 0;
        dialogues[i].struct5.dialogueBoxIndex1 = 0;
        dialogues[i].struct5.scrollSfxIndex = 0xFF;
        dialogues[i].struct5.closeSfxIndex = 0xFF;
        dialogues[i].struct5.unk_8 = 0xFF;
        dialogues[i].struct5.flags = 0;
    }
} 

//INCLUDE_ASM(const s32, "system/dialogue", func_80042FEC);

bool func_80042FEC(u16 index, u16 arg1, u16 arg2) {

    bool result = FALSE;
 
    if (index == 0 && !(dialogues[index].struct5.flags & 1)) {

        dialogues[index].struct5.scrollSfxIndex = 0xFF;
        dialogues[index].struct5.closeSfxIndex = 0xFF;
        dialogues[index].struct5.unk_8 = 0xFF;
        dialogues[index].struct5.dialogueBoxIndex1 = arg1;
        dialogues[index].struct5.dialogueBoxIndex2 = arg2;

        dialogues[index].struct5.flags = 1;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043050);

bool func_80043050(u16 index, u16 arg1, u16 arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7) {

    bool result = FALSE;
    
    if (index < TOTAL_CONVERSATION_BANKS) {

        dialogueAddresses[index].unk_14 = arg1;
        dialogueAddresses[index].unk_16 = arg2;
        dialogueAddresses[index].romStart = arg3;
        dialogueAddresses[index].romEnd = arg4;
        dialogueAddresses[index].vaddr = arg5;
        dialogueAddresses[index].romIndex = arg6;
        dialogueAddresses[index].vaddrIndex = arg7;
        
        result = TRUE;

    } 
    
    return result;
}

//INCLUDE_ASM(const s32, "system/dialogue", setDialogueVariable);

bool setDialogueVariable(u16 index, void *value, u8 type, s32 max) {

    bool result = FALSE;

    if (index < TOTAL_GAME_VARIABLES) {

        if (type < 5) {

            dialogueVariables[index].value = value;
            dialogueVariables[index].type = type;
            dialogueVariables[index].maxValue = max;

            result = TRUE;

        }

    }
 
    return result;

}

//INCLUDE_ASM(const s32, "system/dialogue", setSpecialDialogueBitsPointer);

bool setSpecialDialogueBitsPointer(u32* arg0) {
    specialDialogueBitsPointer = arg0;
    return 0;
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043148);

bool func_80043148(u16 index, u32 scrollSfxIndex, u32 closeSfxIndex, u32 arg3) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].struct5.flags & 1) {
        
        dialogues[index].struct5.scrollSfxIndex = scrollSfxIndex;
        dialogues[index].struct5.closeSfxIndex = closeSfxIndex;
        dialogues[index].struct5.unk_8 = arg3;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_8004318C);

bool func_8004318C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & 1)) {
  
        dialogues[index].struct2.romTextureStart = romTextureStart;
        dialogues[index].struct2.romTextureEnd = romTextureEnd;
        dialogues[index].struct2.romIndexStart = romIndexStart;
        dialogues[index].struct2.romIndexEnd = romIndexEnd;
  
        dialogues[index].struct2.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].struct2.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].struct2.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].struct2.vaddrIndexEnd = vaddrIndexEnd;
        dialogues[index].struct2.unk_20 = argA;
 
        dialogues[index].struct2.spriteIndex = spriteIndex;
        dialogues[index].struct2.spriteOffset = spriteOffset;
        dialogues[index].struct2.flag = flag;

        dialogues[index].struct2.coordinates.x = x;
        dialogues[index].struct2.coordinates.y = y;
        dialogues[index].struct2.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043260);

bool func_80043260(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & 1)) {
  
        dialogues[index].struct3.romTextureStart = romTextureStart;
        dialogues[index].struct3.romTextureEnd = romTextureEnd;
        dialogues[index].struct3.romIndexStart = romIndexStart;
        dialogues[index].struct3.romIndexEnd = romIndexEnd;
  
        dialogues[index].struct3.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].struct3.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].struct3.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].struct3.vaddrIndexEnd = vaddrIndexEnd;
        dialogues[index].struct3.unk_20 = argA;
 
        dialogues[index].struct3.spriteIndex = spriteIndex;
        dialogues[index].struct3.spriteOffset = spriteOffset;
        dialogues[index].struct3.flag = flag;

        dialogues[index].struct3.coordinates.x = x;
        dialogues[index].struct3.coordinates.y = y;
        dialogues[index].struct3.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043334);
 
bool func_80043334(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & 1)) {
        
        dialogues[index].struct4.romTextureStart = romTextureStart;
        dialogues[index].struct4.romTextureEnd = romTextureEnd;
        dialogues[index].struct4.romIndexStart = romIndexStart;
        dialogues[index].struct4.romIndexEnd = romIndexEnd;
  
        dialogues[index].struct4.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].struct4.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].struct4.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].struct4.vaddrIndexEnd = vaddrIndexEnd;
        dialogues[index].struct4.unk_20 = argA;
 
        dialogues[index].struct4.spriteIndex = spriteIndex;
        dialogues[index].struct4.spriteOffset = spriteOffset;
        dialogues[index].struct4.flag = flag;

        dialogues[index].struct4.coordinates.x = x;
        dialogues[index].struct4.coordinates.y = y;
        dialogues[index].struct4.coordinates.z = z;
        
        result = TRUE;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043408);

// adjust value
inline int func_80043408(int initial, int value, int max) {
    
    int temp;
    int adjusted;

    adjusted = value;
    temp = initial + adjusted;
    
    if (max < temp) {
        adjusted -= temp - max;
        temp = max;
    }
    
    if (temp < 0) {
        adjusted -= temp;
    } 
      
    return adjusted;
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043430);

bool func_80043430(u16 index, u16 dialogueAddressIndex, u16 dialogueIndex, u16 arg3) {

    bool result = FALSE;
    u32 temp;

    if (index == 0 && dialogues[index].struct5.flags & 1) {

        func_8002B138(dialogues[index].struct2.spriteIndex, 
            dialogues[index].struct2.romTextureStart, 
            dialogues[index].struct2.romTextureEnd, 
            dialogues[index].struct2.romIndexStart, 
            dialogues[index].struct2.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].struct2.vaddrTextureStart,
            NULL,
            dialogues[index].struct2.vaddrTextureEnd,
            dialogues[index].struct2.vaddrIndexStart,
            dialogues[index].struct2.vaddrIndexEnd,
            dialogues[index].struct2.unk_20,
            0,
            0);
 
        setSpriteShrinkFactor(dialogues[index].struct2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].struct2.spriteIndex, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].struct2.spriteIndex, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].struct2.spriteIndex, 1);
        func_8002C914(dialogues[index].struct2.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].struct2.spriteIndex, 2, 2);
        func_8002C7EC(dialogues[index].struct2.spriteIndex, 3);

        func_8002B138(dialogues[index].struct3.spriteIndex, 
            dialogues[index].struct3.romTextureStart, 
            dialogues[index].struct3.romTextureEnd, 
            dialogues[index].struct3.romIndexStart, 
            dialogues[index].struct3.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].struct3.vaddrTextureStart,
            NULL,
            dialogues[index].struct3.vaddrTextureEnd,
            dialogues[index].struct3.vaddrIndexStart,
            dialogues[index].struct3.vaddrIndexEnd,
            dialogues[index].struct3.unk_20,
            0,
            0);

        setSpriteShrinkFactor(dialogues[index].struct3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].struct3.spriteIndex, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].struct3.spriteIndex, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].struct3.spriteIndex, 1);
        func_8002C914(dialogues[index].struct3.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].struct3.spriteIndex, 2, 2);
        func_8002C7EC(dialogues[index].struct3.spriteIndex, 3);

        func_8002B138(dialogues[index].struct4.spriteIndex, 
            dialogues[index].struct4.romTextureStart, 
            dialogues[index].struct4.romTextureEnd, 
            dialogues[index].struct4.romIndexStart, 
            dialogues[index].struct4.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].struct4.vaddrTextureStart,
            NULL,
            dialogues[index].struct4.vaddrTextureEnd,
            dialogues[index].struct4.vaddrIndexStart,
            dialogues[index].struct4.vaddrIndexEnd,
            dialogues[index].struct4.unk_20,
            0,
            0);

        setSpriteShrinkFactor(dialogues[index].struct4.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].struct4.spriteIndex, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].struct4.spriteIndex, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].struct4.spriteIndex, 1);
        func_8002C914(dialogues[index].struct4.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].struct4.spriteIndex, 2, 2);
        func_8002C7EC(dialogues[index].struct4.spriteIndex, 3);
        
        dialogues[index].struct1.unk_12 = 0xFF;
        dialogues[index].struct5.dialogueAddressIndex = dialogueAddressIndex;
        dialogues[index].struct5.dialogueIndex = dialogueIndex;

        nuPiReadRom(dialogueAddresses[dialogueAddressIndex].romStart, dialogueAddresses[dialogueAddressIndex].vaddr, dialogueAddresses[dialogueAddressIndex].romEnd - dialogueAddresses[dialogueAddressIndex].romStart);

        temp = func_80043C98(0, dialogues[index].struct5.dialogueIndex);

        nuPiReadRom(temp, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].vaddrIndex, func_80043C98(0, dialogues[index].struct5.dialogueIndex + 1) - temp);

        dialogues[index].dialoguePointer = dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].vaddrIndex;
        
        dialogues[index].struct5.unk_16 = 0;
        dialogues[index].struct5.unk_17 = 0;

        dialogues[index].struct1.unk_0 = 0;
        dialogues[index].struct1.unk_2 = 0;
        dialogues[index].struct1.unk_4 = 0;
        dialogues[index].struct1.unk_6 = 0;
        dialogues[index].struct1.unk_8 = 0;
        dialogues[index].struct1.unk_A = 0;
        dialogues[index].struct1.specialDialogueBit = 0;
        dialogues[index].struct1.unk_E = 0;
        dialogues[index].struct1.unk_10 = 0;
        
        dialogues[index].struct1.unk_13 = 0;
        dialogues[index].struct1.unk_14 = 0;
        dialogues[index].struct1.unk_15 = 0;
        dialogues[index].struct1.unk_16 = 0;
        dialogues[index].struct1.unk_17 = 0;
        dialogues[index].struct1.unk_18 = 0;
        
        dialogues[index].struct5.flags &= ~4;
        dialogues[index].struct5.flags |= 2;

        if (arg3 == 0x40) {
            dialogues[index].struct5.flags |= (2 | 0x40);
        }  else {
            dialogues[index].struct5.flags &= ~(0x40);
        }

        if (arg3 == 0x80) {
            dialogues[index].struct5.flags |= 0x80;
        } else {
            dialogues[index].struct5.flags &= ~0x80;
        }

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043A88);

bool func_80043A88(void) {

    bool result = FALSE;
    u16 i;

    for (i = 0; i < 1; i++) {
        if (dialogues[i].struct5.flags & 4) {
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043AD8);

bool func_80043AD8(u16 index) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].struct5.flags & 1) {

        if (!(dialogues[index].struct5.flags & 0x40)) {
            dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex1].flags &= ~0x8000;
        }

        func_8003F130(dialogues[index].struct5.dialogueBoxIndex1);
        func_80043B84(0);
        
        dialogues[index].struct5.flags = 5;
        
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043B84);

void func_80043B84(u16 index) {

    dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].flags &= ~0x8000;
    
    func_8003F130(dialogues[index].struct5.dialogueBoxIndex2);
    
    func_8002BAD8(dialogues[index].struct2.spriteIndex);
    func_8002BAD8(dialogues[index].struct3.spriteIndex);
    func_8002BAD8(dialogues[index].struct4.spriteIndex);
    
    dialogues[index].struct5.flags &= ~0x20;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C6C);

u8 func_80043C6C(u16 index) {
    return dialogues[index].struct5.unk_17;
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C98);

u32 func_80043C98(u16 index, u16 arg1) {

    u32 ptr;

    ptr = dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].romIndex;
    
    return ptr + dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].vaddr[arg1];
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043CF8);

inline void func_80043CF8(u16 index, u16 arg1) {

    switch (dialogueVariables[index].type) {
        case UNSIGNED_CHAR:
            *(u8*)dialogueVariables[index].value = arg1;
            break;
        case UNSIGNED_SHORT:
            *(u16*)dialogueVariables[index].value = arg1;
            break;
        case UNSIGNED_INT:
            *(u32*)dialogueVariables[index].value = arg1;
            break;
    }
    
}
 
//INCLUDE_ASM(const s32, "system/dialogue", func_80043D8C);

inline u32 func_80043D8C(u16 index) {

    u32 value;
    
    switch (dialogueVariables[index].type) {
        case UNSIGNED_CHAR:
            value = *(u8*)dialogueVariables[index].value;
            break;
        case UNSIGNED_SHORT:
            value = *(u16*)dialogueVariables[index].value;
            break;
        case UNSIGNED_INT:
            value = *(u32*)dialogueVariables[index].value;
            break;
    }

    return value;

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043E28);

inline void func_80043E28(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] |= 1 << (temp & 0x1F);
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043E5C);

inline void func_80043E5C(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] &= ~(1 << (temp & 0x1F));

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043E94);

inline u32 func_80043E94(u16 bitIndex) {
    
    u32 temp = bitIndex;
    
    return !(specialDialogueBitsPointer[temp >> 5] & (1 << (temp & 0x1F))) == 0;

}

INCLUDE_ASM(const s32, "system/dialogue", func_80043EC8);

#ifdef PERMUTER
void func_80044684(u16 index) {

    f32 temp1, temp2;

    temp1 = -(dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_92 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_60) - (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_92 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].currentLine) - dialogues[index].struct2.coordinates.x;
    temp2 = (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_61) + (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9C) - dialogues[index].struct2.coordinates.y;    

    setSpriteScale(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A0 * 0.5f, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * 0.6f, 1.0f);
    
    func_8002C7EC(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 3);
    func_8002C914(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 255, 255, 255, 192);
    func_8002CB24(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 1);

    func_8002B80C(dialogues[index].struct2.spriteIndex, dialogues[index].struct2.spriteOffset, dialogues[index].struct2.flag);
    setSpriteShrinkFactor(dialogues[index].struct2.spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.x + temp1, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.y + temp2, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.z);

}
#else
INCLUDE_ASM(const s32, "system/dialogue", func_80044684);
#endif

//INCLUDE_ASM(const s32, "system/dialogue", func_800449C4);

void func_800449C4(u16 index) {

    f32 tempX = dialogues[index].struct3.coordinates.x;
    f32 tempY = dialogues[index].struct3.coordinates.y;

    setSpriteScale(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A0 * 0.5f, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * 0.6f, 1.0f);
    
    // flags
    func_8002C7EC(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 3);
    func_8002CB24(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 1);

    // animation
    func_8002B80C(dialogues[index].struct3.spriteIndex, dialogues[index].struct3.spriteOffset, dialogues[index].struct3.flag);
    setSpriteShrinkFactor(dialogues[index].struct3.spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.x + tempX, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.y + tempY, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.z);

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80044BF4);

void func_80044BF4(u16 index) {
 
    f32 tempX = dialogues[index].struct4.coordinates.x;
    f32 tempY = dialogues[index].struct4.coordinates.y;

    // flags
    func_8002C7EC(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 3);
    func_8002CB24(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 1);

    // animation
    func_8002B80C(dialogues[index].struct4.spriteIndex, dialogues[index].struct4.spriteOffset, dialogues[index].struct4.flag);
    setSpriteShrinkFactor(dialogues[index].struct4.spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.x + tempX, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.y + tempY, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_4C.z);

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80044D78);

void func_80044D78(u16 index) {

    bool set = FALSE;

    if (!(dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].flags & (0x40 | 0x80))) { 

        if (func_8004D3C8(CONTROLLER_1, BUTTON_STICK_UP)) {

            if (dialogues[index].struct5.unk_17 < (dialogues[index].struct5.unk_16 - 1)) {

                dialogues[index].struct5.unk_17++;

                if (dialogues[index].struct5.unk_18 != dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 - 1) {
                    dialogues[index].struct5.unk_18++;
                    adjustSpriteShrinkFactor(dialogues[index].struct2.spriteIndex, 0.0f, -dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_61 - dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9C, 0.0f);
                } else {
                    func_8003FE9C(dialogues[index].struct5.dialogueBoxIndex2);
                    dialogues[index].struct5.unk_19++;
                }

                if (dialogues[index].struct5.scrollSfxIndex != 0xFF) {
                    setSfx(dialogues[index].struct5.scrollSfxIndex + 1);
                    setSfxVolume(dialogues[index].struct5.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                }
                
                set = TRUE;
                
            }
        }

        if (func_8004D3C8(CONTROLLER_1, 0x100000)) {

            if (!set) {

                if (dialogues[index].struct5.unk_17) {

                    dialogues[index].struct5.unk_17--;
                    
                    if (dialogues[index].struct5.unk_18) {
                        dialogues[index].struct5.unk_18--;
                        adjustSpriteShrinkFactor(dialogues[index].struct2.spriteIndex, 0.0f, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_61 + dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9C, 0.0f);
                    } else {
                        func_8003FFF4(dialogues[index].struct5.dialogueBoxIndex2);
                        dialogues[index].struct5.unk_19--;
                    }

                    if (dialogues[index].struct5.scrollSfxIndex != 0xFF) {
                        setSfx(dialogues[index].struct5.scrollSfxIndex + 1);
                        setSfxVolume(dialogues[index].struct5.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                        
                    }
                    
                    set = TRUE;
                
                }
            }
        }

        if (dialogues[index].struct5.unk_16 > dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93) { 

            if (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 < (dialogues[index].struct5.unk_16 - dialogues[index].struct5.unk_19)) {
                func_80044BF4(index);
            } else {
                func_8002BAD8(dialogues[index].struct4.spriteIndex);
            }

            if (dialogues[index].struct5.unk_19) {
                func_800449C4(index);
            } else {
                func_8002BAD8(dialogues[index].struct3.spriteIndex);
            }

        }

        if (func_8004D380(CONTROLLER_1, BUTTON_A)) {

            if (!set) {
                
                func_80043B84(index);
                dialogues[index].struct1.unk_12 = 0xFF;
                
                if (dialogues[index].struct5.closeSfxIndex != 0xFF) {
                    setSfx(dialogues[index].struct5.closeSfxIndex + 1);
                    setSfxVolume(dialogues[index].struct5.closeSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                    set = TRUE;
                }
                
            }
        }
    }
   
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80045260);

void func_80045260(u16 index) {
    
    u16 set = FALSE;
    
    u16 temp;
    u16 temp2;
    u16 temp3;
    
    int temp4;
    int temp5;
    u16 tempIndex;
    
    int adjusted;
    int max;
    int initial;
    
    while (!set) {
        
        if (dialogues[index].struct1.unk_12 == 0xFF) {
            func_80043EC8(index);
        }
        
        switch (dialogues[index].struct1.unk_12) {
        
            case 0:
                if (dialogues[index].struct5.flags & 0x80) {
                    func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, dialogues[index].struct1.unk_0, 0);
                } else {
                    func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, dialogues[index].struct1.unk_0, 0x8000);
                }
                
                set = TRUE;
                dialogues[index].struct1.unk_12 = 0xFF;
                dialogues[index].struct5.flags |= 0x10;
                
                break;

            case 1:
                
                temp = func_80043D8C(dialogues[index].struct1.unk_13);
                
                if ((temp >= dialogues[index].struct1.unk_4) && (dialogues[index].struct1.unk_6 >= temp)) {
                
                    temp2 = dialogues[index].struct1.unk_0;
                    
                    if (temp2 != 0xFFFF) {

                        if (dialogues[index].struct5.flags & 0x80) {
                            func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, temp2, 0);
                        } else {
                            func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, temp2, 0x8000);
                        }
                        
                        set = TRUE;
                        
                        dialogues[index].struct1.unk_0 = 0xFFFF;
                        dialogues[index].struct5.flags |= 0x10;
                        
                    } else {
                        
                        temp2 = dialogues[index].struct1.unk_2;
                        
                        if (temp2 != 0xFFFF) {
                            func_80043430( index, dialogues[index].struct5.dialogueAddressIndex, temp2, dialogues[index].struct5.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].struct1.unk_12 = 0xFF;
                        }
                    }
                    
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            case 2:

                /*

                temp4 = (u16)func_80043D8C(dialogues[index].struct1.unk_13);

                temp4 += adjustValue_dialogue_c(temp4, dialogues[index].struct1.unk_E, dialogueVariables[dialogues[index].struct1.unk_13].maxValue);
                
                func_80043CF8(dialogues[index].struct1.unk_13, temp4);
                
                dialogues[index].struct1.unk_12 = 0xFF;
                
                break;

                */

                temp4 = func_80043D8C(dialogues[index].struct1.unk_13);

                // FIXME: should be inline adjustValue call

                max = dialogueVariables[dialogues[index].struct1.unk_13].maxValue;
                adjusted = dialogues[index].struct1.unk_E;
                
                temp5 = (u16)temp4;
                temp5 += adjusted;
                
                if (max < temp5) {
                    adjusted -= temp5 - max;
                    temp5 = max;
                }
                if (temp5 < 0) {
                    adjusted -= temp5;
                }
                
                max = temp4 + adjusted;
                
                // FIXME: should be inline func_80043CF8 call

                tempIndex = dialogues[index].struct1.unk_13;
                
                switch (dialogueVariables[tempIndex].type) {
                    case UNSIGNED_CHAR:
                        *((u8*)dialogueVariables[tempIndex].value) = max;
                        break;

                    case UNSIGNED_SHORT:
                        *((u16*)dialogueVariables[tempIndex].value) = (u16)max;
                        break;

                    case UNSIGNED_INT:
                        *((u32*)dialogueVariables[tempIndex].value) = (u16)max;
                        break;
                }

                dialogues[index].struct1.unk_12 = 0xFF;
                
                break;

            case 3:
                func_80043CF8(dialogues[index].struct1.unk_13, dialogues[index].struct1.unk_A);
                dialogues[index].struct1.unk_12 = 0xFF;
                break;

            case 4:
                
                if (func_80043E94(dialogues[index].struct1.specialDialogueBit)) {
                    
                    temp2 = dialogues[index].struct1.unk_0;
                
                    if (temp2 != 0xFFFF) {
                        
                        if (dialogues[index].struct5.flags & 0x80) {
                            func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, temp2, 0);
                        } else {
                            func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, temp2, 0x8000);
                        }
                        
                        set = TRUE;
                        dialogues[index].struct1.unk_0 = 0xFFFF;
                        dialogues[index].struct5.flags |= 0x10;
                    
                    } else {
                        
                        temp2 = dialogues[index].struct1.unk_2;
                        if (temp2 != 0xFFFF) {
                            func_80043430(index, dialogues[index].struct5.dialogueAddressIndex, temp2, dialogues[index].struct5.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].struct1.unk_12 = 0xFF;
                        }
                    }
                    
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            case 5:
                func_80043E28(dialogues[index].struct1.specialDialogueBit);
                dialogues[index].struct1.unk_12 = 0xFF;
                break;

            case 6:
                func_80043E5C(dialogues[index].struct1.specialDialogueBit);
                dialogues[index].struct1.unk_12 = 0xFF;
                break;

            case 7:
                
                temp3 = dialogues[index].struct1.unk_8;
                
                if ((temp3 >= dialogues[index].struct1.unk_14) && (dialogues[index].struct1.unk_15 >= temp3)) {
                
                    temp2 = dialogues[index].struct1.unk_0;
                    
                    if (temp2 != 0xFFFF) {
                        if (dialogues[index].struct5.flags & 0x80) {
                            func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, temp2, 0);
                        } else {
                            func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex1, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_14, temp2, 0x8000);
                        }
                        
                        set = TRUE;
                        dialogues[index].struct1.unk_0 = 0xFFFF;
                        dialogues[index].struct5.flags |= 0x10;
                        
                    } else {
                        
                        temp2 = dialogues[index].struct1.unk_2;
                        
                        if (temp2 != 0xFFFF) {
                            func_80043430(index, dialogues[index].struct5.dialogueAddressIndex, temp2, dialogues[index].struct5.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].struct1.unk_12 = 0xFF;
                        }
                    }
                    
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            case 8:
                func_80043430(index, dialogues[index].struct5.dialogueAddressIndex, dialogues[index].struct1.unk_2, dialogues[index].struct5.flags & (0x40 | 0x80));
                break;

            case 10:
                
                dialogues[index].struct1.unk_12 = 0xFF;
                dialogues[index].struct5.unk_17 = 0;
                dialogues[index].struct5.unk_18 = 0;
                dialogues[index].struct5.unk_19 = 0;
                dialogues[index].struct5.unk_14 = dialogues[index].struct1.unk_10;
                dialogues[index].struct5.flags |= 0x20;
                
                func_8003DDF8(dialogues[index].struct5.dialogueBoxIndex2, dialogueAddresses[dialogues[index].struct5.dialogueAddressIndex].unk_16, dialogues[index].struct5.unk_14, 0);
                
                temp2 = dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A1;
                
                if (temp2 >= 5) {
                    func_8003F5D0(dialogues[index].struct5.dialogueBoxIndex2, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A0, 4);
                } else {
                    func_8003F5D0(dialogues[index].struct5.dialogueBoxIndex2, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A0, temp2);
                }
                
                dialogues[index].struct5.unk_16 = dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A1;
                
                func_80044684(index);
                
                if (dialogues[index].struct5.unk_8 != 0xFF) {
                    setSfx(dialogues[index].struct5.unk_8 + 1);
                    setSfxVolume(dialogues[index].struct5.unk_8 + 1, DIALOGUE_SFX_VOLUME);
                }
                
                set = TRUE;
                
                break;

            case 9:
                break;

            case 11:
                
                temp = dialogues[index].struct5.unk_17;
                
                if (dialogues[index].struct1.unk_18 == temp) {
                    func_80043430(index, dialogues[index].struct5.dialogueAddressIndex, dialogues[index].struct1.unk_2, dialogues[index].struct5.flags & (0x40 | 0x80));
                    dialogues[index].struct5.unk_17 = temp;
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            case 12:
                set = TRUE;
                func_80043AD8(index);
                break;
        }
    }
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80045CB0);

void func_80045CB0(void) {

    u16 i;
    bool set;

    for (i = 0; i < MAX_DIALOGUES; i++) {
        
        if (dialogues[i].struct5.flags & 1 && dialogues[i].struct5.flags & 2) {
            
            set = FALSE;
            
            if (dialogues[i].struct5.flags & 0x10) {
                
                if (dialogueBoxes[dialogues[i].struct5.dialogueBoxIndex1].flags & 4 || dialogueBoxes[dialogues[i].struct5.dialogueBoxIndex1].flags & 0x20000) {
                    dialogues[i].struct5.flags &= ~0x10;
                }
                
                set = TRUE;

            }
            
            if (dialogues[i].struct5.flags & 0x20) {
                func_80044D78(i);
                set = TRUE;
            }
            
            if (!set) {
                func_80045260(i);
            }
        }
    }
    
}
