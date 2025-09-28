#include "common.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/graphic.h"
#include "system/message.h"
#include "system/globalSprites.h"
#include "system/interpolation.h"
#include "system/sceneGraph.h"
#include "system/sprite.h"

#include "mainproc.h"

#include "ld_symbols.h"

// bss

MessageBox messageBoxes[MAX_DIALOGUE_BOXES];
GameVariableString gameVariableStrings[64];

// text buffer of current displayed lines in game string characters; each line is size 0x20 (0x10 u16s)
// u16 linesBuffer[8][0x10]
// always 16 chars per line (filled with padding) 
u16 textBuffer[256];

FontData fontTexturesBuffer[2][256];
Vtx fontVertices[2][512][4];
Gfx messageBoxDisplayList[2][3072];

u8* characterAvatarsAnimationsPtr;

// rodata
static const u32 powersOfTen[8];
static const u8 digitCharacterCodes[16];
static const u8 controlByteBitMasks[8];

static const Gfx D_8011EE98;
static const Gfx D_8011EEA0;
static const Gfx D_8011EEA8;
static const Gfx D_8011EEB0;
static const Gfx D_8011EEB8;
static const Gfx D_8011EEC0;

// forward delcarations
bool func_8003F024(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
bool func_8003E77C(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
void func_8003FD74(void);
bool func_8003FDB0(u16 index);
void func_8004022C(u16 index);
u8 func_800403F0(u16 index, u16 arg1);
void advanceToLine(u16, u8); 
void func_80040318(u16 index);
void func_80040858(u16);                               
void func_80040C38(u16);                               
u16 func_80041850(u16 index);
u8 func_800419C4(u16 index);
Gfx* func_80041CD8(Gfx* dl, MessageBox* messageBox);
Gfx* func_80042014(Gfx* dl, MessageBox* messageBox, u8 arg2, s32 arg3); 
void addMessageCharSceneNode(MessageBox* messageBox, u8 line, Gfx* dl);    
u32 getTextAddress(u16 index, u16 offset);
void unpackFontCI2Data(u16, MessageBoxFont*, u8*);


//INCLUDE_ASM("asm/nonmatchings/system/message", initializeMessageBoxes);

void initializeMessageBoxes(void) {

    u16 i = 0;

    for (i = 0; i < MAX_DIALOGUE_BOXES; i++) {

        messageBoxes[i].flags = 0;
        
        messageBoxes[i].textIndex = 0;
        
        messageBoxes[i].currentCompressionControlByte = 0;
        messageBoxes[i].compressionBitIndex = 0;
         
        messageBoxes[i].unk_7C = 0;

        messageBoxes[i].scrollCount = 0;
        messageBoxes[i].totalLinesToPrint = 0;
        messageBoxes[i].currentCharCountOnLine = 0;
        messageBoxes[i].currentLineBeingPrinted = 0;

        messageBoxes[i].characterSpacing = 0;
        messageBoxes[i].lineSpacing = 0;

        messageBoxes[i].characterPrintSfx = 0xFF;
        messageBoxes[i].unk_74 = 0xFF;
        messageBoxes[i].messageCloseSfx = 0xFF;

        messageBoxes[i].totalCharactersProcessed = 0;
        messageBoxes[i].charactersProcessedPerLine = 0;
        messageBoxes[i].defaultScrollSpeed = 0;
        messageBoxes[i].buttonMask = 0;
        messageBoxes[i].textAddressesIndex = 0;
 
        messageBoxes[i].fontContext.characterCellWidth = 0;
        messageBoxes[i].fontContext.characterCellHeight = 0;

        messageBoxes[i].viewSpacePosition.x = 0;
        messageBoxes[i].viewSpacePosition.y = 0;
        messageBoxes[i].viewSpacePosition.z = 0;

        messageBoxes[i].textBoxLineCharWidth = 0;
        messageBoxes[i].textBoxVisibleRows = 0;

        messageBoxes[i].unk_14.r = 255.0f;
        messageBoxes[i].unk_14.g = 255.0f;
        messageBoxes[i].unk_14.b = 255.0f;
        messageBoxes[i].unk_14.a = 255.0f;

        messageBoxes[i].unk_24.r = 255.0f;
        messageBoxes[i].unk_24.g = 255.0f;
        messageBoxes[i].unk_24.b = 255.0f;
        messageBoxes[i].unk_24.a = 255.0f;
       
        initializeInterpolator((Interpolator*)&messageBoxes[i].unk_64, 0, 0);
        initializeInterpolator((Interpolator*)&messageBoxes[i].scrollInterpolator, 0, 0);
         
        messageBoxes[i].unk_9D = 0;
        
    }

    for (i = 0; i < 3; i++) {
        
    }

    func_8003FD74();
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", initializeEmptyMessageBox);

bool initializeEmptyMessageBox(u16 messageBoxIndex, u8* textBufferAddr) {
 
    bool set = FALSE;
    
    if (messageBoxIndex < MAX_DIALOGUE_BOXES) {

        if (!(messageBoxes[messageBoxIndex].flags & DIALOGUE_BOX_ACTIVE)) {

            messageBoxes[messageBoxIndex].currentCompressionControlByte = 0;
            messageBoxes[messageBoxIndex].compressionBitIndex = 0;
 
            messageBoxes[messageBoxIndex].scrollCount = 0;
            messageBoxes[messageBoxIndex].totalLinesToPrint = 0;
            messageBoxes[messageBoxIndex].currentCharCountOnLine = 0;
            messageBoxes[messageBoxIndex].currentLineBeingPrinted = 0;

            messageBoxes[messageBoxIndex].characterPrintSfx = 0xFF;
            messageBoxes[messageBoxIndex].unk_74 = 0xFF;
            messageBoxes[messageBoxIndex].messageCloseSfx = 0xFF;

            messageBoxes[messageBoxIndex].textBufferBase = textBufferAddr;
            
            messageBoxes[messageBoxIndex].flags = 1;

            initializeInterpolator((Interpolator*)&messageBoxes[messageBoxIndex].scrollInterpolator, 0, 0);
            func_8003F024(messageBoxIndex, 0xFF, 0xFF, 0xFF, 0xFF);
            
            set = TRUE;        

        }
        
    }
    
    return set;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003DD14);

bool func_8003DD14(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            
            messageBoxes[index].flags &= ~1;
            
            if (messageBoxes[index].flags & DIALOGUE_BOX_HAS_DIALOGUE_WINDOW) {
                resetAnimationState(dialogueWindows[messageBoxes[index].dialogueWindowIndex].spriteIndex);
            }
            
            if (messageBoxes[index].flags & DIALOGUE_BOX_HAS_CHARACTER_AVATAR) {
                resetAnimationState(characterAvatars[messageBoxes[index].characterAvatarIndex].spriteIndex);
            }
            
            result = TRUE;

        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", initializeMessageBox);

bool initializeMessageBox(u16 messageBoxIndex, u16 textAddressesIndex, u16 textIndex, u32 flag) {

    bool result = FALSE;

    u16 tempTextIndex;
    u32 textAddress;

    tempTextIndex = textIndex;
    
    if (messageBoxIndex < MAX_DIALOGUE_BOXES) {
        
        if (messageBoxes[messageBoxIndex].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[messageBoxIndex].textIndex = textIndex;
            messageBoxes[messageBoxIndex].textAddressesIndex = textAddressesIndex;
            
            nuPiReadRom(textAddresses[textAddressesIndex].romIndexStart, textAddresses[textAddressesIndex].indexVaddr, textAddresses[textAddressesIndex].romIndexEnd - textAddresses[textAddressesIndex].romIndexStart);

            textAddress = getTextAddress(messageBoxIndex, messageBoxes[messageBoxIndex].textIndex);
            nuPiReadRom(textAddress, messageBoxes[messageBoxIndex].textBufferBase, getTextAddress(messageBoxIndex, messageBoxes[messageBoxIndex].textIndex + 1) - textAddress);
            
            messageBoxes[messageBoxIndex].totalLinesToPrint = func_800403F0(messageBoxIndex, tempTextIndex);
            messageBoxes[messageBoxIndex].currentCompressionControlByte = 0;
            messageBoxes[messageBoxIndex].compressionBitIndex = 0;
            messageBoxes[messageBoxIndex].scrollCount = 0;
            messageBoxes[messageBoxIndex].currentCharCountOnLine = 0;
            messageBoxes[messageBoxIndex].currentLineBeingPrinted = 0;
            messageBoxes[messageBoxIndex].unk_9D = 0;

            messageBoxes[messageBoxIndex].currentCharPtr = messageBoxes[messageBoxIndex].textBufferBase;

            messageBoxes[messageBoxIndex].flags &= ~4;
            messageBoxes[messageBoxIndex].flags &= ~0x4000;
            messageBoxes[messageBoxIndex].flags |= DIALOGUE_BOX_INITIALIZED;

            messageBoxes[messageBoxIndex].flags &= ~0x8000;
            messageBoxes[messageBoxIndex].flags &= ~0x40000;
            messageBoxes[messageBoxIndex].flags &= ~0x80000;
            messageBoxes[messageBoxIndex].flags &= ~0x100000;
            messageBoxes[messageBoxIndex].flags &= ~0x20000;

            if (flag == 0x8000) {
                messageBoxes[messageBoxIndex].flags |= 0x8000;
            }
            
            if (flag == 0x40000) {
                messageBoxes[messageBoxIndex].flags |= flag;
            }

            if (flag == 0x80000) {
                messageBoxes[messageBoxIndex].flags |= flag;
            }

            if (flag == 0x100000) {
                messageBoxes[messageBoxIndex].flags |= flag;
            }

            if (messageBoxes[messageBoxIndex].flags & DIALOGUE_BOX_HAS_OVERLAY_ICON) {
                dmaSprite(overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].spriteIndex, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].romTextureStart, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].romTextureEnd, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].romIndexStart, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].romIndexEnd, 0, 0, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].vaddrTextureStart, NULL, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].vaddrTextureEnd, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].vaddrIndexStart, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].vaddrIndexEnd, overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].unk_20, 0, 0);
                setBilinearFiltering(overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].spriteIndex, TRUE);
                setSpriteColor(overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C680(overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].spriteIndex, 2, 2);
                setSpriteRenderingLayer(overlayIcons[messageBoxes[messageBoxIndex].overlayIconIndex].spriteIndex, (1 | 2));
            }

            if (messageBoxes[messageBoxIndex].flags & DIALOGUE_BOX_HAS_CHARACTER_AVATAR) {
                dmaSprite(characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].spriteIndex, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].romTextureStart, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].romTextureEnd, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].romAssetsIndexStart, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].romAssetsIndexEnd, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].romSpritesheetIndexStart, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].romSpritesheetIndexEnd, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].vaddrTexture, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].vaddrSpritesheet, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].vaddrPalette, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].vaddrAnimation, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].vaddrSpriteToPalette, characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].vaddrSpritesheetIndex, 1, 0);
                setBilinearFiltering(characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].spriteIndex, TRUE);
                setSpriteColor(characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C680(characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].spriteIndex, 2, 2);
                setSpriteRenderingLayer(characterAvatars[messageBoxes[messageBoxIndex].characterAvatarIndex].spriteIndex, (1 | 2));
            }

            if (messageBoxes[messageBoxIndex].flags & DIALOGUE_BOX_HAS_DIALOGUE_WINDOW) {
                dmaSprite(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].romTextureStart, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].romTextureEnd, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].romIndexStart, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].romIndexEnd, 0, 0, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].vaddrTextureStart, NULL, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].vaddrTextureEnd, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].vaddrIndexStart, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].vaddrIndexEnd, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].unk_20, 0, 0);
                setBilinearFiltering(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);
                setSpriteColor(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8002C680(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, 2, 2);
                setSpriteRenderingLayer(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, (1 | 2));
                startSpriteAnimation(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteOffset, dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].flag);
                setSpriteViewSpacePosition(dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].spriteIndex, messageBoxes[messageBoxIndex].viewSpacePosition.x + dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].coordinates.x, messageBoxes[messageBoxIndex].viewSpacePosition.y + dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].coordinates.y, (messageBoxes[messageBoxIndex].viewSpacePosition.z + dialogueWindows[messageBoxes[messageBoxIndex].dialogueWindowIndex].coordinates.z) - 2.0f);
            }

            func_8003E77C(messageBoxIndex, 0xFF, 0xFF, 0xFF, 0xFF);

            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003E77C);

