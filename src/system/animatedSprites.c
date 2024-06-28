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
extern AnimatedSprite animatedSprites[MAX_ANIMATED_SPRITES];
extern Shadow shadowSpritesInfo[3];

// forward declaration
Vec3f* func_800313FC(Vec3f*, u16, f32, f32);  
u8 func_80031ED0(AnimatedSprite*, u16, f32, f32, u16, u16);
void func_800326C0(u16);

//INCLUDE_ASM(const s32, "system/animatedSprites", initializeNpcSpriteStructs);

void initializeNpcSpriteStructs(void) {
    
    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        animatedSprites[i].flags = 0;
        animatedSprites[i].globalSpriteIndex = 0;
        animatedSprites[i].anim.animationIndex = 0;
        animatedSprites[i].anim.nextAnimationIndex = 0xFFFF;
        animatedSprites[i].direction = 0;
        animatedSprites[i].unk_58 = 0xFFFF;
        animatedSprites[i].unk_5A = 0;
        animatedSprites[i].collision = 0xFFFF;
        animatedSprites[i].startingCoordinates.x = 0;
        animatedSprites[i].startingCoordinates.y = 0;
        animatedSprites[i].startingCoordinates.z = 0;
        animatedSprites[i].currentCoordinates.x = 0;
        animatedSprites[i].currentCoordinates.y = 0;
        animatedSprites[i].currentCoordinates.z = 0;
        animatedSprites[i].unk_4C = 0.0f;
        animatedSprites[i].unk_64 = 0;
        animatedSprites[i].unk_62 = 0;
 
    }
    
    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        characterSprites[i].collisionBufferX = 0;
        characterSprites[i].collisionBufferY = 0;
        characterSprites[i].unk_1C = 0;
        characterSprites[i].unk_1E = 0;
        characterSprites[i].flags = 0; 
    } 
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002DDDC);

