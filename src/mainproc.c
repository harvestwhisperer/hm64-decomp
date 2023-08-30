#include "common.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/flags.h"
#include "system/graphic.h"
#include "system/message.h"
#include "system/pauseScreen.h"
#include "system/sprite.h"
#include "system/tiles.h"
#include "system/worldGraphics.h"

#include "gameStart.h"
#include "mainproc.h"
#include "mainLoop.h"                                                 

// bss
volatile u16 D_801594E4;
volatile u8 D_8016FB04;
volatile u8 D_8013DC30;
volatile u8 D_8016FB04;
volatile u8 D_801C3BEC;
volatile u8 D_801C3F34;
volatile u8 D_801C3F71;
volatile u8 D_801C4215;
u16 D_801FD610;
volatile u8 D_80204B38;
volatile u8 D_80205208;
volatile u16 D_8020564C;
volatile u32 D_802226E8;
volatile u8 D_802226E2;
volatile u8 D_80222730;
volatile u8 D_802373F1;
volatile u8 D_80237A04;
volatile u8 frameCount;

volatile u32 gDisplayContext;
volatile u8 gfxTaskNo;

volatile u32 D_801C3B68[4];
volatile u8 D_801C3F34;
volatile u8 D_801C3F71;
volatile u8 D_801D6230;

// internal variables
volatile u16 D_8020564A;
volatile u8 D_80237408;


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

    initializeAll();
    
    // load "no controller" screen or intro cutscene
    func_8004DF10();

    mainLoop();

}

//INCLUDE_ASM(const s32, "mainproc", initializeAll);

void initializeAll(void) {

    func_80025F04();

    controllerInit();
    graphicsInit();

    resetSpriteAddressesFlags();
    func_80029170();
    func_80029B30();
    func_8002AFE0();
    func_800337D0();
    func_8003D970();
    func_80042F60();
    func_80045DE0();
    func_80046860();
    func_8004DEB0();
    initializeNpcSpriteStructs();
    func_8003B870();

    nuGfxSwapCfbFuncSet(NULL);
    nuGfxSwapCfbFuncSet(gfxBufferSwap);
    nuGfxPreNMIFuncSet(gfxPreNMICallback);

    registerMainLoopCallback(0, noOpCallback);
    setMainLoopCallbackFunctionIndex(0);

    nuGfxFuncSet((NUGfxFunc)gfxRetraceCallback);
}

//INCLUDE_ASM(const s32, "mainproc", func_80025F04);

void func_80025F04(void) {
    
    u8 i;
    
    D_80205208 = 0;
    D_801594E4 = 0;
    D_8020564A = 0;

    frameCount = 0;

    D_8016FB04 = 0;
    D_801C3F71 = 0;
    D_801D6230 = 0;
    D_801C3F34 = 0;
    D_801C3BEC = 0;
    D_80237A04 = 0;

    gfxTaskNo = 0xFF;

    D_80237408 = 0;
    D_802373F1 = 1;
    D_802226E2 = 1;
    
    D_801C3B68[0] = 0;
    D_801C3B68[1] = 0;
    D_801C3B68[2] = 0;
    D_801C3B68[3] = 0;

    gDisplayContext = 0;
    
    D_80204B38 = 0;
    D_801FD610 = 0;
    
    for (i = 0; i < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE; i++) {
        mainLoopCallbacksTable[i] = 0;
    }

}