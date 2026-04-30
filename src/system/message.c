#include "common.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/graphic.h"
#include "system/message.h"
#include "system/globalSprites.h"
#include "system/interpolation.h"
#include "system/math.h"
#include "system/sceneGraph.h"
#include "system/sprite.h"

#include "buffers/buffers.h"

#include "mainproc.h"

#include "ld_symbols.h"

// bss
MessageBox messageBoxes[MAX_MESSAGE_BOXES];
GameVariableString gameVariableStrings[64];
TextAddresses textAddresses[TOTAL_TEXTS];

// text buffer of current displayed lines in game string characters; each line is size 0x20 (0x10 u16s)
// u16 linesBuffer[8][0x10]
// always 16 chars per line (filled with padding) 
u16 textBuffer[256];

FontData fontTexturesBuffer[2][256];
Vtx fontVertices[2][512][4];
Gfx messageBoxDisplayList[2][3072] __attribute__((aligned(16)));

u8* characterAvatarsAnimationsPtr;

CharacterAvatar characterAvatars[1];
DialogueWindow dialogueWindows[3];
OverlayIcon overlayIcons[2];

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
bool setMessageBoxBaseRGBA(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
bool setMessageBoxRGBA(u16 index, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
void clearTextBuffer(void);
bool clearMessageBoxTextBuffer(u16 index);
void updateScrollDownAnimation(u16 index);
u8 countTextLines(u16 index, u16 arg1);
void advanceToLine(u16, u8); 
void updateScrollUpAnimation(u16 index);
void processText(u16);                               
void updateMessageBoxText(u16);                               
u16 readCompressedCharacter(u16 index);
u8 readGameVariableChar(u16 index);
Gfx* setupMessageBoxScissor(Gfx* dl, MessageBox* messageBox);
Gfx* renderMessageBoxLine(Gfx* dl, MessageBox* messageBox, u8 arg2, s32 arg3); 
void addMessageCharSceneNode(MessageBox* messageBox, u8 line, Gfx* dl);    
u32 getTextAddress(u16 index, u16 offset);
void unpackFontCI2Data(u16, MessageBoxFont*, u8*);

void initializeMessageBoxes(void) {

    u16 i = 0;

    for (i = 0; i < MAX_MESSAGE_BOXES; i++) {

        MessageBox *mb = &messageBoxes[i];

        mb->flags = 0;
        
        mb->textIndex = 0;
        
        mb->currentCompressionControlByte = 0;
        mb->compressionBitIndex = 0;
         
        mb->unk_7C = 0;

        mb->scrollCount = 0;
        mb->totalLinesToPrint = 0;
        mb->currentCharCountOnLine = 0;
        mb->currentLineBeingPrinted = 0;

        mb->characterSpacing = 0;
        mb->lineSpacing = 0;

        mb->characterPrintSfx = 0xFF;
        mb->unk_74 = 0xFF;
        mb->messageCloseSfx = 0xFF;

        mb->totalCharactersProcessed = 0;
        mb->charactersProcessedPerLine = 0;
        mb->defaultScrollSpeed = 0;
        mb->buttonMask = 0;
        mb->textAddressesIndex = 0;
 
        mb->fontContext.characterCellWidth = 0;
        mb->fontContext.characterCellHeight = 0;

        mb->viewSpacePosition.x = 0;
        mb->viewSpacePosition.y = 0;
        mb->viewSpacePosition.z = 0;

        mb->textBoxLineCharWidth = 0;
        mb->textBoxVisibleRows = 0;

        mb->targetRGBA.r = 255.0f;
        mb->targetRGBA.g = 255.0f;
        mb->targetRGBA.b = 255.0f;
        mb->targetRGBA.a = 255.0f;

        mb->currentRGBA.r = 255.0f;
        mb->currentRGBA.g = 255.0f;
        mb->currentRGBA.b = 255.0f;
        mb->currentRGBA.a = 255.0f;
       
        initializeInterpolator((Interpolator*)&mb->unk_64, 0, 0);
        initializeInterpolator((Interpolator*)&mb->scrollInterpolator, 0, 0);
         
        mb->unk_9D = 0;
        
    }

    for (i = 0; i < 3; i++) {
        
    }

    clearTextBuffer();
    
}

bool initializeEmptyMessageBox(u16 messageBoxIndex, u8* textBufferAddr) {

    MessageBox *mb = &messageBoxes[messageBoxIndex];
 
    bool set = FALSE;
    
    if (messageBoxIndex < MAX_MESSAGE_BOXES) {

        if (!(mb->flags & MESSAGE_BOX_ACTIVE)) {

            mb->currentCompressionControlByte = 0;
            mb->compressionBitIndex = 0;
 
            mb->scrollCount = 0;
            mb->totalLinesToPrint = 0;
            mb->currentCharCountOnLine = 0;
            mb->currentLineBeingPrinted = 0;

            mb->characterPrintSfx = 0xFF;
            mb->unk_74 = 0xFF;
            mb->messageCloseSfx = 0xFF;

            mb->textBufferBase = textBufferAddr;
            
            mb->flags = MESSAGE_BOX_ACTIVE;

            initializeInterpolator((Interpolator*)&mb->scrollInterpolator, 0, 0);
            setMessageBoxBaseRGBA(messageBoxIndex, 255, 255, 255, 255);
            
            set = TRUE;        

        }
        
    }
    
    return set;
    
}

bool deactivateMessageBox(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            
            mb->flags &= ~MESSAGE_BOX_ACTIVE;
            
            if (mb->flags & MESSAGE_BOX_HAS_DIALOGUE_WINDOW) {
                resetAnimationState(dialogueWindows[mb->dialogueWindowIndex].spriteIndex);
            }
            
            if (mb->flags & MESSAGE_BOX_HAS_CHARACTER_AVATAR) {
                resetAnimationState(characterAvatars[mb->characterAvatarIndex].spriteIndex);
            }
            
            result = TRUE;

        }
        
    }

    return result;
    
}

