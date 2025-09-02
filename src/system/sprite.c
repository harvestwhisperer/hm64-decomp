#include "common.h"

#include "system/sprite.h"

#include "system/graphic.h"
#include "system/spriteDMAManager.h"
#include "system/sceneGraph.h"

#include "mainproc.h"

// bss
u16 bitmapCounter;

BitmapObject bitmaps[MAX_BITMAPS];
Gfx spriteDisplayList[2][0x880];
Vtx bitmapVertices[2][0x80][4];

//INCLUDE_ASM("asm/nonmatchings/system/sprite", initializeBitmaps);

void initializeBitmaps(void) {

    u16 i, j, k;

    for (i = 0; i < MAX_BITMAPS; i++) {
        bitmaps[i].flags = 0;
        bitmaps[i].renderingFlags = 0;
        bitmaps[i].spriteNumber = 0;
        bitmaps[i].vtxIndex = 0;
        bitmaps[i].shrink.x = 0;
        bitmaps[i].shrink.y = 0;
        bitmaps[i].shrink.z = 0;
        bitmaps[i].scaling.x = 1.0f;
        bitmaps[i].scaling.y = 1.0f;
        bitmaps[i].scaling.z = 1.0f;
        bitmaps[i].rotation.x = 0;
        bitmaps[i].rotation.y = 0;
        bitmaps[i].rotation.z = 0;
        bitmaps[i].rgba.r = 255.0f;
        bitmaps[i].rgba.g = 255.0f;
        bitmaps[i].rgba.b = 255.0f;
        bitmaps[i].rgba.a = 255.0f;
    }

    for (j = 0; j < 2; j++) {
        for (k = 0; k < 0x80; k++) {
            bitmapVertices[j][k][0].v.flag = 0;
            bitmapVertices[j][k][1].v.flag = 0;
            bitmapVertices[j][k][2].v.flag = 0;
            bitmapVertices[j][k][3].v.flag = 0;
        }
    }
    
    bitmapCounter = 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", resetBitmaps);

void resetBitmaps(void) {

    int i = 0;
    
    bitmapCounter = 0;

    while (i < MAX_BITMAPS) {
        bitmaps[i].flags = 0; 
        i++;
    } 

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_80029CF4);

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
        bitmaps[bitmapCounter].renderingFlags = 0;
        bitmapCounter++;
    } else { 
        result = 0xFFFF;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmap);

u16 setBitmap(u8 *timg, u16 *pal, u16 flags) {

    u16 bitmapIndex = bitmapCounter;
    
    if (bitmapCounter < MAX_BITMAPS) {
        bitmaps[bitmapCounter].flags = flags | 1;
        bitmaps[bitmapCounter].renderingFlags = 0;
        bitmaps[bitmapCounter].timg = timg;
        bitmaps[bitmapCounter].pal = pal;
        bitmapCounter++;
    } else {
        bitmapIndex = 0xFFFF;
    }

    return bitmapIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_80029E2C);

