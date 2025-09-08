#include "common.h"

#include "cutscene.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/message.h"
#include "system/globalSprites.h"
#include "system/mapController.h"

#include "mainproc.h"

// forward declarations
void func_800471B0(u16);                               
void func_80047640(u16);                               
void func_80047E34(u16);        
void func_80047E94(u16);
void func_80047F20(u16);
void func_80047F90(u16);
void func_80048124(u16);
void func_80048258(u16);
void func_800482B8(u16);
void func_800482F8(u16);
void func_8004835C(u16);
void func_8004838C(u16);
void func_80048430(u16);
void func_800484D4(u16);
void func_80048578(u16);
void func_800485EC(u16);
void func_80048694(u16);
void func_800486F4(u16);
void func_800488CC(u16);
void func_80048B08(u16);
void func_80048B90(u16);
void func_80048BEC(u16);
void func_80048C48(u16);
void func_80048CA4(u16);
void func_80048DA8(u16);
void func_80048E98(u16);
void func_80048F88(u16);
void func_80048FF4(u16);
void func_80049064(u16);
void func_8004910C(u16);
void func_80049228(u16);
void func_80049350(u16);
void func_800495F0(u16);
void func_8004969C(u16);
void func_80049708(u16);
void func_80049828(u16);
void func_800498B0(u16);
void func_80049950(u16);
void func_80049A28(u16);
void func_80049AC4(u16);
void func_80049D64(u16);
void func_80049DF4(u16);
void func_80049E84(u16);
void func_80049F40(u16);
void func_80049FA0(u16);
void func_8004A000(u16);
void func_8004A0A8(u16);
void func_8004A0F4(u16);
void func_8004A140(u16);
void func_8004A1DC(u16);
void func_8004A234(u16);
void func_8004A320(u16);
void func_8004A38C(u16);
void func_8004A400(u16);
void func_8004A47C(u16);
void func_8004A7A4(u16);
void func_8004AB04(u16);
void func_8004ABA8(u16);
void func_8004AC4C(u16);
void func_8004ACE4(u16);
void func_8004AD44(u16);
void func_8004B050(u16);
void func_8004B09C(u16);
void func_8004B0E8(u16);
void func_8004B2FC(u16);
void func_8004B410(u16);
void func_8004B498(u16);
void func_8004B538(u16);
void func_8004B5F0(u16);
void func_8004B700(u16);
void func_8004B7B8(u16);
void func_8004B920(u16);
void func_8004B9A0(u16);
void func_8004BA34(u16);
void func_8004BB84(u16);
void func_8004BD00(u16);
void func_8004BD6C(u16);
void func_8004BE14(u16);
void func_8004BE88(u16);
void func_8004BFC4(u16);
void func_8004C048(u16);
void func_8004C0D0(u16);
void func_8004C148(u16);
void func_8004C258(u16);
void func_8004C34C(u16);
void func_8004C3D0(u16);
void func_8004C454(u16);
void func_8004C4E0(u16);
void func_8004C56C(u16);
void func_8004C5D8(u16);
void func_8004C6CC(u16);
void func_8004C770(u16);
void func_8004C7E4(u16);
void func_8004C870(u16);
void func_8004C904(u16);
void func_8004C994(u16);
void func_8004CA80(u16);
void func_8004CB1C(u16);
void func_8004CB88(u16);
void func_8004CC3C(u16);
void func_8004CCF0(u16);


// data
// cutscene handler callbacks
void (*cutsceneCommandHandlers[])(u16) = {
    func_80047E94,
    func_80047F20,
    func_80047F90,
    func_80048124,
    func_80048258,
    func_800482B8,
    func_800482F8,
    func_8004835C,
    func_8004838C,
    func_80048430,
    func_800484D4,
    func_80048578,
    func_800485EC,
    func_80048694,
    func_800486F4,
    func_800488CC,
    func_80048B08,
    func_80048B90,
    func_80048BEC,
    func_80048C48,
    func_80048CA4,
    func_80048DA8,
    func_80048E98,
    func_80048F88,
    func_80048FF4,
    func_80049064,
    func_8004910C,
    func_80049228,
    func_80049350,
    func_800495F0,
    func_8004969C,
    func_80049708,
    func_80049828,
    func_800498B0,
    func_80049950,
    func_80049A28,
    func_80049AC4,
    func_80049D64,
    func_80049DF4,
    func_80049E84,
    func_80049F40,
    func_80049FA0,
    func_8004A000,
    func_8004A0A8,
    func_8004A0F4,
    func_8004A140,
    func_8004A1DC,
    func_8004A234,
    func_8004A320,
    func_8004A38C,
    func_8004A400,
    func_8004A47C,
    func_8004A7A4,
    func_8004AB04,
    func_8004ABA8,
    func_8004AC4C,
    func_8004ACE4,
    func_8004AD44,
    func_8004B050,
    func_8004B09C,
    func_8004B0E8,
    func_8004B2FC,
    func_8004B410,
    func_8004B498,
    func_8004B538,
    func_8004B5F0,
    func_8004B700,
    func_8004B7B8,
    func_8004B920,
    func_8004B9A0,
    func_8004BA34,
    func_8004BB84,
    func_8004BD00,
    func_8004BD6C,
    func_8004BE14,
    func_8004BE88,
    func_8004BFC4,
    func_8004C048,
    func_8004C0D0,
    func_8004C148,
    func_8004C258,
    func_8004C34C,
    func_8004C3D0,
    func_8004C454,
    func_8004C4E0,
    func_8004C56C,
    func_8004C5D8,
    func_8004C6CC,
    func_8004C770,
    func_8004C7E4,
    func_8004C870,
    func_8004C904,
    func_8004C994,
    func_8004CA80,
    func_8004CB1C,
    func_8004CB88,
    func_8004CC3C,
    func_8004CCF0
};

// possibly unused
u32 D_80113558[] = { NULL, NULL, 0x802E2CC0, 0x802EE000, 0x802F3200, 0x802B9F40, 0x802C08C0, 0x8030F800, NULL, NULL };

extern CutsceneExecutor cutsceneExecutors[MAX_BYTECODE_EXECUTORS];


//INCLUDE_ASM("asm/nonmatchings/system/cutscene", initializeCutsceneExecutors);

