#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "common.h"

#include "system/message.h"


#define MAX_DIALOGUES 1
#define TOTAL_CONVERSATION_BANKS 70

extern u32 _dialogueIconsTextureSegmentRomStart;
extern u32 _dialogueIconsTextureSegmentRomEnd;
extern u32 _dialogueIconsIndexSegmentRomStart;
extern u32 _dialogueIconsIndexSegmentRomEnd;

extern u32 _charactercharacterAvatarsTextureSegmentRomStart;
extern u32 _charactercharacterAvatarsTextureSegmentRomEnd;
extern u32 _charactercharacterAvatarsAssetsIndexSegmentRomStart;
extern u32 _charactercharacterAvatarsAssetsIndexSegmentRomEnd;
extern u32 _charactercharacterAvatarsSpritesheetIndexSegmentRomStart;
extern u32 _charactercharacterAvatarsSpritesheetIndexSegmentRomEnd;

extern u32 _dialogueWindowTextureSegmentRomStart;
extern u32 _dialogueWindowTextureSegmentRomEnd;
extern u32 _dialogueWindowIndexSegmentRomStart;
extern u32 _dialogueWindowIndexSegmentRomEnd;

// 0x80205760
typedef struct {
    u32 romStart;
    u32 romEnd;
    u32* vaddr;
    u32* romIndex;
    u32* vaddrIndex;
    u16 unk_14;
    u16 unk_16;
} DialogueAddressInfo;

// D_801C3E40
typedef struct {
	u16 unk_0;
	u16 unk_2;
	u16 unk_4;
    u16 unk_6;	
    u16 unk_8;
	u16 unk_A;
    u16 unk_C;
	s16 unk_E; // amount to increase/decrease dialogue variable
	u16 unk_10;
    u8 unk_12;
	u8 unk_13; // index into dialogueVariables
	u8 unk_14;
    u8 unk_15;
	u8 unk_16;
	u8 unk_17;
	u8 unk_18;
    u16 flags;
} UnknownDialogueStruct1;

// 0x801C3F18
typedef struct {
	u32 unk_0;
	u32 unk_4; // sfx index
	u32 unk_8;
	u16 unk_C;
	u16 dialogueIndex; // 0xF26 // index into conversation
	u16 unk_10; // 0xF28, message struct index
	u16 unk_12; // dialogue box index
	u16 unk_14;
	u8 unk_16; // pink overlay max rows
	u8 unk_17; // pink overlay current column
	u8 unk_18;
	u8 unk_19;
    u16 flags; // 0x4 = dialogue finished/closing
} UnknownDialogueStruct2;

// 0x801C3E40
typedef struct {
	UnknownDialogueStruct1 struct1;
	u32 padding[4];  // 0x801C3E5C
	// one for character avatar, button icon, etc.
	DialogueSpriteType2 struct2; // 0x801C3E6C
	DialogueSpriteType2 struct3; // 0x801C3EA4
	DialogueSpriteType2 struct4; // 0x801C3EDC
	u32 unk_D4; // 0x801C3F14
	UnknownDialogueStruct2 struct5; // 0x801C3F18
} Dialogue;

typedef struct {
	void *value;
	u32 maxValue;
	bool set;
} DialogueVariable;

extern bool setDialogueVariable(u16 index, void *address, u8 numSet, s32 max);
extern void func_80042F60();   
extern bool func_80042FEC(u16, u16, u16);
extern bool func_80043050(u16, u16, u16, void*, void*, void*, void*, void*);
extern bool setSpecialDialogueBitsPointer(u32[]);
extern bool func_80043148(u16, u32, u32, u32);
extern bool func_8004318C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);
extern bool func_80043260(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);
extern bool func_80043334(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);
extern bool func_80043430(u16, u16, u16, u16);
extern u8 func_80043A88();  
extern bool func_80043AD8(u16);
extern u8 func_80043C6C(u16);

extern Dialogue dialogues[MAX_DIALOGUES];
extern DialogueAddressInfo dialogueAddresses[69];

#endif