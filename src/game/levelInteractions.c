#include "common.h"

#include "game/levelInteractions.h"

#include "system/controller.h"
#include "system/cutscene.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/overlayScreens.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/time.h"
#include "game/tv.h"
#include "game/transition.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

// bss
static LevelInteractionsInfo levelInteractionsInfo;



// forward declarations
// TODO: label
bool handleFarmLevelInteractions(u16, u8);
bool handleHouseLevelInteractions(u16, u8);
bool handleBarnLevelInteractions(u16, u8);
bool handleCoopLevelInteractions(u16, u8);
bool handleKitchenLevelInteractions(u16, u8);
bool handleBathroomLevelInteractions(u16, u8);
bool handleGreenhouseLevelInteractions(u16, u8);
bool handleVillage1LevelInteractions(u16, u8);
bool handleFlowerShopLevelInteractions(u16, u8);
bool handlePopuriRoomLevelInteractions(u16, u8);
bool handleBakeryLevelInteractions(u16, u8);
bool handleElliRoomLevelInteractions(u16, u8);
bool handleRickStoreLevelInteractions(u16, u8);
bool handleSouvenirShopLevelInteractions(u16, u8);
bool handleChurchLevelInteractions(u16, u8);
bool handleTavernLevelInteractions(u16, u8);
bool handleVillage2LevelInteractions(u16, u8);
bool handleLibraryLevelInteractions(u16, u8);
bool handleMidwifeHouseLevelInteractions(u16, u8);
bool handleMayorHouseLevelInteractions(u16, u8);
bool handleMariaRoomLevelInteractions(u16, u8);
bool handlePotionShopLevelInteractions(u16, u8);
bool handlePotionShopBedroomLevelInteractions(u16, u8);
bool handleSquareLevelInteractions(u16, u8);
bool handleMountain1LevelInteractions(u16, u8);
bool handleMountain2LevelInteractions(u16, u8);
bool handleTopOfMountain1LevelInteractions(u16, u8);
bool handleMoonMountainLevelInteractions(u16, u8);
bool handleRoadLevelInteractions(u16, u8);
bool handleCarpenterHutLevelInteractions(u16, u8);
bool handleDumplingHouseLevelInteractions(u16, u8);
bool handleCaveLevelInteractions(u16, u8);
bool handlePondLevelInteractions(u16, u8);
bool handleVineyardLevelInteractions(u16, u8);
bool handleVineyardHouseLevelInteractions(u16, u8);
bool handleKarenRoomLevelInteractions(u16, u8);
bool handleVineyardCellarLevelInteractions(u16, u8);
bool handleVineyardCellarBasementLevelInteractions(u16, u8);
bool handleRaceTrackLevelInteractions(u16, u8);
bool handleRanchLevelInteractions(u16, u8);
bool handleRanchHouseLevelInteractions(u16, u8);
bool handleRanchStoreLevelInteractions(u16, u8);
bool handleAnnRoomLevelInteractions(u16, u8);
bool handleRanchBarnLevelInteractions(u16, u8);
bool handleBeachLevelInteractions(u16, u8);
bool handleHarvestSpriteCaveLevelInteractions(u16, u8);
bool handleMineLevelInteractions(u16, u8);

                                                                  
//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkShippingBinInteraction);

u8 checkShippingBinInteraction(u16 mapIndex) {

    u8 result = 0xFF;
    
    u8 temp;
    
    if ((mapIndex == FARM || mapIndex == COOP || mapIndex == BARN || mapIndex == GREENHOUSE)) {

        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);

        if (temp == 0x10) {
            result = temp;
        }
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkBarnAnimalFeedInteraction);

u8 checkBarnAnimalFeedInteraction(u16 mapIndex) {

    u8 result = 0xFF;
    
    u8 temp;
    u8 temp2;
    
    if (mapIndex == BARN) {

        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);
        
        if (17 < temp && temp < 27) {
            result = temp;
        }
    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkChickenFeedInteraction);

bool checkChickenFeedInteraction(u16 mapIndex) {

    u8 temp;
    u8 result = 0xFF;
    
    if (mapIndex == COOP) {
        
        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);

        if (18 < temp && temp < 25) {
            result = temp;
        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkFarmDogBowlInteraction);

u8 checkFarmDogBowlInteraction(u16 mapIndex) {

    u8 result = 0xFF;
    
    if (mapIndex == FARM) {

        // autodecomp of ternany for reference
        // -(cVar1 != '\x17') | 1;
        //result = -(getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) != 0x17) | 1;
        //result = -((getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) != 0x17) ? 1 : 0) | 1;        

        result = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x17 ? 1 : 0xFF;

    }

    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkWaterSourceInteraction);

u8 checkWaterSourceInteraction(u16 mapIndex) {
    
    u8 result;
    u8 temp;
     
    result = 0xFF;
    
    if (mapIndex == FARM) {
        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);
        result = ((0x1A < temp && temp < 0x1D) || temp == 0x1D) == FALSE ? 0xFF : 1;
    }
    
    if (mapIndex == MOUNTAIN_1) {
        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);
        if (temp == 0x10 || temp == 0x14) {
            result = TRUE;
        }
    }
    
    if (mapIndex == POND) {
        if (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x10) {
            result = TRUE;
        }
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkWaterableSoilInteraction);

bool checkWaterableSoilInteraction(u16 mapIndex) {

    bool result = FALSE;
    u8 temp;
    
    if (mapIndex == FARM) {
        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);
        result = ((0x1A < temp && temp < 0x1D) || temp == 0x1D);
    }

    if (mapIndex == GREENHOUSE) {
        if (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x12) { 
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkWineBarrelInteraction);

bool checkWineBarrelInteraction(u16 mapIndex) {

    bool result = FALSE;
    
    if (mapIndex == VINEYARD_CELLAR_BASEMENT) {
        result = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x11;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", checkFishingSpotInteraction);

bool checkFishingSpotInteraction(u16 mapIndex) {

    bool result = FALSE;
    u8 levelInteractionIndex;
    
    if (mapIndex == MOUNTAIN_1) {

        levelInteractionIndex = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 64.0f);

        if (levelInteractionIndex == 0x10 || levelInteractionIndex == 0x14) {
            gPlayer.fishingSpotType = 0;
            result = TRUE;
        }
        
        if (levelInteractionIndex == 0x12) {
            gPlayer.fishingSpotType = 1;
            result = TRUE;
        }

    }
    
    if (mapIndex == MOUNTAIN_2) {
        if (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 64.0f) == 0x10) {
            gPlayer.fishingSpotType = 2;
            result = TRUE;
        }
    }
    
    if (mapIndex == BEACH) {
        if (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 64.0f) == 0x10) {
            gPlayer.fishingSpotType = 3;
            result = TRUE;
        }
    }
    
    return result;
    
} 

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleLevelInteraction);

