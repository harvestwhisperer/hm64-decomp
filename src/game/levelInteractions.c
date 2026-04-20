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

#include "assetIndices/dialogueMenus.h"
#include "assetIndices/dialogues.h"
#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

// bss
static LevelInteractionsInfo levelInteractionsInfo;

// forward declarations
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

bool checkWineBarrelInteraction(u16 mapIndex) {

    bool result = FALSE;
    
    if (mapIndex == VINEYARD_CELLAR_BASEMENT) {
        result = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x11;
    }

    return result;

}

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
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, textIndex, 0, 0);
                        setPlayerAction(WALKING_DOWN_STAIRS, ANIM_DEFAULT);
                        gPlayer.actionTimer = 32;
                        gPlayer.savedDirection = SOUTHEAST;
                        break;
                    case POPURI_ROOM:
                    case MARIA_ROOM:
                    case ANN_ROOM:
                    case ELLI_ROOM:
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, textIndex, 0, 0);
                        break;
                    default:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, textIndex, 0, 2);
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

bool handleFarmLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    u8 letterIndex;
    
    switch (levelInteractionIndex) {

        case 1:
        case 2:
            break;
            break;
         
        // exit to road
        case 3:        
        
        if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_1);
            }
            
            return result;
        
        // house entry
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
        
        // bathroom/back door
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
        
        // barn entry
        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 10;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(BARN_SPAWN_POINT_1);
            }

            break;
        
        // coop entry
        case 7:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 11;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(COOP_SPAWN_POINT_1);
            }

            break;

        // greenhouse door
        case 8:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 20;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(GREENHOUSE_SPAWN_POINT_1);
            }

            break;

        case 9:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.direction == SOUTHEAST) {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 12;
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    setSpawnPoint(HOUSE_SPAWN_POINT_2);
                }
            }

            break;
        
        // roof
        case 26:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(HOUSE_SPAWN_POINT_4);
            }

            break;
        
        // lumber storage
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem == 0) {
                
                    if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
                    
                        if (gLumber != 0) {
                            gPlayer.heldItem = LOG_HELD_ITEM;
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
                    showTextBox(1, LETTERS_TEXT_INDEX, getTextIndexFromLetterIndex(letterIndex), 0, 2); // letter body, chosen by letter index

                    if (letterIndex == 35) {
                        acquireKeyItem(GOLD_PENDANT);
                        setSpecialDialogueBit(HAVE_GOLD_PENDANT_DIALOGUE);
                    }

                // no mail
                } else {
                    showTextBox(1, LETTERS_TEXT_INDEX, 58, 0, 2);
                }

                result = TRUE;
            }
            
            break;
        
        // lumber ledger
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.direction == NORTHWEST) {
                    convertNumberToGameVariableString(0x18, gLumber, 0);
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 163, 0, 2);
                    result = TRUE;
                }
            }

            break;

        // fodder ledger
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                convertNumberToGameVariableString(0x17, fodderQuantity, 0);
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 162, 0, 2);
                result = TRUE;
            }

            break;
        
        // shipping crate
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 161, 0, 2);
                result = TRUE;
            }

            break;
        
        // back of tree
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!checkHaveKeyItem(TREASURE_MAP)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 167, 0, 2);
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

bool handleHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {

        // door to farm
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gWeather != TYPHOON) {
                    result = TRUE;
                    setSpawnPoint(FARM_SPAWN_POINT_1);
                }
            }
            break;     

        // kitchen door
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

        // roof
        case 3:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gWeather != TYPHOON) {
                    result = TRUE;
                    setSpawnPoint(FARM_SPAWN_POINT_11);
                }
            }
            break;

        // diary
        case 16:

#if TESTING
            // skip mayor tour
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0 && !checkDailyEventBit(DIARY_BLOCKED)) {
                showDialogueTextBox(DIALOGUE_MENU_DIARY_ROOT);
                result = TRUE;
            }
            break;

#else
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0 && checkLifeEventBit(MAYOR_TOUR) && !checkDailyEventBit(DIARY_BLOCKED)) {
                showDialogueTextBox(DIALOGUE_MENU_DIARY_ROOT);
                result = TRUE;
            }
            break;
#endif

        // TV
        case 18:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.direction == NORTHEAST) {
                loadTVCButtonIcons();
                initializeTVAssets();
                setPlayerAction(WATCHING_TV, ANIM_WATCHING_TV);
                levelInteractionsInfo.interactionSfxIndex = 0x2F;
                result = 2;
            }
            break;

        // toolbox
        case 19:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                activateMapAddition(MAIN_MAP_INDEX, 3, 1);
                loadOverlayScreen(10, TOOLBOX_LOAD);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
            }
            break;

        // calendar
        case 20:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(CALENDAR_LOAD);
                levelInteractionsInfo.interactionSfxIndex  = MENU_OPEN_SFX;
                result = TRUE;
            }
            break;

        // cabinet
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) != 0) {
                activateMapAddition(MAIN_MAP_INDEX, 10, 1);
                loadOverlayScreen(10, CABINET_LOAD);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
            }
            break;

        // crib
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.heldItem == 0) {

                if (checkDailyEventBit(PICKED_UP_BABY)) {

                    if (gBabyAge < 30) {

                        gPlayer.heldItem = BABY_HELD_ITEM;
                        
                        deactivateEntity(npcs[BABY].entityIndex);
                        npcs[BABY].flags &= ~4;

                        setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);

                        if (!checkDailyEventBit(HELD_BABY)) {
                            npcAffection[BABY] += adjustValue(npcAffection[BABY], 2, MAX_AFFECTION);
                            setDailyEventBit(HELD_BABY);
                        }
                        
                        clearDailyEventBit(PICKED_UP_BABY);
                        result = 2;

                    }

                }
            }    

            break;

        // unused
        case 33:
        default:
            break;
            
    }

    return result;
    
}

