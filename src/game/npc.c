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
    ENTITY_ASSET_JOHN, 
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
    DIALOGUE_GOURMET_JUDGE, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_NPC_BABY, 
    DIALOGUE_ENTOMOLOGIST
};

// forward declarations
void handleJohnAnimation(void);
void handleGourmetJudgeAnimation(void);
void updateNPCMovement(u8);
void updateBabyWanderAnimation(u8 index, u8 arg1, u8 arg2);
void updateBabyAnimations(u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8); 
void updateBacheloretteBeachAnimation(u8 npcIndex);
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

void deactivateNPCEntities(void) {
    
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        
        if (npcs[i].flags & NPC_ENTITY_LOADED) {
            deactivateEntity(npcs[i].entityIndex);
        }
        
        npcs[i].flags = 0;
        npcs[i].animationMode = NPC_ANIMATION_IDLE;
        npcs[i].idleAnimation = 0;
        npcs[i].movingAnimation = 0;
        npcs[i].animationState = 0;
        
    }

}

u8 setupNPCEntity(u8 npcIndex, u8 currentEntityOffset) {

    npcInfo *npc = &npcs[npcIndex];

    if ((npc->flags & NPC_ACTIVE) && npc->levelIndex == gBaseMapIndex) {
        
        npc->flags |= NPC_ENTITY_LOADED;

        switch (npcIndex) {
            case MARIA:
                if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                    npc->entityIndex = ENTITY_WIFE; 
                } else {
                    npc->entityIndex = ENTITY_NPC_BASE + currentEntityOffset;
                    currentEntityOffset++;
                }
                break;
            case POPURI:
                if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                    npc->entityIndex = ENTITY_WIFE;
                } else {
                    npc->entityIndex = ENTITY_NPC_BASE + currentEntityOffset;
                    currentEntityOffset++;
                }
                break;
            case ELLI:
                if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                    npc->entityIndex = ENTITY_WIFE;
                } else {
                    npc->entityIndex = ENTITY_NPC_BASE + currentEntityOffset;
                    currentEntityOffset++;
                }
                break;
            case ANN:
                if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                    npc->entityIndex = ENTITY_WIFE;
                } else {
                    npc->entityIndex = ENTITY_NPC_BASE + currentEntityOffset;
                    currentEntityOffset++;
                }
                break;
            case KAREN:
                if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                    npc->entityIndex = ENTITY_WIFE;
                } else {
                    npc->entityIndex = ENTITY_NPC_BASE + currentEntityOffset;
                    currentEntityOffset++;
                }
                break;
            case BABY:                               
                npc->entityIndex = ENTITY_BABY;
                break;
            case JOHN:
                npc->entityIndex = 29;
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
                npc->entityIndex = ENTITY_NPC_BASE + currentEntityOffset;
                currentEntityOffset++;
                break;
        }
    
        npc->currentCoordinates.x = npc->startingCoordinates.x;
        npc->currentCoordinates.y = npc->startingCoordinates.y;
        npc->currentCoordinates.z = npc->startingCoordinates.z;
    
        loadEntity(npc->entityIndex, npcToEntityAssetIndex[npcIndex], TRUE);
        setEntityCollidable(npc->entityIndex, TRUE);
        setEntityYMovement(npc->entityIndex, TRUE);
        setCameraTrackingEntity(npc->entityIndex, FALSE);
    
        // FIXME: macro not matching; should be convertWorldDirectionToScreenDirection(npc->direction, getCurrentMapRotation(MAIN_MAP_INDEX))
        setEntityDirection(npc->entityIndex, (npc->direction + 8 - getCurrentMapRotation(MAIN_MAP_INDEX)) % 8);
        setEntityCoordinates(npc->entityIndex, npc->currentCoordinates.x, npc->currentCoordinates.y, npc->currentCoordinates.z);
    
        if (npc->flags & NPC_ATTACHED) {

            switch (npcIndex) {                   
                case BABY:                             
                    setEntityTrackingTarget(npc->entityIndex, ENTITY_WIFE, 0xFE);
                    break;
                case MARIA_HARRIS_BABY:                            
                    setEntityTrackingTarget(npc->entityIndex, npcs[MARIA].entityIndex, 0xFE);
                    break;
                case POPURI_GRAY_BABY:                            
                    setEntityTrackingTarget(npc->entityIndex, npcs[POPURI].entityIndex, 0xFE);
                    break;
                case ELLI_JEFF_BABY:                            
                    setEntityTrackingTarget(npc->entityIndex, npcs[ELLI].entityIndex, 0xFE);
                    break;
                case ANN_CLIFF_BABY:                            
                    setEntityTrackingTarget(npc->entityIndex, npcs[ANN].entityIndex, 0xFE);
                    break;
                case KAREN_KAI_BABY:                            
                    setEntityTrackingTarget(npc->entityIndex, npcs[KAREN].entityIndex, 0xFE);
                    break;
                }
            
                setEntityAttachmentOffset(npc->entityIndex, 0, 0xE, 0x14);
                setEntityCollidable(npc->entityIndex, FALSE);
                setEntityYMovement(npc->entityIndex, FALSE);
                setEntityTracksCollisions(npc->entityIndex, FALSE);
                enableEntityMovement(npc->entityIndex, FALSE);

        }
    
        npc->animationState = 0;
        npc->animationTimer = 0;
        npc->flags |= NPC_ENTITY_LOADED;
        
    }

    return currentEntityOffset;
    
}

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

void randomizeNPCLocations(void) {
    
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        npcs[i].location = getRandomNumberInRange(0, 3);
    }

}

void updateNPCMovement(u8 npcIndex) {

    npcInfo *npc = &npcs[npcIndex];

    Vec3f vec;
    u8 direction = npc->direction;

    f32 f1, f2, f3;
    f32 f4, f5, f6;
    
    if (npc->animationMode == NPC_ANIMATION_TALKING) {

        direction = (gPlayer.direction + 4) % 8;        

        setEntityDirectionalAnimation(npc->entityIndex, npc->idleAnimation);
        npc->animationMode = npc->defaultAnimationMode;
        
    }

    if (npc->animationMode == NPC_ANIMATION_FACE_PLAYER) {
        direction = (gPlayer.direction + 4) % 8;        
        setEntityDirectionalAnimation(npc->entityIndex, npc->idleAnimation);
    }

    npc->currentCoordinates.x = entities[npc->entityIndex].coordinates.x;
    npc->currentCoordinates.y = entities[npc->entityIndex].coordinates.y;
    npc->currentCoordinates.z = entities[npc->entityIndex].coordinates.z;

    setEntityDirection(npc->entityIndex, convertWorldDirectionToScreenDirection(direction, MAIN_MAP_INDEX));

    vec = getMovementVectorFromDirection(npc->speed, direction, 0.0f);

    f1 = npc->startingCoordinates.x;
    f2 = npc->wanderRadiusX;
    f3 = npc->currentCoordinates.x + vec.x;
    
    if ((s16)(f1 + f2) >= (s16)f3 && (s16)f3 >= (s16)(f1 - f2)) {

        f4 = npc->startingCoordinates.z;
        f5 = npc->wanderRadiusZ;
        f6 = npc->currentCoordinates.z + vec.z;

        if ((s16)(f4 + f5) >= (s16)f6 && (s16)f6 >= (s16)(f4 - f5)) {
            setEntityMovementVector(npc->entityIndex, vec.x, vec.y, vec.z, npc->speed);
        }
        
    }
    
}

inline void stopNPCMovement(u8 npcIndex) {

    npcInfo *npc = &npcs[npcIndex];

    npc->speed = 0;
    npc->animationState = 10;
    npc->animationTimer = 0;

    setEntityDirectionalAnimation(npc->entityIndex, npc->idleAnimation);

    npc->flags |= NPC_NEEDS_UPDATE;
    
}

// FIXME: should be inline?
void updateNPCWanderAnimation(u8 index) {

    npcInfo *npc = &npcs[index];
    
    u16 temp;
    
    if (npc->animationState == 0) {

        npc->speed = 0;
        npc->animationTimer = 0;

        setEntityDirectionalAnimation(npc->entityIndex, npc->idleAnimation);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npc->direction = temp*2;
            npc->animationState = 1;
        }

        npc->flags |= NPC_NEEDS_UPDATE;

    } else {
        
        if (npc->direction & 1) {
    
            npc->speed = 0;
            npc->animationTimer = 0;
            npc->animationState = 0;
    
        } else {
    
            npc->speed = 1;
            npc->animationTimer = 0;
    
            setEntityDirectionalAnimation(npc->entityIndex, npc->movingAnimation);
            
            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 1;
                
            } else {
                npc->animationState = 0;
            }
            
        }
        
        npc->flags |= NPC_NEEDS_UPDATE;

    }

}

inline void handleBabyIdleAnimation(u8 index, u8 animationIndex) {

    npcInfo *npc = &npcs[index];

    npc->speed = 0;
    npc->animationState = 10;
    npc->animationTimer = 0;

    setEntityDirectionalAnimation(npc->entityIndex, animationIndex);

    npc->flags |= NPC_NEEDS_UPDATE;
    
}

void updateBabyWanderAnimation(u8 index, u8 idleAnimation, u8 animationIndex2) {

    npcInfo *npc = &npcs[index];

    u16 temp;
    
    if (npc->animationState == 0) {

        npc->speed = 0;
        npc->animationTimer = 0;

        setEntityDirectionalAnimation(npc->entityIndex, idleAnimation);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npc->direction = temp*2;
            npc->animationState = 1;
        }

        npc->flags |= NPC_NEEDS_UPDATE;
        return;
    }
    
    if (npc->direction & 1) {

        npc->speed = 0;
        npc->animationTimer = 0;
        npc->animationState = 0;

    } else {

        npc->speed = 1;
        npc->animationTimer = 0;
        
        setEntityDirectionalAnimation(npc->entityIndex, animationIndex2);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npc->animationState = 1;
        } else {
            npc->animationState = 0;
        }
        
    }
    
    npc->flags |= NPC_NEEDS_UPDATE;
    
}

void updateBabyAnimations(u8 npcIndex, u8 idleAnimation, u8 animationIndex2, u8 animationIndex3, u8 animationIndex4, u8 animationIndex5, u8 animationIndex6, u8 animationIndex7, u8 animationIndex8, u8 animationIndex9, u8 idleAnimation0, u8 idleAnimation1) {

    npcInfo *npc = &npcs[npcIndex];

    u16 temp;
    
    switch (npc->animationState) {

        // standing
        case 0:

            npc->speed = 0;
            npc->animationTimer = 10;
            
            setEntityDirectionalAnimation(npc->entityIndex, idleAnimation);

            temp = getRandomNumberInRange(0, 60);
            
            if (temp < 4) {
                npc->direction = temp * 2;
                npc->animationState = 1;
            } 
            
            if (temp == 4) {
                npc->animationState = 7;
            } 
            
            if (temp == 5) {
                npc->animationState = 2;
            } 

            if (temp == 6) {
                npc->animationState = 3;
            } 
            
            if (temp == 7) {
                npc->animationState = 4;
            }
        
            npc->flags |= NPC_NEEDS_UPDATE;
            break;

        // walking
        case 1:

            if (npc->direction & 1) {
                
                npc->speed = 0;
                npc->animationTimer = 0;
                npc->animationState = 0;
                
            } else {
                
                npc->speed = 1;
                npc->animationTimer = 0;
                setEntityDirectionalAnimation(npc->entityIndex, animationIndex2);
    
                if (getRandomNumberInRange(0, 19) < 8) {
                    npc->animationState = 1;
                } else {
                    npc->animationState = 0;
                }
                
            }

            npc->flags |= NPC_NEEDS_UPDATE;
            break;

        // happy
        case 2:   
            
            npc->speed = 0;
            npc->animationTimer = 0;

            setEntityDirectionalAnimation(npc->entityIndex, animationIndex3);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 2;
            } else {
                npc->animationState = 0;
            }
            
            break;
        
        // crying
        case 3:

            npc->speed = 0;
            npc->animationTimer = 0;

            setEntityDirectionalAnimation(npc->entityIndex, animationIndex4);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 3;
            } else {
                npc->animationState = 0;
            }
            
            break;

        // crawling, idle
        case 4:

            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, animationIndex5);

            npc->animationState = 0;
            
            break;

        // sitting
        case 5:

            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, animationIndex6);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 5;
            } else {
                npc->animationState = 10;
            }
            
            break;

        // sitting animation 2
        case 6:
            
            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, animationIndex7);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 6;
            } else {
                npc->animationState = 10;
            }
            
            break;

case 7:

            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, animationIndex8);

            npc->animationState = 10;
            
            break;

        // sitting crying
        case 8:

            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, animationIndex9);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 8;
            } else {
                npc->animationState = 10;
            }
            
            break;

        // sitting, sleeping
        case 9:

            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, idleAnimation0);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 9;
            } else {
                npc->animationState = 10;
            }
            
            break;

        case 10:

            npc->speed = 0;
            npc->animationTimer = 10;

            setEntityDirectionalAnimation(npc->entityIndex, idleAnimation1);

            npc->animationState = 10;

            temp = getRandomNumberInRange(0, 60);
            
            if (temp < 10) {
                npc->animationState = 0;
            } 
            
            if (temp == 11) {
                npc->animationState = 5;
            }
            
            if (temp == 12) {
                npc->animationState = 6;
            } 
            
            if (temp == 13) {
                npc->animationState = 8;
            } 
            
            if (temp == 14) {
                npc->animationState = 9;
            }
        
            break;

    }
    
}

void updateBacheloretteBeachAnimation(u8 npcIndex) {

    npcInfo *npc = &npcs[npcIndex];

    u16 temp;
    
    switch (npc->animationState) {

        case 0:

            npc->speed = 0;
            npc->animationTimer = 0;
            
            setEntityDirectionalAnimation(npc->entityIndex, 0);
            
            temp = getRandomNumberInRange(0, 60);

            if (temp < 8) {
                npc->direction = temp;
                npc->animationState = 1;
            }

            if (7 < temp && temp < 21) {
                npc->animationState = 2;
            }

            npc->flags |= NPC_NEEDS_UPDATE;

            break;

        case 1:
            
            npc->speed = 1;
            npc->animationTimer = 0;
            setEntityDirectionalAnimation(npc->entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 8) {
                npc->animationState = 1;
            } else {
                npc->animationState = 0;
            }

            npc->flags |= NPC_NEEDS_UPDATE;

            break;

        case 2:

            npc->speed = 0;
            npc->animationTimer = 60;
            
            // crouching
            setEntityDirectionalAnimation(npc->entityIndex, 25);

            npc->animationState = 1;
            npc->flags |= NPC_NEEDS_UPDATE;
            
            break;
            
    }
    
}

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

void setMariaLocation(void) {
    npcInfo *maria = &npcs[MARIA];


     u8 result;

    maria->wanderRadiusX = 64;
    maria->wanderRadiusZ = 64;
    maria->idleAnimation = 0;
    maria->movingAnimation = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(MARIA_LOCATION_OVERRIDE)) {

        if (checkLifeEventBit(MARRIED) && gWife == MARIA) {

            if (5 < gHour && gHour < 8) {

                maria->levelIndex = HOUSE;
                maria->startingCoordinates.y = 0.0f;
                maria->direction = DIRECTION_SE;
                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                maria->startingCoordinates.x = -128.0f;
                maria->startingCoordinates.z = -64.0f;
                maria->flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (maria->location < 2) {

                        maria->levelIndex = CHURCH;
                        maria->startingCoordinates.y = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        maria->startingCoordinates.x = -112.0f;
                        maria->startingCoordinates.z = -192.0f;
                        maria->flags |= NPC_ACTIVE;
                        
                    }  else {

                        maria->levelIndex = LIBRARY;
                        maria->startingCoordinates.y = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        maria->startingCoordinates.x = 48.0f;
                        maria->startingCoordinates.z = -96.0f;
                        maria->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else if (gWeather == SUNNY) {
                        
                    maria->levelIndex = FARM;
                    maria->startingCoordinates.y = 0.0f;
                    maria->direction = DIRECTION_S;
                    maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    maria->startingCoordinates.x = -256.0f;
                    maria->startingCoordinates.z = -240.0f;
                    maria->flags |= NPC_ACTIVE;
                    
                } else {

                    maria->levelIndex = HOUSE;
                    maria->startingCoordinates.x = 0.0f;
                    maria->startingCoordinates.y = 0.0f;
                    maria->startingCoordinates.z = 0.0f;
                    maria->direction = DIRECTION_S;
                    maria->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    maria->flags |= NPC_ACTIVE;
                    
                }
                
            }

            if (gHour == 12) {
                
                maria->levelIndex = KITCHEN;
                maria->startingCoordinates.y = 0.0f;
                maria->direction = DIRECTION_W;
                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                maria->startingCoordinates.x = -160.0f;
                maria->startingCoordinates.z = -64.0f;
                maria->flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                maria->levelIndex = HOUSE;
                maria->startingCoordinates.x = 0.0f;
                maria->startingCoordinates.y = 0.0f;
                maria->startingCoordinates.z = 0.0f;
                maria->direction = DIRECTION_S;
                maria->defaultAnimationMode = NPC_ANIMATION_WANDER;
                maria->flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                maria->levelIndex = KITCHEN;
                maria->startingCoordinates.y = 0.0f;
                maria->direction = DIRECTION_W;
                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                maria->startingCoordinates.x = -160.0f;
                maria->startingCoordinates.z = -64.0f;
                maria->flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                maria->levelIndex = HOUSE;
                maria->startingCoordinates.y = 0.0f;
                maria->direction = DIRECTION_E;
                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                maria->startingCoordinates.x = -128.0f;
                maria->startingCoordinates.z = -112.0f;
                maria->flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                maria->levelIndex = HOUSE;
                maria->startingCoordinates.y = 0.0f;
                maria->direction = DIRECTION_S;
                maria->defaultAnimationMode = NPC_ANIMATION_SLEEPING;
                maria->startingCoordinates.x = -192.0f;
                maria->startingCoordinates.z = -160.0f;
                maria->flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                maria->idleAnimation = 0x6C;
                maria->movingAnimation = 0x76;
            }
    
        } else if (!checkDailyEventBit(FESTIVAL)) {

            if (checkLifeEventBit(MARIA_HARRIS_MARRIED)) {

                switch (gDayOfWeek) {

                    case TUESDAY:
                    case WEDNESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:
                    case SUNDAY:

                        if (8 < gHour && gHour < 17) {
                            
                            maria->levelIndex = LIBRARY;
                            maria->startingCoordinates.y = 0.0f;
                            maria->direction = DIRECTION_S;
                            maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            maria->startingCoordinates.x = 48.0f;
                            maria->startingCoordinates.z = -96.0f;
                            maria->flags |= NPC_ACTIVE;
        
                        }

                        break;

                    case MONDAY:

                        if (gWeather == SUNNY) {

                            if (maria->location < 2) {
                                
                                if (7 < gHour && gHour < 15) { 
                                
                                    maria->levelIndex = VILLAGE_1;
                                    maria->startingCoordinates.x = 0.0f;
                                    maria->startingCoordinates.y = 0.0f;
                                    maria->direction = DIRECTION_S;
                                    maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    maria->startingCoordinates.z = -144.0f;
                                    maria->flags |= NPC_ACTIVE;
                                    
                                }
                                
                            } else {

                                if (7 < gHour && gHour < 17) { 
                                
                                    maria->levelIndex = VILLAGE_2;
                                    maria->startingCoordinates.x = 0.0f;
                                    maria->startingCoordinates.y = 0.0f;
                                    maria->startingCoordinates.z = 0.0f;
                                    maria->direction = DIRECTION_S;
                                    maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    maria->flags |= NPC_ACTIVE;
                                    
                                }
                                
                            }
                            
                        } else {

                            if (8 < gHour && gHour < 17) {
                                
                                maria->levelIndex = MAYOR_HOUSE;
                                maria->startingCoordinates.y = 0.0f;
                                maria->direction = DIRECTION_S;
                                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                maria->startingCoordinates.x = -128.0f;
                                maria->startingCoordinates.z = -32.0f;
                                maria->flags |= NPC_ACTIVE;
        
                            }
                    
                        }
                        
                        break;
                    
                }

                if (gSeason == SUMMER &&  0 < gDayOfMonth && gDayOfMonth < 11) {
                    
                        maria->levelIndex = MAYOR_HOUSE;
                        maria->startingCoordinates.y = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        maria->startingCoordinates.x = -128.0f;
                        maria->startingCoordinates.z = -32.0f;
                        maria->flags |= NPC_ACTIVE;
                    
                } 

                if (gSeason == WINTER && 0 < gDayOfMonth && gDayOfMonth < 11) {

                    if (maria->location < 2) {
                        
                        maria->levelIndex = MAYOR_HOUSE;
                        maria->startingCoordinates.y = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        maria->startingCoordinates.x = -128.0f;
                        maria->startingCoordinates.z = -32.0f;
                        maria->flags |= NPC_ACTIVE;
                            
                    } else {

                        maria->levelIndex = CHURCH;
                        maria->startingCoordinates.y = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        maria->startingCoordinates.x = -112.0f;
                        maria->startingCoordinates.z = -192.0f;
                        maria->flags |= NPC_ACTIVE;
                        
                    }
                    
                } 

                if (getNPCBabyCarryingState(MARIA) == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                    maria->idleAnimation = 0x6C;
                    maria->movingAnimation = 0x76;
                }
                
            } else {

                if (gDayOfWeek == MONDAY) {
    
                    if (gWeather == SUNNY) {
                        
                        if (maria->location < 2) {
                            
                            if (7 < gHour && gHour < 15) {
                                
                                maria->levelIndex = VILLAGE_1;
                                maria->startingCoordinates.x = 0.0f;
                                maria->startingCoordinates.y = 0.0f;
                                maria->direction = DIRECTION_S;
                                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                maria->startingCoordinates.z = -144.0f;
                                maria->flags |= NPC_ACTIVE;
                                
                            } 
                            
                        } else if (9 < gHour && gHour < 17) {
                                    
                            maria->levelIndex = MOUNTAIN_1;
                            maria->startingCoordinates.y = 0.0f;
                            maria->direction = DIRECTION_S;
                            maria->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            maria->startingCoordinates.x = 128.0f;
                            maria->startingCoordinates.z = 128.0f;
                            maria->flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(MARIA_AT_MOUNTAIN_DIALOGUE);
                                
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            maria->levelIndex = MAYOR_HOUSE;
                            maria->startingCoordinates.y = 0.0f;
                            maria->direction = DIRECTION_S;
                            maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            maria->startingCoordinates.x = -128.0f;
                            maria->startingCoordinates.z = -32.0f;
                            maria->flags |= NPC_ACTIVE;
                            
                        }
                                                
                    }
                                      
                } else {
    
                    if (8 < gHour && gHour < 17) {
                        
                        maria->levelIndex = LIBRARY;
                        maria->startingCoordinates.y = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        maria->startingCoordinates.x = 48.0f;
                        maria->startingCoordinates.z = -96.0f;
                        maria->flags |= NPC_ACTIVE;
                    
                    }
    
                }

                if (gSeason == SUMMER && 0 < gDayOfMonth && gDayOfMonth < 11) {
    
                    if (gWeather == SUNNY) {
                        
                        if (maria->location < 2) {
    
                            if (8 < gHour && gHour < 17) {
    
                                maria->levelIndex = MOUNTAIN_2;
                                maria->startingCoordinates.y = 0.0f;
                                maria->direction = DIRECTION_SE;
                                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                maria->startingCoordinates.x = -208.0f;
                                maria->startingCoordinates.z = -160.0f;
                                maria->flags |= NPC_ACTIVE;
        
                                setSpecialDialogueBit(MARIA_AT_MOUNTAIN_DIALOGUE);
                                
                            }
                            
                        } else {
    
                            if (8 < gHour && gHour < 17) {
                               
                                maria->levelIndex = POND;
                                maria->startingCoordinates.y = 0.0f;
                                maria->direction = DIRECTION_E;
                                maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                maria->startingCoordinates.x = -96.0f;
                                maria->startingCoordinates.z = -32.0f;
                                maria->flags |= NPC_ACTIVE;
    
                                setSpecialDialogueBit(MARIA_AT_MOUNTAIN_DIALOGUE);
                                
                            }
                            
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            maria->levelIndex = MAYOR_HOUSE;
                            maria->startingCoordinates.y = 0.0f;
                            maria->direction = DIRECTION_S;
                            maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            maria->startingCoordinates.x = -128.0f;
                            maria->startingCoordinates.z = -32.0f;
                            maria->flags |= NPC_ACTIVE;
                        
                        }
                        
                    }
                    
                }
    
                if (gSeason == WINTER && 0 < gDayOfMonth && gDayOfMonth < 11) {
    
                    if (maria->location < 2) {
    
                        if (8 < gHour && gHour < 17) { 
                        
                            maria->levelIndex = MAYOR_HOUSE;
                            maria->startingCoordinates.y = 0.0f;
                            maria->direction = DIRECTION_S;
                            maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            maria->startingCoordinates.x = -128.0f;
                            maria->startingCoordinates.z = -32.0f;
                            maria->flags |= NPC_ACTIVE;
                        
                        }
    
                    } else {
    
                        if (8 < gHour && gHour < 17) {
    
                            maria->levelIndex = CHURCH;
                            maria->startingCoordinates.y = 0;
                            maria->direction = DIRECTION_S;
                            maria->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            maria->startingCoordinates.x = -112.0f;
                            maria->startingCoordinates.z = -192.0f;
                            maria->flags |= NPC_ACTIVE;
                            
                        }
                        
                    }
                    
                }
    
                if (gSeason == SUMMER && gDayOfWeek == SUNDAY && gWeather == SUNNY && maria->location == 0) {
    
                    if (17 < gHour && gHour < 21) {
    
                        maria->levelIndex = BEACH;
                        maria->startingCoordinates.y = 0.0f;
                        maria->startingCoordinates.z = 0.0f;
                        maria->direction = DIRECTION_S;
                        maria->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                        maria->startingCoordinates.x = -224.0f;
                        maria->flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(MARIA_AT_BEACH_DIALOGUE);
                        
                    }
                    
                }
                
            }
            
        } 
        
    }

    maria->animationMode = maria->defaultAnimationMode;
    
}

