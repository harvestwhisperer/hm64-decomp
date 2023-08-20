#include "common.h"
#include "npc.h"

u8 func_80075374(u8, u8);                        
void setNpcAnimations();                         

void setNPCSpawningLocations();                     


INCLUDE_ASM(const s32, "unknownGame", func_800752C0);

// jtbl_80120010
INCLUDE_ASM(const s32, "unknownGame", func_80075374);

//INCLUDE_ASM(const s32, "unknownGame", func_800758B8);

void func_800758B8(void) {

    u8 spriteIndex;
    u8 i;

    setNPCSpawningLocations();
    
    spriteIndex = 0;
    
    for (i = 0; i < TOTAL_NPCS; i++) {
        spriteIndex = func_80075374(i, spriteIndex);
    }
    
    // handle all npc animations
    setNpcAnimations();
}
