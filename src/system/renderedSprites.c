#include "common.h"

#include "system/controller.h"
#include "system/sprite.h"

#include "system/graphic.h"
#include "system/map.h"
#include "system/mapContext.h"

#include "mainproc.h"
#include "npc.h"

// bss
extern CharacterSprite characterSprites[MAX_CHARACTER_SPRITES];
extern RenderedSprite renderedSprites[MAX_RENDERED_SPRITES];
extern Shadow shadowSpritesInfo[3];

// forward declaration
void func_800326C0(u16);
u8 func_80031ED0(RenderedSprite*, u16, f32, f32, u16, u16);

//INCLUDE_ASM(const s32, "system/renderedSprites", initializeNpcSpriteStructs);

void initializeNpcSpriteStructs(void) {
    
    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        renderedSprites[i].flags = 0;
        renderedSprites[i].globalSpriteIndex = 0;
        renderedSprites[i].anim.animationIndex = 0;
        renderedSprites[i].anim.nextAnimationIndex = 0xFFFF;
        renderedSprites[i].direction = 0;
        renderedSprites[i].unk_58 = 0xFFFF;
        renderedSprites[i].unk_5A = 0;
        renderedSprites[i].collision = 0xFFFF;
        renderedSprites[i].startingCoordinates.x = 0;
        renderedSprites[i].startingCoordinates.y = 0;
        renderedSprites[i].startingCoordinates.z = 0;
        renderedSprites[i].currentCoordinates.x = 0;
        renderedSprites[i].currentCoordinates.y = 0;
        renderedSprites[i].currentCoordinates.z = 0;
        renderedSprites[i].unk_4C = 0.0f;
        renderedSprites[i].unk_64 = 0;
        renderedSprites[i].unk_62 = 0;
 
    }
    
    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        characterSprites[i].collisionBufferX = 0;
        characterSprites[i].collisionBufferY = 0;
        characterSprites[i].unk_1C = 0;
        characterSprites[i].unk_1E = 0;
        characterSprites[i].flags = 0; 
    } 
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002DDDC);

