#include "common.h"

#include "system/entity.h"

#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"

#include "game/npc.h"

#include "mainproc.h"

#include "data/animation/entityAnimationScripts/entityAnimationScripts.h"


// bss
Entity entities[MAX_ENTITIES];
EntityAssetDescriptor entityAssetDescriptors[MAX_ENTITY_ASSETS];
ShadowSpriteDescriptor shadowSpriteDescriptors[3];

// forward declarations
Vec3f getEntityRelativeTilePosition(u16, f32, f32);  
u16 checkTerrainMovementCollision(Entity*, f32, f32, u8); 
u8 detectEntityOverlap(Entity*, u16, f32, f32, u16, u16);
void updateEntityPhysics(u16);

void initializeEntities(void) {
    
    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        Entity *ent = &entities[i];

        ent->flags = 0;
        ent->globalSpriteIndex = 0;
        ent->animationIndices.animationIndex = 0;
        ent->animationIndices.nextAnimationIndex = 0xFFFF;
        ent->direction = 0;
        ent->entityCollidedWithIndex = 0xFFFF;
        ent->buttonPressed = 0;
        ent->collision = 0xFFFF;
        ent->coordinates.x = 0;
        ent->coordinates.y = 0;
        ent->coordinates.z = 0;
        ent->movementVector.x = 0;
        ent->movementVector.y = 0;
        ent->movementVector.z = 0;
        ent->speed = 0.0f;
        ent->yOffset = 0;
        ent->paletteIndex = 0;
 
    }
    
    for (i = 0; i < MAX_ENTITIES; i++) {
        entityAssetDescriptors[i].collisionBufferX = 0;
        entityAssetDescriptors[i].collisionBufferY = 0;
        entityAssetDescriptors[i].spriteWidth = 0;
        entityAssetDescriptors[i].spriteHeight = 0;
        entityAssetDescriptors[i].flags = 0; 
    } 
}

bool initializeEntityAsset(u16 entityAssetIndex, u32 romTextureStart, u32 romTextureEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, u8 isType1Sprite, u8 shadowSpriteIndex, u16* animationScripts) {

    bool result = FALSE;

    if (entityAssetIndex < MAX_ENTITY_ASSETS) {

        if (!(entityAssetDescriptors[entityAssetIndex].flags & ENTITY_ASSET_ACTIVE)) {

            entityAssetDescriptors[entityAssetIndex].romTextureStart = romTextureStart;
            entityAssetDescriptors[entityAssetIndex].romTextureEnd = romTextureEnd;
            entityAssetDescriptors[entityAssetIndex].romAssetsIndexStart = romAssetsIndexStart;
            entityAssetDescriptors[entityAssetIndex].romAssetsIndexEnd = romAssetsIndexEnd;
            entityAssetDescriptors[entityAssetIndex].romSpritesheetIndexStart = romSpritesheetIndexStart;
            entityAssetDescriptors[entityAssetIndex].romSpritesheetIndexEnd = romSpritesheetIndexEnd;
            entityAssetDescriptors[entityAssetIndex].shadowSpriteIndex = shadowSpriteIndex;
            entityAssetDescriptors[entityAssetIndex].animationScripts = animationScripts;

            entityAssetDescriptors[entityAssetIndex].collisionBufferX = 0;
            entityAssetDescriptors[entityAssetIndex].collisionBufferY = 0;

            entityAssetDescriptors[entityAssetIndex].spriteWidth = 0;
            entityAssetDescriptors[entityAssetIndex].spriteHeight = 0;

            entityAssetDescriptors[entityAssetIndex].flags = ENTITY_ASSET_ACTIVE;

            if (isType1Sprite) {
                entityAssetDescriptors[entityAssetIndex].flags = (ENTITY_ASSET_ACTIVE | ENTITY_ASSET_IS_TYPE_1_SPRITE);
            }
            
            result = TRUE;
            
        }
    }
    
    return result;

}

bool initializeEntity(u16 entityIndex, u16 globalSpriteIndex, u16 shadowSpriteIndex, u8* vaddrTexture1, u8* vaddrTexture2, 
    u16* vaddrPalette, AnimationFrameMetadata* vaddrAnimationMetadata, u32* vaddrTextureToPaletteLookup, u32* vaddrSpritesheetIndex) {
    Entity *ent = &entities[entityIndex];


    bool set = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if (!(ent->flags & ENTITY_ACTIVE)) {

            ent->flags = ENTITY_ACTIVE;

            ent->globalSpriteIndex = globalSpriteIndex;
            ent->shadowSpriteIndex = shadowSpriteIndex;

            ent->vaddrTexture1 = vaddrTexture1;
            ent->vaddrTexture2 = vaddrTexture2;
            ent->vaddrPalette = vaddrPalette;
            ent->vaddrAnimationMetadata = vaddrAnimationMetadata;
            ent->vaddrSpritesheetIndex = vaddrSpritesheetIndex;
            ent->vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

            ent->coordinates.x = 0;
            ent->coordinates.y = 0;
            ent->coordinates.z = 0;

            ent->movementVector.x = 0;
            ent->movementVector.y = 0;
            ent->movementVector.z = 0;

            ent->speed = 0;

            ent->entityCollidedWithIndex = 0xFFFF;

            ent->yOffset = 0;
        
            set = TRUE;
            
        }
    }

    return set;
    
}

bool initializeAnimalEntity(u16 index, u16* vaddrPalette, AnimationFrameMetadata* vaddrAnimationMetadata, u32* vaddrTextureToPaletteLookup, u32* vaddrSpritesheetIndex) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        result = TRUE;

        ent->vaddrSpritesheetIndex = vaddrSpritesheetIndex;

        ent->flags = 1;

        ent->vaddrPalette = vaddrPalette;
        ent->vaddrAnimationMetadata = vaddrAnimationMetadata;
        ent->vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

        ent->coordinates.x = 0;
        ent->coordinates.y = 0;
        ent->coordinates.z = 0;

        ent->movementVector.x = 0;
        ent->movementVector.y = 0;
        ent->movementVector.z = 0;

        ent->speed = 0.0f;
        ent->entityCollidedWithIndex = 0xFFFF;
        ent->yOffset = 0;
        
    }

    return result;

}

// unused; deactivate sprite without checking flags
// bool forceDeactivateEntity(u16 index) {

//     bool result = FALSE;
    
//     if (index < MAX_ENTITIES) {
        
//         entities[index].flags = 0;

//         deactivateSprite(entities[index].globalSpriteIndex);
        
//         if (entityAssetDescriptors[entities[index].entityAssetIndex].shadowSpriteIndex != 0xFF) {
//             deactivateSprite(entities[index].shadowSpriteIndex);
//         }
        
//         result = TRUE;
    
//     }

//     return result;

// }

void deactivateSprites(void) {

    u16 i = 0;
    u16 temp;

    while (i < MAX_ENTITIES) {
        Entity *ent = &entities[i];


        temp = i;

        if (temp < MAX_ENTITIES) {

            ent->flags = 0;
        
            deactivateSprite(ent->globalSpriteIndex);
            
            if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(ent->shadowSpriteIndex);
            } 
        }
        
        i++;
      
    }
}