u8 handleBarnLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;

    switch (levelInteractionIndex) {
        
        // exit to farm
        case 1:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(FARM_SPAWN_POINT_3);
            }
            
            break;

        // fodder
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

        // unused
        case 18:
            
            // if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            // }
            break;

        // stall 1
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && (gFarmAnimals[0].flags & FARM_ANIMAL_ACTIVE)) {

                    switch (gFarmAnimals[0].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[0].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[0].birthdayDayOfMonth, 1);
                            generateMilkTypeString(0);
                            setGameVariableString(38, gFarmAnimals[0].motherName, 6);
                            func_80061690(gFarmAnimals[0].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 229, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 237, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // stall 2
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[1].flags & FARM_ANIMAL_ACTIVE) {

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
                        
                                                        convertNumberToGameVariableString(21, gFarmAnimals[1].birthdayDayOfMonth, 1);
                            generateMilkTypeString(1);
                            setGameVariableString(38, gFarmAnimals[1].motherName, 6);
                            func_80061690(gFarmAnimals[1].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 230, 0, 2);
                            break;

                        case PREGNANT_COW:
                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 238, 0, 2);
                            break;

                    }
                
                    result = 1;
                
                }
            
            }
            
            break;

        // stall 3
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[2].flags & FARM_ANIMAL_ACTIVE) {

                    switch (gFarmAnimals[2].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[2].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[2].birthdayDayOfMonth, 1);
                            generateMilkTypeString(2);
                            setGameVariableString(38, gFarmAnimals[2].motherName, 6);
                            func_80061690(gFarmAnimals[2].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 231, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 239, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // stall 4
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[3].flags & FARM_ANIMAL_ACTIVE) {

                    switch (gFarmAnimals[3].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[3].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[3].birthdayDayOfMonth, 1);
                            generateMilkTypeString(3);
                            setGameVariableString(38, gFarmAnimals[3].motherName, 6);
                            func_80061690(gFarmAnimals[3].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 232, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 240, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // stall 5
        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[4].flags & FARM_ANIMAL_ACTIVE) {

                    switch (gFarmAnimals[4].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[4].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[4].birthdayDayOfMonth, 1);
                            generateMilkTypeString(4);
                            setGameVariableString(38, gFarmAnimals[4].motherName, 6);
                            func_80061690(gFarmAnimals[4].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 233, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 241, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // stall 6
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[5].flags & FARM_ANIMAL_ACTIVE) {

                    switch (gFarmAnimals[5].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[5].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[5].birthdayDayOfMonth, 1);
                            generateMilkTypeString(5);
                            setGameVariableString(38, gFarmAnimals[5].motherName, 6);
                            func_80061690(gFarmAnimals[5].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 234, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 242, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // stall 7
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[6].flags & FARM_ANIMAL_ACTIVE) {

                    switch (gFarmAnimals[6].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[6].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[6].birthdayDayOfMonth, 1);
                            generateMilkTypeString(6);
                            setGameVariableString(38, gFarmAnimals[6].motherName, 6);
                            func_80061690(gFarmAnimals[6].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 235, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 243, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // stall 8
        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem != FODDER_HELD_ITEM && gFarmAnimals[7].flags & FARM_ANIMAL_ACTIVE) {

                    switch (gFarmAnimals[7].type) {

                        case BABY_COW:
                        case CALF:
                        case ADULT_COW:
                        case PREGNANT_COW:
                            setGlobalSeasonName(gFarmAnimals[7].birthdaySeason);

                            gCurrentSeasonName[0] = gGlobalSeasonName[0];
                            gCurrentSeasonName[1] = gGlobalSeasonName[1];
                            gCurrentSeasonName[2] = gGlobalSeasonName[2];
                            gCurrentSeasonName[3] = gGlobalSeasonName[3];
                            gCurrentSeasonName[4] = gGlobalSeasonName[4];
                            gCurrentSeasonName[5] = gGlobalSeasonName[5];

                            convertNumberToGameVariableString(21, gFarmAnimals[7].birthdayDayOfMonth, 1);
                            generateMilkTypeString(7);
                            setGameVariableString(38, gFarmAnimals[7].motherName, 6);
                            func_80061690(gFarmAnimals[7].milkType);
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 236, 0, 2);
                            break;

                        case BABY_SHEEP:
                        case ADULT_SHEEP:
                        case SHEARED_SHEEP:
                            showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 244, 0, 2);
                            break;

                    }

                    result = 1;

                }
            }

            break;

        // fodder ledger
        case 27:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                convertNumberToGameVariableString(23, fodderQuantity, 0);
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 168, 0, 2);
                result = 1;
            }

            break;

        // birthing place
        case 28:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 169, 0, 2);
                result = 1;
            }
            
            break;

        default:
            break;
        
    }

    return result;
    
}

bool handleCoopLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        // exit to farm
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(FARM_SPAWN_POINT_4);
            }
            break;
        
        // incubator
        case 18:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 170, 0, 2);
                result = TRUE;
            }
            break;

        // stall 1
        case 19:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[0].flags & CHICKEN_ACTIVE) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 245, 0, 2);
                    result = TRUE;
                }
            }
            break;

        // stall 2
        case 20:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[1].flags & CHICKEN_ACTIVE) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 246, 0, 2);
                    result = TRUE;
                }
            }
            break;

        // stall 3
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[2].flags & CHICKEN_ACTIVE) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 247, 0, 2);
                    result = TRUE;
                }
            }
            break;

        // stall 4
        case 22:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[3].flags & CHICKEN_ACTIVE) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 248, 0, 2);
                    result = TRUE;
                }
            }
            break;

        // stall 5
        case 23:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[4].flags & CHICKEN_ACTIVE) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 249, 0, 2);
                    result = TRUE;
                }
            }
            break;

        // stall 6
        case 24:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gChickens[5].flags & CHICKEN_ACTIVE) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 250, 0, 2);
                    result = TRUE;
                }
            }
            break;

    }

    return result;
    
}