bool func_8002DDDC(u16 npcIndex, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, u8 arg7, u8 arg8, u16* arg9) {

    bool result = FALSE;

    if (npcIndex < MAX_CHARACTER_SPRITES) {

        if (!(characterSprites[npcIndex].flags & 1)) {
            
            characterSprites[npcIndex].romTextureStart = arg1;
            characterSprites[npcIndex].romTextureEnd = arg2;
            characterSprites[npcIndex].romAssetIndexStart = arg3;
            characterSprites[npcIndex].romAssetIndexEnd = arg4;
            characterSprites[npcIndex].romSpritesheetIndexStart = arg5;
            characterSprites[npcIndex].romSpritesheetIndexEnd = arg6;
            characterSprites[npcIndex].shadowSpriteIndex= arg8;
            characterSprites[npcIndex].spriteData = arg9;

            characterSprites[npcIndex].collisionBufferX = 0;
            characterSprites[npcIndex].collisionBufferY = 0;

            characterSprites[npcIndex].unk_1C = 0;
            characterSprites[npcIndex].unk_1E = 0;

            characterSprites[npcIndex].flags = 1;

            if (arg7) {
                characterSprites[npcIndex].flags = 3;
            }
            
            result = TRUE;
            
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002DEE8);

bool func_8002DEE8(u16 spriteIndex, u16 globalSpriteIndex, u16 shadowSpriteIndex, void* vaddrTexture1, void* vaddrTexture2, 
    void* vaddrPalette, void* vaddrUnknownAsset, void* vaddrTextureToPaletteLookup, void* vaddrSpritesheetIndex) {

    bool set = FALSE;
    
    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if (!(animatedSprites[spriteIndex].flags & 1)) {

            animatedSprites[spriteIndex].flags = 1;

            animatedSprites[spriteIndex].globalSpriteIndex = globalSpriteIndex;
            animatedSprites[spriteIndex].shadowSpriteIndex = shadowSpriteIndex;

            animatedSprites[spriteIndex].vaddrTexture1 = vaddrTexture1;
            animatedSprites[spriteIndex].vaddrTexture2 = vaddrTexture2;
            animatedSprites[spriteIndex].vaddrPalette = vaddrPalette;
            animatedSprites[spriteIndex].vaddrUnknownAsset = vaddrUnknownAsset;
            animatedSprites[spriteIndex].vaddrSpritesheetIndex = vaddrSpritesheetIndex;
            animatedSprites[spriteIndex].vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

            animatedSprites[spriteIndex].startingCoordinates.x = 0;
            animatedSprites[spriteIndex].startingCoordinates.y = 0;
            animatedSprites[spriteIndex].startingCoordinates.z = 0;

            animatedSprites[spriteIndex].currentCoordinates.x = 0;
            animatedSprites[spriteIndex].currentCoordinates.y = 0;
            animatedSprites[spriteIndex].currentCoordinates.z = 0;

            animatedSprites[spriteIndex].unk_4C = 0;

            animatedSprites[spriteIndex].unk_58 = 0xFFFF;

            animatedSprites[spriteIndex].unk_64 = 0;
        
            set = TRUE;
            
        }
    }

    return set;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002E024);

bool func_8002E024(u16 index, void* arg1, void* arg2, void* arg3, void* arg4) {

    bool result = FALSE;
    
    if (index < MAX_ANIMATED_SPRITES) {
        
        result = TRUE;
        
        animatedSprites[index].vaddrSpritesheetIndex = arg4;
        
        animatedSprites[index].flags = 1;
        
        animatedSprites[index].vaddrPalette = arg1;
        animatedSprites[index].vaddrUnknownAsset = arg2;
        animatedSprites[index].vaddrTextureToPaletteLookup = arg3;

        animatedSprites[index].startingCoordinates.x = 0;
        animatedSprites[index].startingCoordinates.y = 0;
        animatedSprites[index].startingCoordinates.z = 0;

        animatedSprites[index].currentCoordinates.x = 0;
        animatedSprites[index].currentCoordinates.y = 0;
        animatedSprites[index].currentCoordinates.z = 0;

        animatedSprites[index].unk_4C = 0.0f;
        animatedSprites[index].unk_58 = 0xFFFF;
        animatedSprites[index].unk_64 = 0;
        
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002E108);

bool func_8002E108(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ANIMATED_SPRITES) {
        
        animatedSprites[index].flags = 0;

        deactivateSprite(animatedSprites[index].globalSpriteIndex);
        
        if (characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
            deactivateSprite(animatedSprites[index].shadowSpriteIndex);
        }
        
        result = TRUE;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", deactivateSprite);
 
void deactivateSprites(void) {

    u16 i = 0;
    u16 temp;

    while (i < MAX_ANIMATED_SPRITES) {

        temp = i;

        if (temp < MAX_ANIMATED_SPRITES) {

            animatedSprites[i].flags = 0;
        
            deactivateSprite(animatedSprites[i].globalSpriteIndex);
            
            // toggle flag on shadow sprite
            if (characterSprites[animatedSprites[i].characterIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(animatedSprites[i].shadowSpriteIndex);
            } 
        }
        
        i++;
      
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002E284);

bool func_8002E284(u16 index, u16 characterIndex, u8 flag) {

    bool result = FALSE;
    
    if (index < MAX_ANIMATED_SPRITES) {
        
        if (animatedSprites[index].flags & 1) {

            animatedSprites[index].characterIndex = characterIndex;
            animatedSprites[index].flag = flag;

            if (characterSprites[animatedSprites[index].characterIndex].flags & 2) {

                dmaSprite(animatedSprites[index].globalSpriteIndex, 
                    characterSprites[animatedSprites[index].characterIndex].romTextureStart, 
                    characterSprites[animatedSprites[index].characterIndex].romTextureEnd, 
                    characterSprites[animatedSprites[index].characterIndex].romAssetIndexStart, 
                    characterSprites[animatedSprites[index].characterIndex].romAssetIndexEnd, 
                    characterSprites[animatedSprites[index].characterIndex].romSpritesheetIndexStart, 
                    characterSprites[animatedSprites[index].characterIndex].romSpritesheetIndexEnd, 
                    animatedSprites[index].vaddrTexture1,
                    animatedSprites[index].vaddrTexture2,
                    animatedSprites[index].vaddrPalette,
                    animatedSprites[index].vaddrUnknownAsset,
                    animatedSprites[index].vaddrTextureToPaletteLookup,
                    animatedSprites[index].vaddrSpritesheetIndex,
                    1,
                    animatedSprites[index].flag                    
                    );
                
            } else {
                
                dmaSprite(animatedSprites[index].globalSpriteIndex, 
                    characterSprites[animatedSprites[index].characterIndex].romTextureStart, 
                    characterSprites[animatedSprites[index].characterIndex].romTextureEnd, 
                    characterSprites[animatedSprites[index].characterIndex].romAssetIndexStart, 
                    characterSprites[animatedSprites[index].characterIndex].romAssetIndexEnd, 
                    characterSprites[animatedSprites[index].characterIndex].romSpritesheetIndexStart, 
                    characterSprites[animatedSprites[index].characterIndex].romSpritesheetIndexEnd, 
                    animatedSprites[index].vaddrTexture1,
                    0,
                    animatedSprites[index].vaddrPalette,
                    animatedSprites[index].vaddrUnknownAsset,
                    animatedSprites[index].vaddrTextureToPaletteLookup,
                    0,
                    characterSprites[animatedSprites[index].characterIndex].flags & 2 == 1,
                    animatedSprites[index].flag                    
                    );
                
            }

            if (characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
                
                dmaSprite(animatedSprites[index].shadowSpriteIndex, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].romTextureStart, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].romTextureEnd, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].romAssetIndexStart, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].romAssetIndexEnd, 
                    0, 
                    0, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].vaddrSpritesheet, 
                    0, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].vaddrPalette, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].vaddrUnknownAssetSheet, 
                    shadowSpritesInfo[characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex].vaddrUnknownAsset2, 
                    0, 
                    0, 
                    animatedSprites[index].flag
                    );

                func_8002C7EC(animatedSprites[index].shadowSpriteIndex, 2);
            
            }

            animatedSprites[index].anim.nextAnimationIndex = 0xFFFF;
            animatedSprites[index].flags |= 4;

            func_8002CB24(animatedSprites[index].globalSpriteIndex, 1);
            func_8002FE10(index, 0.0f, 0.0f, 0.0f, 0.0f);

            func_8002F014(index, D_802373F8.r, D_802373F8.g, D_802373F8.b, D_802373F8.a);

            if (!(func_80036A84(gMapModelContext[gMainMapIndex].mainMapIndex))) {
 
                func_8002F1E0(index, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.r, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.g, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.b, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.a, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapStruct9.unk_8);
                
            }
            
            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002EC18);