bool loadEntity(u16 index, u16 entityAssetIndex, bool transformExempt) {
    Entity *ent = &entities[index];


    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        if (ent->flags & ENTITY_ACTIVE) {

            ent->entityAssetIndex = entityAssetIndex;
            ent->transformExempt = transformExempt;

            if (entityAssetDescriptors[ent->entityAssetIndex].flags & ENTITY_ASSET_IS_TYPE_1_SPRITE) {

                dmaSprite(ent->globalSpriteIndex, 
                    entityAssetDescriptors[ent->entityAssetIndex].romTextureStart, 
                    entityAssetDescriptors[ent->entityAssetIndex].romTextureEnd, 
                    entityAssetDescriptors[ent->entityAssetIndex].romAssetsIndexStart, 
                    entityAssetDescriptors[ent->entityAssetIndex].romAssetsIndexEnd, 
                    entityAssetDescriptors[ent->entityAssetIndex].romSpritesheetIndexStart, 
                    entityAssetDescriptors[ent->entityAssetIndex].romSpritesheetIndexEnd, 
                    ent->vaddrTexture1,
                    ent->vaddrTexture2,
                    ent->vaddrPalette,
                    ent->vaddrAnimationMetadata,
                    ent->vaddrTextureToPaletteLookup,
                    ent->vaddrSpritesheetIndex,
                    1,
                    ent->transformExempt                    
                    );
                
            } else {
                
                dmaSprite(ent->globalSpriteIndex, 
                    entityAssetDescriptors[ent->entityAssetIndex].romTextureStart, 
                    entityAssetDescriptors[ent->entityAssetIndex].romTextureEnd, 
                    entityAssetDescriptors[ent->entityAssetIndex].romAssetsIndexStart, 
                    entityAssetDescriptors[ent->entityAssetIndex].romAssetsIndexEnd, 
                    entityAssetDescriptors[ent->entityAssetIndex].romSpritesheetIndexStart, 
                    entityAssetDescriptors[ent->entityAssetIndex].romSpritesheetIndexEnd, 
                    ent->vaddrTexture1,
                    NULL,
                    ent->vaddrPalette,
                    ent->vaddrAnimationMetadata,
                    ent->vaddrTextureToPaletteLookup,
                    NULL,
                    entityAssetDescriptors[ent->entityAssetIndex].flags & ENTITY_ASSET_IS_TYPE_1_SPRITE == TRUE,
                    ent->transformExempt                    
                    );
                
            }

            if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                
                dmaSprite(ent->shadowSpriteIndex, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].romTextureStart, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].romTextureEnd, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].romAssetsIndexStart, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].romAssetsIndexEnd, 
                    NULL, 
                    NULL, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].vaddrSpritesheet, 
                    NULL, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].vaddrPalette, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].vaddrUnknownAssetSheet, 
                    shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].vaddrAnimationMetadata, 
                    NULL, 
                    NULL, 
                    ent->transformExempt
                    );

                setSpriteBlendMode(ent->shadowSpriteIndex, SPRITE_BLEND_ALPHA_MODULATED);
            
            }

            ent->animationIndices.nextAnimationIndex = 0xFFFF;
            ent->flags |= ENTITY_INITIALIZED;

            setBilinearFiltering(ent->globalSpriteIndex, TRUE);
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

bool initializeShadowSprite(u16 index, u32 romTextureStart, u32 romTextureEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u32* vaddrSpritesheet, u16* vaddrPalette, u16* vaddrUnknownAssetSheet, u16* vaddrAnimationMetadata, u16 animationIndex, u8 frameIndex) {

    bool result = FALSE;
    
    if (index < MAX_SHADOW_SPRITES) {
        result = TRUE;
        shadowSpriteDescriptors[index].romTextureStart = romTextureStart;
        shadowSpriteDescriptors[index].romTextureEnd = romTextureEnd;
        shadowSpriteDescriptors[index].romAssetsIndexStart = romAssetsIndexStart;
        shadowSpriteDescriptors[index].romAssetsIndexEnd = romAssetsIndexEnd;
        shadowSpriteDescriptors[index].vaddrSpritesheet = vaddrSpritesheet;
        shadowSpriteDescriptors[index].vaddrPalette = vaddrPalette;
        shadowSpriteDescriptors[index].vaddrUnknownAssetSheet = vaddrUnknownAssetSheet;
        shadowSpriteDescriptors[index].vaddrAnimationMetadata = vaddrAnimationMetadata;
        shadowSpriteDescriptors[index].animationIndex = animationIndex;
        shadowSpriteDescriptors[index].frameIndex = frameIndex;
    }

    return result;
    
}

bool setEntityTrackingTarget(u16 index, u16 targetEntityIndex, u16 trackingMode) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {

            ent->targetEntityIndex = targetEntityIndex;
            ent->trackingMode = trackingMode;

            if (targetEntityIndex == 0xFFFF) {
                ent->flags &= ~ENTITY_TRACKING_ACTIVE;
            } else {
                ent->flags |= ENTITY_TRACKING_ACTIVE;
                ent->flags &= ~ENTITY_PAUSED;
            }

            result = TRUE;
        }
    }
     
    return result;
}

// likely set a palette index if shared asset has multiple palettes (animals, itemHandlers, and cutscenes)
bool setEntityPaletteIndex(u16 index, u16 paletteIndex) {
    Entity *ent = &entities[index];

    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if (ent->flags & ENTITY_ACTIVE) {
            ent->paletteIndex = paletteIndex;
            ent->flags |= ENTITY_PALETTE_SET;
            result = TRUE; 
        }
    }
    
    return result;

}

// used by animals, npcs, item handlers, and cutscenes
bool setEntityAttachmentOffset(u16 index, s16 x, s16 y, s16 z) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {
            result = TRUE;
            ent->attachmentOffset.x = x;
            ent->attachmentOffset.y = y;
            ent->attachmentOffset.z = z;
        }
    }
    
    return result;
    
}

// used by level.c and cutscene.c
bool setMainMapIndex(u16 mapIndex) {

    bool result = FALSE;

    if ((mapIndex == MAIN_MAP_INDEX) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_INITIALIZED) && (mapControllers[mapIndex].flags & MAP_CONTROLLER_ASSETS_LOADED)) {
        gMainMapIndex = mapIndex;
        result = TRUE;
    }

    return result;
    
}

bool setEntityViewSpacePosition(u16 entityIndex, f32 x, f32 y, f32 z) {
    Entity *ent = &entities[entityIndex];


    bool set = FALSE;

    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {
            setSpriteViewSpacePosition(ent->globalSpriteIndex, x, y, z);
            set = TRUE;
        }
        
    }
    
    return set;

}

bool adjustEntityViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {
            adjustSpriteViewSpacePosition(ent->globalSpriteIndex, x, y, z);
            result = TRUE;
        }
    }
    
    return result;
    
}