void setPopuriLocation(void) {
    npcInfo *popuri = &npcs[POPURI];


    u8 result;

    popuri->wanderRadiusX = 64;
    popuri->wanderRadiusZ = 64;
    popuri->idleAnimation = 0;
    popuri->movingAnimation = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(POPURI_LOCATION_OVERRIDE)) {

        if (checkLifeEventBit(MARRIED) && gWife == POPURI) {

            if (5 < gHour && gHour < 8) {

                popuri->levelIndex = HOUSE;
                popuri->startingCoordinates.y = 0.0f;
                popuri->direction = DIRECTION_SE;
                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                popuri->startingCoordinates.x = -128.0f;
                popuri->startingCoordinates.z = -64.0f;
                popuri->flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == MONDAY) {

                    if (npcs[ELLI].location < 2) {

                        if (8 < gHour && gHour < 17) {

                            if (gWeather == SUNNY) {
      
                                popuri->levelIndex = MOUNTAIN_2;    
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_S;
                                popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                popuri->startingCoordinates.x = -224.0f;
                                popuri->startingCoordinates.z = -96.0f;
                                popuri->flags |= NPC_ACTIVE;
        
                                setSpecialDialogueBit(POPURI_AT_MOUNTAIN_2_DIALOGUE);
                                
                            } else {
                                
                                popuri->levelIndex = KITCHEN;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_S;
                                popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                popuri->startingCoordinates.x = -160.0f;
                                popuri->startingCoordinates.z = -64.0f;
                                popuri->flags |= NPC_ACTIVE;
                                                                
                            }
                            
                        }
                                              
                    } else {

                        if (8 < gHour && gHour < 17) {

                            popuri->levelIndex = FLOWER_SHOP;    
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->direction = DIRECTION_E;
                            popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            popuri->startingCoordinates.x = -112.0f;
                            popuri->startingCoordinates.z = 64.0f;
                            popuri->flags |= NPC_ACTIVE;
                            
                        }

                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                        
                        popuri->levelIndex = FARM;
                        popuri->startingCoordinates.y = 0.0f;
                        popuri->direction = DIRECTION_S;
                        popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        popuri->startingCoordinates.x = 320.0f;
                        popuri->startingCoordinates.z = -352.0f;
                        popuri->flags |= NPC_ACTIVE;
                        
                    } else {
                            
                        popuri->levelIndex = KITCHEN;
                        popuri->startingCoordinates.y = 0.0f;
                        popuri->direction = DIRECTION_S;
                        popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        popuri->startingCoordinates.x = -160.0f;
                        popuri->startingCoordinates.z = -64.0f;
                        popuri->flags |= NPC_ACTIVE;
                                            
                    }
                    
                }
                
            }

            if (gHour == 12) {
                
                popuri->levelIndex = KITCHEN;
                popuri->startingCoordinates.y = 0.0f;
                popuri->direction = DIRECTION_W;
                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                popuri->startingCoordinates.x = -160.0f;
                popuri->startingCoordinates.z = -64.0f;
                popuri->flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                popuri->levelIndex = HOUSE;
                popuri->startingCoordinates.x = 0.0f;
                popuri->startingCoordinates.y = 0.0f;
                popuri->startingCoordinates.z = 0.0f;
                popuri->direction = DIRECTION_S;
                popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                popuri->flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                popuri->levelIndex = KITCHEN;
                popuri->startingCoordinates.y = 0.0f;
                popuri->direction = DIRECTION_W;
                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                popuri->startingCoordinates.x = -160.0f;
                popuri->startingCoordinates.z = -64.0f;
                popuri->flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                popuri->levelIndex = HOUSE;
                popuri->startingCoordinates.y = 0.0f;
                popuri->direction = DIRECTION_E;
                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                popuri->startingCoordinates.x = -128.0f;
                popuri->startingCoordinates.z = -112.0f;
                popuri->flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                popuri->levelIndex = HOUSE;
                popuri->startingCoordinates.y = 0.0f;
                popuri->direction = DIRECTION_S;
                popuri->defaultAnimationMode = NPC_ANIMATION_SLEEPING;
                popuri->startingCoordinates.x = -192.0f;
                popuri->startingCoordinates.z = -160.0f;
                popuri->flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                popuri->idleAnimation = 122;
                popuri->movingAnimation = 132;
            }
    
        } else if (!checkDailyEventBit(FESTIVAL)) {

            if (checkLifeEventBit(POPURI_GRAY_MARRIED)) {

                switch (gDayOfWeek) {

                    case MONDAY:
                    case TUESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (8 < gHour && gHour < 17) {

                            if (gWeather == SUNNY) {
                                
                                popuri->levelIndex = RANCH;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_E;
                                popuri->startingCoordinates.x = -32.0f;
                                popuri->startingCoordinates.z = -96.0f;
                                popuri->flags |= NPC_ACTIVE;

                                if (getNPCBabyCarryingState(POPURI) == 1) {
                                    popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                } else {
                                    popuri->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
                                }

                                setSpecialDialogueBit(POPURI_AT_RANCH_DIALOGUE);
                                                
                            } else {
                                
                                popuri->levelIndex = RANCH_STORE;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_W;
                                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                popuri->startingCoordinates.x = 64.0f;
                                popuri->startingCoordinates.z = 64.0f;
                                popuri->flags |= NPC_ACTIVE;  
                            }
                        
                        }

                        break;

                    case SUNDAY:
                    case WEDNESDAY:
    
                        popuri->levelIndex = RANCH_STORE;
                        popuri->startingCoordinates.y = 0.0f;
                        popuri->direction = DIRECTION_W;
                        popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        popuri->startingCoordinates.x = 64.0f;
                        popuri->startingCoordinates.z = 64.0f;
                        popuri->flags |= NPC_ACTIVE;  
                        break;
                    
                }

                if (getNPCBabyCarryingState(POPURI) == 1) {
                    popuri->idleAnimation = 122;
                    popuri->movingAnimation = 132;
                }
                
            } else {

                if (SUNDAY < gDayOfWeek && gDayOfWeek < FRIDAY) {

                    if (gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {

                            popuri->levelIndex = VILLAGE_1;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->direction = DIRECTION_S;
                            popuri->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
                            popuri->startingCoordinates.x = 96.0f;
                            popuri->startingCoordinates.z = -224.0f;
                            popuri->flags |= NPC_ACTIVE;  
                                
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {

                            popuri->levelIndex = FLOWER_SHOP;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->direction = DIRECTION_E;
                            popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            popuri->startingCoordinates.x = -112.0f;
                            popuri->startingCoordinates.z = 64.0f;
                            popuri->flags |= NPC_ACTIVE;  
                            
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == FRIDAY) {

                    if (gWeather == SUNNY) {

                        if (popuri->location < 2) {

                            if (8 < gHour && gHour < 17) {
                                
                                popuri->levelIndex = MOUNTAIN_2;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_S;
                                popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                popuri->startingCoordinates.x = -224.0f;
                                popuri->startingCoordinates.z = -128.0f;
                                popuri->flags |= NPC_ACTIVE;  

                                setSpecialDialogueBit(POPURI_AT_MOUNTAIN_2_DIALOGUE);
                                
                            }

                        } else {

                            if (8 < gHour && gHour < 17) {
                                
                                popuri->levelIndex = MOON_MOUNTAIN;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->startingCoordinates.z = 0.0f;
                                popuri->direction = DIRECTION_N;
                                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                popuri->startingCoordinates.x = 64.0f;
                                popuri->flags |= NPC_ACTIVE;  

                                setSpecialDialogueBit(POPURI_AT_MOON_MOUNTAIN_DIALOGUE);
                                
                            }
                            
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {

                            popuri->levelIndex = FLOWER_SHOP;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->direction = DIRECTION_E;
                            popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            popuri->startingCoordinates.x = -112.0f;
                            popuri->startingCoordinates.z = 64.0f;
                            popuri->flags |= NPC_ACTIVE;  
                            
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == SATURDAY) {

                    if (gWeather == SUNNY) {

                        if (popuri->location < 2) {

                            if (8 < gHour && gHour < 17) {
                                
                                popuri->levelIndex = MOUNTAIN_2;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_E;
                                popuri->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                popuri->startingCoordinates.x = -128.0f;
                                popuri->startingCoordinates.z = -416.0f;
                                popuri->flags |= NPC_ACTIVE;  

                                setSpecialDialogueBit(POPURI_AT_MOUNTAIN_2_DIALOGUE);
                                
                            }

                        } else {
                          
                            if (8 < gHour && gHour < 17) {
                                
                                popuri->levelIndex = POND;
                                popuri->startingCoordinates.y = 0.0f;
                                popuri->direction = DIRECTION_E;
                                popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                popuri->startingCoordinates.x = -224.0f;
                                popuri->startingCoordinates.z = -96.0f;
                                popuri->flags |= NPC_ACTIVE;  

                            }      
                            
                        } 
                        
                    } else {
                        
                        if (8 < gHour && gHour < 17) {

                            popuri->levelIndex = FLOWER_SHOP;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->direction = DIRECTION_E;
                            popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            popuri->startingCoordinates.x = -112.0f;
                            popuri->startingCoordinates.z = 64.0f;
                            popuri->flags |= NPC_ACTIVE;  
                            
                        }
                    }
                    
                }

                if (gDayOfWeek == SUNDAY) {

                    if (!(popuri->location < 2) && gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {
                            
                            popuri->levelIndex = RANCH;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->direction = DIRECTION_E;
                            popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            popuri->startingCoordinates.x = -32.0f;
                            popuri->startingCoordinates.z = -96.0f;
                            popuri->flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(POPURI_AT_RANCH_DIALOGUE);
                            
                        }
                                            
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            popuri->levelIndex = BAKERY;
                            popuri->startingCoordinates.x = 0.0f;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->startingCoordinates.z = 0.0f;
                            popuri->direction = DIRECTION_S;
                            popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            popuri->flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(POPURI_AT_BAKERY_DIALOGUE);
                            
                        }
                        
                    }
                    
                }

                if (gSeason == SUMMER && gDayOfWeek == THURSDAY) {

                    if (gWeather == SUNNY && popuri->location == 0) {

                        if (18 < gHour && gHour < 21) {
    
                            popuri->levelIndex = BEACH;
                            popuri->startingCoordinates.y = 0.0f;
                            popuri->startingCoordinates.z = 0.0f;
                            popuri->direction = DIRECTION_S;
                            popuri->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                            popuri->startingCoordinates.x = -224.0f;
                            popuri->flags |= NPC_ACTIVE;
    
                            setSpecialDialogueBit(POPURI_AT_BEACH_DIALOGUE);
                            
                        }

}
                    
                }

                if (gWeather == 4 && popuri->location == 0) {

                    if (8 < gHour && gHour < 17) {
                        
                        popuri->levelIndex = MOUNTAIN_2;
                        popuri->startingCoordinates.y = 0.0f;
                        popuri->direction = DIRECTION_S;
                        popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        popuri->startingCoordinates.x = -64.0f;
                        popuri->startingCoordinates.z = -160.0f;
                        popuri->flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(POPURI_AT_MOUNTAIN_2_DIALOGUE);
                                   
                    }
                    
                }

                if (gSeason == WINTER && 24 < gDayOfMonth && gDayOfMonth < 31) {

                    if (8 < gHour && gHour < 17) {
       
                        popuri->levelIndex = FLOWER_SHOP;
                        popuri->startingCoordinates.y = 0.0f;
                        popuri->direction = DIRECTION_E;
                        popuri->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        popuri->startingCoordinates.x = -112.0f;
                        popuri->startingCoordinates.z = 64.0f;
                        popuri->flags |= NPC_ACTIVE;  
                        
                    }
                                     
                }
                
            } 
        
        }
        
    }

    popuri->animationMode = popuri->defaultAnimationMode;
    
}

void setElliLocation(void) {
    npcInfo *elli = &npcs[ELLI];


    u8 result;

    elli->wanderRadiusX = 64;
    elli->wanderRadiusZ = 64;
    elli->idleAnimation = 0;
    elli->movingAnimation = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(ELLI_LOCATION_OVERRIDE)) {

        if (checkLifeEventBit(MARRIED) && gWife == ELLI) {

            if (5 < gHour && gHour < 8) {

                elli->levelIndex = HOUSE;
                elli->startingCoordinates.y = 0.0f;
                elli->direction = DIRECTION_SE;
                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                elli->startingCoordinates.x = -128.0f;
                elli->startingCoordinates.z = -64.0f;
                elli->flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (elli->location < 2) {
                                                    
                        elli->levelIndex = BAKERY;
                        elli->startingCoordinates.y = 0.0f;
                        elli->direction = DIRECTION_S;
                        elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        elli->startingCoordinates.x = -32.0f;
                        elli->startingCoordinates.z = 16.0f;
                        elli->flags |= NPC_ACTIVE;  

                    } else if (gWeather == SUNNY) {
                        
                        elli->levelIndex = MOUNTAIN_2;
                        elli->startingCoordinates.y = 0.0f;
                        elli->direction = DIRECTION_W;
                        elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        elli->startingCoordinates.x = 96.0f;
                        elli->startingCoordinates.z = -64.0f;
                        elli->flags |= NPC_ACTIVE;  
        
                        setSpecialDialogueBit(ELLI_AT_MOUNTAIN_2_DIALOGUE);
                                
                    } else {
                        
                        elli->levelIndex = KITCHEN;
                        elli->startingCoordinates.y = 0.0f;
                        elli->direction = DIRECTION_S;
                        elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        elli->startingCoordinates.x = -160.0f;
                        elli->startingCoordinates.z = -64.0f;
                        elli->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                                                
                        elli->levelIndex = FARM;
                        elli->startingCoordinates.y = 0.0f;
                        elli->startingCoordinates.z = 0.0f;
                        elli->direction = DIRECTION_S;
                        elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        elli->startingCoordinates.x = -448.0f;
                        elli->flags |= NPC_ACTIVE;
                        
                    } else {
                                                
                        elli->levelIndex = KITCHEN;
                        elli->startingCoordinates.y = 0.0f;
                        elli->direction = DIRECTION_S;
                        elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        elli->startingCoordinates.x = -160.0f;
                        elli->startingCoordinates.z = -64.0f;
                        elli->flags |= NPC_ACTIVE;
                        
                    }
                                        
                }
                
            }

            if (gHour == 12) {
                
                elli->levelIndex = KITCHEN;
                elli->startingCoordinates.y = 0.0f;
                elli->direction = DIRECTION_W;
                elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                elli->startingCoordinates.x = -128.0f;
                elli->startingCoordinates.z = -128.0f;
                elli->flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                elli->levelIndex = KITCHEN;
                elli->startingCoordinates.y = 0.0f;
                elli->direction = DIRECTION_S;
                elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                elli->startingCoordinates.x = -160.0f;
                elli->startingCoordinates.z = -64.0f;
                elli->flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                elli->levelIndex = KITCHEN;
                elli->startingCoordinates.y = 0.0f;
                elli->direction = DIRECTION_W;
                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                elli->startingCoordinates.x = -160.0f;
                elli->startingCoordinates.z = -64.0f;
                elli->flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                elli->levelIndex = HOUSE;
                elli->startingCoordinates.y = 0.0f;
                elli->direction = DIRECTION_E;
                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                elli->startingCoordinates.x = -128.0f;
                elli->startingCoordinates.z = -112.0f;
                elli->flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                elli->levelIndex = HOUSE;
                elli->startingCoordinates.y = 0.0f;
                elli->direction = DIRECTION_S;
                elli->defaultAnimationMode = NPC_ANIMATION_SLEEPING;
                elli->startingCoordinates.x = -192.0f;
                elli->startingCoordinates.z = -160.0f;
                elli->flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                elli->idleAnimation = 0x69;
                elli->movingAnimation = 0x73;
            }
    
        } else if (!checkDailyEventBit(FESTIVAL)) {

            if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

                if (gWeather == SUNNY) {

                    switch (gDayOfWeek) {

                        case MONDAY:

                            switch (gSeason) {

                                case SPRING:
                                case SUMMER:
                                    
                                    if (5 < gHour && gHour < 18) {
                            
                                        elli->levelIndex = MOUNTAIN_2;
                                        elli->startingCoordinates.y = 0.0f;
                                        elli->direction = DIRECTION_W;
                                        elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        elli->startingCoordinates.x = 96.0f;
                                        elli->startingCoordinates.z = -64.0f;
                                        elli->flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(ELLI_AT_MOUNTAIN_2_DIALOGUE);
                                        
                                    }            
                                            
                                    break;
                                
                                case WINTER:
                                    
                                    if (5 < gHour && gHour < 18) {
                
                                        elli->levelIndex = BAKERY;
                                        elli->startingCoordinates.y = 0.0f;
                                        elli->direction = DIRECTION_S;
                                        elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                        elli->startingCoordinates.x = -32.0f;
                                        elli->startingCoordinates.z = 16.0f;
                                        elli->flags |= NPC_ACTIVE;  
                                    
                                    }
                                    
                                    break;
                                
                                case AUTUMN:

                                     if (5 < gHour && gHour < 18) {
                                
                                        elli->levelIndex = MOUNTAIN_1;
                                        elli->startingCoordinates.y = 0.0f;
                                        elli->direction = DIRECTION_S;
                                        elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        elli->startingCoordinates.x = 112.0f;
                                        elli->startingCoordinates.z = -32.0f;
                                        elli->flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(ELLI_AT_MOUNTAIN_1_DIALOGUE);
                                        
                                    }       

                                    break;
                                
                            }

                            break;

                        case WEDNESDAY:
                            
                            if (8 < gHour && gHour < 12) {
                                
                                elli->levelIndex = FLOWER_SHOP;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_S;
                                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                elli->startingCoordinates.x = 32.0f;
                                elli->startingCoordinates.z = -16.0f;
                                elli->flags |= NPC_ACTIVE;  
        
                                setSpecialDialogueBit(ELLI_AT_FLOWER_SHOP_DIALOGUE);
        
                            }

                            if (11 < gHour && gHour < 17) {
    
                                elli->levelIndex = VILLAGE_1;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_W;
                                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                elli->startingCoordinates.x = 352.0f;
                                elli->startingCoordinates.z = 96.0f;
                                elli->flags |= NPC_ACTIVE;  
                                
                            }

                            break;

                        case SUNDAY:
                        case TUESDAY:
                        case THURSDAY:
                        case FRIDAY:
                        case SATURDAY:

                            if (5 < gHour && gHour < 18) {

                                elli->levelIndex = BAKERY;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_S;
                                elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                elli->startingCoordinates.x = -32.0f;
                                elli->startingCoordinates.z = 16.0f;
                                elli->flags |= NPC_ACTIVE;  
                                
                            }
                            
                            break;
                        
                    }
                    
                } else {
                    
                    if (5 < gHour && gHour < 18) {

                        elli->levelIndex = BAKERY;
                        elli->startingCoordinates.y = 0.0f;
                        elli->direction = DIRECTION_S;
                        elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        elli->startingCoordinates.x = -32.0f;
                        elli->startingCoordinates.z = 16.0f;
                        elli->flags |= NPC_ACTIVE;  
                    
                    }
                    
                }

                if (getNPCBabyCarryingState(ELLI) == 1) {
                    elli->idleAnimation = 0x69;
                    elli->movingAnimation = 0x73;
                }
                
            } else {

                switch (gDayOfWeek) {
                    
                    case TUESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (gWeather == SUNNY) {
                            
                            if (8 < gHour && gHour < 17) {
                            
                                elli->levelIndex = BAKERY;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_S;
                                elli->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                elli->startingCoordinates.x = -32.0f;
                                elli->startingCoordinates.z = 16.0f;
                                elli->flags |= NPC_ACTIVE;  
        
                            } 
                            
                        } else {
                                                    
                            if (8 < gHour && gHour < 17) {
                                
                                elli->levelIndex = BAKERY;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_S;
                                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                elli->startingCoordinates.x = -32.0f;
                                elli->startingCoordinates.z = 16.0f;
                                elli->flags |= NPC_ACTIVE;  
        
                            } 
                            
                        }
                        
                        break;
                    
                }

                if (gDayOfWeek == WEDNESDAY) {

                    if (8 < gHour && gHour < 12) {
                        
                        elli->levelIndex = FLOWER_SHOP;
                        elli->startingCoordinates.y = 0.0f;
                        elli->direction = DIRECTION_S;
                        elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        elli->startingCoordinates.x = 32.0f;
                        elli->startingCoordinates.z = -16.0f;
                        elli->flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(ELLI_AT_FLOWER_SHOP_DIALOGUE);

                    }

                    if (11 < gHour && gHour < 17) {

                        if (gWeather == SUNNY) {
                                                    
                            elli->levelIndex = VILLAGE_1;
                            elli->startingCoordinates.y = 0.0f;
                            elli->direction = DIRECTION_W;
                            elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            elli->startingCoordinates.x = 352.0f;
                            elli->startingCoordinates.z = 96.0f;
                            elli->flags |= NPC_ACTIVE;  

                        } else {
                            
                            elli->levelIndex = BAKERY;
                            elli->startingCoordinates.y = 0.0f;
                            elli->direction = DIRECTION_S;
                            elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            elli->startingCoordinates.x = -32.0f;
                            elli->startingCoordinates.z = 16.0f;
                            elli->flags |= NPC_ACTIVE;  
    
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == MONDAY) {

                    if (gWeather == SUNNY) {

                        if (elli->location < 2) {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                elli->levelIndex = MOUNTAIN_2;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_W;
                                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                elli->startingCoordinates.x = 96.0f;
                                elli->startingCoordinates.z = -64.0f;
                                elli->flags |= NPC_ACTIVE;  
        
                                setSpecialDialogueBit(ELLI_AT_MOUNTAIN_2_DIALOGUE);
                                
                            }            
                            
                        } else {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                elli->levelIndex = MOUNTAIN_1;
                                elli->startingCoordinates.y = 0.0f;
                                elli->direction = DIRECTION_S;
                                elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                elli->startingCoordinates.x = 112.0f;
                                elli->startingCoordinates.z = -32.0f;
                                elli->flags |= NPC_ACTIVE;  
        
                                setSpecialDialogueBit(ELLI_AT_MOUNTAIN_1_DIALOGUE);
                                
                            }       

}
                        
                    } else {
                             
                        if (8 < gHour && gHour < 17) {
                            
                            elli->levelIndex = BAKERY;
                            elli->startingCoordinates.y = 0.0f;
                            elli->direction = DIRECTION_S;
                            elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            elli->startingCoordinates.x = -32.0f;
                            elli->startingCoordinates.z = 16.0f;
                            elli->flags |= NPC_ACTIVE;  
    
                        }             

                    }
                    
                }

                if (gSeason == SUMMER) {

                    if (gDayOfWeek == WEDNESDAY && gWeather == SUNNY && elli->location == 0) {

                        if (18 < gHour && gHour < 21) {
                            
                            elli->levelIndex = BEACH;
                            elli->startingCoordinates.y = 0.0f;
                            elli->startingCoordinates.z = 0.0f;
                            elli->direction = DIRECTION_S;
                            elli->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                            elli->startingCoordinates.x = -224.0f;
                            elli->flags |= NPC_ACTIVE;  
    
                            setSpecialDialogueBit(ELLI_AT_BEACH_DIALOGUE);
                            
                        }
                        
                    }

                    if (gSeason == SUMMER && gDayOfWeek == MONDAY && gWeather == SUNNY) {
     
                        if (7 < gHour && gHour < 17) {
           
                            elli->levelIndex = BEACH;
                            elli->startingCoordinates.y = 0.0f;
                            elli->direction = DIRECTION_N;
                            elli->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            elli->startingCoordinates.x = -128.0f;
                            elli->startingCoordinates.z = 160.0f;
                            elli->flags |= NPC_ACTIVE;  
    
                            setSpecialDialogueBit(ELLI_AT_BEACH_DIALOGUE);
                            
                        }
                                         
                    }
                    
                }
                
            } 
        
        }
        
    }

    elli->animationMode = elli->defaultAnimationMode;
    
}

