#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "common.h"
#include "audio.h"

#define MAX_DIALOGUE_BOXES 6
#define MAX_DIALOGUE_BANKS 73

#define DIALOGUE_ICONS_TEXTURES_VADDR 0x8023B400

// 1 - 0xA0 = Japanese characters

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

// 0xD5-0xDF = numbers
// 0xDF-0xFF = symbols

// 80180790
typedef struct {
	u32 unk_0; 
	u32 unk_4;
	u32 unk_8;
	u16 unk_C; 
	u16 unk_E; 
	u16 unk_10; 
} UnknownMessageStruct;

typedef struct {
    u8* unk_0;
    u8 unk_4;
} GameVariableReference;

// count: 0x49
// 0x80183200
typedef struct {
    void *romIndexStart;
    void *romIndexEnd;
    void *textBufferVaddr;
    void *romTextStart;
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

// 0x80188B70
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
    void *unk_5C; // palette ptr? func_80042014
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
extern bool func_8003EFD8(u16);           
extern bool func_8003F130(u16);  
extern bool func_8003F30C(u16 dialogueIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 textBufferVaddr) ; 
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
extern bool func_8003FBA0(u16, u8*, s8);  
extern void func_8003FBD8(u8, u16, u8);    
extern bool func_8003F910(u8, u16, void*, void*, void*, void*, void*, void*, void*, u32, u32, u16, u16, f32, f32, f32); 
extern void func_80042634();
extern void func_80045CB0();

extern DialogueBox dialogueBoxes[MAX_DIALOGUE_BOXES];
extern DialogueInfo D_80183200[MAX_DIALOGUE_BANKS];

extern u32 fontTexture_ROM_START;
extern u32 fontTexture_ROM_END;
extern u32 fontPalette1_ROM_START;
extern u32 fontPalette1_ROM_END;
extern u32 fontPalette2_ROM_START;
extern u32 fontPalette2_ROM_END;

#endif