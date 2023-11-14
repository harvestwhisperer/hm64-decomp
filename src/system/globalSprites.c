#include "common.h"

#include "system/sprite.h"
#include "system/mathUtils.h"

// forward declarations
u8 func_8002B8E0(u16, u8, void*);
bool func_8002C6F8(u16, u16);
void func_8002CC84(SpriteAnimation*, u32*);             
u8* func_8002CD34(u16 arg0, void* arg1);
u16* func_8002CD4C(u16, u16*);   

// bss
extern Sprite globalSprites[MAX_ACTIVE_SPRITES];

//INCLUDE_ASM(const s32, "system/globalSprites", initializeGlobalSprites);

void initializeGlobalSprites(void) {

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

        globalSprites[i].unk_44.x = 0;
        globalSprites[i].unk_44.y = 0;
        globalSprites[i].unk_44.z = 0;
        
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

// wrapper for nuPiReadRom
//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B138);

bool func_8002B138(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetIndexStart, u32 romAssetIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, void* texture1Vaddr, void* texture2Vaddr, void* paletteVaddr, void* animationVaddr, 
    void* spriteToPaletteVaddr, void* spritesheetIndexVaddr, u8 assetType, u8 argE) {

    u32 assetIndex[8];
    bool result = 0;
    
    u32 offset1;
    u32 offset2;
    u32 offset3;
    u32 offset4;
    u32 offset5;

    if (index < MAX_ACTIVE_SPRITES) {
        
        if (!(globalSprites[index].flags2 & 1)) { 
            
            nuPiReadRom(romAssetIndexStart, assetIndex, romAssetIndexEnd - romAssetIndexStart);

            // spritesheet
            offset1 = assetIndex[0];
            // palette
            offset2 = assetIndex[1];
            // animation
            offset3 = assetIndex[2];
            // sprite to palette
            offset4 = assetIndex[3];
            // end
            offset5 = assetIndex[4];
        
            // has separate spritesheet index
            if (assetType & 1) {

                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                nuPiReadRom(romSpritesheetIndexStart, spritesheetIndexVaddr, romSpritesheetIndexEnd - romSpritesheetIndexStart);

                func_8002B50C(index, animationVaddr, spritesheetIndexVaddr, paletteVaddr, spriteToPaletteVaddr, romTextureStart, texture1Vaddr, texture2Vaddr);

            // spritesheet index in spritesheet
            } else {

                nuPiReadRom(romTextureStart + offset1, texture1Vaddr, offset2 - offset1);
                nuPiReadRom(romTextureStart + offset2, paletteVaddr, offset3 - offset2);
                nuPiReadRom(romTextureStart + offset3, animationVaddr, offset4 - offset3);
                nuPiReadRom(romTextureStart + offset4, spriteToPaletteVaddr, offset5 - offset4);
                
                func_8002B36C(index, animationVaddr, texture1Vaddr, paletteVaddr, spriteToPaletteVaddr);

            }

            if (!argE) {
                globalSprites[index].flags2 |= 0x200;
            }

            globalSprites[index].flags2 |= 0x40;

            result = 1;
        }
    
    }


    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B36C);

bool func_8002B36C(u16 index, u32* unknownAssetIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (!(globalSprites[index].flags2 & 1)) {

            globalSprites[index].unknownAssetIndexPtr = unknownAssetIndexPtr;
            globalSprites[index].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[index].paletteIndexPtr = paletteIndexPtr;
            globalSprites[index].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[index].texturePtr = (void*)0;
            globalSprites[index].texture2Ptr = (void*)0;
            globalSprites[index].romTexturePtr = (void*)0;

            globalSprites[index].flags2 = 1;
            globalSprites[index].flags1 = 0;

            globalSprites[index].unk_94 = 0;
            globalSprites[index].unk_92 = 0;

            func_8002BD0C(index, 0.0f, 0.0f, 0.0f);
            func_8002BD90(index, 1.0f, 1.0f, 1.0f);
            func_8002BE14(index, 0.0f, 0.0f, 0.0f);
            func_8002C85C(index, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C680(index, 2, 2);
            func_8002C6F8(index, 2);
            func_8002C7EC(index, 3);
            
            result = 1;
            
        }
    }
 
    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B50C);

bool func_8002B50C(u16 index, u32* unknownAssetIndexPtr, u32* spritesheetIndexPtr, u32* paletteIndexPtr, u8* spriteToPaletteMappingPtr, u32 romTexturePtr, u8* texturePtr, u8* texture2Ptr) {
    
    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (!(globalSprites[index].flags2 & 1)) {

            // animation index
            globalSprites[index].unknownAssetIndexPtr = unknownAssetIndexPtr;
            globalSprites[index].spritesheetIndexPtr = spritesheetIndexPtr;
            globalSprites[index].paletteIndexPtr = paletteIndexPtr;
            globalSprites[index].spriteToPaletteMappingPtr = spriteToPaletteMappingPtr;
            globalSprites[index].texturePtr = texturePtr;
            globalSprites[index].texture2Ptr = texture2Ptr;
            globalSprites[index].romTexturePtr = romTexturePtr;

            globalSprites[index].flags2 = 5;
            globalSprites[index].flags1 = 0;

            globalSprites[index].unk_94 = 0;
            globalSprites[index].unk_92 = 0;

            func_8002BD0C(index, 0, 0, 0);
            func_8002BD90(index, 1.0f, 1.0f, 1.0f);
            func_8002BE14(index, 0, 0, 0);
            func_8002C85C(index, 0xFF, 0xFF, 0xFF, 0xFF);
            func_8002C680(index, 2, 2);
            func_8002C6F8(index, 2);
            func_8002C7EC(index, 3);
            
            result = 1;
            
        }
    }
 
    return result;
    
}


