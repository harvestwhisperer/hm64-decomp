#include "common.h"

// vec4f
extern f32 D_8013D248;
extern f32 D_8013D24C;
extern f32 D_8013D250;
extern f32 D_8013D254;

// vec4f
extern f32 D_802373F8;
extern f32 D_802373FC;
extern f32 D_80237400;
extern f32 D_80237404;

// flags
extern u16 D_8017045A;

extern TileInfo gTiles[0x60];

void func_8003423C(u16, f32, f32, f32);
void func_80034298(u16, f32, f32, f32);                
void func_800342F4(u16, f32, f32, f32);                  
void func_80034350(u16, u8, u8, u8, u8);              
void func_8003BC50(u8, u16);                           
void func_8003BDA4(u8, f32, f32, f32);                      
void func_8003C084(u16, u8);                             
void func_8003C1E0(u16, f32, f32, f32, u8, u8);             

extern MapTileContext gTileContext[1];

extern u16 gTileContextFlags;


//INCLUDE_ASM(const s32, "system/tiles", func_8003B870);

void func_8003B870(void) {
    
    u16 i;
    
    // vec4f
    D_802373F8 = 0;
    D_802373FC = 0;
    D_80237400 = 0;
    D_80237404 = 0;
    
    // vec4f
    D_8013D248 = 0;
    D_8013D24C = 0;
    D_8013D250 = 0;
    D_8013D254 = 0;
    
    // flags
    D_8017045A = 0;
    
    for (i = 0; i < 1; i++ ){
        
        gTileContext[i].flags = 0;
        
        gTileContext[i].unk_40 = 0;
        
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

u8 setTileInfo(u16 index, void *start, void *end) {
    u8 result = 0;
    
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

u32 func_8003BA44(u16 arg0, s16 flag, void *tileBank) {
    u32 result;

    result = 0;
    
    // doesn't match if flags are referenced from array
    if (!arg0 && !(gTileContextFlags & 1)) {
        
        result = 1;
        
        gTileContext[0].unk_40 = flag;
        
        gTileContext[0].tileBank = tileBank;
        
        gTileContext[0].flags = 1;

        gTileContext[0].unk_4.x = 0;
        gTileContext[0].unk_4.y = 0;
        gTileContext[0].unk_4.z = 0;

        gTileContext[0].unk_44 = 0;
        gTileContext[0].unk_45 = 0;

        gTileContext[0].unk_10.x = 0;
        gTileContext[0].unk_10.y = 0;
        gTileContext[0].unk_10.z = 0;

        gTileContext[0].unk_1C.x = 0;
        gTileContext[0].unk_1C.y = 0;
        gTileContext[0].unk_1C.z = 0;

        gTileContext[0].unk_28.x = 0;
        gTileContext[0].unk_28.y = 0;
        gTileContext[0].unk_28.z = 0;

        gTileContext[0].unk_34.x = 0;
        gTileContext[0].unk_34.y = 0;
        gTileContext[0].unk_34.z = 0;

    }
    
    return result;
}

// map loading function
u8 func_8003BB14(u16 arg0, u16 mapIndex) {

    u8 result;

    result = 0;
    
    if (!arg0 && (gTileContext[0].flags & 1)) {
        
        func_8003BC50(0, mapIndex);
        func_8003423C(gTileContext[0].unk_40, 0.0f, 0.0f, 0.0f);
        func_80034298(gTileContext[0].unk_40, 1.0f, 1.0f, 1.0f);
        func_800342F4(gTileContext[0].unk_40, 45.0f, 0.0f, 0.0f);
        
        gTileContext[0].rotation = 0;
        
        func_8003BDA4(0, 0.0f, 0.0f, 0.0f);
        func_8003C084(0, 0);
        func_8003C1E0(0, 0.0f, 0.0f, 0.0f, 0, 0);
        func_80034350(gTileContext[0].unk_40, 0, 0, 0, 0);
        
        result = 1;

    }

    return result;
}

INCLUDE_ASM(const s32, "system/tiles", func_8003BC50);

INCLUDE_ASM(const s32, "system/tiles", func_8003BD60);

INCLUDE_ASM(const s32, "system/tiles", func_8003BDA4);

INCLUDE_ASM(const s32, "system/tiles", func_8003BE0C);

INCLUDE_ASM(const s32, "system/tiles", func_8003BE98);

INCLUDE_ASM(const s32, "system/tiles", func_8003BF7C);

INCLUDE_ASM(const s32, "system/tiles", func_8003C084);

INCLUDE_ASM(const s32, "system/tiles", func_8003C1A4);

INCLUDE_ASM(const s32, "system/tiles", func_8003C1E0);

INCLUDE_ASM(const s32, "system/tiles", func_8003C280);

INCLUDE_ASM(const s32, "system/tiles", func_8003C318);

INCLUDE_ASM(const s32, "system/tiles", func_8003C3E4);

INCLUDE_ASM(const s32, "system/tiles", func_8003C4B0);

INCLUDE_ASM(const s32, "system/tiles", func_8003C504);

INCLUDE_ASM(const s32, "system/tiles", func_8003C570);

INCLUDE_ASM(const s32, "system/tiles", func_8003C5C0);

INCLUDE_ASM(const s32, "system/tiles", func_8003C6E4);

INCLUDE_ASM(const s32, "system/tiles", func_8003C8D4);

INCLUDE_ASM(const s32, "system/tiles", func_8003CB3C);