bool func_8002EC18(u16 index, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32* arg5, u16* arg6, u16* arg7, u16* arg8, u16 arg9, u8 argA) {

    bool result = FALSE;
    
    if (index < MAX_SHADOW_SPRITES) {
        result = TRUE;
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

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002ECD4);

// double check
bool func_8002ECD4(u16 index, u16 arg1, u16 arg2) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {

            animatedSprites[index].unk_24 = arg1;
            animatedSprites[index].unk_26 = arg2;

            if (arg1 == 0xFFFF) {
                animatedSprites[index].flags &= ~0x200;
            } else {
                animatedSprites[index].flags |= 0x200;
                animatedSprites[index].flags &= ~0x40;
            }

            result = TRUE;
        }
    }
     
    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002ED80);

bool func_8002ED80(u16 index, s16 arg1) {
    
    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[index].flags & 1) {
            result = TRUE;
            animatedSprites[index].unk_62 = arg1;
            animatedSprites[index].flags |= 0x2000;
        }
    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002EDF0);

bool func_8002EDF0(u16 index, s16 arg1, s16 arg2, s16 arg3) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {
            result = TRUE;
            animatedSprites[index].unk_18.x = arg1;
            animatedSprites[index].unk_18.y = arg2;
            animatedSprites[index].unk_18.z = arg3;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002EEA4);

bool func_8002EEA4(u16 mapIndex) {

    bool result = FALSE;

    if (mapIndex == 0 && gMapModelContext[mapIndex].flags & 1 && gMapModelContext[mapIndex].flags & 2) {
        gMainMapIndex = mapIndex;
        result = TRUE;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002EEE4);

bool func_8002EEE4(u16 spriteIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool set = FALSE;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[spriteIndex].flags & 1) && (animatedSprites[spriteIndex].flags & 4)) {
            setSpriteShrinkFactor(animatedSprites[spriteIndex].globalSpriteIndex, arg1, arg2, arg3);
            set = TRUE;
        }
        
    }
    
    return set;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002EF7C);

bool func_8002EF7C(u16 index, f32 arg1, f32 arg2, f32 arg3) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {
            adjustSpriteShrinkFactor(animatedSprites[index].globalSpriteIndex, arg1, arg2, arg3);
            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F014);