bool func_8003E77C(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) { 

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            
            messageBoxes[index].unk_14.r = (messageBoxes[index].unk_4.r * r) / 255.0f;
            messageBoxes[index].unk_14.g = (messageBoxes[index].unk_4.g * g) / 255.0f;
            messageBoxes[index].unk_14.b = (messageBoxes[index].unk_4.b * b) / 255.0f;
            messageBoxes[index].unk_14.a = (messageBoxes[index].unk_4.a * a) / 255.0f;
    
            messageBoxes[index].unk_24.r = (messageBoxes[index].unk_4.r * r) / 255.0f;
            messageBoxes[index].unk_24.g = (messageBoxes[index].unk_4.g * g) / 255.0f;
            messageBoxes[index].unk_24.b = (messageBoxes[index].unk_4.b * b) / 255.0f;
            messageBoxes[index].unk_24.a = (messageBoxes[index].unk_4.a * a) / 255.0f;
    
            result = TRUE;
     
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003E910);

bool func_8003E910(u16 index, s8 r, s8 g, s8 b, s8 a) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) { 

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].unk_14.r += r;
            messageBoxes[index].unk_14.g += g;
            messageBoxes[index].unk_14.b += b;
            messageBoxes[index].unk_14.a += a;
            
            result = TRUE;
            
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003EA1C);

bool func_8003EA1C(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    f32 tempF;
    s16 temp = getAbsoluteValue(rate);

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) { 

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].unk_24.r = (messageBoxes[index].unk_4.r * r) / 255.0f;
            messageBoxes[index].unk_24.g = (messageBoxes[index].unk_4.g * g) / 255.0f;
            messageBoxes[index].unk_24.b = (messageBoxes[index].unk_4.b * b) / 255.0f;
            messageBoxes[index].unk_24.a = (messageBoxes[index].unk_4.a * a) / 255.0f;
            
            messageBoxes[index].flags &= ~0x200000;

            if (messageBoxes[index].unk_24.r < messageBoxes[index].unk_14.r) {
                tempF = messageBoxes[index].unk_14.r - messageBoxes[index].unk_24.r;
            } else {
                tempF = messageBoxes[index].unk_24.r - messageBoxes[index].unk_14.r;
            }

            messageBoxes[index].unk_34.r = (tempF * temp) / messageBoxes[index].unk_4.r;
            
            if (messageBoxes[index].unk_24.g < messageBoxes[index].unk_14.g) {
                tempF = messageBoxes[index].unk_14.g - messageBoxes[index].unk_24.g;
            } else {
                tempF = messageBoxes[index].unk_24.g - messageBoxes[index].unk_14.g;
            }

            messageBoxes[index].unk_34.g = (tempF * temp) / messageBoxes[index].unk_4.g;
            
            if (messageBoxes[index].unk_24.b < messageBoxes[index].unk_14.b) {
                tempF = messageBoxes[index].unk_14.b - messageBoxes[index].unk_24.b;
            } else {
                tempF = messageBoxes[index].unk_24.b - messageBoxes[index].unk_14.b;
            }

            messageBoxes[index].unk_34.b = (tempF * temp) / messageBoxes[index].unk_4.b;

            if (messageBoxes[index].unk_24.a < messageBoxes[index].unk_14.a) {
                tempF = messageBoxes[index].unk_14.a - messageBoxes[index].unk_24.a;
            } else {
                tempF = messageBoxes[index].unk_24.a - messageBoxes[index].unk_14.a;
            }

            messageBoxes[index].unk_34.a = (tempF * temp) / messageBoxes[index].unk_4.a;
        
            if (messageBoxes[index].flags & 0x200) {
                updateSpriteRGBA(dialogueWindows[messageBoxes[index].dialogueWindowIndex].spriteIndex, 
                    r, g, b, a, temp);
            }

            if (messageBoxes[index].flags & 0x10000) {
                updateSpriteRGBA(characterAvatars[messageBoxes[index].characterAvatarIndex].spriteIndex, 
                    r, g, b, a, temp);
            }

            if (messageBoxes[index].flags & 0x400) {
                updateSpriteRGBA(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex, 
                    r, g, b, a, temp);
            }
            
            result = TRUE;
        
        }

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003EE7C);

// update alpha
bool func_8003EE7C(u16 index, u8 arg1, s16 arg2) {

    bool result;
    s16 temp;
    f32 tempF;

    temp = getAbsoluteValue(arg2);

    result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].unk_24.a = (messageBoxes[index].unk_4.a * arg1) / 255.0f;

            messageBoxes[index].flags &= ~(0x200000);

            if (messageBoxes[index].unk_24.a < messageBoxes[index].unk_14.a) {
                tempF = messageBoxes[index].unk_14.a - messageBoxes[index].unk_24.a;
            } else {
                tempF = messageBoxes[index].unk_24.a - messageBoxes[index].unk_14.a;
            }

            messageBoxes[index].unk_34.a = (tempF * temp) / messageBoxes[index].unk_4.a;

            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003EFD8);

bool func_8003EFD8(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            result = (messageBoxes[index].flags >> 0x15) & 1;
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F024);

bool func_8003F024(u16 index, u8 r, u8 g, u8 b, u8 a) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].unk_4.r = r;
            messageBoxes[index].unk_4.g = g;
            messageBoxes[index].unk_4.b = b;
            messageBoxes[index].unk_4.a = a;

            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F0DC);

