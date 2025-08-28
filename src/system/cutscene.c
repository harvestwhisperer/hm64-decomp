#include "common.h"

#include "cutscene.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/entity.h"
#include "system/map.h"
#include "system/message.h"
#include "system/globalSprites.h"
#include "system/mapContext.h"

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
        
        cutsceneExecutors[i].spriteIndex = 0;
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
            deactivateSprite(cutsceneExecutors[index].spriteIndex);
        }

        // global sprite flags
        if (cutsceneExecutors[index].flags & CHARACTER_SPRITE_ASSET) {
            deactivateEntity(cutsceneExecutors[index].spriteIndex);
        }

        // tiles/map
        if (cutsceneExecutors[index].flags & MAP_ASSET) {
            func_8003C504(cutsceneExecutors[index].spriteIndex);
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
                
                cutsceneExecutors[i].coordinates.x = entities[cutsceneExecutors[i].spriteIndex].coordinates.x;
                cutsceneExecutors[i].coordinates.y = entities[cutsceneExecutors[i].spriteIndex].coordinates.y;
                cutsceneExecutors[i].coordinates.z = entities[cutsceneExecutors[i].spriteIndex].coordinates.z;
                
                if (cutsceneExecutors[i].unk_1C != 0xFFFF && entities[cutsceneExecutors[i].spriteIndex].unk_58 == cutsceneExecutors[i].unk_1C && cutsceneExecutors[i].unk_1E == entities[cutsceneExecutors[i].spriteIndex].unk_5A) {
                    cutsceneExecutors[i].unk_1C = 0xFFFF;
                    cutsceneExecutors[i].waitFrames = 0;
                    cutsceneExecutors[i].bytecodePtr = cutsceneExecutors[i].unk_18;
                
                }
            }
            
            if (cutsceneExecutors[i].flags & MAP_ASSET) {
                cutsceneExecutors[i].coordinates.x = gMapModelContext[cutsceneExecutors[i].spriteIndex].unk_4.x;
                cutsceneExecutors[i].coordinates.y = gMapModelContext[cutsceneExecutors[i].spriteIndex].unk_4.y;
                cutsceneExecutors[i].coordinates.z = gMapModelContext[cutsceneExecutors[i].spriteIndex].unk_4.z;
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

/*
void func_800475F8(u16 index) {
    int temp;
    cutsceneExecutors[index].unk_4 += 2;
    temp = (s16*)cutsceneExecutors[index].unk_4;
    cutsceneExecutors[index].unk_4 = temp + *(s16*)(temp);
}
*/

/*
void func_800475F8(u16 index) {

    int temp;
    cutsceneExecutors[index].unk_4 += 2;
    temp = cutsceneExecutors[index].unk_4;
    cutsceneExecutors[index].unk_4 = temp + *(s16*)cutsceneExecutors[index].unk_4;
    
}

*/
  
INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047640);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80047E34);

// tiles
void func_80047E34(u16 index) {
    func_8003BE0C(cutsceneExecutors[index].spriteIndex, cutsceneExecutors[index].offsets.x, cutsceneExecutors[index].offsets.y, cutsceneExecutors[index].offsets.z);
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
        func_8003BDA4(cutsceneExecutors[index].spriteIndex, cutsceneExecutors[index].coordinates.x, cutsceneExecutors[index].coordinates.y, cutsceneExecutors[index].coordinates.z);
    }

    // set sprite coordinates
    if (cutsceneExecutors[index].flags & CHARACTER_SPRITE_ASSET) {
        setEntityCoordinates(cutsceneExecutors[index].spriteIndex, cutsceneExecutors[index].coordinates.x, cutsceneExecutors[index].coordinates.y, cutsceneExecutors[index].coordinates.z);
    }
    
}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048124);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048258);

// idle function
void func_80048258(u16 index) {

    (u16*)cutsceneExecutors[index].bytecodePtr += 1;
    cutsceneExecutors[index].waitFrames = *(u16*)cutsceneExecutors[index].bytecodePtr;
    (u16*)cutsceneExecutors[index].bytecodePtr += 1;
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800482B8);

// function index 5
// cutscene object finish
void func_800482B8(u16 index) {

    // set done
    cutsceneExecutors[index].waitFrames = 1;
    // update flags on asset
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
        cutsceneExecutors[index].bytecodePtr += *(short*)cutsceneExecutors[index].bytecodePtr; 
        
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
        cutsceneExecutors[index].bytecodePtr += *(short*)cutsceneExecutors[index].bytecodePtr; 
        
    } else {
        cutsceneExecutors[index].bytecodePtr += 0xA;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048578);

