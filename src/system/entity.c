#include "common.h"

#include "system/entity.h"

#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"

#include "mainproc.h"
#include "game/npc.h"

// bss
EntityAssetDescriptor entityAssetDescriptors[MAX_ENTITY_ASSETS];
Entity entities[MAX_ENTITIES];
ShadowSpriteDescriptor shadowSpriteDescriptors[3];

// forward declarations
Vec3f getEntityRelativeTilePosition(u16, f32, f32);  
u16 checkTerrainMovementCollision(Entity*, f32, f32, u8); 
u8 detectEntityOverlap(Entity*, u16, f32, f32, u16, u16);
void updateEntityPhysics(u16);

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeEntities);

void initializeEntities(void) {
    
    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        entities[i].flags = 0;
        entities[i].globalSpriteIndex = 0;
        entities[i].animationIndices.animationIndex = 0;
        entities[i].animationIndices.nextAnimationIndex = 0xFFFF;
        entities[i].direction = 0;
        entities[i].entityCollidedWithIndex = 0xFFFF;
        entities[i].buttonPressed = 0;
        entities[i].collision = 0xFFFF;
        entities[i].coordinates.x = 0;
        entities[i].coordinates.y = 0;
        entities[i].coordinates.z = 0;
        entities[i].movementVector.x = 0;
        entities[i].movementVector.y = 0;
        entities[i].movementVector.z = 0;
        entities[i].speed = 0.0f;
        entities[i].yOffset = 0;
        entities[i].paletteIndex = 0;
 
    }
    
    for (i = 0; i < MAX_ENTITIES; i++) {
        entityAssetDescriptors[i].collisionBufferX = 0;
        entityAssetDescriptors[i].collisionBufferY = 0;
        entityAssetDescriptors[i].spriteWidth = 0;
        entityAssetDescriptors[i].spriteHeight = 0;
        entityAssetDescriptors[i].flags = 0; 
    } 
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeEntityAsset);

bool initializeEntityAsset(u16 entityAssetIndex, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u8 arg7, u8 arg8, u16* arg9) {

    bool result = FALSE;

    if (entityAssetIndex < MAX_ENTITY_ASSETS) {

        if (!(entityAssetDescriptors[entityAssetIndex].flags & 1)) {
            
            entityAssetDescriptors[entityAssetIndex].romTextureStart = arg1;
            entityAssetDescriptors[entityAssetIndex].romTextureEnd = arg2;
            entityAssetDescriptors[entityAssetIndex].romAssetsIndexStart = arg3;
            entityAssetDescriptors[entityAssetIndex].romAssetsIndexEnd = arg4;
            entityAssetDescriptors[entityAssetIndex].romSpritesheetIndexStart = arg5;
            entityAssetDescriptors[entityAssetIndex].romSpritesheetIndexEnd = arg6;
            entityAssetDescriptors[entityAssetIndex].shadowSpriteIndex = arg8;
            entityAssetDescriptors[entityAssetIndex].animationScripts = arg9;

            entityAssetDescriptors[entityAssetIndex].collisionBufferX = 0;
            entityAssetDescriptors[entityAssetIndex].collisionBufferY = 0;

            entityAssetDescriptors[entityAssetIndex].spriteWidth = 0;
            entityAssetDescriptors[entityAssetIndex].spriteHeight = 0;

            entityAssetDescriptors[entityAssetIndex].flags = 1;

            if (arg7) {
                entityAssetDescriptors[entityAssetIndex].flags = 3;
            }
            
            result = TRUE;
            
        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeSpriteInstance);

bool initializeEntity(u16 entityIndex, u16 globalSpriteIndex, u16 shadowSpriteIndex, u8* vaddrTexture1, u8* vaddrTexture2, 
    u16* vaddrPalette, AnimationFrameMetadata* vaddrAnimationMetadata, u32* vaddrTextureToPaletteLookup, u32* vaddrSpritesheetIndex) {

    bool set = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if (!(entities[entityIndex].flags & ENTITY_ACTIVE)) {

            entities[entityIndex].flags = 1;

            entities[entityIndex].globalSpriteIndex = globalSpriteIndex;
            entities[entityIndex].shadowSpriteIndex = shadowSpriteIndex;

            entities[entityIndex].vaddrTexture1 = vaddrTexture1;
            entities[entityIndex].vaddrTexture2 = vaddrTexture2;
            entities[entityIndex].vaddrPalette = vaddrPalette;
            entities[entityIndex].vaddrAnimationMetadata = vaddrAnimationMetadata;
            entities[entityIndex].vaddrSpritesheetIndex = vaddrSpritesheetIndex;
            entities[entityIndex].vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

            entities[entityIndex].coordinates.x = 0;
            entities[entityIndex].coordinates.y = 0;
            entities[entityIndex].coordinates.z = 0;

            entities[entityIndex].movementVector.x = 0;
            entities[entityIndex].movementVector.y = 0;
            entities[entityIndex].movementVector.z = 0;

            entities[entityIndex].speed = 0;

            entities[entityIndex].entityCollidedWithIndex = 0xFFFF;

            entities[entityIndex].yOffset = 0;
        
            set = TRUE;
            
        }
    }

    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", initializeAnimalEntity);

bool initializeAnimalEntity(u16 index, u16* arg1, AnimationFrameMetadata* arg2, u32* arg3, u32* arg4) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        result = TRUE;
        
        entities[index].vaddrSpritesheetIndex = arg4;
        
        entities[index].flags = 1;
        
        entities[index].vaddrPalette = arg1;
        entities[index].vaddrAnimationMetadata = arg2;
        entities[index].vaddrTextureToPaletteLookup = arg3;

        entities[index].coordinates.x = 0;
        entities[index].coordinates.y = 0;
        entities[index].coordinates.z = 0;

        entities[index].movementVector.x = 0;
        entities[index].movementVector.y = 0;
        entities[index].movementVector.z = 0;

        entities[index].speed = 0.0f;
        entities[index].entityCollidedWithIndex = 0xFFFF;
        entities[index].yOffset = 0;
        
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", forceDeactivateEntity);

// unused; deactivate sprite without checking flags
bool forceDeactivateEntity(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        entities[index].flags = 0;

        deactivateSprite(entities[index].globalSpriteIndex);
        
        if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {
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
            if (entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(entities[i].shadowSpriteIndex);
            } 
        }
        
        i++;
      
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", loadEntity);

bool loadEntity(u16 index, u16 entityAssetIndex, bool transformExempt) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        if (entities[index].flags & ENTITY_ACTIVE) {

            entities[index].entityAssetIndex = entityAssetIndex;
            entities[index].transformExempt = transformExempt;

            if (entityAssetDescriptors[entities[index].entityAssetIndex].flags & 2) {

                dmaSprite(entities[index].globalSpriteIndex, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romTextureStart, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romTextureEnd, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romAssetsIndexStart, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romAssetsIndexEnd, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romSpritesheetIndexStart, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romSpritesheetIndexEnd, 
                    entities[index].vaddrTexture1,
                    entities[index].vaddrTexture2,
                    entities[index].vaddrPalette,
                    entities[index].vaddrAnimationMetadata,
                    entities[index].vaddrTextureToPaletteLookup,
                    entities[index].vaddrSpritesheetIndex,
                    1,
                    entities[index].transformExempt                    
                    );
                
            } else {
                
                dmaSprite(entities[index].globalSpriteIndex, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romTextureStart, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romTextureEnd, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romAssetsIndexStart, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romAssetsIndexEnd, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romSpritesheetIndexStart, 
                    entityAssetDescriptors[entities[index].entityAssetIndex].romSpritesheetIndexEnd, 
                    entities[index].vaddrTexture1,
                    NULL,
                    entities[index].vaddrPalette,
                    entities[index].vaddrAnimationMetadata,
                    entities[index].vaddrTextureToPaletteLookup,
                    NULL,
                    entityAssetDescriptors[entities[index].entityAssetIndex].flags & 2 == 1,
                    entities[index].transformExempt                    
                    );
                
            }

            if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {
                
                dmaSprite(entities[index].shadowSpriteIndex, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].romTextureStart, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].romTextureEnd, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].romAssetsIndexStart, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].romAssetsIndexEnd, 
                    NULL, 
                    NULL, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].vaddrSpritesheet, 
                    NULL, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].vaddrPalette, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].vaddrUnknownAssetSheet, 
                    shadowSpriteDescriptors[entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex].vaddrAnimationMetadata, 
                    NULL, 
                    NULL, 
                    entities[index].transformExempt
                    );

                setSpriteBlendMode(entities[index].shadowSpriteIndex, SPRITE_BLEND_ALPHA_MODULATED);
            
            }

            entities[index].animationIndices.nextAnimationIndex = 0xFFFF;
            entities[index].flags |= ENTITY_INITIALIZED;

            setBilinearFiltering(entities[index].globalSpriteIndex, TRUE);
            setEntityMovementVector(index, 0.0f, 0.0f, 0.0f, 0.0f);
            setEntityColor(index, currentMapLightingRGBA.r, currentMapLightingRGBA.g, currentMapLightingRGBA.b, currentMapLightingRGBA.a);

            if (!(checkMapRGBADone(mapControllers[gMainMapIndex].mainMapIndex))) {
 
                updateEntityRGBA(index, 
                    mainMap[mapControllers[gMainMapIndex].mainMapIndex].mapGlobals.targetRGBA.r, 
                    mainMap[mapControllers[gMainMapIndex].mainMapIndex].mapGlobals.targetRGBA.g, 
                    mainMap[mapControllers[gMainMapIndex].mainMapIndex].mapGlobals.targetRGBA.b, 
                    mainMap[mapControllers[gMainMapIndex].mainMapIndex].mapGlobals.targetRGBA.a, 
                    mainMap[mapControllers[gMainMapIndex].mainMapIndex].mapState.RGBARate);
                
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
        shadowSpriteDescriptors[index].romTextureStart = arg1;
        shadowSpriteDescriptors[index].romTextureEnd = arg2;
        shadowSpriteDescriptors[index].romAssetsIndexStart = arg3;
        shadowSpriteDescriptors[index].romAssetsIndexEnd = arg4;
        shadowSpriteDescriptors[index].vaddrSpritesheet = arg5;
        shadowSpriteDescriptors[index].vaddrPalette = arg6;
        shadowSpriteDescriptors[index].vaddrUnknownAssetSheet = arg7;
        shadowSpriteDescriptors[index].vaddrAnimationMetadata = arg8;
        shadowSpriteDescriptors[index].unk_20 = arg9;
        shadowSpriteDescriptors[index].unk_22 = argA;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityTrackingTarget);