void setAnnLocation(void) {
    npcInfo *ann = &npcs[ANN];


    u8 result;

    ann->wanderRadiusX = 64;
    ann->wanderRadiusZ = 64;
    ann->idleAnimation = 0;
    ann->movingAnimation = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkDailyEventBit(ANN_LOCATION_OVERRIDE)) {

        if (checkLifeEventBit(MARRIED) && gWife == ANN) {

            if (5 < gHour && gHour < 8) {

                ann->levelIndex = HOUSE;
                ann->startingCoordinates.y = 0.0f;
                ann->direction = DIRECTION_SE;
                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                ann->startingCoordinates.x = -128.0f;
                ann->startingCoordinates.z = -64.0f;
                ann->flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (ann->location < 2) {
                                                    
                        ann->levelIndex = RICK_STORE;
                        ann->startingCoordinates.x = 0.0f;
                        ann->startingCoordinates.y = 0.0f;
                        ann->startingCoordinates.z = 0.0f;
                        ann->direction = DIRECTION_S;
                        ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        ann->flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(ANN_AT_RICK_STORE_DIALOGUE);

                    } else  {

                        if (gWeather == SUNNY) {
                            
                            ann->levelIndex = RANCH;
                            ann->startingCoordinates.y = 0.0f;
                            ann->direction = DIRECTION_S;
                            ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            ann->startingCoordinates.x = -64.0f;
                            ann->startingCoordinates.z = 128.0f;
                            ann->flags |= NPC_ACTIVE;  
                            
                        } else if (getTotalFarmAnimalsCount() != 0) {

                            ann->levelIndex = BARN;
                            ann->startingCoordinates.x = 0.0f;
                            ann->startingCoordinates.y = 0.0f;
                            ann->startingCoordinates.z = 0.0f;
                            ann->direction = DIRECTION_S;
                            ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            ann->flags |= NPC_ACTIVE;
                            
                        } else {

                            if (getTotalChickenCount()) {

                                ann->levelIndex = COOP;
                                ann->startingCoordinates.x = 0.0f;
                                ann->startingCoordinates.y = 0.0f;
                                ann->startingCoordinates.z = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->flags |= NPC_ACTIVE;
                                
                            } else {
                                
                                ann->levelIndex = HOUSE;
                                ann->startingCoordinates.x = 0.0f;
                                ann->startingCoordinates.y = 0.0f;
                                ann->startingCoordinates.z = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->flags |= NPC_ACTIVE;
                                
                            }
                            
                        }
                        
                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                                                
                        ann->levelIndex = FARM;
                        ann->startingCoordinates.y = 0.0f;
                        ann->direction = DIRECTION_S;
                        ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        ann->startingCoordinates.x = -256.0f;
                        ann->startingCoordinates.z = -320.0f;
                        ann->flags |= NPC_ACTIVE;
                        
                    } else {
                                                
                        if (getTotalFarmAnimalsCount()) {

                            ann->levelIndex = BARN;
                            ann->startingCoordinates.x = 0.0f;
                            ann->startingCoordinates.y = 0.0f;
                            ann->startingCoordinates.z = 0.0f;
                            ann->direction = DIRECTION_S;
                            ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            ann->flags |= NPC_ACTIVE;
                            
                        } else {

                            if (getTotalChickenCount()) {

                                ann->levelIndex = COOP;
                                ann->startingCoordinates.x = 0.0f;
                                ann->startingCoordinates.y = 0.0f;
                                ann->startingCoordinates.z = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->flags |= NPC_ACTIVE;
                                
                            } else {
                                
                                ann->levelIndex = HOUSE;
                                ann->startingCoordinates.x = 0.0f;
                                ann->startingCoordinates.y = 0.0f;
                                ann->startingCoordinates.z = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->flags |= NPC_ACTIVE;
                                
                            }
                            
                        }
                        
                    }
                                        
                }
                
            }

            if (gHour == 12) {
                
                ann->levelIndex = KITCHEN;
                ann->startingCoordinates.y = 0.0f;
                ann->direction = DIRECTION_W;
                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                ann->startingCoordinates.x = -160.0f;
                ann->startingCoordinates.z = -64.0f;
                ann->flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                ann->levelIndex = HOUSE;
                ann->startingCoordinates.x = 0.0f;
                ann->startingCoordinates.y = 0.0f;
                ann->startingCoordinates.z = 0.0f;
                ann->direction = DIRECTION_S;
                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                ann->flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                ann->levelIndex = KITCHEN;
                ann->startingCoordinates.y = 0.0f;
                ann->direction = DIRECTION_W;
                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                ann->startingCoordinates.x = -160.0f;
                ann->startingCoordinates.z = -64.0f;
                ann->flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                ann->levelIndex = HOUSE;
                ann->startingCoordinates.y = 0.0f;
                ann->direction = DIRECTION_E;
                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                ann->startingCoordinates.x = -128.0f;
                ann->startingCoordinates.z = -112.0f;
                ann->flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                ann->levelIndex = HOUSE;
                ann->startingCoordinates.y = 0.0f;
                ann->direction = DIRECTION_S;
                ann->defaultAnimationMode = NPC_ANIMATION_SLEEPING;
                ann->startingCoordinates.x = -192.0f;
                ann->startingCoordinates.z = -160.0f;
                ann->flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                ann->idleAnimation = 0x6F;
                ann->movingAnimation = 0x79;
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
                            
                                ann->levelIndex = RANCH;
                                ann->startingCoordinates.y = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->startingCoordinates.x = -64.0f;
                                ann->startingCoordinates.z = 128.0f;
                                ann->flags |= NPC_ACTIVE;  
                                
                            }
                            
                        } else {

                            if (7 < gHour && gHour < 17) {
                                
                                ann->levelIndex = RANCH_HOUSE;
                                ann->startingCoordinates.y = 0.0f;
                                ann->direction = DIRECTION_E;
                                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                ann->startingCoordinates.x = -192.0f;
                                ann->startingCoordinates.z = -32.0f;
                                ann->flags |= NPC_ACTIVE;  
                                
                            }
                            
                        }

                        break;

                    case THURSDAY:

                        if (gWeather == SUNNY) {

                            if (8 < gHour && gHour < 17) {

                                switch (gSeason) {

                                    case SPRING:
                                    case AUTUMN:
                                        ann->levelIndex = MOUNTAIN_2;
                                        ann->startingCoordinates.y = 0.0f;
                                        ann->direction = DIRECTION_S;
                                        ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        ann->startingCoordinates.x = -64.0f;
                                        ann->startingCoordinates.z = -160.0f;
                                        ann->flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(ANN_AT_MOUNTAIN_DIALOGUE);

                                        break;

                                    case SUMMER:
                                        
                                        ann->levelIndex = CAVE;
                                        ann->startingCoordinates.y = 0.0f;
                                        ann->direction = DIRECTION_S;
                                        ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        ann->startingCoordinates.x = 64.0f;
                                        ann->startingCoordinates.z = -224.0f;
                                        ann->flags |= NPC_ACTIVE;  
                
                                        setSpecialDialogueBit(ANN_AT_MOUNTAIN_DIALOGUE);

                                        break;

                                    case WINTER:
                                                                                
                                        ann->levelIndex = RANCH_HOUSE;
                                        ann->startingCoordinates.y = 0.0f;
                                        ann->direction = DIRECTION_E;
                                        ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        ann->startingCoordinates.x = -192.0f;
                                        ann->startingCoordinates.z = -32.0f;
                                        ann->flags |= NPC_ACTIVE;  
                                        
                                        break;
                                    
                                    }
                                
                            }
                            
                        } else if (7 < gHour && gHour < 17) {
                            
                            ann->levelIndex = RANCH_BARN;
                            ann->startingCoordinates.y = 0.0f;
                            ann->direction = DIRECTION_S;
                            ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            ann->startingCoordinates.x = -128.0f;
                            ann->startingCoordinates.z = -292.0f;
                            ann->flags |= NPC_ACTIVE;  
    
                        }

                        break;
                    
                }
                
                if (getNPCBabyCarryingState(ANN) == 1) {
                    ann->idleAnimation = 0x6F;
                    ann->movingAnimation = 0x79;
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
                            
                                ann->levelIndex = RANCH;
                                ann->startingCoordinates.y = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->startingCoordinates.x = -64.0f;
                                ann->startingCoordinates.z = 128.0f;
                                ann->flags |= NPC_ACTIVE;  
                                
                            }
                            
                        } else {
                            
                            if (7 < gHour && gHour < 17) {
                            
                                ann->levelIndex = RANCH_BARN;
                                ann->startingCoordinates.y = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                ann->startingCoordinates.x = -128.0f;
                                ann->startingCoordinates.z = -292.0f;
                                ann->flags |= NPC_ACTIVE;  
        
                            }
                            
                        }
                        
                        break;
                    
                }

                if (gDayOfWeek == THURSDAY) {

                    if (gWeather == SUNNY) {

                        switch (ann->location) {

                            case 0:

                                if (8 < gHour && gHour < 17) {
                        
                                    ann->levelIndex = MOUNTAIN_1;
                                    ann->startingCoordinates.y = 0.0f;
                                    ann->direction = DIRECTION_S;
                                    ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    ann->startingCoordinates.x = 128.0f;
                                    ann->startingCoordinates.z = 32.0f;
                                    ann->flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(ANN_AT_MOUNTAIN_DIALOGUE);
                                    
                                }        
                                
                                break;

                            case 1:
                                
                                if (8 < gHour && gHour < 17) {
                                
                                    ann->levelIndex = MOUNTAIN_2;
                                    ann->startingCoordinates.y = 0.0f;
                                    ann->direction = DIRECTION_S;
                                    ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    ann->startingCoordinates.x = -64.0f;
                                    ann->startingCoordinates.z = -160.0f;
                                    ann->flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(ANN_AT_MOUNTAIN_DIALOGUE);
                                    
                                }            
                                break;
                            
                            case 2:

                                if (8 < gHour && gHour < 17) {
                        
                                    ann->levelIndex = VINEYARD;
                                    ann->startingCoordinates.y = 0.0f;
                                    ann->direction = DIRECTION_S;
                                    ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    ann->startingCoordinates.x = 16.0f;
                                    ann->startingCoordinates.z = -144.0f;
                                    ann->flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(ANN_AT_VINEYARD_DIALOGUE);
                                    
                                }        
                                
                                break;

case 3:

                                if (8 < gHour && gHour < 17) {
                                    
                                    ann->levelIndex = RICK_STORE;
                                    ann->startingCoordinates.x = 0.0f;
                                    ann->startingCoordinates.y = 0.0f;
                                    ann->startingCoordinates.z = 0.0f;
                                    ann->direction = DIRECTION_S;
                                    ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    ann->flags |= NPC_ACTIVE;  
            
                                    setSpecialDialogueBit(ANN_AT_RICK_STORE_DIALOGUE);
                                    
                                }
                            
                        }

} else {
                        
                       if (8 < gHour && gHour < 17) {
                        
                            ann->levelIndex = RANCH_STORE;
                            ann->startingCoordinates.y = 0.0f;
                            ann->direction = DIRECTION_S;
                            ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            ann->startingCoordinates.x = 80.0f;
                            ann->startingCoordinates.z = -144.0f;
                            ann->flags |= NPC_ACTIVE;  

}      
                        
                    }
                   
                }

                if (gDayOfWeek == SUNDAY) {

                    if (gWeather == SUNNY) {

                        if (ann->location < 2) {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                ann->levelIndex = POND;
                                ann->startingCoordinates.y = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                ann->startingCoordinates.x = 80.0f;
                                ann->startingCoordinates.z = -176.0f;
                                ann->flags |= NPC_ACTIVE;  
        
                            }            
                            
                        } else {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                ann->levelIndex = RANCH;
                                ann->startingCoordinates.y = 0.0f;
                                ann->direction = DIRECTION_S;
                                ann->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                ann->startingCoordinates.x = 256.0f;
                                ann->startingCoordinates.z = 128.0f;
                                ann->flags |= NPC_ACTIVE;  
        
                            }       
                            
                        }
                        
                    } else {
                             
                        if (8 < gHour && gHour < 17) {
                            
                            ann->levelIndex = RANCH_BARN;
                            ann->startingCoordinates.y = 0.0f;
                            ann->direction = DIRECTION_S;
                            ann->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            ann->startingCoordinates.x = -128.0f;
                            ann->startingCoordinates.z = -292.0f;
                            ann->flags |= NPC_ACTIVE;  
    
                        }             
                    }
                    
                }

if (gSeason == SUMMER && gDayOfWeek == TUESDAY && gWeather == SUNNY && ann->location == 0) {

                    if (17 < gHour && gHour < 21) {
       
                        ann->levelIndex = BEACH;
                        ann->startingCoordinates.y = 0.0f;
                        ann->startingCoordinates.z = 0.0f;
                        ann->direction = DIRECTION_S;
                        ann->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                        ann->startingCoordinates.x = -224.0f;
                        ann->flags |= NPC_ACTIVE;  

                        setSpecialDialogueBit(ANN_AT_BEACH_DIALOGUE);
                        
                    }

}
                
            } 
        
        }
        
    }

    ann->animationMode = ann->defaultAnimationMode;
    
}

void setKarenLocation(void) {
    npcInfo *karen = &npcs[KAREN];


    u8 result;

    karen->wanderRadiusX = 64;
    karen->wanderRadiusZ = 64;
    karen->idleAnimation = 0;
    karen->movingAnimation = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && !checkLifeEventBit(KAREN_GONE) && !checkDailyEventBit(KAREN_LOCATION_OVERRIDE)) {

        if (checkLifeEventBit(MARRIED) && gWife == KAREN) {

            if (5 < gHour && gHour < 8) {

                karen->levelIndex = HOUSE;
                karen->startingCoordinates.y = 0.0f;
                karen->direction = DIRECTION_SE;
                karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                karen->startingCoordinates.x = -128.0f;
                karen->startingCoordinates.z = -64.0f;
                karen->flags |= NPC_ACTIVE;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    switch (karen->location) {

                        case 0:
                        case 1:
                            karen->levelIndex = BEACH;
                            karen->startingCoordinates.y = 0.0f;
                            karen->startingCoordinates.z = 0.0f;
                            karen->direction = DIRECTION_E;
                            karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            karen->startingCoordinates.x = -224.0f;
                            karen->flags |= NPC_ACTIVE;

                            setSpecialDialogueBit(KAREN_AT_BEACH_DIALOGUE);

                            break;
                        
                        case 2:
                                                        
                            karen->levelIndex = VINEYARD;
                            karen->startingCoordinates.y = 0.0f;
                            karen->startingCoordinates.z = 0.0f;
                            karen->direction = DIRECTION_W;
                            karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            karen->startingCoordinates.x = -32.0f;
                            karen->flags |= NPC_ACTIVE;
                            
                            break;
                        
                        case 3:

                            karen->levelIndex = MOUNTAIN_1;
                            karen->startingCoordinates.y = 0.0f;
                            karen->direction = DIRECTION_E;
                            karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            karen->startingCoordinates.x = 96.0f;
                            karen->startingCoordinates.z = 96.0f;
                            karen->flags |= NPC_ACTIVE;

                            setSpecialDialogueBit(KAREN_AT_MOUNTAIN_DIALOGUE);

                            break;
                        
                    } 
                    
                } else if (gWeather == SUNNY) {
                        
                    karen->levelIndex = FARM;
                    karen->startingCoordinates.y = 0.0f;
                    karen->direction = DIRECTION_S;
                    karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    karen->startingCoordinates.x = -256.0f;
                    karen->startingCoordinates.z = -320.0f;
                    karen->flags |= NPC_ACTIVE;
                    
                } else {

                    karen->levelIndex = KITCHEN;
                    karen->startingCoordinates.y = 0.0f;
                    karen->direction = DIRECTION_W;
                    karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    karen->startingCoordinates.x = -160.0f;
                    karen->startingCoordinates.z = -64.0f;
                    karen->flags |= NPC_ACTIVE;
                    
                }
                
            }

            if (gHour == 12) {
                
                karen->levelIndex = KITCHEN;
                karen->startingCoordinates.y = 0.0f;
                karen->direction = DIRECTION_W;
                karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                karen->startingCoordinates.x = -160.0f;
                karen->startingCoordinates.z = -64.0f;
                karen->flags |= NPC_ACTIVE;
                
            }

            if (12 < gHour && gHour < 17) {

                karen->levelIndex = HOUSE;
                karen->startingCoordinates.x = 0.0f;
                karen->startingCoordinates.y = 0.0f;
                karen->startingCoordinates.z = 0.0f;
                karen->direction = DIRECTION_S;
                karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                karen->flags |= NPC_ACTIVE;
                
            }

            if (16 < gHour && gHour < 20) {
                
                karen->levelIndex = KITCHEN;
                karen->startingCoordinates.y = 0.0f;
                karen->direction = DIRECTION_W;
                karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                karen->startingCoordinates.x = -160.0f;
                karen->startingCoordinates.z = -64.0f;
                karen->flags |= NPC_ACTIVE;
                
            }
            
            if (19 < gHour && gHour < 22) {

                karen->levelIndex = HOUSE;
                karen->startingCoordinates.y = 0.0f;
                karen->direction = DIRECTION_E;
                karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                karen->startingCoordinates.x = -128.0f;
                karen->startingCoordinates.z = -112.0f;
                karen->flags |= NPC_ACTIVE;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                karen->levelIndex = HOUSE;
                karen->startingCoordinates.y = 0.0f;
                karen->direction = DIRECTION_S;
                karen->defaultAnimationMode = NPC_ANIMATION_SLEEPING;
                karen->startingCoordinates.x = -192.0f;
                karen->startingCoordinates.z = -160.0f;
                karen->flags |= NPC_ACTIVE;
                
            }

            if (getBabyCarryingState() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                karen->idleAnimation = 0x66;
                karen->movingAnimation = 0x70;
            }
    
        } else if (!checkDailyEventBit(FESTIVAL)) {

            if (checkLifeEventBit(KAREN_KAI_MARRIED)) {

                if (gWeather == SUNNY) {

                    switch (gDayOfWeek) {

                        case SUNDAY:

                            if (9 < gHour && gHour < 18) {
                                
                                karen->levelIndex = BEACH;
                                karen->startingCoordinates.y = 0.0f;
                                karen->startingCoordinates.z = 0.0f;
                                karen->direction = DIRECTION_E;
                                karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                karen->startingCoordinates.x = -224.0f;
                                karen->flags |= NPC_ACTIVE;
            
                                setSpecialDialogueBit(KAREN_AT_BEACH_DIALOGUE);
                                
                            }

                            break;

                        case MONDAY:
                        case THURSDAY:
                            
                            if (8 < gHour && gHour < 17) { 
                            
                                karen->levelIndex = VINEYARD;
                                karen->startingCoordinates.y = 0.0f;
                                karen->direction = DIRECTION_N;
                                karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                karen->startingCoordinates.x = 192.0f;
                                karen->startingCoordinates.z = 192.0f;
                                karen->flags |= NPC_ACTIVE;
                                
                            }
                            
                            break;

                        case TUESDAY:
                        case WEDNESDAY:
                        case FRIDAY:
                        case SATURDAY:

                            if (8 < gHour && gHour < 17) {
                                
                                if (karen->location < 2) {
                                                         
                                    karen->levelIndex = VINEYARD_CELLAR_BASEMENT;
                                    karen->startingCoordinates.y = 0.0f;
                                    karen->direction = DIRECTION_W;
                                    karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    karen->startingCoordinates.x = -32.0f;
                                    karen->startingCoordinates.z = -128.0f;
                                    karen->flags |= NPC_ACTIVE;
                                            
                                }
                                
                            }
                        
                    }

                    if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

                        if (8 < gHour && gHour < 17) {
                                             
                            karen->levelIndex = VINEYARD;
                            karen->startingCoordinates.y = 0.0f;
                            karen->startingCoordinates.z = 0.0f;
                            karen->direction = DIRECTION_S;
                            karen->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
                            karen->startingCoordinates.x = -192.0f;
                            karen->flags |= NPC_ACTIVE;
                                              
                        }
                        
                    }
                    
                } else {

                    if (karen->location < 2) {

                        if (8 < gHour && gHour < 17) {

                            karen->levelIndex = VINEYARD_CELLAR;
                            karen->startingCoordinates.y = 0.0f;
                            karen->direction = DIRECTION_W;
                            karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            karen->startingCoordinates.x = -80.0f;
                            karen->startingCoordinates.z = -112.0f;
                            karen->flags |= NPC_ACTIVE;
                            
                        }
                        
                    }

}

                if (getNPCBabyCarryingState(KAREN) == 1) {
                    karen->idleAnimation = 0x66;
                    karen->movingAnimation = 0x70;
                }
                
            } else {

                if ((gDayOfWeek == MONDAY || gDayOfWeek == THURSDAY) && gWeather == SUNNY && 8 < gHour && gHour < 17) {
                        
                    karen->levelIndex = VINEYARD;
                    karen->startingCoordinates.y = 0.0f;
                    karen->direction = DIRECTION_N;
                    karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    karen->startingCoordinates.x = 192.0f;
                    karen->startingCoordinates.z = 192.0f;
                    karen->flags |= NPC_ACTIVE;
                                    
                }

                switch (gDayOfWeek) {

case TUESDAY:
                    case WEDNESDAY:
                    case FRIDAY:
                    case SATURDAY:
                        
                        if (gWeather == SUNNY) {

                            switch (karen->location) {
                                
                                case 0:

                                    if (9 < gHour && gHour < 17) {
                                        
                                        karen->levelIndex = MOUNTAIN_2;
                                        karen->startingCoordinates.y = 0.0f;
                                        karen->direction = DIRECTION_S;
                                        karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        karen->startingCoordinates.x = -80.0f;
                                        karen->startingCoordinates.z = 160.0f;
                                        karen->flags |= NPC_ACTIVE;
            
                                        setSpecialDialogueBit(KAREN_AT_MOUNTAIN_DIALOGUE);
                                        
                                    } 
                                
                                    break;

                                case 1:

                                    if (9 < gHour && gHour < 17) {
                                        
                                        karen->levelIndex = BEACH;
                                        karen->startingCoordinates.y = 0.0f;
                                        karen->direction = DIRECTION_E;
                                        karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                        karen->startingCoordinates.x = -96.0f;
                                        karen->startingCoordinates.z = 176.0f;
                                        karen->flags |= NPC_ACTIVE;
                    
                                        setSpecialDialogueBit(KAREN_AT_BEACH_DIALOGUE);
                                        
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
                        
                        karen->levelIndex = BEACH;
                        karen->startingCoordinates.y = 0.0f;
                        karen->startingCoordinates.z = 0.0f;
                        karen->direction = DIRECTION_E;
                        karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        karen->startingCoordinates.x = -224.0f;
                        karen->flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(KAREN_AT_BEACH_DIALOGUE);

}
                    
                } 

                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11 && gWeather == SUNNY) {

                    if (8 < gHour && gHour < 17) {
                        
                        karen->levelIndex = VINEYARD;
                        karen->startingCoordinates.y = 0.0f;
                        karen->startingCoordinates.z = 0.0f;
                        karen->direction = DIRECTION_S;
                        karen->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
                        karen->startingCoordinates.x = -192.0f;
                        karen->flags |= NPC_ACTIVE;
                        
                    }
                    
                }

                if NIGHTTIME {

                    if (3 < gDayOfWeek && gDayOfWeek < 7) {
                        
                        karen->levelIndex = TAVERN;
                        karen->startingCoordinates.y = 0.0f;
                        karen->direction = DIRECTION_S;
                        karen->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        karen->startingCoordinates.x = 64.0f;
                        karen->startingCoordinates.z = -32.0f;
                        karen->flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(KAREN_AT_TAVERN_DIALOGUE);
                        
                    }

                    if (gDayOfWeek == MONDAY) {

                        karen->levelIndex = TAVERN;
                        karen->startingCoordinates.y = 0;
                        karen->direction = DIRECTION_S;
                        karen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        karen->startingCoordinates.x = -64.0f;
                        karen->startingCoordinates.z = -64.0f;
                        karen->flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(KAREN_AT_TAVERN_DIALOGUE);
                        
                    }
                    
                }

                if (gSeason == SUMMER && gDayOfWeek == SATURDAY && gWeather == SUNNY && karen->location == 0) {

                    if (18 < gHour && gHour < 21) {

                            karen->levelIndex = BEACH;
                            karen->startingCoordinates.y = 0.0f;
                            karen->startingCoordinates.z = 0.0f;
                            karen->direction = DIRECTION_S;
                            karen->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                            karen->startingCoordinates.x = -224.0f;
                            karen->flags |= NPC_ACTIVE;

                            setSpecialDialogueBit(KAREN_AT_BEACH_DIALOGUE);
                        
                    }
                    
                }
                
            }
            
        } 
        
    }

    karen->animationMode = karen->defaultAnimationMode;
    
}

void setBabyLocation(void) {
    npcInfo *baby = &npcs[BABY];


    u8 set = FALSE;
    
    clearDailyEventBit(PICKED_UP_BABY);

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY) && !checkLifeEventBit(WIFE_LEFT) && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {

        baby->wanderRadiusX = 64;
        baby->wanderRadiusZ = 64;
        baby->idleAnimation = 0;
        baby->movingAnimation = 8;

        set = getBabyCarryingState();

        if (set == 1) {

            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                baby->levelIndex = npcs[gWife].levelIndex;
                baby->startingCoordinates.x = 0.0f;
                baby->startingCoordinates.y = 0.0f;
                baby->startingCoordinates.z = 0.0f;
                baby->flags |= (NPC_ACTIVE | NPC_ATTACHED);
                baby->direction = npcs[gWife].direction;
                baby->defaultAnimationMode = NPC_ANIMATION_IDLE;
                
            }
            
        }

        if (set == 2) {

            baby->levelIndex = HOUSE;
            baby->startingCoordinates.y = 0.0f;
            baby->direction = DIRECTION_S;
            baby->defaultAnimationMode = NPC_ANIMATION_WANDER;
            baby->startingCoordinates.x = -192.0f;
            baby->startingCoordinates.z = -64.0f;
            baby->flags |= NPC_ACTIVE;

            setDailyEventBit(PICKED_UP_BABY);
            
        }

        if (set == 3) {

            baby->levelIndex = HOUSE;
            baby->startingCoordinates.y = 0.0f;
            baby->direction = DIRECTION_S;
            baby->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
            baby->startingCoordinates.x = -192.0f;
            baby->startingCoordinates.z = -64.0f;
            baby->flags |= NPC_ACTIVE;

            setDailyEventBit(PICKED_UP_BABY);
            
        } 

        if (set == 4) {

            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                baby->levelIndex = npcs[gWife].levelIndex;
                baby->startingCoordinates.x = npcs[gWife].startingCoordinates.x;
                baby->startingCoordinates.y = npcs[gWife].startingCoordinates.y;
                baby->startingCoordinates.z = npcs[gWife].startingCoordinates.z;
                baby->flags |= NPC_ACTIVE;
                baby->direction = npcs[gWife].direction;
                baby->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
                baby->idleAnimation = 57;
                baby->movingAnimation = 65;
                
            }
            
        }

        if (set == 5) {

            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                baby->levelIndex = HOUSE;
                baby->startingCoordinates.y = 0.0f;
                baby->direction = DIRECTION_S;
                baby->defaultAnimationMode = NPC_ANIMATION_BABY_CRIB;
                baby->startingCoordinates.x = -194.0f;
                baby->startingCoordinates.z = -64.0f;
                baby->flags |= NPC_ACTIVE;
                
            }
            
        }

        if (set == 6) {
            
            if (npcs[gWife].flags & NPC_ACTIVE) {
                
                baby->levelIndex = npcs[gWife].levelIndex;
                baby->startingCoordinates.x = npcs[gWife].startingCoordinates.x;
                baby->startingCoordinates.y = npcs[gWife].startingCoordinates.y;
                baby->startingCoordinates.z = npcs[gWife].startingCoordinates.z;
                baby->flags |= NPC_ACTIVE;
                baby->direction = npcs[gWife].direction;
                baby->defaultAnimationMode = NPC_ANIMATION_BABY_HOUSE;
                baby->idleAnimation = 14;
                baby->movingAnimation = 22;
                
            }
            
        }
        
        baby->animationMode = baby->defaultAnimationMode;
        
    }
    
}

