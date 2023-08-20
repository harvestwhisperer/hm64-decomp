#include "common.h"


// system functions

// gfx wrapper
void drawFrame(void);  

// sprites
void func_800290B8();
void resetSpriteCounter();
// no op or shelved code
u32 func_800293B8();
u32 func_8002AE58();
void func_8002D3D4();

// graphics/collisions
void func_80033058();

// map
void func_8003A1BC();
void func_8003C6E4();

// audio
void func_8003CF38();

// message
void func_80042634();
void func_80045CB0();
// pause screen
void func_800467F8();
// cutscenes
void func_80046D78();
// controller
void func_8004CF68();
void func_8004DF00();

// toggles two unused flags
void func_8004DEC8();       


// forward declarations
void func_800263B0(int);
void func_800264CC(int);


// likely bss
extern u16 mainLoopCallbackCurrentIndex;
extern void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();

extern void *currentGfxTaskPtr;

extern u16 D_80182BA0;
extern u8 D_801D6230;
extern u8 D_80237A04;
extern u8 D_80204B38;

extern volatile u8 D_8013DC30;
extern volatile u16 D_801594E4;
extern volatile u8 D_8016FB04;
extern volatile u8 D_801C3BEC;
extern volatile u8 D_801C3F34;
extern volatile u8 D_801C3F71;
extern volatile u8 D_801C4215;
extern volatile u8 D_80205208;
extern volatile u8 frameCount;
extern volatile u16 D_8020564C;
extern volatile u8 D_802226E2;
extern volatile u32 D_802226E8;
extern volatile u8 D_80222730;
extern volatile u8 D_802373F1;


//INCLUDE_ASM(s32, "mainLoop", mainLoop)