bool func_8002DDDC(u16 npcIndex, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, u8 arg7, u8 arg8, void* arg9) {

    bool result = 0;

    if (npcIndex < MAX_CHARACTER_SPRITES) {

        if (!(characterSprites[npcIndex].flags & 1)) {
            
            characterSprites[npcIndex].romTextureStart = arg1;
            characterSprites[npcIndex].romTextureEnd = arg2;
            characterSprites[npcIndex].romAssetIndexStart = arg3;
            characterSprites[npcIndex].romAssetIndexEnd = arg4;
            characterSprites[npcIndex].romSpritesheetIndexStart = arg5;
            characterSprites[npcIndex].romSpritesheetIndexEnd = arg6;
            characterSprites[npcIndex].shadowSpriteIndex= arg8;
            characterSprites[npcIndex].vaddr = arg9;

            characterSprites[npcIndex].collisionBufferX = 0;
            characterSprites[npcIndex].collisionBufferY = 0;

            characterSprites[npcIndex].unk_1C = 0;
            characterSprites[npcIndex].unk_1E = 0;

            characterSprites[npcIndex].flags = 1;

            if (arg7) {
                characterSprites[npcIndex].flags = 3;
            }
            
            result = 1;
            
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002DEE8);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E024);

bool func_8002E024(u16 index, void* arg1, void* arg2, void* arg3, void* arg4) {

    bool result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        
        result = 1;
        
        renderedSprites[index].vaddrSpritesheetIndex = arg4;
        
        renderedSprites[index].flags = 1;
        
        renderedSprites[index].vaddrPalette = arg1;
        renderedSprites[index].vaddrUnknownAsset = arg2;
        renderedSprites[index].vaddrTextureToPaletteLookup = arg3;

        renderedSprites[index].startingCoordinates.x = 0;
        renderedSprites[index].startingCoordinates.y = 0;
        renderedSprites[index].startingCoordinates.z = 0;

        renderedSprites[index].currentCoordinates.x = 0;
        renderedSprites[index].currentCoordinates.y = 0;
        renderedSprites[index].currentCoordinates.z = 0;

        renderedSprites[index].unk_4C = 0.0f;
        renderedSprites[index].unk_58 = 0xFFFF;
        renderedSprites[index].unk_64 = 0;
        
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E108);

bool func_8002E108(u16 index) {

    bool result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        
        renderedSprites[index].flags = 0;

        func_8002B6B8(renderedSprites[index].globalSpriteIndex);
        
        if (characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
            func_8002B6B8(renderedSprites[index].spriteOffset);
        }
        
        result = 1;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E1B8);
 
void func_8002E1B8(void) {

    u16 i = 0;
    u16 temp;

    while (i < MAX_RENDERED_SPRITES) {

        temp = i;

        if (temp < MAX_RENDERED_SPRITES) {

            renderedSprites[i].flags = 0;
        
            func_8002B6B8(renderedSprites[i].globalSpriteIndex);
            
            if (characterSprites[renderedSprites[i].characterIndex].shadowSpriteIndex != 0xFF) {
                func_8002B6B8(renderedSprites[i].spriteOffset);
            } 
        }
        
        i++;
      
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E284);

bool func_8002E284(u16 index, u16 characterIndex, u8 flag) {

    bool result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        
        if (renderedSprites[index].flags & 1) {

            renderedSprites[index].characterIndex = characterIndex;
            renderedSprites[index].flag = flag;

            if (characterSprites[renderedSprites[index].characterIndex].flags & 2) {

                func_8002B138(renderedSprites[index].globalSpriteIndex, 
                    characterSprites[renderedSprites[index].characterIndex].romTextureStart, 
                    characterSprites[renderedSprites[index].characterIndex].romTextureEnd, 
                    characterSprites[renderedSprites[index].characterIndex].romAssetIndexStart, 
                    characterSprites[renderedSprites[index].characterIndex].romAssetIndexEnd, 
                    characterSprites[renderedSprites[index].characterIndex].romSpritesheetIndexStart, 
                    characterSprites[renderedSprites[index].characterIndex].romSpritesheetIndexEnd, 
                    renderedSprites[index].vaddrTexture1,
                    renderedSprites[index].vaddrTexture2,
                    renderedSprites[index].vaddrPalette,
                    renderedSprites[index].vaddrUnknownAsset,
                    renderedSprites[index].vaddrTextureToPaletteLookup,
                    renderedSprites[index].vaddrSpritesheetIndex,
                    1,
                    renderedSprites[index].flag                    
                    );
                
            } else {
                
                func_8002B138(renderedSprites[index].globalSpriteIndex, 
                    characterSprites[renderedSprites[index].characterIndex].romTextureStart, 
                    characterSprites[renderedSprites[index].characterIndex].romTextureEnd, 
                    characterSprites[renderedSprites[index].characterIndex].romAssetIndexStart, 
                    characterSprites[renderedSprites[index].characterIndex].romAssetIndexEnd, 
                    characterSprites[renderedSprites[index].characterIndex].romSpritesheetIndexStart, 
                    characterSprites[renderedSprites[index].characterIndex].romSpritesheetIndexEnd, 
                    renderedSprites[index].vaddrTexture1,
                    0,
                    renderedSprites[index].vaddrPalette,
                    renderedSprites[index].vaddrUnknownAsset,
                    renderedSprites[index].vaddrTextureToPaletteLookup,
                    0,
                    characterSprites[renderedSprites[index].characterIndex].flags & 2 == 1,
                    renderedSprites[index].flag                    
                    );
                
            }

            if (characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
                
                func_8002B138(renderedSprites[index].spriteOffset, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].romTextureStart, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].romTextureEnd, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].romAssetIndexStart, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].romAssetIndexEnd, 
                    0, 
                    0, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].vaddrSpritesheet, 
                    0, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].vaddrPalette, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].vaddrUnknownAssetSheet, 
                    shadowSpritesInfo[characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex].vaddrUnknownAsset2, 
                    0, 
                    0, 
                    renderedSprites[index].flag
                    );

                func_8002C7EC(renderedSprites[index].spriteOffset, 2);
            
            }

            renderedSprites[index].anim.nextAnimationIndex = 0xFFFF;
            renderedSprites[index].flags |= 4;

            func_8002CB24(renderedSprites[index].globalSpriteIndex, 1);
            func_8002FE10(index, 0.0f, 0.0f, 0.0f, 0.0f);

            func_8002F014(index, D_802373F8.r, D_802373F8.g, D_802373F8.b, D_802373F8.a);

            if (!(func_80036A84(gMapModelContext[gMainMapIndex].mapIndex))) {

                func_8002F1E0(index, 
                    mainMap[gMapModelContext[gMainMapIndex].mapIndex].mapStruct8.defaultRgba.r, 
                    mainMap[gMapModelContext[gMainMapIndex].mapIndex].mapStruct8.defaultRgba.g, 
                    mainMap[gMapModelContext[gMainMapIndex].mapIndex].mapStruct8.defaultRgba.b, 
                    mainMap[gMapModelContext[gMainMapIndex].mapIndex].mapStruct8.defaultRgba.a, 
                    mainMap[gMapModelContext[gMainMapIndex].mapIndex].mapStruct9.unk_8);
                
            }
            
            result = 1;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EC18);