bool initializeMessageBox(u16 messageBoxIndex, u16 textAddressesIndex, u16 textIndex, u32 mode) {

    MessageBox *mb = &messageBoxes[messageBoxIndex];

    bool result = FALSE;

    u16 tempTextIndex;
    u32 textAddress;

    tempTextIndex = textIndex;
    
    if (messageBoxIndex < MAX_MESSAGE_BOXES) {
        
        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            TextAddresses *ta = &textAddresses[textAddressesIndex];

            mb->textIndex = textIndex;
            mb->textAddressesIndex = textAddressesIndex;

            nuPiReadRom(ta->romIndexStart, ta->indexVaddr, ta->romIndexEnd - ta->romIndexStart);

            textAddress = getTextAddress(messageBoxIndex, mb->textIndex);
            nuPiReadRom(textAddress, mb->textBufferBase, getTextAddress(messageBoxIndex, mb->textIndex + 1) - textAddress);
            
            mb->totalLinesToPrint = countTextLines(messageBoxIndex, tempTextIndex);
            mb->currentCompressionControlByte = 0;
            mb->compressionBitIndex = 0;
            mb->scrollCount = 0;
            mb->currentCharCountOnLine = 0;
            mb->currentLineBeingPrinted = 0;
            mb->unk_9D = 0;

            mb->currentCharPtr = mb->textBufferBase;

            mb->flags &= ~MESSAGE_BOX_TEXT_COMPLETE;
            mb->flags &= ~0x4000;
            mb->flags |= MESSAGE_BOX_INITIALIZED;

            mb->flags &= ~MESSAGE_BOX_MODE_UNKNOWN;
            mb->flags &= ~MESSAGE_BOX_MODE_KEEP_ANIMATION;
            mb->flags &= ~MESSAGE_BOX_MODE_NO_INPUT;
            mb->flags &= ~MESSAGE_BOX_SILENT;
            mb->flags &= ~0x20000;

            if (mode == MESSAGE_BOX_MODE_UNKNOWN) {
                mb->flags |= MESSAGE_BOX_MODE_UNKNOWN;
            }
            
            if (mode == MESSAGE_BOX_MODE_KEEP_ANIMATION) {
                mb->flags |= mode;
            }

            if (mode == MESSAGE_BOX_MODE_NO_INPUT) {
                mb->flags |= mode;
            }

            if (mode == MESSAGE_BOX_SILENT) {
                mb->flags |= mode;
            }

            if (mb->flags & MESSAGE_BOX_HAS_OVERLAY_ICON) {
                OverlayIcon *oi = &overlayIcons[mb->overlayIconIndex];
                dmaSprite(oi->spriteIndex, oi->romTextureStart, oi->romTextureEnd, oi->romIndexStart, oi->romIndexEnd, NULL, NULL, oi->vaddrTexture, NULL, oi->vaddrPalette, oi->vaddrAnimationFrameMetadata, oi->vaddrTextureToPaletteLookup, oi->unk_20, 0, 0);
                setBilinearFiltering(oi->spriteIndex, TRUE);
                setSpriteColor(oi->spriteIndex, 255, 255, 255, 255);
                setSpriteAnchorAlignment(oi->spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
                setSpriteBlendMode(oi->spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
            }

            if (mb->flags & MESSAGE_BOX_HAS_CHARACTER_AVATAR) {
                CharacterAvatar *ca = &characterAvatars[mb->characterAvatarIndex];
                dmaSprite(ca->spriteIndex, ca->romTextureStart, ca->romTextureEnd, ca->romAssetsIndexStart, ca->romAssetsIndexEnd, ca->romSpritesheetIndexStart, ca->romSpritesheetIndexEnd, ca->vaddrTexture1, ca->vaddrTexture2, ca->vaddrPalette, ca->vaddrAnimation, ca->vaddrSpriteToPalette, ca->vaddrSpritesheetIndex, 1, 0);
                setBilinearFiltering(ca->spriteIndex, TRUE);
                setSpriteColor(ca->spriteIndex, 255, 255, 255, 255);
                setSpriteAnchorAlignment(ca->spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
                setSpriteBlendMode(ca->spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
            }

            if (mb->flags & MESSAGE_BOX_HAS_DIALOGUE_WINDOW) {
                DialogueWindow *dw = &dialogueWindows[mb->dialogueWindowIndex];
                dmaSprite(dw->spriteIndex, dw->romTextureStart, dw->romTextureEnd, dw->romIndexStart, dw->romIndexEnd, NULL, NULL, dw->vaddrTexture, NULL, dw->vaddrPalette, dw->vaddrAnimationFrameMetadata, dw->vaddrTextureToPaletteLookup, dw->unk_20, 0, 0);
                setBilinearFiltering(dw->spriteIndex, TRUE);
                setSpriteColor(dw->spriteIndex, 255, 255, 255, 255);
                setSpriteAnchorAlignment(dw->spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
                setSpriteBlendMode(dw->spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
                startSpriteAnimation(dw->spriteIndex, dw->spriteOffset, dw->flag);
                setSpriteViewSpacePosition(dw->spriteIndex, mb->viewSpacePosition.x + dw->coordinates.x, mb->viewSpacePosition.y + dw->coordinates.y, (mb->viewSpacePosition.z + dw->coordinates.z) - 2.0f);
            }

            setMessageBoxRGBA(messageBoxIndex, 255, 255, 255, 255);

            result = TRUE;
            
        }
    }

    return result;
    
}

bool setMessageBoxRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) { 

        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            
            mb->targetRGBA.r = (mb->baseRGBA.r * r) / 255.0f;
            mb->targetRGBA.g = (mb->baseRGBA.g * g) / 255.0f;
            mb->targetRGBA.b = (mb->baseRGBA.b * b) / 255.0f;
            mb->targetRGBA.a = (mb->baseRGBA.a * a) / 255.0f;
    
            mb->currentRGBA.r = (mb->baseRGBA.r * r) / 255.0f;
            mb->currentRGBA.g = (mb->baseRGBA.g * g) / 255.0f;
            mb->currentRGBA.b = (mb->baseRGBA.b * b) / 255.0f;
            mb->currentRGBA.a = (mb->baseRGBA.a * a) / 255.0f;
    
            result = TRUE;
     
        }
        
    }

    return result;
    
}

bool adjustMessageBoxTargetRGBA(u16 index, s8 r, s8 g, s8 b, s8 a) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) { 

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->targetRGBA.r += r;
            mb->targetRGBA.g += g;
            mb->targetRGBA.b += b;
            mb->targetRGBA.a += a;
            
            result = TRUE;
            
        }
    }

    return result;

}

bool setMessageBoxRGBAWithTransition(u16 index, u8 r, u8 g, u8 b, u8 a, s16 rate) {

    MessageBox *mb = &messageBoxes[index];

    f32 tempF;
    s16 temp = getAbsoluteValue(rate);

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) { 

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->currentRGBA.r = (mb->baseRGBA.r * r) / 255.0f;
            mb->currentRGBA.g = (mb->baseRGBA.g * g) / 255.0f;
            mb->currentRGBA.b = (mb->baseRGBA.b * b) / 255.0f;
            mb->currentRGBA.a = (mb->baseRGBA.a * a) / 255.0f;
            
            mb->flags &= ~MESSAGE_BOX_RGBA_COMPLETE;

            if (mb->currentRGBA.r < mb->targetRGBA.r) {
                tempF = mb->targetRGBA.r - mb->currentRGBA.r;
            } else {
                tempF = mb->currentRGBA.r - mb->targetRGBA.r;
            }

            mb->deltaRGBA.r = (tempF * temp) / mb->baseRGBA.r;
            
            if (mb->currentRGBA.g < mb->targetRGBA.g) {
                tempF = mb->targetRGBA.g - mb->currentRGBA.g;
            } else {
                tempF = mb->currentRGBA.g - mb->targetRGBA.g;
            }

            mb->deltaRGBA.g = (tempF * temp) / mb->baseRGBA.g;
            
            if (mb->currentRGBA.b < mb->targetRGBA.b) {
                tempF = mb->targetRGBA.b - mb->currentRGBA.b;
            } else {
                tempF = mb->currentRGBA.b - mb->targetRGBA.b;
            }

            mb->deltaRGBA.b = (tempF * temp) / mb->baseRGBA.b;

            if (mb->currentRGBA.a < mb->targetRGBA.a) {
                tempF = mb->targetRGBA.a - mb->currentRGBA.a;
            } else {
                tempF = mb->currentRGBA.a - mb->targetRGBA.a;
            }

            mb->deltaRGBA.a = (tempF * temp) / mb->baseRGBA.a;
        
            if (mb->flags & MESSAGE_BOX_HAS_DIALOGUE_WINDOW) {
                updateSpriteRGBA(dialogueWindows[mb->dialogueWindowIndex].spriteIndex, 
                    r, g, b, a, temp);
            }

            if (mb->flags & MESSAGE_BOX_HAS_CHARACTER_AVATAR) {
                updateSpriteRGBA(characterAvatars[mb->characterAvatarIndex].spriteIndex, 
                    r, g, b, a, temp);
            }

            if (mb->flags & MESSAGE_BOX_HAS_OVERLAY_ICON) {
                updateSpriteRGBA(overlayIcons[mb->overlayIconIndex].spriteIndex, 
                    r, g, b, a, temp);
            }
            
            result = TRUE;
        
        }

    }

    return result;

}

