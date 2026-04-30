#include "common.h"

#include "system/controller.h"

#include "system/graphic.h"
#include "system/math.h"

#include "mainproc.h"

// bss
u16 buttonRepeatRate;
u16 buttonRepeatModeTriggerDelayFrames;
u8 contPattern;

NUContData contData[NU_CONT_MAXCONTROLLERS];

// nucontmgr.c
extern OSContStatus nuContStatus[NU_CONT_MAXCONTROLLERS];

Controller controllers[NU_CONT_MAXCONTROLLERS];
Controller gControllers[NU_CONT_MAXCONTROLLERS];

// forward declaration
void calculateAnalogStickDirection(u8);                         

void controllerInit(void) {
    
    u8 i, j;

    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        Controller *c = &controllers[i];
        Controller *gc = &gControllers[i];

        c->button = 0;
        c->buttonHeld = 0;
        c->buttonPressed = 0;
        c->buttonReleased = 0;
        c->buttonRepeat = 0;
        c->buttonRepeatState = 0;

        c->analogStick.rawX = 0;
        c->analogStick.rawY = 0;
        c->analogStick.direction = 0;
        c->analogStick.magnitude = 0;

        for (j = 0; j < 24; j++) {
            c->buttonFrameCounters[j] = 0;
        }

        gc->button = 0;
        gc->buttonHeld = 0;
        gc->buttonPressed = 0;
        gc->buttonReleased = 0;
        gc->buttonRepeat = 0;
        gc->buttonRepeatState = 0;

        gc->analogStick.rawX = 0;
        gc->analogStick.rawY = 0;
        gc->analogStick.direction = 0;
        gc->analogStick.magnitude = 0;

        for (j = 0; j < 24; j++) {
            gc->buttonFrameCounters[j] = 0;
        }
    }
    
    buttonRepeatRate = 4;
    buttonRepeatModeTriggerDelayFrames = 16;

    contPattern = nuContInit();
    
}

// this requires button on NUContData struct to be volatile
void readControllerData(void) {
    
    u8 i, j;
    
    nuContDataGetExAll(contData);
    
    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        Controller *c = &controllers[i];
        Controller *gc = &gControllers[i];
        NUContData *cd = &contData[i];

        if (!nuContStatus[i].errno) {

            if ((frameCount % mainLoopUpdateRate) == 0) {

                c->analogStick.rawX = cd->stick_x;
                c->analogStick.rawY = cd->stick_y;

                c->button = cd->button;

                calculateAnalogStickDirection(i);

                c->buttonPressed = (c->button ^ c->buttonHeld) & c->button;
                c->buttonReleased =  (c->button ^ c->buttonHeld) & c->buttonHeld;
                c->buttonHeld = c->button;

                for (j = 0; j < 24; j++) {

                    // if bit set
                    if ((c->button >> j) & 1) {

                        // check button mask is in button repeat mode
                        if ((c->buttonRepeatState >> j) & 1) {

                            // > 4
                            // in button repeat mode
                            if (c->buttonFrameCounters[j] > buttonRepeatRate) {
                                c->buttonRepeat |= ((c->button >> j) & 1) << j;
                                c->buttonFrameCounters[j] = 0;
                            } else {
                                c->buttonRepeat &= ~(1 << j);
                            }

                        // > 16
                        // 16 frame delay to trigger button repeat
                        } else if (c->buttonFrameCounters[j] > buttonRepeatModeTriggerDelayFrames) {
                            // enable button repeat mode for specific button bit
                            c->buttonRepeat |= ((c->button >> j) & 1) << j;
                            c->buttonRepeatState |= ((c->button >> j) & 1) << j;
                            c->buttonFrameCounters[j] = 0;
                        } else {
                            c->buttonRepeat = c->buttonPressed;
                        }

                        // increment bit counter
                        c->buttonFrameCounters[j]++;

                    } else {
                        c->buttonFrameCounters[j] = 0;
                        c->buttonRepeat &= ~(1 << j);
                        c->buttonRepeatState &= ~(1 << j);
                    }

                }

            } else {

                gc->button = cd->button;

                gc->buttonPressed = (gc->button ^ gc->buttonHeld) & gc->button;
                gc->buttonReleased = (gc->button ^ gc->buttonHeld) & gc->buttonHeld;

                gc->buttonHeld = gc->button;

                gc->analogStick.rawX = cd->stick_x;
                gc->analogStick.rawY = cd->stick_y;

            }
        }
    }
}

u32 checkButtonHeld(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].button & buttonPattern;
}

u32 checkButtonPressed(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonPressed & buttonPattern;
}

u32 checkButtonReleased(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonReleased & buttonPattern;
}

u32 checkButtonRepeat(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonRepeat & buttonPattern;
}

s8 getAnalogStickRawX(u8 contIndex) {
    return controllers[contIndex].analogStick.rawX;
}

s8 getAnalogStickRawY(u8 contIndex) {
    return controllers[contIndex].analogStick.rawY;
}

u8 getAnalogStickDirection(u8 contIndex) {
    return controllers[contIndex].analogStick.direction;
}

u8 getAnalogStickMagnitude(u8 contIndex) {
    return controllers[contIndex].analogStick.magnitude;
}