bool func_8002EC18(u16 index, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32* arg5, u16* arg6, u16* arg7, u16* arg8, u16 arg9, u8 argA) {

    bool result = 0;
    
    if (index < MAX_SHADOW_SPRITES) {
        result = 1;
        shadowSpritesInfo[index].romTextureStart = arg1;
        shadowSpritesInfo[index].romTextureEnd = arg2;
        shadowSpritesInfo[index].romAssetIndexStart = arg3;
        shadowSpritesInfo[index].romAssetIndexEnd = arg4;
        shadowSpritesInfo[index].vaddrSpritesheet = arg5;
        shadowSpritesInfo[index].vaddrPalette = arg6;
        shadowSpritesInfo[index].vaddrUnknownAssetSheet = arg7;
        shadowSpritesInfo[index].vaddrUnknownAsset2 = arg8;
        shadowSpritesInfo[index].unk_20 = arg9;
        shadowSpritesInfo[index].unk_22 = argA;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002ECD4);

// double check
bool func_8002ECD4(u16 index, u16 arg1, u16 arg2) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {

        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {

            renderedSprites[index].unk_24 = arg1;
            renderedSprites[index].unk_26 = arg2;

            if (arg1 == 0xFFFF) {
                renderedSprites[index].flags &= ~0x200;
            } else {
                renderedSprites[index].flags |= 0x200;
                renderedSprites[index].flags &= ~0x40;
            }

            result = 1;
        }
    }
     
    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002ED80);

bool func_8002ED80(u16 index, s16 arg1) {
    
    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1) {
            result = 1;
            renderedSprites[index].unk_62 = arg1;
            renderedSprites[index].flags |= 0x2000;
        }
    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EDF0);

bool func_8002EDF0(u16 index, s16 arg1, s16 arg2, s16 arg3) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {
            result = 1;
            renderedSprites[index].unk_18.x = arg1;
            renderedSprites[index].unk_18.y = arg2;
            renderedSprites[index].unk_18.z = arg3;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EEA4);

bool func_8002EEA4(u16 mapIndex) {

    bool result = 0;

    if (mapIndex == 0 && gMapModelContext[mapIndex].flags & 1 && gMapModelContext[mapIndex].flags & 2) {
        gMainMapIndex = mapIndex;
        result = 1;
    }

    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EEE4);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EF7C);