bool func_80029E2C(u16 index, u16 arg1, u16 arg2) {
    
    bool result = FALSE;

    u32 temp1;
    u32 temp2;

    if (index < MAX_BITMAPS) {

        if (bitmaps[index].flags & ACTIVE) {

            bitmaps[index].renderingFlags &= ~(8 | 0x10 | 0x20 | 0x40);

            temp1 = arg1 << 3;
            bitmaps[index].renderingFlags |= temp1;

            temp2 = arg2 << 5;
            bitmaps[index].renderingFlags |= temp2;

            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_80029EA4);

bool func_80029EA4(u16 index, u16 arg1) {
    
    bool result = FALSE;
    int temp;
    
    if (index < MAX_BITMAPS) {

        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].renderingFlags &= ~(0x80 | 0x100);
            temp = arg1 << 7;
            bitmaps[index].renderingFlags |= temp;
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_80029F14);

bool func_80029F14(u16 index, u16 arg1) {

    bool result = FALSE;
    
    if (index < MAX_BITMAPS) {
        
        if (bitmaps[index].flags & ACTIVE) {
            
            if (arg1) {
                bitmaps[index].renderingFlags |= 0x200;
            } else {
                bitmaps[index].renderingFlags &= ~0x200;
            }
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmapFlip);

bool setBitmapFlip(u16 index, bool flipHorizontal, bool flipVertical) {
    
    bool result = FALSE;
    
    if (index < MAX_BITMAPS) {

        if (bitmaps[index].flags & ACTIVE) {
    
            if (flipHorizontal) {
                bitmaps[index].renderingFlags |= FLIP_HORIZONTAL;
            }
            
            if (flipVertical) {
               bitmaps[index].renderingFlags |= FLIP_VERTICAL;
            }
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_8002A02C);

bool func_8002A02C(u16 index, u16 arg1) {

    bool result = FALSE;
    int temp;
    
    if (index < MAX_BITMAPS) {
        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].renderingFlags &= ~(0x400 | 0x1000 | 0x800);
            temp = arg1 << 10;
            bitmaps[index].renderingFlags |= temp;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmapShrink);

bool setBitmapShrink(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = FALSE;
    
    if (index < MAX_BITMAPS) {
        
        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].shrink.x = arg1;
            bitmaps[index].shrink.y = arg2;
            bitmaps[index].shrink.z = arg3;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmapScale);

bool setBitmapScale(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = FALSE;
    
    if (index < MAX_BITMAPS) {
        
        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].scaling.x = arg1;
            bitmaps[index].scaling.y = arg2;
            bitmaps[index].scaling.z = arg3;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmapRotation);

bool setBitmapRotation(u16 index, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;

    if (index < MAX_BITMAPS) {
        
        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].rotation.x = arg1;
            bitmaps[index].rotation.y = arg2;
            bitmaps[index].rotation.z = arg3;
            
            result = TRUE;

        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmapRGBA);

bool setBitmapRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = FALSE;
    
    if (index < MAX_BITMAPS) {
        
        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].rgba.r = r;
            bitmaps[index].rgba.g = g;
            bitmaps[index].rgba.b = b;
            bitmaps[index].rgba.a = a;
            
            result = TRUE;

        }
    }
    
    return result; 

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setBitmapAnchor);

bool setBitmapAnchor(u16 index, s16 anchorX, s16 anchorY) {

    bool result = FALSE;
    
    if (index < MAX_BITMAPS) {
        
        if (bitmaps[index].flags & ACTIVE) {
            
            bitmaps[index].anchorX = anchorX;
            bitmaps[index].anchorY = anchorY;
            
            result = TRUE;

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", setSpriteDMAInfo);

u8* setSpriteDMAInfo(u16 index, u32 *spritesheetIndex, u8 *timg, u8 *romAddr) {

    u32 offset = spritesheetIndex[index];

    setSpriteAssetDescriptors(romAddr + offset, timg, getTextureLength(index, spritesheetIndex));

    return timg;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", getTextureLength);

u32 getTextureLength(u16 spriteIndex, u32 spritesheetIndex[]) {
    
    u16 counter = spriteIndex + 1;
    
    // check if next sprite has same address (some sprites are reused with different palettes)
    if (spritesheetIndex[counter] == spritesheetIndex[spriteIndex]) {
        
        while (spritesheetIndex[counter] == spritesheetIndex[spriteIndex]) {
            counter++;
        } 
        
    }
    
    return spritesheetIndex[counter] - spritesheetIndex[spriteIndex];
    
}

static const Gfx D_8011ECC0 = gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA);
static const Gfx D_8011ECC8 = gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
static const Gfx D_8011ECD0 = gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
static const Gfx D_8011ECD8 = gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
static const Gfx D_8011ECE0 = gsDPSetRenderMode(G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ECC0);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ECC8);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ECD0);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ECD8);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ECE0);

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_8002A410);

inline Gfx* func_8002A410(Gfx* dl, u16 flag) {

    switch (flag) {
        case 0:
            break;
        case 1:
            gDPSetCombineMode(dl++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(dl++, G_RM_AA_ZB_OPA_SURF, G_RM_NOOP2);
            break;
        case 2:
            *dl++ = D_8011ECC0;
            *dl++ = D_8011ECC8;
            break;
        case 3:
            *dl++ = D_8011ECD0;
            *dl++ = D_8011ECD8;
            break;
        case 4:
            *dl++ = D_8011ECD0;
            *dl++ = D_8011ECE0;
            break;
    }
    
    return dl++;

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", func_8002A530);

inline void func_8002A530(Vec3f* arg0, BitmapObject* arg1) {
    
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
    
    arg0->x = arg1->shrink.x;
    arg0->y = arg1->shrink.y;
    arg0->z = arg1->shrink.z;

    scaledWidth = (width / 2) * scaleX;
    scaledHeight  = (height / 2) * scaleY;

    if (((arg1->renderingFlags >> 3) & (1 | 2)) == 1) {
        arg0->x = arg1->shrink.x + scaledWidth;
    }
    if (((arg1->renderingFlags >> 3) & (1 | 2)) == 3) {
        arg0->x = arg1->shrink.x - scaledWidth;
    }
    if (((arg1->renderingFlags >> 5) & (1 | 2)) == 1) {
        arg0->y = arg1->shrink.y + scaledHeight;
    }
    if (((arg1->renderingFlags >> 5) & (1 | 2)) == 3) {
        arg0->y = arg1->shrink.y - scaledHeight;
    }

}

static const Gfx D_8011ED00 = gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED00);

static const Gfx D_8011ED08 = gsDPSetTextureFilter(G_TF_BILERP);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED08);

static const Gfx D_8011ED10 = gsDPSetTextureFilter(G_TF_POINT);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED10);

static const Gfx D_8011ED18 = gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED18);

static const Gfx D_8011ED20 = gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED20);

static const Gfx D_8011ED28 = gsDPPipeSync();

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED28);

static const Gfx D_8011ED30 = gsSPEndDisplayList();

//INCLUDE_RODATA("asm/nonmatchings/systemsprite", D_8011ED30);

//INCLUDE_ASM("asm/nonmatchings/system/sprite", generateBitmapDisplayList);