void setHarrisLocation(void) {
    npcInfo *harris = &npcs[HARRIS];


    harris->wanderRadiusX = 0x80;
    harris->wanderRadiusZ = 0x80;
    harris->idleAnimation = 0;
    harris->movingAnimation = 8;

    if (gDayOfWeek != SUNDAY && gDayOfWeek < 7) {

        if (8 < gHour && gHour < 12) {

            if (harris->location < 2) {

                harris->levelIndex = VILLAGE_1;
                harris->startingCoordinates.x = 0.0f;
                harris->startingCoordinates.y = 0.0f;
                harris->startingCoordinates.z = 0.0f;
                harris->direction = DIRECTION_S;
                harris->defaultAnimationMode = NPC_ANIMATION_WANDER;
                harris->flags |= NPC_ACTIVE;

} else {

                harris->levelIndex = VILLAGE_2;
                harris->startingCoordinates.x = 0.0f;
                harris->startingCoordinates.y = 0.0f;
                harris->startingCoordinates.z = 0.0f;
                harris->direction = DIRECTION_S;
                harris->defaultAnimationMode = NPC_ANIMATION_WANDER;
                harris->flags |= NPC_ACTIVE;
                
            }
            
        }
        
        if (11 < gHour && gHour < 18) { 

            switch (harris->location) {

                case 0:
                case 1:
                    harris->levelIndex = MOUNTAIN_1;
                    harris->startingCoordinates.y = 0.0f;
                    harris->direction = DIRECTION_S;
                    harris->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    harris->startingCoordinates.x = -16.0f;
                    harris->startingCoordinates.z = 128.0f;
                    harris->flags |= NPC_ACTIVE;                    
                    break;
                
                case 2:
    
                    harris->levelIndex = ROAD;
                    harris->startingCoordinates.y = 0.0f;
                    harris->startingCoordinates.z = 0.0f;
                    harris->direction = DIRECTION_S;
                    harris->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    harris->startingCoordinates.x = -64.0f;
                    harris->flags |= NPC_ACTIVE;
                    break;
                
                case 3:

                    harris->levelIndex = RANCH;
                    harris->startingCoordinates.y = 0.0f;
                    harris->direction = DIRECTION_S;
                    harris->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    harris->startingCoordinates.x = 160.0f;
                    harris->startingCoordinates.z = -320.0f;
                    harris->flags |= NPC_ACTIVE;

                    break;

}
            
        }
        
    }

    if (gDayOfWeek == SUNDAY) {

        if (11 < gHour && gHour < 17) {
            
            harris->levelIndex = LIBRARY;
            harris->startingCoordinates.y = 0.0f;
            harris->direction = DIRECTION_E;
            harris->defaultAnimationMode = NPC_ANIMATION_IDLE;
            harris->startingCoordinates.x = 32.0f;
            harris->startingCoordinates.z = 96.0f;
            harris->flags |= NPC_ACTIVE;
            
        }
        
    }

    if (!checkLifeEventBit(MARIA_HARRIS_MARRIED) && 17 < gHour && gHour < 24 && harris->location < 2) {

        harris->levelIndex = TAVERN;
        harris->startingCoordinates.y = 0.0f;
        harris->startingCoordinates.z = 0.0f;
        harris->direction = DIRECTION_S;
        harris->defaultAnimationMode = NPC_ANIMATION_IDLE;
        harris->startingCoordinates.x = -144.0f;
        harris->flags |= NPC_ACTIVE;

        setSpecialDialogueBit(HARRIS_AT_TAVERN_DIALOGUE);
        
    }

    harris->animationMode = harris->defaultAnimationMode;

}

void setGrayLocation(void) {
    npcInfo *gray = &npcs[GRAY];


    gray->wanderRadiusX = 64;
    gray->wanderRadiusZ = 64;
    gray->idleAnimation = 0;
    gray->movingAnimation = 8;

    switch (gDayOfWeek) {

        case SUNDAY:
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
        case FRIDAY:
        case SATURDAY:

            if (7 < gHour && gHour < 12) {

                gray->levelIndex = RANCH_BARN;
                gray->startingCoordinates.y = 0.0f;
                gray->direction = DIRECTION_S;
                gray->defaultAnimationMode = NPC_ANIMATION_WANDER;
                gray->startingCoordinates.x = -128.0f;
                gray->startingCoordinates.z = -48.0f;
                gray->flags |= NPC_ACTIVE;
            
            }

            if (11 < gHour && gHour < 18) {

                if (gWeather == SUNNY) {
                    
                    gray->levelIndex = RANCH;
                    gray->startingCoordinates.y = 0.0f;
                    gray->direction = DIRECTION_S;
                    gray->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    gray->startingCoordinates.x = -160.0f;
                    gray->startingCoordinates.z = 128.0f;
                    gray->flags |= NPC_ACTIVE;
                                    
                } else {
                    
                    gray->levelIndex = RANCH_BARN;
                    gray->startingCoordinates.x = 0.0f;
                    gray->startingCoordinates.y = 0.0f;
                    gray->startingCoordinates.z = 0.0f;
                    gray->direction = DIRECTION_S;
                    gray->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                    gray->flags |= NPC_ACTIVE;
                    
                }
                
            }

            break;

        case THURSDAY:

            if (gWeather == SUNNY) {

                 if (7 < gHour && gHour < 13) {

                    gray->levelIndex = MOUNTAIN_2;
                    gray->startingCoordinates.y = 0.0f;
                    gray->direction = DIRECTION_E;
                    gray->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    gray->startingCoordinates.x = -128.0f;
                    gray->startingCoordinates.z = 352.0f;
                    gray->flags |= NPC_ACTIVE;

                 }
                
            } else if (7 < gHour && gHour < 18) {

                gray->levelIndex = RANCH_HOUSE;
                gray->startingCoordinates.y = 0.0f;
                gray->direction = DIRECTION_S;
                gray->defaultAnimationMode = NPC_ANIMATION_WANDER;
                gray->startingCoordinates.x = -64.0f;
                gray->startingCoordinates.z = -64.0f;
                gray->flags |= NPC_ACTIVE;
                
            }
        
    }

    if (!checkLifeEventBit(POPURI_GRAY_MARRIED) && 17 < gHour && gHour < 24 && gray->location < 2) {

        gray->levelIndex = TAVERN;
        gray->startingCoordinates.y = 0.0f;
        gray->startingCoordinates.z = 0.0f;
        gray->direction = DIRECTION_S;
        gray->defaultAnimationMode = NPC_ANIMATION_IDLE;
        gray->startingCoordinates.x = 80.0f;
        gray->flags |= NPC_ACTIVE;

        setSpecialDialogueBit(GRAY_AT_TAVERN_DIALOGUE);
        
    }

    gray->animationMode = gray->defaultAnimationMode;
    
}

void setJeffLocation(void) {
    npcInfo *jeff = &npcs[JEFF];


    jeff->wanderRadiusX = 64;
    jeff->wanderRadiusZ = 64;
    jeff->idleAnimation = 0;
    jeff->movingAnimation = 8;

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        switch (gDayOfWeek) {       

            case TUESDAY ... SATURDAY:
            case SUNDAY:
                
                if (8 < gHour && gHour < 17) {
                    
                    jeff->levelIndex = BAKERY;
                    jeff->startingCoordinates.y = 0.0f;
                    jeff->direction = DIRECTION_S;
                    jeff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    jeff->startingCoordinates.x = -96.0f;
                    jeff->startingCoordinates.z = -96.0f;
                    jeff->flags |= NPC_ACTIVE;
                    
                }
                
                break;

            case MONDAY:               

                switch (gSeason) {
                    
                    case SPRING:
                    case SUMMER:

                        if (7 < gHour && gHour < 17 && gWeather == SUNNY) {

                            jeff->levelIndex = MOUNTAIN_1;
                            jeff->startingCoordinates.y = 0.0f;
                            jeff->direction = DIRECTION_W;
                            jeff->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                            jeff->startingCoordinates.x = -224.0f;
                            jeff->startingCoordinates.z = -128.0f;
                            jeff->flags |= NPC_ACTIVE;
                    
                        }

                        break;

                    case AUTUMN:
                        
                        if (7 < gHour && gHour < 15 && gWeather == SUNNY) {

                            jeff->levelIndex = MOUNTAIN_1;
                            jeff->startingCoordinates.y = 0.0f;
                            jeff->direction = DIRECTION_N;
                            jeff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            jeff->startingCoordinates.x = 192.0f;
                            jeff->startingCoordinates.z = 48.0f;
                            jeff->flags |= NPC_ACTIVE;
                    
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
                    
                    jeff->levelIndex = BAKERY;
                    jeff->startingCoordinates.y = 0.0f;
                    jeff->direction = DIRECTION_S;
                    jeff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    jeff->startingCoordinates.x = -96.0f;
                    jeff->startingCoordinates.z = -96.0f;
                    jeff->flags |= NPC_ACTIVE;
                    
                }

                break;
            
            case MONDAY:         
        
                if (7 < gHour && gHour < 17 && gWeather == SUNNY) {

                    if (jeff->location < 2) {
                        
                        jeff->levelIndex = MOUNTAIN_1;
                        jeff->startingCoordinates.y = 0.0f;
                        jeff->direction = DIRECTION_W;
                        jeff->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                        jeff->startingCoordinates.x = -224.0f;
                        jeff->startingCoordinates.z = -128.0f;
                        jeff->flags |= NPC_ACTIVE;
                            
                    }
            
                }

                break;

        }

        if (17 < gHour && gHour < 24 && jeff->location < 2) {
            
            jeff->levelIndex = TAVERN;
            jeff->startingCoordinates.y = 0.0f;
            jeff->direction = DIRECTION_N;
            jeff->defaultAnimationMode = NPC_ANIMATION_IDLE;
            jeff->startingCoordinates.x = -160.0f;
            jeff->startingCoordinates.z = -64.0f;
            jeff->flags |= NPC_ACTIVE;

            setSpecialDialogueBit(JEFF_AT_TAVERN_DIALOGUE);
    
        }
        
    }

    jeff->animationMode = jeff->defaultAnimationMode;
    
}

void setCliffLocation(void) {
    npcInfo *cliff = &npcs[CLIFF];


    cliff->wanderRadiusX = 64;
    cliff->wanderRadiusZ = 64;
    cliff->idleAnimation = 0;
    cliff->movingAnimation = 8;

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

                            cliff->levelIndex = RANCH;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_E;
                            cliff->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            cliff->startingCoordinates.x = 64.0f;
                            cliff->startingCoordinates.z = 128.0f;
                            cliff->flags |= NPC_ACTIVE;

                        }
                        
                    } else if (7 < gHour && gHour < 17) {

                        cliff->levelIndex = RANCH_BARN;
                        cliff->startingCoordinates.x = 0.0f;
                        cliff->startingCoordinates.y = 0.0f;
                        cliff->startingCoordinates.z = -256.0f;
                        cliff->direction = DIRECTION_S;
                        cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        cliff->flags |= NPC_ACTIVE;
                    
                    }
                    
                    break;

                case THURSDAY:          
                    
                    if (gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {

                            if (cliff->location < 2) {

                                cliff->levelIndex = MOUNTAIN_2;
                                cliff->startingCoordinates.y = 0.0f;
                                cliff->direction = DIRECTION_SE;
                                cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                cliff->startingCoordinates.x = -224.0f;
                                cliff->startingCoordinates.z = 160.0f;
                                cliff->flags |= NPC_ACTIVE;
                                
                            } else {
                                
                                cliff->levelIndex = CAVE;
                                cliff->startingCoordinates.y = 0.0f;
                                cliff->direction = DIRECTION_E;
                                cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                cliff->startingCoordinates.x = -256.0f;
                                cliff->startingCoordinates.z = -128.0f;
                                cliff->flags |= NPC_ACTIVE;
                                
                            }
                        
                        }
                    
                    } else {

                        if (7 < gHour && gHour < 17) {
                            
                            cliff->levelIndex = CARPENTER_HUT;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_NE;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = -144.0f;
                            cliff->startingCoordinates.z = 80.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }
                        
                    }

                    break;

            }
            
        } else if (checkLifeEventBit(CLIFF_ARRIVED) && !checkLifeEventBit(CLIFF_LEAVING)) {

            if (gWeather == SUNNY) {
                
                switch (gDayOfWeek) {    
                    
                    case SUNDAY:                             

                        if (cliff->location < 2 && 8 < gHour && gHour < 17) {
                            
                            cliff->levelIndex = MOUNTAIN_2;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_SE;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = -224.0f;
                            cliff->startingCoordinates.z = 160.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case MONDAY:                 

                        if (8 < gHour && gHour < 17) {

                            cliff->levelIndex = TOP_OF_MOUNTAIN_1;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->startingCoordinates.z = 0.0f;
                            cliff->direction = DIRECTION_W;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = 192.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }

                        break;
                    
                    case TUESDAY:                             

                        if (8 < gHour && gHour < 17) {

                            cliff->levelIndex = BEACH;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->startingCoordinates.z = 0.0f;
                            cliff->direction = DIRECTION_E;
                            cliff->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            cliff->startingCoordinates.x = -224.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case WEDNESDAY:                             

                        if (8 < gHour && gHour < 17) {

                            cliff->levelIndex = RANCH;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_E;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = -32.0f;
                            cliff->startingCoordinates.z = -96.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case THURSDAY:                             

                        if (8 < gHour && gHour < 17) {

                            cliff->levelIndex = MOUNTAIN_2;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_W;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = 64.0f;
                            cliff->startingCoordinates.z = -160.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }
                        
                        break;
                    
                    case FRIDAY:                             
                    case SATURDAY:      

                        if (cliff->location < 2 && 8 < gHour && gHour < 17) {
                            
                            cliff->levelIndex = MOUNTAIN_1;
                            cliff->startingCoordinates.x = 0.0f;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->startingCoordinates.z = 0.0f;
                            cliff->direction = DIRECTION_S;
                            cliff->defaultAnimationMode = NPC_ANIMATION_WANDER;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }

                        break;
                    
                }
            
            } else if (8 < gHour && gHour < 17) {

                if (cliff->location < 2) {

                    cliff->levelIndex = CAVE;
                    cliff->startingCoordinates.y = 0.0f;
                    cliff->direction = DIRECTION_E;
                    cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    cliff->startingCoordinates.x = -256.0f;
                    cliff->startingCoordinates.z = -128.0f;
                    cliff->flags |= NPC_ACTIVE;
                    
                } else {

                    cliff->levelIndex = CARPENTER_HUT;
                    cliff->startingCoordinates.y = 0.0f;
                    cliff->direction = DIRECTION_NE;
                    cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    cliff->startingCoordinates.x = -144.0f;
                    cliff->startingCoordinates.z = 80.0f;
                    cliff->flags |= NPC_ACTIVE;
                
                }
                
            }

            switch (cliff->location) {

                case 0:
                    
                    if (gWeather == SUNNY) {
    
                        if (16 < gHour && gHour < 24) {
                            
                            cliff->levelIndex = MOUNTAIN_1;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_E;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = 96.0f;
                            cliff->startingCoordinates.z = -16.0f;
                            cliff->flags |= NPC_ACTIVE;
                            
                        }
                    
                    }
                        
                    break;

                case 1:
                case 2:

                    if NIGHTTIME {

                        if (cliff->location < 2) {
                            
                            cliff->levelIndex = TAVERN;
                            cliff->startingCoordinates.y = 0.0f;
                            cliff->direction = DIRECTION_N;
                            cliff->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            cliff->startingCoordinates.x = 80.0f;
                            cliff->startingCoordinates.z = -64.0f;
                            cliff->flags |= NPC_ACTIVE;
            
                            setSpecialDialogueBit(CLIFF_AT_TAVERN_DIALOGUE);
                        
                        }
                        
                    }

                    break;
                
            }
            
        }
        
         cliff->animationMode = cliff->defaultAnimationMode;
        
    } 
    
}

void setKaiLocation(void) {
    npcInfo *kai = &npcs[KAI];


    kai->wanderRadiusX = 64;
    kai->wanderRadiusZ = 64;
    kai->idleAnimation = 0;
    kai->movingAnimation = 8;

    if (!checkLifeEventBit(KAI_GONE) && !checkDailyEventBit(KAREN_YELLOW_HEART_EVENT_DAILY)) {

        if (checkLifeEventBit(KAREN_KAI_MARRIED)) {

            if (gWeather == SUNNY) {

                if (gSeason != WINTER) {

                    if (8 < gHour && gHour < 17) {
                        
                        kai->levelIndex = VINEYARD;
                        kai->startingCoordinates.x = 0.0f;
                        kai->startingCoordinates.y = 0.0f;
                        kai->startingCoordinates.z = -64.0f;
                        kai->direction = DIRECTION_S;
                        kai->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        kai->flags |= NPC_ACTIVE;

                    }
                    
                } else {

                    if (8 < gHour && gHour < 17) {
                        
                        kai->levelIndex = VINEYARD_CELLAR_BASEMENT;
                        kai->startingCoordinates.y = 0;
                        kai->direction = DIRECTION_S;
                        kai->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        kai->startingCoordinates.x = -32.0f;
                        kai->startingCoordinates.z = -64.0f;
                        kai->flags |= NPC_ACTIVE;

                    }

                }

                 if (gDayOfWeek == SUNDAY && 9 < gHour && gHour < 18) {

                    kai->levelIndex = BEACH;
                    kai->startingCoordinates.y = 0;
                    kai->direction = DIRECTION_E;
                    kai->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    kai->startingCoordinates.x = -224.0f;
                    kai->startingCoordinates.z = -32.0f;
                    kai->flags |= NPC_ACTIVE;
                     
                 }
                
                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11  && 8 < gHour && gHour < 17) {

                    kai->levelIndex = VINEYARD;
                    kai->startingCoordinates.x = -224.0f;
                    kai->startingCoordinates.y = 0.0f;
                    kai->startingCoordinates.z = 0.0f;
                    kai->direction = DIRECTION_S;
                    kai->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    kai->flags |= NPC_ACTIVE;
                    
                }
                
            } else {

                if (8 < gHour && gHour < 17) {
                    kai->levelIndex = VINEYARD_CELLAR;
                    kai->startingCoordinates.y = 0.0f;
                    kai->direction = DIRECTION_E;
                    kai->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    kai->startingCoordinates.x = -64.0f;
                    kai->startingCoordinates.z = -64.0f;
                    kai->flags |= NPC_ACTIVE;
                    
                }

            }
            
        } else {

            if (gWeather == SUNNY) {
                
                if (gSeason != WINTER) {

                    if (8 < gHour && gHour < 17) {
                    
                        kai->levelIndex = VINEYARD;
                        kai->startingCoordinates.x = 0.0f;
                        kai->startingCoordinates.y = 0;
                        kai->startingCoordinates.z = -64.0f;
                        kai->direction = 0;
                        kai->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        kai->flags |= NPC_ACTIVE;
                        
                    }

                } else {

                    if (8 < gHour && gHour < 17) {
                    
                        kai->levelIndex = VINEYARD_CELLAR_BASEMENT;
                        kai->startingCoordinates.y = 0;
                        kai->direction = DIRECTION_S;
                        kai->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        kai->startingCoordinates.x = -32.0f;
                        kai->startingCoordinates.z = -64.0f;
                        kai->flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                    
            } else {

                if (8 < gHour && gHour < 17) {
                    
                    kai->levelIndex = VINEYARD_CELLAR;
                    kai->startingCoordinates.y = 0;
                    kai->direction = DIRECTION_E;
                    kai->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    kai->startingCoordinates.x = -64.0f;
                    kai->startingCoordinates.z = -64.0f;
                    kai->flags |= NPC_ACTIVE;
                    
                }
                
            }

            if (17 < gHour && gHour < 24 && kai->location < 2) {

                    kai->levelIndex = TAVERN;
                    kai->startingCoordinates.y = 0.0f;
                    kai->direction = DIRECTION_N;
                    kai->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    kai->startingCoordinates.x = -144.0f;
                    kai->startingCoordinates.z = 96.0f;
                    kai->flags |= NPC_ACTIVE;

                    setSpecialDialogueBit(KAI_AT_TAVERN_DIALOGUE);
                
            }
            
        }
        
        kai->animationMode = kai->defaultAnimationMode;
        
    }
    
}

void setMayorLocation(void) {
    npcInfo *mayor = &npcs[MAYOR];


    mayor->wanderRadiusX = 64;
    mayor->wanderRadiusZ = 64;
    mayor->idleAnimation = 0;
    mayor->movingAnimation = 8;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case SUNDAY:

                if (8 < gHour && gHour < 15) {
                    mayor->levelIndex = CHURCH;
                    mayor->startingCoordinates.y = 0;
                    mayor->direction = DIRECTION_N;
                    mayor->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    mayor->startingCoordinates.x = -80.0f;
                    mayor->startingCoordinates.z = -32.0f;
                    mayor->flags |= NPC_ACTIVE;
                }

                break;
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (mayor->location < 2) {
                    
                    if (7 < gHour && gHour < 17) {
                        mayor->levelIndex = VILLAGE_2;
                        mayor->startingCoordinates.y = 0;
                        mayor->direction = DIRECTION_S;
                        mayor->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        mayor->startingCoordinates.x = -224.0f;
                        mayor->startingCoordinates.z = 32.0f;
                        mayor->flags |= NPC_ACTIVE;
                    }
                    
                } else {

                    if (7 < gHour && gHour < 17) {
                        mayor->levelIndex = VILLAGE_1;
                        mayor->startingCoordinates.y = 0;
                        mayor->direction = DIRECTION_E;
                        mayor->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        mayor->startingCoordinates.x = -128.0f;
                        mayor->startingCoordinates.z = -64.0f;
                        mayor->flags |= NPC_ACTIVE;
                    }
                    
                }
                
                break;
            
            case SATURDAY:

                if (mayor->location < 2) {
                    if (7 < gHour && gHour < 17) {
                        mayor->levelIndex = VILLAGE_1;
                        mayor->startingCoordinates.y = 0;
                        mayor->direction = DIRECTION_S;
                        mayor->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        mayor->startingCoordinates.x = -224.0f;
                        mayor->startingCoordinates.z = 288.0f;
                        mayor->flags |= NPC_ACTIVE;
                    }
                } else {

                    if (7 < gHour && gHour < 17) {
                        mayor->levelIndex = LIBRARY;
                        mayor->startingCoordinates.y = 0;
                        mayor->direction = DIRECTION_S;
                        mayor->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        mayor->startingCoordinates.x = 64.0f;
                        mayor->startingCoordinates.z = 64.0f;
                        mayor->flags |= NPC_ACTIVE;
                    }
                    
                }

                break;
            
        }
        
    } else if (8 < gHour && gHour < 15) {
        mayor->levelIndex = MAYOR_HOUSE;
        mayor->startingCoordinates.y = 0;
        mayor->direction = DIRECTION_S;
        mayor->defaultAnimationMode = NPC_ANIMATION_IDLE;
        mayor->startingCoordinates.x = 16.0f;
        mayor->startingCoordinates.z = -32.0f;
        mayor->flags |= NPC_ACTIVE;
    }

    mayor->animationMode = mayor->defaultAnimationMode;
    
}

void setMayorWifeLocation(void) {
    npcInfo *mayorWife = &npcs[MAYOR_WIFE];


    mayorWife->wanderRadiusX = 64;
    mayorWife->wanderRadiusZ = 64;
    mayorWife->idleAnimation = 0;
    mayorWife->movingAnimation = 8;

    if (checkLifeEventBit(MARRIED) && gWife == MARIA) {

        switch (gDayOfWeek) {
            case SUNDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
                
                if (8 < gHour && gHour < 17) {
                    mayorWife->levelIndex = LIBRARY;
                    mayorWife->startingCoordinates.y = 0;
                    mayorWife->direction = 0;
                    mayorWife->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    mayorWife->startingCoordinates.x = 48.0f;
                    mayorWife->startingCoordinates.z = -96.0f;
                    mayorWife->flags |= NPC_ACTIVE;
                }
                break;
            
            default:
                break;
            
        }
        
    } else if (gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case SUNDAY:
                if (8 < gHour && gHour < 15) {
                    mayorWife->levelIndex = CHURCH;
                    mayorWife->startingCoordinates.y = 0;
                    mayorWife->direction = DIRECTION_N;
                    mayorWife->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    mayorWife->startingCoordinates.x = -112.0f;
                    mayorWife->startingCoordinates.z = -32.0f;
                    mayorWife->flags |= NPC_ACTIVE;
                }

                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
                
                if (7 < gHour && gHour < 17) {
                    mayorWife->levelIndex = MAYOR_HOUSE;
                    mayorWife->startingCoordinates.y = 0;
                    mayorWife->direction = DIRECTION_S;
                    mayorWife->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    mayorWife->startingCoordinates.x = -64.0f;
                    mayorWife->startingCoordinates.z = -64.0f;
                    mayorWife->flags |= NPC_ACTIVE;
                }
                
                break;

            case FRIDAY:

                if (11 < gHour && gHour < 17) {
                    mayorWife->levelIndex = BAKERY;
                    mayorWife->startingCoordinates.y = 0;
                    mayorWife->direction = DIRECTION_S;
                    mayorWife->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    mayorWife->startingCoordinates.x = 64.0f;
                    mayorWife->startingCoordinates.z = -32.0f;
                    mayorWife->flags |= NPC_ACTIVE;
                }
                
                break;

            case SATURDAY:
            
                if (7 < gHour && gHour < 17) {
                    mayorWife->levelIndex = MAYOR_HOUSE;
                    mayorWife->startingCoordinates.y = 0;
                    mayorWife->direction = DIRECTION_S;
                    mayorWife->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    mayorWife->startingCoordinates.x = -64.0f;
                    mayorWife->startingCoordinates.z = -64.0f;
                    mayorWife->flags |= NPC_ACTIVE;
                }
                
                break;
            
        }
        
    } else if (7 < gHour && gHour < 17) {
        mayorWife->levelIndex = MAYOR_HOUSE;
        mayorWife->startingCoordinates.y = 0;
        mayorWife->direction = DIRECTION_W;
        mayorWife->defaultAnimationMode = NPC_ANIMATION_IDLE;
        mayorWife->startingCoordinates.x = 64.0f;
        mayorWife->startingCoordinates.z = 16.0f;
        mayorWife->flags |= NPC_ACTIVE;
    }
    
    mayorWife->animationMode = mayorWife->defaultAnimationMode;
    
}