bool setEntityColor(u16 index, u8 r, u8 g, u8 b, u8 a) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {
            
            setSpriteColor(ent->globalSpriteIndex, r, g, b, a);
            
            if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {

                setSpriteColor(ent->shadowSpriteIndex, r, g, b, SHADOW_ALPHA);
                
            }

            result = TRUE;
        }
    }
    
    return result;
    
}

bool setEntityShadow(u16 index, u8 shadowSpriteIndex) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {
            
            entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex = shadowSpriteIndex;

            if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex == 0xFF) {

                resetAnimationState(ent->shadowSpriteIndex);

            }

            result = TRUE;
        }
    }
    
    return result;
    
}

bool updateEntityRGBA(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {
    Entity *ent = &entities[index];


    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {
        
        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {

            updateSpriteRGBA(ent->globalSpriteIndex, r, g, b, a, rate);

            if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                updateSpriteRGBA(ent->shadowSpriteIndex, r, g, b, SHADOW_ALPHA, rate);
            }
            
            result = TRUE;
        }
    }

    return result;

}

bool setEntityAnimation(u16 index, u16 animationIndex) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED) && !(ent->flags & ENTITY_LOAD_PENDING)) {

            ent->animationIndices.animationIndex = animationIndex;
            
            ent->flags |= ENTITY_IS_CURRENTLY_ANIMATED;
            ent->flags &= ~ENTITY_ANIMATION_TRACKS_DIRECTION; 

            globalSprites[ent->globalSpriteIndex].stateFlags &= ~SPRITE_ANIMATION_CYCLE_ENDED;
            globalSprites[ent->globalSpriteIndex].audioTrigger = FALSE;
            
            result = TRUE;
            
        }
    }
    
    return result;
    
}

bool setEntityDirectionalAnimation(u16 index, u16 animationIndex) {
    Entity *ent = &entities[index];

    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED) && !(ent->flags & ENTITY_LOAD_PENDING)) {     
        
            ent->animationIndices.animationIndex = animationIndex;
            ent->flags |= (ENTITY_IS_CURRENTLY_ANIMATED | ENTITY_ANIMATION_TRACKS_DIRECTION);
    
            globalSprites[ent->globalSpriteIndex].stateFlags &= ~SPRITE_ANIMATION_CYCLE_ENDED;
    
            result = TRUE;
    
            globalSprites[ent->globalSpriteIndex].audioTrigger = FALSE;
    
        }
        
    }
    
    return result;

}

bool loadPendingEntity(u16 index) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED) && (ent->flags & ENTITY_LOAD_PENDING)) {
            
            loadEntity(index, ent->entityAssetIndex, ent->transformExempt);

            ent->flags |= ENTITY_IS_CURRENTLY_ANIMATED;
            ent->flags &= ~ENTITY_LOAD_PENDING;

            result = TRUE;

        } 
        
    }
    
    return result;
    
}

bool pauseEntityLoad(u16 index) {
    Entity *ent = &entities[index];


    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED)) {
            
            if (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED) {

                setEntityMovementVector(index, 0.0f, 0.0f, 0.0f, 0.0f);
                
                temp = ent->flags & ENTITY_PALETTE_SET;
                ent->flags |= ENTITY_LOAD_PENDING;

                deactivateEntity(index);
                
                ent->flags |= temp;
                
                result = TRUE;
        
            } else {
                ent->flags &= ~ENTITY_LOAD_PENDING;
            }

        }

    }
    
    return result;
    
}

bool setEntityDirection(u16 index, u8 direction) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_INITIALIZED) && !(ent->flags & ENTITY_LOAD_PENDING)) {
        
            ent->direction = direction;
            result = TRUE;

        } 
    }
    
    return result;
    
}

void pauseEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        pauseEntity(i);
    }
    
}

void unpauseEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        unpauseEntity(i);
    }
    
}

// increment/decrement rotation; used by map controller
void rotateAllEntities(s16 arg0) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        Entity *ent = &entities[i];

        ent->direction += arg0;
        ent->direction &= MAX_DIRECTIONS;
    }

}

void setEntitiesRGBA(u8 r, u8 g, u8 b, u8 a) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        Entity *ent = &entities[i];


        if (ent->flags & ENTITY_ACTIVE && ent->flags & ENTITY_IS_CURRENTLY_ANIMATED) {

            if (i < MAX_ENTITIES && ent->flags & ENTITY_INITIALIZED) {

                setSpriteColor(ent->globalSpriteIndex, r, g, b, a);

                if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                    setSpriteColor(ent->shadowSpriteIndex, r, g, b, SHADOW_ALPHA);
                }

            }

        }

    }

}

void setEntitiesRGBAWithTransition(u8 r, u8 g, u8 b, u8 a, s16 arg4) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        Entity *ent = &entities[i];


        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
        
            if (i < MAX_ENTITIES && ent->flags & ENTITY_INITIALIZED) {
        
                updateSpriteRGBA(ent->globalSpriteIndex, r, g, b, a, arg4);
        
                if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                    updateSpriteRGBA(ent->shadowSpriteIndex, r, g, b, SHADOW_ALPHA, arg4);
                }
        
            }

        }

    }

}

bool deactivateEntity(u16 index) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
            
            ent->flags &= ~( 0x8 | ENTITY_PALETTE_SET);
            deactivateSprite(ent->globalSpriteIndex);
            
            if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                deactivateSprite(ent->shadowSpriteIndex);
            }

            result = TRUE;

        }

    }

    return result;

}

// unused
// void deactivateEntities(void) {

//     u16 i;

//     for (i = 0; i < MAX_ENTITIES; i++) {
//         deactivateEntity(i);
//     }
    
// }

// called by pause function
void pauseAllEntityLoads(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        pauseEntityLoad(i);
    }

}

void loadAllPendingEntities(void) {

    u16 i;

    for (i = 0; i < MAX_ENTITIES; i++) {
        loadPendingEntity(i);
    }

}

bool pauseEntity(u16 index) {
    Entity *ent = &entities[index];


    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {

        if (ent->flags & ENTITY_ACTIVE) {
            
            ent->flags |= ENTITY_PAUSED;
            pauseSpriteAnimation(ent->globalSpriteIndex);
            
            result = TRUE;

        }
    }

    return result;
    
}

bool unpauseEntity(u16 index) {
    Entity *ent = &entities[index];

    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if (ent->flags & ENTITY_ACTIVE) {
        
            ent->flags &= ~ENTITY_PAUSED;
            resumeSpriteAnimation(ent->globalSpriteIndex);
        
            result = TRUE;
        
        }

    }

    return result;
    
}

bool setEntityMapSpaceIndependent(u16 index, bool flag) {
    Entity *ent = &entities[index];

    
    bool result = FALSE;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED)) {

            if (flag) {
                ent->flags &= ~ENTITY_MAP_SPACE_INDEPENDENT;
            } else {
                ent->flags |= ENTITY_MAP_SPACE_INDEPENDENT;
            }

            result = TRUE;

        }
    
    }

    return result;
    
}

// only used by main game loop callback
bool checkEntityMapSpaceDependent(u16 index) {
    Entity *ent = &entities[index];

    
    bool result = FALSE;
    u16 temp;

    if (index < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
            temp = ent->flags & ENTITY_MAP_SPACE_INDEPENDENT;
            result = !temp;
        }

    }

    return result;

}

