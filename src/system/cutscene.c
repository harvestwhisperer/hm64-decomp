#include "common.h"

#include "cutscene.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/map.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/mapContext.h"

// forward declarations
void func_800471B0(u16);                               
void func_80047640(u16);                               
void func_80047E34(u16);                               

// bss
// cutscene handler callbacks
extern void (*cutsceneCallbacksTable[])(u16);
 
extern CutsceneMap cutsceneMaps[MAX_CUTSCENE_ASSETS];

//INCLUDE_ASM(const s32, "system/cutscene", initializeCutsceneMaps);

void initializeCutsceneMaps(void) {

    u16 i;

    for (i = 0; i < MAX_CUTSCENE_ASSETS; i++) {
        
        cutsceneMaps[i].spriteIndex = 0;
        cutsceneMaps[i].unk_66 = 0;
        cutsceneMaps[i].unk_68 = 0;
        cutsceneMaps[i].unk_69 = 0;
        cutsceneMaps[i].unk_6A = 0;

        cutsceneMaps[i].offsets.x = 0;
        cutsceneMaps[i].offsets.y = 0;
        cutsceneMaps[i].offsets.z = 0;

        cutsceneMaps[i].unk_58.x = 0;
        cutsceneMaps[i].unk_58.y = 0;
        cutsceneMaps[i].unk_58.z = 0;

        cutsceneMaps[i].flags = 0;

        cutsceneMaps[i].unk_20 = 0;
        cutsceneMaps[i].unk_22 = 0;
        cutsceneMaps[i].unk_24 = 0;
        cutsceneMaps[i].unk_26 = 0;
        cutsceneMaps[i].unk_28 = 0;
        cutsceneMaps[i].unk_2A = 0;

        cutsceneMaps[i].unk_1C = 0xFFFF;

        cutsceneMaps[i].unk_C.x = 0;
        cutsceneMaps[i].unk_C.y = 0;
        cutsceneMaps[i].unk_C.z = 0;

        cutsceneMaps[i].cameraFlags = 0;
    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800469A8);

bool func_800469A8(u16 index, void *cutscenePointer) {

    bool result = 0;
    
    if (index < MAX_CUTSCENE_ASSETS) {

        if (!(cutsceneMaps[index].flags & ACTIVE)) {
            
            result = 1;
            
            cutsceneMaps[index].cutscenePointer = cutscenePointer;
            
            cutsceneMaps[index].unk_66 = 0;
            
            cutsceneMaps[index].offsets.x = 0;
            cutsceneMaps[index].offsets.y = 0;
            cutsceneMaps[index].offsets.z = 0;

            cutsceneMaps[index].unk_58.x = 0;
            cutsceneMaps[index].unk_58.y = 0;
            cutsceneMaps[index].unk_58.z = 0;

            cutsceneMaps[index].flags = 1;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046A58);

bool func_80046A58(u16 index, void* cutscenePointer) {
    
    bool result = 0;

    if (index < MAX_CUTSCENE_ASSETS) {
        
        if (cutsceneMaps[index].flags & ACTIVE) {
            result = 1;
            cutsceneMaps[index].unk_66 = 0;
            cutsceneMaps[index].cutscenePointer = cutscenePointer;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046AB0);

// toggle flags at end of cutscene work
bool func_80046AB0(u16 index) {

    bool result = 0;

    if (index < MAX_CUTSCENE_ASSETS && (cutsceneMaps[index].flags & ACTIVE)) {

        // global sprite flags
        if (cutsceneMaps[index].flags & 2) {
            func_8002B6B8(cutsceneMaps[index].spriteIndex);
        }

        // global sprite flags
        if (cutsceneMaps[index].flags & CHARACTER_SPRITE_ASSET) {
            func_8002FA2C(cutsceneMaps[index].spriteIndex);
        }

        // tiles/map
        if (cutsceneMaps[index].flags & MAP_ASSET) {
            func_8003C504(cutsceneMaps[index].spriteIndex);
        }
        
        result = 1;
    }

    cutsceneMaps[index].unk_1C = 0xFFFF;
    cutsceneMaps[index].flags = 0;

    return result;

}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046BB8);

void func_80046BB8(void) {

    u16 i;

    for (i = 0; i < MAX_CUTSCENE_ASSETS; i++) {
        // update flags on assets
        func_80046AB0(i);
    }

}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046BF8);

bool func_80046BF8(u16 index) {

    bool result = 0;
    
    if (index < MAX_CUTSCENE_ASSETS && (cutsceneMaps[index].flags & ACTIVE)) {
        cutsceneMaps[index].flags |= 0x20;
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046C48);

bool func_80046C48(u16 index) {

    bool result = 0;
    
    if (index < MAX_CUTSCENE_ASSETS && (cutsceneMaps[index].flags & ACTIVE)) {
        cutsceneMaps[index].flags &= ~0x20;
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046C98);

void func_80046C98(void) {

    u16 i = 0;
    u16 j;
    
    // very odd and unnecessary
    for (; ;) {
        j = i;
        if (j < MAX_CUTSCENE_ASSETS) {
            if (cutsceneMaps[j].flags & ACTIVE) {
                cutsceneMaps[j].flags |= 0x20;
            }
        }
        i++;
        if (i > 0x29) break;
    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046CF4);

void func_80046CF4(void) {

    u16 i = 0;
    u16 j;
    
    for (; ;) {
        j = i;
        if (j < MAX_CUTSCENE_ASSETS) {
            if (cutsceneMaps[j].flags & ACTIVE) {
                cutsceneMaps[j].flags &= ~0x20;
            }
        }
        i++;
        if (i > 0x29) break;
    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046D50);

// same as adjustValue
inline int func_80046D50(int initial, int value, int max) {
    
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
 
//INCLUDE_ASM(const s32, "system/cutscene", mainLoopCutsceneHandler);

void mainLoopCutsceneHandler(void) {

    u16 i;

    for (i = 0; i < MAX_CUTSCENE_ASSETS; i++) {
        
        if (cutsceneMaps[i].flags & 1 && !(cutsceneMaps[i].flags & 0x20)) {
            
            if (cutsceneMaps[i].flags & CHARACTER_SPRITE_ASSET) {
                
                cutsceneMaps[i].coordinates.x = renderedSprites[cutsceneMaps[i].spriteIndex].startingCoordinates.x;
                cutsceneMaps[i].coordinates.y = renderedSprites[cutsceneMaps[i].spriteIndex].startingCoordinates.y;
                cutsceneMaps[i].coordinates.z = renderedSprites[cutsceneMaps[i].spriteIndex].startingCoordinates.z;
                
                if (cutsceneMaps[i].unk_1C != 0xFFFF && renderedSprites[cutsceneMaps[i].spriteIndex].unk_58 == cutsceneMaps[i].unk_1C && cutsceneMaps[i].unk_1E == renderedSprites[cutsceneMaps[i].spriteIndex].unk_5A) {
                    cutsceneMaps[i].unk_1C = 0xFFFF;
                    cutsceneMaps[i].unk_66 = 0;
                    cutsceneMaps[i].cutscenePointer = cutsceneMaps[i].unk_18;
                
                }
            }
            
            if (cutsceneMaps[i].flags & MAP_ASSET) {
                cutsceneMaps[i].coordinates.x = gMapModelContext[cutsceneMaps[i].spriteIndex].unk_4.x;
                cutsceneMaps[i].coordinates.y = gMapModelContext[cutsceneMaps[i].spriteIndex].unk_4.y;
                cutsceneMaps[i].coordinates.z = gMapModelContext[cutsceneMaps[i].spriteIndex].unk_4.z;
            }

            if (cutsceneMaps[i].unk_66) {
                 cutsceneMaps[i].unk_66--;
                if (cutsceneMaps[i].unk_66) {
                    goto skip_callback;
                }
            }
      
 inner_loop:    
            while (1) {
                
                cutsceneMaps[i].offsets.x = 0;
                cutsceneMaps[i].offsets.y = 0;
                cutsceneMaps[i].offsets.z = 0;

                cutsceneCallbacksTable[*(u16*)(cutsceneMaps[i].cutscenePointer)](i);
                
skip_callback:
                cutsceneMaps[i].coordinates.x += cutsceneMaps[i].offsets.x;
                cutsceneMaps[i].coordinates.y += cutsceneMaps[i].offsets.y;
                cutsceneMaps[i].coordinates.z += cutsceneMaps[i].offsets.z;

                if (!cutsceneMaps[i].unk_66)  {
                    goto inner_loop;
                } else {
                    break;
                }
        
            }
            
            // sprite handling
            if (cutsceneMaps[i].flags & MAP_ASSET) {
                func_80047E34(i);
            }
            
            if (cutsceneMaps[i].flags & 2) {
                func_800471B0(i);
            }

            if (cutsceneMaps[i].flags & CHARACTER_SPRITE_ASSET) {
                func_80047640(i);
            }
        }
    }
}

INCLUDE_ASM(const s32, "system/cutscene", func_800471B0);

//INCLUDE_ASM(const s32, "system/cutscene", func_800475B4);

void func_800475B4(u16 index) {
    cutsceneMaps[index].unk_68 = 1;
    cutsceneMaps[index].flags &= ~2;
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800475F8); 

void func_800475F8(u16 index) {

    cutsceneMaps[index].unk_4 += 2;
    cutsceneMaps[index].unk_4 = cutsceneMaps[index].unk_4 + *(s16*)(cutsceneMaps[index].unk_4);

}

/*
void func_800475F8(u16 index) {
    int temp;
    cutsceneMaps[index].unk_4 += 2;
    temp = (s16*)cutsceneMaps[index].unk_4;
    cutsceneMaps[index].unk_4 = temp + *(s16*)(temp);
}
*/

/*
void func_800475F8(u16 index) {

    int temp;
    cutsceneMaps[index].unk_4 += 2;
    temp = cutsceneMaps[index].unk_4;
    cutsceneMaps[index].unk_4 = temp + *(s16*)cutsceneMaps[index].unk_4;
    
}

*/
  
INCLUDE_ASM(const s32, "system/cutscene", func_80047640);

//INCLUDE_ASM(const s32, "system/cutscene", func_80047E34);

// tiles
void func_80047E34(u16 index) {
    func_8003BE0C(cutsceneMaps[index].spriteIndex, cutsceneMaps[index].offsets.x, cutsceneMaps[index].offsets.y, cutsceneMaps[index].offsets.z);
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80047E94);

// function index 0
void func_80047E94(u16 index) {
    
    short offset;
    
    cutsceneMaps[index].cutscenePointer += 2;

    offset = *(short*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].unk_4 = cutsceneMaps[index].cutscenePointer + offset;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    cutsceneMaps[index].unk_68 = 0;
    cutsceneMaps[index].flags |= 2;
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80047F20);

// function index 1
void func_80047F20(u16 index) {

    u32 offset;

    CutscenePointersUpdate *ptr = (CutscenePointersUpdate*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    offset = ptr->offset;
    
    cutsceneMaps[index].unk_4 = cutsceneMaps[index].cutscenePointer + offset;
    cutsceneMaps[index].unk_68 = 0;
    
    cutsceneMaps[index].cutscenePointer += 2;
}

// alternate
/*
void func_80047F20(u16 index) {
    
    u32 offset;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    offset = ((UnknownCutsceneMapUpdate*)cutsceneMaps[index].cutscenePointer)->unk_0;
    
    cutsceneMaps[index].unk_4 = cutsceneMaps[index].cutscenePointer + offset;
    cutsceneMaps[index].unk_68 = 0;
    
    cutsceneMaps[index].cutscenePointer += 2;
}
*/

// alternate
/*
void func_80047F20(u16 index) {
    
    u32 offset;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    offset = *(short*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].unk_4 = cutsceneMaps[index].cutscenePointer + offset;
    cutsceneMaps[index].unk_68 = 0;
    
    cutsceneMaps[index].cutscenePointer += 2;
}
*/

//INCLUDE_ASM(const s32, "system/cutscene", func_80047F90);

// function index 2
void func_80047F90(u16 index) {

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].coordinates.x = *(s16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].coordinates.y = *(s16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].coordinates.z = *(s16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    // tiles
    if (cutsceneMaps[index].flags & MAP_ASSET) {
        func_8003BDA4(cutsceneMaps[index].spriteIndex, cutsceneMaps[index].coordinates.x, cutsceneMaps[index].coordinates.y, cutsceneMaps[index].coordinates.z);
    }

    // set sprite coordinates
    if (cutsceneMaps[index].flags & CHARACTER_SPRITE_ASSET) {
        func_8002FD80(cutsceneMaps[index].spriteIndex, cutsceneMaps[index].coordinates.x, cutsceneMaps[index].coordinates.y, cutsceneMaps[index].coordinates.z);
    }
}

INCLUDE_ASM(const s32, "system/cutscene", func_80048124);

//INCLUDE_ASM(const s32, "system/cutscene", func_80048258);

// idle function
void func_80048258(u16 index) {

    (u16*)cutsceneMaps[index].cutscenePointer += 1;
    cutsceneMaps[index].unk_66 = *(u16*)cutsceneMaps[index].cutscenePointer;
    (u16*)cutsceneMaps[index].cutscenePointer += 1;
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800482B8);

// function index 5
// cutscene object finish
void func_800482B8(u16 index) {

    // set done
    cutsceneMaps[index].unk_66 = 1;
    // update flags on asset
    func_80046AB0(index);

}

//INCLUDE_ASM(const s32, "system/cutscene", func_800482F8);

void func_800482F8(u16 index) {

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer+2;

    cutsceneMaps[index].cutscenePointer += *(s16*)cutsceneMaps[index].cutscenePointer;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004835C);

void func_8004835C(u16 index) {
    cutsceneMaps[index].cutscenePointer = cutsceneMaps[index].unk_8;
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004838C);

void func_8004838C(u16 index) {

    CutsceneButtonCheck* ptr = (CutsceneButtonCheck*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    if (controllers[ptr->controllerIndex].button & ptr->buttonMask) {

        cutsceneMaps[index].cutscenePointer += 6;
        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(short*)cutsceneMaps[index].cutscenePointer; 
        
    } else {
        cutsceneMaps[index].cutscenePointer += 0xA;
    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048430);

void func_80048430(u16 index) {

    CutsceneButtonCheck* ptr = cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    if (controllers[ptr->controllerIndex].buttonPressed & ptr->buttonMask) {
        cutsceneMaps[index].cutscenePointer += 6;
        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(short*)cutsceneMaps[index].cutscenePointer; 
        
    } else {
        cutsceneMaps[index].cutscenePointer += 0xA;
    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800484D4);

void func_800484D4(u16 index) {

    CutsceneButtonCheck* ptr = (CutsceneButtonCheck*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    if (controllers[ptr->controllerIndex].unk_24 & ptr->buttonMask) {

        cutsceneMaps[index].cutscenePointer += 6;
        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(short*)cutsceneMaps[index].cutscenePointer; 
        
    } else {
        cutsceneMaps[index].cutscenePointer += 0xA;
    }
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048578);

// function index 0xB
// sets child cutscene map objects
void func_80048578(u16 index) {

    CutsceneAssetInfo* ptr = (CutsceneAssetInfo*)cutsceneMaps[index].cutscenePointer;

    u16 assetIndex;
    int offset;
    u32 ptr2;
    
    cutsceneMaps[index].cutscenePointer += 2;

    assetIndex = ptr->assetIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    offset = ptr->offset;
    ptr2 = cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    ptr2 += offset;
    
    func_800469A8(assetIndex, ptr2);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800485EC);

void func_800485EC(u16 index) {

    CutsceneAssetInfo* ptr = (CutsceneAssetInfo*)cutsceneMaps[index].cutscenePointer;
    u16 assetIndex;
    u32 offset;
    u32 ptr2;

    cutsceneMaps[index].cutscenePointer += 2;

    assetIndex = ptr->assetIndex;

    cutsceneMaps[index].cutscenePointer += 2; 

    ptr2 = cutsceneMaps[index].cutscenePointer;

    offset = ptr->offset;

    cutsceneMaps[index].cutscenePointer += 4;

    ptr2 += offset;
    
    if (assetIndex < MAX_CUTSCENE_ASSETS) {
        
        if (cutsceneMaps[assetIndex].flags & 1) {
            cutsceneMaps[assetIndex].unk_66 = 0;
            cutsceneMaps[assetIndex].cutscenePointer = ptr2;
        }
    }
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048694);

void func_80048694(u16 index) {

    u16 cutsceneMapIndex;

    CutsceneAssetFlagsInfo* ptr = (CutsceneAssetFlagsInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMapIndex = ptr->cutsceneMapIndex;

    cutsceneMaps[index].cutscenePointer += 2;
    
    func_80046AB0(cutsceneMapIndex);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800486F4);

// function index 0xE
void func_800486F4(u16 index) {

    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romAssetIndexStart;
    u32 romAssetIndexEnd;
    u32 romSpritesheetIndexStart;
    u32 romSpritesheetIndexEnd;
    u8* texture1Vaddr;
    u8* texture2Vaddr;
    u16* paletteVaddr;
    u16* animationVaddr;
    u8* spriteToPaletteVaddr;
    u32* spritesheetIndexVaddr;
    u16 assetType;

    CutsceneSpriteInfo* ptr;
    CutsceneSpriteAddressInfo* ptr2;

    ptr = (CutsceneSpriteInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].spriteIndex = ptr->spriteIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;

    // shoulde be the same struct
    ptr2 = (CutsceneSpriteAddressInfo*)cutsceneMaps[index].cutscenePointer;

    assetType = ptr2->assetType;

    cutsceneMaps[index].cutscenePointer += 4;

    romTextureStart = ptr2->romTextureStart;

    cutsceneMaps[index].cutscenePointer += 4;

    romTextureEnd = ptr2->romTextureEnd;

    cutsceneMaps[index].cutscenePointer += 4;

    romAssetIndexStart = ptr2->romAssetIndexStart;
    
    cutsceneMaps[index].cutscenePointer += 4;

    romAssetIndexEnd = ptr2->romAssetIndexEnd;

    cutsceneMaps[index].cutscenePointer += 4;

    romSpritesheetIndexStart = ptr2->romSpritesheetIndexStart;

    cutsceneMaps[index].cutscenePointer += 4;

    romSpritesheetIndexEnd = ptr2->romSpritesheetIndexEnd;

    cutsceneMaps[index].cutscenePointer += 4;

    texture1Vaddr = ptr2->texture1Vaddr;

    cutsceneMaps[index].cutscenePointer += 4;

    texture2Vaddr = ptr2->texture2Vaddr;

    cutsceneMaps[index].cutscenePointer += 4;

    paletteVaddr = ptr2->paletteVaddr;

    cutsceneMaps[index].cutscenePointer += 4;

    animationVaddr = ptr2->animationVaddr;

    cutsceneMaps[index].cutscenePointer += 4;

    spriteToPaletteVaddr = ptr2->spriteToPaletteVaddr;

    cutsceneMaps[index].cutscenePointer += 4;
 
    spritesheetIndexVaddr = ptr2->spritesheetIndexVaddr;

    cutsceneMaps[index].cutscenePointer += 4;

    func_8002B138(cutsceneMaps[index].spriteIndex, romTextureStart, romTextureEnd, romAssetIndexStart, 
        romAssetIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   

    func_8002CB24(cutsceneMaps[index].spriteIndex, 1);
    
}

// alternate
/*
void func_800486F4(u16 index) {

    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romAssetIndexStart;
    u32 romAssetIndexEnd;
    u32 romSpritesheetIndexStart;
    u32 romSpritesheetIndexEnd;
    u8* texture1Vaddr;
    u8* texture2Vaddr;
    u16* paletteVaddr;
    u16* animationVaddr;
    u8* spriteToPaletteVaddr;
    u32* spritesheetIndexVaddr;
    u16 assetType;

    CutsceneSpriteAddressInfo* ptr = (CutsceneSpriteAddressInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].spriteIndex = ptr->spriteIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;

    assetType = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    romTextureStart = *(u32*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    romTextureEnd = *(u32*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    romAssetIndexStart = *(u32*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 4;

    romAssetIndexEnd = *(u32*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    romSpritesheetIndexStart = *(u32*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    romSpritesheetIndexEnd = *(u32*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    texture1Vaddr = *(u8**)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    texture2Vaddr = *(u8**)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    paletteVaddr = *(u16**)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    animationVaddr = *(u16**)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    spriteToPaletteVaddr = *(u8**)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;

    spritesheetIndexVaddr = *(u32**)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;
    
    func_8002B138(cutsceneMaps[index].spriteIndex, romTextureStart, romTextureEnd, romAssetIndexStart, 
        romAssetIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   

    func_8002CB24(cutsceneMaps[index].spriteIndex, 1);
    
}
*/

//INCLUDE_ASM(const s32, "system/cutscene", func_800488CC);

void func_800488CC(u16 index) {

    CutsceneSpriteInfo3* ptr = (CutsceneSpriteInfo3*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].spriteIndex = ptr->spriteIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].unk_20 = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    cutsceneMaps[index].unk_22 = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].unk_24 = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].unk_26 = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].unk_28 = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].unk_2A = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    if (cutsceneMaps[index].cameraFlags & MAP_ASSET) {
        setSpriteAnimation(cutsceneMaps[index].spriteIndex, cutsceneMaps[index].unk_26);
    } else {
        setSpriteAnimation(cutsceneMaps[index].spriteIndex, cutsceneMaps[index].unk_20);
    }

    func_8002FCB4(cutsceneMaps[index].spriteIndex, 0);

    cutsceneMaps[index].flags |= 8;
    
    cutsceneMaps[index].cameraFlags |= 1;
    cutsceneMaps[index].cameraFlags &= ~(2 | 4);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048B08);

void func_80048B08(u16 index) {

    CutsceneDMA* ptr = (CutsceneDMA*)cutsceneMaps[index].cutscenePointer;
    
    u32 romAddrStart;
    u32 romAddrEnd; 
    void* vaddr;
    
    cutsceneMaps[index].cutscenePointer += 4;
    
    romAddrStart = ptr->romAddrStart;
    
    cutsceneMaps[index].cutscenePointer += 4;

    romAddrEnd = ptr->romAddrEnd;

    cutsceneMaps[index].cutscenePointer += 4;

    vaddr = ptr->vaddr;
    
    cutsceneMaps[index].cutscenePointer += 4;
    
    nuPiReadRom(romAddrStart, vaddr, romAddrEnd - romAddrStart);

}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048B90);

void func_80048B90(u16 index) {

    u8 nextFunctionIndex;

    CutsceneUnknownInfo* ptr2;
    
    CutsceneUnknownInfo* ptr = (CutsceneUnknownInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    nextFunctionIndex = ptr->nextFunctionIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    ptr2 = *(CutsceneUnknownInfo**)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 4;

    ptr2->functionIndex = nextFunctionIndex;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048BEC);

void func_80048BEC(u16 index) {

    CutsceneUnknownInfo2* ptr = (CutsceneUnknownInfo2*)cutsceneMaps[index].cutscenePointer;

    u16 unk_2;
    u16* unk_4;
    
    cutsceneMaps[index].cutscenePointer += 2;

    unk_2 = ptr->unk_2;

    cutsceneMaps[index].cutscenePointer += 2;

    unk_4 = ptr->unk_4;

    cutsceneMaps[index].cutscenePointer += 4;
    
    *unk_4 = unk_2;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048C48);

void func_80048C48(u16 index) {

    CutsceneUnknownInfo6* ptr = (CutsceneUnknownInfo6*)cutsceneMaps[index].cutscenePointer;

    u32 unk_4;
    u32* unk_8;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_4 = ptr->unk_4;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_8 = ptr->unk_8;

    cutsceneMaps[index].cutscenePointer += 4;

    *unk_8 = unk_4;
    
}

INCLUDE_ASM(const s32, "system/cutscene", func_80048CA4);


//INCLUDE_ASM(const s32, "system/cutscene", func_80048DA8);

// checks cutscene index until current cutscene index
void func_80048DA8(u16 index) {
    
    u16* cutsceneIndexPtr;
    u16 cutsceneIndex1;
    u16 cutsceneIndex2;

    CutsceneIndexCheck* ptr = (CutsceneIndexCheck*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    cutsceneIndex1 = ptr->cutsceneIndex1;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneIndex2 = ptr->cutsceneIndex2;

    cutsceneMaps[index].cutscenePointer += 4;    

    cutsceneIndexPtr = ptr->cutsceneIndexPtr;

    cutsceneMaps[index].cutscenePointer += 4;    

    if (*cutsceneIndexPtr < cutsceneIndex1) {
        goto func_end;  
    } 
    
    if (cutsceneIndex2 >= *cutsceneIndexPtr) {
        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(s16*)cutsceneMaps[index].cutscenePointer;
        return;
    }
    
func_end:
    cutsceneMaps[index].cutscenePointer += 4;
        
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048E98);

void func_80048E98(u16 index) {

    CutsceneUnknownInfo3* ptr = (CutsceneUnknownInfo3*)cutsceneMaps[index].cutscenePointer;
    u32 ptr2;

    u16 unk_2;
    u32 unk_4;
    u32 unk_8;
    u32* unk_C;

    cutsceneMaps[index].cutscenePointer += 2;

    unk_2 = ptr->unk_2;

    cutsceneMaps[index].cutscenePointer += 2;

    unk_4 = ptr->unk_4;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_8 = ptr->unk_8;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_C = ptr->unk_C;

    cutsceneMaps[index].cutscenePointer += 4;
 
    if (*unk_C >= unk_4 && unk_8 >= *unk_C) {

        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(s16*)cutsceneMaps[index].cutscenePointer;
        
    } else {
        cutsceneMaps[index].cutscenePointer += 4;
    }

}

//INCLUDE_ASM(const s32, "system/cutscene", func_80048F88);

// index 0x17
void func_80048F88(u16 index) {

    CutsceneMapFlagUpdate* ptr = (CutsceneMapFlagUpdate*)cutsceneMaps[index].cutscenePointer;
    
    u16 flag;
    s32* cutsceneFlagPointer;

    cutsceneMaps[index].cutscenePointer += 2;

    flag = ptr->flag;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneFlagPointer = ptr->cutsceneFlagPointer;

    cutsceneMaps[index].cutscenePointer += 4;

    *cutsceneFlagPointer |= 1 << flag;

}

// toggle bit
INCLUDE_ASM(const s32, "system/cutscene", func_80048FF4);

INCLUDE_ASM(const s32, "system/cutscene", func_80049064);

INCLUDE_ASM(const s32, "system/cutscene", func_8004910C);

//INCLUDE_ASM(const s32, "system/cutscene", func_80049228);

// function index 0x1D
// set up cutscene object as map asset
void func_80049228(u16 index) {

    CutsceneMapInfo *ptr = cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMaps[index].spriteIndex = ptr->index;

    cutsceneMaps[index].cutscenePointer += 2;

    func_8003C1E0(cutsceneMaps[index].spriteIndex, gMapModelContext[cutsceneMaps[index].spriteIndex].unk_4.x, gMapModelContext[cutsceneMaps[index].spriteIndex].unk_4.y, gMapModelContext[cutsceneMaps[index].spriteIndex].unk_4.z, 8, 8);
    func_8003BD60(cutsceneMaps[index].spriteIndex);
    func_8002EEA4(cutsceneMaps[index].spriteIndex);

    cutsceneMaps[index].flags |= MAP_ASSET;
    
} 

INCLUDE_ASM(const s32, "system/cutscene", func_80049350);

INCLUDE_ASM(const s32, "system/cutscene", func_800495F0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004969C);

INCLUDE_ASM(const s32, "system/cutscene", func_80049708);

INCLUDE_ASM(const s32, "system/cutscene", func_80049828);

INCLUDE_ASM(const s32, "system/cutscene", func_800498B0);

INCLUDE_ASM(const s32, "system/cutscene", func_80049950);

INCLUDE_ASM(const s32, "system/cutscene", func_80049A28);

INCLUDE_ASM(const s32, "system/cutscene", func_80049AC4);

INCLUDE_ASM(const s32, "system/cutscene", func_80049D64);

INCLUDE_ASM(const s32, "system/cutscene", func_80049DF4);

INCLUDE_ASM(const s32, "system/cutscene", func_80049E84);

INCLUDE_ASM(const s32, "system/cutscene", func_80049F40);

INCLUDE_ASM(const s32, "system/cutscene", func_80049FA0);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004A000);

void func_8004A000(u16 index) {

    u16 spriteIndex;
    
    CutsceneSpriteInfo* ptr = (CutsceneSpriteInfo*)cutsceneMaps[index].cutscenePointer; 

    cutsceneMaps[index].cutscenePointer += 2;

    spriteIndex = ptr->spriteIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    setSpriteDirection(cutsceneMaps[index].spriteIndex, (renderedSprites[spriteIndex].direction + 4)  % 8);
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004A0A8);

// set flags on all active sprites, rendered and global
void func_8004A0A8(u16 index) {

    cutsceneMaps[index].cutscenePointer += 4;
    
    func_8002F6F0();
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004A0F4);

void func_8004A0F4(u16 index) {

    cutsceneMaps[index].cutscenePointer += 4;

    func_8002F730(index);

}

INCLUDE_ASM(const s32, "system/cutscene", func_8004A140);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A1DC);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004A234);

void func_8004A234(u16 index) {

    CutsceneSpriteInfo4* ptr = (CutsceneSpriteInfo4*)cutsceneMaps[index].cutscenePointer;
    
    u16 spriteIndex;
    u16 characterIndex;
    u16 flag;

    cutsceneMaps[index].cutscenePointer += 2;

    spriteIndex = ptr->spriteIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    characterIndex = ptr->characterIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;

    flag = ptr->flag;
    
    cutsceneMaps[index].cutscenePointer += 2;

    func_8002E284(spriteIndex, characterIndex, flag);
    func_8002ECD4(spriteIndex, 0xFFFF, 0xFF);
    func_8002EDF0(spriteIndex, 0, 0, 0);
    func_8003019C(spriteIndex, 1);
    func_80030054(spriteIndex, 1);
    func_80030240(spriteIndex, 1);
    func_800302E4(spriteIndex, 1);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004A320);

void func_8004A320(u16 index) {

    CutsceneSpriteInfo2* ptr = (CutsceneSpriteInfo2*)cutsceneMaps[index].cutscenePointer;

    u16 flag;

    cutsceneMaps[index].cutscenePointer +=  2;

    flag = ptr->flag;

    cutsceneMaps[index].cutscenePointer += 2;

    func_8002FCB4(cutsceneMaps[index].spriteIndex, flag);

}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004A38C);

void func_8004A38C(u16 index) {

    CutsceneMapModelContextInfo* ptr = (CutsceneMapModelContextInfo*)cutsceneMaps[index].cutscenePointer;
    
    u16 mapModelContextIndex;
    u16 mapIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    mapModelContextIndex = ptr->mapModelContextIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    mapIndex = ptr->mainMapIndex;

    cutsceneMaps[index].cutscenePointer += 4;

    // set Vec3fs on mapStruct8
    func_8003BB14(mapModelContextIndex, mapIndex);

}


INCLUDE_ASM(const s32, "system/cutscene", func_8004A400);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A47C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A7A4);

INCLUDE_ASM(const s32, "system/cutscene", func_8004AB04);

INCLUDE_ASM(const s32, "system/cutscene", func_8004ABA8);

INCLUDE_ASM(const s32, "system/cutscene", func_8004AC4C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004ACE4);

INCLUDE_ASM(const s32, "system/cutscene", func_8004AD44);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B050);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B09C);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004B0E8);

// index 0x3C
void func_8004B0E8(u16 index) {

    cutsceneMaps[index].cutscenePointer += 2;
    
    if (cutsceneMaps[index].flags & 2) {

        if (func_8002CBF8(cutsceneMaps[index].spriteIndex)) {
            cutsceneMaps[index].cutscenePointer += 2;
        } else {
            cutsceneMaps[index].unk_66 = 1;
            cutsceneMaps[index].cutscenePointer -= 2; 
        }
        
    }

    if (cutsceneMaps[index].flags & 8) {
        if (func_8002CBF8(renderedSprites[cutsceneMaps[index].spriteIndex].globalSpriteIndex)) {
            cutsceneMaps[index].cutscenePointer += 2;
        } else {
            cutsceneMaps[index].unk_66 = 1;
            cutsceneMaps[index].cutscenePointer -= 2; 
        }
    }

    if (cutsceneMaps[index].flags & 0x10) {
        if (func_80036A84(gMapModelContext[cutsceneMaps[index].spriteIndex].mainMapIndex)) {
            cutsceneMaps[index].cutscenePointer += 2;
        } else {
            cutsceneMaps[index].unk_66 = 1;
            cutsceneMaps[index].cutscenePointer -= 2; 
        }
    }
    
}

INCLUDE_ASM(const s32, "system/cutscene", func_8004B2FC);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B410);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B498);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B538);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B5F0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B700);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B7B8);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B920);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004B9A0);

