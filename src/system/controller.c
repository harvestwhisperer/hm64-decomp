#include "common.h"

#include "controller.h"

#include "system/mathUtils.h"

#include "mainproc.h"

// forward declaration
void func_8004D47C(u8);                         
      
// bss
extern u16 D_80182FBA;
extern u16 D_801FADB0;

//Controller gControllers[NU_CONT_MAXCONTROLLERS];

// likely bss (or gameStatus.c)
extern u8 contPattern;

//INCLUDE_ASM(const s32, "system/controller", controllerInit);

void controllerInit(void) {
    
    u8 i, j;

    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        
        controllers[i].button = 0;
        controllers[i].buttonHeld = 0;
        controllers[i].buttonPressed = 0;
        controllers[i].buttonReleased = 0;
        controllers[i].unk_24 = 0;
        controllers[i].unk_28 = 0;
        
        controllers[i].sticks.s_stick_x = 0;
        controllers[i].sticks.s_stick_y = 0;
        controllers[i].sticks.u_stick_x = 0;
        controllers[i].sticks.u_stick_y = 0;

        for (j = 0; j < 24; j++) {
            controllers[i].unk_2C[j] = 0;
        }

        gControllers[i].button = 0;
        gControllers[i].buttonHeld = 0;
        gControllers[i].buttonPressed = 0;
        gControllers[i].buttonReleased = 0;
        gControllers[i].unk_24 = 0;
        gControllers[i].unk_28 = 0;
        
        gControllers[i].sticks.s_stick_x = 0;
        gControllers[i].sticks.s_stick_y = 0;
        gControllers[i].sticks.u_stick_x = 0;
        gControllers[i].sticks.u_stick_y = 0;

        for (j = 0; j < 24; j++) {
            gControllers[i].unk_2C[j] = 0;
        }
    }
    
    D_80182FBA = 4;
    D_801FADB0 = 0x10;

    contPattern = nuContInit();
}

//INCLUDE_ASM(const s32, "system/controller", func_8004CF68);

// this requires button on NUContData struct to be volatile
void func_8004CF68(void) {
    
    u8 i, j;
    
    nuContDataGetExAll(contData);
    
    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        
        if (!contStatus[i].errno) {
            
            if ((frameCount % D_802226E2) == 0) {
                
                controllers[i].sticks.s_stick_x = contData[i].stick_x;
                controllers[i].sticks.s_stick_y = contData[i].stick_y;
                
                controllers[i].button = contData[i].button;
                
                func_8004D47C(i);
                
                controllers[i].buttonPressed = (controllers[i].button ^ controllers[i].buttonHeld) & controllers[i].button;
                controllers[i].buttonReleased =  (controllers[i].button ^ controllers[i].buttonHeld) & controllers[i].buttonHeld;
                controllers[i].buttonHeld = controllers[i].button;
                
                for (j = 0; j < 24; j++) {
                    
                    if ((controllers[i].button >> j) & 1) {
                        
                        if ((controllers[i].unk_28 >> j) & 1) {
                            
                            if (controllers[i].unk_2C[j] > D_80182FBA) {
                                controllers[i].unk_24 |= ((controllers[i].button >> j) & 1) << j;
                                controllers[i].unk_2C[j] = 0;
                            } else {
                                controllers[i].unk_24 &= ~(1 << j);
                            }
                            
                        } else if (controllers[i].unk_2C[j] > D_801FADB0) {
                            controllers[i].unk_24 |= ((controllers[i].button >> j) & 1) << j;
                            controllers[i].unk_28 |= ((controllers[i].button >> j) & 1) << j;
                            controllers[i].unk_2C[j] = 0;
                        } else {
                            controllers[i].unk_24 = controllers[i].buttonPressed;
                        }
                        
                        controllers[i].unk_2C[j]++;
                        
                    } else {
                        controllers[i].unk_2C[j] = 0;
                        controllers[i].unk_24 &= ~(1 << j);
                        controllers[i].unk_28 &= ~(1 << j);
                    }
                }

            } else {

                gControllers[i].button = contData[i].button & 0xFFFF;
                gControllers[i].buttonPressed = (gControllers[i].button ^ gControllers[i].buttonHeld) & gControllers[i].button;
                
                gControllers[i].buttonReleased = (gControllers[i].button ^ gControllers[i].buttonHeld) & gControllers[i].buttonHeld;
                
                gControllers[i].buttonHeld = gControllers[i].button;
                
                gControllers[i].sticks.s_stick_x = contData[i].stick_x;
                gControllers[i].sticks.s_stick_y = contData[i].stick_y;
                
            }
        }
    }
}


