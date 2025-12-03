#include "common.h"

#include "system/controller.h" 
#include "system/dialogue.h"
#include "system/math.h"
#include "system/message.h"
#include "system/globalSprites.h"

#include "game/game.h"

#include "ld_symbols.h"


// game state updated by/talked about in dialogues
DialogueVariable dialogueVariables[MAX_DIALOGUE_VARIABLES];

u32* specialDialogueBitsPointer;

// forward declarations
u32 func_80043C98(u16, u16);
void func_80043B84(u16);
void setOverlayMessageBoxSprite(u16 index);
void func_800449C4(u16 index);
void func_80044BF4(u16 index);
void func_80044D78(u16);
void updateCurrentDialogue(u16);


//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80042F60);

void func_80042F60(void) {
 
    u16 i;
 
    for (i = 0; i < MAX_DIALOGUES; i++) {
        dialogues[i].sessionManager.dialogueIndex = 0;
        dialogues[i].sessionManager.dialogueBytecodeAddressesIndex = 0;
        dialogues[i].sessionManager.mainMessageBoxIndex = 0;
        dialogues[i].sessionManager.scrollSfxIndex = 0xFF;
        dialogues[i].sessionManager.closeSfxIndex = 0xFF;
        dialogues[i].sessionManager.unk_8 = 0xFF;
        dialogues[i].sessionManager.flags = 0;
    }

} 

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80042FEC);

bool func_80042FEC(u16 index, u16 mainMessageBoxIndex, u16 overlayMessageBoxIndex) {

    bool result = FALSE;
 
    if (index == 0 && !(dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {

        dialogues[index].sessionManager.scrollSfxIndex = 0xFF;
        dialogues[index].sessionManager.closeSfxIndex = 0xFF;
        dialogues[index].sessionManager.unk_8 = 0xFF;
        dialogues[index].sessionManager.mainMessageBoxIndex = mainMessageBoxIndex;
        dialogues[index].sessionManager.overlayMessageBoxIndex = overlayMessageBoxIndex;

        dialogues[index].sessionManager.flags = 1;
        
        result = TRUE;
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", setDialogueBytecodeAddressInfo);

bool setDialogueBytecodeAddressInfo(u16 index, u16 textAddressesIndex, u16 subdialogueTextAddressesIndex, u32 romIndexStart, u32 romIndexEnd, void* vaddrIndex, u32 romStart, void* vaddr) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_ADDRESSES) {

        dialogueBytecodeAddresses[index].textAddressesIndex = textAddressesIndex;
        dialogueBytecodeAddresses[index].subdialogueTextAddressesIndex = subdialogueTextAddressesIndex;
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

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043148);

bool func_80043148(u16 index, u32 scrollSfxIndex, u32 closeSfxIndex, u32 sfxIndex) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE) {
        
        dialogues[index].sessionManager.scrollSfxIndex = scrollSfxIndex;
        dialogues[index].sessionManager.closeSfxIndex = closeSfxIndex;
        dialogues[index].sessionManager.unk_8 = sfxIndex;
        
        result = TRUE;

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_8004318C);

bool func_8004318C(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {
  
        dialogues[index].dialogueButtonIcon1.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon1.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon1.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon1.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon1.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].dialogueButtonIcon1.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon1.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].dialogueButtonIcon1.vaddrIndexEnd = vaddrIndexEnd;
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

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043260);

bool func_80043260(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {
  
        dialogues[index].dialogueButtonIcon2.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon2.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon2.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon2.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon2.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].dialogueButtonIcon2.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon2.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].dialogueButtonIcon2.vaddrIndexEnd = vaddrIndexEnd;
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

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043334);
 
