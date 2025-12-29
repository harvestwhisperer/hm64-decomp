#include "common.h"

#include "system/controller.h"

#include "system/graphic.h"
#include "system/math.h"

#include "mainproc.h"

// bss
u16 buttonRepeatRate;
u16 buttonRepeatModeTriggerDelayFrames;
u8 contPattern;

NUContDataPatch contData[NU_CONT_MAXCONTROLLERS];

// nucontmgr.c
extern OSContStatus nuContStatus[NU_CONT_MAXCONTROLLERS];

Controller controllers[NU_CONT_MAXCONTROLLERS];
Controller gControllers[NU_CONT_MAXCONTROLLERS];


// forward declaration
void calculateAnalogStickDirection(u8);                         
      

//INCLUDE_ASM("asm/nonmatchings/system/controller", controllerInit);

void controllerInit(void) {
    
    u8 i, j;

    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        
        controllers[i].button = 0;
        controllers[i].buttonHeld = 0;
        controllers[i].buttonPressed = 0;
        controllers[i].buttonReleased = 0;
        controllers[i].buttonRepeat = 0;
        controllers[i].buttonRepeatState = 0;
        
        controllers[i].analogStick.rawX = 0;
        controllers[i].analogStick.rawY = 0;
        controllers[i].analogStick.direction = 0;
        controllers[i].analogStick.magnitude = 0;

        for (j = 0; j < 24; j++) {
            controllers[i].buttonFrameCounters[j] = 0;
        }

        gControllers[i].button = 0;
        gControllers[i].buttonHeld = 0;
        gControllers[i].buttonPressed = 0;
        gControllers[i].buttonReleased = 0;
        gControllers[i].buttonRepeat = 0;
        gControllers[i].buttonRepeatState = 0;
        
        gControllers[i].analogStick.rawX = 0;
        gControllers[i].analogStick.rawY = 0;
        gControllers[i].analogStick.direction = 0;
        gControllers[i].analogStick.magnitude = 0;

        for (j = 0; j < 24; j++) {
            gControllers[i].buttonFrameCounters[j] = 0;
        }
    }
    
    buttonRepeatRate = 4;
    buttonRepeatModeTriggerDelayFrames = 16;

    contPattern = nuContInit();
    
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", readControllerData);

// this requires button on NUContData struct to be volatile
void readControllerData(void) {
    
    u8 i, j;
    
    nuContDataGetExAll(contData);
    
    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        
        if (!nuContStatus[i].errno) {
            
            if ((frameCount % D_802226E2) == 0) {
                
                controllers[i].analogStick.rawX = contData[i].stick_x;
                controllers[i].analogStick.rawY = contData[i].stick_y;
                
                controllers[i].button = contData[i].button;
                
                calculateAnalogStickDirection(i);
                
                controllers[i].buttonPressed = (controllers[i].button ^ controllers[i].buttonHeld) & controllers[i].button;
                controllers[i].buttonReleased =  (controllers[i].button ^ controllers[i].buttonHeld) & controllers[i].buttonHeld;
                controllers[i].buttonHeld = controllers[i].button;
                
                for (j = 0; j < 24; j++) {
                    
                    // if bit set
                    if ((controllers[i].button >> j) & 1) {
                        
                        // check button mask is in button repeat mode
                        if ((controllers[i].buttonRepeatState >> j) & 1) {
                            
                            // > 4
                            // in button repeat mode
                            if (controllers[i].buttonFrameCounters[j] > buttonRepeatRate) {
                                controllers[i].buttonRepeat |= ((controllers[i].button >> j) & 1) << j;
                                controllers[i].buttonFrameCounters[j] = 0;
                            } else {
                                controllers[i].buttonRepeat &= ~(1 << j);
                            }
                            
                        // > 16
                        // 16 frame delay to trigger button repeat
                        } else if (controllers[i].buttonFrameCounters[j] > buttonRepeatModeTriggerDelayFrames) {
                            // enable button repeat mode for specific button bit
                            controllers[i].buttonRepeat |= ((controllers[i].button >> j) & 1) << j;
                            controllers[i].buttonRepeatState |= ((controllers[i].button >> j) & 1) << j;
                            controllers[i].buttonFrameCounters[j] = 0;
                        } else {
                            controllers[i].buttonRepeat = controllers[i].buttonPressed;
                        }
                        
                        // increment bit counter
                        controllers[i].buttonFrameCounters[j]++;
                        
                    } else {
                        controllers[i].buttonFrameCounters[j] = 0;
                        controllers[i].buttonRepeat &= ~(1 << j);
                        controllers[i].buttonRepeatState &= ~(1 << j);
                    }
                
                }

            } else {

                gControllers[i].button = contData[i].button;

                gControllers[i].buttonPressed = (gControllers[i].button ^ gControllers[i].buttonHeld) & gControllers[i].button;
                gControllers[i].buttonReleased = (gControllers[i].button ^ gControllers[i].buttonHeld) & gControllers[i].buttonHeld;
                
                gControllers[i].buttonHeld = gControllers[i].button;
                
                gControllers[i].analogStick.rawX = contData[i].stick_x;
                gControllers[i].analogStick.rawY = contData[i].stick_y;
                
            }
        }
    }
}


