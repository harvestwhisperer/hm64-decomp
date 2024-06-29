#include "common.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/flags.h"
#include "system/graphic.h"
#include "system/message.h"
#include "system/pauseScreen.h"
#include "system/sprite.h"
#include "system/mapContext.h"
#include "system/worldGraphics.h"

#include "gameStart.h"
#include "mainproc.h"
#include "mainLoop.h"                        

// bss
extern volatile u16 unknownFlag;
extern volatile u8 drawnFrameCount;
extern volatile u8 D_8013DC30;
extern volatile u8 drawnFrameCount;
extern volatile u8 retraceCount;
extern volatile u8 loopStepsPerCycle;
extern volatile u8 framebufferCount;
extern volatile u8 D_801C4215;
extern volatile u8 D_80204B38;
extern volatile u8 stepMainLoop;
extern volatile u32 pendingGfxNum;
extern volatile u8 D_802226E2;
extern volatile u8 D_80222730;

// per 60; 1 = 60 fps
extern volatile u8 frameRate;
extern volatile u8 D_80237A04;

extern u16 gMainMapIndex;

extern volatile u32 gDisplayContextIndex;
extern volatile u8 gfxTaskNo;
extern volatile u8 frameCount;

extern volatile u32 D_801C3B68[4];
extern volatile u8 loopStepsPerCycle;
extern volatile u8 framebufferCount;
extern volatile u8 currentFramebufferIndex;

// shared
extern volatile u16 mainLoopCallbackCurrentIndex;

// internal variable
extern volatile u8 D_80237408;

// forward declarations
void initializeEngine(void);
void func_80025F04(void);


//INCLUDE_ASM(const s32, "mainproc", func_80025D90);

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
    func_8004DF10();

    mainLoop();

}

//INCLUDE_ASM(const s32, "mainproc", initializeEngine);

void initializeEngine(void) {

    // initialize mainproc and main loop variables
    func_80025F04();

    controllerInit();
    graphicsInit();

    resetSpriteAddressesFlags();
    initializeWorldGraphics();
    initializeBitmaps();
    initializeGlobalSprites();
    // system/map.c
    func_800337D0();
    // system/message.c
    func_8003D970();
    // system/dialogue.c
    func_80042F60();
    // system/pauseScreen.c
    func_80045DE0();
    initializeCutsceneMaps();
    // system/flags.c
    func_8004DEB0();
    initializeNpcSpriteStructs();
    initializeMapContext();

    nuGfxSwapCfbFuncSet(NULL);
    nuGfxSwapCfbFuncSet(gfxBufferSwap);
    nuGfxPreNMIFuncSet(gfxPreNMICallback);

    registerMainLoopCallback(NO_OP, noOpCallback);
    setMainLoopCallbackFunctionIndex(NO_OP);

    nuGfxFuncSet((NUGfxFunc)gfxRetraceCallback);

}

//INCLUDE_ASM(const s32, "mainproc", func_80025F04);

void func_80025F04(void) {
    
    u8 i;
    
    stepMainLoop = 0;
    unknownFlag = 0;
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
    D_802226E2 = 1;
    
    D_801C3B68[0] = 0;
    D_801C3B68[1] = 0;
    D_801C3B68[2] = 0;
    D_801C3B68[3] = 0;

    gDisplayContextIndex = 0;
    
    D_80204B38 = 0;
    gMainMapIndex = 0;
    
    for (i = 0; i < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE; i++) {
        mainLoopCallbacksTable[i] = 0;
    }

}