bool func_8002F014(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {
            
            func_8002C914(animatedSprites[index].globalSpriteIndex, arg1, arg2, arg3, arg4);
            
            if (characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {

                func_8002C914(animatedSprites[index].shadowSpriteIndex, arg1, arg2, arg3, 0x60);
                
            }

            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F114);

bool func_8002F114(u16 index, u8 shadowSpriteIndex) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {
            
            characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex = shadowSpriteIndex;

            if (characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex == 0xFF) {

                func_8002BAD8(animatedSprites[index].shadowSpriteIndex);

            }

            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F1E0);

bool func_8002F1E0(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5) {

    bool result = FALSE;
    
    if (index < MAX_ANIMATED_SPRITES) {
        
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {

            updateSpriteRGBA(animatedSprites[index].globalSpriteIndex, r, g, b, a, arg5);

            if (characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
                updateSpriteRGBA(animatedSprites[index].shadowSpriteIndex, r, g, b, 0x60, arg5);
            }
            
            result = TRUE;
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F2FC);

bool func_8002F2FC(u16 index, u16 arg1) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        
        if (animatedSprites[index].flags & 1 && animatedSprites[index].flags & 4 && !(animatedSprites[index].flags & 0x1000)) {

            animatedSprites[index].anim.animationIndex = arg1;
            
            animatedSprites[index].flags |= 8;
            animatedSprites[index].flags &= ~0x10; 

            globalSprites[animatedSprites[index].globalSpriteIndex].flags2 &= ~0x40;
            globalSprites[animatedSprites[index].globalSpriteIndex].unk_92 = 0;
            
            result = TRUE;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", setSpriteAnimation);

bool setSpriteAnimation(u16 index, u16 arg1) {
    
    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4) && !(animatedSprites[index].flags & 0x1000)) {     
                animatedSprites[index].anim.animationIndex = arg1;
                animatedSprites[index].flags |= 0x18;
                globalSprites[animatedSprites[index].globalSpriteIndex].flags2 &= ~0x40;
                result = TRUE;
                globalSprites[animatedSprites[index].globalSpriteIndex].unk_92 = 0;
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F4E0);

bool func_8002F4E0(u16 index) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4) && (animatedSprites[index].flags & 0x1000)) {
            
            func_8002E284(index, animatedSprites[index].characterIndex, animatedSprites[index].flag);

            animatedSprites[index].flags |= 8;
 
            animatedSprites[index].flags &= ~0x1000;

            result = TRUE;
        } 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F594);

bool func_8002F594(u16 index) {

    bool result = FALSE;
    u16 temp;

    if (index < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4)) {
            
            if (animatedSprites[index].flags & 8) {

                func_8002FE10(index, 0.0f, 0.0f, 0.0f, 0.0f);
                temp = animatedSprites[index].flags & 0x2000;
                animatedSprites[index].flags |= 0x1000;
                func_8002FA2C(index);
                animatedSprites[index].flags |= temp;
                result = TRUE;
        
            } else {
                animatedSprites[index].flags &= ~0x1000;
            }

        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", setSpriteDirection);

bool setSpriteDirection(u16 index, u8 direction) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 4) && !(animatedSprites[index].flags & 0x1000)) {
        
            animatedSprites[index].direction = direction;

            result = TRUE;
        } 
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F6F0);

void func_8002F6F0(void) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        func_8002FBBC(i);
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F730);

void func_8002F730(void) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        func_8002FC38(i);
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F770);

void func_8002F770(s16 arg0) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        animatedSprites[i].direction += arg0;
        animatedSprites[i].direction &= 7;
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F7C8);

void func_8002F7C8(u8 r, u8 g, u8 b, u8 a) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        if (animatedSprites[i].flags & 1 && animatedSprites[i].flags & 8) {
            if (i < MAX_ANIMATED_SPRITES && animatedSprites[i].flags & 4) {
                func_8002C914(animatedSprites[i].globalSpriteIndex, r, g, b, a);
                if (characterSprites[animatedSprites[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    func_8002C914(animatedSprites[i].shadowSpriteIndex, r, g, b, SHADOW_ALPHA);
                }
            }
        }
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002F8F0);

void func_8002F8F0(u8 r, u8 g, u8 b, u8 a, s16 arg4) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {

        if (animatedSprites[i].flags & 1 && animatedSprites[i].flags & 8) {
        
            if (i < MAX_ANIMATED_SPRITES && animatedSprites[i].flags & 4) {
        
                updateSpriteRGBA(animatedSprites[i].globalSpriteIndex, r, g, b, a, arg4);
        
                if (characterSprites[animatedSprites[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    updateSpriteRGBA(animatedSprites[i].shadowSpriteIndex, r, g, b, 0x60, arg4);
                }
        
            }
        }
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FA2C);

bool func_8002FA2C(u16 index) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {

        if (animatedSprites[index].flags & 1 && animatedSprites[index].flags & 8) {
            
            animatedSprites[index].flags &= ~( 0x8 | 0x2000);
            deactivateSprite(animatedSprites[index].globalSpriteIndex);
            
            if (characterSprites[animatedSprites[index].characterIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(animatedSprites[index].shadowSpriteIndex);
            }

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FAFC);

void func_8002FAFC(void) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        func_8002FA2C(i);
    }
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FB3C);

void func_8002FB3C(void) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        func_8002F594(i);
    }
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FB7C);

void func_8002FB7C(void) {

    u16 i;

    for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {
        func_8002F4E0(i);
    }
}


//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FBBC);

bool func_8002FBBC(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[index].flags & 1) {
            animatedSprites[index].flags |= 0x40;
            func_8002BB88(animatedSprites[index].globalSpriteIndex);
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FC38);

bool func_8002FC38(u16 index) {
    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[index].flags & 1) {
            animatedSprites[index].flags &= ~0x40;
            func_8002BB30(animatedSprites[index].globalSpriteIndex);
            result = TRUE;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FCB4);

bool func_8002FCB4(u16 index, u8 flag) {
    
    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 8)) {

            if (flag) {
                animatedSprites[index].flags &= ~0x100;
            } else {
                animatedSprites[index].flags |= 0x100;
            }

            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FD24);

