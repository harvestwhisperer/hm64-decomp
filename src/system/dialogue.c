#include "common.h"

#include "system/controller.h" 
#include "system/dialogue.h"
#include "system/mathUtils.h"
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
 
    for (i = 0; i < MAX_DIALOGUES; i++) {
        dialogues[i].struct5.dialogueIndex = 0;
        dialogues[i].struct5.dialogueMapAddressesIndex = 0;
        dialogues[i].struct5.dialogueBoxIndex1 = 0;
        dialogues[i].struct5.scrollSfxIndex = 0xFF;
        dialogues[i].struct5.closeSfxIndex = 0xFF;
        dialogues[i].struct5.unk_8 = 0xFF;
        dialogues[i].struct5.flags = 0;
    }
} 

//INCLUDE_ASM(const s32, "system/dialogue", func_80042FEC);

bool func_80042FEC(u16 index, u16 dialogueBoxIndex1, u16 dialogueBoxIndex2) {

    bool result = FALSE;
 
    if (index == 0 && !(dialogues[index].struct5.flags & ACTIVE)) {

        dialogues[index].struct5.scrollSfxIndex = 0xFF;
        dialogues[index].struct5.closeSfxIndex = 0xFF;
        dialogues[index].struct5.unk_8 = 0xFF;
        dialogues[index].struct5.dialogueBoxIndex1 = dialogueBoxIndex1;
        dialogues[index].struct5.dialogueBoxIndex2 = dialogueBoxIndex2;

        dialogues[index].struct5.flags = 1;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", setdialogueMapAddressInfo);

bool setDialogueMapAddressInfo(u16 index, u16 arg1, u16 arg2, u32 romStart, u32 romEnd, void* vaddr, u32 romIndex, void* vaddrIndex) {

    bool result = FALSE;
    
    if (index < TOTAL_CONVERSATION_BANKS) {

        dialogueMapAddresses[index].unk_14 = arg1;
        dialogueMapAddresses[index].unk_16 = arg2;
        dialogueMapAddresses[index].romStart = romStart;
        dialogueMapAddresses[index].romEnd = romEnd;
        dialogueMapAddresses[index].vaddr = vaddr;
        dialogueMapAddresses[index].romIndex = romIndex;
        dialogueMapAddresses[index].vaddrIndex = vaddrIndex;
        
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

    if (index == 0 && dialogues[index].struct5.flags & ACTIVE) {
        
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

    if (index == 0 && (dialogues[index].struct5.flags & ACTIVE)) {
  
        dialogues[index].dialogueButtonIcon1.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon1.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon1.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon1.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon1.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].dialogueButtonIcon1.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon1.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].dialogueButtonIcon1.vaddrIndexEnd = vaddrIndexEnd;
        dialogues[index].dialogueButtonIcon1.unk_20 = argA;
 
        dialogues[index].dialogueButtonIcon1.spriteIndex = spriteIndex;
        dialogues[index].dialogueButtonIcon1.spriteOffset = spriteOffset;
        dialogues[index].dialogueButtonIcon1.flag = flag;

        dialogues[index].dialogueButtonIcon1.coordinates.x = x;
        dialogues[index].dialogueButtonIcon1.coordinates.y = y;
        dialogues[index].dialogueButtonIcon1.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043260);

bool func_80043260(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & ACTIVE)) {
  
        dialogues[index].dialogueButtonIcon2.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon2.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon2.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon2.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon2.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].dialogueButtonIcon2.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon2.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].dialogueButtonIcon2.vaddrIndexEnd = vaddrIndexEnd;
        dialogues[index].dialogueButtonIcon2.unk_20 = argA;
 
        dialogues[index].dialogueButtonIcon2.spriteIndex = spriteIndex;
        dialogues[index].dialogueButtonIcon2.spriteOffset = spriteOffset;
        dialogues[index].dialogueButtonIcon2.flag = flag;

        dialogues[index].dialogueButtonIcon2.coordinates.x = x;
        dialogues[index].dialogueButtonIcon2.coordinates.y = y;
        dialogues[index].dialogueButtonIcon2.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043334);
 