void func_8004B9A0(u16 index) {

    CutsceneMapInfo* ptr = (CutsceneMapInfo*)cutsceneMaps[index].cutscenePointer; 

    cutsceneMaps[index].cutscenePointer += 2;

    if (!(gMapModelContext[ptr->index].flags & (0x8 | 0x10))) {
        
        cutsceneMaps[index].cutscenePointer += 2;

    } else {

        cutsceneMaps[index].cutscenePointer = ptr;
        cutsceneMaps[index].unk_66 = 1;
        
    }

}

INCLUDE_ASM(const s32, "system/cutscene", func_8004BA34);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BB84);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BD00);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BD6C);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004BE14);

void func_8004BE14(u16 index) {

    u16 spriteIndex;
    u16 flags;
    
    CutsceneCharacterSpriteInfo* ptr = (CutsceneCharacterSpriteInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    spriteIndex = ptr->spriteIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    flags = ptr->flags;
    
    cutsceneMaps[index].cutscenePointer += 4;

    characterSprites[spriteIndex].shadowSpriteIndex = flags;

}

INCLUDE_ASM(const s32, "system/cutscene", func_8004BE88);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BFC4);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C048);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C0D0);

void func_8004C0D0(u16 index) {

    u16 structIndex;
    u16 flag;
    
    CutsceneMapStruct7Info* ptr = (CutsceneMapStruct7Info*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    structIndex = ptr->structIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    flag = ptr->flag;
    
    cutsceneMaps[index].cutscenePointer += 4;

    // initialize mapStruct7
    func_80038990(0, structIndex, flag);

}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C148);

