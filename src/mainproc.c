#include "common.h"

void initializeAll();                                                                    
void func_8004DF10();               

void mainLoop();

void registerMainLoopCallback(int, void*);       
void setMainLoopCallbackFunctionIndex(int);    

void controllerInit();
void graphicsInit();  

void func_80025F04();           
void resetSpriteAddressesFlags();                                   
void func_80029170();                                   
void func_80029B30();                                   
void func_8002AFE0();                                   
void initializeNpcSpriteStructs();                                   
void func_800337D0();                                   
void func_8003B870();                                   
void func_8003D970();                                   
void func_80042F60();                                   
void func_80045DE0();                                   
void func_80046860();                                             
void func_8004DEB0();              
 
void nuGfxPreNMIFuncSet(void (*func)(void));                                                      

// fix name
void noOpCallback(void);

void gfxRetraceCallback(void);
void gfxBufferSwap(void);
void gfxPreNMICallback(void);

extern u8 gfxTaskNo;

extern volatile u16 D_801594E4;
extern volatile u8 D_8016FB04;

extern volatile u32 D_801C3B68[4];
extern volatile u8 D_801C3BEC;
extern volatile u8 D_801C3F34;
extern volatile u8 D_801C3F71;
extern volatile u8 D_801D6230;
extern volatile u16 D_801FD610;
extern volatile u8 D_80204B38;
extern volatile u8 D_80205208;
extern volatile u8 D_8020520A;
extern volatile u32 D_80205630;
extern volatile u8 D_80205634;
extern volatile u16 D_8020564A;
extern volatile u8 D_802226E2;
extern volatile u8 D_802373F1;
extern volatile u8 D_80237408;
extern volatile u8 D_80237A04;

extern u8 frameCount;
extern u32 gDisplayContext;

extern u16 mainLoopCallbackCurrentIndex;
extern void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();


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
    D_80205634 = 0;
    D_8016FB04 = 0;
    D_801C3F71 = 0;
    D_801D6230 = 0;
    D_801C3F34 = 0;
    D_801C3BEC = 0;
    D_80237A04 = 0;
    D_8020520A = 0xFF;
    D_80237408 = 0;
    D_802373F1 = 1;
    D_802226E2 = 1;
    
    D_801C3B68[0] = 0;
    D_801C3B68[1] = 0;
    D_801C3B68[2] = 0;
    D_801C3B68[3] = 0;

    D_80205630 = 0;
    D_80204B38 = 0;
    D_801FD610 = 0;
    
    for (i = 0; i < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE; i++) {
        mainLoopCallbacksTable[i] = 0;
    }

}