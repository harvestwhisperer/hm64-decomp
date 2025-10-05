#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "common.h"

#define MAX_DIALOGUE_BOXES 6
#define MAX_TEXT_BANKS 73
#define MAX_GAME_VARIABLE_STRINGS 64

#define MAIN_DIALOGUE_BOX_INDEX 0

/* flags */
#define DIALOGUE_BOX_ACTIVE 1
#define DIALOGUE_BOX_INITIALIZED 2
#define DIALOGUE_BOX_HAS_DIALOGUE_WINDOW 0x200
#define DIALOGUE_BOX_HAS_OVERLAY_ICON 0x400
#define DIALOGUE_BOX_HAS_CHARACTER_AVATAR 0x10000
// 0x100 need to update length of game variable string 
// 0x200000 = need to do rgba

#define INLINE_TEXT 0
#define STANDALONE 1

#define COMPRESSED_FONT_VADDR 0x802FF000
#define FONT_PALETTE_1_VADDR 0x8030A000
#define FONT_PALETTE_2_VADDR 0x8030A400

#define CHARACTER_CONTROL_LINEBREAK 0
#define CHARACTER_CONTROL_SOFTBREAK 1
#define CHARACTER_CONTROL_TEXT_END 2
#define CHARACTER_CONTROL_WAIT 3
#define CHARACTER_CONTROL_WAIT_WITH_ICON 4
#define CHARACTER_CONTROL_LOAD_TEXT 5
#define CHARACTER_CONTROL_UNUSED 6
#define CHARACTER_CONTROL_INSERT_GAME_VARIABLE 7
#define CHARACTER_CONTROL_WAIT_ALTERNATE 8
#define CHARACTER_CONTROL_CHARACTER_AVATAR 9

// TODO: add japanese characters
// 1 - 0xA0 = Japanese characters
#define char_A 0xA1
#define char_B 0xA2
#define char_C 0xA3
#define char_D 0xA4
#define char_E 0xA5
#define char_F 0xA6
#define char_G 0xA7
#define char_H 0xA8
#define char_I 0xA9
#define char_J 0xAA
#define char_K 0xAB
#define char_L 0xAC
#define char_M 0xAD
#define char_N 0xAE
#define char_O 0xAF
#define char_P 0xB0
#define char_Q 0xB1
#define char_R 0xB2
#define char_S 0xB3
#define char_T 0xB4
#define char_U 0xB5
#define char_V 0xB6
#define char_W 0xB7
#define char_X 0xB8
#define char_Y 0xB9
#define char_Z 0xBA
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
#define char_1 0xD5
#define char_2 0xD6
#define char_3 0xD7
#define char_4 0xD8
#define char_5 0xD9
#define char_6 0xDA
#define char_7 0xDB
#define char_8 0xDC
#define char_9 0xDD
#define char_0 0xDE
#define char_QUESTION_MARK 0xDF
#define char_EXCLAMATION_MARK 0xE0
#define char_DASH 0xE1
#define char_TILDE 0xE2
#define char_PERIOD 0xE3
#define char_COMMA 0xE4
#define char_DOT 0xE5
#define char_SLASH 0xE6
#define char_STAR_1 0xE7
#define char_STAR_2 0xE8
#define char_AMPERSAND 0xE9
#define char_HEART_1 0xEA
#define char_HEART_2 0xEB
#define char_DROP 0xEC
#define char_PAW_PRINT 0xED
#define char_SPACE 0xEE
#define char_TERMINATOR 0xFF

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
    u8 maxLength;
} GameVariableString;

// count: 0x49
// 0x80183200
typedef struct {
    u32 romIndexStart;
    u32 romIndexEnd;
    u32 *indexVaddr;
    u32 romTextStart;
} TextAddresses;

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
} OverlayIcon;

typedef struct {
    u32 romTextureStart;
	u32 romTextureEnd;
	u32 romAssetsIndexStart;
	u32 romAssetsIndexEnd;
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
} MessageBoxInterpolator;

// 0x80188BC8
typedef struct {
    u8 *compressedCI2FontData;
    u16 *fontPalettePtr;
    u8 characterCellWidth;
    u8 characterCellHeight;
} MessageBoxFont;

