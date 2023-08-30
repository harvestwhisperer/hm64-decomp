#include "common.h"

#include "system/sprite.h"

#include "system/graphic.h"
#include "system/map.h"
#include "system/tiles.h"

#include "mainproc.h"
#include "npc.h"

// bss
extern CharacterSprite npcSpriteInfoArray[MAX_NPC_SPRITES];
extern RenderedSprite renderedSprites[MAX_RENDERED_SPRITES];
extern Shadow D_8016FFF8[3];

//INCLUDE_ASM(const s32, "system/renderedSprites", initializeNpcSpriteStructs);

void initializeNpcSpriteStructs(void) {
    
    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        renderedSprites[i].flags = 0;
        renderedSprites[i].unk_52 = 0;
        renderedSprites[i].anim.animationIndex = 0;
        renderedSprites[i].anim.animationIndexPlusOne = 0xFFFF;
        renderedSprites[i].direction = 0;
        renderedSprites[i].unk_58 = 0xFFFF;
        renderedSprites[i].unk_5A = 0;
        renderedSprites[i].collision = 0xFFFF;
        renderedSprites[i].startingCoordinates.x = 0;
        renderedSprites[i].startingCoordinates.y = 0;
        renderedSprites[i].startingCoordinates.z = 0;
        renderedSprites[i].currentCoordinates.x = 0;
        renderedSprites[i].currentCoordinates.y = 0;
        renderedSprites[i].currentCoordinates.z = 0;
        renderedSprites[i].unk_4C = 0;
        renderedSprites[i].unk_64 = 0;
        renderedSprites[i].unk_62 = 0;

    }
    
    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        npcSpriteInfoArray[i].collisionBufferX = 0;
        npcSpriteInfoArray[i].collisionBufferY = 0;
        npcSpriteInfoArray[i].unk_1C = 0;
        npcSpriteInfoArray[i].unk_1E = 0;
        npcSpriteInfoArray[i].flags = 0; 
    } 
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002DDDC);

u8 func_8002DDDC(u16 npcIndex, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, u8 arg7, u8 arg8, void* arg9) {

    u8 result = 0;

    if (npcIndex < 0x66) {

        if (!(npcSpriteInfoArray[npcIndex].flags & 1)) {
            
            
            npcSpriteInfoArray[npcIndex].romTextureStart = arg1;
            npcSpriteInfoArray[npcIndex].romTextureEnd = arg2;
            npcSpriteInfoArray[npcIndex].romPaletteStart = arg3;
            npcSpriteInfoArray[npcIndex].romPaletteEnd = arg4;
            npcSpriteInfoArray[npcIndex].romIndexStart = arg5;
            npcSpriteInfoArray[npcIndex].romIndexEnd = arg6;
            npcSpriteInfoArray[npcIndex].flag = arg8;
            npcSpriteInfoArray[npcIndex].vaddr = arg9;

            npcSpriteInfoArray[npcIndex].collisionBufferX = 0;
            npcSpriteInfoArray[npcIndex].collisionBufferY = 0;

            npcSpriteInfoArray[npcIndex].unk_1C = 0;
            npcSpriteInfoArray[npcIndex].unk_1E = 0;

            npcSpriteInfoArray[npcIndex].flags = 1;

            if (arg7) {
                npcSpriteInfoArray[npcIndex].flags = 3;
            }
            
            result = 1;
            
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002DEE8);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E024);

u8 func_8002E024(u16 index, void* arg1, void* arg2, void* arg3, void* arg4) {

    u8 result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        
        result = 1;
        
        renderedSprites[index].ptr_14 = arg4;
        
        renderedSprites[index].flags = 1;
        
        renderedSprites[index].ptr_08 = arg1;
        renderedSprites[index].ptr_0C = arg2;
        renderedSprites[index].ptr_10 = arg3;

        renderedSprites[index].startingCoordinates.x = 0;
        renderedSprites[index].startingCoordinates.y = 0;
        renderedSprites[index].startingCoordinates.z = 0;

        renderedSprites[index].currentCoordinates.x = 0;
        renderedSprites[index].currentCoordinates.y = 0;
        renderedSprites[index].currentCoordinates.z = 0;

        renderedSprites[index].unk_4C = 0;
        renderedSprites[index].unk_58 = 0xFFFF;
        renderedSprites[index].unk_64 = 0;
        
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E108);

u8 func_8002E108(u16 index) {

    u8 result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        
        renderedSprites[index].flags = 0;

        func_8002B6B8(renderedSprites[index].unk_52);
        
        if (npcSpriteInfoArray[renderedSprites[index].characterIndex].flag != 0xFF) {
            func_8002B6B8(renderedSprites[index].spriteOffset);
        }
        
        result = 1;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E1B8);
 
void func_8002E1B8(void) {

    u16 i = 0;
    u16 temp;

    while (i < MAX_RENDERED_SPRITES) {

        temp = i;

        if (temp < MAX_RENDERED_SPRITES) {

            renderedSprites[i].flags = 0;
        
            func_8002B6B8(renderedSprites[i].unk_52);
            
            if (npcSpriteInfoArray[renderedSprites[i].characterIndex].flag != 0xFF) {
                func_8002B6B8(renderedSprites[i].spriteOffset);
            } 
        }
        
        i++;
      
    }
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E284);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EC18);

