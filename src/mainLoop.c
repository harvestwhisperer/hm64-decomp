#include "common.h"

#include "mainLoop.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/cutscene.h"
#include "system/flags.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/message.h"
#include "system/overlayScreenSprites.h"
#include "system/sprite.h"
#include "system/mapController.h"
#include "system/sceneGraph.h"

#include "mainproc.h"

// bss
volatile u16 mainLoopCallbackCurrentIndex;
void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();

volatile u16 D_80182BA0;
volatile u16 D_8020564C;
void *currentGfxTaskPtr;

// forward declarations
void func_80026284(void);
void handleGraphicsUpdate(int pendingGfx);
void updateMainLoopTimer(int pendingGfx);

  
//INCLUDE_ASM("asm/nonmatchings/mainLoop", mainLoop)

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
            
            resetBitmaps();
            updateAudio(); 
            resetSceneNodeCounter();
            updateCutsceneExecutors();
            updateEntities();
            updateMapController();
            updateMapGraphics();
            updateOverlayScreenSprites();
            updateSprites(); 
            dmaSprites(); 
            updateBitmaps(); 
            updateMessageBox(); 
            updateDialogues();

            // no op/shelved/debug code
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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", registerMainLoopCallback);

bool registerMainLoopCallback(u16 index, void *(func)()) {

    bool result = FALSE;

    if (index < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE) {
        if (mainLoopCallbacksTable[index] == NULL) {
            mainLoopCallbacksTable[index] = func;
            result = TRUE;
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/mainLoop", setMainLoopCallbackFunctionIndex);

u32 setMainLoopCallbackFunctionIndex(u16 index) {
    
    u32 result = FALSE;

    volatile int *temp;
    
    if (index < MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE) {

        temp = (volatile int*)mainLoopCallbacksTable;
      
        if (temp[index]) {
            mainLoopCallbackCurrentIndex = index;
            result = TRUE;
        }
    } 
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/mainLoop", func_8002620C);

// unused
void func_8002620C(void) {
    D_80204B38 = 1;
}

//INCLUDE_ASM("asm/nonmatchings/mainLoop", func_80026220);

// unused
void func_80026220(void) {
    D_80204B38 = 0;
}

//INCLUDE_ASM("asm/nonmatchings/mainLoop", func_80026230);

// unused
void func_80026230(u16 num) {
    D_80182BA0 = num;
}

void noOpCallback(void) {}

//INCLUDE_ASM("asm/nonmatchings/mainLoop", func_80026248);

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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", func_80026284);

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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", gfxRetraceCallback);

// NUGfxFunc
u8 gfxRetraceCallback(int pendingGfx) {
  
    u8 temp;
    
    pendingGfxNum = pendingGfx;

    unknownFlag &= ~2;
    
    readControllerData();

    handleGraphicsUpdate(pendingGfx);
    updateMainLoopTimer(pendingGfx);
    
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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", handleGraphicsUpdate);

void handleGraphicsUpdate(int pendingGfx) {

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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", updateMainLoopTimer);

void updateMainLoopTimer(int pendingGfx) {
    
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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", gfxBufferSwap);

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

//INCLUDE_ASM("asm/nonmatchings/mainLoop", gfxPreNMICallback);

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