bool func_8002FD24(u16 index) {
    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[index].flags & 1) && (animatedSprites[index].flags & 8)) {
            temp = animatedSprites[index].flags & 0x100;
            result = !temp;
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", setSpriteStartingCoordinates);

bool setSpriteStartingCoordinates(u16 spriteIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x1000)) {
            
            animatedSprites[spriteIndex].startingCoordinates.x = x;
            animatedSprites[spriteIndex].startingCoordinates.y = y;
            animatedSprites[spriteIndex].startingCoordinates.z = z;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FE10);

bool func_8002FE10(u16 spriteIndex, f32 x, f32 y, f32 z, f32 arg4) {

    bool result = FALSE;
    
    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[spriteIndex].flags & 1) && (animatedSprites[spriteIndex].flags & 8) && !(animatedSprites[spriteIndex].flags & 0x1000)) {
            
            animatedSprites[spriteIndex].currentCoordinates.x = x;
            animatedSprites[spriteIndex].currentCoordinates.y = y;
            animatedSprites[spriteIndex].currentCoordinates.z = z;
            animatedSprites[spriteIndex].unk_4C = arg4 / 2.0f;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FECC);

bool func_8002FECC(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[index].flags & 1) {
            result = func_8002CC44(animatedSprites[index].globalSpriteIndex);
        }
    }

    return result;

}

//sINCLUDE_ASM(const s32, "system/animatedSprites", func_8002FF38);