//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B6B8);

bool func_8002B6B8(u16 index) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].flags2 &= ~(1 | 2);
            result = 1;
        }        
    }

    return result; 
}

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B710);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002B7A0);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B80C);

bool func_8002B80C(u16 spriteIndex, u16 offset, u8 arg2) {

    u32 *ptr;
    bool result = 0;
    
    if (spriteIndex < MAX_ACTIVE_SPRITES) {

        if (globalSprites[spriteIndex].flags2 & 1) {
            
            if (globalSprites[spriteIndex].unknownAssetIndexPtr[offset] != globalSprites[spriteIndex].unknownAssetIndexPtr[offset+1]) {

                // func_8002CD34(offset, globalSprites[spriteIndex].unknownAssetIndexPtr) = get address + offset for unknown asset ptr based on sprite index
                result = func_8002B8E0(spriteIndex, arg2, func_8002CD34(offset, globalSprites[spriteIndex].unknownAssetIndexPtr));
                
                globalSprites[spriteIndex].animationCounter1 = 0xFF;
                globalSprites[spriteIndex].animationCounter2 = 0xFF;
           
            }
        }
    }

    return result;   
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002B8E0);

bool func_8002B8E0(u16 index, u8 arg1, void* arg2) {

    bool result = 0;
    u16 *temp;
    
    if (index < MAX_ACTIVE_SPRITES && globalSprites[index].flags2 & 1 && !(globalSprites[index].flags2 & 2)) {

        globalSprites[index].unknownAssetPtr = arg2;

        globalSprites[index].unk_91 = 0;
        globalSprites[index].unk_92 = 0;

        globalSprites[index].flags2 &= ~0x40;
        globalSprites[index].flags2 |= 2;

        switch (arg1) {
            case 0xFF:
                globalSprites[index].unk_90 = 0;
                globalSprites[index].flags2 &= ~0x10;
                globalSprites[index].flags2 |= 8;
                break;
            case 0xFE:
                globalSprites[index].unk_90 = 0;
                globalSprites[index].flags2 |= 8 | 0x10;
                break;
            case 0xFD:
                globalSprites[index].unk_90 = 0;
                globalSprites[index].flags2 |= 8 | 0x800;
                break;
            default:
                globalSprites[index].unk_90 = arg1;
                globalSprites[index].flags2 &= ~(8 | 0x10);
                break;
        }

        // update animation from byteswapped table
        func_8002CC84(&globalSprites[index].animation, globalSprites[index].unknownAssetPtr);

        temp = func_8002CD4C(globalSprites[index].unk_90, globalSprites[index].unknownAssetPtr+8);
        
        globalSprites[index].unknownAsset3Ptr = temp;
        globalSprites[index].unknownAsset4Ptr = temp + 2;

        result = 1;
        
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BAD8);

bool func_8002BAD8(u16 index) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].flags2 &= ~2;
            result = 1;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BB30);

