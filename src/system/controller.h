#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "common.h"

#define CONTROLLER_1 0

#define BUTTON_C_RIGHT       0x00000001
#define BUTTON_C_LEFT        0x00000002
#define BUTTON_C_DOWN        0x00000004
#define BUTTON_C_UP          0x00000008
#define BUTTON_R             0x00000010
#define BUTTON_L             0x00000020
#define BUTTON_D_RIGHT       0x00000100
#define BUTTON_D_LEFT        0x00000200
#define BUTTON_D_DOWN        0x00000400
#define BUTTON_D_UP          0x00000800
#define BUTTON_START         0x00001000
#define BUTTON_Z             0x00002000
#define BUTTON_B             0x00004000
#define BUTTON_A             0x00008000
#define BUTTON_STICK_UP      0x00010000
#define BUTTON_STICK_DOWN    0x00020000
#define BUTTON_STICK_LEFT    0x00040000
#define BUTTON_STICK_RIGHT   0x00080000

typedef struct {
    s8 s_stick_x;
    s8 s_stick_y;
    u8 u_stick_x;
    u8 u_stick_y;
} Sticks;

// 0x8013D440
typedef struct {
    NUContPakFile pak;
    Sticks sticks;
    u32 button;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u8 unk_2C[0x18];
} Controller;


extern void controllerInit(void);
extern void func_8004CF68(void);

/* get buttons */
extern u32 func_8004D35C(u8 contIndex, u32 buttonPattern);
extern u32 func_8004D380(u8 contIndex, u32 buttonPattern);
extern u32 func_8004D3A4(u8 contIndex, u32 buttonPattern);
extern u32 func_8004D3C8(u8 contIndex, u32 buttonPattern);
extern s8 func_8004D3EC(u8 contIndex);
extern s8 func_8004D410(u8 contIndex);
extern u8 func_8004D434(u8 contIndex);
extern u8 func_8004D458(u8 contIndex);

extern void func_8004D47C(u8 arg0);

/* mem pak */
extern u8 func_8004D788(u8 contIndex);
extern s32 func_8004D87C(u8 contIndex);
extern u32 func_8004D8B4(u8 contIndex, s32 *max_files, s32 *used_files);
extern u32 func_8004D904(u8 contIndex, u8 *companyCode, u8 *gameCode);
extern u32 func_8004D954(u8 contIndex, u8* noteName, u8 *extName);
extern u32 func_8004D9AC(u8 contIndex, u8 *noteName, u8 *extName, s32 offset, s32 size, s32 buf);
extern u32 func_8004DA48(u8 contIndex, u8* noteName, u8* extName, s32 offset, s32 size, u8* buf);
extern u32 func_8004DAF4(u8 contIndex, u8 *noteName, u8 *extName);


NUContData contData[NU_CONT_MAXCONTROLLERS];
// nu
OSContStatus D_80181B90[NU_CONT_MAXCONTROLLERS];

extern Controller controllers[NU_CONT_MAXCONTROLLERS];
extern Controller D_8018A738[NU_CONT_MAXCONTROLLERS];

extern u8 contPattern;

#endif