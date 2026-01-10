#include "common.h"

#include "game/npc.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/player.h"
#include "game/time.h"
#include "game/weather.h"

#include "assetIndices/dialogues.h"
#include "assetIndices/entities.h"
#include "assetIndices/maps.h"

#include "buffers/buffers.h"

#include "ld_symbols.h"

// bss
npcInfo npcs[MAX_NPCS];
u8 npcAffection[MAX_NPCS];
u8 npcTalkingTo;

// data
u16 npcToEntityAssetIndex[] = { 
    ENTITY_ASSET_MARIA, 
    ENTITY_ASSET_POPURI, 
    ENTITY_ASSET_ELLI, 
    ENTITY_ASSET_ANN, 
    ENTITY_ASSET_KAREN, 
    ENTITY_ASSET_BABY, 
    ENTITY_ASSET_HARRIS, 
    ENTITY_ASSET_GRAY, 
    ENTITY_ASSET_JEFF, 
    ENTITY_ASSET_CLIFF, 
    ENTITY_ASSET_KAI, 
    ENTITY_ASSET_MAYOR, 
    ENTITY_ASSET_MAYOR_WIFE, 
    ENTITY_ASSET_LILLIA, 
    ENTITY_ASSET_BASIL, 
    ENTITY_ASSET_ELLEN, 
    ENTITY_ASSET_DOUG, 
    ENTITY_ASSET_GOTZ, 
    ENTITY_ASSET_SASHA, 
    ENTITY_ASSET_POTION_SHOP_DEALER, 
    ENTITY_ASSET_KENT, 
    ENTITY_ASSET_STU, 
    ENTITY_ASSET_MIDWIFE, 
    ENTITY_ASSET_MAY, 
    ENTITY_ASSET_RICK, 
    ENTITY_ASSET_PASTOR, 
    ENTITY_ASSET_SHIPPER, 
    ENTITY_ASSET_SAIBARA, 
    ENTITY_ASSET_DUKE, 
    ENTITY_ASSET_GREG, 
    ENTITY_ASSET_ASSISTANT_CARPENTER, 
    ENTITY_ASSET_ASSISTANT_CARPENTER, 
    ENTITY_ASSET_MASTER_CARPENTER, 
    ENTITY_ASSET_HARVEST_SPRITE,
    ENTITY_ASSET_HARVEST_SPRITE, 
    ENTITY_ASSET_HARVEST_SPRITE, 
    ENTITY_ASSET_BARLEY, 
    ENTITY_ASSET_SYDNEY, 
    ENTITY_ASSET_MRS_MANA, 
    ENTITY_ASSET_MRS_MANA_SON, 
    ENTITY_ASSET_GOURMET_JUDGE, 
    ENTITY_ASSET_MARIA_HARRIS_BABY,
    ENTITY_ASSET_POPURI_GRAY_BABY, 
    ENTITY_ASSET_ELLI_JEFF_BABY, 
    ENTITY_ASSET_ANN_CLIFF_BABY, 
    ENTITY_ASSET_KAREN_KAI_BABY, 
    ENTITY_ASSET_ENTOMOLOGIST 
};

u16 npcToDialogueBytecodeIndex[] = {
    DIALOGUE_MARIA, 
    DIALOGUE_POPURI, 
    DIALOGUE_ELLI, 
    DIALOGUE_ANN, 
    DIALOGUE_KAREN, 
    DIALOGUE_BABY, 
    DIALOGUE_HARRIS, 
    DIALOGUE_GRAY, 
    DIALOGUE_JEFF, 
    DIALOGUE_CLIFF, 
    DIALOGUE_KAI, 
    DIALOGUE_MAYOR, 
    DIALOGUE_MAYOR_WIFE, 
    DIALOGUE_LILLIA, 
    DIALOGUE_BASIL, 
    DIALOGUE_ELLEN, 
    DIALOGUE_DOUG, 
    DIALOGUE_GOTZ, 
    DIALOGUE_SASHA,
    DIALOGUE_POTION_SHOP_DEALER, 
    DIALOGUE_KENT, 
    DIALOGUE_STU, 
    DIALOGUE_MIDWIFE, 
    DIALOGUE_MAY, 
    DIALOGUE_RICK, 
    DIALOGUE_PASTOR, 
    DIALOGUE_SHIPPER, 
    DIALOGUE_SAIBARA,
    DIALOGUE_DUKE, 
    DIALOGUE_GREG, 
    DIALOGUE_ASSISTANT_CARPENTERS_1, 
    DIALOGUE_ASSISTANT_CARPENTERS_2, 
    DIALOGUE_MASTER_CARPENTER, 
    DIALOGUE_HARVEST_SPRITE_1, 
    DIALOGUE_HARVEST_SPRITE_2, 
    DIALOGUE_HARVEST_SPRITE_3, 
    DIALOGUE_BARLEY,
    DIALOGUE_SYDNEY, 
    DIALOGUE_MRS_MANA_AND_JOHN_1, 
    DIALOGUE_MRS_MANA_AND_JOHN_2, 
    DIALOGUE_ADDITIONAL_NPCS_1, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_ADDITIONAL_NPCS_2
};

// forward declarations
void handleJohnAnimation(void);
void handleGourmetJudgeAnimation(void);
void updateNPCMovement(u8);
void updateNPCRandomAnimationWithAnims(u8 index, u8 arg1, u8 arg2);
void updateNPCComplexAnimation(u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8); 
void updateNPCSpecialAnimation(u8 npcIndex);
u8 getNPCBabyCarryingState(u8);
void setMariaLocation(void);
void setPopuriLocation(void);
void setElliLocation(void);
void setAnnLocation(void);
void setKarenLocation(void);
void setBabyLocation(void);
void setHarrisLocation(void);
void setGrayLocation(void);
void setJeffLocation(void);
void setCliffLocation(void);
void setKaiLocation(void);
void setMayorLocation(void);
void setMayorWifeLocation(void);
void setLilliaLocation(void);
void setBasilLocation(void);
void setEllenLocation(void);
void setDougLocation(void);
void setGotzLocation(void);
void setSashaLocation(void);
void setPotionShopDealerLocation(void);
void setKentLocation(void);
void setStuLocation(void);
void setMidwifeLocation(void);
void setMayLocation(void);
void setRickLocation(void);
void setPastorLocation(void);
void setShipperLocation(void);
void setSaibaraLocation(void);
void setDukeLocation(void);
void setGregLocation(void);
void setCarpenter1Location(void);
void setCarpenter2Location(void);
void setMasterCarpenterLocation(void);
void setHarvestSprite1Location(void);
void setHarvestSprite2Location(void);
void setHarvestSprite3Location(void);
void setSydneyLocation(void);
void setBarleyLocation(void);
void setMrsManaLocation(void);
void setJohnLocation(void);
void setGourmetJudgeLocation(void);
void setMariaHarrisBabyLocation(void);
void setPopuriGrayBabyLocation(void);
void setElliJeffBabyLocation(void);
void setAnnCliffBabyLocation(void);
void setKarenKaiBabyLocation(void);
void setEntomologistLocation(void);
void handleMariaAnimation(void);
void handlePopuriAnimation(void);
void handleElliAnimation(void);
void handleAnnAnimation(void);
void handleKarenAnimation(void);
void handleBabyAnimation(void);
void handleHarrisAnimation(void);
void handleGrayAnimation(void);
void handleJeffAnimation(void);
void handleCliffAnimation(void);
void handleKaiAnimation(void);
void handleMayorAnimation(void);
void handleMayorWifeAnimation(void);
void handleLilliaAnimation(void);
void handleBasilAnimation(void);
void handleEllenAnimation(void);
void handleDougAnimation(void);
void handleGotzAnimation(void);
void handleSashaAnimation(void);
void handlePotionShopDealerAnimation(void);
void handleKentAnimation(void);
void handleStuAnimation(void);
void handleMidwifeAnimation(void);
void handleMayAnimation(void);
void handleRickAnimation(void);
void handlePastorAnimation(void);
void handleShipperAnimation(void);
void handleSaibaraAnimation(void);
void handleDukeAnimation(void);
void handleGregAnimation(void);
void handleCarpenter1Animation(void);
void handleCarpenter2Animation(void);
void handleMasterCarpenterAnimation(void);
void handleHarvestSprite1Animation(void);
void handleHarvestSprite2Animation(void);
void handleHarvestSprite3Animation(void);
void handleSydneyAnimation(void);
void handleBarleyAnimation(void);
void handleMrsManaAnimation(void);
void handleJohnAnimation(void);
void handleGourmetJudgeAnimation(void);
void handleMariaHarrisBabyAnimation(void);
void handlePopuriGrayBabyAnimation(void);
void handleElliJeffBabyAnimation(void);
void handleAnnCliffBabyAnimation(void);
void handleKarenKaiBabyAnimation(void);
void handleEntomologistAnimation(void);
 
//INCLUDE_ASM("asm/nonmatchings/game/npc", deactivateNPCEntities);

void deactivateNPCEntities(void) {
    
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        
        if (npcs[i].flags & NPC_ENTITY_LOADED) {
            deactivateEntity(npcs[i].entityIndex);
        }
        
        npcs[i].flags = 0;
        npcs[i].animationMode = NPC_ANIM_IDLE;
        npcs[i].animationIndex1 = 0;
        npcs[i].animationIndex2 = 0;
        npcs[i].animationState = 0;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", ma);

u8 setupNPCEntity(u8 npcIndex, int currentEntityOffset) {

    int currentEntityIndex = currentEntityOffset;

    if ((npcs[npcIndex].flags & NPC_ACTIVE) && npcs[npcIndex].levelIndex == gBaseMapIndex) {
        
        npcs[npcIndex].flags |= NPC_ENTITY_LOADED;

        switch (npcIndex) {
            case MARIA:
                if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE; 
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case POPURI:
                if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case ELLI:
                if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case ANN:
                if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case KAREN:
                if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case BABY:                               
                npcs[npcIndex].entityIndex = ENTITY_BABY;
                break;
            case JOHN:
                npcs[npcIndex].entityIndex = 29;
                break;
            case HARRIS:                                 
            case GRAY:                                 
            case JEFF:                                 
            case CLIFF:                                 
            case KAI:                                
            case MAYOR:                                
            case MAYOR_WIFE:                                
            case LILLIA:                                
            case BASIL:                                
            case ELLEN:                                
            case DOUG:                                
            case GOTZ:                                
            case SASHA:                                
            case POTION_SHOP_DEALER:                                
            case KENT:                                
            case STU:                                
            case MIDWIFE:                                
            case MAY:                                
            case RICK:                                
            case PASTOR:                                
            case SHIPPER:                                
            case SAIBARA:                                
            case DUKE:                                
            case GREG:                                
            case CARPENTER_1:                                
            case CARPENTER_2:                                
            case MASTER_CARPENTER:                                
            case HARVEST_SPRITE_1:                                
            case HARVEST_SPRITE_2:                                
            case HARVEST_SPRITE_3:                                
            case SYDNEY:                                
            case BARLEY:                                
            case MRS_MANA:
            default:
                npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                currentEntityIndex++;
                break;
        }
    
        npcs[npcIndex].currentCoordinates.x = npcs[npcIndex].startingCoordinates.x;
        npcs[npcIndex].currentCoordinates.y = npcs[npcIndex].startingCoordinates.y;
        npcs[npcIndex].currentCoordinates.z = npcs[npcIndex].startingCoordinates.z;
    
        loadEntity(npcs[npcIndex].entityIndex, npcToEntityAssetIndex[npcIndex], TRUE);
        setEntityCollidable(npcs[npcIndex].entityIndex, TRUE);
        setEntityYMovement(npcs[npcIndex].entityIndex, TRUE);
        setCameraTrackingEntity(npcs[npcIndex].entityIndex, FALSE);
    
        // FIXME: macro not matching; should be convertSpriteToWorldDirection(npcs[npcIndex].direction, getCurrentMapRotation(MAIN_MAP_INDEX))
        setEntityDirection(npcs[npcIndex].entityIndex, (npcs[npcIndex].direction + 8 - getCurrentMapRotation(MAIN_MAP_INDEX)) % 8);
        setEntityCoordinates(npcs[npcIndex].entityIndex, npcs[npcIndex].currentCoordinates.x, npcs[npcIndex].currentCoordinates.y, npcs[npcIndex].currentCoordinates.z);
    
        if (npcs[npcIndex].flags & NPC_ATTACHED) {

            switch (npcIndex) {                   
                case BABY:                             
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, ENTITY_WIFE, 0xFE);
                    break;
                case MARIA_HARRIS_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[MARIA].entityIndex, 0xFE);
                    break;
                case POPURI_GRAY_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[POPURI].entityIndex, 0xFE);
                    break;
                case ELLI_JEFF_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[ELLI].entityIndex, 0xFE);
                    break;
                case ANN_CLIFF_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[ANN].entityIndex, 0xFE);
                    break;
                case KAREN_KAI_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[KAREN].entityIndex, 0xFE);
                    break;
                }
            
                setEntityAttachmentOffset(npcs[npcIndex].entityIndex, 0, 0xE, 0x14);
                setEntityCollidable(npcs[npcIndex].entityIndex, FALSE);
                setEntityYMovement(npcs[npcIndex].entityIndex, FALSE);
                setEntityTracksCollisions(npcs[npcIndex].entityIndex, FALSE);
                enableEntityMovement(npcs[npcIndex].entityIndex, FALSE);

        }
    
        npcs[npcIndex].animationState = 0;
        npcs[npcIndex].animationTimer = 0;
        npcs[npcIndex].flags |= NPC_ENTITY_LOADED;
        
    }

    return currentEntityIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setupActiveNPCs);

// called on map load
void setupActiveNPCs(void) {

    u8 entityIndexOffset;
    u8 i;

    setNPCSpawningLocations();
    
    entityIndexOffset = 0;
    
    for (i = 0; i < MAX_NPCS; i++) {
        // loop through all NPCs but only increment entity index offset when active NPC is found
        entityIndexOffset = setupNPCEntity(i, entityIndexOffset);
    }
    
    // handle all npc animations
    setNPCAnimations();

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCCoordinates);

void updateNPCCoordinates(void) {

    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        if (npcs[i].flags & NPC_ACTIVE) {
            if (npcs[i].levelIndex == gBaseMapIndex) {
                npcs[i].currentCoordinates.x = entities[npcs[i].entityIndex].coordinates.x;
                npcs[i].currentCoordinates.y = entities[npcs[i].entityIndex].coordinates.y;
                npcs[i].currentCoordinates.z = entities[npcs[i].entityIndex].coordinates.z; 
            }
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", randomizeNPCLocations);

void randomizeNPCLocations(void) {
    
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        npcs[i].location = getRandomNumberInRange(0, 3);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCMovement);
void updateNPCMovement(u8 npcIndex) {

    Vec3f vec;
    u8 direction = npcs[npcIndex].direction;

    f32 f1, f2, f3;
    f32 f4, f5, f6;
    
    if (npcs[npcIndex].animationMode == NPC_ANIM_TALKING) {

        direction = (gPlayer.direction + 4) % 8;        

        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].animationIndex1);
        npcs[npcIndex].animationMode = npcs[npcIndex].defaultAnimationMode;
        
    }

    if (npcs[npcIndex].animationMode == NPC_ANIM_FACE_PLAYER) {
        direction = (gPlayer.direction + 4) % 8;        
        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].animationIndex1);
    }

    npcs[npcIndex].currentCoordinates.x = entities[npcs[npcIndex].entityIndex].coordinates.x;
    npcs[npcIndex].currentCoordinates.y = entities[npcs[npcIndex].entityIndex].coordinates.y;
    npcs[npcIndex].currentCoordinates.z = entities[npcs[npcIndex].entityIndex].coordinates.z;

    setEntityDirection(npcs[npcIndex].entityIndex, convertSpriteToWorldDirection(direction, MAIN_MAP_INDEX));

    vec = getMovementVectorFromDirection(npcs[npcIndex].speed, direction, 0.0f);

    f1 = npcs[npcIndex].startingCoordinates.x;
    f2 = npcs[npcIndex].wanderRadiusX;
    f3 = npcs[npcIndex].currentCoordinates.x + vec.x;
    
    if ((s16)(f1 + f2) >= (s16)f3 && (s16)f3 >= (s16)(f1 - f2)) {

        f4 = npcs[npcIndex].startingCoordinates.z;
        f5 = npcs[npcIndex].wanderRadiusZ;
        f6 = npcs[npcIndex].currentCoordinates.z + vec.z;

        if ((s16)(f4 + f5) >= (s16)f6 && (s16)f6 >= (s16)(f4 - f5)) {
            setEntityMovementVector(npcs[npcIndex].entityIndex, vec.x, vec.y, vec.z, npcs[npcIndex].speed);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", stopNPCMovement);

inline void stopNPCMovement(u8 npcIndex) {

    npcs[npcIndex].speed = 0;
    npcs[npcIndex].animationState = 10;
    npcs[npcIndex].animationTimer = 0;

    setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].animationIndex1);

    npcs[npcIndex].flags |= NPC_NEEDS_UPDATE;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCRandomAnimation);

// FIXME: should be inline?
void updateNPCRandomAnimation(u8 index) {
    
    u16 temp;
    
    if (npcs[index].animationState == 0) {

        npcs[index].speed = 0;
        npcs[index].animationTimer = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, npcs[index].animationIndex1);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcs[index].direction = temp*2;
            npcs[index].animationState = 1;
        }

        npcs[index].flags |= NPC_NEEDS_UPDATE;

        return;
    }
    
    if (npcs[index].direction & 1) {

        npcs[index].speed = 0;
        npcs[index].animationTimer = 0;
        npcs[index].animationState = 0;

    } else {

        npcs[index].speed = 1;
        npcs[index].animationTimer = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, npcs[index].animationIndex2);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npcs[index].animationState = 1;
            
        } else {
            npcs[index].animationState = 0;
        }
        
    }
    
    npcs[index].flags |= NPC_NEEDS_UPDATE;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", stopNPCWithAnimation);

inline void stopNPCWithAnimation(u8 index, u8 arg1) {

    npcs[index].speed = 0;
    npcs[index].animationState = 10;
    npcs[index].animationTimer = 0;

    setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg1);

    npcs[index].flags |= NPC_NEEDS_UPDATE;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCRandomAnimationWithAnims);

void updateNPCRandomAnimationWithAnims(u8 index, u8 arg1, u8 arg2) {

    u16 temp;
    
    if (npcs[index].animationState == 0) {

        npcs[index].speed = 0;
        npcs[index].animationTimer = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg1);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcs[index].direction = temp*2;
            npcs[index].animationState = 1;
        }

        npcs[index].flags |= NPC_NEEDS_UPDATE;
        return;
    }
    
    if (npcs[index].direction & 1) {

        npcs[index].speed = 0;
        npcs[index].animationTimer = 0;
        npcs[index].animationState = 0;

    } else {

        npcs[index].speed = 1;
        npcs[index].animationTimer = 0;
        
        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg2);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npcs[index].animationState = 1;
        } else {
            npcs[index].animationState = 0;
        }
        
    }
    
    npcs[index].flags |= NPC_NEEDS_UPDATE;
    
}



//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCComplexAnimation);

void updateNPCComplexAnimation(u8 npcIndex, u8 animationIndex1, u8 animationIndex2, u8 animationIndex3, u8 animationIndex4, u8 animationIndex5, u8 animationIndex6, u8 animationIndex7, u8 animationIndex8, u8 animationIndex9, u8 animationIndex10, u8 animationIndex11) {

    u16 temp;
    
    switch (npcs[npcIndex].animationState) {

        case 0:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 10;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex1);

            temp = getRandomNumberInRange(0, 60);
            
            if (temp < 4) {
                npcs[npcIndex].direction = temp * 2;
                npcs[npcIndex].animationState = 1;
            } 
            
            if (temp == 4) {
                npcs[npcIndex].animationState = 7;
            } 
            
            if (temp == 5) {
                npcs[npcIndex].animationState = 2;
            } 

            if (temp == 6) {
                npcs[npcIndex].animationState = 3;
            } 
            
            if (temp == 7) {
                npcs[npcIndex].animationState = 4;
            }
        
            npcs[npcIndex].flags |= NPC_NEEDS_UPDATE;
            break;

        case 1:

            if (npcs[npcIndex].direction & 1) {
                
                npcs[npcIndex].speed = 0;
                npcs[npcIndex].animationTimer = 0;
                npcs[npcIndex].animationState = 0;
                
            } else {
                
                npcs[npcIndex].speed = 1;
                npcs[npcIndex].animationTimer = 0;
                setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex2);
    
                if (getRandomNumberInRange(0, 19) < 8) {
                    npcs[npcIndex].animationState = 1;
                } else {
                    npcs[npcIndex].animationState = 0;
                }
                
            }

            npcs[npcIndex].flags |= NPC_NEEDS_UPDATE;
            break;

        case 2:   
            
            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;

            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex3);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].animationState = 2;
            } else {
                npcs[npcIndex].animationState = 0;
            }
            
            break;

        case 3:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;

            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex4);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].animationState = 3;
            } else {
                npcs[npcIndex].animationState = 0;
            }
            
            break;

        case 4:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex5);

            npcs[npcIndex].animationState = 0;
            
            break;

        case 5:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex6);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].animationState = 5;
            } else {
                npcs[npcIndex].animationState = 10;
            }
            
            break;

        case 6:
            
            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex7);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].animationState = 6;
            } else {
                npcs[npcIndex].animationState = 10;
            }
            
            break;


        case 7:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex8);

            npcs[npcIndex].animationState = 10;
            
            break;

        case 8:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex9);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].animationState = 8;
            } else {
                npcs[npcIndex].animationState = 10;
            }
            
            break;

    case 9:

        npcs[npcIndex].speed = 0;
        npcs[npcIndex].animationTimer = 0;
        
        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex10);

        if (getRandomNumberInRange(0, 19) < 8) {
            npcs[npcIndex].animationState = 9;
        } else {
            npcs[npcIndex].animationState = 10;
        }
        
        break;


    case 10:

        npcs[npcIndex].speed = 0;
        npcs[npcIndex].animationTimer = 10;

        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex11);

        npcs[npcIndex].animationState = 10;

        temp = getRandomNumberInRange(0, 60);
        
        if (temp < 10) {
            npcs[npcIndex].animationState = 0;
        } 
        
        if (temp == 11) {
            npcs[npcIndex].animationState = 5;
        }
        
        if (temp == 12) {
            npcs[npcIndex].animationState = 6;
        } 
        
        if (temp == 13) {
            npcs[npcIndex].animationState = 8;
        } 
        
        if (temp == 14) {
            npcs[npcIndex].animationState = 9;
        }
    
        break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCSpecialAnimation);

void updateNPCSpecialAnimation(u8 npcIndex) {

    u16 temp;
    
    switch (npcs[npcIndex].animationState) {

        case 0:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, 0);
            
            temp = getRandomNumberInRange(0, 60);

            if (temp < 8) {
                npcs[npcIndex].direction = temp;
                npcs[npcIndex].animationState = 1;
            }

            if (7 < temp && temp < 21) {
                npcs[npcIndex].animationState = 2;
            }

            npcs[npcIndex].flags |= NPC_NEEDS_UPDATE;

            break;

        case 1:
            
            npcs[npcIndex].speed = 1;
            npcs[npcIndex].animationTimer = 0;
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].animationState = 1;
            } else {
                npcs[npcIndex].animationState = 0;
            }

            npcs[npcIndex].flags |= NPC_NEEDS_UPDATE;

            break;

        case 2:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].animationTimer = 60;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, 0x19);

            npcs[npcIndex].animationState = 1;
            npcs[npcIndex].flags |= NPC_NEEDS_UPDATE;
            
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setNPCSpawningLocations);

