#include "common.h"

#include "system/entity.h"

#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapContext.h"

#include "mainproc.h"
#include "game/npc.h"

// bss
extern EntityAssetDescriptor entityAssetDescriptors[MAX_ENTITY_ASSETS];
extern Entity entities[MAX_ENTITIES];
extern ShadowSpriteDescriptor ShadowSpritesDescriptors[3];

// forward declarations
Vec3f* func_800313FC(Vec3f*, u16, f32, f32);  
u16 func_80031A10(Entity*, f32, f32, u8); 
u8 func_80031ED0(Entity*, u16, f32, f32, u16, u16);
void func_800326C0(u16);

#define convertEntityToMapDirection(direction, mapIndex) (direction + getCurrentMapRotation(mapIndex)) % 8  

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeEntities);

void initializeEntities(void) {
    
    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        entities[i].flags = 0;
        entities[i].globalSpriteIndex = 0;
        entities[i].animationIndices.animationIndex = 0;
        entities[i].animationIndices.nextAnimationIndex = 0xFFFF;
        entities[i].direction = 0;
        entities[i].unk_58 = 0xFFFF;
        entities[i].unk_5A = 0;
        entities[i].collision = 0xFFFF;
        entities[i].coordinates.x = 0;
        entities[i].coordinates.y = 0;
        entities[i].coordinates.z = 0;
        entities[i].unk_34.x = 0;
        entities[i].unk_34.y = 0;
        entities[i].unk_34.z = 0;
        entities[i].unk_4C = 0.0f;
        entities[i].unk_64 = 0;
        entities[i].paletteIndex = 0;
 
    }
    
    for (i = 0; i < MAX_ENTITIES; i++) {
        entityAssetDescriptors[i].collisionBufferX = 0;
        entityAssetDescriptors[i].collisionBufferY = 0;
        entityAssetDescriptors[i].unk_1C = 0;
        entityAssetDescriptors[i].unk_1E = 0;
        entityAssetDescriptors[i].flags = 0; 
    } 
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeEntityAsset);