void func_8004C148(u16 index) {

    CutsceneUnknownInfo4* ptr= (CutsceneUnknownInfo4*)cutsceneMaps[index].cutscenePointer;

    u16 unk_2;
    u16 unk_4;
    u16 temp;

    cutsceneMaps[index].cutscenePointer += 2;

    unk_2 = ptr->unk_2;

    cutsceneMaps[index].cutscenePointer += 2;

    unk_4 = ptr->unk_4;
    
    cutsceneMaps[index].cutscenePointer += 2;

    temp = getRandomNumberInRange(0, 100);

    if ((temp < unk_2)^ 1 && (unk_4 < temp)^1) {

        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(s16*)cutsceneMaps[index].cutscenePointer;
        
    } else {
        cutsceneMaps[index].cutscenePointer += 2;
    }

}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C258);

void func_8004C258(u16 index) {

    CutsceneUnknownInfo5* ptr = (CutsceneUnknownInfo5*)cutsceneMaps[index].cutscenePointer;
    
    u16* unk_4;
    u16* unk_8;
    u32 unk_C;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_4 = ptr->unk_4;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_8 = ptr->unk_8;

    cutsceneMaps[index].cutscenePointer += 4;

    unk_C = ptr->unk_C;

    cutsceneMaps[index].cutscenePointer += 4;

    if (*unk_4 >= *unk_8 && unk_C >= *unk_4) {

        cutsceneMaps[index].unk_8 = cutsceneMaps[index].cutscenePointer + 4;
        cutsceneMaps[index].cutscenePointer += *(s16*)cutsceneMaps[index].cutscenePointer;
        
    } else {
        cutsceneMaps[index].cutscenePointer += 4;
    }

}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C34C);