bool func_80043334(u16 index, u16 spriteIndex, u32 romTextureStart, u32 romTextureEnd, u32 romIndexStart, u32 romIndexEnd, 
    void* vaddrTextureStart, void* vaddrTextureEnd, void* vaddrIndexStart, void* vaddrIndexEnd, u32 argA, 
    u16 spriteOffset, u8 flag, f32 x, f32 y, f32 z) {

    bool result = FALSE;

    if (index == 0 && (dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE)) {
        
        dialogues[index].dialogueButtonIcon3.romTextureStart = romTextureStart;
        dialogues[index].dialogueButtonIcon3.romTextureEnd = romTextureEnd;
        dialogues[index].dialogueButtonIcon3.romIndexStart = romIndexStart;
        dialogues[index].dialogueButtonIcon3.romIndexEnd = romIndexEnd;
  
        dialogues[index].dialogueButtonIcon3.vaddrTextureStart = vaddrTextureStart;
        dialogues[index].dialogueButtonIcon3.vaddrTextureEnd = vaddrTextureEnd;
        dialogues[index].dialogueButtonIcon3.vaddrIndexStart = vaddrIndexStart;
        dialogues[index].dialogueButtonIcon3.vaddrIndexEnd = vaddrIndexEnd;
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
            dialogues[index].dialogueButtonIcon1.vaddrTextureStart,
            NULL,
            dialogues[index].dialogueButtonIcon1.vaddrTextureEnd,
            dialogues[index].dialogueButtonIcon1.vaddrIndexStart,
            dialogues[index].dialogueButtonIcon1.vaddrIndexEnd,
            dialogues[index].dialogueButtonIcon1.unk_20,
            0,
            0);
 
        setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon1.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(dialogues[index].dialogueButtonIcon1.spriteIndex, TRUE);
        setSpriteColor(dialogues[index].dialogueButtonIcon1.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].dialogueButtonIcon1.spriteIndex, 2, 2);
        setSpriteRenderingLayer(dialogues[index].dialogueButtonIcon1.spriteIndex, (1 | 2));

        dmaSprite(dialogues[index].dialogueButtonIcon2.spriteIndex, 
            dialogues[index].dialogueButtonIcon2.romTextureStart, 
            dialogues[index].dialogueButtonIcon2.romTextureEnd, 
            dialogues[index].dialogueButtonIcon2.romIndexStart, 
            dialogues[index].dialogueButtonIcon2.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon2.vaddrTextureStart,
            NULL,
            dialogues[index].dialogueButtonIcon2.vaddrTextureEnd,
            dialogues[index].dialogueButtonIcon2.vaddrIndexStart,
            dialogues[index].dialogueButtonIcon2.vaddrIndexEnd,
            dialogues[index].dialogueButtonIcon2.unk_20,
            0,
            0);

        setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon2.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(dialogues[index].dialogueButtonIcon2.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(dialogues[index].dialogueButtonIcon2.spriteIndex, TRUE);
        setSpriteColor(dialogues[index].dialogueButtonIcon2.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].dialogueButtonIcon2.spriteIndex, 2, 2);
        setSpriteRenderingLayer(dialogues[index].dialogueButtonIcon2.spriteIndex, (1 | 2));

        dmaSprite(dialogues[index].dialogueButtonIcon3.spriteIndex, 
            dialogues[index].dialogueButtonIcon3.romTextureStart, 
            dialogues[index].dialogueButtonIcon3.romTextureEnd, 
            dialogues[index].dialogueButtonIcon3.romIndexStart, 
            dialogues[index].dialogueButtonIcon3.romIndexEnd, 
            NULL, 
            NULL, 
            dialogues[index].dialogueButtonIcon3.vaddrTextureStart,
            NULL,
            dialogues[index].dialogueButtonIcon3.vaddrTextureEnd,
            dialogues[index].dialogueButtonIcon3.vaddrIndexStart,
            dialogues[index].dialogueButtonIcon3.vaddrIndexEnd,
            dialogues[index].dialogueButtonIcon3.unk_20,
            0,
            0);

        setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setSpriteScale(dialogues[index].dialogueButtonIcon3.spriteIndex, 1.0f, 1.0f, 1.0f);
        setSpriteRotation(dialogues[index].dialogueButtonIcon3.spriteIndex, 0.0f, 0.0f, 0.0f);
        setBilinearFiltering(dialogues[index].dialogueButtonIcon3.spriteIndex, TRUE);
        setSpriteColor(dialogues[index].dialogueButtonIcon3.spriteIndex, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002C680(dialogues[index].dialogueButtonIcon3.spriteIndex, 2, 2);
        setSpriteRenderingLayer(dialogues[index].dialogueButtonIcon3.spriteIndex, (1 | 2));
        
        dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
        dialogues[index].sessionManager.dialogueBytecodeAddressesIndex = dialogueBytecodeAddressesIndex;
        dialogues[index].sessionManager.dialogueIndex = dialogueIndex;

        nuPiReadRom(dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexStart, dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].vaddrIndex, dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexEnd - dialogueBytecodeAddresses[dialogueBytecodeAddressesIndex].romIndexStart);

        romAddr = func_80043C98(0, dialogues[index].sessionManager.dialogueIndex);

        nuPiReadRom(romAddr, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].vaddr, func_80043C98(0, dialogues[index].sessionManager.dialogueIndex + 1) - romAddr);

        dialogues[index].dialogueBytecodePointer = dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].vaddr;
        
        dialogues[index].sessionManager.unk_16 = 0;
        dialogues[index].sessionManager.unk_17 = 0;
 
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
        dialogues[index].bytecodeExecutor.unusedField2 = 0;
        dialogues[index].bytecodeExecutor.unk_18 = 0;
        
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

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043A88);