//INCLUDE_ASM(const s32, "system/controller", func_8004D35C);

u32 func_8004D35C(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].button & buttonPattern;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D380);

u32 func_8004D380(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonPressed & buttonPattern;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D3A4);

u32 func_8004D3A4(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].buttonReleased & buttonPattern;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D3C8);

u32 func_8004D3C8(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].unk_24 & buttonPattern;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D3EC);

s8 func_8004D3EC(u8 contIndex) {
    return controllers[contIndex].sticks.s_stick_x;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D410);

s8 func_8004D410(u8 contIndex) {
    return controllers[contIndex].sticks.s_stick_y;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D434);

u8 func_8004D434(u8 contIndex) {
    return controllers[contIndex].sticks.u_stick_x;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D458);

u8 func_8004D458(u8 contIndex) {
    return controllers[contIndex].sticks.u_stick_y;
}

#ifdef PERMUTER
void func_8004D47C(u8 arg0) {

    volatile Sticks sticks;
    u8 temp4;

    sticks.s_stick_x = D_8013D440[arg0].sticks.s_stick_x;
    sticks.s_stick_y = D_8013D440[arg0].sticks.s_stick_y;

    sticks.u_stick_x = getAbsoluteValue((s32)sticks.s_stick_x) / 10;
    sticks.u_stick_y = getAbsoluteValue((s32)sticks.s_stick_y) / 10;
    
    D_8013D440[arg0].sticks.u_stick_x = 0;

    if (sticks.u_stick_x < 3 && sticks.u_stick_y < 3) {
        D_8013D440[arg0].sticks.u_stick_x = 0xFF;
    } else {
        
        if (sticks.u_stick_x > sticks.u_stick_y) {
                    
            D_8013D440[arg0].sticks.u_stick_y = sticks.u_stick_x - sticks.u_stick_y;
            
            if ((sticks.u_stick_x >> 1) < D_8013D440[arg0].sticks.u_stick_y) {
                if ((sticks.s_stick_x << 0x18) < 0) {
                    D_8013D440[arg0].sticks.u_stick_x = 2;
                } else {
                    D_8013D440[arg0].sticks.u_stick_x = 6;
                }
            } else if ((sticks.s_stick_y << 0x18) < 0) {
                if ((sticks.s_stick_x << 0x18) < 0) {
                    D_8013D440[arg0].sticks.u_stick_x = 1;
                } else {
                    D_8013D440[arg0].sticks.u_stick_x = 7;
                }
            } else {
                if ((sticks.s_stick_x << 0x18) < 0) {
                    D_8013D440[arg0].sticks.u_stick_x = 3;
                } else {
                    D_8013D440[arg0].sticks.u_stick_x = 5;
                }
    
            }
            
            temp4 = sticks.u_stick_x;
            
        } else {

            D_8013D440[arg0].sticks.u_stick_y = sticks.u_stick_y - sticks.u_stick_x;
            
            if ((sticks.u_stick_y >> 1) < D_8013D440[arg0].sticks.u_stick_y) {
                if ((sticks.s_stick_y << 0x18) < 0) {
                    D_8013D440[arg0].sticks.u_stick_x = 0;
                    goto label;
                } else if ((sticks.s_stick_x << 0x18) < 0) {
                    if ((sticks.s_stick_y << 0x18) < 0) {
                        D_8013D440[arg0].sticks.u_stick_x = 1;
                    } else {
                        D_8013D440[arg0].sticks.u_stick_x = 3;
                    }
                } else {
                    if ((sticks.s_stick_y << 0x18) < 0) { 
                        D_8013D440[arg0].sticks.u_stick_x = 7;
                    } else {
                        D_8013D440[arg0].sticks.u_stick_x = 5;
                    }
                }
            }
            
        }

label:
        D_8013D440[arg0].sticks.u_stick_y = sticks.u_stick_y;
    }
    
    if (D_8013D440[arg0].sticks.u_stick_y >= 7) {
        D_8013D440[arg0].sticks.u_stick_y = 6;
    }
    
    D_8013D440[arg0].button |= 1U << D_8013D440[arg0].sticks.u_stick_x; 
   
}
#else
INCLUDE_ASM(const s32, "system/controller", func_8004D47C);
#endif

