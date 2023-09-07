#include "common.h"

#include "system/tiles.h"

#include "system/graphic.h"
#include "system/map.h"

// bss
extern Vec4f D_8013D248;
extern Vec4f D_802373F8;

// flags
extern u16 D_8017045A;

extern MapTileContext gTileContext[1];
extern TileInfo gTiles[0x60];

// forward declarations
void func_8003C8D4(u8*); 

static const f32 D_8011EDE0[];
                    
//INCLUDE_ASM(const s32, "system/tiles", intializeTileContext);

void intializeTileContext(void) {
    
    u16 i;
    
    D_802373F8.r = 0;
    D_802373F8.g = 0;
    D_802373F8.b = 0;
    D_802373F8.a = 0;
    
    D_8013D248.r = 0;
    D_8013D248.g = 0;
    D_8013D248.b = 0;
    D_8013D248.a = 0;
    
    // flags
    D_8017045A = 0;
    
    for (i = 0; i < 1; i++) {
        
        gTileContext[i].flags = 0;
        
        gTileContext[i].mapIndex = 0;
        
        gTileContext[i].unk_4.x = 0;
        gTileContext[i].unk_4.y = 0;
        gTileContext[i].unk_4.z = 0;

        gTileContext[i].rotation = 0;
        
        gTileContext[i].unk_44 = 0;
        gTileContext[i].unk_45 = 0;
        
        gTileContext[i].unk_10.x = 0;
        gTileContext[i].unk_10.y = 0;
        gTileContext[i].unk_10.z = 0;

        gTileContext[i].unk_1C.x = 0;
        gTileContext[i].unk_1C.y = 0;
        gTileContext[i].unk_1C.z = 0;

        gTileContext[i].unk_28.x = 0;
        gTileContext[i].unk_28.y = 0;
        gTileContext[i].unk_28.z = 0;

        gTileContext[i].unk_34.x = 0;
        gTileContext[i].unk_34.y = 0;
        gTileContext[i].unk_34.z = 0;
    
    }
}

//INCLUDE_ASM(const s32, "system/tiles", setTileInfo);

