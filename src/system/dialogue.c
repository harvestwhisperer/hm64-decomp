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
        dialogues[i].struct5.conversationIndex = 0;
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
    
    if (index < 0x46) {

        D_80205760[index].unk_14 = arg1;
        D_80205760[index].unk_16 = arg2;
        D_80205760[index].romStart = arg3;
        D_80205760[index].romEnd = arg4;
        D_80205760[index].vaddr = arg5;
        D_80205760[index].romIndex = arg6;
        D_80205760[index].vaddrIndex = arg7;
        
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

//INCLUDE_ASM(const s32, "system/dialogue", func_80043138);

bool func_80043138(u32* arg0) {
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

bool func_8004318C(u16 index, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & 1)) {

        dialogues[index].struct2.romTextureStart = arg2;
        dialogues[index].struct2.romTextureEnd = arg3;
        dialogues[index].struct2.romAssetIndexStart = arg4;
        dialogues[index].struct2.romAssetIndexEnd = arg5;
        dialogues[index].struct2.vaddrSpritesheet = arg6;
        dialogues[index].struct2.vaddrPalette = arg7;
        dialogues[index].struct2.vaddrUnknownAssetSheet = arg8;
        dialogues[index].struct2.vaddrUnknownAsset2 = arg9;
        dialogues[index].struct2.unk_20 = argA;
 
        dialogues[index].struct2.unk_30 = arg1;
        dialogues[index].struct2.unk_32 = argB;
        dialogues[index].struct2.unk_34 = argC;

        dialogues[index].struct2.unk_24.x = argD;
        dialogues[index].struct2.unk_24.y = argE;
        dialogues[index].struct2.unk_24.z = argF;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043260);

bool func_80043260(u16 index, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & 1)) {
        
        dialogues[index].struct3.romTextureStart = arg2;
        dialogues[index].struct3.romTextureEnd = arg3; 
        dialogues[index].struct3.romAssetIndexStart = arg4;
        dialogues[index].struct3.romAssetIndexEnd = arg5;

        dialogues[index].struct3.vaddrSpritesheet = arg6;
        dialogues[index].struct3.vaddrPalette = arg7;
        dialogues[index].struct3.vaddrUnknownAssetSheet = arg8;
        dialogues[index].struct3.vaddrUnknownAsset2 = arg9;

        dialogues[index].struct3.unk_20 = argA;

        dialogues[index].struct3.unk_30 = arg1;
        dialogues[index].struct3.unk_32 = argB;
        dialogues[index].struct3.unk_34 = argC;

        dialogues[index].struct3.unk_24.x = argD;
        dialogues[index].struct3.unk_24.y = argE;
        dialogues[index].struct3.unk_24.z = argF;
        
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043334);

