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
        D_801C3E40[i].struct5.unk_C = 0;
        D_801C3E40[i].struct5.unk_E = 0;
        D_801C3E40[i].struct5.unk_10 = 0;
        D_801C3E40[i].struct5.unk_0 = 0xFF;
        D_801C3E40[i].struct5.unk_4 = 0xFF;
        D_801C3E40[i].struct5.unk_8 = 0xFF;
        D_801C3E40[i].struct5.flags = 0;
    }
} 

//INCLUDE_ASM(const s32, "system/dialogue", func_80042FEC);

bool func_80042FEC(u16 arg0, u16 arg1, u16 arg2) {

    bool result = 0;

    if (arg0 == 0 && !(D_801C3E40[arg0].struct5.flags & 1)) {

        D_801C3E40[arg0].struct5.unk_0 = 0xFF;
        D_801C3E40[arg0].struct5.unk_4 = 0xFF;
        D_801C3E40[arg0].struct5.unk_8 = 0xFF;
        D_801C3E40[arg0].struct5.unk_10 = arg1;
        D_801C3E40[arg0].struct5.unk_12 = arg2;

        D_801C3E40[arg0].struct5.flags = 1;
        
        result = 1;
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043050);

bool func_80043050(u16 arg0, u16 arg1, u16 arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7) {

    bool result = 0;
    
    if (arg0 < 0x46) {

        result = 1;
        
        D_80205760[arg0].unk_14 = arg1;
        D_80205760[arg0].unk_16 = arg2;
        D_80205760[arg0].romStart = arg3;
        D_80205760[arg0].romEnd = arg4;
        D_80205760[arg0].vaddr = arg5;
        D_80205760[arg0].romIndex = arg6;
        D_80205760[arg0].vaddrIndex = arg7;
        
    } 
    
    return result;
}

//INCLUDE_ASM(const s32, "system/dialogue", setGameVariable);

bool setGameVariable(u16 index, void *address, u8 numSet, s32 max) {

    bool result = 0;

    if (index < TOTAL_GAME_VARIABLES) {

        if (numSet < 5) {
            gameVariables[index].address = address;
            gameVariables[index].set = numSet;
            gameVariables[index].maxValue = max;
            result = 1;
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

bool func_80043148(u16 arg0, u32 arg1, u32 arg2, u32 arg3) {

    bool result = 0;

    if (arg0 == 0 && D_801C3E40[arg0].struct5.flags & 1) {
        result = 1;
        D_801C3E40[arg0].struct5.unk_0 = arg1;
        D_801C3E40[arg0].struct5.unk_4 = arg2;
        D_801C3E40[arg0].struct5.unk_8 = arg3;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_8004318C);

bool func_8004318C(u16 arg0, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF) {

    bool result = 0;

    if (arg0 == 0 && (D_801C3E40[arg0].struct5.flags & 1)) {

        result = 1;

        D_801C3E40[arg0].struct2.unk_0 = arg2;
        D_801C3E40[arg0].struct2.unk_4 = arg3;
        D_801C3E40[arg0].struct2.unk_8 = arg4;
        D_801C3E40[arg0].struct2.unk_C = arg5;
        D_801C3E40[arg0].struct2.unk_10 = arg6;
        D_801C3E40[arg0].struct2.unk_14 = arg7;
        D_801C3E40[arg0].struct2.unk_18 = arg8;
        D_801C3E40[arg0].struct2.unk_1C = arg9;
        D_801C3E40[arg0].struct2.unk_20 = argA;
 
        D_801C3E40[arg0].struct2.unk_30 = arg1;
        D_801C3E40[arg0].struct2.unk_32 = argB;
        D_801C3E40[arg0].struct2.unk_34 = argC;

        D_801C3E40[arg0].struct2.unk_24.x = argD;
        D_801C3E40[arg0].struct2.unk_24.y = argE;
        D_801C3E40[arg0].struct2.unk_24.z = argF;
        
        
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043260);

bool func_80043260(u16 arg0, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF) {

    bool result = 0;

    if (arg0 == 0 && (D_801C3E40[arg0].struct5.flags & 1)) {
        
        result = 1;

        D_801C3E40[arg0].struct3.unk_0 = arg2;
        D_801C3E40[arg0].struct3.unk_4 = arg3; 
        D_801C3E40[arg0].struct3.unk_8 = arg4;
        D_801C3E40[arg0].struct3.unk_C = arg5;

        D_801C3E40[arg0].struct3.unk_10 = arg6;
        D_801C3E40[arg0].struct3.unk_14 = arg7;
        D_801C3E40[arg0].struct3.unk_18 = arg8;
        D_801C3E40[arg0].struct3.unk_1C = arg9;

        D_801C3E40[arg0].struct3.unk_20 = argA;

        D_801C3E40[arg0].struct3.unk_30 = arg1;
        D_801C3E40[arg0].struct3.unk_32 = argB;
        D_801C3E40[arg0].struct3.unk_34 = argC;

        D_801C3E40[arg0].struct3.unk_24.x = argD;
        D_801C3E40[arg0].struct3.unk_24.y = argE;
        D_801C3E40[arg0].struct3.unk_24.z = argF;
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043334);

bool func_80043334(u16 arg0, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF) {

    bool result = 0;

    if (arg0 == 0 && (D_801C3E40[arg0].struct5.flags & 1)) {

        result = 1;
        
        D_801C3E40[arg0].struct4.unk_0 = arg2;
        D_801C3E40[arg0].struct4.unk_4 = arg3; 
        D_801C3E40[arg0].struct4.unk_8 = arg4;
        D_801C3E40[arg0].struct4.unk_C = arg5;

        D_801C3E40[arg0].struct4.unk_10 = arg6;
        D_801C3E40[arg0].struct4.unk_14 = arg7;
        D_801C3E40[arg0].struct4.unk_18 = arg8;
        D_801C3E40[arg0].struct4.unk_1C = arg9;

        D_801C3E40[arg0].struct4.unk_20 = argA;

        D_801C3E40[arg0].struct4.unk_30 = arg1;
        D_801C3E40[arg0].struct4.unk_32 = argB;
        D_801C3E40[arg0].struct4.unk_34 = argC;

        D_801C3E40[arg0].struct4.unk_24.x = argD;
        D_801C3E40[arg0].struct4.unk_24.y = argE;
        D_801C3E40[arg0].struct4.unk_24.z = argF;
        
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

bool func_80043430(u16 index, u16 arg1, u16 arg2, u16 arg3) {

    bool result = 0;
    u32 temp;

    if (index == 0 && D_801C3E40[index].struct5.flags & 1) {

        func_8002B138(D_801C3E40[index].struct2.unk_30, 
            D_801C3E40[index].struct2.unk_0, 
            D_801C3E40[index].struct2.unk_4, 
            D_801C3E40[index].struct2.unk_8, 
            D_801C3E40[index].struct2.unk_C, 
            0, 
            0,  
            D_801C3E40[index].struct2.unk_10,
            0,
            D_801C3E40[index].struct2.unk_14,
            D_801C3E40[index].struct2.unk_18,
            D_801C3E40[index].struct2.unk_1C,
            D_801C3E40[index].struct2.unk_20,
            0,
            0);
 
        func_8002BD0C(D_801C3E40[index].struct2.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002BD90(D_801C3E40[index].struct2.unk_30, 1.0f, 1.0f, 1.0f);
        func_8002BE14(D_801C3E40[index].struct2.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002CB24(D_801C3E40[index].struct2.unk_30, 1);
        func_8002C914(D_801C3E40[index].struct2.unk_30, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(D_801C3E40[index].struct2.unk_30, 2, 2);
        func_8002C7EC(D_801C3E40[index].struct2.unk_30, 3);

        func_8002B138(D_801C3E40[index].struct3.unk_30, 
            D_801C3E40[index].struct3.unk_0, 
            D_801C3E40[index].struct3.unk_4, 
            D_801C3E40[index].struct3.unk_8, 
            D_801C3E40[index].struct3.unk_C, 
            0, 
            0,  
            D_801C3E40[index].struct3.unk_10,
            0,
            D_801C3E40[index].struct3.unk_14,
            D_801C3E40[index].struct3.unk_18,
            D_801C3E40[index].struct3.unk_1C,
            D_801C3E40[index].struct3.unk_20,
            0,
            0);

        func_8002BD0C(D_801C3E40[index].struct3.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002BD90(D_801C3E40[index].struct3.unk_30, 1.0f, 1.0f, 1.0f);
        func_8002BE14(D_801C3E40[index].struct3.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002CB24(D_801C3E40[index].struct3.unk_30, 1);
        func_8002C914(D_801C3E40[index].struct3.unk_30, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(D_801C3E40[index].struct3.unk_30, 2, 2);
        func_8002C7EC(D_801C3E40[index].struct3.unk_30, 3);

        func_8002B138(D_801C3E40[index].struct4.unk_30, 
            D_801C3E40[index].struct4.unk_0, 
            D_801C3E40[index].struct4.unk_4, 
            D_801C3E40[index].struct4.unk_8, 
            D_801C3E40[index].struct4.unk_C, 
            0, 
            0,  
            D_801C3E40[index].struct4.unk_10,
            0,
            D_801C3E40[index].struct4.unk_14,
            D_801C3E40[index].struct4.unk_18,
            D_801C3E40[index].struct4.unk_1C,
            D_801C3E40[index].struct4.unk_20,
            0,
            0);

        func_8002BD0C(D_801C3E40[index].struct4.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002BD90(D_801C3E40[index].struct4.unk_30, 1.0f, 1.0f, 1.0f);
        func_8002BE14(D_801C3E40[index].struct4.unk_30, 0.0f, 0.0f, 0.0f);
        func_8002CB24(D_801C3E40[index].struct4.unk_30, 1);
        func_8002C914(D_801C3E40[index].struct4.unk_30, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(D_801C3E40[index].struct4.unk_30, 2, 2);
        func_8002C7EC(D_801C3E40[index].struct4.unk_30, 3);
        
        D_801C3E40[index].struct1.unk_12 = 0xFF;
        D_801C3E40[index].struct5.unk_E = arg1;
        D_801C3E40[index].struct5.unk_C = arg2;

        nuPiReadRom(D_80205760[arg1].romStart, D_80205760[arg1].vaddr, D_80205760[arg1].romEnd - D_80205760[arg1].romStart);

        temp = func_80043C98(0, D_801C3E40[index].struct5.unk_C);

        nuPiReadRom(temp, D_80205760[D_801C3E40[index].struct5.unk_E].vaddrIndex, func_80043C98(0, D_801C3E40[index].struct5.unk_C + 1) - temp);

        D_801C3E40[index].unk_D4 = D_80205760[D_801C3E40[index].struct5.unk_E].vaddrIndex;
        
        D_801C3E40[index].struct5.unk_16 = 0;
        D_801C3E40[index].struct5.unk_17 = 0;

        D_801C3E40[index].struct1.unk_0 = 0;
        D_801C3E40[index].struct1.unk_2 = 0;
        D_801C3E40[index].struct1.unk_4 = 0;
        D_801C3E40[index].struct1.unk_6 = 0;
        D_801C3E40[index].struct1.unk_8 = 0;
        D_801C3E40[index].struct1.unk_A = 0;
        D_801C3E40[index].struct1.unk_C = 0;
        D_801C3E40[index].struct1.unk_E = 0;
        D_801C3E40[index].struct1.unk_10 = 0;
        
        D_801C3E40[index].struct1.unk_13 = 0;
        D_801C3E40[index].struct1.unk_14 = 0;
        D_801C3E40[index].struct1.unk_15 = 0;
        D_801C3E40[index].struct1.unk_16 = 0;
        D_801C3E40[index].struct1.unk_17 = 0;
        D_801C3E40[index].struct1.unk_18 = 0;

        
        D_801C3E40[index].struct5.flags &= ~4;
        D_801C3E40[index].struct5.flags |= 2;

        if (arg3 == 0x40) {
            D_801C3E40[index].struct5.flags |= (2 | 0x40);
        }  else {
            D_801C3E40[index].struct5.flags &= ~(0x40);
        }

        if (arg3 == 0x80) {
            D_801C3E40[index].struct5.flags |= 0x80;
        } else {
            D_801C3E40[index].struct5.flags &= ~0x80;
        }


        result = 1;

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043A88);

bool func_80043A88(void) {

    u16 i;
    u8 result = 0;

    for (i = 0; i < 1; i++) {
        if (D_801C3E40[i].struct5.flags & 4) {
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043AD8);

bool func_80043AD8(u16 index) {

    bool result = 0;

    if (index == 0 && D_801C3E40[index].struct5.flags & 1) {

        if (!(D_801C3E40[index].struct5.flags & 0x40)) {
            dialogueBoxes[D_801C3E40[index].struct5.unk_10].flags &= ~0x8000;
        }

        func_8003F130(D_801C3E40[index].struct5.unk_10);
        func_80043B84(0);
        
        D_801C3E40[index].struct5.flags = 5;
        
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043B84);

void func_80043B84(u16 arg0) {

    dialogueBoxes[D_801C3E40[arg0].struct5.unk_12].flags &= ~0x8000;
    
    func_8003F130(D_801C3E40[arg0].struct5.unk_12);
    
    func_8002BAD8(D_801C3E40[arg0].struct2.unk_30);
    func_8002BAD8(D_801C3E40[arg0].struct3.unk_30);
    func_8002BAD8(D_801C3E40[arg0].struct4.unk_30);
    
    D_801C3E40[arg0].struct5.flags &= ~0x20;
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C6C);

u8 func_80043C6C(u16 arg0) {
    return D_801C3E40[arg0].struct5.unk_17;
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043C98);

u32 func_80043C98(u16 arg0, u16 arg1) {

    u32 ptr;

    ptr = D_80205760[D_801C3E40[arg0].struct5.unk_E].romIndex;
    
    return ptr + D_80205760[D_801C3E40[arg0].struct5.unk_E].vaddr[arg1];
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043CF8);

void func_80043CF8(u16 arg0, u16 arg1) {

    switch (gameVariables[arg0].set) {
        case 1:
            *(u8*)gameVariables[arg0].address = arg1;
            break;
        case 2:
            *(u16*)gameVariables[arg0].address = arg1;
            break;
        case 4:
            *(u32*)gameVariables[arg0].address = arg1;
            break;
    }
    
}

//INCLUDE_ASM(const s32, "system/dialogue", func_80043D8C);

u32 func_80043D8C(u16 arg0, u32 arg1) {

    switch (gameVariables[arg0].set) {
        case 1:
            arg1 = *(u8*)gameVariables[arg0].address;
            break;
        case 2:
            arg1 = *(u16*)gameVariables[arg0].address;
            break;
        case 4:
            arg1 = *(u32*)gameVariables[arg0].address;
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
    u8 set;

    for (i = 0; i < 1; i++) {
        
        if (D_801C3E40[i].struct5.flags & 1 && D_801C3E40[i].struct5.flags & 2) {
            
            set = 0;
            
            if (D_801C3E40[i].struct5.flags & 0x10) {
                
                if (dialogueBoxes[D_801C3E40[i].struct5.unk_10].flags & 4 || dialogueBoxes[D_801C3E40[i].struct5.unk_10].flags & 0x20000) {
                    D_801C3E40[i].struct5.flags &= ~0x10;
                }
                
                set = 1;
            }
            
            if (D_801C3E40[i].struct5.flags & 0x20) {
                func_80044D78(i);
                set = 1;
            }
            
            if (!set) {
                func_80045260(i);
            }
        }
    }
    
}