// unused
// bool setMessageBoxAlphaWithTransition(u16 index, u8 arg1, s16 arg2) {

//     bool result;
//     s16 temp;
//     f32 tempF;

//     temp = getAbsoluteValue(arg2);

//     result = FALSE;
    
//     if (index < MAX_MESSAGE_BOXES) {

//         if (messageBoxes[index].flags & MESSAGE_BOX_ACTIVE) {

//             messageBoxes[index].currentRGBA.a = (messageBoxes[index].baseRGBA.a * arg1) / 255.0f;

//             messageBoxes[index].flags &= ~(MESSAGE_BOX_RGBA_COMPLETE);

//             if (messageBoxes[index].currentRGBA.a < messageBoxes[index].targetRGBA.a) {
//                 tempF = messageBoxes[index].targetRGBA.a - messageBoxes[index].currentRGBA.a;
//             } else {
//                 tempF = messageBoxes[index].currentRGBA.a - messageBoxes[index].targetRGBA.a;
//             }

//             messageBoxes[index].deltaRGBA.a = (tempF * temp) / messageBoxes[index].baseRGBA.a;

//             result = TRUE;
            
//         }
        
//     }

//     return result;
    
// }

bool checkMessageBoxRGBAComplete(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            result = (mb->flags >> 0x15) & 1;
        }
        
    }

    return result;
    
}

bool setMessageBoxBaseRGBA(u16 index, u8 r, u8 g, u8 b, u8 a) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->baseRGBA.r = r;
            mb->baseRGBA.g = g;
            mb->baseRGBA.b = b;
            mb->baseRGBA.a = a;

            result = TRUE;
            
        }
        
    }

    return result;
    
}

bool checkAnyMessageBoxTextFinished(void) {

    u16 i;
    bool result = FALSE;

    for (i = 0; i < MAX_MESSAGE_BOXES; i++) {

        if (messageBoxes[i].flags & MESSAGE_BOX_TEXT_COMPLETE) {
            result = TRUE;
        }
        
    }

    return result;
    
}

bool resetMessageBoxAnimation(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            if (!(mb->flags & MESSAGE_BOX_MODE_UNKNOWN) && !(mb->flags & MESSAGE_BOX_MODE_KEEP_ANIMATION) && !(mb->flags & MESSAGE_BOX_MODE_NO_INPUT)) {
                mb->flags &= ~(MESSAGE_BOX_INITIALIZED | MESSAGE_BOX_TEXT_END_REACHED);
            }

            if ((mb->flags & MESSAGE_BOX_HAS_DIALOGUE_WINDOW) && !(mb->flags & MESSAGE_BOX_MODE_UNKNOWN) && !(mb->flags & MESSAGE_BOX_MODE_KEEP_ANIMATION) && !(mb->flags & MESSAGE_BOX_MODE_NO_INPUT)) {

                resetAnimationState(dialogueWindows[mb->dialogueWindowIndex].spriteIndex);

                if (mb->flags & MESSAGE_BOX_HAS_CHARACTER_AVATAR) {
                    resetAnimationState(characterAvatars[mb->characterAvatarIndex].spriteIndex);        
                }
            
            }

            result = TRUE;
            
        }
        
    }

    return result;
    
}

// unused
// bool setMessageBoxInterpolationRate(u16 index, s16 rate) {

//     bool result = FALSE;
    
//     if (index < MAX_MESSAGE_BOXES && (messageBoxes[index].flags & MESSAGE_BOX_ACTIVE)) {

//         messageBoxes[index].unk_7C = rate;
        
//         initializeInterpolator((Interpolator*)&messageBoxes[index].unk_64, rate, 0);
        
//         result = TRUE;
            
//     }

//     return result;
    
// }

bool setTextAddresses(u16 textAddressesIndex, u32 romIndexStart, u32 romIndexEnd, u32 romTextStart, u32 indexVaddr) {

    TextAddresses *ta = &textAddresses[textAddressesIndex];

    bool result = FALSE;

    if (textAddressesIndex < TOTAL_TEXTS) {

        ta->romIndexStart = romIndexStart;
        ta->romIndexEnd = romIndexEnd;
        ta->indexVaddr = indexVaddr;
        ta->romTextStart = romTextStart;

        result = TRUE;

    }

    return result;

}

bool setMessageBoxInterpolationWithFlags(u16 index, s16 rate, u8 interpolationMode) {

    MessageBox *mb = &messageBoxes[index];
    
    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {
        
        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->unk_7C = rate;
            initializeInterpolator((Interpolator*)&mb->unk_64, rate, 0);

            mb->flags &= ~(MESSAGE_BOX_INTERPOLATION_MODE_1 | MESSAGE_BOX_INTERPOLATION_MODE_2);

            switch (interpolationMode) {

                case 0:
                    break;
                
                case 1:
                    mb->flags |= MESSAGE_BOX_INTERPOLATION_MODE_1;
                    break;

                case 2:
                    mb->flags |= MESSAGE_BOX_INTERPOLATION_MODE_2;
                    break;                    

                case 3:
                    mb->flags |= (MESSAGE_BOX_INTERPOLATION_MODE_1 | MESSAGE_BOX_INTERPOLATION_MODE_2);
                    break;               
                
            }
            
            result = TRUE;
        
        }
    
    }

    return result;
    
}

bool setMessageBoxFont(u16 index, u8 arg1, u8 arg2, u8* compressedCI2FontData, u16* fontPalettePtr) {

    MessageBox *mb = &messageBoxes[index];
    
    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {
        
        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            
            mb->fontContext.characterCellWidth = arg1;
            mb->fontContext.characterCellHeight = arg2;
            mb->fontContext.compressedCI2FontData = compressedCI2FontData;
            mb->fontContext.fontPalettePtr = fontPalettePtr;
             
            result = TRUE;
            
        }

    }
    
    return result;
    
}

bool setMessageBoxSfx(u16 index, u32 arg1, u32 arg2, u32 arg3) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->characterPrintSfx = arg1;
            mb->unk_74 = arg2;
            mb->messageCloseSfx = arg3;
            
            result = TRUE;
        
        }
    
    }

    return result;
    
}

bool setMessageBoxViewSpacePosition(u16 index, f32 x, f32 y, f32 z) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            
            mb->viewSpacePosition.x = x;
            mb->viewSpacePosition.y = y;
            mb->viewSpacePosition.z = z;
            
            result = TRUE;

        }

    }

    return result;

}

bool setMessageBoxLineAndRowSizes(u16 index, u8 arg1, u8 arg2) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->textBoxLineCharWidth = arg1;
            mb->textBoxVisibleRows = arg2;
            
            result = TRUE;
            
        }
    }
    
    return result;

}

bool setMessageBoxSpacing(u16 index, u8 charSpacing, u8 lineSpacing) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            mb->characterSpacing = charSpacing;
            mb->lineSpacing = lineSpacing;
            
            result = TRUE;
            
        }

    }
    
    return result;
    
}

