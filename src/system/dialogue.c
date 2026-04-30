#include "common.h"

#include "system/audio.h"
#include "system/controller.h" 
#include "system/dialogue.h"
#include "system/math.h"
#include "system/message.h"
#include "system/globalSprites.h"
#include "system/sprite.h"

#include "game/game.h"

#include "ld_symbols.h"

// bss
// u32* specialDialogueBitsPointer;
struct {
    u32* ptr;
    u32 pad;
} specialDialogueBitsWrapper;

#define specialDialogueBitsPointer (specialDialogueBitsWrapper.ptr)

Dialogue dialogues[MAX_DIALOGUES];
// game state updated by/talked about in dialogues
DialogueVariable dialogueVariables[MAX_DIALOGUE_VARIABLES];
DialogueBytecodeAddressInfo dialogueBytecodeAddresses[MAX_DIALOGUE_BYTECODE_BANKS];

// forward declarations
u32 getDialogueBytecodeAddress(u16, u16);
void cleanupDialogueOverlayBox(u16);
void setOverlayMessageBoxSprite(u16 index);
void updateDialogueButtonIcon2Display(u16 index);
void updateDialogueButtonIcon3Display(u16 index);
void handleMenuNavigation(u16);
void updateCurrentDialogue(u16);

void initializeDialogueSessionManagers(void) {
 
    u16 i;
 
    for (i = 0; i < MAX_DIALOGUES; i++) {
        Dialogue *d = &dialogues[i];
        d->sessionManager.dialogueIndex = 0;
        d->sessionManager.dialogueBytecodeAddressesIndex = 0;
        d->sessionManager.mainMessageBoxIndex = 0;
        d->sessionManager.scrollSfxIndex = 0xFF;
        d->sessionManager.closeSfxIndex = 0xFF;
        d->sessionManager.buttonPressSfxIndex = 0xFF;
        d->sessionManager.flags = 0;
    }

} 

bool initializeDialogueSessionManager(u16 index, u16 mainMessageBoxIndex, u16 overlayMessageBoxIndex) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;
 
    if (index == 0 && !(d->sessionManager.flags & DIALOGUE_ACTIVE)) {

        d->sessionManager.scrollSfxIndex = 0xFF;
        d->sessionManager.closeSfxIndex = 0xFF;
        d->sessionManager.buttonPressSfxIndex = 0xFF;
        d->sessionManager.mainMessageBoxIndex = mainMessageBoxIndex;
        d->sessionManager.overlayMessageBoxIndex = overlayMessageBoxIndex;

        d->sessionManager.flags = 1;
        
        result = TRUE;
        
    }
    
    return result;
    
}

bool setDialogueBytecodeAddressInfo(u16 index, u16 textAddressesIndex, u16 selectionMenuTextAddressesIndex, u32 romIndexStart, u32 romIndexEnd, u32* vaddrIndex, u32 romStart, void* vaddr) {

    DialogueBytecodeAddressInfo *dba = &dialogueBytecodeAddresses[index];

    bool result = FALSE;

    if (index < MAX_BYTECODE_ADDRESSES) {

        dba->textAddressesIndex = textAddressesIndex;
        dba->selectionMenuTextAddressesIndex = selectionMenuTextAddressesIndex;
        dba->romIndexStart = romIndexStart;
        dba->romIndexEnd = romIndexEnd;
        dba->vaddrIndex = vaddrIndex;
        dba->romStart = romStart;
        dba->vaddr = vaddr;
        
        result = TRUE;

    } 
    
    return result;

}

bool setDialogueVariable(u16 index, void *value, u8 type, s32 max) {

    DialogueVariable *dv = &dialogueVariables[index];

    bool result = FALSE;

    if (index < TOTAL_GAME_VARIABLES) {

        if (type < 5) {

            dv->value = value;
            dv->type = type;
            dv->maxValue = max;

            result = TRUE;

        }

    }
 
    return result;

}

bool setSpecialDialogueBitsPointer(u32* dialogueBitsPointer) {
    specialDialogueBitsPointer = dialogueBitsPointer;
    return 0;
}

bool setDialogueSfxIndices(u16 index, u32 scrollSfxIndex, u32 closeSfxIndex, u32 sfxIndex) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;

    if (index == 0 && d->sessionManager.flags & DIALOGUE_ACTIVE) {
        
        d->sessionManager.scrollSfxIndex = scrollSfxIndex;
        d->sessionManager.closeSfxIndex = closeSfxIndex;
        d->sessionManager.buttonPressSfxIndex = sfxIndex;
        
        result = TRUE;

    }
    
    return result;
    
}

