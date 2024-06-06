#include "common.h"

#include "title.h"

#include "system/controller.h"  
#include "system/dialogue.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/sprite.h"
                                                        
#include "gameAudio.h"
#include "initialize2.h"     
#include "loadGameScreen.h"      
#include "mainLoop.h" 
#include "setCutscenes.h"                     
     
// forward declarations
void func_800E0ED4();                                  
void func_800E0FEC();                                  
void func_800E10E8();                                  

// bss
TitleContext titleScreenContext;

extern u32 _dogTitleTextureSegmentRomStart;
extern u32 _dogTitleTextureSegmentRomEnd;
extern u32 _dogTitleAssetsIndexSegmentRomStart;
extern u32 _dogTitleAssetsIndexSegmentRomEnd;
extern u32 _dogTitleSpritesheetIndexSegmentRomStart;
extern u32 _dogTitleSpritesheetIndexSegmentRomEnd;

extern u32 _titleSpritesTextureSegmentRomStart;
extern u32 _titleSpritesTextureSegmentRomEnd;
extern u32 _titleSpritesIndexSegmentRomStart;
extern u32 _titleSpritesIndexSegmentRomEnd;

//INCLUDE_ASM(const s32, "title", initializeTitleScreen);

void initializeTitleScreen(u8 arg0) {
    
    titleScreenContext.rowHighlighted = 0;
    titleScreenContext.dogAnimationIndex = 0;
    titleScreenContext.unk_4 = 0;
    titleScreenContext.dogIdleCounter = 0;
    titleScreenContext.centerCoordinate = -160.0f;
    
    func_8002B138(0x42, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x42, 0.0f, 64.0f, 64.0f);
    func_8002BD90(0x42, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x42, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x42, 3);
    func_8002C914(0x42, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x42, 1);
    func_8002B80C(0x42, 0, 0xFE);
    
    func_8002B138(0x43, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x43, 0.0f, -96.0f, 64.0f);
    func_8002BD90(0x43, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x43, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x43, 3);
    func_8002C914(0x43, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(0x43, 1, 0xFE);
    
    func_8002B138(0x46, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x46, titleScreenContext.centerCoordinate + 160.0f, -16.0f, 48.0f);
    func_8002BD90(0x46, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x46, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x46, 3);
    func_8002C914(0x46, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x46, 1);
    func_8002B80C(0x46, 3, 0xFE);
    
    func_8002B138(0x47, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x47, titleScreenContext.centerCoordinate - 160.0f, -12.0f, 48.0f);
    func_8002BD90(0x47, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x47, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x47, 3);
    func_8002C914(0x47, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x47, 1);
    func_8002B80C(0x47, 5, 0xFE);
    
    func_8002B138(0x48, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x48, titleScreenContext.centerCoordinate - 160.0f, -44.0f, 48.0f);
    func_8002BD90(0x48, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x48, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x48, 3);
    func_8002C914(0x48, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x48, 1);
    func_8002B80C(0x48, 7, 0xFE);
    
    func_8002B138(0x49, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x49, titleScreenContext.centerCoordinate + 160.0f, -32.0f, 40.0f);
    func_8002BD90(0x49, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x49, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x49, 3);
    func_8002C914(0x49, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x49, 1);
    func_8002B80C(0x49, 2, 0xFE);
    
    func_8002B138(0x4A, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x4A, titleScreenContext.centerCoordinate - 192.0f, -32.0f, 40.0f);
    func_8002BD90(0x4A, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x4A, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x4A, 3);
    func_8002C914(0x4A, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x4A, 1);
    func_8002B80C(0x4A, 2, 0xFE);
    
    func_8002B138(0x4B, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x4B, titleScreenContext.centerCoordinate - 128.0f, -32.0f, 40.0f);
    func_8002BD90(0x4B, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x4B, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x4B, 3);
    func_8002C914(0x4B, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x4B, 1);
    func_8002B80C(0x4B, 2, 0xFE);
    
    func_8002B138(0x54, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x54, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 56.0f);
    func_8002BD90(0x54, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x54, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x54, 3);
    func_8002C914(0x54, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x54, 1);
    func_8002B80C(0x54, 9, 0xFE);
    
    func_8002B138(0x55, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x55, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 56.0f);
    func_8002BD90(0x55, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x55, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x55, 3);
    func_8002C914(0x55, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x55, 1);
    func_8002B80C(0x55, 9, 0xFE);
    
    func_8002B138(0x56, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x56, titleScreenContext.centerCoordinate + 160.0f, 64.0f, 64.0f);
    func_8002BD90(0x56, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x56, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x56, 3);
    func_8002C914(0x56, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x56, 1);
    func_8002B80C(0x56, 0x10, 0xFE);
    
    func_8002B138(0x57, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x57, titleScreenContext.centerCoordinate + 32.0f, 96.0f, 64.0f);
    func_8002BD90(0x57, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x57, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x57, 3);
    func_8002C914(0x57, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x57, 1);
    func_8002B80C(0x57, 0x11, 0xFE);
    
    func_8002B138(0x58, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x58, titleScreenContext.centerCoordinate + 224.0f, 80.0f, 64.0f);
    func_8002BD90(0x58, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x58, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x58, 3);
    func_8002C914(0x58, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x58, 1);
    func_8002B80C(0x58, 0x12, 0xFE);
    
    func_8002B138(0x59, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x59, titleScreenContext.centerCoordinate - 64.0f, 72.0f, 64.0f);
    func_8002BD90(0x59, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x59, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x59, 3);
    func_8002C914(0x59, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x59, 1);
    func_8002B80C(0x59, 0x12, 0xFE);
    
    func_8002B138(0x5A, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x5A, titleScreenContext.centerCoordinate - 192.0f, 108.0f, 64.0f);
    func_8002BD90(0x5A, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x5A, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x5A, 3);
    func_8002C914(0x5A, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x5A, 1);
    func_8002B80C(0x5A, 0x11, 0xFE);
    
    func_8002B138(0x5B, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x5B, titleScreenContext.centerCoordinate - 288.0f, 88.0f, 64.0f);
    func_8002BD90(0x5B, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x5B, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x5B, 3);
    func_8002C914(0x5B, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x5B, 1);
    func_8002B80C(0x5B, 0x10, 0xFE);
    
    func_8002B138(0x52, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x52, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 8.0f);
    func_8002BD90(0x52, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x52, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x52, 3);
    func_8002C914(0x52, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x52, 1);
    func_8002B80C(0x52, 0xA, 0xFE);
    
    func_8002B138(0x53, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x53, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 8.0f);
    func_8002BD90(0x53, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x53, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x53, 3);
    func_8002C914(0x53, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x53, 1);
    func_8002B80C(0x53, 0xA, 0xFE);
    
    func_8002B138(0x4E, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x4E, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 16.0f);
    func_8002BD90(0x4E, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x4E, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x4E, 3);
    func_8002C914(0x4E, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x4E, 1);
    func_8002B80C(0x4E, 0xB, 0xFE);
    
    func_8002B138(0x4D, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x4D, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 24.0f);
    func_8002BD90(0x4D, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x4D, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x4D, 3);
    func_8002C914(0x4D, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x4D, 1);
    func_8002B80C(0x4D, 0xC, 0xFE);
    
    func_8002B138(0x4C, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x4C, titleScreenContext.centerCoordinate + 160.0f, 0.0f, 32.0f);
    func_8002BD90(0x4C, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x4C, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x4C, 3);
    func_8002C914(0x4C, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x4C, 1);
    func_8002B80C(0x4C, 0xE, 0xFE);
    
    func_8002B138(0x51, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x51, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 16.0f);
    func_8002BD90(0x51, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x51, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x51, 3);
    func_8002C914(0x51, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x51, 1);
    func_8002B80C(0x51, 0xB, 0xFE);
    
    func_8002B138(0x50, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x50, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 24.0f);
    func_8002BD90(0x50, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x50, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x50, 3);
    func_8002C914(0x50, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x50, 1);
    func_8002B80C(0x50, 0xC, 0xFE);
    
    func_8002B138(0x4F, &_titleSpritesTextureSegmentRomStart, &_titleSpritesTextureSegmentRomEnd, &_titleSpritesIndexSegmentRomStart, &_titleSpritesIndexSegmentRomEnd, NULL, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_START, NULL, (void*)TITLE_SCREEN_TEXTURES_VADDR_END, (void*)TITLE_SCREEN_PALETTE_VADDR_START, (void*)TITLE_SCREEN_PALETTE_VADDR_END, NULL, NULL, 0);
    func_8002BD0C(0x4F, titleScreenContext.centerCoordinate - 160.0f, 0.0f, 32.0f);
    func_8002BD90(0x4F, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x4F, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x4F, 3);
    func_8002C914(0x4F, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x4F, 1);
    func_8002B80C(0x4F, 0xE, 0xFE);
    
    func_8002B138(0x45, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsIndexSegmentRomStart, &_dialogueIconsIndexSegmentRomEnd, NULL, NULL, (void*)0x8023B400, NULL, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, NULL, NULL, 0);
    func_8002BD0C(0x45, -54.0f, -24.0f, 56.0f);
    func_8002BD90(0x45, 1.0f, 1.0f, 1.0f);
    func_8002BE14(0x45, 0.0f, 0.0f, 0.0f);
    func_8002C7EC(0x45, 3);
    func_8002C914(0x45, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002CB24(0x45, 1);
    func_8002B80C(0x45, 3, 0xFE);
    
    setMainLoopCallbackFunctionIndex(TITLE_SCREEN);
    
    if (arg0 != 0) {

        titleScreenContext.mode = 2;

        func_8002BE98(0x46, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x47, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x48, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x49, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x4A, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x4B, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x52, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x53, 320.0f, 0.0f, 0.0f);
        func_8002BE98(0x4E, 128.0f, 0.0f, 0.0f);
        func_8002BE98(0x4D, 192.0f, 0.0f, 0.0f);
        func_8002BE98(0x4C, 256.0f, 0.0f, 0.0f);
        func_8002BE98(0x51, 128.0f, 0.0f, 0.0f);
        func_8002BE98(0x50, 192.0f, 0.0f, 0.0f);
        func_8002BE98(0x4F, 256.0f, 0.0f, 0.0f);
        func_8002BE98(0x54, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x55, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x56, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x57, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x58, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x59, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x5A, 64.0f, 0.0f, 0.0f);
        func_8002BE98(0x5B, 64.0f, 0.0f, 0.0f);

    } else {

        titleScreenContext.mode = 0;
    }
    
    titleScreenContext.frameCounter = 0;
    
    loadDogTitleSprite();
    
    // set audio
    func_800ACB04(TITLE_THEME);
    setSongWithVolume(TITLE_THEME, SONG_VOLUME);

}

//INCLUDE_ASM(const s32, "title", loadDogTitleSprite);

void loadDogTitleSprite(void) {
    func_8002B138(DOG_TITLE_SPRITE, &_dogTitleTextureSegmentRomStart, &_dogTitleTextureSegmentRomEnd, &_dogTitleAssetsIndexSegmentRomStart, &_dogTitleAssetsIndexSegmentRomEnd, &_dogTitleSpritesheetIndexSegmentRomStart, &_dogTitleSpritesheetIndexSegmentRomEnd, (void*)0x80240B00, (void*)0x80243B00, (void*)0x80246B00, (void*)0x8024AB00, (void*)0x80252300, (void*)0x80252B00, 1, 0);
    func_8002BD0C(DOG_TITLE_SPRITE, 64.0f, -80.0f, 56.0f);
    func_8002BD90(DOG_TITLE_SPRITE, 1.0f, 1.0f, 1.0f);
    func_8002BE14(DOG_TITLE_SPRITE, 0, 0, 0);
    func_8002C7EC(DOG_TITLE_SPRITE, 3);
    func_8002C914(DOG_TITLE_SPRITE, 0xFF, 0xFF, 0xFF, 0xFF);
    func_8002B80C(DOG_TITLE_SPRITE, 0x1D, 0xFE);
}

static inline u32 increment(u32 x) {
    return x+1;
}

static inline u32 incrementUnk4() {
    titleScreenContext.unk_4 = increment(titleScreenContext.unk_4);
}

static inline void setNewMode(u8 mode) {
    titleScreenContext.mode = mode;
    titleScreenContext.unk_4 = 0;
}

static inline void resetSpriteFlags() {
    func_8002B6B8(0x42);
    func_8002B6B8(0x43);
    func_8002B6B8(DOG_TITLE_SPRITE);
    func_8002B6B8(0x45);
    func_8002B6B8(0x46);
    func_8002B6B8(0x47);
    func_8002B6B8(0x48);
    func_8002B6B8(0x49);
    func_8002B6B8(0x4A);
    func_8002B6B8(0x4B);
    func_8002B6B8(0x4C);
    func_8002B6B8(0x4D);
    func_8002B6B8(0x4E);
    func_8002B6B8(0x4F);
    func_8002B6B8(0x50);
    func_8002B6B8(0x51);
    func_8002B6B8(0x52);
    func_8002B6B8(0x53);
    func_8002B6B8(0x54);
    func_8002B6B8(0x55);
    func_8002B6B8(0x56);
    func_8002B6B8(0x57);
    func_8002B6B8(0x58);
    func_8002B6B8(0x59);
    func_8002B6B8(0x5A);
    func_8002B6B8(0x5B);
}

// TODO: label properly
static inline bool inline1() {

    if (titleScreenContext.rowHighlighted != 1) {
        func_8002BD0C(0x45, -56.0f, -52.0f, 56.0f);
        titleScreenContext.rowHighlighted = 1;
        setAudio(2);
    }

   return 1;

}

// TODO: label properly
static inline bool inline2() {

    if (titleScreenContext.rowHighlighted != 0) {
        func_8002BD0C(0x45, -56.0f, -20.0f, 56.0f);
        titleScreenContext.rowHighlighted = 0;
        setAudio(2);
    }

    return 1;
}

static inline bool handleFrameIncrement() {

    titleScreenContext.frameCounter++;
    
    if (titleScreenContext.frameCounter == 500) {

        func_8002C1C0(0x42, 0, 0, 0, 0, 8);
        func_8002C1C0(0x43, 0, 0, 0, 0, 8);
        func_8002C1C0(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
        func_8002C1C0(0x45, 0, 0, 0, 0, 8);
        func_8002C1C0(0x46, 0, 0, 0, 0, 8);
        func_8002C1C0(0x47, 0, 0, 0, 0, 8);
        func_8002C1C0(0x48, 0, 0, 0, 0, 8);
        func_8002C1C0(0x49, 0, 0, 0, 0, 8);
        func_8002C1C0(0x4A, 0, 0, 0, 0, 8);
        func_8002C1C0(0x4B, 0, 0, 0, 0, 8);
        func_8002C1C0(0x4C, 0, 0, 0, 0, 8);
        func_8002C1C0(0x4D, 0, 0, 0, 0, 8);
        func_8002C1C0(0x4E, 0, 0, 0, 0, 8);
        func_8002C1C0(0x4F, 0, 0, 0, 0, 8);
        func_8002C1C0(0x50, 0, 0, 0, 0, 8);
        func_8002C1C0(0x51, 0, 0, 0, 0, 8);
        func_8002C1C0(0x52, 0, 0, 0, 0, 8);
        func_8002C1C0(0x53, 0, 0, 0, 0, 8);
        func_8002C1C0(0x54, 0, 0, 0, 0, 8);
        func_8002C1C0(0x55, 0, 0, 0, 0, 8);
        func_8002C1C0(0x56, 0, 0, 0, 0, 8);
        func_8002C1C0(0x57, 0, 0, 0, 0, 8);
        func_8002C1C0(0x58, 0, 0, 0, 0, 8);
        func_8002C1C0(0x59, 0, 0, 0, 0, 8);
        func_8002C1C0(0x5A, 0, 0, 0, 0, 8);
        func_8002C1C0(0x5B, 0, 0, 0, 0, 8);

        setSongWithDefaultSpeed(TITLE_THEME);

        titleScreenContext.mode = 5;

        return 1;

    }
    
}

// jtbl_80123658
//INCLUDE_ASM(const s32, "title", titleScreenMainLoopCallback);

// main loop callback
void titleScreenMainLoopCallback(void) {

    u8 set = 0;
    
    switch (titleScreenContext.mode) {

        case 0:

            set = handleFrameIncrement();

            if (func_8004D380(CONTROLLER_1, BUTTON_START)) {

                if (!set) {
                    
                    func_8002BAD8(0x46);
                    func_8002B80C(0x46, 4, 0xFF);
                    func_8002BAD8(DOG_TITLE_SPRITE);
                    func_8002BAD8(0x45);
                    func_8002B80C(DOG_TITLE_SPRITE, 8, 0xFE);
    
                    titleScreenContext.mode = 1;
                    setAudio(0);
                }
            }

            func_800E0ED4();
            break;
        
        case 1:

            // static inline?
            if (titleScreenContext.unk_4 < 0x40) {

                func_8002BE98(0x46, 5.0f, 0.0f, 0.0f);
                func_8002BE98(0x47, 5.0f, 0, 0);
                func_8002BE98(0x48, 5.0f, 0, 0);
                func_8002BE98(0x49, 5.0f, 0, 0);
                func_8002BE98(0x4A, 5.0f, 0, 0);
                func_8002BE98(0x4B, 5.0f, 0, 0);
                func_8002BE98(0x52, 5.0f, 0, 0);
                func_8002BE98(0x53, 5.0f, 0, 0);
                func_8002BE98(0x4E, 2.0f, 0, 0);
                func_8002BE98(0x4D, 3.0f, 0, 0);
                func_8002BE98(0x4C, 5.0f, 0, 0);
                func_8002BE98(0x51, 2.0f, 0, 0);
                func_8002BE98(0x50, 3.0f, 0, 0);
                func_8002BE98(0x4F, 5.0f, 0, 0);
                func_8002BE98(0x54, 1.0f, 0, 0);
                func_8002BE98(0x55, 1.0f, 0, 0);
                func_8002BE98(0x56, 1.0f, 0, 0);
                func_8002BE98(0x57, 1.0f, 0, 0);
                func_8002BE98(0x58, 1.0f, 0, 0);
                func_8002BE98(0x59, 1.0f, 0, 0);
                func_8002BE98(0x5A, 1.0f, 0, 0);
                func_8002BE98(0x5B, 1.0f, 0, 0);

            } else {
                setNewMode(2);
            }

            // FIXME: shouldn't be right
            incrementUnk4();
            
            break;
        
        case 2:

            func_8002BAD8(DOG_TITLE_SPRITE);
            func_8002B80C(DOG_TITLE_SPRITE, 0x1D, 0xFE);
            func_8002BD0C(0x45, -56.0f, -20.0f, 56.0f);
            func_8002B80C(0x45, 3, 0xFE);
            
            titleScreenContext.mode = 3;
            titleScreenContext.rowHighlighted = 0;

            break;
        
        case 3:

            if (func_8004D3C8(CONTROLLER_1, BUTTON_STICK_UP)) {        

                if (!set) {
                    set = inline1();
                }
            }
            
            if (func_8004D3C8(CONTROLLER_1, 0x100000)) {

                if (!set) {
                    set = inline2();
                }
            }
            
            if (func_8004D380(CONTROLLER_1, BUTTON_A | BUTTON_START)) {

                if (!set) {
                    
                    if (!titleScreenContext.rowHighlighted) {
                        func_8002BAD8(0x47);
                        func_8002B80C(0x47, 6, 0xFF);
                    } else {
                        func_8002BAD8(0x48);
                        func_8002B80C(0x48, 8, 0xFF);
                    }
                      
                    func_8002C1C0(0x42, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x43, 0, 0, 0, 0, 8);
                    func_8002C1C0(DOG_TITLE_SPRITE, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x45, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x46, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x47, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x48, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x49, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x4A, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x4B, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x4C, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x4D, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x4E, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x4F, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x50, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x51, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x52, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x53, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x54, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x55, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x56, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x57, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x58, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x59, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x5A, 0, 0, 0, 0, 8);
                    func_8002C1C0(0x5B, 0, 0, 0, 0, 8);
    
                    titleScreenContext.mode = 4;
    
                    setAudio(0);
                    
                }
                
            }

            func_800E0ED4();

            break;

        case 4:
            
            // check sprite flag
            if (func_8002CBF8(0x42)) {

                resetSpriteFlags();
                
                if (titleScreenContext.rowHighlighted == 0) {
                    // load select game screen
                    func_800E1380(FALSE);
                } else {
                    // how to play screen
                    func_80055F08(0x640, 0, 0);
                }
            }

            break;

        case 5:
            
            if (func_8002CBF8(0x42) && checkDefaultSongChannelOpen(TITLE_THEME)) {
                
                resetSpriteFlags();
 
                // demo cutscene index
                switch (D_801C3F50) {
                    case 0:
                        func_80055F08(DEMO_CUTSCENE_1, 0, 1);
                        break;
                    case 1:
                        func_80055F08(DEMO_CUTSCENE_2, 0x1D, 1);
                        break;
                    case 2:
                        func_80055F08(DEMO_CUTSCENE_3, 0x32, 1);
                        break;
                    default:
                        break;
                }

                D_801C3F50++;

                if (D_801C3F50 > 2) {
                    D_801C3F50 = 0;
                }   
            }
        }  
            
    func_800E0FEC();
    func_800E10E8();
}

static inline void handleUpdateDogAnimation() {
    
    switch (titleScreenContext.dogAnimationIndex) {
            case 0:
                func_8002B80C(DOG_TITLE_SPRITE, 0xB, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 1:
                func_8002B80C(DOG_TITLE_SPRITE, 0x13, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 2:
                func_8002B80C(DOG_TITLE_SPRITE, 0x2D, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;
            case 3:
                func_8002B80C(DOG_TITLE_SPRITE, 0x32, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            case 4:
                func_8002B80C(DOG_TITLE_SPRITE, 0x48, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;         
            case 5:
                func_8002B80C(DOG_TITLE_SPRITE, 0x63, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            case 6:
                func_8002B80C(DOG_TITLE_SPRITE, 0x69, 0xFF);
                titleScreenContext.dogAnimationIndex = 0xFF;
                break;            
            default:
               func_8002B80C(DOG_TITLE_SPRITE, 0x1D, 0xFE);
                if (titleScreenContext.dogIdleCounter >= 0x1F) {
                    titleScreenContext.dogAnimationIndex = getRandomNumberInRange(0, 10);
                    titleScreenContext.dogIdleCounter = 0;
                }
                break;
        }
    
        titleScreenContext.dogIdleCounter++;
}

// jtbl_80123670
//INCLUDE_ASM(const s32, "title", func_800E0ED4);

void func_800E0ED4(void) {

    if (func_8002CC44(DOG_TITLE_SPRITE)) {
        func_8002BAD8(DOG_TITLE_SPRITE);
        handleUpdateDogAnimation();
    }
}

// alternate
/*
void func_800E0ED4(void) {

    if (func_8002CC44(0x44)) {
        
        func_8002BAD8(0x44);

        switch (D_80126572) {
            case 0:
                func_8002B80C(0x44, 0xB, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 1:
                func_8002B80C(0x44, 0x13, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 2:
                func_8002B80C(0x44, 0x2D, 0xFF);
                D_80126572 = 0xFF;
                break;
            case 3:
                func_8002B80C(0x44, 0x32, 0xFF);
                D_80126572 = 0xFF;
                break;            
            case 4:
                func_8002B80C(0x44, 0x48, 0xFF);
                D_80126572 = 0xFF;
                break;         
            case 5:
                func_8002B80C(0x44, 0x63, 0xFF);
                D_80126572 = 0xFF;
                break;            
            case 6:
                func_8002B80C(0x44, 0x69, 0xFF);
                D_80126572 = 0xFF;
                break;            
            default:
               func_8002B80C(0x44, 0x1D, 0xFE);
                if (D_80126576 >= 0x1F) {
                    D_80126572 = getRandomNumberInRange(0, 10);
                    D_80126576 = 0;
                }
                break;
        }

        D_80126576++;
    }
}
*/
//INCLUDE_ASM(const s32, "title", func_800E0FEC);

void func_800E0FEC(void) {

    if (func_8002CC44(0x4D)) {
        if (getRandomNumberInRange(0, 1)) {
            func_8002BAD8(0x4D);
            func_8002BAD8(0x4C);
            func_8002BAD8(0x50);
            func_8002BAD8(0x4F);
            func_8002B80C(0x4D, 0xC, 0xFE);
            func_8002B80C(0x4C, 0xE, 0xFE);
            func_8002B80C(0x50, 0xC, 0xFE);
            func_8002B80C(0x4F, 0xE, 0xFE);
        } else {
            func_8002BAD8(0x4D);
            func_8002BAD8(0x4C);
            func_8002BAD8(0x50);
            func_8002BAD8(0x4F);
            func_8002B80C(0x4D, 0xD, 0xFE);
            func_8002B80C(0x4C, 0xF, 0xFE);
            func_8002B80C(0x50, 0xD, 0xFE);
            func_8002B80C(0x4F, 0xF, 0xFE);
        }
    }

}

//INCLUDE_ASM(const s32, "title", func_800E10E8);

void func_800E10E8(void) {
    
    if (globalSprites[0x56].shrink.x > 320.0f) {
        func_8002BD0C(0x56, titleScreenContext.centerCoordinate - 320.0f, 64.0f, 64.0f);
    }
    if (globalSprites[0x57].shrink.x > 320.0f) {
        func_8002BD0C(0x57, titleScreenContext.centerCoordinate - 320.0f, 96.0f, 64.0f);
    }
    if (globalSprites[0x58].shrink.x > 320.0f) {
        func_8002BD0C(0x58, titleScreenContext.centerCoordinate - 320.0f, 80.0f, 64.0f);
    }
    if (globalSprites[0x59].shrink.x > 320.0f) {
        func_8002BD0C(0x59, titleScreenContext.centerCoordinate - 320.0f, 72.0f, 64.0f);
    }
    if (globalSprites[0x5A].shrink.x > 320.0f) {
        func_8002BD0C(0x5A, titleScreenContext.centerCoordinate - 320.0f, 108.0f, 64.0f);
    }
    if (globalSprites[0x5B].shrink.x > 320.0f) {
        func_8002BD0C(0x5B, titleScreenContext.centerCoordinate - 320.0f, 88.0f, 64.0f);
    }
    
    func_8002BE98(0x56, 0.2f, 0.0f, 0.0f);
    func_8002BE98(0x57, 0.1f, 0.0f, 0.0f);
    func_8002BE98(0x58, 0.3f, 0.0f, 0.0f);
    func_8002BE98(0x59, 0.4f, 0.0f, 0.0f);
    func_8002BE98(0x5A, 0.1f, 0.0f, 0.0f);
    func_8002BE98(0x5B, 0.2f, 0.0f, 0.0f);
    
}