bool handleKitchenLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to main house
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(HOUSE_SPAWN_POINT_2);
            }
            break;

        // bathroom door
        case 2:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                result = TRUE;
                levelInteractionsInfo.mapAdditionsIndex = 2;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                setSpawnPoint(BATHROOM_SPAWN_POINT_1);
            }        
            break;

        // freezer
        case 16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                activateMapAddition(MAIN_MAP_INDEX, 0, 1);
                loadOverlayScreen(10, FREEZER_LOAD);
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = TRUE;
            }
            break;

        // recipe book
        case 21:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                setMainLoopCallbackFunctionIndex(RECIPE_BOOK_LOAD);
                levelInteractionsInfo.interactionSfxIndex = MENU_OPEN_SFX;
                result = TRUE;
            }
            break;
        
        // kitchen picture
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

bool handleBathroomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to house or kitchen
        case 1:
            
            result = TRUE;
            
            if (checkLifeEventBit(HAVE_KITCHEN)) {
                setSpawnPoint(KITCHEN_SPAWN_POINT_2);
            } else {            
                setSpawnPoint(FARM_SPAWN_POINT_7);
            }

            break;

        // unused
        case 2:
            break;

        // bath
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && entities[ENTITY_PLAYER].coordinates.z >= -44.0f && entities[ENTITY_PLAYER].coordinates.z < -36.0f) {
                    setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
                    setPlayerAction(BATHING, ANIM_DEFAULT);
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    result = 2;
                }
            }         

            break;

        // toilet
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0 && entities[ENTITY_PLAYER].coordinates.x >= 42.0f && entities[ENTITY_PLAYER].coordinates.x < 50.0f) {
                    setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
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

bool handleGreenhouseLevelInteractions(u16 arg0, u8 levelInteractionIndex) {

    bool result = FALSE;

    // exit to farm
    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
            result = TRUE;
            setSpawnPoint(FARM_SPAWN_POINT_8);
        }
    } 
    
    return result;
    
}

bool handleVillage1LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        // exit to road
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_4);
            }
            break;
        
        // exit to village 2
        case 2:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_2);
            }
            break;

        // flower shop entrance
        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(FLOWER_SHOP_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // bakery entrance
        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST) {
                    
                    if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                        result = TRUE;
                    } else {
                        levelInteractionsInfo.mapAdditionsIndex = 1;
                        result = TRUE;
                        setSpawnPoint(BAKERY_SPAWN_POINT_1);
                    }
                    
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                        
                }
                
            }
        
            break;

        // rick store entrance
        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 4;
                    result = TRUE;
                    setSpawnPoint(RICK_STORE_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // saibara entrance
        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 3;
                    result = TRUE;
                    setSpawnPoint(SOUVENIR_SHOP_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // church entrance
        case 7:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setSpawnPoint(CHURCH_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // tavern entrance
        case 8:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setSpawnPoint(TAVERN_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // square entrance 1
        case 9:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gPlayer.direction == NORTHWEST) {
                    result = TRUE;
                    setSpawnPoint(SQUARE_SPAWN_POINT_1);
                }
            }
            
            break;

        // square entrance 2
        case 10:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gPlayer.direction == NORTHWEST) {
                    result = TRUE;
                    setSpawnPoint(SQUARE_SPAWN_POINT_3);
                }
            }
            
            break;

        // square entrance 3
        case 37:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gPlayer.direction == WEST) {
                    result = TRUE;
                    setSpawnPoint(SQUARE_SPAWN_POINT_3);
                }
            }
            
            break;

        // square entrance 4
        case 38:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gPlayer.direction == NORTH) {
                    result = TRUE;
                    setSpawnPoint(SQUARE_SPAWN_POINT_3);
                }
            }
            
            break;

        // square entrance 5
        case 39:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gPlayer.direction == WEST) {
                    result = TRUE;
                    setSpawnPoint(SQUARE_SPAWN_POINT_1);
                }
            }
            
            break;

        // square entrance 6
        case 40:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                if (gPlayer.direction == NORTH) {
                    result = TRUE;
                    setSpawnPoint(SQUARE_SPAWN_POINT_1);
                }
            }
            
            break;

        // village 2 entrance 2
        case 11:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_7);
            }
            
            break;

        // bakery entrance 2
        case 12:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;

                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 2;
                    result = TRUE;
                    setSpawnPoint(BAKERY_SPAWN_POINT_3);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
    }
    
    return result;
   
}

u8 handleFlowerShopLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;
    
    switch (levelInteractionIndex) {

        // exit to village
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_3);
            }
            break;
        
        // popuri room entrance
        case 2:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setSpawnPoint(POPURI_ROOM_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // Lilia interaction
        case 16:

            if (gPlayer.direction == NORTHEAST) {
                
                if (gSeason == WINTER && (24 < gDayOfMonth && gDayOfMonth < 30)) {

                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                        if (gPlayer.heldItem == 0) {
                            if (adjustValue(flowerShopPoints, -10, 100) == -10) {
                                setMainLoopCallbackFunctionIndex(LOTTERY_LOAD);
                                result = 1; 
                            } else {
                                showTextBox(0, FESTIVALS_TEXT_INDEX, 62, 0, 0);
                                result = 1;
                            }
                        }
                        result = 1;
                    }

                } else {
                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                        showMessageBox(0, DIALOGUE_SHOP, 39, 0x80, 0);
                        result = 1;
                    }
                }
                
            }

            break;

        // selling prices list
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                switch (gSeason) {
                    case SPRING:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 14, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 15, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 16, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 201, 0, 2);
                        break;
                    
                }

                result = 1;
        
            }

            break;

        // calendar
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                switch (gSeason) {

                    case SPRING:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 17, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 18, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 19, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 20, 0, 2);
                        break;
                    
                }

                result = 1;
                
            }

            break;

        // turnip seeds slot
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_TURNIP_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_TURNIP_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // potato seeds slot
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_POTATO_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_POTATO_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // cabbage seeds slot
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_CABBAGE_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_CABBAGE_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // tomato seeds slot
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_TOMATO_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_TOMATO_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // corn seeds slot
        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_CORN_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_CORN_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // eggplant seeds slot
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_EGGPLANT_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_EGGPLANT_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // grass seeds slot
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_GRASS_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_GRASS_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // moon drop seeds slot
        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    
                    if (!checkShopItemShouldBeDisplayed(FLOWER_SHOP_STRAWBERRY_SEEDS_ITEM)) {
                        
                        if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_MOON_DROP_SEEDS_ITEM)) {
                            handlePickUpShopItem(FLOWER_SHOP_MOON_DROP_SEEDS_ITEM);
                        }

                        result = 2;
                        
                    } else {
                        handlePickUpShopItem(FLOWER_SHOP_STRAWBERRY_SEEDS_ITEM);
                        result = 2;
                    }
                    
                }
            
            }

            break;

        // blue mist seeds slot
        case 27:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(FLOWER_SHOP_BLUE_MIST_SEEDS_ITEM)) {
                        handlePickUpShopItem(FLOWER_SHOP_BLUE_MIST_SEEDS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // popuri's flower notebook
        case 30:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showMessageBox(0, DIALOGUE_SHOP, DIALOGUE_SHOP_POPURIS_NOTEBOOK, 0x80, 0);
                result = 1;
            }

}

    return result;
    
}

