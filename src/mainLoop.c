#include "common.h"

#include "mainLoop.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/cutscene.h"
#include "system/flags.h"
#include "system/graphic.h"
#include "system/message.h"
#include "system/pauseScreen.h"
#include "system/sprite.h"
#include "system/mapContext.h"
#include "system/worldGraphics.h"

#include "mainproc.h"

// bss
volatile u16 mainLoopCallbackCurrentIndex;
void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();

volatile u16 D_80182BA0;
volatile u16 D_8020564C;
void *currentGfxTaskPtr;

// forward declarations
void func_80026284(void);
void func_800263B0(int pendingGfx);
void func_800264CC(int arg0);

  
//INCLUDE_ASM(s32, "mainLoop", mainLoop)

void mainLoop(void) {

    D_80205208 = 0;
    D_801594E4 = 1;
    
    // wait for frames?
    func_80026284();
    // toggle flags
    func_8004DEC8();
    
    // could be inline func_80026230
    D_80182BA0 = 1;
    D_8020564C = 0;

    while (TRUE) {
      
        nuGfxDisplayOn();
          
        while (D_801594E4 & 1) {
            
            while (!D_80205208);
            
            if (!D_8020564C) { 
              
              // could be inline func_80026230
              D_80182BA0 = 1;
              
              // handle specific logic depending on game mode/screen
              mainLoopCallbacksTable[mainLoopCallbackCurrentIndex](); 

              D_8020564C = D_80182BA0; 

            } 
            
            D_8020564C -= 1;    
            // FIXME: ?
            D_8020564C;
            
            // reset bitmap flags
            func_80029CC8();

            mainLoopAudioHandler(); 
            resetSpriteCounter();
            mainLoopCutsceneHandler(); 

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
            dmaSprites(); 
            // sprite.c: sprite microcode, texturing
            func_8002AE58(); 
            // message.c
            func_80042634(); 
            // handle dialogue box close
            func_80045CB0(); 
            // no op/shelved code
            func_800293B8(); 

            D_80205208 = 0; 
            D_80237A04 = D_801C3BEC;
              
            rand();
            
        }
    
        nuGfxDisplayOff();
    }
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", registerMainLoopCallback);

bool registerMainLoopCallback(u16 index, void *(func)()) {

    bool result = 0;

    if (index < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE) {
        if (mainLoopCallbacksTable[index] == NULL) {
            mainLoopCallbacksTable[index] = func;
            result = 1;
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "mainLoop", setMainLoopCallbackFunctionIndex);

u32 setMainLoopCallbackFunctionIndex(u16 index) {
    
    u32 result = 0;

    volatile int *temp;
    
    if (index < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE) {

        temp = (volatile int*)mainLoopCallbacksTable;
      
        if (temp[index]) {
            mainLoopCallbackCurrentIndex = index;
            result = 1;
        }
    } 
    
    return result;
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", func_8002620C);

// unused
void func_8002620C(void) {
    D_80204B38 = 1;
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", func_80026220);

// unused
void func_80026220(void) {
    D_80204B38 = 0;
}

//INCLUDE_ASM(const s32, "registerMainLoopCallbacks", func_80026230);

// unused or inline
// n64sym identified this as fbSetBg
void func_80026230(u16 arg0) {
    // identified as FB_BGCOLOR
    D_80182BA0 = arg0;

}

void noOpCallback(void) {}

//INCLUDE_ASM(const s32, "mainLoop", func_80026248);

// unused or inline
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

// start up before main loop
void func_80026284(void) {

    u16 counter;
    u16 check;
    
    goto loop_end;
    
    // after frame count exceeds 60 and before gfx retrace callback is called again
    while (!(D_801594E4 & 2)) {

        // TODO: replace with inline func_80026248

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


// gfx retrace funcs

//INCLUDE_ASM(const s32, "mainLoop", gfxRetraceCallback);

// NUGfxFunc
u8 gfxRetraceCallback(int arg0) {
  
    u8 temp;
    
    D_802226E8 = arg0;
    D_801594E4 &= ~2;
    
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

    // FIXME: unnecessary assignment
    temp = D_801C3F71++;
    
    return D_801C3F71;
    
}

//INCLUDE_ASM(const s32, "mainLoop", func_800263B0);

void func_800263B0(int pendingGfx) {

  u8 temp;
    
  if ((frameCount % D_802373F1) == 0) {
      
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
        // FIXME: unnecessary assignment, might be return value
        temp = D_8016FB04;
        D_801C3F71 = 0;
        
      }
    }
  }

}

//INCLUDE_ASM(const s32, "mainLoop", func_800264CC);

void func_800264CC(int arg0) {
    
    if ((frameCount % D_802226E2) == 0) {
        
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
          // FIXME: ? might be return value
          D_801C3F34;  

        }
    }
}

//INCLUDE_ASM(const s32, "mainLoop", gfxBufferSwap);

void gfxBufferSwap(void *gfxTask) {
    currentGfxTaskPtr = gfxTask;
    nuGfxSwapCfb(gfxTask);
    D_801D6230 = D_801C3F71;
}

// FIXME: static inline?
// manual copy of library func?
s32 osAfterPreNMI(void) {
    return __osSpSetPc(0);
}

//INCLUDE_ASM(const s32, "mainLoop", gfxPreNMICallback);

void gfxPreNMICallback(void) {
    
    volatile int *callbackTablePtr;
    
    callbackTablePtr = (volatile int*)&mainLoopCallbacksTable;
    
    if (*callbackTablePtr) {
        mainLoopCallbackCurrentIndex = 0;
    }
    
    nuGfxFuncSet(NULL);
    nuGfxTaskAllEndWait();
    nuGfxDisplayOff();

    osStopThread(&nusched.graphicsThread);
    osStopThread(&nusched.audioThread);
    
    osViSetYScale(1.0f);
    
    // FIXME: maybe should be static inline osAfterPreNMI call?  
    while (__osSpSetPc(0));
    
    while (TRUE);

}