void setLilliaLocation(void) {
    npcInfo *lillia = &npcs[LILLIA];


    lillia->wanderRadiusX = 64;
    lillia->wanderRadiusZ = 64;
    lillia->movingAnimation = 8;
    lillia->idleAnimation = 0;

    if (gWeather != SUNNY) goto DEFAULT;

    switch (gDayOfWeek) {

        case SUNDAY:

            if (8 < gHour && gHour < 17) { 
                lillia->levelIndex = SQUARE;
                lillia->startingCoordinates.y = 0;
                lillia->direction = DIRECTION_N;
                lillia->defaultAnimationMode = NPC_ANIMATION_IDLE;
                lillia->startingCoordinates.x = -144.0f;
                lillia->startingCoordinates.z = 256.0f;
                lillia->flags |= NPC_ACTIVE;
            }
            break;

        default:
            break;

        case MONDAY ... SATURDAY:
DEFAULT:
            if (8 < gHour && gHour < 17) {
                lillia->levelIndex = FLOWER_SHOP;
                lillia->startingCoordinates.y = 0;
                lillia->direction = DIRECTION_S;
                lillia->defaultAnimationMode = NPC_ANIMATION_IDLE;
                lillia->startingCoordinates.x = 64.0f;
                lillia->startingCoordinates.z = -128.0f;
                lillia->flags |= NPC_ACTIVE;
            }
    
            break;

    }
    
    lillia->animationMode = lillia->defaultAnimationMode;

}

void setBasilLocation(void) {
    npcInfo *basil = &npcs[BASIL];


    basil->wanderRadiusX = 64;
    basil->wanderRadiusZ = 64;
    basil->idleAnimation = 0;
    basil->movingAnimation = 8;

    if (!checkLifeEventBit(BASIL_IN_TOWN)) goto FUNC_END;

    if (gWeather != SUNNY) goto NOT_SUNNY;
            
    switch (gDayOfWeek) {
        case SUNDAY:
            if (8 < gHour && gHour < 17) {
                basil->levelIndex = SQUARE;
                basil->startingCoordinates.y = 0;
                basil->direction = DIRECTION_N;
                basil->defaultAnimationMode = NPC_ANIMATION_IDLE;
                basil->startingCoordinates.x = -176.0f;
                basil->startingCoordinates.z = 256.0f;
                basil->flags |= NPC_ACTIVE;
            }
            break;
        case THURSDAY:
            if (7  < gHour && gHour < 18) {
                basil->levelIndex = VILLAGE_1;
                basil->startingCoordinates.y = 0;
                basil->direction = DIRECTION_E;
                basil->defaultAnimationMode = NPC_ANIMATION_IDLE;
                basil->startingCoordinates.x = 128.0f;
                basil->startingCoordinates.z = -416.0f;
                basil->flags |= NPC_ACTIVE;
            }
            break;
        case FRIDAY:
        case SATURDAY:
            if (7  < gHour && gHour < 18) {
                basil->levelIndex = MOUNTAIN_1;
                basil->startingCoordinates.y = 0;
                basil->direction = DIRECTION_N;
                basil->defaultAnimationMode = NPC_ANIMATION_WANDER;
                basil->startingCoordinates.x = 192.0f;
                basil->startingCoordinates.z = -64.0f;
                basil->flags |= NPC_ACTIVE;
            }
            break;
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
NOT_SUNNY:
            if (8 < gHour && gHour < 17) {
                basil->levelIndex = FLOWER_SHOP;
                basil->startingCoordinates.y = 0;
                basil->direction = DIRECTION_S;
                basil->defaultAnimationMode = NPC_ANIMATION_IDLE;
                basil->startingCoordinates.x = -48.0f;
                basil->startingCoordinates.z = -64.0f;
                basil->flags |= NPC_ACTIVE;
            }
            break;
        default:
            break;
    }
        
    if (NIGHTTIME && basil->location < 2) {
        basil->levelIndex = 0x3B;
        basil->startingCoordinates.y = 0;
        basil->direction = DIRECTION_E;
        basil->defaultAnimationMode = NPC_ANIMATION_IDLE;
        basil->startingCoordinates.x = 32.0f;
        basil->startingCoordinates.z = 48.0f;
        basil->flags |= NPC_ACTIVE;
    }

FUNC_END:
    basil->animationMode = basil->defaultAnimationMode;
    
}

void setEllenLocation(void) {
    npcInfo *ellen = &npcs[ELLEN];


    ellen->wanderRadiusX = 64;
    ellen->wanderRadiusZ = 64;
    ellen->idleAnimation = 0;
    ellen->movingAnimation = 8;
    
    if (!checkLifeEventBit(ELLEN_DIED)) {

        if (gWeather == SUNNY && (7 < gHour && gHour < 17)) {

                ellen->levelIndex = VILLAGE_1;
                ellen->startingCoordinates.y = 0;
                ellen->direction = DIRECTION_S;
                ellen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                ellen->startingCoordinates.x = 320.0f;
                ellen->startingCoordinates.z = 128.0f;
                ellen->flags |= NPC_ACTIVE;

        } else {

            if (7 < gHour && gHour < 17) {

                ellen->levelIndex = BAKERY;
                ellen->startingCoordinates.y = 0;
                ellen->direction = DIRECTION_S;
                ellen->defaultAnimationMode = NPC_ANIMATION_IDLE;
                ellen->startingCoordinates.x = -128.0f;
                ellen->startingCoordinates.z = -32.0f;
                ellen->flags |= NPC_ACTIVE;

            }

        }

    }
    
    ellen->animationMode = ellen->defaultAnimationMode;

}

void setDougLocation(void) {
    npcInfo *doug = &npcs[DOUG];


    int temp = gDayOfWeek;
    int temp2;

    doug->wanderRadiusX = 64;
    doug->wanderRadiusZ = 64;
    doug->idleAnimation = 0;
    doug->movingAnimation = 8;

    // FIXME: something off here
    if (temp >= SUNDAY && (temp < THURSDAY || temp < 7 && (temp2 = temp) >= FRIDAY) && (7 < gHour && gHour < 17)) {
        doug->levelIndex = RANCH_STORE;
        doug->startingCoordinates.y = 0;
        doug->direction = DIRECTION_E;
        doug->defaultAnimationMode = NPC_ANIMATION_IDLE;
        doug->startingCoordinates.x = -96.0f;
        doug->startingCoordinates.z = 64.0f;
        doug->flags |= NPC_ACTIVE;
    }

    if (NIGHTTIME && doug->location < 2) {
        doug->levelIndex = TAVERN;
        doug->startingCoordinates.y = 0;
        doug->direction = DIRECTION_W;
        doug->defaultAnimationMode = NPC_ANIMATION_IDLE;
        doug->startingCoordinates.x = -96.0f;
        doug->startingCoordinates.z = 48.0f;
        doug->flags |= NPC_ACTIVE;
    }

    doug->animationMode = doug->defaultAnimationMode;
    
}

void setGotzLocation(void) {
    npcInfo *gotz = &npcs[GOTZ];


    gotz->wanderRadiusX = 64;
    gotz->wanderRadiusZ = 64;
    gotz->idleAnimation = 0;
    gotz->movingAnimation = 8;

    if (gWeather == SUNNY) {

        switch (gSeason) {

            case SPRING:
            case SUMMER:
            case AUTUMN:

                if (8 < gHour && gHour < 17) {

                    gotz->levelIndex = VINEYARD;
                    gotz->startingCoordinates.y = 0.0f;
                    gotz->direction = DIRECTION_S;
                    gotz->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    gotz->startingCoordinates.x = 128.0f;
                    gotz->startingCoordinates.z = -128.0f;
                    gotz->flags |= NPC_ACTIVE;
                    
                }

                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

                    if (8 < gHour && gHour < 17) {
    
                        gotz->levelIndex = VINEYARD;
                        gotz->startingCoordinates.y = 0.0f;
                        gotz->direction = DIRECTION_S;
                        gotz->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
                        gotz->startingCoordinates.x = -192.0f;
                        gotz->startingCoordinates.z = -64.0f;
                        gotz->flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                
                break;

            case WINTER:
                
                if (8 < gHour && gHour < 17) {

                    gotz->levelIndex = VINEYARD_HOUSE;
                    gotz->startingCoordinates.y = 0.0f;
                    gotz->direction = DIRECTION_SE;
                    gotz->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    gotz->startingCoordinates.x = -160.0f;
                    gotz->startingCoordinates.z = -32.0f;
                    gotz->flags |= NPC_ACTIVE;
                    
                }
                
                break;
            
        }
        
    } else {

        if (8 < gHour && gHour < 17) {

            gotz->levelIndex = VINEYARD_HOUSE;
            gotz->startingCoordinates.y = 0.0f;
            gotz->direction = DIRECTION_SE;
            gotz->defaultAnimationMode = NPC_ANIMATION_IDLE;
            gotz->startingCoordinates.x = -160.0f;
            gotz->startingCoordinates.z = -32.0f;
            gotz->flags |= NPC_ACTIVE;
            
        }
        
    } 

    if NIGHTTIME {

        if (gotz->location < 2) {
            
            gotz->levelIndex = TAVERN;
            gotz->startingCoordinates.y = 0.0f;
            gotz->direction = DIRECTION_S;
            gotz->defaultAnimationMode = NPC_ANIMATION_IDLE;
            gotz->startingCoordinates.x = 80.0f;
            gotz->startingCoordinates.z = -160.0f;
            gotz->flags |= NPC_ACTIVE;

}
            
    }
        
    gotz->animationMode = gotz->defaultAnimationMode;
    
}

void setSashaLocation(void) {
    npcInfo *sasha = &npcs[SASHA];


    sasha->wanderRadiusX = 64;
    sasha->wanderRadiusZ = 64;
    sasha->idleAnimation = 0;
    sasha->movingAnimation = 8;

    if (7 < gHour && gHour < 17) {
        
        sasha->levelIndex = VINEYARD_HOUSE;
        sasha->startingCoordinates.y = 0.0f;
        sasha->direction = DIRECTION_S;
        sasha->defaultAnimationMode = NPC_ANIMATION_IDLE;
        sasha->startingCoordinates.x = -128.0f;
        sasha->startingCoordinates.z = -96.0f;
        sasha->flags |= NPC_ACTIVE;
        
    }

    if (gWeather == SUNNY && gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

        if (8 < gHour && gHour < 17) {
        
            sasha->levelIndex = VINEYARD;
            sasha->startingCoordinates.y = 0.0f;
            sasha->direction = DIRECTION_S;
            sasha->defaultAnimationMode = NPC_ANIMATION_CUSTOM;
            sasha->startingCoordinates.x = -192.0f;
            sasha->startingCoordinates.z = 64.0f;
            sasha->flags |= NPC_ACTIVE;
            
        }
        
    }
    
    sasha->animationMode = sasha->defaultAnimationMode;
    
}

void setPotionShopDealerLocation(void) {
    npcInfo *potionShopDealer = &npcs[POTION_SHOP_DEALER];


    potionShopDealer->wanderRadiusX = 64;
    potionShopDealer->movingAnimation = 8;
    potionShopDealer->wanderRadiusZ = 64;
    potionShopDealer->idleAnimation = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
                
                if (8 < gHour && gHour < 17) {

                    potionShopDealer->levelIndex = POTION_SHOP;
                    potionShopDealer->startingCoordinates.y = 0.0f;
                    potionShopDealer->direction = DIRECTION_E;
                    potionShopDealer->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    potionShopDealer->startingCoordinates.x = -96.0f;
                    potionShopDealer->startingCoordinates.z = -16.0f;
                    potionShopDealer->flags |= NPC_ACTIVE;
                        
                    }
                
                break;
            
            case SATURDAY:
            case SUNDAY:

                if (potionShopDealer->location < 2) {

                    if (5 < gHour && gHour < 16) {
            
                        potionShopDealer->levelIndex = MOUNTAIN_2;
                        potionShopDealer->startingCoordinates.y = 0.0f;
                        potionShopDealer->direction = DIRECTION_SE;
                        potionShopDealer->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        potionShopDealer->startingCoordinates.x = -112.0f;
                        potionShopDealer->startingCoordinates.z = 192.0f;
                        potionShopDealer->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else if (8 < gHour && gHour < 17) {
            
                    potionShopDealer->levelIndex = CAVE;
                    potionShopDealer->startingCoordinates.y = 0.0f;
                    potionShopDealer->direction = DIRECTION_SE;
                    potionShopDealer->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    potionShopDealer->startingCoordinates.x = -288.0f;
                    potionShopDealer->startingCoordinates.z = -224.0f;
                    potionShopDealer->flags |= NPC_ACTIVE;
                    
                }
    
                break;
            
        }
        
    } else if (8 < gHour && gHour < 17) {

        potionShopDealer->levelIndex = POTION_SHOP;
        potionShopDealer->startingCoordinates.y = 0.0f;
        potionShopDealer->direction = DIRECTION_E;
        potionShopDealer->defaultAnimationMode = NPC_ANIMATION_IDLE;
        potionShopDealer->startingCoordinates.x = -96.0f;
        potionShopDealer->startingCoordinates.z = -16.0f;
        potionShopDealer->flags |= NPC_ACTIVE;
            
    }
        
    potionShopDealer->animationMode = potionShopDealer->defaultAnimationMode;
    
}

void setKentLocation(void) {
    npcInfo *kent = &npcs[KENT];


    kent->wanderRadiusX = 64;
    kent->wanderRadiusZ = 64;
    kent->movingAnimation = 8;
    kent->idleAnimation = 0;

    if (gWeather == SUNNY) {
        
        switch (gDayOfWeek) {

            case SUNDAY:

                switch (gSeason) {

                    case SPRING:
                    case AUTUMN:

                        switch (kent->location) {

                            case 0:
                            case 1:

                                if (7 < gHour && gHour < 16) {
                
                                    kent->levelIndex = MOUNTAIN_2;
                                    kent->startingCoordinates.y = 0.0f;
                                    kent->direction = DIRECTION_S;
                                    kent->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                    kent->startingCoordinates.x = -160.0f;
                                    kent->startingCoordinates.z = -160.0f;
                                    kent->flags |= NPC_ACTIVE;
                                    
                                    setSpecialDialogueBit(KENT_AT_MOUNTAIN_DIALOGUE);
                                    
                                }
                                
                                break;

                            case 2:

                                if (7 < gHour && gHour < 16) {
                
                                    kent->levelIndex = LIBRARY;
                                    kent->startingCoordinates.y = 0.0f;
                                    kent->startingCoordinates.z = 0.0f;
                                    kent->direction = DIRECTION_E;
                                    kent->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                    kent->startingCoordinates.x = -32.0f;
                                    kent->flags |= NPC_ACTIVE;
                                    
                                }
           
                                break;

                            case 3:

                                if (7 < gHour && gHour < 16) {
                
                                    kent->levelIndex = VILLAGE_2;
                                    kent->startingCoordinates.y = 0.0f;
                                    kent->startingCoordinates.z = 0.0f;
                                    kent->direction = DIRECTION_SE;
                                    kent->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    kent->startingCoordinates.x = 288.0f;
                                    kent->flags |= NPC_ACTIVE;
                                    
                                }

                                break;
                            
                        }
                        
                        break;

                    case SUMMER:

                        if (kent->location < 2) {

                            if (7 < gHour && gHour < 16) {

                                kent->levelIndex = BEACH;
                                kent->startingCoordinates.y = 0.0f;
                                kent->direction = DIRECTION_E;
                                kent->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                kent->startingCoordinates.x = -224.0f;
                                kent->startingCoordinates.z = -96.0f;
                                kent->flags |= NPC_ACTIVE;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                kent->levelIndex = MOUNTAIN_2;
                                kent->startingCoordinates.y = 0.0f;
                                kent->direction = DIRECTION_S;
                                kent->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                kent->startingCoordinates.x = -128.0f;
                                kent->startingCoordinates.z = -160.0f;
                                kent->flags |= NPC_ACTIVE;

                                setSpecialDialogueBit(KENT_AT_MOUNTAIN_DIALOGUE);
                                 
                             }
                            
                        }

                        break;

                    case WINTER:

                        if (kent->location < 2) {

                            if (7 < gHour && gHour < 16) {

                                kent->levelIndex = POTION_SHOP_BEDROOM;
                                kent->startingCoordinates.y = 0.0f;
                                kent->direction = DIRECTION_S;
                                kent->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                kent->startingCoordinates.x = 32.0f;
                                kent->startingCoordinates.z = -32.0f;
                                kent->flags |= NPC_ACTIVE;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                kent->levelIndex = MOUNTAIN_2;
                                kent->startingCoordinates.y = 0.0f;
                                kent->direction = DIRECTION_S;
                                kent->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                kent->startingCoordinates.x = -128.0f;
                                kent->startingCoordinates.z = -160.0f;
                                kent->flags |= NPC_ACTIVE;

                                setSpecialDialogueBit(KENT_AT_MOUNTAIN_DIALOGUE);
                                 
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

                    kent->levelIndex = CHURCH;
                    kent->startingCoordinates.y = 0.0f;
                    kent->direction = 4;
                    kent->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    kent->startingCoordinates.x = 48.0f;
                    kent->startingCoordinates.z = -32.0f;
                    kent->flags |= NPC_ACTIVE;
                     
                 } 
                
                if (14 < gHour && gHour < 17) {
                     
                    kent->levelIndex = VILLAGE_1;
                    kent->startingCoordinates.x = 0.0f;
                    kent->startingCoordinates.y = 0.0f;
                    kent->direction = DIRECTION_W;
                    kent->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    kent->startingCoordinates.z = 352.0f;
                    kent->flags |= NPC_ACTIVE;
                             
                 } 
                
                break;

            case SATURDAY:

                if (kent->location < 2) {

                    if (8 < gHour && gHour < 15) {

                        kent->levelIndex = SQUARE;
                        kent->startingCoordinates.x = 0.0f;
                        kent->startingCoordinates.y = 0.0f;
                        kent->startingCoordinates.z = 0.0f;
                        kent->direction = DIRECTION_N;
                        kent->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                        kent->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (8 < gHour && gHour < 15) {
                                    
                        kent->levelIndex = MOUNTAIN_1;
                        kent->startingCoordinates.y = 0.0f;
                        kent->direction = DIRECTION_E;
                        kent->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        kent->startingCoordinates.x = 64.0f;
                        kent->startingCoordinates.z = -96.0f;
                        kent->flags |= NPC_ACTIVE;

                        setSpecialDialogueBit(KENT_AT_MOUNTAIN_DIALOGUE);
                        
                    }
                    
                }
                
                break;
            
        }
        
    } else if (gWeather == SNOW && kent->location >= 2) {
        
        if (7 < gHour && gHour < 17) {

            kent->levelIndex = VILLAGE_2;
            kent->startingCoordinates.y = 0.0f;
            kent->direction = DIRECTION_S;
            kent->defaultAnimationMode = NPC_ANIMATION_IDLE;
            kent->startingCoordinates.x = 412.0f;
            kent->startingCoordinates.z = -64.0f;
            kent->flags |= NPC_ACTIVE;
            
        }
        
    } else if (7 < gHour && gHour < 17) {

        kent->levelIndex = POTION_SHOP;
        kent->startingCoordinates.x = 0.0f;
        kent->startingCoordinates.y = 0.0f;
        kent->direction = DIRECTION_W;
        kent->defaultAnimationMode = NPC_ANIMATION_IDLE;
        kent->startingCoordinates.z = -16.0f;
        kent->flags |= NPC_ACTIVE;
            
    }

    kent->animationMode = kent->defaultAnimationMode;
    
}

void setStuLocation(void) {
    npcInfo *stu = &npcs[STU];

    
    stu->wanderRadiusX = 64;
    stu->wanderRadiusZ = 64;
    stu->movingAnimation = 8;
    stu->idleAnimation = 0;

    if (gWeather == SUNNY) {
        
        switch (gDayOfWeek) {

            case SUNDAY:

                switch (gSeason) {

                    case SPRING:
                    case AUTUMN:

                        switch (stu->location) {

                            case 0:
                            case 1:

                                if (7 < gHour && gHour < 16) {
                
                                    stu->levelIndex = MOUNTAIN_2;
                                    stu->startingCoordinates.y = 0.0f;
                                    stu->direction = DIRECTION_S;
                                    stu->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                    stu->startingCoordinates.x = -160.0f;
                                    stu->startingCoordinates.z = -160.0f;
                                    stu->flags |= NPC_ACTIVE;
                                    
                                }
                                
                                break;

                            case 2:

                                if (7 < gHour && gHour < 16) {
                
                                    stu->levelIndex = LIBRARY;
                                    stu->startingCoordinates.y = 0.0f;
                                    stu->direction = DIRECTION_E;
                                    stu->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                    stu->startingCoordinates.x = -64.0f;
                                    stu->startingCoordinates.z = -64.0f;
                                    stu->flags |= NPC_ACTIVE;
                                    
                                }
           
                                break;

                            case 3:

                                if (7 < gHour && gHour < 16) {
                
                                    stu->levelIndex = VILLAGE_2;
                                    stu->startingCoordinates.y = 0.0f;
                                    stu->startingCoordinates.z = 0.0f;
                                    stu->direction = DIRECTION_SE;
                                    stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                    stu->startingCoordinates.x = 256.0f;
                                    stu->flags |= NPC_ACTIVE;
                                    
                                }

                                break;
                            
                        }
                        
                        break;

                    case SUMMER:

                        if (stu->location < 2) {

                            if (7 < gHour && gHour < 16) {

                                stu->levelIndex = BEACH;
                                stu->startingCoordinates.y = 0.0f;
                                stu->direction = DIRECTION_E;
                                stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
                                stu->startingCoordinates.x = -224.0f;
                                stu->startingCoordinates.z = -128.0f;
                                stu->flags |= NPC_ACTIVE;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                stu->levelIndex = MOUNTAIN_2;
                                stu->startingCoordinates.y = 0.0f;
                                stu->direction = DIRECTION_S;
                                stu->defaultAnimationMode = NPC_ANIMATION_WANDER;
                                stu->startingCoordinates.x = -160.0f;
                                stu->startingCoordinates.z = -160.0f;
                                stu->flags |= NPC_ACTIVE;

                             }
                            
                        }

                        break;

                    case WINTER:

                        if (7 < gHour && gHour < 16) {

                            stu->levelIndex = POTION_SHOP_BEDROOM;
                            stu->startingCoordinates.x = 0.0f;
                            stu->startingCoordinates.y = 0.0f;
                            stu->direction = DIRECTION_S;
                            stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
                            stu->startingCoordinates.z = -32.0f;
                            stu->flags |= NPC_ACTIVE;
                        
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

                    stu->levelIndex = CHURCH;
                    stu->startingCoordinates.y = 0.0f;
                    stu->direction = 4;
                    stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    stu->startingCoordinates.x = 80.0f;
                    stu->startingCoordinates.z = -32.0f;
                    stu->flags |= NPC_ACTIVE;
                     
                 } 
                
                if (14 < gHour && gHour < 17) {
                     
                    stu->levelIndex = VILLAGE_1;
                    stu->startingCoordinates.y = 0.0f;
                    stu->direction = DIRECTION_W;
                    stu->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    stu->startingCoordinates.x = 32.0f;
                    stu->startingCoordinates.z = 352.0f;
                    stu->flags |= NPC_ACTIVE;
                             
                 } 
                
                break;

            case SATURDAY:

                if (stu->location < 2) {

                    if (8 < gHour && gHour < 15) {

                        stu->levelIndex = SQUARE;
                        stu->startingCoordinates.y = 0.0f;
                        stu->startingCoordinates.z = 0.0f;
                        stu->direction = DIRECTION_N;
                        stu->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        stu->startingCoordinates.x = -32.0f;
                        stu->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (8 < gHour && gHour < 15) {
                                    
                        stu->levelIndex = MOUNTAIN_1;
                        stu->startingCoordinates.y = 0.0f;
                        stu->direction = DIRECTION_E;
                        stu->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        stu->startingCoordinates.x = 64.0f;
                        stu->startingCoordinates.z = -64.0f;
                        stu->flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                
                break;
            
        }
        
    } else if (gWeather == SNOW) {

        if (stu->location < 2) {
            
            if (7 < gHour && gHour < 17) {
    
                stu->levelIndex = POTION_SHOP_BEDROOM;
                stu->startingCoordinates.x = 0.0f;
                stu->startingCoordinates.y = 0.0f;
                stu->direction = DIRECTION_S;
                stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
                stu->startingCoordinates.z = -32.0f;
                stu->flags |= NPC_ACTIVE;
                
            }
            
        } else {

            if (7 < gHour && gHour < 17) {
    
                stu->levelIndex = VILLAGE_2;
                stu->startingCoordinates.y = 0.0f;
                stu->direction = DIRECTION_S;
                stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
                stu->startingCoordinates.x = 444.0f;
                stu->startingCoordinates.z = -64.0f;
                stu->flags |= NPC_ACTIVE;
                
            }
            
        }

    } else if (7 < gHour && gHour < 17) {

        stu->levelIndex = POTION_SHOP;
        stu->startingCoordinates.x = 0.0f;
        stu->startingCoordinates.y = 0.0f;
        stu->direction = DIRECTION_W;
        stu->defaultAnimationMode = NPC_ANIMATION_IDLE;
        stu->startingCoordinates.z = -48.0f;
        stu->flags |= NPC_ACTIVE;
            
    }

    stu->animationMode = stu->defaultAnimationMode;
    
}

void setMidwifeLocation(void) {
    npcInfo *midwife = &npcs[MIDWIFE];


    midwife->wanderRadiusX = 64;
    midwife->wanderRadiusZ = 64;
    midwife->idleAnimation = 0;
    midwife->movingAnimation = 8;

    if (!checkDailyEventBit(BIRTH_EVENT_DAILY)) {

        if (gWeather == SUNNY) {

            switch (gDayOfWeek) {
            
                case SUNDAY:
    
                    if (8 < gHour && gHour < 17) {
            
                        midwife->levelIndex = VILLAGE_2;
                        midwife->startingCoordinates.y = 0.0f;
                        midwife->direction = DIRECTION_S;
                        midwife->defaultAnimationMode = NPC_ANIMATION_WANDER;
                        midwife->startingCoordinates.x = 48.0f;
                        midwife->startingCoordinates.z = -64.0f;
                        midwife->flags |= NPC_ACTIVE;
                        
                    }
                    
                    break;
                
                case MONDAY:
                case TUESDAY:
                case WEDNESDAY:
                case THURSDAY:
                case FRIDAY:
                case SATURDAY:
                    
                    if (8 < gHour && gHour < 17) {
    
                        midwife->levelIndex = MIDWIFE_HOUSE;
                        midwife->startingCoordinates.y = 0.0f;
                        midwife->direction = DIRECTION_S;
                        midwife->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        midwife->startingCoordinates.x = -16.0f;
                        midwife->startingCoordinates.z = -32.0f;
                        midwife->flags |= NPC_ACTIVE;
                        
                    }
                    
                    break;

}
            
        } else if (8 < gHour && gHour < 17) {

            midwife->levelIndex = MIDWIFE_HOUSE;
            midwife->startingCoordinates.y = 0.0f;
            midwife->direction = DIRECTION_S;
            midwife->defaultAnimationMode = NPC_ANIMATION_IDLE;
            midwife->startingCoordinates.x = -16.0f;
            midwife->startingCoordinates.z = -32.0f;
            midwife->flags |= NPC_ACTIVE;
            
        }
            
    }
    
    midwife->animationMode = midwife->defaultAnimationMode;
    
}

void setMayLocation(void) {
    npcInfo *may = &npcs[MAY];

    
    may->wanderRadiusX = 64;
    may->wanderRadiusZ = 64;
    may->movingAnimation = 8;
    may->idleAnimation = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case SUNDAY:

                if (may->location < 2) {
                    
                    if (8 < gHour && gHour < 15) {
            
                        may->levelIndex = SQUARE;
                        may->startingCoordinates.y = 0.0f;
                        may->direction = DIRECTION_N;
                        may->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        may->startingCoordinates.x = -32.0f;
                        may->startingCoordinates.z = -320.0f;
                        may->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {
                    
                    if (8 < gHour && gHour < 15) {
            
                        may->levelIndex = MOUNTAIN_2;
                        may->startingCoordinates.y = 0.0f;
                        may->direction = DIRECTION_E;
                        may->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        may->startingCoordinates.x = -160.0f;
                        may->startingCoordinates.z = 288.0f;
                        may->flags |= NPC_ACTIVE;
                        
                    }
                }
    
                break;
            
            case MONDAY ... FRIDAY:

                if (7 < gHour && gHour < 15) {
        
                    may->levelIndex = CHURCH;
                    may->startingCoordinates.y = 0.0f;
                    may->direction = DIRECTION_N;
                    may->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    may->startingCoordinates.x = -96.0f;
                    may->startingCoordinates.z = -32.0f;
                    may->flags |= NPC_ACTIVE;
                    
                }

                if (14 < gHour && gHour < 17) {
                    
                    may->levelIndex = VILLAGE_2;
                    may->startingCoordinates.y = 0.0f;
                    may->direction = DIRECTION_N;
                    may->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    may->startingCoordinates.x = -64.0f;
                    may->startingCoordinates.z = 64.0f;
                    may->flags |= NPC_ACTIVE;
                    
                }
                
                break;
                
            case SATURDAY:

                if (8 < gHour && gHour < 15) {
        
                    may->levelIndex = VILLAGE_2;
                    may->startingCoordinates.y = 0.0f;
                    may->direction = DIRECTION_S;
                    may->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    may->startingCoordinates.x = 48.f;
                    may->startingCoordinates.z = -64.0f;
                    may->flags |= NPC_ACTIVE;
                    
                }
            
                break;
            
        }
        
    } else if (may->location < 2) {

        if (8 < gHour && gHour < 15) {
    
            may->levelIndex = LIBRARY;
            may->startingCoordinates.x = 0.0f;
            may->startingCoordinates.y = 0.0f;
            may->direction = DIRECTION_E;
            may->defaultAnimationMode = NPC_ANIMATION_IDLE;
            may->startingCoordinates.z = -64.0f;
            may->flags |= NPC_ACTIVE;
        
        }
        
    } else {
        
        if (8 < gHour && gHour < 15) {
    
            may->levelIndex = MIDWIFE_HOUSE;
            may->startingCoordinates.y = 0.0f;
            may->direction = DIRECTION_E;
            may->defaultAnimationMode = NPC_ANIMATION_IDLE;
            may->startingCoordinates.x = -96.0f;
            may->startingCoordinates.z = -32.0f;
            may->flags |= NPC_ACTIVE;
        
        }
        
    }
        
    may->animationMode = may->defaultAnimationMode;
    
}

void setRickLocation(void) {
    npcInfo *rick = &npcs[RICK];


    rick->wanderRadiusX = 64;
    rick->wanderRadiusZ = 64;
    rick->movingAnimation = 8;
    rick->idleAnimation = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case SUNDAY:

                if (rick->location < 2) {
                    
                    if (9 < gHour && gHour < 18) {
            
                        rick->levelIndex = SQUARE;
                        rick->startingCoordinates.y = 0.0f;
                        rick->direction = DIRECTION_E;
                        rick->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        rick->startingCoordinates.x = -192.0f;
                        rick->startingCoordinates.z = -32.0f;
                        rick->flags |= NPC_ACTIVE;
                        
                    }
                    
                }
    
                break;
            
            case MONDAY:
            case TUESDAY:

                if (9 < gHour && gHour < 18) {
        
                    rick->levelIndex = RICK_STORE;
                    rick->startingCoordinates.x = 0.0f;
                    rick->startingCoordinates.y = 0.0f;
                    rick->startingCoordinates.z = 0.0f;
                    rick->direction = DIRECTION_S;
                    rick->defaultAnimationMode = NPC_ANIMATION_WANDER;
                    rick->flags |= NPC_ACTIVE;
                    
                }
                
                break;
            
            case THURSDAY:
            case FRIDAY:
                
                if (9 < gHour && gHour < 18) {
        
                    rick->levelIndex = RICK_STORE;
                    rick->startingCoordinates.y = 0.0f;
                    rick->direction = DIRECTION_S;
                    rick->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    rick->startingCoordinates.x = -96.0f;
                    rick->startingCoordinates.z = -96.0f;
                    rick->flags |= NPC_ACTIVE;
                    
                }
            
                break;
            
            case SATURDAY:

                if (rick->location < 2) {
                
                    if (9 < gHour && gHour < 18) {
    
                        rick->levelIndex = VILLAGE_1;
                        rick->startingCoordinates.y = 0.0f;
                        rick->direction = DIRECTION_E;
                        rick->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        rick->startingCoordinates.x = -96.0f;
                        rick->startingCoordinates.z = 160.0f;
                        rick->flags |= NPC_ACTIVE;
                            
                    }
                    
                }
                
                break;

}
        
    } 
        
    rick->animationMode = rick->defaultAnimationMode;
    
}

void setPastorLocation(void) {
    npcInfo *pastor = &npcs[PASTOR];


    pastor->wanderRadiusX = 64;
    pastor->wanderRadiusZ = 64;
    pastor->movingAnimation = 8;
    pastor->idleAnimation = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SUNDAY:

                if (7 < gHour && gHour < 17) {
        
                    pastor->levelIndex = CHURCH;
                    pastor->startingCoordinates.y = 0.0f;
                    pastor->direction = DIRECTION_S;
                    pastor->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    pastor->startingCoordinates.x = -16.0f;
                    pastor->startingCoordinates.z = -192.0f;
                    pastor->flags |= NPC_ACTIVE;
                    
                }

break;
            
            case SATURDAY:
                
                if (7 < gHour && gHour < 17) {

                    pastor->levelIndex = VILLAGE_1;
                    pastor->startingCoordinates.y = 0.0f;
                    pastor->direction = DIRECTION_S;
                    pastor->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    pastor->startingCoordinates.x = -128.0f;
                    pastor->startingCoordinates.z = -128.0f;
                    pastor->flags |= NPC_ACTIVE;
                        
                    }
                
                break;

}
        
    } else if (7 < gHour && gHour < 17) {

        pastor->levelIndex = CHURCH;
        pastor->startingCoordinates.x = 0.0f;
        pastor->startingCoordinates.y = 0.0f;
        pastor->startingCoordinates.z = 0.0f;
        pastor->direction = DIRECTION_S;
        pastor->defaultAnimationMode = NPC_ANIMATION_WANDER;
        pastor->flags |= NPC_ACTIVE;
            
    }
        
    pastor->animationMode = pastor->defaultAnimationMode;
    
}

void setShipperLocation(void) {
    npcInfo *shipper = &npcs[SHIPPER];


    shipper->wanderRadiusX = 64;
    shipper->wanderRadiusZ = 64;
    shipper->idleAnimation = 0;
    shipper->movingAnimation = 8;

    if (NIGHTTIME && shipper->location < 2) {
        
        shipper->levelIndex = TAVERN;
        shipper->startingCoordinates.y = 0.0f;
        shipper->direction = DIRECTION_W;
        shipper->defaultAnimationMode = NPC_ANIMATION_IDLE;
    
        shipper->startingCoordinates.x = 128.0f;
        shipper->startingCoordinates.z = 64.0f;
    
        shipper->flags |= NPC_ACTIVE;

    }

    shipper->animationMode =  shipper->defaultAnimationMode;
    
}

void setSaibaraLocation(void) {
    npcInfo *saibara = &npcs[SAIBARA];


    saibara->wanderRadiusX = 64;
    saibara->wanderRadiusZ = 64;
    saibara->movingAnimation = 8;
    saibara->idleAnimation = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
                
                if (7 < gHour && gHour < 17) {
        
                    saibara->levelIndex = TOP_OF_MOUNTAIN_1;
                    saibara->startingCoordinates.y = 0.0f;
                    saibara->startingCoordinates.z = 0.0f;
                    saibara->direction = DIRECTION_E;
                    saibara->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    saibara->startingCoordinates.x = -128.0f;
                    saibara->flags |= NPC_ACTIVE;
                    
                }
                break;

            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
                
                if (7 < gHour && gHour < 17) {
        
                    saibara->levelIndex = SOUVENIR_SHOP;
                    saibara->startingCoordinates.y = 0.0f;
                    saibara->direction = DIRECTION_S;
                    saibara->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    saibara->startingCoordinates.x = 16.0f;
                    saibara->startingCoordinates.z = -64.0f;
                    saibara->flags |= NPC_ACTIVE;
                    
                }

                break;
            
        }
        
    } else {

        if (7 < gHour && gHour < 17) {

            saibara->levelIndex = SOUVENIR_SHOP;
            saibara->startingCoordinates.y = 0.0f;
            saibara->direction = DIRECTION_S;
            saibara->defaultAnimationMode = NPC_ANIMATION_IDLE;
            saibara->startingCoordinates.x = 16.0f;
            saibara->startingCoordinates.z = -64.0f;
            saibara->flags |= NPC_ACTIVE;
            
        }
        
    }

    saibara->animationMode = saibara->defaultAnimationMode;
    
}

void setDukeLocation(void) {
    npcInfo *duke = &npcs[DUKE];
 

    int temp = gDayOfWeek;

    duke->wanderRadiusX = 64;
    duke->wanderRadiusZ = 64;
    duke->idleAnimation = 0;
    duke->movingAnimation = 8;

    switch (temp) {
        
        case MONDAY ... SATURDAY:
            if NIGHTTIME {
                duke->levelIndex = TAVERN;
                duke->startingCoordinates.y = 0.0f;
                duke->direction = 0;
                duke->defaultAnimationMode = NPC_ANIMATION_IDLE;
            
                duke->startingCoordinates.x = -128.0f;
                duke->startingCoordinates.z = -128.0f;
            
                duke->flags |= NPC_ACTIVE;
            }
            break;

        default:
            break;
        
    }

    duke->animationMode =  duke->defaultAnimationMode;

}

// alternate without switch
/*
void setDukeLocation(void) {
    npcInfo *duke = &npcs[DUKE];

    
        int temp = gDayOfWeek;

        duke->wanderRadiusX = 64;
        duke->wanderRadiusZ = 64;
        duke->idleAnimation = 0;
        duke->movingAnimation = 8;

        if (temp < 7 && gDayOfWeek && NIGHTTIME) {
            
        duke->levelIndex = TAVERN;
        duke->startingCoordinates.y = 0.0f;
        duke->direction = 0;
        duke->defaultAnimationMode = NPC_ANIMATION_IDLE;

        duke->startingCoordinates.x = -128.0f;
        duke->startingCoordinates.z = -128.0f;
        
        duke->flags |= NPC_ACTIVE;
        
    }

    duke->animationMode =  duke->defaultAnimationMode;
    
}
*/

void setGregLocation(void) {
    npcInfo *greg = &npcs[GREG];


    greg->wanderRadiusX = 64;
    greg->wanderRadiusZ = 64;
    greg->idleAnimation = 0;
    greg->movingAnimation = 8;

    if (gSeason != WINTER && checkHaveTool(FISHING_POLE) && gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
                
                if (greg->location < 2) {

                    if (5 < gHour && gHour < 17) {
                        
                        greg->levelIndex = MOUNTAIN_1;
                
                        greg->startingCoordinates.y = 0;
                        greg->direction = DIRECTION_W;
                        greg->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        
                        greg->startingCoordinates.x = -160.0f;
                        greg->startingCoordinates.z = -128.0f;
                
                        greg->flags |= NPC_ACTIVE;
                        
                    }
                    
                } else {

                    if (5 < gHour && gHour < 17) {

                        greg->levelIndex = MOUNTAIN_2;
                
                        greg->startingCoordinates.y = 0;
                        greg->direction = DIRECTION_E;
                        greg->defaultAnimationMode = NPC_ANIMATION_IDLE;
                        
                        greg->startingCoordinates.x = 96.0f;
                        greg->startingCoordinates.z = 32.0f;
                
                        greg->flags |= NPC_ACTIVE;
                        
                    }
                    
                }
                
                break;
                    
            case MONDAY:
    
                if (6 < gHour && gHour < 17) {    
                
                    greg->levelIndex = BEACH;
            
                    greg->startingCoordinates.y = 0;
                    greg->direction = DIRECTION_E;
                    greg->defaultAnimationMode = NPC_ANIMATION_IDLE;
                    
                    greg->startingCoordinates.x = -96.0f;
                    greg->startingCoordinates.z = 176.0f;
            
                    greg->flags |= NPC_ACTIVE;
                    
                }
                    
                break;
            
        }
        
    }

    greg->animationMode = greg->defaultAnimationMode;
    
}

void setCarpenter1Location(void) {

    npcs[CARPENTER_1].wanderRadiusX = 64;
    npcs[CARPENTER_1].wanderRadiusZ = 64;
    npcs[CARPENTER_1].idleAnimation = 0;
    npcs[CARPENTER_1].movingAnimation = 8;

    if (!checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && !checkDailyEventBit(CARPENTER_ESTIMATE) 
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
                    npcs[CARPENTER_1].direction = DIRECTION_S;
                    npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                    npcs[CARPENTER_1].startingCoordinates.z = 32.0f;
                    npcs[CARPENTER_1].flags |= NPC_ACTIVE;
                    
                }
    
                 if (16 < gHour && gHour < 22) {
    
                    npcs[CARPENTER_1].levelIndex = TOP_OF_MOUNTAIN_1;
                    npcs[CARPENTER_1].startingCoordinates.x = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.y = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_1].direction = DIRECTION_S;
                    npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIMATION_IDLE;
                    npcs[CARPENTER_1].flags |= NPC_ACTIVE;

                }
                
            } else {
                
                if (7 < gHour && gHour < 18) {

                    npcs[CARPENTER_1].levelIndex = CARPENTER_HUT;
            
                    npcs[CARPENTER_1].startingCoordinates.y = 0;
                    npcs[CARPENTER_1].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_1].direction = DIRECTION_E;
                    npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIMATION_IDLE;
                    npcs[CARPENTER_1].startingCoordinates.x = -80.0f;
                    npcs[CARPENTER_1].flags |= NPC_ACTIVE;
                
                }
                
            }

        } else if (8 < gHour && gHour < 17) {

            npcs[CARPENTER_1].levelIndex = CAVE;
    
            npcs[CARPENTER_1].startingCoordinates.y = 0;
            npcs[CARPENTER_1].direction = DIRECTION_E;
            npcs[CARPENTER_1].defaultAnimationMode = NPC_ANIMATION_IDLE;
            npcs[CARPENTER_1].startingCoordinates.x = -304.0f;
            npcs[CARPENTER_1].startingCoordinates.z = -80.0f;
            npcs[CARPENTER_1].flags |= NPC_ACTIVE;
        
        }
        
    } 

    npcs[CARPENTER_1].animationMode = npcs[CARPENTER_1].defaultAnimationMode;
    
}