bool handleLevelInteraction(u16 mapIndex) {

    bool canEnter;
    s32 tempSpawnPoint;
    u8 levelInteractionIndex;
    u16 textIndex;

    tempSpawnPoint = gSpawnPointIndex;
    
    levelInteractionsInfo.mapAdditionsIndex = 0xFF;
    levelInteractionsInfo.interactionSfxIndex = 0xFF;

    levelInteractionIndex = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);

    // checked by cutscene bytecode for triggering cutscene flows based on player position
    cutsceneLevelInteractionIndex = levelInteractionIndex;
    
    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
        // checked by cutscene bytecode
        // i.e., cutscene executor will idle until this value is set to a particular value 
        interactedWithLevelInteractionIndex = cutsceneLevelInteractionIndex;
    } else {
        interactedWithLevelInteractionIndex = 0xFF;
    }

    canEnter = FALSE;
    
    if (levelInteractionIndex) {

        switch (mapIndex) {

            case FARM:
                canEnter = handleFarmLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case HOUSE:
                canEnter = handleHouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case BARN:
                canEnter = handleBarnLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case COOP:
                canEnter = handleCoopLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case KITCHEN:
                canEnter = handleKitchenLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case BATHROOM:
                canEnter = handleBathroomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case GREENHOUSE:
                canEnter = handleGreenhouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case VILLAGE_1:
                canEnter = handleVillage1LevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case FLOWER_SHOP:
                canEnter = handleFlowerShopLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case POPURI_ROOM:
                canEnter = handlePopuriRoomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case BAKERY:
                canEnter = handleBakeryLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case ELLI_ROOM:
                canEnter = handleElliRoomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case RICK_STORE:
                canEnter = handleRickStoreLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case SOUVENIR_SHOP:
                canEnter = handleSouvenirShopLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case CHURCH:
                canEnter = handleChurchLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case TAVERN:
                canEnter = handleTavernLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case VILLAGE_2:
                canEnter = handleVillage2LevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case LIBRARY:
                canEnter = handleLibraryLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MIDWIFE_HOUSE:
                canEnter = handleMidwifeHouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MAYOR_HOUSE:
                canEnter = handleMayorHouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MARIA_ROOM:
                canEnter = handleMariaRoomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case POTION_SHOP:
                canEnter = handlePotionShopLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case POTION_SHOP_BEDROOM:
                canEnter = handlePotionShopBedroomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case SQUARE:
                canEnter = handleSquareLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MOUNTAIN_1:
                canEnter = handleMountain1LevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MOUNTAIN_2:
                canEnter = handleMountain2LevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case TOP_OF_MOUNTAIN_1:
                canEnter = handleTopOfMountain1LevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MOON_MOUNTAIN:
                canEnter = handleMoonMountainLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case ROAD:
                canEnter = handleRoadLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case CARPENTER_HUT:
                canEnter = handleCarpenterHutLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case DUMPLING_HOUSE:
                canEnter = handleDumplingHouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case CAVE:
                canEnter = handleCaveLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case POND:
                canEnter = handlePondLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case VINEYARD:
                canEnter = handleVineyardLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case VINEYARD_HOUSE:
                canEnter = handleVineyardHouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case KAREN_ROOM:
                canEnter = handleKarenRoomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case VINEYARD_CELLAR:
                canEnter = handleVineyardCellarLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case VINEYARD_CELLAR_BASEMENT:
                canEnter = handleVineyardCellarBasementLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case RACE_TRACK:
                canEnter = handleRaceTrackLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case RANCH:
                canEnter = handleRanchLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case RANCH_HOUSE:
                canEnter = handleRanchHouseLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case RANCH_STORE:
                canEnter = handleRanchStoreLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case ANN_ROOM:
                canEnter = handleAnnRoomLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case RANCH_BARN:
                canEnter = handleRanchBarnLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case BEACH:
                canEnter = handleBeachLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case HARVEST_SPRITE_CAVE:
                canEnter = handleHarvestSpriteCaveLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            case MINE:
                canEnter = handleMineLevelInteractions(mapIndex, levelInteractionIndex);
                break;
            default:
                break;

        }
    }

    if (canEnter == 1) {
        setPlayerAction(CONTROLLER_INPUT, ANIM_DEFAULT);
    }

    if (levelInteractionsInfo.interactionSfxIndex != 0xFF) {
        playSfx(levelInteractionsInfo.interactionSfxIndex);
    }

    if (tempSpawnPoint != gSpawnPointIndex) {
        
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            
            textIndex = getCantEnterTextIndex(gSpawnPointIndex);
         
            if (gSpawnPointIndex == 0x5F) {

                if (!checkLifeEventBit(MARRIED)) {

                    if (gSeason == WINTER) {
                        if (gDayOfMonth == 24 && (18 < gHour && gHour < 21)) {
                            textIndex = 0xFFFF;
                        }
                        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
                            textIndex = 0xFFFF;
                        }
                    }

                } else {

                    if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
                        textIndex = 0xFFFF;
                    }

                }

            }

            if ((gSpawnPointIndex == 0x59 || gSpawnPointIndex == 0x5B) && !checkLifeEventBit(MARRIED) && gSeason == SUMMER && gDayOfMonth == 1 && (18 < gHour && gHour < 21)) {
                textIndex = 0xFFFF;
            }
    
            if (textIndex == 0xFFFF) {

                if (levelInteractionsInfo.mapAdditionsIndex != 0xFF) {
                    activateMapAddition(MAIN_MAP_INDEX, levelInteractionsInfo.mapAdditionsIndex, 0);
                }
                
                handleExitLevel(8, MAP_LOAD);
                return canEnter;
                
            } else {

                switch (getMapForSpawnPoint(gSpawnPointIndex)) {                           
                    case KAREN_ROOM:                                  
                        showTextBox(0, SHOP_TEXT_INDEX, textIndex, 0, 0);
                        setPlayerAction(WALKING_DOWN_STAIRS, ANIM_DEFAULT);
                        gPlayer.actionTimer = 32;
                        gPlayer.savedDirection = SOUTHEAST;
                        break;
                    case POPURI_ROOM:                               
                    case MARIA_ROOM:                                
                    case ANN_ROOM:                                  
                    case ELLI_ROOM:                    
                        showTextBox(0, SHOP_TEXT_INDEX, textIndex, 0, 0);
                        break;
                    default:            
                        showTextBox(1, SHOP_TEXT_INDEX, textIndex, 0, 2);
                        break;
                    }
                    
                gSpawnPointIndex = tempSpawnPoint;

            }

        } else {
            canEnter = FALSE;
        }
        
        gSpawnPointIndex = tempSpawnPoint;
        
    } 
    
    return canEnter;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleFarmLevelInteractions);