bool handlePopuriRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {
    
    bool result = FALSE;
    
    // exit to flower shop
    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
            result = TRUE;
            setSpawnPoint(FLOWER_SHOP_SPAWN_POINT_2);
        }
    } 
    
    return result;

}

bool handleBakeryLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village
        case 1:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_4);
            }
            
            break;

        // elli room entrance
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(ELLI_ROOM_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // exit 2 to village 
        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_12);
            }
            
            break;

        // jeff interaction
        case 16:

            if (gPlayer.direction == NORTHEAST) {
                
                if (gSeason == WINTER && (24 < gDayOfMonth && gDayOfMonth < 30)) {

                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                        if (gPlayer.heldItem == 0) {
                            if (adjustValue(bakeryCardPoints, -10, 100) == -10) {
                                setMainLoopCallbackFunctionIndex(LOTTERY_LOAD);
                                result = TRUE; 
                            } else {
                                showTextBox(0, FESTIVALS_TEXT_INDEX, 70, 0, 0);
                                result = TRUE;
                            }
                        }
                        result = TRUE;
                    }

                } else {
                    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 67, 0, 0);
                        result = TRUE;
                    }
                }
                
            }

            break;

        // food menu
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST && gPlayer.heldItem == 0) {
                    showDialogueTextBox(DIALOGUE_MENU_BAKERY_FOOD);
                    result = TRUE;
                }
            
            }

            break;

        // drink menu
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.heldItem == 0) {
                    showDialogueTextBox(DIALOGUE_MENU_BAKERY_DRINK_MENU);
                    result = TRUE;
                }
            
            }

            break;

}
    
    return result;
    
}

bool handleElliRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    // exit to bakery
    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
            result = TRUE;
            setSpawnPoint(BAKERY_SPAWN_POINT_2);
        }
    }
    
    return result;
    
}

bool handleRickStoreLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village
        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_5);
            }
            
            break;

        // interaction with Rick
        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST) {
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 68, 0, 0);
                        result = 1;
                }
            
            }

            break;

        // clippers slot
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_CLIPPERS_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_CLIPPERS_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // brush slot
        case 18:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_BRUSH_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_BRUSH_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // milker slot
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_MILKER_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_MILKER_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // blue feather slot
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_BLUE_FEATHER_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_BLUE_FEATHER_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // cabinet or ice cream maker slot
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_CABINET_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_CABINET_ITEM);
                    }
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_ICE_CREAM_MAKER_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_ICE_CREAM_MAKER_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // rug slot
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RICK_STORE_RUG_ITEM)) {
                        handlePickUpShopItem(RICK_STORE_RUG_ITEM);
                    }
                    result = 2;
                }
            
            }

            break;

        // bookcases
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 54, 0, 0);
                result = 1;
            }

            break;
        
    }
    
    return result;
    
}

u8 handleSouvenirShopLevelInteractions(u16 arg0, u8 levelInteractionIndex) {

    u8 result = 0;
    
    switch (levelInteractionIndex) {

        // exit to village
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_6);
            }
            break;

        // ocarina slot
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(SOUVENIR_SHOP_OCARINA_ITEM)) {
                        handlePickUpShopItem(SOUVENIR_SHOP_OCARINA_ITEM);
                    }
                    result = 2;
                }
            }
            break;

        // vase slot
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(SOUVENIR_SHOP_VASE_EARLY_ITEM)) {
                        handlePickUpShopItem(SOUVENIR_SHOP_VASE_EARLY_ITEM);
                    }
                    if (checkShopItemShouldBeDisplayed(SOUVENIR_SHOP_BLUE_VASE_ITEM)) {
                        handlePickUpShopItem(SOUVENIR_SHOP_BLUE_VASE_ITEM);
                    }
                    result = 2;
                }
            }
            break;

        // don't touch
        case 16:
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 202, 0, 0);
                result = 1;
            }
            break;

    }

    return result;
    
}

bool handleChurchLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_7);
            }

            break;

        // harvest goddess statue
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 192, 0, 2);
                result = TRUE;
            }

            break;

        // organ
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 191, 0, 2);
                result = TRUE;
            }

            break;

        // stove
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 208, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

bool handleTavernLevelInteractions(u16 arg0, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_8);
            }

            break;

        // interaction with Duke
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 120, 0, 0);
                result = TRUE;
            }

            break;

        // menu
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {            
                if (gPlayer.heldItem == 0) {
                    showDialogueTextBox(DIALOGUE_MENU_TAVERN_MENU);
                    result = TRUE;
                }  
            }

            break;

        // calendar
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {   

                switch (gSeason) {

                    case SPRING:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 203, 0, 2);
                        break;
                    case SUMMER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 204, 0, 2);
                        break;
                    case AUTUMN:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 205, 0, 2);
                        break;
                    case WINTER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 206, 0, 2);
                        break;      

                }
                
                result = TRUE;

            }

            break;

    }
    
    return result;
    
}