void setNPCSpawningLocations(void) {
    
    // set girls' and baby's location
    setMariaLocation();
    setPopuriLocation();
    setElliLocation();
    setAnnLocation();
    setKarenLocation();
    setBabyLocation();
    
    if (!checkDailyEventBit(FESTIVAL)) {
        
        // set rest of npcs' locations
        setHarrisLocation();
        setGrayLocation();
        setJeffLocation();
        setCliffLocation();
        setKaiLocation();
        setMayorLocation();
        setMayorWifeLocation();
        setLilliaLocation();
        setBasilLocation();
        setEllenLocation();
        setDougLocation();
        setGotzLocation();
        setSashaLocation();
        setPotionShopDealerLocation();
        setKentLocation();
        setStuLocation();
        setMidwifeLocation();
        setMayLocation();
        setRickLocation();
        setPastorLocation();
        setShipperLocation();
        setSaibaraLocation();
        setDukeLocation();
        setGregLocation();
        setCarpenter1Location();
        setCarpenter2Location();
        setMasterCarpenterLocation();
        setHarvestSprite1Location();
        setHarvestSprite2Location();
        setHarvestSprite3Location();
        setSydneyLocation();
        setBarleyLocation();
        setMrsManaLocation();
        setJohnLocation();
        setGourmetJudgeLocation();
        setMariaHarrisBabyLocation();
        setPopuriGrayBabyLocation();
        setElliJeffBabyLocation();
        setAnnCliffBabyLocation();
        setKarenKaiBabyLocation();
        setEntomologistLocation();
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", getBabyCarryingState);

inline u8 getBabyCarryingState(void) {

    u8 result = 0;

    if (checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY)) {

        if (gBabyAge < 30) {
            
            if (5 < gHour && gHour < 20) {
            
                if (npcs[gWife].levelIndex != HOUSE) {
                    result = 1;
                } else {
                    result = 2;
                }
            
            } else {
                result = 3;
            }  

        } else {

            if (5 < gHour && gHour < 20) {
                // reference: -(120 < gBabyAge) & 6U | 4;
                result = (gBabyAge < 120) ? 4 : 6;
            } else {
                result = 5;
            }

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", getNPCBabyCarryingState);

u8 getNPCBabyCarryingState(u8 npcIndex) {

    u8 result = 0; 
    
    switch (npcIndex) {

        case MARIA:

            if (checkLifeEventBit(MARIA_HARRIS_MARRIED) && checkLifeEventBit(MARIA_HARRIS_HAVE_BABY)) {
    
                if (mariaHarrisBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case POPURI:

            if (checkLifeEventBit(POPURI_GRAY_MARRIED) && checkLifeEventBit(POPURI_GRAY_HAVE_BABY)) {
    
                if (popuriGrayBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case ELLI:

            if (checkLifeEventBit(ELLI_JEFF_MARRIED) && checkLifeEventBit(ELLI_JEFF_HAVE_BABY)) {
    
                if (elliJeffBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case ANN:

            if (checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(ANN_CLIFF_HAVE_BABY)) {
    
                if (annCliffBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case KAREN:

            if (checkLifeEventBit(KAREN_KAI_MARRIED) && checkLifeEventBit(KAREN_KAI_HAVE_BABY)) {
    
                if (karenKaiBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;
        
    }

    return result;
    
}



/* set starting locations */

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMariaLocation);

void setMariaLocation(void) {

     u8 result;

    npcs[MARIA].wanderRadiusX = 64;
    npcs[MARIA].wanderRadiusZ = 64;
    npcs[MARIA].animationIndex1 = 0;
    npcs[MARIA].animationIndex2 = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(0x56)) {

        if (checkLifeEventBit(MARRIED) && gWife == MARIA) {

            if (5 < gHour && gHour < 8) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = SOUTH;
                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[MARIA].startingCoordinates.x = -128.0f;
                npcs[MARIA].startingCoordinates.z = -64.0f;
                npcs[MARIA].flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (npcs[MARIA].location < 2) {

                        npcs[MARIA].levelIndex = CHURCH;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MARIA].startingCoordinates.x = -112.0f;
                        npcs[MARIA].startingCoordinates.z = -192.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;
                        
                    }  else {

                        npcs[MARIA].levelIndex = LIBRARY;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MARIA].startingCoordinates.x = 48.0f;
                        npcs[MARIA].startingCoordinates.z = -96.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else if (gWeather == SUNNY) {
                        
                    npcs[MARIA].levelIndex = FARM;
                    npcs[MARIA].startingCoordinates.y = 0.0f;
                    npcs[MARIA].direction = SOUTHWEST;
                    npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MARIA].startingCoordinates.x = -256.0f;
                    npcs[MARIA].startingCoordinates.z = -240.0f;
                    npcs[MARIA].flags |= NPC_ACTIVE;
                    
                } else {

                    npcs[MARIA].levelIndex = HOUSE;
                    npcs[MARIA].startingCoordinates.x = 0.0f;
                    npcs[MARIA].startingCoordinates.y = 0.0f;
                    npcs[MARIA].startingCoordinates.z = 0.0f;
                    npcs[MARIA].direction = SOUTHWEST;
                    npcs[MARIA].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[MARIA].flags |= NPC_ACTIVE;
                    
                }
                
            }

            if (gHour == 12) {
                
                npcs[MARIA].levelIndex = KITCHEN;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = NORTHWEST;
                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[MARIA].startingCoordinates.x = -160.0f;
                npcs[MARIA].startingCoordinates.z = -64.0f;
                npcs[MARIA].flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.x = 0.0f;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].startingCoordinates.z = 0.0f;
                npcs[MARIA].direction = SOUTHWEST;
                npcs[MARIA].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[MARIA].flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[MARIA].levelIndex = KITCHEN;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = NORTHWEST;
                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[MARIA].startingCoordinates.x = -160.0f;
                npcs[MARIA].startingCoordinates.z = -64.0f;
                npcs[MARIA].flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = SOUTHEAST;
                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[MARIA].startingCoordinates.x = -128.0f;
                npcs[MARIA].startingCoordinates.z = -112.0f;
                npcs[MARIA].flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = SOUTHWEST;
                npcs[MARIA].defaultAnimationMode = NPC_ANIM_FIXED;
                npcs[MARIA].startingCoordinates.x = -192.0f;
                npcs[MARIA].startingCoordinates.z = -160.0f;
                npcs[MARIA].flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[MARIA].animationIndex1 = 0x6C;
                npcs[MARIA].animationIndex2 = 0x76;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1A)) {

                switch (gDayOfWeek) {

                    case TUESDAY:
                    case WEDNESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:
                    case SUNDAY:

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[MARIA].levelIndex = LIBRARY;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[MARIA].startingCoordinates.x = 48.0f;
                            npcs[MARIA].startingCoordinates.z = -96.0f;
                            npcs[MARIA].flags |= NPC_ACTIVE;
        
                        }

                        break;

                    case MONDAY:

                        if (gWeather == SUNNY) {

                            if (npcs[MARIA].location < 2) {
                                
                                if (7 < gHour && gHour < 15) { 
                                
                                    npcs[MARIA].levelIndex = VILLAGE_1;
                                    npcs[MARIA].startingCoordinates.x = 0.0f;
                                    npcs[MARIA].startingCoordinates.y = 0.0f;
                                    npcs[MARIA].direction = SOUTHWEST;
                                    npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[MARIA].startingCoordinates.z = -144.0f;
                                    npcs[MARIA].flags |= NPC_ACTIVE;
                                    
                                }
                                
                            } else {

                                if (7 < gHour && gHour < 17) { 
                                
                                    npcs[MARIA].levelIndex = VILLAGE_2;
                                    npcs[MARIA].startingCoordinates.x = 0.0f;
                                    npcs[MARIA].startingCoordinates.y = 0.0f;
                                    npcs[MARIA].startingCoordinates.z = 0.0f;
                                    npcs[MARIA].direction = SOUTHWEST;
                                    npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[MARIA].flags |= NPC_ACTIVE;
                                    
                                }
                                
                            }
                            
                        } else {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[MARIA].levelIndex = MAYOR_HOUSE;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTHWEST;
                                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[MARIA].startingCoordinates.x = -128.0f;
                                npcs[MARIA].startingCoordinates.z = -32.0f;
                                npcs[MARIA].flags |= NPC_ACTIVE;
        
                            }
                    
                        }
                        
                        break;
                    
                }

                if (gSeason == SUMMER &&  0 < gDayOfMonth && gDayOfMonth < 11) {
                    
                        npcs[MARIA].levelIndex = MAYOR_HOUSE;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MARIA].startingCoordinates.x = -128.0f;
                        npcs[MARIA].startingCoordinates.z = -32.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;
                    
                } 

                if (gSeason == WINTER && 0 < gDayOfMonth && gDayOfMonth < 11) {

                    if (npcs[MARIA].location < 2) {
                        
                        npcs[MARIA].levelIndex = MAYOR_HOUSE;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MARIA].startingCoordinates.x = -128.0f;
                        npcs[MARIA].startingCoordinates.z = -32.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;
                            
                    } else {

                        npcs[MARIA].levelIndex = CHURCH;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MARIA].startingCoordinates.x = -112.0f;
                        npcs[MARIA].startingCoordinates.z = -192.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;
                        
                    }
                    
                } 

                if (getNPCBabyCarryingState(MARIA) == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                    npcs[MARIA].animationIndex1 = 0x6C;
                    npcs[MARIA].animationIndex2 = 0x76;
                }
                
            } else {

                if (gDayOfWeek == MONDAY) {
    
                    if (gWeather == SUNNY) {
                        
                        if (npcs[MARIA].location < 2) {
                            
                            if (7 < gHour && gHour < 15) {
                                
                                npcs[MARIA].levelIndex = VILLAGE_1;
                                npcs[MARIA].startingCoordinates.x = 0.0f;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTHWEST;
                                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[MARIA].startingCoordinates.z = -144.0f;
                                npcs[MARIA].flags |= NPC_ACTIVE;
                                
                            } 
                            
                        } else if (9 < gHour && gHour < 17) {
                                    
                            npcs[MARIA].levelIndex = MOUNTAIN_1;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[MARIA].startingCoordinates.x = 128.0f;
                            npcs[MARIA].startingCoordinates.z = 128.0f;
                            npcs[MARIA].flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(0x136);
                                
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[MARIA].levelIndex = MAYOR_HOUSE;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[MARIA].startingCoordinates.x = -128.0f;
                            npcs[MARIA].startingCoordinates.z = -32.0f;
                            npcs[MARIA].flags |= NPC_ACTIVE;
                            
                        }
                                                
                    }
                                      
                } else {
    
                    if (8 < gHour && gHour < 17) {
                        
                        npcs[MARIA].levelIndex = LIBRARY;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MARIA].startingCoordinates.x = 48.0f;
                        npcs[MARIA].startingCoordinates.z = -96.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;
                    
                    }
    
                }

                if (gSeason == SUMMER && 0 < gDayOfMonth && gDayOfMonth < 11) {
    
                    if (gWeather == SUNNY) {
                        
                        if (npcs[MARIA].location < 2) {
    
                            if (8 < gHour && gHour < 17) {
    
                                npcs[MARIA].levelIndex = MOUNTAIN_2;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTH;
                                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[MARIA].startingCoordinates.x = -208.0f;
                                npcs[MARIA].startingCoordinates.z = -160.0f;
                                npcs[MARIA].flags |= NPC_ACTIVE;
        
                                setSpecialDialogueBit(0x136);
                                
                            }
                            
                        } else {
    
                            if (8 < gHour && gHour < 17) {
                               
                                npcs[MARIA].levelIndex = POND;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTHEAST;
                                npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[MARIA].startingCoordinates.x = -96.0f;
                                npcs[MARIA].startingCoordinates.z = -32.0f;
                                npcs[MARIA].flags |= NPC_ACTIVE;
    
                                setSpecialDialogueBit(0x136);
                                
                            }
                            
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[MARIA].levelIndex = MAYOR_HOUSE;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[MARIA].startingCoordinates.x = -128.0f;
                            npcs[MARIA].startingCoordinates.z = -32.0f;
                            npcs[MARIA].flags |= NPC_ACTIVE;
                        
                        }
                        
                    }
                    
                }
    
                if (gSeason == WINTER && 0 < gDayOfMonth && gDayOfMonth < 11) {
    
                    if (npcs[MARIA].location < 2) {
    
                        if (8 < gHour && gHour < 17) { 
                        
                            npcs[MARIA].levelIndex = MAYOR_HOUSE;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[MARIA].startingCoordinates.x = -128.0f;
                            npcs[MARIA].startingCoordinates.z = -32.0f;
                            npcs[MARIA].flags |= NPC_ACTIVE;
                        
                        }
    
                    } else {
    
                        if (8 < gHour && gHour < 17) {
    
                            npcs[MARIA].levelIndex = CHURCH;
                            npcs[MARIA].startingCoordinates.y = 0;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[MARIA].startingCoordinates.x = -112.0f;
                            npcs[MARIA].startingCoordinates.z = -192.0f;
                            npcs[MARIA].flags |= NPC_ACTIVE;
                            
                        }
                        
                    }
                    
                }
    
                if (gSeason == SUMMER && gDayOfWeek == SUNDAY && gWeather == SUNNY && npcs[MARIA].location == 0) {
    
                    if (17 < gHour && gHour < 21) {
    
                        npcs[MARIA].levelIndex = BEACH;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].startingCoordinates.z = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].defaultAnimationMode = NPC_ANIM_SPECIAL;
                        npcs[MARIA].startingCoordinates.x = -224.0f;
                        npcs[MARIA].flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(0xB1);
                        
                    }
                    
                }
                
            }
            
        } 
        
    }

    npcs[MARIA].animationMode = npcs[MARIA].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setPopuriLocation);

void setPopuriLocation(void) {

    u8 result;

    npcs[POPURI].wanderRadiusX = 64;
    npcs[POPURI].wanderRadiusZ = 64;
    npcs[POPURI].animationIndex1 = 0;
    npcs[POPURI].animationIndex2 = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(0x57)) {

        if (checkLifeEventBit(MARRIED) && gWife == POPURI) {

            if (5 < gHour && gHour < 8) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = SOUTH;
                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[POPURI].startingCoordinates.x = -128.0f;
                npcs[POPURI].startingCoordinates.z = -64.0f;
                npcs[POPURI].flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == MONDAY) {

                    if (npcs[ELLI].location < 2) {

                        if (8 < gHour && gHour < 17) {

                            if (gWeather == SUNNY) {
      
                                npcs[POPURI].levelIndex = MOUNTAIN_2;    
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHWEST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[POPURI].startingCoordinates.x = -224.0f;
                                npcs[POPURI].startingCoordinates.z = -96.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;
        
                                setSpecialDialogueBit(0x91);
                                
                            } else {
                                
                                npcs[POPURI].levelIndex = KITCHEN;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHWEST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[POPURI].startingCoordinates.x = -160.0f;
                                npcs[POPURI].startingCoordinates.z = -64.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;
                                                                
                            }
                            
                        }
                                              
                    } else {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;    
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;
                            
                        }

                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                        
                        npcs[POPURI].levelIndex = FARM;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHWEST;
                        npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[POPURI].startingCoordinates.x = 320.0f;
                        npcs[POPURI].startingCoordinates.z = -352.0f;
                        npcs[POPURI].flags |= NPC_ACTIVE;
                        
                    } else {
                            
                        npcs[POPURI].levelIndex = KITCHEN;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHWEST;
                        npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[POPURI].startingCoordinates.x = -160.0f;
                        npcs[POPURI].startingCoordinates.z = -64.0f;
                        npcs[POPURI].flags |= NPC_ACTIVE;
                                            
                    }
                    
                }
                
            }

            if (gHour == 12) {
                
                npcs[POPURI].levelIndex = KITCHEN;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = NORTHWEST;
                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[POPURI].startingCoordinates.x = -160.0f;
                npcs[POPURI].startingCoordinates.z = -64.0f;
                npcs[POPURI].flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.x = 0.0f;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].startingCoordinates.z = 0.0f;
                npcs[POPURI].direction = SOUTHWEST;
                npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[POPURI].flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[POPURI].levelIndex = KITCHEN;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = NORTHWEST;
                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[POPURI].startingCoordinates.x = -160.0f;
                npcs[POPURI].startingCoordinates.z = -64.0f;
                npcs[POPURI].flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = SOUTHEAST;
                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[POPURI].startingCoordinates.x = -128.0f;
                npcs[POPURI].startingCoordinates.z = -112.0f;
                npcs[POPURI].flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = SOUTHWEST;
                npcs[POPURI].defaultAnimationMode = NPC_ANIM_FIXED;
                npcs[POPURI].startingCoordinates.x = -192.0f;
                npcs[POPURI].startingCoordinates.z = -160.0f;
                npcs[POPURI].flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[POPURI].animationIndex1 = 0x7A;
                npcs[POPURI].animationIndex2 = 0x84;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1B)) {

                switch (gDayOfWeek) {

                    case MONDAY:
                    case TUESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (8 < gHour && gHour < 17) {

                            if (gWeather == SUNNY) {
                                
                                npcs[POPURI].levelIndex = RANCH;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHEAST;
                                npcs[POPURI].startingCoordinates.x = -32.0f;
                                npcs[POPURI].startingCoordinates.z = -96.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;

                                if (getNPCBabyCarryingState(POPURI) == 1) {
                                    npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                                } else {
                                    npcs[POPURI].defaultAnimationMode = NPC_ANIM_CUSTOM;
                                }

                                setSpecialDialogueBit(0x46);
                                                
                            } else {
                                
                                npcs[POPURI].levelIndex = RANCH_STORE;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = NORTHWEST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[POPURI].startingCoordinates.x = 64.0f;
                                npcs[POPURI].startingCoordinates.z = 64.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;  
                            }
                        
                        }

                        break;

                    case SUNDAY:
                    case WEDNESDAY:
    
                        npcs[POPURI].levelIndex = RANCH_STORE;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = NORTHWEST;
                        npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[POPURI].startingCoordinates.x = 64.0f;
                        npcs[POPURI].startingCoordinates.z = 64.0f;
                        npcs[POPURI].flags |= NPC_ACTIVE;  
                        break;
                    
                }

                if (getNPCBabyCarryingState(POPURI) == 1) {
                    npcs[POPURI].animationIndex1 = 0x7A;
                    npcs[POPURI].animationIndex2 = 0x84;
                }
                
            } else {

                if (SUNDAY < gDayOfWeek && gDayOfWeek < FRIDAY) {

                    if (gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = VILLAGE_1;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHWEST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_CUSTOM;
                            npcs[POPURI].startingCoordinates.x = 96.0f;
                            npcs[POPURI].startingCoordinates.z = -224.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;  
                                
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;  
                            
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == FRIDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[POPURI].location < 2) {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = MOUNTAIN_2;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHWEST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[POPURI].startingCoordinates.x = -224.0f;
                                npcs[POPURI].startingCoordinates.z = -128.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;  

                                setSpecialDialogueBit(0x91);
                                
                            }

                        } else {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = MOON_MOUNTAIN;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].startingCoordinates.z = 0.0f;
                                npcs[POPURI].direction = NORTHEAST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[POPURI].startingCoordinates.x = 64.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;  

                                setSpecialDialogueBit(0x92);
                                
                            }
                            
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;  
                            
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == SATURDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[POPURI].location < 2) {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = MOUNTAIN_2;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHEAST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[POPURI].startingCoordinates.x = -128.0f;
                                npcs[POPURI].startingCoordinates.z = -416.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;  

                                setSpecialDialogueBit(0x91);
                                
                            }

                        } else {
                          
                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = POND;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHEAST;
                                npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[POPURI].startingCoordinates.x = -224.0f;
                                npcs[POPURI].startingCoordinates.z = -96.0f;
                                npcs[POPURI].flags |= NPC_ACTIVE;  

                            }      
                            
                        } 
                        
                    } else {
                        
                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;  
                            
                        }
                    }
                    
                }

                if (gDayOfWeek == SUNDAY) {

                    if (!(npcs[POPURI].location < 2) && gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[POPURI].levelIndex = RANCH;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[POPURI].startingCoordinates.x = -32.0f;
                            npcs[POPURI].startingCoordinates.z = -96.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(0x46);
                            
                        }
                                            
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[POPURI].levelIndex = BAKERY;
                            npcs[POPURI].startingCoordinates.x = 0.0f;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].startingCoordinates.z = 0.0f;
                            npcs[POPURI].direction = SOUTHWEST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[POPURI].flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(0x93);
                            
                        }
                        
                    }
                    
                }

                if (gSeason == SUMMER && gDayOfWeek == THURSDAY) {

                    if (gWeather == SUNNY && npcs[POPURI].location == 0) {

                        if (18 < gHour && gHour < 21) {
    
                            npcs[POPURI].levelIndex = BEACH;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].startingCoordinates.z = 0.0f;
                            npcs[POPURI].direction = SOUTHWEST;
                            npcs[POPURI].defaultAnimationMode = NPC_ANIM_SPECIAL;
                            npcs[POPURI].startingCoordinates.x = -224.0f;
                            npcs[POPURI].flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(0xB4);
                            
                        }
                    
                        
                    }
                    
                }

                if (gWeather == 4 && npcs[POPURI].location == 0) {

                    if (8 < gHour && gHour < 17) {
                        
                        npcs[POPURI].levelIndex = MOUNTAIN_2;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHWEST;
                        npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[POPURI].startingCoordinates.x = -64.0f;
                        npcs[POPURI].startingCoordinates.z = -160.0f;
                        npcs[POPURI].flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(0x91);
                                   
                    }
                    
                }

                if (gSeason == WINTER && 24 < gDayOfMonth && gDayOfMonth < 31) {

                    if (8 < gHour && gHour < 17) {
       
                        npcs[POPURI].levelIndex = FLOWER_SHOP;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHEAST;
                        npcs[POPURI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[POPURI].startingCoordinates.x = -112.0f;
                        npcs[POPURI].startingCoordinates.z = 64.0f;
                        npcs[POPURI].flags |= NPC_ACTIVE;  
                        
                    }
                                     
                }
                
            } 
        
        }
        
    }

    npcs[POPURI].animationMode = npcs[POPURI].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setElliLocation);

void setElliLocation(void) {

    u8 result;

    npcs[ELLI].wanderRadiusX = 64;
    npcs[ELLI].wanderRadiusZ = 64;
    npcs[ELLI].animationIndex1 = 0;
    npcs[ELLI].animationIndex2 = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(0x58)) {

        if (checkLifeEventBit(MARRIED) && gWife == ELLI) {

            if (5 < gHour && gHour < 8) {

                npcs[ELLI].levelIndex = HOUSE;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTH;
                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ELLI].startingCoordinates.x = -128.0f;
                npcs[ELLI].startingCoordinates.z = -64.0f;
                npcs[ELLI].flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (npcs[ELLI].location < 2) {
                                                    
                        npcs[ELLI].levelIndex = BAKERY;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[ELLI].startingCoordinates.x = -32.0f;
                        npcs[ELLI].startingCoordinates.z = 16.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;  

                    } else if (gWeather == SUNNY) {
                        
                        npcs[ELLI].levelIndex = MOUNTAIN_2;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = NORTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[ELLI].startingCoordinates.x = 96.0f;
                        npcs[ELLI].startingCoordinates.z = -64.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;  
        
                        setSpecialDialogueBit(0x97);
                                
                    } else {
                        
                        npcs[ELLI].levelIndex = KITCHEN;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[ELLI].startingCoordinates.x = -160.0f;
                        npcs[ELLI].startingCoordinates.z = -64.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                                                
                        npcs[ELLI].levelIndex = FARM;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].startingCoordinates.z = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[ELLI].startingCoordinates.x = -448.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;
                        
                    } else {
                                                
                        npcs[ELLI].levelIndex = KITCHEN;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[ELLI].startingCoordinates.x = -160.0f;
                        npcs[ELLI].startingCoordinates.z = -64.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;
                        
                    }
                                        
                }
                
            }

            if (gHour == 12) {
                
                npcs[ELLI].levelIndex = KITCHEN;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = NORTHWEST;
                npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[ELLI].startingCoordinates.x = -128.0f;
                npcs[ELLI].startingCoordinates.z = -128.0f;
                npcs[ELLI].flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[ELLI].levelIndex = KITCHEN;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTHWEST;
                npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[ELLI].startingCoordinates.x = -160.0f;
                npcs[ELLI].startingCoordinates.z = -64.0f;
                npcs[ELLI].flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[ELLI].levelIndex = KITCHEN;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = NORTHWEST;
                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ELLI].startingCoordinates.x = -160.0f;
                npcs[ELLI].startingCoordinates.z = -64.0f;
                npcs[ELLI].flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[ELLI].levelIndex = HOUSE;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTHEAST;
                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ELLI].startingCoordinates.x = -128.0f;
                npcs[ELLI].startingCoordinates.z = -112.0f;
                npcs[ELLI].flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[ELLI].levelIndex = HOUSE;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTHWEST;
                npcs[ELLI].defaultAnimationMode = NPC_ANIM_FIXED;
                npcs[ELLI].startingCoordinates.x = -192.0f;
                npcs[ELLI].startingCoordinates.z = -160.0f;
                npcs[ELLI].flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[ELLI].animationIndex1 = 0x69;
                npcs[ELLI].animationIndex2 = 0x73;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1C)) {

                if (gWeather == SUNNY) {

                    switch (gDayOfWeek) {

                        case MONDAY:

                            switch (gSeason) {

                                case SPRING:
                                case SUMMER:
                                    
                                    if (5 < gHour && gHour < 18) {
                            
                                        npcs[ELLI].levelIndex = MOUNTAIN_2;
                                        npcs[ELLI].startingCoordinates.y = 0.0f;
                                        npcs[ELLI].direction = NORTHWEST;
                                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                        npcs[ELLI].startingCoordinates.x = 96.0f;
                                        npcs[ELLI].startingCoordinates.z = -64.0f;
                                        npcs[ELLI].flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(0x97);
                                        
                                    }            
                                            
                                    break;
                                
                                case WINTER:
                                    
                                    if (5 < gHour && gHour < 18) {
                
                                        npcs[ELLI].levelIndex = BAKERY;
                                        npcs[ELLI].startingCoordinates.y = 0.0f;
                                        npcs[ELLI].direction = SOUTHWEST;
                                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                                        npcs[ELLI].startingCoordinates.x = -32.0f;
                                        npcs[ELLI].startingCoordinates.z = 16.0f;
                                        npcs[ELLI].flags |= NPC_ACTIVE;  
                                    
                                    }
                                    
                                    break;
                                
                                case AUTUMN:

                                     if (5 < gHour && gHour < 18) {
                                
                                        npcs[ELLI].levelIndex = MOUNTAIN_1;
                                        npcs[ELLI].startingCoordinates.y = 0.0f;
                                        npcs[ELLI].direction = SOUTHWEST;
                                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                        npcs[ELLI].startingCoordinates.x = 112.0f;
                                        npcs[ELLI].startingCoordinates.z = -32.0f;
                                        npcs[ELLI].flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(0x96);
                                        
                                    }       

                                    break;
                                
                            }

                            break;

                        case WEDNESDAY:
                            
                            if (8 < gHour && gHour < 12) {
                                
                                npcs[ELLI].levelIndex = FLOWER_SHOP;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ELLI].startingCoordinates.x = 32.0f;
                                npcs[ELLI].startingCoordinates.z = -16.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
        
                                setSpecialDialogueBit(0x98);
        
                            }

                            if (11 < gHour && gHour < 17) {
    
                                npcs[ELLI].levelIndex = VILLAGE_1;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = NORTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ELLI].startingCoordinates.x = 352.0f;
                                npcs[ELLI].startingCoordinates.z = 96.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
                                
                            }

                            break;

                        case SUNDAY:
                        case TUESDAY:
                        case THURSDAY:
                        case FRIDAY:
                        case SATURDAY:

                            if (5 < gHour && gHour < 18) {

                                npcs[ELLI].levelIndex = BAKERY;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ELLI].startingCoordinates.x = -32.0f;
                                npcs[ELLI].startingCoordinates.z = 16.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
                                
                            }
                            
                            break;
                        
                    }
                    
                } else {
                    
                    if (5 < gHour && gHour < 18) {

                        npcs[ELLI].levelIndex = BAKERY;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[ELLI].startingCoordinates.x = -32.0f;
                        npcs[ELLI].startingCoordinates.z = 16.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;  
                    
                    }
                    
                }

                if (getNPCBabyCarryingState(ELLI) == 1) {
                    npcs[ELLI].animationIndex1 = 0x69;
                    npcs[ELLI].animationIndex2 = 0x73;
                }
                
            } else {

                switch (gDayOfWeek) {
                    
                    case TUESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (gWeather == SUNNY) {
                            
                            if (8 < gHour && gHour < 17) {
                            
                                npcs[ELLI].levelIndex = BAKERY;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ELLI].startingCoordinates.x = -32.0f;
                                npcs[ELLI].startingCoordinates.z = 16.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
        
                            } 
                            
                        } else {
                                                    
                            if (8 < gHour && gHour < 17) {
                                
                                npcs[ELLI].levelIndex = BAKERY;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ELLI].startingCoordinates.x = -32.0f;
                                npcs[ELLI].startingCoordinates.z = 16.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
        
                            } 
                            
                        }
                        
                        break;
                    
                }

                if (gDayOfWeek == WEDNESDAY) {

                    if (8 < gHour && gHour < 12) {
                        
                        npcs[ELLI].levelIndex = FLOWER_SHOP;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[ELLI].startingCoordinates.x = 32.0f;
                        npcs[ELLI].startingCoordinates.z = -16.0f;
                        npcs[ELLI].flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(0x98);

                    }

                    if (11 < gHour && gHour < 17) {

                        if (gWeather == SUNNY) {
                                                    
                            npcs[ELLI].levelIndex = VILLAGE_1;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = NORTHWEST;
                            npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ELLI].startingCoordinates.x = 352.0f;
                            npcs[ELLI].startingCoordinates.z = 96.0f;
                            npcs[ELLI].flags |= NPC_ACTIVE;  

                        } else {
                            
                            npcs[ELLI].levelIndex = BAKERY;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = SOUTHWEST;
                            npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ELLI].startingCoordinates.x = -32.0f;
                            npcs[ELLI].startingCoordinates.z = 16.0f;
                            npcs[ELLI].flags |= NPC_ACTIVE;  
    
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == MONDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[ELLI].location < 2) {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ELLI].levelIndex = MOUNTAIN_2;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = NORTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ELLI].startingCoordinates.x = 96.0f;
                                npcs[ELLI].startingCoordinates.z = -64.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
        
                                setSpecialDialogueBit(0x97);
                                
                            }            
                            
                        } else {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ELLI].levelIndex = MOUNTAIN_1;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ELLI].startingCoordinates.x = 112.0f;
                                npcs[ELLI].startingCoordinates.z = -32.0f;
                                npcs[ELLI].flags |= NPC_ACTIVE;  
        
                                setSpecialDialogueBit(0x96);
                                
                            }       
                            
                            
                        }
                        
                    } else {
                             
                        if (8 < gHour && gHour < 17) {
                            
                            npcs[ELLI].levelIndex = BAKERY;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = SOUTHWEST;
                            npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ELLI].startingCoordinates.x = -32.0f;
                            npcs[ELLI].startingCoordinates.z = 16.0f;
                            npcs[ELLI].flags |= NPC_ACTIVE;  
    
                        }             

                    }
                    
                }

                if (gSeason == SUMMER) {

                    if (gDayOfWeek == WEDNESDAY && gWeather == SUNNY && npcs[ELLI].location == 0) {

                        if (18 < gHour && gHour < 21) {
                            
                            npcs[ELLI].levelIndex = BEACH;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].startingCoordinates.z = 0.0f;
                            npcs[ELLI].direction = SOUTHWEST;
                            npcs[ELLI].defaultAnimationMode = NPC_ANIM_SPECIAL;
                            npcs[ELLI].startingCoordinates.x = -224.0f;
                            npcs[ELLI].flags |= NPC_ACTIVE;  
    
                            setSpecialDialogueBit(0x99);
                            
                        }
                        
                    }

                    if (gSeason == SUMMER && gDayOfWeek == MONDAY && gWeather == SUNNY) {
    
                        if (7 < gHour && gHour < 17) {
           
                            npcs[ELLI].levelIndex = BEACH;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = NORTHEAST;
                            npcs[ELLI].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ELLI].startingCoordinates.x = -128.0f;
                            npcs[ELLI].startingCoordinates.z = 160.0f;
                            npcs[ELLI].flags |= NPC_ACTIVE;  
    
                            setSpecialDialogueBit(0x99);
                            
                        }
                                         
                    }
                    
                }
                
            } 
        
        }
        
    }

    npcs[ELLI].animationMode = npcs[ELLI].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setAnnLocation);