bool func_8002EF7C(u16 index, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {
            func_8002BE98(renderedSprites[index].globalSpriteIndex, arg1, arg2, arg3);
            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F014);

bool func_8002F014(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {
            
            func_8002C914(renderedSprites[index].globalSpriteIndex, arg1, arg2, arg3, arg4);
            
            if (characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {

                func_8002C914(renderedSprites[index].spriteOffset, arg1, arg2, arg3, 0x60);
            }

            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F114);

bool func_8002F114(u16 index, u8 shadowSpriteIndex) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {
            
            characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex = shadowSpriteIndex;

            if (characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex == 0xFF) {

                func_8002BAD8(renderedSprites[index].spriteOffset);

            }

            result = 1;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F1E0);

bool func_8002F1E0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5) {

    bool result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {

            func_8002C1C0(renderedSprites[index].globalSpriteIndex, r, g, b, a, arg5);

            if (characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
                func_8002C1C0(renderedSprites[index].spriteOffset, r, g, b, 0x60, arg5);
            }
            
            result = 1;
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F2FC);

bool func_8002F2FC(u16 index, u16 arg1) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        
        if (renderedSprites[index].flags & 1 && renderedSprites[index].flags & 4 && !(renderedSprites[index].flags & 0x1000)) {

            renderedSprites[index].anim.animationIndex = arg1;
            
            renderedSprites[index].flags |= 8;
            renderedSprites[index].flags &= ~0x10; 

            globalSprites[renderedSprites[index].globalSpriteIndex].flags2 &= ~0x40;
            globalSprites[renderedSprites[index].globalSpriteIndex].unk_92 = 0;
            
            result = 1;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", setSpriteAnimation);

u8 setSpriteAnimation(u16 index, u16 arg1) {
    
    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4) && !(renderedSprites[index].flags & 0x1000)) {     
                renderedSprites[index].anim.animationIndex = arg1;
                renderedSprites[index].flags |= 0x18;
                globalSprites[renderedSprites[index].globalSpriteIndex].flags2 &= ~0x40;
                result = 1;
                globalSprites[renderedSprites[index].globalSpriteIndex].unk_92 = 0;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F4E0);

bool func_8002F4E0(u16 index) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {

        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4) && (renderedSprites[index].flags & 0x1000)) {
            
            func_8002E284(index, renderedSprites[index].characterIndex, renderedSprites[index].flag);

            renderedSprites[index].flags |= 8;
 
            renderedSprites[index].flags &= ~0x1000;

            result = 1;
        } 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F594);

bool func_8002F594(u16 index) {

    bool result = 0;
    u16 temp;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {
            
            if (renderedSprites[index].flags & 8) {

                func_8002FE10(index, 0.0f, 0.0f, 0.0f, 0.0f);
                temp = renderedSprites[index].flags & 0x2000;
                renderedSprites[index].flags |= 0x1000;
                func_8002FA2C(index);
                renderedSprites[index].flags |= temp;
                result = 1;
        
            } else {
                renderedSprites[index].flags &= ~0x1000;
            }
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", setSpriteDirection);

bool setSpriteDirection(u16 index, u8 direction) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {

        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4) && !(renderedSprites[index].flags & 0x1000)) {
        
            renderedSprites[index].direction = direction;

            result = 1;
        } 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F6F0);

void func_8002F6F0(void) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002FBBC(i);
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F730);

void func_8002F730(void) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002FC38(i);
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F770);

void func_8002F770(s16 arg0) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        renderedSprites[i].direction += arg0;
        renderedSprites[i].direction &= 7;
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F7C8);

void func_8002F7C8(u8 r, u8 g, u8 b, u8 a) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        if (renderedSprites[i].flags & 1 && renderedSprites[i].flags & 8) {
            if (i < MAX_RENDERED_SPRITES && renderedSprites[i].flags & 4) {
                func_8002C914(renderedSprites[i].globalSpriteIndex, r, g, b, a);
                if (characterSprites[renderedSprites[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    func_8002C914(renderedSprites[i].spriteOffset, r, g, b, 0x60);
                }
            }
        }
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F8F0);

void func_8002F8F0(u8 r, u8 g, u8 b, u8 a, s16 arg4) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        if (renderedSprites[i].flags & 1 && renderedSprites[i].flags & 8) {
            if (i < MAX_RENDERED_SPRITES && renderedSprites[i].flags & 4) {
                func_8002C1C0(renderedSprites[i].globalSpriteIndex, r, g, b, a, arg4);
                if (characterSprites[renderedSprites[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    func_8002C1C0(renderedSprites[i].spriteOffset, r, g, b, 0x60, arg4);
                }
            }
        }
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FA2C);

bool func_8002FA2C(u16 index) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1 && renderedSprites[index].flags & 8) {
            renderedSprites[index].flags &= ~( 0x8 | 0x2000);
            func_8002B6B8(renderedSprites[index].globalSpriteIndex);
            if (characterSprites[renderedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
                func_8002B6B8(renderedSprites[index].spriteOffset);
            }
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FAFC);

void func_8002FAFC(void) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002FA2C(i);
    }
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FB3C);