bool handleVillage2LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to road
        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_5);
            }
            
            break;

        // exit to village 1
        case 2:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_2);
            }
            
            break;

        //  library entrance
        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL) || checkDailyEventBit(MARIA_LOCATION_OVERRIDE)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setSpawnPoint(LIBRARY_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // mayor house entrance
        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(MAYOR_HOUSE_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // potion shop entrance
        case 5:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 3;
                    result = TRUE;
                    setSpawnPoint(POTION_SHOP_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // midwife's house entrance
        case 6:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 2;
                    result = TRUE;
                    setSpawnPoint(MIDWIFE_HOUSE_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;
        
        // exit to village 1 2
        case 7:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_11);
            }
            
            break;

        // library sign
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 184, 0, 2);
                result = TRUE;

            }

            break;

        // cemetery signs
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 181, 0, 2);
                result = TRUE;

            }

            break;

        // grandpa grave
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 352, 0, 2);
                result = TRUE;

            }

            break;

        // animal gravestone
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (checkLifeEventBit(ANIMAL_FUNERAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 351, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 359, 0, 2);
                    result = TRUE;
                }
            
            }

            break;

        // grave 3
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 353, 0, 2);
                result = TRUE;
            
            }

            break;

        // grave 4
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 353, 0, 2);
                result = TRUE;
            
            }

            break;

        // grave 5
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 354, 0, 2);
                result = TRUE;

            }

            break;

        // grave 6
        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 355, 0, 2);
                result = TRUE;

            }

            break;
        
    }
    
    return result;
    
}

bool handleLibraryLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_3);
            }
            
            break;

        // interaction with Maria
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.direction == NORTHEAST) {

                    if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 215, 0, 0);
                        result = TRUE;
                    } else {
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 214, 0, 0);
                        result = TRUE;
                    }

}
                
            }

            break;

        // bookshelf 1
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 0, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;    
        
        // bookshelf 2
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 1, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          

        // bookshelf 3
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 2, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        // bookshelf 4
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 3, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        // bookshelf 5
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 4, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
         
        // bookshelf 6
        case 22:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 5, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;
        
        // bookshelf 7
        case 23:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 6, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        // bookshelf 8
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 7, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;          
        
        // bookshelf 9
        case 25:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.direction == NORTHWEST && gPlayer.heldItem == 0) {

                    showTextBox(1, LIBRARY_TEXT_INDEX, 8, 0, 2);
                    result = TRUE;
                    
                } 
                        
            }

            break;   
        
        // bookshelf 10
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

bool handleMidwifeHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_6);
            }

            break;

        // bookshelf
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 209, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

bool handleMayorHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to village 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_4);
            }
            
            break;

        // maria room entrance
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(MARIA_ROOM_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // old stove
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                switch (gSeason) {

                    case SPRING:
                    case SUMMER:
                    case AUTUMN:
                    case WINTER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 207, 0, 2);
                        break;

                }
                
                result = TRUE; 
            
            }

            break;

    }
    
    return result;
    
}

bool handleMariaRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

   bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to mayor house
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(MAYOR_HOUSE_SPAWN_POINT_2);
            }
            
            break;

    } 

    return result;
    
}

u8 handlePotionShopLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = 0;

    switch (levelInteractionIndex) {

        // exit to village 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_5);
            }
            
            break;
        
        // bedroom door
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setSpawnPoint(POTION_SHOP_BEDROOM_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;
        
        // cure-all slot
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(POTION_SHOP_CURE_ALL_ITEM)) {

                        case 0:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 108, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            handlePickUpShopItem(POTION_SHOP_CURE_ALL_ITEM);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 111, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 107, 0, 0);
                            result = 1;
                            break;

                    }
                    
                } 
                        
            }

            break;    
        
        // vitamin slot
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(POTION_SHOP_VITAMIN_ITEM)) {

                        case 0:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 108, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            handlePickUpShopItem(POTION_SHOP_VITAMIN_ITEM);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 111, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 107, 0, 0);
                            result = 1;
                            break;

                    }
                    
                } 
                        
            }

            break;    
        
        // strong vitamin slot
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (gPlayer.heldItem == 0) {

                    switch (checkShopItemShouldBeDisplayed(POTION_SHOP_STRONG_VITAMINS_ITEM)) {

                        case 0:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 108, 0, 0);
                            result = 1;
                            break;
                        case 1:
                            handlePickUpShopItem(POTION_SHOP_STRONG_VITAMINS_ITEM);
                            result = 2;
                            break;
                        case 2:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 111, 0, 0);
                            result = 1;
                            break;
                        case 3:
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 107, 0, 0);
                            result = 1;
                            break;

                    }
                    
                } 
                        
            }

            break;   
        
        // bookshelf
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 112, 0, 0);
                result = 1;
            
            }

            break;
        
    }
    
    return result;
    
}

bool handlePotionShopBedroomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to potion shop
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(POTION_SHOP_SPAWN_POINT_2);
            }
            break;

        // stove
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                switch (gSeason) {

                    case SPRING:
                    case SUMMER:
                    case AUTUMN:
                    case WINTER:
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 208, 0, 2);
                        break;

                }

                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