bool func_8002BB30(u16 index) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].flags2 &= ~0x20;
            result = 1;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BB88);

bool func_8002BB88(u16 index) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].flags2 |= 0x20;
            result = 1;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BBE0);

bool func_8002BBE0(u16 index, u8 arg1, u8 arg2) {

    bool result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {

            if (arg1) {
                globalSprites[index].flags1 |= 1;
            } else {
                globalSprites[index].flags1 &= ~1;
            }

            if (arg2) {
                globalSprites[index].flags1 |= 2;
            } else {
                globalSprites[index].flags1 &= ~2;
            }
            
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BCC8);

bool func_8002BCC8(u16 index) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {
            result = globalSprites[index].flags2 & 0x40;;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BD0C);

bool func_8002BD0C(u16 index, f32 x, f32 y, f32 z) {

    bool result = 0;

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

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BD90);

bool func_8002BD90(u16 index, f32 x, f32 y, f32 z) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].scale.x = x;
            globalSprites[index].scale.y = y;
            globalSprites[index].scale.z = z;
            
            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BE14);

bool func_8002BE14(u16 index, f32 x, f32 y, f32 z) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].unk_44.x = x;
            globalSprites[index].unk_44.y = y;
            globalSprites[index].unk_44.z = z;

            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002BE98);

bool func_8002BE98(u16 index, f32 x, f32 y, f32 z) {

    bool result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].shrink.x += x;
            globalSprites[index].shrink.y += y;
            globalSprites[index].shrink.z += z;
                
            result = 1;
            
        }   
    }
    
    return result;
}

// rgba
INCLUDE_ASM(const s32, "system/globalSprites", func_8002BF4C);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C000);

bool func_8002C000(u16 index, f32 x, f32 y, f32 z) {

    bool result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].unk_44.x += x;
            globalSprites[index].unk_44.y += y;
            globalSprites[index].unk_44.z += z;
                
            result = 1;
            
        }   
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C0B4);

bool func_8002C0B4(u16 index, s8 r, s8 g, s8 b, s8 a) {

    bool result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].rgbaCurrent.r += r;
            globalSprites[index].rgbaCurrent.g += g;
            globalSprites[index].rgbaCurrent.b += b;
            globalSprites[index].rgbaCurrent.a += a;
            
            result = 1;
            
        }   
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C1C0);

// update sprite rgba
bool func_8002C1C0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5) {

    bool result;
    
    s16 temp = getAbsoluteValue(arg5);
    f32 tempFloat;
    
    result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & 1) {
 
            globalSprites[index].rgbaDefault.r = (globalSprites[index].rgba.r * r) / 255.0f;
            globalSprites[index].rgbaDefault.g = (globalSprites[index].rgba.g * g) / 255.0f;
            globalSprites[index].rgbaDefault.b = (globalSprites[index].rgba.b * b) / 255.0f;
            globalSprites[index].rgbaDefault.a = (globalSprites[index].rgba.a * a) / 255.0f;
            
            globalSprites[index].flags2 &= ~0x400;

            if (globalSprites[index].rgbaDefault.r < globalSprites[index].rgbaCurrent.r) {
                tempFloat = globalSprites[index].rgbaCurrent.r - globalSprites[index].rgbaDefault.r;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.r - globalSprites[index].rgbaCurrent.r;
            }

            globalSprites[index].normalized.r = (tempFloat * temp) / globalSprites[index].rgba.r;

            if (globalSprites[index].rgbaDefault.g < globalSprites[index].rgbaCurrent.g) {
                tempFloat = globalSprites[index].rgbaCurrent.g - globalSprites[index].rgbaDefault.g;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.g - globalSprites[index].rgbaCurrent.g;
            }

            globalSprites[index].normalized.g = (tempFloat * temp) / globalSprites[index].rgba.g;

            if (globalSprites[index].rgbaDefault.b < globalSprites[index].rgbaCurrent.b) {
                tempFloat = globalSprites[index].rgbaCurrent.b - globalSprites[index].rgbaDefault.b;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.b - globalSprites[index].rgbaCurrent.b;
            }

            globalSprites[index].normalized.b = (tempFloat * temp) / globalSprites[index].rgba.b;

            if (globalSprites[index].rgbaDefault.a < globalSprites[index].rgbaCurrent.a) {
                tempFloat = globalSprites[index].rgbaCurrent.a - globalSprites[index].rgbaDefault.a;
            } else {
                tempFloat = globalSprites[index].rgbaDefault.a - globalSprites[index].rgbaCurrent.a;
            }

            globalSprites[index].normalized.a = (tempFloat * temp) / globalSprites[index].rgba.a;
            
            result = 1;
            
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C52C);