// function index 0xB
// sets child cutscene executor objects
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

    CutsceneSpriteCmd1* ptr;
    CutsceneSpriteDMAManagerCmd* ptr2;

    ptr = (CutsceneSpriteCmd1*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].spriteIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    ptr2 = (CutsceneSpriteDMAManagerCmd*)cutsceneExecutors[index].bytecodePtr;

    assetType = ptr2->assetType;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureStart = ptr2->romTextureStart;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureEnd = ptr2->romTextureEnd;

    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetIndexStart = ptr2->romAssetIndexStart;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetIndexEnd = ptr2->romAssetIndexEnd;

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

    dmaSprite(cutsceneExecutors[index].spriteIndex, romTextureStart, romTextureEnd, romAssetIndexStart, 
        romAssetIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   

    setBilinearFiltering(cutsceneExecutors[index].spriteIndex, TRUE);
    
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

    CutsceneSpriteDMAManagerCmd* ptr = (CutsceneSpriteDMAManagerCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].spriteIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    assetType = *(u16*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureStart = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romTextureEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetIndexStart = *(u32*)cutsceneExecutors[index].bytecodePtr;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    romAssetIndexEnd = *(u32*)cutsceneExecutors[index].bytecodePtr;

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
    
    dmaSprite(cutsceneExecutors[index].spriteIndex, romTextureStart, romTextureEnd, romAssetIndexStart, 
        romAssetIndexEnd, romSpritesheetIndexStart, romSpritesheetIndexEnd, texture1Vaddr, texture2Vaddr, 
        paletteVaddr, animationVaddr, spriteToPaletteVaddr, spritesheetIndexVaddr, assetType, 0);
   

    setBilinearFiltering(cutsceneExecutors[index].spriteIndex, TRUE);
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800488CC);

void func_800488CC(u16 index) {

    CutsceneSpriteCmd3* ptr = (CutsceneSpriteCmd3*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].spriteIndex = ptr->spriteIndex;

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
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].spriteIndex, cutsceneExecutors[index].unk_26);
    } else {
        setEntityAnimationWithDirectionChange(cutsceneExecutors[index].spriteIndex, cutsceneExecutors[index].unk_20);
    }

    func_8002FCB4(cutsceneExecutors[index].spriteIndex, 0);

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

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048CA4);


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

// toggle bit
INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80048FF4);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049064);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004910C);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049228);

// function index 0x1D
// set up cutscene object as map asset
void func_80049228(u16 index) {

    CutsceneMapModelContextCmd2 *ptr = cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;

    cutsceneExecutors[index].spriteIndex = ptr->index;

    cutsceneExecutors[index].bytecodePtr += 2;

    func_8003C1E0(cutsceneExecutors[index].spriteIndex, gMapModelContext[cutsceneExecutors[index].spriteIndex].unk_4.x, gMapModelContext[cutsceneExecutors[index].spriteIndex].unk_4.y, gMapModelContext[cutsceneExecutors[index].spriteIndex].unk_4.z, 8, 8);
    func_8003BD60(cutsceneExecutors[index].spriteIndex);
    setMainMapIndex(cutsceneExecutors[index].spriteIndex);

    cutsceneExecutors[index].flags |= MAP_ASSET;
    
} 

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049350);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800495F0);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004969C);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049708);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049828);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_800498B0);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049950);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049A28);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049AC4);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049D64);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049DF4);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049E84);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049F40);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_80049FA0);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A000);

void func_8004A000(u16 index) {

    u16 spriteIndex;
    
    CutsceneSpriteCmd1* ptr = (CutsceneSpriteCmd1*)cutsceneExecutors[index].bytecodePtr; 

    cutsceneExecutors[index].bytecodePtr += 2;

    spriteIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    setEntityDirection(cutsceneExecutors[index].spriteIndex, (entities[spriteIndex].direction + 4)  % 8);
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

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A140);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A1DC);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A234);