// 0x80188B70
typedef struct {
    u8 *textBufferBase;
    Vec4f unk_4;
    Vec4f unk_14;
    Vec4f unk_24;
    Vec4f unk_34;
    u8 *savedCharPtr; // backup/store char
    u8 *gameVariableStringPtr;
    Vec3f viewSpacePosition;
    MessageBoxFont fontContext; // 0x58
    u16 unk_64; // interpolator
    u16 frameCounter; // 0x66
    u16 unk_68; // counter
    MessageBoxInterpolator scrollInterpolator; // 0x6A
    u32 characterPrintSfx;
    u32 unk_74;
    u32 messageCloseSfx;
    s16 unk_7C; // interpolation value
    u16 textIndex;
    u16 totalCharactersProcessed;
    u16 charactersProcessedPerLine; // might be characters processed per message
    s16 defaultScrollSpeed;
    u16 buttonMask;
    u16 textAddressesIndex;
    u8* currentCharPtr; // as font indices
    u8 currentCompressionControlByte;
    u8 compressionBitIndex;
    u8 textBoxLineCharWidth; // seemingly in character units
    u8 textBoxVisibleRows; // number of lines before scrolling
    u8 scrollCount; // 0x94
    u8 totalLinesToPrint; // 0 = first line
    u8 currentCharCountOnLine; // 0x96
    u8 currentLineBeingPrinted; // 0x97
    u8 dialogueWindowIndex; // 0x98
    u8 overlayIconIndex; // 0x99
    u8 characterAvatarIndex; // 0x9A
    u8 characterSpacing; // 0x9B
    u8 lineSpacing; // 0x9C
    u8 unk_9D;
    u8 gameVariableStringIndex; // index
    u8 gameVariableStringLength;
    u8 unk_A0;
    u8 unk_A1;
    u16 unk_A2;
    u32 flags; // 0xA4
} MessageBox;

extern void initializeMessageBoxes();  
extern bool initializeEmptyMessageBox(u16 messageBoxIndex, u8* textBufferAddr);
extern bool func_8003DD14(u16);                      
extern bool initializeMessageBox(u16 messageBoxIndex, u16, u16, u32 flag);     
extern bool func_8003E77C(u16, u8, u8, u8, u8);        
extern bool func_8003EA1C(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate);     
extern bool func_8003EFD8(u16);           
extern bool func_8003F130(u16);  
extern bool setTextAddresses(u16 dialogueIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 textBufferVaddr) ; 
extern bool func_8003F0DC();  
extern bool func_8003F360(u16, s16, u8);                           
extern bool func_8003F464(u16 index, u8 arg1, u8 arg2, u8* compressedCI2FontData, u16* fontPalettePtr);             
extern bool setMessageBoxSfx(u16, u32, u32, u32);        
extern bool setMessageBoxViewSpacePosition(u16, f32, f32, f32);                        
extern bool func_8003F5D0(u16, u8, u8);                           
extern bool func_8003F630(u16, u8, u8);                           
extern bool setMessageBoxSpriteIndices(u16, u8, u8, u8);    
extern bool func_8003F80C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z);                    
extern bool func_8003F910(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 offset, u8 flag, f32 x, f32 y, f32 z);
extern bool func_8003FA1C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romAssetsIndexStart, u32 romAssetsIndexEnd, u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, void* vaddrTexture, void* vaddrTexture2, void* vaddrPalette, void* vaddrAnimation, void* vaddrSpriteToPalette, void* vaddrSpritesheetIndex, f32 x, f32 y, f32 z);          
extern void func_8003FAE8(u8* arg0);
extern bool setMessageBoxButtonMask(u16, u16);                    
extern bool func_8003FB4C(u16, s16);                      
extern bool setGameVariableString(u16, u8*, s8);  
extern u8 convertNumberToString(u16 index, u32 arg1, u8 arg2);
extern bool func_8003FE9C(u16 index);
extern bool func_8003FFF4(u16 index);
extern void updateMessageBox();
extern void updateDialogues();


extern MessageBox messageBoxes[MAX_DIALOGUE_BOXES];
extern TextAddresses textAddresses[MAX_TEXT_BANKS];

extern CharacterAvatar characterAvatars[1];
extern DialogueWindow dialogueWindows[3];
extern OverlayIcon overlayIcons[2];

#endif