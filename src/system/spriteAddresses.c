#include "common.h"

#include "system/sprite.h"

// bss
extern SpriteInfo spriteAddresses[MAX_SPRITE_INFO];

extern u16 spritesToLoadCounter;


//INCLUDE_ASM(const s32, "system/spriteAddresses", resetSpriteAddressesFlags);

void resetSpriteAddressesFlags(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_INFO; i++) {
        spriteAddresses[i].flags = 0;
    }
    
    spritesToLoadCounter= 0;

}

//INCLUDE_ASM(const s32, "system/spriteAddresses", setSpriteAddresses);

bool setSpriteAddresses(u8 *romAddr, u8 *vaddr, u32 length) {
    
    bool result = FALSE;

    if (spritesToLoadCounter< MAX_SPRITE_INFO) {
        
        if (!(spriteAddresses[spritesToLoadCounter].flags & 1)) {
            
            spriteAddresses[spritesToLoadCounter].romAddr = romAddr;
            spriteAddresses[spritesToLoadCounter].vaddr = vaddr;
            spriteAddresses[spritesToLoadCounter].length = length;
            spriteAddresses[spritesToLoadCounter].flags = 1;
            
            spritesToLoadCounter++;

            result = TRUE;
        
        }
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/spriteAddresses", dmaSprites);

void dmaSprites(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_INFO; i++) {
        if (spriteAddresses[i].flags & NEED_DMA) {
            nuPiReadRom(spriteAddresses[i].romAddr, spriteAddresses[i].vaddr, spriteAddresses[i].length);
            spriteAddresses[i].flags &= ~NEED_DMA;
        }
    }

    spritesToLoadCounter= 0;

}
