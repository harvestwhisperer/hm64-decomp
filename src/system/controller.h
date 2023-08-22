#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

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

#endif