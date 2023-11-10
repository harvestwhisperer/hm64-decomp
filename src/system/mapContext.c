#include "common.h"

#include "system/mapContext.h"

#include "system/graphic.h"
#include "system/map.h"
#include "sprite.h"
#include "system/worldGraphics.h"

// bss
extern Vec4f D_8013D248;
extern Vec4f D_802373F8;

// flags
extern s16 D_8017045A;

extern LevelMapContext gMapModelContext[1];
extern MapContextAddresses gMapModelAddresses[0x60];

// forward declarations
void func_8003C8D4(u8*); 
 
static const f32 D_8011EDE0[];
                    
//INCLUDE_ASM(const s32, "system/mapContext", initializeMapContext);

void initializeMapContext(void) {
    
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
        
        gMapModelContext[i].flags = 0;
        
        gMapModelContext[i].mapIndex = 0;
        
        gMapModelContext[i].unk_4.x = 0;
        gMapModelContext[i].unk_4.y = 0;
        gMapModelContext[i].unk_4.z = 0;

        gMapModelContext[i].rotation = 0;
        
        gMapModelContext[i].unk_44 = 0;
        gMapModelContext[i].unk_45 = 0;
        
        gMapModelContext[i].unk_10.x = 0;
        gMapModelContext[i].unk_10.y = 0;
        gMapModelContext[i].unk_10.z = 0;

        gMapModelContext[i].unk_1C.x = 0;
        gMapModelContext[i].unk_1C.y = 0;
        gMapModelContext[i].unk_1C.z = 0;

        gMapModelContext[i].unk_28.x = 0;
        gMapModelContext[i].unk_28.y = 0;
        gMapModelContext[i].unk_28.z = 0;

        gMapModelContext[i].unk_34.x = 0;
        gMapModelContext[i].unk_34.y = 0;
        gMapModelContext[i].unk_34.z = 0;
    
    }
}

//INCLUDE_ASM(const s32, "system/mapContext", setMapModelAddresses);