bool setMessageBoxSpriteIndices(u16 index, u8 dialogueWindowIndex, u8 overlayIconIndex, u8 characterAvatarIndex) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;
    
    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) { 
            
            mb->dialogueWindowIndex = dialogueWindowIndex;
            mb->overlayIconIndex = overlayIconIndex;
            mb->characterAvatarIndex = characterAvatarIndex;
 
            if (mb->dialogueWindowIndex != 0xFF) {
                mb->flags |= MESSAGE_BOX_HAS_DIALOGUE_WINDOW;
            } else {
                mb->flags &= ~MESSAGE_BOX_HAS_DIALOGUE_WINDOW;
            }

            if (mb->overlayIconIndex != 0xFF) {
                mb->flags |= MESSAGE_BOX_HAS_OVERLAY_ICON;
            } else {
                mb->flags &= ~MESSAGE_BOX_HAS_OVERLAY_ICON;
            }

            if (mb->characterAvatarIndex != 0xFF) {
                mb->flags |= MESSAGE_BOX_HAS_CHARACTER_AVATAR;
            } else {
                mb->flags &= ~MESSAGE_BOX_HAS_CHARACTER_AVATAR;
            }

            result = TRUE;
        
        }

    }

    return result;
    
}

bool setDialogueWindowSprite(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, u8* vaddrTexture, u16* vaddrPalette, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    DialogueWindow *dw = &dialogueWindows[index];

    bool result = FALSE;

    if (index < 3) {

        dw->romTextureStart = romTextureStart;
        dw->romTextureEnd = romTextureEnd;
        dw->romIndexStart = romIndexStart;
        dw->romIndexEnd = romIndexEnd;

        dw->vaddrTexture = vaddrTexture;
        dw->vaddrPalette = vaddrPalette;
        dw->vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        dw->vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

        dw->unk_20 = argA;

        dw->spriteIndex = spriteIndex;
        dw->spriteOffset = spriteOffset;

        dw->flag= flag;

        dw->coordinates.x = x;
        dw->coordinates.y = y;
        dw->coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

bool setOverlayIconSprite(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u16* vaddrPalette, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, 
    u32 argA, u16 offset, u8 flag, f32 x, f32 y, f32 z) {

    OverlayIcon *oi = &overlayIcons[index];

    bool result = FALSE;
    
    if (index < 2) {

        oi->romTextureStart = romTextureStart;
        oi->romTextureEnd = romTextureEnd;
        oi->romIndexStart = romIndexStart;
        oi->romIndexEnd = romIndexEnd;

        oi->vaddrTexture = vaddrTexture;
        oi->vaddrPalette = vaddrPalette;
        oi->vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        oi->vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;

        oi->unk_20 = argA;
        oi->spriteIndex = spriteIndex;

        oi->spriteOffset = offset;
        
        oi->flag = flag;

        oi->coordinates.x = x;
        oi->coordinates.y = y;
        oi->coordinates.z = z;
        
        result = TRUE;
        
    }

    return result;
    
}

bool setCharacterAvatarSprite(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, 
    u32 romAssetsIndexStart, u32 romAssetsIndexEnd, 
    u32 romSpritesheetIndexStart, u32 romSpritesheetIndexEnd, 
    u8* vaddrTexture1, u8* vaddrTexture2, 
    u16* vaddrPalette, void* vaddrAnimation, 
    void* vaddrSpriteToPalette, void* vaddrSpritesheetIndex, 
    f32 x, f32 y, f32 z) {

    CharacterAvatar *ca = &characterAvatars[index];

    bool result = FALSE;

    if (index == 0) {
        
        ca->romTextureStart = romTextureStart;
        ca->romTextureEnd = romTextureEnd;
        ca->romAssetsIndexStart = romAssetsIndexStart;
        ca->romAssetsIndexEnd = romAssetsIndexEnd;
        ca->romSpritesheetIndexStart = romSpritesheetIndexStart;
        ca->romSpritesheetIndexEnd = romSpritesheetIndexEnd;
  
        ca->vaddrTexture1 = vaddrTexture1;
        ca->vaddrTexture2 = vaddrTexture2;
        ca->vaddrPalette = vaddrPalette;
        ca->vaddrAnimation = vaddrAnimation;
        ca->vaddrSpriteToPalette = vaddrSpriteToPalette;
        ca->vaddrSpritesheetIndex = vaddrSpritesheetIndex;

        ca->spriteIndex = spriteIndex;

        ca->coordinates.x = x;
        ca->coordinates.y = y;
        ca->coordinates.z = z;

        result = TRUE;
        
    }
    
    return result;
    
}

void setCharacterAvatarAnimationsPtr(u8* arg0) {
    characterAvatarsAnimationsPtr = arg0;
}

bool setMessageBoxButtonMask(u16 index, u16 arg1) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) {
        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            mb->buttonMask = arg1;
            result = TRUE;
        }
    }

    return result;
    
}

bool setMessageBoxScrollSpeed(u16 index, s16 speed) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {
            mb->defaultScrollSpeed = speed;
            result = TRUE;
        }
    }

    return result;
    
}

bool setGameVariableString(u16 index, u8* ptr, s8 length) {

    bool result = FALSE;

    if (index < MAX_GAME_VARIABLE_STRINGS) {
        
        gameVariableStrings[index].ptr = ptr;
        gameVariableStrings[index].maxLength = length;

        result = TRUE;

    }
    
    return result;
    
}

// set number string for game variables based on their current quantity
bool convertNumberToGameVariableString(u16 index, u32 number, u8 terminatorType) {

    GameVariableString *gvs = &gameVariableStrings[index];

    u8 digitPosition;
    u8 position;
    bool startedOutputting;

    u32 divisor;
    u32 extractedDigit;
    u32 unprocessedDigits;

    unprocessedDigits = number;
    position = 0;
    startedOutputting = 0;

    digitPosition = gvs->maxLength - 1;

    do {

        divisor = powersOfTen[digitPosition];
        extractedDigit = unprocessedDigits / divisor;
        unprocessedDigits %= divisor;

        // first non-zero significant digit or force output of a 0 if no value
        if (extractedDigit != 0 || digitPosition == 0) {
            startedOutputting = TRUE;
            goto writeDigitChar;
        }

        if (startedOutputting) goto writeDigitChar;

        if (digitPosition) goto writePaddingChar;

writeDigitChar:
            gvs->ptr[position] = digitCharacterCodes[extractedDigit];

            goto advancePosition;

            // leading zeros
writePaddingChar:
            if (terminatorType == STANDALONE) {
                gvs->ptr[position] = char_SPACE;
            } else {
                gvs->ptr[position] = char_TERMINATOR;
            }

advancePosition:
            position++;

    } while (digitPosition--);
    
    return 0;
    
}

void clearTextBuffer(void) {

    u16 i;

    for (i = 0; i < 256; i++) {
        textBuffer[i] = char_SPACE;
    }
    
}

bool clearMessageBoxTextBuffer(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    u16 i;
    u16 cellCount;
    u32 temp2;

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) {

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            cellCount = mb->textBoxLineCharWidth * mb->textBoxVisibleRows;

            if (((mb->flags >> 4) & 3) / 3) {
                cellCount += mb->textBoxLineCharWidth;
            }

            for (i = 0; i < cellCount; i++) {
                textBuffer[mb->totalCharactersProcessed + i] = char_SPACE;
            }
        
            result = TRUE;
            
        }

    }

    return result;
    
}

bool scrollMessageBoxDown(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) { 

        if ((mb->flags & MESSAGE_BOX_ACTIVE) && (mb->flags & MESSAGE_BOX_INITIALIZED) && !(mb->flags & (MESSAGE_BOX_SCROLLING_DOWN | MESSAGE_BOX_SCROLLING_UP))) {

            if (mb->totalLinesToPrint + 1 != mb->scrollCount + mb->textBoxVisibleRows) {
 
                initializeInterpolator((Interpolator*)&mb->scrollInterpolator, mb->defaultScrollSpeed, 0);

                mb->flags |= MESSAGE_BOX_SCROLLING_DOWN;
                mb->flags &= ~MESSAGE_BOX_TEXT_END_REACHED;

                mb->currentLineBeingPrinted++;
                
                // handle next line/scroll
                advanceToLine(index, mb->scrollCount + mb->textBoxVisibleRows + 1);
                
                mb->currentCharPtr--;
                
                result = TRUE ;
                
            }
        }
    }

    return result;
    
}