bool handleFarmLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    u8 letterIndex;
    
    switch (levelInteractionIndex) {

        case 1:
        case 2:
            break;
            break;
         
        case 3:        
        
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_1);
            }
            
            return result;
        
        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == NORTHEAST) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 9;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    setSpawnPoint(HOUSE_SPAWN_POINT_1);
                }
            }

            break;
        
        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == NORTHEAST) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0xD;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    setSpawnPoint(BATHROOM_SPAWN_POINT_2);
                }
            }

            break;
        
        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0xA;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(BARN_SPAWN_POINT_1);
            }

            break;
        
        case 7:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0xB;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(COOP_SPAWN_POINT_1);
            }

            break;

        case 8:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 0x14;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(GREENHOUSE_SPAWN_POINT_1);
            }

            break;

        case 9:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == SOUTHEAST) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0xC;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    setSpawnPoint(HOUSE_SPAWN_POINT_2);
                }
            }

            break;
        
        case 26:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x70);
            }

            break;
        
        // lumber storage
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem == 0) {
                
                    if (!(gPlayer.flags & 1)) {
                    
                        if (gLumber != 0) {
                            gPlayer.heldItem = 3;
                            setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                            gLumber += adjustValue(gLumber, -1, MAX_LUMBER);
                            result = 2;
                        }

                    }

                }
            }

            break;

        // mailbox
        case 18:
        
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                // check if receive photo or item in mail 
                letterIndex = readMail(); 
 
                if (letterIndex != 0xFF) {

                    setAlbumPictureFromLetter(letterIndex);
                    showTextBox(1, LETTERS_TEXT_INDEX, getTextIndexFromLetterIndex(letterIndex), 0, 2);
                    
                    if (letterIndex == 35) {
                        acquireKeyItem(GOLD_PENDANT);
                        setSpecialDialogueBit(HAVE_GOLD_PENDANT);
                    }

                } else {
                    showTextBox(1, LETTERS_TEXT_INDEX, 58, 0, 2);
                }

                result = TRUE;
            }
            
            break;
        
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.direction == NORTHWEST) {
                    convertNumberToGameVariableString(0x18, gLumber, 0);
                    showTextBox(1, SHOP_TEXT_INDEX, 0xA3, 0, 2);
                    result = TRUE;
                }
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                convertNumberToGameVariableString(0x17, fodderQuantity, 0);
                showTextBox(1, SHOP_TEXT_INDEX, 0xA2, 0, 2);
                result = TRUE;
            }

            break;
        
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xA1, 0, 2);
                result = TRUE;
            }

            break;
        
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!checkHaveKeyItem(TREASURE_MAP)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xA7, 0, 2);
                    acquireKeyItem(TREASURE_MAP);
                    result = TRUE;
                    toolUse.musicBoxTileDigCounter = 0;
                }

            }

            break;

        case 30:
            break;

    }

    return result;
   
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleHouseLevelInteractions);

bool handleHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {

        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gWeather != TYPHOON) {
                    result = TRUE;
                    setSpawnPoint(FARM_SPAWN_POINT_1);
                }
            }
            break;     

        case 2:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gWeather != TYPHOON) {
                levelInteractionsInfo.mapAdditionsIndex = 2;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
                if (checkLifeEventBit(HAVE_KITCHEN)) {                   
                    setSpawnPoint(KITCHEN_SPAWN_POINT_1);
                } else {
                    setSpawnPoint(FARM_SPAWN_POINT_9);
                }
            }            
            break;

        case 3:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gWeather != TYPHOON) {
                    result = TRUE;
                    setSpawnPoint(0x6F);
                }
            }
            break;

        // diary
        case 0x10:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0 && checkLifeEventBit(MAYOR_TOUR) && !checkDailyEventBit(0x15)) {
                showPinkOverlayText(0);
                result = TRUE; 
            }
            break;

        // TV
        case 0x12:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.direction == NORTHEAST) {
                loadTVCButtonIcons();
                initializeTVAssets();
                setPlayerAction(WATCHING_TV, ANIM_WATCHING_TV);
                levelInteractionsInfo.interactionSfxIndex = 0x2F;
                result = 2;
            }
            break;

        case 0x13:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                activateMapAddition(MAIN_MAP_INDEX, 3, 1);
                loadOverlayScreen(10, TOOLBOX_LOAD);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
            }
            break;

        case 0x14:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(CALENDAR_LOAD);
                levelInteractionsInfo.interactionSfxIndex  = MENU_OPEN_SFX;
                result = TRUE;
            }
            break;

        case 0x15:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) != 0) {
                activateMapAddition(MAIN_MAP_INDEX, 10, 1);
                loadOverlayScreen(10, CABINET_LOAD);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
            }
            break;

        // baby
        case 0x16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0) {

                if (checkDailyEventBit(0x16)) {

                    if (gBabyAge < 30) {

                        gPlayer.heldItem = 0xBA;
                        
                        deactivateEntity(npcs[BABY].entityIndex);
                        npcs[BABY].flags &= ~4;

                        setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);

                        if (!checkDailyEventBit(0x55)) {
                            npcAffection[BABY] += adjustValue(npcAffection[BABY], 2, MAX_AFFECTION);
                            setDailyEventBit(0x55);
                        }
                        
                        toggleDailyEventBit(0x16);
                        result = 2;

                    }

                }
            }    

            break;

        case 0x21:
        default:
            break;
            
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleBarnLevelInteractions);

