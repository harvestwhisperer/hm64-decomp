#include "common.h"

#include "system/sprite.h"

#include "system/graphic.h"
#include "system/worldGraphics.h"

#include "mainproc.h"

// bss
extern u16 bitmapCounter;

// extern const char D_8011ED4C[];
// extern const char D_8011ED50[];

extern Bitmap bitmaps[MAX_BITMAPS];
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
        bitmaps[i].flags = 0;
        bitmaps[i].unk_54 = 0;
        bitmaps[i].spriteNumber = 0;
        bitmaps[i].vtxIndex = 0;
        bitmaps[i].unk_1C.x = 0;
        bitmaps[i].unk_1C.y = 0;
        bitmaps[i].unk_1C.z = 0;
        bitmaps[i].scaling.x = 1.0f;
        bitmaps[i].scaling.y = 1.0f;
        bitmaps[i].scaling.z = 1.0f;
        bitmaps[i].unk_34.x = 0;
        bitmaps[i].unk_34.y = 0;
        bitmaps[i].unk_34.z = 0;
        bitmaps[i].rgba.r = 255.0f;
        bitmaps[i].rgba.g = 255.0f;
        bitmaps[i].rgba.b = 255.0f;
        bitmaps[i].rgba.a = 255.0f;
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
        bitmaps[i].flags = 0; 
        i++;
    } 
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029CF4);