// sprite shadows
u8 func_8002EC18(u16 index, u32 arg1, void *arg2, void *arg3, void *arg4, void *arg5, s32 arg6, s32 arg7, s32 arg8, u16 arg9, u8 argA) {

    u8 result = 0;
    
    if (index < 3) {
        result = 1;
        D_8016FFF8[index].romTextureStart = arg1;
        D_8016FFF8[index].romTextureEnd = arg2;
        D_8016FFF8[index].romPaletteStart = arg3;
        D_8016FFF8[index].romPaletteEnd = arg4;
        D_8016FFF8[index].vaddr1 = arg5;
        D_8016FFF8[index].vaddr2 = arg6;
        D_8016FFF8[index].vaddr3 = arg7;
        D_8016FFF8[index].vaddr4 = arg8;
        D_8016FFF8[index].unk_20 = arg9;
        D_8016FFF8[index].unk_22 = argA;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002ECD4);

// double check
bool func_8002ECD4(u16 index, u16 arg1, u16 arg2) {

    bool result = 0;

    if (index < MAX_RENDERED_SPRITES) {

        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {

            renderedSprites[index].unk_24 = arg1;
            renderedSprites[index].unk_26 = arg2;

            if (arg1 == 0xFFFF) {
                renderedSprites[index].flags &= 0xFDFF;
            } else {
                renderedSprites[index].flags |= 0x200;
                renderedSprites[index].flags &= 0xFFBF;
            }

            result = 1;
        }
    }
     
    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002ED80);

u8 func_8002ED80(u16 index, s16 arg1) {
    
    u8 result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1) {
            result = 1;
            renderedSprites[index].unk_62 = arg1;
            renderedSprites[index].flags |= 0x2000;
        }
    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EDF0);

u8 func_8002EDF0(u16 index, s16 arg1, s16 arg2, s16 arg3) {

    u8 result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4)) {
            result = 1;
            renderedSprites[index].unk_18.x = arg1;
            renderedSprites[index].unk_18.y = arg2;
            renderedSprites[index].unk_18.z = arg3;
        }
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EEA4);

u8 func_8002EEA4(u16 arg0) {

    u8 result = 0;

    if (!arg0 && gTileContextFlags & 1 && gTileContextFlags & 2) {
        D_801FD610 = arg0;
        result = 1;
    }

    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EEE4);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EF7C);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F014);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F114);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F1E0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F2FC);

//INCLUDE_ASM(const s32, "system/renderedSprites", setSpriteAnimation);

u8 setSpriteAnimation(u16 index, u16 arg1) {
    
    u8 result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 4) && !(renderedSprites[index].flags & 0x1000)) {     
                renderedSprites[index].anim.animationIndex = arg1;
                renderedSprites[index].flags |= 0x18;
                globalSprites[renderedSprites[index].unk_52].flags2 &= 0xFFBF;
                result = 1;
                globalSprites[renderedSprites[index].unk_52].unk_92 = 0;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F4E0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F594);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F684);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F6F0);

void func_8002F6F0(void) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002FBBC(i);
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F730);

void func_8002F730(void) {
    u16 i;
    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        func_8002FC38(i);
    }
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F770);

void func_8002F770(u16 arg0) {

    u16 i;

    for (i = 0; i < MAX_RENDERED_SPRITES; i++) {
        renderedSprites[i].direction += arg0;
        renderedSprites[i].direction &= 7;
    }
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F7C8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002F8F0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FA2C);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FAFC);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FB3C);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FB7C);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FBBC);

u8 func_8002FBBC(u16 index) {

    u8 result = 0;
    
    if (index < MAX_RENDERED_SPRITES) {
        if (renderedSprites[index].flags & 1) {
            // bit 7
            renderedSprites[index].flags |= 0x40;
            func_8002BB88(renderedSprites[index].unk_52);
            result = 1;
        }
    }

    return result;
    
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FC38);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FCB4);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FD24);

u8 func_8002FD24(u16 index) {
    
    u8 result = 0;
    u16 temp;

    if (index < MAX_RENDERED_SPRITES) {
        // bits 1 and 3
        if ((renderedSprites[index].flags & 1) && (renderedSprites[index].flags & 8)) {
            // check bit 9
            temp = (renderedSprites[index].flags & 0x100);
            result = !temp;
        }
    }
    return result;
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FD80);

