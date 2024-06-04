#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "common.h"

extern u32 _dialogueIconsTextureSegmentRomStart;
extern u32 _dialogueIconsTextureSegmentRomEnd;
extern u32 _dialogueIconsIndexSegmentRomStart;
extern u32 _dialogueIconsIndexSegmentRomEnd;

extern u32 _characterDialogueIconsTextureSegmentRomStart;
extern u32 _characterDialogueIconsTextureSegmentRomEnd;
extern u32 _characterdialogueIconsAssetsIndexSegmentRomStart;
extern u32 _characterdialogueIconsAssetsIndexSegmentRomEnd;
extern u32 _characterDialogueIconsSpritesheetIndexSegmentRomStart;
extern u32 _characterDialogueIconsSpritesheetIndexSegmentRomEnd;

extern u32 _dialogueWindowTextureSegmentRomStart;
extern u32 _dialogueWindowTextureSegmentRomEnd;
extern u32 _dialogueWindowIndexSegmentRomStart;
extern u32 _dialogueWindowIndexSegmentRomEnd;

// 0x80205760
typedef struct {
    void* romStart;
    void* romEnd;
    u32* vaddr;
    u32* romIndex;
    u32* vaddrIndex;
    u16 unk_14;
    u16 unk_16;
} Conversation;

// D_801C3E40
typedef struct {
	u16 unk_0;
	u16 unk_2;
	u16 unk_4;
    u16 unk_6;	
    u16 unk_8;
	u16 unk_A;
    u16 unk_C;
	u16 unk_E;
	u16 unk_10;
    u8 unk_12;
	u8 unk_13;
	u8 unk_14;
    u8 unk_15;
	u8 unk_16;
	u8 unk_17;
	u8 unk_18;
    u16 flags;
} UnknownDialogueStruct1;

// D_801C3E6C
// ci4 format
typedef struct {
	void* romTextureStart;
	void* romTextureEnd;
	void* romAssetIndexStart;
	void* romAssetIndexEnd;
	void* vaddrSpritesheet; // 0x7C
	void* vaddrPalette;
    void* vaddrUnknownAssetSheet;
	void* vaddrUnknownAsset2;
	u32 unk_20; // 0x8C
	Vec3f unk_24;
	u16 unk_30; // 0x9C // spriteIndex
	u16 unk_32; // 0x98
	u8 unk_34; // 0xA0
} DialogueSpriteInfo;

// 0x801C3F18
typedef struct {
	u32 unk_0;
	u32 unk_4;
	u32 unk_8;
	u16 unk_C;
	u16 unk_E; // 0xF26 // index into conversation
	u16 unk_10; // 0xF28, message struct index
	u16 unk_12;
	u16 unk_14;
	u8 unk_16; // pink overlay max rows
	u8 unk_17; // pink overlay current column
	u8 unk_18;
    u16 flags;
} UnknownDialogueStruct3;

// 0x801C3E40
typedef struct {
	UnknownDialogueStruct1 struct1;
	u32 padding[4];  // 0x801C3E5C
	DialogueSpriteInfo struct2; // 0x801C3E6C
	DialogueSpriteInfo struct3; // 0x801C3EA4
	DialogueSpriteInfo struct4;
	u32 unk_D4; // 0x801C3F14
	UnknownDialogueStruct3 struct5; // 0x801C3F18
} Dialogue;

// 0x801806D0
typedef struct {
	void *romTextureStart;
	void *romTextureEnd;
	void *romAssetIndexStart;
	void *romAssetIndexEnd;
	void *romSpritesheetIndexStart;
	void *romSpritesheetIndexEnd;
	void *vaddrTexture;
	void *vaddrTexture2; // index + palettes
	void *vaddrPalette;
	void *vaddrUnknownAsset;
	void *vaddrUnknownAsset2;
	void *vaddrSpritesheetIndex;
	Vec3f unk_30;
	u16 unk_3C;
	u16 unk_3E;
	u32 unk_40;
	u16 unk_44;
    u16 unk_46;
	Vec4f unk_48;
} DialogueIcon;

extern bool setGameVariable(u16 index, void *address, u8 numSet, s32 max);
extern void func_80042F60();   
extern bool func_80042FEC(u16, u16, u16);
extern bool func_80043050(u16, u16, u16, void*, void*, void*, void*, void*);
extern bool func_80043138(u32[]);
extern bool func_80043148(u16, u32, u32, u32);
extern bool func_8004318C(u16 arg0, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF);
extern bool func_80043260(u16 arg0, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF);
extern bool func_80043334(u16 arg0, u16 arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* argA, u16 argB, u8 argC, f32 argD, f32 argE, f32 argF);
extern bool func_80043430(u16, u16, u16, u16);
extern u8 func_80043A88();  
extern bool func_80043AD8(u16);
extern u8 func_80043C6C(u16);

// character avatars
extern DialogueIcon dialogueIcons[1];

extern Dialogue D_801C3E40[1];
extern Conversation D_80205760[69];

#endif