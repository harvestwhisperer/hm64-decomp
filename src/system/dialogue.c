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


//INCLUDE_ASM("asm/nonmatchings/system/dialogue", initializeDialogueSessionManagers);

void initializeDialogueSessionManagers(void) {
 
    u16 i;
 
    for (i = 0; i < MAX_DIALOGUES; i++) {
        dialogues[i].sessionManager.dialogueIndex = 0;
        dialogues[i].sessionManager.dialogueBytecodeAddressesIndex = 0;
        dialogues[i].sessionManager.mainMessageBoxIndex = 0;
        dialogues[i].sessionManager.scrollSfxIndex = 0xFF;
        dialogues[i].sessionManager.closeSfxIndex = 0xFF;
        dialogues[i].sessionManager.buttonPressSfxIndex = 0xFF;
        dialogues[i].sessionManager.flags = 0;
    }

} 

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", initializeDialogueSessionManager);

bool initializeDialogueSessionManager(u16 index, u16 mainMessageBoxIndex, u16 overlayMessageBoxIndex) {

    bool result = FALSE;
 
    if (index == 0 && !(dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {

        dialogues[index].sessionManager.scrollSfxIndex = 0xFF;
        dialogues[index].sessionManager.closeSfxIndex = 0xFF;
        dialogues[index].sessionManager.buttonPressSfxIndex = 0xFF;
        dialogues[index].sessionManager.mainMessageBoxIndex = mainMessageBoxIndex;
        dialogues[index].sessionManager.overlayMessageBoxIndex = overlayMessageBoxIndex;

        dialogues[index].sessionManager.flags = 1;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueBytecodeAddressInfo);

bool setDialogueBytecodeAddressInfo(u16 index, u16 textAddressesIndex, u16 selectionMenuTextAddressesIndex, u32 romIndexStart, u32 romIndexEnd, u32* vaddrIndex, u32 romStart, void* vaddr) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_ADDRESSES) {

        dialogueBytecodeAddresses[index].textAddressesIndex = textAddressesIndex;
        dialogueBytecodeAddresses[index].selectionMenuTextAddressesIndex = selectionMenuTextAddressesIndex;
        dialogueBytecodeAddresses[index].romIndexStart = romIndexStart;
        dialogueBytecodeAddresses[index].romIndexEnd = romIndexEnd;
        dialogueBytecodeAddresses[index].vaddrIndex = vaddrIndex;
        dialogueBytecodeAddresses[index].romStart = romStart;
        dialogueBytecodeAddresses[index].vaddr = vaddr;
        
        result = TRUE;

    } 
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueVariable);

bool setDialogueVariable(u16 index, void *value, u8 type, s32 max) {

    bool result = FALSE;

    if (index < TOTAL_GAME_VARIABLES) {

        if (type < 5) {

            dialogueVariables[index].value = value;
            dialogueVariables[index].type = type;
            dialogueVariables[index].maxValue = max;

            result = TRUE;

        }

    }
 
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setSpecialDialogueBitsPointer);

bool setSpecialDialogueBitsPointer(u32* arg0) {
    specialDialogueBitsPointer = arg0;
    return 0;
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueSfxIndices);

bool setDialogueSfxIndices(u16 index, u32 scrollSfxIndex, u32 closeSfxIndex, u32 sfxIndex) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE) {
        
        dialogues[index].sessionManager.scrollSfxIndex = scrollSfxIndex;
        dialogues[index].sessionManager.closeSfxIndex = closeSfxIndex;
        dialogues[index].sessionManager.buttonPressSfxIndex = sfxIndex;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueButtonIcon1);