//INCLUDE_ASM(const s32, "system/controller", func_8004D788);

u8 func_8004D788(u8 contIndex) {
    
    u8 temp;
    u8 result;
    u32 error;
    
    nuContPakOpen(&controllers[contIndex].pak, contIndex);
    
    result = controllers[contIndex].pak.type != 2;
    
    error = 2;
    
    if (controllers[contIndex].pak.error) {
        
        temp = controllers[contIndex].pak.error == 2;

        result = 0;
        
        if (controllers[contIndex].pak.error == 10)  {
            nuContDataRead(&controllers[contIndex].pak);
            if (controllers[contIndex].pak.error == error) {
                temp = 1;
            }
        }
        
        if (temp) {
            if (func_8004D788(contIndex)) {
                result = 1;
            }
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D87C);

s32 func_8004D87C(u8 contIndex) {
    return nuContPakGetFree(&controllers[contIndex].pak);
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D8B4);

u32 func_8004D8B4(u8 contIndex, s32 *max_files, s32 *used_files) {
    nuContPakFileNum(&controllers[contIndex].pak, max_files, used_files);
    return controllers[contIndex].pak.error == 0;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D904);

u32 func_8004D904(u8 contIndex, u8 *companyCode, u8 *gameCode) {
    nuContPakCodeSet(companyCode, gameCode);
    return !controllers[contIndex].pak.error;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D954);

u32 func_8004D954(u8 contIndex, u8* noteName, u8 *extName) {
    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);
    return !controllers[contIndex].pak.error;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D9AC);

u32 func_8004D9AC(u8 contIndex, u8 *noteName, u8 *extName, s32 offset, s32 size, s32 buf) {

    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);

    if (!controllers[contIndex].pak.error) {
        nuContPakFileReadWrite(&controllers[contIndex].pak, offset, size, buf, 0);
        return !controllers[contIndex].pak.error;
    }
    
    return 0;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004DA48);

u32 func_8004DA48(u8 contIndex, u8* noteName, u8* extName, s32 offset, s32 size, u8* buf) {

    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, 1, size);

    if (!controllers[contIndex].pak.error) {
        nuContPakFileReadWrite(&controllers[contIndex].pak, offset, size, buf, NU_CONT_PAK_MODE_CREATE);
        return !controllers[contIndex].pak.error;
    }

    return 0;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004DAF4);

u32 func_8004DAF4(u8 contIndex, u8 *noteName, u8 *extName) {
    
    nuContPakFileOpenJis(&controllers[contIndex].pak, noteName, extName, NU_CONT_PAK_MODE_NOCREATE, 0);

    if (!controllers[contIndex].pak.error) {
        nuContPakFileDeleteJis(&controllers[contIndex].pak, noteName, extName);
        return !controllers[contIndex].pak.error;
    }
    
    return 0;
}