bool setDialogueButtonIcon1(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;

    if (index == 0 && (d->sessionManager.flags & DIALOGUE_ACTIVE)) {
  
        d->dialogueButtonIcon1.romTextureStart = romTextureStart;
        d->dialogueButtonIcon1.romTextureEnd = romTextureEnd;
        d->dialogueButtonIcon1.romIndexStart = romIndexStart;
        d->dialogueButtonIcon1.romIndexEnd = romIndexEnd;
  
        d->dialogueButtonIcon1.vaddrTexture = vaddrTexture;
        d->dialogueButtonIcon1.vaddrPalette = vaddrTextureEnd;
        d->dialogueButtonIcon1.vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        d->dialogueButtonIcon1.vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;
        d->dialogueButtonIcon1.unk_20 = argA;
 
        d->dialogueButtonIcon1.spriteIndex = spriteIndex;
        d->dialogueButtonIcon1.spriteOffset = spriteOffset;
        d->dialogueButtonIcon1.flag = flag;

        d->dialogueButtonIcon1.coordinates.x = x;
        d->dialogueButtonIcon1.coordinates.y = y;
        d->dialogueButtonIcon1.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

bool setDialogueButtonIcon2(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;

    if (index == 0 && (d->sessionManager.flags & DIALOGUE_ACTIVE)) {
  
        d->dialogueButtonIcon2.romTextureStart = romTextureStart;
        d->dialogueButtonIcon2.romTextureEnd = romTextureEnd;
        d->dialogueButtonIcon2.romIndexStart = romIndexStart;
        d->dialogueButtonIcon2.romIndexEnd = romIndexEnd;
  
        d->dialogueButtonIcon2.vaddrTexture = vaddrTexture;
        d->dialogueButtonIcon2.vaddrPalette = vaddrTextureEnd;
        d->dialogueButtonIcon2.vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        d->dialogueButtonIcon2.vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;
        d->dialogueButtonIcon2.unk_20 = argA;
 
        d->dialogueButtonIcon2.spriteIndex = spriteIndex;
        d->dialogueButtonIcon2.spriteOffset = spriteOffset;
        d->dialogueButtonIcon2.flag = flag;

        d->dialogueButtonIcon2.coordinates.x = x;
        d->dialogueButtonIcon2.coordinates.y = y;
        d->dialogueButtonIcon2.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

bool setDialogueButtonIcon3(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;

    if (index == 0 && (d->sessionManager.flags & DIALOGUE_ACTIVE)) {
        
        d->dialogueButtonIcon3.romTextureStart = romTextureStart;
        d->dialogueButtonIcon3.romTextureEnd = romTextureEnd;
        d->dialogueButtonIcon3.romIndexStart = romIndexStart;
        d->dialogueButtonIcon3.romIndexEnd = romIndexEnd;
  
        d->dialogueButtonIcon3.vaddrTexture = vaddrTexture;
        d->dialogueButtonIcon3.vaddrPalette = vaddrTextureEnd;
        d->dialogueButtonIcon3.vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        d->dialogueButtonIcon3.vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;
        d->dialogueButtonIcon3.unk_20 = argA;
 
        d->dialogueButtonIcon3.spriteIndex = spriteIndex;
        d->dialogueButtonIcon3.spriteOffset = spriteOffset;
        d->dialogueButtonIcon3.flag = flag;

        d->dialogueButtonIcon3.coordinates.x = x;
        d->dialogueButtonIcon3.coordinates.y = y;
        d->dialogueButtonIcon3.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

bool initializeDialogueSession(u16 index, u16 dialogueBytecodeAddressesIndex, u16 dialogueIndex, u16 flag) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;

    u32 romAddr;

    if (index == 0 && d->sessionManager.flags & DIALOGUE_ACTIVE) {

        dmaSprite(d->dialogueButtonIcon1.spriteIndex, 
            d->dialogueButtonIcon1.romTextureStart, 
            d->dialogueButtonIcon1.romTextureEnd, 
            d->dialogueButtonIcon1.romIndexStart, 
            d->dialogueButtonIcon1.romIndexEnd, 
            NULL, 
            NULL, 
            d->dialogueButtonIcon1.vaddrTexture,
            NULL,
            d->dialogueButtonIcon1.vaddrPalette,
            d->dialogueButtonIcon1.vaddrAnimationFrameMetadata,
            d->dialogueButtonIcon1.vaddrTextureToPaletteLookup,
            d->dialogueButtonIcon1.unk_20,
            0,
            0);
 
        setSpriteViewSpacePosition(d->dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(d->dialogueButtonIcon1.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(d->dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(d->dialogueButtonIcon1.spriteIndex, TRUE);
        setSpriteColor(d->dialogueButtonIcon1.spriteIndex, 255, 255, 255, 255);
        setSpriteAnchorAlignment(d->dialogueButtonIcon1.spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
        setSpriteBlendMode(d->dialogueButtonIcon1.spriteIndex, SPRITE_BLEND_ALPHA_DECAL);

        dmaSprite(d->dialogueButtonIcon2.spriteIndex, 
            d->dialogueButtonIcon2.romTextureStart, 
            d->dialogueButtonIcon2.romTextureEnd, 
            d->dialogueButtonIcon2.romIndexStart, 
            d->dialogueButtonIcon2.romIndexEnd, 
            NULL, 
            NULL, 
            d->dialogueButtonIcon2.vaddrTexture,
            NULL,
            d->dialogueButtonIcon2.vaddrPalette,
            d->dialogueButtonIcon2.vaddrAnimationFrameMetadata,
            d->dialogueButtonIcon2.vaddrTextureToPaletteLookup,
            d->dialogueButtonIcon2.unk_20,
            0,
            0);

        setSpriteViewSpacePosition(d->dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(d->dialogueButtonIcon2.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(d->dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(d->dialogueButtonIcon2.spriteIndex, TRUE);
        setSpriteColor(d->dialogueButtonIcon2.spriteIndex, 255, 255, 255, 255);
        setSpriteAnchorAlignment(d->dialogueButtonIcon2.spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
        setSpriteBlendMode(d->dialogueButtonIcon2.spriteIndex, SPRITE_BLEND_ALPHA_DECAL);

        dmaSprite(d->dialogueButtonIcon3.spriteIndex, 
            d->dialogueButtonIcon3.romTextureStart, 
            d->dialogueButtonIcon3.romTextureEnd, 
            d->dialogueButtonIcon3.romIndexStart, 
            d->dialogueButtonIcon3.romIndexEnd, 
            NULL, 
            NULL, 
            d->dialogueButtonIcon3.vaddrTexture,
            NULL,
            d->dialogueButtonIcon3.vaddrPalette,
            d->dialogueButtonIcon3.vaddrAnimationFrameMetadata,
            d->dialogueButtonIcon3.vaddrTextureToPaletteLookup,
            d->dialogueButtonIcon3.unk_20,
            0,
            0);

        setSpriteViewSpacePosition(d->dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(d->dialogueButtonIcon3.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(d->dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(d->dialogueButtonIcon3.spriteIndex, TRUE);
        setSpriteColor(d->dialogueButtonIcon3.spriteIndex, 255, 255, 255, 255);
        setSpriteAnchorAlignment(d->dialogueButtonIcon3.spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
        setSpriteBlendMode(d->dialogueButtonIcon3.spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
        
        d->bytecodeExecutor.currentOpcode = 0xFF;
        d->sessionManager.dialogueBytecodeAddressesIndex = dialogueBytecodeAddressesIndex;
        d->sessionManager.dialogueIndex = dialogueIndex;

        nuPiReadRom(dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexStart, dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].vaddrIndex, dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexEnd - dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexStart);

        romAddr = getDialogueBytecodeAddress(0, d->sessionManager.dialogueIndex);

        nuPiReadRom(romAddr, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].vaddr, getDialogueBytecodeAddress(0, d->sessionManager.dialogueIndex + 1) - romAddr);

        d->dialogueBytecodePointer = dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].vaddr;
        
        d->sessionManager.totalMenuRows = 0;
        d->sessionManager.selectedMenuRow = 0;
 
        d->bytecodeExecutor.textIndex = 0;
        d->bytecodeExecutor.branchingDialogueIndex = 0;
        d->bytecodeExecutor.minimumDialogueVariableValue = 0;
        d->bytecodeExecutor.maximumDialogueVariableValue = 0;
        d->bytecodeExecutor.randomValue = 0;
        d->bytecodeExecutor.dialogueVariableValue = 0;
        d->bytecodeExecutor.specialDialogueBit = 0;
        d->bytecodeExecutor.updatedDialogueVariableAdjustment = 0;
        d->bytecodeExecutor.textOffset = 0;
        d->bytecodeExecutor.dialogueVariablesIndex = 0;
        d->bytecodeExecutor.randomMinimumValue = 0;
        d->bytecodeExecutor.randomMaximumValue = 0;
        d->bytecodeExecutor.unusedField = 0;
        d->bytecodeExecutor.selectionMenuRowsCount = 0;
        d->bytecodeExecutor.targetMenuRow = 0;
        
        d->sessionManager.flags &= ~4;
        d->sessionManager.flags |= DIALOGUE_INITIALIZED;

        if (flag == 0x40) {
            d->sessionManager.flags |= (DIALOGUE_INITIALIZED | 0x40);
        }  else {
            d->sessionManager.flags &= ~(0x40);
        }

        if (flag == 0x80) {
            d->sessionManager.flags |= 0x80;
        } else {
            d->sessionManager.flags &= ~0x80;
        }

        result = TRUE;

    }

    return result;
    
}

bool checkAllDialoguesCompleted(void) {

    bool result = FALSE;
    u16 i;

    for (i = 0; i < 1; i++) {
        if (dialogues[i].sessionManager.flags & DIALOGUE_COMPLETED) {
            result = TRUE;
        }
    }

    return result;
    
}

bool closeDialogueSession(u16 index) {

    Dialogue *d = &dialogues[index];

    bool result = FALSE;

    if (index == 0 && d->sessionManager.flags & DIALOGUE_ACTIVE) {

        if (!(d->sessionManager.flags & 0x40)) {
            messageBoxes[d->sessionManager.mainMessageBoxIndex].flags &= ~MESSAGE_BOX_MODE_UNKNOWN;
        }

        resetMessageBoxAnimation(d->sessionManager.mainMessageBoxIndex);
        cleanupDialogueOverlayBox(0);
        
        d->sessionManager.flags = (DIALOGUE_ACTIVE | DIALOGUE_COMPLETED);
        
        result = TRUE;

    }

    return result;
    
}

void cleanupDialogueOverlayBox(u16 index) {

    Dialogue *d = &dialogues[index];

    messageBoxes[d->sessionManager.overlayMessageBoxIndex].flags &= ~MESSAGE_BOX_MODE_UNKNOWN;
    
    resetMessageBoxAnimation(d->sessionManager.overlayMessageBoxIndex);
    
    resetAnimationState(d->dialogueButtonIcon1.spriteIndex);
    resetAnimationState(d->dialogueButtonIcon2.spriteIndex);
    resetAnimationState(d->dialogueButtonIcon3.spriteIndex);
    
    d->sessionManager.flags &= ~DIALOGUE_PAUSE_FOR_USER_INPUT;
    
}

u8 getSelectedMenuRow(u16 index) {

    Dialogue *d = &dialogues[index];
    return d->sessionManager.selectedMenuRow;
}

u32 getDialogueBytecodeAddress(u16 index, u16 dialogueOffset) {

    Dialogue *d = &dialogues[index];

    u32 ptr = dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].romStart;
    
    return ptr + dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].vaddrIndex[dialogueOffset];
    
}

inline void setDialogueVariableValue(u16 index, u16 value) {

    DialogueVariable *dv = &dialogueVariables[index];

    switch (dv->type) {
        case UNSIGNED_CHAR:
            *(u8*)dv->value = value;
            break;
        case UNSIGNED_SHORT:
            *(u16*)dv->value = value;
            break;
        case UNSIGNED_INT:
            *(u32*)dv->value = value;
            break;
    }
    
}

inline u32 getDialogueVariableValue(u16 index) {

    DialogueVariable *dv = &dialogueVariables[index];

    u32 value;
    
    switch (dv->type) {
        case UNSIGNED_CHAR:
            value = *(u8*)dv->value;
            break;
        case UNSIGNED_SHORT:
            value = *(u16*)dv->value;
            break;
        case UNSIGNED_INT:
            value = *(u32*)dv->value;
            break;
    }

    return value;

}

inline void setSpecialDialogueBitFromPointer(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] |= 1 << (temp & 0x1F);
    
}

inline void clearSpecialDialogueBitFromPointer(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] &= ~(1 << (temp & 0x1F));

}

inline u32 checkSpecialDialogueBitFromPointer(u16 bitIndex) {
    
    u32 temp = bitIndex;
    
    return !(specialDialogueBitsPointer[temp >> 5] & (1 << (temp & 0x1F))) == 0;

}

void parseDialogueBytecode(u16 index) {

    Dialogue *d = &dialogues[index];

    Swap16 byteswap;

    d->bytecodeExecutor.currentOpcode = *(u8*)d->dialogueBytecodePointer++;

    switch (d->bytecodeExecutor.currentOpcode) {
        
        case DIALOGUE_OPCODE_SHOW_TEXT:
                        
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;            
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.textIndex = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_DIALOGUE_VARIABLE_BRANCH:
            
            d->bytecodeExecutor.dialogueVariablesIndex = *(u8*)d->dialogueBytecodePointer++;  

            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.minimumDialogueVariableValue = byteswap.halfword;
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.maximumDialogueVariableValue = byteswap.halfword;
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.textIndex = byteswap.halfword;
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_UPDATE_DIALOGUE_VARIABLE:
            
            d->bytecodeExecutor.dialogueVariablesIndex = *(u8*)d->dialogueBytecodePointer++;

            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.updatedDialogueVariableAdjustment = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_SET_DIALOGUE_VARIABLE:
            
            d->bytecodeExecutor.dialogueVariablesIndex = *(u8*)d->dialogueBytecodePointer++;  

            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;

            d->bytecodeExecutor.dialogueVariableValue = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_SPECIAL_DIALOGUE_BIT_BRANCH:
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.specialDialogueBit = byteswap.halfword;
                        
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.textIndex = byteswap.halfword;
                        
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_SET_SPECIAL_DIALOGUE_BIT:
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.specialDialogueBit = byteswap.halfword;  
            
            break;

        case DIALOGUE_OPCODE_CLEAR_SPECIAL_DIALOGUE_BIT:
                    
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.specialDialogueBit = byteswap.halfword;
            
            break;

        case DIALOGUE_OPCODE_RANDOM_BRANCH:
            
            d->bytecodeExecutor.randomMinimumValue = *(u8*)d->dialogueBytecodePointer++;
            d->bytecodeExecutor.randomMaximumValue = *(u8*)d->dialogueBytecodePointer++;
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.textIndex = byteswap.halfword;
                        
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
    
            d->bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;
            
            d->bytecodeExecutor.randomValue = getRandomNumberInRange(0, 99);  
            
            break;

        case DIALOGUE_OPCODE_BRANCH:
             
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;  
            
            break;
        
        case DIALOGUE_OPCODE_SHOW_SELECTION_MENU:
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.textOffset = byteswap.halfword;
            // unused
            d->bytecodeExecutor.selectionMenuRowsCount = *(u8*)d->dialogueBytecodePointer++;
            
            break;
    
        case DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH:
            
            // row
            d->bytecodeExecutor.targetMenuRow = *(u8*)d->dialogueBytecodePointer++;
            
            byteswap.byte[1] = *(u8*)d->dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)d->dialogueBytecodePointer++;
            
            d->bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;  
            
            break;

        case DIALOGUE_OPCODE_END_DIALOGUE:
            break;
    
    }
    
}

void setOverlayMessageBoxSprite(u16 index) {

    Dialogue *d = &dialogues[index];

    f32 xPosition, yPosition;

    xPosition = -((messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxLineCharWidth * messageBoxes[d->sessionManager.overlayMessageBoxIndex].fontContext.characterCellWidth) / 2) 
        - ((messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxLineCharWidth * messageBoxes[d->sessionManager.overlayMessageBoxIndex].characterSpacing) / 2) 
        - d->dialogueButtonIcon1.coordinates.x;

    yPosition = ((messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * messageBoxes[d->sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight) / 2) 
        + ((messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * messageBoxes[d->sessionManager.overlayMessageBoxIndex].lineSpacing) / 2) 
        + d->dialogueButtonIcon1.coordinates.y;

    setSpriteScale(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 
       messageBoxes[d->sessionManager.overlayMessageBoxIndex].maxCharsPerLine * 0.5f, 
       messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * 0.6f, 
       1.0f);

    setSpriteBlendMode(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 255, 255, 255, 192);
    setBilinearFiltering(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);
    startSpriteAnimation(d->dialogueButtonIcon1.spriteIndex, d->dialogueButtonIcon1.spriteOffset, d->dialogueButtonIcon1.flag);
    setSpriteViewSpacePosition(d->dialogueButtonIcon1.spriteIndex, 
       messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + xPosition, 
       messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + yPosition, 
       messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

void updateDialogueButtonIcon2Display(u16 index) {

    Dialogue *d = &dialogues[index];

    f32 tempX = d->dialogueButtonIcon2.coordinates.x;
    f32 tempY = d->dialogueButtonIcon2.coordinates.y;

    setSpriteScale(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, messageBoxes[d->sessionManager.overlayMessageBoxIndex].maxCharsPerLine * 0.5f, messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * 0.6f, 1.0f);
    
    setSpriteBlendMode(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
    setBilinearFiltering(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);

    startSpriteAnimation(d->dialogueButtonIcon2.spriteIndex, d->dialogueButtonIcon2.spriteOffset, d->dialogueButtonIcon2.flag);
    setSpriteViewSpacePosition(d->dialogueButtonIcon2.spriteIndex, messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + tempX, messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + tempY, messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

void updateDialogueButtonIcon3Display(u16 index) {

    Dialogue *d = &dialogues[index];
 
    f32 tempX = d->dialogueButtonIcon3.coordinates.x;
    f32 tempY = d->dialogueButtonIcon3.coordinates.y;

    setSpriteBlendMode(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
    setBilinearFiltering(dialogueWindows[messageBoxes[d->sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 1);

    startSpriteAnimation(d->dialogueButtonIcon3.spriteIndex, d->dialogueButtonIcon3.spriteOffset, d->dialogueButtonIcon3.flag);
    setSpriteViewSpacePosition(d->dialogueButtonIcon3.spriteIndex, messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + tempX, messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + tempY, messageBoxes[d->sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

void handleMenuNavigation(u16 index) {

    Dialogue *d = &dialogues[index];

    bool set = FALSE;

    if (!(messageBoxes[d->sessionManager.overlayMessageBoxIndex].flags & (MESSAGE_BOX_SCROLLING_DOWN | MESSAGE_BOX_SCROLLING_UP))) { 

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {

            if (d->sessionManager.selectedMenuRow < (d->sessionManager.totalMenuRows - 1)) {

                d->sessionManager.selectedMenuRow++;

                if (d->sessionManager.visibleMenuCursorRow != messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows - 1) {
                    d->sessionManager.visibleMenuCursorRow++;
                    adjustSpriteViewSpacePosition(d->dialogueButtonIcon1.spriteIndex, 0.0f, -messageBoxes[d->sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight - messageBoxes[d->sessionManager.overlayMessageBoxIndex].lineSpacing, 0.0f);
                } else {
                    scrollMessageBoxDown(d->sessionManager.overlayMessageBoxIndex);
                    d->sessionManager.menuScrollOffset++;
                }

                if (d->sessionManager.scrollSfxIndex != 0xFF) {
                    setSfx(d->sessionManager.scrollSfxIndex + 1);
                    setSfxVolume(d->sessionManager.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                }
                
                set = TRUE;
                
            }

        }

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {

            if (!set) {

                if (d->sessionManager.selectedMenuRow) {

                    d->sessionManager.selectedMenuRow--;
                    
                    if (d->sessionManager.visibleMenuCursorRow) {
                        d->sessionManager.visibleMenuCursorRow--;
                        adjustSpriteViewSpacePosition(d->dialogueButtonIcon1.spriteIndex, 0.0f, messageBoxes[d->sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight + messageBoxes[d->sessionManager.overlayMessageBoxIndex].lineSpacing, 0.0f);
                    } else {
                        scrollMessageBoxUp(d->sessionManager.overlayMessageBoxIndex);
                        d->sessionManager.menuScrollOffset--;
                    }

                    if (d->sessionManager.scrollSfxIndex != 0xFF) {
                        setSfx(d->sessionManager.scrollSfxIndex + 1);
                        setSfxVolume(d->sessionManager.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                        
                    }
                    
                    set = TRUE;
                
                }

            }

        }

        if (d->sessionManager.totalMenuRows > messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows) { 

            if (messageBoxes[d->sessionManager.overlayMessageBoxIndex].textBoxVisibleRows < (d->sessionManager.totalMenuRows - d->sessionManager.menuScrollOffset)) {
                updateDialogueButtonIcon3Display(index);
            } else {
                resetAnimationState(d->dialogueButtonIcon3.spriteIndex);
            }

            if (d->sessionManager.menuScrollOffset) {
                updateDialogueButtonIcon2Display(index);
            } else {
                resetAnimationState(d->dialogueButtonIcon2.spriteIndex);
            }

        }

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

            if (!set) {
                
                cleanupDialogueOverlayBox(index);
                d->bytecodeExecutor.currentOpcode = 0xFF;
                
                if (d->sessionManager.closeSfxIndex != 0xFF) {
                    setSfx(d->sessionManager.closeSfxIndex + 1);
                    setSfxVolume(d->sessionManager.closeSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                    set = TRUE;
                }
                
            }

        }
        
    }
   
}

void updateCurrentDialogue(u16 index) {

    Dialogue *d = &dialogues[index];
    
    u16 finishCurrentDialogueBlockProcessing = FALSE;
    
    u16 dialogueVariableValue;
    u16 totalLines;
    u16 randomValue;
    u16 selectedMenuRow;

    u16 textOrDialogueIndex;


    // loop and continue to process new opcodes unless marked as done
    while (!finishCurrentDialogueBlockProcessing) {
        
        // currentOpcode is reset to 0xFF after processing
        if (d->bytecodeExecutor.currentOpcode == 0xFF) {
            parseDialogueBytecode(index);
        }
        
        switch (d->bytecodeExecutor.currentOpcode) {
        
            case DIALOGUE_OPCODE_SHOW_TEXT:

                if (d->sessionManager.flags & 0x80) {
                    initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, d->bytecodeExecutor.textIndex, 0);
                } else {
                    initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, d->bytecodeExecutor.textIndex, MESSAGE_BOX_MODE_UNKNOWN);
                }
                
                finishCurrentDialogueBlockProcessing = TRUE;

                d->bytecodeExecutor.currentOpcode = 0xFF;
                d->sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                
                break;

            case DIALOGUE_OPCODE_DIALOGUE_VARIABLE_BRANCH:
                
                dialogueVariableValue = getDialogueVariableValue(d->bytecodeExecutor.dialogueVariablesIndex);
                
                if ((dialogueVariableValue >= d->bytecodeExecutor.minimumDialogueVariableValue) && (d->bytecodeExecutor.maximumDialogueVariableValue >= dialogueVariableValue)) {
                
                    textOrDialogueIndex = d->bytecodeExecutor.textIndex;
                    
                    // if a specific text is set, branch to that; otherwise, branch to another bytecode segment
                    if (textOrDialogueIndex != 0xFFFF) {

                        if (d->sessionManager.flags & 0x80) {
                            initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0);
                        } else {
                            initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0x8000);
                        }
                        
                        finishCurrentDialogueBlockProcessing = TRUE;
                        
                        d->bytecodeExecutor.textIndex = 0xFFFF;
                        d->sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                        
                    // branch to another bytecode segment
                    } else {
                        
                        textOrDialogueIndex = d->bytecodeExecutor.branchingDialogueIndex;
                        
                        if (textOrDialogueIndex != 0xFFFF) {
                            initializeDialogueSession(index, d->sessionManager.dialogueBytecodeAddressesIndex, textOrDialogueIndex, d->sessionManager.flags & (0x40 | 0x80));
                        } else {
                            d->bytecodeExecutor.currentOpcode = 0xFF;
                        }
                        
                    }
                    
                } else {
                    d->bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_UPDATE_DIALOGUE_VARIABLE:

                dialogueVariableValue = getDialogueVariableValue(d->bytecodeExecutor.dialogueVariablesIndex);
                dialogueVariableValue += adjustValue(dialogueVariableValue, d->bytecodeExecutor.updatedDialogueVariableAdjustment, dialogueVariables[d->bytecodeExecutor.dialogueVariablesIndex].maxValue);
                setDialogueVariableValue(d->bytecodeExecutor.dialogueVariablesIndex, dialogueVariableValue);

                d->bytecodeExecutor.currentOpcode = 0xFF;

                break;

            case DIALOGUE_OPCODE_SET_DIALOGUE_VARIABLE:
                setDialogueVariableValue(d->bytecodeExecutor.dialogueVariablesIndex, d->bytecodeExecutor.dialogueVariableValue);
                d->bytecodeExecutor.currentOpcode = 0xFF;
                break;

            case DIALOGUE_OPCODE_SPECIAL_DIALOGUE_BIT_BRANCH:
                
                if (checkSpecialDialogueBitFromPointer(d->bytecodeExecutor.specialDialogueBit)) {
                    
                    textOrDialogueIndex = d->bytecodeExecutor.textIndex;
                
                    // if specific text is set, branch to that; otherwise branch to another bytecode segment
                    if (textOrDialogueIndex != 0xFFFF) {
                        
                        if (d->sessionManager.flags & 0x80) {
                            initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0);
                        } else {
                            initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0x8000);
                        }
                        
                        finishCurrentDialogueBlockProcessing = TRUE;
                        d->bytecodeExecutor.textIndex = 0xFFFF;
                        d->sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                    
                    // branch to another bytecode segment
                    } else {
                        
                        textOrDialogueIndex = d->bytecodeExecutor.branchingDialogueIndex;

                        if (textOrDialogueIndex != 0xFFFF) {
                            initializeDialogueSession(index, d->sessionManager.dialogueBytecodeAddressesIndex, textOrDialogueIndex, d->sessionManager.flags & (0x40 | 0x80));
                        } else {
                            d->bytecodeExecutor.currentOpcode = 0xFF;
                        }

                    }
                    
                } else {
                    d->bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_SET_SPECIAL_DIALOGUE_BIT:
                setSpecialDialogueBitFromPointer(d->bytecodeExecutor.specialDialogueBit);
                d->bytecodeExecutor.currentOpcode = 0xFF;
                break;

            case DIALOGUE_OPCODE_CLEAR_SPECIAL_DIALOGUE_BIT:
                clearSpecialDialogueBitFromPointer(d->bytecodeExecutor.specialDialogueBit);
                d->bytecodeExecutor.currentOpcode = 0xFF;
                break;

            case DIALOGUE_OPCODE_RANDOM_BRANCH:
                
                randomValue = d->bytecodeExecutor.randomValue;

                if ((randomValue >= d->bytecodeExecutor.randomMinimumValue) && (d->bytecodeExecutor.randomMaximumValue >= randomValue)) {
                
                    textOrDialogueIndex = d->bytecodeExecutor.textIndex;
                    
                    if (textOrDialogueIndex != 0xFFFF) {

                        if (d->sessionManager.flags & 0x80) {
                            initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0);
                        } else {
                            initializeMessageBox(d->sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0x8000);
                        }
                        
                        finishCurrentDialogueBlockProcessing = TRUE;

                        d->bytecodeExecutor.textIndex = 0xFFFF;
                        d->sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                        
                    } else {
                        
                        textOrDialogueIndex = d->bytecodeExecutor.branchingDialogueIndex;
                        
                        if (textOrDialogueIndex != 0xFFFF) {
                            initializeDialogueSession(index, d->sessionManager.dialogueBytecodeAddressesIndex, textOrDialogueIndex, d->sessionManager.flags & (0x40 | 0x80));
                        } else {
                            d->bytecodeExecutor.currentOpcode = 0xFF;
                        }
                    }
                    
                } else {
                    d->bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_BRANCH:
                initializeDialogueSession(index, d->sessionManager.dialogueBytecodeAddressesIndex, d->bytecodeExecutor.branchingDialogueIndex, d->sessionManager.flags & (0x40 | 0x80));
                break;

            case DIALOGUE_OPCODE_SHOW_SELECTION_MENU:
                
                d->bytecodeExecutor.currentOpcode = 0xFF;
                d->sessionManager.selectedMenuRow = 0;
                d->sessionManager.visibleMenuCursorRow = 0;
                d->sessionManager.menuScrollOffset = 0;
                d->sessionManager.overlayTextOffset = d->bytecodeExecutor.textOffset;
                d->sessionManager.flags |= DIALOGUE_PAUSE_FOR_USER_INPUT;
                
                initializeMessageBox(d->sessionManager.overlayMessageBoxIndex, dialogueBytecodeAddresses[d->sessionManager.dialogueBytecodeAddressesIndex].selectionMenuTextAddressesIndex, d->sessionManager.overlayTextOffset, 0);
                
                totalLines = messageBoxes[d->sessionManager.overlayMessageBoxIndex].totalLines;

                if (totalLines >= 5) {
                    setMessageBoxLineAndRowSizes(d->sessionManager.overlayMessageBoxIndex, messageBoxes[d->sessionManager.overlayMessageBoxIndex].maxCharsPerLine, 4);
                } else {
                    setMessageBoxLineAndRowSizes(d->sessionManager.overlayMessageBoxIndex, messageBoxes[d->sessionManager.overlayMessageBoxIndex].maxCharsPerLine, totalLines);
                }
                
                d->sessionManager.totalMenuRows = messageBoxes[d->sessionManager.overlayMessageBoxIndex].totalLines;
                
                setOverlayMessageBoxSprite(index);
                
                if (d->sessionManager.buttonPressSfxIndex != 0xFF) {
                    setSfx(d->sessionManager.buttonPressSfxIndex + 1);
                    setSfxVolume(d->sessionManager.buttonPressSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                }
                
                finishCurrentDialogueBlockProcessing = TRUE;
                
                break;

            case DIALOGUE_OPCODE_UNUSED:
                break;

            case DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH:
                
                selectedMenuRow = d->sessionManager.selectedMenuRow;

                if (d->bytecodeExecutor.targetMenuRow == selectedMenuRow) {

                    initializeDialogueSession(index, d->sessionManager.dialogueBytecodeAddressesIndex, d->bytecodeExecutor.branchingDialogueIndex, d->sessionManager.flags & (0x40 | 0x80));
                    // preserve across session re-init
                    d->sessionManager.selectedMenuRow = selectedMenuRow;

                } else {
                    d->bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_END_DIALOGUE:
                finishCurrentDialogueBlockProcessing = TRUE;
                closeDialogueSession(index);
                break;
        
            }

    }
}

void updateDialogues(void) {

    u16 i;
    bool skipDialogueUpdate;

    for (i = 0; i < MAX_DIALOGUES; i++) {
        Dialogue *d = &dialogues[i];

        if ((d->sessionManager.flags & DIALOGUE_ACTIVE) && (d->sessionManager.flags & DIALOGUE_INITIALIZED)) {

            skipDialogueUpdate = FALSE;

            if (d->sessionManager.flags & DIALOGUE_WAIT_FOR_MESSAGE_BOX) {

                if ((messageBoxes[d->sessionManager.mainMessageBoxIndex].flags & MESSAGE_BOX_TEXT_COMPLETE) || (messageBoxes[d->sessionManager.mainMessageBoxIndex].flags & 0x20000)) {
                    d->sessionManager.flags &= ~DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                }

                skipDialogueUpdate = TRUE;

            }

            if (d->sessionManager.flags & DIALOGUE_PAUSE_FOR_USER_INPUT) {
                handleMenuNavigation(i);
                skipDialogueUpdate = TRUE;
            }

            if (!skipDialogueUpdate) {
                updateCurrentDialogue(i);
            }

        }

    }
    
}