u8 handleSquareLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;

    switch (levelInteractionIndex) {

        // exit to village 1
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_9);
            }
            
            break;

        // race track entrance
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                levelInteractionsInfo.mapAdditionsIndex = 0;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                result = 1;
                setSpawnPoint(RACE_TRACK_SPAWN_POINT_1);
                
            }
            
            break;

        // exit to village 1
        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_10);
            }
            
            break;

        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gPlayer.heldItem == 0) {
                    showDialogueTextBox(DIALOGUE_MENU_SQUARE_STALL);
                    result = 1;
                }

            }

            break;
        
        // horse/dog race prizes/mayor interaction
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (checkDailyEventBit(HORSE_RACE)) {

                    if (checkAlreadyHaveHorseRacePrize(0) && checkAlreadyHaveHorseRacePrize(1) && checkAlreadyHaveHorseRacePrize(2)) {
                        showTextBox(0, FESTIVALS_TEXT_INDEX, 71, 0, 0);
                    } else {
                        pauseAllCutsceneExecutors();
                        setMainLoopCallbackFunctionIndex(RACE_GIFTS_LOAD);
                    }

                } else if (checkAlreadyHaveDogRacePrize(0) && checkAlreadyHaveDogRacePrize(1) && checkAlreadyHaveDogRacePrize(2)) {
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 71, 0, 0);
                } else {
                    pauseAllCutsceneExecutors();
                    setMainLoopCallbackFunctionIndex(RACE_GIFTS_LOAD);
                }

                result = 1;

            }

            break;

        // mayor wife horse/dog race interaction
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

        // flower festival seeds stall
        case 24:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (gPlayer.heldItem == 0) {
                
                    if (checkShopItemShouldBeDisplayed(FLOWER_FESTIVAL_PINK_CAT_MINT_SEEDS_ITEM)) {
                        pauseAllCutsceneExecutors();
                        // pick up shop item
                        handlePickUpShopItem(FLOWER_FESTIVAL_PINK_CAT_MINT_SEEDS_ITEM);
                    }
                    
                    result = 2;
                
                }
                
            }
            
            break;

        // flower festival goddess voting box and rick stall
        case 26:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (gSeason == SPRING && (18 < gDayOfMonth && gDayOfMonth < 23)) {

                    if (gVoteForFlowerFestivalGoddess == 0xFF) {
                        showDialogueTextBox(DIALOGUE_MENU_FLOWER_FESTIVAL_VOTE);
                    } else {
                        showTextBox(0, FESTIVALS_TEXT_INDEX, 35, 0, 2);
                    }

                    result = 1;

                } else if (!checkHaveTool(EMPTY_BOTTLE)) {

                    if (gPlayer.heldItem == 0) {

                        if (checkShopItemShouldBeDisplayed(FLOWER_FESTIVAL_EMPTY_BOTTLE_ITEM)) {
                            pauseAllCutsceneExecutors();
                            handlePickUpShopItem(FLOWER_FESTIVAL_EMPTY_BOTTLE_ITEM);
                        } else if (checkShopItemShouldBeDisplayed(FIREWORKS_FESTIVAL_EMPTY_BOTTLE_ITEM)) {
                            pauseAllCutsceneExecutors();
                            handlePickUpShopItem(FIREWORKS_FESTIVAL_EMPTY_BOTTLE_ITEM);
                        }

                        result = 2;

                    }

                } else {
                    showTextBox(0, FESTIVALS_TEXT_INDEX, 38, 0, 0);
                }

            }

            break;
        
    }
    
    return result;
    
}

bool handleMountain1LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to road
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_6);                
            }
            break;

        // exit to mountain 2
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_1);                
            }

            break;

        // exit to top of mountain 1
        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(TOP_OF_MOUNTAIN_1_SPAWN_POINT_1);                
            }
            break;

        // fisherman tent
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 173, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;    
    
}

u8 handleMountain2LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    u8 result = 0;
    
    switch (levelInteractionIndex) {
        
        // exit to mountain 1
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(MOUNTAIN_1_SPAWN_POINT_2);
            }
            
            break;

        // cave entrance
        case 2:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(CAVE_SPAWN_POINT_1);
            }
            
            break;

        // exit to pond
        case 3:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(POND_SPAWN_POINT_1);
            }
            
            break;

        // carpenter hut entrance
        case 4:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == NORTHEAST) {
                    
                    if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                        
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                        result = 1;
                        
                    } else {
                        
                        if (gYear == 1) {

                            if (!(gSeason == AUTUMN && (19 < gDayOfMonth && gDayOfMonth < 28))) {
                                
                                if (gSeason == WINTER) {
                                    if (8 < gDayOfMonth && gDayOfMonth < 17) {
                                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                                        result = 1;
                                    } else {
                                        levelInteractionsInfo.mapAdditionsIndex = 3;
                                        result = 1;
                                        setSpawnPoint(CARPENTER_HUT_SPAWN_POINT_1);
                                    }
                                } else {
                                    levelInteractionsInfo.mapAdditionsIndex = 3;
                                    result = 1;
                                    setSpawnPoint(CARPENTER_HUT_SPAWN_POINT_1);
                                }

                            } else {
                                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                                result = 1;
                            }
                            
                        } else {
                            levelInteractionsInfo.mapAdditionsIndex = 3;
                            result = 1;
                            setSpawnPoint(CARPENTER_HUT_SPAWN_POINT_1);
                        }
                    }
                    
                    levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                    
                }
            
            }

            break;
    
        // exit to moon mountain
        case 5:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(MOON_MOUNTAIN_SPAWN_POINT_1);
            }
            
            break;

        // exit to top of mountain 1
        case 6:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(TOP_OF_MOUNTAIN_1_SPAWN_POINT_2);
            }
            
            break;

        // climbing tree
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.heldItem == 0) {

                    if (getLevelInteractionIndexFromEntityPosition(PLAYER, 0.0f, 16.0f) == 17) {
                        setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
                        setPlayerAction(CLIMBING_TREE, ANIM_CLIMBING_TREE);
                        result = 2;
                    }
                    
                }
            }
            
            break;
    
        // jumping down spot
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (gPlayer.direction == SOUTHWEST) {
                    setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
                    setPlayerAction(JUMPING_DOWN, ANIM_JUMPING);
                    result = 2;
                }
            
            }

            break;
    
        // carpenter hut sign
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 175, 0, 2);
                result = 1;
            }

            break;

    }

    return result;
    
}

u8 handleTopOfMountain1LevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = 0;

    switch (levelInteractionIndex) {

        // exit to mountain 1
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(MOUNTAIN_1_SPAWN_POINT_3);
            }
            
            break;

        // exit to mountain 2
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_6);
            }
            
            break;

        // bridge out sign
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 171, 0, 2);
                result = 1;

            }

            break;

        // hot springs
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {  

                if (!checkDailyEventBit(DAY_CONSUMING_CUTSCENE)) {

                    if (gPlayer.heldItem == 0) {
                        if (-48.0f <= entities[ENTITY_PLAYER].coordinates.x && entities[ENTITY_PLAYER].coordinates.x < -40.0f) {
                            setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
                            setPlayerAction(USING_HOT_SPRINGS, ANIM_DEFAULT);
                            levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                            result = 2;
                        }
                    }
                    
                }
            
            }

            break;
        
        // hot springs sign
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 174, 0, 2);
                result = 1;
            }

            break;
        
    }
    
    return result;
    
}

