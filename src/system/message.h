#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "common.h"
#include "audio.h"

#define MAX_DIALOGUE_BOXES 6

#define DIALOGUE_ICONS_TEXTURES_VADDR 0x8023B400

// dialogue rom addresses
extern void* D_E13800;
extern void* D_E13800_2;
extern void* D_E13920;
extern void* D_E13990;
extern void* D_E13990_2;
extern void* D_E13C30;
extern void* D_E13C60;
extern void* D_E13C60_2;
extern void* D_E167C0;
extern void* D_E16A90;
extern void* D_E16A90_2;
extern void* D_E19F60;
extern void* D_E19FF0;
extern void* D_E19FF0_2;
extern void* D_E1B3A0;
extern void* D_E1B4E0;
extern void* D_E1B4E0_2;
extern void* D_E1C8C0;
extern void* D_E1C9C0;
extern void* D_E1C9C0_2;
extern void* D_E21150;
extern void* D_E21700;
extern void* D_E21700_2;
extern void* D_E218A0;
extern void* D_E218D0;
extern void* D_E218D0_2;
extern void* D_E24E20;
extern void* D_E24F60;
extern void* D_E24F60_2;
extern void* D_E24F70;
extern void* D_E24F80;
extern void* D_E24F80_2;
extern void* D_E25170;
extern void* D_E251D0;
extern void* D_E251D0_2;
extern void* D_E28460;
extern void* D_E28720;
extern void* D_E28720_2;
extern void* D_E2A230;
extern void* D_E2A380;
extern void* D_E2A380_2;
extern void* D_E2D080;
extern void* D_E2D330;
extern void* D_E2D330_2;
extern void* D_E2E500;
extern void* D_E2E600;
extern void* D_E2E600_2;
extern void* D_E2F650;
extern void* D_E2F730;
extern void* D_E2F730_2;
extern void* D_E30ED0;
extern void* D_E31010;
extern void* D_E31010_2;
extern void* D_E33020;
extern void* D_E33190;
extern void* D_E33190_2;
extern void* D_E34650;
extern void* D_E34790;
extern void* D_E34790_2;
extern void* D_E36000;
extern void* D_E36150;
extern void* D_E36150_2;
extern void* D_E39360;
extern void* D_E39610;
extern void* D_E39610_2;
extern void* D_E3C700;
extern void* D_E3C9A0;
extern void* D_E3C9A0_2;
extern void* D_E3D600;
extern void* D_E3D6D0;
extern void* D_E3D6D0_2;
extern void* D_E40B90;
extern void* D_E40E50;
extern void* D_E40E50_2;
extern void* D_E42010;
extern void* D_E42100;
extern void* D_E42100_2;
extern void* D_E42D50;
extern void* D_E42E60;
extern void* D_E42E60_2;
extern void* D_E43E80;
extern void* D_E43F60;
extern void* D_E43F60_2;
extern void* D_E45130;
extern void* D_E45200;
extern void* D_E45200_2;
extern void* D_E461C0;
extern void* D_E46280;
extern void* D_E46280_2;
extern void* D_E47140;
extern void* D_E471E0;
extern void* D_E471E0_2;
extern void* D_E485D0;
extern void* D_E48720;
extern void* D_E48720_2;
extern void* D_E49480;
extern void* D_E49540;
extern void* D_E49540_2;
extern void* D_E4A230;
extern void* D_E4A2D0;
extern void* D_E4A2D0_2;
extern void* D_E4B6E0;
extern void* D_E4B7D0;
extern void* D_E4B7D0_2;
extern void* D_E4C8D0;
extern void* D_E4C9B0;
extern void* D_E4C9B0_2;
extern void* D_E4E230;
extern void* D_E4E320;
extern void* D_E4E320_2;
extern void* D_E4EC70;
extern void* D_E4ED10;
extern void* D_E4ED10_2;
extern void* D_E4F370;
extern void* D_E4F3F0;
extern void* D_E4F3F0_2;
extern void* D_E50520;
extern void* D_E50600;
extern void* D_E50600_2;
extern void* D_E51A00;
extern void* D_E51AE0;
extern void* D_E51AE0_2;
extern void* D_E52740;
extern void* D_E527E0;
extern void* D_E527E0_2;
extern void* D_E53450;
extern void* D_E53540;
extern void* D_E53540_2;
extern void* D_E54C60;
extern void* D_E54D60;
extern void* D_E54D60_2;
extern void* D_E55AE0;
extern void* D_E55BA0;
extern void* D_E55BA0_2;
extern void* D_E569C0;
extern void* D_E56A80;
extern void* D_E56A80_2;
extern void* D_E58010;
extern void* D_E58160;
extern void* D_E58160_2;
extern void* D_E583C0;
extern void* D_E583F0;
extern void* D_E583F0_2;
extern void* D_E58880;
extern void* D_E588D0;
extern void* D_E588D0_2;
extern void* D_E5A6D0;
extern void* D_E5A8C0;
extern void* D_E5A8C0_2;
extern void* D_E5D240;
extern void* D_E5D4C0;
extern void* D_E5D4C0_2;
extern void* D_E5E270;
extern void* D_E5E340;
extern void* D_E5E340_2;
extern void* D_E5F220;
extern void* D_E5F300;
extern void* D_E5F300_2;
extern void* D_E60080;
extern void* D_E60170;
extern void* D_E60170_2;
extern void* D_E64680;
extern void* D_E64AF0;
extern void* D_E64AF0_2;
extern void* D_E66260;
extern void* D_E663F0;
extern void* D_E663F0_2;
extern void* D_E677B0;
extern void* D_E678E0;
extern void* D_E678E0_2;
extern void* D_E68620;
extern void* D_E68700;
extern void* D_E68700_2;
extern void* D_E68BA0;
extern void* D_E68BF0;
extern void* D_E68BF0_2;
extern void* D_E6ADC0;
extern void* D_E6AFE0;
extern void* D_E6AFE0_2;
extern void* D_E6F1E0;
extern void* D_E6F5C0;
extern void* D_E6F5C0_2;
extern void* D_E714D0;
extern void* D_E71690;
extern void* D_E71690_2;
extern void* D_E72680;
extern void* D_E72780;
extern void* D_E72780_2;
extern void* D_E73070;
extern void* D_E73110;
extern void* D_E73110_2;
extern void* D_E7C7B0;
extern void* D_E7D1E0;
extern void* D_E7D1E0_2;
extern void* D_E81660;
extern void* D_E81F50;
extern void* D_E81F50_2;
extern void* D_E82B30;
extern void* D_E82C40;
extern void* D_E82C40_2;
extern void* D_E83880;
extern void* D_E83960;
extern void* D_E83960_2;
extern void* D_E84690;
extern void* D_E84760;
extern void* D_E84760_2;
extern void* D_E84910;
extern void* D_E84990;
extern void* D_E84990_2;
extern void* D_E84F50;
extern void* D_E84FD0;
extern void* D_E84FD0_2;
extern void* D_E85730;
extern void* D_E857A0;
extern void* D_E857A0_2;
extern void* D_E87080;
extern void* D_E871A0;