void setCarpenter2Location(void) {

    npcs[CARPENTER_2].wanderRadiusX = 64;
    npcs[CARPENTER_2].wanderRadiusZ = 64;
    npcs[CARPENTER_2].idleAnimation = 0;
    npcs[CARPENTER_2].movingAnimation = 8;

    if (!checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && !checkDailyEventBit(CARPENTER_ESTIMATE) 
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
                        npcs[CARPENTER_2].direction = DIRECTION_S;
                        npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
                        npcs[CARPENTER_2].startingCoordinates.x = -224.0f;
                        npcs[CARPENTER_2].startingCoordinates.z = 64.0f;
                        npcs[CARPENTER_2].flags |= NPC_ACTIVE;
                        
                    }
        
                } else if (7 < gHour && gHour < 18) {
    
                    npcs[CARPENTER_2].levelIndex = 0x21;
                    npcs[CARPENTER_2].startingCoordinates.y = 0.0f;
                    npcs[CARPENTER_2].direction = DIRECTION_N;
                    npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIMATION_IDLE;
                    npcs[CARPENTER_2].startingCoordinates.x = 64.0f;
                    npcs[CARPENTER_2].startingCoordinates.z = -112.0f;
                    npcs[CARPENTER_2].flags |= NPC_ACTIVE;

                }
                
            } else {
                
                if (7 < gHour && gHour < 18) {

                    npcs[CARPENTER_2].levelIndex = CARPENTER_HUT;
                    npcs[CARPENTER_2].startingCoordinates.y = 0;
                    npcs[CARPENTER_2].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_2].direction = DIRECTION_W;
                    npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIMATION_IDLE;
                    npcs[CARPENTER_2].startingCoordinates.x = 16.0f;
                    npcs[CARPENTER_2].flags |= NPC_ACTIVE;
                
                }
                
            }

        } else if (npcs[CARPENTER_2].location == 0 && 8 < gHour && gHour < 17) {

            npcs[CARPENTER_2].levelIndex = MINE;
            npcs[CARPENTER_2].startingCoordinates.x = 0.0f;
            npcs[CARPENTER_2].startingCoordinates.y = 0;
            npcs[CARPENTER_2].startingCoordinates.z = 0.0f;
            npcs[CARPENTER_2].direction = DIRECTION_S;
            npcs[CARPENTER_2].defaultAnimationMode = NPC_ANIMATION_WANDER;
            npcs[CARPENTER_2].flags |= NPC_ACTIVE;
        
        }
        
    } 

    npcs[CARPENTER_2].animationMode = npcs[CARPENTER_2].defaultAnimationMode;
    
}

void setMasterCarpenterLocation(void) {
    npcInfo *masterCarpenter = &npcs[MASTER_CARPENTER];


    masterCarpenter->wanderRadiusX = 64;
    masterCarpenter->wanderRadiusZ = 64;
    masterCarpenter->idleAnimation = 0;
    masterCarpenter->movingAnimation = 8;

    if (!checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && !checkDailyEventBit(CARPENTER_ESTIMATE) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gWeather == SUNNY && masterCarpenter->location >= 3) {

            if (7 < gHour && gHour < 18) {    
            
                masterCarpenter->levelIndex = MOUNTAIN_2;
        
                masterCarpenter->startingCoordinates.y = 0;
                masterCarpenter->direction = DIRECTION_S;
                masterCarpenter->defaultAnimationMode = NPC_ANIMATION_IDLE;
                
                masterCarpenter->startingCoordinates.x = -112.0f;
                masterCarpenter->startingCoordinates.z = 112.0f;
        
                masterCarpenter->flags |= NPC_ACTIVE;
                
            }

        } else if (7 < gHour && gHour < 18) {

            masterCarpenter->levelIndex = CARPENTER_HUT;
    
            masterCarpenter->startingCoordinates.y = 0;
            masterCarpenter->direction = DIRECTION_S;
            masterCarpenter->defaultAnimationMode = NPC_ANIMATION_LOCATION_SPECIAL;
            
            masterCarpenter->startingCoordinates.x = -32.0f;
            masterCarpenter->startingCoordinates.z = -48.0f;
    
            masterCarpenter->flags |= NPC_ACTIVE;
        
        }
        
    } 

    masterCarpenter->animationMode = masterCarpenter->defaultAnimationMode;
    
}

void setHarvestSprite1Location(void) {

    npcs[HARVEST_SPRITE_1].wanderRadiusX = 64;
    npcs[HARVEST_SPRITE_1].wanderRadiusZ = 64;
    npcs[HARVEST_SPRITE_1].idleAnimation = 0;
    npcs[HARVEST_SPRITE_1].movingAnimation = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_1].levelIndex = CAVE;
        npcs[HARVEST_SPRITE_1].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_1].direction = 0;
        npcs[HARVEST_SPRITE_1].defaultAnimationMode = NPC_ANIMATION_WANDER;
    
        npcs[HARVEST_SPRITE_1].startingCoordinates.x = -32.0f;
        npcs[HARVEST_SPRITE_1].startingCoordinates.z = -192.0f;
    
        npcs[HARVEST_SPRITE_1].flags |= NPC_ACTIVE;

    }

    npcs[HARVEST_SPRITE_1].animationMode =  npcs[HARVEST_SPRITE_1].defaultAnimationMode;
    
}

void setHarvestSprite2Location(void) {

    npcs[HARVEST_SPRITE_2].wanderRadiusX = 64;
    npcs[HARVEST_SPRITE_2].wanderRadiusZ = 64;
    npcs[HARVEST_SPRITE_2].idleAnimation = 0;
    npcs[HARVEST_SPRITE_2].movingAnimation = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_2].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_2].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_2].direction = DIRECTION_S;
        npcs[HARVEST_SPRITE_2].defaultAnimationMode = NPC_ANIMATION_IDLE;
    
        npcs[HARVEST_SPRITE_2].startingCoordinates.x = -128.0f;
        npcs[HARVEST_SPRITE_2].startingCoordinates.z = -128.0f;
    
        npcs[HARVEST_SPRITE_2].flags |= NPC_ACTIVE;

    }

    npcs[HARVEST_SPRITE_2].animationMode =  npcs[HARVEST_SPRITE_2].defaultAnimationMode;
    
}

void setHarvestSprite3Location(void) {

    npcs[HARVEST_SPRITE_3].wanderRadiusX = 64;
    npcs[HARVEST_SPRITE_3].wanderRadiusZ = 64;
    npcs[HARVEST_SPRITE_3].idleAnimation = 0;
    npcs[HARVEST_SPRITE_3].movingAnimation = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_3].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_3].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_3].direction = DIRECTION_W;
        npcs[HARVEST_SPRITE_3].defaultAnimationMode = NPC_ANIMATION_IDLE;
    
        npcs[HARVEST_SPRITE_3].startingCoordinates.x = 64.0f;
        npcs[HARVEST_SPRITE_3].startingCoordinates.z = -64.0f;
    
        npcs[HARVEST_SPRITE_3].flags |= NPC_ACTIVE;

    }

    npcs[HARVEST_SPRITE_3].animationMode =  npcs[HARVEST_SPRITE_3].defaultAnimationMode;
    
}

void setSydneyLocation(void) {
    npcInfo *sydney = &npcs[SYDNEY];


    sydney->wanderRadiusX = 64;
    sydney->wanderRadiusZ = 64;
    sydney->idleAnimation = 0;
    sydney->movingAnimation = 8;

    if (gSeason != WINTER && gWeather == SUNNY && sydney->location >= 2 && (7 < gHour && gHour < 17)) {

        sydney->levelIndex = MOON_MOUNTAIN;
        sydney->startingCoordinates.y = 0.0f;
        sydney->direction = DIRECTION_W;
        sydney->defaultAnimationMode = NPC_ANIMATION_WANDER;

        sydney->startingCoordinates.x = 64.0F;
        sydney->startingCoordinates.z = -32.0f;

        sydney->flags |= NPC_ACTIVE;
        
    } else if (7 < gHour && gHour < 17) {
        
        sydney->levelIndex = DUMPLING_HOUSE;
        sydney->startingCoordinates.y = 0.0f;
        sydney->direction = DIRECTION_E;
        sydney->defaultAnimationMode = NPC_ANIMATION_IDLE;

        sydney->startingCoordinates.x = -128.0f;
        sydney->startingCoordinates.z = -128.0f;

        sydney->flags |= NPC_ACTIVE;

    }

    sydney->animationMode =  sydney->defaultAnimationMode;
    
}