bool scrollMessageBoxUp(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) { 

        if ((mb->flags & MESSAGE_BOX_ACTIVE) && (mb->flags & MESSAGE_BOX_INITIALIZED) && !(mb->flags & (MESSAGE_BOX_SCROLLING_DOWN | MESSAGE_BOX_SCROLLING_UP))) {

            if (mb->scrollCount) {

                initializeInterpolator((Interpolator*)&mb->scrollInterpolator, mb->defaultScrollSpeed, 0);

                mb->flags |= MESSAGE_BOX_SCROLLING_UP;
                mb->flags &= ~MESSAGE_BOX_TEXT_END_REACHED;

                mb->scrollCount--;
                mb->currentLineBeingPrinted++;
                
                // handle next line/scroll
                advanceToLine(index, mb->scrollCount + mb->textBoxVisibleRows + 1);
                
                mb->currentCharPtr--;
                
                result = TRUE;
                
            }

        }

    }

    return result;

}

bool checkMoreLinesToPrint(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    bool result = FALSE;

    if (index < MAX_MESSAGE_BOXES) { 

        if ((mb->flags & MESSAGE_BOX_ACTIVE) && (mb->flags & MESSAGE_BOX_INITIALIZED)) {
            result = mb->totalLinesToPrint + 1 != mb->scrollCount + mb->textBoxVisibleRows;
        }
        
    }

    return result;
    
}

u8 checkMessageBoxScrolled(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    u8 count = 0;

    if (index < MAX_MESSAGE_BOXES) {
        if (mb->flags & MESSAGE_BOX_ACTIVE && mb->flags & MESSAGE_BOX_INITIALIZED) {
            count = mb->scrollCount != 0;
        }
    }

    return count;
    
}

void updateScrollDownAnimation(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    calculateInterpolatorStep((Interpolator*)&mb->scrollInterpolator);

    // FIXME: probably a u16 return value
    if ((u16)getInterpolatorValue((Interpolator*)&mb->scrollInterpolator) >= mb->fontContext.characterCellHeight + mb->lineSpacing) {

        mb->flags &= ~MESSAGE_BOX_SCROLLING_DOWN;
         
        initializeInterpolator((Interpolator*)&mb->scrollInterpolator, 0, 0);
 
        mb->scrollCount++;
        mb->currentLineBeingPrinted--;
        
    }
       
}