bool handleMoonMountainLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to mountain 2
        case 1:
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_5);                
            }
            break;

        // dumpling house entrance
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    setSpawnPoint(DUMPLING_HOUSE_SPAWN_POINT_1);                
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
            }

            break;        

        // sign
        case 16:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 177, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;
    
}

bool handleRoadLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to farm
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(FARM_SPAWN_POINT_2);
            }
            
            break;

        // exit to ranch
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(RANCH_SPAWN_POINT_1);
            }
            
            break;

        // exit to vineyard
        case 3:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VINEYARD_SPAWN_POINT_1);
            }
            
            break;

        // exit to village 1
        case 4:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_1_SPAWN_POINT_1);
            }
            
            break;

        // exit to village 2
        case 5:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VILLAGE_2_SPAWN_POINT_1);
            }
            
            break;

        // exit to mountain 1
        case 6:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(MOUNTAIN_1_SPAWN_POINT_1);
            }
            
            break;

        // vineyard sign
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 180, 0, 2);
                result = TRUE;

            }

            break;

        // ranch sign
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 182, 0, 2);
                result = TRUE;

            }

            break;

        // village sign
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 183, 0, 2);
                result = TRUE;

            }

            break;
        
    }
    
    return result;
    
}

bool handleCarpenterHutLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {
        
        // exit to mountain 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_4);
            } 
        
            break;

        // house extensions selection
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!checkDailyEventBit(HOUSE_EXTENSION_SELECTED)) {
                    pauseGameplay();
                    loadOverlayScreen(1, HOME_EXTENSIONS_SELECT_LOAD);
                    result = TRUE;
                } else {
                    showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 110, 0, 0);
                    result = TRUE;
                }
            
            }  

            break;

        // tools
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 145, 0, 0);
                result = TRUE;
            }

            break;
    }

    return result;

}

bool handleDumplingHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to moon mountain
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(MOON_MOUNTAIN_SPAWN_POINT_2);
            }

            break;

    } 

    return result;

}

bool handleCaveLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to mountain 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_2);                
            }

            break;

        // harvest sprite cave entrance
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    result = TRUE;
                    setSpawnPoint(HARVEST_SPRITE_CAVE_SPAWN_POINT_1);
                }
            }

            break;

        // mine entrance
        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 178, 0, 2);
                result = TRUE;
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
            }

            break;
    }
    
    return result;
    
}

bool handleHarvestSpriteCaveLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to cave
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(CAVE_SPAWN_POINT_2);                
            }

            break;

        // tools
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                if (getRandomNumberInRange(0, 1)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 148, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 149, 0, 2);
                    result = TRUE;         
                }
            }

            break;

        // bunk bed
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 157, 0, 2);
                result = TRUE;
            }

            break;

        // little hole
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else if (getRandomNumberInRange(0, 1)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 152, 0, 2);
                    result = TRUE;
                } else {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 153, 0, 2);
                    result = TRUE;
                }
                
            }
            
            break;

    }
    
    return result;
    
}

bool handleMineLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        // exit to cave
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(CAVE_SPAWN_POINT_3);
            }

            break;

    } 
    
    return result;

}

bool handlePondLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {
    
    bool result = FALSE;

    switch (levelInteractionIndex) {
        
        // exit to mountain 2
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(MOUNTAIN_2_SPAWN_POINT_3);                
            }
            
            break;

    }
    
    return result;

}

bool handleVineyardLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // exit to vineyard
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_3);
            }
            
            break;

        // vineyard house entrance
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(VINEYARD_HOUSE_SPAWN_POINT_2);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;

        // vineyard cellar entrance
        case 3:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setSpawnPoint(VINEYARD_CELLAR_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
        
            break;
        
    }
    
    return result;
    
}

bool handleVineyardHouseLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;

    switch (levelInteractionIndex) {

        // karen room entrance
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(KAREN_ROOM_SPAWN_POINT_1);                
            }

            break;

        // exit to vineyard
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(VINEYARD_SPAWN_POINT_2);                
            }

            break;

        // stove
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 207, 0, 2);
                result = TRUE;
            }

            break;

    }
    
    return result;

}

bool handleKarenRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;    
    
    switch (levelInteractionIndex) {

        // exit to vineyard house
        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {

                if (gPlayer.direction == SOUTHEAST) { 
                    result = TRUE;
                    setSpawnPoint(VINEYARD_HOUSE_SPAWN_POINT_1);
                }

            }

            break;

    }
    
    return result;

}

bool handleVineyardCellarLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {

        // exit to vineyard
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(VINEYARD_SPAWN_POINT_3);
            }
            break;

        // stairs to basement
         case 2:

             if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                 if (EAST < gPlayer.direction && gPlayer.direction < (MAX_DIRECTIONS + 1) || gPlayer.direction == EAST) {
                    result = TRUE;
                    setSpawnPoint(VINEYARD_CELLAR_BASEMENT_SPAWN_POINT_1);
                 }
             }

            break;     

        // press barrel
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 210, 0, 2);
                result = TRUE;
            }

            break;

        // machine
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 211, 0, 2);
                result = TRUE;
            }
            break;

    }

    return result;
    
}

bool handleVineyardCellarBasementLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE; 

    switch (levelInteractionIndex) {

        // stairs to upper cellar
        case 1:
            
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) { 
                result = TRUE;
                setSpawnPoint(VINEYARD_CELLAR_SPAWN_POINT_2);
            }

            break;

        // wine rack
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 212, 0, 2);
                result = TRUE;
            }

            break;

    }

    return result;
    
}

