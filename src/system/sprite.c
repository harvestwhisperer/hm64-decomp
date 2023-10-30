#include "common.h"

#include "system/sprite.h"

#include "system/graphic.h"
#include "system/worldGraphics.h"

#include "mainproc.h"

// bss
extern u16 bitmapCounter;

extern const char *D_8011ED4C;
extern const char *D_8011ED50;

extern Bitmap D_801F7110[MAX_BITMAPS];
extern Gfx D_80215ED0[2][0x880];
// unused
extern u16 D_8021E6E6[2][0x80][0x20];
               
// forward declarations
Gfx* func_8002A66C(Gfx*, Bitmap*, u16);             
void func_8002ACA4(Bitmap* sprite, Gfx *dl);


//INCLUDE_ASM(const s32, "system/sprite", initializeBitmaps);

void initializeBitmaps(void) {

    u16 i, j, k;

    for (i = 0; i < MAX_BITMAPS; i++) {
        D_801F7110[i].flags = 0;
        D_801F7110[i].unk_54 = 0;
        D_801F7110[i].spriteNumber = 0;
        D_801F7110[i].unk_1A = 0;
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
    
    bitmapCounter = 0;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029CC8);

void func_80029CC8(void) {

    int i = 0;
    
    bitmapCounter = 0;

    while (i < MAX_BITMAPS) {
        D_801F7110[i].flags = 0; 
        i++;
    } 
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029CF4);

// unused
u16 func_80029CF4(u8 *timg, u8 *pal, s32 width, s32 height, s32 fmt, s32 size, u32 arg6, u32 arg7, u16 flags) {
    
    u16 result;
    
    result = bitmapCounter;  
     
    if (bitmapCounter < MAX_BITMAPS) {
        D_801F7110[bitmapCounter].timg = timg;
        D_801F7110[bitmapCounter].pal = pal;
        D_801F7110[bitmapCounter].width = width;
        D_801F7110[bitmapCounter].height = height;
        D_801F7110[bitmapCounter].fmt = fmt;
        D_801F7110[bitmapCounter].pixelSize = size; 
        D_801F7110[bitmapCounter].flags = flags | 1;
        D_801F7110[bitmapCounter].unk_54 = 0;
        bitmapCounter++;
    } else { 
        result = 0xFFFF;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029DAC);

u16 func_80029DAC(void *timg, void *pal, u16 flags) {

    u16 bitmapIndex = bitmapCounter;
    
    if (bitmapCounter < MAX_BITMAPS) {
        D_801F7110[bitmapCounter].flags = flags | 1;
        D_801F7110[bitmapCounter].unk_54 = 0;
        D_801F7110[bitmapCounter].timg = timg;
        D_801F7110[bitmapCounter].pal = pal;
        bitmapCounter++;
    } else {
        bitmapIndex = 0xFFFF;
    }

    return bitmapIndex;
    
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029E2C);

bool func_80029E2C(u16 index, u16 arg1, u16 arg2) {
    
    bool result;
    u32 temp1;
    u32 temp2;

    result = 0;

    if (index < MAX_BITMAPS) {
        if (D_801F7110[index].flags & 1) {

            D_801F7110[index].unk_54 &= ~(8 | 0x10 | 0x20 | 0x40);

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
    
    if (index < MAX_BITMAPS) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].unk_54 &= ~(0x80 | 0x100);
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
    
    if (index < MAX_BITMAPS) {
        if (D_801F7110[index].flags & 1) {
            if (arg1) {
                D_801F7110[index].unk_54 |= 0x200;
            } else {
                D_801F7110[index].unk_54 &= ~0x200;
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
    
    if (index < MAX_BITMAPS) {
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
    
    if (index < MAX_BITMAPS) {
        if (D_801F7110[index].flags & 1) {
            result = 1;
            D_801F7110[index].unk_54 &= ~(0x400 | 0x1000 | 0x800);
            temp = arg1 << 10;
            D_801F7110[index].unk_54 |= temp;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A09C);

bool func_8002A09C(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index < MAX_BITMAPS) {
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
    
    if (index < MAX_BITMAPS) {
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

    
    if (index < MAX_BITMAPS) {
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
    
    if (index < MAX_BITMAPS) {
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

//INCLUDE_ASM(const s32, "system/sprite", func_8002A2E0);

bool func_8002A2E0(u16 index, u16 arg1, u16 arg2) {

    bool result = 0;
    
    if (index < MAX_BITMAPS) {
        if (D_801F7110[index].flags & 1) {
            D_801F7110[index].unk_50 = arg1;
            D_801F7110[index].unk_52 = arg2;
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A340);

u8 *func_8002A340(u16 index, u32 *start, u8 *timg, u8 *romAddr) {

    int offset = *(start + index);

    setSpriteAddresses(romAddr + offset, timg, func_8002A3A0(index, start));

    return timg;

}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A3A0);

// get length of texture segment
u32 func_8002A3A0(u16 arg0, u32 arg1[]) {

    u16 counter = arg0+1;
    
    if (arg1[counter] == arg1[arg0]) {
        
        while (arg1[counter] == arg1[arg0]) {
            counter++;
        } 
        
    }
    
    return arg1[counter] - arg1[arg0];
}

// graphics render modes enum

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

// matches but rodata issue
/*
inline Gfx* func_8002A410(Gfx* dl, u16 arg1) {

    switch (arg1) {
        case 0:
            break;
        case 1:
            gDPSetCombineMode(dl++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(dl++, G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2);
            break;
        case 2:
            *dl = D_8011ECC0[0];
            dl++;
            *dl = D_8011ECC0[1];
            dl++;
            break;
        case 3:
            *dl = D_8011ECC0[2];
            dl++;
            *dl = D_8011ECC0[3];
            dl++;
            break;
        case 4:
            *dl = D_8011ECC0[2];
            dl++;
            *dl = D_8011ECC0[4];
            dl++;
            break;
    }
    
    return dl++;
}
*/

//INCLUDE_ASM(const s32, "system/sprite", func_8002A530);

inline void func_8002A530(Vec3f* arg0, Bitmap* arg1) {
    
    s32 width;
    s32 height;
    f32 scaleX;
    f32 scaleY;
    s32 scaledWidth;
    s32 scaledHeight;

    width = arg1->width;
    scaleX = arg1->scaling.x;
    height = arg1->height;
    scaleY = arg1->scaling.y;
    
    arg0->x = arg1->unk_1C.x;
    arg0->y = arg1->unk_1C.y;
    arg0->z = arg1->unk_1C.z;

    scaledWidth = (width / 2) * scaleX;
    scaledHeight  = (height / 2) * scaleY;

    if (((arg1->unk_54 >> 3) & 3) == 1) {
        arg0->x = arg1->unk_1C.x + scaledWidth;
    }
    if (((arg1->unk_54 >> 3) & 3) == 3) {
        arg0->x = arg1->unk_1C.x - scaledWidth;
    }
    if (((arg1->unk_54 >> 5) & 3) == 1) {
        arg0->y = arg1->unk_1C.y + scaledHeight;
    }
    if (((arg1->unk_54 >> 5) & 3) == 3) {
        arg0->y = arg1->unk_1C.y - scaledHeight;
    }
}


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

// uses D_8021E6E0[2][]
// Gfx* func_8002A66C(Gfx* arg0, Bitmap* arg1, u16 arg2)
INCLUDE_ASM(const s32, "system/sprite", func_8002A66C);

//INCLUDE_ASM(const s32, "system/sprite", func_8002ACA4);

void func_8002ACA4(Bitmap* sprite, Gfx *dl) {

    Vec3f vec;

    u16 temp;

    func_8002A530(&vec, sprite);

    // maybe a ternary
    temp = func_8002929C(dl, (sprite->flags & 0x78) | 0x80);
    
    func_800292EC(temp, vec.x, vec.y, vec.z);
    func_80029330(temp, sprite->scaling.x, sprite->scaling.y, sprite->scaling.z);
    func_80029374(temp, sprite->unk_34.x, sprite->unk_34.y, sprite->unk_34.z);
   
}

#ifdef PERMUTER
void func_8002AE58(void) {

    u16 i;
    Gfx *tempDl;
    int size = 0;
    Gfx *dl = D_80215ED0[gDisplayContext];

    for (i = 0; i < 0xB0; i++) {

        if (D_801F7110[i].flags & 1) {

            func_80026E78(&D_801F7110[i], D_801F7110[i].timg, D_801F7110[i].pal);  
            
            tempDl = dl;

            dl = func_8002A66C(dl, &D_801F7110[i], size);
            
            func_8002ACA4(&D_801F7110[i], tempDl);

            size += D_801F7110[i].size;
            
            D_801F7110[i].flags &= ~1;

            dl = tempDl;
        }
    }

    if (dl - D_80215ED0[gDisplayContext] >= 0x880) {
        __assert(&D_8011ED4C, &D_8011ED50, 820);
    }
}
#else
INCLUDE_ASM(const s32, "system/sprite", func_8002AE58);
#endif