void setAnnLocation(void) {

    u8 result;

    npcs[ANN].wanderRadiusX = 64;
    npcs[ANN].wanderRadiusZ = 64;
    npcs[ANN].animationIndex1 = 0;
    npcs[ANN].animationIndex2 = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(0x59)) {

        if (checkLifeEventBit(MARRIED) && gWife == ANN) {

            if (5 < gHour && gHour < 8) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = SOUTH;
                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ANN].startingCoordinates.x = -128.0f;
                npcs[ANN].startingCoordinates.z = -64.0f;
                npcs[ANN].flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (npcs[ANN].location < 2) {
                                                    
                        npcs[ANN].levelIndex = RICK_STORE;
                        npcs[ANN].startingCoordinates.x = 0.0f;
                        npcs[ANN].startingCoordinates.y = 0.0f;
                        npcs[ANN].startingCoordinates.z = 0.0f;
                        npcs[ANN].direction = SOUTHWEST;
                        npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[ANN].flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(0xA1);

                    } else  {

                        if (gWeather == SUNNY) {
                            
                            npcs[ANN].levelIndex = RANCH;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[ANN].startingCoordinates.x = -64.0f;
                            npcs[ANN].startingCoordinates.z = 128.0f;
                            npcs[ANN].flags |= NPC_ACTIVE;  
                            
                        } else if (getTotalFarmAnimalsCount() != 0) {

                            npcs[ANN].levelIndex = BARN;
                            npcs[ANN].startingCoordinates.x = 0.0f;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].startingCoordinates.z = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[ANN].flags |= NPC_ACTIVE;
                            
                        } else {

                            if (getTotalChickenCount()) {

                                npcs[ANN].levelIndex = COOP;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].flags |= NPC_ACTIVE;
                                
                            } else {
                                
                                npcs[ANN].levelIndex = HOUSE;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].flags |= NPC_ACTIVE;
                                
                            }
                            
                        }
                        
                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                                                
                        npcs[ANN].levelIndex = FARM;
                        npcs[ANN].startingCoordinates.y = 0.0f;
                        npcs[ANN].direction = SOUTHWEST;
                        npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[ANN].startingCoordinates.x = -256.0f;
                        npcs[ANN].startingCoordinates.z = -320.0f;
                        npcs[ANN].flags |= NPC_ACTIVE;
                        
                    } else {
                                                
                        if (getTotalFarmAnimalsCount()) {

                            npcs[ANN].levelIndex = BARN;
                            npcs[ANN].startingCoordinates.x = 0.0f;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].startingCoordinates.z = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[ANN].flags |= NPC_ACTIVE;
                            
                        } else {

                            if (getTotalChickenCount()) {

                                npcs[ANN].levelIndex = COOP;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].flags |= NPC_ACTIVE;
                                
                            } else {
                                
                                npcs[ANN].levelIndex = HOUSE;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].flags |= NPC_ACTIVE;
                                
                            }
                            
                        }
                        
                    }
                                        
                }
                
            }

            if (gHour == 12) {
                
                npcs[ANN].levelIndex = KITCHEN;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = NORTHWEST;
                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ANN].startingCoordinates.x = -160.0f;
                npcs[ANN].startingCoordinates.z = -64.0f;
                npcs[ANN].flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.x = 0.0f;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].startingCoordinates.z = 0.0f;
                npcs[ANN].direction = SOUTHWEST;
                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[ANN].flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[ANN].levelIndex = KITCHEN;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = NORTHWEST;
                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ANN].startingCoordinates.x = -160.0f;
                npcs[ANN].startingCoordinates.z = -64.0f;
                npcs[ANN].flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = SOUTHEAST;
                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ANN].startingCoordinates.x = -128.0f;
                npcs[ANN].startingCoordinates.z = -112.0f;
                npcs[ANN].flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = SOUTHWEST;
                npcs[ANN].defaultAnimationMode = NPC_ANIM_FIXED;
                npcs[ANN].startingCoordinates.x = -192.0f;
                npcs[ANN].startingCoordinates.z = -160.0f;
                npcs[ANN].flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[ANN].animationIndex1 = 0x6F;
                npcs[ANN].animationIndex2 = 0x79;
            }
    
        } else if (!checkDailyEventBit(FESTIVAL)) {

            if (checkLifeEventBit(ANN_CLIFF_MARRIED)) {

                switch (gDayOfWeek) {

                    case SUNDAY:
                    case MONDAY:
                    case TUESDAY:
                    case WEDNESDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (gWeather == SUNNY) {
                            
                            if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].startingCoordinates.x = -64.0f;
                                npcs[ANN].startingCoordinates.z = 128.0f;
                                npcs[ANN].flags |= NPC_ACTIVE;  
                                
                            }
                            
                        } else {

                            if (7 < gHour && gHour < 17) {
                                
                                npcs[ANN].levelIndex = RANCH_HOUSE;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHEAST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ANN].startingCoordinates.x = -192.0f;
                                npcs[ANN].startingCoordinates.z = -32.0f;
                                npcs[ANN].flags |= NPC_ACTIVE;  
                                
                            }
                            
                        }

                        break;

                    case THURSDAY:

                        if (gWeather == SUNNY) {

                            if (8 < gHour && gHour < 17) {

                                switch (gSeason) {

                                    case SPRING:
                                    case AUTUMN:
                                        npcs[ANN].levelIndex = MOUNTAIN_2;
                                        npcs[ANN].startingCoordinates.y = 0.0f;
                                        npcs[ANN].direction = SOUTHWEST;
                                        npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                        npcs[ANN].startingCoordinates.x = -64.0f;
                                        npcs[ANN].startingCoordinates.z = -160.0f;
                                        npcs[ANN].flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(0x9E);

                                        break;

                                    case SUMMER:
                                        
                                        npcs[ANN].levelIndex = CAVE;
                                        npcs[ANN].startingCoordinates.y = 0.0f;
                                        npcs[ANN].direction = SOUTHWEST;
                                        npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                        npcs[ANN].startingCoordinates.x = 64.0f;
                                        npcs[ANN].startingCoordinates.z = -224.0f;
                                        npcs[ANN].flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(0x9E);

                                        break;

                                    case WINTER:
                                                                                
                                        npcs[ANN].levelIndex = RANCH_HOUSE;
                                        npcs[ANN].startingCoordinates.y = 0.0f;
                                        npcs[ANN].direction = SOUTHEAST;
                                        npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                        npcs[ANN].startingCoordinates.x = -192.0f;
                                        npcs[ANN].startingCoordinates.z = -32.0f;
                                        npcs[ANN].flags |= NPC_ACTIVE;  
                                        
                                        break;
                                    
                                    }
                                
                            }
                            
                        } else if (7 < gHour && gHour < 17) {
                            
                            npcs[ANN].levelIndex = RANCH_BARN;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ANN].startingCoordinates.x = -128.0f;
                            npcs[ANN].startingCoordinates.z = -292.0f;
                            npcs[ANN].flags |= NPC_ACTIVE;  
    
                        }

                        break;
                    
                }
                
                if (getNPCBabyCarryingState(ANN) == 1) {
                    npcs[ANN].animationIndex1 = 0x6F;
                    npcs[ANN].animationIndex2 = 0x79;
                }
                
            } else {

                switch (gDayOfWeek) {

                    case MONDAY:
                    case TUESDAY:
                    case WEDNESDAY:
                    case FRIDAY:
                    case SATURDAY:
                        
                        if (gWeather == SUNNY) {
                           
                            if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].startingCoordinates.x = -64.0f;
                                npcs[ANN].startingCoordinates.z = 128.0f;
                                npcs[ANN].flags |= NPC_ACTIVE;  
                                
                            }
                            
                        } else {
                            
                            if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH_BARN;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ANN].startingCoordinates.x = -128.0f;
                                npcs[ANN].startingCoordinates.z = -292.0f;
                                npcs[ANN].flags |= NPC_ACTIVE;  
        
                            }
                            
                        }
                        
                        break;
                    
                }

                if (gDayOfWeek == THURSDAY) {

                    if (gWeather == SUNNY) {

                        switch (npcs[ANN].location) {

                            case 0:

                                if (8 < gHour && gHour < 17) {
                        
                                    npcs[ANN].levelIndex = MOUNTAIN_1;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[ANN].startingCoordinates.x = 128.0f;
                                    npcs[ANN].startingCoordinates.z = 32.0f;
                                    npcs[ANN].flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(0x9E);
                                    
                                }        
                                
                                break;

                            case 1:
                                
                                if (8 < gHour && gHour < 17) {
                                
                                    npcs[ANN].levelIndex = MOUNTAIN_2;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[ANN].startingCoordinates.x = -64.0f;
                                    npcs[ANN].startingCoordinates.z = -160.0f;
                                    npcs[ANN].flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(0x9E);
                                    
                                }            
                                break;
                            
                            case 2:

                                if (8 < gHour && gHour < 17) {
                        
                                    npcs[ANN].levelIndex = VINEYARD;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[ANN].startingCoordinates.x = 16.0f;
                                    npcs[ANN].startingCoordinates.z = -144.0f;
                                    npcs[ANN].flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(0xA0);
                                    
                                }        
                                
                                break;

                            
                            case 3:

                                if (8 < gHour && gHour < 17) {
                                    
                                    npcs[ANN].levelIndex = RICK_STORE;
                                    npcs[ANN].startingCoordinates.x = 0.0f;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].startingCoordinates.z = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[ANN].flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(0xA1);
                                    
                                }
                            
                        }
                            
                        
                            
                        
                        
                    } else {
                        
                       if (8 < gHour && gHour < 17) {
                        
                            npcs[ANN].levelIndex = RANCH_STORE;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ANN].startingCoordinates.x = 80.0f;
                            npcs[ANN].startingCoordinates.z = -144.0f;
                            npcs[ANN].flags |= NPC_ACTIVE;  
    
                                
                        }      
                        
                    }
                   
                }

                if (gDayOfWeek == SUNDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[ANN].location < 2) {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = POND;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[ANN].startingCoordinates.x = 80.0f;
                                npcs[ANN].startingCoordinates.z = -176.0f;
                                npcs[ANN].flags |= NPC_ACTIVE;  
        
                            }            
                            
                        } else {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[ANN].startingCoordinates.x = 256.0f;
                                npcs[ANN].startingCoordinates.z = 128.0f;
                                npcs[ANN].flags |= NPC_ACTIVE;  
        
                            }       
                            
                        }
                        
                    } else {
                             
                        if (8 < gHour && gHour < 17) {
                            
                            npcs[ANN].levelIndex = RANCH_BARN;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[ANN].startingCoordinates.x = -128.0f;
                            npcs[ANN].startingCoordinates.z = -292.0f;
                            npcs[ANN].flags |= NPC_ACTIVE;  
    
                        }             
                    }
                    
                }


                if (gSeason == SUMMER && gDayOfWeek == TUESDAY && gWeather == SUNNY && npcs[ANN].location == 0) {

                    if (17 < gHour && gHour < 21) {
       
                        npcs[ANN].levelIndex = BEACH;
                        npcs[ANN].startingCoordinates.y = 0.0f;
                        npcs[ANN].startingCoordinates.z = 0.0f;
                        npcs[ANN].direction = SOUTHWEST;
                        npcs[ANN].defaultAnimationMode = NPC_ANIM_SPECIAL;
                        npcs[ANN].startingCoordinates.x = -224.0f;
                        npcs[ANN].flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(0x9F);
                        
                    }
                                     
                    
                }
                
            } 
        
        }
        
    }

    npcs[ANN].animationMode = npcs[ANN].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setKarenLocation);

void setKarenLocation(void) {

    u8 result;

    npcs[KAREN].wanderRadiusX = 64;
    npcs[KAREN].wanderRadiusZ = 64;
    npcs[KAREN].animationIndex1 = 0;
    npcs[KAREN].animationIndex2 = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkLifeEventBit(0x2F) && !checkDailyEventBit(0x5A)) {

        if (checkLifeEventBit(MARRIED) && gWife == KAREN) {

            if (5 < gHour && gHour < 8) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = SOUTH;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[KAREN].startingCoordinates.x = -128.0f;
                npcs[KAREN].startingCoordinates.z = -64.0f;
                npcs[KAREN].flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    switch (npcs[KAREN].location) {

                        case 0:
                        case 1:
                            npcs[KAREN].levelIndex = BEACH;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].startingCoordinates.z = 0.0f;
                            npcs[KAREN].direction = SOUTHEAST;
                            npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[KAREN].startingCoordinates.x = -224.0f;
                            npcs[KAREN].flags |= NPC_ACTIVE;

                            setSpecialDialogueBit(0xA9);

                            break;
                        
                        case 2:
                                                        
                            npcs[KAREN].levelIndex = VINEYARD;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].startingCoordinates.z = 0.0f;
                            npcs[KAREN].direction = NORTHWEST;
                            npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[KAREN].startingCoordinates.x = -32.0f;
                            npcs[KAREN].flags |= NPC_ACTIVE;
                            
                            break;
                        
                        case 3:

                            npcs[KAREN].levelIndex = MOUNTAIN_1;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].direction = SOUTHEAST;
                            npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[KAREN].startingCoordinates.x = 96.0f;
                            npcs[KAREN].startingCoordinates.z = 96.0f;
                            npcs[KAREN].flags |= NPC_ACTIVE;

                            setSpecialDialogueBit(0xAa);

                            break;
                        
                    } 
                    
                } else if (gWeather == SUNNY) {
                        
                    npcs[KAREN].levelIndex = FARM;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAREN].startingCoordinates.x = -256.0f;
                    npcs[KAREN].startingCoordinates.z = -320.0f;
                    npcs[KAREN].flags |= NPC_ACTIVE;
                    
                } else {

                    npcs[KAREN].levelIndex = KITCHEN;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].direction = NORTHWEST;
                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[KAREN].startingCoordinates.x = -160.0f;
                    npcs[KAREN].startingCoordinates.z = -64.0f;
                    npcs[KAREN].flags |= NPC_ACTIVE;
                    
                }
                
            }

            if (gHour == 12) {
                
                npcs[KAREN].levelIndex = KITCHEN;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = NORTHWEST;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[KAREN].startingCoordinates.x = -160.0f;
                npcs[KAREN].startingCoordinates.z = -64.0f;
                npcs[KAREN].flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.x = 0.0f;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].startingCoordinates.z = 0.0f;
                npcs[KAREN].direction = SOUTHWEST;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[KAREN].flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[KAREN].levelIndex = KITCHEN;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = NORTHWEST;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[KAREN].startingCoordinates.x = -160.0f;
                npcs[KAREN].startingCoordinates.z = -64.0f;
                npcs[KAREN].flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = SOUTHEAST;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[KAREN].startingCoordinates.x = -128.0f;
                npcs[KAREN].startingCoordinates.z = -112.0f;
                npcs[KAREN].flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = SOUTHWEST;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_FIXED;
                npcs[KAREN].startingCoordinates.x = -192.0f;
                npcs[KAREN].startingCoordinates.z = -160.0f;
                npcs[KAREN].flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[KAREN].animationIndex1 = 0x66;
                npcs[KAREN].animationIndex2 = 0x70;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1E)) {

                if (gWeather == SUNNY) {

                    switch (gDayOfWeek) {

                        case SUNDAY:

                            if (9 < gHour && gHour < 18) {
                                
                                npcs[KAREN].levelIndex = BEACH;
                                npcs[KAREN].startingCoordinates.y = 0.0f;
                                npcs[KAREN].startingCoordinates.z = 0.0f;
                                npcs[KAREN].direction = SOUTHEAST;
                                npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[KAREN].startingCoordinates.x = -224.0f;
                                npcs[KAREN].flags |= NPC_ACTIVE;
            
                                setSpecialDialogueBit(0xA9);
                                
                            }

                            break;

                        case MONDAY:
                        case THURSDAY:
                            
                            if (8 < gHour && gHour < 17) { 
                            
                                npcs[KAREN].levelIndex = VINEYARD;
                                npcs[KAREN].startingCoordinates.y = 0.0f;
                                npcs[KAREN].direction = NORTHEAST;
                                npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[KAREN].startingCoordinates.x = 192.0f;
                                npcs[KAREN].startingCoordinates.z = 192.0f;
                                npcs[KAREN].flags |= NPC_ACTIVE;
                                
                            }
                            
                            break;

                        case TUESDAY:
                        case WEDNESDAY:
                        case FRIDAY:
                        case SATURDAY:

                            if (8 < gHour && gHour < 17) {
                                
                                if (npcs[KAREN].location < 2) {
                                                         
                                    npcs[KAREN].levelIndex = VINEYARD_CELLAR_BASEMENT;
                                    npcs[KAREN].startingCoordinates.y = 0.0f;
                                    npcs[KAREN].direction = NORTHWEST;
                                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[KAREN].startingCoordinates.x = -32.0f;
                                    npcs[KAREN].startingCoordinates.z = -128.0f;
                                    npcs[KAREN].flags |= NPC_ACTIVE;
                                            
                                }
                                
                            }
                        
                    }

                    if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

                        if (8 < gHour && gHour < 17) {
                                             
                            npcs[KAREN].levelIndex = VINEYARD;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].startingCoordinates.z = 0.0f;
                            npcs[KAREN].direction = SOUTHWEST;
                            npcs[KAREN].defaultAnimationMode = NPC_ANIM_CUSTOM;
                            npcs[KAREN].startingCoordinates.x = -192.0f;
                            npcs[KAREN].flags |= NPC_ACTIVE;
                                              
                        }
                        
                    }
                    
                } else {

                    if (npcs[KAREN].location < 2) {

                        if (8 < gHour && gHour < 17) {

                            npcs[KAREN].levelIndex = VINEYARD_CELLAR;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].direction = NORTHWEST;
                            npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[KAREN].startingCoordinates.x = -80.0f;
                            npcs[KAREN].startingCoordinates.z = -112.0f;
                            npcs[KAREN].flags |= NPC_ACTIVE;
                            
                        }
                        
                    }
                    
                    
                }

                if (getNPCBabyCarryingState(KAREN) == 1) {
                    npcs[KAREN].animationIndex1 = 0x66;
                    npcs[KAREN].animationIndex2 = 0x70;
                }
                
            } else {

            if ((gDayOfWeek == MONDAY || gDayOfWeek == THURSDAY) && gWeather == SUNNY && 8 < gHour && gHour < 17) {
                    
                npcs[KAREN].levelIndex = VINEYARD;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = NORTHEAST;
                npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[KAREN].startingCoordinates.x = 192.0f;
                npcs[KAREN].startingCoordinates.z = 192.0f;
                npcs[KAREN].flags |= NPC_ACTIVE;
                                  
            }

            switch (gDayOfWeek) {


                case TUESDAY:
                case WEDNESDAY:
                case FRIDAY:
                case SATURDAY:
                    
                    if (gWeather == SUNNY) {

                        switch (npcs[KAREN].location) {
                            
                            case 0:

                                if (9 < gHour && gHour < 17) {
                                    
                                    npcs[KAREN].levelIndex = MOUNTAIN_2;
                                    npcs[KAREN].startingCoordinates.y = 0.0f;
                                    npcs[KAREN].direction = SOUTHWEST;
                                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[KAREN].startingCoordinates.x = -80.0f;
                                    npcs[KAREN].startingCoordinates.z = 160.0f;
                                    npcs[KAREN].flags |= NPC_ACTIVE;
        
                                    setSpecialDialogueBit(0xAA);
                                    
                                } 
                            
                                break;

                            case 1:

                                if (9 < gHour && gHour < 17) {
                                    
                                    npcs[KAREN].levelIndex = BEACH;
                                    npcs[KAREN].startingCoordinates.y = 0.0f;
                                    npcs[KAREN].direction = SOUTHEAST;
                                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[KAREN].startingCoordinates.x = -96.0f;
                                    npcs[KAREN].startingCoordinates.z = 176.0f;
                                    npcs[KAREN].flags |= NPC_ACTIVE;
                
                                    setSpecialDialogueBit(0xA9);
                                    
                                }

                                break;
                            
                            case 2:
                            case 3:
                                break;
                            
                        }
                        
                    }


                
            }

            if (gDayOfWeek == SUNDAY && gWeather == SUNNY) {

                if (9 < gHour && gHour < 18) {
                    
                    npcs[KAREN].levelIndex = BEACH;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].startingCoordinates.z = 0.0f;
                    npcs[KAREN].direction = SOUTHEAST;
                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAREN].startingCoordinates.x = -224.0f;
                    npcs[KAREN].flags |= NPC_ACTIVE;

                    setSpecialDialogueBit(0xA9);
                
                    
                }
                
            } 

            if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11 && gWeather == SUNNY) {

                if (8 < gHour && gHour < 17) {
                    
                    npcs[KAREN].levelIndex = VINEYARD;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].startingCoordinates.z = 0.0f;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_CUSTOM;
                    npcs[KAREN].startingCoordinates.x = -192.0f;
                    npcs[KAREN].flags |= NPC_ACTIVE;
                    
                }
                
            }

            if NIGHTTIME {

                if (3 < gDayOfWeek && gDayOfWeek < 7) {
                    
                    npcs[KAREN].levelIndex = TAVERN;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[KAREN].startingCoordinates.x = 64.0f;
                    npcs[KAREN].startingCoordinates.z = -32.0f;
                    npcs[KAREN].flags |= NPC_ACTIVE;

                    setSpecialDialogueBit(0xAB);
                    
                }

                if (gDayOfWeek == MONDAY) {

                    npcs[KAREN].levelIndex = TAVERN;
                    npcs[KAREN].startingCoordinates.y = 0;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAREN].startingCoordinates.x = -64.0f;
                    npcs[KAREN].startingCoordinates.z = -64.0f;
                    npcs[KAREN].flags |= NPC_ACTIVE;

                    setSpecialDialogueBit(0xAB);
                    
                }
                
            }

            if (gSeason == SUMMER && gDayOfWeek == SATURDAY && gWeather == SUNNY && npcs[KAREN].location == 0) {

                if (18 < gHour && gHour < 21) {

                        npcs[KAREN].levelIndex = BEACH;
                        npcs[KAREN].startingCoordinates.y = 0.0f;
                        npcs[KAREN].startingCoordinates.z = 0.0f;
                        npcs[KAREN].direction = SOUTHWEST;
                        npcs[KAREN].defaultAnimationMode = NPC_ANIM_SPECIAL;
                        npcs[KAREN].startingCoordinates.x = -224.0f;
                        npcs[KAREN].flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(0xA9);
                    
                }
                
            }
            
        }
            
        } 
        
    }

    npcs[KAREN].animationMode = npcs[KAREN].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setBabyLocation);

void setBabyLocation(void) {

    u8 set = FALSE;
    
    toggleDailyEventBit(0x16);

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {

        npcs[BABY].wanderRadiusX = 64;
        npcs[BABY].wanderRadiusZ = 64;
        npcs[BABY].animationIndex1 = 0;
        npcs[BABY].animationIndex2 = 8;

        set = getBabyCarryingState();

        if (set == 1) {

            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                npcs[BABY].levelIndex = npcs[gWife].levelIndex;
                npcs[BABY].startingCoordinates.x = 0.0f;
                npcs[BABY].startingCoordinates.y = 0.0f;
                npcs[BABY].startingCoordinates.z = 0.0f;
                npcs[BABY].flags |= (NPC_ACTIVE | NPC_ATTACHED);
                npcs[BABY].direction = npcs[gWife].direction;
                npcs[BABY].defaultAnimationMode = NPC_ANIM_IDLE;
                
            }
            
        }

        if (set == 2) {

            npcs[BABY].levelIndex = HOUSE;
            npcs[BABY].startingCoordinates.y = 0.0f;
            npcs[BABY].direction = SOUTHWEST;
            npcs[BABY].defaultAnimationMode = NPC_ANIM_WANDER;
            npcs[BABY].startingCoordinates.x = -192.0f;
            npcs[BABY].startingCoordinates.z = -64.0f;
            npcs[BABY].flags |= NPC_ACTIVE;

            setDailyEventBit(0x16);
            
        }

        if (set == 3) {

            npcs[BABY].levelIndex = HOUSE;
            npcs[BABY].startingCoordinates.y = 0.0f;
            npcs[BABY].direction = SOUTHWEST;
            npcs[BABY].defaultAnimationMode = NPC_ANIM_SPECIAL;
            npcs[BABY].startingCoordinates.x = -192.0f;
            npcs[BABY].startingCoordinates.z = -64.0f;
            npcs[BABY].flags |= NPC_ACTIVE;

            setDailyEventBit(0x16);
            
        } 

        if (set == 4) {

            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                npcs[BABY].levelIndex = npcs[gWife].levelIndex;
                npcs[BABY].startingCoordinates.x = npcs[gWife].startingCoordinates.x;
                npcs[BABY].startingCoordinates.y = npcs[gWife].startingCoordinates.y;
                npcs[BABY].startingCoordinates.z = npcs[gWife].startingCoordinates.z;
                npcs[BABY].flags |= NPC_ACTIVE;
                npcs[BABY].direction = npcs[gWife].direction;
                npcs[BABY].defaultAnimationMode = NPC_ANIM_CUSTOM;
                npcs[BABY].animationIndex1 = 0x39;
                npcs[BABY].animationIndex2 = 0x41;
                
            }
            
        }

        if (set == 5) {

            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                npcs[BABY].levelIndex = HOUSE;
                npcs[BABY].startingCoordinates.y = 0.0f;
                npcs[BABY].direction = SOUTHWEST;
                npcs[BABY].defaultAnimationMode = NPC_ANIM_BABY_CRIB;
                npcs[BABY].startingCoordinates.x = -194.0f;
                npcs[BABY].startingCoordinates.z = -64.0f;
                npcs[BABY].flags |= NPC_ACTIVE;
                
            }
            
        }

        if (set == 6) {
            
            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                npcs[BABY].levelIndex = npcs[gWife].levelIndex;
                npcs[BABY].startingCoordinates.x = npcs[gWife].startingCoordinates.x;
                npcs[BABY].startingCoordinates.y = npcs[gWife].startingCoordinates.y;
                npcs[BABY].startingCoordinates.z = npcs[gWife].startingCoordinates.z;
                npcs[BABY].flags |= NPC_ACTIVE;
                npcs[BABY].direction = npcs[gWife].direction;
                npcs[BABY].defaultAnimationMode = NPC_ANIM_FIXED;
                npcs[BABY].animationIndex1 = 0xE;
                npcs[BABY].animationIndex2 = 0x16;
                
            }
            
        }
        
        npcs[BABY].animationMode = npcs[BABY].defaultAnimationMode;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setHarrisLocation);

void setHarrisLocation(void) {

    npcs[HARRIS].wanderRadiusX = 0x80;
    npcs[HARRIS].wanderRadiusZ = 0x80;
    npcs[HARRIS].animationIndex1 = 0;
    npcs[HARRIS].animationIndex2 = 8;

    if (gDayOfWeek != SUNDAY && gDayOfWeek < 7) {

        if (8 < gHour && gHour < 12) {

            if (npcs[HARRIS].location < 2) {

                npcs[HARRIS].levelIndex = VILLAGE_1;
                npcs[HARRIS].startingCoordinates.x = 0.0f;
                npcs[HARRIS].startingCoordinates.y = 0.0f;
                npcs[HARRIS].startingCoordinates.z = 0.0f;
                npcs[HARRIS].direction = SOUTHWEST;
                npcs[HARRIS].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[HARRIS].flags |= NPC_ACTIVE;
                
                
            } else {

                npcs[HARRIS].levelIndex = VILLAGE_2;
                npcs[HARRIS].startingCoordinates.x = 0.0f;
                npcs[HARRIS].startingCoordinates.y = 0.0f;
                npcs[HARRIS].startingCoordinates.z = 0.0f;
                npcs[HARRIS].direction = SOUTHWEST;
                npcs[HARRIS].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[HARRIS].flags |= NPC_ACTIVE;
                
            }
            
        }
        
        if (11 < gHour && gHour < 18) { 

            switch (npcs[HARRIS].location) {

                case 0:
                case 1:
                    npcs[HARRIS].levelIndex = MOUNTAIN_1;
                    npcs[HARRIS].startingCoordinates.y = 0.0f;
                    npcs[HARRIS].direction = SOUTHWEST;
                    npcs[HARRIS].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[HARRIS].startingCoordinates.x = -16.0f;
                    npcs[HARRIS].startingCoordinates.z = 128.0f;
                    npcs[HARRIS].flags |= NPC_ACTIVE;                    
                    break;
                
                case 2:
    
                    npcs[HARRIS].levelIndex = ROAD;
                    npcs[HARRIS].startingCoordinates.y = 0.0f;
                    npcs[HARRIS].startingCoordinates.z = 0.0f;
                    npcs[HARRIS].direction = SOUTHWEST;
                    npcs[HARRIS].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[HARRIS].startingCoordinates.x = -64.0f;
                    npcs[HARRIS].flags |= NPC_ACTIVE;
                    break;
                
                case 3:

                    npcs[HARRIS].levelIndex = RANCH;
                    npcs[HARRIS].startingCoordinates.y = 0.0f;
                    npcs[HARRIS].direction = SOUTHWEST;
                    npcs[HARRIS].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[HARRIS].startingCoordinates.x = 160.0f;
                    npcs[HARRIS].startingCoordinates.z = -320.0f;
                    npcs[HARRIS].flags |= NPC_ACTIVE;

                    break;
                
                
            }
            
        }
        
    }

    if (gDayOfWeek == SUNDAY) {

        if (11 < gHour && gHour < 17) {
            
            npcs[HARRIS].levelIndex = LIBRARY;
            npcs[HARRIS].startingCoordinates.y = 0.0f;
            npcs[HARRIS].direction = SOUTHEAST;
            npcs[HARRIS].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[HARRIS].startingCoordinates.x = 32.0f;
            npcs[HARRIS].startingCoordinates.z = 96.0f;
            npcs[HARRIS].flags |= NPC_ACTIVE;
            
        }
        
    }

    if (!checkLifeEventBit(MARIA_HARRIS_MARRIED) && 17 < gHour && gHour < 24 && npcs[HARRIS].location < 2) {

        npcs[HARRIS].levelIndex = TAVERN;
        npcs[HARRIS].startingCoordinates.y = 0.0f;
        npcs[HARRIS].startingCoordinates.z = 0.0f;
        npcs[HARRIS].direction = SOUTHWEST;
        npcs[HARRIS].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[HARRIS].startingCoordinates.x = -144.0f;
        npcs[HARRIS].flags |= NPC_ACTIVE;

        setSpecialDialogueBit(0x40);
        
    }

    npcs[HARRIS].animationMode = npcs[HARRIS].defaultAnimationMode;
    
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setGrayLocation);