#define char_A 0xA1
#define char_C 0xA3
#define char_E 0xA5
#define char_G 0xA7
#define char_H 0xA8
#define char_J 0xAA
#define char_K 0xAB
#define char_L 0xAC
#define char_M 0xAD
#define char_P 0xB0
#define char_S 0xB3
#define char_W 0xB7
#define char_Z 0xB9
#define char_a 0xBB
#define char_b 0xBC
#define char_c 0xBD
#define char_d 0xBE
#define char_e 0xBF
#define char_f 0xC0
#define char_g 0xC1
#define char_h 0xC2
#define char_i 0xC3
#define char_j 0xC4
#define char_k 0xC5
#define char_l 0xC6
#define char_m 0xC7
#define char_n 0xC8
#define char_o 0xC9
#define char_p 0xCA
#define char_q 0xCB
#define char_r 0xCC
#define char_s 0xCD
#define char_t 0xCE
#define char_u 0xCF
#define char_v 0xD0
#define char_w 0xD1
#define char_x 0xD2
#define char_y 0xD3
#define char_z 0xD4

// 80180790
typedef struct {
	u32 unk_0; 
	u32 unk_4;
	u32 unk_8;
	u16 unk_C; 
	u16 unk_E; 
	u16 unk_10; 
} UnknownMessageStruct;

