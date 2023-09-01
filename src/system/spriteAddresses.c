#include "common.h"

#include "system/sprite.h"

// bss
extern SpriteInfo spriteAddresses[MAX_SPRITE_INFO];

extern u16 D_801FADB2;


//INCLUDE_ASM(const s32, "system/spriteAddresses", resetSpriteAddressesFlags);

void resetSpriteAddressesFlags(void) {

    u16 i;

    for (i = 0; i < MAX_SPRITE_INFO; i++) {
        spriteAddresses[i].flags = 0;
    }
    
    D_801FADB2 = 0;
}

//INCLUDE_ASM(const s32, "system/spriteAddresses", setSpriteAddresses);

u16 setSpriteAddresses(u8 *romAddr, u8 *vaddr, u32 length) {
    
    u16 result = 0;

    if (D_801FADB2 < MAX_SPRITE_INFO) {
        if (!(spriteAddresses[D_801FADB2].flags & 1)) {
            spriteAddresses[D_801FADB2].romAddr = romAddr;
            spriteAddresses[D_801FADB2].vaddr = vaddr;
            spriteAddresses[D_801FADB2].length = length;
            spriteAddresses[D_801FADB2].flags = 1;
            D_801FADB2++;
            result = 1;
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

    D_801FADB2 = 0;
}