void setGrayLocation(void) {

    npcs[GRAY].wanderRadiusX = 64;
    npcs[GRAY].wanderRadiusZ = 64;
    npcs[GRAY].animationIndex1 = 0;
    npcs[GRAY].animationIndex2 = 8;

    switch (gDayOfWeek) {

        case SUNDAY:
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
        case FRIDAY:
        case SATURDAY:

            if (7 < gHour && gHour < 12) {

                npcs[GRAY].levelIndex = RANCH_BARN;
                npcs[GRAY].startingCoordinates.y = 0.0f;
                npcs[GRAY].direction = SOUTHWEST;
                npcs[GRAY].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[GRAY].startingCoordinates.x = -128.0f;
                npcs[GRAY].startingCoordinates.z = -48.0f;
                npcs[GRAY].flags |= NPC_ACTIVE;
            
            }

            if (11 < gHour && gHour < 18) {

                if (gWeather == SUNNY) {
                    
                    npcs[GRAY].levelIndex = RANCH;
                    npcs[GRAY].startingCoordinates.y = 0.0f;
                    npcs[GRAY].direction = SOUTHWEST;
                    npcs[GRAY].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[GRAY].startingCoordinates.x = -160.0f;
                    npcs[GRAY].startingCoordinates.z = 128.0f;
                    npcs[GRAY].flags |= NPC_ACTIVE;
                                    
                } else {
                    
                    npcs[GRAY].levelIndex = RANCH_BARN;
                    npcs[GRAY].startingCoordinates.x = 0.0f;
                    npcs[GRAY].startingCoordinates.y = 0.0f;
                    npcs[GRAY].startingCoordinates.z = 0.0f;
                    npcs[GRAY].direction = SOUTHWEST;
                    npcs[GRAY].defaultAnimationMode = NPC_ANIM_SPECIAL;
                    npcs[GRAY].flags |= NPC_ACTIVE;
                    
                }
                
            }

            break;

        case THURSDAY:

            if (gWeather == SUNNY) {

                 if (7 < gHour && gHour < 13) {

                    npcs[GRAY].levelIndex = MOUNTAIN_2;
                    npcs[GRAY].startingCoordinates.y = 0.0f;
                    npcs[GRAY].direction = SOUTHEAST;
                    npcs[GRAY].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[GRAY].startingCoordinates.x = -128.0f;
                    npcs[GRAY].startingCoordinates.z = 352.0f;
                    npcs[GRAY].flags |= NPC_ACTIVE;

                 }
                
            } else if (7 < gHour && gHour < 18) {

                npcs[GRAY].levelIndex = RANCH_HOUSE;
                npcs[GRAY].startingCoordinates.y = 0.0f;
                npcs[GRAY].direction = SOUTHWEST;
                npcs[GRAY].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[GRAY].startingCoordinates.x = -64.0f;
                npcs[GRAY].startingCoordinates.z = -64.0f;
                npcs[GRAY].flags |= NPC_ACTIVE;
                
            }
        
    }

    if (!checkLifeEventBit(POPURI_GRAY_MARRIED) && 17 < gHour && gHour < 24 && npcs[GRAY].location < 2) {

        npcs[GRAY].levelIndex = TAVERN;
        npcs[GRAY].startingCoordinates.y = 0.0f;
        npcs[GRAY].startingCoordinates.z = 0.0f;
        npcs[GRAY].direction = SOUTHWEST;
        npcs[GRAY].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[GRAY].startingCoordinates.x = 80.0f;
        npcs[GRAY].flags |= NPC_ACTIVE;

        setSpecialDialogueBit(0x131);
        
    }

    npcs[GRAY].animationMode = npcs[GRAY].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setJeffLocation);

void setJeffLocation(void) {

    npcs[JEFF].wanderRadiusX = 64;
    npcs[JEFF].wanderRadiusZ = 64;
    npcs[JEFF].animationIndex1 = 0;
    npcs[JEFF].animationIndex2 = 8;

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        switch (gDayOfWeek) {       

            case TUESDAY ... SATURDAY:
            case SUNDAY:
                
                if (8 < gHour && gHour < 17) {
                    
                    npcs[JEFF].levelIndex = BAKERY;
                    npcs[JEFF].startingCoordinates.y = 0.0f;
                    npcs[JEFF].direction = SOUTHWEST;
                    npcs[JEFF].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[JEFF].startingCoordinates.x = -96.0f;
                    npcs[JEFF].startingCoordinates.z = -96.0f;
                    npcs[JEFF].flags |= NPC_ACTIVE;
                    
                }
                
                break;

            case MONDAY:               

                switch (gSeason) {
                    
                    case SPRING:
                    case SUMMER:

                        if (7 < gHour && gHour < 17 && gWeather == SUNNY) {

                            npcs[JEFF].levelIndex = MOUNTAIN_1;
                            npcs[JEFF].startingCoordinates.y = 0.0f;
                            npcs[JEFF].direction = NORTHWEST;
                            npcs[JEFF].defaultAnimationMode = NPC_ANIM_SPECIAL;
                            npcs[JEFF].startingCoordinates.x = -224.0f;
                            npcs[JEFF].startingCoordinates.z = -128.0f;
                            npcs[JEFF].flags |= NPC_ACTIVE;
                    
                        }

                        break;

                    case AUTUMN:
                        
                        if (7 < gHour && gHour < 15 && gWeather == SUNNY) {

                            npcs[JEFF].levelIndex = MOUNTAIN_1;
                            npcs[JEFF].startingCoordinates.y = 0.0f;
                            npcs[JEFF].direction = NORTHEAST;
                            npcs[JEFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[JEFF].startingCoordinates.x = 192.0f;
                            npcs[JEFF].startingCoordinates.z = 48.0f;
                            npcs[JEFF].flags |= NPC_ACTIVE;
                    
                        }
                        
                        break;
                }

                break;

        }
        
    } else {

        switch (gDayOfWeek) {       

            case TUESDAY ... SATURDAY:
            case SUNDAY:
                
                if (8 < gHour && gHour < 17) {
                    
                    npcs[JEFF].levelIndex = BAKERY;
                    npcs[JEFF].startingCoordinates.y = 0.0f;
                    npcs[JEFF].direction = SOUTHWEST;
                    npcs[JEFF].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[JEFF].startingCoordinates.x = -96.0f;
                    npcs[JEFF].startingCoordinates.z = -96.0f;
                    npcs[JEFF].flags |= NPC_ACTIVE;
                    
                }

                break;
            
            case MONDAY:         
        
                if (7 < gHour && gHour < 17 && gWeather == SUNNY) {

                    if (npcs[JEFF].location < 2) {
                        
                        npcs[JEFF].levelIndex = MOUNTAIN_1;
                        npcs[JEFF].startingCoordinates.y = 0.0f;
                        npcs[JEFF].direction = NORTHWEST;
                        npcs[JEFF].defaultAnimationMode = NPC_ANIM_SPECIAL;
                        npcs[JEFF].startingCoordinates.x = -224.0f;
                        npcs[JEFF].startingCoordinates.z = -128.0f;
                        npcs[JEFF].flags |= NPC_ACTIVE;
                            
                    }
            
                }

                break;

        }

        if (17 < gHour && gHour < 24 && npcs[JEFF].location < 2) {
            
            npcs[JEFF].levelIndex = TAVERN;
            npcs[JEFF].startingCoordinates.y = 0.0f;
            npcs[JEFF].direction = NORTHEAST;
            npcs[JEFF].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[JEFF].startingCoordinates.x = -160.0f;
            npcs[JEFF].startingCoordinates.z = -64.0f;
            npcs[JEFF].flags |= NPC_ACTIVE;

            setSpecialDialogueBit(0x132);
    
        }
        
    }

    npcs[JEFF].animationMode = npcs[JEFF].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setCliffLocation);

void setCliffLocation(void) {

    npcs[CLIFF].wanderRadiusX = 64;
    npcs[CLIFF].wanderRadiusZ = 64;
    npcs[CLIFF].animationIndex1 = 0;
    npcs[CLIFF].animationIndex2 = 8;

    if (!checkLifeEventBit(CLIFF_GONE)) {

        if (checkLifeEventBit(ANN_CLIFF_MARRIED)) {

            switch (gDayOfWeek) {       

                case MONDAY:                                 
                case TUESDAY:                                 
                case WEDNESDAY:                                 
                case FRIDAY:                                 
                case SATURDAY:             
                    
                    if (gWeather == SUNNY) {
                        
                        if (7 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = RANCH;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[CLIFF].startingCoordinates.x = 64.0f;
                            npcs[CLIFF].startingCoordinates.z = 128.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;

                        }
                        
                    } else if (7 < gHour && gHour < 17) {

                        npcs[CLIFF].levelIndex = RANCH_BARN;
                        npcs[CLIFF].startingCoordinates.x = 0.0f;
                        npcs[CLIFF].startingCoordinates.y = 0.0f;
                        npcs[CLIFF].startingCoordinates.z = -256.0f;
                        npcs[CLIFF].direction = SOUTHWEST;
                        npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[CLIFF].flags |= NPC_ACTIVE;
                    
                    }
                    
                    break;

                case THURSDAY:          
                    
                    if (gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {

                            if (npcs[CLIFF].location < 2) {

                                npcs[CLIFF].levelIndex = MOUNTAIN_2;
                                npcs[CLIFF].startingCoordinates.y = 0.0f;
                                npcs[CLIFF].direction = SOUTH;
                                npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[CLIFF].startingCoordinates.x = -224.0f;
                                npcs[CLIFF].startingCoordinates.z = 160.0f;
                                npcs[CLIFF].flags |= NPC_ACTIVE;
                                
                            } else {
                                
                                npcs[CLIFF].levelIndex = CAVE;
                                npcs[CLIFF].startingCoordinates.y = 0.0f;
                                npcs[CLIFF].direction = SOUTHEAST;
                                npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[CLIFF].startingCoordinates.x = -256.0f;
                                npcs[CLIFF].startingCoordinates.z = -128.0f;
                                npcs[CLIFF].flags |= NPC_ACTIVE;
                                
                            }
                        
                        }
                    
                    } else {

                        if (7 < gHour && gHour < 17) {
                            
                            npcs[CLIFF].levelIndex = CARPENTER_HUT;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = EAST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = -144.0f;
                            npcs[CLIFF].startingCoordinates.z = 80.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }
                        
                    }

                    break;

            }
            
        } else if (checkLifeEventBit(CLIFF_ARRIVED) && !checkLifeEventBit(0x42)) {

            if (gWeather == SUNNY) {
                
                switch (gDayOfWeek) {    
                    
                    case SUNDAY:                             

                        if (npcs[CLIFF].location < 2 && 8 < gHour && gHour < 17) {
                            
                            npcs[CLIFF].levelIndex = MOUNTAIN_2;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTH;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = -224.0f;
                            npcs[CLIFF].startingCoordinates.z = 160.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case MONDAY:                 

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = TOP_OF_MOUNTAIN_1;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].startingCoordinates.z = 0.0f;
                            npcs[CLIFF].direction = NORTHWEST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = 192.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }

                        break;
                    
                    case TUESDAY:                             

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = BEACH;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].startingCoordinates.z = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[CLIFF].startingCoordinates.x = -224.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case WEDNESDAY:                             

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = RANCH;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = -32.0f;
                            npcs[CLIFF].startingCoordinates.z = -96.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case THURSDAY:                             

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = MOUNTAIN_2;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = NORTHWEST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = 64.0f;
                            npcs[CLIFF].startingCoordinates.z = -160.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case FRIDAY:                             
                    case SATURDAY:      

                        if (npcs[CLIFF].location < 2 && 8 < gHour && gHour < 17) {
                            
                            npcs[CLIFF].levelIndex = MOUNTAIN_1;
                            npcs[CLIFF].startingCoordinates.x = 0.0f;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].startingCoordinates.z = 0.0f;
                            npcs[CLIFF].direction = SOUTHWEST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_WANDER;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }

                        break;
                    
                }
            
            } else if (8 < gHour && gHour < 17) {

                if (npcs[CLIFF].location < 2) {

                    npcs[CLIFF].levelIndex = CAVE;
                    npcs[CLIFF].startingCoordinates.y = 0.0f;
                    npcs[CLIFF].direction = SOUTHEAST;
                    npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[CLIFF].startingCoordinates.x = -256.0f;
                    npcs[CLIFF].startingCoordinates.z = -128.0f;
                    npcs[CLIFF].flags |= NPC_ACTIVE;
                    
                } else {

                    npcs[CLIFF].levelIndex = CARPENTER_HUT;
                    npcs[CLIFF].startingCoordinates.y = 0.0f;
                    npcs[CLIFF].direction = EAST;
                    npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[CLIFF].startingCoordinates.x = -144.0f;
                    npcs[CLIFF].startingCoordinates.z = 80.0f;
                    npcs[CLIFF].flags |= NPC_ACTIVE;
                
                }
                
            }

            switch (npcs[CLIFF].location) {

                case 0:
                    
                    if (gWeather == SUNNY) {
    
                        if (16 < gHour && gHour < 24) {
                            
                            npcs[CLIFF].levelIndex = MOUNTAIN_1;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = 96.0f;
                            npcs[CLIFF].startingCoordinates.z = -16.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
                            
                        }
                    
                    }
                        
                    break;

                case 1:
                case 2:

                    if NIGHTTIME {

                        if (npcs[CLIFF].location < 2) {
                            
                            npcs[CLIFF].levelIndex = TAVERN;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = NORTHEAST;
                            npcs[CLIFF].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[CLIFF].startingCoordinates.x = 80.0f;
                            npcs[CLIFF].startingCoordinates.z = -64.0f;
                            npcs[CLIFF].flags |= NPC_ACTIVE;
            
                            setSpecialDialogueBit(0x133);
                        
                        }
                        
                    }

                    break;
                
            }
            
        }
        
         npcs[CLIFF].animationMode = npcs[CLIFF].defaultAnimationMode;
        
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setKaiLocation);

void setKaiLocation(void) {

    npcs[KAI].wanderRadiusX = 64;
    npcs[KAI].wanderRadiusZ = 64;
    npcs[KAI].animationIndex1 = 0;
    npcs[KAI].animationIndex2 = 8;

    if (!checkLifeEventBit(KAI_GONE) && !checkDailyEventBit(0x4F)) {

        if (checkLifeEventBit(KAREN_KAI_MARRIED)) {

            if (gWeather == SUNNY) {

                if (gSeason != WINTER) {

                    if (8 < gHour && gHour < 17) {
                        
                        npcs[KAI].levelIndex = VINEYARD;
                        npcs[KAI].startingCoordinates.x = 0.0f;
                        npcs[KAI].startingCoordinates.y = 0.0f;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].direction = SOUTHWEST;
                        npcs[KAI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[KAI].flags |= NPC_ACTIVE;

                    }
                    
                } else {

                    if (8 < gHour && gHour < 17) {
                        
                        npcs[KAI].levelIndex = VINEYARD_CELLAR_BASEMENT;
                        npcs[KAI].startingCoordinates.y = 0;
                        npcs[KAI].direction = SOUTHWEST;
                        npcs[KAI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[KAI].startingCoordinates.x = -32.0f;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].flags |= NPC_ACTIVE;

                    }

                }

                 if (gDayOfWeek == SUNDAY && 9 < gHour && gHour < 18) {

                    npcs[KAI].levelIndex = BEACH;
                    npcs[KAI].startingCoordinates.y = 0;
                    npcs[KAI].direction = SOUTHEAST;
                    npcs[KAI].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAI].startingCoordinates.x = -224.0f;
                    npcs[KAI].startingCoordinates.z = -32.0f;
                    npcs[KAI].flags |= NPC_ACTIVE;
                     
                 }
                
                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11  && 8 < gHour && gHour < 17) {

                    npcs[KAI].levelIndex = VINEYARD;
                    npcs[KAI].startingCoordinates.x = -224.0f;
                    npcs[KAI].startingCoordinates.y = 0.0f;
                    npcs[KAI].startingCoordinates.z = 0.0f;
                    npcs[KAI].direction = SOUTHWEST;
                    npcs[KAI].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[KAI].flags |= NPC_ACTIVE;
                    
                }
                
            } else {

                if (8 < gHour && gHour < 17) {
                    npcs[KAI].levelIndex = VINEYARD_CELLAR;
                    npcs[KAI].startingCoordinates.y = 0.0f;
                    npcs[KAI].direction = SOUTHEAST;
                    npcs[KAI].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAI].startingCoordinates.x = -64.0f;
                    npcs[KAI].startingCoordinates.z = -64.0f;
                    npcs[KAI].flags |= NPC_ACTIVE;
                    
                }

            }
            
        } else {

            if (gWeather == SUNNY) {
                
                if (gSeason != WINTER) {

                    if (8 < gHour && gHour < 17) {
                    
                        npcs[KAI].levelIndex = VINEYARD;
                        npcs[KAI].startingCoordinates.x = 0.0f;
                        npcs[KAI].startingCoordinates.y = 0;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].direction = 0;
                        npcs[KAI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[KAI].flags |= NPC_ACTIVE;
                        
                    }

                } else {

                    if (8 < gHour && gHour < 17) {
                    
                        npcs[KAI].levelIndex = VINEYARD_CELLAR_BASEMENT;
                        npcs[KAI].startingCoordinates.y = 0;
                        npcs[KAI].direction = SOUTHWEST;
                        npcs[KAI].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[KAI].startingCoordinates.x = -32.0f;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                    
            } else {

                if (8 < gHour && gHour < 17) {
                    
                    npcs[KAI].levelIndex = VINEYARD_CELLAR;
                    npcs[KAI].startingCoordinates.y = 0;
                    npcs[KAI].direction = SOUTHEAST;
                    npcs[KAI].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAI].startingCoordinates.x = -64.0f;
                    npcs[KAI].startingCoordinates.z = -64.0f;
                    npcs[KAI].flags |= NPC_ACTIVE;
                    
                }
                
            }

            if (17 < gHour && gHour < 24 && npcs[KAI].location < 2) {

                    npcs[KAI].levelIndex = TAVERN;
                    npcs[KAI].startingCoordinates.y = 0.0f;
                    npcs[KAI].direction = NORTHEAST;
                    npcs[KAI].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KAI].startingCoordinates.x = -144.0f;
                    npcs[KAI].startingCoordinates.z = 96.0f;
                    npcs[KAI].flags |= NPC_ACTIVE;

                    setSpecialDialogueBit(0x134);
                
            }
            
        }
        
        npcs[KAI].animationMode = npcs[KAI].defaultAnimationMode;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMayorLocation);

void setMayorLocation(void) {

    npcs[MAYOR].wanderRadiusX = 64;
    npcs[MAYOR].wanderRadiusZ = 64;
    npcs[MAYOR].animationIndex1 = 0;
    npcs[MAYOR].animationIndex2 = 8;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case SUNDAY:

                if (8 < gHour && gHour < 15) {
                    npcs[MAYOR].levelIndex = CHURCH;
                    npcs[MAYOR].startingCoordinates.y = 0;
                    npcs[MAYOR].direction = NORTHEAST;
                    npcs[MAYOR].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MAYOR].startingCoordinates.x = -80.0f;
                    npcs[MAYOR].startingCoordinates.z = -32.0f;
                    npcs[MAYOR].flags |= NPC_ACTIVE;
                }

                break;
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (npcs[MAYOR].location < 2) {
                    
                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = VILLAGE_2;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHWEST;
                        npcs[MAYOR].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[MAYOR].startingCoordinates.x = -224.0f;
                        npcs[MAYOR].startingCoordinates.z = 32.0f;
                        npcs[MAYOR].flags |= NPC_ACTIVE;
                    }
                    
                } else {

                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = VILLAGE_1;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHEAST;
                        npcs[MAYOR].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[MAYOR].startingCoordinates.x = -128.0f;
                        npcs[MAYOR].startingCoordinates.z = -64.0f;
                        npcs[MAYOR].flags |= NPC_ACTIVE;
                    }
                    
                }
                
                break;
            
            case SATURDAY:

                if (npcs[MAYOR].location < 2) {
                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = VILLAGE_1;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHWEST;
                        npcs[MAYOR].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[MAYOR].startingCoordinates.x = -224.0f;
                        npcs[MAYOR].startingCoordinates.z = 288.0f;
                        npcs[MAYOR].flags |= NPC_ACTIVE;
                    }
                } else {

                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = LIBRARY;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHWEST;
                        npcs[MAYOR].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MAYOR].startingCoordinates.x = 64.0f;
                        npcs[MAYOR].startingCoordinates.z = 64.0f;
                        npcs[MAYOR].flags |= NPC_ACTIVE;
                    }
                    
                }

                break;
            
        }
        
    } else if (8 < gHour && gHour < 15) {
        npcs[MAYOR].levelIndex = MAYOR_HOUSE;
        npcs[MAYOR].startingCoordinates.y = 0;
        npcs[MAYOR].direction = SOUTHWEST;
        npcs[MAYOR].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[MAYOR].startingCoordinates.x = 16.0f;
        npcs[MAYOR].startingCoordinates.z = -32.0f;
        npcs[MAYOR].flags |= NPC_ACTIVE;
    }

    npcs[MAYOR].animationMode = npcs[MAYOR].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMayorWifeLocation);

void setMayorWifeLocation(void) {

    npcs[MAYOR_WIFE].wanderRadiusX = 64;
    npcs[MAYOR_WIFE].wanderRadiusZ = 64;
    npcs[MAYOR_WIFE].animationIndex1 = 0;
    npcs[MAYOR_WIFE].animationIndex2 = 8;

    if (checkLifeEventBit(MARRIED) && gWife == MARIA) {

        switch (gDayOfWeek) {
            case SUNDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
                
                if (8 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = LIBRARY;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = 0;
                    npcs[MAYOR_WIFE].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MAYOR_WIFE].startingCoordinates.x = 48.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -96.0f;
                    npcs[MAYOR_WIFE].flags |= NPC_ACTIVE;
                }
                break;
            
            default:
                break;
            
        }
        
    } else if (gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case SUNDAY:
                if (8 < gHour && gHour < 15) {
                    npcs[MAYOR_WIFE].levelIndex = CHURCH;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = NORTHEAST;
                    npcs[MAYOR_WIFE].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MAYOR_WIFE].startingCoordinates.x = -112.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -32.0f;
                    npcs[MAYOR_WIFE].flags |= NPC_ACTIVE;
                }

                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
                
                if (7 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = MAYOR_HOUSE;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = SOUTHWEST;
                    npcs[MAYOR_WIFE].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[MAYOR_WIFE].startingCoordinates.x = -64.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -64.0f;
                    npcs[MAYOR_WIFE].flags |= NPC_ACTIVE;
                }
                
                break;

            case FRIDAY:

                if (11 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = BAKERY;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = SOUTHWEST;
                    npcs[MAYOR_WIFE].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MAYOR_WIFE].startingCoordinates.x = 64.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -32.0f;
                    npcs[MAYOR_WIFE].flags |= NPC_ACTIVE;
                }
                
                break;

            case SATURDAY:
            
                if (7 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = MAYOR_HOUSE;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = SOUTHWEST;
                    npcs[MAYOR_WIFE].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[MAYOR_WIFE].startingCoordinates.x = -64.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -64.0f;
                    npcs[MAYOR_WIFE].flags |= NPC_ACTIVE;
                }
                
                break;
            
        }
        
    } else if (7 < gHour && gHour < 17) {
        npcs[MAYOR_WIFE].levelIndex = MAYOR_HOUSE;
        npcs[MAYOR_WIFE].startingCoordinates.y = 0;
        npcs[MAYOR_WIFE].direction = NORTHWEST;
        npcs[MAYOR_WIFE].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[MAYOR_WIFE].startingCoordinates.x = 64.0f;
        npcs[MAYOR_WIFE].startingCoordinates.z = 16.0f;
        npcs[MAYOR_WIFE].flags |= NPC_ACTIVE;
    }
    
    npcs[MAYOR_WIFE].animationMode = npcs[MAYOR_WIFE].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setLilliaLocation);

void setLilliaLocation(void) {

    npcs[LILLIA].wanderRadiusX = 64;
    npcs[LILLIA].wanderRadiusZ = 64;
    npcs[LILLIA].animationIndex2 = 8;
    npcs[LILLIA].animationIndex1 = 0;

    if (gWeather != SUNNY) goto DEFAULT;

    switch (gDayOfWeek) {

        case SUNDAY:

            if (8 < gHour && gHour < 17) { 
                npcs[LILLIA].levelIndex = SQUARE;
                npcs[LILLIA].startingCoordinates.y = 0;
                npcs[LILLIA].direction = NORTHEAST;
                npcs[LILLIA].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[LILLIA].startingCoordinates.x = -144.0f;
                npcs[LILLIA].startingCoordinates.z = 256.0f;
                npcs[LILLIA].flags |= NPC_ACTIVE;
            }
            break;

        default:
            break;

        case MONDAY ... SATURDAY:
DEFAULT:
            if (8 < gHour && gHour < 17) {
                npcs[LILLIA].levelIndex = FLOWER_SHOP;
                npcs[LILLIA].startingCoordinates.y = 0;
                npcs[LILLIA].direction = SOUTHWEST;
                npcs[LILLIA].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[LILLIA].startingCoordinates.x = 64.0f;
                npcs[LILLIA].startingCoordinates.z = -128.0f;
                npcs[LILLIA].flags |= NPC_ACTIVE;
            }
    
            break;

    }
    
    npcs[LILLIA].animationMode = npcs[LILLIA].defaultAnimationMode;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setBasilLocation);

void setBasilLocation(void) {

    npcs[BASIL].wanderRadiusX = 64;
    npcs[BASIL].wanderRadiusZ = 64;
    npcs[BASIL].animationIndex1 = 0;
    npcs[BASIL].animationIndex2 = 8;

    if (!checkLifeEventBit(BASIL_IN_TOWN)) goto FUNC_END;

    if (gWeather != SUNNY) goto NOT_SUNNY;
            
    switch (gDayOfWeek) {
        case SUNDAY:
            if (8 < gHour && gHour < 17) {
                npcs[BASIL].levelIndex = SQUARE;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = NORTHEAST;
                npcs[BASIL].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[BASIL].startingCoordinates.x = -176.0f;
                npcs[BASIL].startingCoordinates.z = 256.0f;
                npcs[BASIL].flags |= NPC_ACTIVE;
            }
            break;
        case THURSDAY:
            if (7  < gHour && gHour < 18) {
                npcs[BASIL].levelIndex = VILLAGE_1;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = SOUTHEAST;
                npcs[BASIL].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[BASIL].startingCoordinates.x = 128.0f;
                npcs[BASIL].startingCoordinates.z = -416.0f;
                npcs[BASIL].flags |= NPC_ACTIVE;
            }
            break;
        case FRIDAY:
        case SATURDAY:
            if (7  < gHour && gHour < 18) {
                npcs[BASIL].levelIndex = MOUNTAIN_1;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = NORTHEAST;
                npcs[BASIL].defaultAnimationMode = NPC_ANIM_WANDER;
                npcs[BASIL].startingCoordinates.x = 192.0f;
                npcs[BASIL].startingCoordinates.z = -64.0f;
                npcs[BASIL].flags |= NPC_ACTIVE;
            }
            break;
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
NOT_SUNNY:
            if (8 < gHour && gHour < 17) {
                npcs[BASIL].levelIndex = FLOWER_SHOP;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = SOUTHWEST;
                npcs[BASIL].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[BASIL].startingCoordinates.x = -48.0f;
                npcs[BASIL].startingCoordinates.z = -64.0f;
                npcs[BASIL].flags |= NPC_ACTIVE;
            }
            break;
        default:
            break;
    }
        
    if (NIGHTTIME && npcs[BASIL].location < 2) {
        npcs[BASIL].levelIndex = 0x3B;
        npcs[BASIL].startingCoordinates.y = 0;
        npcs[BASIL].direction = SOUTHEAST;
        npcs[BASIL].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[BASIL].startingCoordinates.x = 32.0f;
        npcs[BASIL].startingCoordinates.z = 48.0f;
        npcs[BASIL].flags |= NPC_ACTIVE;
    }

FUNC_END:
    npcs[BASIL].animationMode = npcs[BASIL].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setEllenLocation);