bool func_8003F0DC(void) {

    u16 i;
    bool result = FALSE;

    for (i = 0; i < MAX_DIALOGUE_BOXES; i++) {

        if (messageBoxes[i].flags & 4) {
            result = TRUE;
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F130);

bool func_8003F130(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            if (!(messageBoxes[index].flags & 0x8000) && !(messageBoxes[index].flags & 0x40000) && !(messageBoxes[index].flags & 0x80000)) {
                messageBoxes[index].flags &= ~(2 | 8);
            }

            if ((messageBoxes[index].flags & 0x200) && !(messageBoxes[index].flags & 0x8000) && !(messageBoxes[index].flags & 0x40000) && !(messageBoxes[index].flags & 0x80000)) {

                resetAnimationState(dialogueWindows[messageBoxes[index].dialogueWindowIndex].spriteIndex);

                if (messageBoxes[index].flags & 0x10000) {
                    resetAnimationState(characterAvatars[messageBoxes[index].characterAvatarIndex].spriteIndex);        
                }
            
            }

            result = TRUE;
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F28C);

bool func_8003F28C(u16 index, s16 rate) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES && (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE)) {

        messageBoxes[index].unk_7C = rate;
        
        initializeInterpolator((Interpolator*)&messageBoxes[index].unk_64, rate, 0);
        
        result = TRUE;
            
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", setTextAddresses);

bool setTextAddresses(u16 textAddressesIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 indexVaddr) {

    bool result = FALSE;

    if (textAddressesIndex < MAX_TEXT_BANKS) {

        textAddresses[textAddressesIndex].romIndexStart = romIndexStart;
        textAddresses[textAddressesIndex].romIndexEnd = romIndexEnd;
        textAddresses[textAddressesIndex].indexVaddr = indexVaddr;
        textAddresses[textAddressesIndex].romTextStart = romTextStart;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F360);

bool func_8003F360(u16 index, s16 rate, u8 flags) {
    
    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {
        
        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].unk_7C = rate;
            initializeInterpolator((Interpolator*)&messageBoxes[index].unk_64, rate, 0);

            messageBoxes[index].flags &= ~(0x10 | 0x20);

            switch (flags) {

                case 0:
                    break;
                
                case 1:
                    messageBoxes[index].flags |= 0x10;
                    break;

                case 2:
                    messageBoxes[index].flags |= 0x20;
                    break;                    

                case 3:
                    messageBoxes[index].flags |= (0x10 | 0x20);
                    break;               
                
            }
            
            result = TRUE;
        
        }
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F464);

bool func_8003F464(u16 index, u8 arg1, u8 arg2, u8* compressedCI2FontData, u16* fontPalettePtr) {
    
    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {
        
        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            
            messageBoxes[index].fontContext.characterCellWidth = arg1;
            messageBoxes[index].fontContext.characterCellHeight = arg2;
            messageBoxes[index].fontContext.compressedCI2FontData = compressedCI2FontData;
            messageBoxes[index].fontContext.fontPalettePtr = fontPalettePtr;
             
            result = TRUE;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", setMessageBoxSfx);

bool setMessageBoxSfx(u16 index, u32 arg1, u32 arg2, u32 arg3) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].characterPrintSfx = arg1;
            messageBoxes[index].unk_74 = arg2;
            messageBoxes[index].messageCloseSfx = arg3;
            
            result = TRUE;
        
        }
    
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", setMessageBoxViewSpacePosition);

bool setMessageBoxViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            
            messageBoxes[index].viewSpacePosition.x = x;
            messageBoxes[index].viewSpacePosition.y = y;
            messageBoxes[index].viewSpacePosition.z = z;
            
            result = TRUE;

        }

    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F5D0);

bool func_8003F5D0(u16 index, u8 arg1, u8 arg2) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].textBoxLineCharWidth = arg1;
            messageBoxes[index].textBoxVisibleRows = arg2;
            
            result = TRUE;
            
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F630);

bool func_8003F630(u16 index, u8 arg1, u8 arg2) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            messageBoxes[index].characterSpacing = arg1;
            messageBoxes[index].lineSpacing = arg2;
            
            result = TRUE;
            
        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", setMessageBoxSpriteIndices);

bool setMessageBoxSpriteIndices(u16 index, u8 dialogueWindowIndex, u8 overlayIconIndex, u8 characterAvatarIndex) {

    bool result = FALSE;
    
    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) { 
            
            messageBoxes[index].dialogueWindowIndex = dialogueWindowIndex;
            messageBoxes[index].overlayIconIndex = overlayIconIndex;
            messageBoxes[index].characterAvatarIndex = characterAvatarIndex;

            if (messageBoxes[index].dialogueWindowIndex != 0xFF) {
                messageBoxes[index].flags |= DIALOGUE_BOX_HAS_DIALOGUE_WINDOW;
            } else {
                messageBoxes[index].flags &= ~DIALOGUE_BOX_HAS_DIALOGUE_WINDOW;
            }

            if (messageBoxes[index].overlayIconIndex != 0xFF) {
                messageBoxes[index].flags |= DIALOGUE_BOX_HAS_OVERLAY_ICON;
            } else {
                messageBoxes[index].flags &= ~DIALOGUE_BOX_HAS_OVERLAY_ICON;
            }

            if (messageBoxes[index].characterAvatarIndex != 0xFF) {
                messageBoxes[index].flags |= DIALOGUE_BOX_HAS_CHARACTER_AVATAR;
            } else {
                messageBoxes[index].flags &= ~DIALOGUE_BOX_HAS_CHARACTER_AVATAR;
            }

            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F80C);

bool func_8003F80C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index < 3) {

        dialogueWindows[index].romTextureStart = romTextureStart;
        dialogueWindows[index].romTextureEnd = romTextureEnd;
        dialogueWindows[index].romIndexStart = romIndexStart;
        dialogueWindows[index].romIndexEnd = romIndexEnd;

        dialogueWindows[index].vaddrTextureStart = vaddrTextureStart;
        dialogueWindows[index].vaddrTextureEnd = vaddrTextureEnd;
        dialogueWindows[index].vaddrIndexStart = vaddrIndexStart;
        dialogueWindows[index].vaddrIndexEnd = vaddrIndexEnd;

        dialogueWindows[index].unk_20 = argA;

        dialogueWindows[index].spriteIndex = spriteIndex;
        dialogueWindows[index].spriteOffset = spriteOffset;

        dialogueWindows[index].flag= flag;

        dialogueWindows[index].coordinates.x = x;
        dialogueWindows[index].coordinates.y = y;
        dialogueWindows[index].coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003F910);

bool func_8003F910(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, 
    u32 argA, u16 offset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = 0;
    
    if (index < 2) {

        overlayIcons[index].romTextureStart = romTextureStart;
        overlayIcons[index].romTextureEnd = romTextureEnd;
        overlayIcons[index].romIndexStart = romIndexStart;
        overlayIcons[index].romIndexEnd = romIndexEnd;

        overlayIcons[index].vaddrTextureStart = vaddrTextureStart;
        overlayIcons[index].vaddrTextureEnd = vaddrTextureEnd;
        overlayIcons[index].vaddrIndexStart = vaddrIndexStart;
        overlayIcons[index].vaddrIndexEnd = vaddrIndexEnd;

        overlayIcons[index].unk_20 = argA;
        overlayIcons[index].spriteIndex = spriteIndex;

        overlayIcons[index].spriteOffset = offset;
        
        overlayIcons[index].flag = flag;

        overlayIcons[index].coordinates.x = x;
        overlayIcons[index].coordinates.y = y;
        overlayIcons[index].coordinates.z = z;
        
        result = 1;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FA1C);

bool func_8003FA1C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, 
    u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, 
    void* vaddrTexture, void* vaddrTexture2, 
    void* vaddrPalette, void* vaddrAnimation, 
    void* vaddrSpriteToPalette, void* vaddrSpritesheetIndex, 
    f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0) {
        
        characterAvatars[index].romTextureStart = romTextureStart;
        characterAvatars[index].romTextureEnd = romTextureEnd;
        characterAvatars[index].romAssetsIndexStart = romAssetsIndexStart;
        characterAvatars[index].romAssetsIndexEnd = romAssetsIndexEnd;
        characterAvatars[index].romSpritesheetIndexStart = romSpritesheetIndexStart;
        characterAvatars[index].romSpritesheetIndexEnd = romSpritesheetIndexEnd;
  
        characterAvatars[index].vaddrTexture = vaddrTexture;
        characterAvatars[index].vaddrSpritesheet = vaddrTexture2;
        characterAvatars[index].vaddrPalette = vaddrPalette;
        characterAvatars[index].vaddrAnimation = vaddrAnimation;
        characterAvatars[index].vaddrSpriteToPalette = vaddrSpriteToPalette;
        characterAvatars[index].vaddrSpritesheetIndex = vaddrSpritesheetIndex;

        characterAvatars[index].spriteIndex = spriteIndex;

        characterAvatars[index].coordinates.x = x;
        characterAvatars[index].coordinates.y = y;
        characterAvatars[index].coordinates.z = z;

        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FAE8);

