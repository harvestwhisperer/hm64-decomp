#include "common.h"

#include "system/worldGraphics.h"

#include "system/graphic.h"
#include "system/map.h"

// bss
extern u16 spriteCounter;

// unused
extern u8 D_80189135[9];

extern WorldGraphics D_8018A850[0x150];


//INCLUDE_ASM(const s32, "system/worldGraphics", func_80029170);

void func_80029170(void) {

    u16 i;
    
    D_80189135[0] = 0xFF;
    D_80189135[1] = 0xFF;
    D_80189135[2] = 0xFF;
    D_80189135[3] = 0xFF;
    D_80189135[4] = 0xFF;
    D_80189135[5] = 0xFF;
    D_80189135[6] = 0xFF;
    D_80189135[7] = 0xFF;
    D_80189135[8] = 0xFF;

    for (i = 0; i < 0x150; i++) {
        D_8018A850[i].flags = 0;
        D_8018A850[i].position.x = 0;
        D_8018A850[i].position.y = 0;
        D_8018A850[i].position.z = 0;
        D_8018A850[i].scaling.x = 0;
        D_8018A850[i].scaling.y = 0;
        D_8018A850[i].scaling.z = 0;
        D_8018A850[i].rotation.x = 0;
        D_8018A850[i].rotation.y = 0;
        D_8018A850[i].rotation.z = 0;
    }
    
    resetSpriteCounter();
    
}

//INCLUDE_ASM(const s32, "system/worldGraphics", resetSpriteCounter);

void resetSpriteCounter(void) {
    spriteCounter = 0;
}

void func_80029294(void) {}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_8002929C);

u16 func_8002929C(Gfx *dl, u16 flags) {

    u16 index = spriteCounter;

    D_8018A850[index].dl = dl;
    D_8018A850[index].flags = flags | 1;
    
    spriteCounter++;
    
    return index;
}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_800292EC);

u8 func_800292EC(u16 index, f32 x, f32 y, f32 z) {
    D_8018A850[index].position.x = x;
    D_8018A850[index].position.y = y;
    D_8018A850[index].position.z = z;
    return 1;
}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_80029330);

u8 func_80029330(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    D_8018A850[index].scaling.x = arg1;
    D_8018A850[index].scaling.y = arg2;
    D_8018A850[index].scaling.z = arg3;
    return 1;
}

//INCLUDE_ASM(const s32, "system/worldGraphics", func_80029374);

u8 func_80029374(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    D_8018A850[index].rotation.x = arg1;
    D_8018A850[index].rotation.y = arg2;
    D_8018A850[index].rotation.z = arg3;
    return 1;
}

void func_800293B8(void) {}

INCLUDE_ASM(const s32, "system/worldGraphics", func_800293C0);