// unused
u16 func_80029CF4(u8 *timg, u8 *pal, s32 width, s32 height, s32 fmt, s32 size, u32 arg6, u32 arg7, u16 flags) {
    
    u16 result;
    
    result = bitmapCounter;  
     
    if (bitmapCounter < MAX_BITMAPS) {
        bitmaps[bitmapCounter].timg = timg;
        bitmaps[bitmapCounter].pal = pal;
        bitmaps[bitmapCounter].width = width;
        bitmaps[bitmapCounter].height = height;
        bitmaps[bitmapCounter].fmt = fmt;
        bitmaps[bitmapCounter].pixelSize = size; 
        bitmaps[bitmapCounter].flags = flags | 1;
        bitmaps[bitmapCounter].unk_54 = 0;
        bitmapCounter++;
    } else { 
        result = 0xFFFF;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029DAC);

u16 func_80029DAC(u8 *timg, u16 *pal, u16 flags) {

    u16 bitmapIndex = bitmapCounter;
    
    if (bitmapCounter < MAX_BITMAPS) {
        bitmaps[bitmapCounter].flags = flags | 1;
        bitmaps[bitmapCounter].unk_54 = 0;
        bitmaps[bitmapCounter].timg = timg;
        bitmaps[bitmapCounter].pal = pal;
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
        if (bitmaps[index].flags & 1) {

            bitmaps[index].unk_54 &= ~(8 | 0x10 | 0x20 | 0x40);

            temp1 = arg1 << 3;
            bitmaps[index].unk_54 |= temp1;

            temp2 = arg2 << 5;
            bitmaps[index].unk_54 |= temp2;

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
        if (bitmaps[index].flags & 1) {
            result = 1;
            bitmaps[index].unk_54 &= ~(0x80 | 0x100);
            temp = arg1 << 7;
            bitmaps[index].unk_54 |= temp;
        }
    }
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_80029F14);

bool func_80029F14(u16 index, u16 arg1) {

    bool result;
    
    result = 0;
    
    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & 1) {
            if (arg1) {
                bitmaps[index].unk_54 |= 0x200;
            } else {
                bitmaps[index].unk_54 &= ~0x200;
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
        if (bitmaps[index].flags & 1) {
    
            if (arg1) {
                bitmaps[index].unk_54 |= 1;
            }
            
            result = 1;
            
            if (arg2) {
               bitmaps[index].unk_54 |= 2;
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
        if (bitmaps[index].flags & 1) {
            result = 1;
            bitmaps[index].unk_54 &= ~(0x400 | 0x1000 | 0x800);
            temp = arg1 << 10;
            bitmaps[index].unk_54 |= temp;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A09C);

bool func_8002A09C(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & 1) {
            result = 1;
            bitmaps[index].unk_1C.x = arg1;
            bitmaps[index].unk_1C.y = arg2;
            bitmaps[index].unk_1C.z = arg3;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A120);

bool func_8002A120(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & 1) {
            result = 1;
            bitmaps[index].scaling.x = arg1;
            bitmaps[index].scaling.y = arg2;
            bitmaps[index].scaling.z = arg3;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A1A4);

bool func_8002A1A4(u16 index, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;

    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & 1) {
            result = 1;
            bitmaps[index].unk_34.x = arg1;
            bitmaps[index].unk_34.y = arg2;
            bitmaps[index].unk_34.z = arg3;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A228);

bool func_8002A228(u16 index, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = 0;
    
    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & 1) {
            result = 1;
            bitmaps[index].rgba.r = r;
            bitmaps[index].rgba.g = g;
            bitmaps[index].rgba.b = b;
            bitmaps[index].rgba.a = a;
        }
    }
    
    return result; 
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A2E0);

bool func_8002A2E0(u16 index, u16 arg1, u16 arg2) {

    bool result = 0;
    
    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & 1) {
            bitmaps[index].unk_50 = arg1;
            bitmaps[index].unk_52 = arg2;
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A340);

u8 *func_8002A340(u16 index, u32 *spritesheetIndex, u8 *timg, u8 *romAddr) {

    u32 offset = spritesheetIndex[index];

    // func_8002A3A0 = get texture length
    // timg = vaddr
    setSpriteAddresses(romAddr + offset, timg, func_8002A3A0(index, spritesheetIndex));

    return timg;

}

//INCLUDE_ASM(const s32, "system/sprite", func_8002A3A0);

// check if next sprite has same address (some sprites are reused with different palettes)
// get length of texture segment
u32 func_8002A3A0(u16 spriteIndex, u32 spritesheetIndex[]) {

    u16 counter = spriteIndex+1;
    
    if (spritesheetIndex[counter] == spritesheetIndex[spriteIndex]) {
        
        while (spritesheetIndex[counter] == spritesheetIndex[spriteIndex]) {
            counter++;
        } 
        
    }
    
    return spritesheetIndex[counter] - spritesheetIndex[spriteIndex];
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

// bitmap to vertices
#ifdef PERMUTER
Gfx* func_8002A66C(Gfx* dl, Bitmap* sprite, u16 arg2) {

    u16 vtxIndex;
    
    u32 textureDimensions;
    u16 textureOffset;
    volatile u16 textureWidth;
    u16 textureHeight[1];
    u16 textureSize;
    u16 remainingSize;

    Gfx tempDl[2];

    sprite->spriteNumber = arg2;

    dl = func_8002A410(dl, (sprite->unk_54 >> 10) & 7);

    // gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON)
    *dl = D_8011ED00[0];
    dl++;

    if (sprite->flags & 4) {
        // gsDPSetTextureFilter(G_TF_BILERP)
        *dl = D_8011ED00[1];
        dl++;
    } else {
        // gsDPSetTextureFilter(G_TF_POINT)
        *dl = D_8011ED00[2];
        dl++;
    }

    switch (sprite->pixelSize) {
        case G_IM_SIZ_4b:
            textureWidth = ((u32)sprite->width + ((u32)sprite->width >> 0x1F)) >> 1;
            textureHeight[0] = 4096 / sprite->width;
            break;
        case G_IM_SIZ_8b:
            textureWidth = sprite->width;
            textureHeight[0] = 2048 / textureWidth;
            break;
        default:
        case G_IM_SIZ_16b:
            textureWidth = sprite->width * 2;
            textureHeight[0] = 2048 / sprite->width;
            break;
        case G_IM_SIZ_32b:
            textureWidth = sprite->width * 4;
            textureHeight[0] = 2048 / sprite->width;
            break;
    }

    remainingSize = sprite->height;

    textureOffset = 0;
    textureDimensions = 0;
    vtxIndex = 0;
    
    do {
        
        textureSize = remainingSize;

        if (textureSize > textureHeight[0]) {
            textureSize = textureHeight[0];
        }
        
        func_800276AC((Vtx*)&D_8021E6E0[gDisplayContextIndex][sprite->spriteNumber+vtxIndex], 
            sprite->width, 
            sprite->height, 
            textureSize, 
            textureOffset, 
            sprite->unk_54&1,
            sprite->unk_54&2, 
            sprite->unk_50, 
            sprite->unk_52,
            sprite->unk_54, 
            sprite->rgba.r, 
            sprite->rgba.g, 
            sprite->rgba.b,
            sprite->rgba.a);

        // load texture tile
        dl = func_80026F88(dl, sprite, textureDimensions, textureSize);

        gSPVertex(&tempDl[0], (Vtx*)&D_8021E6E0[gDisplayContextIndex][sprite->spriteNumber + vtxIndex], 4, 0);

        *(tempDl-1) = *(tempDl);
        *dl = *(tempDl-1);
        dl++;
        
       if (sprite->unk_54 & 0x200) {
            // gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)
            *dl = D_8011ED00[3];
            dl++;
        } else {
            // gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0)
            *dl = D_8011ED00[4];
            dl++;
        }
        
        remainingSize -= textureSize;
        textureOffset += textureSize;
        vtxIndex++;
        
        textureDimensions += textureSize * *(&textureWidth+11);
        
        // gsDPPipeSync()
        *dl = D_8011ED00[5];        
        dl++;
        
    } while (remainingSize);

    // gsSPEndDisplayList()
    *dl = D_8011ED00[6];

    sprite->vtxIndex = vtxIndex;

    dl++;

    return dl;
    
}
#else
INCLUDE_ASM(const s32, "system/sprite", func_8002A66C);
#endif

//INCLUDE_ASM(const s32, "system/sprite", func_8002ACA4);

// update world graphics from sprite
void func_8002ACA4(Bitmap* sprite, Gfx *dl) {

    Vec3f vec;

    u16 spriteIndex; 

    // adjust scaling if needed
    func_8002A530(&vec, sprite);

    // update worldGraphics struct
    spriteIndex = func_8002929C(dl, (sprite->flags & 0x78) | 0x80);
    
    func_800292EC(spriteIndex, vec.x, vec.y, vec.z);
    func_80029330(spriteIndex, sprite->scaling.x, sprite->scaling.y, sprite->scaling.z);
    func_80029374(spriteIndex, sprite->unk_34.x, sprite->unk_34.y, sprite->unk_34.z);

}

//INCLUDE_ASM(const s32, "system/sprite", func_8002AE58);

static const char D_8011ED4C[] = "EX";
static const char D_8011ED50[] = "s:/system/sprite.c";

// main loop function
void func_8002AE58(void) {

    u16 i;

    Gfx *tempDl;

    Gfx *dl = D_80215ED0[gDisplayContextIndex];

    u16 spriteNumber = 0;
    
    for (i = 0; i < MAX_BITMAPS; i++) {
        
        if (bitmaps[i].flags & 1) {
    
            setBitmapFormat(&bitmaps[i], bitmaps[i].timg, bitmaps[i].pal);

            tempDl = dl;
            dl = func_8002A66C(tempDl, &bitmaps[i], spriteNumber);

            func_8002ACA4(&bitmaps[i], tempDl); 
            
            spriteNumber += bitmaps[i].vtxIndex;
            bitmaps[i].flags &= ~1;
            
       }
   }
    
    if (dl - D_80215ED0[gDisplayContextIndex] >= 0x880) {
        // FIXME: get string literals working
        __assert(D_8011ED4C, D_8011ED50, 820);
        //__assert("EX", "s:/system/sprite.c", 820);
    }
}