//INCLUDE_ASM("asm/nonmatchings/system/controller", checkButtonHeld);

u32 checkButtonHeld(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].button & buttonPattern;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", checkButtonPressed);

u32 checkButtonPressed(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonPressed & buttonPattern;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", checkButtonReleased);

u32 checkButtonReleased(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonReleased & buttonPattern;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", checkButtonRepeat);

u32 checkButtonRepeat(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonRepeat & buttonPattern;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getAnalogStickRawX);

s8 getAnalogStickRawX(u8 contIndex) {
    return controllers[contIndex].analogStick.rawX;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getAnalogStickRawY);

s8 getAnalogStickRawY(u8 contIndex) {
    return controllers[contIndex].analogStick.rawY;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getAnalogStickDirection);

u8 getAnalogStickDirection(u8 contIndex) {
    return controllers[contIndex].analogStick.direction;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getAnalogStickMagnitude);

u8 getAnalogStickMagnitude(u8 contIndex) {
    return controllers[contIndex].analogStick.magnitude;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", calculateAnalogStickDirection);

void calculateAnalogStickDirection(u8 controllerIndex) {

    volatile AnalogStick analogStick;

    analogStick.rawX = controllers[controllerIndex].analogStick.rawX;
    analogStick.rawY = controllers[controllerIndex].analogStick.rawY;

    // deadzone
    analogStick.direction = (s16)(getAbsoluteValue(analogStick.rawX)) / 10;
    analogStick.magnitude = (s16)(getAbsoluteValue(analogStick.rawY)) / 10;
    
    controllers[controllerIndex].analogStick.direction = 0;

    if (analogStick.direction < 3 && analogStick.magnitude < 3) {
        
        controllers[controllerIndex].analogStick.direction = 0xFF;
        
    } else {
        
        if (analogStick.direction > analogStick.magnitude) {
                    
            controllers[controllerIndex].analogStick.magnitude = analogStick.direction - analogStick.magnitude;
            
            if ((analogStick.direction / 2) < controllers[controllerIndex].analogStick.magnitude) {
                if ((analogStick.rawX << 0x18) < 0) {
                    controllers[controllerIndex].analogStick.direction = NORTHWEST;
                } else {
                    controllers[controllerIndex].analogStick.direction = SOUTHEAST;
                }
            } else if ((analogStick.rawY << 0x18) < 0) {
                if ((analogStick.rawX << 0x18) < 0) {
                    controllers[controllerIndex].analogStick.direction = WEST;
                } else {
                    controllers[controllerIndex].analogStick.direction = SOUTH;
                }
            } else {
                if ((analogStick.rawX << 0x18) < 0) {
                    controllers[controllerIndex].analogStick.direction = NORTH;
                } else {
                    controllers[controllerIndex].analogStick.direction = EAST;
                }
    
            }
            
            controllers[controllerIndex].analogStick.magnitude = analogStick.direction;
            
        } else {

            controllers[controllerIndex].analogStick.magnitude = analogStick.magnitude - analogStick.direction;
            
            if ((analogStick.magnitude / 2) < controllers[controllerIndex].analogStick.magnitude) {
                if ((analogStick.rawY << 24) < 0) {
                    controllers[controllerIndex].analogStick.direction = SOUTHWEST;
                } else {
                    controllers[controllerIndex].analogStick.direction = NORTHEAST;
                } 
            } else if ((analogStick.rawX << 24) < 0) {
                if ((analogStick.rawY << 24) < 0) {
                    controllers[controllerIndex].analogStick.direction = WEST;
                } else {
                    controllers[controllerIndex].analogStick.direction = NORTH;
                }
            } else {
                if ((analogStick.rawY << 24) < 0) { 
                    controllers[controllerIndex].analogStick.direction = SOUTH;
                } else {
                    controllers[controllerIndex].analogStick.direction = EAST;
                }
            }
    
            controllers[controllerIndex].analogStick.magnitude = analogStick.magnitude;
            
        }
     }
    
    if (controllers[controllerIndex].analogStick.magnitude >= MAX_DIRECTIONS) {
        controllers[controllerIndex].analogStick.magnitude = SOUTHEAST;
    }
    
    controllers[controllerIndex].button |= (0x10000 << (controllers[controllerIndex].analogStick.direction)); 
   
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004D788);

bool func_8004D788(u8 contIndex) {
    
    bool result;
    u32 error;
    
    bool temp;
    
    nuContPakOpen(&controllers[contIndex].pak, contIndex);
    
    result = controllers[contIndex].pak.type != 2;
    
    error = 2;
    
    if (controllers[contIndex].pak.error) {
        
        temp = controllers[contIndex].pak.error == 2;

        result = FALSE;
        
        if (controllers[contIndex].pak.error == 10)  {
            nuContPakRepairId(&controllers[contIndex].pak);
            if (controllers[contIndex].pak.error == error) {
                temp = TRUE;
            }
        }
        
        if (temp) {
            if (func_8004D788(contIndex)) {
                result = TRUE;
            }
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004D87C);

s32 func_8004D87C(u8 contIndex) {
    return nuContPakGetFree(&controllers[contIndex].pak);
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004D8B4);

u32 func_8004D8B4(u8 contIndex, s32 *max_files, s32 *used_files) {
    nuContPakFileNum(&controllers[contIndex].pak, max_files, used_files);
    return controllers[contIndex].pak.error == 0;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004D904);

u32 func_8004D904(u8 contIndex, u8 *companyCode, u8 *gameCode) {
    nuContPakCodeSet(companyCode, gameCode);
    return !controllers[contIndex].pak.error;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004D954);

u32 func_8004D954(u8 contIndex, u8* noteName, u8 *extName) {
    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);
    return !controllers[contIndex].pak.error;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004D9AC);

u32 func_8004D9AC(u8 contIndex, u8 *noteName, u8 *extName, s32 offset, s32 size, s32 buf) {

    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);

    if (!controllers[contIndex].pak.error) {
        nuContPakFileReadWrite(&controllers[contIndex].pak, offset, size, buf, 0);
        return !controllers[contIndex].pak.error;
    }
    
    return 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004DA48);

u32 func_8004DA48(u8 contIndex, u8* noteName, u8* extName, s32 offset, s32 size, u8* buf) {

    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, 1, size);

    if (!controllers[contIndex].pak.error) {
        nuContPakFileReadWrite(&controllers[contIndex].pak, offset, size, buf, NU_CONT_PAK_MODE_CREATE);
        return !controllers[contIndex].pak.error;
    }

    return 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/controller", func_8004DAF4);

u32 func_8004DAF4(u8 contIndex, u8 *noteName, u8 *extName) {
    
    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);

    if (!controllers[contIndex].pak.error) {
        nuContPakFileDeleteJis(&controllers[contIndex].pak, noteName, extName);
        return !controllers[contIndex].pak.error;
    }
    
    return 0;

}