void func_8002FB3C(void) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002F594(i);
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FB7C);

void func_8002FB7C(void) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002F4E0(i);
    }
}


//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FBBC);

bool func_8002FBBC(u16 index) {

    bool result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1) {
            renderedSprites[index].flags |= 0x40;
            func_8002BB88(renderedSprites[index].globalSpriteIndex);
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FC38);

bool func_8002FC38(u16 index) {
    
    bool result = 0;
    u16 temp;

    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1) {
            renderedSprites[index].flags &= ~0x40;
            func_8002BB30(renderedSprites[index].globalSpriteIndex);
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FCB4);

bool func_8002FCB4(u16 index, u8 flag) {
    
    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 8)) {

            if (flag) {
                renderedSprites[index].flags &= ~0x100;
            } else {
                renderedSprites[index].flags |= 0x100;
            }

            result = 1;

        }
    }
    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FD24);

bool func_8002FD24(u16 index) {
    
    bool result = 0;
    u16 temp;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 8)) {
            temp = renderedSprites[index].flags & 0x100;
            result = !temp;
        }
    }
    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FD80);

bool func_8002FD80(u16 spriteIndex, f32 x, f32 y, f32 z) {

    bool result = 0;
    
    if (spriteIndex < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[spriteIndex].flags & 1) && !(renderedSprites[spriteIndex].flags & 0x1000)) {
            result = 1;
            renderedSprites[spriteIndex].startingCoordinates.x = x;
            renderedSprites[spriteIndex].startingCoordinates.y = y;
            renderedSprites[spriteIndex].startingCoordinates.z = z;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FE10);

bool func_8002FE10(u16 spriteIndex, f32 x, f32 y, f32 z, f32 arg4) {

    bool result = 0;
    
    if (spriteIndex < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[spriteIndex].flags & 1) && (renderedSprites[spriteIndex].flags & 8) && !(renderedSprites[spriteIndex].flags & 0x1000)) {
            result = 1;
            renderedSprites[spriteIndex].currentCoordinates.x = x;
            renderedSprites[spriteIndex].currentCoordinates.y = y;
            renderedSprites[spriteIndex].currentCoordinates.z = z;
            renderedSprites[spriteIndex].unk_4C = arg4 / 2.0f;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FECC);

bool func_8002FECC(u16 index) {

    bool result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1) {
            result = func_8002CC44(renderedSprites[index].globalSpriteIndex);
        }
    }

    return result;

}

//sINCLUDE_ASM(const s32, "system/renderedSprites", func_8002FF38);