void setBarleyLocation(void) {
    npcInfo *barley = &npcs[BARLEY];


    barley->wanderRadiusX = 64;
    barley->wanderRadiusZ = 64;
    barley->idleAnimation = 0;
    barley->movingAnimation = 8;

    if (barley->location < 2 && (7 < gHour && gHour < 17)) {

        barley->levelIndex = DUMPLING_HOUSE;
        barley->startingCoordinates.y = 0.0f;
        barley->direction = 0;
        barley->defaultAnimationMode = NPC_ANIMATION_WANDER;

        barley->startingCoordinates.x = -32.0f;
        barley->startingCoordinates.z = -128.0f;

        barley->flags |= NPC_ACTIVE;
        
    } else if (7 < gHour && gHour < 17) {
        
        barley->levelIndex = DUMPLING_HOUSE;
        barley->startingCoordinates.x = 0.0f;
        barley->startingCoordinates.y = 0.0f;
        barley->direction = DIRECTION_S;
        barley->defaultAnimationMode = NPC_ANIMATION_IDLE;

        barley->startingCoordinates.z = -192.0f;

        barley->flags |= NPC_ACTIVE;

    }

    barley->animationMode =  barley->defaultAnimationMode;
    
}

void setMrsManaLocation(void) {
    npcInfo *mrsMana = &npcs[MRS_MANA];

    
    mrsMana->wanderRadiusX = 64;
    mrsMana->wanderRadiusZ = 64;
    mrsMana->idleAnimation = 0;
    mrsMana->movingAnimation = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(MRS_MANA_COWS_EVENT) && NIGHTTIME) {

        mrsMana->levelIndex = TAVERN;
        mrsMana->startingCoordinates.y = 0;
        mrsMana->direction = DIRECTION_W;
        mrsMana->defaultAnimationMode = NPC_ANIMATION_IDLE;

        mrsMana->startingCoordinates.x = 128.0f;
        mrsMana->startingCoordinates.z = -112.0f;

        mrsMana->flags |= NPC_ACTIVE;
        
    }

    mrsMana->animationMode =  mrsMana->defaultAnimationMode;
    
}

void setJohnLocation(void) {
    npcInfo *john = &npcs[JOHN];

    
    john->wanderRadiusX = 64;
    john->wanderRadiusZ = 64;
    john->idleAnimation = 0;
    john->movingAnimation = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(MRS_MANA_COWS_EVENT) && (9 < gHour && gHour < 16)) {

        john->levelIndex = BARN;
        john->startingCoordinates.y = 0;
        john->direction = DIRECTION_SE;
        john->defaultAnimationMode = NPC_ANIMATION_IDLE;

        john->startingCoordinates.x = -192.0f;
        john->startingCoordinates.z = -256.0f;

        john->flags |= NPC_ACTIVE;
        
    }

    john->animationMode =  john->defaultAnimationMode;
    
}

void setGourmetJudgeLocation(void) {
    npcInfo *gourmetJudge = &npcs[GOURMET_JUDGE];


    gourmetJudge->wanderRadiusX = 64;
    gourmetJudge->wanderRadiusZ = 64;
    gourmetJudge->idleAnimation = 0;
    gourmetJudge->movingAnimation = 8;

    if (checkLifeEventBit(VINEYARD_FIRST_WINE) && gourmetJudge->location == 0 && (8 < gHour && gHour < 17)) {

        gourmetJudge->levelIndex = ROAD;
        gourmetJudge->startingCoordinates.y = 0;
        gourmetJudge->direction = DIRECTION_N;
        gourmetJudge->defaultAnimationMode = NPC_ANIMATION_WANDER;

        gourmetJudge->startingCoordinates.x = -32.0f;
        gourmetJudge->startingCoordinates.z = 64.0f;

        gourmetJudge->flags |= NPC_ACTIVE;

    }

    if (checkLifeEventBit(HAVE_ICE_CREAM_MAKER) && (8 < gHour && gHour < 17)) {
        
        gourmetJudge->levelIndex = VILLAGE_1;
        gourmetJudge->startingCoordinates.y = 0;
        gourmetJudge->direction = DIRECTION_S;
        gourmetJudge->defaultAnimationMode = NPC_ANIMATION_WANDER;

        gourmetJudge->startingCoordinates.x = 320.0f;
        gourmetJudge->startingCoordinates.z = 288.0f;

        gourmetJudge->flags |= NPC_ACTIVE;
    
    }

     gourmetJudge->animationMode =  gourmetJudge->defaultAnimationMode; 
    
}

void setMariaHarrisBabyLocation(void) {
    npcInfo *mariaHarrisBaby = &npcs[MARIA_HARRIS_BABY];
    npcInfo *maria = &npcs[MARIA];


    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    mariaHarrisBaby->wanderRadiusX = 64;
    mariaHarrisBaby->wanderRadiusZ = 64;
    mariaHarrisBaby->idleAnimation = 0;
    mariaHarrisBaby->movingAnimation = 0;

    temp = getNPCBabyCarryingState(MARIA);

    if (temp == 1) {

        if (maria->flags & NPC_ACTIVE) {
            
            mariaHarrisBaby->startingCoordinates.x = 0.0f;
            mariaHarrisBaby->startingCoordinates.y = 0.0f;
            mariaHarrisBaby->startingCoordinates.z = 0.0f;

            mariaHarrisBaby->defaultAnimationMode = NPC_ANIMATION_IDLE;

            mariaHarrisBaby->idleAnimation = 8;
            mariaHarrisBaby->movingAnimation = 8;

            mariaHarrisBaby->levelIndex = maria->levelIndex;
            mariaHarrisBaby->flags |= (NPC_ACTIVE | NPC_ATTACHED);

            mariaHarrisBaby->direction = maria->direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (maria->flags & NPC_ACTIVE) {

            mariaHarrisBaby->levelIndex = maria->levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(maria->direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &mariaHarrisBaby->startingCoordinates, rotationAngles);

            mariaHarrisBaby->defaultAnimationMode = NPC_ANIMATION_WANDER;

            mariaHarrisBaby->idleAnimation = 0;
            mariaHarrisBaby->movingAnimation = 0;

            mariaHarrisBaby->flags |= NPC_ACTIVE;

            mariaHarrisBaby->startingCoordinates.x = maria->startingCoordinates.x + vec1.x;
            mariaHarrisBaby->startingCoordinates.y = maria->startingCoordinates.y;
            mariaHarrisBaby->startingCoordinates.z = maria->startingCoordinates.z + vec1.z;
            
            mariaHarrisBaby->direction = maria->direction;
                            
        }
        
    }

    mariaHarrisBaby->animationMode = mariaHarrisBaby->defaultAnimationMode;
    
}

void setPopuriGrayBabyLocation(void) {
    npcInfo *popuriGrayBaby = &npcs[POPURI_GRAY_BABY];
    npcInfo *popuri = &npcs[POPURI];

    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    popuriGrayBaby->wanderRadiusX = 64;
    popuriGrayBaby->wanderRadiusZ = 64;
    popuriGrayBaby->idleAnimation = 0;
    popuriGrayBaby->movingAnimation = 0;

    temp = getNPCBabyCarryingState(POPURI);

    if (temp == 1) {

        if (popuri->flags & NPC_ACTIVE) {
            
            popuriGrayBaby->startingCoordinates.x = 0.0f;
            popuriGrayBaby->startingCoordinates.y = 0.0f;
            popuriGrayBaby->startingCoordinates.z = 0.0f;

            popuriGrayBaby->defaultAnimationMode = NPC_ANIMATION_IDLE;

            popuriGrayBaby->idleAnimation = 8;
            popuriGrayBaby->movingAnimation = 8;

            popuriGrayBaby->levelIndex = popuri->levelIndex;
            popuriGrayBaby->flags |= (NPC_ACTIVE | NPC_ATTACHED);

            popuriGrayBaby->direction = popuri->direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (popuri->flags & NPC_ACTIVE) {

            popuriGrayBaby->levelIndex = popuri->levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(popuri->direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &popuriGrayBaby->startingCoordinates, rotationAngles);

            popuriGrayBaby->defaultAnimationMode = NPC_ANIMATION_WANDER;

            popuriGrayBaby->idleAnimation = 0;
            popuriGrayBaby->movingAnimation = 0;

            popuriGrayBaby->flags |= NPC_ACTIVE;

            popuriGrayBaby->startingCoordinates.x = popuri->startingCoordinates.x + vec1.x;
            popuriGrayBaby->startingCoordinates.y = popuri->startingCoordinates.y;
            popuriGrayBaby->startingCoordinates.z = popuri->startingCoordinates.z + vec1.z;
            
            popuriGrayBaby->direction = popuri->direction;
                            
        }
        
    }

    popuriGrayBaby->animationMode = popuriGrayBaby->defaultAnimationMode;

}

void setElliJeffBabyLocation(void) {
    npcInfo *elliJeffBaby = &npcs[ELLI_JEFF_BABY];
    npcInfo *elli = &npcs[ELLI];

    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    elliJeffBaby->wanderRadiusX = 64;
    elliJeffBaby->wanderRadiusZ = 64;
    elliJeffBaby->idleAnimation = 0;
    elliJeffBaby->movingAnimation = 0;

    temp = getNPCBabyCarryingState(ELLI);

    if (temp == 1) {

        if (elli->flags & NPC_ACTIVE) {
            
            elliJeffBaby->startingCoordinates.x = 0.0f;
            elliJeffBaby->startingCoordinates.y = 0.0f;
            elliJeffBaby->startingCoordinates.z = 0.0f;

            elliJeffBaby->defaultAnimationMode = NPC_ANIMATION_IDLE;

            elliJeffBaby->idleAnimation = 8;
            elliJeffBaby->movingAnimation = 8;

            elliJeffBaby->levelIndex = elli->levelIndex;
            elliJeffBaby->flags |= (NPC_ACTIVE | NPC_ATTACHED);

            elliJeffBaby->direction = elli->direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (elli->flags & NPC_ACTIVE) {

            elliJeffBaby->levelIndex = elli->levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(elli->direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &elliJeffBaby->startingCoordinates, rotationAngles);

            elliJeffBaby->defaultAnimationMode = NPC_ANIMATION_WANDER;

            elliJeffBaby->idleAnimation = 0;
            elliJeffBaby->movingAnimation = 0;

            elliJeffBaby->flags |= NPC_ACTIVE;

            elliJeffBaby->startingCoordinates.x = elli->startingCoordinates.x + vec1.x;
            elliJeffBaby->startingCoordinates.y = elli->startingCoordinates.y;
            elliJeffBaby->startingCoordinates.z = elli->startingCoordinates.z + vec1.z;
            
            elliJeffBaby->direction = elli->direction;
                            
        }
        
    }

    elliJeffBaby->animationMode = elliJeffBaby->defaultAnimationMode;

}

void setAnnCliffBabyLocation(void) {
    npcInfo *annCliffBaby = &npcs[ANN_CLIFF_BABY];
    npcInfo *ann = &npcs[ANN];

    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    annCliffBaby->wanderRadiusX = 64;
    annCliffBaby->wanderRadiusZ = 64;
    annCliffBaby->idleAnimation = 0;
    annCliffBaby->movingAnimation = 0;

    temp = getNPCBabyCarryingState(ANN);

    if (temp == 1) {

        if (ann->flags & NPC_ACTIVE) {
            
            annCliffBaby->startingCoordinates.x = 0.0f;
            annCliffBaby->startingCoordinates.y = 0.0f;
            annCliffBaby->startingCoordinates.z = 0.0f;

            annCliffBaby->defaultAnimationMode = NPC_ANIMATION_IDLE;

            annCliffBaby->idleAnimation = 8;
            annCliffBaby->movingAnimation = 8;

            annCliffBaby->levelIndex = ann->levelIndex;
            annCliffBaby->flags |= (NPC_ACTIVE | NPC_ATTACHED);

            annCliffBaby->direction = ann->direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (ann->flags & NPC_ACTIVE) {

            annCliffBaby->levelIndex = ann->levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(ann->direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &annCliffBaby->startingCoordinates, rotationAngles);

            annCliffBaby->defaultAnimationMode = NPC_ANIMATION_WANDER;

            annCliffBaby->idleAnimation = 0;
            annCliffBaby->movingAnimation = 0;

            annCliffBaby->flags |= NPC_ACTIVE;

            annCliffBaby->startingCoordinates.x = ann->startingCoordinates.x + vec1.x;
            annCliffBaby->startingCoordinates.y = ann->startingCoordinates.y;
            annCliffBaby->startingCoordinates.z = ann->startingCoordinates.z + vec1.z;
            
            annCliffBaby->direction = ann->direction;
                            
        }
        
    }

    annCliffBaby->animationMode = annCliffBaby->defaultAnimationMode;

}

void setKarenKaiBabyLocation(void) {
    npcInfo *karenKaiBaby = &npcs[KAREN_KAI_BABY];
    npcInfo *karen = &npcs[KAREN];


    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    karenKaiBaby->wanderRadiusX = 64;
    karenKaiBaby->wanderRadiusZ = 64;
    karenKaiBaby->idleAnimation = 0;
    karenKaiBaby->movingAnimation = 0;

    temp = getNPCBabyCarryingState(KAREN);

    if (temp == 1) {

        if (karen->flags & NPC_ACTIVE) {
            
            karenKaiBaby->startingCoordinates.x = 0.0f;
            karenKaiBaby->startingCoordinates.y = 0.0f;
            karenKaiBaby->startingCoordinates.z = 0.0f;

            karenKaiBaby->defaultAnimationMode = NPC_ANIMATION_IDLE;

            karenKaiBaby->idleAnimation = 8;
            karenKaiBaby->movingAnimation = 8;

            karenKaiBaby->levelIndex = karen->levelIndex;
            karenKaiBaby->flags |= (NPC_ACTIVE | NPC_ATTACHED);

            karenKaiBaby->direction = karen->direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (karen->flags & NPC_ACTIVE) {

            karenKaiBaby->levelIndex = karen->levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(karen->direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &karenKaiBaby->startingCoordinates, rotationAngles);

            karenKaiBaby->defaultAnimationMode = NPC_ANIMATION_WANDER;

            karenKaiBaby->idleAnimation = 0;
            karenKaiBaby->movingAnimation = 0;

            karenKaiBaby->flags |= NPC_ACTIVE;

            karenKaiBaby->startingCoordinates.x = karen->startingCoordinates.x + vec1.x;
            karenKaiBaby->startingCoordinates.y = karen->startingCoordinates.y;
            karenKaiBaby->startingCoordinates.z = karen->startingCoordinates.z + vec1.z;
            
            karenKaiBaby->direction = karen->direction;
                            
        }
        
    }

    karenKaiBaby->animationMode = karenKaiBaby->defaultAnimationMode;
    
}

void setEntomologistLocation(void) {
    npcInfo *entomologist = &npcs[ENTOMOLOGIST];

    
    entomologist->wanderRadiusX = 64;
    entomologist->wanderRadiusZ = 64;
    entomologist->idleAnimation = 0;
    entomologist->movingAnimation = 8;

    if (gYear == 2 && gSeason == SPRING && 1 < gDayOfMonth && gDayOfMonth < 21 && (5 < gHour && gHour < 18)) {

        entomologist->levelIndex = MOUNTAIN_1;
        entomologist->startingCoordinates.y = 0;
        entomologist->direction = DIRECTION_SE;
        entomologist->defaultAnimationMode = NPC_ANIMATION_WANDER;

        entomologist->startingCoordinates.x = -32.0f;
        entomologist->startingCoordinates.z = -64.0f;

        entomologist->flags |= NPC_ACTIVE;
        
    }

    entomologist->animationMode =  entomologist->defaultAnimationMode;
    
}

/* animations */

static inline void updateAnimation(u8 index) {

    switch (npcs[index].animationMode) {

        case NPC_ANIMATION_IDLE:
            stopNPCMovement(index);
            break;

        case NPC_ANIMATION_WANDER:
            updateNPCWanderAnimation(index);
            break;

    }
}

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

void handleMariaAnimation(void) {
    npcInfo *maria = &npcs[MARIA];


    u16 temp;

    if (maria->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(maria->entityIndex) && maria->animationTimer == 0) {

            switch (maria->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(MARIA);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(MARIA);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:
                    updateBacheloretteBeachAnimation(MARIA);
                    break;

                case NPC_ANIMATION_SLEEPING:

                    setEntityAnimation(maria->entityIndex, 24);

                    entities[maria->entityIndex].coordinates.x = -192.0f;
                    entities[maria->entityIndex].coordinates.y = 0.0f;
                    entities[maria->entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (maria->animationTimer) {
            maria->animationTimer--;
        }
    
        updateNPCMovement(MARIA);
    
    }
    
}

void handlePopuriAnimation(void) {
    npcInfo *popuri = &npcs[POPURI];


    u16 temp;

    if (popuri->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(popuri->entityIndex) && popuri->animationTimer == 0) {

            switch (popuri->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(POPURI);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(POPURI);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:
                    updateBacheloretteBeachAnimation(POPURI);
                    break;

                case NPC_ANIMATION_CUSTOM:

                    switch (popuri->animationState) {

                        case 0:
                            popuri->speed = 0;
                            popuri->animationTimer = 0;

                            setEntityDirectionalAnimation(popuri->entityIndex, 0);

                            temp = getRandomNumberInRange(0, 60);

                            if (temp < 8) {
                                popuri->direction = temp;
                                popuri->animationState = 1;
                            }

                            if (7 < temp && temp < 21) {
                                popuri->animationState = 2;
                            }

                            popuri->flags |= NPC_NEEDS_UPDATE;
                            break;

                        case 1:
                            popuri->speed = 1;
                            popuri->animationTimer = 0;

                            setEntityDirectionalAnimation(popuri->entityIndex, 8);

                            if (getRandomNumberInRange(0, 19) <= 7) {
                                popuri->animationState = 1;
                            } else {
                                popuri->animationState = 0;
                            }

                            popuri->flags |= NPC_NEEDS_UPDATE;
                            break;

                        case 2:

                            popuri->speed = 0;
                            popuri->animationTimer = 60;

                            setEntityDirectionalAnimation(popuri->entityIndex, 0x72);
                            popuri->animationState = 1;

                            popuri->flags |= NPC_NEEDS_UPDATE;
                            break;

                    }

                    break;

                case NPC_ANIMATION_SLEEPING:

                    setEntityAnimation(popuri->entityIndex, 24);

                    entities[popuri->entityIndex].coordinates.x = -192.0f;
                    entities[popuri->entityIndex].coordinates.y = 0.0f;
                    entities[popuri->entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (popuri->animationTimer) {
            popuri->animationTimer--;
        }
    
        updateNPCMovement(POPURI);
    
    }
    
}

void handleElliAnimation(void) {
    npcInfo *elli = &npcs[ELLI];

    
    u16 temp;

    if (elli->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(elli->entityIndex) && elli->animationTimer == 0) {

            switch (elli->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(ELLI);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(ELLI);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:
                    updateBacheloretteBeachAnimation(ELLI);
                    break;

                case NPC_ANIMATION_SLEEPING:

                    setEntityAnimation(elli->entityIndex, 24);

                    entities[elli->entityIndex].coordinates.x = -192.0f;
                    entities[elli->entityIndex].coordinates.y = 0.0f;
                    entities[elli->entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (elli->animationTimer) {
            elli->animationTimer--;
        }
    
        updateNPCMovement(ELLI);
    
    }
    
}

void handleAnnAnimation(void) {
    npcInfo *ann = &npcs[ANN];


    u16 temp;

    if (ann->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(ann->entityIndex) && ann->animationTimer == 0) {

            switch (ann->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(ANN);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(ANN);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:
                    updateBacheloretteBeachAnimation(ANN);
                    break;

                case NPC_ANIMATION_SLEEPING:

                    setEntityAnimation(ann->entityIndex, 24);

                    entities[ann->entityIndex].coordinates.x = -192.0f;
                    entities[ann->entityIndex].coordinates.y = 0.0f;
                    entities[ann->entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (ann->animationTimer) {
            ann->animationTimer--;
        }
    
        updateNPCMovement(ANN);
    
    }
    
}

void handleKarenAnimation(void) {
    npcInfo *karen = &npcs[KAREN];


    u16 temp;

    if (karen->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(karen->entityIndex) && karen->animationTimer == 0) {

            switch (karen->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(KAREN);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(KAREN);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:
                    updateBacheloretteBeachAnimation(KAREN);
                    break;

                case NPC_ANIMATION_CUSTOM:

                    if (karen->animationState == 0) {

                        karen->speed = 0;
                        karen->animationTimer = 0;

                        setEntityDirectionalAnimation(karen->entityIndex, 0);

                        temp = getRandomNumberInRange(0, 60);

                        if (temp < 4) {
                            karen->direction = temp * 2;
                            karen->animationState = 1;
                        }

                        karen->flags |= NPC_NEEDS_UPDATE;

                    } else {

                        karen->speed = 1;
                        karen->animationTimer = 0;

                        setEntityDirectionalAnimation(karen->entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) <=7) {
                            karen->animationState = 1;
                        } else {
                            karen->animationState = 0;
                        }

                        karen->flags |= NPC_NEEDS_UPDATE;

                    }

                    break;

                case NPC_ANIMATION_SLEEPING:

                    setEntityAnimation(karen->entityIndex, 24);

                    entities[karen->entityIndex].coordinates.x = -192.0f;
                    entities[karen->entityIndex].coordinates.y = 0.0f;
                    entities[karen->entityIndex].coordinates.z = -160.0f;

            }
            
        }
        
        if (karen->animationTimer) {
            karen->animationTimer--;
        }
    
        updateNPCMovement(KAREN);
    
    }

}

void handleBabyAnimation(void) {
    npcInfo *baby = &npcs[BABY];


    if (baby->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(baby->entityIndex)) {
            
            if (baby->animationTimer == 0) {
    
                switch (baby->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        handleBabyIdleAnimation(BABY, 0);
                        break;

                    case NPC_ANIMATION_WANDER:

                        baby->speed = 0;
                        baby->animationState = 10;
                        baby->animationTimer = 0;

                        if (npcAffection[BABY] < 50) {

                            if (getRandomNumberInRange(0, 100)) {
                                setEntityAnimation(baby->entityIndex, 3);
                            } else {
                                setEntityAnimation(baby->entityIndex, 11);
                            }

                        } else if (npcAffection[BABY] >= 200 && !getRandomNumberInRange(0, 100)) {
                            setEntityAnimation(baby->entityIndex, 9);
                        } else {
                            setEntityAnimation(baby->entityIndex, 3);
                        }

                        baby->flags |= NPC_NEEDS_UPDATE;
                        break;

                    case NPC_ANIMATION_LOCATION_SPECIAL:
                        baby->speed = 0;
                        baby->animationState = 10;
                        baby->animationTimer = 0;
                        setEntityAnimation(baby->entityIndex, 13);
                        baby->flags |= NPC_NEEDS_UPDATE;
                        break;

                    case NPC_ANIMATION_CUSTOM:
                        switch (getRandomNumberInRange(0, 1)) {
                            // different crawling animations
                            case 0:
                                updateBabyWanderAnimation(5, 57, 65);
                                break;
                            case 1:
                                updateBabyWanderAnimation(5, 57, 73);
                                break;
                            }
                        break;

                    case NPC_ANIMATION_BABY_HOUSE:
                        updateBabyAnimations(BABY, 14, 22, 81, 89, 121, 105, 113, 30, 46, 97, 38);
                        break;

                    case NPC_ANIMATION_BABY_CRIB:
                        baby->speed = 0;
                        baby->animationState = 10;
                        baby->animationTimer = 0;
                        setEntityAnimation(baby->entityIndex, 54);
                        baby->flags |= NPC_NEEDS_UPDATE;
                        break;

                }

            }
            
        }

        if (baby->animationTimer) {
            baby->animationTimer--;
        }

        updateNPCMovement(BABY);
        
    }
    
}

void handleHarrisAnimation(void) {
    npcInfo *harris = &npcs[HARRIS];


    if (harris->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(harris->entityIndex)) {

            if (harris->animationTimer == 0) {

                switch (harris->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(HARRIS);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(HARRIS);
                        break;
                }
            } 
            
        }

        if (harris->animationTimer) {
            harris->animationTimer--;
        }
    
        updateNPCMovement(HARRIS);
        
    }
    
}

void handleGrayAnimation(void) {
    npcInfo *gray = &npcs[GRAY];


    if (gray->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(gray->entityIndex)) {

            if (gray->animationTimer == 0) {

                switch (gray->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(GRAY);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(GRAY);
                        break;

                    // petting horse
                    case NPC_ANIMATION_LOCATION_SPECIAL:

                        gray->speed = 0;
                        gray->animationState = 10;
                        gray->animationTimer = 0;

                        setEntityAnimation(gray->entityIndex, 42);

                        gray->flags |= NPC_NEEDS_UPDATE;
                        break;

                }

            } 
            
        }

        if (gray->animationTimer) {
            gray->animationTimer--;
        }
    
        updateNPCMovement(GRAY);
        
    }
    
}

void handleJeffAnimation(void) {
    npcInfo *jeff = &npcs[JEFF];


    if (jeff->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(jeff->entityIndex)) {

            if (jeff->animationTimer == 0) {

                switch (jeff->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(JEFF);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(JEFF);
                        break;

                    case NPC_ANIMATION_LOCATION_SPECIAL:

                        jeff->speed = 0;
                        jeff->animationState = 10;
                        jeff->animationTimer = 0;

                        // fishing
                        setEntityAnimation(jeff->entityIndex, 51);

                        jeff->flags |= NPC_NEEDS_UPDATE;
                        break;

                }
            } 
            
        }

        if (jeff->animationTimer) {
            jeff->animationTimer--;
        }
    
        updateNPCMovement(JEFF);
        
    }
    
}

void handleCliffAnimation(void) {
    npcInfo *cliff = &npcs[CLIFF];

    
    if (cliff->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(cliff->entityIndex)) {

            if (cliff->animationTimer == 0) {

                switch (cliff->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(CLIFF);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(CLIFF);
                        break;
                }
            } 
            
        }

        if (cliff->animationTimer) {
            cliff->animationTimer--;
        }
    
        updateNPCMovement(CLIFF);
        
    }
    
}

void handleKaiAnimation(void) {
    npcInfo *kai = &npcs[KAI];

    
    if (kai->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(kai->entityIndex)) {

            if (kai->animationTimer == 0) {

                switch (kai->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(KAI);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(KAI);
                        break;

                }

            } 
            
        }

        if (kai->animationTimer) {
            kai->animationTimer--;
        }
    
        updateNPCMovement(KAI);
        
    }
    
}

void handleMayorAnimation(void) {
    npcInfo *mayor = &npcs[MAYOR];

    
    if (mayor->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(mayor->entityIndex)) {

            if (mayor->animationTimer == 0) {

                switch (mayor->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(MAYOR);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(MAYOR);
                        break;

                }

            } 
            
        }

        if (mayor->animationTimer) {
            mayor->animationTimer--;
        }
    
        updateNPCMovement(MAYOR);
        
    }
    
}

void handleMayorWifeAnimation(void) {
    npcInfo *mayorWife = &npcs[MAYOR_WIFE];

    
    if (mayorWife->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(mayorWife->entityIndex)) {

            if (mayorWife->animationTimer == 0) {

                switch (mayorWife->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(MAYOR_WIFE);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(MAYOR_WIFE);
                        break;

                }

            } 
            
        }

        if (mayorWife->animationTimer) {
            mayorWife->animationTimer--;
        }
    
        updateNPCMovement(MAYOR_WIFE);
        
    }
    
}

void handleLilliaAnimation(void) {
    npcInfo *lillia = &npcs[LILLIA];

    
    if (lillia->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(lillia->entityIndex)) {

            if (lillia->animationTimer == 0) {

                switch (lillia->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(LILLIA);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(LILLIA);
                        break;
                }

            } 
            
        }

        if (lillia->animationTimer) {
            lillia->animationTimer--;
        }
    
        updateNPCMovement(LILLIA);
        
    }
    
}

void handleBasilAnimation(void) {
    npcInfo *basil = &npcs[BASIL];


    if (basil->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(basil->entityIndex)) {

            if (basil->animationTimer == 0) {

                switch (basil->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(BASIL);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(BASIL);
                        break;
                }
            } 
            
        }

        if (basil->animationTimer) {
            basil->animationTimer--;
        }
    
        updateNPCMovement(BASIL);
        
    }

}

void handleEllenAnimation(void) {
    npcInfo *ellen = &npcs[ELLEN];


    if (ellen->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(ellen->entityIndex)) {

            if (ellen->animationTimer == 0 && !ellen->animationMode) {
                ellen->speed = 0;
                ellen->animationState = 10;
                ellen->animationTimer = 0;
                setEntityAnimation(ellen->entityIndex, 8);
                ellen->flags |= NPC_NEEDS_UPDATE;
            } 
            
        }

        if (ellen->animationTimer) {
            ellen->animationTimer--;
        }
    
        updateNPCMovement(ELLEN);
        
    }
    
}