void setEllenLocation(void) {

    npcs[ELLEN].wanderRadiusX = 64;
    npcs[ELLEN].wanderRadiusZ = 64;
    npcs[ELLEN].animationIndex1 = 0;
    npcs[ELLEN].animationIndex2 = 8;
    
    if (!checkLifeEventBit(ELLEN_DIED)) {

        if (gWeather == SUNNY && (7 < gHour && gHour < 17)) {

                npcs[ELLEN].levelIndex = VILLAGE_1;
                npcs[ELLEN].startingCoordinates.y = 0;
                npcs[ELLEN].direction = SOUTHWEST;
                npcs[ELLEN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ELLEN].startingCoordinates.x = 320.0f;
                npcs[ELLEN].startingCoordinates.z = 128.0f;
                npcs[ELLEN].flags |= NPC_ACTIVE;

        } else {

            if (7 < gHour && gHour < 17) {

                npcs[ELLEN].levelIndex = BAKERY;
                npcs[ELLEN].startingCoordinates.y = 0;
                npcs[ELLEN].direction = SOUTHWEST;
                npcs[ELLEN].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[ELLEN].startingCoordinates.x = -128.0f;
                npcs[ELLEN].startingCoordinates.z = -32.0f;
                npcs[ELLEN].flags |= NPC_ACTIVE;

            }

        }

    }
    
    npcs[ELLEN].animationMode = npcs[ELLEN].defaultAnimationMode;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setDougLocation);

void setDougLocation(void) {

    int temp = gDayOfWeek;
    int temp2;

    npcs[DOUG].wanderRadiusX = 64;
    npcs[DOUG].wanderRadiusZ = 64;
    npcs[DOUG].animationIndex1 = 0;
    npcs[DOUG].animationIndex2 = 8;

    // FIXME: something off here
    if (temp >= SUNDAY && (temp < THURSDAY || temp < 7 && (temp2 = temp) >= FRIDAY) && (7 < gHour && gHour < 17)) {
        npcs[DOUG].levelIndex = RANCH_STORE;
        npcs[DOUG].startingCoordinates.y = 0;
        npcs[DOUG].direction = SOUTHEAST;
        npcs[DOUG].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[DOUG].startingCoordinates.x = -96.0f;
        npcs[DOUG].startingCoordinates.z = 64.0f;
        npcs[DOUG].flags |= NPC_ACTIVE;
    }

    if (NIGHTTIME && npcs[DOUG].location < 2) {
        npcs[DOUG].levelIndex = TAVERN;
        npcs[DOUG].startingCoordinates.y = 0;
        npcs[DOUG].direction = NORTHWEST;
        npcs[DOUG].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[DOUG].startingCoordinates.x = -96.0f;
        npcs[DOUG].startingCoordinates.z = 48.0f;
        npcs[DOUG].flags |= NPC_ACTIVE;
    }

    npcs[DOUG].animationMode = npcs[DOUG].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setGotzLocation);

void setGotzLocation(void) {

    npcs[GOTZ].wanderRadiusX = 64;
    npcs[GOTZ].wanderRadiusZ = 64;
    npcs[GOTZ].animationIndex1 = 0;
    npcs[GOTZ].animationIndex2 = 8;

    if (gWeather == SUNNY) {

        switch (gSeason) {

            case SPRING:
            case SUMMER:
            case AUTUMN:

                if (8 < gHour && gHour < 17) {

                    npcs[GOTZ].levelIndex = VINEYARD;
                    npcs[GOTZ].startingCoordinates.y = 0.0f;
                    npcs[GOTZ].direction = SOUTHWEST;
                    npcs[GOTZ].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[GOTZ].startingCoordinates.x = 128.0f;
                    npcs[GOTZ].startingCoordinates.z = -128.0f;
                    npcs[GOTZ].flags |= NPC_ACTIVE;
                    
                }

                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

                    if (8 < gHour && gHour < 17) {
    
                        npcs[GOTZ].levelIndex = VINEYARD;
                        npcs[GOTZ].startingCoordinates.y = 0.0f;
                        npcs[GOTZ].direction = SOUTHWEST;
                        npcs[GOTZ].defaultAnimationMode = NPC_ANIM_CUSTOM;
                        npcs[GOTZ].startingCoordinates.x = -192.0f;
                        npcs[GOTZ].startingCoordinates.z = -64.0f;
                        npcs[GOTZ].flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                
                break;

            case WINTER:
                
                if (8 < gHour && gHour < 17) {

                    npcs[GOTZ].levelIndex = VINEYARD_HOUSE;
                    npcs[GOTZ].startingCoordinates.y = 0.0f;
                    npcs[GOTZ].direction = SOUTH;
                    npcs[GOTZ].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[GOTZ].startingCoordinates.x = -160.0f;
                    npcs[GOTZ].startingCoordinates.z = -32.0f;
                    npcs[GOTZ].flags |= NPC_ACTIVE;
                    
                }
                
                break;
            
        }
        
    } else {

        if (8 < gHour && gHour < 17) {

            npcs[GOTZ].levelIndex = VINEYARD_HOUSE;
            npcs[GOTZ].startingCoordinates.y = 0.0f;
            npcs[GOTZ].direction = SOUTH;
            npcs[GOTZ].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[GOTZ].startingCoordinates.x = -160.0f;
            npcs[GOTZ].startingCoordinates.z = -32.0f;
            npcs[GOTZ].flags |= NPC_ACTIVE;
            
        }
        
    } 

    if NIGHTTIME {

        if (npcs[GOTZ].location < 2) {
            
            npcs[GOTZ].levelIndex = TAVERN;
            npcs[GOTZ].startingCoordinates.y = 0.0f;
            npcs[GOTZ].direction = SOUTHWEST;
            npcs[GOTZ].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[GOTZ].startingCoordinates.x = 80.0f;
            npcs[GOTZ].startingCoordinates.z = -160.0f;
            npcs[GOTZ].flags |= NPC_ACTIVE;

        
        }
            
    }
        
    npcs[GOTZ].animationMode = npcs[GOTZ].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setSashaLocation);

void setSashaLocation(void) {

    npcs[SASHA].wanderRadiusX = 64;
    npcs[SASHA].wanderRadiusZ = 64;
    npcs[SASHA].animationIndex1 = 0;
    npcs[SASHA].animationIndex2 = 8;

    if (7 < gHour && gHour < 17) {
        
        npcs[SASHA].levelIndex = VINEYARD_HOUSE;
        npcs[SASHA].startingCoordinates.y = 0.0f;
        npcs[SASHA].direction = SOUTHWEST;
        npcs[SASHA].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[SASHA].startingCoordinates.x = -128.0f;
        npcs[SASHA].startingCoordinates.z = -96.0f;
        npcs[SASHA].flags |= NPC_ACTIVE;
        
    }

    if (gWeather == SUNNY && gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

        if (8 < gHour && gHour < 17) {
        
            npcs[SASHA].levelIndex = VINEYARD;
            npcs[SASHA].startingCoordinates.y = 0.0f;
            npcs[SASHA].direction = SOUTHWEST;
            npcs[SASHA].defaultAnimationMode = NPC_ANIM_CUSTOM;
            npcs[SASHA].startingCoordinates.x = -192.0f;
            npcs[SASHA].startingCoordinates.z = 64.0f;
            npcs[SASHA].flags |= NPC_ACTIVE;
            
        }
        
    }
    
    npcs[SASHA].animationMode = npcs[SASHA].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setPotionShopDealerLocation);

void setPotionShopDealerLocation(void) {

    npcs[POTION_SHOP_DEALER].wanderRadiusX = 64;
    npcs[POTION_SHOP_DEALER].animationIndex2 = 8;
    npcs[POTION_SHOP_DEALER].wanderRadiusZ = 64;
    npcs[POTION_SHOP_DEALER].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
                
                if (8 < gHour && gHour < 17) {

                    npcs[POTION_SHOP_DEALER].levelIndex = POTION_SHOP;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
                    npcs[POTION_SHOP_DEALER].direction = SOUTHEAST;
                    npcs[POTION_SHOP_DEALER].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.x = -96.0f;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.z = -16.0f;
                    npcs[POTION_SHOP_DEALER].flags |= NPC_ACTIVE;
                        
                    }
                
                break;
            
            case SATURDAY:
            case SUNDAY:

                if (npcs[POTION_SHOP_DEALER].location < 2) {

                    if (5 < gHour && gHour < 16) {
            
                        npcs[POTION_SHOP_DEALER].levelIndex = MOUNTAIN_2;
                        npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
                        npcs[POTION_SHOP_DEALER].direction = SOUTH;
                        npcs[POTION_SHOP_DEALER].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[POTION_SHOP_DEALER].startingCoordinates.x = -112.0f;
                        npcs[POTION_SHOP_DEALER].startingCoordinates.z = 192.0f;
                        npcs[POTION_SHOP_DEALER].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else if (8 < gHour && gHour < 17) {
            
                    npcs[POTION_SHOP_DEALER].levelIndex = CAVE;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
                    npcs[POTION_SHOP_DEALER].direction = SOUTH;
                    npcs[POTION_SHOP_DEALER].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.x = -288.0f;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.z = -224.0f;
                    npcs[POTION_SHOP_DEALER].flags |= NPC_ACTIVE;
                    
                }
    
                break;
            
        }
        
    } else if (8 < gHour && gHour < 17) {

        npcs[POTION_SHOP_DEALER].levelIndex = POTION_SHOP;
        npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
        npcs[POTION_SHOP_DEALER].direction = SOUTHEAST;
        npcs[POTION_SHOP_DEALER].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[POTION_SHOP_DEALER].startingCoordinates.x = -96.0f;
        npcs[POTION_SHOP_DEALER].startingCoordinates.z = -16.0f;
        npcs[POTION_SHOP_DEALER].flags |= NPC_ACTIVE;
            
    }
        
    npcs[POTION_SHOP_DEALER].animationMode = npcs[POTION_SHOP_DEALER].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setKentLocation);

void setKentLocation(void) {

    // FIXME: seems fake
    int temp = 0x40;

    npcs[KENT].wanderRadiusX = temp;
    npcs[KENT].wanderRadiusZ = temp;
    npcs[KENT].animationIndex2 = 8;
    npcs[KENT].animationIndex1 = 0;

    if (gWeather == SUNNY) {
        
        switch (gDayOfWeek) {

            case SUNDAY:

                switch (gSeason) {

                    case SPRING:
                    case AUTUMN:

                        switch (npcs[KENT].location) {

                            case 0:
                            case 1:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[KENT].levelIndex = MOUNTAIN_2;
                                    npcs[KENT].startingCoordinates.y = 0.0f;
                                    npcs[KENT].direction = SOUTHWEST;
                                    npcs[KENT].defaultAnimationMode = NPC_ANIM_WANDER;
                                    npcs[KENT].startingCoordinates.x = -160.0f;
                                    npcs[KENT].startingCoordinates.z = -160.0f;
                                    npcs[KENT].flags |= NPC_ACTIVE;
                                    
                                    setSpecialDialogueBit(0x137);
                                    
                                }
                                
                                break;

                            case 2:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[KENT].levelIndex = LIBRARY;
                                    npcs[KENT].startingCoordinates.y = 0.0f;
                                    npcs[KENT].startingCoordinates.z = 0.0f;
                                    npcs[KENT].direction = SOUTHEAST;
                                    npcs[KENT].defaultAnimationMode = NPC_ANIM_WANDER;
                                    npcs[KENT].startingCoordinates.x = -32.0f;
                                    npcs[KENT].flags |= NPC_ACTIVE;
                                    
                                }
           
                                break;

                            case 3:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[KENT].levelIndex = VILLAGE_2;
                                    npcs[KENT].startingCoordinates.y = 0.0f;
                                    npcs[KENT].startingCoordinates.z = 0.0f;
                                    npcs[KENT].direction = SOUTH;
                                    npcs[KENT].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[KENT].startingCoordinates.x = 288.0f;
                                    npcs[KENT].flags |= NPC_ACTIVE;
                                    
                                }

                                break;
                            
                        }
                        
                        break;

                    case SUMMER:

                        if (npcs[KENT].location < 2) {

                            if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = BEACH;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHEAST;
                                npcs[KENT].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[KENT].startingCoordinates.x = -224.0f;
                                npcs[KENT].startingCoordinates.z = -96.0f;
                                npcs[KENT].flags |= NPC_ACTIVE;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = MOUNTAIN_2;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHWEST;
                                npcs[KENT].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[KENT].startingCoordinates.x = -128.0f;
                                npcs[KENT].startingCoordinates.z = -160.0f;
                                npcs[KENT].flags |= NPC_ACTIVE;

                                setSpecialDialogueBit(0x137);
                                 
                             }
                            
                        }

                        break;

                    case WINTER:

                        if (npcs[KENT].location < 2) {

                            if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = POTION_SHOP_BEDROOM;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHWEST;
                                npcs[KENT].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[KENT].startingCoordinates.x = 32.0f;
                                npcs[KENT].startingCoordinates.z = -32.0f;
                                npcs[KENT].flags |= NPC_ACTIVE;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = MOUNTAIN_2;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHWEST;
                                npcs[KENT].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[KENT].startingCoordinates.x = -128.0f;
                                npcs[KENT].startingCoordinates.z = -160.0f;
                                npcs[KENT].flags |= NPC_ACTIVE;

                                setSpecialDialogueBit(0x137);
                                 
                             }
                            
                        }

                        break;

                }
                
                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (7 < gHour && gHour < 15) {

                    npcs[KENT].levelIndex = CHURCH;
                    npcs[KENT].startingCoordinates.y = 0.0f;
                    npcs[KENT].direction = 4;
                    npcs[KENT].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[KENT].startingCoordinates.x = 48.0f;
                    npcs[KENT].startingCoordinates.z = -32.0f;
                    npcs[KENT].flags |= NPC_ACTIVE;
                     
                 } 
                
                if (14 < gHour && gHour < 17) {
                     
                    npcs[KENT].levelIndex = VILLAGE_1;
                    npcs[KENT].startingCoordinates.x = 0.0f;
                    npcs[KENT].startingCoordinates.y = 0.0f;
                    npcs[KENT].direction = NORTHWEST;
                    npcs[KENT].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[KENT].startingCoordinates.z = 352.0f;
                    npcs[KENT].flags |= NPC_ACTIVE;
                             
                 } 
                
                break;

            case SATURDAY:

                if (npcs[KENT].location < 2) {

                    if (8 < gHour && gHour < 15) {

                        npcs[KENT].levelIndex = SQUARE;
                        npcs[KENT].startingCoordinates.x = 0.0f;
                        npcs[KENT].startingCoordinates.y = 0.0f;
                        npcs[KENT].startingCoordinates.z = 0.0f;
                        npcs[KENT].direction = NORTHEAST;
                        npcs[KENT].defaultAnimationMode = NPC_ANIM_SPECIAL;
                        npcs[KENT].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (8 < gHour && gHour < 15) {
                                    
                        npcs[KENT].levelIndex = MOUNTAIN_1;
                        npcs[KENT].startingCoordinates.y = 0.0f;
                        npcs[KENT].direction = SOUTHEAST;
                        npcs[KENT].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[KENT].startingCoordinates.x = 64.0f;
                        npcs[KENT].startingCoordinates.z = -96.0f;
                        npcs[KENT].flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(0x137);
                        
                    }
                    
                }
                
                break;
            
        }
        
    } else if (gWeather == SNOW && npcs[KENT].location >= 2) {
        
        if (7 < gHour && gHour < 17) {

            npcs[KENT].levelIndex = VILLAGE_2;
            npcs[KENT].startingCoordinates.y = 0.0f;
            npcs[KENT].direction = SOUTHWEST;
            npcs[KENT].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[KENT].startingCoordinates.x = 412.0f;
            npcs[KENT].startingCoordinates.z = -64.0f;
            npcs[KENT].flags |= NPC_ACTIVE;
            
        }
        
    } else if (7 < gHour && gHour < 17) {

        npcs[KENT].levelIndex = temp;
        npcs[KENT].startingCoordinates.x = 0.0f;
        npcs[KENT].startingCoordinates.y = 0.0f;
        npcs[KENT].direction = NORTHWEST;
        npcs[KENT].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[KENT].startingCoordinates.z = -16.0f;
        npcs[KENT].flags |= NPC_ACTIVE;
            
    }

    npcs[KENT].animationMode = npcs[KENT].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setStuLocation);

void setStuLocation(void) {
    
    // FIXME: seems fake
    int temp = 64;

    npcs[STU].wanderRadiusX = temp;
    npcs[STU].wanderRadiusZ = temp;
    npcs[STU].animationIndex2 = 8;
    npcs[STU].animationIndex1 = 0;

    if (gWeather == SUNNY) {
        
        switch (gDayOfWeek) {

            case SUNDAY:

                switch (gSeason) {

                    case SPRING:
                    case AUTUMN:

                        switch (npcs[STU].location) {

                            case 0:
                            case 1:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[STU].levelIndex = MOUNTAIN_2;
                                    npcs[STU].startingCoordinates.y = 0.0f;
                                    npcs[STU].direction = SOUTHWEST;
                                    npcs[STU].defaultAnimationMode = NPC_ANIM_WANDER;
                                    npcs[STU].startingCoordinates.x = -160.0f;
                                    npcs[STU].startingCoordinates.z = -160.0f;
                                    npcs[STU].flags |= NPC_ACTIVE;
                                    
                                }
                                
                                break;

                            case 2:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[STU].levelIndex = LIBRARY;
                                    npcs[STU].startingCoordinates.y = 0.0f;
                                    npcs[STU].direction = SOUTHEAST;
                                    npcs[STU].defaultAnimationMode = NPC_ANIM_WANDER;
                                    npcs[STU].startingCoordinates.x = -64.0f;
                                    npcs[STU].startingCoordinates.z = -64.0f;
                                    npcs[STU].flags |= NPC_ACTIVE;
                                    
                                }
           
                                break;

                            case 3:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[STU].levelIndex = VILLAGE_2;
                                    npcs[STU].startingCoordinates.y = 0.0f;
                                    npcs[STU].startingCoordinates.z = 0.0f;
                                    npcs[STU].direction = SOUTH;
                                    npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
                                    npcs[STU].startingCoordinates.x = 256.0f;
                                    npcs[STU].flags |= NPC_ACTIVE;
                                    
                                }

                                break;
                            
                        }
                        
                        break;

                    case SUMMER:

                        if (npcs[STU].location < 2) {

                            if (7 < gHour && gHour < 16) {

                                npcs[STU].levelIndex = BEACH;
                                npcs[STU].startingCoordinates.y = 0.0f;
                                npcs[STU].direction = SOUTHEAST;
                                npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
                                npcs[STU].startingCoordinates.x = -224.0f;
                                npcs[STU].startingCoordinates.z = -128.0f;
                                npcs[STU].flags |= NPC_ACTIVE;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                npcs[STU].levelIndex = MOUNTAIN_2;
                                npcs[STU].startingCoordinates.y = 0.0f;
                                npcs[STU].direction = SOUTHWEST;
                                npcs[STU].defaultAnimationMode = NPC_ANIM_WANDER;
                                npcs[STU].startingCoordinates.x = -160.0f;
                                npcs[STU].startingCoordinates.z = -160.0f;
                                npcs[STU].flags |= NPC_ACTIVE;

                             }
                            
                        }

                        break;

                    case WINTER:

                        if (7 < gHour && gHour < 16) {

                            npcs[STU].levelIndex = POTION_SHOP_BEDROOM;
                            npcs[STU].startingCoordinates.x = 0.0f;
                            npcs[STU].startingCoordinates.y = 0.0f;
                            npcs[STU].direction = SOUTHWEST;
                            npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
                            npcs[STU].startingCoordinates.z = -32.0f;
                            npcs[STU].flags |= NPC_ACTIVE;
                        
                        } 

                        break;

                }
                
                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (7 < gHour && gHour < 15) {

                    npcs[STU].levelIndex = CHURCH;
                    npcs[STU].startingCoordinates.y = 0.0f;
                    npcs[STU].direction = 4;
                    npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[STU].startingCoordinates.x = 80.0f;
                    npcs[STU].startingCoordinates.z = -32.0f;
                    npcs[STU].flags |= NPC_ACTIVE;
                     
                 } 
                
                if (14 < gHour && gHour < 17) {
                     
                    npcs[STU].levelIndex = VILLAGE_1;
                    npcs[STU].startingCoordinates.y = 0.0f;
                    npcs[STU].direction = NORTHWEST;
                    npcs[STU].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[STU].startingCoordinates.x = 32.0f;
                    npcs[STU].startingCoordinates.z = 352.0f;
                    npcs[STU].flags |= NPC_ACTIVE;
                             
                 } 
                
                break;

            case SATURDAY:

                if (npcs[STU].location < 2) {

                    if (8 < gHour && gHour < 15) {

                        npcs[STU].levelIndex = SQUARE;
                        npcs[STU].startingCoordinates.y = 0.0f;
                        npcs[STU].startingCoordinates.z = 0.0f;
                        npcs[STU].direction = NORTHEAST;
                        npcs[STU].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[STU].startingCoordinates.x = -32.0f;
                        npcs[STU].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (8 < gHour && gHour < 15) {
                                    
                        npcs[STU].levelIndex = MOUNTAIN_1;
                        npcs[STU].startingCoordinates.y = 0.0f;
                        npcs[STU].direction = SOUTHEAST;
                        npcs[STU].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[STU].startingCoordinates.x = 64.0f;
                        npcs[STU].startingCoordinates.z = -64.0f;
                        npcs[STU].flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                
                break;
            
        }
        
    } else if (gWeather == SNOW) {

        if (npcs[STU].location < 2) {
            
            if (7 < gHour && gHour < 17) {
    
                npcs[STU].levelIndex = POTION_SHOP_BEDROOM;
                npcs[STU].startingCoordinates.x = 0.0f;
                npcs[STU].startingCoordinates.y = 0.0f;
                npcs[STU].direction = SOUTHWEST;
                npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[STU].startingCoordinates.z = -32.0f;
                npcs[STU].flags |= NPC_ACTIVE;
                
            }
            
        } else {

            if (7 < gHour && gHour < 17) {
    
                npcs[STU].levelIndex = VILLAGE_2;
                npcs[STU].startingCoordinates.y = 0.0f;
                npcs[STU].direction = SOUTHWEST;
                npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
                npcs[STU].startingCoordinates.x = 444.0f;
                npcs[STU].startingCoordinates.z = -64.0f;
                npcs[STU].flags |= NPC_ACTIVE;
                
            }
            
        }
        
        
    } else if (7 < gHour && gHour < 17) {

        npcs[STU].levelIndex = temp;
        npcs[STU].startingCoordinates.x = 0.0f;
        npcs[STU].startingCoordinates.y = 0.0f;
        npcs[STU].direction = NORTHWEST;
        npcs[STU].defaultAnimationMode = NPC_ANIM_IDLE;
        npcs[STU].startingCoordinates.z = -48.0f;
        npcs[STU].flags |= NPC_ACTIVE;
            
    }

    npcs[STU].animationMode = npcs[STU].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMidwifeLocation);

void setMidwifeLocation(void) {

    npcs[MIDWIFE].wanderRadiusX = 64;
    npcs[MIDWIFE].wanderRadiusZ = 64;
    npcs[MIDWIFE].animationIndex1 = 0;
    npcs[MIDWIFE].animationIndex2 = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY)) {

        if (gWeather == SUNNY) {

            switch (gDayOfWeek) {
            
                case SUNDAY:
    
                    if (8 < gHour && gHour < 17) {
            
                        npcs[MIDWIFE].levelIndex = VILLAGE_2;
                        npcs[MIDWIFE].startingCoordinates.y = 0.0f;
                        npcs[MIDWIFE].direction = SOUTHWEST;
                        npcs[MIDWIFE].defaultAnimationMode = NPC_ANIM_WANDER;
                        npcs[MIDWIFE].startingCoordinates.x = 48.0f;
                        npcs[MIDWIFE].startingCoordinates.z = -64.0f;
                        npcs[MIDWIFE].flags |= NPC_ACTIVE;
                        
                    }
                    
                    break;
                
                case MONDAY:
                case TUESDAY:
                case WEDNESDAY:
                case THURSDAY:
                case FRIDAY:
                case SATURDAY:
                    
                    if (8 < gHour && gHour < 17) {
    
                        npcs[MIDWIFE].levelIndex = MIDWIFE_HOUSE;
                        npcs[MIDWIFE].startingCoordinates.y = 0.0f;
                        npcs[MIDWIFE].direction = SOUTHWEST;
                        npcs[MIDWIFE].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MIDWIFE].startingCoordinates.x = -16.0f;
                        npcs[MIDWIFE].startingCoordinates.z = -32.0f;
                        npcs[MIDWIFE].flags |= NPC_ACTIVE;
                        
                    }
                    
                    break;
                    
            
            }
            
        } else if (8 < gHour && gHour < 17) {

            npcs[MIDWIFE].levelIndex = MIDWIFE_HOUSE;
            npcs[MIDWIFE].startingCoordinates.y = 0.0f;
            npcs[MIDWIFE].direction = SOUTHWEST;
            npcs[MIDWIFE].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[MIDWIFE].startingCoordinates.x = -16.0f;
            npcs[MIDWIFE].startingCoordinates.z = -32.0f;
            npcs[MIDWIFE].flags |= NPC_ACTIVE;
            
        }
            
    }
    
    npcs[MIDWIFE].animationMode = npcs[MIDWIFE].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMayLocation);

void setMayLocation(void) {
    
    npcs[MAY].wanderRadiusX = 64;
    npcs[MAY].wanderRadiusZ = 64;
    npcs[MAY].animationIndex2 = 8;
    npcs[MAY].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case SUNDAY:

                if (npcs[MAY].location < 2) {
                    
                    if (8 < gHour && gHour < 15) {
            
                        npcs[MAY].levelIndex = SQUARE;
                        npcs[MAY].startingCoordinates.y = 0.0f;
                        npcs[MAY].direction = NORTHEAST;
                        npcs[MAY].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MAY].startingCoordinates.x = -32.0f;
                        npcs[MAY].startingCoordinates.z = -320.0f;
                        npcs[MAY].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {
                    
                    if (8 < gHour && gHour < 15) {
            
                        npcs[MAY].levelIndex = MOUNTAIN_2;
                        npcs[MAY].startingCoordinates.y = 0.0f;
                        npcs[MAY].direction = SOUTHEAST;
                        npcs[MAY].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[MAY].startingCoordinates.x = -160.0f;
                        npcs[MAY].startingCoordinates.z = 288.0f;
                        npcs[MAY].flags |= NPC_ACTIVE;
                        
                    }
                }
    
                break;
            
            case MONDAY ... FRIDAY:

                if (7 < gHour && gHour < 15) {
        
                    npcs[MAY].levelIndex = CHURCH;
                    npcs[MAY].startingCoordinates.y = 0.0f;
                    npcs[MAY].direction = NORTHEAST;
                    npcs[MAY].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MAY].startingCoordinates.x = -96.0f;
                    npcs[MAY].startingCoordinates.z = -32.0f;
                    npcs[MAY].flags |= NPC_ACTIVE;
                    
                }

                if (14 < gHour && gHour < 17) {
                    
                    npcs[MAY].levelIndex = VILLAGE_2;
                    npcs[MAY].startingCoordinates.y = 0.0f;
                    npcs[MAY].direction = NORTHEAST;
                    npcs[MAY].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[MAY].startingCoordinates.x = -64.0f;
                    npcs[MAY].startingCoordinates.z = 64.0f;
                    npcs[MAY].flags |= NPC_ACTIVE;
                    
                }
                
                break;
                
            case SATURDAY:

                if (8 < gHour && gHour < 15) {
        
                    npcs[MAY].levelIndex = VILLAGE_2;
                    npcs[MAY].startingCoordinates.y = 0.0f;
                    npcs[MAY].direction = SOUTHWEST;
                    npcs[MAY].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[MAY].startingCoordinates.x = 48.f;
                    npcs[MAY].startingCoordinates.z = -64.0f;
                    npcs[MAY].flags |= NPC_ACTIVE;
                    
                }
            
                break;
            
        }
        
    } else if (npcs[MAY].location < 2) {

        if (8 < gHour && gHour < 15) {
    
            npcs[MAY].levelIndex = LIBRARY;
            npcs[MAY].startingCoordinates.x = 0.0f;
            npcs[MAY].startingCoordinates.y = 0.0f;
            npcs[MAY].direction = SOUTHEAST;
            npcs[MAY].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[MAY].startingCoordinates.z = -64.0f;
            npcs[MAY].flags |= NPC_ACTIVE;
        
        }
        
    } else {
        
        if (8 < gHour && gHour < 15) {
    
            npcs[MAY].levelIndex = MIDWIFE_HOUSE;
            npcs[MAY].startingCoordinates.y = 0.0f;
            npcs[MAY].direction = SOUTHEAST;
            npcs[MAY].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[MAY].startingCoordinates.x = -96.0f;
            npcs[MAY].startingCoordinates.z = -32.0f;
            npcs[MAY].flags |= NPC_ACTIVE;
        
        }
        
    }
        
    npcs[MAY].animationMode = npcs[MAY].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setRickLocation);