bool func_80043A88(void) {

    bool result = FALSE;
    u16 i;

    for (i = 0; i < 1; i++) {
        if (dialogues[i].sessionManager.flags & 4) {
            result = TRUE;
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043AD8);

bool func_80043AD8(u16 index) {

    bool result = FALSE;

    if (index == 0 && dialogues[index].sessionManager.flags & DIALOGUE_ACTIVE) {

        if (!(dialogues[index].sessionManager.flags & 0x40)) {
            messageBoxes[dialogues[index].sessionManager.mainMessageBoxIndex].flags &= ~0x8000;
        }

        func_8003F130(dialogues[index].sessionManager.mainMessageBoxIndex);
        func_80043B84(0);
        
        dialogues[index].sessionManager.flags = (1 | 4);
        
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043B84);

void func_80043B84(u16 index) {

    messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].flags &= ~0x8000;
    
    func_8003F130(dialogues[index].sessionManager.overlayMessageBoxIndex);
    
    resetAnimationState(dialogues[index].dialogueButtonIcon1.spriteIndex);
    resetAnimationState(dialogues[index].dialogueButtonIcon2.spriteIndex);
    resetAnimationState(dialogues[index].dialogueButtonIcon3.spriteIndex);
    
    dialogues[index].sessionManager.flags &= ~DIALOGUE_PAUSE_FOR_USER_INPUT;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043C6C);

u8 func_80043C6C(u16 index) {
    return dialogues[index].sessionManager.unk_17;
}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80043C98);

u32 func_80043C98(u16 index, u16 dialogueOffset) {

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
        
        case DIALOGUE_OPCODE_SHOW_SUBDIALOGUE_BOX:
            
            byteswap.byte[1] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            byteswap.byte[0] = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            dialogues[index].bytecodeExecutor.textOffset = byteswap.halfword;
            
            dialogues[index].bytecodeExecutor.unusedField2 = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
            break;
    
        case DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH:
            
            // row
            dialogues[index].bytecodeExecutor.unk_18 = *(u8*)dialogues[index].dialogueBytecodePointer++;
            
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
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].unk_A0 * 0.5f, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * 0.6f, 
       1.0f);

    setSpriteRenderingLayer(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, (1 | 2));
    setSpriteColor(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 255, 255, 255, 192);
    setBilinearFiltering(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);
    startSpriteAnimation(dialogues[index].dialogueButtonIcon1.spriteIndex, dialogues[index].dialogueButtonIcon1.spriteOffset, dialogues[index].dialogueButtonIcon1.flag);
    setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + xPosition, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + yPosition, 
       messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_800449C4);