bool initializeEntityAsset(u16 characterIndex, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, u8 arg7, u8 arg8, u16* arg9) {

    bool result = FALSE;

    if (characterIndex < MAX_ENTITY_ASSETS) {

        if (!(entityAssetDescriptors[characterIndex].flags & 1)) {
            
            entityAssetDescriptors[characterIndex].romTextureStart = arg1;
            entityAssetDescriptors[characterIndex].romTextureEnd = arg2;
            entityAssetDescriptors[characterIndex].romAssetIndexStart = arg3;
            entityAssetDescriptors[characterIndex].romAssetIndexEnd = arg4;
            entityAssetDescriptors[characterIndex].romSpritesheetIndexStart = arg5;
            entityAssetDescriptors[characterIndex].romSpritesheetIndexEnd = arg6;
            entityAssetDescriptors[characterIndex].shadowSpriteIndex= arg8;
            entityAssetDescriptors[characterIndex].spriteAnimationData = arg9;

            entityAssetDescriptors[characterIndex].collisionBufferX = 0;
            entityAssetDescriptors[characterIndex].collisionBufferY = 0;

            entityAssetDescriptors[characterIndex].unk_1C = 0;
            entityAssetDescriptors[characterIndex].unk_1E = 0;

            entityAssetDescriptors[characterIndex].flags = 1;

            if (arg7) {
                entityAssetDescriptors[characterIndex].flags = 3;
            }
            
            result = TRUE;
            
        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeSpriteInstance);

bool initializeEntity(u16 entityIndex, u16 globalSpriteIndex, u16 shadowSpriteIndex, void* vaddrTexture1, void* vaddrTexture2, 
    void* vaddrPalette, void* vaddrUnknownAsset, void* vaddrTextureToPaletteLookup, void* vaddrSpritesheetIndex) {

    bool set = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if (!(entities[entityIndex].flags & ENTITY_ACTIVE)) {

            entities[entityIndex].flags = 1;

            entities[entityIndex].globalSpriteIndex = globalSpriteIndex;
            entities[entityIndex].shadowSpriteIndex = shadowSpriteIndex;

            entities[entityIndex].vaddrTexture1 = vaddrTexture1;
            entities[entityIndex].vaddrTexture2 = vaddrTexture2;
            entities[entityIndex].vaddrPalette = vaddrPalette;
            entities[entityIndex].vaddrUnknownAsset = vaddrUnknownAsset;
            entities[entityIndex].vaddrSpritesheetIndex = vaddrSpritesheetIndex;
            entities[entityIndex].vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

            entities[entityIndex].coordinates.x = 0;
            entities[entityIndex].coordinates.y = 0;
            entities[entityIndex].coordinates.z = 0;

            entities[entityIndex].unk_34.x = 0;
            entities[entityIndex].unk_34.y = 0;
            entities[entityIndex].unk_34.z = 0;

            entities[entityIndex].unk_4C = 0;

            entities[entityIndex].unk_58 = 0xFFFF;

            entities[entityIndex].unk_64 = 0;
        
            set = TRUE;
            
        }
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeAnimalEntity);

bool initializeAnimalEntity(u16 index, void* arg1, void* arg2, void* arg3, void* arg4) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        result = TRUE;
        
        entities[index].vaddrSpritesheetIndex = arg4;
        
        entities[index].flags = 1;
        
        entities[index].vaddrPalette = arg1;
        entities[index].vaddrUnknownAsset = arg2;
        entities[index].vaddrTextureToPaletteLookup = arg3;

        entities[index].coordinates.x = 0;
        entities[index].coordinates.y = 0;
        entities[index].coordinates.z = 0;

        entities[index].unk_34.x = 0;
        entities[index].unk_34.y = 0;
        entities[index].unk_34.z = 0;

        entities[index].unk_4C = 0.0f;
        entities[index].unk_58 = 0xFFFF;
        entities[index].unk_64 = 0;
        
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002E108);

// unused; deactivate sprite without checking flags
bool func_8002E108(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        entities[index].flags = 0;

        deactivateSprite(entities[index].globalSpriteIndex);
        
        if (entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex != 0xFF) {
            deactivateSprite(entities[index].shadowSpriteIndex);
        }
        
        result = TRUE;
    
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", deactivateSprite);
 
void deactivateSprites(void) {

    u16 i = 0;
    u16 temp;

    while (i < MAX_ENTITIES) {

        temp = i;

        if (temp < MAX_ENTITIES) {

            entities[i].flags = 0;
        
            deactivateSprite(entities[i].globalSpriteIndex);
            
            // toggle flag on shadow sprite
            if (entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(entities[i].shadowSpriteIndex);
            } 
        }
        
        i++;
      
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", loadEntity);

bool loadEntity(u16 index, u16 characterIndex, u8 flag) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        if (entities[index].flags & ENTITY_ACTIVE) {

            entities[index].characterIndex = characterIndex;
            entities[index].flag = flag;

            if (entityAssetDescriptors[entities[index].characterIndex].flags & 2) {

                dmaSprite(entities[index].globalSpriteIndex, 
                    entityAssetDescriptors[entities[index].characterIndex].romTextureStart, 
                    entityAssetDescriptors[entities[index].characterIndex].romTextureEnd, 
                    entityAssetDescriptors[entities[index].characterIndex].romAssetIndexStart, 
                    entityAssetDescriptors[entities[index].characterIndex].romAssetIndexEnd, 
                    entityAssetDescriptors[entities[index].characterIndex].romSpritesheetIndexStart, 
                    entityAssetDescriptors[entities[index].characterIndex].romSpritesheetIndexEnd, 
                    entities[index].vaddrTexture1,
                    entities[index].vaddrTexture2,
                    entities[index].vaddrPalette,
                    entities[index].vaddrUnknownAsset,
                    entities[index].vaddrTextureToPaletteLookup,
                    entities[index].vaddrSpritesheetIndex,
                    1,
                    entities[index].flag                    
                    );
                
            } else {
                
                dmaSprite(entities[index].globalSpriteIndex, 
                    entityAssetDescriptors[entities[index].characterIndex].romTextureStart, 
                    entityAssetDescriptors[entities[index].characterIndex].romTextureEnd, 
                    entityAssetDescriptors[entities[index].characterIndex].romAssetIndexStart, 
                    entityAssetDescriptors[entities[index].characterIndex].romAssetIndexEnd, 
                    entityAssetDescriptors[entities[index].characterIndex].romSpritesheetIndexStart, 
                    entityAssetDescriptors[entities[index].characterIndex].romSpritesheetIndexEnd, 
                    entities[index].vaddrTexture1,
                    0,
                    entities[index].vaddrPalette,
                    entities[index].vaddrUnknownAsset,
                    entities[index].vaddrTextureToPaletteLookup,
                    0,
                    entityAssetDescriptors[entities[index].characterIndex].flags & 2 == 1,
                    entities[index].flag                    
                    );
                
            }

            if (entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex != 0xFF) {
                
                dmaSprite(entities[index].shadowSpriteIndex, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].romTextureStart, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].romTextureEnd, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].romAssetIndexStart, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].romAssetIndexEnd, 
                    0, 
                    0, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].vaddrSpritesheet, 
                    0, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].vaddrPalette, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].vaddrUnknownAssetSheet, 
                    ShadowSpritesDescriptors[entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex].vaddrUnknownAsset2, 
                    0, 
                    0, 
                    entities[index].flag
                    );

                setSpriteRenderingLayer(entities[index].shadowSpriteIndex, 2);
            
            }

            entities[index].animationIndices.nextAnimationIndex = 0xFFFF;
            entities[index].flags |= ENTITY_INITIALIZED;

            setBilinearFiltering(entities[index].globalSpriteIndex, TRUE);
            func_8002FE10(index, 0.0f, 0.0f, 0.0f, 0.0f);
            setEntityColor(index, D_802373F8.r, D_802373F8.g, D_802373F8.b, D_802373F8.a);

            if (!(checkMapRGBADone(gMapModelContext[gMainMapIndex].mainMapIndex))) {
 
                updateEntityRGBA(index, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.r, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.g, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.b, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapFloats.defaultRgba.a, 
                    mainMap[gMapModelContext[gMainMapIndex].mainMapIndex].mapState.unk_8);
                
            }
            
            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeShadowSprite);