bool setMapModelAddresses(u16 index, void *start, void *end) {
    
    bool result = 0;
    
    if (index < MAX_MODELS) {
        if (!(gMapModelAddresses[index].flags & 1)) {
            result = 1;
            gMapModelAddresses[index].romStart = start;
            gMapModelAddresses[index].romEnd = end;
            gMapModelAddresses[index].flags = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003BA44);

bool func_8003BA44(u16 index, u16 mapIndex, void *modelData) {
    
    bool result;

    result = 0;
    
    if (index == 0 && !(gMapModelContext[index].flags & 1)) {
        
        result = 1;
        
        gMapModelContext[index].mapIndex = mapIndex;
        
        gMapModelContext[index].modelData = modelData;
        
        gMapModelContext[index].flags = 1;

        gMapModelContext[index].unk_4.x = 0;
        gMapModelContext[index].unk_4.y = 0;
        gMapModelContext[index].unk_4.z = 0;

        gMapModelContext[index].unk_44 = 0;
        gMapModelContext[index].unk_45 = 0;

        gMapModelContext[index].unk_10.x = 0;
        gMapModelContext[index].unk_10.y = 0;
        gMapModelContext[index].unk_10.z = 0;

        gMapModelContext[index].unk_1C.x = 0;
        gMapModelContext[index].unk_1C.y = 0;
        gMapModelContext[index].unk_1C.z = 0;

        gMapModelContext[index].unk_28.x = 0;
        gMapModelContext[index].unk_28.y = 0;
        gMapModelContext[index].unk_28.z = 0;

        gMapModelContext[index].unk_34.x = 0;
        gMapModelContext[index].unk_34.y = 0;
        gMapModelContext[index].unk_34.z = 0;

    }
    
    return result;
}

// map loading function
bool func_8003BB14(u16 index, u16 mapIndex) {

    bool result = 0;
    
    if (index == 0 && (gMapModelContext[index].flags & 1)) {
        
        // dma map data
        func_8003BC50(0, mapIndex);
        
        // set vec3fs on mapStruct8
        func_8003423C(gMapModelContext[index].mapIndex, 0.0f, 0.0f, 0.0f);
        func_80034298(gMapModelContext[index].mapIndex, 1.0f, 1.0f, 1.0f);
        func_800342F4(gMapModelContext[index].mapIndex, 45.0f, 0.0f, 0.0f);
        
        gMapModelContext[index].rotation = 0;
        
        func_8003BDA4(0, 0.0f, 0.0f, 0.0f);
        func_8003C084(0, 0);
        func_8003C1E0(0, 0.0f, 0.0f, 0.0f, 0, 0);
        func_80034350(gMapModelContext[index].mapIndex, 0, 0, 0, 0);
        
        result = 1; 

    }

    return result;
}

INCLUDE_ASM(const s32, "system/mapContext", func_8003BC50);

//INCLUDE_ASM(const s32, "system/mapContext", func_8003BD60);

bool func_8003BD60(u16 index) {
    
    bool result = 0;
    
    if (index == 0 &&  (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 2)) {
        gMapModelContext[index].flags |= 4;
        result = 1;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003BDA4);

bool func_8003BDA4(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index == 0 &&  (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 2)) {
        result = 1;
        gMapModelContext[index].unk_4.x = arg1;
        gMapModelContext[index].unk_4.y = arg2;
        gMapModelContext[index].unk_4.z = arg3;
    }
    
    return result;
}


//INCLUDE_ASM(const s32, "system/mapContext", func_8003BE0C);

bool func_8003BE0C(u16 index, f32 arg1, f32 arg2, f32 arg3) {
    
    bool result = 0;
    
    if (index == 0 &&  (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 2)) {
        result = 1;
        gMapModelContext[index].unk_4.x += arg1;
        gMapModelContext[index].unk_4.y += arg2;
        gMapModelContext[index].unk_4.z += arg3;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003BE98);

bool func_8003BE98(u16 index, u8 r, u8 g, u8 b, u8 a) {
    
    bool result = 0;
    
    if (index == 0 &&  (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 2)) {
        func_80034350(gMapModelContext[index].mapIndex, r, g, b, a);
        D_802373F8.r = r;
        D_802373F8.g = g;
        D_802373F8.b = b;
        D_802373F8.a = a;
        result = 1;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003BF7C);

bool func_8003BF7C(u16 index, u8 r, u8 g, u8 b, u8 a, s16 arg5) {
    
    bool result = 0;
    
    if (index == 0 &&  (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 2)) {
        func_80034738(gMapModelContext[index].mapIndex, r, g, b, a, arg5);
        D_8013D248.r = r;
        D_8013D248.g = g;
        D_8013D248.b = b;
        D_8013D248.a = a;
        D_8017045A = arg5;
        result = 1;
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003C084);

bool func_8003C084(u16 index, u8 arg1) {

    bool result;

    Angles angles;
    f32 *ptr;
    f32 tempf;
    
    angles = *(Angles*)D_8011EDE0;
    ptr = (f32*)&angles;

    result = 0;

    if (index == 0 && gMapModelContext[index].flags & 1 && gMapModelContext[index].flags & 2) {

        if (!(gMapModelContext[index].flags & 0x18)) {
        
            gMapModelContext[index].rotation = arg1;
            tempf = ptr[arg1];
        
            func_800342F4(gMapModelContext[index].mapIndex, 45.0f, tempf, 0);
            func_80028EB8(45.0f, tempf, 0);
         
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003C1A4);

u8 func_8003C1A4(u16 index) {

    u32 padding[3];
    
    u8 result = 0;

    if (index == 0 && (gMapModelContext[index].flags & 1)) {
        result = gMapModelContext[index].rotation;
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003C1E0);

bool func_8003C1E0(u16 index, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5) {

    bool result = 0;

    if (index == 0 && (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 2)) {
        
        gMapModelContext[index].unk_44 = arg4;
        gMapModelContext[index].unk_45 = arg5;

        gMapModelContext[index].unk_4.x = arg1;
        gMapModelContext[index].unk_4.y = arg2;
        gMapModelContext[index].unk_4.z = arg3;

        func_8003C8D4(&gMapModelContext[index].modelData);

        result = 1;
    }
    
    return result;
    
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003C280);

bool func_8003C280(u16 index, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8) {

    bool result = 0;
    
    if (index == 0 && (gMapModelContext[index].flags & 1)) {

        gMapModelContext[index].unk_10.x = arg1;
        gMapModelContext[index].unk_10.z = arg2;

        gMapModelContext[index].unk_1C.x = arg3;
        gMapModelContext[index].unk_1C.z = arg4;

        gMapModelContext[index].unk_28.x = arg5;
        gMapModelContext[index].unk_28.z = arg6;

        gMapModelContext[index].unk_34.x = arg7;
        gMapModelContext[index].unk_34.z = arg8;
        
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003C318);

f32 func_8003C318(u16 index, f32 arg1) {

    f32 result = 0.0f;

    if (index == 0 && (gMapModelContext[index].flags & 1)) { 

        if (gMapModelContext[index].unk_10.x < arg1 && gMapModelContext[index].unk_34.x < arg1) {
            
            if (!(arg1 < gMapModelContext[index].unk_1C.x) || !(arg1 < gMapModelContext[index].unk_28.x)) {
                result = gMapModelContext[index].unk_1C.x;
            } 
            
        } else {
            result = gMapModelContext[index].unk_10.x;
        }
    }
    
    return result;
}


//INCLUDE_ASM(const s32, "system/mapContext", func_8003C3E4);

// get z value
f32 func_8003C3E4(u16 index, f32 arg1) {

    f32 result = 0.0f;
    
    if (index == 0 && gMapModelContext[index].flags & 1) {

        if (arg1 < gMapModelContext[index].unk_10.z && arg1 < gMapModelContext[index].unk_1C.z) {

            if (!(arg1 > gMapModelContext[index].unk_28.z)) {
                goto label;
            } else if (!(arg1 > gMapModelContext[index].unk_34.z)) {
label:
                result = gMapModelContext[index].unk_28.z;
            }
            
        } else {
            result = gMapModelContext[index].unk_10.z;
        }
    } 

    return result;
    
}

//INCLUDE_ASM(const s32, "system/mapContext", func_8003C4B0);

bool func_8003C4B0(u16 index) {

    bool result = 0;

    if (index == 0 && (gMapModelContext[index].flags & 1)) {
        func_80034090(gMapModelContext[index].mapIndex);
        result = 1;
        gMapModelContext[index].flags = 0;
    }

    return result;
}


//INCLUDE_ASM(const s32, "system/mapContext", func_8003C504);

bool func_8003C504(u16 index) {

    bool result = 0;

    if (index == 0 && (gMapModelContext[index].flags & 1) && (gMapModelContext[index].flags & 4)) {
        func_80034090(gMapModelContext[index].mapIndex);
        result = 1;
        gMapModelContext[index].flags &= ~(2 | 4);
    }

    return result;

}


//INCLUDE_ASM(const s32, "system/mapContext", func_8003C570);

void func_8003C570(void) {

    u16 i = 0;

    do {

        gMapModelContext[i].flags &= ~( 2 | 4);
        i++;
        
    } while (i == 0);
    
}

INCLUDE_ASM(const s32, "system/mapContext", func_8003C5C0);

INCLUDE_ASM(const s32, "system/mapContext", func_8003C6E4);

INCLUDE_ASM(const s32, "system/mapContext", func_8003C8D4);

//INCLUDE_ASM(const s32, "system/mapContext", func_8003CB3C);

void func_8003CB3C(u16 index) {

    if (gMapModelContext[index].flags & 8) {
        func_800345E8(gMapModelContext[index].mapIndex, 0, -1.0f, 0);
        func_80028EF8(0, -1.0f, 0);
    } else {
        func_800345E8(gMapModelContext[index].mapIndex, 0, 1.0f, 0);
        func_80028EF8(0, 1.0f, 0);
    }

    gMapModelContext[index].unk_46++;

    if (gMapModelContext[index].unk_46 == 22) {
        if (gMapModelContext[index].flags & 8) {
            func_8002F770(1);
        } else {
            func_8002F770(-1);
        }
    }

    if (gMapModelContext[index].unk_46 >= 45) { 

        if (gMapModelContext[index].flags & 8) { 
            gMapModelContext[index].rotation = (gMapModelContext[index].rotation + 7) % 8; 
        } else {
            gMapModelContext[index].rotation = (gMapModelContext[index].rotation + 1) % 8;
        }

        gMapModelContext[index].flags &= ~( 8 | 0x10);

        if (gMapModelContext[index].unk_47 == 0xFF || gMapModelContext[index].rotation == gMapModelContext[index].unk_47) {
            previousWorldRotationAngles.x = currentWorldRotationAngles.x;
            previousWorldRotationAngles.y = currentWorldRotationAngles.y;
            previousWorldRotationAngles.z = currentWorldRotationAngles.z;
        } else {
            func_8003C5C0(index, 0xFF, gMapModelContext[index].unk_47);
        }
        
    }
}

//INCLUDE_RODATA(const s32, "system/mapContext", D_8011EDE0);

static const f32 D_8011EDE0[] = { 0, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f };