bool func_80043334(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & ACTIVE)) {
        
        dialogues[index].dialogueButtonIcon3.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon3.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon3.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon3.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon3.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].dialogueButtonIcon3.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon3.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].dialogueButtonIcon3.vaddrIndexEnd = vaddrIndexEnd;
        dialogues[index].dialogueButtonIcon3.unk_20 = argA;
 
        dialogues[index].dialogueButtonIcon3.spriteIndex = spriteIndex;
        dialogues[index].dialogueButtonIcon3.spriteOffset = spriteOffset;
        dialogues[index].dialogueButtonIcon3.flag = flag;

        dialogues[index].dialogueButtonIcon3.coordinates.x = x;
        dialogues[index].dialogueButtonIcon3.coordinates.y = y;
        dialogues[index].dialogueButtonIcon3.coordinates.z = z;
        
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

bool func_80043430(u16 index, u16 dialogueMapAddressesIndex, u16 dialogueIndex, u16 flag) {

    bool result = FALSE;

    u32 romAddr;

    if (index == 0 && dialogues[index].struct5.flags & ACTIVE) {

        dmaSprite(dialogues[index].dialogueButtonIcon1.spriteIndex, 
            dialogues[index].dialogueButtonIcon1.romTextureStart, 
            dialogues[index].dialogueButtonIcon1.romTextureEnd, 
            dialogues[index].dialogueButtonIcon1.romIndexStart, 
            dialogues[index].dialogueButtonIcon1.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon1.vaddrTextureStart,
            NULL,
            dialogues[index].dialogueButtonIcon1.vaddrTextureEnd,
            dialogues[index].dialogueButtonIcon1.vaddrIndexStart,
            dialogues[index].dialogueButtonIcon1.vaddrIndexEnd,
            dialogues[index].dialogueButtonIcon1.unk_20,
            0,
            0);
 
        setSpriteShrinkFactor(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon1.spriteIndex, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].dialogueButtonIcon1.spriteIndex, 1);
        func_8002C914(dialogues[index].dialogueButtonIcon1.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].dialogueButtonIcon1.spriteIndex, 2, 2);
        func_8002C7EC(dialogues[index].dialogueButtonIcon1.spriteIndex, 3);

        dmaSprite(dialogues[index].dialogueButtonIcon2.spriteIndex, 
            dialogues[index].dialogueButtonIcon2.romTextureStart, 
            dialogues[index].dialogueButtonIcon2.romTextureEnd, 
            dialogues[index].dialogueButtonIcon2.romIndexStart, 
            dialogues[index].dialogueButtonIcon2.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon2.vaddrTextureStart,
            NULL,
            dialogues[index].dialogueButtonIcon2.vaddrTextureEnd,
            dialogues[index].dialogueButtonIcon2.vaddrIndexStart,
            dialogues[index].dialogueButtonIcon2.vaddrIndexEnd,
            dialogues[index].dialogueButtonIcon2.unk_20,
            0,
            0);

        setSpriteShrinkFactor(dialogues[index].dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon2.spriteIndex, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].dialogueButtonIcon2.spriteIndex, 1);
        func_8002C914(dialogues[index].dialogueButtonIcon2.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].dialogueButtonIcon2.spriteIndex, 2, 2);
        func_8002C7EC(dialogues[index].dialogueButtonIcon2.spriteIndex, 3);

        dmaSprite(dialogues[index].dialogueButtonIcon3.spriteIndex, 
            dialogues[index].dialogueButtonIcon3.romTextureStart, 
            dialogues[index].dialogueButtonIcon3.romTextureEnd, 
            dialogues[index].dialogueButtonIcon3.romIndexStart, 
            dialogues[index].dialogueButtonIcon3.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon3.vaddrTextureStart,
            NULL,
            dialogues[index].dialogueButtonIcon3.vaddrTextureEnd,
            dialogues[index].dialogueButtonIcon3.vaddrIndexStart,
            dialogues[index].dialogueButtonIcon3.vaddrIndexEnd,
            dialogues[index].dialogueButtonIcon3.unk_20,
            0,
            0);

        setSpriteShrinkFactor(dialogues[index].dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon3.spriteIndex, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].dialogueButtonIcon3.spriteIndex, 1);
        func_8002C914(dialogues[index].dialogueButtonIcon3.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].dialogueButtonIcon3.spriteIndex, 2, 2);
        func_8002C7EC(dialogues[index].dialogueButtonIcon3.spriteIndex, 3);
        
        dialogues[index].struct1.unk_12 = 0xFF;
        dialogues[index].struct5.dialogueMapAddressesIndex = dialogueMapAddressesIndex;
        dialogues[index].struct5.dialogueIndex = dialogueIndex;

        nuPiReadRom(dialogueMapAddresses[dialogueMapAddressesIndex].romStart, dialogueMapAddresses[dialogueMapAddressesIndex].vaddr, dialogueMapAddresses[dialogueMapAddressesIndex].romEnd - dialogueMapAddresses[dialogueMapAddressesIndex].romStart);

        romAddr = func_80043C98(0, dialogues[index].struct5.dialogueIndex);

        nuPiReadRom(romAddr, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].vaddrIndex, func_80043C98(0, dialogues[index].struct5.dialogueIndex + 1) - romAddr);

        dialogues[index].dialoguePointer = dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].vaddrIndex;
        
        dialogues[index].struct5.unk_16 = 0;
        dialogues[index].struct5.unk_17 = 0;

        dialogues[index].struct1.unk_0 = 0;
        dialogues[index].struct1.unk_2 = 0;
        dialogues[index].struct1.unk_4 = 0;
        dialogues[index].struct1.unk_6 = 0;
        dialogues[index].struct1.unk_8 = 0;
        dialogues[index].struct1.dialogueVariableValue = 0;
        dialogues[index].struct1.specialDialogueBit = 0;
        dialogues[index].struct1.updatedDialogueVariableAdjustment = 0;
        dialogues[index].struct1.unk_10 = 0;
        
        dialogues[index].struct1.dialogueVariablesIndex = 0;
        dialogues[index].struct1.unk_14 = 0;
        dialogues[index].struct1.unk_15 = 0;
        dialogues[index].struct1.unk_16 = 0;
        dialogues[index].struct1.unk_17 = 0;
        dialogues[index].struct1.unk_18 = 0;
        
        dialogues[index].struct5.flags &= ~4;
        dialogues[index].struct5.flags |= INITIALIZED;

        if (flag == 0x40) {
            dialogues[index].struct5.flags |= (INITIALIZED | 0x40);
        }  else {
            dialogues[index].struct5.flags &= ~(0x40);
        }

        if (flag == 0x80) {
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

    if (index == 0 && dialogues[index].struct5.flags & ACTIVE) {

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
    
    func_8002BAD8(dialogues[index].dialogueButtonIcon1.spriteIndex);
    func_8002BAD8(dialogues[index].dialogueButtonIcon2.spriteIndex);
    func_8002BAD8(dialogues[index].dialogueButtonIcon3.spriteIndex);
    
    dialogues[index].struct5.flags &= ~0x20;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C6C);

u8 func_80043C6C(u16 index) {
    return dialogues[index].struct5.unk_17;
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C98);

u32 func_80043C98(u16 index, u16 textIndex) {

    u32 ptr = dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].romIndex;
    
    return ptr + dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].vaddr[textIndex];
    
}