// count: 0x49
typedef struct {
    void *indexMap;
    void *indexEnd;
    void *vaddr;
    void *scriptRomAddress;
} DialogueInfo;

typedef struct {
	void *romPtr1;
	void *romPtr2;
	void *romPtr3;
	void *romPtr4;
	void *vaddr1;
	void *vaddr2;
	void *vaddr3;
	void *vaddr4;
	void *unk_20;
	Vec3f unk_24;
	u16 unk_30;
	u16 unk_32;
	u16 unk_34;
} DialogueSprite;

typedef struct {
    void *unk_0;
    Vec4f unk_4;
    Vec4f unk_14;
    Vec4f unk_24;
    u32 unk_34[4];
    void *unk_44;
    void *unk_48;
    Vec3f unk_4C;
    void *unk_58;
    /* sfx struct */
    void *unk_5C;
    u8 unk_60;
    u8 unk_61;
    u16 unk_62;
    u16 unk_64;
    u16 frameCounter; // 0x66
    u16 unk_68;
    u16 unk_6A;
    Volume volume; // 0x6C
    u16 unk_7C;
    u16 unk_7E;
    u16 unk_80;
    u16 unk_82;
    u16 unk_84;
    u16 unk_86;
    u16 unk_88;
    u32 unk_8C;
    u8 unk_90;
    u8 unk_91;
    u16 unk_92;
    u8 buttonSfxCounter; // 0x94
    u8 flag; // 0x95
    u8 margins; // 0x96
    u8 maxLinesInBox; // 0x97
    u8 currentLineFromTop; // 0x98
    u8 maxLinesInText; // 0x99
    u8 charPerLineCount; // 0x9A
    u8 currentLine; // 0x9B
    u8 unk_9C;
    u8 unk_9D;
    u8 unk_9E;
    u8 unk_9F;
    u8 unk_A0;
    u8 unk_A1;
    u16 unk_A2;
    u32 flags; // 0xA4
} DialogueBox;

extern void func_8003D970();  
extern bool func_8003DBE8(u16, s32);     
extern bool func_8003DD14(u16);                      
extern void func_8003DDF8(u32, u32, u32, u32);     
extern bool func_8003E77C(u16, u8, u8, u8, u8);                   
extern bool func_8003EA1C(u16, u8, u8, u8, u8, s32);       
extern bool func_8003F30C(u8, void*, void*, void*, void*); 
extern bool func_8003F0DC();  
extern bool func_8003F360(u16, s16, u8);                           
extern bool func_8003F464(u16, u8, u8, s32, s32);                   
extern bool func_8003F4E0(u16, u32, u32, u32);        
extern bool func_8003F54C(u16, f32, f32, f32);                        
extern bool func_8003F5D0(u16, u8, u8);                           
extern bool func_8003F630(u16, u8, u8);                           
extern bool func_8003F690(u16, u8, u8, u8);                        
extern bool func_8003FAF8(u16, u16);                              
extern bool func_8003FB4C(u16, u16);                      
extern bool func_8003FBA0(u16, u8*, s32);  
extern void func_8003FBD8(u8, u16, u8);    
extern bool func_8003F910(u8, u16, void*, void*, void*, void*, void*, void*, void*, u32, u32, u16, u16, f32, f32, f32); 
extern void func_80042634();
extern void func_80045CB0();

extern DialogueBox D_80188B70[MAX_DIALOGUE_BOXES];

extern void *dialogueIconsTextureStart;
extern void *dialogueIconsTextureEnd;
extern void *dialogueIconsPaletteStart;
extern void *dialogueIconsPaletteEnd;

#endif