bool func_80043334(u16 index, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].struct5.flags & 1)) {
        
        dialogues[index].struct4.romTextureStart = arg2;
        dialogues[index].struct4.romTextureEnd = arg3; 
        dialogues[index].struct4.romAssetIndexStart = arg4;
        dialogues[index].struct4.romAssetIndexEnd = arg5;

        dialogues[index].struct4.vaddrSpritesheet = arg6;
        dialogues[index].struct4.vaddrPalette = arg7;
        dialogues[index].struct4.vaddrUnknownAssetSheet = arg8;
        dialogues[index].struct4.vaddrUnknownAsset2 = arg9;

        dialogues[index].struct4.unk_20 = argA;

        dialogues[index].struct4.unk_30 = arg1;
        dialogues[index].struct4.unk_32 = argB;
        dialogues[index].struct4.unk_34 = argC;

        dialogues[index].struct4.unk_24.x = argD;
        dialogues[index].struct4.unk_24.y = argE;
        dialogues[index].struct4.unk_24.z = argF;
        
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

bool func_80043430(u16 index, u16 conversationIndex, u16 arg2, u16 arg3) {

    bool result = FALSE;
    u32 temp;

    if (index == 0 && dialogues[index].struct5.flags & 1) {

        func_8002B138(dialogues[index].struct2.unk_30, 
            dialogues[index].struct2.romTextureStart, 
            dialogues[index].struct2.romTextureEnd, 
            dialogues[index].struct2.romAssetIndexStart, 
            dialogues[index].struct2.romAssetIndexEnd, 
            0, 
            0,   
            dialogues[index].struct2.vaddrSpritesheet,
            0,
            dialogues[index].struct2.vaddrPalette,
            dialogues[index].struct2.vaddrUnknownAssetSheet,
            dialogues[index].struct2.vaddrUnknownAsset2,
            dialogues[index].struct2.unk_20,
            0,
            0);
 
        setSpriteShrinkFactor(dialogues[index].struct2.unk_30, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].struct2.unk_30, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].struct2.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].struct2.unk_30, 1);
        func_8002C914(dialogues[index].struct2.unk_30, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].struct2.unk_30, 2, 2);
        func_8002C7EC(dialogues[index].struct2.unk_30, 3);

        func_8002B138(dialogues[index].struct3.unk_30, 
            dialogues[index].struct3.romTextureStart, 
            dialogues[index].struct3.romTextureEnd,  
            dialogues[index].struct3.romAssetIndexStart, 
            dialogues[index].struct3.romAssetIndexEnd, 
            0, 
            0,  
            dialogues[index].struct3.vaddrSpritesheet,
            0,
            dialogues[index].struct3.vaddrPalette,
            dialogues[index].struct3.vaddrUnknownAssetSheet,
            dialogues[index].struct3.vaddrUnknownAsset2,
            dialogues[index].struct3.unk_20,
            0,
            0);

        setSpriteShrinkFactor(dialogues[index].struct3.unk_30, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].struct3.unk_30, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].struct3.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].struct3.unk_30, 1);
        func_8002C914(dialogues[index].struct3.unk_30, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].struct3.unk_30, 2, 2);
        func_8002C7EC(dialogues[index].struct3.unk_30, 3);

        func_8002B138(dialogues[index].struct4.unk_30, 
            dialogues[index].struct4.romTextureStart, 
            dialogues[index].struct4.romTextureEnd, 
            dialogues[index].struct4.romAssetIndexStart, 
            dialogues[index].struct4.romAssetIndexEnd, 
            0, 
            0,  
            dialogues[index].struct4.vaddrSpritesheet,
            0,
            dialogues[index].struct4.vaddrPalette,
            dialogues[index].struct4.vaddrUnknownAssetSheet,
            dialogues[index].struct4.vaddrUnknownAsset2,
            dialogues[index].struct4.unk_20,
            0,
            0);

        setSpriteShrinkFactor(dialogues[index].struct4.unk_30, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].struct4.unk_30, 1.0f, 1.0f, 1.0f);
        func_8002BE14(dialogues[index].struct4.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002CB24(dialogues[index].struct4.unk_30, 1);
        func_8002C914(dialogues[index].struct4.unk_30, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].struct4.unk_30, 2, 2);
        func_8002C7EC(dialogues[index].struct4.unk_30, 3);
        
        dialogues[index].struct1.unk_12 = 0xFF;
        dialogues[index].struct5.conversationIndex = conversationIndex;
        dialogues[index].struct5.unk_C = arg2;

        nuPiReadRom(D_80205760[conversationIndex].romStart, D_80205760[conversationIndex].vaddr, D_80205760[conversationIndex].romEnd - D_80205760[conversationIndex].romStart);

        temp = func_80043C98(0, dialogues[index].struct5.unk_C);

        nuPiReadRom(temp, D_80205760[dialogues[index].struct5.conversationIndex].vaddrIndex, func_80043C98(0, dialogues[index].struct5.unk_C + 1) - temp);

        dialogues[index].unk_D4 = D_80205760[dialogues[index].struct5.conversationIndex].vaddrIndex;
        
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
    
    func_8002BAD8(dialogues[index].struct2.unk_30);
    func_8002BAD8(dialogues[index].struct3.unk_30);
    func_8002BAD8(dialogues[index].struct4.unk_30);
    
    dialogues[index].struct5.flags &= ~0x20;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C6C);

u8 func_80043C6C(u16 index) {
    return dialogues[index].struct5.unk_17;
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C98);

u32 func_80043C98(u16 index, u16 arg1) {

    u32 ptr;

    ptr = D_80205760[dialogues[index].struct5.conversationIndex].romIndex;
    
    return ptr + D_80205760[dialogues[index].struct5.conversationIndex].vaddr[arg1];
    
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
