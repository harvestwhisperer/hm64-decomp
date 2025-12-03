#include "common.h"

#include "system/spriteDMAManager.h"

// bss
SpriteAssetDescriptor spriteAssetDescriptors[MAX_SPRITE_ASSET_DESCRIPTORS];
u16 spritesToLoadCounter;


//INCLUDE_ASM("asm/nonmatchings/system/spriteDMAManager", resetSpriteAssetDescriptorFlags);

void resetSpriteAssetDescriptorFlags(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_ASSET_DESCRIPTORS; i++) {
        spriteAssetDescriptors[i].flags = 0;
    }
    
    spritesToLoadCounter= 0;

}

//INCLUDE_ASM("asm/nonmatchings/system/spriteDMAManager", setSpriteAssetDescriptors);

bool setSpriteAssetDescriptors(u8 *romAddr, u8 *vaddr, u32 length) {
    
    bool result = FALSE;

    if (spritesToLoadCounter< MAX_SPRITE_ASSET_DESCRIPTORS) {
        
        if (!(spriteAssetDescriptors[spritesToLoadCounter].flags & 1)) {
            
            spriteAssetDescriptors[spritesToLoadCounter].romAddr = romAddr;
            spriteAssetDescriptors[spritesToLoadCounter].vaddr = vaddr;
            spriteAssetDescriptors[spritesToLoadCounter].length = length;
            spriteAssetDescriptors[spritesToLoadCounter].flags = NEED_DMA;
            
            spritesToLoadCounter++;

            result = TRUE;
        
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/spriteDMAManager", dmaSprites);

void dmaSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_ASSET_DESCRIPTORS; i++) {
        if (spriteAssetDescriptors[i].flags & NEED_DMA) {
            nuPiReadRom(spriteAssetDescriptors[i].romAddr, spriteAssetDescriptors[i].vaddr, spriteAssetDescriptors[i].length);
            spriteAssetDescriptors[i].flags &= ~NEED_DMA;
        }
    }

    spritesToLoadCounter = 0;

}