bool func_8002FF38(u16 spriteIndex, u16 flag) {

    bool result = 0;

    if (spriteIndex < MAX_RENDERED_SPRITES) {
        if (renderedSprites[spriteIndex].flags & 1) { 

            if (flag) {
                renderedSprites[spriteIndex].flags |= 0x20;
            } else {
                renderedSprites[spriteIndex].flags &= ~0x20;
            }

            result = 1;

        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/renderedSprites", setSpriteCollisionBuffers);

u16 setSpriteCollisionBuffers(u16 spriteIndex, u8 xValue, u8 yValue) {
    
    u16 result = 0;
    
    if (spriteIndex < MAX_CHARACTER_SPRITES) {
        if (characterSprites[spriteIndex].flags & 1) {
            characterSprites[spriteIndex].collisionBufferX = xValue;
            characterSprites[spriteIndex].collisionBufferY = yValue;
            result = 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FFF4);

bool func_8002FFF4(u16 npcIndex, u8 arg1, u8 arg2) {

    bool result = 0;

    if (npcIndex < MAX_CHARACTER_SPRITES) {
        if (characterSprites[npcIndex].flags & 1) {
            result = 1;
            characterSprites[npcIndex].unk_1C = arg1;
            characterSprites[npcIndex].unk_1E = arg2;
        }
    }

    return result;
    

}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80030054);

bool func_80030054(u16 index, u8 flag) {

    bool result = 0;

    if (flag == 1) {
        renderedSprites[index].flags &= ~0x80;
        result = 1;
    } else {
        if (index < MAX_RENDERED_SPRITES && renderedSprites[index].flags & 1) {
            renderedSprites[index].flags |= 0x80;
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800300F8);

bool func_800300F8(u16 index, u8 flag) {

    bool result = 0;

    if (flag == 1) {
        renderedSprites[index].flags &= ~0x8000;
        result = 1;
    } else {
        if (index < MAX_RENDERED_SPRITES && renderedSprites[index].flags & 1) {
            renderedSprites[index].flags |= 0x8000;
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8003019C);

bool func_8003019C(u16 index, u8 flag) {

    bool result = 0;

    if (flag == 1) {
        renderedSprites[index].flags &= ~0x400;
        result = 1;
    } else {
        if (index < MAX_RENDERED_SPRITES && renderedSprites[index].flags & 1) {
            renderedSprites[index].flags |= 0x400;
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80030240);

bool func_80030240(u16 index, u8 flag) {

    bool result = 0;

    if (flag == 1) {
        renderedSprites[index].flags &= ~0x4000;
        result = 1;
    } else {
        if (index < MAX_RENDERED_SPRITES && renderedSprites[index].flags & 1) {
            renderedSprites[index].flags |= 0x4000;
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800302E4);

bool func_800302E4(u16 index, u8 flag) {

    bool result = 0;

    if (flag == 1) {
        renderedSprites[index].flags &= ~2;
        result = 1;
    } else {
        if (index < MAX_RENDERED_SPRITES && renderedSprites[index].flags & 1) {
            renderedSprites[index].flags |= 2;
            result = 1;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80030388);

bool func_80030388(u16 index) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        
        if (renderedSprites[index].flags & 1) {
            result = (renderedSprites[index].flags >> 6) & 1;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_800303D4);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800305CC);

u16 func_800305CC(u16 index, f32 arg1, f32 arg2, u16 arg3) {

    u16 result = 0;
    u32 temp;
    u16 i = 0;
    u8 set = 0;
    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    if (index < MAX_RENDERED_SPRITES) {
        
        if (renderedSprites[index].flags & 1) {
            
            if (!(renderedSprites[index].flags & 0x40)) {
                
                vec1.x = arg1;
                vec1.y = 0;
                vec1.z = arg2;

                // angles/degrees
                vec3.x = 0;
                vec3.y = func_80028820((renderedSprites[index].direction + func_8003C1A4(gMainMapIndex)) % 8);
                vec3.z = 0;

                // do 3D rotation
                func_80027950(vec1, &vec2, vec3);

                for (i = 0; i < MAX_RENDERED_SPRITES; i++) {

                    if (i != index && !(renderedSprites[i].flags & 0x200)) {
                        
                        if (temp = func_80031ED0(&renderedSprites[index], i, vec2.x, vec2.z, characterSprites[renderedSprites[index].characterIndex].unk_1C, characterSprites[renderedSprites[index].characterIndex].unk_1E)) {
                           
                            result |= temp;
                            
                            if (func_8004D380(CONTROLLER_1, arg3) || !arg3) {
                                if (!set) {
                                    renderedSprites[i].unk_58 = index;
                                    renderedSprites[i].unk_5A = arg3;
                                    if (!(renderedSprites[i].flags & 0x4000)) {
                                        renderedSprites[index].collision = i;    
                                    } else {
                                        renderedSprites[index].collision = 0xFFFF;
                                    }
                                    renderedSprites[i].unk_5E = 0xFFFF;
                                    set = 1;
                                    continue;
                                }
                            }

                            renderedSprites[i].unk_5E = index;
                            
                        } else {

                            renderedSprites[i].unk_5E = 0xFFFF;
                            renderedSprites[i].unk_58 = 0xFFFF;
                            renderedSprites[i].unk_5A = 0;
                            
                            if (!set) {
                                renderedSprites[index].collision = 0xFFFF;
                            }
                            
                        }       
                    }
                }
            }
        }
    }
    
    return result;
   
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800308E0);

bool func_800308E0(u16 spriteIndex, f32 arg1, f32 arg2) {
    
    bool result = 0; 

    if (spriteIndex < MAX_RENDERED_SPRITES) {

        if (renderedSprites[spriteIndex].flags & 1 && !(renderedSprites[spriteIndex].flags & 0x40)) {
            result = func_80036318(gMapModelContext[gMainMapIndex].mapIndex, renderedSprites[spriteIndex].startingCoordinates.x + arg1, renderedSprites[spriteIndex].startingCoordinates.z + arg2);
        }

    }

    return result;
    
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_800309B4);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030B24);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80030BA0);

u16 func_80030BA0(u16* ptr, u16 offset) {
    return *(ptr + offset) & 0x1FFF;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030BB8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030C34);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030CB0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030DB0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030EAC);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031050);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800311E0);

// get audio related flag from sprite
//INCLUDE_ASM(const s32, "system/renderedSprites", func_80031380);

bool func_80031380(u16 spriteIndex) {
    
    bool result = 0; 

    if (spriteIndex < MAX_RENDERED_SPRITES) {

        if (renderedSprites[spriteIndex].flags & 1 && !(renderedSprites[spriteIndex].flags & 0x40)) {
            result = globalSprites[renderedSprites[spriteIndex].globalSpriteIndex].unk_92;
        }

    }

    return result;

}

INCLUDE_ASM(const s32, "system/renderedSprites", func_800313FC);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800315A0);

Vec3f* func_800315A0(Vec3f* arg0, u16 index) {

    // likely a struct
    int padding[11];
    
    Vec3f vec;

    vec.x = 0;
    vec.z = 0;
    vec.y = MAX_UNSIGNED_SHORT;
    
    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && !(renderedSprites[index].flags & 0x40) && !(renderedSprites[index].flags & 0x100)) {
            func_80036610(&vec, gMapModelContext[gMainMapIndex].mapIndex, renderedSprites[index].startingCoordinates.x, renderedSprites[index].startingCoordinates.z);
        } 
    }

    *arg0 = vec;
 
    return arg0;
}

// alternate
// Vec3f func_800315A0(u16 index) {
    
//     int padding[11];

//     Vec3f vec;

//     vec.x = 0;
//     vec.z = 0;
//     vec.y = 65535.0f;
    
//     if (index < MAX_RENDERED_SPRITES) {
//         if ((renderedSprites[index].flags & 1) && !(renderedSprites[index].flags & 0x40) && !(renderedSprites[index].flags & 0x100)) {
//             func_80036610(&vec, gMapModelContext[gMainMapIndex].mapIndex, renderedSprites[index].startingCoordinates.x, renderedSprites[index].startingCoordinates.z);
//         } 
//     }
 
//     return vec;
// }

INCLUDE_ASM(const s32, "system/renderedSprites", func_8003168C);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80031830);

bool func_80031830(u16 index, u32 arg1, u8 arg2) {

    Vec3f vec;
    
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    
    bool result = 0;
    u8 check1;
    u8 check2;
    
    temp3 = renderedSprites[index].startingCoordinates.y + 24.0f;
    temp4 = renderedSprites[index].startingCoordinates.y - 24.0f;
    
    func_80031904(&vec, index, arg1, arg2);
    temp2 = func_80035150(MAIN_MAP_INDEX, vec.x, vec.z);

    check2 = 1;
    
    if (!(temp3 <= temp2)) {
        check2 = 0;
    }

    check1 = 1;
    
    if (!(temp2 <= temp4)) {
        check1 = 0;
    }

    if (check1 || check2) {
        result = 1;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80031904);

Vec3f* func_80031904(Vec3f* vec, u16 index, s16 arg2, u8 arg3) {

    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = arg2;

    vec3.x = 0;
    vec3.y = func_80028820(arg3 % 8);
    vec3.z = 0;

    // do 3D rotation
    func_80027950(vec1, &vec2, vec3);

    vec1.x = renderedSprites[index].startingCoordinates.x + vec2.x;
    vec1.y = renderedSprites[index].startingCoordinates.y + vec2.y;
    vec1.z = renderedSprites[index].startingCoordinates.z + vec2.z;

    *vec = vec1;
    
    return vec;
    
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031A10);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031ED0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800321A8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80032330);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800326C0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80033058);