bool setEntityTrackingTarget(u16 index, u16 targetEntityIndex, u16 trackingMode) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {

            entities[index].targetEntityIndex = targetEntityIndex;
            entities[index].trackingMode = trackingMode;

            if (targetEntityIndex == 0xFFFF) {
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

// likely set a palette index if shared asset has multiple palettes (animals, itemHandlers, and cutscenes)
bool setEntityPaletteIndex(u16 index, u16 paletteIndex) {
    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if (entities[index].flags & ENTITY_ACTIVE) {
            entities[index].paletteIndex = paletteIndex;
            entities[index].flags |= ENTITY_PALETTE_SET;
            result = TRUE; 
        }
    }
    
    return result;
}
 
//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityAttachmentOffset);

// used by animals, npcs, item handlers, and cutscenes
bool setEntityAttachmentOffset(u16 index, s16 x, s16 y, s16 z) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            result = TRUE;
            entities[index].attachmentOffset.x = x;
            entities[index].attachmentOffset.y = y;
            entities[index].attachmentOffset.z = z;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setMainMapIndex);

// used by level.c and cutscene.c
bool setMainMapIndex(u16 mapIndex) {

    bool result = FALSE;

    if ((mapIndex == MAIN_MAP_INDEX) && (mapControllers[mapIndex].flags & 1) && (mapControllers[mapIndex].flags & 2)) {
        gMainMapIndex = mapIndex;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityViewSpacePosition);

bool setEntityViewSpacePosition(u16 entityIndex, f32 x, f32 y, f32 z) {

    bool set = FALSE;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && (entities[entityIndex].flags & ENTITY_INITIALIZED)) {
            setSpriteViewSpacePosition(entities[entityIndex].globalSpriteIndex, x, y, z);
            set = TRUE;
        }
        
    }
    
    return set;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", adjustEntityViewSpacePosition);

bool adjustEntityViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            adjustSpriteViewSpacePosition(entities[index].globalSpriteIndex, x, y, z);
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
            
            if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {

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
            
            entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex = shadowSpriteIndex;

            if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex == 0xFF) {

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

            if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {
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
        
        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && !(entities[index].flags & ENTITY_LOAD_PENDING)) {

            entities[index].animationIndices.animationIndex = animationIndex;
            
            entities[index].flags |= ENTITY_IS_CURRENTLY_ANIMATED;
            entities[index].flags &= ~ENTITY_ANIMATION_DIRECTION_CHANGE; 

            globalSprites[entities[index].globalSpriteIndex].stateFlags &= ~SPRITE_ANIMATION_STATE_CHANGED;
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

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && !(entities[index].flags & ENTITY_LOAD_PENDING)) {     
        
            entities[index].animationIndices.animationIndex = animationIndex;
            entities[index].flags |= (8 | ENTITY_ANIMATION_DIRECTION_CHANGE);
    
            globalSprites[entities[index].globalSpriteIndex].stateFlags &= ~SPRITE_ANIMATION_STATE_CHANGED;
    
            result = TRUE;
    
            globalSprites[entities[index].globalSpriteIndex].audioTrigger = FALSE;
    
        }
        
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", loadPendingEntity);

bool loadPendingEntity(u16 index) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && (entities[index].flags & ENTITY_LOAD_PENDING)) {
            
            loadEntity(index, entities[index].entityAssetIndex, entities[index].transformExempt);

            entities[index].flags |= ENTITY_IS_CURRENTLY_ANIMATED;
            entities[index].flags &= ~ENTITY_LOAD_PENDING;

            result = TRUE;

        } 
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", pauseEntityLoad);

