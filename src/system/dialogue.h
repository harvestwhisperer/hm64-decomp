#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "common.h"

#include "system/globalSprites.h"
#include "system/message.h"

#define MAX_DIALOGUES 1
#define MAX_DIALOGUE_VARIABLES 46
#define MAX_DIALOGUE_BYTECODE_BANKS 69
#define MAX_BYTECODE_ADDRESSES 70

#define DIALOGUE_SFX_VOLUME 128

#define UNSIGNED_CHAR 1
#define UNSIGNED_SHORT 2
#define UNSIGNED_INT 4

/* sessionManager flags */
#define DIALOGUE_ACTIVE 1
#define DIALOGUE_INITIALIZED 2
#define DIALOGUE_COMPLETED 4
#define DIALOGUE_WAIT_FOR_MESSAGE_BOX 0x10
#define DIALOGUE_PAUSE_FOR_USER_INPUT 0x20

/* dialogue bytecode opcodes */
#define DIALOGUE_OPCODE_SHOW_TEXT 0
#define DIALOGUE_OPCODE_DIALOGUE_VARIABLE_BRANCH 1  
#define DIALOGUE_OPCODE_UPDATE_DIALOGUE_VARIABLE 2
#define DIALOGUE_OPCODE_SET_DIALOGUE_VARIABLE 3
#define DIALOGUE_OPCODE_SPECIAL_DIALOGUE_BIT_BRANCH 4
#define DIALOGUE_OPCODE_SET_SPECIAL_DIALOGUE_BIT 5
#define DIALOGUE_OPCODE_TOGGLE_SPECIAL_DIALOGUE_BIT 6
#define DIALOGUE_OPCODE_RANDOM_BRANCH 7
#define DIALOGUE_OPCODE_BRANCH 8
#define DIALOGUE_OPCODE_UNUSED 9
#define DIALOGUE_OPCODE_SHOW_SUBMESSAGE_BOX 10
#define DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH 11
#define DIALOGUE_OPCODE_END_DIALOGUE 12


// 0x80205760
typedef struct {
    u32 romIndexStart;
    u32 romIndexEnd;
    u32* vaddrIndex;
    u32* romStart;
    u32* vaddr;
    u16 textAddressesIndex;
    u16 subdialogueTextAddressesIndex;
} DialogueBytecodeAddressInfo;

// D_801C3E40
typedef struct {
	u16 textIndex;
	u16 branchingDialogueIndex;
	u16 minimumDialogueVariableValue; 
    u16 maximumDialogueVariableValue; 
    u16 randomValue;
	u16 dialogueVariableValue;
    u16 specialDialogueBit;
	s16 updatedDialogueVariableAdjustment;
	u16 textOffset; // used by messageBoxes struct to index into dialogue lookup table
    u8 currentOpcode;
	u8 dialogueVariablesIndex;
	u8 randomMinimumValue;
    u8 randomMaximumValue;
	u8 unusedField;
	u8 unusedField2;
	u8 targetMenuRow;
} DialogueBytecodeExecutor;

// 0x801C3F18
typedef struct {
	u32 scrollSfxIndex;
	u32 closeSfxIndex;
	u32 buttonPressSfxIndex;
	u16 dialogueIndex;
	u16 dialogueBytecodeAddressesIndex;
	u16 mainMessageBoxIndex;
	u16 overlayMessageBoxIndex;
	u16 overlayTextOffset; // sets unk_7E on messageBox
	u8 totalMenuRows; // pink overlay max rows
	u8 selectedMenuRow; // pink overlay current row
	u8 visibleMenuCursorRow;
	u8 menuScrollOffset;
    u16 flags; // 0x4 = dialogue finished/closing
} DialogueSessionManager;

// 0x801C3E40
typedef struct {
	DialogueBytecodeExecutor bytecodeExecutor;
	u32 unused[4];  // 0x801C3E5C
	OverlayIcon dialogueButtonIcon1; // 0x801C3E6C
	OverlayIcon dialogueButtonIcon2; // 0x801C3EA4
	OverlayIcon dialogueButtonIcon3; // 0x801C3EDC
	void* dialogueBytecodePointer; // 0x801C3F14
	DialogueSessionManager sessionManager; // 0x801C3F18
} Dialogue;

typedef struct {
	void *value;
	u32 maxValue;
	u8 type;
} DialogueVariable;

extern bool setDialogueVariable(u16 index, void *address, u8 numSet, s32 max);
extern void initializeDialogueSessionManagers();   
extern bool initializeDialogueSessionManager(u16, u16, u16);
extern bool setDialogueBytecodeAddressInfo(u16 index, u16 arg1, u16 arg2, u32 romStart, u32 romEnd, u32* vaddrIndex, u32 romIndex, void* vaddrBytecode);
extern bool setSpecialDialogueBitsPointer(u32[]);
extern bool setDialogueSfxIndices(u16, u32, u32, u32);
extern bool setDialogueButtonIcon1(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);
extern bool setDialogueButtonIcon2(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);
extern bool setDialogueButtonIcon3(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);
extern bool initializeDialogueSession(u16 dialogueIndex, u16, u16, u16);
extern u8 checkAllDialoguesCompleted();  
extern bool closeDialogueSession(u16);
extern u8 getSelectedMenuRow(u16);

extern Dialogue dialogues[MAX_DIALOGUES];
extern DialogueBytecodeAddressInfo dialogueBytecodeAddresses[MAX_DIALOGUE_BYTECODE_BANKS];

#endif