bool setEntityCoordinates(u16 entityIndex, f32 x, f32 y, f32 z) {
    Entity *ent = &entities[entityIndex];


    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_LOAD_PENDING)) {
            
            ent->coordinates.x = x;
            ent->coordinates.y = y;
            ent->coordinates.z = z;
            
            result = TRUE;

        }
    }

    return result;
    
}

// manually set movement vector 
// animals, player, item handlers, cutscene
bool setEntityMovementVector(u16 entityIndex, f32 x, f32 y, f32 z, f32 arg4) {
    Entity *ent = &entities[entityIndex];


    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED) && !(ent->flags & ENTITY_LOAD_PENDING)) {
            
            ent->movementVector.x = x;
            ent->movementVector.y = y;
            ent->movementVector.z = z;
            ent->speed = arg4 / 2.0f;
            
            result = TRUE;

        }

    }

    return result;
    
}

bool checkEntityAnimationStateChanged(u16 index) {
    Entity *ent = &entities[index];


    bool result = FALSE;
    
    if (index < MAX_ENTITIES) {

        if (ent->flags & ENTITY_ACTIVE) {
            result = checkSpriteAnimationCycleEnded(ent->globalSpriteIndex);
        }

    }

    return result;

}

bool setCameraTrackingEntity(u16 entityIndex, u16 flag) {
    Entity *ent = &entities[entityIndex];


    bool result = FALSE;

    if (entityIndex < MAX_ENTITIES) {

        if (ent->flags & ENTITY_ACTIVE) { 

            if (flag) {
                ent->flags |= ENTITY_CAMERA_TRACKING;
            } else {
                ent->flags &= ~ENTITY_CAMERA_TRACKING;
            }

            result = TRUE;

        }
    }

    return result;

}

bool setEntityCollisionBuffers(u16 entityIndex, u8 xValue, u8 yValue) {
    
    bool result = FALSE;
    
    if (entityIndex < MAX_ENTITY_ASSETS) {
    
        if (entityAssetDescriptors[entityIndex].flags & ENTITY_ASSET_ACTIVE) {
            entityAssetDescriptors[entityIndex].collisionBufferX = xValue;
            entityAssetDescriptors[entityIndex].collisionBufferY = yValue;
            result = TRUE;
        }

    }
    
    return result;

}

bool setEntitySpriteDimensions(u16 entityAssetIndex, u8 spriteWidth, u8 spriteHeight) {

    bool result = FALSE;

    if (entityAssetIndex < MAX_ENTITY_ASSETS) {

        if (entityAssetDescriptors[entityAssetIndex].flags & ENTITY_ASSET_ACTIVE) {
            entityAssetDescriptors[entityAssetIndex].spriteWidth = spriteWidth;
            entityAssetDescriptors[entityAssetIndex].spriteHeight = spriteHeight;
            result = TRUE;
        }

    }

    return result;

}

bool setEntityYMovement(u16 index, bool flag) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (flag == TRUE) {
        
        ent->flags &= ~ENTITY_Y_MOVEMENT;
        result = TRUE;

    } else {

        if (index < MAX_ENTITIES && ent->flags & ENTITY_ACTIVE) {
            ent->flags |= ENTITY_Y_MOVEMENT;
            result = TRUE;
        }

    }

    return result;
    
}

// only used by farm animals and misc. animals
bool setEntityHandlesMultipleCollisions(u16 index, u8 flag) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (flag == TRUE) {
        ent->flags &= ~ENTITY_SINGLE_COLLISION_CHECK;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && ent->flags & ENTITY_ACTIVE) {
            ent->flags |= ENTITY_SINGLE_COLLISION_CHECK;
            result = TRUE;
        }
    }

    return result;
    
}

bool setEntityCollidable(u16 index, bool flag) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (flag == TRUE) {
        ent->flags &= ~ENTITY_NON_COLLIDABLE;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && ent->flags & ENTITY_ACTIVE) {
            ent->flags |= ENTITY_NON_COLLIDABLE;
            result = TRUE;
        }
    }

    return result;
    
}

bool setEntityTracksCollisions(u16 index, bool flag) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (flag == TRUE) {
        ent->flags &= ~ENTITY_COLLISION_EXEMPT;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && ent->flags & ENTITY_ACTIVE) {
            ent->flags |= ENTITY_COLLISION_EXEMPT;
            result = TRUE;
        }
    }

    return result;
    
}

bool enableEntityMovement(u16 index, bool flag) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (flag == TRUE) {
        ent->flags &= ~ENTITY_MOVEMENT_DISABLED;
        result = TRUE;
    } else {
        if (index < MAX_ENTITIES && ent->flags & ENTITY_ACTIVE) {
            ent->flags |= ENTITY_MOVEMENT_DISABLED;
            result = TRUE;
        }
    }

    return result;
    
}

bool checkEntityPaused(u16 index) {
    Entity *ent = &entities[index];


    bool result = FALSE;

    if (index < MAX_ENTITIES) {
        
        if (ent->flags & ENTITY_ACTIVE) {
            result = (ent->flags >> 6) & 1;
        }

    }
    
    return result;

}

u16 checkEntityToEntityCollision(u16 entityIndex, f32 x, f32 z, u16 buttonPressed, u16 entityCount) {
    Entity *ent = &entities[entityIndex];


    u16 i;
    u16 count = 0;
    
    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_PAUSED)) {

            for (i = 0; i < entityCount; i++) {
                
                if (i != entityIndex) {
                    
                    if (!(entities[i].flags & ENTITY_COLLISION_EXEMPT)) {

                        count = detectEntityOverlap(ent, i, x, z, entityAssetDescriptors[ent->entityAssetIndex].spriteWidth, entityAssetDescriptors[ent->entityAssetIndex].spriteHeight);
                        
                        if (count) {

                            entities[i].entityCollidedWithIndex = entityIndex;
                            // seems to imply plans to support different interactions with entities based on button pressed
                            entities[i].buttonPressed = buttonPressed;
                            ent->collision = i;
                            
                            i = MAX_ENTITIES;
                            
                        } else {

                            entities[i].entityCollidedWithIndex = 0xFFFF;
                            entities[i].buttonPressed = 0;
                            ent->collision = 0xFFFF;
                            
                        }

                    }

                }

            }   

        }

    }
    
    return count;
    
}