void func_800449C4(u16 index) {

    f32 tempX = dialogues[index].dialogueButtonIcon2.coordinates.x;
    f32 tempY = dialogues[index].dialogueButtonIcon2.coordinates.y;

    setSpriteScale(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].unk_A0 * 0.5f, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows * 0.6f, 1.0f);
    
    setSpriteRenderingLayer(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, (1 | 2));
    setBilinearFiltering(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, TRUE);

    startSpriteAnimation(dialogues[index].dialogueButtonIcon2.spriteIndex, dialogues[index].dialogueButtonIcon2.spriteOffset, dialogues[index].dialogueButtonIcon2.flag);
    setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon2.spriteIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + tempX, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + tempY, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80044BF4);

void func_80044BF4(u16 index) {
 
    f32 tempX = dialogues[index].dialogueButtonIcon3.coordinates.x;
    f32 tempY = dialogues[index].dialogueButtonIcon3.coordinates.y;

    setSpriteRenderingLayer(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, (1 | 2));
    setBilinearFiltering(dialogueWindows[messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].dialogueWindowIndex].spriteIndex, 1);

    startSpriteAnimation(dialogues[index].dialogueButtonIcon3.spriteIndex, dialogues[index].dialogueButtonIcon3.spriteOffset, dialogues[index].dialogueButtonIcon3.flag);
    setSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon3.spriteIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.x + tempX, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.y + tempY, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].viewSpacePosition.z);

}

//INCLUDE_ASM("asm/nonmatchings/system/dialogue", func_80044D78);