bool pauseEntityLoad(u16 index) {

    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED)) {
            
            if (entities[index].flags & ENTITY_IS_CURRENTLY_ANIMATED) {

                setEntityMovementVector(index, 0.0f, 0.0f, 0.0f, 0.0f);
                
                temp = entities[index].flags & ENTITY_PALETTE_SET;
                entities[index].flags |= ENTITY_LOAD_PENDING;

                deactivateEntity(index);
                
                entities[index].flags |= temp;
                
                result = TRUE;
        
            } else {
                entities[index].flags &= ~ENTITY_LOAD_PENDING;
            }

        }

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityDirection);

bool setEntityDirection(u16 index, u8 direction) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_INITIALIZED) && !(entities[index].flags & ENTITY_LOAD_PENDING)) {
        
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
        unpauseEntity(i);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", rotateAllEntities);

// increment/decrement rotation; used by map controller
void rotateAllEntities(s16 arg0) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        entities[i].direction += arg0;
        entities[i].direction &= MAX_DIRECTIONS;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntitiesRGBA);

void setEntitiesRGBA(u8 r, u8 g, u8 b, u8 a) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {

        if (entities[i].flags & ENTITY_ACTIVE && entities[i].flags & ENTITY_IS_CURRENTLY_ANIMATED) {

            if (i < MAX_ENTITIES && entities[i].flags & ENTITY_INITIALIZED) {

                setSpriteColor(entities[i].globalSpriteIndex, r, g, b, a);

                if (entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex != 0xFF) {
                    setSpriteColor(entities[i].shadowSpriteIndex, r, g, b, SHADOW_ALPHA);
                }

            }

        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntitiesRGBAWithTransition);

void setEntitiesRGBAWithTransition(u8 r, u8 g, u8 b, u8 a, s16 arg4) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {

        if ((entities[i].flags & ENTITY_ACTIVE) && (entities[i].flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
        
            if (i < MAX_ENTITIES && entities[i].flags & ENTITY_INITIALIZED) {
        
                updateSpriteRGBA(entities[i].globalSpriteIndex, r, g, b, a, arg4);
        
                if (entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex != 0xFF) {
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

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
            
            entities[index].flags &= ~( 0x8 | ENTITY_PALETTE_SET);
            deactivateSprite(entities[index].globalSpriteIndex);
            
            if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {
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

//INCLUDE_ASM("asm/nonmatchings/system/entity", pauseAllEntityLoads);

// called by pause function
void pauseAllEntityLoads(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        pauseEntityLoad(i);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", loadAllPendingEntities);

void loadAllPendingEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        loadPendingEntity(i);
    }

}


//INCLUDE_ASM("asm/nonmatchings/system/entity", pauseEntity);

bool pauseEntity(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {

        if (entities[index].flags & ENTITY_ACTIVE) {
            
            entities[index].flags |= ENTITY_PAUSED;
            pauseSpriteAnimation(entities[index].globalSpriteIndex);
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", unpauseEntity);

bool unpauseEntity(u16 index) {
    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if (entities[index].flags & ENTITY_ACTIVE) {
        
            entities[index].flags &= ~ENTITY_PAUSED;
            resumeSpriteAnimation(entities[index].globalSpriteIndex);
        
            result = TRUE;
        
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityMapSpaceIndependent);

bool setEntityMapSpaceIndependent(u16 index, bool flag) {
    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_IS_CURRENTLY_ANIMATED)) {

            if (flag) {
                entities[index].flags &= ~ENTITY_MAP_SPACE_INDEPENDENT;
            } else {
                entities[index].flags |= ENTITY_MAP_SPACE_INDEPENDENT;
            }

            result = TRUE;

        }
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityMapSpaceDependent);

// only used by main game loop callback
bool checkEntityMapSpaceDependent(u16 index) {
    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if ((entities[index].flags & ENTITY_ACTIVE) && (entities[index].flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
            temp = entities[index].flags & ENTITY_MAP_SPACE_INDEPENDENT;
            result = !temp;
        }

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntitycoordinates);

bool setEntityCoordinates(u16 entityIndex, f32 x, f32 y, f32 z) {

    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_LOAD_PENDING)) {
            
            entities[entityIndex].coordinates.x = x;
            entities[entityIndex].coordinates.y = y;
            entities[entityIndex].coordinates.z = z;
            
            result = TRUE;

        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityMovementVector);

// manually set movement vector 
// animals, player, item handlers, cutscene
bool setEntityMovementVector(u16 entityIndex, f32 x, f32 y, f32 z, f32 arg4) {

    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && (entities[entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) && !(entities[entityIndex].flags & ENTITY_LOAD_PENDING)) {
            
            entities[entityIndex].movementVector.x = x;
            entities[entityIndex].movementVector.y = y;
            entities[entityIndex].movementVector.z = z;
            entities[entityIndex].speed = arg4 / 2.0f;
            
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

//INCLUDE_ASM("asm/nonmatchings/system/entity", setCameraTrackingEntity);

bool setCameraTrackingEntity(u16 entityIndex, u16 flag) {

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

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntitySpriteDimensions);

bool setEntitySpriteDimensions(u16 entityAssetIndex, u8 arg1, u8 arg2) {

    bool result = FALSE;

    if (entityAssetIndex < MAX_ENTITY_ASSETS) {

        if (entityAssetDescriptors[entityAssetIndex].flags & 1) {
            entityAssetDescriptors[entityAssetIndex].spriteWidth = arg1;
            entityAssetDescriptors[entityAssetIndex].spriteHeight = arg2;
            result = TRUE;
        }

    }

    return result;
    

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityYMovement);

bool setEntityYMovement(u16 index, bool flag) {

    bool result = FALSE;

    if (flag == TRUE) {
        
        entities[index].flags &= ~ENTITY_Y_MOVEMENT;
        result = TRUE;

    } else {

        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= ENTITY_Y_MOVEMENT;
            result = TRUE;
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityHandlesMultipleCollisions);

// only used by farm animals and misc. animals
bool setEntityHandlesMultipleCollisions(u16 index, u8 flag) {

    bool result = FALSE;

    if (flag == TRUE) {
        entities[index].flags &= ~ENTITY_SINGLE_COLLISION_CHECK;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= ENTITY_SINGLE_COLLISION_CHECK;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityCollidable);

bool setEntityCollidable(u16 index, bool flag) {

    bool result = FALSE;

    if (flag == TRUE) {
        entities[index].flags &= ~ENTITY_NON_COLLIDABLE;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= ENTITY_NON_COLLIDABLE;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setEntityTracksCollisions);

bool setEntityTracksCollisions(u16 index, bool flag) {

    bool result = FALSE;

    if (flag == TRUE) {
        entities[index].flags &= ~ENTITY_COLLISION_EXEMPT;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= ENTITY_COLLISION_EXEMPT;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", enableEntityMovement);

bool enableEntityMovement(u16 index, bool flag) {

    bool result = FALSE;

    if (flag == TRUE) {
        entities[index].flags &= ~ENTITY_MOVEMENT_DISABLED;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && entities[index].flags & ENTITY_ACTIVE) {
            entities[index].flags |= ENTITY_MOVEMENT_DISABLED;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityPaused);

bool checkEntityPaused(u16 index) {

    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if (entities[index].flags & ENTITY_ACTIVE) {
            result = (entities[index].flags >> 6) & 1;
        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityToEntityCollision);

u16 checkEntityToEntityCollision(u16 entityIndex, f32 x, f32 z, u16 buttonPressed, u16 entityCount) {

    u32 padding[4];
    
    u16 i;
    u16 count = 0;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED)) {

            for (i = 0; i < entityCount; i++) {
                
                if (i != entityIndex) {
                    
                    if (!(entities[i].flags & ENTITY_COLLISION_EXEMPT)) {

                        count = detectEntityOverlap(&entities[entityIndex], i, x, z, entityAssetDescriptors[entities[entityIndex].entityAssetIndex].spriteWidth, entityAssetDescriptors[entities[entityIndex].entityAssetIndex].spriteHeight);
                        
                        if (count) {

                            entities[i].entityCollidedWithIndex = entityIndex;
                            // seems to imply plans to support different interactions with entities based on button pressed
                            entities[i].buttonPressed = buttonPressed;
                            entities[entityIndex].collision = i;
                            
                            i = MAX_ENTITIES;
                            
                        } else {

                            entities[i].entityCollidedWithIndex = 0xFFFF;
                            entities[i].buttonPressed = 0;
                            entities[entityIndex].collision = 0xFFFF;
                            
                        }

                    }

                }

            }   

        }

    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityProximity);

u16 checkEntityProximity(u16 index, f32 x, f32 z, u16 buttonPressed) {

    Vec3f worldCoordinates;
    Vec3f rotatedPosition;
    Vec3f rotationAngles;
    
    u16 result = 0;
    bool set = FALSE;
    u32 temp;
    u16 i;

    if (index < MAX_ENTITIES) {
        
        if (entities[index].flags & ENTITY_ACTIVE) {
            
            if (!(entities[index].flags & ENTITY_PAUSED)) {
                
                worldCoordinates.x = x;
                worldCoordinates.y = 0;
                worldCoordinates.z = z;

                rotationAngles.x = 0;
                // FIXME: macro not working; should be convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex)
                rotationAngles.y = getSpriteYValueFromDirection((entities[index].direction + getCurrentMapRotation(gMainMapIndex)) % 8);
                rotationAngles.z = 0;

                rotateVector3D(worldCoordinates, &rotatedPosition, rotationAngles);

                for (i = 0; i < MAX_ENTITIES; i++) {

                    if (i != index && !(entities[i].flags & ENTITY_TRACKING_ACTIVE)) {
                        
                        if (temp = detectEntityOverlap(&entities[index], i, rotatedPosition.x, rotatedPosition.z, entityAssetDescriptors[entities[index].entityAssetIndex].spriteWidth, entityAssetDescriptors[entities[index].entityAssetIndex].spriteHeight)) {
                           
                            result |= temp;
                            
                            if (checkButtonPressed(CONTROLLER_1, buttonPressed) || !buttonPressed) {

                                if (!set) {

                                    entities[i].entityCollidedWithIndex = index;
                                    entities[i].buttonPressed = buttonPressed;

                                    if (!(entities[i].flags & ENTITY_COLLISION_EXEMPT)) {
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
                            entities[i].entityCollidedWithIndex = 0xFFFF;
                            entities[i].buttonPressed = 0;
                            
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

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkEntityInteractionAtPosition);

// unused; check if position has level interaction
bool checkEntityInteractionAtPosition(u16 entityIndex, f32 arg1, f32 arg2) {
    
    bool result = FALSE; 

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED)) {
            result = getLevelInteractionIndexFromPosition(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + arg1, entities[entityIndex].coordinates.z + arg2);
        }

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getLevelInteractionIndexFromEntityPosition);

u8 getLevelInteractionIndexFromEntityPosition(u16 entityIndex, f32 x, f32 z) {
    
    Vec3f worldCoordinates;
    Vec3f rotatedPosition;
    Vec3f rotation;

    u8 levelInteractionIndex = 0;

    if ((entityIndex < MAX_ENTITIES)) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE)) {
            
            if (!(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

                worldCoordinates.x = x;
                worldCoordinates.y = 0;
                worldCoordinates.z = z;

                rotation.x = 0;
                rotation.y = getSpriteYValueFromDirection(convertWorldToSpriteDirection(entities[entityIndex].direction, gMainMapIndex));
                rotation.z = 0;
                
                rotateVector3D(worldCoordinates, &rotatedPosition, rotation);

                levelInteractionIndex = getLevelInteractionIndexFromPosition(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + rotatedPosition.x, entities[entityIndex].coordinates.z + rotatedPosition.z);
                
            }
        }
        
    } 
    
    return levelInteractionIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityAnimationOffset);

// returns param 2 of startSpriteAnimation
inline u16 getEntityAnimationOffset(u16 entityIndex, u16 offset) {

    u16 animationIndex = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (entities[entityIndex].flags & ENTITY_ACTIVE) {
            animationIndex = entityAssetDescriptors[entities[entityIndex].entityAssetIndex].animationScripts[offset] & 0x1FFF;
        }
    }
    
    return animationIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getAnimationOffsetFromScript);

// alternate get animation offset from animation scripts
// used by shop and overlay screens
u16 getAnimationOffsetFromScript(u16* vaddr, u16 offset) {
    return vaddr[offset] & 0x1FFF;
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityAnimationType);

inline u16 getEntityAnimationType(u16 entityIndex, u16 offset) {

    u16 flags = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (entities[entityIndex].flags & ENTITY_ACTIVE) {
            flags = entityAssetDescriptors[entities[entityIndex].entityAssetIndex].animationScripts[offset] & 0x6000;
        }
    }
    
    return flags;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityAnimationHorizontalFlip);

inline u16 getEntityAnimationHorizontalFlip(u16 entityIndex, u16 offset) {

    u16 flipHorizontal = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (entities[entityIndex].flags & ENTITY_ACTIVE) {
            flipHorizontal = entityAssetDescriptors[entities[entityIndex].entityAssetIndex].animationScripts[offset] & 0x8000;
        }
    }
    
    return flipHorizontal;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getGroundObjectAtEntityPosition);

// unused or inline
u16 getGroundObjectAtEntityPosition(u16 entityIndex, f32 x, f32 z) {

    Vec3f padding[4];
    
    Vec3f vec;

    u16 index = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        
        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
            
            vec = getEntityRelativeGroundObjectCoords(entityIndex, x, z);

            if (vec.y != 65535.0f) {
                index = getMapGroundObjectSpriteIndex(mapControllers[gMainMapIndex].mainMapIndex, vec.x, vec.z);
            }
            
        }
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setGroundObjectAtEntityPosition);

// unused or inline
// map ground objects
bool setGroundObjectAtEntityPosition(u16 entityIndex, f32 arg1, f32 arg2, u16 spriteIndex) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    Vec3f vec;
    
    if ((entityIndex < MAX_ENTITIES) && (entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
            
        vec = getEntityRelativeGroundObjectCoords(entityIndex, arg1, arg2);
        
        if (vec.y != 65535.0f) {
            setMapGroundObjectSpriteIndexFromFloat(mapControllers[gMainMapIndex].mainMapIndex, spriteIndex, vec.x, vec.z);
        }
        
    }
    
    return 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityRelativeGroundObjectCoords);

Vec3f getEntityRelativeGroundObjectCoords(u16 entityIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f rotatedPosition;
    Vec3f rotation;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            rotation.x = 0;
            rotation.y = getSpriteYValueFromDirection(convertWorldToSpriteDirection(entities[entityIndex].direction, gMainMapIndex));
            rotation.z = 0;

            rotateVector3D(vec, &rotatedPosition, rotation);

            vec4 = getMapGroundObjectCoordinates(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + rotatedPosition.x, entities[entityIndex].coordinates.z + rotatedPosition.z);
            
        }
        
    }

    return vec4;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/entity", getTerrainAtEntityPosition);

// unused
u16 getTerrainAtEntityPosition(u16 entityIndex, f32 x, f32 z) {

    u32 padding[11];

    Vec3f tileCoordinates;
    
    u16 terrainIndex = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            tileCoordinates = getEntityRelativeTilePosition(entityIndex, x, z);

            if (tileCoordinates.y != 65535.0f) {   
                terrainIndex = getTileIndexFromGrid(mapControllers[gMainMapIndex].mainMapIndex, tileCoordinates.x, tileCoordinates.z);
            }
        }
    
    }

    return terrainIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", setMapAdditionAtEntityPosition);

// unused or inline
bool setMapAdditionAtEntityPosition(u16 entityIndex, f32 arg1, f32 arg2, u16 arg3) {

    // FIXME: shouldn't be necessary
    Vec3f padding[4];
    
    Vec3f tileCoordinates;
    
    bool set = FALSE;

     if (entityIndex < MAX_ENTITIES) {
        
        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            tileCoordinates = getEntityRelativeTilePosition(entityIndex, arg1, arg2);

            if (tileCoordinates.y != 65535.0f) {

                // set map additions
                initializeMapAdditionAtPosition(mapControllers[gMainMapIndex].mainMapIndex, arg3, tileCoordinates.x, tileCoordinates.z);
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

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityRelativeTilePosition);

Vec3f getEntityRelativeTilePosition(u16 entityIndex, f32 x, f32 z) {

    Vec3f worldCoordinates;
    Vec3f rotatedCoordinates;
    Vec3f rotation;
    Vec3f tileCoordinates;

    tileCoordinates.x = 0.0f;
    tileCoordinates.y = 65535.0f;
    tileCoordinates.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            worldCoordinates.x = x;
            worldCoordinates.y = 0;
            worldCoordinates.z = z;

            rotation.x = 0;
            rotation.y = getSpriteYValueFromDirection(convertWorldToSpriteDirection(entities[entityIndex].direction, gMainMapIndex));
            rotation.z = 0;

            rotateVector3D(worldCoordinates, &rotatedCoordinates, rotation);

            tileCoordinates = convertWorldToTileCoordinates(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x + rotatedCoordinates.x, entities[entityIndex].coordinates.z + rotatedCoordinates.z);
            
        }
        
    }

    return tileCoordinates;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityTileCoordinates);

Vec3f getEntityTileCoordinates(u16 entityIndex) {

    // FIXME: shouldn't be necessary
    u32 padding[11];
    
    Vec3f tileCoordinates;

    tileCoordinates.x = 0;
    tileCoordinates.z = 0;
    tileCoordinates.y = 65535.0f;
    
    if (entityIndex < MAX_ENTITIES) {
        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
            tileCoordinates = convertWorldToTileCoordinates(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x, entities[entityIndex].coordinates.z);
        } 
    }

    return tileCoordinates;

}

// alternate
// Vec3f getEntityTileCoordinates(u16 index) {
    
//     int padding[11];

//     Vec3f vec;

//     vec.x = 0;
//     vec.z = 0;
//     vec.y = 65535.0f;
    
//     if (index < MAX_ENTITIES) {
//         if ((entities[index].flags & ENTITY_ACTIVE) && !(entities[index].flags & ENTITY_PAUSED) && !(entities[index].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
//             convertWorldToTileCoordinates(&vec, mapControllers[gMainMapIndex].mainMapIndex, entities[index].coordinates.x, entities[index].coordinates.z);
//         } 
//     }
 
//     return vec;
// }

//INCLUDE_ASM("asm/nonmatchings/system/entity", getEntityRelativeTileCoords);

// unused
Vec3f getEntityRelativeTileCoords(u16 entityIndex, f32 x, f32 z) {

    Vec3f position;
    Vec3f rotatedCoordinates;
    Vec3f rotation;
    Vec3f tileCoordinates;

    tileCoordinates.x = 0.0f;
    tileCoordinates.y = 65535.0f;
    tileCoordinates.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            position.x = x;
            position.y = 0;
            position.z = z;

            rotation.x = 0;
            rotation.y = getSpriteYValueFromDirection(convertWorldToSpriteDirection(entities[entityIndex].direction, gMainMapIndex));
            rotation.z = 0;

            rotateVector3D(position, &rotatedCoordinates, rotation);
            tileCoordinates = convertWorldToTileCoordinates(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x, entities[entityIndex].coordinates.z);
            
            tileCoordinates.x += rotatedCoordinates.x;
            tileCoordinates.z += rotatedCoordinates.z;

        }
        
    }

    return tileCoordinates;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkTerrainCollisionInDirection);

bool checkTerrainCollisionInDirection(u16 index, s16 moveDistance, u8 direction) {

    Vec3f projectedPosition;
    
    f32 terrainHeight;
    f32 entityTopBound;
    f32 entityLowerBound;
    
    bool result = FALSE;
    
    u8 isGroundTooLow;
    u8 isGroundTooHigh;
    
    entityTopBound = entities[index].coordinates.y + 24.0f;
    entityLowerBound = entities[index].coordinates.y - 24.0f;
    
    projectedPosition = projectEntityPosition(index, moveDistance, direction);
    terrainHeight = getTerrainHeightAtPosition(MAIN_MAP_INDEX, projectedPosition.x, projectedPosition.z);

    isGroundTooHigh = TRUE;
    
    if (!(entityTopBound <= terrainHeight)) {
        isGroundTooHigh = 0;
    }

    isGroundTooLow = TRUE;
    
    if (!(terrainHeight <= entityLowerBound)) {
        isGroundTooLow = 0;
    }

    if (isGroundTooLow || isGroundTooHigh) {
        result = TRUE;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", projectEntityPosition);

Vec3f projectEntityPosition(u16 index, s16 zDisplacement, u8 direction) {

    Vec3f offset;
    Vec3f rotatedOffset;
    Vec3f rotation;

    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = zDisplacement;

    rotation.x = 0;
    rotation.y = getSpriteYValueFromDirection(direction % 8);
    rotation.z = 0;

    rotateVector3D(offset, &rotatedOffset, rotation);

    offset.x = entities[index].coordinates.x + rotatedOffset.x;
    offset.y = entities[index].coordinates.y + rotatedOffset.y;
    offset.z = entities[index].coordinates.z + rotatedOffset.z;

    return offset;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", checkTerrainMovementCollision);

u16 checkTerrainMovementCollision(Entity* entity, f32 deltaX, f32 deltaZ, u8 direction) {

    Vec3f collisionOffset;
    Vec3f rotatedCollisionPoint;
    Vec3f rotationVector;

    f32 entityCurrentY;
    f32 entityTopBound;
    f32 entityLowerBound;
    f32 directionAngle;
    f32 terrainHeight;

    u8 isGroundTooLow;
    u8 isGroundTooHigh;

    u16 collisionFlags;

    entityCurrentY = entity->coordinates.y;

    // upper and lower bounds; 48-unit collision box
    entityTopBound = entityCurrentY + 24.0f;
    entityLowerBound = entityCurrentY - 24.0f;

    directionAngle = getSpriteYValueFromDirection(direction % 8);
    
    collisionOffset.x = 0;
    collisionOffset.y = 0;
    collisionOffset.z = entityAssetDescriptors[entity->entityAssetIndex].collisionBufferY;

    rotationVector.x = 0;
    rotationVector.y = directionAngle + 8.0f;
    rotationVector.z = 0;
    
    rotateVector3D(collisionOffset, &rotatedCollisionPoint, rotationVector);
    
    terrainHeight = getTerrainHeightAtPosition(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ);

    collisionFlags = 0;
    isGroundTooHigh = TRUE;
    
    if (!(entityTopBound <= terrainHeight)) {
        isGroundTooHigh = FALSE;
    }

    isGroundTooLow = TRUE;
    
    if (!(terrainHeight <= entityLowerBound)) {
        isGroundTooLow = FALSE;
    }

    if ((isGroundTooHigh | isGroundTooLow) || (checkTileHasGroundObject(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ))) {
        collisionFlags = 0xF00;
    }

    rotationVector.y = directionAngle - 8.0f;

    rotateVector3D(collisionOffset, &rotatedCollisionPoint, rotationVector);

    terrainHeight = getTerrainHeightAtPosition(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ);

    isGroundTooHigh = TRUE;
    
    if (!(entityTopBound <= terrainHeight)) {
        isGroundTooHigh = FALSE;
    }

    isGroundTooLow = TRUE;
    
    if (!(terrainHeight <= entityLowerBound)) {
        isGroundTooLow = FALSE;
    }

    if ((isGroundTooHigh | isGroundTooLow) || (checkTileHasGroundObject(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ))) {
        collisionFlags |= 0xF000;
    }
    
    rotationVector.y = directionAngle + 45.0f;

    rotateVector3D(collisionOffset, &rotatedCollisionPoint, rotationVector);

    terrainHeight = getTerrainHeightAtPosition(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ);

    isGroundTooHigh = TRUE;
    
    if (!(entityTopBound <= terrainHeight)) {
        isGroundTooHigh = FALSE;
    }

    isGroundTooLow = TRUE;
    
    if (!(terrainHeight <= entityLowerBound)) {
        isGroundTooLow = FALSE;
    }

    if ((isGroundTooHigh | isGroundTooLow) || (checkTileHasGroundObject(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ))) {
        collisionFlags |= 2;
    }
    
    rotationVector.y = (directionAngle + 360.f) - 45.0f;
    
    rotateVector3D(collisionOffset, &rotatedCollisionPoint, rotationVector);

    terrainHeight = getTerrainHeightAtPosition(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ);
    
    isGroundTooHigh = TRUE;
    
    if (!(entityTopBound <= terrainHeight)) {
        isGroundTooHigh = 0;
    }

    isGroundTooLow = TRUE;
    
    if (!(terrainHeight <= entityLowerBound)) {
        isGroundTooLow = 0;
    }

    if ((isGroundTooHigh | isGroundTooLow) || (checkTileHasGroundObject(MAIN_MAP_INDEX, rotatedCollisionPoint.x + entity->coordinates.x + deltaX, rotatedCollisionPoint.z + entity->coordinates.z + deltaZ))) {
        collisionFlags |= 0x20;
    }
    
    return collisionFlags;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", detectEntityOverlap);

u8 detectEntityOverlap(Entity* entity, u16 entityIndex, f32 deltaX, f32 deltaZ, u16 entityWidth, u16 entityHeight) {

    u32 padding[12];
 
    f32 dimensionX;           
    f32 dimensionZ;           

    f32 distanceX;            
    f32 distanceZ;            
    
    f32 adjustedX;            
    f32 adjustedZ;            

    f32 xBoundaryHalf;       
    f32 zBoundaryHalf;       
    
    u8 counter = 0;
    u8 result = 0;
    
    if (entityIndex < MAX_ENTITIES && entities[entityIndex].flags & ENTITY_ACTIVE && entities[entityIndex].flags & ENTITY_IS_CURRENTLY_ANIMATED) { 

        adjustedX = entity->coordinates.x + deltaX;
        adjustedZ = entity->coordinates.z + deltaZ;

        if (entityAssetDescriptors[entities[entityIndex].entityAssetIndex].spriteWidth < entityWidth) {
            dimensionX = entityWidth;
        } else {
            dimensionX = entityAssetDescriptors[entities[entityIndex].entityAssetIndex].spriteWidth;
        }

        if (entityAssetDescriptors[entities[entityIndex].entityAssetIndex].spriteHeight < entityHeight) {
            dimensionZ = entityHeight;
        } else {
            dimensionZ = entityAssetDescriptors[entities[entityIndex].entityAssetIndex].spriteHeight;
        }

        if (adjustedX <= entities[entityIndex].coordinates.x) {
            
            xBoundaryHalf = (2.0f * dimensionX) - 1.0f;
            distanceX = (entities[entityIndex].coordinates.x - adjustedX) + xBoundaryHalf;

            if (deltaX < 0.0f) {
                counter++;
            }
            
            if (deltaX == 0.0f) {
                counter++;
            }
            
        } else {

            xBoundaryHalf = (2.0f * dimensionX) - 1.0f;
            distanceX = (adjustedX - entities[entityIndex].coordinates.x) + xBoundaryHalf;
        
            if (0.0f < deltaX) {
                counter++;
            }
            
            if (deltaX == 0.0f) {
                counter++;
            }
            
        }

        if (adjustedZ <= entities[entityIndex].coordinates.z) {
            
            zBoundaryHalf = (2.0f * dimensionZ) - 1.0f;
            distanceZ = (entities[entityIndex].coordinates.z - adjustedZ) + zBoundaryHalf;
            
            if (deltaZ < 0.0f) {
                counter++;
            }
            
             if (deltaZ == 0.0f) {
                counter++;
            }   
            
        } else {

            zBoundaryHalf = (2.0f * dimensionZ) - 1.0f;
            distanceZ = (adjustedZ -  entities[entityIndex].coordinates.z) + zBoundaryHalf; 

            if (deltaZ > 0.0f) {
                counter++;
            }
            
            if (deltaZ == 0.0f) {
                counter++;
            }            
        
        }
 
        if (counter != 2) {
            
            if ((distanceX <= (2.0f * xBoundaryHalf)) && (distanceZ <= (2.0f * zBoundaryHalf))) {
              result = 3; 
            }
        
        }
        
    }

    return result;
      
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", attemptEntityMovement);

// unused or inline
u16 attemptEntityMovement(u16 index) {

    Vec3f projectedPosition;

    u16 collisionFlags = checkTerrainMovementCollision(&entities[index], entities[index].movementVector.x, entities[index].movementVector.z, convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex));

    if (collisionFlags) {

        projectedPosition = getMovementVectorFromDirection(entities[index].speed, convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex), 0.0f);

        collisionFlags = checkTerrainMovementCollision(&entities[index], projectedPosition.x, projectedPosition.z, convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex));
        
        if (!collisionFlags) {
            entities[index].movementVector.x = projectedPosition.x;
            entities[index].movementVector.z = projectedPosition.z;
        }
        
    }

    return collisionFlags;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", doEntityPathfinding);

void doEntityPathfinding(u16 index) {
    
    Vec3f projectedPosition;
    u8 direction;

    u16 collisionFlags = checkTerrainMovementCollision(&entities[index], entities[index].movementVector.x, entities[index].movementVector.z, convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex));

    if ((collisionFlags & 0xFF00) != 0xFF00) {

        if (collisionFlags & 0xF0F) {            
            
            // try right turn
            direction = (convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex) + 1) % 8;
    
            projectedPosition = getMovementVectorFromDirection(entities[index].speed, direction, 0.0f);
    
            if (checkTerrainMovementCollision(&entities[index], projectedPosition.x, projectedPosition.z, direction)) {
                entities[index].movementVector.x = 0.0f;
                entities[index].movementVector.z = 0.0f; 
            } else {
                entities[index].movementVector.x = projectedPosition.x;
                entities[index].movementVector.z = projectedPosition.z;
            }
            
        }
        
        if (collisionFlags & 0xF0F0) {

            // try left turn
            direction = (convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex) + 7) % 8;

            projectedPosition = getMovementVectorFromDirection(entities[index].speed, direction, 0.0f);

            if (checkTerrainMovementCollision(&entities[index], projectedPosition.x, projectedPosition.z, direction)) {
                entities[index].movementVector.x = 0.0f;
                entities[index].movementVector.z = 0.0f; 
            } else {
                entities[index].movementVector.x = projectedPosition.x;
                entities[index].movementVector.z = projectedPosition.z;
            }
       
        }
        
    } else {
    
        projectedPosition = getMovementVectorFromDirection(entities[index].speed, convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex), 0.0f);

        if (checkTerrainMovementCollision(&entities[index], projectedPosition.x, projectedPosition.z, convertWorldToSpriteDirection(entities[index].direction, gMainMapIndex)) & 0xFFFF) {
            entities[index].movementVector.x = 0.0f;
            entities[index].movementVector.z = 0.0f;
        } else {
            entities[index].movementVector.x = projectedPosition.x;
            entities[index].movementVector.z = projectedPosition.z;
        }

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", updateEntityPhysics);

void updateEntityPhysics(u16 index) {

    Vec3f appliedMovement;
    Vec3f rotatedPosition;
    Vec3f rotation;
    
    f32 shadowHeight;
    
    f32 tempX;
    f32 tempZ;

    u16 collisionCheck;

    appliedMovement.x = 0.0f;
    appliedMovement.y = 0.0f;
    appliedMovement.z = 0.0f;

    entities[index].flags &= ~ENTITY_TOUCHING_GROUND;

    if (entities[index].flags & 0x200) {

        if (!(mapControllers[gMainMapIndex].flags & (0x8 | 0x10))) {

            // FIXME: something off here 
            if (((entities[index].trackingMode + 2) & 0xFF) < 2U) {
                
                rotation.x = 0.0f;
                rotation.y = getSpriteYValueFromDirection((entities[entities[index].targetEntityIndex].direction + mapControllers[gMainMapIndex].rotation) % 8);
                rotation.z = 0.0f;
                
            } else {

                rotation.x = 0.0f;
                rotation.y = getSpriteYValueFromDirection((mapControllers[gMainMapIndex].rotation + entities[index].trackingMode) % 8);
                rotation.z = 0.0f;

            }

            rotateVector3D(entities[index].attachmentOffset, &rotatedPosition, rotation);

            entities[index].viewSpacePosition.x = entities[entities[index].targetEntityIndex].viewSpacePosition.x + rotatedPosition.x;
            entities[index].viewSpacePosition.y = entities[entities[index].targetEntityIndex].viewSpacePosition.y + rotatedPosition.y;
            entities[index].viewSpacePosition.z = entities[entities[index].targetEntityIndex].viewSpacePosition.z + rotatedPosition.z;

            entities[index].coordinates.x = entities[entities[index].targetEntityIndex].coordinates.x + rotatedPosition.x;
            entities[index].coordinates.y = entities[entities[index].targetEntityIndex].coordinates.y + rotatedPosition.y;
            entities[index].coordinates.z = entities[entities[index].targetEntityIndex].coordinates.z + rotatedPosition.z;

        }
        
        shadowHeight = getTerrainHeightAtPosition(0, entities[index].coordinates.x, entities[index].coordinates.z);
        shadowHeight -= mapControllers[gMainMapIndex].viewPosition.y;
                
    } else {

        if (!(entities[index].flags & ENTITY_MOVEMENT_DISABLED)) {

            if ((entities[index].movementVector.x != 0.0f) || (entities[index].movementVector.y != 0.0f) || (entities[index].movementVector.z != 0.0f)) {
                doEntityPathfinding(index);
            }
        
        }

        if (entities[index].flags & ENTITY_SINGLE_COLLISION_CHECK) {
            collisionCheck = checkEntityToEntityCollision(index, entities[index].movementVector.x, entities[index].movementVector.z, 0, 1);        
        } else {
            collisionCheck = checkEntityToEntityCollision(index, entities[index].movementVector.x, entities[index].movementVector.z, 0, MAX_ENTITIES);
        }

        // FIXME: seems odd
        collisionCheck &= entities[index].flags & ENTITY_NON_COLLIDABLE ? 0 : 0xFFFF;

        if (!collisionCheck) {

            appliedMovement.x = entities[index].movementVector.x;
            appliedMovement.y = entities[index].movementVector.y;
            appliedMovement.z = entities[index].movementVector.z;
            
        }  else {
            
            appliedMovement.x = 0.0f;
            appliedMovement.y = 0.0f;
            appliedMovement.z = 0.0f;
            
        }

        entities[index].coordinates.x += appliedMovement.x;
        entities[index].coordinates.z += appliedMovement.z;

        if (entities[index].flags & 0x20) {

            tempX = entities[index].coordinates.x;
            tempZ = entities[index].coordinates.z;

            if (!(entities[index].flags & ENTITY_Y_MOVEMENT)) {
                
                entities[index].coordinates.y = getTerrainHeightAtPosition(0, entities[index].coordinates.x, entities[index].coordinates.z);
                shadowHeight = entities[index].coordinates.y - mapControllers[gMainMapIndex].viewPosition.y;
                    
            } else {

                entities[index].coordinates.y += appliedMovement.y;
                tempZ += appliedMovement.y;
                
                shadowHeight = getTerrainHeightAtPosition(0, entities[index].coordinates.x, entities[index].coordinates.z) - entities[index].coordinates.y;
                
            }

            setMapControllerViewPosition(gMainMapIndex, tempX, entities[index].coordinates.y, tempZ);

            entities[index].viewSpacePosition.x = entities[index].coordinates.x - tempX;
            entities[index].viewSpacePosition.y = 0;
            entities[index].viewSpacePosition.z = entities[index].coordinates.z - tempZ;
            
        } else {

            if (!(entities[index].flags & ENTITY_Y_MOVEMENT)) {
                
                entities[index].coordinates.y = getTerrainHeightAtPosition(0, entities[index].coordinates.x, entities[index].coordinates.z);
                shadowHeight = entities[index].coordinates.y - mapControllers[gMainMapIndex].viewPosition.y;
                
            } else {
                
                entities[index].coordinates.y += appliedMovement.y;
                
                shadowHeight = getTerrainHeightAtPosition(0, entities[index].coordinates.x, entities[index].coordinates.z);

                if (entities[index].coordinates.y <= shadowHeight) {
                    entities[index].flags |= ENTITY_TOUCHING_GROUND;
                }

                shadowHeight -= mapControllers[gMainMapIndex].viewPosition.y;
                
            }

            entities[index].viewSpacePosition.x = entities[index].coordinates.x - mapControllers[gMainMapIndex].viewPosition.x;
            entities[index].viewSpacePosition.y = entities[index].coordinates.y - mapControllers[gMainMapIndex].viewPosition.y;
            entities[index].viewSpacePosition.z = entities[index].coordinates.z - mapControllers[gMainMapIndex].viewPosition.z;
            
        }
            
    }

    setSpriteViewSpacePosition(entities[index].globalSpriteIndex, entities[index].viewSpacePosition.x, entities[index].viewSpacePosition.y + entities[index].yOffset, entities[index].viewSpacePosition.z);

    if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {
        setSpriteViewSpacePosition(entities[index].shadowSpriteIndex, entities[index].viewSpacePosition.x, shadowHeight, entities[index].viewSpacePosition.z);
    }
    
    entities[index].movementVector.x = 0.0f;
    entities[index].movementVector.y = 0.0f;
    entities[index].movementVector.z = 0.0f;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/entity", updateEntities);

void updateEntities(void) {

    Vec3f tileCoordinates;
    
    u16 i = 0;
    
    u8 directionalOffset;
    u16 animationMetadataIndex;
    u16 animationType;
    u16 flipHorizontal;

    while (i < MAX_ENTITIES) {

        if ((entities[i].flags & ENTITY_ACTIVE) && (entities[i].flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
            
            entities[i].unk_66 = 0;

            if (!(entities[i].flags & ENTITY_PAUSED) || (entities[i].flags & 0x200)) {
                
                updateEntityPhysics(i);

                 if (entities[i].flags & 0x200) {
                    if (entities[i].trackingMode == 0xFE) {
                        entities[i].direction = entities[entities[i].targetEntityIndex].direction;
                    }
                }
                
            }

            if (entities[i].flags & ENTITY_ANIMATION_DIRECTION_CHANGE) {
                directionalOffset = entities[i].direction;
            } else {
                directionalOffset = 0;
            }

            animationMetadataIndex = getEntityAnimationOffset(i, entities[i].animationIndices.animationIndex + directionalOffset);
            animationType = getEntityAnimationType(i, entities[i].animationIndices.animationIndex + directionalOffset);
            flipHorizontal = getEntityAnimationHorizontalFlip(i, entities[i].animationIndices.animationIndex + directionalOffset);

            tileCoordinates = getTileCoordinates(&mainMap[MAIN_MAP_INDEX], 
                entities[i].coordinates.x + mainMap[MAIN_MAP_INDEX].mapGrid.tileSizeX / 2,
                entities[i].coordinates.z + mainMap[MAIN_MAP_INDEX].mapGrid.tileSizeZ / 2);
            
            if (mainMap[MAIN_MAP_INDEX].visibilityGrid[(u8)tileCoordinates.z][(u8)tileCoordinates.x] || entities[i].flags & 0x20) {

                if ((directionalOffset + entities[i].animationIndices.animationIndex) != entities[i].animationIndices.nextAnimationIndex) {
                    
                    resetAnimationState(entities[i].globalSpriteIndex);

                    switch (animationType) {

                        case 0:
                            startSpriteAnimation(entities[i].globalSpriteIndex, animationMetadataIndex, 0xFF);
                            break;

                        case 0x2000: 
                            startSpriteAnimation(entities[i].globalSpriteIndex, animationMetadataIndex, 0xFE);
                            break;
                        
                        case 0x4000:
                            startSpriteAnimation(entities[i].globalSpriteIndex, animationMetadataIndex, 0xFD);
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

                if (entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex != 0xFF) {

                    if (globalSprites[entities[i].shadowSpriteIndex].viewSpacePosition.y <= entities[i].viewSpacePosition.y) {

                        startSpriteAnimation(entities[i].shadowSpriteIndex, 
                             shadowSpriteDescriptors[entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex].unk_20,
                             shadowSpriteDescriptors[entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex].unk_22);
                                             
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

                if (entityAssetDescriptors[entities[i].entityAssetIndex].shadowSpriteIndex != 0xFF) {
                    resetAnimationState(entities[i].shadowSpriteIndex);
                }
                    
            }
             
        }

        i++;
        
    } 
    
}