Gfx* generateBitmapDisplayList(Gfx* dl, BitmapObject* bitmap, u16 spriteNumber) {

    u16 vtxIndex;

    u32 textureDimensions;
    u16 textureOffset;
    u16 textureHeight;
    u16 textureWidth; 
    u16 textureSize;

    u16 remainingSize;
    
    Gfx tempDl[2];
    
    bitmap->spriteNumber = spriteNumber;

    dl = func_8002A410(dl, (bitmap->renderingFlags >> 10) & 7);

    *dl++ = D_8011ED00;

    if (bitmap->flags & USE_BILINEAR_FILTERING) {
        // gsDPSetTextureFilter(G_TF_BILERP)
        *dl++ = D_8011ED08;
    } else {
        // gsDPSetTextureFilter(G_TF_POINT)
        *dl++ = D_8011ED10;;
    }
    
    switch (bitmap->pixelSize) {
        
        case G_IM_SIZ_4b:
            textureHeight = 4096 / bitmap->width;
            textureWidth = bitmap->width / 2;
            break;
            
        case G_IM_SIZ_8b:
            textureHeight = 2048 / bitmap->width;
            textureWidth = bitmap->width;
            break;
        
        case G_IM_SIZ_16b:
            textureHeight = 2048 / bitmap->width;
            textureWidth = bitmap->width * 2;
            break;
            
        case G_IM_SIZ_32b:
            textureHeight = 2048 / bitmap->width;
            textureWidth = bitmap->width * 4;
            break;

    }
    
    remainingSize = bitmap->height;

    textureOffset = 0;
    textureDimensions = 0;
    vtxIndex = 0;
    
    do {
    
        textureSize = remainingSize;
    
        if (textureSize > textureHeight) {
          textureSize = textureHeight;
        }
        
        setupBitmapVertices((&bitmapVertices[gGraphicsBufferIndex][bitmap->spriteNumber + vtxIndex]), 
            bitmap->width, 
            bitmap->height, 
            textureSize, 
            textureOffset, 
            bitmap->renderingFlags & FLIP_HORIZONTAL, 
            bitmap->renderingFlags & FLIP_VERTICAL, 
            bitmap->anchorX,
            bitmap->anchorY, 
            bitmap->renderingFlags, 
            bitmap->rgba.r, 
            bitmap->rgba.g, 
            bitmap->rgba.b, 
            bitmap->rgba.a
        );
        
        dl = loadBitmapTexture(dl, bitmap, textureDimensions, textureSize);
    
        // FIXME: might be a wrapper around gSPVertex
        gSPVertex(&tempDl[1], &bitmapVertices[gGraphicsBufferIndex][bitmap->spriteNumber + vtxIndex][0], 4, 0);
    
        *tempDl = *(tempDl + 1);
        *dl++ = *tempDl;
        
        if (bitmap->renderingFlags & 0x200) {
          *(dl++) = D_8011ED18;
        }
        else {
          *(dl++) = D_8011ED20;
        }
            
        remainingSize -= textureSize;
        textureOffset += textureSize;
        vtxIndex++;
    
        textureDimensions += textureSize * textureWidth;
        *(dl++) = D_8011ED28;
        
    } while (remainingSize);
    
    *(dl++) = D_8011ED30;
    bitmap->vtxIndex = vtxIndex;
    
    return dl;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", processBitmapSceneNode);

static void processBitmapSceneNode(BitmapObject* sprite, Gfx *dl) {

    Vec3f vec;

    u16 spriteIndex; 

    // adjust scaling if needed
    func_8002A530(&vec, sprite);

    spriteIndex = addSceneNode(dl, (sprite->flags & (0x8 | 0x10 | 0x20 | 0x40)) | 0x80);

    addSceneNodePosition(spriteIndex, vec.x, vec.y, vec.z);
    addSceneNodeScaling(spriteIndex, sprite->scaling.x, sprite->scaling.y, sprite->scaling.z);
    addSceneNodeRotation(spriteIndex, sprite->rotation.x, sprite->rotation.y, sprite->rotation.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/sprite", updateBitmaps);

void updateBitmaps(void) {

    u16 i;

    Gfx *dlStartPosition;
    Gfx *dl = spriteDisplayList[gGraphicsBufferIndex];

    u16 spriteNumber = 0;
    
    for (i = 0; i < MAX_BITMAPS; i++) {
        
        if (bitmaps[i].flags & ACTIVE) {
    
            setBitmapFormat(&bitmaps[i], bitmaps[i].timg, bitmaps[i].pal);

            dlStartPosition = dl;
            dl = generateBitmapDisplayList(dl, &bitmaps[i], spriteNumber);

            processBitmapSceneNode(&bitmaps[i], dlStartPosition); 
            
            spriteNumber += bitmaps[i].vtxIndex;
            bitmaps[i].flags &= ~1;
            
       }
   }
    
    if (dl - spriteDisplayList[gGraphicsBufferIndex] >= 0x880) {
        __assert("EX", "s:/system/sprite.c", 820);
    }

}
