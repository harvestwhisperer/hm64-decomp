#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "common.h"
#include "audio.h"

#define MAX_DIALOGUE_BOXES 6

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
} Dialogue;

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
extern s32 func_8003F0DC();  
extern bool func_8003F360(u16, s16, u8);                           
extern bool func_8003F464(u16, u8, u8, s32, s32);                   
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

#endif