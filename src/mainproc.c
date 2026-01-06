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
volatile u8 D_80204B38;
volatile u8 stepMainLoop;
volatile u32 pendingGfxNum;
volatile u8 mainLoopUpdateRate;
volatile u8 D_80222730;

// per 60; 1 = 60 fps
volatile u8 frameRate;
volatile u8 D_80237A04;

u16 gMainMapIndex;

volatile u32 gGraphicsBufferIndex;
volatile u8 gfxTaskNo;
volatile u8 frameCount;

volatile u32 D_801C3B68[4];
volatile u8 loopStepsPerCycle;
volatile u8 framebufferCount;
volatile u8 currentFramebufferIndex;

volatile u8 D_80237408;

// forward declarations
void initializeEngine(void);
void initializeMainProcess(void);

//INCLUDE_ASM("asm/nonmatchings/mainproc", mainproc);

void mainproc(void *arg) {
    
    #ifndef JP
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

    #else
    nuPakMenu(&frameBuffer, &fontBuffer);
    #endif

    initializeEngine();
    
    // load "no controller" screen or intro cutscene
    setupGameStart();

    mainLoop();

}

//INCLUDE_ASM("asm/nonmatchings/mainproc", initializeEngine);

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

//INCLUDE_ASM("asm/nonmatchings/mainproc", initializeMainProcess);

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
    #ifndef JP
    D_80237A04 = 0;
    #else
    D_80237E94 = 0;
    #endif

    gfxTaskNo = 0xFF;

    #ifndef JP
    D_80237408 = 0;
    #else
    D_80237898 = 0;
    #endif

    frameRate = 1;

    #ifndef JP
    mainLoopUpdateRate = 1;
    #else
    D_80222B72 = 1;
    #endif

    #ifndef JP

    D_801C3B68[0] = 0;
    D_801C3B68[1] = 0;
    D_801C3B68[2] = 0;
    D_801C3B68[3] = 0;

    #else
    D_801C4008[0] = 0;
    D_801C4008[1] = 0;
    D_801C4008[2] = 0;
    D_801C4008[3] = 0;
    #endif

    gGraphicsBufferIndex = 0;

    #ifndef JP
    D_80204B38 = 0;
    #else
    D_80204FC8 = 0;
    #endif
    
    gMainMapIndex = 0;
    
    for (i = 0; i < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE; i++) {
        mainLoopCallbacksTable[i] = 0;
    }

}