void func_8004A234(u16 index) {

    CutsceneSpriteCmd4* ptr = (CutsceneSpriteCmd4*)cutsceneExecutors[index].bytecodePtr;
    
    u16 spriteIndex;
    u16 characterIndex;
    u16 flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    spriteIndex = ptr->spriteIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    characterIndex = ptr->characterIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    flag = ptr->flag;
    
    cutsceneExecutors[index].bytecodePtr += 2;

    loadEntity(spriteIndex, characterIndex, flag);
    setEntityTrackingTarget(spriteIndex, 0xFFFF, 0xFF);
    func_8002EDF0(spriteIndex, 0, 0, 0);
    func_8003019C(spriteIndex, 1);
    func_80030054(spriteIndex, 1);
    func_80030240(spriteIndex, 1);
    func_800302E4(spriteIndex, 1);
    
}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A320);

void func_8004A320(u16 index) {

    CutsceneSetBilinearFilteringCmd* ptr = (CutsceneSetBilinearFilteringCmd*)cutsceneExecutors[index].bytecodePtr;

    u16 flag;

    cutsceneExecutors[index].bytecodePtr +=  2;

    flag = ptr->flag;

    cutsceneExecutors[index].bytecodePtr += 2;

    func_8002FCB4(cutsceneExecutors[index].spriteIndex, flag);

}

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A38C);

void func_8004A38C(u16 index) {

    CutsceneMapModelContextCmd* ptr = (CutsceneMapModelContextCmd*)cutsceneExecutors[index].bytecodePtr;
    
    u16 mapModelContextIndex;
    u16 mapIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapModelContextIndex = ptr->mapModelContextIndex;

    cutsceneExecutors[index].bytecodePtr += 2;

    mapIndex = ptr->mainMapIndex;

    cutsceneExecutors[index].bytecodePtr += 4;
    
    loadMap(mapModelContextIndex, mapIndex);

}


INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A400);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A47C);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004A7A4);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004AB04);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004ABA8);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004AC4C);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004ACE4);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004AD44);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B050);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B09C);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B0E8);

// index 0x3C
void func_8004B0E8(u16 index) {

    cutsceneExecutors[index].bytecodePtr += 2;
    
    if (cutsceneExecutors[index].flags & 2) {

        if (func_8002CBF8(cutsceneExecutors[index].spriteIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
        
    }

    if (cutsceneExecutors[index].flags & 8) {
        if (func_8002CBF8(entities[cutsceneExecutors[index].spriteIndex].globalSpriteIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
    }

    if (cutsceneExecutors[index].flags & 0x10) {
        if (checkMapRGBADone(gMapModelContext[cutsceneExecutors[index].spriteIndex].mainMapIndex)) {
            cutsceneExecutors[index].bytecodePtr += 2;
        } else {
            cutsceneExecutors[index].waitFrames = 1;
            cutsceneExecutors[index].bytecodePtr -= 2; 
        }
    }
    
}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B2FC);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B410);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B498);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B538);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B5F0);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B700);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B7B8);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B920);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004B9A0);

void func_8004B9A0(u16 index) {

    CutsceneMapModelContextCmd2* ptr = (CutsceneMapModelContextCmd2*)cutsceneExecutors[index].bytecodePtr; 

    cutsceneExecutors[index].bytecodePtr += 2;

    if (!(gMapModelContext[ptr->index].flags & (0x8 | 0x10))) {
        
        cutsceneExecutors[index].bytecodePtr += 2;

    } else {

        cutsceneExecutors[index].bytecodePtr = ptr;
        cutsceneExecutors[index].waitFrames = 1;
        
    }

}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BA34);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BB84);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BD00);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BD6C);

//INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BE14);

void func_8004BE14(u16 index) {

    u16 spriteIndex;
    u16 flags;
    
    CutsceneCharacterSpriteCmd* ptr = (CutsceneCharacterSpriteCmd*)cutsceneExecutors[index].bytecodePtr;

    cutsceneExecutors[index].bytecodePtr += 2;
    
    spriteIndex = ptr->spriteIndex;
    
    cutsceneExecutors[index].bytecodePtr += 2;
    
    flags = ptr->flags;
    
    cutsceneExecutors[index].bytecodePtr += 4;

    characterSprites[spriteIndex].shadowSpriteIndex = flags;

}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BE88);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004BFC4);

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C048);

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
    
    setSpritePaletteIndex(cutsceneExecutors[index].spriteIndex, paletteIndex);
    
}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004C5D8);

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

    setBilinearFiltering(cutsceneExecutors[index].spriteIndex, useBilinearFiltering);
    
}

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CB88);

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

INCLUDE_ASM("asm/nonmatchings/system/cutscene", func_8004CCF0);