bool func_8002FD80(u16 spriteIndex, f32 x, f32 y, f32 z) {

    bool result = 0;
    
    if (spriteIndex < MAX_RENDERED_SPRITES) {
        // bits 1 and 13
        if ((renderedSprites[spriteIndex].flags & 1) && !(renderedSprites[spriteIndex].flags & 0x1000)) {
            result = 1;
            renderedSprites[spriteIndex].startingCoordinates.x = x;
            renderedSprites[spriteIndex].startingCoordinates.y = y;
            renderedSprites[spriteIndex].startingCoordinates.z = z;
        }
    }

    return result;
    
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FE10);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FECC);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FF38);

//INCLUDE_ASM(const s32, "system/renderedSprites", setSpriteCollisionBuffers);

u16 setSpriteCollisionBuffers(u16 spriteIndex, u8 xValue, u8 yValue) {
    
    u16 result = 0;
    
    if (spriteIndex < MAX_NPC_SPRITES) {
        if (npcSpriteInfoArray[spriteIndex].flags & 1) {
            npcSpriteInfoArray[spriteIndex].collisionBufferX = xValue;
            npcSpriteInfoArray[spriteIndex].collisionBufferY = yValue;
            result = 1;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002FFF4);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030054);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800300F8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8003019C);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030240);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800302E4);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80030388);

u8 func_80030388(u16 index) {

    u8 result = 0;

    if (index < MAX_RENDERED_SPRITES) {
        
        if (renderedSprites[index].flags & 1) {
            // bit 7
            result = (renderedSprites[index].flags >> 6) & 1;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_800303D4);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800305CC);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800308E0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800309B4);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030B24);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80030BA0);

u16 func_80030BA0(u16* ptr, u16 offset) {
    return *(ptr + offset) & 0x1FFF;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030BB8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030C34);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030CB0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030DB0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80030EAC);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031050);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800311E0);

// get audio related flag from sprite
INCLUDE_ASM(const s32, "system/renderedSprites", func_80031380);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800313FC);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800315A0);

Vec3f* func_800315A0(Vec3f* arg0, u16 index) {

    // maybe a struct?
    int padding[11];
    
    Vec3f vec;

    vec.x = 0;
    vec.z = 0;
    vec.y = MAX_UNSIGNED_SHORT;
    
    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && !(renderedSprites[index].flags & 0x40) && !(renderedSprites[index].flags & 0x100)) {
            func_80036610(&vec, gTileContext[D_801FD610].unk_40, renderedSprites[index].startingCoordinates.x, renderedSprites[index].startingCoordinates.z);
        } 
    }

    *arg0 = vec;
 
    return arg0;
}

// alternate
// Vec3f func_800315A0(u16 index) {
    
//     int padding[11];

//     Vec3f vec;

//     vec.x = 0;
//     vec.z = 0;
//     vec.y = 65535.0f;
    
//     if (index < MAX_RENDERED_SPRITES) {
//         if ((renderedSprites[index].flags & 1) && !(renderedSprites[index].flags & 0x40) && !(renderedSprites[index].flags & 0x100)) {
//             func_80036610(&vec, gTileContext[D_801FD610].unk_40, renderedSprites[index].startingCoordinates.x, renderedSprites[index].startingCoordinates.z);
//         } 
//     }
 
//     return vec;
// }

INCLUDE_ASM(const s32, "system/renderedSprites", func_8003168C);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80031830);

u8 func_80031830(u16 index, u32 arg1, u8 arg2) {

    Vec3f vec;
    
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    
    u8 result = 0;
    u8 check1;
    u8 check2;
    
    temp3 = renderedSprites[index].startingCoordinates.y + 24.0f;
    temp4 = renderedSprites[index].startingCoordinates.y - 24.0f;
    
    func_80031904(&vec, index, arg1, arg2);
    temp2 = func_80035150(0, vec.x, vec.z);

    check2 = 1;
    
    if (!(temp3 <= temp2)) {
        check2 = 0;
    }

    check1 = 1;
    
    if (!(temp2 <= temp4)) {
        check1 = 0;
    }

    if (check1 || check2) {
        result = 1;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/renderedSprites", func_80031904);

Vec3f* func_80031904(Vec3f* vec, u16 index, s16 arg2, u8 arg3) {

    Vec3f vec1;
    Vec3f vec2;
    Vec3f vec3;

    vec1.x = 0.0f;
    vec1.y = 0.0f;
    vec1.z = arg2;

    vec3.x = 0;
    vec3.y = func_80028820(arg3 & 7);
    vec3.z = 0;

    func_80027950(vec1, &vec2, vec3);

    vec1.x = renderedSprites[index].startingCoordinates.x + vec2.x;
    vec1.y = renderedSprites[index].startingCoordinates.y + vec2.y;
    vec1.z = renderedSprites[index].startingCoordinates.z + vec2.z;

    *vec = vec1;
    
    return vec;
    
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031A10);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031ED0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800321A8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80032330);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800326C0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80033058);
