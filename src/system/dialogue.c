#include "common.h"

#include "system/dialogue.h"
#include "system/message.h"
#include "system/sprite.h"

#include "game.h"

// bss
// struct/array
extern u32 D_8013CE0C;
extern u8 D_8013CE10;

// state that appears in conversation
extern GameVariable gameVariables[];

extern u32* specialDialogueBitsPointer;

// forward declarations
extern u32 func_80043C98(u16, u16);
extern void func_80043B84(u16);
extern void func_80044D78(u16);
extern void func_80045260(u16);

//INCLUDE_ASM(const s32, "system/dialogue", func_80042F60);

void func_80042F60(void) {

    u16 i;
 
    for (i = 0; i < 1; i++) {
        dialogues[i].struct5.unk_C = 0;
        dialogues[i].struct5.dialogueIndex = 0;
        dialogues[i].struct5.unk_10 = 0;
        dialogues[i].struct5.unk_0 = 0xFF;
        dialogues[i].struct5.unk_4 = 0xFF;
        dialogues[i].struct5.unk_8 = 0xFF;
        dialogues[i].struct5.flags = 0;
    }
} 

//INCLUDE_ASM(const s32, "system/dialogue", func_80042FEC);

bool func_80042FEC(u16 index, u16 arg1, u16 arg2) {

    bool result = FALSE;
 
    if (index == 0 && !(dialogues[index].struct5.flags & 1)) {

        dialogues[index].struct5.unk_0 = 0xFF;
        dialogues[index].struct5.unk_4 = 0xFF;
        dialogues[index].struct5.unk_8 = 0xFF;
        dialogues[index].struct5.unk_10 = arg1;
        dialogues[index].struct5.unk_12 = arg2;

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

bool setDialogueVariable(u16 index, void *address, u8 numSet, s32 max) {

    bool result = FALSE;

    if (index < TOTAL_GAME_VARIABLES) {

        if (numSet < 5) {

            gameVariables[index].address = address;
            gameVariables[index].set = numSet;
            gameVariables[index].maxValue = max;

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

bool func_80043148(u16 index, u32 arg1, u32 arg2, u32 arg3) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].struct5.flags & 1) {
        
        dialogues[index].struct5.unk_0 = arg1;
        dialogues[index].struct5.unk_4 = arg2;
        dialogues[index].struct5.unk_8 = arg3;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_8004318C);

bool func_8004318C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u32 vaddrTextureStart, u32 vaddrTextureEnd, u32 vaddrIndexStart, u32 vaddrIndexEnd, u32 argA, 
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
    u32 vaddrTextureStart, u32 vaddrTextureEnd, u32 vaddrIndexStart, u32 vaddrIndexEnd, u32 argA, 
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
    u32 vaddrTextureStart, u32 vaddrTextureEnd, u32 vaddrIndexStart, u32 vaddrIndexEnd, u32 argA, 
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

bool func_80043430(u16 index, u16 dialogueIndex, u16 arg2, u16 arg3) {

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
        dialogues[index].struct5.dialogueIndex = dialogueIndex;
        dialogues[index].struct5.unk_C = arg2;

        nuPiReadRom(dialogueAddresses[dialogueIndex].romStart, dialogueAddresses[dialogueIndex].vaddr, dialogueAddresses[dialogueIndex].romEnd - dialogueAddresses[dialogueIndex].romStart);

        temp = func_80043C98(0, dialogues[index].struct5.unk_C);

        nuPiReadRom(temp, dialogueAddresses[dialogues[index].struct5.dialogueIndex].vaddrIndex, func_80043C98(0, dialogues[index].struct5.unk_C + 1) - temp);

        dialogues[index].unk_D4 = dialogueAddresses[dialogues[index].struct5.dialogueIndex].vaddrIndex;
        
        dialogues[index].struct5.unk_16 = 0;
        dialogues[index].struct5.unk_17 = 0;

        dialogues[index].struct1.unk_0 = 0;
        dialogues[index].struct1.unk_2 = 0;
        dialogues[index].struct1.unk_4 = 0;
        dialogues[index].struct1.unk_6 = 0;
        dialogues[index].struct1.unk_8 = 0;
        dialogues[index].struct1.unk_A = 0;
        dialogues[index].struct1.unk_C = 0;
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
            dialogueBoxes[dialogues[index].struct5.unk_10].flags &= ~0x8000;
        }

        func_8003F130(dialogues[index].struct5.unk_10);
        func_80043B84(0);
        
        dialogues[index].struct5.flags = 5;
        
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043B84);

void func_80043B84(u16 index) {

    dialogueBoxes[dialogues[index].struct5.unk_12].flags &= ~0x8000;
    
    func_8003F130(dialogues[index].struct5.unk_12);
    
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

    ptr = dialogueAddresses[dialogues[index].struct5.dialogueIndex].romIndex;
    
    return ptr + dialogueAddresses[dialogues[index].struct5.dialogueIndex].vaddr[arg1];
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043CF8);

void func_80043CF8(u16 index, u16 arg1) {

    switch (gameVariables[index].set) {
        case 1:
            *(u8*)gameVariables[index].address = arg1;
            break;
        case 2:
            *(u16*)gameVariables[index].address = arg1;
            break;
        case 4:
            *(u32*)gameVariables[index].address = arg1;
            break;
    }
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043D8C);

u32 func_80043D8C(u16 index, u32 arg1) {

    switch (gameVariables[index].set) {
        case 1:
            arg1 = *(u8*)gameVariables[index].address;
            break;
        case 2:
            arg1 = *(u16*)gameVariables[index].address;
            break;
        case 4:
            arg1 = *(u32*)gameVariables[index].address;
            break;
    }

    return arg1;

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043E28);

void func_80043E28(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] |= 1 << (temp & 0x1F);
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043E5C);

void func_80043E5C(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] &= ~(1 << (temp & 0x1F));

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043E94);

u32 func_80043E94(u16 bitIndex) {
    
    u32 temp = bitIndex;
    
    return !(specialDialogueBitsPointer[temp >> 5] & (1 << (temp & 0x1F))) == 0;

}

INCLUDE_ASM(const s32, "system/dialogue", func_80043EC8);

INCLUDE_ASM(const s32, "system/dialogue", func_80044684);

INCLUDE_ASM(const s32, "system/dialogue", func_800449C4);

INCLUDE_ASM(const s32, "system/dialogue", func_80044BF4);

INCLUDE_ASM(const s32, "system/dialogue", func_80044D78);

INCLUDE_ASM(const s32, "system/dialogue", func_80045260);

//INCLUDE_ASM(const s32, "system/dialogue", func_80045CB0);

void func_80045CB0(void) {

    u16 i;
    bool set;

    for (i = 0; i < MAX_DIALOGUES; i++) {
        
        if (dialogues[i].struct5.flags & 1 && dialogues[i].struct5.flags & 2) {
            
            set = FALSE;
            
            if (dialogues[i].struct5.flags & 0x10) {
                
                if (dialogueBoxes[dialogues[i].struct5.unk_10].flags & 4 || dialogueBoxes[dialogues[i].struct5.unk_10].flags & 0x20000) {
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
