#include "common.h"

#include "npc.h"
#include "sprite.h"

extern npcSpriteInfo npcSpriteInfoArray[MAX_NPC_SPRITES];
extern RenderedSprite renderedSprites[MAX_RENDERED_SPRITES];
extern Sprite globalSprites[MAX_RENDERED_SPRITES];
extern MapTileContext D_802055D8[];

extern u16 D_801FD610;

void func_8002BB88(u16);
u8 func_8002FBBC(u16); 
void func_8002FC38(u16); 
Vec3f *func_80036610(Vec3f*, u16, f32, f32);               


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

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002DDDC);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002DEE8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E024);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E108);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E1B8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002E284);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EC18);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002ECD4);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002ED80);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EDF0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_8002EEA4);

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

u8 func_8002FD80(u16 spriteIndex, f32 x, f32 y, f32 z) {

    u8 result = 0;
    
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

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031380);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800313FC);

//INCLUDE_ASM(const s32, "system/renderedSprites", func_800315A0);

Vec3f func_800315A0(u16 index) {
    
    int padding[11];

    Vec3f vec;

    vec.x = 0;
    vec.z = 0;
    vec.y = 65535.0f;
    
    if (index < MAX_RENDERED_SPRITES) {
        if ((renderedSprites[index].flags & 1) && !(renderedSprites[index].flags & 0x40) && !(renderedSprites[index].flags & 0x100)) {
            func_80036610(&vec, D_802055D8[D_801FD610].unk_40, renderedSprites[index].startingCoordinates.x, renderedSprites[index].startingCoordinates.z);
        } 
    }
 
    return vec;
}

INCLUDE_ASM(const s32, "system/renderedSprites", func_8003168C);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031830);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031904);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031A10);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80031ED0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800321A8);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80032330);

INCLUDE_ASM(const s32, "system/renderedSprites", func_800326C0);

INCLUDE_ASM(const s32, "system/renderedSprites", func_80033058);
