#include "common.h"
#include "yay0.h"

#include "system/spriteDMAManager.h"

// bss
SpriteAssetDescriptor spriteAssetDescriptors[MAX_SPRITE_ASSET_DESCRIPTORS];
u16 spritesToLoadCounter;

void initializeSpriteAssetDescriptorFlags(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_ASSET_DESCRIPTORS; i++) {
        spriteAssetDescriptors[i].flags = 0;
    }
    
    spritesToLoadCounter= 0;

}

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

void dmaSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_ASSET_DESCRIPTORS; i++) {
        if (spriteAssetDescriptors[i].flags & NEED_DMA) {
            dmaReadRom((u32)spriteAssetDescriptors[i].romAddr, spriteAssetDescriptors[i].vaddr, spriteAssetDescriptors[i].length);
            spriteAssetDescriptors[i].flags &= ~NEED_DMA;
        }
    }

    spritesToLoadCounter = 0;

}