void updateScrollUpAnimation(u16 index) {

    MessageBox *mb = &messageBoxes[index];

    calculateInterpolatorStep((Interpolator*)&mb->scrollInterpolator);

    if ((u16)getInterpolatorValue((Interpolator*)&mb->scrollInterpolator) >= mb->fontContext.characterCellHeight + mb->lineSpacing) {

        mb->flags &= ~MESSAGE_BOX_SCROLLING_UP;
        
        initializeInterpolator((Interpolator*)&mb->scrollInterpolator, 0, 0);

        mb->currentLineBeingPrinted--;
        
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

u8 countTextLines(u16 index, u16 textIndex) {

    MessageBox *mb = &messageBoxes[index];
    
    u8 result = 0;
    
    bool done = FALSE;
    
    u8 count2 = 0;
    u8 count = 0;
    
    u8 i;
    u8 gameVariableStringIndex;
    
    mb->currentCompressionControlByte = 0;
    mb->compressionBitIndex = 0;
    mb->maxCharsPerLine = 0;
    mb->totalLines = 0;
    mb->currentCharPtr = mb->textBufferBase;
    
    do {

        switch (readCompressedCharacter(index)) {

            case CHARACTER_CONTROL_LINEBREAK:

                if (mb->maxCharsPerLine < count2) {
                    mb->maxCharsPerLine = count2;
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

                if (mb->maxCharsPerLine < count2) {
                    mb->maxCharsPerLine = count2;
                }

                done = TRUE;
                count++;

                break;

            case CHARACTER_CONTROL_INSERT_GAME_VARIABLE: {
                GameVariableString *gvs;

                gameVariableStringIndex = *mb->currentCharPtr;
                mb->currentCharPtr++;

                gvs = &gameVariableStrings[gameVariableStringIndex];

                if (gvs->ptr[0] != char_TERMINATOR) {

                    i = 0;

                    while ((gvs->ptr[i] != char_TERMINATOR) && (i < gvs->maxLength)) {
                        i++;
                        // get length of string
                        count2++;
                    }

                }

                break;
            }

            case CHARACTER_CONTROL_WAIT:
            case CHARACTER_CONTROL_CHARACTER_AVATAR:
                mb->currentCharPtr++;
                break;

            default:
                count2++;
                break;

        }

    } while (!done);
    
    mb->totalLines = count;
    
    return result;
    
}

void advanceToLine(u16 index, u8 linesToSkip) {

    MessageBox *mb = &messageBoxes[index];
    
    u8 i;
    u8 position;
    u16 character;

    bool done = FALSE;

    i = 0;
    
    mb->currentCompressionControlByte = 0;
    mb->compressionBitIndex = 0;

    mb->currentCharPtr = mb->textBufferBase;
    
    if (linesToSkip != 0) {
        
        do {

            if (done) {
                GameVariableString *gvs = &gameVariableStrings[mb->gameVariableStringIndex];

                position++;

                if (gvs->ptr[position] == char_TERMINATOR) {

                    while ((gvs->ptr[position] == char_TERMINATOR) && (position < gvs->maxLength)) {
                        position++;
                    }

                }

                character = char_SPACE;

                if (position == gvs->maxLength) {
                    done = FALSE;
                }

            } else {
                // loop back to here until done
                character = readCompressedCharacter(index);
            }
            
            // handle control character
            switch (character) {

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
                    mb->currentCharPtr++;
                    break;
                case CHARACTER_CONTROL_WAIT:
                case CHARACTER_CONTROL_CHARACTER_AVATAR:
                    mb->currentCharPtr++;
                    break;
                default:
                    break;

            }

        } while (i < linesToSkip);

    }

}

void processText(u16 index) {

    MessageBox *mb = &messageBoxes[index];
    
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
    
    switch ((mb->flags >> 4) & 3) {
            
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

    originalCharPtr = mb->currentCharPtr;
    
    characterPositionOnLine = 0;
    
    advanceToLine(index, mb->scrollCount);

    currentLine = 0;
    processingGameVariableString = FALSE;

    while (mb->currentCharPtr < originalCharPtr || processingGameVariableString) {
        
        if (processingGameVariableString) {
            GameVariableString *gvs = &gameVariableStrings[gameVariableStringIndex];

            character = gvs->ptr[gameVariableStringPosition];
            gameVariableStringPosition++;

            while (gvs->ptr[gameVariableStringPosition] == char_TERMINATOR &&
                   gameVariableStringPosition < gvs->maxLength) {
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
            character = readCompressedCharacter(index);
        }

        switch (character) {

            case CHARACTER_CONTROL_LINEBREAK:
                characterPositionOnLine = 0;
                currentLine++;
                break;
            
            // clear and start new line (overwrite text box)
            case CHARACTER_CONTROL_SOFTBREAK:

                clearMessageBoxTextBuffer(index);
                characterPositionOnLine = 0;
                currentLine = 0;
                break;
                    
            case CHARACTER_CONTROL_TEXT_END:

                if (renderMode == 3) {
                    
                    mb->currentCharPtr--;
                    
                } else if (!(mb->flags & 0x4000) || 
                          (mb->savedCharPtr != mb->currentCharPtr)) {
                    
                    characterPositionOnLine = 0;
                    currentLine++;
                    
                }
                
                break;

            case CHARACTER_CONTROL_WAIT:
                mb->currentCharPtr++;
                break;

            // set up game variable printing
            case CHARACTER_CONTROL_INSERT_GAME_VARIABLE:
                
                if (mb->gameVariableStringPtr == mb->currentCharPtr) {
                    
                    mb->currentCharPtr++;
                    gameVariableStringIndex = mb->gameVariableStringIndex;
                    gameVariableStringLength = mb->gameVariableStringLength;
                    processingGameVariableString = 2;
                    
                } else {
                    
                    gameVariableStringIndex = *mb->currentCharPtr;
                    mb->currentCharPtr++;
                    gameVariableStringLength = gameVariableStrings[gameVariableStringIndex].maxLength;
                    processingGameVariableString = TRUE;
                }

                gameVariableStringPosition = 0;
                
                break;

            case CHARACTER_CONTROL_CHARACTER_AVATAR:
                mb->currentCharPtr++;
                break;
            
            // update text buffer, converting from ROM char to font index 
            default:
                textBuffer[mb->totalCharactersProcessed + 
                                   (currentLine * mb->textBoxLineCharWidth) + 
                                   characterPositionOnLine] = character - 0xB;
                
                characterPositionOnLine++;

                break;

            case CHARACTER_CONTROL_WAIT_WITH_ICON:
            case 0xFFFF:
                break;

        }
    }
    
}

void updateMessageBoxText(u16 index) {

    MessageBox *mb = &messageBoxes[index];
    
    Swap16 swap;

    u8 temp;
    u8 temp2;

    u16 character;

    u8 animationIndex;

    u8 renderMode;
    u8 currentLine;
        
    bool continueProcessing;
        
    switch ((mb->flags >> 4) & 3) {
                
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

    temp = (((mb->flags >> 4) & 3) / 3) ^ 1;
    
    continueProcessing = FALSE;
    
    while (!continueProcessing) {
        
        if (mb->flags & MESSAGE_BOX_PROCESSING_GAME_VARIABLE) {
            
            character = readGameVariableChar(index);
            
            if (character == 0xFF) {
                character = 0xFFFF;
            } else {
                character += 0xB;
            }
            
        } else if ((mb->flags & MESSAGE_BOX_WAITING_WITH_ICON) || (mb->flags & 0x4000)) {
            
            if (mb->flags & MESSAGE_BOX_WAITING_WITH_ICON) {
                character = 4;
            } else {
                character = 2;
            }
            
        } else {
            character = readCompressedCharacter(index);
        }
        
        switch (character) {
            
            case CHARACTER_CONTROL_LINEBREAK:
                
                currentLine = mb->currentLineBeingPrinted;
                
                // Check if need to scroll
                if (currentLine >= (mb->textBoxVisibleRows - temp)) {
                    
                    if (renderMode != 3) {
                                
                        if (checkButtonHeld(CONTROLLER_1, mb->buttonMask) || (renderMode == 2)) {
                            // fast scroll if button held
                            initializeInterpolator(&mb->scrollInterpolator, 4, 0);
                        } else {
                            // default scroll speed
                            initializeInterpolator(&mb->scrollInterpolator, mb->defaultScrollSpeed, 0);
                        }
                        
                        mb->flags |= MESSAGE_BOX_SCROLLING_DOWN;
                        mb->currentLineBeingPrinted++;
                                
                    } else {
                        mb->flags |= MESSAGE_BOX_TEXT_END_REACHED;
                    }
                        
                    temp2 = renderMode;
                    temp2 -= 2;
                    
                    if (temp2 < 2) {
                        continueProcessing = TRUE;
                    }
                        
                } else {
                    mb->currentLineBeingPrinted++;
                }   
            
                mb->currentCharCountOnLine = 0;
                
                if (renderMode < 2) {
                    continueProcessing = TRUE;
                }
                
                break;
                    
            case CHARACTER_CONTROL_SOFTBREAK:
                    
                clearMessageBoxTextBuffer(index);
                
                mb->currentCharCountOnLine = 0;
                mb->scrollCount += mb->currentLineBeingPrinted + 1;
                mb->currentLineBeingPrinted = 0;
                
                continueProcessing = TRUE;
                
                break;
                    
            case CHARACTER_CONTROL_TEXT_END:
                
                if (renderMode == 3) {
                    mb->flags |= MESSAGE_BOX_TEXT_END_REACHED;
                } else {
                
                    mb->flags |= 0x4000;
                    mb->savedCharPtr = mb->currentCharPtr;
                    
                    if ((mb->flags & MESSAGE_BOX_HAS_OVERLAY_ICON) && !(mb->flags & MESSAGE_BOX_MODE_NO_INPUT)) {
                        OverlayIcon *oi = &overlayIcons[mb->overlayIconIndex];

                        startSpriteAnimation(oi->spriteIndex,
                                overlayIcons[mb->overlayIconIndex + 1].spriteOffset,
                                overlayIcons[mb->overlayIconIndex + 1].flag
                        );

                        setSpriteViewSpacePosition(oi->spriteIndex,
                        mb->viewSpacePosition.x + oi->coordinates.x,
                            mb->viewSpacePosition.y + oi->coordinates.y,
                            mb->viewSpacePosition.z + oi->coordinates.z + 1.0f);

                    }
                    
                    if ((mb->flags & MESSAGE_BOX_BUTTON_PRESSED) || (mb->flags & MESSAGE_BOX_SILENT)) {
                        
                        mb->flags &= ~0x4000;
                        mb->flags |= MESSAGE_BOX_TEXT_COMPLETE;
                        
                        if (!(mb->flags & MESSAGE_BOX_MODE_KEEP_ANIMATION)) {
                            resetMessageBoxAnimation(index);
                        }
                        
                        resetAnimationState(overlayIcons[mb->overlayIconIndex].spriteIndex);
                        
                        // play close sound effect
                        if (!(mb->flags & MESSAGE_BOX_SILENT) && (mb->characterPrintSfx != 0xFF)) {
                            setSfx(mb->messageCloseSfx + 1);
                            setSfxVolume(mb->messageCloseSfx + 1, 0x80);
                        }
                        
                    }
                }
                
                mb->currentCharCountOnLine = 0;
                continueProcessing = TRUE;
                
                break;
                
            case CHARACTER_CONTROL_WAIT:
                
                initializeInterpolator(&mb->unk_64, -(s16)*mb->currentCharPtr, 0);
                
                mb->currentCharPtr++;
                mb->flags |= MESSAGE_BOX_NEEDS_INTERPOLATOR_INIT;
                continueProcessing = TRUE;
                
                break;
                
            case CHARACTER_CONTROL_WAIT_WITH_ICON:
                
                mb->flags |= MESSAGE_BOX_WAITING_WITH_ICON;
                mb->savedCharPtr = mb->currentCharPtr;
                
                if (mb->flags & MESSAGE_BOX_HAS_OVERLAY_ICON) {
                    OverlayIcon *oi = &overlayIcons[mb->overlayIconIndex];

                    startSpriteAnimation(oi->spriteIndex, oi->spriteOffset, oi->flag);
                    setSpriteViewSpacePosition(oi->spriteIndex,
                    mb->viewSpacePosition.x + oi->coordinates.x,
                    mb->viewSpacePosition.y + oi->coordinates.y,
                    mb->viewSpacePosition.z + oi->coordinates.z + 1.0f);

                }
                
                if (mb->flags & MESSAGE_BOX_BUTTON_PRESSED) {
                    
                    mb->flags &= ~MESSAGE_BOX_BUTTON_PRESSED;
                    mb->flags &= ~MESSAGE_BOX_WAITING_WITH_ICON;
                    
                    resetAnimationState(overlayIcons[mb->overlayIconIndex].spriteIndex);
                    
                    if (mb->characterPrintSfx != 0xFF) {
                        setSfx(mb->unk_74 + 1);
                        setSfxVolume(mb->unk_74 + 1, 0x80);
                    }
                    
                } else {
                    continueProcessing = TRUE;
                }
                
                break;
                
            case CHARACTER_CONTROL_LOAD_TEXT:
                
                // Read dialogue index (2 bytes, little endian)
                swap.byte[1] = *mb->currentCharPtr;
                mb->currentCharPtr++;
                swap.byte[0] = *mb->currentCharPtr;
                mb->currentCharPtr++;
                
                mb->textIndex = swap.halfword;
                
                // Reset text state
                mb->currentCompressionControlByte = 0;
                mb->compressionBitIndex = 0;
                mb->currentCharCountOnLine = 0;
                mb->currentLineBeingPrinted = 0;
                mb->scrollCount = 0;
                mb->totalLinesToPrint = 0;
                
                mb->currentCharPtr = mb->textBufferBase;
                
                break;
                
            case CHARACTER_CONTROL_UNUSED:
                break;
            
            case CHARACTER_CONTROL_INSERT_GAME_VARIABLE:

                mb->gameVariableStringPtr = mb->currentCharPtr;
                
                swap.byte[0] = *mb->currentCharPtr;
                
                mb->currentCharPtr++;
                
                mb->gameVariableStringIndex = swap.byte[0];
                
                mb->gameVariableStringLength = 0;
                
                mb->flags |= MESSAGE_BOX_PROCESSING_GAME_VARIABLE;
                
                break;
                
            case CHARACTER_CONTROL_WAIT_ALTERNATE: 
            
                mb->flags |= 0x4000;
                mb->savedCharPtr = mb->currentCharPtr;
                
                if (mb->flags & MESSAGE_BOX_BUTTON_PRESSED) {
                    mb->flags = (mb->flags & ~0x4000) | MESSAGE_BOX_TEXT_COMPLETE;
                } else {
                    continueProcessing = TRUE;
                }
            
                break;
                
            case CHARACTER_CONTROL_CHARACTER_AVATAR: {
                CharacterAvatar *ca = &characterAvatars[mb->characterAvatarIndex];

                animationIndex = *mb->currentCharPtr;
                mb->currentCharPtr++;

                resetAnimationState(ca->spriteIndex);
                startSpriteAnimation(ca->spriteIndex, characterAvatarsAnimationsPtr[animationIndex], 0);

                setSpriteViewSpacePosition(ca->spriteIndex,
                    mb->viewSpacePosition.x + ca->coordinates.x,
                    mb->viewSpacePosition.y + ca->coordinates.y,
                    mb->viewSpacePosition.z + ca->coordinates.z - 1.0f);

                break;
            }
                
            case 0xFFFF:
                break;

            case 0xA:
                break;
                
            default:
                
                textBuffer[mb->totalCharactersProcessed + 
                    (mb->currentLineBeingPrinted * mb->textBoxLineCharWidth) + 
                    mb->currentCharCountOnLine] = character - 0xB;
                
                mb->currentCharCountOnLine++;
                
                if (renderMode != 0) break;
                
                if (mb->characterPrintSfx != 0xFF) {
                    
                    // skip space characters
                    if ((character - 0xB) != 0xEE) {
                            
                        setSfx(mb->characterPrintSfx + 1);
                        setSfxVolume(mb->characterPrintSfx + 1, 0x80);
                        
                    }
            
                }
        
                continueProcessing = TRUE;
            
                break;
                
        }
            
    } 
    
    calculateInterpolatorStep((Interpolator*)&mb->unk_64);
    
}

u16 readCompressedCharacter(u16 index) {

    MessageBox *mb = &messageBoxes[index];
    
    u16 character;
    
    u8 lower;
    u32 higher;
    
    // every 8 characters, store control byte (which determines how to read 8 character stream)
    if ((mb->compressionBitIndex % 8) == 0) {
        mb->currentCompressionControlByte = *mb->currentCharPtr;        
        mb->currentCharPtr++;
    }
    
    // get two-byte value if needed
    if (mb->currentCompressionControlByte & controlByteBitMasks[mb->compressionBitIndex % 8]) {
        
        lower = *mb->currentCharPtr;
        mb->currentCharPtr++;
        
        higher = *mb->currentCharPtr;
        mb->currentCharPtr++;
        
        character = (lower | (higher << 8));
        
    } else {
        character = *mb->currentCharPtr; 
        mb->currentCharPtr++;
    }

    // only reset on new text
    mb->compressionBitIndex++;

    return character;
    
}

u8 readGameVariableChar(u16 index) {

    MessageBox *mb = &messageBoxes[index];
    GameVariableString *gvs = &gameVariableStrings[mb->gameVariableStringIndex];

    u8 result;

    result = gvs->ptr[mb->gameVariableStringLength];

    mb->gameVariableStringLength++;

    while (gvs->ptr[mb->gameVariableStringLength] == char_TERMINATOR
        && mb->gameVariableStringLength < gvs->maxLength) {

        mb->gameVariableStringLength++;

    }

    if (mb->gameVariableStringLength == gvs->maxLength) {
        mb->flags &= ~MESSAGE_BOX_PROCESSING_GAME_VARIABLE;
    }

    return result;
    
}

// returns rom address for text
u32 getTextAddress(u16 index, u16 textIndex) {

    MessageBox *mb = &messageBoxes[index];

    u32 result = textAddresses[mb->textAddressesIndex].romTextStart;

    result += textAddresses[mb->textAddressesIndex].indexVaddr[textIndex];

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

Gfx* setupMessageBoxScissor(Gfx* dl, MessageBox* messageBox) {

    u16 ulx, uly, lrx, lry;
    u16 temp;
    
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

    gDPSetScissor(dl++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    
    *dl++ = D_8011EEA8;

    return dl++;
    
}

Gfx* renderMessageBoxLine(Gfx* dl, MessageBox* messageBox, u8 lineNumber, s32 arg3) {

    FontBitmap sprite;
    
    u16 charOffset;

    u32 flags;

    u16 characterCount = 0;
    u8 i = 0;
    
    while (i < messageBox->textBoxLineCharWidth) {

        charOffset = i + (lineNumber * messageBox->textBoxLineCharWidth);
        
        unpackFontCI2Data(
            textBuffer[messageBox->totalCharactersProcessed + charOffset], 
            &messageBox->fontContext.compressedCI2FontData, 
            fontTexturesBuffer[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset].timg
        );
        
        if (messageBox->flags & MESSAGE_BOX_INITIALIZED) {

            flags = (BITMAP_ANCHOR_H(SPRITE_ANCHOR_CENTER) | BITMAP_ANCHOR_V(SPRITE_ANCHOR_CENTER) | BITMAP_AXIS(SPRITE_BILLBOARD_XY) | BITMAP_BLEND(SPRITE_BLEND_OPAQUE));

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
                messageBox->targetRGBA.r,
                messageBox->targetRGBA.g,
                messageBox->targetRGBA.b,
                messageBox->targetRGBA.a
            );

            sprite.timg = fontTexturesBuffer[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset].timg;

            setBitmapFormatAndSize((BitmapObject*)&sprite, messageBox->fontContext.fontPalettePtr);

            sprite.width = messageBox->fontContext.characterCellWidth;
            sprite.height = messageBox->fontContext.characterCellHeight;
            sprite.fmt = 2; // CI2
            sprite.pixelSize = 0;

            dl = loadBitmapTexture(dl, (BitmapObject*)&sprite, 0, sprite.height);
            
            gSPVertex(dl++, &fontVertices[gGraphicsBufferIndex][messageBox->totalCharactersProcessed + charOffset][0], 4, 0);

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

void addMessageCharSceneNode(MessageBox* messageBox, u8 line, Gfx* dl) {

    Vec3f vec;

    f32 f1, f2;
    u16 sceneNodeIndex;
    
    if (messageBox->flags & MESSAGE_BOX_SCROLLING_UP) {
        f1 = messageBox->fontContext.characterCellHeight + messageBox->lineSpacing;
    } else {
        f1 = 0.0f;
    }

    if (messageBox->flags & MESSAGE_BOX_SCROLLING_UP) {
        f2 = -(f32)getInterpolatorValue((Interpolator*)&messageBox->scrollInterpolator);
    } else {
        f2 = getInterpolatorValue((Interpolator*)&messageBox->scrollInterpolator);
    }
    
    sceneNodeIndex = addSceneNode(dl, (0x8 | SCENE_NODE_TRANSFORM_EXEMPT));
    
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

    MessageBox *mb = &messageBoxes[i];
    
    // bug: never initialized
    u8 count;

    if (mb->targetRGBA.r < mb->currentRGBA.r) {

        mb->targetRGBA.r += mb->deltaRGBA.r;
        
            if (mb->targetRGBA.r >= mb->currentRGBA.r) {
                mb->targetRGBA.r = mb->currentRGBA.r;
            } else {
                count++;
        }

    }

    if (mb->targetRGBA.r > mb->currentRGBA.r) {
        
        mb->targetRGBA.r -= mb->deltaRGBA.r;

        if (mb->targetRGBA.r <= mb->currentRGBA.r) {
                mb->targetRGBA.r = mb->currentRGBA.r;
            } else {
                count++;
            }
            
    }
    
    if (mb->targetRGBA.g < mb->currentRGBA.g) {
        
        mb->targetRGBA.g += mb->deltaRGBA.g;
        
        if (mb->targetRGBA.g >= mb->currentRGBA.g) {
            mb->targetRGBA.g = mb->currentRGBA.g;
            } else {
                count++;
            }
            
    }

    if (mb->targetRGBA.g > mb->currentRGBA.g) {
        
        mb->targetRGBA.g -= mb->deltaRGBA.g;
        
        if (mb->targetRGBA.g <= mb->currentRGBA.g) {
            mb->targetRGBA.g = mb->currentRGBA.g;
        } else {
            count++;
        }
        
    }

    if (mb->targetRGBA.b < mb->currentRGBA.b) {
        
        mb->targetRGBA.b += mb->deltaRGBA.b;
        
        if (mb->targetRGBA.b >= mb->currentRGBA.b) {
            mb->targetRGBA.b = mb->currentRGBA.b;
        } else {
            count++;
        }
    
    }

    if (mb->targetRGBA.b > mb->currentRGBA.b) {
        
        mb->targetRGBA.b -= mb->deltaRGBA.b;
        
        if (mb->targetRGBA.b <= mb->currentRGBA.b) {
            mb->targetRGBA.b = mb->currentRGBA.b;
        } else {
            count++;
        }
        
    }
    
    if (mb->targetRGBA.a < mb->currentRGBA.a) {

    mb->targetRGBA.a += mb->deltaRGBA.a;

        if (mb->targetRGBA.a >= mb->currentRGBA.a) {
            mb->targetRGBA.a = mb->currentRGBA.a;
        } else {
            count++;
        }
        
    }
    
    if (mb->targetRGBA.a > mb->currentRGBA.a) {
        
        mb->targetRGBA.a -= mb->deltaRGBA.a;

        if (mb->targetRGBA.a <= mb->currentRGBA.a) {
            mb->targetRGBA.a = mb->currentRGBA.a;
        } else {
            count++;
    }
    
    }
    
    return count;
    
}

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
    
    for (i = 0; i < MAX_MESSAGE_BOXES; i++) {

        MessageBox *mb = &messageBoxes[i];

        if (mb->flags & MESSAGE_BOX_ACTIVE) {

            if (updateMessageBoxRGBA(i) == 0) {
                mb->flags |= MESSAGE_BOX_RGBA_COMPLETE;
            } 

            mb->totalCharactersProcessed = totalProcessedCharacterCount;

            if (!(mb->flags & MESSAGE_BOX_TEXT_COMPLETE) && (mb->flags & MESSAGE_BOX_INITIALIZED)) {

                if (!(mb->flags & MESSAGE_BOX_TEXT_END_REACHED)) {

                    // reset message buffer
                    clearMessageBoxTextBuffer(i);

                    if (mb->flags & MESSAGE_BOX_SCROLLING_DOWN) {
                        updateScrollDownAnimation(i);
                    }                    

                    if (mb->flags & MESSAGE_BOX_SCROLLING_UP) {
                        updateScrollUpAnimation(i);
                    }    

                    processText(i);

                    if (!(mb->flags & (MESSAGE_BOX_SCROLLING_DOWN | MESSAGE_BOX_SCROLLING_UP))) {

                        temp = calculateInterpolatorStep((Interpolator*)&mb->unk_64);
                        j = 0;
                        
                        if (checkButtonPressed(CONTROLLER_1, mb->buttonMask)) {

                            if (!(mb->flags & MESSAGE_BOX_MODE_NO_INPUT)) {
                                mb->flags |= MESSAGE_BOX_BUTTON_PRESSED;
                            }
                            
                        }

                        if (checkButtonHeld(CONTROLLER_1, mb->buttonMask) && !(mb->flags & MESSAGE_BOX_SILENT)) {
                            temp = 1;
                        }

                        while (j < temp) {

                            if (mb->flags & MESSAGE_BOX_NEEDS_INTERPOLATOR_INIT) {
                                initializeInterpolator((Interpolator*)&mb->unk_64, mb->unk_7C, 0);
                                mb->flags &= ~MESSAGE_BOX_NEEDS_INTERPOLATOR_INIT;    
                            }

                            updateMessageBoxText(i);
                            mb->flags &= ~MESSAGE_BOX_BUTTON_PRESSED;    
                            j++;
                            
                        }
                        
                    }

        }
                
            }

            mb->charactersProcessedPerLine = 0;

            // divide by max lines per text box
            extraLines = ((mb->flags >> 4) & 3) / 3;

            dlStartingPosition = dl;
            dl = setupMessageBoxScissor(dl, mb);

            k = 0;
            
            if (mb->textBoxVisibleRows + extraLines) {

                do {

                    dl = renderMessageBoxLine(dl, mb, k, totalProcessedCharacterCount);
                    addMessageCharSceneNode(mb, k, dlStartingPosition);
                    
                    dlStartingPosition = dl;
                    k++;
                    
                } while (k < mb->textBoxVisibleRows + extraLines);
                
            }

            // updated by renderMessageBoxLine
            totalProcessedCharacterCount += mb->charactersProcessedPerLine;
            
        }
        
    }

#ifdef DEBUG
    if (dl - messageBoxDisplayList[gGraphicsBufferIndex] >= 3072) {
        __assert("EX", "s:/system/message.c", 2524);
    }
#endif
    
}