//INCLUDE_ASM(const s32, "system/dialogue", setDialogueVariable_inline);

inline void setDialogueVariable_inline(u16 index, u16 value) {

    switch (dialogueVariables[index].type) {
        case UNSIGNED_CHAR:
            *(u8*)dialogueVariables[index].value = value;
            break;
        case UNSIGNED_SHORT:
            *(u16*)dialogueVariables[index].value = value;
            break;
        case UNSIGNED_INT:
            *(u32*)dialogueVariables[index].value = value;
            break;
    }
    
}
 
//INCLUDE_ASM(const s32, "system/dialogue", getDialogueVariableValue);

inline u32 getDialogueVariableValue(u16 index) {

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

//INCLUDE_ASM(const s32, "system/dialogue", setSpecialDialogueBitFromPointer);

inline void setSpecialDialogueBitFromPointer(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] |= 1 << (temp & 0x1F);
    
}

//INCLUDE_ASM(const s32, "system/dialogue", toggleSpecialDialogueBitFromPointer);

inline void toggleSpecialDialogueBitFromPointer(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] &= ~(1 << (temp & 0x1F));

}

//INCLUDE_ASM(const s32, "system/dialogue", checkSpecialDialogueBitFromPointer);

inline u32 checkSpecialDialogueBitFromPointer(u16 bitIndex) {
    
    u32 temp = bitIndex;
    
    return !(specialDialogueBitsPointer[temp >> 5] & (1 << (temp & 0x1F))) == 0;

}