bool handleRaceTrackLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE; 
    
    switch (levelInteractionIndex) {
        
        // exit to square
        case 1:
            result = TRUE;
            setSpawnPoint(SQUARE_SPAWN_POINT_2);
            break;
        
        // stall
        case 16:
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkDailyEventBit(HORSE_RACE)) {
                        showDialogueTextBox(DIALOGUE_MENU_HORSE_RACE_STALL);
                    } else {
                        showDialogueTextBox(DIALOGUE_MENU_DOG_RACE_STALL);
                    }
                    result = TRUE;
                }            
            }
            break;

    }
    
    return result;
}

bool handleRanchLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;
    
    switch (levelInteractionIndex) {

        // exit to road
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(ROAD_SPAWN_POINT_2);
            }

            break;
        
        // exit to beach
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(BEACH_SPAWN_POINT_2);
            }

            break;
        
        // ranch store entrance
        case 3:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!checkDailyEventBit(DAY_CONSUMING_CUTSCENE) && !checkDailyEventBit(FESTIVAL)) {

                     if (!(gSeason == AUTUMN && gDayOfMonth == 3)) { 
                        levelInteractionsInfo.mapAdditionsIndex = 2;
                        result = TRUE;
                        setSpawnPoint(RANCH_STORE_SPAWN_POINT_1);
                     } else {
                        result = TRUE;
                        showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                        result = TRUE;
                     }
                     
                } else {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
        // ranch house entrance
        case 4:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                
                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) { 
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 1;
                    result = TRUE;
                    setSpawnPoint(RANCH_HOUSE_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
        // ranch barn entrance
        case 5:
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 

                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = TRUE;

                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = TRUE;
                    setSpawnPoint(RANCH_BARN_SPAWN_POINT_1);
                }
                
                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
                
            }
            
            break;
        
    }

    return result;
    
}

bool handleRanchHouseLevelInteractions(u16 mapIndex, u8 collisionIndex) {

    bool result = FALSE;

    switch (collisionIndex) {

        // exit to ranch
        case 1: 

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(RANCH_SPAWN_POINT_4);
            }

            break;

        // tools
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 149, 0, 2);
                result = TRUE;
            }

            break;

        // bed
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 147, 0, 2);
                result = TRUE;
            }

            break;

        // stove
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 208, 0, 2);
                result = TRUE;
            }

            break;
    }
    
    return result;
}

u8 handleRanchStoreLevelInteractions(u16 mapIndex, u8 collisionIndex) {

    u8 result = 0;
    
    switch (collisionIndex) {

        // exit
        case 1:
        
            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = 1;
                setSpawnPoint(RANCH_SPAWN_POINT_3);
            }
            break;

        // ann's room
        case 2:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (checkDailyEventBit(DAY_CONSUMING_CUTSCENE) || checkDailyEventBit(FESTIVAL)) {
                    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 360, 0, 2);
                    result = 1;
                } else {
                    levelInteractionsInfo.mapAdditionsIndex = 0;
                    result = 1;
                    setSpawnPoint(ANN_ROOM_SPAWN_POINT_1);
                }

                levelInteractionsInfo.interactionSfxIndex = DOOR_OPEN_SFX;
            } 

            break;

        // doug interaction
        case 16:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && gPlayer.direction == NORTHWEST) {

                    if (!checkDailyEventBit(PURCHASED_ANIMAL) && !checkDailyEventBit(ANIMAL_SALE_IN_PROGRESS)) {

                         if (checkDailyEventBit(DAILY_ANIMAL_TRANSACTION)) {
                            showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 357, 0, 0);
                        } else {
                            showDialogueTextBox(DIALOGUE_MENU_RANCH_STORE_ANIMALS_ROOT);
                        }

                    } else {
                        showTextBox(0, LEVEL_INTERACTIONS_TEXT_INDEX, 356, 0, 0);
                    }

                result = 1;   

            } 

            break;

        // chicken feed slot
        case 17:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RANCH_STORE_CHICKEN_FEED_ITEM)) {
                        handlePickUpShopItem(RANCH_STORE_CHICKEN_FEED_ITEM);
                    }
                    result = 2;
                }
            }

            break;

        // fodder slot
        case 18:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                    if (checkShopItemShouldBeDisplayed(RANCH_STORE_FODDER_ITEM)) {
                        handlePickUpShopItem(RANCH_STORE_FODDER_ITEM);
                    }
                    result = 2;
                }
            }

            break;

        // miracle potion slot
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(RANCH_STORE_MIRACLE_POTION_ITEM)) {
                        handlePickUpShopItem(RANCH_STORE_MIRACLE_POTION_ITEM);
                    }
                    result = 2;
                }
            }

            break;

        // cow medicine slot
        case 20:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                if (gPlayer.heldItem == 0) {
                     if (checkShopItemShouldBeDisplayed(RANCH_STORE_COW_MEDICINE_ITEM)) {
                        handlePickUpShopItem(RANCH_STORE_COW_MEDICINE_ITEM);
                    }
                    result = 2;
                }
            }

            break;

        // animal notebook
        case 21:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                showMessageBox(1, DIALOGUE_SHOP, DIALOGUE_SHOP_ANIMAL_NOTEBOOK, 0x80, 2);
                result = 1;
            }

            break;

   }
    
    return result;

}

bool handleAnnRoomLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE; 

    switch (levelInteractionIndex) {

        // exit to ranch store
        case 1:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(RANCH_STORE_SPAWN_POINT_2);
            }

            break;

        // stove
        case 19:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) { 
                showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, 207, 0, 2);
                result = TRUE;
            }

            break;

    }

    return result;
    
}

bool handleRanchBarnLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;    

    // exit to ranch
    if (levelInteractionIndex == 1) {
        if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
            result = TRUE;
            setSpawnPoint(RANCH_SPAWN_POINT_5);
        }
    }
    
    return result;
    
}

bool handleBeachLevelInteractions(u16 mapIndex, u8 levelInteractionIndex) {

    bool result = FALSE;    
    
    switch (levelInteractionIndex) {

        // unused
        case 1:
            break;

        // exit to ranch
        case 2:

            if (!checkDailyEventBit(FESTIVAL_DAY_INTERACTION_GUARD)) {
                result = TRUE;
                setSpawnPoint(RANCH_SPAWN_POINT_2);
            }

            break;

    }
    
    return result;

}