bool initializeShadowSprite(u16 index, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32* arg5, u16* arg6, u16* arg7, u16* arg8, u16 arg9, u8 argA) {

    bool result = FALSE;
    
    if (index < MAX_SHADOW_SPRITES) {
        result = TRUE;
        ShadowSpritesDescriptors[index].romTextureStart = arg1;
        ShadowSpritesDescriptors[index].romTextureEnd = arg2;
        ShadowSpritesDescriptors[index].romAssetIndexStart = arg3;
        ShadowSpritesDescriptors[index].romAssetIndexEnd = arg4;
        ShadowSpritesDescriptors[index].vaddrSpritesheet = arg5;
        ShadowSpritesDescriptors[index].vaddrPalette = arg6;
        ShadowSpritesDescriptors[index].vaddrUnknownAssetSheet = arg7;
        ShadowSpritesDescriptors[index].vaddrUnknownAsset2 = arg8;
        ShadowSpritesDescriptors[index].unk_20 = arg9;
        ShadowSpritesDescriptors[index].unk_22 = argA;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityTrackingTarget);

bool setEntityTrackingTarget(u16 index, u16 targetSpriteIndex, u16 trackingMode) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {

            entities[index].targetSpriteIndex = targetSpriteIndex;
            entities[index].trackingMode = trackingMode;

            if (targetSpriteIndex == 0xFFFF) {
                entities[index].flags &= ~ENTITY_TRACKING_ACTIVE;
            } else {
                entities[index].flags |= ENTITY_TRACKING_ACTIVE;
                entities[index].flags &= ~ENTITY_PAUSED;
            }

            result = TRUE;
        }
    }
     
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityPaletteIndex);

// likely set a palette index if asset has multiple palettes (animals, itemHandlers, and cutscenes)
bool setEntityPaletteIndex(u16 index, s16 paletteIndex) {
    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if (entities[index].flags & ENTITY_ACTIVE) {
            result = TRUE;
            entities[index].paletteIndex = paletteIndex;
            entities[index].flags |= ENTITY_PALETTE_SET;
        }
    }
    
    return result;
}
 
//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002EDF0);

// used by animals, npcs, item handlers, and cutscenes
bool func_8002EDF0(u16 index, s16 arg1, s16 arg2, s16 arg3) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            result = TRUE;
            entities[index].unk_18.x = arg1;
            entities[index].unk_18.y = arg2;
            entities[index].unk_18.z = arg3;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setMainMapIndex);

bool setMainMapIndex(u16 mapIndex) {

    bool result = FALSE;

    if ((mapIndex == MAIN_MAP_INDEX) && (gMapModelContext[mapIndex].flags & 1) && (gMapModelContext[mapIndex].flags & 2)) {
        gMainMapIndex = mapIndex;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002EEE4);

bool func_8002EEE4(u16 entityIndex, f32 x, f32 y, f32 z) {

    bool set = FALSE;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && (entities[entityIndex].flags & ENTITY_INITIALIZED)) {
            setSpriteShrinkFactor(entities[entityIndex].globalSpriteIndex, x, y, z);
            set = TRUE;
        }
        
    }
    
    return set;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002EF7C);

bool func_8002EF7C(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            adjustSpriteShrinkFactor(entities[index].globalSpriteIndex, x, y, z);
            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityColor);

bool setEntityColor(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            
            setSpriteColor(entities[index].globalSpriteIndex, r, g, b, a);
            
            if (entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex != 0xFF) {

                setSpriteColor(entities[index].shadowSpriteIndex, r, g, b, SHADOW_ALPHA);
                
            }

            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityShadow);

bool setEntityShadow(u16 index, u8 shadowSpriteIndex) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            
            entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex = shadowSpriteIndex;

            if (entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex == 0xFF) {

                resetAnimationState(entities[index].shadowSpriteIndex);

            }

            result = TRUE;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", updateEntityRGBA);

bool updateEntityRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {

            updateSpriteRGBA(entities[index].globalSpriteIndex, r, g, b, a, rate);

            if (entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex != 0xFF) {
                updateSpriteRGBA(entities[index].shadowSpriteIndex, r, g, b, SHADOW_ALPHA, rate);
            }
            
            result = TRUE;
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityAnimation);

