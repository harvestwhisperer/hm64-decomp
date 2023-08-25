#include "common.h"

void func_8002B138(u16, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, u8); 
void func_8002BD90(u16, f32, f32, f32);                        
void func_8002BE14(u16, f32, f32, f32);                        
void func_8002C7EC(u16, u16);                              
u8 func_8002C914(u16, u8, u8, u8, u8);                    
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

// convert to array?
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

// jtbl_80122760
INCLUDE_ASM(const s32, "tv", func_800D86D8);

// void func_800D86D8(void) {
    
//     u8 *ptr;
//     u8 temp;
    
//     switch (gDayOfWeek) {
//         case 2:
//             ptr = &D_80170276;
//             break;
        
//         case 3:
//             ptr = &D_80170277;
//             break;
        
//         case 4:
//             ptr = &D_80170278;
//             break;
        
//         case 5:
//             ptr = &D_80170279;
//             break;

//         case 6:
//             ptr = &D_8017027A;
//             break;
        
//         case 0:
//             ptr = &D_8017027B;
//             temp = *ptr+1;
//             *ptr = temp;
//             if (!(temp < 0x14)) {
//                 *ptr = 0;
//             }
//             return;
        
//         case 1:
//             ptr = &D_8017027C;
//             break;

//         default:
//             return;
//     }

//     temp = *ptr+1;
//     *ptr = temp;
    
// check:
//     if (!(temp < 0xA)) {
//         *ptr = 0;
//     }
// }

// jtbl_80122780
INCLUDE_ASM(const s32, "tv", func_800D879C);

// jtbl_801227A0
// jtbl_801227F0
INCLUDE_ASM(const s32, "tv", func_800D897C);

// jtbl_80122808
INCLUDE_ASM(const s32, "tv", func_800D9298);

INCLUDE_ASM(const s32, "tv", func_800D93CC);