void initializeCutsceneExecutors(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        
        cutsceneExecutors[i].assetIndex = 0;
        cutsceneExecutors[i].waitFrames = 0;
        cutsceneExecutors[i].unk_68 = 0;
        cutsceneExecutors[i].unk_69 = 0;
        cutsceneExecutors[i].unk_6A = 0;

        cutsceneExecutors[i].offsets.x = 0;
        cutsceneExecutors[i].offsets.y = 0;
        cutsceneExecutors[i].offsets.z = 0;

        cutsceneExecutors[i].unk_58.x = 0;
        cutsceneExecutors[i].unk_58.y = 0;
        cutsceneExecutors[i].unk_58.z = 0;

        cutsceneExecutors[i].flags = 0;

        cutsceneExecutors[i].unk_20 = 0;
        cutsceneExecutors[i].unk_22 = 0;
        cutsceneExecutors[i].unk_24 = 0;
        cutsceneExecutors[i].unk_26 = 0;
        cutsceneExecutors[i].unk_28 = 0;
        cutsceneExecutors[i].unk_2A = 0;

        cutsceneExecutors[i].unk_1C = 0xFFFF;

        cutsceneExecutors[i].unk_C.x = 0;
        cutsceneExecutors[i].unk_C.y = 0;
        cutsceneExecutors[i].unk_C.z = 0;

        cutsceneExecutors[i].cameraFlags = 0;
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", spawnCutsceneExecutor);

bool spawnCutsceneExecutor(u16 index, void *bytecodePtr) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS) {

        if (!(cutsceneExecutors[index].flags & ACTIVE)) {
            
            cutsceneExecutors[index].bytecodePtr = bytecodePtr;
            
            cutsceneExecutors[index].waitFrames = 0;
            
            cutsceneExecutors[index].offsets.x = 0;
            cutsceneExecutors[index].offsets.y = 0;
            cutsceneExecutors[index].offsets.z = 0;

            cutsceneExecutors[index].unk_58.x = 0;
            cutsceneExecutors[index].unk_58.y = 0;
            cutsceneExecutors[index].unk_58.z = 0;

            cutsceneExecutors[index].flags = 1;
            
            result = TRUE;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046A58);

bool func_80046A58(u16 index, void* bytecodePtr) {
    
    bool result = FALSE;

    if (index < MAX_BYTECODE_EXECUTORS) {
        
        if (cutsceneExecutors[index].flags & ACTIVE) {
            
            cutsceneExecutors[index].waitFrames = 0;
            cutsceneExecutors[index].bytecodePtr = bytecodePtr;
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046AB0);

// toggle flags at end of cutscene work
bool func_80046AB0(u16 index) {

    bool result = FALSE;

    if (index < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[index].flags & ACTIVE)) {

        // global sprite flags
        if (cutsceneExecutors[index].flags & 2) {
            deactivateSprite(cutsceneExecutors[index].assetIndex);
        }

        // global sprite flags
        if (cutsceneExecutors[index].flags & CHARACTER_SPRITE_ASSET) {
            deactivateEntity(cutsceneExecutors[index].assetIndex);
        }

        // tiles/map
        if (cutsceneExecutors[index].flags & MAP_ASSET) {
            func_8003C504(cutsceneExecutors[index].assetIndex);
        }
        
        result = TRUE;

    }

    cutsceneExecutors[index].unk_1C = 0xFFFF;
    cutsceneExecutors[index].flags = 0;

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046BB8);

void func_80046BB8(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        // update flags on assets
        func_80046AB0(i);
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046BF8);

bool func_80046BF8(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[index].flags & ACTIVE)) {
        cutsceneExecutors[index].flags |= 0x20;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046C48);

bool func_80046C48(u16 index) {

    bool result = FALSE;
    
    if (index < MAX_BYTECODE_EXECUTORS && (cutsceneExecutors[index].flags & ACTIVE)) {
        cutsceneExecutors[index].flags &= ~0x20;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046C98);

void func_80046C98(void) {

    u16 i = 0;
    u16 j;
    
    // ??
    for (; ;) {
        j = i;
        if (j < MAX_BYTECODE_EXECUTORS) {
            if (cutsceneExecutors[j].flags & ACTIVE) {
                cutsceneExecutors[j].flags |= 0x20;
            }
        }
        i++;
        if (i > (MAX_BYTECODE_EXECUTORS - 1)) break;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046CF4);

void func_80046CF4(void) {

    u16 i = 0;
    u16 j;
    
    for (; ;) {
        j = i;
        if (j < MAX_BYTECODE_EXECUTORS) {
            if (cutsceneExecutors[j].flags & ACTIVE) {
                cutsceneExecutors[j].flags &= ~0x20;
            }
        }
        i++;
        if (i > (MAX_BYTECODE_EXECUTORS - 1)) break;
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80046D50);

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
 
//INCLUDE_ASM("asm/nonmatchings/system/cutscene", updateCutsceneExecutors);

void updateCutsceneExecutors(void) {

    u16 i;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {
        
        if (cutsceneExecutors[i].flags & 1 && !(cutsceneExecutors[i].flags & 0x20)) {
            
            if (cutsceneExecutors[i].flags & CHARACTER_SPRITE_ASSET) {
                
                cutsceneExecutors[i].coordinates.x = entities[cutsceneExecutors[i].assetIndex].coordinates.x;
                cutsceneExecutors[i].coordinates.y = entities[cutsceneExecutors[i].assetIndex].coordinates.y;
                cutsceneExecutors[i].coordinates.z = entities[cutsceneExecutors[i].assetIndex].coordinates.z;
                
                if (cutsceneExecutors[i].unk_1C != 0xFFFF && entities[cutsceneExecutors[i].assetIndex].entityCollidedWithIndex == cutsceneExecutors[i].unk_1C && cutsceneExecutors[i].unk_1E == entities[cutsceneExecutors[i].assetIndex].buttonPressed) {
                    cutsceneExecutors[i].unk_1C = 0xFFFF;
                    cutsceneExecutors[i].waitFrames = 0;
                    cutsceneExecutors[i].bytecodePtr = cutsceneExecutors[i].unk_18;
                
                }
            }
            
            if (cutsceneExecutors[i].flags & MAP_ASSET) {
                cutsceneExecutors[i].coordinates.x = mapControllers[cutsceneExecutors[i].assetIndex].viewPosition.x;
                cutsceneExecutors[i].coordinates.y = mapControllers[cutsceneExecutors[i].assetIndex].viewPosition.y;
                cutsceneExecutors[i].coordinates.z = mapControllers[cutsceneExecutors[i].assetIndex].viewPosition.z;
            }

            if (cutsceneExecutors[i].waitFrames) {
                 cutsceneExecutors[i].waitFrames--;
                if (cutsceneExecutors[i].waitFrames) {
                    goto skip_callback;
                }
            }
      
 inner_loop:    
            while (1) {
                
                cutsceneExecutors[i].offsets.x = 0;
                cutsceneExecutors[i].offsets.y = 0;
                cutsceneExecutors[i].offsets.z = 0;

                cutsceneCommandHandlers[*(u16*)(cutsceneExecutors[i].bytecodePtr)](i);
                
skip_callback:
                cutsceneExecutors[i].coordinates.x += cutsceneExecutors[i].offsets.x;
                cutsceneExecutors[i].coordinates.y += cutsceneExecutors[i].offsets.y;
                cutsceneExecutors[i].coordinates.z += cutsceneExecutors[i].offsets.z;

                if (!cutsceneExecutors[i].waitFrames)  {
                    goto inner_loop;
                } else {
                    break;
                }
        
            }
            
            if (cutsceneExecutors[i].flags & MAP_ASSET) {
                func_80047E34(i);
            }
            
            if (cutsceneExecutors[i].flags & 2) {
                func_800471B0(i);
            }

            if (cutsceneExecutors[i].flags & CHARACTER_SPRITE_ASSET) {
                func_80047640(i);
            }
        }
    }

}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800471B0);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800475B4);

void func_800475B4(u16 index) {
    cutsceneExecutors[index].unk_68 = 1;
    cutsceneExecutors[index].flags &= ~2;
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800475F8); 

void func_800475F8(u16 index) {

    cutsceneExecutors[index].unk_4 += 2;
    cutsceneExecutors[index].unk_4 = cutsceneExecutors[index].unk_4 + *(s16*)(cutsceneExecutors[index].unk_4);

}
  
INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047640);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047E34);

// tiles
void func_80047E34(u16 index) {
    func_8003BE0C(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].offsets.x, cutsceneExecutors[index].offsets.y, cutsceneExecutors[index].offsets.z);
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047E94);

// function index 0
void func_80047E94(u16 index) {
    
    short offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    offset = *(short*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].unk_4 = cutsceneExecutors[index].bytecodePtr + offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].unk_68 = 0;
    cutsceneExecutors[index].flags |= 2;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047F20);