void handleDougAnimation(void) {
    npcInfo *doug = &npcs[DOUG];


    if (doug->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(doug->entityIndex) && !doug->animationTimer) {
            updateAnimation(DOUG);
        }

        if (doug->animationTimer) {
            doug->animationTimer--; 
        } 
        
        updateNPCMovement(DOUG);
    }   
}

void handleGotzAnimation(void) {
    npcInfo *gotz = &npcs[GOTZ];


   u16 tempDirection;

    if (gotz->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(gotz->entityIndex) && !gotz->animationTimer) {

            switch (gotz->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(GOTZ);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(GOTZ);
                    break;

                case NPC_ANIMATION_CUSTOM:

                    // FIXME: should be inline function
                    if (gotz->animationState == 0) {

                        gotz->speed = 0;
                        gotz->animationTimer = 0;

                        setEntityDirectionalAnimation(gotz->entityIndex, 0);

                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < DIRECTION_N) {
                            gotz->direction = tempDirection * 2;
                            gotz->animationState = 1;
                        }

                        gotz->flags |= NPC_NEEDS_UPDATE;

                    } else {

                        gotz->speed = 1;
                        gotz->animationTimer = 0;

                        setEntityDirectionalAnimation(gotz->entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            gotz->animationState = 1;
                        } else {
                            gotz->animationState = 0;
                        }

                        gotz->flags |= NPC_NEEDS_UPDATE;
                    }

                    break;

                default:
                    break;
            }
        }
    
    if (gotz->animationTimer) {
        gotz->animationTimer--;
    }
    
    updateNPCMovement(GOTZ);

    }
}

void handleSashaAnimation(void) {
    npcInfo *sasha = &npcs[SASHA];


   u16 tempDirection;

    if (sasha->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(sasha->entityIndex) && !sasha->animationTimer) {

            switch (sasha->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(SASHA);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(SASHA);
                    break;

                case NPC_ANIMATION_CUSTOM:

                    // FIXME: should be inline function
                    if (sasha->animationState == 0 ) {

                        sasha->speed = 0;
                        sasha->animationTimer = 0;

                        setEntityDirectionalAnimation(sasha->entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < DIRECTION_N) {
                            sasha->direction = tempDirection * 2;
                            sasha->animationState = 1;
                        }

                        sasha->flags |= NPC_NEEDS_UPDATE;

                    } else {

                        sasha->speed = 1;
                        sasha->animationTimer = 0;

                        setEntityDirectionalAnimation(sasha->entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            sasha->animationState = 1;
                        } else {
                            sasha->animationState = 0;
                        }

                        sasha->flags |= NPC_NEEDS_UPDATE;

                    }

                    break;

                default:
                    break;
            }
        }
    
    if (sasha->animationTimer) {
        sasha->animationTimer--;
    }
    
    updateNPCMovement(SASHA);

    }

}

void handlePotionShopDealerAnimation(void) {
    npcInfo *potionShopDealer = &npcs[POTION_SHOP_DEALER];


    if (potionShopDealer->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(potionShopDealer->entityIndex) && !potionShopDealer->animationTimer) {
            updateAnimation(POTION_SHOP_DEALER);
        }

        if (potionShopDealer->animationTimer) {
            potionShopDealer->animationTimer--; 
        } 
        
        updateNPCMovement(POTION_SHOP_DEALER);

    }   

}

void handleKentAnimation(void) {
    npcInfo *kent = &npcs[KENT];


    u16 tempDirection;

    if (kent->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(kent->entityIndex) && !kent->animationTimer) {

            switch (kent->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(KENT);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(KENT);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:

                    // FIXME: should be inline function
                    if (kent->animationState == 0) {

                        kent->speed = 0;
                        kent->animationTimer = 0;
                        setEntityDirectionalAnimation(kent->entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < DIRECTION_N) {
                            kent->direction = tempDirection * 2 ;
                            kent->animationState = 1;
                        }

                        kent->flags |= NPC_NEEDS_UPDATE;

                    } else {

                        kent->speed = 2;
                        kent->animationTimer = 0;

                        // running
                        setEntityDirectionalAnimation(kent->entityIndex, 16);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            kent->animationState = 1;
                        } else {
                            kent->animationState = 0;
                        }

                        kent->flags |= NPC_NEEDS_UPDATE;
                    }

                    break;

                default:
                    break;
            }
       }    

    if (kent->animationTimer) {
        kent->animationTimer--;
    }
    
    updateNPCMovement(KENT);
    
    }
}

void handleStuAnimation(void) {
    npcInfo *stu = &npcs[STU];


    if (stu->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(stu->entityIndex) && !stu->animationTimer) {
            updateAnimation(STU);
        }

        if (stu->animationTimer) {
            stu->animationTimer--; 
        } 
        
        updateNPCMovement(STU);

    }   

}

void handleMidwifeAnimation(void) {
    npcInfo *midwife = &npcs[MIDWIFE];


    if (midwife->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(midwife->entityIndex) && !midwife->animationTimer) {
            updateAnimation(MIDWIFE);
        }

        if (midwife->animationTimer) {
            midwife->animationTimer--; 
        } 
        
        updateNPCMovement(MIDWIFE);

    }   

}

void handleMayAnimation(void) {
    npcInfo *may = &npcs[MAY];


    if (may->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(may->entityIndex) && !may->animationTimer) {
            updateAnimation(MAY);
        }

        if (may->animationTimer) {
            may->animationTimer--; 
        } 
        
        updateNPCMovement(MAY);
    
    }   

}

void handleRickAnimation(void) {
    npcInfo *rick = &npcs[RICK];


    if (rick->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(rick->entityIndex) && !rick->animationTimer) {
            updateAnimation(RICK);
        }

        if (rick->animationTimer) {
            rick->animationTimer--; 
        } 
        
        updateNPCMovement(RICK);
    
    }   

}

void handlePastorAnimation(void) {
    npcInfo *pastor = &npcs[PASTOR];


    if (pastor->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(pastor->entityIndex) && !pastor->animationTimer) {
            updateAnimation(PASTOR);
        }

        if (pastor->animationTimer) {
            pastor->animationTimer--; 
        } 
        
        updateNPCMovement(PASTOR);

    }   

}

void handleShipperAnimation(void) {
    npcInfo *shipper = &npcs[SHIPPER];


    if (shipper->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(shipper->entityIndex) && !shipper->animationTimer) {
            updateAnimation(SHIPPER);
        }

        if (shipper->animationTimer) {
            shipper->animationTimer--; 
        } 
        
        updateNPCMovement(SHIPPER);
    
    }   

}

void handleSaibaraAnimation(void) {
    npcInfo *saibara = &npcs[SAIBARA];


    if (saibara->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(saibara->entityIndex) && !saibara->animationTimer) {
            updateAnimation(SAIBARA);
        }

        if (saibara->animationTimer) {
            saibara->animationTimer--; 
        } 
        
        updateNPCMovement(SAIBARA);

    }   

}

void handleDukeAnimation(void) {
    npcInfo *duke = &npcs[DUKE];


    if (duke->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(duke->entityIndex) && !duke->animationTimer) {
            updateAnimation(DUKE);
        }

        if (duke->animationTimer) {
            duke->animationTimer--; 
        } 
        
        updateNPCMovement(DUKE);
    }
    
}

void handleGregAnimation(void) {
    npcInfo *greg = &npcs[GREG];


    if (greg->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(greg->entityIndex) && !greg->animationTimer) {
            updateAnimation(GREG);
        }

        if (greg->animationTimer) {
            greg->animationTimer--; 
        } 
        
        updateNPCMovement(GREG);

    }   

}

void handleCarpenter1Animation(void) {

    u16 temp;

    if (npcs[CARPENTER_1].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[CARPENTER_1].entityIndex) && npcs[CARPENTER_1].animationTimer == 0) {

            switch (npcs[CARPENTER_1].animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(CARPENTER_1);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(CARPENTER_1);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:

                    switch (npcs[CARPENTER_1].animationState) {

                        // idle
                        case 0:

                            npcs[CARPENTER_1].speed = 0;
                            npcs[CARPENTER_1].animationTimer = 0;

                            setEntityDirectionalAnimation(npcs[CARPENTER_1].entityIndex, 0);

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

                        // walking
                        case 1:

                            npcs[CARPENTER_1].speed = 1;
                            npcs[CARPENTER_1].animationTimer = 0;
                            setEntityDirectionalAnimation(npcs[CARPENTER_1].entityIndex, 8);

                            temp = getRandomNumberInRange(0, 19);

                            if (temp < 8) {
                                npcs[CARPENTER_1].animationState = 1;
                            } else {
                                npcs[CARPENTER_1].animationState = 0;
                            }

                            npcs[CARPENTER_1].flags |= NPC_NEEDS_UPDATE;

                            break;

                        // sawing
                        case 2:

                            npcs[CARPENTER_1].speed = 0;
                            npcs[CARPENTER_1].animationTimer = 60;
                            setEntityDirectionalAnimation(npcs[CARPENTER_1].entityIndex, 20);

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

void handleCarpenter2Animation(void) {

    u16 temp;

    if (npcs[CARPENTER_2].flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(npcs[CARPENTER_2].entityIndex) && npcs[CARPENTER_2].animationTimer == 0) {

            switch (npcs[CARPENTER_2].animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(CARPENTER_2);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(CARPENTER_2);
                    break;

                case NPC_ANIMATION_LOCATION_SPECIAL:

                    switch (npcs[CARPENTER_2].animationState) {

                        case 0:

                            npcs[CARPENTER_2].speed = 0;
                            npcs[CARPENTER_2].animationTimer = 0;

                            setEntityDirectionalAnimation(npcs[CARPENTER_2].entityIndex, 0);

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
                            setEntityDirectionalAnimation(npcs[CARPENTER_2].entityIndex, 8);

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
                            setEntityDirectionalAnimation(npcs[CARPENTER_2].entityIndex, 28);

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

void handleMasterCarpenterAnimation(void) {
    npcInfo *masterCarpenter = &npcs[MASTER_CARPENTER];

    
    if (masterCarpenter->flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(masterCarpenter->entityIndex)) {

            if (masterCarpenter->animationTimer == 0) {

                switch (masterCarpenter->animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(MASTER_CARPENTER);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(MASTER_CARPENTER);
                        break;

                    case NPC_ANIMATION_LOCATION_SPECIAL:
                        masterCarpenter->speed = 0;
                        masterCarpenter->animationState = 10;
                        masterCarpenter->animationTimer = 0;

                        // smoking pipe
                        setEntityAnimation(masterCarpenter->entityIndex, 16);

                        masterCarpenter->flags |= NPC_NEEDS_UPDATE;
                        break;

                }
                
            }
            
        }
 
        if (masterCarpenter->animationTimer) {
            masterCarpenter->animationTimer--;
        }
        
        updateNPCMovement(MASTER_CARPENTER);
        
    }

}

void handleHarvestSprite1Animation(void) {

    if (npcs[HARVEST_SPRITE_1].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_1].entityIndex)) {

            if (npcs[HARVEST_SPRITE_1].animationTimer == 0) {

                switch (npcs[HARVEST_SPRITE_1].animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(HARVEST_SPRITE_1);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(HARVEST_SPRITE_1);
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

void handleHarvestSprite2Animation(void) {

    if (npcs[HARVEST_SPRITE_2].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_2].entityIndex)) {

            if (npcs[HARVEST_SPRITE_2].animationTimer == 0) {

                switch (npcs[HARVEST_SPRITE_2].animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(HARVEST_SPRITE_2);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(HARVEST_SPRITE_2);
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

void handleHarvestSprite3Animation(void) {

    if (npcs[HARVEST_SPRITE_3].flags & NPC_ENTITY_LOADED) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_3].entityIndex)) {

            if (npcs[HARVEST_SPRITE_3].animationTimer == 0) {

                switch (npcs[HARVEST_SPRITE_3].animationMode) {

                    case NPC_ANIMATION_IDLE:
                        stopNPCMovement(HARVEST_SPRITE_3);
                        break;

                    case NPC_ANIMATION_WANDER:
                        updateNPCWanderAnimation(HARVEST_SPRITE_3);
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

void handleSydneyAnimation(void) {
    npcInfo *sydney = &npcs[SYDNEY];


    if (sydney->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(sydney->entityIndex) && !sydney->animationTimer) {
            updateAnimation(SYDNEY);
        }

        if (sydney->animationTimer) {
            sydney->animationTimer--; 
        } 
        
        updateNPCMovement(SYDNEY);

    }   

}

void handleBarleyAnimation(void) {
    npcInfo *barley = &npcs[BARLEY];


    if (barley->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(barley->entityIndex) && !barley->animationTimer) {
            updateAnimation(BARLEY);
        }

        if (barley->animationTimer) {
            barley->animationTimer--; 
        } 
        
        updateNPCMovement(BARLEY);
    
    }   

}

void handleMrsManaAnimation(void) {
    npcInfo *mrsMana = &npcs[MRS_MANA];


    if (mrsMana->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(mrsMana->entityIndex) && !mrsMana->animationTimer) {
            updateAnimation(MRS_MANA);
        }

        if (mrsMana->animationTimer) {
            mrsMana->animationTimer--; 
        } 
        
        updateNPCMovement(MRS_MANA);

    }   

}

void handleJohnAnimation(void) {
    npcInfo *john = &npcs[JOHN];


    if (john->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(john->entityIndex) && !john->animationTimer) {
            updateAnimation(JOHN);
        }

        if (john->animationTimer) {
            john->animationTimer--; 
        } 
        
        updateNPCMovement(JOHN);
    
    }   

}

void handleGourmetJudgeAnimation(void) {
    npcInfo *gourmetJudge = &npcs[GOURMET_JUDGE];


    if (gourmetJudge->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(gourmetJudge->entityIndex) && !gourmetJudge->animationTimer) {
            updateAnimation(GOURMET_JUDGE);
        }

        if (gourmetJudge->animationTimer) {
            gourmetJudge->animationTimer--; 
        } 
        
        updateNPCMovement(0x28);
    
    }   

}

void handleMariaHarrisBabyAnimation(void) {
    npcInfo *mariaHarrisBaby = &npcs[MARIA_HARRIS_BABY];


    if (mariaHarrisBaby->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(mariaHarrisBaby->entityIndex) && mariaHarrisBaby->animationTimer == 0) {

            switch (mariaHarrisBaby->animationMode) {

                case NPC_ANIMATION_IDLE:
                    mariaHarrisBaby->speed = 0;
                    mariaHarrisBaby->animationState = 10;
                    mariaHarrisBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(mariaHarrisBaby->entityIndex, 8);

                    mariaHarrisBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIMATION_WANDER:
                    mariaHarrisBaby->speed = 0;
                    mariaHarrisBaby->animationState = 10;
                    mariaHarrisBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(mariaHarrisBaby->entityIndex, 0);

                    mariaHarrisBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (mariaHarrisBaby->animationTimer) {
            mariaHarrisBaby->animationTimer--; 
        } 
        
        updateNPCMovement(MARIA_HARRIS_BABY);
    
    }   
    
}

void handlePopuriGrayBabyAnimation(void) {
    npcInfo *popuriGrayBaby = &npcs[POPURI_GRAY_BABY];


    if (popuriGrayBaby->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(popuriGrayBaby->entityIndex) && popuriGrayBaby->animationTimer == 0) {

            switch (popuriGrayBaby->animationMode) {

                case NPC_ANIMATION_IDLE:
                    popuriGrayBaby->speed = 0;
                    popuriGrayBaby->animationState = 10;
                    popuriGrayBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(popuriGrayBaby->entityIndex, 8);

                    popuriGrayBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIMATION_WANDER:
                    popuriGrayBaby->speed = 0;
                    popuriGrayBaby->animationState = 10;
                    popuriGrayBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(popuriGrayBaby->entityIndex, 0);

                    popuriGrayBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (popuriGrayBaby->animationTimer) {
            popuriGrayBaby->animationTimer--; 
        } 
        
        updateNPCMovement(POPURI_GRAY_BABY);
    
    }   
    
}

void handleElliJeffBabyAnimation(void) {
    npcInfo *elliJeffBaby = &npcs[ELLI_JEFF_BABY];


    if (elliJeffBaby->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(elliJeffBaby->entityIndex) && elliJeffBaby->animationTimer == 0) {

            switch (elliJeffBaby->animationMode) {

                case NPC_ANIMATION_IDLE:
                    elliJeffBaby->speed = 0;
                    elliJeffBaby->animationState = 10;
                    elliJeffBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(elliJeffBaby->entityIndex, 8);

                    elliJeffBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIMATION_WANDER:
                    elliJeffBaby->speed = 0;
                    elliJeffBaby->animationState = 10;
                    elliJeffBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(elliJeffBaby->entityIndex, 0);

                    elliJeffBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (elliJeffBaby->animationTimer) {
            elliJeffBaby->animationTimer--; 
        } 
        
        updateNPCMovement(ELLI_JEFF_BABY);
    
    }   
    
}

void handleAnnCliffBabyAnimation(void) {
    npcInfo *annCliffBaby = &npcs[ANN_CLIFF_BABY];


    if (annCliffBaby->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(annCliffBaby->entityIndex) && annCliffBaby->animationTimer == 0) {

            switch (annCliffBaby->animationMode) {

                case NPC_ANIMATION_IDLE:
                    annCliffBaby->speed = 0;
                    annCliffBaby->animationState = 10;
                    annCliffBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(annCliffBaby->entityIndex, 8);

                    annCliffBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIMATION_WANDER:
                    annCliffBaby->speed = 0;
                    annCliffBaby->animationState = 10;
                    annCliffBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(annCliffBaby->entityIndex, 0);

                    annCliffBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }   

        if (annCliffBaby->animationTimer) {
            annCliffBaby->animationTimer--; 
        } 
        
        updateNPCMovement(ANN_CLIFF_BABY);
    
    }    
    
}

void handleKarenKaiBabyAnimation(void) {
    npcInfo *karenKaiBaby = &npcs[KAREN_KAI_BABY];


    if (karenKaiBaby->flags & NPC_ENTITY_LOADED) {
        
        if (checkEntityAnimationStateChanged(karenKaiBaby->entityIndex) && karenKaiBaby->animationTimer == 0) {

            switch (karenKaiBaby->animationMode) {

                case NPC_ANIMATION_IDLE:
                    karenKaiBaby->speed = 0;
                    karenKaiBaby->animationState = 10;
                    karenKaiBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(karenKaiBaby->entityIndex, 8);

                    karenKaiBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

                case NPC_ANIMATION_WANDER:
                    karenKaiBaby->speed = 0;
                    karenKaiBaby->animationState = 10;
                    karenKaiBaby->animationTimer = 0;

                    setEntityDirectionalAnimation(karenKaiBaby->entityIndex, 0);

                    karenKaiBaby->flags |= NPC_NEEDS_UPDATE;
                    break;

            }
        }

        if (karenKaiBaby->animationTimer) {
            karenKaiBaby->animationTimer--; 
        } 
        
        updateNPCMovement(KAREN_KAI_BABY);
    
    }   
    
}

void handleEntomologistAnimation(void) {
    npcInfo *entomologist = &npcs[ENTOMOLOGIST];


    if (entomologist->flags & NPC_ENTITY_LOADED) {
    
        if (checkEntityAnimationStateChanged(entomologist->entityIndex) && entomologist->animationTimer == 0) {

            switch (entomologist->animationMode) {

                case NPC_ANIMATION_IDLE:
                    stopNPCMovement(ENTOMOLOGIST);
                    break;

                case NPC_ANIMATION_WANDER:
                    updateNPCWanderAnimation(ENTOMOLOGIST);
                    break;

            }
        }

        if (entomologist->animationTimer) {
            entomologist->animationTimer--; 
        } 
        
        updateNPCMovement(ENTOMOLOGIST);
    
    }   
    
}

static inline bool checkHoldingItem() {
    return gPlayer.heldItem != 0;
}

// show the affection heart icon for a marriage-candidate NPC
static inline void showAffectionHeart(u8 index) {

    switch (index) {
        case MARIA:
        case POPURI:
        case ELLI:
        case ANN:
        case KAREN:
            setOverlayIconSprite(0, 120, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
            setOverlayIconSprite(1, 120, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
            break;
    }

}

// initiates dialogue with NPC on button press or pick up baby
u8 checkNPCInteraction(void) {
    
    u8 result = 0;
    u8 i = 0;
    
    u16 dialogueEntryIndex = 0xFFFF;

    do {

        if (npcs[i].flags & NPC_ENTITY_LOADED) {
            
            if (entities[npcs[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[npcs[i].entityIndex].buttonPressed == BUTTON_A) {

                if (i == BABY && gBabyAge < 120) {
                                 
                    if (gPlayer.heldItem == 0) {
         
                        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {

                            if (gBabyAge >= 30) {
                                gPlayer.heldItem = 0xC2;
                            } else {
                                gPlayer.heldItem = 0xBA;  
                            }

                            if (!checkDailyEventBit(HELD_BABY)) {
                                npcAffection[BABY] += adjustValue(npcAffection[BABY], 2, MAX_AFFECTION);
                                setDailyEventBit(HELD_BABY);
                            }
                            
                            deactivateEntity(npcs[i].entityIndex);
                         
                            result = 2;
                            npcs[i].flags &= ~NPC_ENTITY_LOADED;
                           
                        } 
                        
                        dialogueEntryIndex = 0xFFFF;
                        
                    } else {
                        dialogueEntryIndex = 1;
                    } 
                                 
                } else {
                    // use entry point 1 if player holding item, otherwie use entry point 0
                    dialogueEntryIndex = checkHoldingItem();
                }
   
                if (dialogueEntryIndex != 0xFFFF) {

                    // show heart icon
                    showAffectionHeart(i);
                    
                    showMessageBox(0, npcToDialogueBytecodeIndex[i], dialogueEntryIndex, 0, 0);
                    result = 1;
                    npcs[i].animationMode = NPC_ANIMATION_TALKING;
                    
                } 
            
            }    

        }
        
        i++;
            
    } while (i < MAX_NPCS && result == 0);

    return result;
    
}

bool findNPCToTalkTo(void) {

    bool found = FALSE;
    u8 i = 0;
    
    npcTalkingTo = 0xFF;

    while (i < MAX_NPCS && !found) {

        if (npcs[i].flags & NPC_ENTITY_LOADED) {

            if (entities[npcs[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {
                npcs[i].animationMode = NPC_ANIMATION_FACE_PLAYER;
                npcTalkingTo = i;
                found = TRUE;
            }

        }  

        i++;

    } 
    
    return found;

}

bool getBlueFeatherResponse(u8 index, u16 dialogueEntryIndex) {

    bool result;
    
    result = FALSE;
    
    if (npcs[index].flags & NPC_ENTITY_LOADED) {

        // show heart
        showAffectionHeart(index);

        showMessageBox(0, npcToDialogueBytecodeIndex[index], dialogueEntryIndex, 0, 0);
        result = TRUE;
        
    }
    
    return result;

}