bool setEntityAnimation(u16 index, u16 animationIndex) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && !(entities[index].flags & 0x1000)) {

            entities[index].animationIndices.animationIndex = animationIndex;
            
            entities[index].flags |= 8;
            entities[index].flags &= ~ENTITY_ANIMATION_DIRECTION_CHANGE; 

            globalSprites[entities[index].globalSpriteIndex].stateFlags &= ~ANIMATION_STATE_CHANGED;
            globalSprites[entities[index].globalSpriteIndex].audioTrigger = FALSE;
            
            result = TRUE;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityAnimationWithDirectionChange);

bool setEntityAnimationWithDirectionChange(u16 index, u16 animationIndex) {
    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && !(entities[index].flags & 0x1000)) {     
        
            entities[index].animationIndices.animationIndex = animationIndex;
            entities[index].flags |= (8 | ENTITY_ANIMATION_DIRECTION_CHANGE);
    
            globalSprites[entities[index].globalSpriteIndex].stateFlags &= ~ANIMATION_STATE_CHANGED;
    
            result = TRUE;
    
            globalSprites[entities[index].globalSpriteIndex].audioTrigger = FALSE;
    
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002F4E0);

bool func_8002F4E0(u16 index) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && (entities[index].flags & 0x1000)) {
            
            loadEntity(index, entities[index].characterIndex, entities[index].flag);

            entities[index].flags |= 8;
            entities[index].flags &= ~0x1000;

            result = TRUE;
        } 
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002F594);

bool func_8002F594(u16 index) {

    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            
            if (entities[index].flags & 8) {

                func_8002FE10(index, 0.0f, 0.0f, 0.0f, 0.0f);
                
                temp = entities[index].flags & 0x2000;
                entities[index].flags |= 0x1000;

                deactivateEntity(index);
                
                entities[index].flags |= temp;
                
                result = TRUE;
        
            } else {
                entities[index].flags &= ~0x1000;
            }

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityDirection);

bool setEntityDirection(u16 index, u8 direction) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && !(entities[index].flags & 0x1000)) {
        
            entities[index].direction = direction;
            result = TRUE;

        } 
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", pauseEntities);

void pauseEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        pauseEntity(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", togglePauseEntities);

void togglePauseEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        func_8002FC38(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002F770);

void func_8002F770(s16 arg0) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        entities[i].direction += arg0;
        entities[i].direction &= MAX_DIRECTIONS;
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntitiesColor);