// update alpha
bool func_8002C52C(u16 index, u8 arg1, s16 arg2) {

    bool result;

    f32 tempF;
    s16 temp = getAbsoluteValue(arg2);

    result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].rgbaDefault.a = (globalSprites[index].rgba.a * arg1) / 255.0f;

            globalSprites[index].flags2 &= ~0x400;

            if (globalSprites[index].rgbaDefault.a < globalSprites[index].rgbaCurrent.a) {
                tempF = globalSprites[index].rgbaCurrent.a - globalSprites[index].rgbaDefault.a;
            } else {
                tempF = globalSprites[index].rgbaDefault.a - globalSprites[index].rgbaCurrent.a;
            }

            globalSprites[index].normalized.a = (tempF * temp) / globalSprites[index].rgba.a;
            
            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C680);

bool func_8002C680(u16 index, u16 arg1, u16 arg2) {

    bool result = 0;
    int temp;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) { 
            
            globalSprites[index].flags1 &= ~( 8 | 0x10 | 0x20 | 0x40);
            
            globalSprites[index].flags1 |= arg1 * 8;
            
            temp = arg2 << 5;
            globalSprites[index].flags1 |= temp;
            
            result = 1;
            
        } 
    }
    
    return result;

}

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C6F8);

INCLUDE_ASM(const s32, "system/globalSprites", func_8002C768);

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C7EC);

