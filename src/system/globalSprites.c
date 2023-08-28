#include "common.h"

#include "sprite.h"

extern Sprite globalSprites[MAX_ACTIVE_SPRITES];

u8 func_8002B8E0(u16, u8, void*);
void *func_8002CD34(u16, void*);  


//INCLUDE_ASM(const s32, "system/globalSprites", func_8002AFE0);

void func_8002AFE0(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITES; i++) {
        
        globalSprites[i].flags2 = 0;
        globalSprites[i].flags1 = 0;
        
        globalSprites[i].unk_94 = 0;
        globalSprites[i].unk_90 = 0;
        globalSprites[i].unk_92 = 0;
        globalSprites[i].unk_91 = 0;
        
        globalSprites[i].shrink.x = 0;
        globalSprites[i].shrink.y = 0;
        globalSprites[i].shrink.z = 0;
        
        globalSprites[i].scale.x = 1.0f;
        globalSprites[i].scale.y = 1.0f;
        globalSprites[i].scale.z = 1.0f;

        globalSprites[i].vec3Unk.x = 0;
        globalSprites[i].vec3Unk.y = 0;
        globalSprites[i].vec3Unk.z = 0;
        
        globalSprites[i].rgbaCurrent.r = 0;
        globalSprites[i].rgbaCurrent.g = 0;
        globalSprites[i].rgbaCurrent.b = 0;
        globalSprites[i].rgbaCurrent.a = 0;
        
        globalSprites[i].rgbaDefault.r = 0;
        globalSprites[i].rgbaDefault.g = 0;
        globalSprites[i].rgbaDefault.b = 0;
        globalSprites[i].rgbaDefault.a = 0;
    }
    
}

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B138);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B36C);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B50C);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B6B8);

u32 func_8002B6B8(u16 index) {
    u32 result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].flags2 &= 0xFFFC;
            result = 1;
        }        
    }

    return result;
}

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B710);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B7A0);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B80C);

u8 func_8002B80C(u16 index, u16 offset, u8 arg2) {

    u32 *ptr;
    u8 result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {
            
            ptr = globalSprites[index].vaddrStart + (offset*(long long)4);
            
            // this also matches, but vaddrStart member must be int/u32/long pointer
            // if (*(globalSpritesindex].vaddrStart + offset) != *((globalSprites[index].vaddrStart + offset) + 1)) {

            if (*ptr != *(ptr+1)) {
                result = func_8002B8E0(index, arg2, func_8002CD34(offset, globalSprites[index].vaddrStart));
                globalSprites[index].animationCounter1 = 0xFF;
                globalSprites[index].animationCounter2 = 0xFF;
            }
        }
    }

    return result;   
}

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B8E0);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BAD8);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BB30);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BB88);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BBE0);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BCC8);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BD0C);

u8 func_8002BD0C(u16 index, f32 x, f32 y, f32 z) {

    u8 result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].shrink.x = x;
            globalSprites[index].shrink.y = y;
            globalSprites[index].shrink.z = z;
            result = 1;
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BD90);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BE14);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BE98);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002BF4C);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C000);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C0B4);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C1C0);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C52C);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C680);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C6F8);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C768);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C7EC);

u8 func_8002C7EC(u16 index, u16 arg1) {
    
    int temp;
    u8 result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & 1) {
            
            globalSprites[index].flags1 &= 0xE3FF;
            temp = arg1 << 10;
            globalSprites[index].flags1 |= temp;
            
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C85C);

u32 func_8002C85C(u16 index, u8 r, u8 g, u8 b, u8 a) {

    u32 result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            result = 1;
            globalSprites[index].rgba.r = r;
            globalSprites[index].rgba.g = g;
            globalSprites[index].rgba.b = b;
            globalSprites[index].rgba.a = a;
        }
    }
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C914);

u8 func_8002C914(u16 index, u8 r, u8 g, u8 b, u8 a) {

    u8 result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & 1) {        

            result = 1;
            
            globalSprites[index].rgbaCurrent.r = (globalSprites[index].rgba.r * r) / 255.0f;
            globalSprites[index].rgbaCurrent.g = (globalSprites[index].rgba.g * g) / 255.0f;
            globalSprites[index].rgbaCurrent.b = (globalSprites[index].rgba.b * b) / 255.0f;
            globalSprites[index].rgbaCurrent.a = (globalSprites[index].rgba.a * a) / 255.0f;

            globalSprites[index].rgbaDefault.r = (globalSprites[index].rgba.r * r) / 255.0f;
            globalSprites[index].rgbaDefault.g = (globalSprites[index].rgba.g * g) / 255.0f;
            globalSprites[index].rgbaDefault.b = (globalSprites[index].rgba.b * b) / 255.0f;
            globalSprites[index].rgbaDefault.a = (globalSprites[index].rgba.a * a) / 255.0f;
        }
    }
    
    return result;
}


INCLUDE_ASM(const s32, "system/globalSprites", func_8002CAA8);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CB24);

u8 func_8002CB24(u16 index, u8 flag) {

    u8 result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            if (flag == TRUE) {
                globalSprites[index].flags2 |= 0x80;
            } else {
                globalSprites[index].flags2 &= 0xFF7F;
            }
            result = 1;
        }
    }
    
    return result;
}


INCLUDE_ASM(const s32, "system/globalSprites", func_8002CB88);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CBF8);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CC44);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CC84);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CCA8);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CCDC);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CD34);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CD4C);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CDB4);

// unused
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u16 i = 0;
    // necessary to match
    u32 padding[2];
    
    if (arg0) {
        do {
            arg1 += 8;
            i++;
        } while (i < arg0);
    }
    
    return arg1;
}

// also matches
/*
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u32 result;
    u32 padding[2];
    
    u16 i = 0;
    
    if (arg0) {
        do {
            result += 8;
            i++;
        } while (i < arg0);
    }
    
    return result;
}
*/

INCLUDE_ASM(const s32, "system/globalSprites", func_8002CDE8);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002D3D4);