void setRickLocation(void) {

    npcs[RICK].wanderRadiusX = 64;
    npcs[RICK].wanderRadiusZ = 64;
    npcs[RICK].animationIndex2 = 8;
    npcs[RICK].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case SUNDAY:

                if (npcs[RICK].location < 2) {
                    
                    if (9 < gHour && gHour < 18) {
            
                        npcs[RICK].levelIndex = SQUARE;
                        npcs[RICK].startingCoordinates.y = 0.0f;
                        npcs[RICK].direction = SOUTHEAST;
                        npcs[RICK].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[RICK].startingCoordinates.x = -192.0f;
                        npcs[RICK].startingCoordinates.z = -32.0f;
                        npcs[RICK].flags |= NPC_ACTIVE;
                        
                    }
                    
                }
    
                break;
            
            case MONDAY:
            case TUESDAY:

                if (9 < gHour && gHour < 18) {
        
                    npcs[RICK].levelIndex = RICK_STORE;
                    npcs[RICK].startingCoordinates.x = 0.0f;
                    npcs[RICK].startingCoordinates.y = 0.0f;
                    npcs[RICK].startingCoordinates.z = 0.0f;
                    npcs[RICK].direction = SOUTHWEST;
                    npcs[RICK].defaultAnimationMode = NPC_ANIM_WANDER;
                    npcs[RICK].flags |= NPC_ACTIVE;
                    
                }
                
                break;
            
            case THURSDAY:
            case FRIDAY:
                
                if (9 < gHour && gHour < 18) {
        
                    npcs[RICK].levelIndex = RICK_STORE;
                    npcs[RICK].startingCoordinates.y = 0.0f;
                    npcs[RICK].direction = SOUTHWEST;
                    npcs[RICK].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[RICK].startingCoordinates.x = -96.0f;
                    npcs[RICK].startingCoordinates.z = -96.0f;
                    npcs[RICK].flags |= NPC_ACTIVE;
                    
                }
            
                break;
            
            case SATURDAY:

                if (npcs[RICK].location < 2) {
                
                    if (9 < gHour && gHour < 18) {
    
                        npcs[RICK].levelIndex = VILLAGE_1;
                        npcs[RICK].startingCoordinates.y = 0.0f;
                        npcs[RICK].direction = SOUTHEAST;
                        npcs[RICK].defaultAnimationMode = NPC_ANIM_IDLE;
                        npcs[RICK].startingCoordinates.x = -96.0f;
                        npcs[RICK].startingCoordinates.z = 160.0f;
                        npcs[RICK].flags |= NPC_ACTIVE;
                            
                    }
                    
                }
                
                break;
            
            
        }
        
    } 
        
    npcs[RICK].animationMode = npcs[RICK].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setPastorLocation);

void setPastorLocation(void) {

    npcs[PASTOR].wanderRadiusX = 64;
    npcs[PASTOR].wanderRadiusZ = 64;
    npcs[PASTOR].animationIndex2 = 8;
    npcs[PASTOR].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SUNDAY:

                if (7 < gHour && gHour < 17) {
        
                    npcs[PASTOR].levelIndex = CHURCH;
                    npcs[PASTOR].startingCoordinates.y = 0.0f;
                    npcs[PASTOR].direction = SOUTHWEST;
                    npcs[PASTOR].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[PASTOR].startingCoordinates.x = -16.0f;
                    npcs[PASTOR].startingCoordinates.z = -192.0f;
                    npcs[PASTOR].flags |= NPC_ACTIVE;
                    
                }
                
    
                break;
            
            case SATURDAY:
                
                if (7 < gHour && gHour < 17) {

                    npcs[PASTOR].levelIndex = VILLAGE_1;
                    npcs[PASTOR].startingCoordinates.y = 0.0f;
                    npcs[PASTOR].direction = SOUTHWEST;
                    npcs[PASTOR].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[PASTOR].startingCoordinates.x = -128.0f;
                    npcs[PASTOR].startingCoordinates.z = -128.0f;
                    npcs[PASTOR].flags |= NPC_ACTIVE;
                        
                    }
                
                break;
            
            
        }
        
    } else if (7 < gHour && gHour < 17) {

        npcs[PASTOR].levelIndex = CHURCH;
        npcs[PASTOR].startingCoordinates.x = 0.0f;
        npcs[PASTOR].startingCoordinates.y = 0.0f;
        npcs[PASTOR].startingCoordinates.z = 0.0f;
        npcs[PASTOR].direction = SOUTHWEST;
        npcs[PASTOR].defaultAnimationMode = NPC_ANIM_WANDER;
        npcs[PASTOR].flags |= NPC_ACTIVE;
            
    }
        
    npcs[PASTOR].animationMode = npcs[PASTOR].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setShipperLocation);

void setShipperLocation(void) {

    npcs[SHIPPER].wanderRadiusX = 64;
    npcs[SHIPPER].wanderRadiusZ = 64;
    npcs[SHIPPER].animationIndex1 = 0;
    npcs[SHIPPER].animationIndex2 = 8;

    if (NIGHTTIME && npcs[SHIPPER].location < 2) {
        
        npcs[SHIPPER].levelIndex = TAVERN;
        npcs[SHIPPER].startingCoordinates.y = 0.0f;
        npcs[SHIPPER].direction = NORTHWEST;
        npcs[SHIPPER].defaultAnimationMode = NPC_ANIM_IDLE;
    
        npcs[SHIPPER].startingCoordinates.x = 128.0f;
        npcs[SHIPPER].startingCoordinates.z = 64.0f;
    
        npcs[SHIPPER].flags |= NPC_ACTIVE;

    }

    npcs[SHIPPER].animationMode =  npcs[SHIPPER].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setSaibaraLocation);

void setSaibaraLocation(void) {

    npcs[SAIBARA].wanderRadiusX = 64;
    npcs[SAIBARA].wanderRadiusZ = 64;
    npcs[SAIBARA].animationIndex2 = 8;
    npcs[SAIBARA].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
                
                if (7 < gHour && gHour < 17) {
        
                    npcs[SAIBARA].levelIndex = TOP_OF_MOUNTAIN_1;
                    npcs[SAIBARA].startingCoordinates.y = 0.0f;
                    npcs[SAIBARA].startingCoordinates.z = 0.0f;
                    npcs[SAIBARA].direction = SOUTHEAST;
                    npcs[SAIBARA].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[SAIBARA].startingCoordinates.x = -128.0f;
                    npcs[SAIBARA].flags |= NPC_ACTIVE;
                    
                }
                break;

            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
                
                if (7 < gHour && gHour < 17) {
        
                    npcs[SAIBARA].levelIndex = SOUVENIR_SHOP;
                    npcs[SAIBARA].startingCoordinates.y = 0.0f;
                    npcs[SAIBARA].direction = SOUTHWEST;
                    npcs[SAIBARA].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[SAIBARA].startingCoordinates.x = 16.0f;
                    npcs[SAIBARA].startingCoordinates.z = -64.0f;
                    npcs[SAIBARA].flags |= NPC_ACTIVE;
                    
                }

                break;
            
        }
        
    } else {

        if (7 < gHour && gHour < 17) {

            npcs[SAIBARA].levelIndex = SOUVENIR_SHOP;
            npcs[SAIBARA].startingCoordinates.y = 0.0f;
            npcs[SAIBARA].direction = SOUTHWEST;
            npcs[SAIBARA].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[SAIBARA].startingCoordinates.x = 16.0f;
            npcs[SAIBARA].startingCoordinates.z = -64.0f;
            npcs[SAIBARA].flags |= NPC_ACTIVE;
            
        }
        
    }

    npcs[SAIBARA].animationMode = npcs[SAIBARA].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setDukeLocation);

void setDukeLocation(void) { 

    int temp = gDayOfWeek;

    npcs[DUKE].wanderRadiusX = 64;
    npcs[DUKE].wanderRadiusZ = 64;
    npcs[DUKE].animationIndex1 = 0;
    npcs[DUKE].animationIndex2 = 8;

    switch (temp) {
        
        case MONDAY ... SATURDAY:
            if NIGHTTIME {
                npcs[DUKE].levelIndex = TAVERN;
                npcs[DUKE].startingCoordinates.y = 0.0f;
                npcs[DUKE].direction = 0;
                npcs[DUKE].defaultAnimationMode = NPC_ANIM_IDLE;
            
                npcs[DUKE].startingCoordinates.x = -128.0f;
                npcs[DUKE].startingCoordinates.z = -128.0f;
            
                npcs[DUKE].flags |= NPC_ACTIVE;
            }
            break;

        default:
            break;
        
    }

    npcs[DUKE].animationMode =  npcs[DUKE].defaultAnimationMode;

}

// alternate without switch
/*
void setDukeLocation(void) {
    
        int temp = gDayOfWeek;

        npcs[DUKE].wanderRadiusX = 64;
        npcs[DUKE].wanderRadiusZ = 64;
        npcs[DUKE].animationIndex1 = 0;
        npcs[DUKE].animationIndex2 = 8;

        if (temp < 7 && gDayOfWeek && NIGHTTIME) {
            
        npcs[DUKE].levelIndex = TAVERN;
        npcs[DUKE].startingCoordinates.y = 0.0f;
        npcs[DUKE].direction = 0;
        npcs[DUKE].defaultAnimationMode = NPC_ANIM_IDLE;

        npcs[DUKE].startingCoordinates.x = -128.0f;
        npcs[DUKE].startingCoordinates.z = -128.0f;
        
        npcs[DUKE].flags |= NPC_ACTIVE;
        
    }

    npcs[DUKE].animationMode =  npcs[DUKE].defaultAnimationMode;
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/npc", setGregLocation);

void setGregLocation(void) {

    npcs[GREG].wanderRadiusX = 64;
    npcs[GREG].wanderRadiusZ = 64;
    npcs[GREG].animationIndex1 = 0;
    npcs[GREG].animationIndex2 = 8;

    if (gSeason != WINTER && checkHaveTool(FISHING_POLE) && gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
                
                if (npcs[GREG].location < 2) {

                    if (5 < gHour && gHour < 17) {
                        
                        npcs[GREG].levelIndex = MOUNTAIN_1;
                
                        npcs[GREG].startingCoordinates.y = 0;
                        npcs[GREG].direction = NORTHWEST;
                        npcs[GREG].defaultAnimationMode = NPC_ANIM_IDLE;
                        
                        npcs[GREG].startingCoordinates.x = -160.0f;
                        npcs[GREG].startingCoordinates.z = -128.0f;
                
                        npcs[GREG].flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (5 < gHour && gHour < 17) {

                        npcs[GREG].levelIndex = MOUNTAIN_2;
                
                        npcs[GREG].startingCoordinates.y = 0;
                        npcs[GREG].direction = SOUTHEAST;
                        npcs[GREG].defaultAnimationMode = NPC_ANIM_IDLE;
                        
                        npcs[GREG].startingCoordinates.x = 96.0f;
                        npcs[GREG].startingCoordinates.z = 32.0f;
                
                        npcs[GREG].flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                
                break;
                    
            case MONDAY:
    
                if (6 < gHour && gHour < 17) {    
                
                    npcs[GREG].levelIndex = BEACH;
            
                    npcs[GREG].startingCoordinates.y = 0;
                    npcs[GREG].direction = SOUTHEAST;
                    npcs[GREG].defaultAnimationMode = NPC_ANIM_IDLE;
                    
                    npcs[GREG].startingCoordinates.x = -96.0f;
                    npcs[GREG].startingCoordinates.z = 176.0f;
            
                    npcs[GREG].flags |= NPC_ACTIVE;
                    
                }
                    
                break;
            
        }
        
    }

    npcs[GREG].animationMode = npcs[GREG].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setCarpenter1Location);

void setCarpenter1Location(void) {

    npcs[CARPENTER_1].wanderRadiusX = 64;
    npcs[CARPENTER_1].wanderRadiusZ = 64;
    npcs[CARPENTER_1].animationIndex1 = 0;
    npcs[CARPENTER_1].animationIndex2 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gSeason != WINTER) {

            if (gWeather == SUNNY) {
                
                if (7 < gHour && gHour < 17) {    
                
                    npcs[CARPENTER_1].levelIndex = MOUNTAIN_2;
                    npcs[CARPENTER_1].startingCoordinates.x = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.y = 0;
                    npcs[CARPENTER_1].direction = SOUTHWEST;
                    npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIM_SPECIAL;
                    npcs[CARPENTER_1].startingCoordinates.z = 32.0f;
                    npcs[CARPENTER_1].flags |= NPC_ACTIVE;
                    
                }
    
                 if (16 < gHour && gHour < 22) {
    
                    npcs[CARPENTER_1].levelIndex = TOP_OF_MOUNTAIN_1;
                    npcs[CARPENTER_1].startingCoordinates.x = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.y = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_1].direction = SOUTHWEST;
                    npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[CARPENTER_1].flags |= NPC_ACTIVE;

                }
                
            } else {
                
                if (7 < gHour && gHour < 18) {

                    npcs[CARPENTER_1].levelIndex = CARPENTER_HUT;
            
                    npcs[CARPENTER_1].startingCoordinates.y = 0;
                    npcs[CARPENTER_1].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_1].direction = SOUTHEAST;
                    npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[CARPENTER_1].startingCoordinates.x = -80.0f;
                    npcs[CARPENTER_1].flags |= NPC_ACTIVE;
                
                }
                
            }

        } else if (8 < gHour && gHour < 17) {

            npcs[CARPENTER_1].levelIndex = CAVE;
    
            npcs[CARPENTER_1].startingCoordinates.y = 0;
            npcs[CARPENTER_1].direction = SOUTHEAST;
            npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIM_IDLE;
            npcs[CARPENTER_1].startingCoordinates.x = -304.0f;
            npcs[CARPENTER_1].startingCoordinates.z = -80.0f;
            npcs[CARPENTER_1].flags |= NPC_ACTIVE;
        
        }
        
    } 

    npcs[CARPENTER_1].animationMode = npcs[CARPENTER_1].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setCarpenter2Location);

void setCarpenter2Location(void) {

    npcs[CARPENTER_2].wanderRadiusX = 64;
    npcs[CARPENTER_2].wanderRadiusZ = 64;
    npcs[CARPENTER_2].animationIndex1 = 0;
    npcs[CARPENTER_2].animationIndex2 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gSeason != WINTER) {

            if (gWeather == SUNNY) {

                if (npcs[CARPENTER_2].location < 2) {
                    
                    if (7 < gHour && gHour < 17) {    
                    
                        npcs[CARPENTER_2].levelIndex = MOUNTAIN_2;
                        npcs[CARPENTER_2].startingCoordinates.y = 0;
                        npcs[CARPENTER_2].direction = SOUTHWEST;
                        npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIM_SPECIAL;
                        npcs[CARPENTER_2].startingCoordinates.x = -224.0f;
                        npcs[CARPENTER_2].startingCoordinates.z = 64.0f;
                        npcs[CARPENTER_2].flags |= NPC_ACTIVE;
                        
                    }
        
                } else if (7 < gHour && gHour < 18) {
    
                    npcs[CARPENTER_2].levelIndex = 0x21;
                    npcs[CARPENTER_2].startingCoordinates.y = 0.0f;
                    npcs[CARPENTER_2].direction = NORTHEAST;
                    npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[CARPENTER_2].startingCoordinates.x = 64.0f;
                    npcs[CARPENTER_2].startingCoordinates.z = -112.0f;
                    npcs[CARPENTER_2].flags |= NPC_ACTIVE;

                }
                
            } else {
                
                if (7 < gHour && gHour < 18) {

                    npcs[CARPENTER_2].levelIndex = CARPENTER_HUT;
                    npcs[CARPENTER_2].startingCoordinates.y = 0;
                    npcs[CARPENTER_2].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_2].direction = NORTHWEST;
                    npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIM_IDLE;
                    npcs[CARPENTER_2].startingCoordinates.x = 16.0f;
                    npcs[CARPENTER_2].flags |= NPC_ACTIVE;
                
                }
                
            }

        } else if (npcs[CARPENTER_2].location == 0 && 8 < gHour && gHour < 17) {

            npcs[CARPENTER_2].levelIndex = MINE;
            npcs[CARPENTER_2].startingCoordinates.x = 0.0f;
            npcs[CARPENTER_2].startingCoordinates.y = 0;
            npcs[CARPENTER_2].startingCoordinates.z = 0.0f;
            npcs[CARPENTER_2].direction = SOUTHWEST;
            npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIM_WANDER;
            npcs[CARPENTER_2].flags |= NPC_ACTIVE;
        
        }
        
    } 

    npcs[CARPENTER_2].animationMode = npcs[CARPENTER_2].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMasterCarpenterLocation);

void setMasterCarpenterLocation(void) {

    npcs[MASTER_CARPENTER].wanderRadiusX = 64;
    npcs[MASTER_CARPENTER].wanderRadiusZ = 64;
    npcs[MASTER_CARPENTER].animationIndex1 = 0;
    npcs[MASTER_CARPENTER].animationIndex2 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gWeather == SUNNY && npcs[MASTER_CARPENTER].location >= 3) {

            if (7 < gHour && gHour < 18) {    
            
                npcs[MASTER_CARPENTER].levelIndex = MOUNTAIN_2;
        
                npcs[MASTER_CARPENTER].startingCoordinates.y = 0;
                npcs[MASTER_CARPENTER].direction = SOUTHWEST;
                npcs[MASTER_CARPENTER].defaultAnimationMode = NPC_ANIM_IDLE;
                
                npcs[MASTER_CARPENTER].startingCoordinates.x = -112.0f;
                npcs[MASTER_CARPENTER].startingCoordinates.z = 112.0f;
        
                npcs[MASTER_CARPENTER].flags |= NPC_ACTIVE;
                
            }

        } else if (7 < gHour && gHour < 18) {

            npcs[MASTER_CARPENTER].levelIndex = CARPENTER_HUT;
    
            npcs[MASTER_CARPENTER].startingCoordinates.y = 0;
            npcs[MASTER_CARPENTER].direction = SOUTHWEST;
            npcs[MASTER_CARPENTER].defaultAnimationMode = NPC_ANIM_SPECIAL;
            
            npcs[MASTER_CARPENTER].startingCoordinates.x = -32.0f;
            npcs[MASTER_CARPENTER].startingCoordinates.z = -48.0f;
    
            npcs[MASTER_CARPENTER].flags |= NPC_ACTIVE;
        
        }
        
    } 

    npcs[MASTER_CARPENTER].animationMode = npcs[MASTER_CARPENTER].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setHarvestSprite1Location);

void setHarvestSprite1Location(void) {

    npcs[HARVEST_SPRITE_1].wanderRadiusX = 64;
    npcs[HARVEST_SPRITE_1].wanderRadiusZ = 64;
    npcs[HARVEST_SPRITE_1].animationIndex1 = 0;
    npcs[HARVEST_SPRITE_1].animationIndex2 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_1].levelIndex = CAVE;
        npcs[HARVEST_SPRITE_1].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_1].direction = 0;
        npcs[HARVEST_SPRITE_1].defaultAnimationMode = NPC_ANIM_WANDER;
    
        npcs[HARVEST_SPRITE_1].startingCoordinates.x = -32.0f;
        npcs[HARVEST_SPRITE_1].startingCoordinates.z = -192.0f;
    
        npcs[HARVEST_SPRITE_1].flags |= NPC_ACTIVE;

    }

    npcs[HARVEST_SPRITE_1].animationMode =  npcs[HARVEST_SPRITE_1].defaultAnimationMode;
    
}


//INCLUDE_ASM("asm/nonmatchings/game/npc", setHarvestSprite2Location);

void setHarvestSprite2Location(void) {

    npcs[HARVEST_SPRITE_2].wanderRadiusX = 64;
    npcs[HARVEST_SPRITE_2].wanderRadiusZ = 64;
    npcs[HARVEST_SPRITE_2].animationIndex1 = 0;
    npcs[HARVEST_SPRITE_2].animationIndex2 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_2].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_2].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_2].direction = SOUTHWEST;
        npcs[HARVEST_SPRITE_2].defaultAnimationMode = NPC_ANIM_IDLE;
    
        npcs[HARVEST_SPRITE_2].startingCoordinates.x = -128.0f;
        npcs[HARVEST_SPRITE_2].startingCoordinates.z = -128.0f;
    
        npcs[HARVEST_SPRITE_2].flags |= NPC_ACTIVE;

    }

    npcs[HARVEST_SPRITE_2].animationMode =  npcs[HARVEST_SPRITE_2].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setHarvestSprite3Location);

void setHarvestSprite3Location(void) {

    npcs[HARVEST_SPRITE_3].wanderRadiusX = 64;
    npcs[HARVEST_SPRITE_3].wanderRadiusZ = 64;
    npcs[HARVEST_SPRITE_3].animationIndex1 = 0;
    npcs[HARVEST_SPRITE_3].animationIndex2 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_3].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_3].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_3].direction = NORTHWEST;
        npcs[HARVEST_SPRITE_3].defaultAnimationMode = NPC_ANIM_IDLE;
    
        npcs[HARVEST_SPRITE_3].startingCoordinates.x = 64.0f;
        npcs[HARVEST_SPRITE_3].startingCoordinates.z = -64.0f;
    
        npcs[HARVEST_SPRITE_3].flags |= NPC_ACTIVE;

    }

    npcs[HARVEST_SPRITE_3].animationMode =  npcs[HARVEST_SPRITE_3].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setSydneyLocation);

void setSydneyLocation(void) {

    npcs[SYDNEY].wanderRadiusX = 64;
    npcs[SYDNEY].wanderRadiusZ = 64;
    npcs[SYDNEY].animationIndex1 = 0;
    npcs[SYDNEY].animationIndex2 = 8;

    if (gSeason != WINTER && gWeather == SUNNY && npcs[SYDNEY].location >= 2 && (7 < gHour && gHour < 17)) {

        npcs[SYDNEY].levelIndex = MOON_MOUNTAIN;
        npcs[SYDNEY].startingCoordinates.y = 0.0f;
        npcs[SYDNEY].direction = NORTHWEST;
        npcs[SYDNEY].defaultAnimationMode = NPC_ANIM_WANDER;

        npcs[SYDNEY].startingCoordinates.x = 64.0F;
        npcs[SYDNEY].startingCoordinates.z = -32.0f;

        npcs[SYDNEY].flags |= NPC_ACTIVE;
        
    } else if (7 < gHour && gHour < 17) {
        
        npcs[SYDNEY].levelIndex = DUMPLING_HOUSE;
        npcs[SYDNEY].startingCoordinates.y = 0.0f;
        npcs[SYDNEY].direction = SOUTHEAST;
        npcs[SYDNEY].defaultAnimationMode = NPC_ANIM_IDLE;

        npcs[SYDNEY].startingCoordinates.x = -128.0f;
        npcs[SYDNEY].startingCoordinates.z = -128.0f;

        npcs[SYDNEY].flags |= NPC_ACTIVE;

    }

    npcs[SYDNEY].animationMode =  npcs[SYDNEY].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setBarleyLocation);

void setBarleyLocation(void) {

    npcs[BARLEY].wanderRadiusX = 64;
    npcs[BARLEY].wanderRadiusZ = 64;
    npcs[BARLEY].animationIndex1 = 0;
    npcs[BARLEY].animationIndex2 = 8;

    if (npcs[BARLEY].location < 2 && (7 < gHour && gHour < 17)) {

        npcs[BARLEY].levelIndex = DUMPLING_HOUSE;
        npcs[BARLEY].startingCoordinates.y = 0.0f;
        npcs[BARLEY].direction = 0;
        npcs[BARLEY].defaultAnimationMode = NPC_ANIM_WANDER;

        npcs[BARLEY].startingCoordinates.x = -32.0f;
        npcs[BARLEY].startingCoordinates.z = -128.0f;

        npcs[BARLEY].flags |= NPC_ACTIVE;
        
    } else if (7 < gHour && gHour < 17) {
        
        npcs[BARLEY].levelIndex = DUMPLING_HOUSE;
        npcs[BARLEY].startingCoordinates.x = 0.0f;
        npcs[BARLEY].startingCoordinates.y = 0.0f;
        npcs[BARLEY].direction = SOUTHWEST;
        npcs[BARLEY].defaultAnimationMode = NPC_ANIM_IDLE;

        npcs[BARLEY].startingCoordinates.z = -192.0f;

        npcs[BARLEY].flags |= NPC_ACTIVE;

    }

    npcs[BARLEY].animationMode =  npcs[BARLEY].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMrsManaLocation);

void setMrsManaLocation(void) {
    
    npcs[MRS_MANA].wanderRadiusX = 64;
    npcs[MRS_MANA].wanderRadiusZ = 64;
    npcs[MRS_MANA].animationIndex1 = 0;
    npcs[MRS_MANA].animationIndex2 = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(0x94) && NIGHTTIME) {

        npcs[MRS_MANA].levelIndex = TAVERN;
        npcs[MRS_MANA].startingCoordinates.y = 0;
        npcs[MRS_MANA].direction = NORTHWEST;
        npcs[MRS_MANA].defaultAnimationMode = NPC_ANIM_IDLE;

        npcs[MRS_MANA].startingCoordinates.x = 128.0f;
        npcs[MRS_MANA].startingCoordinates.z = -112.0f;

        npcs[MRS_MANA].flags |= NPC_ACTIVE;
        
    }

    npcs[MRS_MANA].animationMode =  npcs[MRS_MANA].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setJohnLocation);

void setJohnLocation(void) {
    
    npcs[JOHN].wanderRadiusX = 64;
    npcs[JOHN].wanderRadiusZ = 64;
    npcs[JOHN].animationIndex1 = 0;
    npcs[JOHN].animationIndex2 = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(0x94) && (9 < gHour && gHour < 16)) {

        npcs[JOHN].levelIndex = BARN;
        npcs[JOHN].startingCoordinates.y = 0;
        npcs[JOHN].direction = SOUTH;
        npcs[JOHN].defaultAnimationMode = NPC_ANIM_IDLE;

        npcs[JOHN].startingCoordinates.x = -192.0f;
        npcs[JOHN].startingCoordinates.z = -256.0f;

        npcs[JOHN].flags |= NPC_ACTIVE;
        
    }

    npcs[JOHN].animationMode =  npcs[JOHN].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setGourmetJudgeLocation);

void setGourmetJudgeLocation(void) {

    npcs[GOURMET_JUDGE].wanderRadiusX = 64;
    npcs[GOURMET_JUDGE].wanderRadiusZ = 64;
    npcs[GOURMET_JUDGE].animationIndex1 = 0;
    npcs[GOURMET_JUDGE].animationIndex2 = 8;

    if (checkLifeEventBit(0x20) && npcs[GOURMET_JUDGE].location == 0 && (8 < gHour && gHour < 17)) {

        npcs[GOURMET_JUDGE].levelIndex = ROAD;
        npcs[GOURMET_JUDGE].startingCoordinates.y = 0;
        npcs[GOURMET_JUDGE].direction = NORTHEAST;
        npcs[GOURMET_JUDGE].defaultAnimationMode = NPC_ANIM_WANDER;

        npcs[GOURMET_JUDGE].startingCoordinates.x = -32.0f;
        npcs[GOURMET_JUDGE].startingCoordinates.z = 64.0f;

        npcs[GOURMET_JUDGE].flags |= NPC_ACTIVE;

    }

    if (checkLifeEventBit(0x19) && (8 < gHour && gHour < 17)) {
        
        npcs[GOURMET_JUDGE].levelIndex = VILLAGE_1;
        npcs[GOURMET_JUDGE].startingCoordinates.y = 0;
        npcs[GOURMET_JUDGE].direction = SOUTHWEST;
        npcs[GOURMET_JUDGE].defaultAnimationMode = NPC_ANIM_WANDER;

        npcs[GOURMET_JUDGE].startingCoordinates.x = 320.0f;
        npcs[GOURMET_JUDGE].startingCoordinates.z = 288.0f;

        npcs[GOURMET_JUDGE].flags |= NPC_ACTIVE;
    
    }

     npcs[GOURMET_JUDGE].animationMode =  npcs[GOURMET_JUDGE].defaultAnimationMode; 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setMariaHarrisBabyLocation);

void setMariaHarrisBabyLocation(void) {

    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[MARIA_HARRIS_BABY].wanderRadiusX = 64;
    npcs[MARIA_HARRIS_BABY].wanderRadiusZ = 64;
    npcs[MARIA_HARRIS_BABY].animationIndex1 = 0;
    npcs[MARIA_HARRIS_BABY].animationIndex2 = 0;

    temp = getNPCBabyCarryingState(MARIA);

    if (temp == 1) {

        if (npcs[MARIA].flags & NPC_ACTIVE) {
            
            npcs[MARIA_HARRIS_BABY].startingCoordinates.x = 0.0f;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.y = 0.0f;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.z = 0.0f;

            npcs[MARIA_HARRIS_BABY].defaultAnimationMode = NPC_ANIM_IDLE;

            npcs[MARIA_HARRIS_BABY].animationIndex1 = 8;
            npcs[MARIA_HARRIS_BABY].animationIndex2 = 8;

            npcs[MARIA_HARRIS_BABY].levelIndex = npcs[MARIA].levelIndex;
            npcs[MARIA_HARRIS_BABY].flags |= (NPC_ACTIVE | NPC_ATTACHED);

            npcs[MARIA_HARRIS_BABY].direction = npcs[MARIA].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[MARIA].flags & NPC_ACTIVE) {

            npcs[MARIA_HARRIS_BABY].levelIndex = npcs[MARIA].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[MARIA].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[MARIA_HARRIS_BABY].startingCoordinates, rotationAngles);

            npcs[MARIA_HARRIS_BABY].defaultAnimationMode = NPC_ANIM_WANDER;

            npcs[MARIA_HARRIS_BABY].animationIndex1 = 0;
            npcs[MARIA_HARRIS_BABY].animationIndex2 = 0;

            npcs[MARIA_HARRIS_BABY].flags |= NPC_ACTIVE;

            npcs[MARIA_HARRIS_BABY].startingCoordinates.x = npcs[MARIA].startingCoordinates.x + vec1.x;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.y = npcs[MARIA].startingCoordinates.y;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.z = npcs[MARIA].startingCoordinates.z + vec1.z;
            
            npcs[MARIA_HARRIS_BABY].direction = npcs[MARIA].direction;
                            
        }
        
    }

    npcs[MARIA_HARRIS_BABY].animationMode = npcs[MARIA_HARRIS_BABY].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setPopuriGrayBabyLocation);

void setPopuriGrayBabyLocation(void) {
    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[POPURI_GRAY_BABY].wanderRadiusX = 64;
    npcs[POPURI_GRAY_BABY].wanderRadiusZ = 64;
    npcs[POPURI_GRAY_BABY].animationIndex1 = 0;
    npcs[POPURI_GRAY_BABY].animationIndex2 = 0;

    temp = getNPCBabyCarryingState(POPURI);

    if (temp == 1) {

        if (npcs[POPURI].flags & NPC_ACTIVE) {
            
            npcs[POPURI_GRAY_BABY].startingCoordinates.x = 0.0f;
            npcs[POPURI_GRAY_BABY].startingCoordinates.y = 0.0f;
            npcs[POPURI_GRAY_BABY].startingCoordinates.z = 0.0f;

            npcs[POPURI_GRAY_BABY].defaultAnimationMode = NPC_ANIM_IDLE;

            npcs[POPURI_GRAY_BABY].animationIndex1 = 8;
            npcs[POPURI_GRAY_BABY].animationIndex2 = 8;

            npcs[POPURI_GRAY_BABY].levelIndex = npcs[POPURI].levelIndex;
            npcs[POPURI_GRAY_BABY].flags |= (NPC_ACTIVE | NPC_ATTACHED);

            npcs[POPURI_GRAY_BABY].direction = npcs[POPURI].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[POPURI].flags & NPC_ACTIVE) {

            npcs[POPURI_GRAY_BABY].levelIndex = npcs[POPURI].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[POPURI].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[POPURI_GRAY_BABY].startingCoordinates, rotationAngles);

            npcs[POPURI_GRAY_BABY].defaultAnimationMode = NPC_ANIM_WANDER;

            npcs[POPURI_GRAY_BABY].animationIndex1 = 0;
            npcs[POPURI_GRAY_BABY].animationIndex2 = 0;

            npcs[POPURI_GRAY_BABY].flags |= NPC_ACTIVE;

            npcs[POPURI_GRAY_BABY].startingCoordinates.x = npcs[POPURI].startingCoordinates.x + vec1.x;
            npcs[POPURI_GRAY_BABY].startingCoordinates.y = npcs[POPURI].startingCoordinates.y;
            npcs[POPURI_GRAY_BABY].startingCoordinates.z = npcs[POPURI].startingCoordinates.z + vec1.z;
            
            npcs[POPURI_GRAY_BABY].direction = npcs[POPURI].direction;
                            
        }
        
    }

    npcs[POPURI_GRAY_BABY].animationMode = npcs[POPURI_GRAY_BABY].defaultAnimationMode;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setElliJeffBabyLocation);

void setElliJeffBabyLocation(void) {
    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[ELLI_JEFF_BABY].wanderRadiusX = 64;
    npcs[ELLI_JEFF_BABY].wanderRadiusZ = 64;
    npcs[ELLI_JEFF_BABY].animationIndex1 = 0;
    npcs[ELLI_JEFF_BABY].animationIndex2 = 0;

    temp = getNPCBabyCarryingState(ELLI);

    if (temp == 1) {

        if (npcs[ELLI].flags & NPC_ACTIVE) {
            
            npcs[ELLI_JEFF_BABY].startingCoordinates.x = 0.0f;
            npcs[ELLI_JEFF_BABY].startingCoordinates.y = 0.0f;
            npcs[ELLI_JEFF_BABY].startingCoordinates.z = 0.0f;

            npcs[ELLI_JEFF_BABY].defaultAnimationMode = NPC_ANIM_IDLE;

            npcs[ELLI_JEFF_BABY].animationIndex1 = 8;
            npcs[ELLI_JEFF_BABY].animationIndex2 = 8;

            npcs[ELLI_JEFF_BABY].levelIndex = npcs[ELLI].levelIndex;
            npcs[ELLI_JEFF_BABY].flags |= (NPC_ACTIVE | NPC_ATTACHED);

            npcs[ELLI_JEFF_BABY].direction = npcs[ELLI].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[ELLI].flags & NPC_ACTIVE) {

            npcs[ELLI_JEFF_BABY].levelIndex = npcs[ELLI].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[ELLI].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[ELLI_JEFF_BABY].startingCoordinates, rotationAngles);

            npcs[ELLI_JEFF_BABY].defaultAnimationMode = NPC_ANIM_WANDER;

            npcs[ELLI_JEFF_BABY].animationIndex1 = 0;
            npcs[ELLI_JEFF_BABY].animationIndex2 = 0;

            npcs[ELLI_JEFF_BABY].flags |= NPC_ACTIVE;

            npcs[ELLI_JEFF_BABY].startingCoordinates.x = npcs[ELLI].startingCoordinates.x + vec1.x;
            npcs[ELLI_JEFF_BABY].startingCoordinates.y = npcs[ELLI].startingCoordinates.y;
            npcs[ELLI_JEFF_BABY].startingCoordinates.z = npcs[ELLI].startingCoordinates.z + vec1.z;
            
            npcs[ELLI_JEFF_BABY].direction = npcs[ELLI].direction;
                            
        }
        
    }

    npcs[ELLI_JEFF_BABY].animationMode = npcs[ELLI_JEFF_BABY].defaultAnimationMode;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setAnnCliffBabyLocation);

void setAnnCliffBabyLocation(void) {
    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[ANN_CLIFF_BABY].wanderRadiusX = 64;
    npcs[ANN_CLIFF_BABY].wanderRadiusZ = 64;
    npcs[ANN_CLIFF_BABY].animationIndex1 = 0;
    npcs[ANN_CLIFF_BABY].animationIndex2 = 0;

    temp = getNPCBabyCarryingState(ANN);

    if (temp == 1) {

        if (npcs[ANN].flags & NPC_ACTIVE) {
            
            npcs[ANN_CLIFF_BABY].startingCoordinates.x = 0.0f;
            npcs[ANN_CLIFF_BABY].startingCoordinates.y = 0.0f;
            npcs[ANN_CLIFF_BABY].startingCoordinates.z = 0.0f;

            npcs[ANN_CLIFF_BABY].defaultAnimationMode = NPC_ANIM_IDLE;

            npcs[ANN_CLIFF_BABY].animationIndex1 = 8;
            npcs[ANN_CLIFF_BABY].animationIndex2 = 8;

            npcs[ANN_CLIFF_BABY].levelIndex = npcs[ANN].levelIndex;
            npcs[ANN_CLIFF_BABY].flags |= (NPC_ACTIVE | NPC_ATTACHED);

            npcs[ANN_CLIFF_BABY].direction = npcs[ANN].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[ANN].flags & NPC_ACTIVE) {

            npcs[ANN_CLIFF_BABY].levelIndex = npcs[ANN].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[ANN].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[ANN_CLIFF_BABY].startingCoordinates, rotationAngles);

            npcs[ANN_CLIFF_BABY].defaultAnimationMode = NPC_ANIM_WANDER;

            npcs[ANN_CLIFF_BABY].animationIndex1 = 0;
            npcs[ANN_CLIFF_BABY].animationIndex2 = 0;

            npcs[ANN_CLIFF_BABY].flags |= NPC_ACTIVE;

            npcs[ANN_CLIFF_BABY].startingCoordinates.x = npcs[ANN].startingCoordinates.x + vec1.x;
            npcs[ANN_CLIFF_BABY].startingCoordinates.y = npcs[ANN].startingCoordinates.y;
            npcs[ANN_CLIFF_BABY].startingCoordinates.z = npcs[ANN].startingCoordinates.z + vec1.z;
            
            npcs[ANN_CLIFF_BABY].direction = npcs[ANN].direction;
                            
        }
        
    }

    npcs[ANN_CLIFF_BABY].animationMode = npcs[ANN_CLIFF_BABY].defaultAnimationMode;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setKarenKaiBabyLocation);

void setKarenKaiBabyLocation(void) {

    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[KAREN_KAI_BABY].wanderRadiusX = 64;
    npcs[KAREN_KAI_BABY].wanderRadiusZ = 64;
    npcs[KAREN_KAI_BABY].animationIndex1 = 0;
    npcs[KAREN_KAI_BABY].animationIndex2 = 0;

    temp = getNPCBabyCarryingState(KAREN);

    if (temp == 1) {

        if (npcs[KAREN].flags & NPC_ACTIVE) {
            
            npcs[KAREN_KAI_BABY].startingCoordinates.x = 0.0f;
            npcs[KAREN_KAI_BABY].startingCoordinates.y = 0.0f;
            npcs[KAREN_KAI_BABY].startingCoordinates.z = 0.0f;

            npcs[KAREN_KAI_BABY].defaultAnimationMode = NPC_ANIM_IDLE;

            npcs[KAREN_KAI_BABY].animationIndex1 = 8;
            npcs[KAREN_KAI_BABY].animationIndex2 = 8;

            npcs[KAREN_KAI_BABY].levelIndex = npcs[KAREN].levelIndex;
            npcs[KAREN_KAI_BABY].flags |= (NPC_ACTIVE | NPC_ATTACHED);

            npcs[KAREN_KAI_BABY].direction = npcs[KAREN].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[KAREN].flags & NPC_ACTIVE) {

            npcs[KAREN_KAI_BABY].levelIndex = npcs[KAREN].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[KAREN].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[KAREN_KAI_BABY].startingCoordinates, rotationAngles);

            npcs[KAREN_KAI_BABY].defaultAnimationMode = NPC_ANIM_WANDER;

            npcs[KAREN_KAI_BABY].animationIndex1 = 0;
            npcs[KAREN_KAI_BABY].animationIndex2 = 0;

            npcs[KAREN_KAI_BABY].flags |= NPC_ACTIVE;

            npcs[KAREN_KAI_BABY].startingCoordinates.x = npcs[KAREN].startingCoordinates.x + vec1.x;
            npcs[KAREN_KAI_BABY].startingCoordinates.y = npcs[KAREN].startingCoordinates.y;
            npcs[KAREN_KAI_BABY].startingCoordinates.z = npcs[KAREN].startingCoordinates.z + vec1.z;
            
            npcs[KAREN_KAI_BABY].direction = npcs[KAREN].direction;
                            
        }
        
    }

    npcs[KAREN_KAI_BABY].animationMode = npcs[KAREN_KAI_BABY].defaultAnimationMode;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setEntomologistLocation);

void setEntomologistLocation(void) {
    
    npcs[ENTOMOLOGIST].wanderRadiusX = 64;
    npcs[ENTOMOLOGIST].wanderRadiusZ = 64;
    npcs[ENTOMOLOGIST].animationIndex1 = 0;
    npcs[ENTOMOLOGIST].animationIndex2 = 8;

    if (gYear == 2 && gSeason == SPRING && 1 < gDayOfMonth && gDayOfMonth < 21 && (5 < gHour && gHour < 18)) {

        npcs[ENTOMOLOGIST].levelIndex = MOUNTAIN_1;
        npcs[ENTOMOLOGIST].startingCoordinates.y = 0;
        npcs[ENTOMOLOGIST].direction = SOUTH;
        npcs[ENTOMOLOGIST].defaultAnimationMode = NPC_ANIM_WANDER;

        npcs[ENTOMOLOGIST].startingCoordinates.x = -32.0f;
        npcs[ENTOMOLOGIST].startingCoordinates.z = -64.0f;

        npcs[ENTOMOLOGIST].flags |= NPC_ACTIVE;
        
    }

    npcs[ENTOMOLOGIST].animationMode =  npcs[ENTOMOLOGIST].defaultAnimationMode;
    
}



/* animations */