u8 handleBarnLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;
    s32 temp;
    u16 temp2;

    switch (levelInteractionIndex) {
        
        case 1:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(FARM_SPAWN_POINT_3);
            }
            
            break;

        case 17:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && fodderQuantity != 0) {
                        gPlayer.heldItem = FODDER_HELD_ITEM;
                        setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                        fodderQuantity += adjustValue((s32) fodderQuantity, -1, 999);
                        result = 2;
                }
            }
            
            break;

        case 18:
            
            // if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            //     // unused code    
            // }
            break;

        case 19:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && (gFarmAnimals[0].flags & 1)) {

                    temp = gFarmAnimals[0].type;
                    
                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[0].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(0x15, gFarmAnimals[0].birthdayDayOfMonth, 1);
                            generateMilkTypeString(0);
                            setGameVariableString(0x26, gFarmAnimals[0].motherName, 6);
                            func_80061690(gFarmAnimals[0].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xE5, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xED, 0, 2);
                        }
                                    
                    } 
                    
                    result = 1;

                }
            }
            
            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[1].flags & 1) {

                    switch (gFarmAnimals[1].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                            setGlobalSeasonName(gFarmAnimals[1].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[1].birthdayDayOfMonth, 1);
                            generateMilkTypeString(1);
                            setGameVariableString(0x26, gFarmAnimals[1].motherName, 6);
                            func_80061690(gFarmAnimals[1].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xE6, 0, 2);
                            break;

                        case PREGNANT_COW:
                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, SHOP_TEXT_INDEX, 0xEE, 0, 2);
                            break;

                    }
                
                    result = 1;
                
                }
            
            }
            
            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[2].flags & 1) {

                    temp = gFarmAnimals[2].type;
                
                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[2].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[2].birthdayDayOfMonth, 1);
                            
                            generateMilkTypeString(2);
                            
                            setGameVariableString(38, gFarmAnimals[2].motherName, 6);
                            func_80061690(gFarmAnimals[2].milkType);

                            showTextBox(1, SHOP_TEXT_INDEX, 0xE7, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xEF, 0, 2);
                        }

                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[3].flags & 1) {

                    temp = gFarmAnimals[3].type;
                    
                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[3].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[3].birthdayDayOfMonth, 1);
                            generateMilkTypeString(3);
                            setGameVariableString(38, gFarmAnimals[3].motherName, 6);
                            func_80061690(gFarmAnimals[3].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xE8, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xF0, 0, 2);
                        }

                    } 

                    result = 1;
        
                }
            }
            
            break;

        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[4].flags & 1) {

                    temp = gFarmAnimals[4].type;

                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[4].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[4].birthdayDayOfMonth, 1);
                            generateMilkTypeString(4);
                            setGameVariableString(0x26, gFarmAnimals[4].motherName, 6);
                            func_80061690(gFarmAnimals[4].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xE9, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xF1, 0, 2);
                        }
                        
                        
                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[5].flags & 1) {

                    temp = gFarmAnimals[5].type;
                    
                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[5].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[5].birthdayDayOfMonth, 1);
                            generateMilkTypeString(5);
                            setGameVariableString(0x26, gFarmAnimals[5].motherName, 6);
                            func_80061690(gFarmAnimals[5].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xEA, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xF2, 0, 2);
                        }
                        
                        
                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[6].flags & 1) {

                    temp = gFarmAnimals[6].type;
                    
                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[6].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[6].birthdayDayOfMonth, 1);
                            generateMilkTypeString(6);
                            setGameVariableString(0x26, gFarmAnimals[6].motherName, 6);
                            func_80061690(gFarmAnimals[6].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xEB, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xF3, 0, 2);
                        }

                    } 

                    result = 1; 

                }
            }
            
            break;

        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[7].flags & 1) {

                    temp = gFarmAnimals[7].type;
                    
                    // FIXME: actually a switch
                    if (temp >= 0) {

                        if (temp < 4) {
                            
                            setGlobalSeasonName(gFarmAnimals[7].birthdaySeason);
                        
                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];
                        
                            convertNumberToGameVariableString(0x15, gFarmAnimals[7].birthdayDayOfMonth, 1);
                            generateMilkTypeString(7);
                            setGameVariableString(38, gFarmAnimals[7].motherName, 6);
                            func_80061690(gFarmAnimals[7].milkType);
                            showTextBox(1, SHOP_TEXT_INDEX, 0xEC, 0, 2);
                            
                        } else if (temp < 7) {
                            showTextBox(1, SHOP_TEXT_INDEX, 0xF4, 0, 2);
                        }
                            
                    } 

                    result = 1;
                    
                }
            }
            
            break;

        case 27:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                convertNumberToGameVariableString(0x17, fodderQuantity, 0);
                showTextBox(1, SHOP_TEXT_INDEX, 0xA8, 0, 2);
                result = 1;
            }
            
            break;
        
        case 28:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xA9, 0, 2);
                result = 1;
            }
            
            break;

        default:
            break;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleCoopLevelInteractions);

bool handleCoopLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(FARM_SPAWN_POINT_4);
            }
            break;
        
        case 18:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xAA, 0, 2);
                result = TRUE;
            }
            break;

        case 19:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[0].flags & 1) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xF5, 0, 2);
                    result = TRUE;
                }
            }
            break;
        
        case 20:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[1].flags & 1) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xF6, 0, 2);
                    result = TRUE;
                }
            }
            break;
        
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[2].flags & 1) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xF7, 0, 2);
                    result = TRUE;
                }   
            }           
            break;
        
        case 22:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[3].flags & 1) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xF8, 0, 2);
                    result = TRUE;
                }         
            }       
            break;
        
        case 23:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[4].flags & 1) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xF9, 0, 2);
                    result = TRUE;
                }
            }       
            break;
        
        case 24:     
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[5].flags & 1) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0xFA, 0, 2);
                    result = TRUE;
                }          
            }       
            break;        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleKitchenLevelInteractions);

bool handleKitchenLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(HOUSE_SPAWN_POINT_2);
            }
            break;
        case 2:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 2;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(BATHROOM_SPAWN_POINT_1);
            }        
            break;
        case 16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                activateMapAddition(MAIN_MAP_INDEX, 0, 1);
                loadOverlayScreen(10, FREEZER_LOAD);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
            }
            break;
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(RECIPE_BOOK_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN_SFX;
                result = TRUE;
            }
            break;
        case 22:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN_SFX;
                result = TRUE;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleBathroomLevelInteractions);

bool handleBathroomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:
            
            result = TRUE;
            
            if (checkLifeEventBit(HAVE_KITCHEN)) {
                setSpawnPoint(KITCHEN_SPAWN_POINT_2);
            } else {            
                setSpawnPoint(FARM_SPAWN_POINT_7);
            }

            break;

        case 2:
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && entities[ENTITY_PLAYER].coordinates.z >= -44.0f && entities[ENTITY_PLAYER].coordinates.z < -36.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(BATHING, ANIM_DEFAULT);
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    result = 2;
                }
            }         

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && entities[ENTITY_PLAYER].coordinates.x >= 42.0f && entities[ENTITY_PLAYER].coordinates.x < 50.0f) {
                    setDailyEventBit(6);
                    setPlayerAction(USING_TOILET, ANIM_DEFAULT);
                    gHappiness += adjustValue(gHappiness, 2, 0xFF);
                    result = 2;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                }
            }            

            break;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleGreenhouseLevelInteractions);

bool handleGreenhouseLevelInteractions(u16 arg0, u8 levelInteractionIndex) {

    bool result = FALSE;

    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
            result = TRUE;
            setSpawnPoint(FARM_SPAWN_POINT_8);
        }
    } 
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleVillage1LevelInteractions);

