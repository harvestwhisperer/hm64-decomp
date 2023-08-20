#include "common.h"

void func_8002B138(u16, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, u8); 
void func_8002BD90(u16, f32, f32, f32);                        
void func_8002BE14(u16, f32, f32, f32);                        
void func_8002C7EC(u16, u16);                              
void func_8002C914(u16, u8, u8, u8, u8);                    
void func_80034C40(u16, u16, u16, u16, f32, f32, f32, s32, s32, s32, s32); 
void func_80034E64(u8, u8);                              
void func_800D879C();                                  
void func_800D897C();                                  
void func_800D9298();                                  

// likely bss
extern u8 D_80170273;
extern u8 D_80170274;

// rodata
extern void *tvSprites_romTextureStart;
extern void *tvSprites_romTextureEnd;
extern void *tvSprites_romPaletteStart;
extern void *tvSprites_romPaletteEnd;
extern void *tvSprites_romIndexStart;
extern void *tvSprites_romIndexEnd;

// convert to array
extern u8 D_80170276;
extern u8 D_80170277;
extern u8 D_80170278;
extern u8 D_80170279;
extern u8 D_8017027A;
extern u8 D_8017027B;
extern u8 D_8017027C;

extern u8 gDayOfWeek;

//INCLUDE_ASM(const s32, "tv", func_800D8540);

void func_800D8540(void) {
    func_800D879C();
    func_800D897C();
    func_800D9298();
    func_80034E64(0, 9);
    func_8002B138(0x6A, &tvSprites_romTextureStart, &tvSprites_romTextureEnd, &tvSprites_romPaletteStart, &tvSprites_romPaletteEnd, &tvSprites_romIndexStart, &tvSprites_romIndexEnd, 0x802EB800, 0x802EC800, 0x802ED800, 0x802EDB00, 0x802EDE00, 0x802EDF00, 1, 1);
    func_8002BD90(0x6A, 2.0f, 2.0f, 1.0f);
    func_8002BE14(0x6A, 45.0f, -45.0f, 0.0f);
    func_8002C7EC(0x6A, 3);
    func_8002C914(0x6A, 0xFF, 0xFF, 0xFF, 0xFF);
    func_80034C40(0, 9, 0x6A, D_80170273, -12.0f, 36.0f, -154.0f, 0xFF, 0xFE, 0, 0);
    D_80170274 = 0;
}


#ifdef PERMUTER
void func_800D86D8(void) {
    
    u8 temp_v0;
    u8 temp_v0_2;

    u32 checkSize;
    u8* ptr;

    switch (gDayOfWeek) {
        case 2:
            ptr = &D_80170276;
            break;
        case 3:
            ptr = &D_80170277;
            break;
        case 4:
            ptr = &D_80170278;
            break;
        case 5:
            ptr = &D_80170279;
            break;
        case 6:
            ptr = &D_8017027A;
            goto block_9;
        case 0:
            ptr = &D_8017027B;
            temp_v0 = *ptr;
            *ptr = temp_v0+1;
            checkSize = *ptr < 0x14;
            goto block_10;
        case 1:
            ptr = &D_8017027C;
            goto block_9;
        }
block_9:
        temp_v0 = *ptr + 1;
        *ptr++;
        checkSize = temp_v0 < 0xA;
block_10:
    if (!checkSize) {
        *ptr = 0;
    }
}
#else
INCLUDE_ASM(const s32, "tv", func_800D86D8);
#endif

INCLUDE_ASM(const s32, "tv", func_800D879C);

INCLUDE_ASM(const s32, "tv", func_800D897C);

INCLUDE_ASM(const s32, "tv", func_800D9298);

INCLUDE_ASM(const s32, "tv", func_800D93CC);