static inline void updateAnimation(u8 index) {

    switch (npcs[index].animationMode) {

        case NPC_ANIM_IDLE:
            stopNPCMovement(index);
            break;

        case NPC_ANIM_WANDER:
            updateNPCRandomAnimation(index);
            break;

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setNPCAnimations);

void setNPCAnimations(void) {
    handleMariaAnimation();
    handlePopuriAnimation();
    handleElliAnimation();
    handleAnnAnimation();
    handleKarenAnimation();
    handleBabyAnimation();
    handleHarrisAnimation();
    handleGrayAnimation();
    handleJeffAnimation();
    handleCliffAnimation();
    handleKaiAnimation();
    handleMayorAnimation();
    handleMayorWifeAnimation();
    handleLilliaAnimation();
    handleBasilAnimation();
    handleEllenAnimation();
    handleDougAnimation();
    handleGotzAnimation();
    handleSashaAnimation();
    handlePotionShopDealerAnimation();
    handleKentAnimation();
    handleStuAnimation();
    handleMidwifeAnimation();
    handleMayAnimation();
    handleRickAnimation();
    handlePastorAnimation();
    handleShipperAnimation();
    handleSaibaraAnimation();
    handleDukeAnimation();
    handleGregAnimation();
    handleCarpenter1Animation();
    handleCarpenter2Animation();
    handleMasterCarpenterAnimation();
    handleHarvestSprite1Animation();
    handleHarvestSprite2Animation();
    handleHarvestSprite3Animation();
    handleSydneyAnimation();
    handleBarleyAnimation();
    handleMrsManaAnimation();
    handleJohnAnimation();
    handleGourmetJudgeAnimation();
    handleMariaHarrisBabyAnimation();
    handlePopuriGrayBabyAnimation();
    handleElliJeffBabyAnimation();
    handleAnnCliffBabyAnimation();
    handleKarenKaiBabyAnimation();
    handleEntomologistAnimation();
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMariaAnimation);

void handleMariaAnimation(void) {

    u16 temp;

    if (npcs[MARIA].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[MARIA].entityIndex) && npcs[MARIA].animationTimer == 0) {

            switch (npcs[MARIA].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(MARIA);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(MARIA);
                    break;

                case NPC_ANIM_SPECIAL:
                    updateNPCSpecialAnimation(MARIA);
                    break;

                case NPC_ANIM_FIXED:

                    setEntityAnimation(npcs[MARIA].entityIndex, 0x18);

                    entities[npcs[MARIA].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[MARIA].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[MARIA].entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (npcs[MARIA].animationTimer) {
            npcs[MARIA].animationTimer--;
        }
    
        updateNPCMovement(MARIA);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handlePopuriAnimation);

void handlePopuriAnimation(void) {

    u16 temp;

    if (npcs[POPURI].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[POPURI].entityIndex) && npcs[POPURI].animationTimer == 0) {

            switch (npcs[POPURI].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(POPURI);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(POPURI);
                    break;

                case NPC_ANIM_SPECIAL:
                    updateNPCSpecialAnimation(POPURI);
                    break;

                case NPC_ANIM_CUSTOM:

                    switch (npcs[POPURI].animationState) {

                        case 0:
                            npcs[POPURI].speed = 0;
                            npcs[POPURI].animationTimer = 0;

                            setEntityAnimationWithDirectionChange(npcs[POPURI].entityIndex, 0);

                            temp = getRandomNumberInRange(0, 60);

                            if (temp < 8) {
                                npcs[POPURI].direction = temp;
                                npcs[POPURI].animationState = 1;
                            }

                            if (7 < temp && temp < 21) {
                                npcs[POPURI].animationState = 2;
                            }

                            npcs[POPURI].flags |= NPC_NEEDS_UPDATE;
                            break;

                        case 1:
                            npcs[POPURI].speed = 1;
                            npcs[POPURI].animationTimer = 0;

                            setEntityAnimationWithDirectionChange(npcs[POPURI].entityIndex, 8);

                            if (getRandomNumberInRange(0, 19) <= 7) {
                                npcs[POPURI].animationState = 1;
                            } else {
                                npcs[POPURI].animationState = 0;
                            }

                            npcs[POPURI].flags |= NPC_NEEDS_UPDATE;
                            break;

                        case 2:

                            npcs[POPURI].speed = 0;
                            npcs[POPURI].animationTimer = 60;

                            setEntityAnimationWithDirectionChange(npcs[POPURI].entityIndex, 0x72);
                            npcs[POPURI].animationState = 1;

                            npcs[POPURI].flags |= NPC_NEEDS_UPDATE;
                            break;

                    }

                    break;

                case NPC_ANIM_FIXED:

                    setEntityAnimation(npcs[POPURI].entityIndex, 0x18);

                    entities[npcs[POPURI].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[POPURI].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[POPURI].entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (npcs[POPURI].animationTimer) {
            npcs[POPURI].animationTimer--;
        }
    
        updateNPCMovement(POPURI);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleElliAnimation);

void handleElliAnimation(void) {
    
    u16 temp;

    if (npcs[ELLI].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[ELLI].entityIndex) && npcs[ELLI].animationTimer == 0) {

            switch (npcs[ELLI].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(ELLI);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(ELLI);
                    break;

                case NPC_ANIM_SPECIAL:
                    updateNPCSpecialAnimation(ELLI);
                    break;

                case NPC_ANIM_FIXED:

                    setEntityAnimation(npcs[ELLI].entityIndex, 0x18);

                    entities[npcs[ELLI].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[ELLI].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[ELLI].entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (npcs[ELLI].animationTimer) {
            npcs[ELLI].animationTimer--;
        }
    
        updateNPCMovement(ELLI);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleAnnAnimation);

void handleAnnAnimation(void) {

    u16 temp;

    if (npcs[ANN].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[ANN].entityIndex) && npcs[ANN].animationTimer == 0) {

            switch (npcs[ANN].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(ANN);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(ANN);
                    break;

                case NPC_ANIM_SPECIAL:
                    updateNPCSpecialAnimation(ANN);
                    break;

                case NPC_ANIM_FIXED:

                    setEntityAnimation(npcs[ANN].entityIndex, 0x18);

                    entities[npcs[ANN].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[ANN].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[ANN].entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (npcs[ANN].animationTimer) {
            npcs[ANN].animationTimer--;
        }
    
        updateNPCMovement(ANN);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleKarenAnimation);

void handleKarenAnimation(void) {

    u16 temp;

    if (npcs[KAREN].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[KAREN].entityIndex) && npcs[KAREN].animationTimer == 0) {

            switch (npcs[KAREN].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(KAREN);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(KAREN);
                    break;

                case NPC_ANIM_SPECIAL:
                    updateNPCSpecialAnimation(KAREN);
                    break;

                case NPC_ANIM_CUSTOM:

                    if (npcs[KAREN].animationState == 0) {

                        npcs[KAREN].speed = 0;
                        npcs[KAREN].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[KAREN].entityIndex, 0);

                        temp = getRandomNumberInRange(0, 60);

                        if (temp < 4) {
                            npcs[KAREN].direction = temp * 2;
                            npcs[KAREN].animationState = 1;
                        }

                        npcs[KAREN].flags |= NPC_NEEDS_UPDATE;

                    } else {

                        npcs[KAREN].speed = 1;
                        npcs[KAREN].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[KAREN].entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) <=7) {
                            npcs[KAREN].animationState = 1;
                        } else {
                            npcs[KAREN].animationState = 0;
                        }

                        npcs[KAREN].flags |= NPC_NEEDS_UPDATE;

                    }

                    break;

                case NPC_ANIM_FIXED:

                    setEntityAnimation(npcs[KAREN].entityIndex, 0x18);

                    entities[npcs[KAREN].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[KAREN].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[KAREN].entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (npcs[KAREN].animationTimer) {
            npcs[KAREN].animationTimer--;
        }
    
        updateNPCMovement(KAREN);
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleBabyAnimation);

void handleBabyAnimation(void) {

    if (npcs[BABY].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[BABY].entityIndex)) {
            
            if (npcs[BABY].animationTimer == 0) {
    
                switch (npcs[BABY].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCWithAnimation(BABY, 0);
                        break;

                    case NPC_ANIM_WANDER:

                        npcs[BABY].speed = 0;
                        npcs[BABY].animationState = 10;
                        npcs[BABY].animationTimer = 0;

                        if (npcAffection[BABY] < 50) {

                            if (getRandomNumberInRange(0, 100)) {
                                setEntityAnimation(npcs[BABY].entityIndex, 3);
                            } else {
                                setEntityAnimation(npcs[BABY].entityIndex, 11);
                            }

                        } else if (npcAffection[BABY] >= 200 && !getRandomNumberInRange(0, 100)) {
                            setEntityAnimation(npcs[BABY].entityIndex, 9);
                        } else {
                            setEntityAnimation(npcs[BABY].entityIndex, 3);
                        }

                        npcs[BABY].flags |= NPC_NEEDS_UPDATE;
                        break;

                    case NPC_ANIM_SPECIAL:
                        npcs[BABY].speed = 0;
                        npcs[BABY].animationState = 10;
                        npcs[BABY].animationTimer = 0;
                        setEntityAnimation(npcs[BABY].entityIndex, 0xD);
                        npcs[BABY].flags |= NPC_NEEDS_UPDATE;
                        break;

                    case NPC_ANIM_CUSTOM:
                        switch (getRandomNumberInRange(0, 1)) {
                            case 0:
                                updateNPCRandomAnimationWithAnims(5, 0x39, 0x41);
                                break;
                            case 1:
                                updateNPCRandomAnimationWithAnims(5, 0x39, 0x49);
                                break;
                            }
                        break;

                    case NPC_ANIM_FIXED:
                        updateNPCComplexAnimation(5, 0xE, 0x16, 0x51, 0x59, 0x79, 0x69, 0x71, 0x1E, 0x2E, 0x61, 0x26);
                        break;

                    case NPC_ANIM_BABY_CRIB:
                        npcs[BABY].speed = 0;
                        npcs[BABY].animationState = 10;
                        npcs[BABY].animationTimer = 0;
                        setEntityAnimation(npcs[BABY].entityIndex, 0x36);
                        npcs[BABY].flags |= NPC_NEEDS_UPDATE;
                        break;

                }

            }
            
        }

        if (npcs[BABY].animationTimer) {
            npcs[BABY].animationTimer--;
        }

        updateNPCMovement(BABY);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleHarrisAnimation);

void handleHarrisAnimation(void) {

    if (npcs[HARRIS].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARRIS].entityIndex)) {

            if (npcs[HARRIS].animationTimer == 0) {

                switch (npcs[HARRIS].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(HARRIS);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(HARRIS);
                        break;
                }
            } 
            
        }

        if (npcs[HARRIS].animationTimer) {
            npcs[HARRIS].animationTimer--;
        }
    
        updateNPCMovement(HARRIS);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleGrayAnimation);

void handleGrayAnimation(void) {

    if (npcs[GRAY].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[GRAY].entityIndex)) {

            if (npcs[GRAY].animationTimer == 0) {

                switch (npcs[GRAY].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(GRAY);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(GRAY);
                        break;

                    case NPC_ANIM_SPECIAL:

                        npcs[GRAY].speed = 0;
                        npcs[GRAY].animationState = 10;
                        npcs[GRAY].animationTimer = 0;

                        setEntityAnimation(npcs[GRAY].entityIndex, 0x2A);

                        npcs[GRAY].flags |= NPC_NEEDS_UPDATE;
                        break;

                }
            } 
            
        }

        if (npcs[GRAY].animationTimer) {
            npcs[GRAY].animationTimer--;
        }
    
        updateNPCMovement(GRAY);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleJeffAnimation);

void handleJeffAnimation(void) {

    if (npcs[JEFF].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[JEFF].entityIndex)) {

            if (npcs[JEFF].animationTimer == 0) {

                switch (npcs[JEFF].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(JEFF);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(JEFF);
                        break;

                    case NPC_ANIM_SPECIAL:

                        npcs[JEFF].speed = 0;
                        npcs[JEFF].animationState = 10;
                        npcs[JEFF].animationTimer = 0;

                        setEntityAnimation(npcs[JEFF].entityIndex, 0x33);

                        npcs[JEFF].flags |= NPC_NEEDS_UPDATE;
                        break;

                }
            } 
            
        }

        if (npcs[JEFF].animationTimer) {
            npcs[JEFF].animationTimer--;
        }
    
        updateNPCMovement(JEFF);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleCliffAnimation);

void handleCliffAnimation(void) {
    
    if (npcs[CLIFF].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[CLIFF].entityIndex)) {

            if (npcs[CLIFF].animationTimer == 0) {

                switch (npcs[CLIFF].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(CLIFF);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(CLIFF);
                        break;
                }
            } 
            
        }

        if (npcs[CLIFF].animationTimer) {
            npcs[CLIFF].animationTimer--;
        }
    
        updateNPCMovement(CLIFF);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleKaiAnimation);

void handleKaiAnimation(void) {
    
    if (npcs[KAI].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[KAI].entityIndex)) {

            if (npcs[KAI].animationTimer == 0) {

                switch (npcs[KAI].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(KAI);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(KAI);
                        break;

                }

            } 
            
        }

        if (npcs[KAI].animationTimer) {
            npcs[KAI].animationTimer--;
        }
    
        updateNPCMovement(KAI);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMayorAnimation);

void handleMayorAnimation(void) {
    
    if (npcs[MAYOR].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[MAYOR].entityIndex)) {

            if (npcs[MAYOR].animationTimer == 0) {

                switch (npcs[MAYOR].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(MAYOR);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(MAYOR);
                        break;

                }

            } 
            
        }

        if (npcs[MAYOR].animationTimer) {
            npcs[MAYOR].animationTimer--;
        }
    
        updateNPCMovement(MAYOR);
        
    }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMayorWifeAnimation);

void handleMayorWifeAnimation(void) {
    
    if (npcs[MAYOR_WIFE].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[MAYOR_WIFE].entityIndex)) {

            if (npcs[MAYOR_WIFE].animationTimer == 0) {

                switch (npcs[MAYOR_WIFE].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(MAYOR_WIFE);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(MAYOR_WIFE);
                        break;

                }

            } 
            
        }

        if (npcs[MAYOR_WIFE].animationTimer) {
            npcs[MAYOR_WIFE].animationTimer--;
        }
    
        updateNPCMovement(MAYOR_WIFE);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleLilliaAnimation);

void handleLilliaAnimation(void) {
    
    if (npcs[LILLIA].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[LILLIA].entityIndex)) {

            if (npcs[LILLIA].animationTimer == 0) {

                switch (npcs[LILLIA].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(LILLIA);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(LILLIA);
                        break;
                }

            } 
            
        }

        if (npcs[LILLIA].animationTimer) {
            npcs[LILLIA].animationTimer--;
        }
    
        updateNPCMovement(LILLIA);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleBasilAnimation);

