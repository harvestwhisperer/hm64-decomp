#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "common.h"

// dialogue rom addresses
extern u32 D_E13800;
extern u32 D_E13800_2;
extern u32 D_E13920;
extern u32 D_E13990;
extern u32 D_E13990_2;
extern u32 D_E13C30;
extern u32 D_E13C60;
extern u32 D_E13C60_2;
extern u32 D_E167C0;
extern u32 D_E16A90;
extern u32 D_E16A90_2;
extern u32 D_E19F60;
extern u32 D_E19FF0;
extern u32 D_E19FF0_2;
extern u32 D_E1B3A0;
extern u32 D_E1B4E0;
extern u32 D_E1B4E0_2;
extern u32 D_E1C8C0;
extern u32 D_E1C9C0;
extern u32 D_E1C9C0_2;
extern u32 D_E21150;
extern u32 D_E21700;
extern u32 D_E21700_2;
extern u32 D_E218A0;
extern u32 D_E218D0;
extern u32 D_E218D0_2;
extern u32 D_E24E20;
extern u32 D_E24F60;
extern u32 D_E24F60_2;
extern u32 D_E24F70;
extern u32 D_E24F80;
extern u32 D_E24F80_2;
extern u32 D_E25170;
extern u32 D_E251D0;
extern u32 D_E251D0_2;
extern u32 D_E28460;
extern u32 D_E28720;
extern u32 D_E28720_2;
extern u32 D_E2A230;
extern u32 D_E2A380;
extern u32 D_E2A380_2;
extern u32 D_E2D080;
extern u32 D_E2D330;
extern u32 D_E2D330_2;
extern u32 D_E2E500;
extern u32 D_E2E600;
extern u32 D_E2E600_2;
extern u32 D_E2F650;
extern u32 D_E2F730;
extern u32 D_E2F730_2;
extern u32 D_E30ED0;
extern u32 D_E31010;
extern u32 D_E31010_2;
extern u32 D_E33020;
extern u32 D_E33190;
extern u32 D_E33190_2;
extern u32 D_E34650;
extern u32 D_E34790;
extern u32 D_E34790_2;
extern u32 D_E36000;
extern u32 D_E36150;
extern u32 D_E36150_2;
extern u32 D_E39360;
extern u32 D_E39610;
extern u32 D_E39610_2;
extern u32 D_E3C700;
extern u32 D_E3C9A0;
extern u32 D_E3C9A0_2;
extern u32 D_E3D600;
extern u32 D_E3D6D0;
extern u32 D_E3D6D0_2;
extern u32 D_E40B90;
extern u32 D_E40E50;
extern u32 D_E40E50_2;
extern u32 D_E42010;
extern u32 D_E42100;
extern u32 D_E42100_2;
extern u32 D_E42D50;
extern u32 D_E42E60;
extern u32 D_E42E60_2;
extern u32 D_E43E80;
extern u32 D_E43F60;
extern u32 D_E43F60_2;
extern u32 D_E45130;
extern u32 D_E45200;
extern u32 D_E45200_2;
extern u32 D_E461C0;
extern u32 D_E46280;
extern u32 D_E46280_2;
extern u32 D_E47140;
extern u32 D_E471E0;
extern u32 D_E471E0_2;
extern u32 D_E485D0;
extern u32 D_E48720;
extern u32 D_E48720_2;
extern u32 D_E49480;
extern u32 D_E49540;
extern u32 D_E49540_2;
extern u32 D_E4A230;
extern u32 D_E4A2D0;
extern u32 D_E4A2D0_2;
extern u32 D_E4B6E0;
extern u32 D_E4B7D0;
extern u32 D_E4B7D0_2;
extern u32 D_E4C8D0;
extern u32 D_E4C9B0;
extern u32 D_E4C9B0_2;
extern u32 D_E4E230;
extern u32 D_E4E320;
extern u32 D_E4E320_2;
extern u32 D_E4EC70;
extern u32 D_E4ED10;
extern u32 D_E4ED10_2;
extern u32 D_E4F370;
extern u32 D_E4F3F0;
extern u32 D_E4F3F0_2;
extern u32 D_E50520;
extern u32 D_E50600;
extern u32 D_E50600_2;
extern u32 D_E51A00;
extern u32 D_E51AE0;
extern u32 D_E51AE0_2;
extern u32 D_E52740;
extern u32 D_E527E0;
extern u32 D_E527E0_2;
extern u32 D_E53450;
extern u32 D_E53540;
extern u32 D_E53540_2;
extern u32 D_E54C60;
extern u32 D_E54D60;
extern u32 D_E54D60_2;
extern u32 D_E55AE0;
extern u32 D_E55BA0;
extern u32 D_E55BA0_2;
extern u32 D_E569C0;
extern u32 D_E56A80;
extern u32 D_E56A80_2;
extern u32 D_E58010;
extern u32 D_E58160;
extern u32 D_E58160_2;
extern u32 D_E583C0;
extern u32 D_E583F0;
extern u32 D_E583F0_2;
extern u32 D_E58880;
extern u32 D_E588D0;
extern u32 D_E588D0_2;
extern u32 D_E5A6D0;
extern u32 D_E5A8C0;
extern u32 D_E5A8C0_2;
extern u32 D_E5D240;
extern u32 D_E5D4C0;
extern u32 D_E5D4C0_2;
extern u32 D_E5E270;
extern u32 D_E5E340;
extern u32 D_E5E340_2;
extern u32 D_E5F220;
extern u32 D_E5F300;
extern u32 D_E5F300_2;
extern u32 D_E60080;
extern u32 D_E60170;
extern u32 D_E60170_2;
extern u32 D_E64680;
extern u32 D_E64AF0;
extern u32 D_E64AF0_2;
extern u32 D_E66260;
extern u32 D_E663F0;
extern u32 D_E663F0_2;
extern u32 D_E677B0;
extern u32 D_E678E0;
extern u32 D_E678E0_2;
extern u32 D_E68620;
extern u32 D_E68700;
extern u32 D_E68700_2;
extern u32 D_E68BA0;
extern u32 D_E68BF0;
extern u32 D_E68BF0_2;
extern u32 D_E6ADC0;
extern u32 D_E6AFE0;
extern u32 D_E6AFE0_2;
extern u32 D_E6F1E0;
extern u32 D_E6F5C0;
extern u32 D_E6F5C0_2;
extern u32 D_E714D0;
extern u32 D_E71690;
extern u32 D_E71690_2;
extern u32 D_E72680;
extern u32 D_E72780;
extern u32 D_E72780_2;
extern u32 D_E73070;
extern u32 D_E73110;
extern u32 D_E73110_2;
extern u32 D_E7C7B0;
extern u32 D_E7D1E0;
extern u32 D_E7D1E0_2;
extern u32 D_E81660;
extern u32 D_E81F50;
extern u32 D_E81F50_2;
extern u32 D_E82B30;
extern u32 D_E82C40;
extern u32 D_E82C40_2;
extern u32 D_E83880;
extern u32 D_E83960;
extern u32 D_E83960_2;
extern u32 D_E84690;
extern u32 D_E84760;
extern u32 D_E84760_2;
extern u32 D_E84910;
extern u32 D_E84990;
extern u32 D_E84990_2;
extern u32 D_E84F50;
extern u32 D_E84FD0;
extern u32 D_E84FD0_2;
extern u32 D_E85730;
extern u32 D_E857A0;
extern u32 D_E857A0_2;
extern u32 D_E87080;

extern u32 dialogueIconsTexture_ROM_START;
extern u32 dialogueIconsTexture_ROM_END;
extern u32 dialogueIconsIndex_ROM_START;
extern u32 dialogueIconsIndex_ROM_END;

extern u32 characterDialogueIconsTexture_ROM_START;
extern u32 characterDialogueIconsTexture_ROM_END;
extern u32 characterdialogueIconsAssetsIndex_ROM_START;
extern u32 characterdialogueIconsAssetsIndex_ROM_END;
extern u32 characterDialogueIconsSpritesheetIndex_ROM_START;
extern u32 characterDialogueIconsSpritesheetIndex_ROM_END;

extern u32 dialogueWindowTexture_ROM_START;
extern u32 dialogueWindowTexture_ROM_END;
extern u32 dialogueWindowIndex_ROM_START;
extern u32 dialogueWindowIndex_ROM_END;

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