int mainLoop(void) {

    int res;
    
    D_80205208 = 0;
    D_801594E4 = 1;

    func_80026284();
    // toggle flags
    func_8004DEC8();

    D_80182BA0 = 1;
    D_8020564C = 0;

    while (TRUE) {
      
        nuGfxDisplayOn();
            
        goto inner_loop_end;
            
        while (D_801594E4 & 1) {
            
            while (!D_80205208);
            
                do { 
                    
                    if (!D_8020564C) { 

                        D_80182BA0 = 1;
                        
                        do {} while (FALSE); 

                        // handle specific logic depending on game mode/screen
                        mainLoopCallbacksTable[mainLoopCallbackCurrentIndex](); 
                        D_8020564C = D_80182BA0; 
                        
                    } 

                    D_8020564C -= 1;    
                    res = D_8020564C;

                    // reset tile flags
                    func_80029CC8();

                    // audio/music
                    func_8003CF38(); 
                    resetSpriteCounter();
                    // cutscenes 
                    func_80046D78(); 
                    // sprite graphics, collisions
                    func_80033058(); 
                    // tiling
                    func_8003C6E4(); 
                    // map graphics/display lists
                    func_8003A1BC();
                    // pause screen sprites
                    func_800467F8(); 
                    // sprite lighting adjustments
                    func_8002D3D4(); 
                    // load object sprites with nuPiReadRom
                    func_800290B8(); 
                    // sprite.c: sprite microcode, texturing
                    func_8002AE58(); 
                    // message.c
                    func_80042634(); 
                    // handle dialogue box close
                    func_80045CB0(); 
                    // no op/shelved code
                    func_800293B8(); 

                    D_80205208 = 0; 

                } while (FALSE);
        
            D_80237A04 = D_801C3BEC;

            // ?
            getRandomNumber();

inner_loop_end:
    
        }

        nuGfxDisplayOff();
    }

    return res;
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", registerMainLoopCallback);

u32 registerMainLoopCallback(u16 index, void *(func)()) {

    u32 result = 0;

    if (index < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE) {
        if (mainLoopCallbacksTable[index] == NULL) {
            mainLoopCallbacksTable[index] = func;
            result = 1;
        }
    }

    return result;

}

// matches 99% with SN compiler
#ifdef PERMUTER
u32 setMainLoopCallbackFunctionIndex(u16 index) {

    u32 result = 0;
    if (index < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE) {
        if (mainLoopCallbacksTable[index]) {
            mainLoopCallbackCurrentIndex = index;
            result = 1; 
        }
    }
    return result;
    
}
#else
INCLUDE_ASM(const s32, "mainLoop", setMainLoopCallbackFunctionIndex);
#endif

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", func_8002620C);

void func_8002620C(void) {
    D_80204B38 = 1;
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", func_80026220);

void func_80026220(void) {
    D_80204B38 = 0;
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", func_80026230);

// n64sym identified this as fbSetBg
void func_80026230(u16 arg0) {
    // identified as FB_BGCOLOR
    D_80182BA0 = arg0;
}

void noOpCallback(void) {}

//INCLUDE_ASM(const s32, "mainLoop", func_80026248);

void func_80026248(u16 arg0) {

  u16 counter = 1;
  u16 currentCount;

  if (arg0) {
    do {
      D_80205208 = 0;
      while (!D_80205208);
      currentCount = counter;
      counter++;
    } while (currentCount < arg0);
  }

}

//INCLUDE_ASM(const s32, "mainLoop", func_80026284);

void func_80026284(void) {

    u16 counter;
    u16 check;
    
    goto loop_end;
    
    while (!(D_801594E4 & 2)) {

        counter = 1;

        do {

          D_80205208 = 0;
          while (!D_80205208);
          check = counter;
          counter++;
          check = check == 0;

        } while (check);
loop_end:     
    }

}

//INCLUDE_ASM(const s32, "mainLoop", gfxRetraceCallback);

// NUGfxFunc
u8 gfxRetraceCallback(int arg0) {
  
    u8 temp;
    
    D_802226E8 = arg0;
    D_801594E4 &= 0xFFFD;
    
    // controller
    func_8004CF68();
    // checks pending gfx and calls gfx wrapper
    func_800263B0(arg0);
    func_800264CC(arg0);
    
    if (frameCount > 59) {
        frameCount = 0;
        D_801594E4 |= 2;
    }
    
    // no op
    func_8004DF00();
    
    frameCount++;
    D_801C3BEC++;
    // why?
    temp = D_801C3F71++;
    
    return D_801C3F71;
    
}

//INCLUDE_ASM(const s32, "mainLoop", func_800263B0);

void func_800263B0(int pendingGfx) {

  u8 temp;
    
  if (!(frameCount % D_802373F1)) {
      
    if (frameCount > 59) {

      D_801C4215 = D_8016FB04;
      D_8016FB04 = 0;

      if (D_801C4215 < (60 / D_802373F1)) {
        D_80222730 = 2;
      }

    }
      
    if (pendingGfx < 3) {

      temp = D_80205208;

      if (!temp) {
        
        drawFrame();

        D_8016FB04 += 1;
        // ?
        temp = D_8016FB04;
        D_801C3F71 = 0;
        
      }
    }
  }

}

//INCLUDE_ASM(const s32, "mainLoop", func_800264CC);

void func_800264CC(int arg0) {

    u8 temp;
    
    if (!(frameCount % D_802226E2)) {
        
        if (frameCount > 59) {
          D_8013DC30 = D_801C3F34;
          D_801C3F34 = 0;
          if (D_8013DC30 < (60 / D_802226E2)) {
            D_80222730 = 1;
          }
        }
      
        if (!D_80205208) {
          D_80205208 = 1;
          D_801C3BEC = 0;
          D_801C3F34 += 1;
          // ?
          temp = D_801C3F34;  
        }
    }
}

//INCLUDE_ASM(const s32, "mainLoop", gfxBufferSwap);

// buffer swap
void gfxBufferSwap(void *gfxTask) {
    currentGfxTaskPtr = gfxTask;
    nuGfxSwapCfb(gfxTask);
    D_801D6230 = D_801C3F71;
}

// static inline?
// why is a library function apparently in this file? (maybe a manual copy?)
s32 osAfterPreNMI(void) {
    return __osSpSetPc(0);
}

//INCLUDE_ASM(const s32, "mainLoop", gfxPreNMICallback);

void gfxPreNMICallback(void) {
    
    volatile int *callbackTablePtr;
    
    callbackTablePtr = &mainLoopCallbacksTable;
    
    if (*callbackTablePtr) {
        mainLoopCallbackCurrentIndex = 0;
    }
    
    nuGfxFuncSet(NULL);
    nuGfxTaskAllEndWait();
    nuGfxDisplayOff();

    osStopThread(&nusched.graphicsThread);
    osStopThread(&nusched.audioThread);
    
    osViSetYScale(1.0f);
    
    // static inline osAfterPreNMI call?  
    while (__osSpSetPc(0));
    
    while (TRUE);

}