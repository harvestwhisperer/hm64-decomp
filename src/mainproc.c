#include "common.h"

#include "system/controller.h"
#include "system/cutscene.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/flags.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/message.h"
#include "system/numberSprites.h"
#include "system/sprite.h"
#include "system/mapController.h"
#include "system/sceneGraph.h"
#include "system/spriteDMAManager.h"

#include "game/gameStart.h"

#include "mainproc.h"
#include "mainLoop.h"                   


// bss
volatile u16 engineStateFlags;
volatile u8 drawnFrameCount;
volatile u8 previousLoopStepsPerCycle;
volatile u8 drawnFrameCount;
volatile u8 retraceCount;
volatile u8 loopStepsPerCycle;
volatile u8 framebufferCount;
volatile u8 previousDrawnFrameCount;
volatile u8 stepMainLoop;
volatile u32 pendingGfxNum;
volatile u8 mainLoopUpdateRate;

// per 60; 1 = 60 fps
volatile u8 frameRate;

u16 gMainMapIndex;

volatile u32 gGraphicsBufferIndex;
volatile u8 gfxTaskNo;
volatile u8 frameCount;

volatile u8 loopStepsPerCycle;
volatile u8 framebufferCount;
volatile u8 currentFramebufferIndex;

volatile u32 D_801C3B68[4];
volatile u8 D_80204B38;
volatile u8 D_80222730;
volatile u8 D_80237408;
volatile u8 D_80237A04;

// forward declarations
void initializeEngine(void);
void initializeMainProcess(void);

void mainproc(void *arg) {
    
    OSViMode *mode;

    nuGfxDisplayOff();

    if (osTvType == OS_TV_NTSC) {
        mode = &osViModeTable[OS_VI_NTSC_LAN1];
        osViSetMode(mode);
        osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON);
    }

    else {
        if (osTvType == OS_TV_MPAL) {
            mode = &osViModeTable[OS_VI_MPAL_LAN1];
            osViSetMode(mode);
            osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON);
        }
        else {
            while(TRUE);
        }
    }

    nuGfxDisplayOff();

    initializeEngine();
    

    // load "no controller" screen or intro cutscene
    setupGameStart();

    mainLoop();

}

void initializeEngine(void) {

    // initialize mainproc and main loop variables
    initializeMainProcess();

    controllerInit();
    graphicsInit();

    initializeSpriteAssetDescriptorFlags();
    
    initializeSceneNodes();
    initializeBitmaps();
    initializeGlobalSprites();
    initializeMap();
    initializeMessageBoxes();
    initializeDialogueSessionManagers();
    initializeNumberSprites();
    initializeCutsceneExecutors();
    initializeFlags();
    initializeEntities();
    initializeMapControllers();

    nuGfxSwapCfbFuncSet(NULL);
    nuGfxSwapCfbFuncSet(gfxBufferSwap);
    nuGfxPreNMIFuncSet(gfxPreNMICallback);

    registerMainLoopCallback(NO_OP, noOpCallback);
    setMainLoopCallbackFunctionIndex(NO_OP);

    nuGfxFuncSet((NUGfxFunc)gfxRetraceCallback);

}

void initializeMainProcess(void) {
    
    u8 i;
    
    stepMainLoop = 0;
    engineStateFlags = 0;
    mainLoopCallbackCurrentIndex = 0;

    frameCount = 0;

    drawnFrameCount = 0;
    framebufferCount = 0;
    currentFramebufferIndex = 0;
    loopStepsPerCycle = 0;
    retraceCount = 0;
    
    D_80237A04 = 0;

    gfxTaskNo = 0xFF;

    D_80237408 = 0;

    frameRate = 1;

    mainLoopUpdateRate = 1;

    D_801C3B68[0] = 0;
    D_801C3B68[1] = 0;
    D_801C3B68[2] = 0;
    D_801C3B68[3] = 0;

    gGraphicsBufferIndex = 0;

    D_80204B38 = 0;
    
    gMainMapIndex = 0;
    
    for (i = 0; i < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE; i++) {
        mainLoopCallbacksTable[i] = 0;
    }

}