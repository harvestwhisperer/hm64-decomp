#include "common.h"

#include "controller.h"

#include "system/mathUtils.h"

#include "mainproc.h"

// forward declaration
void func_8004D47C(u8);                         
      
// bss
extern u16 D_80182FBA;
extern u16 D_801FADB0;

// likely bss (or gameStatus.c)
extern u8 contPattern;

//INCLUDE_ASM(const s32, "system/controller", controllerInit);

void controllerInit(void) {
    
    u8 i, j;

    for (i = 0; i < NU_CONT_MAXCONTROLLERS; i++) {
        
        controllers[i].button = 0;
        controllers[i].unk_18 = 0;
        controllers[i].unk_1C = 0;
        controllers[i].unk_20 = 0;
        controllers[i].unk_24 = 0;
        controllers[i].unk_28 = 0;
        
        controllers[i].sticks.s_stick_x = 0;
        controllers[i].sticks.s_stick_y = 0;
        controllers[i].sticks.u_stick_x = 0;
        controllers[i].sticks.u_stick_y = 0;

        for (j = 0; j < 24; j++) {
            controllers[i].unk_2C[j] = 0;
        }

        D_8018A738[i].button = 0;
        D_8018A738[i].unk_18 = 0;
        D_8018A738[i].unk_1C = 0;
        D_8018A738[i].unk_20 = 0;
        D_8018A738[i].unk_24 = 0;
        D_8018A738[i].unk_28 = 0;
        
        D_8018A738[i].sticks.s_stick_x = 0;
        D_8018A738[i].sticks.s_stick_y = 0;
        D_8018A738[i].sticks.u_stick_x = 0;
        D_8018A738[i].sticks.u_stick_y = 0;

        for (j = 0; j < 24; j++) {
            D_8018A738[i].unk_2C[j] = 0;
        }
    }
    
    D_80182FBA = 4;
    D_801FADB0 = 0x10;

    contPattern = nuContInit();
}


#ifdef PERMUTER
void func_8004CF68(void) {

    u8 i, j;
    
    nuContDataGetExAll(contData);

    for (i = 0; i < 4; i++) {

        if (!D_80181B90[i].errno) {
        
            if ((frameCount % D_802226E2) == 0) {            
                
                D_8013D450[i].stick_x = contData[i].stick_x;
                D_8013D450[i].stick_y = contData[i].stick_y;
                D_8013D450[i].button = contData[i].button;
                
                func_8004D47C(i);

                D_8013D450[i].unk_C = (D_8013D450[i].button ^ D_8013D450[i].unk_8) & D_8013D450[i].button;
                D_8013D450[i].unk_10 = (D_8013D450[i].button ^ D_8013D450[i].unk_8) & D_8013D450[i].unk_8;
                D_8013D450[i].unk_8 = D_8013D450[i].button;

                j = 0;

                while (1) {
                    
                    if (((D_8013D450[i].button >> j) & 1)) {
                        if ((D_8013D450[i].unk_18 >> j) & 1) {
                            if (D_80182FBA < D_8013D450[i].unk_1C[j]) {
                                D_8013D450[i].unk_14 = D_8013D450[i].unk_14 | (D_8013D450[i].unk_18 >> j) << j;
                                D_8013D450[i].unk_1C[j] = 0;
                                goto inner_loop_end;
                            }
                            
                            D_8013D450[i].unk_14 &= ~(1 << (j & 0x1F));
                            
                        } else {
                            if (D_801FADB0 < D_8013D450[i].unk_1C[j]) {
                                D_8013D450[i].unk_14 = (D_8013D450[i].unk_14  | ((D_8013D450[i].button >> j) & 1) << j);
                                D_8013D450[i].unk_18 = D_8013D450[i].unk_18 | ((D_8013D450[i].button >> j) & 1) << j;
                                D_8013D450[i].unk_1C[j] = 0;
                            } else {
                                D_8013D450[i].unk_14 = D_8013D450[i].unk_C; 
                            }
                        }

                        D_8013D450[i].unk_1C[j]++;
                        
                    } else {
                        D_8013D450[i].unk_1C[j] = 0;
                        D_8013D450[i].unk_14 &= ~(1 << (j & 0x1F));
                        D_8013D450[i].unk_18 &= ~(1 << (j & 0x1F));
                    }
inner_loop_end:
                    if (j > 24) break;
                }
                
            } else {
                D_8018A748[i].button = contData[i].button;
                D_8018A748[i].unk_8 = (contData[i].button ^ D_8018A748[i].unk_C) & contData[i].button;
                D_8018A748[i].unk_10 = (D_8018A748[i].button ^ D_8018A748[i].unk_8) & D_8018A748[i].unk_8; 
                D_8018A748[i].unk_8 = D_8018A748[i].button;
                D_8018A748[i].stick_x = contData[i].stick_x;
                D_8018A748[i].stick_y = contData[i].stick_y;
            }
            
        }
    }
}
#else
INCLUDE_ASM(const s32, "system/controller", func_8004CF68);
#endif

//INCLUDE_ASM(const s32, "system/controller", func_8004D35C);

u32 func_8004D35C(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].button & buttonPattern;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D380);

u32 func_8004D380(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].unk_1C & buttonPattern;
}

//INCLUDE_ASM(const s32, "system/controller", func_8004D3A4);

u32 func_8004D3A4(u8 contIndex, u32 buttonPattern) {
    return controllers[contIndex].unk_20 & buttonPattern;
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