void func_80044D78(u16 index) {

    bool set = FALSE;

    if (!(messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].flags & (0x40 | 0x80))) { 

        if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_UP)) {

            if (dialogues[index].sessionManager.unk_17 < (dialogues[index].sessionManager.unk_16 - 1)) {

                dialogues[index].sessionManager.unk_17++;

                if (dialogues[index].sessionManager.unk_18 != messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows - 1) {
                    dialogues[index].sessionManager.unk_18++;
                    adjustSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, -messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight - messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].lineSpacing, 0.0f);
                } else {
                    func_8003FE9C(dialogues[index].sessionManager.overlayMessageBoxIndex);
                    dialogues[index].sessionManager.unk_19++;
                }

                if (dialogues[index].sessionManager.scrollSfxIndex != 0xFF) {
                    setSfx(dialogues[index].sessionManager.scrollSfxIndex + 1);
                    setSfxVolume(dialogues[index].sessionManager.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                }
                
                set = TRUE;
                
            }
        }

        if (checkButtonRepeat(CONTROLLER_1, 0x100000)) {

            if (!set) {

                if (dialogues[index].sessionManager.unk_17) {

                    dialogues[index].sessionManager.unk_17--;
                    
                    if (dialogues[index].sessionManager.unk_18) {
                        dialogues[index].sessionManager.unk_18--;
                        adjustSpriteViewSpacePosition(dialogues[index].dialogueButtonIcon1.spriteIndex, 0.0f, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].fontContext.characterCellHeight + messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].lineSpacing, 0.0f);
                    } else {
                        func_8003FFF4(dialogues[index].sessionManager.overlayMessageBoxIndex);
                        dialogues[index].sessionManager.unk_19--;
                    }

                    if (dialogues[index].sessionManager.scrollSfxIndex != 0xFF) {
                        setSfx(dialogues[index].sessionManager.scrollSfxIndex + 1);
                        setSfxVolume(dialogues[index].sessionManager.scrollSfxIndex + 1, DIALOGUE_SFX_VOLUME);
                        
                    }
                    
                    set = TRUE;
                
                }
            }
        }

        if (dialogues[index].sessionManager.unk_16 > messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows) { 

            if (messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].textBoxVisibleRows < (dialogues[index].sessionManager.unk_16 - dialogues[index].sessionManager.unk_19)) {
                func_80044BF4(index);
            } else {
                resetAnimationState(dialogues[index].dialogueButtonIcon3.spriteIndex);
            }

            if (dialogues[index].sessionManager.unk_19) {
                func_800449C4(index);
            } else {
                resetAnimationState(dialogues[index].dialogueButtonIcon2.spriteIndex);
            }

        }

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

            if (!set) {
                
                func_80043B84(index);
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
                    initializeMessageBox(dialogues[index].sessionManager.mainMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].textAddressesIndex, dialogues[index].bytecodeExecutor.textIndex, 0x8000);
                }
                
                finishCurrentDialogueBlockProcessing = TRUE;

                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_DIALOGUE_BOX;
                
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
                        dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_DIALOGUE_BOX;
                        
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
                        dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_DIALOGUE_BOX;
                    
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
                        dialogues[index].sessionManager.flags |= DIALOGUE_WAIT_FOR_DIALOGUE_BOX;
                        
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

            case DIALOGUE_OPCODE_SHOW_SUBDIALOGUE_BOX:
                
                dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                dialogues[index].sessionManager.unk_17 = 0;
                dialogues[index].sessionManager.unk_18 = 0;
                dialogues[index].sessionManager.unk_19 = 0;
                dialogues[index].sessionManager.unk_14 = dialogues[index].bytecodeExecutor.textOffset;
                dialogues[index].sessionManager.flags |= DIALOGUE_PAUSE_FOR_USER_INPUT;
                
                initializeMessageBox(dialogues[index].sessionManager.overlayMessageBoxIndex, dialogueBytecodeAddresses[dialogues[index].sessionManager.dialogueBytecodeAddressesIndex].subdialogueTextAddressesIndex, dialogues[index].sessionManager.unk_14, 0);
                
                // probably line height or selection count
                temp2 = messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].unk_A1;
                
                if (temp2 >= 5) {
                    func_8003F5D0(dialogues[index].sessionManager.overlayMessageBoxIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].unk_A0, 4);
                } else {
                    func_8003F5D0(dialogues[index].sessionManager.overlayMessageBoxIndex, messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].unk_A0, temp2);
                }
                
                dialogues[index].sessionManager.unk_16 = messageBoxes[dialogues[index].sessionManager.overlayMessageBoxIndex].unk_A1;
                
                setOverlayMessageBoxSprite(index);
                
                if (dialogues[index].sessionManager.unk_8 != 0xFF) {
                    setSfx(dialogues[index].sessionManager.unk_8 + 1);
                    setSfxVolume(dialogues[index].sessionManager.unk_8 + 1, DIALOGUE_SFX_VOLUME);
                }
                
                finishCurrentDialogueBlockProcessing = TRUE;
                
                break;

            case DIALOGUE_OPCODE_UNUSED:
                break;

            case DIALOGUE_OPCODE_HANDLE_MENU_SELECTION_BRANCH:
                
                temp6 = dialogues[index].sessionManager.unk_17;
                
                if (dialogues[index].bytecodeExecutor.unk_18 == temp6) {
                    
                    initializeDialogueSession(index, dialogues[index].sessionManager.dialogueBytecodeAddressesIndex, dialogues[index].bytecodeExecutor.branchingDialogueIndex, dialogues[index].sessionManager.flags & (0x40 | 0x80));
                    // ?
                    dialogues[index].sessionManager.unk_17 = temp6;

                } else {
                    dialogues[index].bytecodeExecutor.currentOpcode = 0xFF;
                }
                
                break;

            case DIALOGUE_OPCODE_END_DIALOGUE:
                finishCurrentDialogueBlockProcessing = TRUE;
                func_80043AD8(index);
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
            
            if (dialogues[i].sessionManager.flags & DIALOGUE_WAIT_FOR_DIALOGUE_BOX) {
                
                if ((messageBoxes[dialogues[i].sessionManager.mainMessageBoxIndex].flags & 4) || (messageBoxes[dialogues[i].sessionManager.mainMessageBoxIndex].flags & 0x20000)) {
                    dialogues[i].sessionManager.flags &= ~DIALOGUE_WAIT_FOR_DIALOGUE_BOX;
                }
                
                skipDialogueUpdate = TRUE;

            }
            
            if (dialogues[i].sessionManager.flags & DIALOGUE_PAUSE_FOR_USER_INPUT) {
                func_80044D78(i);
                skipDialogueUpdate = TRUE;
            }
            
            if (!skipDialogueUpdate) {
                updateCurrentDialogue(i);
            }

        }
    
    }
    
}