void func_8003FAE8(u8* arg0) {
    characterAvatarsAnimationsPtr = arg0;
}

//INCLUDE_ASM("asm/nonmatchings/system/message", setMessageBoxButtonMask);

bool setMessageBoxButtonMask(u16 index, u16 arg1) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {
        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            messageBoxes[index].buttonMask = arg1;
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FB4C);

bool func_8003FB4C(u16 index, s16 speed) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {
            messageBoxes[index].defaultScrollSpeed = speed;
            result = TRUE;
        }
    }

    return result;
    
}


//INCLUDE_ASM("asm/nonmatchings/system/message", setGameVariableString);

bool setGameVariableString(u16 index, u8* ptr, s8 length) {

    bool result = FALSE;

    if (index < MAX_GAME_VARIABLE_STRINGS) {
        
        gameVariableStrings[index].ptr = ptr;
        gameVariableStrings[index].maxLength = length;

        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", convertNumberToString);

// set number string for game variables based on their current quantity
// FIXME: fake matches, but need do {} while (0) to get registers right
bool convertNumberToString(u16 index, u32 number, u8 terminatorType) {
    
    u32 digitDivisorsBuffer[8];
    u8 digitCharacterCodesBuffer[16];
    
    u8 digitPosition;
    u8 position;
    bool startedOutputting;
    
    u32 divisor;
    u32 extractedDigit;
    u32 unprocessedDigits;
    
    memcpy(digitDivisorsBuffer, powersOfTen, 32);
    memcpy(digitCharacterCodesBuffer, digitCharacterCodes, 10);
    
    unprocessedDigits = number;
    position = 0;
    startedOutputting = 0;
    
    digitPosition = gameVariableStrings[index].maxLength - 1;
    
    do {

        divisor = digitDivisorsBuffer[digitPosition];
        extractedDigit = unprocessedDigits / divisor;
        unprocessedDigits %= divisor;
        
        // first non-zero significant digit or force output of a 0 if no value
        if (extractedDigit != 0 || digitPosition == 0) {
            startedOutputting = TRUE;
            goto writeDigitChar;
        }
        
        if (startedOutputting) goto writeDigitChar;
        
        do {
            
            if (digitPosition) goto writePaddingChar;
            
        } while (0);
        
        do {
            
            do {
writeDigitChar:
                gameVariableStrings[index].ptr[position] = digitCharacterCodesBuffer[extractedDigit];
                
            } while (0);
            
            goto advancePosition;
            
            // leading zeros
writePaddingChar:
            if (terminatorType == STANDALONE) {
                gameVariableStrings[index].ptr[position] = char_SPACE;
            } else {
                gameVariableStrings[index].ptr[position] = char_TERMINATOR;
            }

advancePosition:
            position++;

        } while (0);
        
    } while (digitPosition--);
    
    return 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FD74);

void func_8003FD74(void) {

    u16 i;

    for (i = 0; i < 256; i++) {
        textBuffer[i] = char_SPACE;
    }
    
}
 
//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FDB0);

bool func_8003FDB0(u16 index) {

    u16 i;
    u16 cellCount;
    u32 temp2;

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) {

        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) {

            cellCount = messageBoxes[index].textBoxLineCharWidth * messageBoxes[index].textBoxVisibleRows;

            if (((messageBoxes[index].flags >> 4) & 3) / 3) {
                cellCount += messageBoxes[index].textBoxLineCharWidth;
            }

            for (i = 0; i < cellCount; i++) {
                textBuffer[messageBoxes[index].totalCharactersProcessed + i] = char_SPACE;
            }
        
            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FE9C);

// overlay dialogue boxes
bool func_8003FE9C(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) { 

        if ((messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) && (messageBoxes[index].flags & DIALOGUE_BOX_INITIALIZED) && !(messageBoxes[index].flags & (0x40 | 0x80))) {

            if (messageBoxes[index].totalLinesToPrint + 1 != messageBoxes[index].scrollCount + messageBoxes[index].textBoxVisibleRows) {
 
                initializeInterpolator((Interpolator*)&messageBoxes[index].scrollInterpolator, messageBoxes[index].defaultScrollSpeed, 0);

                messageBoxes[index].flags |= 0x40;
                messageBoxes[index].flags &= ~8;

                messageBoxes[index].currentLineBeingPrinted++;
                
                // handle next line/scroll
                advanceToLine(index, messageBoxes[index].scrollCount + messageBoxes[index].textBoxVisibleRows + 1);
                
                messageBoxes[index].currentCharPtr--;
                
                result = TRUE ;
                
            }
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8003FFF4);

bool func_8003FFF4(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) { 

        if ((messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) && (messageBoxes[index].flags & DIALOGUE_BOX_INITIALIZED) && !(messageBoxes[index].flags & (0x40 | 0x80))) {

            if (messageBoxes[index].scrollCount) {

                initializeInterpolator((Interpolator*)&messageBoxes[index].scrollInterpolator, messageBoxes[index].defaultScrollSpeed, 0);

                messageBoxes[index].flags |= 0x80;
                messageBoxes[index].flags &= ~8;

                messageBoxes[index].scrollCount--;
                messageBoxes[index].currentLineBeingPrinted++;
                
                // handle next line/scroll
                advanceToLine(index, messageBoxes[index].scrollCount + messageBoxes[index].textBoxVisibleRows + 1);
                
                messageBoxes[index].currentCharPtr--;
                
                result = TRUE;
                
            }
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80040140);

bool func_80040140(u16 index) {

    bool result = FALSE;

    if (index < MAX_DIALOGUE_BOXES) { 

        if ((messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE) && (messageBoxes[index].flags & DIALOGUE_BOX_INITIALIZED)) {
            result = messageBoxes[index].totalLinesToPrint + 1 != messageBoxes[index].scrollCount + messageBoxes[index].textBoxVisibleRows;
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_800401C8);
 
u8 func_800401C8(u16 index) {

    u8 count = 0;

    if (index < MAX_DIALOGUE_BOXES) {
        if (messageBoxes[index].flags & DIALOGUE_BOX_ACTIVE && messageBoxes[index].flags & DIALOGUE_BOX_INITIALIZED) {
            count = messageBoxes[index].scrollCount != 0;
        }
    }

    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_8004022C);

// handle text auto-scroll
void func_8004022C(u16 index) {

    func_800267B4((Interpolator*)&messageBoxes[index].scrollInterpolator);

    // FIXME: probably a u16 return value
    if ((u16)getInterpolatorValue((Interpolator*)&messageBoxes[index].scrollInterpolator) >= messageBoxes[index].fontContext.characterCellHeight + messageBoxes[index].lineSpacing) {

        messageBoxes[index].flags &= ~0x40;
         
        initializeInterpolator((Interpolator*)&messageBoxes[index].scrollInterpolator, 0, 0);
 
        messageBoxes[index].scrollCount++;
        messageBoxes[index].currentLineBeingPrinted--;
        
    }
       
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80040318);

void func_80040318(u16 index) {

    func_800267B4((Interpolator*)&messageBoxes[index].scrollInterpolator);

    if ((u16)getInterpolatorValue((Interpolator*)&messageBoxes[index].scrollInterpolator) >= messageBoxes[index].fontContext.characterCellHeight + messageBoxes[index].lineSpacing) {

        messageBoxes[index].flags &= ~0x80;
        
        initializeInterpolator((Interpolator*)&messageBoxes[index].scrollInterpolator, 0, 0);

        messageBoxes[index].currentLineBeingPrinted--;
        
    }

}

static const u32 powersOfTen[8] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", powersOfTen);

static const u8 digitCharacterCodes[16] = {
    222, 213, 214, 215, 
    216, 217, 218, 219,
    220, 221, 0, 0,
    0, 0, 0, 0 
};

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", digitCharacterCodes);

//INCLUDE_ASM("asm/nonmatchings/system/message", func_800403F0);

// get total lines to print
u8 func_800403F0(u16 index, u16 textIndex) {
    
    u8 result = 0;
    
    bool done = FALSE;
    
    u8 count2 = 0;
    u8 count = 0;
    
    u8 i;
    u8 gameVariableStringIndex;
    
    messageBoxes[index].currentCompressionControlByte = 0;
    messageBoxes[index].compressionBitIndex = 0;
    messageBoxes[index].unk_A0 = 0;
    messageBoxes[index].unk_A1 = 0;
    messageBoxes[index].currentCharPtr = messageBoxes[index].textBufferBase;
    
    do {

        switch (func_80041850(index)) {

            case CHARACTER_CONTROL_LINEBREAK:

                if (messageBoxes[index].unk_A0 < count2) {
                    messageBoxes[index].unk_A0 = count2;
                }

                count2 = 0;
                result += 1;
                count++;

                break;

            case CHARACTER_CONTROL_SOFTBREAK:
                result += 1;
                count++;
                break;

            case CHARACTER_CONTROL_TEXT_END:

                if (messageBoxes[index].unk_A0 < count2) {
                    messageBoxes[index].unk_A0 = count2;
                }

                done = TRUE;
                count++;

                break;

            case CHARACTER_CONTROL_INSERT_GAME_VARIABLE:

                gameVariableStringIndex = *messageBoxes[index].currentCharPtr;
                messageBoxes[index].currentCharPtr++;
                
                if (gameVariableStrings[gameVariableStringIndex].ptr[0] != char_TERMINATOR) {
                    
                    i = 0;
                    
                    while ((gameVariableStrings[gameVariableStringIndex].ptr[i] != char_TERMINATOR) && (i < gameVariableStrings[gameVariableStringIndex].maxLength)) {
                        i++;
                        // get length of string
                        count2++;
                    }

                }

                break;

            case CHARACTER_CONTROL_WAIT:
            case CHARACTER_CONTROL_CHARACTER_AVATAR:
                messageBoxes[index].currentCharPtr++;
                break;

            default:
                count2++;
                break;
        }

    } while (!done);
    
    messageBoxes[index].unk_A1 = count;
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", advanceToLine);

// FIXME: do {} while (0) switch statement
void advanceToLine(u16 index, u8 linesToSkip) {
    
    u8 i;
    u8 position;
    u16 character;

    bool done = FALSE;

    i = 0;
    
    messageBoxes[index].currentCompressionControlByte = 0;
    messageBoxes[index].compressionBitIndex = 0;

    messageBoxes[index].currentCharPtr = messageBoxes[index].textBufferBase;
    
    if (linesToSkip != 0) {
        
        do {

            if (done) {
                
                position++;
                
                if (gameVariableStrings[messageBoxes[index].gameVariableStringIndex].ptr[position] == char_TERMINATOR) {
                    
                    while ((gameVariableStrings[messageBoxes[index].gameVariableStringIndex].ptr[position] == char_TERMINATOR) && (position < gameVariableStrings[messageBoxes[index].gameVariableStringIndex].maxLength)) {
                        position++;
                    }

                }
                
                character = char_SPACE;
                
                if (position == gameVariableStrings[messageBoxes[index].gameVariableStringIndex].maxLength) {
                    done = FALSE;
                }
                
            } else {
                // loop back to here until done
                character = func_80041850(index);
            }
            
            // handle control character
            switch (character) {
                
                do {

                    case CHARACTER_CONTROL_TEXT_END:
                        i = linesToSkip;
                        break;
                    case CHARACTER_CONTROL_LINEBREAK:
                    case CHARACTER_CONTROL_SOFTBREAK:
                        i++;
                        break;
                    case CHARACTER_CONTROL_INSERT_GAME_VARIABLE:
                        position = 0;
                        done = TRUE;
                        messageBoxes[index].currentCharPtr++;
                        break;
                    case CHARACTER_CONTROL_WAIT:
                    case CHARACTER_CONTROL_CHARACTER_AVATAR:
                        messageBoxes[index].currentCharPtr++;
                        do {} while (0);
                        break;

                } while (0);
            
                default:
                    break;
            }

        } while (i < linesToSkip);

    }

}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80040858);

void func_80040858(u16 index) {
    
    u8* originalCharPtr;
    
    u8 characterPositionOnLine;
    u8 currentLine;
    
    u8 gameVariableStringIndex;
    u8 gameVariableStringPosition;
    u8 gameVariableStringLength;
    bool processingGameVariableString;
    
    u16 character;
    u8 renderMode;
    u16 bufferPosition;
    
    switch ((messageBoxes[index].flags >> 4) & 3) {
            
        case 0:
            renderMode = 0;
            break;
        case 1:
            renderMode = 1;
            break;
        case 2:
            renderMode = 2;
            break;
        case 3:
            renderMode = 3;
            break;

    }

    originalCharPtr = messageBoxes[index].currentCharPtr;
    
    characterPositionOnLine = 0;
    
    advanceToLine(index, messageBoxes[index].scrollCount);

    currentLine = 0;
    processingGameVariableString = FALSE;

    while (messageBoxes[index].currentCharPtr < originalCharPtr || processingGameVariableString) {
        
        if (processingGameVariableString) {

            character = gameVariableStrings[gameVariableStringIndex].ptr[gameVariableStringPosition];
            gameVariableStringPosition++;
            
            while (gameVariableStrings[gameVariableStringIndex].ptr[gameVariableStringPosition] == char_TERMINATOR && 
                   gameVariableStringPosition < gameVariableStrings[gameVariableStringIndex].maxLength) {
                gameVariableStringPosition++;
            }
            
            if (character != char_TERMINATOR) {
                character += 0xB;
            } else {
                character = 0xFFFF;
            }
            
            if (gameVariableStringPosition == gameVariableStringLength) {
                processingGameVariableString = FALSE;
            }
            
        } else {
            character = func_80041850(index);
        }

        switch (character) {

            case CHARACTER_CONTROL_LINEBREAK:
                characterPositionOnLine = 0;
                currentLine++;
                break;
            
            // clear and start new line (overwrite text box)
            case CHARACTER_CONTROL_SOFTBREAK:

                func_8003FDB0(index);
                characterPositionOnLine = 0;
                currentLine = 0;
                break;
                    
            case CHARACTER_CONTROL_TEXT_END:

                if (renderMode == 3) {
                    
                    messageBoxes[index].currentCharPtr--;
                    
                } else if (!(messageBoxes[index].flags & 0x4000) || 
                          (messageBoxes[index].savedCharPtr != messageBoxes[index].currentCharPtr)) {
                    
                    characterPositionOnLine = 0;
                    currentLine++;
                    
                }
                
                break;

            case CHARACTER_CONTROL_WAIT:
                messageBoxes[index].currentCharPtr++;
                break;

            // set up game variable printing
            case CHARACTER_CONTROL_INSERT_GAME_VARIABLE:
                
                if (messageBoxes[index].gameVariableStringPtr == messageBoxes[index].currentCharPtr) {
                    
                    messageBoxes[index].currentCharPtr++;
                    gameVariableStringIndex = messageBoxes[index].gameVariableStringIndex;
                    gameVariableStringLength = messageBoxes[index].gameVariableStringLength;
                    processingGameVariableString = 2;
                    
                } else {
                    
                    gameVariableStringIndex = *messageBoxes[index].currentCharPtr;
                    messageBoxes[index].currentCharPtr++;
                    gameVariableStringLength = gameVariableStrings[gameVariableStringIndex].maxLength;
                    processingGameVariableString = TRUE;
                }

                gameVariableStringPosition = 0;
                
                break;

            case CHARACTER_CONTROL_CHARACTER_AVATAR:
                messageBoxes[index].currentCharPtr++;
                break;
            
            // update text buffer, converting from ROM char to font index 
            default:
                textBuffer[messageBoxes[index].totalCharactersProcessed + 
                                   (currentLine * messageBoxes[index].textBoxLineCharWidth) + 
                                   characterPositionOnLine] = character - 0xB;
                
                characterPositionOnLine++;

                break;

            case CHARACTER_CONTROL_WAIT_WITH_ICON:
            case 0xFFFF:
                break;

        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80040C38);

void func_80040C38(u16 index) {
    
    Swap16 swap;

    u8 temp;
    u8 temp2;

    u16 character;

    u8 animationIndex;

    u8 renderMode;
    u8 currentLine;
        
    bool continueProcessing;
        
    switch ((messageBoxes[index].flags >> 4) & 3) {
                
        case 0:
            renderMode = 0;
            break;
        case 1:
            renderMode = 1;
            break;
        case 2:
            renderMode = 2;
            break;
        case 3:
            renderMode = 3;
            break;
                
    }

    temp = (((messageBoxes[index].flags >> 4) & 3) / 3) ^ 1;
    
    continueProcessing = FALSE;
    
    while (!continueProcessing) {
        
        if (messageBoxes[index].flags & 0x100) {
            
            character = func_800419C4(index);
            
            if (character == 0xFF) {
                character = 0xFFFF;
            } else {
                character += 0xB;
            }
            
        } else if ((messageBoxes[index].flags & 0x1000) || (messageBoxes[index].flags & 0x4000)) {
            
            if (messageBoxes[index].flags & 0x1000) {
                character = 4;
            } else {
                character = 2;
            }
            
        } else {
            character = func_80041850(index);
        }
        
        switch (character) {
            
            case CHARACTER_CONTROL_LINEBREAK:
                
                currentLine = messageBoxes[index].currentLineBeingPrinted;
                
                // Check if need to scroll
                if (currentLine >= (messageBoxes[index].textBoxVisibleRows - temp)) {
                    
                    if (renderMode != 3) {
                                
                        if (checkButtonHeld(CONTROLLER_1, messageBoxes[index].buttonMask) || (renderMode == 2)) {
                            // fast scroll if button held
                            initializeInterpolator(&messageBoxes[index].scrollInterpolator, 4, 0);
                        } else {
                            // default scroll speed
                            initializeInterpolator(&messageBoxes[index].scrollInterpolator, messageBoxes[index].defaultScrollSpeed, 0);
                        }
                        
                        // scrolling flag
                        messageBoxes[index].flags |= 0x40;
                        messageBoxes[index].currentLineBeingPrinted++;
                                
                    } else {
                        messageBoxes[index].flags |= 8;
                    }
                        
                    temp2 = renderMode;
                    temp2 -= 2;
                    
                    if (temp2 < 2) {
                        continueProcessing = TRUE;
                    }
                        
                } else {
                    messageBoxes[index].currentLineBeingPrinted++;
                }   
            
                messageBoxes[index].currentCharCountOnLine = 0;
                
                if (renderMode < 2) {
                    continueProcessing = TRUE;
                }
                
                break;
                    
            case CHARACTER_CONTROL_SOFTBREAK:
                    
                func_8003FDB0(index);
                
                messageBoxes[index].currentCharCountOnLine = 0;
                messageBoxes[index].scrollCount += messageBoxes[index].currentLineBeingPrinted + 1;
                messageBoxes[index].currentLineBeingPrinted = 0;
                
                continueProcessing = TRUE;
                
                break;
                    
            case CHARACTER_CONTROL_TEXT_END:
                
                if (renderMode == 3) {
                    
                    messageBoxes[index].flags |= 8;
                
                } else {
                
                    messageBoxes[index].flags |= 0x4000;
                    messageBoxes[index].savedCharPtr = messageBoxes[index].currentCharPtr;
                    
                    if ((messageBoxes[index].flags & 0x400) && !(messageBoxes[index].flags & 0x80000)) {
                        
                        startSpriteAnimation(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex, 
                                overlayIcons[messageBoxes[index].overlayIconIndex + 1].spriteOffset, 
                                overlayIcons[messageBoxes[index].overlayIconIndex + 1].flag
                        );
                        
                        setSpriteViewSpacePosition(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex, 
                        messageBoxes[index].viewSpacePosition.x + overlayIcons[messageBoxes[index].overlayIconIndex].coordinates.x,
                            messageBoxes[index].viewSpacePosition.y + overlayIcons[messageBoxes[index].overlayIconIndex].coordinates.y,
                            messageBoxes[index].viewSpacePosition.z + overlayIcons[messageBoxes[index].overlayIconIndex].coordinates.z + 1.0f);
                            
                    }
                    
                    if ((messageBoxes[index].flags & 0x800) || (messageBoxes[index].flags & 0x100000)) {
                        
                        __asm__ __volatile__("" : : : "memory");
                        
                        messageBoxes[index].flags &= ~0x4000;
                        messageBoxes[index].flags |= 4;
                        
                        if (!(messageBoxes[index].flags & 0x40000)) {
                            func_8003F130(index);
                        }
                        
                        resetAnimationState(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex);
                        
                        // play close sound effect
                        if (!(messageBoxes[index].flags & 0x100000) && (messageBoxes[index].characterPrintSfx != 0xFF)) {
                            setSfx(messageBoxes[index].messageCloseSfx + 1);
                            setSfxVolume(messageBoxes[index].messageCloseSfx + 1, 0x80);
                        }
                        
                    }
                }
                
                messageBoxes[index].currentCharCountOnLine = 0;
                continueProcessing = TRUE;
                
                break;
                
            case CHARACTER_CONTROL_WAIT:
                
                initializeInterpolator(&messageBoxes[index].unk_64, -(s16)*messageBoxes[index].currentCharPtr, 0);
                
                messageBoxes[index].currentCharPtr++;
                messageBoxes[index].flags |= 0x2000;
                continueProcessing = TRUE;
                
                break;
                
            case CHARACTER_CONTROL_WAIT_WITH_ICON:
                
                messageBoxes[index].flags |= 0x1000;
                messageBoxes[index].savedCharPtr = messageBoxes[index].currentCharPtr;
                
                if (messageBoxes[index].flags & 0x400) {
                    
                    startSpriteAnimation(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex, overlayIcons[messageBoxes[index].overlayIconIndex].spriteOffset, overlayIcons[messageBoxes[index].overlayIconIndex].flag);
                    setSpriteViewSpacePosition(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex,
                    messageBoxes[index].viewSpacePosition.x + overlayIcons[messageBoxes[index].overlayIconIndex].coordinates.x,
                    messageBoxes[index].viewSpacePosition.y + overlayIcons[messageBoxes[index].overlayIconIndex].coordinates.y,
                    messageBoxes[index].viewSpacePosition.z + overlayIcons[messageBoxes[index].overlayIconIndex].coordinates.z + 1.0f);
                
                }
                
                if (messageBoxes[index].flags & 0x800) {
                    
                    messageBoxes[index].flags &= ~0x800;
                    messageBoxes[index].flags &= ~0x1000;
                    
                    resetAnimationState(overlayIcons[messageBoxes[index].overlayIconIndex].spriteIndex);
                    
                    if (messageBoxes[index].characterPrintSfx != 0xFF) {
                        setSfx(messageBoxes[index].unk_74 + 1);
                        setSfxVolume(messageBoxes[index].unk_74 + 1, 0x80);
                    }
                    
                } else {
                    continueProcessing = TRUE;
                }
                
                break;
                
            case CHARACTER_CONTROL_LOAD_TEXT:
                
                // Read dialogue index (2 bytes, little endian)
                swap.byte[1] = *messageBoxes[index].currentCharPtr;
                messageBoxes[index].currentCharPtr++;
                swap.byte[0] = *messageBoxes[index].currentCharPtr;
                messageBoxes[index].currentCharPtr++;
                
                messageBoxes[index].textIndex = swap.halfword;
                
                // Reset text state
                messageBoxes[index].currentCompressionControlByte = 0;
                messageBoxes[index].compressionBitIndex = 0;
                messageBoxes[index].currentCharCountOnLine = 0;
                messageBoxes[index].currentLineBeingPrinted = 0;
                messageBoxes[index].scrollCount = 0;
                messageBoxes[index].totalLinesToPrint = 0;
                
                messageBoxes[index].currentCharPtr = messageBoxes[index].textBufferBase;
                
                break;
                
            case CHARACTER_CONTROL_UNUSED:
                break;
            
            case CHARACTER_CONTROL_INSERT_GAME_VARIABLE:

                messageBoxes[index].gameVariableStringPtr = messageBoxes[index].currentCharPtr;
                
                swap.byte[0] = *messageBoxes[index].currentCharPtr;
                
                messageBoxes[index].currentCharPtr++;
                
                messageBoxes[index].gameVariableStringIndex = swap.byte[0];
                
                messageBoxes[index].gameVariableStringLength = 0;
                
                messageBoxes[index].flags |= 0x100;
                
                break;
                
            case CHARACTER_CONTROL_WAIT_ALTERNATE: 
            
                messageBoxes[index].flags |= 0x4000;
                messageBoxes[index].savedCharPtr = messageBoxes[index].currentCharPtr;
                
                if (messageBoxes[index].flags & 0x800) {
                    messageBoxes[index].flags = (messageBoxes[index].flags & ~0x4000) | 4;
                } else {
                    continueProcessing = TRUE;
                }
            
                break;
                
            case CHARACTER_CONTROL_CHARACTER_AVATAR:
                
                animationIndex = *messageBoxes[index].currentCharPtr;
                messageBoxes[index].currentCharPtr++;
                
                resetAnimationState(characterAvatars[messageBoxes[index].characterAvatarIndex].spriteIndex);
                startSpriteAnimation(characterAvatars[messageBoxes[index].characterAvatarIndex].spriteIndex, characterAvatarsAnimationsPtr[animationIndex], 0);
                
                setSpriteViewSpacePosition(characterAvatars[messageBoxes[index].characterAvatarIndex].spriteIndex,
                    messageBoxes[index].viewSpacePosition.x + characterAvatars[messageBoxes[index].characterAvatarIndex].coordinates.x,
                    messageBoxes[index].viewSpacePosition.y + characterAvatars[messageBoxes[index].characterAvatarIndex].coordinates.y,
                    messageBoxes[index].viewSpacePosition.z + characterAvatars[messageBoxes[index].characterAvatarIndex].coordinates.z - 1.0f);
                    
                break;
                
            case 0xFFFF:
                break;

            case 0xA:
                break;
                
            default:
                
                textBuffer[messageBoxes[index].totalCharactersProcessed + 
                    (messageBoxes[index].currentLineBeingPrinted * messageBoxes[index].textBoxLineCharWidth) + 
                    messageBoxes[index].currentCharCountOnLine] = character - 0xB;
                
                messageBoxes[index].currentCharCountOnLine++;
                
                if (renderMode != 0) break;
                
                if (messageBoxes[index].characterPrintSfx != 0xFF) {
                    
                    // skip space characters
                    if ((character - 0xB) != 0xEE) {
                            
                        setSfx(messageBoxes[index].characterPrintSfx + 1);
                        setSfxVolume(messageBoxes[index].characterPrintSfx + 1, 0x80);
                        
                    }
            
                }
        
                continueProcessing = TRUE;
            
                break;
                
        }
            
    } 
    
    func_800267B4((Interpolator*)&messageBoxes[index].unk_64);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80041850);

u16 func_80041850(u16 index) {
    
    u8 buffer[2];
    u32 padding[4];

    u16 character;
    
    u8 lower;
    u32 higher;
    
    memcpy(buffer, controlByteBitMasks, 8);

    // every 8 characters, store control byte (which determines how to read 8 character stream)
    if ((messageBoxes[index].compressionBitIndex % 8) == 0) {
        messageBoxes[index].currentCompressionControlByte = *messageBoxes[index].currentCharPtr;        
        messageBoxes[index].currentCharPtr++;
    }
    
    // get two-byte value if needed
    if (messageBoxes[index].currentCompressionControlByte & buffer[messageBoxes[index].compressionBitIndex % 8]) {
        
        lower = *messageBoxes[index].currentCharPtr;
        messageBoxes[index].currentCharPtr++;
        
        higher = *messageBoxes[index].currentCharPtr;
        messageBoxes[index].currentCharPtr++;
        
        character = (lower | (higher << 8));
        
    } else {
        character = *messageBoxes[index].currentCharPtr; 
        messageBoxes[index].currentCharPtr++;
    }

    // only reset on new text
    messageBoxes[index].compressionBitIndex++;

    return character;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_800419C4);

u8 func_800419C4(u16 index) {

    u8 result;

    result = gameVariableStrings[messageBoxes[index].gameVariableStringIndex].ptr[messageBoxes[index].gameVariableStringLength];
    
    messageBoxes[index].gameVariableStringLength++;
    
    while (gameVariableStrings[messageBoxes[index].gameVariableStringIndex].ptr[messageBoxes[index].gameVariableStringLength] == char_TERMINATOR 
        && messageBoxes[index].gameVariableStringLength < gameVariableStrings[messageBoxes[index].gameVariableStringIndex].maxLength) {
       
        messageBoxes[index].gameVariableStringLength++;
    
    } 

    if (messageBoxes[index].gameVariableStringLength == gameVariableStrings[messageBoxes[index].gameVariableStringIndex].maxLength) {
        messageBoxes[index].flags &= ~0x100;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", getTextAddress);

// returns rom address for text
u32 getTextAddress(u16 index, u16 textIndex) {

    u32 result = textAddresses[messageBoxes[index].textAddressesIndex].romTextStart;

    result += textAddresses[messageBoxes[index].textAddressesIndex].indexVaddr[textIndex];

    return result;
    
}

static inline void swapTop(u8* source, u8* dest) {

    u8 temp;
    
    // 0xC0 = 1100 0000
    // 0x30 = 0011 0000

    // 1100 0000 --> 0011 0000
    temp = ((*source) & 0xC0) >> 2;
    *dest = temp;
    // 0011 0000 --> 0000 0011
    *dest |= ((*source) & 0x30) >> 4;
    
}

static inline void swapBottom(u8* source, u8* dest) {

    u8 temp;

    // 0xC = 1100
    // 0x3 = 0011

    // 1100 --> 0011 0000
    temp = ((*source) & 0xC) << 2;
    *dest = temp;
    *dest |= *source & 3;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", unpackFontCI2Data);

// outputs CI2 data (pixel values range from 0-3, corresponding to one of four palettes)
void unpackFontCI2Data(u16 characterIndex, MessageBoxFont* fontContext, u8 outputBuffer[]) {

    u8 i;

    u8 *source;
    u8 *out;

    source = fontContext->compressedCI2FontData + 3;

    source = &source[characterIndex * 0x40];
    
    out = outputBuffer; 
    
    i = 0;
    
    if (fontContext->characterCellWidth) {

        // process 4 bytes, output 8
        do {

            // byte 1
            swapTop(source, out);
            swapBottom(source, ++out);
            source--;

            // byte 2
            swapTop(source, ++out);
            swapBottom(source, ++out);
            source--;

            // byte 3
            swapTop(source, ++out);
            swapBottom(source, ++out);
            source--;

            // byte 4; only process top nibble
            swapTop(source, ++out);
            
            // skip last nibble
            source += 7;
            out++;
            
            i++;
            
        } while (i < fontContext->characterCellWidth);

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80041CD8);

Gfx* func_80041CD8(Gfx* dl, MessageBox* messageBox) {

    u16 ulx, uly, lrx, lry;
    u16 temp;
    
    Gfx tempDl;

    *dl++ = D_8011EE98;
    *dl++ = D_8011EEA0;

    temp = (messageBox->fontContext.characterCellWidth / 2);

    ulx = messageBox->viewSpacePosition.x + 160.0f - temp - ((messageBox->textBoxLineCharWidth * messageBox->fontContext.characterCellWidth) / 2) + temp - ((messageBox->textBoxLineCharWidth * messageBox->characterSpacing) / 2);

    temp = (messageBox->fontContext.characterCellHeight / 2);

    uly = 120.0f - messageBox->viewSpacePosition.y - temp - (messageBox->textBoxVisibleRows * messageBox->fontContext.characterCellHeight / 2) + temp - (messageBox->textBoxVisibleRows * messageBox->lineSpacing / 2);
    
    lrx = ulx  + (messageBox->textBoxLineCharWidth * messageBox->fontContext.characterCellWidth) 
        + (messageBox->textBoxLineCharWidth * messageBox->characterSpacing);

    lry = uly + (messageBox->textBoxVisibleRows * messageBox->fontContext.characterCellHeight) 
        + (messageBox->textBoxVisibleRows * messageBox->lineSpacing);

    gDPSetScissor(&tempDl, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    
    *dl++ = tempDl;

    *dl++ = D_8011EEA8;

    return dl++;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", func_80042014);

Gfx* func_80042014(Gfx* dl, MessageBox* messageBox, u8 lineNumber, s32 arg3) {

    FontBitmap sprite;
    
    Gfx tempDl[2];
    
    u16 charOffset;

    u32 flags;

    u16 characterCount = 0;
    u8 i = 0;
    
    u32 i2;
    
    while (i < messageBox->textBoxLineCharWidth) {

        // FIXME: seems fake
        i2 = i; 
        charOffset = i2 + (lineNumber * messageBox->textBoxLineCharWidth);
        
        unpackFontCI2Data(
            textBuffer[messageBox->totalCharactersProcessed + charOffset], 
            &messageBox->fontContext.compressedCI2FontData, 
            fontTexturesBuffer[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset].timg
        );
        
        if (messageBox->flags & 2) {

            flags = (0x10 | 0x40 | 0x100 | 0x400);

            setupBitmapVertices(
                &fontVertices[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset][0],
                messageBox->fontContext.characterCellWidth,
                messageBox->fontContext.characterCellHeight,
                messageBox->fontContext.characterCellHeight,
                0,
                0,
                0,
                ((messageBox->fontContext.characterCellWidth + messageBox->characterSpacing)*i),
                0,
                flags,
                messageBox->unk_14.r,
                messageBox->unk_14.g,
                messageBox->unk_14.b,
                messageBox->unk_14.a
            );

            sprite.timg = fontTexturesBuffer[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset].timg;

            setBitmapFormatAndSize((BitmapObject*)&sprite, messageBox->fontContext.fontPalettePtr);

            sprite.width = messageBox->fontContext.characterCellWidth;
            sprite.height = messageBox->fontContext.characterCellHeight;
            sprite.fmt = 2; // CI2
            sprite.pixelSize = 0;

            dl = loadBitmapTexture(dl, (BitmapObject*)&sprite, 0, sprite.height);
            
            // FIXME: likely wrapper around gSPVertex
            gSPVertex(&tempDl[1], &fontVertices[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset][0], 4, 0);

            *tempDl = *(tempDl+1);
            *dl++ = *tempDl;
            
            // set triangle
            *dl++ = D_8011EEB0;
            // pipe sync
            *dl++ = D_8011EEB8;
            
        }

        characterCount++;
        i++;

    }

    // pipe sync
    *dl++ = D_8011EEB8;
    // end display list
    *dl++ = D_8011EEC0;

    messageBox->charactersProcessedPerLine += characterCount;
    
    return dl++;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", addMessageCharSceneNode);

void addMessageCharSceneNode(MessageBox* messageBox, u8 line, Gfx* dl) {

    Vec3f vec;

    f32 f1, f2;
    u16 sceneNodeIndex;
    
    if (messageBox->flags & 0x80) {
        f1 = messageBox->fontContext.characterCellHeight + messageBox->lineSpacing;
    } else {
        f1 = 0.0f;
    }

    if (messageBox->flags & 0x80) {
        f2 = -(f32)getInterpolatorValue((Interpolator*)&messageBox->scrollInterpolator);
    } else {
        f2 = getInterpolatorValue((Interpolator*)&messageBox->scrollInterpolator);
    }
    
    sceneNodeIndex = addSceneNode(dl, (8 | 0x40));
    
    vec.x = messageBox->viewSpacePosition.x 
        - ((messageBox->textBoxLineCharWidth * messageBox->fontContext.characterCellWidth) / 2) 
        + (messageBox->fontContext.characterCellWidth / 2) 
        - ((messageBox->textBoxLineCharWidth * messageBox->characterSpacing) / 2);

    vec.y = messageBox->viewSpacePosition.y + f2 + f1
        + (-line * messageBox->fontContext.characterCellHeight)
        + (-line * messageBox->lineSpacing) 
        + ((messageBox->textBoxVisibleRows * messageBox->fontContext.characterCellHeight) / 2)
        - (messageBox->fontContext.characterCellHeight / 2)
        + ((messageBox->textBoxVisibleRows * messageBox->lineSpacing) / 2);

    vec.z = messageBox->viewSpacePosition.z + 16.0f;

    addSceneNodePosition(sceneNodeIndex, vec.x, vec.y, vec.z);
    
}

static const u8 controlByteBitMasks[8] = { 0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1 };

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", controlByteBitMasks);

static const Gfx D_8011EE98 = gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", D_8011EE98);

static const Gfx D_8011EEA0 = gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", D_8011EEA0);

static const Gfx D_8011EEA8 = gsDPSetTextureFilter(G_TF_BILERP);

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", D_8011EEA8);

static const Gfx D_8011EEB0 = gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0);

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", D_8011EEB0);

static const Gfx D_8011EEB8 = gsDPPipeSync();

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", D_8011EEB8);

static const Gfx D_8011EEC0 = gsSPEndDisplayList();

//INCLUDE_RODATA("asm/nonmatchings/systemmessage", D_8011EEC0);

static inline u8 updateMessageBoxRGBA(u16 i) {
    
    // bug: never initialized
    u8 count;

    if (messageBoxes[i].unk_14.r < messageBoxes[i].unk_24.r) {

        messageBoxes[i].unk_14.r += messageBoxes[i].unk_34.r;
        
            if (messageBoxes[i].unk_14.r >= messageBoxes[i].unk_24.r) {
                messageBoxes[i].unk_14.r = messageBoxes[i].unk_24.r;
            } else {
                count++;
        }

    }

    if (messageBoxes[i].unk_14.r > messageBoxes[i].unk_24.r) {
        
        messageBoxes[i].unk_14.r -= messageBoxes[i].unk_34.r;

        if (messageBoxes[i].unk_14.r <= messageBoxes[i].unk_24.r) {
                messageBoxes[i].unk_14.r = messageBoxes[i].unk_24.r;
            } else {
                count++;
            }
            
    }
    
    if (messageBoxes[i].unk_14.g < messageBoxes[i].unk_24.g) {
        
        messageBoxes[i].unk_14.g += messageBoxes[i].unk_34.g;
        
        if (messageBoxes[i].unk_14.g >= messageBoxes[i].unk_24.g) {
            messageBoxes[i].unk_14.g = messageBoxes[i].unk_24.g;
            } else {
                count++;
            }
            
    }

    if (messageBoxes[i].unk_14.g > messageBoxes[i].unk_24.g) {
        
        messageBoxes[i].unk_14.g -= messageBoxes[i].unk_34.g;
        
        if (messageBoxes[i].unk_14.g <= messageBoxes[i].unk_24.g) {
            messageBoxes[i].unk_14.g = messageBoxes[i].unk_24.g;
        } else {
            count++;
        }
        
    }

    if (messageBoxes[i].unk_14.b < messageBoxes[i].unk_24.b) {
        
        messageBoxes[i].unk_14.b += messageBoxes[i].unk_34.b;
        
        if (messageBoxes[i].unk_14.b >= messageBoxes[i].unk_24.b) {
            messageBoxes[i].unk_14.b = messageBoxes[i].unk_24.b;
        } else {
            count++;
        }
    
    }

    if (messageBoxes[i].unk_14.b > messageBoxes[i].unk_24.b) {
        
        messageBoxes[i].unk_14.b -= messageBoxes[i].unk_34.b;
        
        if (messageBoxes[i].unk_14.b <= messageBoxes[i].unk_24.b) {
            messageBoxes[i].unk_14.b = messageBoxes[i].unk_24.b;
        } else {
            count++;
        }
        
    }
    
    if (messageBoxes[i].unk_14.a < messageBoxes[i].unk_24.a) {

    messageBoxes[i].unk_14.a += messageBoxes[i].unk_34.a;

        if (messageBoxes[i].unk_14.a >= messageBoxes[i].unk_24.a) {
            messageBoxes[i].unk_14.a = messageBoxes[i].unk_24.a;
        } else {
            count++;
        }
        
    }
    
    if (messageBoxes[i].unk_14.a > messageBoxes[i].unk_24.a) {
        
        messageBoxes[i].unk_14.a -= messageBoxes[i].unk_34.a;

        if (messageBoxes[i].unk_14.a <= messageBoxes[i].unk_24.a) {
            messageBoxes[i].unk_14.a = messageBoxes[i].unk_24.a;
        } else {
            count++;
    }
    
    }
    
    return count;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/message", updateMessageBox);

void updateMessageBox(void) {
    
    u16 i = 0;
    u16 j;
    u8 k;

    u8 extraLines;
    
    Gfx* dl;
    Gfx* dlStartingPosition;
    
    s16 temp;
    u16 totalProcessedCharacterCount;

    dl = messageBoxDisplayList[gGraphicsBufferIndex];

    totalProcessedCharacterCount = 0;
    
    for (i = 0; i < MAX_DIALOGUE_BOXES; i++) {

        if (messageBoxes[i].flags & DIALOGUE_BOX_ACTIVE) {

            if (updateMessageBoxRGBA(i) == 0) {
                messageBoxes[i].flags |= 0x200000;
            } 

            messageBoxes[i].totalCharactersProcessed = totalProcessedCharacterCount;

            if (!(messageBoxes[i].flags & 4) && (messageBoxes[i].flags & 2)) {

                if (!(messageBoxes[i].flags & 8)) {

                    // reset message buffer
                    func_8003FDB0(i);

                    // handle scroll
                    if (messageBoxes[i].flags & 0x40) {
                        func_8004022C(i);
                    }                    

                    // handle scroll
                    if (messageBoxes[i].flags & 0x80) {
                        func_80040318(i);
                    }    

                    func_80040858(i);

                    if (!(messageBoxes[i].flags & (0x40 | 0x80))) {

                        temp = func_800267B4((Interpolator*)&messageBoxes[i].unk_64);
                        j = 0;
                        
                        if (checkButtonPressed(CONTROLLER_1, messageBoxes[i].buttonMask)) {

                            if (!(messageBoxes[i].flags & 0x80000)) {
                                messageBoxes[i].flags |= 0x800;
                            }
                            
                        }

                        if (checkButtonHeld(CONTROLLER_1, messageBoxes[i].buttonMask) && !(messageBoxes[i].flags & 0x100000)) {
                            temp = 1;
                        }

                        while (j < temp) {

                            if (messageBoxes[i].flags & 0x2000) {
                                initializeInterpolator((Interpolator*)&messageBoxes[i].unk_64, messageBoxes[i].unk_7C, 0);
                                messageBoxes[i].flags &= ~0x2000;    
                            }

                            func_80040C38(i);
                            messageBoxes[i].flags &= ~0x800;    
                            j++;
                            
                        }
                        
                    }
                    
                
                }
                
            }

            messageBoxes[i].charactersProcessedPerLine = 0;

            // divide by max lines per text box
            extraLines = ((messageBoxes[i].flags >> 4) & 3) / 3;

            dlStartingPosition = dl;
            dl = func_80041CD8(dl, &messageBoxes[i]);

            k = 0;
            
            if (messageBoxes[i].textBoxVisibleRows + extraLines) {

                do {

                    dl = func_80042014(dl, &messageBoxes[i], k, totalProcessedCharacterCount);
                    addMessageCharSceneNode(&messageBoxes[i], k, dlStartingPosition);
                    
                    dlStartingPosition = dl;
                    k++;
                    
                } while (k < messageBoxes[i].textBoxVisibleRows + extraLines);
                
            }

            // updated by func_80042014
            totalProcessedCharacterCount += messageBoxes[i].charactersProcessedPerLine;
            
        }
        
    }

    if (dl - messageBoxDisplayList[gGraphicsBufferIndex] >= 3072) {
        __assert("EX", "s:/system/message.c", 2524);
    }
    
}