bool func_8002FF38(u16 spriteIndex, u16 flag) {

    bool result = FALSE;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[spriteIndex].flags & 1) { 

            if (flag) {
                animatedSprites[spriteIndex].flags |= 0x20;
            } else {
                animatedSprites[spriteIndex].flags &= ~0x20;
            }

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", setSpriteCollisionBuffers);

bool setSpriteCollisionBuffers(u16 spriteIndex, u8 xValue, u8 yValue) {
    
    bool result = FALSE;
    
    if (spriteIndex < MAX_CHARACTER_SPRITES) {
        if (characterSprites[spriteIndex].flags & 1) {
            characterSprites[spriteIndex].collisionBufferX = xValue;
            characterSprites[spriteIndex].collisionBufferY = yValue;
            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8002FFF4);

bool func_8002FFF4(u16 npcIndex, u8 arg1, u8 arg2) {

    bool result = FALSE;

    if (npcIndex < MAX_CHARACTER_SPRITES) {
        if (characterSprites[npcIndex].flags & 1) {
            characterSprites[npcIndex].unk_1C = arg1;
            characterSprites[npcIndex].unk_1E = arg2;
            result = TRUE;
        }
    }

    return result;
    

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030054);

bool func_80030054(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        animatedSprites[index].flags &= ~0x80;
        result = TRUE;
    } else {
        if (index < MAX_ANIMATED_SPRITES && animatedSprites[index].flags & 1) {
            animatedSprites[index].flags |= 0x80;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800300F8);

bool func_800300F8(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        animatedSprites[index].flags &= ~0x8000;
        result = TRUE;
    } else {
        if (index < MAX_ANIMATED_SPRITES && animatedSprites[index].flags & 1) {
            animatedSprites[index].flags |= 0x8000;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8003019C);

bool func_8003019C(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        animatedSprites[index].flags &= ~0x400;
        result = TRUE;
    } else {
        if (index < MAX_ANIMATED_SPRITES && animatedSprites[index].flags & 1) {
            animatedSprites[index].flags |= 0x400;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030240);

bool func_80030240(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        animatedSprites[index].flags &= ~0x4000;
        result = TRUE;
    } else {
        if (index < MAX_ANIMATED_SPRITES && animatedSprites[index].flags & 1) {
            animatedSprites[index].flags |= 0x4000;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800302E4);

bool func_800302E4(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        animatedSprites[index].flags &= ~2;
        result = TRUE;
    } else {
        if (index < MAX_ANIMATED_SPRITES && animatedSprites[index].flags & 1) {
            animatedSprites[index].flags |= 2;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030388);

bool func_80030388(u16 index) {

    bool result = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        
        if (animatedSprites[index].flags & 1) {
            result = (animatedSprites[index].flags >> 6) & 1;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800303D4);

u16 func_800303D4(u16 spriteIndex, f32 arg1, f32 arg2, u16 arg3, u16 arg4) {

    // FIXME: shouldn't be necessary
    u32 padding[4];
    
    u16 i;
    u16 index = 0;
    
    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if (animatedSprites[spriteIndex].flags & 1 && !(animatedSprites[spriteIndex].flags & 0x40)) {

            for (i = 0; i < arg4; i++) {
                
                if (i != spriteIndex) {
                    
                    if (!(animatedSprites[i].flags & 0x4000)) {

                        index = func_80031ED0(&animatedSprites[spriteIndex], i, arg1, arg2, characterSprites[animatedSprites[spriteIndex].characterIndex].unk_1C, characterSprites[animatedSprites[spriteIndex].characterIndex].unk_1E);
                        
                        if (index) {

                            animatedSprites[i].unk_58 = spriteIndex;
                            animatedSprites[i].unk_5A = arg3;
                            animatedSprites[spriteIndex].collision = i;
                            
                            i = MAX_ANIMATED_SPRITES;
                            
                        } else {

                            animatedSprites[i].unk_58 = 0xFFFF;
                            animatedSprites[i].unk_5A = 0;
                            animatedSprites[spriteIndex].collision = 0xFFFF;
                            
                        }
                    }
                }
            }   
        }
    }
    
    return index;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800305CC);

u16 func_800305CC(u16 index, f32 arg1, f32 arg2, u16 arg3) {

    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;
    
    u16 result = 0;
    u32 temp;
    u16 i = 0;
    bool set = FALSE;

    if (index < MAX_ANIMATED_SPRITES) {
        
        if (animatedSprites[index].flags & 1) {
            
            if (!(animatedSprites[index].flags & 0x40)) {
                
                vec1.x = arg1;
                vec1.y = 0;
                vec1.z = arg2;

                vec3.x = 0;
                vec3.y = getSpriteYValueFromDirection((animatedSprites[index].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
                vec3.z = 0;

                // do 3D rotation
                func_80027950(vec1, &vec2, vec3);

                for (i = 0; i < MAX_ANIMATED_SPRITES; i++) {

                    if (i != index && !(animatedSprites[i].flags & 0x200)) {
                        
                        if (temp = func_80031ED0(&animatedSprites[index], i, vec2.x, vec2.z, characterSprites[animatedSprites[index].characterIndex].unk_1C, characterSprites[animatedSprites[index].characterIndex].unk_1E)) {
                           
                            result |= temp;
                            
                            if (func_8004D380(CONTROLLER_1, arg3) || !arg3) {

                                if (!set) {

                                    animatedSprites[i].unk_58 = index;
                                    animatedSprites[i].unk_5A = arg3;

                                    if (!(animatedSprites[i].flags & 0x4000)) {
                                        animatedSprites[index].collision = i;    
                                    } else {
                                        animatedSprites[index].collision = 0xFFFF;
                                    }

                                    animatedSprites[i].unk_5E = 0xFFFF;
                                    set = TRUE;
                                    
                                    continue;
                                }
                            }

                            animatedSprites[i].unk_5E = index;
                            
                        } else {

                            animatedSprites[i].unk_5E = 0xFFFF;
                            animatedSprites[i].unk_58 = 0xFFFF;
                            animatedSprites[i].unk_5A = 0;
                            
                            if (!set) {
                                animatedSprites[index].collision = 0xFFFF;
                            }
                            
                        }       
                    }
                }
            }
        }
    }
    
    return result;
   
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800308E0);

bool func_800308E0(u16 spriteIndex, f32 arg1, f32 arg2) {
    
    bool result = FALSE; 

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if (animatedSprites[spriteIndex].flags & 1 && !(animatedSprites[spriteIndex].flags & 0x40)) {
            result = func_80036318(gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[spriteIndex].startingCoordinates.x + arg1, animatedSprites[spriteIndex].startingCoordinates.z + arg2);
        }

    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800309B4);

bool func_800309B4(u16 spriteIndex, f32 arg1, f32 arg2) {
    
    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    bool result = FALSE;

    if ((spriteIndex < MAX_ANIMATED_SPRITES)) {

        if ((animatedSprites[spriteIndex].flags & 1)) {
            
            if (!(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {

                vec1.x = arg1;
                vec1.y = 0;
                vec1.z = arg2;

                vec3.x = 0;
                vec3.y = getSpriteYValueFromDirection((animatedSprites[spriteIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
                vec3.z = 0;
                
                func_80027950(vec1, &vec2, vec3);

                result = func_80036318(gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[spriteIndex].startingCoordinates.x + vec2.x, animatedSprites[spriteIndex].startingCoordinates.z + vec2.z);
                
            }
        }
        
    } 
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030B24);

// unused
u16 func_80030B24(u16 spriteIndex, u16 offset) {

    u16 index = 0xFFFF;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[spriteIndex].flags & 1) {
            index = characterSprites[animatedSprites[spriteIndex].characterIndex].spriteData[offset] & 0x1FFF;
        }
    }
    
    return index;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030BA0);

u16 func_80030BA0(u16* vaddr, u16 offset) {
    return vaddr[offset] & 0x1FFF;
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030BB8);

// unused
u16 func_80030BB8(u16 spriteIndex, u16 offset) {

    u16 index = 0xFFFF;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[spriteIndex].flags & 1) {
            index = characterSprites[animatedSprites[spriteIndex].characterIndex].spriteData[offset] & 0x6000;
        }
    }
    
    return index;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030C34);

// unused
u16 func_80030C34(u16 spriteIndex, u16 offset) {

    u16 index = 0xFFFF;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        if (animatedSprites[spriteIndex].flags & 1) {
            index = characterSprites[animatedSprites[spriteIndex].characterIndex].spriteData[offset] & 0x8000;
        }
    }
    
    return index;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030CB0);

// unused
u16 func_80030CB0(u16 spriteIndex, f32 arg1, f32 arg2) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    
    Vec3f vec;

    u16 index = 0xFFFF;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {
        
        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {
            
            func_80030EAC(&vec, spriteIndex, arg1, arg2);

            if (vec.y != 65535.0f) {
                index = func_80036880(gMapModelContext[gMainMapIndex].mainMapIndex, vec.x, vec.z);
            }
            
        }
    }
    
    return index;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030DB0);

// unused
// map ground objects
bool func_80030DB0(u16 spriteIndex, f32 arg1, f32 arg2, u16 arg3) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    Vec3f vec;
    
    if ((spriteIndex < MAX_ANIMATED_SPRITES) && (animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {
            
        func_80030EAC(&vec, spriteIndex, arg1, arg2);
        
        if (vec.y != 65535.0f) {
            func_80036980(gMapModelContext[gMainMapIndex].mainMapIndex, arg3, vec.x, vec.z);
        }
        
    }
    
    return 0;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80030EAC);

Vec3f* func_80030EAC(Vec3f* arg0, u16 spriteIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            vec3.x = 0;
            vec3.y = getSpriteYValueFromDirection((animatedSprites[spriteIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
            vec3.z = 0;

            func_80027950(vec, &vec2, vec3);

            func_800366F4(&vec4, gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[spriteIndex].startingCoordinates.x + vec2.x, animatedSprites[spriteIndex].startingCoordinates.z + vec2.z);
            
        }
        
    }

    *arg0 = vec4;
    
    return arg0;
    
}


//INCLUDE_ASM(const s32, "system/animatedSprites", func_80031050);

u16 func_80031050(u16 spriteIndex, f32 arg1, f32 arg2) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    
    u16 index = 0xFFFF;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {

            func_800313FC(&vec4, spriteIndex, arg1, arg2);

            if (vec4.y != 65535.0f) {   
                index = func_80037254(gMapModelContext[gMainMapIndex].mainMapIndex, vec4.x, vec4.z);
            }
        }
    
    }

    return index;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800311E0);

// unused
bool func_800311E0(u16 spriteIndex, f32 arg1, f32 arg2, u16 arg3) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    
    Vec3f vec;
    
    bool set = FALSE;

     if (spriteIndex < MAX_ANIMATED_SPRITES) {
        
        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {

            func_800313FC(&vec, spriteIndex, arg1, arg2);

            if (vec.y != 65535.0f) {

                // set map additions
                func_80038A2C(gMapModelContext[gMainMapIndex].mainMapIndex, arg3, vec.x, vec.z);
                set = TRUE;

            }
            
        }
     }
    
    return set;
    
}

// get audio related flag from sprite
//INCLUDE_ASM(const s32, "system/animatedSprites", func_80031380);

bool func_80031380(u16 spriteIndex) {
    
    bool result = FALSE; 

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if (animatedSprites[spriteIndex].flags & 1 && !(animatedSprites[spriteIndex].flags & 0x40)) {
            result = globalSprites[animatedSprites[spriteIndex].globalSpriteIndex].unk_92;
        }

    }

    return result;

}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800313FC);

// convert sprite coordinates
Vec3f* func_800313FC(Vec3f* arg0, u16 spriteIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            vec3.x = 0;
            vec3.y = getSpriteYValueFromDirection((animatedSprites[spriteIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
            vec3.z = 0;

            func_80027950(vec, &vec2, vec3);

            func_80036610(&vec4, gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[spriteIndex].startingCoordinates.x + vec2.x, animatedSprites[spriteIndex].startingCoordinates.z + vec2.z);
            
        }
        
    }

    *arg0 = vec4;
    
    return arg0;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_800315A0);

Vec3f* func_800315A0(Vec3f* arg0, u16 index) {

    // FIXME: shouldn't be necessary
    u32 padding[11];
    
    Vec3f vec;

    vec.x = 0;
    vec.z = 0;
    vec.y = 65535.0f;
    
    if (index < MAX_ANIMATED_SPRITES) {
        if ((animatedSprites[index].flags & 1) && !(animatedSprites[index].flags & 0x40) && !(animatedSprites[index].flags & 0x100)) {
            func_80036610(&vec, gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[index].startingCoordinates.x, animatedSprites[index].startingCoordinates.z);
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
    
//     if (index < MAX_ANIMATED_SPRITES) {
//         if ((animatedSprites[index].flags & 1) && !(animatedSprites[index].flags & 0x40) && !(animatedSprites[index].flags & 0x100)) {
//             func_80036610(&vec, gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[index].startingCoordinates.x, animatedSprites[index].startingCoordinates.z);
//         } 
//     }
 
//     return vec;
// }

//INCLUDE_ASM(const s32, "system/animatedSprites", func_8003168C);

Vec3f* func_8003168C(Vec3f* arg0, u16 spriteIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (spriteIndex < MAX_ANIMATED_SPRITES) {

        if ((animatedSprites[spriteIndex].flags & 1) && !(animatedSprites[spriteIndex].flags & 0x40) && !(animatedSprites[spriteIndex].flags & 0x100)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            vec3.x = 0;
            vec3.y = getSpriteYValueFromDirection((animatedSprites[spriteIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
            vec3.z = 0;

            func_80027950(vec, &vec2, vec3);
            func_80036610(&vec4, gMapModelContext[gMainMapIndex].mainMapIndex, animatedSprites[spriteIndex].startingCoordinates.x, animatedSprites[spriteIndex].startingCoordinates.z);
            
            vec4.x += vec2.x;
            vec4.z += vec2.z;
        }
        
    }

    *arg0 = vec4;
    
    return arg0;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80031830);

bool func_80031830(u16 index, u32 arg1, u8 arg2) {

    Vec3f vec;
    
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    
    bool result = FALSE;
    u8 check1;
    u8 check2;
    
    temp3 = animatedSprites[index].startingCoordinates.y + 24.0f;
    temp4 = animatedSprites[index].startingCoordinates.y - 24.0f;
    
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
        result = TRUE;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/animatedSprites", func_80031904);

Vec3f* func_80031904(Vec3f* vec, u16 index, s16 arg2, u8 arg3) {

    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = arg2;

    vec3.x = 0;
    vec3.y = getSpriteYValueFromDirection(arg3 % 8);
    vec3.z = 0;

    // do 3D rotation
    func_80027950(vec1, &vec2, vec3);

    vec1.x = animatedSprites[index].startingCoordinates.x + vec2.x;
    vec1.y = animatedSprites[index].startingCoordinates.y + vec2.y;
    vec1.z = animatedSprites[index].startingCoordinates.z + vec2.z;

    *vec = vec1;
    
    return vec;
    
}

INCLUDE_ASM(const s32, "system/animatedSprites", func_80031A10);

INCLUDE_ASM(const s32, "system/animatedSprites", func_80031ED0);

INCLUDE_ASM(const s32, "system/animatedSprites", func_800321A8);

INCLUDE_ASM(const s32, "system/animatedSprites", func_80032330);

INCLUDE_ASM(const s32, "system/animatedSprites", func_800326C0);

// main loop function
INCLUDE_ASM(const s32, "system/animatedSprites", func_80033058);