void func_8004C34C(u16 index) {

    u16 cutsceneMapIndex;
    
    CutsceneAssetFlagsInfo* ptr = (CutsceneAssetFlagsInfo*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMapIndex = ptr->cutsceneMapIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;

    if (cutsceneMapIndex < MAX_CUTSCENE_ASSETS) {
        if (cutsceneMaps[cutsceneMapIndex].flags & 1) {
            cutsceneMaps[cutsceneMapIndex].flags |= 0x20;
        }
    }
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C3D0);

void func_8004C3D0(u16 index) {

    u16 cutsceneMapIndex;
    
    CutsceneAssetFlagsInfo* ptr = (CutsceneAssetFlagsInfo*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 2;

    cutsceneMapIndex = ptr->cutsceneMapIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;

    if (cutsceneMapIndex < MAX_CUTSCENE_ASSETS) {
        if (cutsceneMaps[cutsceneMapIndex].flags & 1) {
            cutsceneMaps[cutsceneMapIndex].flags &= ~0x20;
        }
    }
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C454);

void func_8004C454(u16 index) {

    u16 i;

    cutsceneMaps[index].cutscenePointer += 4;

    for (i = 0; i < MAX_CUTSCENE_ASSETS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_CUTSCENE_ASSETS) continue;

        if (cutsceneMaps[i].flags & 1) {
            cutsceneMaps[i].flags |= 0x20;
        }

    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C4E0);

void func_8004C4E0(u16 index) {

    u16 i;

    cutsceneMaps[index].cutscenePointer += 4;

    for (i = 0; i < MAX_CUTSCENE_ASSETS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_CUTSCENE_ASSETS) continue;

        if (cutsceneMaps[i].flags & 1) {
            cutsceneMaps[i].flags &= ~0x20;
        }

    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C56C);

void func_8004C56C(u16 index) {

    u16 paletteIndex;
    
    CutscenePaletteInfo* ptr = (CutscenePaletteInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    // globalSprites.paletteIndex
    paletteIndex = ptr->paletteIndex;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    func_8002CB88(cutsceneMaps[index].spriteIndex, paletteIndex);
    
}

INCLUDE_ASM(const s32, "system/cutscene", func_8004C5D8);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C6CC);

void func_8004C6CC(u16 index) {

    u16 songIndex;
    u8 *songStart;
    u8 *songEnd;

    CutsceneSongInfo* ptr = (CutsceneSongInfo*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 2;

    songIndex = ptr->index;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    songStart = ptr->songStart;
    
    cutsceneMaps[index].cutscenePointer += 4;
    
    songEnd = ptr->songEnd;
    
    cutsceneMaps[index].cutscenePointer += 4;    

    setSong(songIndex, songStart, songEnd);
    setSongVolumes(songIndex, 0, 0);
    
}

/*
void func_8004C6CC(u16 index) {
 
    u16 index;
    u8 *songStart;
    u8 *songEnd;

    cutsceneMaps[index].cutscenePointer += 2;

    index = *(short*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    songStart = *(u8**)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 4;
    
    songEnd = *(u8**)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer += 4;    

    setSong(index, songStart, songEnd);
    setSongVolumes(index, 0, 0);
    
}
*/

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C770);

void func_8004C770(u16 index) {

    CutsceneSongSpeedInfo* ptr = (CutsceneSongSpeedInfo*)cutsceneMaps[index].cutscenePointer;
    
    u16 songIndex;
    u32 speed;

    cutsceneMaps[index].cutscenePointer += 2;

    songIndex = ptr->index;
    
    cutsceneMaps[index].cutscenePointer += 2;

    speed = ptr->speed;

    cutsceneMaps[index].cutscenePointer += 4;

    setSongSpeed(songIndex, speed);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C7E4);

void func_8004C7E4(u16 index) {

    CutsceneSongVolumeInfo* ptr = (CutsceneSongVolumeInfo*)cutsceneMaps[index].cutscenePointer;
    
    u16 songIndex;
    s32 maxVolume;
    s16 volume;

    cutsceneMaps[index].cutscenePointer += 2;

    songIndex = ptr->index;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    maxVolume = ptr->maxVolume;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    volume = ptr->volume;
    
    cutsceneMaps[index].cutscenePointer += 2;    

    setSongVolumes(songIndex, maxVolume, volume);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C870);

void func_8004C870(u16 index) {

    CutsceneSfxVolumeInfo* ptr = (CutsceneSfxVolumeInfo*)cutsceneMaps[index].cutscenePointer;
    
    u32 sfxIndex;
    u16 volume;

    cutsceneMaps[index].cutscenePointer += 2;
    
    sfxIndex = ptr->index;

    cutsceneMaps[index].cutscenePointer += 2;

    volume = ptr->volume;

    cutsceneMaps[index].cutscenePointer += 4;

    sfxIndex++;
    
    setSfx(sfxIndex);
    setSfxVolume(sfxIndex, volume);
    
} 

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C904);

void func_8004C904(u16 index) {

    CutsceneSongInfo* ptr = (CutsceneSongInfo*)cutsceneMaps[index].cutscenePointer;

    u16 songIndex;

    cutsceneMaps[index].cutscenePointer += 2;
    
    songIndex = ptr->index;

    if (!gSongs[songIndex].flags) {
        
        cutsceneMaps[index].cutscenePointer += 2;
        
    } else {

        cutsceneMaps[index].cutscenePointer = ptr;
        cutsceneMaps[index].unk_66 = 1;
    
    }

}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004C994);