void calculateAnalogStickDirection(u8 controllerIndex) {

    Controller *c = &controllers[controllerIndex];

    volatile AnalogStick analogStick;

    analogStick.rawX = c->analogStick.rawX;
    analogStick.rawY = c->analogStick.rawY;

    // deadzone
    analogStick.direction = (s16)(getAbsoluteValue(analogStick.rawX)) / 10;
    analogStick.magnitude = (s16)(getAbsoluteValue(analogStick.rawY)) / 10;
    
    c->analogStick.direction = 0;

    if (analogStick.direction < 3 && analogStick.magnitude < 3) {
        
        c->analogStick.direction = 0xFF;
        
    } else {
        
        if (analogStick.direction > analogStick.magnitude) {
                    
            c->analogStick.magnitude = analogStick.direction - analogStick.magnitude;
            
            if ((analogStick.direction / 2) < c->analogStick.magnitude) {
                if ((analogStick.rawX << 0x18) < 0) {
                    c->analogStick.direction = DIRECTION_W;
                } else {
                    c->analogStick.direction = DIRECTION_E;
                }
            } else if ((analogStick.rawY << 0x18) < 0) {
                if ((analogStick.rawX << 0x18) < 0) {
                    c->analogStick.direction = DIRECTION_SW;
                } else {
                    c->analogStick.direction = DIRECTION_SE;
                }
            } else {
                if ((analogStick.rawX << 0x18) < 0) {
                    c->analogStick.direction = DIRECTION_NW;
                } else {
                    c->analogStick.direction = DIRECTION_NE;
                }
    
            }
            
            c->analogStick.magnitude = analogStick.direction;
            
        } else {

            c->analogStick.magnitude = analogStick.magnitude - analogStick.direction;
            
            if ((analogStick.magnitude / 2) < c->analogStick.magnitude) {
                if ((analogStick.rawY << 24) < 0) {
                    c->analogStick.direction = DIRECTION_S;
                } else {
                    c->analogStick.direction = DIRECTION_N;
                } 
            } else if ((analogStick.rawX << 24) < 0) {
                if ((analogStick.rawY << 24) < 0) {
                    c->analogStick.direction = DIRECTION_SW;
                } else {
                    c->analogStick.direction = DIRECTION_NW;
                }
            } else {
                if ((analogStick.rawY << 24) < 0) { 
                    c->analogStick.direction = DIRECTION_SE;
                } else {
                    c->analogStick.direction = DIRECTION_NE;
                }
            }
    
            c->analogStick.magnitude = analogStick.magnitude;
            
        }
     }
    
    if (c->analogStick.magnitude >= MAX_DIRECTIONS) {
        c->analogStick.magnitude = DIRECTION_E;
    }
    
    c->button |= (0x10000 << (c->analogStick.direction)); 
   
}

bool func_8004D788(u8 contIndex) {

    Controller *c = &controllers[contIndex];
    
    bool result;
    u32 error;
    
    bool temp;
    
    nuContPakOpen(&c->pak, contIndex);
    
    result = c->pak.type != 2;
    
    error = 2;
    
    if (c->pak.error) {
        
        temp = c->pak.error == 2;

        result = FALSE;
        
        if (c->pak.error == 10)  {
            nuContPakRepairId(&c->pak);
            if (c->pak.error == error) {
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

s32 func_8004D87C(u8 contIndex) {
    return nuContPakGetFree(&controllers[contIndex].pak);
}

u32 func_8004D8B4(u8 contIndex, s32 *max_files, s32 *used_files) {

    Controller *c = &controllers[contIndex];
    nuContPakFileNum(&c->pak, max_files, used_files);
    return c->pak.error == 0;
}

u32 func_8004D904(u8 contIndex, u8 *companyCode, u8 *gameCode) {
    nuContPakCodeSet(companyCode, gameCode);
    return !controllers[contIndex].pak.error;
}

u32 func_8004D954(u8 contIndex, u8* noteName, u8 *extName) {
    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);
    return !controllers[contIndex].pak.error;
}

u32 func_8004D9AC(u8 contIndex, u8 *noteName, u8 *extName, s32 offset, s32 size, s32 buf) {

    Controller *c = &controllers[contIndex];

    nuContPakFileOpenJis(&c->pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);

    if (!c->pak.error) {
        nuContPakFileReadWrite(&c->pak, offset, size, buf, 0);
        return !c->pak.error;
    }
    
    return 0;

}

u32 func_8004DA48(u8 contIndex, u8* noteName, u8* extName, s32 offset, s32 size, u8* buf) {

    Controller *c = &controllers[contIndex];

    nuContPakFileOpenJis(&c->pak, noteName, extName, 1, size);

    if (!c->pak.error) {
        nuContPakFileReadWrite(&c->pak, offset, size, buf, NU_CONT_PAK_MODE_CREATE);
        return !c->pak.error;
    }

    return 0;

}

u32 func_8004DAF4(u8 contIndex, u8 *noteName, u8 *extName) {

    Controller *c = &controllers[contIndex];
    
    nuContPakFileOpenJis(&c->pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);

    if (!c->pak.error) {
        nuContPakFileDeleteJis(&c->pak, noteName, extName);
        return !c->pak.error;
    }
    
    return 0;

}