u16 checkEntityProximity(u16 index, f32 x, f32 z, u16 buttonPressed) {
    Entity *ent = &entities[index];


    Vec3f worldCoordinates;
    Vec3f rotatedPosition;
    Vec3f rotationAngles;
    
    u16 result = 0;
    bool set = FALSE;
    u32 temp;
    u16 i;

    if (index < MAX_ENTITIES) {
        
        if (ent->flags & ENTITY_ACTIVE) {
            
            if (!(ent->flags & ENTITY_PAUSED)) {
                
                worldCoordinates.x = x;
                worldCoordinates.y = 0;
                worldCoordinates.z = z;

                rotationAngles.x = 0;
                // FIXME: macro not working; should be convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex)
                rotationAngles.y = getSpriteYValueFromDirection((ent->direction + getCurrentMapRotation(gMainMapIndex)) % 8);
                rotationAngles.z = 0;

                rotateVector3D(worldCoordinates, &rotatedPosition, rotationAngles);

                for (i = 0; i < MAX_ENTITIES; i++) {

                    if (i != index && !(entities[i].flags & ENTITY_TRACKING_ACTIVE)) {
                        
                        if (temp = detectEntityOverlap(ent, i, rotatedPosition.x, rotatedPosition.z, entityAssetDescriptors[ent->entityAssetIndex].spriteWidth, entityAssetDescriptors[ent->entityAssetIndex].spriteHeight)) {
                           
                            result |= temp;
                            
                            if (checkButtonPressed(CONTROLLER_1, buttonPressed) || !buttonPressed) {

                                if (!set) {

                                    entities[i].entityCollidedWithIndex = index;
                                    entities[i].buttonPressed = buttonPressed;

                                    if (!(entities[i].flags & ENTITY_COLLISION_EXEMPT)) {
                                        ent->collision = i;    
                                    } else {
                                        ent->collision = 0xFFFF;
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
                                ent->collision = 0xFFFF;
                            }
                            
                        }       

                    }

                }

            }

        }

    }
    
    return result;
   
}

// unused; check if position has level interaction
// bool checkEntityInteractionAtPosition(u16 entityIndex, f32 arg1, f32 arg2) {
//     Entity *ent = &entities[entityIndex];

    
//     bool result = FALSE; 

//     if (entityIndex < MAX_ENTITIES) {

//         if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_PAUSED)) {
//             result = getLevelInteractionIndexFromPosition(mapControllers[gMainMapIndex].mainMapIndex, ent->coordinates.x + arg1, ent->coordinates.z + arg2);
//         }

//     }

//     return result;
    
// }

u8 getLevelInteractionIndexFromEntityPosition(u16 entityIndex, f32 x, f32 z) {
    Entity *ent = &entities[entityIndex];

    
    Vec3f worldCoordinates;
    Vec3f rotatedPosition;
    Vec3f rotation;

    u8 levelInteractionIndex = 0;

    if ((entityIndex < MAX_ENTITIES)) {

        if ((ent->flags & ENTITY_ACTIVE)) {
            
            if (!(ent->flags & ENTITY_PAUSED) && !(ent->flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

                worldCoordinates.x = x;
                worldCoordinates.y = 0;
                worldCoordinates.z = z;

                rotation.x = 0;
                rotation.y = getSpriteYValueFromDirection(convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex));
                rotation.z = 0;
                
                rotateVector3D(worldCoordinates, &rotatedPosition, rotation);

                levelInteractionIndex = getLevelInteractionIndexFromPosition(mapControllers[gMainMapIndex].mainMapIndex, ent->coordinates.x + rotatedPosition.x, ent->coordinates.z + rotatedPosition.z);
                
            }
        }
        
    } 
    
    return levelInteractionIndex;
    
}

// returns param 2 of startSpriteAnimation
inline u16 getEntityAnimationOffset(u16 entityIndex, u16 offset) {
    Entity *ent = &entities[entityIndex];


    u16 animationIndex = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (ent->flags & ENTITY_ACTIVE) {
            animationIndex = entityAssetDescriptors[ent->entityAssetIndex].animationScripts[offset] & 0x1FFF;
        }
    }
    
    return animationIndex;
    
}

// alternate get animation offset from animation scripts
// used by shop and overlay screens
u16 getAnimationOffsetFromScript(u16* vaddr, u16 offset) {
    return vaddr[offset] & 0x1FFF;
}

inline u16 getEntityAnimationType(u16 entityIndex, u16 offset) {
    Entity *ent = &entities[entityIndex];


    u16 flags = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (ent->flags & ENTITY_ACTIVE) {
            flags = entityAssetDescriptors[ent->entityAssetIndex].animationScripts[offset] & 0x6000;
        }
    }
    
    return flags;
    
}

inline u16 getEntityAnimationHorizontalFlip(u16 entityIndex, u16 offset) {
    Entity *ent = &entities[entityIndex];


    u16 flipHorizontal = 0xFFFF;

    if (entityIndex < MAX_ENTITIES) {
        if (ent->flags & ENTITY_ACTIVE) {
            flipHorizontal = entityAssetDescriptors[ent->entityAssetIndex].animationScripts[offset] & 0x8000;
        }
    }
    
    return flipHorizontal;
    
}

// unused or inline
// u16 getGroundObjectAtEntityPosition(u16 entityIndex, f32 x, f32 z) {

//     Vec3f vec;

//     u16 index = 0xFFFF;

//     if (entityIndex < MAX_ENTITIES) {
        
//         if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
            
//             vec = getEntityRelativeGroundObjectCoords(entityIndex, x, z);

//             if (vec.y != 65535.0f) {
//                 index = getMapGroundObjectSpriteIndex(mapControllers[gMainMapIndex].mainMapIndex, vec.x, vec.z);
//             }
            
//         }
//     }
    
//     return index;

// }

// unused or inline
// map ground objects
// bool setGroundObjectAtEntityPosition(u16 entityIndex, f32 arg1, f32 arg2, u16 spriteIndex) {

//     Vec3f vec;
    
//     if ((entityIndex < MAX_ENTITIES) && (entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
            
//         vec = getEntityRelativeGroundObjectCoords(entityIndex, arg1, arg2);
        
//         if (vec.y != 65535.0f) {
//             setMapGroundObjectSpriteIndexFromFloat(mapControllers[gMainMapIndex].mainMapIndex, spriteIndex, vec.x, vec.z);
//         }
        
//     }
    
//     return 0;

// }

Vec3f getEntityRelativeGroundObjectCoords(u16 entityIndex, f32 arg2, f32 arg3) {
    Entity *ent = &entities[entityIndex];


    Vec3f vec;
    Vec3f rotatedPosition;
    Vec3f rotation;
    Vec3f vec4;

    vec4.x = 0.0f;
    vec4.y = 65535.0f;
    vec4.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_PAUSED) && !(ent->flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            vec.x = arg2;
            vec.y = 0;
            vec.z = arg3;

            rotation.x = 0;
            rotation.y = getSpriteYValueFromDirection(convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex));
            rotation.z = 0;

            rotateVector3D(vec, &rotatedPosition, rotation);

            vec4 = getMapGroundObjectCoordinates(mapControllers[gMainMapIndex].mainMapIndex, ent->coordinates.x + rotatedPosition.x, ent->coordinates.z + rotatedPosition.z);
            
        }
        
    }

    return vec4;
    
}

// unused
// u16 getTerrainAtEntityPosition(u16 entityIndex, f32 x, f32 z) {

//     Vec3f tileCoordinates;
    
//     u16 terrainIndex = 0xFFFF;

//     if (entityIndex < MAX_ENTITIES) {

//         if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