bool handleVillage1LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_4);
            }
            break;
        
        case 2:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x3F);
            }
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(0x56);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST) {
                    
                    if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                        showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                        result = TRUE;
                    } else {
                        levelInteractionsInfo.mapAdditionsIndex = 1;
                        result = TRUE;
                        setSpawnPoint(0x59);
                    }
                    
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                        
                }
                
            }
        
            break;

        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 4;
                    result = TRUE;
                    setSpawnPoint(0x5D);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 3;
                    result = TRUE;
                    setSpawnPoint(0x5E);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 7:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setSpawnPoint(0x5F);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 8:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setSpawnPoint(0x60);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 9:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gPlayer.direction == NORTHWEST) {
                    result = TRUE;
                    setSpawnPoint(0x61);
                }
            }
            
            break;

        case 10:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gPlayer.direction == NORTHWEST) {
                    result = TRUE;
                    setSpawnPoint(0x63);
                }
            }
            
            break;

        case 37:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gPlayer.direction == WEST) {
                    result = TRUE;
                    setSpawnPoint(0x63);
                }
            }
            
            break;

        case 38:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gPlayer.direction == NORTH) {
                    result = TRUE;
                    setSpawnPoint(0x63);
                }
            }
            
            break;

        case 39:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gPlayer.direction == WEST) {
                    result = TRUE;
                    setSpawnPoint(0x61);
                }
            }
            
            break;

        case 40:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                if (gPlayer.direction == NORTH) {
                    result = TRUE;
                    setSpawnPoint(0x61);
                }
            }
            
            break;

        case 11:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x44);
            }
            
            break;

        case 12:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;

                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 2;
                    result = TRUE;
                    setSpawnPoint(0x5B);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
    }
    
    return result;
   
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleFlowerShopLevelInteractions);

u8 handleFlowerShopLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;
    
    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x34);
            }
            break;
        
        case 2:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setSpawnPoint(0x58);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 16:

            if (gPlayer.direction == NORTHEAST) {
                
                if (gSeason == WINTER && (24 < gDayOfMonth && gDayOfMonth < 30)) {

                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                        if (gPlayer.heldItem == 0) {
                            if (adjustValue(flowerShopPoints, -10, 100) == -10) {
                                setMainLoopCallbackFunctionIndex(LOTTERY_LOAD);
                                result = 1; 
                            } else {
                                showTextBox(0, FESTIVALS_TEXT_INDEX, 0x3E, 0, 0);
                                result = 1;
                            }
                        }
                        result = 1;
                    }
                    
                } else {
                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                        showMessageBox(0, 0x3D, 0x27, 0x80, 0);
                        result = 1;
                    }
                }
                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                switch (gSeason) {
                    case SPRING:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xE, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xF, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, SHOP_TEXT_INDEX, 0x10, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xC9, 0, 2);
                        break;
                    
                }

                result = 1;
        
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                switch (gSeason) {

                    case SPRING:
                        showTextBox(1, SHOP_TEXT_INDEX, 0x11, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0x12, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, SHOP_TEXT_INDEX, 0x13, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0x14, 0, 2);
                        break;
                    
                }

                result = 1;
                
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0)) {
                        handlePickUpShopItem(0);
                    }
                    result = 2;
                }
            
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(1)) {
                        handlePickUpShopItem(1);
                    }
                    result = 2;
                }
            
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(2)) {
                        handlePickUpShopItem(2);
                    }
                    result = 2;
                }
            
            }

            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(4)) {
                        handlePickUpShopItem(4);
                    }
                    result = 2;
                }
            
            }

            break;

        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(5)) {
                        handlePickUpShopItem(5);
                    }
                    result = 2;
                }
            
            }

            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(6)) {
                        handlePickUpShopItem(6);
                    }
                    result = 2;
                }
            
            }

            break;

        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(3)) {
                        handlePickUpShopItem(3);
                    }
                    result = 2;
                }
            
            }

            break;

        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    
                    if (!checkShopItemShouldBeDisplayed(7)) {
                        
                        if (checkShopItemShouldBeDisplayed(8)) {
                            handlePickUpShopItem(8);
                        }

                        result = 2;
                        
                    } else {
                        handlePickUpShopItem(7);
                        result = 2;
                    }
                    
                }
            
            }

            break;

        case 27:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(10)) {
                        handlePickUpShopItem(10);
                    }
                    result = 2;
                }
            
            }

            break;

        case 30:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showMessageBox(0, 0x3D, 0x28, 0x80, 0);
                result = 1;
            }
        
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handlePopuriRoomLevelInteractions);

bool handlePopuriRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {
    
    bool result = FALSE;
    
    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
            result = TRUE;
            setSpawnPoint(FLOWER_SHOP_SPAWN_POINT_2);
        }
    } 
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleBakeryLevelInteractions);

bool handleBakeryLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x35);
            }
            
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(0x5C);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x3D);
            }
            
            break;

        case 16:

            if (gPlayer.direction == NORTHEAST) {
                
                if (gSeason == WINTER && (24 < gDayOfMonth && gDayOfMonth < 30)) {

                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                        if (gPlayer.heldItem == 0) {
                            if (adjustValue(bakeryCardPoints, -10, 100) == -10) {
                                setMainLoopCallbackFunctionIndex(LOTTERY_LOAD);
                                result = TRUE; 
                            } else {
                                showTextBox(0, FESTIVALS_TEXT_INDEX, 0x46, 0, 0);
                                result = TRUE;
                            }
                        }
                        result = TRUE;
                    }
                    
                } else {
                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                        showTextBox(0, SHOP_TEXT_INDEX, 0x43, 0, 0);
                        result = TRUE;
                    }
                }
                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST && gPlayer.heldItem == 0) {
                    // show pink overlay message
                    showPinkOverlayText(2);
                    result = TRUE;
                }
            
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.heldItem == 0) {
                    // show pink overlay message
                    showPinkOverlayText(3);
                    result = TRUE;
                }
            
            }

            break;

        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleElliRoomLevelInteractions);

bool handleElliRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
            result = TRUE;
            setSpawnPoint(0x5A);
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRickStoreLevelInteractions);

bool handleRickStoreLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x36);
            }
            
            break;

        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST) {
                        showTextBox(0, SHOP_TEXT_INDEX, 0x44, 0, 0);
                        result = 1;
                }
            
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x15)) {
                        handlePickUpShopItem(0x15);
                    }
                    result = 2;
                }
            
            }

            break;

        case 18:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x14)) {
                        handlePickUpShopItem(0x14);
                    }
                    result = 2;
                }
            
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x13)) {
                        handlePickUpShopItem(0x13);
                    }
                    result = 2;
                }
            
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x16)) {
                        handlePickUpShopItem(0x16);
                    }
                    result = 2;
                }
            
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x18)) {
                        handlePickUpShopItem(0x18);
                    }
                    if (checkShopItemShouldBeDisplayed(0x19)) {
                        handlePickUpShopItem(0x19);
                    }
                    result = 2;
                }
            
            }

            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x17)) {
                        handlePickUpShopItem(0x17);
                    }
                    result = 2;
                }
            
            }

            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(0, SHOP_TEXT_INDEX, 0x36, 0, 0);
                result = 1;
            
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleSouvenirShopLevelInteractions);

u8 handleSouvenirShopLevelInteractions(u16 arg0, u8 levelInteractionIndex) {

    u8 result = 0;
    
    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(0x2D)) {
                result = 1;
                setSpawnPoint(0x37);
            }
            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x25)) {
                        handlePickUpShopItem(0x25);
                    }
                    result = 2;
                }
            }
            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x26)) {
                        handlePickUpShopItem(0x26);
                    }
                    if (checkShopItemShouldBeDisplayed(0x27)) {
                        handlePickUpShopItem(0x27);
                    }
                    result = 2;
                }
            }
            break;

        case 16:
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, SHOP_TEXT_INDEX, 0xCA, 0, 0);
                result = 1;
            }
            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleChurchLevelInteractions);

// church
bool handleChurchLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x38);
            }

            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                showTextBox(1, SHOP_TEXT_INDEX, 0xC0, 0, 2);
                result = TRUE;
            }           

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xBF, 0, 2);
                result = TRUE;
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xD0, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleTavernLevelInteractions);

bool handleTavernLevelInteractions(u16 arg0, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x39);
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, SHOP_TEXT_INDEX, 0x78, 0, 0);
                result = TRUE;
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                if (gPlayer.heldItem == 0) {
                    // show pink overlay message
                    showPinkOverlayText(4);
                    result = TRUE;
                }  
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {   

                switch (gSeason) {

                    case SPRING:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xCB, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xCC, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xCD, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xCE, 0, 2);
                        break;      

                }
                
                result = TRUE;

            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleVillage2LevelInteractions);

bool handleVillage2LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_5);
            }
            
            break;

        case 2:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x33);
            }
            
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL) || checkDailyEventBit(0x56)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setSpawnPoint(0x64);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(0x65);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 3;
                    result = TRUE;
                    setSpawnPoint(0x68);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 2;
                    result = TRUE;
                    setSpawnPoint(0x6B);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;
        
        case 7:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x3C);
            }
            
            break;

        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0xB8, 0, 2);
                result = TRUE;
            
            }

            break;

        case 17:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0xB5, 0, 2);
                result = TRUE;
            
            }

            break;
        
        case 18:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0x160, 0, 2);
                result = TRUE;
            
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkLifeEventBit(0x50)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x15F, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x167, 0, 2);
                    result = TRUE;
                }
            
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0x161, 0, 2);
                result = TRUE;
            
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0x161, 0, 2);
                result = TRUE;
            
            }

            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0x162, 0, 2);
                result = TRUE;
            
            }

            break;

        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0x163, 0, 2);
                result = TRUE;
            
            }

            break;
        
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleLibraryLevelInteractions);

bool handleLibraryLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x40);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.direction == NORTHEAST) {

                    if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                        showTextBox(0, SHOP_TEXT_INDEX, 0xD7, 0, 0);
                        result = TRUE;
                    } else {
                        showTextBox(0, SHOP_TEXT_INDEX, 0xD6, 0, 0);
                        result = TRUE;
                    }
                    
                    
                }
                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 0, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;    
        
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 1, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 2, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 3, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 4, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
         
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 5, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;
        
        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 6, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 7, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.direction == NORTHWEST && gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 8, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;   
        
        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.direction == NORTHWEST && gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 9, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMidwifeHouseLevelInteractions);

bool handleMidwifeHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(0x43);
            }

            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xD1, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMayorHouseLevelInteractions);

bool handleMayorHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x41);
            }
            
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(0x67);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                switch (gSeason) {

                    case SPRING:
                    case SUMMER:
                    case AUTUMN:
                    case WINTER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xCF, 0, 2);
                        break;

                }
                
                result = TRUE; 
            
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMariaRoomLevelInteractions);

bool handleMariaRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

   bool result = FALSE;

    switch (levelInteractionIndex) {
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x66);
            }
            
            break;

    } 

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handlePotionShopLevelInteractions);

u8 handlePotionShopLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = 0;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x42);
            }
            
            break;
        
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setSpawnPoint(0x6A);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;
        
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(0x22)) {

                        case 0:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6C, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            handlePickUpShopItem(0x22);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6F, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6B, 0, 0);
                            result = 1;
                            break;
                        
                    }
                    
                } 
                        
            }

            break;    
        
        
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(0x23)) {

                        case 0:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6C, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            handlePickUpShopItem(0x23);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6F, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6B, 0, 0);
                            result = 1;
                            break;
                        
                    }
                    
                } 
                        
            }

            break;    
        
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(0x24)) {

                        case 0:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6C, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            handlePickUpShopItem(0x24);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6F, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, SHOP_TEXT_INDEX, 0x6B, 0, 0);
                            result = 1;
                            break;
                        
                    }
                    
                } 
                        
            }

            break;   
        
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(0, SHOP_TEXT_INDEX, 0x70, 0, 0);
                result = 1;
            
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handlePotionShopBedroomLevelInteractions);

// potion shop bedroom
bool handlePotionShopBedroomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x69);
            }
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                switch (gSeason) {

                    case SPRING:
                    case SUMMER:
                    case AUTUMN:
                    case WINTER:
                        showTextBox(1, SHOP_TEXT_INDEX, 0xD0, 0, 2);
                        break;

                }

                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleSquareLevelInteractions);