// function index 1
void func_80047F20(u16 index) {

    u32 offset;

    CutsceneJumpCmd *ptr = (CutsceneJumpCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = ptr->offset;
    
    cutsceneExecutors[index].unk_4 = cutsceneExecutors[index].bytecodePtr + offset;
    cutsceneExecutors[index].unk_68 = 0;
    
    cutsceneExecutors[index].bytecodePtr += 2;

}

// alternate
/*
void func_80047F20(u16 index) {
    
    u32 offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = ((UnknownCutsceneMapUpdate*)cutsceneExecutors[index].bytecodePtr)->unk_0;
    
    cutsceneExecutors[index].unk_4 = cutsceneExecutors[index].bytecodePtr + offset;
    cutsceneExecutors[index].unk_68 = 0;
    
    cutsceneExecutors[index].bytecodePtr += 2;
}
*/

// alternate
/*
void func_80047F20(u16 index) {
    
    u32 offset;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = *(short*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].unk_4 = cutsceneExecutors[index].bytecodePtr + offset;
    cutsceneExecutors[index].unk_68 = 0;
    
    cutsceneExecutors[index].bytecodePtr += 2;
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047F90);

// function index 2
void func_80047F90(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].coordinates.x = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].coordinates.y = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].coordinates.z = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    // tiles
    if (cutsceneExecutors[index].flags & MAP_ASSET) {
        func_8003BDA4(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].coordinates.x, cutsceneExecutors[index].coordinates.y, cutsceneExecutors[index].coordinates.z);
    }

    // set sprite coordinates
    if (cutsceneExecutors[index].flags & CHARACTER_SPRITE_ASSET) {
        setEntityCoordinates(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].coordinates.x, cutsceneExecutors[index].coordinates.y, cutsceneExecutors[index].coordinates.z);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048124);

void func_80048124(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].offsets.x = (f32)*(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].offsets.y = (f32)*(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].offsets.z = (f32)*(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].waitFrames = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048258);

// idle function
void func_80048258(u16 index) {

    CutsceneWaitFramesCmd* ptr = (CutsceneWaitFramesCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 waitFrames;

    (u16*)cutsceneExecutors[index].bytecodePtr += 1;

    waitFrames = ptr->frames;
    
    cutsceneExecutors[index].waitFrames = waitFrames;
    
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;

}

// alternate
/*
void func_80048258(u16 index) {
    
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;
    cutsceneExecutors[index].waitFrames = *(u16*)cutsceneExecutors[index].bytecodePtr;
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;

}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800482B8);

void func_800482B8(u16 index) {

    cutsceneExecutors[index].waitFrames = 1;
    func_80046AB0(index);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800482F8);

void func_800482F8(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr+2;

    cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004835C);

void func_8004835C(u16 index) {
    cutsceneExecutors[index].bytecodePtr = cutsceneExecutors[index].unk_8;
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004838C);

void func_8004838C(u16 index) {

    CutsceneButtonCmd* ptr = (CutsceneButtonCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].button & ptr->buttonMask) {

        cutsceneExecutors[index].bytecodePtr += 6;
        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(short*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 0xA;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048430);

void func_80048430(u16 index) {

    CutsceneButtonCmd* ptr = cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].buttonPressed & ptr->buttonMask) {
        
        cutsceneExecutors[index].bytecodePtr += 6;
        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 0xA;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800484D4);

void func_800484D4(u16 index) {

    CutsceneButtonCmd* ptr = (CutsceneButtonCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (controllers[ptr->controllerIndex].buttonRepeat & ptr->buttonMask) {

        cutsceneExecutors[index].bytecodePtr += 6;
        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 0xA;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048578);

void func_80048578(u16 index) {

    CutsceneExecutorInitializationCmd* ptr = (CutsceneExecutorInitializationCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 assetIndex;
    int offset;
    u32 ptr2;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    assetIndex = ptr->assetIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    offset = ptr->offset;
    ptr2 = cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    ptr2 += offset;
    
    spawnCutsceneExecutor(assetIndex, ptr2);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800485EC);

void func_800485EC(u16 index) {

    CutsceneExecutorInitializationCmd* ptr = (CutsceneExecutorInitializationCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 assetIndex;
    u32 offset;
    u32 ptr2;

    cutsceneExecutors[index].bytecodePtr += 2;

    assetIndex = ptr->assetIndex;

    cutsceneExecutors[index].bytecodePtr += 2; 

    ptr2 = cutsceneExecutors[index].bytecodePtr;

    offset = ptr->offset;

    cutsceneExecutors[index].bytecodePtr += 4;

    ptr2 += offset;
    
    if (assetIndex < MAX_BYTECODE_EXECUTORS) {
        
        if (cutsceneExecutors[assetIndex].flags & 1) {
            cutsceneExecutors[assetIndex].waitFrames = 0;
            cutsceneExecutors[assetIndex].bytecodePtr = ptr2;
        }

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048694);

void func_80048694(u16 index) {

    u16 CutsceneExecutorIndex;

    CutsceneAssetFlagsCmd* ptr = (CutsceneAssetFlagsCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    CutsceneExecutorIndex = ptr->CutsceneExecutorIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    func_80046AB0(CutsceneExecutorIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800486F4);

// function index 0xE
void func_800486F4(u16 index) {

    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u32 romSpritesheetIndexStart;
    u32 romSpritesheetIndexEnd;
    u8* texture1Vaddr;
    u8* texture2Vaddr;
    u16* paletteVaddr;
    u16* animationVaddr;
    u8* spriteToPaletteVaddr;
    u32* spritesheetIndexVaddr;
    u16 assetType;

    CutsceneSpriteCmd1* ptr;
    CutsceneSpriteDMAManagerCmd* ptr2;

    ptr = (CutsceneSpriteCmd1*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    ptr2 = (CutsceneSpriteDMAManagerCmd*)cutsceneExecutors[index].bytecodePtr;

    assetType = ptr2->assetType;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureStart = ptr2->romTextureStart;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureEnd = ptr2->romTextureEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexStart = ptr2->romAssetsIndexStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexEnd = ptr2->romAssetsIndexEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexStart = ptr2->romSpritesheetIndexStart;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexEnd = ptr2->romSpritesheetIndexEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture1Vaddr = ptr2->texture1Vaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture2Vaddr = ptr2->texture2Vaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    paletteVaddr = ptr2->paletteVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    animationVaddr = ptr2->animationVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spriteToPaletteVaddr = ptr2->spriteToPaletteVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;
 
    spritesheetIndexVaddr = ptr2->spritesheetIndexVaddr;

    cutsceneExecutors[index].bytecodePtr += 4;

    dmaSprite(cutsceneExecutors[index].assetIndex, romTextureStart, romTextureEnd, romAssetsIndexStart, 
        romAssetsIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   
    setBilinearFiltering(cutsceneExecutors[index].assetIndex, TRUE);
    
}

// alternate
/*
void func_800486F4(u16 index) {

    u32 romTextureStart;
    u32 romTextureEnd;
    u32 romAssetsIndexStart;
    u32 romAssetsIndexEnd;
    u32 romSpritesheetIndexStart;
    u32 romSpritesheetIndexEnd;
    u8* texture1Vaddr;
    u8* texture2Vaddr;
    u16* paletteVaddr;
    u16* animationVaddr;
    u8* spriteToPaletteVaddr;
    u32* spritesheetIndexVaddr;
    u16 assetType;

    CutsceneSpriteDMAManagerCmd* ptr = (CutsceneSpriteDMAManagerCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    assetType = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureStart = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexStart = *(u32*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetsIndexEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexStart = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romSpritesheetIndexEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture1Vaddr = *(u8**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    texture2Vaddr = *(u8**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    paletteVaddr = *(u16**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    animationVaddr = *(u16**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spriteToPaletteVaddr = *(u8**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    spritesheetIndexVaddr = *(u32**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    dmaSprite(cutsceneExecutors[index].assetIndex, romTextureStart, romTextureEnd, romAssetsIndexStart, 
        romAssetsIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   

    setBilinearFiltering(cutsceneExecutors[index].assetIndex, TRUE);
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800488CC);

void func_800488CC(u16 index) {

    CutsceneEntityCmd2* ptr = (CutsceneEntityCmd2*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_20 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].unk_22 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_24 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_26 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_28 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_2A = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].cameraFlags & MAP_ASSET) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_26);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_20);
    }

    func_8002FCB4(cutsceneExecutors[index].assetIndex, FALSE);

    cutsceneExecutors[index].flags |= 8;
    
    cutsceneExecutors[index].cameraFlags |= 1;
    cutsceneExecutors[index].cameraFlags &= ~(2 | 4);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048B08);

void func_80048B08(u16 index) {

    CutsceneDMACmd* ptr = (CutsceneDMACmd*)cutsceneExecutors[index].bytecodePtr;
    
    u32 romAddrStart;
    u32 romAddrEnd; 
    void* vaddr;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    romAddrStart = ptr->romAddrStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAddrEnd = ptr->romAddrEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    vaddr = ptr->vaddr;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    nuPiReadRom(romAddrStart, vaddr, romAddrEnd - romAddrStart);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048B90);

void func_80048B90(u16 index) {

    u8 nextFunctionIndex;

    CutsceneUnknownCmd1* ptr2;
    
    CutsceneUnknownCmd1* ptr = (CutsceneUnknownCmd1*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    nextFunctionIndex = ptr->nextFunctionIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    ptr2 = *(CutsceneUnknownCmd1**)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    ptr2->functionIndex = nextFunctionIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048BEC);

void func_80048BEC(u16 index) {

    CutsceneUnknownCmd2* ptr = (CutsceneUnknownCmd2*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;
    u16* unk_4;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    *unk_4 = unk_2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048C48);

void func_80048C48(u16 index) {

    CutsceneUnknownCmd6* ptr = (CutsceneUnknownCmd6*)cutsceneExecutors[index].bytecodePtr;

    u32 unk_4;
    u32* unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_4 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_8 = ptr->unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    *unk_8 = unk_4;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048CA4);

void func_80048CA4(u16 index) {

    u8* cutsceneIndexPtr;
    u16 cutsceneIndex1;
    u16 cutsceneIndex2;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneIndex1 = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneIndex2 = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneIndexPtr = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;    

    if (*cutsceneIndexPtr < cutsceneIndex1) {
        goto func_end;  
    } 
    
    if (cutsceneIndex2 >= *cutsceneIndexPtr) {
        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        return;
    }
    
func_end:
    cutsceneExecutors[index].bytecodePtr += 4;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048DA8);

// checks cutscene index until current cutscene index
void func_80048DA8(u16 index) {
    
    u16* cutsceneIndexPtr;
    u16 cutsceneIndex1;
    u16 cutsceneIndex2;

    CutsceneIndexCheckCmd* ptr = (CutsceneIndexCheckCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneIndex1 = ptr->cutsceneIndex1;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneIndex2 = ptr->cutsceneIndex2;

    cutsceneExecutors[index].bytecodePtr += 4;    

    cutsceneIndexPtr = ptr->cutsceneIndexPtr;

    cutsceneExecutors[index].bytecodePtr += 4;    

    if (*cutsceneIndexPtr < cutsceneIndex1) {
        goto func_end;  
    } 
    
    if (cutsceneIndex2 >= *cutsceneIndexPtr) {
        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        return;
    }
    
func_end:
    cutsceneExecutors[index].bytecodePtr += 4;
        
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048E98);

void func_80048E98(u16 index) {

    CutsceneUnknownCmd3* ptr = (CutsceneUnknownCmd3*)cutsceneExecutors[index].bytecodePtr;
    u32 ptr2;

    u16 unk_2;
    u32 unk_4;
    u32 unk_8;
    u32* unk_C;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_8 = ptr->unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_C = ptr->unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;
 
    if (*unk_C >= unk_4 && unk_8 >= *unk_C) {

        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048F88);

// index 0x17
void func_80048F88(u16 index) {

    CutsceneMapFlagCmd* ptr = (CutsceneMapFlagCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 flag;
    s32* cutsceneFlagPointer;

    cutsceneExecutors[index].bytecodePtr += 2;

    flag = ptr->flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneFlagPointer = ptr->cutsceneFlagPointer;

    cutsceneExecutors[index].bytecodePtr += 4;

    *cutsceneFlagPointer |= 1 << flag;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048FF4);

void func_80048FF4(u16 index) {

    CutsceneSpecialBitToggleCmd* ptr = (CutsceneSpecialBitToggleCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 bit;
    u32* bitfield;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    bit = ptr->bit;

    cutsceneExecutors[index].bytecodePtr += 2;

    bitfield = ptr->bitfield;

    cutsceneExecutors[index].bytecodePtr += 4;

    *bitfield &= ~(1 << bit);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049064);

void func_80049064(u16 index) {

    CutsceneUnknown2Cmd* ptr = (CutsceneUnknown2Cmd*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;
    u32* unk_4;
    s32 unk_8;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    unk_2 = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = *(u32**)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (*unk_4 & (1 << unk_2)) {
        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004910C);

void func_8004910C(u16 index) {

    CutsceneEntityDirectionCmd* ptr = (CutsceneEntityDirectionCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].unk_69 = ptr->direction;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & 0x10) {
        func_8003C084(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_69);
    }

    if (cutsceneExecutors[index].flags & 8) {
        setEntityDirection(cutsceneExecutors[index].assetIndex, convertSpriteToWorldDirection(cutsceneExecutors[index].unk_69, gMainMapIndex));
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049228);

// set up cutscene object as map asset
void func_80049228(u16 index) {

    CutsceneMapControllerCmd2 *ptr = cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].assetIndex = ptr->index;

    cutsceneExecutors[index].bytecodePtr += 2;

    func_8003C1E0(cutsceneExecutors[index].assetIndex, mapControllers[cutsceneExecutors[index].assetIndex].viewPosition.x, mapControllers[cutsceneExecutors[index].assetIndex].viewPosition.y, mapControllers[cutsceneExecutors[index].assetIndex].viewPosition.z, 8, 8);
    func_8003BD60(cutsceneExecutors[index].assetIndex);
    setMainMapIndex(cutsceneExecutors[index].assetIndex);

    cutsceneExecutors[index].flags |= MAP_ASSET;
    
} 

#ifdef PERMUTER
void func_80049350(u16 index) {

    Vec3f vec;
    
    cutsceneExecutors[index].bytecodePtr += 2;    

    cutsceneExecutors[index].waitFrames = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_69 = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].unk_6A = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr += 2;
    
    if (cutsceneExecutors[index].cameraFlags & 0x10) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_28);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_22);
    }

    setEntityDirection(cutsceneExecutors[index].assetIndex, convertSpriteToWorldDirection(cutsceneExecutors[index].unk_69, gMainMapIndex));

    vec = getMovementVectorFromDirection(cutsceneExecutors[index].unk_6A, convertWorldToSpriteDirection(entities[cutsceneExecutors[index].assetIndex].direction, gMainMapIndex), 0.0f);
    
    cutsceneExecutors[index].offsets.x = vec.x;
    cutsceneExecutors[index].offsets.y = vec.y;
    cutsceneExecutors[index].offsets.z = vec.z;
    
    cutsceneExecutors[index].cameraFlags |= 4;
    
}
#else
INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049350);
#endif

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800495F0);

void func_800495F0(u16 index) {

    CutsceneMapControllerSetRotationFlagCmd* ptr = (CutsceneMapControllerSetRotationFlagCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapIndex;
    u8 arg1, rotation;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mapIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    arg1 = ptr->arg1;

    cutsceneExecutors[index].bytecodePtr++;

    rotation = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    func_8003C5C0(mapIndex, arg1, rotation);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004969C);

void func_8004969C(u16 index) {

    CutsceneCameraFlagsCmd* ptr = (CutsceneCameraFlagsCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u8 cameraFlags;

    cutsceneExecutors[index].bytecodePtr += 2;

    cameraFlags = ptr->cameraFlags;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].cameraFlags &= ~1;
    cutsceneExecutors[index].cameraFlags |= cameraFlags;  
    cutsceneExecutors[index].cameraFlags |= 4;
    
}

#ifdef PERMUTER
void func_80049708(u16 index) {

    u16 temp1, temp2;
    u16* ptr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_6A = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr++;

    ptr = cutsceneExecutors[index].bytecodePtr;

    temp1 = ptr[0];
    
    cutsceneExecutors[index].bytecodePtr = *ptr + 2;

    cutsceneExecutors[index].unk_2C = temp1;
    
    cutsceneExecutors[index].unk_2E = ptr[1];
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    cutsceneExecutors[index].unk_C = cutsceneExecutors[index].coordinates;
    
    cutsceneExecutors[index].cameraFlags |= (2 | 4);
    
}
#else
INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049708);
#endif

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049828);

void func_80049828(u16 index) {
    
    CutscenInitializeDialogueBoxCmd* ptr = (CutscenInitializeDialogueBoxCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 dialogueBoxIndex;
    u16 dialogueBytecodeUnk14;
    u16 dialogueStruct1Unk0;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBytecodeUnk14 = ptr->dialogueBytecodeUnk14;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueStruct1Unk0 = ptr->dialogueStruct1Unk0;

    cutsceneExecutors[index].bytecodePtr += 2;

    initializeDialogueBox(dialogueBoxIndex, dialogueBytecodeUnk14, dialogueStruct1Unk0, 0x8000);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800498B0);

void func_800498B0(u16 index) {

    CutsceneDialogueWaitCmd* ptr = (CutsceneDialogueWaitCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(dialogueBoxes[dialogueBoxIndex].flags & 4)) {

        cutsceneExecutors[index].waitFrames = 1;
        cutsceneExecutors[index].bytecodePtr -= 4;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049950);

void func_80049950(u16 index) {

    CutsceneDialogueBoxViewSpaceCmd* ptr = (CutsceneDialogueBoxViewSpaceCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialogueBoxIndex;
    f32 x, y, z;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    x = ptr->x;

    cutsceneExecutors[index].bytecodePtr += 2;

    y = ptr->y;

    cutsceneExecutors[index].bytecodePtr += 2;

    z = ptr->z;

    cutsceneExecutors[index].bytecodePtr += 4;

    func_8003F54C(dialogueBoxIndex, x, y, z);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049A28);


void func_80049A28(u16 index) {

    CutsceneUnknownDialogueBoxCmd* ptr = (CutsceneUnknownDialogueBoxCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialogueBoxIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxes[dialogueBoxIndex].flags &= ~0x8000;

    func_8003F130(dialogueBoxIndex);
    
}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049AC4);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049D64);

void func_80049D64(u16 index) {

    CutsceneEntitySetAnimationCmd* ptr = (CutsceneEntitySetAnimationCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 animation;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    animation = ptr->animation;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityAnimation(cutsceneExecutors[index].assetIndex, animation);
    
    cutsceneExecutors[index].cameraFlags &= ~(4 | 8);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049DF4);

void func_80049DF4(u16 index) {

    CutsceneEntitySetAnimationWithDirectionCmd* ptr = (CutsceneEntitySetAnimationWithDirectionCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 animation;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    animation = ptr->animation;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, animation);
    
    cutsceneExecutors[index].cameraFlags &= ~(4 | 8);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049E84);

void func_80049E84(u16 index) {
        
    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_1C = *(u16*)cutsceneExecutors[index].bytecodePtr; 
        
    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_1E = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].unk_18 = cutsceneExecutors[index].bytecodePtr + *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049F40);

void func_80049F40(u16 index) {

    CutsceneEntityPauseCmd* ptr = (CutsceneEntityPauseCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    pauseEntity(entityIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049FA0);

void func_80049FA0(u16 index) {
    
    CutsceneEntityTogglePauseCmd* ptr = (CutsceneEntityTogglePauseCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    func_8002FC38(entityIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A000);

void func_8004A000(u16 index) {

    u16 entityIndex;
    
    CutsceneEntityCmd1* ptr = (CutsceneEntityCmd1*)cutsceneExecutors[index].bytecodePtr; 

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityDirection(cutsceneExecutors[index].assetIndex, (entities[entityIndex].direction + 4)  % 8);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A0A8);

// set flags on all active sprites, rendered and global
void func_8004A0A8(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 4;
    
    pauseEntities();
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A0F4);

void func_8004A0F4(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 4;

    togglePauseEntities(index);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A140);

void func_8004A140(u16 index) {

    cutsceneExecutors[index].bytecodePtr = cutsceneExecutors[index].bytecodePtr + 4;

    if (cutsceneExecutors[index].cameraFlags & 0x10) {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_26);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].unk_20);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A1DC);

void func_8004A1DC(u16 index) {
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A234);

void func_8004A234(u16 index) {

    CutsceneEntityCmd3* ptr = (CutsceneEntityCmd3*)cutsceneExecutors[index].bytecodePtr;
    
    u16 entityIndex;
    u16 entityAssetIndex;
    u16 flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityAssetIndex = ptr->entityAssetIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    flag = ptr->flag;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    loadEntity(entityIndex, entityAssetIndex, flag);
    setEntityTrackingTarget(entityIndex, 0xFFFF, 0xFF);
    setEntityAttachmentOffset(entityIndex, 0, 0, 0);
    setEntityCollidable(entityIndex, TRUE);
    setEntityYMovement(entityIndex, TRUE);
    setEntityTracksCollisions(entityIndex, TRUE);
    enableEntityMovement(entityIndex, TRUE);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A320);

void func_8004A320(u16 index) {

    CutsceneSetBilinearFilteringCmd* ptr = (CutsceneSetBilinearFilteringCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 flag;

    cutsceneExecutors[index].bytecodePtr +=  2;

    flag = ptr->flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    func_8002FCB4(cutsceneExecutors[index].assetIndex, flag);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A38C);

void func_8004A38C(u16 index) {

    CutsceneMapControllerCmd* ptr = (CutsceneMapControllerCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapModelIndex;
    u16 mapIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapModelIndex = ptr->mapModelIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mainMapIndex;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    loadMap(mapModelIndex, mapIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A400);

void func_8004A400(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 4;

    func_8002FCB4(cutsceneExecutors[index].assetIndex, TRUE);
    
    cutsceneExecutors[index].flags &= ~8;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A47C);

void func_8004A47C(u16 index) {

    u8 r, g, b, a;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = D_802373F8.r;
        g = D_802373F8.g;
        b = D_802373F8.b;
        a = D_802373F8.a;

    }

    if (cutsceneExecutors[index].flags & 2) {
        setSpriteColor(cutsceneExecutors[index].assetIndex, r, g, b, a);
    }
    if (cutsceneExecutors[index].flags & 8) {
        setEntityColor(cutsceneExecutors[index].assetIndex, r, g, b, a);
    }
    if (cutsceneExecutors[index].flags & 0x10) {
        func_8003BE98(cutsceneExecutors[index].assetIndex, r, g, b, a);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A7A4);

void func_8004A7A4(u16 index) {
    
    u8 r, g, b, a;
    s16 rate;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    rate = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = D_8013D248.r;
        g = D_8013D248.g;
        b = D_8013D248.b;
        a = D_8013D248.a;

        rate = D_8017045A;
        
    }

    if (cutsceneExecutors[index].flags & 2) {
        updateSpriteRGBA(cutsceneExecutors[index].assetIndex, r, g, b, a, rate);
    }
    if (cutsceneExecutors[index].flags & 8) {
        updateEntityRGBA(cutsceneExecutors[index].assetIndex, r, g, b, a, rate);
    }
    if (cutsceneExecutors[index].flags & 0x10) {
        func_8003BF7C(cutsceneExecutors[index].assetIndex, r, g, b, a, rate);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004AB04);

void func_8004AB04(u16 index) {

    CutsceneU8UpdateCmd* ptr = (CutsceneU8UpdateCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u8* valuePtr;
    s8 value;
    u8 max;
    u8 initial;

    cutsceneExecutors[index].bytecodePtr += 2;

    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 2;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *valuePtr += func_80046D50(*valuePtr, value, max);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004ABA8);

void func_8004ABA8(u16 index) {

    CutsceneU16UpdateCmd* ptr = (CutsceneU16UpdateCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16* valuePtr;
    s16 value;
    u16 max;

    cutsceneExecutors[index].bytecodePtr += 2;

    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 2;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *valuePtr += func_80046D50(*valuePtr, value, max);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004AC4C);

void func_8004AC4C(u16 index) {

    CutsceneU32UpdateCmd* ptr = (CutsceneU32UpdateCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u32* valuePtr;
    s32 value;
    s32 max;

    cutsceneExecutors[index].bytecodePtr += 4;

    value = ptr->value;

    cutsceneExecutors[index].bytecodePtr += 4;

    max = ptr->max;

    cutsceneExecutors[index].bytecodePtr += 4;

    valuePtr = ptr->valuePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    *valuePtr += func_80046D50(*valuePtr, value, max);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004ACE4);

void func_8004ACE4(u16 index) {

    CutsceneMapControllerCmd3* ptr = (CutsceneMapControllerCmd3*)cutsceneExecutors[index].bytecodePtr;

    u16 mapIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mapIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    func_8003C504(mapIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004AD44);

void func_8004AD44(u16 index) {

    u8 r, g, b, a;
    s16 rate;

    u16 i;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    rate = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (r == 0xFE && g == 0xFE && b == 0xFE && a == 0xFE) {

        r = D_8013D248.r;
        g = D_8013D248.g;
        b = D_8013D248.b;
        a = D_8013D248.a;

        rate = D_8017045A;
        
    }

   for (i = 0; i < MAX_ENTITIES; i++) {
       updateEntityRGBA(i, r, g, b, a, rate);
   }

    for (i = 0; i < MAX_MAPS; i++) {
        func_8003BF7C(i, r, g, b, a, rate);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B050);

void func_8004B050(u16 index) {
    
    cutsceneExecutors[index].bytecodePtr += 4;
    deactivateSprites();

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B09C);

void func_8004B09C(u16 index) {
    
    cutsceneExecutors[index].bytecodePtr += 4;
    // deactivate all map controllers
    func_8003C570();

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B0E8);

// index 0x3C
void func_8004B0E8(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;
    
    if (cutsceneExecutors[index].flags & 2) {

        if (func_8002CBF8(cutsceneExecutors[index].assetIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
        
    }

    if (cutsceneExecutors[index].flags & 8) {
        if (func_8002CBF8(entities[cutsceneExecutors[index].assetIndex].globalSpriteIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
    }

    if (cutsceneExecutors[index].flags & 0x10) {
        if (checkMapRGBADone(mapControllers[cutsceneExecutors[index].assetIndex].mainMapIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B2FC);

void func_8004B2FC(u16 index) {

    u16 entityIndex;
    u16 collisionWidth;
    u16 collisionHeight;

    CutsceneEntityCollisionCheckCmd *ptr = (CutsceneEntityCollisionCheckCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;   

    entityIndex = ptr->entityIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    collisionWidth = ptr->collisionWidth;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    collisionHeight = ptr->collisionHeight;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (detectEntityOverlap(&entities[cutsceneExecutors[index].assetIndex], entityIndex, 0.0f, 0.0f, collisionWidth, collisionHeight)) {
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B410);

void func_8004B410(u16 index) {

    CutsceneDialogueSpritesDMACmd* ptr = (CutsceneDialogueSpritesDMACmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 dialoguesIndex;
    u16 dialogueMapAddressIndex;
    u16 dialogueIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueMapAddressIndex = ptr->dialogueMapAddressIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueIndex = ptr->dialogueIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    func_80043430(dialoguesIndex, dialogueMapAddressIndex, dialogueIndex, 0);
        
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B498);

void func_8004B498(u16 index) {

    CutsceneDialoguesWaitCmd* ptr = (CutsceneDialoguesWaitCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(dialogues[dialoguesIndex].struct5.flags & 4)) {

        cutsceneExecutors[index].waitFrames = 1;
        cutsceneExecutors[index].bytecodePtr -= 4;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B538);

void func_8004B538(u16 index) {

    CutsceneDialoguesUnknownCmd* ptr = (CutsceneDialoguesUnknownCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 dialoguesIndex;
    u16 unk;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialoguesIndex = ptr->dialoguesIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    unk = ptr->unk;

    cutsceneExecutors[index].bytecodePtr += 4;

    
    if (dialogues[dialoguesIndex].struct5.unk_17 == unk) {

        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B5F0);

void func_8004B5F0(u16 index) {

    u16* base = cutsceneExecutors[index].bytecodePtr; 
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & 8) {

        if (checkSpriteAnimationStateChanged(entities[cutsceneExecutors[index].assetIndex].globalSpriteIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2;
        }
                
    } else if (cutsceneExecutors[index].flags & 2) {
    
        cutsceneExecutors[index].bytecodePtr = base;
        cutsceneExecutors[index].waitFrames = 1;

    } else {
    
        cutsceneExecutors[index].bytecodePtr += 2;
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B700);

void func_8004B700(u16 index) {

    CutsceneDialogueBoxSetupCmd* ptr = (CutsceneDialogueBoxSetupCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 spriteIndex;
    u8 dialogueWindowIndex;
    u8 overlayIconIndex;
    u8 characterAvatarIndex;

    cutsceneExecutors[index].bytecodePtr += 2;    

    spriteIndex = ptr->spriteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    dialogueWindowIndex = ptr->dialogueWindowIndex;

    cutsceneExecutors[index].bytecodePtr++;

    overlayIconIndex = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    characterAvatarIndex = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr++;
    
    setDialogueBoxSpriteIndices(spriteIndex, dialogueWindowIndex, overlayIconIndex, characterAvatarIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B7B8);

void func_8004B7B8(u16 index) {

    CutsceneEntityUnknownCmd* ptr = (CutsceneEntityUnknownCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 targetSpriteIndex;
    s16 arg1;
    s16 arg2;
    s16 arg3;
    u16 trackingMode;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    targetSpriteIndex = ptr->targetSpriteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    arg1 = ptr->arg1;

    cutsceneExecutors[index].bytecodePtr += 2;

    arg2 = ptr->arg2;

    cutsceneExecutors[index].bytecodePtr += 2;

    arg3 = ptr->arg3;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    trackingMode = ptr->trackingMode;
    
    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr++;

    setEntityTrackingTarget(cutsceneExecutors[index].assetIndex, targetSpriteIndex, trackingMode);
    setEntityAttachmentOffset(cutsceneExecutors[index].assetIndex, arg1, arg2, arg3);
    setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
    setEntityYMovement(cutsceneExecutors[index].assetIndex, FALSE);
    setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, FALSE);
    enableEntityMovement(cutsceneExecutors[index].assetIndex, FALSE);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B920);

void func_8004B920(u16 index) {

    CutsceneCameraFlagsUpdateCmd* ptr = (CutsceneCameraFlagsUpdateCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (unk_2) {
        cutsceneExecutors[index].cameraFlags |= 0x10;
    } else {
        cutsceneExecutors[index].cameraFlags &= ~0x10;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B9A0);

void func_8004B9A0(u16 index) {

    CutsceneMapControllerCmd2* ptr = (CutsceneMapControllerCmd2*)cutsceneExecutors[index].bytecodePtr; 

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(mapControllers[ptr->index].flags & (0x8 | 0x10))) {
        
        cutsceneExecutors[index].bytecodePtr += 2;

    } else {

        cutsceneExecutors[index].bytecodePtr = ptr;
        cutsceneExecutors[index].waitFrames = 1;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BA34);

void func_8004BA34(u16 index) {

    CutscenEntityCheckDirectionCmd* ptr = (CutscenEntityCheckDirectionCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 entityIndex;
    u8 targetDirecton;

    cutsceneExecutors[index].bytecodePtr += 2;

    entityIndex = ptr->entityIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    targetDirecton = ptr->targetDirecton;
    
    cutsceneExecutors[index].bytecodePtr++;
    cutsceneExecutors[index].bytecodePtr++;
    
    if (convertWorldToSpriteDirection(entities[entityIndex].direction, gMainMapIndex) == targetDirecton) {

        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 2;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        return;
        
    }

    cutsceneExecutors[index].bytecodePtr += 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BB84);

void func_8004BB84(u16 index) {

    CutsceneEntitySetFlagsCmd* ptr = (CutsceneEntitySetFlagsCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 temp;

    cutsceneExecutors[index].bytecodePtr += 2;

    temp = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    switch (temp) {

        case 0:
            setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, FALSE);
            enableEntityMovement(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityYMovement(cutsceneExecutors[index].assetIndex, FALSE);
            break;
        case 1:
            setEntityCollidable(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, TRUE);
            enableEntityMovement(cutsceneExecutors[index].assetIndex, FALSE);
            setEntityYMovement(cutsceneExecutors[index].assetIndex, TRUE);
            break;
        case 2:
            setEntityCollidable(cutsceneExecutors[index].assetIndex, TRUE);
            setEntityTracksCollisions(cutsceneExecutors[index].assetIndex, TRUE);
            enableEntityMovement(cutsceneExecutors[index].assetIndex, TRUE);
            setEntityYMovement(cutsceneExecutors[index].assetIndex, TRUE);
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BD00);

void func_8004BD00(u16 index) {

    CutsceneEntitySetPaletteIndexCmd* ptr = (CutsceneEntitySetPaletteIndexCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 paletteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    paletteIndex = ptr->paletteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityPaletteIndex(cutsceneExecutors[index].assetIndex, paletteIndex);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BD6C);

void func_8004BD6C(u16 index) {

    u8 bufferX, bufferY;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    bufferX = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    bufferY = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    func_8002FFF4(entities[cutsceneExecutors[index].assetIndex].entityAssetIndex, bufferX, bufferY);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BE14);

void func_8004BE14(u16 index) {

    u16 entityAssetIndex;
    u16 flags;
    
    CutsceneEntitySetShadowFlagsCmd* ptr = (CutsceneEntitySetShadowFlagsCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    entityAssetIndex = ptr->entityAssetIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    flags = ptr->flags;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    entityAssetDescriptors[entityAssetIndex].shadowSpriteIndex = flags;

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BE88);

void func_8004BE88(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].scaling.x = *(s16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].scaling.y = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
        
    cutsceneExecutors[index].scaling.z = *(s16*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & 2) {
        setSpriteScale(cutsceneExecutors[index].assetIndex, cutsceneExecutors[index].scaling.x, cutsceneExecutors[index].scaling.y, cutsceneExecutors[index].scaling.z);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BFC4);

void func_8004BFC4(u16 index) {

    CutsceneSetSpriteRenderingLayerCmd* ptr = (CutsceneSetSpriteRenderingLayerCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 renderingLayerFlags;

    cutsceneExecutors[index].bytecodePtr += 2;

    renderingLayerFlags = ptr->renderingLayer;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (cutsceneExecutors[index].flags & 2) {
        setSpriteRenderingLayer(cutsceneExecutors[index].assetIndex, renderingLayerFlags);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C048);

void func_8004C048(u16 index) {

    CutscenInitializeDialogueBoxCmd* ptr = (CutscenInitializeDialogueBoxCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 dialogueBoxIndex;
    u16 dialogueBytecodeUnk14;
    u16 dialogueStruct1Unk0;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBytecodeUnk14 = ptr->dialogueBytecodeUnk14;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueStruct1Unk0 = ptr->dialogueStruct1Unk0;

    cutsceneExecutors[index].bytecodePtr += 2;

    initializeDialogueBox(dialogueBoxIndex, dialogueBytecodeUnk14, dialogueStruct1Unk0, 0x100000);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C0D0);

void func_8004C0D0(u16 index) {

    u16 mapAdditionIndex;
    u16 flag;
    
    CutsceneMapAdditionsCmd* ptr = (CutsceneMapAdditionsCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    mapAdditionIndex = ptr->mapAdditionIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    flag = ptr->flag;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    // initialize mapAdditions struct
    func_80038990(0, mapAdditionIndex, flag);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C148);

void func_8004C148(u16 index) {

    CutsceneUnknownCmd4* ptr= (CutsceneUnknownCmd4*)cutsceneExecutors[index].bytecodePtr;

    u16 unk_2;
    u16 unk_4;
    u16 temp;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_2 = ptr->unk_2;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = ptr->unk_4;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    temp = getRandomNumberInRange(0, 100);

    if ((temp < unk_2) ^ 1 && (unk_4 < temp) ^ 1) {

        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 2;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C258);

void func_8004C258(u16 index) {

    CutsceneUnknownCmd5* ptr = (CutsceneUnknownCmd5*)cutsceneExecutors[index].bytecodePtr;
    
    u16* unk_4;
    u16* unk_8;
    u32 unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_4 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_8 = ptr->unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    unk_C = ptr->unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (*unk_4 >= *unk_8 && unk_C >= *unk_4) {

        cutsceneExecutors[index].unk_8 = cutsceneExecutors[index].bytecodePtr + 4;
        cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 4;
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C34C);

void func_8004C34C(u16 index) {

    u16 CutsceneExecutorIndex;
    
    CutsceneAssetFlagsCmd* ptr = (CutsceneAssetFlagsCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    CutsceneExecutorIndex = ptr->CutsceneExecutorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (CutsceneExecutorIndex < MAX_BYTECODE_EXECUTORS) {
        if (cutsceneExecutors[CutsceneExecutorIndex].flags & 1) {
            cutsceneExecutors[CutsceneExecutorIndex].flags |= 0x20;
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C3D0);

void func_8004C3D0(u16 index) {

    u16 CutsceneExecutorIndex;
    
    CutsceneAssetFlagsCmd* ptr = (CutsceneAssetFlagsCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    CutsceneExecutorIndex = ptr->CutsceneExecutorIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    if (CutsceneExecutorIndex < MAX_BYTECODE_EXECUTORS) {
        if (cutsceneExecutors[CutsceneExecutorIndex].flags & 1) {
            cutsceneExecutors[CutsceneExecutorIndex].flags &= ~0x20;
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C454);

void func_8004C454(u16 index) {

    u16 i;

    cutsceneExecutors[index].bytecodePtr += 4;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_BYTECODE_EXECUTORS) continue;

        if (cutsceneExecutors[i].flags & 1) {
            cutsceneExecutors[i].flags |= 0x20;
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C4E0);

void func_8004C4E0(u16 index) {

    u16 i;

    cutsceneExecutors[index].bytecodePtr += 4;

    for (i = 0; i < MAX_BYTECODE_EXECUTORS; i++) {

        if (i == index) continue;
        // why?
        if (i >= MAX_BYTECODE_EXECUTORS) continue;

        if (cutsceneExecutors[i].flags & 1) {
            cutsceneExecutors[i].flags &= ~0x20;
        }

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C56C);

void func_8004C56C(u16 index) {

    u16 paletteIndex;
    
    CutscenePaletteCmd* ptr = (CutscenePaletteCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    paletteIndex = ptr->paletteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    setSpritePaletteIndex(cutsceneExecutors[index].assetIndex, paletteIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C5D8);

void func_8004C5D8(u16 index) {

    CutsceneUnknownCmd7* ptr = (CutsceneUnknownCmd7*)cutsceneExecutors[index].bytecodePtr;

    u8* temp1;
    u8* temp2;
    u32 temp3;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    temp1 = ptr->unk_4;

    cutsceneExecutors[index].bytecodePtr += 4;

    temp2 = ptr->unk_8;

    cutsceneExecutors[index].bytecodePtr += 4;

    temp3 = ptr->unk_C;

    cutsceneExecutors[index].bytecodePtr += 4;

    if (*temp1 >= *temp2) {

        if (temp3 >= *temp1) {
            cutsceneExecutors[index].unk_8 = (u16*)ptr + 10;
            cutsceneExecutors[index].bytecodePtr += *(s16*)cutsceneExecutors[index].bytecodePtr;
            return;
        }
        
    }

    cutsceneExecutors[index].bytecodePtr += 4;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C6CC);

void func_8004C6CC(u16 index) {

    u16 songIndex;
    u8 *songStart;
    u8 *songEnd;

    CutsceneSongCmd* ptr = (CutsceneSongCmd*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    songIndex = ptr->index;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    songStart = ptr->songStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    songEnd = ptr->songEnd;
    
    cutsceneExecutors[index].bytecodePtr += 4;    

    setSong(songIndex, songStart, songEnd);
    setSongVolumes(songIndex, 0, 0);
    
}

/*
void func_8004C6CC(u16 index) {
 
    u16 index;
    u8 *songStart;
    u8 *songEnd;

    cutsceneExecutors[index].bytecodePtr += 2;

    index = *(short*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    songStart = *(u8**)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;
    
    songEnd = *(u8**)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;    

    setSong(index, songStart, songEnd);
    setSongVolumes(index, 0, 0);
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C770);

void func_8004C770(u16 index) {

    CutsceneSongSpeedCmd* ptr = (CutsceneSongSpeedCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 songIndex;
    u32 speed;

    cutsceneExecutors[index].bytecodePtr += 2;

    songIndex = ptr->index;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    speed = ptr->speed;

    cutsceneExecutors[index].bytecodePtr += 4;

    setSongSpeed(songIndex, speed);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C7E4);

void func_8004C7E4(u16 index) {

    CutsceneSongVolumeCmd* ptr = (CutsceneSongVolumeCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 songIndex;
    s32 maxVolume;
    s16 volume;

    cutsceneExecutors[index].bytecodePtr += 2;

    songIndex = ptr->index;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    maxVolume = ptr->maxVolume;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    volume = ptr->volume;
    
    cutsceneExecutors[index].bytecodePtr += 2;    

    setSongVolumes(songIndex, maxVolume, volume);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C870);

void func_8004C870(u16 index) {

    CutsceneSfxVolumeCmd* ptr = (CutsceneSfxVolumeCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u32 sfxIndex;
    u16 volume;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    sfxIndex = ptr->index;

    cutsceneExecutors[index].bytecodePtr += 2;

    volume = ptr->volume;

    cutsceneExecutors[index].bytecodePtr += 4;

    sfxIndex++;
    
    setSfx(sfxIndex);
    setSfxVolume(sfxIndex, volume);
    
} 

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C904);

void func_8004C904(u16 index) {

    CutsceneSongCmd* ptr = (CutsceneSongCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 songIndex;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    songIndex = ptr->index;

    if (!gSongs[songIndex].flags) {
        
        cutsceneExecutors[index].bytecodePtr += 2;
        
    } else {

        cutsceneExecutors[index].bytecodePtr = ptr;
        cutsceneExecutors[index].waitFrames = 1;
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C994);

void func_8004C994(u16 index) {

    u16 dialogueBoxIndex;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    s16 flags;

    CutsceneDialogueBoxRGBACmd* ptr = (CutsceneDialogueBoxRGBACmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    r = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;

    g = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;

    b = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    a = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    flags = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    func_8003EA1C(dialogueBoxIndex, r, g, b, a, flags);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CA80);

void func_8004CA80(u16 index) {

    u16 dialogueBoxIndex;
    
    CutsceneDialogueBoxIndexCmd* ptr = (CutsceneDialogueBoxIndexCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    dialogueBoxIndex = ptr->dialogueBoxIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!func_8003EFD8(dialogueBoxIndex)) {

        cutsceneExecutors[index].waitFrames = 1;
        cutsceneExecutors[index].bytecodePtr -= 4;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CB1C);

// function index 0x5E
void func_8004CB1C(u16 index) {

    CutsceneSetBilinearFilteringCmd* ptr = (CutsceneSetBilinearFilteringCmd*)cutsceneExecutors[index].bytecodePtr;
    u16 useBilinearFiltering;

    cutsceneExecutors[index].bytecodePtr += 2;

    useBilinearFiltering = ptr->flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    setBilinearFiltering(cutsceneExecutors[index].assetIndex, useBilinearFiltering);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CB88);

void func_8004CB88(u16 index) {

    CutsceneMapSetMapAdditionsCmd* ptr = (CutsceneMapSetMapAdditionsCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapAdditionIndex;
    u8 arg2;
    u8 arg3;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapAdditionIndex = ptr->mapAdditionIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    arg2 = ptr->arg2;

    cutsceneExecutors[index].bytecodePtr++;

    arg3 = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    func_80038B58(MAIN_MAP_INDEX, mapAdditionIndex, arg2, arg3);
    func_80036C08(MAIN_MAP_INDEX);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CC3C);

void func_8004CC3C(u16 index) {

    u16 value;
    u8 index1;
    u8 index2;

    CutsceneMapStruct6Cmd* ptr = (CutsceneMapStruct6Cmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    value = ptr->value;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    index1 = ptr->index1;

    cutsceneExecutors[index].bytecodePtr++;
    
    index2 = *(u8*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr++;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    func_80035004(MAIN_MAP_INDEX, value, index1, index2);
    
    func_80036FA0(MAIN_MAP_INDEX);
    
}

#ifdef PERMUTER
void func_8004CCF0(u16 index) {
    
    u16 messageIndex;
    s8 unk_4;
    u8 unk_8;

    cutsceneExecutors[index].bytecodePtr += 2;

    messageIndex = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    unk_4 = *(s8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    unk_8 = *(u8*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr++;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    func_8003F360(messageIndex, unk_4, unk_8);
    
}

#else
INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CCF0);
#endif