bool setDialogueButtonIcon1(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {
  
        dialogues[index].dialogueButtonIcon1.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon1.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon1.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon1.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon1.vaddrTexture = vaddrTexture;
        dialogues[index].dialogueButtonIcon1.vaddrPalette = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon1.vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        dialogues[index].dialogueButtonIcon1.vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;
        dialogues[index].dialogueButtonIcon1.unk_20 = argA;
 
        dialogues[index].dialogueButtonIcon1.spriteIndex = spriteIndex;
        dialogues[index].dialogueButtonIcon1.spriteOffset = spriteOffset;
        dialogues[index].dialogueButtonIcon1.flag = flag;

        dialogues[index].dialogueButtonIcon1.coordinates.x = x;
        dialogues[index].dialogueButtonIcon1.coordinates.y = y;
        dialogues[index].dialogueButtonIcon1.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueButtonIcon2);

bool setDialogueButtonIcon2(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {
  
        dialogues[index].dialogueButtonIcon2.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon2.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon2.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon2.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon2.vaddrTexture = vaddrTexture;
        dialogues[index].dialogueButtonIcon2.vaddrPalette = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon2.vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        dialogues[index].dialogueButtonIcon2.vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;
        dialogues[index].dialogueButtonIcon2.unk_20 = argA;
 
        dialogues[index].dialogueButtonIcon2.spriteIndex = spriteIndex;
        dialogues[index].dialogueButtonIcon2.spriteOffset = spriteOffset;
        dialogues[index].dialogueButtonIcon2.flag = flag;

        dialogues[index].dialogueButtonIcon2.coordinates.x = x;
        dialogues[index].dialogueButtonIcon2.coordinates.y = y;
        dialogues[index].dialogueButtonIcon2.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueButtonIcon3);
 
bool setDialogueButtonIcon3(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    u8* vaddrTexture, u8* vaddrTextureEnd, AnimationFrameMetadata* vaddrAnimationFrameMetadata, u8* vaddrTextureToPaletteLookup, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {
        
        dialogues[index].dialogueButtonIcon3.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon3.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon3.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon3.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon3.vaddrTexture = vaddrTexture;
        dialogues[index].dialogueButtonIcon3.vaddrPalette = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon3.vaddrAnimationFrameMetadata = vaddrAnimationFrameMetadata;
        dialogues[index].dialogueButtonIcon3.vaddrTextureToPaletteLookup = vaddrTextureToPaletteLookup;
        dialogues[index].dialogueButtonIcon3.unk_20 = argA;
 
        dialogues[index].dialogueButtonIcon3.spriteIndex = spriteIndex;
        dialogues[index].dialogueButtonIcon3.spriteOffset = spriteOffset;
        dialogues[index].dialogueButtonIcon3.flag = flag;

        dialogues[index].dialogueButtonIcon3.coordinates.x = x;
        dialogues[index].dialogueButtonIcon3.coordinates.y = y;
        dialogues[index].dialogueButtonIcon3.coordinates.z = z;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043408);

// adjust value
inline int func_80043408(int initial, int value, int max) {
    
    int temp;
    int adjusted;

    adjusted = value;
    temp = initial + adjusted;
    
    if (max < temp) {
        adjusted -= temp - max;
        temp = max;
    }
    
    if (temp < 0) {
        adjusted -= temp;
    } 
      
    return adjusted;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", initializeDialogueSession);

bool initializeDialogueSession(u16 index, u16 dialogueBytecodeAddressesIndex, u16 dialogueIndex, u16 flag) {

    bool result = FALSE;

    u32 romAddr;

    if (index == 0 && dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE) {

        dmaSprite(dialogues[index].dialogueButtonIcon1.spriteIndex, 
            dialogues[index].dialogueButtonIcon1.romTextureStart, 
            dialogues[index].dialogueButtonIcon1.romTextureEnd, 
            dialogues[index].dialogueButtonIcon1.romIndexStart, 
            dialogues[index].dialogueButtonIcon1.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon1.vaddrTexture,
            NULL,
            dialogues[index].dialogueButtonIcon1.vaddrPalette,
            dialogues[index].dialogueButtonIcon1.vaddrAnimationFrameMetadata,
            dialogues[index].dialogueButtonIcon1.vaddrTextureToPaletteLookup,
            dialogues[index].dialogueButtonIcon1.unk_20,
            0,
            0);
 
        setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon1.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(dialogues[index].dialogueButtonIcon1.spriteIndex, TRUE);
        setSpriteColor(dialogues[index].dialogueButtonIcon1.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteAnchorAlignment(dialogues[index].dialogueButtonIcon1.spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
        setSpriteBlendMode(dialogues[index].dialogueButtonIcon1.spriteIndex, SPRITE_BLEND_ALPHA_DECAL);

        dmaSprite(dialogues[index].dialogueButtonIcon2.spriteIndex, 
            dialogues[index].dialogueButtonIcon2.romTextureStart, 
            dialogues[index].dialogueButtonIcon2.romTextureEnd, 
            dialogues[index].dialogueButtonIcon2.romIndexStart, 
            dialogues[index].dialogueButtonIcon2.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon2.vaddrTexture,
            NULL,
            dialogues[index].dialogueButtonIcon2.vaddrPalette,
            dialogues[index].dialogueButtonIcon2.vaddrAnimationFrameMetadata,
            dialogues[index].dialogueButtonIcon2.vaddrTextureToPaletteLookup,
            dialogues[index].dialogueButtonIcon2.unk_20,
            0,
            0);

        setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon2.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(dialogues[index].dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(dialogues[index].dialogueButtonIcon2.spriteIndex, TRUE);
        setSpriteColor(dialogues[index].dialogueButtonIcon2.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteAnchorAlignment(dialogues[index].dialogueButtonIcon2.spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
        setSpriteBlendMode(dialogues[index].dialogueButtonIcon2.spriteIndex, SPRITE_BLEND_ALPHA_DECAL);

        dmaSprite(dialogues[index].dialogueButtonIcon3.spriteIndex, 
            dialogues[index].dialogueButtonIcon3.romTextureStart, 
            dialogues[index].dialogueButtonIcon3.romTextureEnd, 
            dialogues[index].dialogueButtonIcon3.romIndexStart, 
            dialogues[index].dialogueButtonIcon3.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon3.vaddrTexture,
            NULL,
            dialogues[index].dialogueButtonIcon3.vaddrPalette,
            dialogues[index].dialogueButtonIcon3.vaddrAnimationFrameMetadata,
            dialogues[index].dialogueButtonIcon3.vaddrTextureToPaletteLookup,
            dialogues[index].dialogueButtonIcon3.unk_20,
            0,
            0);

        setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon3.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(dialogues[index].dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(dialogues[index].dialogueButtonIcon3.spriteIndex, TRUE);
        setSpriteColor(dialogues[index].dialogueButtonIcon3.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        setSpriteAnchorAlignment(dialogues[index].dialogueButtonIcon3.spriteIndex, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
        setSpriteBlendMode(dialogues[index].dialogueButtonIcon3.spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
        
        dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
        dialogues[index].sessionManager.dialogueBytecodeAddressesIndex = dialogueBytecodeAddressesIndex;
        dialogues[index].sessionManager.dialogueIndex = dialogueIndex;

        nuPiReadRom(dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexStart, dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].vaddrIndex, dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexEnd - dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexStart);

        romAddr = getDialogueBytecodeAddress(0, dialogues[index].sessionManager.dialogueIndex);

        nuPiReadRom(romAddr, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].vaddr, getDialogueBytecodeAddress(0, dialogues[index].sessionManager.dialogueIndex + 1) - romAddr);

        dialogues[index].dialogueBytecodePointer = dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].vaddr;
        
        dialogues[index].sessionManager.totalMenuRows = 0;
        dialogues[index].sessionManager.selectedMenuRow = 0;
 
        dialogues[index].bytecodeExecutor.textIndex = 0;
        dialogues[index].bytecodeExecutor.branchingDialogueIndex = 0;
        dialogues[index].bytecodeExecutor.minimumDialogueVariableValue = 0;
        dialogues[index].bytecodeExecutor.maximumDialogueVariableValue = 0;
        dialogues[index].bytecodeExecutor.randomValue = 0;
        dialogues[index].bytecodeExecutor.dialogueVariableValue = 0;
        dialogues[index].bytecodeExecutor.specialDialogueBit = 0;
        dialogues[index].bytecodeExecutor.updatedDialogueVariableAdjustment = 0;
        dialogues[index].bytecodeExecutor.textOffset = 0;
        dialogues[index].bytecodeExecutor.dialogueVariablesIndex = 0;
        dialogues[index].bytecodeExecutor.randomMinimumValue = 0;
        dialogues[index].bytecodeExecutor.randomMaximumValue = 0;
        dialogues[index].bytecodeExecutor.unusedField = 0;
        dialogues[index].bytecodeExecutor.selectionMenuRowsCount = 0;
        dialogues[index].bytecodeExecutor.targetMenuRow = 0;
        
        dialogues[index].sessionManager.flags &= ~4;
        dialogues[index].sessionManager.flags |= DIALOGUE_INITIALIZED;

        if (flag == 0x40) {
            dialogues[index].sessionManager.flags |= (DIALOGUE_INITIALIZED | 0x40);
        }  else {
            dialogues[index].sessionManager.flags &= ~(0x40);
        }

        if (flag == 0x80) {
            dialogues[index].sessionManager.flags |= 0x80;
        } else {
            dialogues[index].sessionManager.flags &= ~0x80;
        }

        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", checkAllDialoguesCompleted);

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

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", closeDialogueSession);

bool closeDialogueSession(u16 index) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE) {

        if (!(dialogues[index].sessionManager.flags & 0x40)) {
            messageBoxes[dialogues[index].sessionManager.mainMessageBoxIndex].flags &= ~MESSAGE_BOX_MODE_UNKNOWN;
        }

        resetMessageBoxAnimation(dialogues[index].sessionManager.mainMessageBoxIndex);
        cleanupDialogueOverlayBox(0);
        
        dialogues[index].sessionManager.flags = (1 | 4);
        
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", cleanupDialogueOverlayBox);

void cleanupDialogueOverlayBox(u16 index) {

    messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].flags &= ~MESSAGE_BOX_MODE_UNKNOWN;
    
    resetMessageBoxAnimation(dialogues[index].sessionManager.overlayMessageBoxIndex);
    
    resetAnimationState(dialogues[index].dialogueButtonIcon1.spriteIndex);
    resetAnimationState(dialogues[index].dialogueButtonIcon2.spriteIndex);
    resetAnimationState(dialogues[index].dialogueButtonIcon3.spriteIndex);
    
    dialogues[index].sessionManager.flags &= ~DIALOGUE_PAUSE_FOR_USER_INPUT;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", getSelectedMenuRow);

u8 getSelectedMenuRow(u16 index) {
    return dialogues[index].sessionManager.selectedMenuRow;
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", getDialogueBytecodeAddress);

u32 getDialogueBytecodeAddress(u16 index, u16 dialogueOffset) {

    u32 ptr = dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].romStart;
    
    return ptr + dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].vaddrIndex[dialogueOffset];
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueVariableValue);

inline void setDialogueVariableValue(u16 index, u16 value) {

    switch (dialogueVariables[index].type) {
        case UNSIGNED_CHAR:
            *(u8*)dialogueVariables[index].value = value;
            break;
        case UNSIGNED_SHORT:
            *(u16*)dialogueVariables[index].value = value;
            break;
        case UNSIGNED_INT:
            *(u32*)dialogueVariables[index].value = value;
            break;
    }
    
}
 
//INCLUDE_ASM("asm/nonmatchings/system/dialogue", getDialogueVariableValue);

inline u32 getDialogueVariableValue(u16 index) {

    u32 value;
    
    switch (dialogueVariables[index].type) {
        case UNSIGNED_CHAR:
            value = *(u8*)dialogueVariables[index].value;
            break;
        case UNSIGNED_SHORT:
            value = *(u16*)dialogueVariables[index].value;
            break;
        case UNSIGNED_INT:
            value = *(u32*)dialogueVariables[index].value;
            break;
    }

    return value;

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setSpecialDialogueBitFromPointer);

inline void setSpecialDialogueBitFromPointer(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] |= 1 << (temp & 0x1F);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", toggleSpecialDialogueBitFromPointer);

inline void toggleSpecialDialogueBitFromPointer(u16 bitIndex) {

    u32 temp = bitIndex;
    specialDialogueBitsPointer[temp >> 5] &= ~(1 << (temp & 0x1F));

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", checkSpecialDialogueBitFromPointer);

inline u32 checkSpecialDialogueBitFromPointer(u16 bitIndex) {
    
    u32 temp = bitIndex;
    
    return !(specialDialogueBitsPointer[temp >> 5] & (1 << (temp & 0x1F))) == 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", parseDialogueBytecode);

void parseDialogueBytecode(u16 index) {

    Swap16 byteswap;

    dialogues[index].bytecodeExecutor.currentOpcode = *(u8*)dialogues[index].dialogueBytecodePointer++;

    switch (dialogues[index].bytecodeExecutor.currentOpcode) {
        
        case DIALOGUE_OPCODE_SHOW_TEXT:
                        
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;            
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.textIndex = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_DIALOGUE_VARIABLE_BRANCH:
            
            dialogues[index].bytecodeExecutor.dialogueVariablesIndex = *(u8*)dialogues[index].dialogueBytecodePointer++;  

            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.minimumDialogueVariableValue = byteswap.halfword;
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.maximumDialogueVariableValue = byteswap.halfword;
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.textIndex = byteswap.halfword;
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_UPDATE_DIALOGUE_VARIABLE:
            
            dialogues[index].bytecodeExecutor.dialogueVariablesIndex = *(u8*)dialogues[index].dialogueBytecodePointer++;

            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.updatedDialogueVariableAdjustment = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_SET_DIALOGUE_VARIABLE:
            
            dialogues[index].bytecodeExecutor.dialogueVariablesIndex = *(u8*)dialogues[index].dialogueBytecodePointer++;  

            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;

            dialogues[index].bytecodeExecutor.dialogueVariableValue = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_SPECIAL_DIALOGUE_BIT_BRANCH:
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.specialDialogueBit = byteswap.halfword;
                        
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.textIndex = byteswap.halfword;
                        
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;
            
            break;
        
        case DIALOGUE_OPCODE_SET_SPECIAL_DIALOGUE_BIT:
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.specialDialogueBit = byteswap.halfword;  
            
            break;

        case DIALOGUE_OPCODE_TOGGLE_SPECIAL_DIALOGUE_BIT:
                    
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.specialDialogueBit = byteswap.halfword;
            
            break;

        case DIALOGUE_OPCODE_RANDOM_BRANCH:
            
            dialogues[index].bytecodeExecutor.randomMinimumValue = *(u8*)dialogues[index].dialogueBytecodePointer++;
            dialogues[index].bytecodeExecutor.randomMaximumValue = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.textIndex = byteswap.halfword;
                        
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
    
            dialogues[index].bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;
            
            dialogues[index].bytecodeExecutor.randomValue = getRandomNumberInRange(0, 99);  
            
            break;

        case DIALOGUE_OPCODE_BRANCH:
             
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;  
            
            break;
        
        case DIALOGUE_OPCODE_SHOW_SELECTION_MENU:
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.textOffset = byteswap.halfword;

            // unused
            dialogues[index].bytecodeExecutor.selectionMenuRowsCount = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            break;
    
        case DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH:
            
            // row
            dialogues[index].bytecodeExecutor.targetMenuRow = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.branchingDialogueIndex = byteswap.halfword;  
            
            break;

        case DIALOGUE_OPCODE_END_DIALOGUE:
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setOverlayMessageBoxSprite);

void setOverlayMessageBoxSprite(u16 index) {

    f32 xPosition, yPosition;

    // pain
    xPosition = -((messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxLineCharWidth * messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].fontContext.characterCellWidth) / 2) 
        - ((messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxLineCharWidth * messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].characterSpacing) / 2) 
        - dialogues[index].dialogueButtonIcon1.coordinates.x;

    yPosition = ((messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight) / 2) 
        + ((messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].lineSpacing) / 2) 
        + dialogues[index].dialogueButtonIcon1.coordinates.y;

    setSpriteScale(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].maxCharsPerLine * 0.5f, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * 0.6f, 
       1.0f);

    setSpriteBlendMode(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 255, 255, 255, 192);
    setBilinearFiltering(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);
    startSpriteAnimation(dialogues[index].dialogueButtonIcon1.spriteIndex, dialogues[index].dialogueButtonIcon1.spriteOffset, dialogues[index].dialogueButtonIcon1.flag);
    setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + xPosition, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + yPosition, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", updateDialogueButtonIcon2Display);

void updateDialogueButtonIcon2Display(u16 index) {

    f32 tempX = dialogues[index].dialogueButtonIcon2.coordinates.x;
    f32 tempY = dialogues[index].dialogueButtonIcon2.coordinates.y;

    setSpriteScale(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].maxCharsPerLine * 0.5f, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * 0.6f, 1.0f);
    
    setSpriteBlendMode(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
    setBilinearFiltering(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);

    startSpriteAnimation(dialogues[index].dialogueButtonIcon2.spriteIndex, dialogues[index].dialogueButtonIcon2.spriteOffset, dialogues[index].dialogueButtonIcon2.flag);
    setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon2.spriteIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + tempX, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + tempY, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", updateDialogueButtonIcon3Display);

void updateDialogueButtonIcon3Display(u16 index) {
 
    f32 tempX = dialogues[index].dialogueButtonIcon3.coordinates.x;
    f32 tempY = dialogues[index].dialogueButtonIcon3.coordinates.y;

    setSpriteBlendMode(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, SPRITE_BLEND_ALPHA_DECAL);
    setBilinearFiltering(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 1);

    startSpriteAnimation(dialogues[index].dialogueButtonIcon3.spriteIndex, dialogues[index].dialogueButtonIcon3.spriteOffset, dialogues[index].dialogueButtonIcon3.flag);
    setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon3.spriteIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + tempX, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + tempY, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", handleMenuNavigation);

void handleMenuNavigation(u16 index) {

    bool set = FALSE;

    if (!(messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].flags & (MESSAGE_BOX_SCROLLING_DOWN | MESSAGE_BOX_SCROLLING_UP))) { 

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {

            if (dialogues[index].sessionManager.selectedMenuRow < (dialogues[index].sessionManager.totalMenuRows - 1)) {

                dialogues[index].sessionManager.selectedMenuRow++;

                if (dialogues[index].sessionManager.visibleMenuCursorRow != messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows - 1) {
                    dialogues[index].sessionManager.visibleMenuCursorRow++;
                    adjustSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, -messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight - messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].lineSpacing, 0.0f);
                } else {
                    scrollMessageBoxDown(dialogues[index].sessionManager.overlayMessageBoxIndex);
                    dialogues[index].sessionManager.menuScrollOffset++;
                }

                if (dialogues[index].sessionManager.scrollSfxIndex != 0xFF) {
                    setSfx(dialogues[index].sessionManager.scrollSfxIndex + 1);
                    setSfxVolume(dialogues[index].sessionManager.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                }
                
                set = TRUE;
                
            }

        }

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {

            if (!set) {

                if (dialogues[index].sessionManager.selectedMenuRow) {

                    dialogues[index].sessionManager.selectedMenuRow--;
                    
                    if (dialogues[index].sessionManager.visibleMenuCursorRow) {
                        dialogues[index].sessionManager.visibleMenuCursorRow--;
                        adjustSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight + messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].lineSpacing, 0.0f);
                    } else {
                        scrollMessageBoxUp(dialogues[index].sessionManager.overlayMessageBoxIndex);
                        dialogues[index].sessionManager.menuScrollOffset--;
                    }

                    if (dialogues[index].sessionManager.scrollSfxIndex != 0xFF) {
                        setSfx(dialogues[index].sessionManager.scrollSfxIndex + 1);
                        setSfxVolume(dialogues[index].sessionManager.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                        
                    }
                    
                    set = TRUE;
                
                }

            }

        }

        if (dialogues[index].sessionManager.totalMenuRows > messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows) { 

            if (messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows < (dialogues[index].sessionManager.totalMenuRows - dialogues[index].sessionManager.menuScrollOffset)) {
                updateDialogueButtonIcon3Display(index);
            } else {
                resetAnimationState(dialogues[index].dialogueButtonIcon3.spriteIndex);
            }

            if (dialogues[index].sessionManager.menuScrollOffset) {
                updateDialogueButtonIcon2Display(index);
            } else {
                resetAnimationState(dialogues[index].dialogueButtonIcon2.spriteIndex);
            }

        }

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

            if (!set) {
                
                cleanupDialogueOverlayBox(index);
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                
                if (dialogues[index].sessionManager.closeSfxIndex != 0xFF) {
                    setSfx(dialogues[index].sessionManager.closeSfxIndex + 1);
                    setSfxVolume(dialogues[index].sessionManager.closeSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                    set = TRUE;
                }
                
            }

        }
        
    }
   
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", updateCurrentDialogue);

void updateCurrentDialogue(u16 index) {
    
    u16 finishCurrentDialogueBlockProcessing = FALSE;
    
    u16 dialogueVariableValue;
    u16 temp2;
    u16 temp3;
    int temp4;
    int temp5;
    u16 temp6;

    u16 tempIndex;
    u16 textOrDialogueIndex;
    
    int adjusted;
    int max;
    int initial;
    
    // loop and continue to process new opcodes unless marked as done
    while (!finishCurrentDialogueBlockProcessing) {
        
        // currentOpcode is reset to 0xFF after processing
        if (dialogues[index].bytecodeExecutor.currentOpcode == 0xFF) {
            parseDialogueBytecode(index);
        }
        
        switch (dialogues[index].bytecodeExecutor.currentOpcode) {
        
            case DIALOGUE_OPCODE_SHOW_TEXT:

                if (dialogues[index].sessionManager.flags & 0x80) {
                    initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, dialogues[index].bytecodeExecutor.textIndex, 0);
                } else {
                    initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, dialogues[index].bytecodeExecutor.textIndex, MESSAGE_BOX_MODE_UNKNOWN);
                }
                
                finishCurrentDialogueBlockProcessing = TRUE;

                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                
                break;

            case DIALOGUE_OPCODE_DIALOGUE_VARIABLE_BRANCH:
                
                dialogueVariableValue = getDialogueVariableValue(dialogues[index].bytecodeExecutor.dialogueVariablesIndex);
                
                if ((dialogueVariableValue >= dialogues[index].bytecodeExecutor.minimumDialogueVariableValue) && (dialogues[index].bytecodeExecutor.maximumDialogueVariableValue >= dialogueVariableValue)) {
                
                    textOrDialogueIndex = dialogues[index].bytecodeExecutor.textIndex;
                    
                    // if a specific text is set, branch to that; otherwise, branch to another bytecode segment
                    if (textOrDialogueIndex != 0xFFFF) {

                        if (dialogues[index].sessionManager.flags & 0x80) {
                            initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0);
                        } else {
                            initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0x8000);
                        }
                        
                        finishCurrentDialogueBlockProcessing = TRUE;
                        
                        dialogues[index].bytecodeExecutor.textIndex = 0xFFFF;
                        dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                        
                    // branch to another bytecode segment
                    } else {
                        
                        textOrDialogueIndex = dialogues[index].bytecodeExecutor.branchingDialogueIndex;
                        
                        if (textOrDialogueIndex != 0xFFFF) {
                            initializeDialogueSession(index, dialogues[index].sessionManager.dialogueBytecodeAddressesIndex, textOrDialogueIndex, dialogues[index].sessionManager.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                        }
                        
                    }
                    
                } else {
                    dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_UPDATE_DIALOGUE_VARIABLE:

                /*

                temp4 = (u16)getDialogueVariableValue(dialogues[index].bytecodeExecutor.dialogueVariablesIndex);

                temp4 += adjustValue_dialogue_c(temp4, dialogues[index].bytecodeExecutor.updatedDialogueVariableAdjustment, dialogueVariables[dialogues[index].bytecodeExecutor.dialogueVariablesIndex].maxValue);
                
                setDialogueVariableValue(dialogues[index].bytecodeExecutor.dialogueVariablesIndex, temp4);
                
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                
                break;

                */

                temp4 = getDialogueVariableValue(dialogues[index].bytecodeExecutor.dialogueVariablesIndex);

                // FIXME: should be inline adjustValue call

                max = dialogueVariables[dialogues[index].bytecodeExecutor.dialogueVariablesIndex].maxValue;
                adjusted = dialogues[index].bytecodeExecutor.updatedDialogueVariableAdjustment;
                
                temp5 = (u16)temp4;
                temp5 += adjusted;
                
                if (max < temp5) {
                    adjusted -= temp5 - max;
                    temp5 = max;
                }
                if (temp5 < 0) {
                    adjusted -= temp5;
                }
                
                max = temp4 + adjusted;
                
                // FIXME: should be inline setDialogueVariableValue call

                tempIndex = dialogues[index].bytecodeExecutor.dialogueVariablesIndex;
                
                switch (dialogueVariables[tempIndex].type) {
                    case UNSIGNED_CHAR:
                        *((u8*)dialogueVariables[tempIndex].value) = max;
                        break;

                    case UNSIGNED_SHORT:
                        *((u16*)dialogueVariables[tempIndex].value) = (u16)max;
                        break;

                    case UNSIGNED_INT:
                        *((u32*)dialogueVariables[tempIndex].value) = (u16)max;
                        break;
                }

                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                
                break;

            case DIALOGUE_OPCODE_SET_DIALOGUE_VARIABLE:
                setDialogueVariableValue(dialogues[index].bytecodeExecutor.dialogueVariablesIndex, dialogues[index].bytecodeExecutor.dialogueVariableValue);
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                break;

            case DIALOGUE_OPCODE_SPECIAL_DIALOGUE_BIT_BRANCH:
                
                if (checkSpecialDialogueBitFromPointer(dialogues[index].bytecodeExecutor.specialDialogueBit)) {
                    
                    textOrDialogueIndex = dialogues[index].bytecodeExecutor.textIndex;
                
                    // if specific text is set, branch to that; otherwise branch to another bytecode segment
                    if (textOrDialogueIndex != 0xFFFF) {
                        
                        if (dialogues[index].sessionManager.flags & 0x80) {
                            initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0);
                        } else {
                            initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0x8000);
                        }
                        
                        finishCurrentDialogueBlockProcessing = TRUE;
                        dialogues[index].bytecodeExecutor.textIndex = 0xFFFF;
                        dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                    
                    // branch to another bytecode segment
                    } else {
                        
                        textOrDialogueIndex = dialogues[index].bytecodeExecutor.branchingDialogueIndex;

                        if (textOrDialogueIndex != 0xFFFF) {
                            initializeDialogueSession(index, dialogues[index].sessionManager.dialogueBytecodeAddressesIndex, textOrDialogueIndex, dialogues[index].sessionManager.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                        }

                    }
                    
                } else {
                    dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_SET_SPECIAL_DIALOGUE_BIT:
                setSpecialDialogueBitFromPointer(dialogues[index].bytecodeExecutor.specialDialogueBit);
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                break;

            case DIALOGUE_OPCODE_TOGGLE_SPECIAL_DIALOGUE_BIT:
                toggleSpecialDialogueBitFromPointer(dialogues[index].bytecodeExecutor.specialDialogueBit);
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                break;

            case DIALOGUE_OPCODE_RANDOM_BRANCH:
                
                temp3 = dialogues[index].bytecodeExecutor.randomValue;
                
                if ((temp3 >= dialogues[index].bytecodeExecutor.randomMinimumValue) && (dialogues[index].bytecodeExecutor.randomMaximumValue >= temp3)) {
                
                    textOrDialogueIndex = dialogues[index].bytecodeExecutor.textIndex;
                    
                    if (textOrDialogueIndex != 0xFFFF) {

                        if (dialogues[index].sessionManager.flags & 0x80) {
                            initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0);
                        } else {
                            initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, textOrDialogueIndex, 0x8000);
                        }
                        
                        finishCurrentDialogueBlockProcessing = TRUE;

                        dialogues[index].bytecodeExecutor.textIndex = 0xFFFF;
                        dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                        
                    } else {
                        
                        textOrDialogueIndex = dialogues[index].bytecodeExecutor.branchingDialogueIndex;
                        
                        if (textOrDialogueIndex != 0xFFFF) {
                            initializeDialogueSession(index, dialogues[index].sessionManager.dialogueBytecodeAddressesIndex, textOrDialogueIndex, dialogues[index].sessionManager.flags & (0x40 | 0x80));
                        } else {
                            dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                        }
                    }
                    
                } else {
                    dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_BRANCH:
                initializeDialogueSession(index, dialogues[index].sessionManager.dialogueBytecodeAddressesIndex, dialogues[index].bytecodeExecutor.branchingDialogueIndex, dialogues[index].sessionManager.flags & (0x40 | 0x80));
                break;

            case DIALOGUE_OPCODE_SHOW_SELECTION_MENU:
                
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                dialogues[index].sessionManager.selectedMenuRow = 0;
                dialogues[index].sessionManager.visibleMenuCursorRow = 0;
                dialogues[index].sessionManager.menuScrollOffset = 0;
                dialogues[index].sessionManager.overlayTextOffset = dialogues[index].bytecodeExecutor.textOffset;
                dialogues[index].sessionManager.flags |= DIALOGUE_PAUSE_FOR_USER_INPUT;
                
                initializeMessageBox(dialogues[index].sessionManager.overlayMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].selectionMenuTextAddressesIndex, dialogues[index].sessionManager.overlayTextOffset, 0);
                
                // probably line height or selection count
                temp2 = messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].totalLines;
                
                if (temp2 >= 5) {
                    setMessageBoxLineAndRowSizes(dialogues[index].sessionManager.overlayMessageBoxIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].maxCharsPerLine, 4);
                } else {
                    setMessageBoxLineAndRowSizes(dialogues[index].sessionManager.overlayMessageBoxIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].maxCharsPerLine, temp2);
                }
                
                dialogues[index].sessionManager.totalMenuRows = messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].totalLines;
                
                setOverlayMessageBoxSprite(index);
                
                if (dialogues[index].sessionManager.buttonPressSfxIndex != 0xFF) {
                    setSfx(dialogues[index].sessionManager.buttonPressSfxIndex + 1);
                    setSfxVolume(dialogues[index].sessionManager.buttonPressSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                }
                
                finishCurrentDialogueBlockProcessing = TRUE;
                
                break;

            case DIALOGUE_OPCODE_UNUSED:
                break;

            case DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH:
                
                temp6 = dialogues[index].sessionManager.selectedMenuRow;
                
                if (dialogues[index].bytecodeExecutor.targetMenuRow == temp6) {
                    
                    initializeDialogueSession(index, dialogues[index].sessionManager.dialogueBytecodeAddressesIndex, dialogues[index].bytecodeExecutor.branchingDialogueIndex, dialogues[index].sessionManager.flags & (0x40 | 0x80));
                    // ?
                    dialogues[index].sessionManager.selectedMenuRow = temp6;

                } else {
                    dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_END_DIALOGUE:
                finishCurrentDialogueBlockProcessing = TRUE;
                closeDialogueSession(index);
                break;
        
            }

    }
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", updateDialogues);

void updateDialogues(void) {

    u16 i;
    bool skipDialogueUpdate;

    for (i = 0; i < MAX_DIALOGUES; i++) {
        
        if ((dialogues[i].sessionManager.flags & DIALOGUE_ACTIVE) && (dialogues[i].sessionManager.flags & DIALOGUE_INITIALIZED)) {
            
            skipDialogueUpdate = FALSE;
            
            if (dialogues[i].sessionManager.flags & DIALOGUE_WAIT_FOR_MESSAGE_BOX) {
                
                if ((messageBoxes[dialogues[i].sessionManager.mainMessageBoxIndex].flags & MESSAGE_BOX_TEXT_COMPLETE) || (messageBoxes[dialogues[i].sessionManager.mainMessageBoxIndex].flags & 0x20000)) {
                    dialogues[i].sessionManager.flags &= ~DIALOGUE_WAIT_FOR_MESSAGE_BOX;
                }
                
                skipDialogueUpdate = TRUE;

            }
            
            if (dialogues[i].sessionManager.flags & DIALOGUE_PAUSE_FOR_USER_INPUT) {
                handleMenuNavigation(i);
                skipDialogueUpdate = TRUE;
            }
            
            if (!skipDialogueUpdate) {
                updateCurrentDialogue(i);
            }

        }
    
    }
    
}