void handleBasilAnimation(void) {

    if (npcs[BASIL].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[BASIL].entityIndex)) {

            if (npcs[BASIL].animationTimer == 0) {

                switch (npcs[BASIL].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(BASIL);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(BASIL);
                        break;
                }
            } 
            
        }

        if (npcs[BASIL].animationTimer) {
            npcs[BASIL].animationTimer--;
        }
    
        updateNPCMovement(BASIL);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleEllenAnimation);

void handleEllenAnimation(void) {

    if (npcs[ELLEN].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[ELLEN].entityIndex)) {

            if (npcs[ELLEN].animationTimer == 0 && !npcs[ELLEN].animationMode) {
                npcs[ELLEN].speed = 0;
                npcs[ELLEN].animationState = 10;
                npcs[ELLEN].animationTimer = 0;
                setEntityAnimation(npcs[ELLEN].entityIndex, 8);
                npcs[ELLEN].flags |= NPC_NEEDS_UPDATE;
            } 
            
        }

        if (npcs[ELLEN].animationTimer) {
            npcs[ELLEN].animationTimer--;
        }
    
        updateNPCMovement(ELLEN);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleDougAnimation);

void handleDougAnimation(void) {

    if (npcs[DOUG].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[DOUG].entityIndex) && !npcs[DOUG].animationTimer) {
            updateAnimation(DOUG);
        }

        if (npcs[DOUG].animationTimer) {
            npcs[DOUG].animationTimer--; 
        } 
        
        updateNPCMovement(DOUG);
    }   
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleGotzAnimation);

void handleGotzAnimation(void) {

   u16 tempDirection;

    if (npcs[GOTZ].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[GOTZ].entityIndex) && !npcs[GOTZ].animationTimer) {

            switch (npcs[GOTZ].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(GOTZ);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(GOTZ);
                    break;

                case NPC_ANIM_CUSTOM:

                    // FIXME: should be inline function
                    if (npcs[GOTZ].animationState == 0 ) {

                        npcs[GOTZ].speed = 0;
                        npcs[GOTZ].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[GOTZ].entityIndex, 0);

                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < NORTHEAST) {
                            npcs[GOTZ].direction = tempDirection * 2;
                            npcs[GOTZ].animationState = 1;
                        }

                        npcs[GOTZ].flags |= NPC_NEEDS_UPDATE;

                    } else {

                        npcs[GOTZ].speed = 1;
                        npcs[GOTZ].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[GOTZ].entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcs[GOTZ].animationState = 1;
                        } else {
                            npcs[GOTZ].animationState = 0;
                        }

                        npcs[GOTZ].flags |= NPC_NEEDS_UPDATE;
                    }

                    break;

                default:
                    break;
            }
        }
    
    if (npcs[GOTZ].animationTimer) {
        npcs[GOTZ].animationTimer--;
    }
    
    updateNPCMovement(GOTZ);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleSashaAnimation);

void handleSashaAnimation(void) {

   u16 tempDirection;

    if (npcs[SASHA].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[SASHA].entityIndex) && !npcs[SASHA].animationTimer) {

            switch (npcs[SASHA].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(SASHA);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(SASHA);
                    break;

                case NPC_ANIM_CUSTOM:

                    // FIXME: should be inline function
                    if (npcs[SASHA].animationState == 0 ) {

                        npcs[SASHA].speed = 0;
                        npcs[SASHA].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[SASHA].entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < NORTHEAST) {
                            npcs[SASHA].direction = tempDirection * 2;
                            npcs[SASHA].animationState = 1;
                        }

                        npcs[SASHA].flags |= NPC_NEEDS_UPDATE;

                    } else {

                        npcs[SASHA].speed = 1;
                        npcs[SASHA].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[SASHA].entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcs[SASHA].animationState = 1;
                        } else {
                            npcs[SASHA].animationState = 0;
                        }

                        npcs[SASHA].flags |= NPC_NEEDS_UPDATE;

                    }

                    break;

                default:
                    break;
            }
        }
    
    if (npcs[SASHA].animationTimer) {
        npcs[SASHA].animationTimer--;
    }
    
    updateNPCMovement(SASHA);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handlePotionShopDealerAnimation);

void handlePotionShopDealerAnimation(void) {

    if (npcs[POTION_SHOP_DEALER].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[POTION_SHOP_DEALER].entityIndex) && !npcs[POTION_SHOP_DEALER].animationTimer) {
            updateAnimation(POTION_SHOP_DEALER);
        }

        if (npcs[POTION_SHOP_DEALER].animationTimer) {
            npcs[POTION_SHOP_DEALER].animationTimer--; 
        } 
        
        updateNPCMovement(POTION_SHOP_DEALER);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleKentAnimation);

void handleKentAnimation(void) {

    u16 tempDirection;

    if (npcs[KENT].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[KENT].entityIndex) && !npcs[KENT].animationTimer) {

            switch (npcs[KENT].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(KENT);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(KENT);
                    break;

                case NPC_ANIM_SPECIAL:

                    // FIXME: should be inline function
                    if (npcs[KENT].animationState == 0) {

                        npcs[KENT].speed = 0;
                        npcs[KENT].animationTimer = 0;
                        setEntityAnimationWithDirectionChange(npcs[KENT].entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < NORTHEAST) {
                            npcs[KENT].direction = tempDirection * 2 ;
                            npcs[KENT].animationState = 1;
                        }

                        npcs[KENT].flags |= NPC_NEEDS_UPDATE;

                    } else {

                        npcs[KENT].speed = 2;
                        npcs[KENT].animationTimer = 0;

                        setEntityAnimationWithDirectionChange(npcs[KENT].entityIndex, 0x10);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcs[KENT].animationState = 1;
                        } else {
                            npcs[KENT].animationState = 0;
                        }

                        npcs[KENT].flags |= NPC_NEEDS_UPDATE;
                    }

                    break;

                default:
                    break;
            }
       }    

    if (npcs[KENT].animationTimer) {
        npcs[KENT].animationTimer--;
    }
    
    updateNPCMovement(KENT);
    
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleStuAnimation);

void handleStuAnimation(void) {

    if (npcs[STU].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[STU].entityIndex) && !npcs[STU].animationTimer) {
            updateAnimation(STU);
        }

        if (npcs[STU].animationTimer) {
            npcs[STU].animationTimer--; 
        } 
        
        updateNPCMovement(STU);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMidwifeAnimation);

void handleMidwifeAnimation(void) {

    if (npcs[MIDWIFE].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[MIDWIFE].entityIndex) && !npcs[MIDWIFE].animationTimer) {
            updateAnimation(MIDWIFE);
        }

        if (npcs[MIDWIFE].animationTimer) {
            npcs[MIDWIFE].animationTimer--; 
        } 
        
        updateNPCMovement(MIDWIFE);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMayAnimation);

void handleMayAnimation(void) {

    if (npcs[MAY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[MAY].entityIndex) && !npcs[MAY].animationTimer) {
            updateAnimation(MAY);
        }

        if (npcs[MAY].animationTimer) {
            npcs[MAY].animationTimer--; 
        } 
        
        updateNPCMovement(MAY);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleRickAnimation);

void handleRickAnimation(void) {

    if (npcs[RICK].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[RICK].entityIndex) && !npcs[RICK].animationTimer) {
            updateAnimation(RICK);
        }

        if (npcs[RICK].animationTimer) {
            npcs[RICK].animationTimer--; 
        } 
        
        updateNPCMovement(RICK);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handlePastorAnimation);

void handlePastorAnimation(void) {

    if (npcs[PASTOR].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[PASTOR].entityIndex) && !npcs[PASTOR].animationTimer) {
            updateAnimation(PASTOR);
        }

        if (npcs[PASTOR].animationTimer) {
            npcs[PASTOR].animationTimer--; 
        } 
        
        updateNPCMovement(PASTOR);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleShipperAnimation);

void handleShipperAnimation(void) {

    if (npcs[SHIPPER].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[SHIPPER].entityIndex) && !npcs[SHIPPER].animationTimer) {
            updateAnimation(SHIPPER);
        }

        if (npcs[SHIPPER].animationTimer) {
            npcs[SHIPPER].animationTimer--; 
        } 
        
        updateNPCMovement(SHIPPER);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleSaibaraAnimation);

void handleSaibaraAnimation(void) {

    if (npcs[SAIBARA].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[SAIBARA].entityIndex) && !npcs[SAIBARA].animationTimer) {
            updateAnimation(SAIBARA);
        }

        if (npcs[SAIBARA].animationTimer) {
            npcs[SAIBARA].animationTimer--; 
        } 
        
        updateNPCMovement(SAIBARA);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleDukeAnimation);
void handleDukeAnimation(void) {

    if (npcs[DUKE].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[DUKE].entityIndex) && !npcs[DUKE].animationTimer) {
            updateAnimation(DUKE);
        }

        if (npcs[DUKE].animationTimer) {
            npcs[DUKE].animationTimer--; 
        } 
        
        updateNPCMovement(DUKE);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleGregAnimation);

void handleGregAnimation(void) {

    if (npcs[GREG].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[GREG].entityIndex) && !npcs[GREG].animationTimer) {
            updateAnimation(GREG);
        }

        if (npcs[GREG].animationTimer) {
            npcs[GREG].animationTimer--; 
        } 
        
        updateNPCMovement(GREG);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleCarpenter1Animation);

void handleCarpenter1Animation(void) {

    u16 temp;

    if (npcs[CARPENTER_1].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[CARPENTER_1].entityIndex) && npcs[CARPENTER_1].animationTimer == 0) {

            switch (npcs[CARPENTER_1].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(CARPENTER_1);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(CARPENTER_1);
                    break;

                case NPC_ANIM_SPECIAL:

                    switch (npcs[CARPENTER_1].animationState) {

                        case 0:

                            npcs[CARPENTER_1].speed = 0;
                            npcs[CARPENTER_1].animationTimer = 0;

                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_1].entityIndex, 0);

                            temp = getRandomNumberInRange(0, 60);

                            if (temp < 8) {
                                npcs[CARPENTER_1].direction = temp;
                                npcs[CARPENTER_1].animationState = 1;
                            }

                            if (7 < temp && temp < 21) {
                                npcs[CARPENTER_1].animationState = 2;
                            }

                            npcs[CARPENTER_1].flags |= NPC_NEEDS_UPDATE;

                            break;

                        case 1:

                            npcs[CARPENTER_1].speed = 1;
                            npcs[CARPENTER_1].animationTimer = 0;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_1].entityIndex, 8);

                            temp = getRandomNumberInRange(0, 19);

                            if (temp < 8) {
                                npcs[CARPENTER_1].animationState = 1;
                            } else {
                                npcs[CARPENTER_1].animationState = 0;
                            }

                            npcs[CARPENTER_1].flags |= NPC_NEEDS_UPDATE;

                            break;

                        case 2:

                            npcs[CARPENTER_1].speed = 0;
                            npcs[CARPENTER_1].animationTimer = 60;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_1].entityIndex, 20);

                            npcs[CARPENTER_1].animationState = 1;
                            npcs[CARPENTER_1].flags |= NPC_NEEDS_UPDATE;

                            break;

                    }

            }

        }

        if (npcs[CARPENTER_1].animationTimer) {
            npcs[CARPENTER_1].animationTimer--; 
        } 
        
        updateNPCMovement(CARPENTER_1);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleCarpenter2Animation);

void handleCarpenter2Animation(void) {

    u16 temp;

    if (npcs[CARPENTER_2].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[CARPENTER_2].entityIndex) && npcs[CARPENTER_2].animationTimer == 0) {

            switch (npcs[CARPENTER_2].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(CARPENTER_2);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(CARPENTER_2);
                    break;

                case NPC_ANIM_SPECIAL:

                    switch (npcs[CARPENTER_2].animationState) {

                        case 0:

                            npcs[CARPENTER_2].speed = 0;
                            npcs[CARPENTER_2].animationTimer = 0;

                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_2].entityIndex, 0);

                            temp = getRandomNumberInRange(0, 60);

                            if (temp < 8) {
                                npcs[CARPENTER_2].direction = temp;
                                npcs[CARPENTER_2].animationState = 1;
                            }

                            if (7 < temp && temp < 21) {
                                npcs[CARPENTER_2].animationState = 2;
                            }

                            npcs[CARPENTER_2].flags |= NPC_NEEDS_UPDATE;

                            break;

                        case 1:

                            npcs[CARPENTER_2].speed = 1;
                            npcs[CARPENTER_2].animationTimer = 0;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_2].entityIndex, 8);

                            temp = getRandomNumberInRange(0, 19);

                            if (temp < 8) {
                                npcs[CARPENTER_2].animationState = 1;
                            } else {
                                npcs[CARPENTER_2].animationState = 0;
                            }

                            npcs[CARPENTER_2].flags |= NPC_NEEDS_UPDATE;

                            break;

                        case 2:

                            npcs[CARPENTER_2].speed = 0;
                            npcs[CARPENTER_2].animationTimer = 60;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_2].entityIndex, 28);

                            npcs[CARPENTER_2].animationState = 1;
                            npcs[CARPENTER_2].flags |= NPC_NEEDS_UPDATE;

                            break;

                    }

            }

        }

        if (npcs[CARPENTER_2].animationTimer) {
            npcs[CARPENTER_2].animationTimer--; 
        } 
        
        updateNPCMovement(CARPENTER_2);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMasterCarpenterAnimation);

void handleMasterCarpenterAnimation(void) {
    
    if (npcs[MASTER_CARPENTER].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[MASTER_CARPENTER].entityIndex)) {

            if (npcs[MASTER_CARPENTER].animationTimer == 0) {

                switch (npcs[MASTER_CARPENTER].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(MASTER_CARPENTER);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(MASTER_CARPENTER);
                        break;

                    case NPC_ANIM_SPECIAL:
                        npcs[MASTER_CARPENTER].speed = 0;
                        npcs[MASTER_CARPENTER].animationState = 10;
                        npcs[MASTER_CARPENTER].animationTimer = 0;

                        setEntityAnimation(npcs[MASTER_CARPENTER].entityIndex, 0x10);

                        npcs[MASTER_CARPENTER].flags |= NPC_NEEDS_UPDATE;
                        break;

                }
                
            }
            
        }
 
        if (npcs[MASTER_CARPENTER].animationTimer) {
            npcs[MASTER_CARPENTER].animationTimer--;
        }
        
        updateNPCMovement(MASTER_CARPENTER);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleHarvestSprite1Animation);

void handleHarvestSprite1Animation(void) {

    if (npcs[HARVEST_SPRITE_1].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_1].entityIndex)) {

            if (npcs[HARVEST_SPRITE_1].animationTimer == 0) {

                switch (npcs[HARVEST_SPRITE_1].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(HARVEST_SPRITE_1);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(HARVEST_SPRITE_1);
                        break;

                }
                
            }
            
        }
 
        if (npcs[HARVEST_SPRITE_1].animationTimer) {
            npcs[HARVEST_SPRITE_1].animationTimer--;
        }
        
        updateNPCMovement(HARVEST_SPRITE_1);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleHarvestSprite2Animation);

void handleHarvestSprite2Animation(void) {

    if (npcs[HARVEST_SPRITE_2].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_2].entityIndex)) {

            if (npcs[HARVEST_SPRITE_2].animationTimer == 0) {

                switch (npcs[HARVEST_SPRITE_2].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(HARVEST_SPRITE_2);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(HARVEST_SPRITE_2);
                        break;

                }
                
            }
            
        }
 
        if (npcs[HARVEST_SPRITE_2].animationTimer) {
            npcs[HARVEST_SPRITE_2].animationTimer--;
        }
        
        updateNPCMovement(HARVEST_SPRITE_2);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleHarvestSprite3Animation);

void handleHarvestSprite3Animation(void) {

    if (npcs[HARVEST_SPRITE_3].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_3].entityIndex)) {

            if (npcs[HARVEST_SPRITE_3].animationTimer == 0) {

                switch (npcs[HARVEST_SPRITE_3].animationMode) {

                    case NPC_ANIM_IDLE:
                        stopNPCMovement(HARVEST_SPRITE_3);
                        break;

                    case NPC_ANIM_WANDER:
                        updateNPCRandomAnimation(HARVEST_SPRITE_3);
                        break;

                }
                
            }
            
        }
 
        if (npcs[HARVEST_SPRITE_3].animationTimer) {
            npcs[HARVEST_SPRITE_3].animationTimer--;
        }
        
        updateNPCMovement(HARVEST_SPRITE_3);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleSydneyAnimation);

void handleSydneyAnimation(void) {

    if (npcs[SYDNEY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[SYDNEY].entityIndex) && !npcs[SYDNEY].animationTimer) {
            updateAnimation(SYDNEY);
        }

        if (npcs[SYDNEY].animationTimer) {
            npcs[SYDNEY].animationTimer--; 
        } 
        
        updateNPCMovement(SYDNEY);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleBarleyAnimation);

void handleBarleyAnimation(void) {

    if (npcs[BARLEY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[BARLEY].entityIndex) && !npcs[BARLEY].animationTimer) {
            updateAnimation(BARLEY);
        }

        if (npcs[BARLEY].animationTimer) {
            npcs[BARLEY].animationTimer--; 
        } 
        
        updateNPCMovement(BARLEY);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMrsManaAnimation);

void handleMrsManaAnimation(void) {

    if (npcs[MRS_MANA].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[MRS_MANA].entityIndex) && !npcs[MRS_MANA].animationTimer) {
            updateAnimation(MRS_MANA);
        }

        if (npcs[MRS_MANA].animationTimer) {
            npcs[MRS_MANA].animationTimer--; 
        } 
        
        updateNPCMovement(MRS_MANA);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleJohnAnimation);

void handleJohnAnimation(void) {

    if (npcs[JOHN].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[JOHN].entityIndex) && !npcs[JOHN].animationTimer) {
            updateAnimation(JOHN);
        }

        if (npcs[JOHN].animationTimer) {
            npcs[JOHN].animationTimer--; 
        } 
        
        updateNPCMovement(JOHN);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleGourmetJudgeAnimation);

void handleGourmetJudgeAnimation(void) {

    if (npcs[GOURMET_JUDGE].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[GOURMET_JUDGE].entityIndex) && !npcs[GOURMET_JUDGE].animationTimer) {
            updateAnimation(GOURMET_JUDGE);
        }

        if (npcs[GOURMET_JUDGE].animationTimer) {
            npcs[GOURMET_JUDGE].animationTimer--; 
        } 
        
        updateNPCMovement(0x28);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleMariaHarrisBabyAnimation);

void handleMariaHarrisBabyAnimation(void) {

    if (npcs[MARIA_HARRIS_BABY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[MARIA_HARRIS_BABY].entityIndex) && npcs[MARIA_HARRIS_BABY].animationTimer == 0) {

            switch (npcs[MARIA_HARRIS_BABY].animationMode) {

                case NPC_ANIM_IDLE:
                    npcs[MARIA_HARRIS_BABY].speed = 0;
                    npcs[MARIA_HARRIS_BABY].animationState = 10;
                    npcs[MARIA_HARRIS_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[MARIA_HARRIS_BABY].entityIndex, 8);

                    npcs[MARIA_HARRIS_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIM_WANDER:
                    npcs[MARIA_HARRIS_BABY].speed = 0;
                    npcs[MARIA_HARRIS_BABY].animationState = 10;
                    npcs[MARIA_HARRIS_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[MARIA_HARRIS_BABY].entityIndex, 0);

                    npcs[MARIA_HARRIS_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (npcs[MARIA_HARRIS_BABY].animationTimer) {
            npcs[MARIA_HARRIS_BABY].animationTimer--; 
        } 
        
        updateNPCMovement(MARIA_HARRIS_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handlePopuriGrayBabyAnimation);

void handlePopuriGrayBabyAnimation(void) {

    if (npcs[POPURI_GRAY_BABY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[POPURI_GRAY_BABY].entityIndex) && npcs[POPURI_GRAY_BABY].animationTimer == 0) {

            switch (npcs[POPURI_GRAY_BABY].animationMode) {

                case NPC_ANIM_IDLE:
                    npcs[POPURI_GRAY_BABY].speed = 0;
                    npcs[POPURI_GRAY_BABY].animationState = 10;
                    npcs[POPURI_GRAY_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[POPURI_GRAY_BABY].entityIndex, 8);

                    npcs[POPURI_GRAY_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIM_WANDER:
                    npcs[POPURI_GRAY_BABY].speed = 0;
                    npcs[POPURI_GRAY_BABY].animationState = 10;
                    npcs[POPURI_GRAY_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[POPURI_GRAY_BABY].entityIndex, 0);

                    npcs[POPURI_GRAY_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (npcs[POPURI_GRAY_BABY].animationTimer) {
            npcs[POPURI_GRAY_BABY].animationTimer--; 
        } 
        
        updateNPCMovement(POPURI_GRAY_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleElliJeffBabyAnimation);

void handleElliJeffBabyAnimation(void) {

    if (npcs[ELLI_JEFF_BABY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[ELLI_JEFF_BABY].entityIndex) && npcs[ELLI_JEFF_BABY].animationTimer == 0) {

            switch (npcs[ELLI_JEFF_BABY].animationMode) {

                case NPC_ANIM_IDLE:
                    npcs[ELLI_JEFF_BABY].speed = 0;
                    npcs[ELLI_JEFF_BABY].animationState = 10;
                    npcs[ELLI_JEFF_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[ELLI_JEFF_BABY].entityIndex, 8);

                    npcs[ELLI_JEFF_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIM_WANDER:
                    npcs[ELLI_JEFF_BABY].speed = 0;
                    npcs[ELLI_JEFF_BABY].animationState = 10;
                    npcs[ELLI_JEFF_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[ELLI_JEFF_BABY].entityIndex, 0);

                    npcs[ELLI_JEFF_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (npcs[ELLI_JEFF_BABY].animationTimer) {
            npcs[ELLI_JEFF_BABY].animationTimer--; 
        } 
        
        updateNPCMovement(ELLI_JEFF_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleAnnCliffBabyAnimation);

void handleAnnCliffBabyAnimation(void) {

    if (npcs[ANN_CLIFF_BABY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[ANN_CLIFF_BABY].entityIndex) && npcs[ANN_CLIFF_BABY].animationTimer == 0) {

            switch (npcs[ANN_CLIFF_BABY].animationMode) {

                case NPC_ANIM_IDLE:
                    npcs[ANN_CLIFF_BABY].speed = 0;
                    npcs[ANN_CLIFF_BABY].animationState = 10;
                    npcs[ANN_CLIFF_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[ANN_CLIFF_BABY].entityIndex, 8);

                    npcs[ANN_CLIFF_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIM_WANDER:
                    npcs[ANN_CLIFF_BABY].speed = 0;
                    npcs[ANN_CLIFF_BABY].animationState = 10;
                    npcs[ANN_CLIFF_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[ANN_CLIFF_BABY].entityIndex, 0);

                    npcs[ANN_CLIFF_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }   

        if (npcs[ANN_CLIFF_BABY].animationTimer) {
            npcs[ANN_CLIFF_BABY].animationTimer--; 
        } 
        
        updateNPCMovement(ANN_CLIFF_BABY);
    
    }    
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleKarenKaiBabyAnimation);

void handleKarenKaiBabyAnimation(void) {

    if (npcs[KAREN_KAI_BABY].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[KAREN_KAI_BABY].entityIndex) && npcs[KAREN_KAI_BABY].animationTimer == 0) {

            switch (npcs[KAREN_KAI_BABY].animationMode) {

                case NPC_ANIM_IDLE:
                    npcs[KAREN_KAI_BABY].speed = 0;
                    npcs[KAREN_KAI_BABY].animationState = 10;
                    npcs[KAREN_KAI_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[KAREN_KAI_BABY].entityIndex, 8);

                    npcs[KAREN_KAI_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIM_WANDER:
                    npcs[KAREN_KAI_BABY].speed = 0;
                    npcs[KAREN_KAI_BABY].animationState = 10;
                    npcs[KAREN_KAI_BABY].animationTimer = 0;

                    setEntityAnimationWithDirectionChange(npcs[KAREN_KAI_BABY].entityIndex, 0);

                    npcs[KAREN_KAI_BABY].flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (npcs[KAREN_KAI_BABY].animationTimer) {
            npcs[KAREN_KAI_BABY].animationTimer--; 
        } 
        
        updateNPCMovement(KAREN_KAI_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", handleEntomologistAnimation);

void handleEntomologistAnimation(void) {

    if (npcs[ENTOMOLOGIST].flags & NPC_ENTITY_LOADED) {
    
        if (checkEntityAnimationStateChanged(npcs[ENTOMOLOGIST].entityIndex) && npcs[ENTOMOLOGIST].animationTimer == 0) {

            switch (npcs[ENTOMOLOGIST].animationMode) {

                case NPC_ANIM_IDLE:
                    stopNPCMovement(ENTOMOLOGIST);
                    break;

                case NPC_ANIM_WANDER:
                    updateNPCRandomAnimation(ENTOMOLOGIST);
                    break;

            }
        }

        if (npcs[ENTOMOLOGIST].animationTimer) {
            npcs[ENTOMOLOGIST].animationTimer--; 
        } 
        
        updateNPCMovement(ENTOMOLOGIST);
    
    }   
    
}



//INCLUDE_ASM("asm/nonmatchings/game/npc", checkNPCInteraction);

static inline bool checkHoldingItem() {
    return gPlayer.heldItem != 0;
}

// FIXME: totally cursed
// initiates dialogue with NPC on button press or pick up baby
u8 checkNPCInteraction(void) {
    
    u8 result = 0;
    u8 i = 0;
    
    s32 i2;
    u16 temp;

    // ??
    u8 *ptr2 = &gPlayer.heldItem;
    u8 *ptr = npcAffection + 5;

    do {

        // ???
        ptr = npcAffection+5;
        
        if (npcs[i].flags & NPC_ENTITY_LOADED) {
            
            if (entities[npcs[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[npcs[i].entityIndex].buttonPressed == BUTTON_A) {

                // FIXME: might be static inline baby handler function
                if (i == BABY && gBabyAge < 120) {
                                 
                    // should be gPlayer.heldItem == 0
                    if (*ptr2 == 0) {
         
                        // should be !(gPlayer.flags & PLAYER_RIDING_HORSE)
                        if (!(*(u16*)(ptr2+0x4A) & 1)) {

                            if (gBabyAge >= 30) {
                                *ptr2 = 0xC2;
                            } else {
                                *ptr2 = 0xBA;  
                            }

                            if (!checkDailyEventBit(HELD_BABY)) {
                                *ptr += adjustValue(*ptr, 2, MAX_AFFECTION);
                                setDailyEventBit(HELD_BABY);
                            }
                            
                            deactivateEntity(npcs[i].entityIndex);
                         
                            result = 2;
                            npcs[i].flags &= ~NPC_ENTITY_LOADED;
                           
                        } 
                        
                        temp = 0xFFFF;
                        
                    } else {
                        temp = 1;
                    } 
                                 
                } else {
                    temp = checkHoldingItem();
                }
   
                if (temp != 0xFFFF) {

                    // ?? switch statement?
                    i2 = i;

                    if (i2 < 5) {

                        if (!(i2 < 0)) {

                            setOverlayIconSprite(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[i] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
                            setOverlayIconSprite(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[i] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
                       
                        }
                        
                    }
                    
                    showMessageBox(0, npcToDialogueBytecodeIndex[i], temp, 0, 0);
                    result = 1;
                    npcs[i].animationMode = NPC_ANIM_TALKING;
                    
                } 
            }    
        }
        
        i++;
            
    } while (i < MAX_NPCS && result == 0);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", findNPCToTalkTo);

bool findNPCToTalkTo(void) {

    bool found = FALSE;
    u8 i = 0;
    
    npcTalkingTo = 0xFF;

    while (i < MAX_NPCS && !found) {

        if (npcs[i].flags & NPC_ENTITY_LOADED) {

            if (entities[npcs[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {
                npcs[i].animationMode = NPC_ANIM_FACE_PLAYER;
                npcTalkingTo = i;
                found = TRUE;
            }

        }  

        i++;

    } 
    
    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", checkBlueFeatherResponse);

bool getBlueFeatherResponse(int index, u16 arg1) {

    bool result;
    
    int temp;
    // FIXME: likely a struct?
    u8 arr[8];
    
    arr[7] = index;
    index = temp;
    index = arr[7];
    
    result = FALSE;
    
    if (npcs[index].flags & NPC_ENTITY_LOADED) {

        // FIXME: fake match
        // check if girl and load heart icon
        if ((index < 5) && (index >= (result = 0))) {
            setOverlayIconSprite(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
            setOverlayIconSprite(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
        }

        showMessageBox(0, npcToDialogueBytecodeIndex[arr[7]], arg1, 0, 0);
        result = TRUE;
        
    }
    
    return result;

}