void func_8004C994(u16 index) {

    u16 dialogueBoxIndex;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    s16 flags;

    CutsceneDialogueBoxRGBAInfo* ptr = (CutsceneDialogueBoxRGBAInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    r = *(u8*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer++;

    g = *(u8*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer++;

    b = *(u8*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer++;

    a = *(u8*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer++;

    flags = *(u16*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 4;
    
    func_8003EA1C(dialogueBoxIndex, r, g, b, a, flags);
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004CA80);

void func_8004CA80(u16 index) {

    u16 dialogueBoxIndex;
    
    CutsceneDialogueBoxInfo* ptr = (CutsceneDialogueBoxInfo*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;
    
    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneMaps[index].cutscenePointer += 2;

    if (!func_8003EFD8(dialogueBoxIndex)) {

        cutsceneMaps[index].unk_66 = 1;
        cutsceneMaps[index].cutscenePointer -= 4;
        
    }
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_8004CB1C);

// function index 0x5E
void func_8004CB1C(u16 index) {

    CutsceneSpriteInfo2* ptr = (CutsceneSpriteInfo2*)cutsceneMaps[index].cutscenePointer;
    u16 flag;

    cutsceneMaps[index].cutscenePointer += 2;

    flag = ptr->flag;

    cutsceneMaps[index].cutscenePointer += 2;

    func_8002CB24(cutsceneMaps[index].spriteIndex, flag);
    
}

INCLUDE_ASM(const s32, "system/cutscene", func_8004CB88);

//INCLUDE_ASM(const s32, "system/cutscene", func_8004CC3C);

void func_8004CC3C(u16 index) {

    u16 value;
    u8 index1;
    u8 index2;

    CutsceneMapStruct6Info* ptr = (CutsceneMapStruct6Info*)cutsceneMaps[index].cutscenePointer;

    cutsceneMaps[index].cutscenePointer += 2;

    value = ptr->value;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    index1 = ptr->index1;

    cutsceneMaps[index].cutscenePointer++;
    
    index2 = *(u8*)cutsceneMaps[index].cutscenePointer;
    
    cutsceneMaps[index].cutscenePointer++;
    
    cutsceneMaps[index].cutscenePointer += 2;
    
    func_80035004(0, value, index1, index2);
    
    func_80036FA0(0);
    
}

INCLUDE_ASM(const s32, "system/cutscene", func_8004CCF0);
