#include "common.h"

#include "system/controller.h"

#include "system/graphic.h"
#include "system/math.h"

#include "mainproc.h"

// bss
u16 D_80182FBA;
u16 D_801FADB0;
u8 contPattern;

// forward declaration
void convertAnalogSticksToDirections(u8);                         
      

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
        
        controllers[i].sticks.s_stick_x = 0;
        controllers[i].sticks.s_stick_y = 0;
        controllers[i].sticks.u_stick_x = 0;
        controllers[i].sticks.u_stick_y = 0;

        for (j = 0; j < 24; j++) {
            controllers[i].buttonFrameCounters[j] = 0;
        }

        gControllers[i].button = 0;
        gControllers[i].buttonHeld = 0;
        gControllers[i].buttonPressed = 0;
        gControllers[i].buttonReleased = 0;
        gControllers[i].buttonRepeat = 0;
        gControllers[i].buttonRepeatState = 0;
        
        gControllers[i].sticks.s_stick_x = 0;
        gControllers[i].sticks.s_stick_y = 0;
        gControllers[i].sticks.u_stick_x = 0;
        gControllers[i].sticks.u_stick_y = 0;

        for (j = 0; j < 24; j++) {
            gControllers[i].buttonFrameCounters[j] = 0;
        }
    }
    
    D_80182FBA = 4;
    D_801FADB0 = 0x10;

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
                
                controllers[i].sticks.s_stick_x = contData[i].stick_x;
                controllers[i].sticks.s_stick_y = contData[i].stick_y;
                
                controllers[i].button = contData[i].button;
                
                convertAnalogSticksToDirections(i);
                
                controllers[i].buttonPressed = (controllers[i].button ^ controllers[i].buttonHeld) & controllers[i].button;
                controllers[i].buttonReleased =  (controllers[i].button ^ controllers[i].buttonHeld) & controllers[i].buttonHeld;
                controllers[i].buttonHeld = controllers[i].button;
                
                for (j = 0; j < 24; j++) {
                    
                    // if bit set
                    if ((controllers[i].button >> j) & 1) {
                        
                        if ((controllers[i].buttonRepeatState >> j) & 1) {
                            
                            // > 4
                            if (controllers[i].buttonFrameCounters[j] > D_80182FBA) {
                                controllers[i].buttonRepeat |= ((controllers[i].button >> j) & 1) << j;
                                controllers[i].buttonFrameCounters[j] = 0;
                            } else {
                                controllers[i].buttonRepeat &= ~(1 << j);
                            }
                            
                            // > 16
                        } else if (controllers[i].buttonFrameCounters[j] > D_801FADB0) {
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
                
                gControllers[i].sticks.s_stick_x = contData[i].stick_x;
                gControllers[i].sticks.s_stick_y = contData[i].stick_y;
                
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

//INCLUDE_ASM("asm/nonmatchings/system/controller", getStickXValueSigned);

s8 getStickXValueSigned(u8 contIndex) {
    return controllers[contIndex].sticks.s_stick_x;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getStickYValueSigned);

s8 getStickYValueSigned(u8 contIndex) {
    return controllers[contIndex].sticks.s_stick_y;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getStickXValueUnsigned);

u8 getStickXValueUnsigned(u8 contIndex) {
    return controllers[contIndex].sticks.u_stick_x;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", getStickYValueUnsigned);

u8 getStickYValueUnsigned(u8 contIndex) {
    return controllers[contIndex].sticks.u_stick_y;
}

//INCLUDE_ASM("asm/nonmatchings/system/controller", convertAnalogSticksToDirections);

void convertAnalogSticksToDirections(u8 controllerIndex) {

    volatile Sticks sticks;

    sticks.s_stick_x = controllers[controllerIndex].sticks.s_stick_x;
    sticks.s_stick_y = controllers[controllerIndex].sticks.s_stick_y;

    // deadzone
    sticks.u_stick_x = (s16)(getAbsoluteValue(sticks.s_stick_x)) / 10;
    sticks.u_stick_y = (s16)(getAbsoluteValue(sticks.s_stick_y)) / 10;
    
    controllers[controllerIndex].sticks.u_stick_x = 0;

    if (sticks.u_stick_x < NORTH && sticks.u_stick_y < NORTH) {
        
        controllers[controllerIndex].sticks.u_stick_x = 0xFF;
        
    } else {
        
        if (sticks.u_stick_x > sticks.u_stick_y) {
                    
            controllers[controllerIndex].sticks.u_stick_y = sticks.u_stick_x - sticks.u_stick_y;
            
            if ((sticks.u_stick_x / 2) < controllers[controllerIndex].sticks.u_stick_y) {
                if ((sticks.s_stick_x << 0x18) < 0) {
                    controllers[controllerIndex].sticks.u_stick_x = NORTHWEST;
                } else {
                    controllers[controllerIndex].sticks.u_stick_x = SOUTHEAST;
                }
            } else if ((sticks.s_stick_y << 0x18) < 0) {
                if ((sticks.s_stick_x << 0x18) < 0) {
                    controllers[controllerIndex].sticks.u_stick_x = WEST;
                } else {
                    controllers[controllerIndex].sticks.u_stick_x = SOUTH;
                }
            } else {
                if ((sticks.s_stick_x << 0x18) < 0) {
                    controllers[controllerIndex].sticks.u_stick_x = NORTH;
                } else {
                    controllers[controllerIndex].sticks.u_stick_x = EAST;
                }
    
            }
            
            controllers[controllerIndex].sticks.u_stick_y = sticks.u_stick_x;
            
        } else {

            controllers[controllerIndex].sticks.u_stick_y = sticks.u_stick_y - sticks.u_stick_x;
            
            if ((sticks.u_stick_y / 2) < controllers[controllerIndex].sticks.u_stick_y) {
                if ((sticks.s_stick_y << 0x18) < 0) {
                    controllers[controllerIndex].sticks.u_stick_x = SOUTHWEST;
                } else {
                    controllers[controllerIndex].sticks.u_stick_x = NORTHEAST;
                } 
            } else if ((sticks.s_stick_x << 0x18) < 0) {
                if ((sticks.s_stick_y << 0x18) < 0) {
                    controllers[controllerIndex].sticks.u_stick_x = WEST;
                } else {
                    controllers[controllerIndex].sticks.u_stick_x = NORTH;
                }
            } else {
                if ((sticks.s_stick_y << 0x18) < 0) { 
                    controllers[controllerIndex].sticks.u_stick_x = SOUTH;
                } else {
                    controllers[controllerIndex].sticks.u_stick_x = EAST;
                }
            }
    
            controllers[controllerIndex].sticks.u_stick_y = sticks.u_stick_y;
            
        }
     }
    
    if (controllers[controllerIndex].sticks.u_stick_y >= MAX_DIRECTIONS) {
        controllers[controllerIndex].sticks.u_stick_y = SOUTHEAST;
    }
    
    controllers[controllerIndex].button |= (0x10000 << (controllers[controllerIndex].sticks.u_stick_x)); 
   
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