//             tileCoordinates = getEntityRelativeTilePosition(entityIndex, x, z);

//             if (tileCoordinates.y != 65535.0f) {   
//                 terrainIndex = getTileIndexFromGrid(mapControllers[gMainMapIndex].mainMapIndex, tileCoordinates.x, tileCoordinates.z);
//             }
//         }
    
//     }

//     return terrainIndex;
    
// }

// unused or inline
// bool setMapAdditionAtEntityPosition(u16 entityIndex, f32 arg1, f32 arg2, u16 arg3) {

//     Vec3f tileCoordinates;
    
//     bool set = FALSE;

//      if (entityIndex < MAX_ENTITIES) {
        
//         if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

//             tileCoordinates = getEntityRelativeTilePosition(entityIndex, arg1, arg2);

//             if (tileCoordinates.y != 65535.0f) {

//                 // set map additions
//                 initializeMapAdditionAtPosition(mapControllers[gMainMapIndex].mainMapIndex, arg3, tileCoordinates.x, tileCoordinates.z);
//                 set = TRUE;

//             }
            
//         }
//      }
    
//     return set;
    
// }

bool checkEntityShouldPlaySoundEffect(u16 entityIndex) {
    Entity *ent = &entities[entityIndex];

    
    bool result = FALSE; 

    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_PAUSED)) {
            result = globalSprites[ent->globalSpriteIndex].audioTrigger;
        }

    }

    return result;

}

Vec3f getEntityRelativeTilePosition(u16 entityIndex, f32 x, f32 z) {
    Entity *ent = &entities[entityIndex];


    Vec3f worldCoordinates;
    Vec3f rotatedCoordinates;
    Vec3f rotation;
    Vec3f tileCoordinates;

    tileCoordinates.x = 0.0f;
    tileCoordinates.y = 65535.0f;
    tileCoordinates.z = 0.0f;

    if (entityIndex < MAX_ENTITIES) {

        if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_PAUSED) && !(ent->flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

            worldCoordinates.x = x;
            worldCoordinates.y = 0;
            worldCoordinates.z = z;

            rotation.x = 0;
            rotation.y = getSpriteYValueFromDirection(convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex));
            rotation.z = 0;

            rotateVector3D(worldCoordinates, &rotatedCoordinates, rotation);

            tileCoordinates = convertWorldToTileCoordinates(mapControllers[gMainMapIndex].mainMapIndex, ent->coordinates.x + rotatedCoordinates.x, ent->coordinates.z + rotatedCoordinates.z);
            
        }
        
    }

    return tileCoordinates;
    
}

Vec3f getEntityTileCoordinates(u16 entityIndex) {
    Entity *ent = &entities[entityIndex];


    Vec3f tileCoordinates;

    tileCoordinates.x = 0;
    tileCoordinates.z = 0;
    tileCoordinates.y = 65535.0f;
    
    if (entityIndex < MAX_ENTITIES) {
        if ((ent->flags & ENTITY_ACTIVE) && !(ent->flags & ENTITY_PAUSED) && !(ent->flags & ENTITY_MAP_SPACE_INDEPENDENT)) {
            tileCoordinates = convertWorldToTileCoordinates(mapControllers[gMainMapIndex].mainMapIndex, ent->coordinates.x, ent->coordinates.z);
        } 
    }

    return tileCoordinates;

}

// unused
// Vec3f getEntityRelativeTileCoords(u16 entityIndex, f32 x, f32 z) {

//     Vec3f position;
//     Vec3f rotatedCoordinates;
//     Vec3f rotation;
//     Vec3f tileCoordinates;

//     tileCoordinates.x = 0.0f;
//     tileCoordinates.y = 65535.0f;
//     tileCoordinates.z = 0.0f;

//     if (entityIndex < MAX_ENTITIES) {

//         if ((entities[entityIndex].flags & ENTITY_ACTIVE) && !(entities[entityIndex].flags & ENTITY_PAUSED) && !(entities[entityIndex].flags & ENTITY_MAP_SPACE_INDEPENDENT)) {

//             position.x = x;
//             position.y = 0;
//             position.z = z;

//             rotation.x = 0;
//             rotation.y = getSpriteYValueFromDirection(convertScreenDirectionToWorldDirection(entities[entityIndex].direction, gMainMapIndex));
//             rotation.z = 0;

//             rotateVector3D(position, &rotatedCoordinates, rotation);
//             tileCoordinates = convertWorldToTileCoordinates(mapControllers[gMainMapIndex].mainMapIndex, entities[entityIndex].coordinates.x, entities[entityIndex].coordinates.z);
            
//             tileCoordinates.x += rotatedCoordinates.x;
//             tileCoordinates.z += rotatedCoordinates.z;

//         }
        
//     }

//     return tileCoordinates;
    
// }