bool setTileInfo(u16 index, void *start, void *end) {
    
    bool result = 0;
    
    if (index < MAX_TILES) {
        if (!(gTiles[index].flags & 1)) {
            result = 1;
            gTiles[index].romStart = start;
            gTiles[index].romEnd = end;
            gTiles[index].flags = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/tiles", func_8003BA44);

bool func_8003BA44(u16 index, s16 flag, void *tileBank) {
    
    bool result;

    result = 0;
    
    if (index == 0 && !(gTileContext[index].flags & 1)) {
        
        result = 1;
        
        gTileContext[index].mapIndex = flag;
        
        gTileContext[index].tileBank = tileBank;
        
        gTileContext[index].flags = 1;

        gTileContext[index].unk_4.x = 0;
        gTileContext[index].unk_4.y = 0;
        gTileContext[index].unk_4.z = 0;

        gTileContext[index].unk_44 = 0;
        gTileContext[index].unk_45 = 0;

        gTileContext[index].unk_10.x = 0;
        gTileContext[index].unk_10.y = 0;
        gTileContext[index].unk_10.z = 0;

        gTileContext[index].unk_1C.x = 0;
        gTileContext[index].unk_1C.y = 0;
        gTileContext[index].unk_1C.z = 0;

        gTileContext[index].unk_28.x = 0;
        gTileContext[index].unk_28.y = 0;
        gTileContext[index].unk_28.z = 0;

        gTileContext[index].unk_34.x = 0;
        gTileContext[index].unk_34.y = 0;
        gTileContext[index].unk_34.z = 0;

    }
    
    return result;
}

// map loading function
bool func_8003BB14(u16 index, u16 mapIndex) {

    bool result = 0;
    
    if (index == 0 && (gTileContext[index].flags & 1)) {
        
        func_8003BC50(0, mapIndex);
        func_8003423C(gTileContext[index].mapIndex, 0.0f, 0.0f, 0.0f);
        func_80034298(gTileContext[index].mapIndex, 1.0f, 1.0f, 1.0f);
        func_800342F4(gTileContext[index].mapIndex, 45.0f, 0.0f, 0.0f);
        
        gTileContext[index].rotation = 0;
        
        func_8003BDA4(0, 0.0f, 0.0f, 0.0f);
        func_8003C084(0, 0);
        func_8003C1E0(0, 0.0f, 0.0f, 0.0f, 0, 0);
        func_80034350(gTileContext[index].mapIndex, 0, 0, 0, 0);
        
        result = 1; 

    }

    return result;
}

INCLUDE_ASM(const s32, "system/tiles", func_8003BC50);

INCLUDE_ASM(const s32, "system/tiles", func_8003BD60);

INCLUDE_ASM(const s32, "system/tiles", func_8003BDA4);

//INCLUDE_ASM(const s32, "system/tiles", func_8003BE0C);

bool func_8003BE0C(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index == 0 &&  (gTileContext[index].flags & 1) && (gTileContext[index].flags & 2)) {
        result = 1;
        gTileContext[index].unk_4.x += arg1;
        gTileContext[index].unk_4.y += arg2;
        gTileContext[index].unk_4.z += arg3;
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/tiles", func_8003BE98);

INCLUDE_ASM(const s32, "system/tiles", func_8003BF7C);

//INCLUDE_ASM(const s32, "system/tiles", func_8003C084);

bool func_8003C084(u16 index, u8 arg1) {

    bool result;

    Angles angles;
    f32 *ptr;
    f32 tempf;
    
    angles = *(Angles*)D_8011EDE0;
    ptr = (f32*)&angles;

    result = 0;

    if (index == 0 && gTileContext[index].flags & 1 && gTileContext[index].flags & 2) {

        if (!(gTileContext[index].flags & 0x18)) {
        
            gTileContext[index].rotation = arg1;
            tempf = ptr[arg1];
        
            func_800342F4(gTileContext[index].mapIndex, 45.0f, tempf, 0);
            func_80028EB8(45.0f, tempf, 0);
         
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/tiles", func_8003C1A4);

u8 func_8003C1A4(u16 index) {

    u32 padding[3];
    
    u8 result = 0;

    if (index == 0 && (gTileContext[index].flags & 1)) {
        result = gTileContext[index].rotation;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/tiles", func_8003C1E0);

bool func_8003C1E0(u16 index, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5) {

    bool result = 0;

    if (index == 0 && (gTileContext[index].flags & 1) && (gTileContext[index].flags & 2)) {
        
        gTileContext[index].unk_44 = arg4;
        gTileContext[index].unk_45 = arg5;

        gTileContext[index].unk_4.x = arg1;
        gTileContext[index].unk_4.y = arg2;
        gTileContext[index].unk_4.z = arg3;

        func_8003C8D4(&gTileContext[index].tileBank);

        result = 1;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/tiles", func_8003C280);

bool func_8003C280(u16 index, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {

    bool result = 0;
    
    if (index == 0 && (gTileContext[index].flags & 1)) {

        gTileContext[index].unk_10.x = arg1;
        gTileContext[index].unk_10.z = arg2;

        gTileContext[index].unk_1C.x = arg3;
        gTileContext[index].unk_1C.z = arg4;

        gTileContext[index].unk_28.x = arg5;
        gTileContext[index].unk_28.z = arg6;

        gTileContext[index].unk_34.x = arg7;
        gTileContext[index].unk_34.z = arg8;
        
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/tiles", func_8003C318);

f32 func_8003C318(u16 index, f32 arg1) {

    f32 result = 0.0f;

    if (index == 0 && (gTileContext[index].flags & 1)) { 

        if (gTileContext[index].unk_10.x < arg1 && gTileContext[index].unk_34.x < arg1) {
            
            if (!(arg1 < gTileContext[index].unk_1C.x) || !(arg1 < gTileContext[index].unk_28.x)) {
                result = gTileContext[index].unk_1C.x;
            } 
            
        } else {
            result = gTileContext[index].unk_10.x;
        }
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/tiles", func_8003C3E4);

INCLUDE_ASM(const s32, "system/tiles", func_8003C4B0);

INCLUDE_ASM(const s32, "system/tiles", func_8003C504);

//INCLUDE_ASM(const s32, "system/tiles", func_8003C570);

void func_8003C570(void) {

    u16 i = 0;

    do {

        gTileContext[i].flags &= ~( 2 | 4);
        i++;
        
    } while (i == 0);
    
}

INCLUDE_ASM(const s32, "system/tiles", func_8003C5C0);

INCLUDE_ASM(const s32, "system/tiles", func_8003C6E4);

INCLUDE_ASM(const s32, "system/tiles", func_8003C8D4);

INCLUDE_ASM(const s32, "system/tiles", func_8003CB3C);

//INCLUDE_RODATA(const s32, "system/tiles", D_8011EDE0);

static const f32 D_8011EDE0[] = { 0, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };