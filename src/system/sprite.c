#include "common.h"

#include "sprite.h"
#include "graphic.h"

extern u16 D_801C3E2A;

extern const char *D_8011ED4C;
extern const char *D_8011ED50;

extern Bitmap D_801F7110[0xB0];
extern Gfx D_80215ED0[4][1];
extern u16 D_8021E6E6[2][128][32];

extern volatile u32 gDisplayContext;
 
void __assert(const char*, const char*, u32);                              
               
Gfx* func_8002A66C(Gfx*, Bitmap*, u16);             
bool func_8002ACA4(Bitmap*, Gfx*);                      

//INCLUDE_ASM(const s32, "system/sprite", func_80029B30);

void func_80029B30(void) {

    u16 i, j, k;

    for (i = 0; i < 0xB0; i++) {
        D_801F7110[i].flags = 0;
        D_801F7110[i].unk_54 = 0;
        D_801F7110[i].spriteNumber = 0;
        D_801F7110[i].size = 0;
        D_801F7110[i].unk_1C.x = 0;
        D_801F7110[i].unk_1C.y = 0;
        D_801F7110[i].unk_1C.z = 0;
        D_801F7110[i].scaling.x = 1.0f;
        D_801F7110[i].scaling.y = 1.0f;
        D_801F7110[i].scaling.z = 1.0f;
        D_801F7110[i].unk_34.x = 0;
        D_801F7110[i].unk_34.y = 0;
        D_801F7110[i].unk_34.z = 0;
        D_801F7110[i].rgba.r = 255.0f;
        D_801F7110[i].rgba.g = 255.0f;
        D_801F7110[i].rgba.b = 255.0f;
        D_801F7110[i].rgba.a = 255.0f;
    }

    for (j = 0; j < 2; j++) {
        for (k = 0; k < 0x80; k++) {
            D_8021E6E6[j][k][0] = 0;
            D_8021E6E6[j][k][8] = 0;
            D_8021E6E6[j][k][16] = 0;
            D_8021E6E6[j][k][24] = 0;
        }
    }
    
    D_801C3E2A = 0;
}

#ifdef PERMUTER
void func_80029CC8(void) {

    u32 i;
    D_801C3E2A = 0;

    i = 0x3C28;

    while (1) {
        D_801F7110[i].flags = 0;
        i -= 0x58;
        if (i = 0) break;
    };
}
#else
INCLUDE_ASM(const s32, "system/sprite", func_80029CC8);
#endif

//INCLUDE_ASM(const s32, "system/sprite", func_80029CF4);

u16 func_80029CF4(u8 *timg, u8 *pal, u32 width, u32 height, u32 fmt, u32 flag, u32 arg6, u32 arg7, u16 flags) {
    
    u16 result;
    
    result = D_801C3E2A;
    
    if (D_801C3E2A < 176) {
        D_801F7110[D_801C3E2A].timg = timg;
        D_801F7110[D_801C3E2A].pal = pal;
        D_801F7110[D_801C3E2A].width = width;
        D_801F7110[D_801C3E2A].height = height;
        D_801F7110[D_801C3E2A].fmt = fmt;
        D_801F7110[D_801C3E2A].flag = flag;
        D_801F7110[D_801C3E2A].flags = flags | 1;
        D_801F7110[D_801C3E2A].unk_54 = 0;
        D_801C3E2A++;
    } else {
        result = 0xFFFF;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029DAC);

u16 func_80029DAC(void *timg, void *pal, u16 flags) {

    u16 result = D_801C3E2A;
    
    if (D_801C3E2A < 0xB0) {
        D_801F7110[D_801C3E2A].flags = flags | 1;
        D_801F7110[D_801C3E2A].unk_54 = 0;
        D_801F7110[D_801C3E2A].timg = timg;
        D_801F7110[D_801C3E2A].pal = pal;
        D_801C3E2A++;
    } else {
        result = 0xFFFF;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029E2C);

bool func_80029E2C(u16 index, u16 arg1, u16 arg2) {
    
    bool result;
    u32 temp1;
    u32 temp2;

    result = 0;

    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {

            D_801F7110[index].unk_54 &= 0xFF87;

            temp1 = arg1 << 3;
            D_801F7110[index].unk_54 |= temp1;

            temp2 = arg2 << 5;
            D_801F7110[index].unk_54 |= temp2;

            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029EA4);

bool func_80029EA4(u16 index, u16 arg1) {
    
    bool result;
    int temp;
    result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].unk_54 &= 0xFE7F;
            temp = arg1 << 7;
            D_801F7110[index].unk_54 |= temp;
        }
    }
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029F14);

bool func_80029F14(u16 index, u16 arg1) {

    bool result;
    
    result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            if (arg1) {
                D_801F7110[index].unk_54 |= 0x200;
            } else {
                D_801F7110[index].unk_54 &= 0xFDFF;
            }
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029F98);

bool func_80029F98(u16 index, u8 arg1, u8 arg2) {
    
    bool result;
    
    result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
    
            if (arg1) {
                D_801F7110[index].unk_54 |= 1;
            }
            
            result = 1;
            
            if (arg2) {
               D_801F7110[index].unk_54 |= 2;
            }
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A02C);

bool func_8002A02C(u16 index, u16 arg1) {

    bool result;
    int temp;
    
    result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].unk_54 &= 0xE3FF;
            temp = arg1 << 10;
            D_801F7110[index].unk_54 |= temp;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A09C);

bool func_8002A09C(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].unk_1C.x = arg1;
            D_801F7110[index].unk_1C.y = arg2;
            D_801F7110[index].unk_1C.z = arg3;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A120);

bool func_8002A120(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].scaling.x = arg1;
            D_801F7110[index].scaling.y = arg2;
            D_801F7110[index].scaling.z = arg3;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A1A4);

bool func_8002A1A4(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    bool result = 0;

    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].unk_34.x = arg1;
            D_801F7110[index].unk_34.y = arg2;
            D_801F7110[index].unk_34.z = arg3;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A228);

bool func_8002A228(u16 index, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = 0;
    
    if (index < 0xB0) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].rgba.r = r;
            D_801F7110[index].rgba.g = g;
            D_801F7110[index].rgba.b = b;
            D_801F7110[index].rgba.a = a;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/sprite", func_8002A2E0);

//INCLUDE_ASM(const s32, "system/sprite", func_8002A340);

u8 *func_8002A340(u16 arg0, u32 *start, u8 *timg, u8 *romAddr) {

    int offset = *(start + arg0);

    setSpriteAddresses(romAddr + offset, timg, func_8002A3A0(arg0, start));

    return timg;

}

// get length of texture segment
INCLUDE_ASM(const s32, "system/sprite", func_8002A3A0);

// display lists

// have to match functions to fix asm referencing each array element as its own variable
// func_8002A66C and func_8002A410
/*
static const Gfx D_8011ECC0[] = {

    gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2),

};
*/

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECC0);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECC4);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECC8);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECCC);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECD0);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECD4);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECD8);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECDC);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECE0);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ECE4);


INCLUDE_ASM(const s32, "system/sprite", func_8002A410);

INCLUDE_ASM(const s32, "system/sprite", func_8002A530);


// func_8002A66C
/*
static const Gfx D_8011ED00[] = {

    gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureFilter(G_TF_POINT),
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsDPPipeSync(),
    gsSPEndDisplayList()

};
*/

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED00);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED04);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED08);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED0C);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED10);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED14);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED18);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED1C);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED20);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED24);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED28);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED2C);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED30);

INCLUDE_RODATA(const s32, "system/sprite", D_8011ED34);



INCLUDE_ASM(const s32, "system/sprite", func_8002A66C);

INCLUDE_ASM(const s32, "system/sprite", func_8002ACA4);

#ifdef PERMUTER
void func_8002AE58(void) {

    u16 i;
    u32 count = 0;
    Gfx *dl;
    Gfx *tempDl;

    dl = &D_80215ED0[gDisplayContext*0x880][0];

    for (i = 0; i < 0xB0; i++) {
        if (D_801F7110[i].flags & 1) {
            func_80026E78(&D_801F7110[i], D_801F7110[i].vaddr1, D_801F7110[i].vaddr2);
            tempDl = func_8002A66C(dl, &D_801F7110[i], count);
            dl = tempDl;
            func_8002ACA4(&D_801F7110[i], dl);
            count += D_801F7110[i].unk_1A;
            D_801F7110[i].flags &= 0xFFFE;
        }
    }

    if (((dl - &D_80215ED0[0][0]) - (gDisplayContext * 0x880)) >= 0x880) {
        __assert(&D_8011ED4C, &D_8011ED50, 0x334);
    }
}
#else
INCLUDE_ASM(const s32, "system/sprite", func_8002AE58);
#endif