static inline u32 swap(u16 index) {

    u8* ptr;
    u8 temp;
    u32 temp2;

    ptr = dialogues[index].dialoguePointer++; 
    
    temp = *ptr++;
    
    dialogues[index].dialoguePointer += 1;
    
    temp2 = *(ptr) << 8;
    
    return temp | temp2;  
    
}

// parse dialogue map data and set struct
#ifdef PERMUTER
void func_80043EC8(u16 index) {

    u32 padding[0x12];

    dialogues[index].struct1.unk_12 = *(u8*)dialogues[index].dialoguePointer++;

    switch (dialogues[index].struct1.unk_12) {
        
        case 0:
            dialogues[index].struct1.unk_0 = swap(index);  
            break;
        
        case 1:
            dialogues[index].struct1.unk_13 = (*(u8*)dialogues[index].dialoguePointer++);  
            dialogues[index].struct1.unk_4 = swap(index);
            dialogues[index].struct1.unk_6 = swap(index);        
            dialogues[index].struct1.unk_0 = swap(index);
            dialogues[index].struct1.unk_2 = swap(index);  
            
            break;
        
        case 2:
            
            dialogues[index].struct1.unk_13 = (*(u8*)dialogues[index].dialoguePointer++);
            dialogues[index].struct1.unk_E = swap(index);  
            break;
        
        case 3:
            
            dialogues[index].struct1.unk_13 = (*(u8*)dialogues[index].dialoguePointer++);  
            dialogues[index].struct1.unk_A = swap(index);  
            break;
        
        case 4:
            
            dialogues[index].struct1.unk_C = swap(index);
            dialogues[index].struct1.unk_0 = swap(index);  
            dialogues[index].struct1.unk_2 = swap(index);  
            break;
        
        case 5:
            dialogues[index].struct1.unk_C = swap(index);                
            break;

        case 6:
            dialogues[index].struct1.unk_C = swap(index);
            break;

        case 7:
 
            dialogues[index].struct1.unk_14 = swap(index);
            dialogues[index].struct1.unk_15 = swap(index);
            dialogues[index].struct1.unk_0 = swap(index);
            dialogues[index].struct1.unk_2 = swap(index);  
            dialogues[index].struct1.unk_8 = getRandomNumberInRange(0, 99);  
            break;

        case 8:
            dialogues[index].struct1.unk_2 = swap(index);  
            break;
        
        case 10:
            dialogues[index].struct1.unk_10 = swap(index);
            dialogues[index].struct1.unk_17 = swap(index);     
            break;
    
        case 11:
            dialogues[index].struct1.unk_18 = swap(index);
            dialogues[index].struct1.unk_2 = swap(index);     
            break;

        case 12:
            break;
    
    }
    
}
#else
INCLUDE_ASM(const s32, "system/dialogue", func_80043EC8);
#endif

#ifdef PERMUTER
void func_80044684(u16 index) {

    f32 temp1, temp2;

    temp1 = -(dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_92 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].fontContext.unk_60) - (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_92 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9B) - dialogues[index].dialogueButtonIcon1.coordinates.x;
    temp2 = (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].fontContext.unk_61) + (dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9C) - dialogues[index].dialogueButtonIcon1.coordinates.y;    

    setSpriteScale(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A0 * 0.5f, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * 0.6f, 1.0f);
    
    func_8002C7EC(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 3);
    func_8002C914(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 255, 255, 255, 192);
    func_8002CB24(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 1);

    func_8002B80C(dialogues[index].dialogueButtonIcon1.spriteIndex, dialogues[index].dialogueButtonIcon1.spriteOffset, dialogues[index].dialogueButtonIcon1.flag);
    setSpriteShrinkFactor(dialogues[index].dialogueButtonIcon1.spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.x + temp1, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.y + temp2, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.z);

}
#else
INCLUDE_ASM(const s32, "system/dialogue", func_80044684);
#endif

//INCLUDE_ASM(const s32, "system/dialogue", func_800449C4);

void func_800449C4(u16 index) {

    f32 tempX = dialogues[index].dialogueButtonIcon2.coordinates.x;
    f32 tempY = dialogues[index].dialogueButtonIcon2.coordinates.y;

    setSpriteScale(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_A0 * 0.5f, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_93 * 0.6f, 1.0f);
    
    // flags
    func_8002C7EC(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 3);
    func_8002CB24(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 1);

    // animation
    func_8002B80C(dialogues[index].dialogueButtonIcon2.spriteIndex, dialogues[index].dialogueButtonIcon2.spriteOffset, dialogues[index].dialogueButtonIcon2.flag);
    setSpriteShrinkFactor(dialogues[index].dialogueButtonIcon2.spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.x + tempX, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.y + tempY, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.z);

}

