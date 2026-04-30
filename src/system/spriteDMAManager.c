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
    
    spritesToLoadCounter = 0;

}

bool setSpriteAssetDescriptors(u8 *romAddr, u8 *vaddr, u32 length) {
    
    bool result = FALSE;

    if (spritesToLoadCounter< MAX_SPRITE_ASSET_DESCRIPTORS) {
        SpriteAssetDescriptor *sad = &spriteAssetDescriptors[spritesToLoadCounter];

        if (!(sad->flags & 1)) {

            sad->romAddr = romAddr;
            sad->vaddr = vaddr;
            sad->length = length;
            sad->flags = NEED_DMA;

            spritesToLoadCounter++;

            result = TRUE;

        }
    }
    
    return result;

}

void dmaSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_ASSET_DESCRIPTORS; i++) {
        SpriteAssetDescriptor *sad = &spriteAssetDescriptors[i];
        if (sad->flags & NEED_DMA) {
            dmaReadRom((u32)sad->romAddr, sad->vaddr, sad->length);
            sad->flags &= ~NEED_DMA;
        }
    }

    spritesToLoadCounter = 0;

}
