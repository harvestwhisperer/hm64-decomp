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
#define DIALOGUE_OPCODE_CLEAR_SPECIAL_DIALOGUE_BIT 6
#define DIALOGUE_OPCODE_RANDOM_BRANCH 7
#define DIALOGUE_OPCODE_BRANCH 8
#define DIALOGUE_OPCODE_UNUSED 9
#define DIALOGUE_OPCODE_SHOW_SELECTION_MENU 10
#define DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH 11
#define DIALOGUE_OPCODE_END_DIALOGUE 12

/* dialogue variables */
#define VAR_ALCOHOL_TOLERANCE 0
#define VAR_SEASON            1
#define VAR_HOUR              2
#define VAR_WEATHER           3
#define VAR_DAY_OF_WEEK       4
#define VAR_YEAR              5
#define VAR_ITEM_BEING_HELD   6
#define VAR_AFFECTION_MARIA              7
#define VAR_AFFECTION_POPURI             8
#define VAR_AFFECTION_ELLI               9
#define VAR_AFFECTION_ANN                10
#define VAR_AFFECTION_KAREN              11
#define VAR_AFFECTION_HARRIS             12
#define VAR_AFFECTION_GRAY               13
#define VAR_AFFECTION_JEFF               14
#define VAR_AFFECTION_CLIFF              15
#define VAR_AFFECTION_KAI                16
#define VAR_AFFECTION_MAYOR              17
#define VAR_AFFECTION_MAYOR_WIFE         18
#define VAR_AFFECTION_LILLIA             19
#define VAR_AFFECTION_BASIL              20
#define VAR_AFFECTION_ELLEN              21
#define VAR_AFFECTION_PASTOR             22
#define VAR_AFFECTION_RICK               23
#define VAR_AFFECTION_SAIBARA            24
#define VAR_AFFECTION_POTION_SHOP_DEALER 25
#define VAR_AFFECTION_KENT               26
#define VAR_AFFECTION_STU                27
#define VAR_AFFECTION_MIDWIFE            28
#define VAR_AFFECTION_MAY                29
#define VAR_AFFECTION_CARPENTER_1        30
#define VAR_AFFECTION_CARPENTER_2        31
#define VAR_AFFECTION_MASTER_CARPENTER   32
#define VAR_AFFECTION_HARVEST_SPRITE_1   33
#define VAR_AFFECTION_SYDNEY             34
#define VAR_AFFECTION_BARLEY             35
#define VAR_AFFECTION_GREG               36
#define VAR_AFFECTION_BABY               37
#define VAR_TOTAL_BACHELORETTE_AFFECTION 38
#define VAR_DAY_OF_MONTH                 39
#define VAR_GOLD                         40
#define VAR_TOTAL_GRASS_TILES            41
#define VAR_TOTAL_PINK_CAT_MINT_FLOWERS  42
#define VAR_CROPS_SHIPPED_1 43
#define VAR_CROPS_SHIPPED_2 44
#define VAR_CROPS_SHIPPED_3 45
#define VAR_CROPS_SHIPPED_4 46
#define VAR_CROPS_SHIPPED_5 47
#define VAR_CROPS_SHIPPED_6 48
#define VAR_AFFECTION_DOUG    49
#define VAR_AFFECTION_GOTZ    50
#define VAR_AFFECTION_SASHA   51
#define VAR_AFFECTION_SHIPPER 52
#define VAR_AFFECTION_DUKE    53
#define VAR_AFFECTION_HORSE             54
#define VAR_AFFECTION_FIRST_FARM_ANIMAL 55
#define VAR_AFFECTION_DOG               56
#define VAR_ELLI_GRIEVING_COUNTER 57
#define VAR_BABY_AGE                   58
#define VAR_WIFE_CONCEPTION_COUNTER    59
#define VAR_WIFE_CONCEPTION_COUNTER_2  60
#define VAR_BABY_AGE_2                 61
#define VAR_UNUSED_3E                  62
#define VAR_BABY_AGE_3                 63
#define VAR_UNUSED_40                  64
#define VAR_BABY_AGE_4                 65
#define VAR_BABY_AGE_5                 66
#define VAR_UNUSED_43                  67
#define VAR_UNUSED_44                  68
#define VAR_WIFE_PREGNANCY_COUNTER     69
#define VAR_WIFE_PREGNANCY_COUNTER_2   70
#define VAR_UNUSED_47                  71
#define VAR_UNUSED_48                  72
#define VAR_WIFE_PREGNANCY_COUNTER_3   73
#define VAR_WIFE_PREGNANCY_COUNTER_4   74
#define VAR_WIFE_PREGNANCY_COUNTER_5   75
/* end dialogue variables */


// 0x80205760
typedef struct {
    u32 romIndexStart;
    u32 romIndexEnd;
    u32* vaddrIndex;
    u32* romStart;
    u32* vaddr;
    u16 textAddressesIndex;
    u16 selectionMenuTextAddressesIndex;
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
	u8 selectionMenuRowsCount;
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