void setEntitiesColor(u8 r, u8 g, u8 b, u8 a) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {

        if (entities[i].flags & ENTITY_ACTIVE && entities[i].flags & 8) {

            if (i < MAX_ENTITIES && entities[i].flags & ENTITY_INITIALIZED) {

                setSpriteColor(entities[i].globalSpriteIndex, r, g, b, a);

                if (entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    setSpriteColor(entities[i].shadowSpriteIndex, r, g, b, SHADOW_ALPHA);
                }

            }
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", updateEntitiesColor);

void updateEntitiesColor(u8 r, u8 g, u8 b, u8 a, s16 arg4) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {

        if ((entities[i].flags & ENTITY_ACTIVE) && (entities[i].flags & 8)) {
        
            if (i < MAX_ENTITIES && entities[i].flags & ENTITY_INITIALIZED) {
        
                updateSpriteRGBA(entities[i].globalSpriteIndex, r, g, b, a, arg4);
        
                if (entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    updateSpriteRGBA(entities[i].shadowSpriteIndex, r, g, b, SHADOW_ALPHA, arg4);
                }
        
            }
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", deactivateEntity);

bool deactivateEntity(u16 index) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & 8)) {
            
            entities[index].flags &= ~( 0x8 | 0x2000);
            deactivateSprite(entities[index].globalSpriteIndex);
            
            if (entityAssetDescriptors[entities[index].characterIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(entities[index].shadowSpriteIndex);
            }

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", deactivateEntities);

// unused
void deactivateEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        deactivateEntity(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FB3C);

void func_8002FB3C(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        func_8002F594(i);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FB7C);

void func_8002FB7C(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        func_8002F4E0(i);
    }

}


//INCLUDE_ASM("asm/nonmatchings/system/entity", pauseEntity);

bool pauseEntity(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {

        if (entities[index].flags & ENTITY_ACTIVE) {
            
            entities[index].flags |= ENTITY_PAUSED;
            func_8002BB88(entities[index].globalSpriteIndex);
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FC38);

bool func_8002FC38(u16 index) {
    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if (entities[index].flags & ENTITY_ACTIVE) {
        
            entities[index].flags &= ~ENTITY_PAUSED;
            func_8002BB30(entities[index].globalSpriteIndex);
        
            result = TRUE;
        
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FCB4);

bool func_8002FCB4(u16 index, u8 flag) {
    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & 8)) {

            if (flag) {
                entities[index].flags &= ~0x100;
            } else {
                entities[index].flags |= 0x100;
            }

            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FD24);

// only used by main game loop callback
// check visible but inactive?
bool func_8002FD24(u16 index) {
    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & 8)) {
            temp = entities[index].flags & 0x100;
            result = !temp;
        }

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntitycoordinates);

bool setEntityCoordinates(u16 entityIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & 0x1000)) {
            
            entities[entityIndex].coordinates.x = x;
            entities[entityIndex].coordinates.y = y;
            entities[entityIndex].coordinates.z = z;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FE10);

// animals, player, item handlers, cutscene
// likely stores saved position for objects and animals
bool func_8002FE10(u16 entityIndex, f32 x, f32 y, f32 z, f32 arg4) {

    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && (entities[entityIndex].flags & 8) && !(entities[entityIndex].flags & 0x1000)) {
            
            entities[entityIndex].unk_34.x = x;
            entities[entityIndex].unk_34.y = y;
            entities[entityIndex].unk_34.z = z;
            entities[entityIndex].unk_4C = arg4 / 2.0f;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityAnimationStateChanged);

bool checkEntityAnimationStateChanged(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {

        if (entities[index].flags & ENTITY_ACTIVE) {
            result = checkSpriteAnimationStateChanged(entities[index].globalSpriteIndex);
        }

    }

    return result;

}

//sINCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FF38);

bool func_8002FF38(u16 entityIndex, u16 flag) {

    bool result = FALSE;

    if (entityIndex < MAX_ENTITIES) {

        if (entities[entityIndex].flags & ENTITY_ACTIVE) { 

            if (flag) {
                entities[entityIndex].flags |= 0x20;
            } else {
                entities[entityIndex].flags &= ~0x20;
            }

            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityCollisionBuffers);

bool setEntityCollisionBuffers(u16 entityIndex, u8 xValue, u8 yValue) {
    
    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITY_ASSETS) {
    
        if (entityAssetDescriptors[entityIndex].flags & 1) {
            entityAssetDescriptors[entityIndex].collisionBufferX = xValue;
            entityAssetDescriptors[entityIndex].collisionBufferY = yValue;
            result = TRUE;
        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8002FFF4);

bool func_8002FFF4(u16 characterIndex, u8 arg1, u8 arg2) {

    bool result = FALSE;

    if (characterIndex < MAX_ENTITY_ASSETS) {

        if (entityAssetDescriptors[characterIndex].flags & 1) {
            entityAssetDescriptors[characterIndex].unk_1C = arg1;
            entityAssetDescriptors[characterIndex].unk_1E = arg2;
            result = TRUE;
        }

    }

    return result;
    

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030054);

bool func_80030054(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        entities[index].flags &= ~0x80;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= 0x80;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800300F8);

bool func_800300F8(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        entities[index].flags &= ~0x8000;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= 0x8000;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8003019C);

bool func_8003019C(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        entities[index].flags &= ~0x400;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= 0x400;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030240);

bool func_80030240(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        entities[index].flags &= ~0x4000;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= 0x4000;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800302E4);

bool func_800302E4(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == 1) {
        entities[index].flags &= ~2;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= 2;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030388);

bool func_80030388(u16 index) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if (entities[index].flags & ENTITY_ACTIVE) {
            result = (entities[index].flags >> 6) & 1;
        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800303D4);

u16 func_800303D4(u16 entityIndex, f32 arg1, f32 arg2, u16 arg3, u16 arg4) {

    // FIXME: shouldn't be necessary
    u32 padding[4];
    
    u16 i;
    u16 index = 0;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED)) {

            for (i = 0; i < arg4; i++) {
                
                if (i != entityIndex) {
                    
                    if (!(entities[i].flags & 0x4000)) {

                        index = func_80031ED0(&entities[entityIndex], i, arg1, arg2, entityAssetDescriptors[entities[entityIndex].characterIndex].unk_1C, entityAssetDescriptors[entities[entityIndex].characterIndex].unk_1E);
                        
                        if (index) {

                            entities[i].unk_58 = entityIndex;
                            entities[i].unk_5A = arg3;
                            entities[entityIndex].collision = i;
                            
                            i = MAX_ENTITIES;
                            
                        } else {

                            entities[i].unk_58 = 0xFFFF;
                            entities[i].unk_5A = 0;
                            entities[entityIndex].collision = 0xFFFF;
                            
                        }
                    }
                }
            }   
        }
    }
    
    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800305CC);

u16 func_800305CC(u16 index, f32 arg1, f32 arg2, u16 arg3) {

    Vec3f vec1;
    Vec3f vec2;
    Vec3f rotationAngles;
    
    u16 result = 0;
    u32 temp;
    u16 i = 0;
    bool set = FALSE;

    if (index < MAX_ENTITIES) {
        
        if (entities[index].flags & ENTITY_ACTIVE) {
            
            if (!(entities[index].flags & ENTITY_PAUSED)) {
                
                vec1.x = arg1;
                vec1.y = 0;
                vec1.z = arg2;

                rotationAngles.x = 0;
                rotationAngles.y = getSpriteYValueFromDirection((entities[index].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
                rotationAngles.z = 0;

                rotateVector3D(vec1, &vec2, rotationAngles);

                for (i = 0; i < MAX_ENTITIES; i++) {

                    if (i != index && !(entities[i].flags & ENTITY_TRACKING_ACTIVE)) {
                        
                        if (temp = func_80031ED0(&entities[index], i, vec2.x, vec2.z, entityAssetDescriptors[entities[index].characterIndex].unk_1C, entityAssetDescriptors[entities[index].characterIndex].unk_1E)) {
                           
                            result |= temp;
                            
                            if (checkButtonPressed(CONTROLLER_1, arg3) || !arg3) {

                                if (!set) {

                                    entities[i].unk_58 = index;
                                    entities[i].unk_5A = arg3;

                                    if (!(entities[i].flags & 0x4000)) {
                                        entities[index].collision = i;    
                                    } else {
                                        entities[index].collision = 0xFFFF;
                                    }

                                    entities[i].unk_5E = 0xFFFF;
                                    set = TRUE;
                                    
                                    continue;
                                }
                            }

                            entities[i].unk_5E = index;
                            
                        } else {

                            entities[i].unk_5E = 0xFFFF;
                            entities[i].unk_58 = 0xFFFF;
                            entities[i].unk_5A = 0;
                            
                            if (!set) {
                                entities[index].collision = 0xFFFF;
                            }
                            
                        }       
                    }
                }
            }
        }
    }
    
    return result;
   
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800308E0);

bool func_800308E0(u16 entityIndex, f32 arg1, f32 arg2) {
    
    bool result = FALSE; 

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED)) {
            result = func_80036318(gMapModelContext[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + arg1, entities[entityIndex].coordinates.z + arg2);
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800309B4);

u8 func_800309B4(u16 entityIndex, f32 arg1, f32 arg2) {
    
    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    u8 result = 0;

    if ((entityIndex < MAX_ENTITIES)) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE)) {
            
            if (!(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {

                vec1.x = arg1;
                vec1.y = 0;
                vec1.z = arg2;

                vec3.x = 0;
                vec3.y = getSpriteYValueFromDirection((entities[entityIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
                vec3.z = 0;
                
                rotateVector3D(vec1, &vec2, vec3);

                result = func_80036318(gMapModelContext[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + vec2.x, entities[entityIndex].coordinates.z + vec2.z);
                
            }
        }
        
    } 
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getSpriteAnimationOffset);

// returns param 2 of startSpriteAnimation
inline u16 getSpriteAnimationOffset(u16 entityIndex, u16 offset) {

    u16 animationDataOffset = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (entities[entityIndex].flags & ENTITY_ACTIVE) {
            animationDataOffset = entityAssetDescriptors[entities[entityIndex].characterIndex].spriteAnimationData[offset] & 0x1FFF;
        }
    }
    
    return animationDataOffset;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030BA0);

u16 func_80030BA0(u16* vaddr, u16 offset) {
    return vaddr[offset] & 0x1FFF;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getSpriteAnimationType);

inline u16 getSpriteAnimationType(u16 entityIndex, u16 offset) {

    u16 flags = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (entities[entityIndex].flags & ENTITY_ACTIVE) {
            flags = entityAssetDescriptors[entities[entityIndex].characterIndex].spriteAnimationData[offset] & 0x6000;
        }
    }
    
    return flags;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getSpriteAnimationHorizontalFlip);

inline u16 getSpriteAnimationHorizontalFlip(u16 entityIndex, u16 offset) {

    u16 flipHorizontal = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (entities[entityIndex].flags & ENTITY_ACTIVE) {
            flipHorizontal = entityAssetDescriptors[entities[entityIndex].characterIndex].spriteAnimationData[offset] & 0x8000;
        }
    }
    
    return flipHorizontal;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030CB0);

// unused or inline
u16 func_80030CB0(u16 entityIndex, f32 arg1, f32 arg2) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    
    Vec3f vec;

    u16 index = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        
        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {
            
            func_80030EAC(&vec, entityIndex, arg1, arg2);

            if (vec.y != 65535.0f) {
                index = func_80036880(gMapModelContext[gMainMapIndex].mainMapIndex, vec.x, vec.z);
            }
            
        }
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030DB0);

// unused or inline
// map ground objects
bool func_80030DB0(u16 entityIndex, f32 arg1, f32 arg2, u16 arg3) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    Vec3f vec;
    
    if ((entityIndex < MAX_ENTITIES) && (entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {
            
        func_80030EAC(&vec, entityIndex, arg1, arg2);
        
        if (vec.y != 65535.0f) {
            func_80036980(gMapModelContext[gMainMapIndex].mainMapIndex, arg3, vec.x, vec.z);
        }
        
    }
    
    return 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80030EAC);

Vec3f* func_80030EAC(Vec3f* arg0, u16 entityIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            vec3.x = 0;
            vec3.y = getSpriteYValueFromDirection((entities[entityIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
            vec3.z = 0;

            rotateVector3D(vec, &vec2, vec3);

            func_800366F4(&vec4, gMapModelContext[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + vec2.x, entities[entityIndex].coordinates.z + vec2.z);
            
        }
        
    }

    *arg0 = vec4;
    
    return arg0;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80031050);

u16 func_80031050(u16 entityIndex, f32 arg1, f32 arg2) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    
    u16 index = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {

            func_800313FC(&vec4, entityIndex, arg1, arg2);

            if (vec4.y != 65535.0f) {   
                index = func_80037254(gMapModelContext[gMainMapIndex].mainMapIndex, vec4.x, vec4.z);
            }
        }
    
    }

    return index;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800311E0);

// unused or inline
bool func_800311E0(u16 entityIndex, f32 arg1, f32 arg2, u16 arg3) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    
    Vec3f vec;
    
    bool set = FALSE;

     if (entityIndex < MAX_ENTITIES) {
        
        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {

            func_800313FC(&vec, entityIndex, arg1, arg2);

            if (vec.y != 65535.0f) {

                // set map additions
                func_80038A2C(gMapModelContext[gMainMapIndex].mainMapIndex, arg3, vec.x, vec.z);
                set = TRUE;

            }
            
        }
     }
    
    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityShouldPlaySoundEffect);

bool checkEntityShouldPlaySoundEffect(u16 entityIndex) {
    
    bool result = FALSE; 

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED)) {
            result = globalSprites[entities[entityIndex].globalSpriteIndex].audioTrigger;
        }

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800313FC);

// convert sprite coordinates
Vec3f* func_800313FC(Vec3f* arg0, u16 entityIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            vec3.x = 0;
            vec3.y = getSpriteYValueFromDirection((entities[entityIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
            vec3.z = 0;

            rotateVector3D(vec, &vec2, vec3);

            func_80036610(&vec4, gMapModelContext[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + vec2.x, entities[entityIndex].coordinates.z + vec2.z);
            
        }
        
    }

    *arg0 = vec4;
    
    return arg0;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_800315A0);

Vec3f* func_800315A0(Vec3f* arg0, u16 index) {

    // FIXME: shouldn't be necessary
    u32 padding[11];
    
    Vec3f vec;

    vec.x = 0;
    vec.z = 0;
    vec.y = 65535.0f;
    
    if (index < MAX_ENTITIES) {
        if ((entities[index].flags & ENTITY_ACTIVE) && !(entities[index].flags & ENTITY_PAUSED) && !(entities[index].flags & 0x100)) {
            func_80036610(&vec, gMapModelContext[gMainMapIndex].mainMapIndex, entities[index].coordinates.x, entities[index].coordinates.z);
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
    
//     if (index < MAX_ENTITIES) {
//         if ((entities[index].flags & ENTITY_ACTIVE) && !(entities[index].flags & ENTITY_PAUSED) && !(entities[index].flags & 0x100)) {
//             func_80036610(&vec, gMapModelContext[gMainMapIndex].mainMapIndex, entities[index].coordinates.x, entities[index].coordinates.z);
//         } 
//     }
 
//     return vec;
// }

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_8003168C);

Vec3f* func_8003168C(Vec3f* arg0, u16 entityIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & 0x100)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            vec3.x = 0;
            vec3.y = getSpriteYValueFromDirection((entities[entityIndex].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
            vec3.z = 0;

            rotateVector3D(vec, &vec2, vec3);
            func_80036610(&vec4, gMapModelContext[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x, entities[entityIndex].coordinates.z);
            
            vec4.x += vec2.x;
            vec4.z += vec2.z;
        }
        
    }

    *arg0 = vec4;
    
    return arg0;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80031830);

bool func_80031830(u16 index, s16 z, u8 direction) {

    Vec3f vec;
    
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    
    bool result = FALSE;
    u8 check1;
    u8 check2;
    
    temp3 = entities[index].coordinates.y + 24.0f;
    temp4 = entities[index].coordinates.y - 24.0f;
    
    vec = func_80031904(index, z, direction);
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

//INCLUDE_ASM("asm/nonmatchings/system/entity", func_80031904);

Vec3f func_80031904(u16 index, s16 arg2, u8 arg3) {

    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = arg2;

    vec3.x = 0;
    vec3.y = getSpriteYValueFromDirection(arg3 % 8);
    vec3.z = 0;

    rotateVector3D(vec1, &vec2, vec3);

    vec1.x = entities[index].coordinates.x + vec2.x;
    vec1.y = entities[index].coordinates.y + vec2.y;
    vec1.z = entities[index].coordinates.z + vec2.z;

    return vec1;
    
}

INCLUDE_ASM("asm/nonmatchings/system/entity", func_80031A10);

INCLUDE_ASM("asm/nonmatchings/system/entity", func_80031ED0);

#ifdef PERMUTER
void func_800321A8(u16 index) {

    Vec3f vec;

    if (func_80031A10(&entities[index], entities[index].unk_34.x, entities[index].unk_34.z, convertEntityToMapDirection(entities[index].direction, gMainMapIndex))) {

        vec = getMovementVectorFromDirection(entities[index].unk_4C, convertEntityToMapDirection(entities[index].direction, gMainMapIndex), 0);

        if (!(func_80031A10(&entities[index], vec.x, vec.z, convertEntityToMapDirection(entities[index].direction, gMainMapIndex)))) {
            entities[index].unk_34.x = vec.x;
            entities[index].unk_34.z = vec.z;
        }
        
    }
    
}
#else
INCLUDE_ASM("asm/nonmatchings/system/entity", func_800321A8);
#endif

INCLUDE_ASM("asm/nonmatchings/system/entity", func_80032330);

INCLUDE_ASM("asm/nonmatchings/system/entity", func_800326C0);

//INCLUDE_ASM("asm/nonmatchings/system/entity", updateEntities);

void updateEntities(void) {

    Vec3f vec;
    
    u16 i = 0;
    
    u8 directionalOffset;
    u16 animationDataOffset;
    u16 animationType;
    u16 flipHorizontal;

    while (i < MAX_ENTITIES) {

        if ((entities[i].flags & ENTITY_ACTIVE) && (entities[i].flags & 8)) {
            
            entities[i].unk_66 = 0;

            if (!(entities[i].flags & 0x40) || (entities[i].flags & 0x200)) {
                
                func_800326C0(i);

                 if (entities[i].flags & 0x200) {
                    if (entities[i].trackingMode == 0xFE) {
                        entities[i].direction = entities[entities[i].targetSpriteIndex].direction;
                    }
                }
                
            }

            if (entities[i].flags & ENTITY_ANIMATION_DIRECTION_CHANGE) {
                directionalOffset = entities[i].direction;
            } else {
                directionalOffset = 0;
            }

            animationDataOffset = getSpriteAnimationOffset(i, entities[i].animationIndices.animationIndex + directionalOffset);
            animationType = getSpriteAnimationType(i, entities[i].animationIndices.animationIndex + directionalOffset);
            flipHorizontal = getSpriteAnimationHorizontalFlip(i, entities[i].animationIndices.animationIndex + directionalOffset);

            vec = func_800359C8(&mainMap[MAIN_MAP_INDEX], 
                entities[i].coordinates.x + mainMap[MAIN_MAP_INDEX].mapStruct1.scalingFactorX / 2,
                entities[i].coordinates.z + mainMap[MAIN_MAP_INDEX].mapStruct1.scalingFactorZ / 2);
            
            if (mainMap[MAIN_MAP_INDEX].visibilityGrid[(u8)vec.z][(u8)vec.x] || entities[i].flags & 0x20) {

                if ((directionalOffset + entities[i].animationIndices.animationIndex) != entities[i].animationIndices.nextAnimationIndex) {
                    
                    resetAnimationState(entities[i].globalSpriteIndex);

                    switch (animationType) {

                        case 0:
                            startSpriteAnimation(entities[i].globalSpriteIndex, animationDataOffset, 0xFF);
                            break;

                        case 0x2000: 
                            startSpriteAnimation(entities[i].globalSpriteIndex, animationDataOffset, 0xFE);
                            break;
                        
                        case 0x4000:
                            startSpriteAnimation(entities[i].globalSpriteIndex, animationDataOffset, 0xFD);
                            break;

                        default:
                            break;
                        
                    }

                    if (entities[i].flags & ENTITY_PALETTE_SET) {
                        setSpritePaletteIndex(entities[i].globalSpriteIndex, entities[i].paletteIndex);
                    }

                    if (flipHorizontal) {
                        setSpriteFlip(entities[i].globalSpriteIndex, TRUE, FALSE);
                    } else {
                        setSpriteFlip(entities[i].globalSpriteIndex, FALSE, FALSE);
                    }
                    
                    entities[i].animationIndices.nextAnimationIndex = entities[i].animationIndices.animationIndex + directionalOffset;
                          
                }

                if (entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex != 0xFF) {

                    if (globalSprites[entities[i].shadowSpriteIndex].shrink.y <= entities[i].unk_3C.y) {

                        startSpriteAnimation(entities[i].shadowSpriteIndex, 
                             ShadowSpritesDescriptors[entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex].unk_20,
                             ShadowSpritesDescriptors[entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex].unk_22);
                                             
                    } else {
                        resetAnimationState(entities[i].shadowSpriteIndex);
                    }
                    
                } else {
                    resetAnimationState(entities[i].shadowSpriteIndex);
                }

                if ((animationType == 0x4000) && checkSpriteAnimationStateChanged(entities[i].globalSpriteIndex)) {
                    deactivateEntity(i);                   
                }
                
            } else {

                entities[i].animationIndices.nextAnimationIndex = 0xFFFF;
                resetAnimationState(entities[i].globalSpriteIndex);

                if (entityAssetDescriptors[entities[i].characterIndex].shadowSpriteIndex != 0xFF) {
                    resetAnimationState(entities[i].shadowSpriteIndex);
                }
                    
            }
             
        }

        i++;
        
    } 
    
}