//INCLUDE_ASM(const s32, "system/dialogue", func_80044BF4);

void func_80044BF4(u16 index) {
 
    f32 tempX = dialogues[index].dialogueButtonIcon3.coordinates.x;
    f32 tempY = dialogues[index].dialogueButtonIcon3.coordinates.y;

    // flags
    func_8002C7EC(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 3);
    func_8002CB24(dialogueWindows[dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].dialogueWindowIndex].spriteIndex, 1);

    // animation
    func_8002B80C(dialogues[index].dialogueButtonIcon3.spriteIndex, dialogues[index].dialogueButtonIcon3.spriteOffset, dialogues[index].dialogueButtonIcon3.flag);
    setSpriteShrinkFactor(dialogues[index].dialogueButtonIcon3.spriteIndex, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.x + tempX, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.y + tempY, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].shrink.z);

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
                    adjustSpriteShrinkFactor(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, -dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].fontContext.unk_61 - dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9C, 0.0f);
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
                        adjustSpriteShrinkFactor(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].fontContext.unk_61 + dialogueBoxes[dialogues[index].struct5.dialogueBoxIndex2].unk_9C, 0.0f);
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
                func_8002BAD8(dialogues[index].dialogueButtonIcon3.spriteIndex);
            }

            if (dialogues[index].struct5.unk_19) {
                func_800449C4(index);
            } else {
                func_8002BAD8(dialogues[index].dialogueButtonIcon2.spriteIndex);
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
    u16 dialogueIndex;
    
    int adjusted;
    int max;
    int initial;
    
    while (!set) {
        
        if (dialogues[index].struct1.unk_12 == 0xFF) {
            // set struct data from dialogue map
            func_80043EC8(index);
        }
        
        switch (dialogues[index].struct1.unk_12) {
        
            case 0:
                if (dialogues[index].struct5.flags & 0x80) {
                    initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogues[index].struct1.unk_0, 0);
                } else {
                    initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogues[index].struct1.unk_0, 0x8000);
                }
                
                set = TRUE;

                dialogues[index].struct1.unk_12 = 0xFF;
                dialogues[index].struct5.flags |= 0x10;
                
                break;

            case 1:
                
                temp = getDialogueVariableValue(dialogues[index].struct1.dialogueVariablesIndex);
                
                if ((temp >= dialogues[index].struct1.unk_4) && (dialogues[index].struct1.unk_6 >= temp)) {
                
                    dialogueIndex = dialogues[index].struct1.unk_0;
                    
                    if (dialogueIndex != 0xFFFF) {

                        if (dialogues[index].struct5.flags & 0x80) {
                            initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogueIndex, 0);
                        } else {
                            initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogueIndex, 0x8000);
                        }
                        
                        set = TRUE;
                        
                        dialogues[index].struct1.unk_0 = 0xFFFF;
                        dialogues[index].struct5.flags |= 0x10;
                        
                    } else {
                        
                        dialogueIndex = dialogues[index].struct1.unk_2;
                        
                        if (dialogueIndex != 0xFFFF) {
                            func_80043430(index, dialogues[index].struct5.dialogueMapAddressesIndex, dialogueIndex, dialogues[index].struct5.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].struct1.unk_12 = 0xFF;
                        }
                    }
                    
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            // update dialogue variable value
            case 2:

                /*

                temp4 = (u16)getDialogueVariableValue(dialogues[index].struct1.dialogueVariablesIndex);

                temp4 += adjustValue_dialogue_c(temp4, dialogues[index].struct1.updatedDialogueVariableAdjustment, dialogueVariables[dialogues[index].struct1.dialogueVariablesIndex].maxValue);
                
                setDialogueVariable_inline(dialogues[index].struct1.dialogueVariablesIndex, temp4);
                
                dialogues[index].struct1.unk_12 = 0xFF;
                
                break;

                */

                temp4 = getDialogueVariableValue(dialogues[index].struct1.dialogueVariablesIndex);

                // FIXME: should be inline adjustValue call

                max = dialogueVariables[dialogues[index].struct1.dialogueVariablesIndex].maxValue;
                adjusted = dialogues[index].struct1.updatedDialogueVariableAdjustment;
                
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
                
                // FIXME: should be inline setDialogueVariable_inline call

                tempIndex = dialogues[index].struct1.dialogueVariablesIndex;
                
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
                setDialogueVariable_inline(dialogues[index].struct1.dialogueVariablesIndex, dialogues[index].struct1.dialogueVariableValue);
                dialogues[index].struct1.unk_12 = 0xFF;
                break;

            case 4:
                
                if (checkSpecialDialogueBitFromPointer(dialogues[index].struct1.specialDialogueBit)) {
                    
                    dialogueIndex = dialogues[index].struct1.unk_0;
                
                    if (dialogueIndex != 0xFFFF) {
                        
                        if (dialogues[index].struct5.flags & 0x80) {
                            initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogueIndex, 0);
                        } else {
                            initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogueIndex, 0x8000);
                        }
                        
                        set = TRUE;
                        dialogues[index].struct1.unk_0 = 0xFFFF;
                        dialogues[index].struct5.flags |= 0x10;
                    
                    } else {
                        
                        dialogueIndex = dialogues[index].struct1.unk_2;

                        if (dialogueIndex != 0xFFFF) {
                            func_80043430(index, dialogues[index].struct5.dialogueMapAddressesIndex, dialogueIndex, dialogues[index].struct5.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].struct1.unk_12 = 0xFF;
                        }

                    }
                    
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            case 5:
                setSpecialDialogueBitFromPointer(dialogues[index].struct1.specialDialogueBit);
                dialogues[index].struct1.unk_12 = 0xFF;
                break;

            case 6:
                toggleSpecialDialogueBitFromPointer(dialogues[index].struct1.specialDialogueBit);
                dialogues[index].struct1.unk_12 = 0xFF;
                break;

            case 7:
                
                temp3 = dialogues[index].struct1.unk_8;
                
                if ((temp3 >= dialogues[index].struct1.unk_14) && (dialogues[index].struct1.unk_15 >= temp3)) {
                
                    dialogueIndex = dialogues[index].struct1.unk_0;
                    
                    if (dialogueIndex != 0xFFFF) {
                        if (dialogues[index].struct5.flags & 0x80) {
                            initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogueIndex, 0);
                        } else {
                            initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex1, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_14, dialogueIndex, 0x8000);
                        }
                        
                        set = TRUE;
                        dialogues[index].struct1.unk_0 = 0xFFFF;
                        dialogues[index].struct5.flags |= 0x10;
                        
                    } else {
                        
                        dialogueIndex = dialogues[index].struct1.unk_2;
                        
                        if (dialogueIndex != 0xFFFF) {
                            func_80043430(index, dialogues[index].struct5.dialogueMapAddressesIndex, dialogueIndex, dialogues[index].struct5.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].struct1.unk_12 = 0xFF;
                        }
                    }
                    
                } else {
                    dialogues[index].struct1.unk_12 = 0xFF;
                }
                
                break;

            case 8:
                func_80043430(index, dialogues[index].struct5.dialogueMapAddressesIndex, dialogues[index].struct1.unk_2, dialogues[index].struct5.flags & (0x40 | 0x80));
                break;

            case 10:
                
                dialogues[index].struct1.unk_12 = 0xFF;
                dialogues[index].struct5.unk_17 = 0;
                dialogues[index].struct5.unk_18 = 0;
                dialogues[index].struct5.unk_19 = 0;
                dialogues[index].struct5.unk_14 = dialogues[index].struct1.unk_10;
                dialogues[index].struct5.flags |= 0x20;
                
                initializeDialogueBox(dialogues[index].struct5.dialogueBoxIndex2, dialogueMapAddresses[dialogues[index].struct5.dialogueMapAddressesIndex].unk_16, dialogues[index].struct5.unk_14, 0);
                
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
                    func_80043430(index, dialogues[index].struct5.dialogueMapAddressesIndex, dialogues[index].struct1.unk_2, dialogues[index].struct5.flags & (0x40 | 0x80));
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
        
        if ((dialogues[i].struct5.flags & ACTIVE) && (dialogues[i].struct5.flags & INITIALIZED)) {
            
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
