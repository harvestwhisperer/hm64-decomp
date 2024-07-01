#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "common.h"

#define MAX_DIALOGUE_BOXES 6
#define MAX_DIALOGUE_BANKS 73
#define MAX_GAME_VARIABLE_STRINGS 64

#define MAIN_DIALOGUE_BOX_INDEX 0

/* flags */
#define ACTIVE 1
#define INITIALIZED 2
#define HAS_DIALOGUE_WINDOW 0x200
#define HAS_OVERLAY_ICON 0x400
#define HAS_CHARACTER_AVATAR 0x10000
// 0x200000 = need to do rgba

// TODO: add japanese characters
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

// TODO: finish adding
// 0xD5-0xDF = numbers
// 0xDF-0xFF = symbols

typedef struct {
    u8 timg[6];
    u32 pad[30];
} FontData;

typedef struct {
	u8 *timg;
	u16 *pal;
	s32 width; 
	s32 height;
	u32 fmt;
	s32 pixelSize;
} FontBitmap;

typedef struct {
    u8* ptr;
    u8 length;
} GameVariableString;

// count: 0x49
// 0x80183200
typedef struct {
    u32 romIndexStart;
    u32 romIndexEnd;
    u32 *index;
    u32 romTextStart;
} DialogueInfo;

// D_801806D0
typedef struct {
	u32 romTextureStart;
	u32 romTextureEnd;
	u32 romAssetIndexStart;
	u32 romAssetIndexEnd;
	u32 romSpritesheetIndexStart;
	u32 romSpritesheetIndexEnd;
	void* vaddrTexture;
	void* vaddrSpritesheet;
	void* vaddrPalette;
	void* vaddrAnimation;
	void* vaddrSpriteToPalette;
	void* vaddrSpritesheetIndex;
	Vec3f coordinates;
	u16 spriteIndex;
    u16 spriteOffset;
    u8 flag;
} DialogueSpriteType1;

typedef struct {
    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romIndexStart;
    u32 romIndexEnd;
    void* vaddrTextureStart;
    void* vaddrTextureEnd;
    void* vaddrIndexStart;
    void* vaddrIndexEnd;
    u32 unk_20;
    Vec3f coordinates;
    u16 spriteIndex;
    u16 spriteOffset;
    u8 flag; // 0x8013D58C
} DialogueSpriteType2;

typedef struct {
    u32 romTextureStart;
	u32 romTextureEnd;
	u32 romAssetIndexStart;
	u32 romAssetIndexEnd;
	u32 romSpritesheetIndexStart;
	u32 romSpritesheetIndexEnd;
	void* vaddrTexture;
	void* vaddrSpritesheet;
	void* vaddrPalette;
	void* vaddrAnimation;
	void* vaddrSpriteToPalette;
	void* vaddrSpritesheetIndex;
	Vec3f coordinates;
	u16 spriteIndex;
} CharacterAvatar;

// D_80180728
typedef struct {
    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romIndexStart;
    u32 romIndexEnd;
    void* vaddrTextureStart;
    void* vaddrTextureEnd;
    void* vaddrIndexStart;
    void* vaddrIndexEnd;
    u32 unk_20;
    u32 unused[0x11];
    Vec3f coordinates;
    u16 spriteIndex;
    u16 spriteOffset;
    u8 flag;
    u32 padding;
} DialogueWindow;

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
} DialogueBoxVolume;

// 0x80188BC8
typedef struct {
    u8 *fontTexturePtr;
    u16 *fontPalettePtr;
    u8 unk_60;
    u8 unk_61;
} DialogueBoxFont;

// 0x80188B70
typedef struct {
    u8 *textBufferBase;
    Vec4f unk_4;
    Vec4f unk_14;
    Vec4f unk_24;
    f32 unk_34[4];
    void *unk_44;
    void *unk_48;
    Vec3f shrink;
    DialogueBoxFont fontContext; // 0x58
    u16 unk_64; // passed as Volume struct
    u16 frameCounter; // 0x66
    u16 unk_68; // counter
    DialogueBoxVolume volume; // 0x6A
    u32 unk_70;
    u32 unk_74;
    u32 unk_78;
    u16 unk_7C;
    u16 dialogueIndex;
    u16 unk_80;
    u16 unk_82;
    u16 unk_84;
    u16 unk_86;
    u16 dialogueInfoIndex;
    u8* currentCharPtr;
    u8 unk_90;
    u8 unk_91;
    u8 unk_92;
    u8 unk_93;
    u8 buttonSfxCounter; // 0x94
    u8 flag; // 0x95
    u8 margins; // 0x96
    u8 maxLinesInBox; // 0x97
    u8 dialogueWindowIndex; // 0x98
    u8 overlayIconIndex; // 0x99
    u8 characterAvatarIndex; // 0x9A
    u8 unk_9B; // 0x9B
    u8 unk_9C;
    u8 unk_9D;
    u8 gameVariableStringPosition;
    u8 gameVariableStringLength;
    u8 unk_A0;
    u8 unk_A1;
    u16 unk_A2;
    u32 flags; // 0xA4, 0x100 need to update length of game variable string 
} DialogueBox;

extern void func_8003D970();  
extern bool func_8003DBE8(u16 dialogueBoxIndex, u8* textBufferAddr);
extern bool func_8003DD14(u16);                      
extern bool initializeDialogueBox(u16 dialogueBoxIndex, u16, u16, u32 flag);     
extern bool func_8003E77C(u16, u8, u8, u8, u8);        
extern bool func_8003EA1C(u16, u8, u8, u8, u8, s32);       
extern bool func_8003EFD8(u16);           
extern bool func_8003F130(u16);  
extern bool setDialogueInfo(u16 dialogueIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 textBufferVaddr) ; 
extern bool func_8003F0DC();  
extern bool func_8003F360(u16, s16, u8);                           
extern bool func_8003F464(u16 index, u8 arg1, u8 arg2, u8* fontTexturePtr, u16* fontPalettePtr);             
extern bool func_8003F4E0(u16, u32, u32, u32);        
extern bool func_8003F54C(u16, f32, f32, f32);                        
extern bool func_8003F5D0(u16, u8, u8);                           
extern bool func_8003F630(u16, u8, u8);                           
extern bool setDialogueBoxSpriteIndices(u16, u8, u8, u8);    
extern bool func_8003F80C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);                    
extern bool func_8003F910(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 offset, u8 flag, f32 x, f32 y, f32 z);
extern bool func_8003FAF8(u16, u16);                              
extern bool func_8003FB4C(u16, u16);                      
extern bool setGameVariableString(u16, u8*, s8);  
extern u8 func_8003FBD8(u16 index, u32 arg1, u8 arg2);
extern bool func_8003FE9C(u16 index);
extern bool func_8003FFF4(u16 index);
extern void func_80042634();
extern void func_80045CB0();


extern DialogueBox dialogueBoxes[MAX_DIALOGUE_BOXES];
extern DialogueInfo dialogueInfo[MAX_DIALOGUE_BANKS];

// D_801806D0
extern CharacterAvatar characterAvatars[1];
// D_80180728
extern DialogueWindow dialogueWindows[3];
// D_8013D558
extern DialogueSpriteType2 overlayIcons[2];

extern u32 _fontTextureSegmentRomStart;
extern u32 _fontTextureSegmentRomEnd;
extern u32 _fontPalette1SegmentRomStart;
extern u32 _fontPalette1SegmentRomEnd;
extern u32 _fontPalette2SegmentRomStart;
extern u32 _fontPalette2SegmentRomEnd;

// pointer to character sprite data: D_8011BD18
extern void* D_801891C8;

#endif