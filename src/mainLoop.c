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
void func_800264CC(int pendingGfx);

  
//INCLUDE_ASM(s32, "mainLoop", mainLoop)

void mainLoop(void) {

    stepMainLoop = FALSE;
    unknownFlag = 1;
    
    // wait 60 frames until unknownFlag |= 2
    func_80026284();

    // toggle flags
    func_8004DEC8();
    
    // could be inline func_80026230
    D_80182BA0 = 1;
    D_8020564C = 0;

    while (TRUE) {
      
        nuGfxDisplayOn();
          
        while (unknownFlag & 1) {
            
            while (stepMainLoop == FALSE);
            
            if (!D_8020564C) { 
              
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
            // load sprites with nuPiReadRom
            dmaSprites(); 
            // sprite microcode, texturing
            func_8002AE58(); 
            // message.c
            func_80042634(); 
            // handle dialogue box close
            func_80045CB0(); 
            // no op/shelved code
            func_800293B8(); 

            stepMainLoop = FALSE; 

            // unused
            D_80237A04 = retraceCount;
              
            // update RNG seed
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

// unused
void func_80026230(u16 num) {
    D_80182BA0 = num;
}

void noOpCallback(void) {}

//INCLUDE_ASM(const s32, "mainLoop", func_80026248);

inline void func_80026248(u16 count) {

  u16 counter = 1;
  u16 currentCount;

  if (count != 0) {
    
    do {
      
      stepMainLoop = FALSE;
      
      while (stepMainLoop == FALSE);

      currentCount = counter;
      counter++;

    } while (currentCount < count);
  }

}

//INCLUDE_ASM(const s32, "mainLoop", func_80026284);

// start up before main loop
void func_80026284(void) {

    goto loop_end;
    
    // first 60 frames
    while (!(unknownFlag & 2)) {

        func_80026248(1);

loop_end:     

    }

}


// gfx retrace funcs

//INCLUDE_ASM(const s32, "mainLoop", gfxRetraceCallback);

// NUGfxFunc
u8 gfxRetraceCallback(int pendingGfx) {
  
    u8 temp;
    
    pendingGfxNum = pendingGfx;

    unknownFlag &= ~2;
    
    // controller
    func_8004CF68();

    func_800263B0(pendingGfx);
    func_800264CC(pendingGfx);
    
    if (frameCount > 59) {
        frameCount = 0;
        unknownFlag |= 2;
    }
    
    // no op
    func_8004DF00();
    
    frameCount++;
    retraceCount++;

    // reset to 0 when pendingGfx < 3
    // FIXME: unnecessary assignment
    temp = framebufferCount++;
    
    return framebufferCount;
    
}

//INCLUDE_ASM(const s32, "mainLoop", func_800263B0);

void func_800263B0(int pendingGfx) {

  u8 temp;
    
  if ((frameCount % frameRate) == 0) {
      
    if (frameCount > 59) {

      D_801C4215 = drawnFrameCount;
      drawnFrameCount = 0;

      // < hz
      if (D_801C4215 < (60 / frameRate)) {
        // unused
        D_80222730 = 2;
      }

    }
      
    if (pendingGfx < 3) { 

      temp = stepMainLoop;

      if (temp == FALSE) {
        
        drawFrame();

        drawnFrameCount += 1;
        // FIXME: unnecessary assignment, might be return value
        temp = drawnFrameCount;

        framebufferCount = 0;
        
      }
    }
  }

}

//INCLUDE_ASM(const s32, "mainLoop", func_800264CC);

void func_800264CC(int pendingGfx) {
    
    if ((frameCount % D_802226E2) == 0) {
        
        if (frameCount > 59) {

          D_8013DC30 = loopStepsPerCycle;
          loopStepsPerCycle = 0;

          // < hz
          if (D_8013DC30 < (60 / D_802226E2)) {
            // unused
            D_80222730 = 1;
          }

        }
      
        if (stepMainLoop == FALSE) {

          stepMainLoop = TRUE;

          retraceCount = 0;
          loopStepsPerCycle += 1;
          // FIXME: ? might be return value
          loopStepsPerCycle;  

        }
    }
    
}

//INCLUDE_ASM(const s32, "mainLoop", gfxBufferSwap);

void gfxBufferSwap(void *gfxTask) {
    
    currentGfxTaskPtr = gfxTask;
    nuGfxSwapCfb(gfxTask);
    
    // unused
    currentFramebufferIndex = framebufferCount;
    
}

// manual copy of library func
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
    
    while (__osSpSetPc(0));
    
    while (TRUE);

}