u8 func_8002C7EC(u16 index, u16 arg1) {
    
    int temp;
    u8 result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        
        if (globalSprites[index].flags2 & 1) {
            
            globalSprites[index].flags1 &= ~(0x400 | 0x800 | 0x1000);
            temp = arg1 << 10;
            globalSprites[index].flags1 |= temp;
            
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002C85C);

bool func_8002C85C(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = 0;

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


//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CAA8);

bool func_8002CAA8(u16 index, u8 a) {
    
    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {

        if (globalSprites[index].flags2 & 1) {

            globalSprites[index].rgba.a = a;
            globalSprites[index].rgbaCurrent.a = a;
            globalSprites[index].rgbaDefault.a = a;
            
            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CB24);

u8 func_8002CB24(u16 index, u8 flag) {

    u8 result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            if (flag == TRUE) {
                globalSprites[index].flags2 |= 0x80;
            } else {
                globalSprites[index].flags2 &= ~0x80;
            }
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CB88);

bool func_8002CB88(u16 index, u16 arg1) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            globalSprites[index].unk_94 = arg1;
            globalSprites[index].flags2 |= 0x100;
            result = 1;
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CBF8);

bool func_8002CBF8(u16 index) {

    bool result;

    result = 0;
    
    if (index < MAX_ACTIVE_SPRITES) {
        if (globalSprites[index].flags2 & 1) {
            result = (globalSprites[index].flags2 >> 10) & 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CC44);

bool func_8002CC44(u16 index) {

    bool result = 0;

    if (index < MAX_ACTIVE_SPRITES) {
        // flag 0x40
        result = (globalSprites[index].flags2 >> 6) & 1;
    }

    return result;
    
}

// arg0 = ptr to animation index on sprite struct
// arg1 = ptr to unknown asset (byteswapped u16)
// 16 bit swap
#ifdef PERMUTER
void func_8002CC84(SpriteAnimation* arg0, u16* arg1) {

    u16 arr[2];
    
    u16 temp;
    u16 temp2;
    u16 temp3;

    temp = (arg1[2] << 8);
    temp2 = (arg1[2] >> 8);
    
    *arg0 = temp | temp2;

}
#else
INCLUDE_ASM(const s32, "system/globalSprites", func_8002CC84);
#endif

#ifdef PERMUTER
inline void func_8002CCA8(SpriteAnimation* arg0, u16* arg1) {

    u32 arr[2];

    u32 temp;
    u32 temp1;
    u32 temp2;
    u32 temp3;

    temp = *arg1;
    temp1 = (*arg1 << 8);
    temp >>= 8;
    temp3 |= temp;
    temp3 |= temp1;
    
    arg0->animation = temp3;

    temp1 = *(arg1+1);

    arg0->unk_2 = temp1 >> 8;
    arg0->unk_3 = temp1;
    
}
#else
INCLUDE_ASM(const s32, "system/globalSprites", func_8002CCA8);
#endif

#ifdef PERMUTER
void func_8002CCDC(UnknownAnimation* arg0, u16* arg1) {

    u32 temp;
    u32 temp2;
    u32 temp3;
    UnknownAnimation struc[2];

    temp = arg1[0];
    
    arg0->unk_2 = 0;
    

    temp2 = (temp & 0xFF) << 8;
    temp3 = temp >> 8;
    
    temp = temp2 | temp3;
    
    arg0->unk_0 = temp;

    
    temp = arg1[2];

    temp2 = (temp & 0xFF) << 8;
    temp3 = temp >> 8;
    
    temp = temp2 | temp3;
    
    arg0->unk_4 = temp;
    

    temp = arg1[3];

    temp2 = temp << 8;
    temp3 = temp >> 8;
    
    temp = temp2 | temp3;
    
    arg0->unk_6 = temp;
    
}
#else
INCLUDE_ASM(const s32, "system/globalSprites", func_8002CCDC);
#endif

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CD34);

// unknown sprite asset index lookup
u8* func_8002CD34(u16 arg0, void* arg1) {
 
    u32 *arr = (u32*)arg1;
    
    return (u8*)(arg1 + arr[arg0]);
    
}

// alternate
/*
u8* func_8002CD34(u16 arg0, void* arg1) {
    return (u8*)(arg1 + *(u32*)(arg1 + arg0*4));
}
*/

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CD4C);

// no clue; probably uses static inlines
u16* func_8002CD4C(u16 arg0, u16* arg1) {
    
    u16 i;

    u32 temp;
    u32 temp2;
    u32 temp3;
    u8 temp4;
    
    UnknownAnimation unk;
    
    i = 0;
    
    if (arg0) {
        
        do {
            
            temp2 = arg1[0] << 8;
            temp3 = arg1[0] >> 8;
            
            temp = temp2 | temp3;
            
            unk.unk_0 = temp;


            // ?

            temp2 = arg1[1];
            temp = temp2;
            temp2 = temp;
            
            temp2 = temp >> 8;
            temp4 = temp >> 8;
            
            unk.unk_2 = temp4;
            unk.unk_3 = temp;
              
            arg1 += 2;
            arg1 += unk.unk_0 * 4;
            
            i++;
            
        } while (i < arg0);
    }
    
    return arg1;
}

//INCLUDE_ASM(const s32, "system/globalSprites", func_8002CDB4);

// unused or inline
Gfx* func_8002CDB4(u16 arg0, Gfx* arg1) {
    
    u32 i;
    
    for (i = 0; (u16)i < arg0; i++) {
        arg1++;
    }

    return arg1;
}

// alternate
/*
u32 func_8002CDB4(u16 arg0, u32 arg1) {
    
    u16 i = 0;

    u32 padding[2];
    
    if (arg0) {
        do {
            arg1 += 8;
            i++;
        } while (i < arg0);
    }
    
    return arg1;
}
*/

// alternate
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

// loop through all sprites and update (rgba, animation)
INCLUDE_ASM(const s32, "system/globalSprites", func_8002D3D4);