bool checkTerrainCollisionInDirection(u16 index, s16 moveDistance, u8 direction) {
    Entity *ent = &entities[index];


    Vec3f projectedPosition;
    
    f32 terrainHeight;
    f32 entityTopBound;
    f32 entityLowerBound;
    
    bool result = FALSE;
    
    u8 isGroundTooLow;
    u8 isGroundTooHigh;
    
    entityTopBound = ent->coordinates.y + 24.0f;
    entityLowerBound = ent->coordinates.y - 24.0f;
    
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

Vec3f projectEntityPosition(u16 index, s16 zDisplacement, u8 direction) {
    Entity *ent = &entities[index];


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

    offset.x = ent->coordinates.x + rotatedOffset.x;
    offset.y = ent->coordinates.y + rotatedOffset.y;
    offset.z = ent->coordinates.z + rotatedOffset.z;

    return offset;
    
}

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

u8 detectEntityOverlap(Entity* entity, u16 entityIndex, f32 deltaX, f32 deltaZ, u16 entityWidth, u16 entityHeight) {
    Entity *ent = &entities[entityIndex];


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
    
    if (entityIndex < MAX_ENTITIES && ent->flags & ENTITY_ACTIVE && ent->flags & ENTITY_IS_CURRENTLY_ANIMATED) { 

        adjustedX = entity->coordinates.x + deltaX;
        adjustedZ = entity->coordinates.z + deltaZ;

        if (entityAssetDescriptors[ent->entityAssetIndex].spriteWidth < entityWidth) {
            dimensionX = entityWidth;
        } else {
            dimensionX = entityAssetDescriptors[ent->entityAssetIndex].spriteWidth;
        }

        if (entityAssetDescriptors[ent->entityAssetIndex].spriteHeight < entityHeight) {
            dimensionZ = entityHeight;
        } else {
            dimensionZ = entityAssetDescriptors[ent->entityAssetIndex].spriteHeight;
        }

        if (adjustedX <= ent->coordinates.x) {
            
            xBoundaryHalf = (2.0f * dimensionX) - 1.0f;
            distanceX = (ent->coordinates.x - adjustedX) + xBoundaryHalf;

            if (deltaX < 0.0f) {
                counter++;
            }
            
            if (deltaX == 0.0f) {
                counter++;
            }
            
        } else {

            xBoundaryHalf = (2.0f * dimensionX) - 1.0f;
            distanceX = (adjustedX - ent->coordinates.x) + xBoundaryHalf;
        
            if (0.0f < deltaX) {
                counter++;
            }
            
            if (deltaX == 0.0f) {
                counter++;
            }
            
        }

        if (adjustedZ <= ent->coordinates.z) {
            
            zBoundaryHalf = (2.0f * dimensionZ) - 1.0f;
            distanceZ = (ent->coordinates.z - adjustedZ) + zBoundaryHalf;
            
            if (deltaZ < 0.0f) {
                counter++;
            }
            
             if (deltaZ == 0.0f) {
                counter++;
            }   
            
        } else {

            zBoundaryHalf = (2.0f * dimensionZ) - 1.0f;
            distanceZ = (adjustedZ -  ent->coordinates.z) + zBoundaryHalf; 

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

// unused or inline
// u16 attemptEntityMovement(u16 index) {

//     Vec3f projectedPosition;

//     u16 collisionFlags = checkTerrainMovementCollision(&entities[index], entities[index].movementVector.x, entities[index].movementVector.z, convertScreenDirectionToWorldDirection(entities[index].direction, gMainMapIndex));

//     if (collisionFlags) {

//         projectedPosition = getMovementVectorFromDirection(entities[index].speed, convertScreenDirectionToWorldDirection(entities[index].direction, gMainMapIndex), 0.0f);

//         collisionFlags = checkTerrainMovementCollision(&entities[index], projectedPosition.x, projectedPosition.z, convertScreenDirectionToWorldDirection(entities[index].direction, gMainMapIndex));
        
//         if (!collisionFlags) {
//             entities[index].movementVector.x = projectedPosition.x;
//             entities[index].movementVector.z = projectedPosition.z;
//         }
        
//     }

//     return collisionFlags;
    
// }

void doEntityPathfinding(u16 index) {
    Entity *ent = &entities[index];

    
    Vec3f projectedPosition;
    u8 direction;

    u16 collisionFlags = checkTerrainMovementCollision(ent, ent->movementVector.x, ent->movementVector.z, convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex));

    if ((collisionFlags & 0xFF00) != 0xFF00) {

        if (collisionFlags & 0xF0F) {            
            
            // try right turn
            direction = (convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex) + 1) % 8;
    
            projectedPosition = getMovementVectorFromDirection(ent->speed, direction, 0.0f);
    
            if (checkTerrainMovementCollision(ent, projectedPosition.x, projectedPosition.z, direction)) {
                ent->movementVector.x = 0.0f;
                ent->movementVector.z = 0.0f; 
            } else {
                ent->movementVector.x = projectedPosition.x;
                ent->movementVector.z = projectedPosition.z;
            }
            
        }
        
        if (collisionFlags & 0xF0F0) {

            // try left turn
            direction = (convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex) + 7) % 8;

            projectedPosition = getMovementVectorFromDirection(ent->speed, direction, 0.0f);

            if (checkTerrainMovementCollision(ent, projectedPosition.x, projectedPosition.z, direction)) {
                ent->movementVector.x = 0.0f;
                ent->movementVector.z = 0.0f; 
            } else {
                ent->movementVector.x = projectedPosition.x;
                ent->movementVector.z = projectedPosition.z;
            }
       
        }
        
    } else {
    
        projectedPosition = getMovementVectorFromDirection(ent->speed, convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex), 0.0f);

        if (checkTerrainMovementCollision(ent, projectedPosition.x, projectedPosition.z, convertScreenDirectionToWorldDirection(ent->direction, gMainMapIndex)) & 0xFFFF) {
            ent->movementVector.x = 0.0f;
            ent->movementVector.z = 0.0f;
        } else {
            ent->movementVector.x = projectedPosition.x;
            ent->movementVector.z = projectedPosition.z;
        }

    }
    
}

void updateEntityPhysics(u16 index) {
    Entity *ent = &entities[index];


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

    ent->flags &= ~ENTITY_TOUCHING_GROUND;

    if (ent->flags & ENTITY_TRACKING_ACTIVE) {

        if (!(mapControllers[gMainMapIndex].flags & (MAP_CONTROLLER_ROTATING_COUNTERCLOCKWISE | MAP_CONTROLLER_ROTATING_CLOCKWISE))) {

            if (ent->trackingMode == 0xFE || ent->trackingMode == 0xFF) {
                
                rotation.x = 0.0f;
                rotation.y = getSpriteYValueFromDirection((entities[ent->targetEntityIndex].direction + mapControllers[gMainMapIndex].rotation) % 8);
                rotation.z = 0.0f;
                
            } else {

                rotation.x = 0.0f;
                rotation.y = getSpriteYValueFromDirection((mapControllers[gMainMapIndex].rotation + ent->trackingMode) % 8);
                rotation.z = 0.0f;

            }

            rotateVector3D(ent->attachmentOffset, &rotatedPosition, rotation);

            ent->viewSpacePosition.x = entities[ent->targetEntityIndex].viewSpacePosition.x + rotatedPosition.x;
            ent->viewSpacePosition.y = entities[ent->targetEntityIndex].viewSpacePosition.y + rotatedPosition.y;
            ent->viewSpacePosition.z = entities[ent->targetEntityIndex].viewSpacePosition.z + rotatedPosition.z;

            ent->coordinates.x = entities[ent->targetEntityIndex].coordinates.x + rotatedPosition.x;
            ent->coordinates.y = entities[ent->targetEntityIndex].coordinates.y + rotatedPosition.y;
            ent->coordinates.z = entities[ent->targetEntityIndex].coordinates.z + rotatedPosition.z;

        }
        
        shadowHeight = getTerrainHeightAtPosition(0, ent->coordinates.x, ent->coordinates.z);
        shadowHeight -= mapControllers[gMainMapIndex].viewPosition.y;
                
    } else {

        if (!(ent->flags & ENTITY_MOVEMENT_DISABLED)) {

            if ((ent->movementVector.x != 0.0f) || (ent->movementVector.y != 0.0f) || (ent->movementVector.z != 0.0f)) {
                doEntityPathfinding(index);
            }
        
        }

        if (ent->flags & ENTITY_SINGLE_COLLISION_CHECK) {
            collisionCheck = checkEntityToEntityCollision(index, ent->movementVector.x, ent->movementVector.z, 0, 1);        
        } else {
            collisionCheck = checkEntityToEntityCollision(index, ent->movementVector.x, ent->movementVector.z, 0, MAX_ENTITIES);
        }

        // FIXME: seems odd
        collisionCheck &= ent->flags & ENTITY_NON_COLLIDABLE ? 0 : 0xFFFF;

        if (!collisionCheck) {

            appliedMovement.x = ent->movementVector.x;
            appliedMovement.y = ent->movementVector.y;
            appliedMovement.z = ent->movementVector.z;
            
        }  else {
            
            appliedMovement.x = 0.0f;
            appliedMovement.y = 0.0f;
            appliedMovement.z = 0.0f;
            
        }

        ent->coordinates.x += appliedMovement.x;
        ent->coordinates.z += appliedMovement.z;

        if (ent->flags & ENTITY_CAMERA_TRACKING) {

            tempX = ent->coordinates.x;
            tempZ = ent->coordinates.z;

            if (!(ent->flags & ENTITY_Y_MOVEMENT)) {
                
                ent->coordinates.y = getTerrainHeightAtPosition(0, ent->coordinates.x, ent->coordinates.z);
                shadowHeight = ent->coordinates.y - mapControllers[gMainMapIndex].viewPosition.y;
                    
            } else {

                ent->coordinates.y += appliedMovement.y;
                tempZ += appliedMovement.y;
                
                shadowHeight = getTerrainHeightAtPosition(0, ent->coordinates.x, ent->coordinates.z) - ent->coordinates.y;
                
            }

            setMapControllerViewPosition(gMainMapIndex, tempX, ent->coordinates.y, tempZ);

            ent->viewSpacePosition.x = ent->coordinates.x - tempX;
            ent->viewSpacePosition.y = 0;
            ent->viewSpacePosition.z = ent->coordinates.z - tempZ;
            
        } else {

            if (!(ent->flags & ENTITY_Y_MOVEMENT)) {
                
                ent->coordinates.y = getTerrainHeightAtPosition(0, ent->coordinates.x, ent->coordinates.z);
                shadowHeight = ent->coordinates.y - mapControllers[gMainMapIndex].viewPosition.y;
                
            } else {
                
                ent->coordinates.y += appliedMovement.y;
                
                shadowHeight = getTerrainHeightAtPosition(0, ent->coordinates.x, ent->coordinates.z);

                if (ent->coordinates.y <= shadowHeight) {
                    ent->flags |= ENTITY_TOUCHING_GROUND;
                }

                shadowHeight -= mapControllers[gMainMapIndex].viewPosition.y;
                
            }

            ent->viewSpacePosition.x = ent->coordinates.x - mapControllers[gMainMapIndex].viewPosition.x;
            ent->viewSpacePosition.y = ent->coordinates.y - mapControllers[gMainMapIndex].viewPosition.y;
            ent->viewSpacePosition.z = ent->coordinates.z - mapControllers[gMainMapIndex].viewPosition.z;
            
        }
            
    }

    setSpriteViewSpacePosition(ent->globalSpriteIndex, ent->viewSpacePosition.x, ent->viewSpacePosition.y + ent->yOffset, ent->viewSpacePosition.z);

    if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
        setSpriteViewSpacePosition(ent->shadowSpriteIndex, ent->viewSpacePosition.x, shadowHeight, ent->viewSpacePosition.z);
    }
    
    ent->movementVector.x = 0.0f;
    ent->movementVector.y = 0.0f;
    ent->movementVector.z = 0.0f;
    
}

void updateEntities(void) {

    Vec3f tileCoordinates;
    
    u16 i = 0;
    
    u8 directionalOffset;
    u16 animationMetadataIndex;
    u16 animationType;
    u16 flipHorizontal;

    while (i < MAX_ENTITIES) {
        Entity *ent = &entities[i];


        if ((ent->flags & ENTITY_ACTIVE) && (ent->flags & ENTITY_IS_CURRENTLY_ANIMATED)) {
            
            ent->unk_66 = 0;

            if (!(ent->flags & ENTITY_PAUSED) || (ent->flags & ENTITY_TRACKING_ACTIVE)) {
                
                updateEntityPhysics(i);

                 if (ent->flags & ENTITY_TRACKING_ACTIVE) {
                    if (ent->trackingMode == 0xFE) {
                        ent->direction = entities[ent->targetEntityIndex].direction;
                    }
                }
                
            }

            if (ent->flags & ENTITY_ANIMATION_TRACKS_DIRECTION) {
                directionalOffset = ent->direction;
            } else {
                directionalOffset = 0;
            }

            animationMetadataIndex = getEntityAnimationOffset(i, ent->animationIndices.animationIndex + directionalOffset);
            animationType = getEntityAnimationType(i, ent->animationIndices.animationIndex + directionalOffset);
            flipHorizontal = getEntityAnimationHorizontalFlip(i, ent->animationIndices.animationIndex + directionalOffset);

            tileCoordinates = getTileCoordinates(&mainMap[MAIN_MAP_INDEX], 
                ent->coordinates.x + mainMap[MAIN_MAP_INDEX].mapGrid.tileSizeX / 2,
                ent->coordinates.z + mainMap[MAIN_MAP_INDEX].mapGrid.tileSizeZ / 2);
            
            if (mainMap[MAIN_MAP_INDEX].visibilityGrid[(u8)tileCoordinates.z][(u8)tileCoordinates.x] || ent->flags & ENTITY_CAMERA_TRACKING) {

                if ((directionalOffset + ent->animationIndices.animationIndex) != ent->animationIndices.nextAnimationIndex) {
                    
                    resetAnimationState(ent->globalSpriteIndex);

                    switch (animationType) {

                        case ANIM_TYPE_ONE_SHOT:
                            startSpriteAnimation(ent->globalSpriteIndex, animationMetadataIndex, 0xFF);
                            break;

                        case ANIM_TYPE_LOOP: 
                            startSpriteAnimation(ent->globalSpriteIndex, animationMetadataIndex, 0xFE);
                            break;
                        
                        case ANIM_TYPE_DESTROY_ON_END:
                            startSpriteAnimation(ent->globalSpriteIndex, animationMetadataIndex, 0xFD);
                            break;

                        default:
                            break;
                        
                    }

                    if (ent->flags & ENTITY_PALETTE_SET) {
                        setSpritePaletteIndex(ent->globalSpriteIndex, ent->paletteIndex);
                    }

                    if (flipHorizontal) {
                        setSpriteFlip(ent->globalSpriteIndex, TRUE, FALSE);
                    } else {
                        setSpriteFlip(ent->globalSpriteIndex, FALSE, FALSE);
                    }
                    
                    ent->animationIndices.nextAnimationIndex = ent->animationIndices.animationIndex + directionalOffset;
                          
                }

                if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {

                    if (globalSprites[ent->shadowSpriteIndex].viewSpacePosition.y <= ent->viewSpacePosition.y) {

                        startSpriteAnimation(ent->shadowSpriteIndex, 
                             shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].animationIndex,
                             shadowSpriteDescriptors[entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex].frameIndex);
                                             
                    } else {
                        resetAnimationState(ent->shadowSpriteIndex);
                    }
                    
                } else {
                    resetAnimationState(ent->shadowSpriteIndex);
                }

                if ((animationType == ANIM_TYPE_DESTROY_ON_END) && checkSpriteAnimationCycleEnded(ent->globalSpriteIndex)) {
                    deactivateEntity(i);                   
                }
                
            } else {

                ent->animationIndices.nextAnimationIndex = 0xFFFF;
                resetAnimationState(ent->globalSpriteIndex);

                if (entityAssetDescriptors[ent->entityAssetIndex].shadowSpriteIndex != 0xFF) {
                    resetAnimationState(ent->shadowSpriteIndex);
                }
                    
            }
             
        }

        i++;
        
    } 
    
}