// FIXME: lots of unnecessary gotos
u8 handleSquareLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;
    u8 temp;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x3A);
            }
            
            break;


        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                levelInteractionsInfo.mapAdditionsIndex = 0;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = 1;
                setSpawnPoint(0x6C);
                
            }
            
            break;

        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x3B);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem == 0) {
                    temp = 10;
label3:
                    // show pink overlay message
                    showPinkOverlayText(temp);
                    result = 1;
label4:
                }
                
            }

            break;
        
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x41)) {

                    if (func_800CE714(0) && func_800CE714(1)) {
                        
                        if (func_800CE714(2)) {
                            goto label2;
                        } else {
                            goto label;
                        }
                        
                    } else {
                        pauseAllCutsceneExecutors();
                        setMainLoopCallbackFunctionIndex(RACE_GIFTS_LOAD);
                    }
                    
                } else if (func_800CE828(0) && func_800CE828(1) && func_800CE828(2)) {
label2:
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 0x47, 0, 0);
                    
                } else {
label:
                    pauseAllCutsceneExecutors();
                    setMainLoopCallbackFunctionIndex(RACE_GIFTS_LOAD);
                    
                }
                
                result = 1;
                
            }
            
            break;

        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 10, 0, 0);
                } else if (gRacingContext.currentRaceIndex == 3) {
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 8, 0, 0);
                } else if (!gRacingContext.betPlacedFlags[gRacingContext.currentRaceIndex]) {
                    pauseAllCutsceneExecutors();
                    setMainLoopCallbackFunctionIndex(RACE_BETTING_LOAD);
                } else {
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 7, 0, 0);
                }
                
                result = 1;
                
            }
            
            break;

        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                
                    if (checkShopItemShouldBeDisplayed(0x35)) {
                        pauseAllCutsceneExecutors();
                        // pick up shop item
                        handlePickUpShopItem(0x35);
                    }
                    
                    result = 2;
                
                }
                
            }
            
            break;

        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gSeason == SPRING && (18 < gDayOfMonth && gDayOfMonth < 23)) {

                    if (gVoteForFlowerFestivalGoddess == 0xFF) {
                        temp = 9;
                        goto label3;
                    } else {
                        showTextBox(0, FESTIVALS_TEXT_INDEX, 0x23, 0, 2);
                        result = 1;
                        goto label4;
                    }
                    
                }

                if (!checkHaveTool(EMPTY_BOTTLE)) {

                    if (gPlayer.heldItem == 0) {
                    
                        if (checkShopItemShouldBeDisplayed(0x36)) {
                            pauseAllCutsceneExecutors();
                            handlePickUpShopItem(0x36);
                            goto label5;
                        }
                        
                        if (checkShopItemShouldBeDisplayed(0x37)) {
                            pauseAllCutsceneExecutors();   
                            handlePickUpShopItem(0x37);
label5:
                        }
                        
                        result = 2;
                        
                    }
                    
                } else { 
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 0x26, 0, 0);
                }
            }
            
            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMountain1LevelInteractions);

bool handleMountain1LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_6);                
            }
            break;

        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_1);                
            }

            break;

        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(TOP_OF_MOUNTAIN_1_SPAWN_POINT_1);                
            }
            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xAD, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;    
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMountain2LevelInteractions);

u8 handleMountain2LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;
    
    switch (levelInteractionIndex) {
        
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(MOUNTAIN_1_SPAWN_POINT_2);
            }
            
            break;

    
        case 2:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x45);
            }
            
            break;

        case 3:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x48);
            }
            
            break;

        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST) {
                    
                    if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                        
                        showTextBox(1, SHOP_TEXT_INDEX, 360, 0, 2);
                        result = 1;
                        
                    } else {
                        
                        if (gYear == 1) {

                            if (!(gSeason == AUTUMN && (19 < gDayOfMonth && gDayOfMonth < 28))) {
                                
                                if (gSeason == WINTER) {
                                    if (8 < gDayOfMonth && gDayOfMonth < 17) {
                                        showTextBox(1, SHOP_TEXT_INDEX, 360, 0, 2);
                                        result = 1;
                                    } else {
                                        levelInteractionsInfo.mapAdditionsIndex = 3;
                                        result = 1;
                                        setSpawnPoint(0x49);
                                    }
                                } else {
                                    levelInteractionsInfo.mapAdditionsIndex = 3;
                                    result = 1;
                                    setSpawnPoint(0x49);
                                }

                            } else {
                                showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                                result = 1;
                            }
                            
                        } else {
                            levelInteractionsInfo.mapAdditionsIndex = 3;
                            result = 1;
                            setSpawnPoint(0x49);
                        }
                    }
                    
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    
                }
            
            }

            break;
    
        case 5:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(0x25);
            }
            
            break;

    
        case 6:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(TOP_OF_MOUNTAIN_1_SPAWN_POINT_2);
            }
            
            break;

        // climbing tree
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.heldItem == 0) {

                    if (getLevelInteractionIndexFromEntityPosition(PLAYER, 0.0f, 16.0f) == 17) {
                        setDailyEventBit(6);
                        setPlayerAction(CLIMBING_TREE, ANIM_CLIMBING_TREE);
                        result = 2;
                    }
                    
                }
            }
            
            break;
    
        // jumping down
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == SOUTHWEST) {
                    setDailyEventBit(6);
                    setPlayerAction(JUMPING_DOWN, ANIM_JUMPING);
                    result = 2;
                }
            
            }

            break;
    
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 175, 0, 2);
                result = 1;
            }

            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleTopOfMountain1LevelInteractions);

u8 handleTopOfMountain1LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = 0;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(MOUNTAIN_1_SPAWN_POINT_3);
            }
            
            break;

        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_6);
            }
            
            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0xAB, 0, 2);
                result = 1;
            
            }

            break;

        // hot springs
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (!checkDailyEventBit(0x28)) {

                    if (gPlayer.heldItem == 0) {
                        if (-48.0f <= entities[ENTITY_PLAYER].coordinates.x && entities[ENTITY_PLAYER].coordinates.x < -40.0f) {
                            setDailyEventBit(6);
                            setPlayerAction(USING_HOT_SPRINGS, ANIM_DEFAULT);
                            levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                            result = 2;
                        }
                    }
                    
                }
            
            }

            break;
        
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xAE, 0, 2);
                result = 1;
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMoonMountainLevelInteractions);

bool handleMoonMountainLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_5);                
            }
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    setSpawnPoint(0x4A);                
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
            }

            break;        

        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xB1, 0, 2);
                result = TRUE;            
            }

            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRoadLevelInteractions);

bool handleRoadLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x1);
            }
            
            break;


        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x29);
            }
            
            break;


        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x2E);
            }
            
            break;

        case 4:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x32);
            }
            
            break;


        case 5:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x3E);
            }
            
            break;


        case 6:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(MOUNTAIN_1_SPAWN_POINT_1);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0xB4, 0, 2);
                result = TRUE;
            
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0xB6, 0, 2);
                result = TRUE;
            
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, SHOP_TEXT_INDEX, 0xB7, 0, 2);
                result = TRUE;
            
            }

            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleCarpenterHutLevelInteractions);

bool handleCarpenterHutLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {
        
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_4);
            } 
        
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!checkDailyEventBit(8)) {
                    pauseGameplay();
                    loadOverlayScreen(1, HOME_EXTENSIONS_SELECT_LOAD);
                    result = TRUE;
                } else {
                    showTextBox(0, SHOP_TEXT_INDEX, 0x6E, 0, 0);
                    result = TRUE;
                }     
            
            }  

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, SHOP_TEXT_INDEX, 0x91, 0, 0);
                result = TRUE;
            }

            break;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleDumplingHouseLevelInteractions);

bool handleDumplingHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(MOON_MOUNTAIN_SPAWN_POINT_2);
            }

            break;

    } 

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleCaveLevelInteractions);

bool handleCaveLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_2);                
            }

            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setSpawnPoint(0x6D);
                }
            }

            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xB2, 0, 2);
                result = TRUE;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;        
            }

            break;
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleHarvestSpriteCaveLevelInteractions);

bool handleHarvestSpriteCaveLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(0x46);                
            }

            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (getRandomNumberInRange(0, 1)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x94, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x95, 0, 2);
                    result = TRUE;         
                }
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  
                showTextBox(1, SHOP_TEXT_INDEX, 0x9D, 0, 2);
                result = TRUE;
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else if (getRandomNumberInRange(0, 1)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x98, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x99, 0, 2);
                    result = TRUE;
                }
                
            }
            
            break;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleMineLevelInteractions);

bool handleMineLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(71);
            }

            break;

    } 
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handlePondLevelInteractions);

bool handlePondLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {
    
    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_3);                
            }
            
            break;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleVineyardLevelInteractions);

bool handleVineyardLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_3);
            }
            
            break;

        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(0x51);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setSpawnPoint(0x53);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleVineyardHouseLevelInteractions);

bool handleVineyardHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(0x52);                
            }

            break;

        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(0x2F);                
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xCF, 0, 2);
                result = TRUE;
            }

            break;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleKarenRoomLevelInteractions);

bool handleKarenRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;    
    
    switch (levelInteractionIndex) {

        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {

                if (gPlayer.direction == SOUTHEAST) { 
                    result = TRUE;
                    setSpawnPoint(0x50);
                }

            }

            break;

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleVineyardCellarLevelInteractions);

bool handleVineyardCellarLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x30);
            }
            break;

         case 2:

             if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                 if (EAST < gPlayer.direction && gPlayer.direction < (MAX_DIRECTIONS + 1) || gPlayer.direction == EAST) {
                    result = TRUE;
                    setSpawnPoint(0x55);
                 }
             }

            break;     

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                showTextBox(1, SHOP_TEXT_INDEX, 0xD2, 0, 2);
                result = TRUE;
            }          

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, SHOP_TEXT_INDEX, 0xD3, 0, 2);
                result = TRUE;
            }
            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleVineyardCellarBasementLevelInteractions);

bool handleVineyardCellarBasementLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE; 

    switch (levelInteractionIndex) {

        case 1:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) { 
                result = TRUE;
                setSpawnPoint(0x54);
            }

            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xD4, 0, 2);            
                result = TRUE;
            }

            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRaceTrackLevelInteractions);

bool handleRaceTrackLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE; 
    
    switch (levelInteractionIndex) {
        case 1:
            result = TRUE;
            setSpawnPoint(0x62);
            break;
        case 16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkDailyEventBit(0x41)) {
                        // show pink overlay message
                        showPinkOverlayText(0xB);
                    } else {
                        // show pink overlay message
                        showPinkOverlayText(0xC);
                    }
                    result = TRUE;
                }            
            }
            break;
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRanchLevelInteractions);

// FIXME: gotos
bool handleRanchLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_2);
            }

            break;
        
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x28);
            }

            break;
        
        case 3:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!checkDailyEventBit(0x28) && !checkDailyEventBit(FESTIVAL)) {

                     if (!(gSeason == AUTUMN && gDayOfMonth == 3)) { 
                        levelInteractionsInfo.mapAdditionsIndex = 2;
                        result = TRUE;
                        setSpawnPoint(0x4B);
                     } else {
                        result = TRUE;
                        goto label;
                     }
                     
                } else {
                    goto label;
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
        case 4:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setSpawnPoint(0x4E);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
        case 5:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
label:
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = TRUE;

                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(0x4F);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRanchHouseLevelInteractions);

bool handleRanchHouseLevelInteractions(u16 mapIndex, u8 collisionIndex) {

    bool result = FALSE;

    switch (collisionIndex) {

        case 1: 

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(RANCH_SPAWN_POINT_4);
            }

            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0x95, 0, 2);
                result = TRUE;
            }

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0x93, 0, 2);
                result = TRUE;
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xD0, 0, 2);
                result = TRUE;
            }

            break;
    }
    
    return result;
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRanchStoreLevelInteractions);

u8 handleRanchStoreLevelInteractions(u16 mapIndex, u8 collisionIndex) {

    u8 result = 0;
    
    switch (collisionIndex) {

        // exit
        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = 1;
                setSpawnPoint(RANCH_SPAWN_POINT_3);
            }
            break;

        // ann's room
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (checkDailyEventBit(0x28) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, SHOP_TEXT_INDEX, 0x168, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setSpawnPoint(ANN_ROOM_SPAWN_POINT_1);
                }

                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
            } 

            break;

        // the following seem like unused items/interactions
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.direction == NORTHWEST) {

                    if (!checkDailyEventBit(1) && !checkDailyEventBit(2)) {

                         if (checkDailyEventBit(5)) {
                            showTextBox(0, SHOP_TEXT_INDEX, 357, 0, 0);
                        } else {
                            // show pink overlay message
                            showPinkOverlayText(5);  
                        }

                    } else {
                        showTextBox(0, SHOP_TEXT_INDEX, 356, 0, 0);
                    }

                result = 1;   
            } 

            break;

        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x1A)) {
                        handlePickUpShopItem(0x1A);
                    }
                    result = 2;
                }
            }

            break;

        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(0x1D)) {
                        handlePickUpShopItem(0x1D);
                    }
                    result = 2;
                }
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(0x1B)) {
                        handlePickUpShopItem(0x1B);
                    }
                    result = 2;
                }
            }

            break;

        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(0x1C)) {
                        handlePickUpShopItem(0x1C);
                    }
                    result = 2;
                }
            }

            break;

        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showMessageBox(1, 0x3D, 0x29, 0x80, 2);
                result = 1;
            }

            break;

   }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleAnnRoomLevelInteractions);

bool handleAnnRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE; 

    switch (levelInteractionIndex) {

        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x4C);
            }

            break;

        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, SHOP_TEXT_INDEX, 0xCF, 0, 2);
                result = TRUE;
            }

            break;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleRanchBarnLevelInteractions);

bool handleRanchBarnLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;    

    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
            result = TRUE;
            setSpawnPoint(0x2D);
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/levelInteractions", handleBeachLevelInteractions);

bool handleBeachLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;    
    
    switch (levelInteractionIndex) {

        case 1:
            break;

        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_1)) {
                result = TRUE;
                setSpawnPoint(0x2A);
            }

            break;

    }
    
    return result;

}
