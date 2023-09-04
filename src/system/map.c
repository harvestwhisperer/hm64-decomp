#include "common.h"

#include "system/map.h"

#include "system/graphic.h"

// forward declarations
void func_800366F4(Vec3f*, u16, f32, f32);                  
u16 func_80036880(u32, f32, f32);  

// bss
extern MainMap mainMap[1];

extern f32 D_8013D550;
extern f32 D_80170460;
extern u8 D_801FB5CB;
extern u8 D_801FB700;
extern f32 D_801FB5D4;
extern f32 D_802226EC;

// rodata
extern Gfx D_8011ED68[4];
extern Gfx D_8011ED88[5];
extern Gfx D_8011EDC8[3];

//INCLUDE_ASM(const s32, "system/map", func_800337D0);

void func_800337D0(void) {

    u16 i, j;
    
    D_801FB700 = 0;
    D_801FB5CB = 0;
    
    for (i = 0; i < 1; i++) {
        
        mainMap[i].mapStruct8.flags = 0;
        mainMap[i].mapStruct8.unk_A = 0;
        mainMap[i].mapStruct8.unk_C = 0;
        mainMap[i].mapStruct8.unk_E = 0;

        mainMap[i].mapStruct7.unk_0.x = 0;
        mainMap[i].mapStruct7.unk_0.y = 0;
        mainMap[i].mapStruct7.unk_0.z = 0;

        mainMap[i].mapStruct7.unk_C.x = 1.0f;
        mainMap[i].mapStruct7.unk_C.y = 1.0f;
        mainMap[i].mapStruct7.unk_C.z = 1.0f;

        mainMap[i].mapStruct7.angles.x = 0;
        mainMap[i].mapStruct7.angles.y = 0;
        mainMap[i].mapStruct7.angles.z = 0;

        mainMap[i].mapStruct7.groundRgba.r = 255.0f;
        mainMap[i].mapStruct7.groundRgba.g = 255.0f;
        mainMap[i].mapStruct7.groundRgba.b = 255.0f;
        mainMap[i].mapStruct7.groundRgba.a = 255.0f;

        mainMap[i].mapStruct2.unk_48 = 0;
        mainMap[i].mapStruct2.unk_49 = 0;
        mainMap[i].mapStruct2.unk_4A = 0;
        mainMap[i].mapStruct2.unk_4B = 0;

        mainMap[i].mapStruct2.unk_3C.x = 0;
        mainMap[i].mapStruct2.unk_3C.y = 0;
        mainMap[i].mapStruct2.unk_3C.z = 0;

        mainMap[i].mapStruct2.unk_0.x = 0;
        mainMap[i].mapStruct2.unk_0.y = 0;
        mainMap[i].mapStruct2.unk_0.z = 0;

        mainMap[i].mapStruct2.unk_C.x = 0;
        mainMap[i].mapStruct2.unk_C.y = 0;
        mainMap[i].mapStruct2.unk_C.z = 0;
        
        mainMap[i].mapStruct2.unk_18.x = 0;
        mainMap[i].mapStruct2.unk_18.y = 0;
        mainMap[i].mapStruct2.unk_18.z = 0;

        mainMap[i].mapStruct2.unk_24.x = 0;
        mainMap[i].mapStruct2.unk_24.y = 0;
        mainMap[i].mapStruct2.unk_24.z = 0;

        mainMap[i].mapStruct2.unk_30.x = 0;
        mainMap[i].mapStruct2.unk_30.y = 0;
        mainMap[i].mapStruct2.unk_30.z = 0;
 
        for (j = 0; j < 0x400; j++) {
            mainMap[i].mapStruct1[j].unk_4 = 0;
            mainMap[i].mapStruct1[j].unk_6 = 0;
            mainMap[i].mapStruct1[j].unk_8 = 0;
            mainMap[i].mapStruct1[j].unk_9 = 0;
            mainMap[i].mapStruct1[j].unk_A = 0;
            mainMap[i].mapStruct1[j].unk_B = 0;
        }
    }
}

INCLUDE_ASM(const s32, "system/map", func_80033A90);

//INCLUDE_ASM(const s32, "system/map", func_80034090);

bool func_80034090(u16 mapIndex) {

    u8 i;
    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        
        D_801FB700 = 0;
        D_801FB5CB = 0;

        for (i = 0; i < 0x10; i++) {
            
            if (mainMap[mapIndex].mapStruct4[i].flags & 1) {
                func_8002B6B8(mainMap[mapIndex].mapStruct4[i].unk_C);
            }
            
            mainMap[mapIndex].mapStruct4[i].flags = 0;
        }
        
        for (i = 0; i < 0x10; i++) { 
            if (mainMap[mapIndex].mapStruct5[i].flags & 1) {
                func_8002B6B8(mainMap[mapIndex].mapStruct5[i].unk_0);
            }
            mainMap[mapIndex].mapStruct5[i].flags = 0;
        }
        
        mainMap[mapIndex].mapStruct8.flags = 0;
        result = 1;
        
    }

    return result;
}

INCLUDE_ASM(const s32, "system/map", func_8003423C);

//INCLUDE_ASM(const s32, "system/map", func_80034298);

bool func_80034298(u16 mapIndex, f32 arg1, f32 arg2, f32 arg3) {

    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = 1;
        mainMap[mapIndex].mapStruct7.unk_C.x = arg1;
        mainMap[mapIndex].mapStruct7.unk_C.y = arg2;
        mainMap[mapIndex].mapStruct7.unk_C.z = arg3;
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_800342F4);

//INCLUDE_ASM(const s32, "system/map", func_80034350);

bool func_80034350(u16 mapIndex, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    
    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        
        result = 1;

        mainMap[mapIndex].mapStruct7.groundRgba.r = arg1;
        mainMap[mapIndex].mapStruct7.groundRgba.g = arg2;
        mainMap[mapIndex].mapStruct7.groundRgba.b = arg3;
        mainMap[mapIndex].mapStruct7.groundRgba.a = arg4;
        
        mainMap[mapIndex].mapStruct7.defaultRgba.r = arg1;
        mainMap[mapIndex].mapStruct7.defaultRgba.g = arg2;
        mainMap[mapIndex].mapStruct7.defaultRgba.b = arg3;
        mainMap[mapIndex].mapStruct7.defaultRgba.a = arg4;
    
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_800343FC);

INCLUDE_ASM(const s32, "system/map", func_800344E8);

INCLUDE_ASM(const s32, "system/map", func_80034568);

INCLUDE_ASM(const s32, "system/map", func_800345E8);

INCLUDE_ASM(const s32, "system/map", func_80034668);

INCLUDE_ASM(const s32, "system/map", func_80034738);

INCLUDE_ASM(const s32, "system/map", func_80034A6C);

//INCLUDE_ASM(const s32, "system/map", func_80034C40);

bool func_80034C40(u16 mapIndex, u8 arg1, u16 arg2, u16 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA) {

    bool result = 0; 

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
         
        mainMap[mapIndex].mapStruct4[arg1].unk_C = arg2; 
        mainMap[mapIndex].mapStruct4[arg1].unk_E = arg3;

        mainMap[mapIndex].mapStruct4[arg1].unk_0.x = arg4;
        mainMap[mapIndex].mapStruct4[arg1].unk_0.y = arg5;
        mainMap[mapIndex].mapStruct4[arg1].unk_0.z = arg6;

        mainMap[mapIndex].mapStruct4[arg1].unk_10 = arg7;
        mainMap[mapIndex].mapStruct4[arg1].unk_11 = arg8;

        mainMap[mapIndex].mapStruct4[arg1].flags |= 1;
        
        if (arg9) {
            mainMap[mapIndex].mapStruct4[arg1].flags |= 4;
        }

        if (argA) {
            mainMap[mapIndex].mapStruct4[arg1].flags |= 8;
        }

        result = 1;
        
        mainMap[mapIndex].mapStruct8.unk_11++;
    }
    
    return result;
}
 
//INCLUDE_ASM(const s32, "system/map", func_80034D64);

bool func_80034D64(u16 mapIndex, u8 arg1, u16 arg2, u16 arg3) {

    bool result = 0;
 
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        mainMap[mapIndex].mapStruct5[arg1].unk_0 = arg2;
        mainMap[mapIndex].mapStruct5[arg1].unk_2 = arg3;
        mainMap[mapIndex].mapStruct5[arg1].flags = 1;
        result = 1;
    }

    return result; 
        
}

INCLUDE_ASM(const s32, "system/map", func_80034DC8);

INCLUDE_ASM(const s32, "system/map", func_80034E64);
 
INCLUDE_ASM(const s32, "system/map", func_80034EF0);
  
//INCLUDE_ASM(const s32, "system/map", func_80035004);

bool func_80035004(u16 mapIndex, u16 arg1, u8 arg2, u8 arg3) {
    
    bool result = 0;
    
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        // 2D array?
        mainMap[mapIndex].mapStruct6.arr2[arg3*0x14+arg2] = arg1;
        result = 1;
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/map", func_80035054);

bool func_80035054(u16 mapIndex, u16 bitmapIndex, u16 arg2, f32 arg3, f32 arg4, f32 arg5) {

    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
    
        if (bitmapIndex) {
            
            func_80026E78(&mainMap[mapIndex].mapBitmaps[bitmapIndex], func_80028888(arg2, mainMap[mapIndex].mapStruct6.vaddr1), func_800288B8(arg2, mainMap[mapIndex].mapStruct6.vaddr2, mainMap[mapIndex].mapStruct6.vaddr3));
            
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.x = arg3;
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.y = arg4;
            mainMap[mapIndex].mapBitmaps[bitmapIndex].unk_1C.z = arg5;
            
            result = 1;
        }    
    }
    
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80035150);

INCLUDE_ASM(const s32, "system/map", func_8003544C);

//INCLUDE_ASM(const s32, "system/map", func_80035914);

bool func_80035914(u16 mapIndex, f32 arg1, f32 arg2) {

    Vec3f vec;
    
    bool result = 0;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {

        func_800366F4(&vec, 0, arg1, arg2);

        if (vec.y != MAX_UNSIGNED_SHORT) {
            result = func_80036880(0, vec.x, vec.z) != 0;
        }

    }
    
    return result;
}
 
INCLUDE_ASM(const s32, "system/map", func_800359C8);

INCLUDE_ASM(const s32, "system/map", func_80035A58);

//INCLUDE_ASM(const s32, "system/map", func_80035DA4);

bool func_80035DA4(MainMap *map, u8 arg1, u8 arg2) {
    
  bool result = 0;

  if (-1.0f <= (((map->mapStruct2.unk_0.z * (map->mapStruct2.unk_C.x - arg1)) + (map->mapStruct2.unk_C.z * (arg1 - map->mapStruct2.unk_0.x))) + (arg2 * D_802226EC))) {
    if (-1.0f <= (((map->mapStruct2.unk_C.z * (map->mapStruct2.unk_18.x - arg1)) + (map->mapStruct2.unk_18.z * (arg1 - map->mapStruct2.unk_C.x))) + (arg2 * D_80170460))) {
      if (-1.0f <= (((map->mapStruct2.unk_18.z * (map->mapStruct2.unk_24.x - arg1)) + (map->mapStruct2.unk_24.z * (arg1 - map->mapStruct2.unk_18.x))) + (arg2 * D_8013D550))) {
        if (-1.0f <= (((map->mapStruct2.unk_24.z * (map->mapStruct2.unk_0.x - arg1)) + (map->mapStruct2.unk_0.z * (arg1 - map->mapStruct2.unk_24.x))) + (arg2 * D_801FB5D4))) {
          result = 1;
        }
      }
    }
  }
    
  return result;
}

INCLUDE_ASM(const s32, "system/map", func_80035EE0);

INCLUDE_ASM(const s32, "system/map", func_800360BC);

// returns index for interactable object/exit from rodata array per level
INCLUDE_ASM(const s32, "system/map", func_80036318);

INCLUDE_ASM(const s32, "system/map", func_80036490);

//INCLUDE_ASM(const s32, "system/map", func_80036610);

Vec3f *func_80036610(Vec3f *arg0, u16 mapIndex, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.z = 0.0f;
    vec.y = MAX_UNSIGNED_SHORT;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        
        vec2.y = 0;
        vec2.x = (arg2 + mainMap[mapIndex].mapStruct8.unk_0) / mainMap[mapIndex].unk_2C;
        vec2.z = (arg3 + mainMap[mapIndex].mapStruct8.unk_4) / mainMap[mapIndex].unk_2D;

        vec = vec2;
    }

    *arg0 = vec;
    
    return arg0;
}

INCLUDE_ASM(const s32, "system/map", func_800366F4);

INCLUDE_ASM(const s32, "system/map", func_80036880);

INCLUDE_ASM(const s32, "system/map", func_80036980);

//INCLUDE_ASM(const s32, "system/map", func_80036A84);

bool func_80036A84(u16 mapIndex) {

    bool result = 0;
     
    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        result = (mainMap[mapIndex].mapStruct8.flags >> 3) & 1;
    }

    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80036AB4);

INCLUDE_ASM(const s32, "system/map", func_80036C08);

INCLUDE_ASM(const s32, "system/map", func_80036FA0);

INCLUDE_ASM(const s32, "system/map", func_80037254);

INCLUDE_ASM(const s32, "system/map", func_800372F0);

INCLUDE_ASM(const s32, "system/map", func_80037350);

INCLUDE_ASM(const s32, "system/map", func_80037388);

INCLUDE_ASM(const s32, "system/map", func_80037400);

INCLUDE_ASM(const s32, "system/map", func_800374C0);

INCLUDE_ASM(const s32, "system/map", func_80037614);

// (signed or unsigned) int func_80037650(<signed/unsigned> int/short arg0, int arg1) {
//     return arg1 + *(int*)((arg0 & 0xffff) * 4 + arg1);

INCLUDE_ASM(const s32, "system/map", func_80037650);

INCLUDE_ASM(const s32, "system/map", func_80037668);

// param1: Gfx*
// param2: 8013DC40
//INCLUDE_ASM(const s32, "system/map", func_8003797C);

Gfx* func_8003797C(Gfx* dl, MainMap* arg1, u8 arg2) {

    Tile bitmap;

    func_80026E78((Bitmap*)&bitmap, func_80028888(arg2, arg1->unk_10), func_800288A0(arg2, arg1->unk_14));

    gDPLoadTextureTile_4b(dl++, bitmap.timg, bitmap.fmt, bitmap.width, bitmap.height, 0, 0, bitmap.width - 1, bitmap.height - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTLUT_pal16(dl++, 0, bitmap.pal);

    return dl++;
}

INCLUDE_ASM(const s32, "system/map", func_80037BC4);

INCLUDE_ASM(const s32, "system/map", func_80037DF0);

INCLUDE_ASM(const s32, "system/map", func_80037F08);

INCLUDE_ASM(const s32, "system/map", func_800383B0);

void func_80038514(void) {}

INCLUDE_ASM(const s32, "system/map", func_8003851C);

//INCLUDE_ASM(const s32, "system/map", func_80038630);

// decompress vec3fs
void func_80038630(Decompressed* arg0, Compressed* arg1) {

    u32 padding[8];
    
    arg0->size = arg1->size;
    arg0->x = (s16)(arg1->part1 | (arg1->part2 << 8));
    arg0->y = (s16)(arg1->part3 | (arg1->part4 << 8));
    arg0->z = (s16)(arg1->part5 | (arg1->part6 << 8));
}

//INCLUDE_ASM(const s32, "system/map", func_800386C0);

void func_800386C0(UnknownMapStruct3* arg0, u8* arg1) {
    arg0->unk_2 = arg1[0];
    arg0->unk_3 = arg1[1];
}

#ifdef PERMUTER
u8* func_800386D4(u16 arg0, u8* arg1) {

    u16 i;
    u8 arr[4];

    arg1++;

    for (i = 0; i < arg0; i++) {
        arr[2] = arg1[0];
        arg1 += arg1[1] * 7;
        arr[3] = 2;
   }

    return arg1;

}
#else
INCLUDE_ASM(const s32, "system/map", func_800386D4);
#endif

#ifdef PERMUTER
u32 func_800387E0(u16 arg0, u32* arg1) {
    return arg1 + *(arg1 + arg0);
}
#else
INCLUDE_ASM(const s32, "system/map", func_80038728);
#endif

INCLUDE_ASM(const s32, "system/map", func_800387E0);

INCLUDE_ASM(const s32, "system/map", func_800387F8);

//INCLUDE_ASM(const s32, "system/map", func_80038810);

u8 func_80038810(u16 mapIndex) {

    u8 result;

    if (mapIndex == 0 && (mainMap[mapIndex].mapStruct8.flags & 1)) {
        if (!(mainMap[mapIndex].mapStruct8.flags & 2)) {
            mainMap[mapIndex].mapStruct8.flags |= 2;
            func_80036FA0(0);
        }
    }

    // never initialized; whoops
    return result;
}

INCLUDE_ASM(const s32, "system/map", func_8003886C);

INCLUDE_ASM(const s32, "system/map", func_800388A4);

INCLUDE_ASM(const s32, "system/map", func_80038900);

INCLUDE_ASM(const s32, "system/map", func_80038990);

INCLUDE_ASM(const s32, "system/map", func_80038A2C);

INCLUDE_ASM(const s32, "system/map", func_80038AE0);

INCLUDE_ASM(const s32, "system/map", func_80038B58);

INCLUDE_ASM(const s32, "system/map", func_80038BC4);

INCLUDE_ASM(const s32, "system/map", func_800393E0);

INCLUDE_ASM(const s32, "system/map", func_80039990);

//INCLUDE_ASM(const s32, "system/map", func_80039E00);

INCLUDE_ASM(const s32, "system/map", func_80039E20);

INCLUDE_ASM(const s32, "system/map", func_80039F58);

INCLUDE_ASM(const s32, "system/map", func_8003A1BC);

//INCLUDE_ASM(const s32, "system/map", func_8003AC14);

void func_8003AC14(Gfx* dl, UnknownMapStruct8* arg1) {
    
    *dl = D_8011ED88[1];
    dl++;
    *dl = D_8011EDC8[0];
    dl++;
    *dl = D_8011EDC8[1];
    dl++;
    *dl = D_8011ED68[2];
    dl++;

    func_80026F88(dl++, arg1, 0, arg1->unk_E);
}
 
// param1: Gfx* 8020DE78
// Vertex bank: D_80165500[]
//      size 0x40
// param2: 8013DC40
// param3: 8014318C
// param4 = offset into vertex bank
INCLUDE_ASM(const s32, "system/map", func_8003ACA8);

INCLUDE_ASM(const s32, "system/map", func_8003AF58);

INCLUDE_ASM(const s32, "system/map", func_8003B100);

// main loop function
INCLUDE_ASM(const s32, "system/map", func_8003B1BC);


// static const Gfx D_8011ED68[4] = {

//     gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
//     gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2),
//     gsDPSetTextureFilter(G_TF_BILERP),
//     gsSPEndDisplayList()

// };

// gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA)

INCLUDE_RODATA(const s32, "system/map", D_8011ED68);

INCLUDE_RODATA(const s32, "system/map", D_8011ED6C);

// gsDPSetRenderMode(G_RM_RA_ZB_OPA_SURF, G_RM_RA_ZB_OPA_SURF2)

INCLUDE_RODATA(const s32, "system/map", D_8011ED70);

INCLUDE_RODATA(const s32, "system/map", D_8011ED74);

// gsDPSetTextureFilter(G_TF_BILERP)

INCLUDE_RODATA(const s32, "system/map", D_8011ED78);

INCLUDE_RODATA(const s32, "system/map", D_8011ED7C);

// gsSPEndDisplayList()

INCLUDE_RODATA(const s32, "system/map", D_8011ED80);

INCLUDE_RODATA(const s32, "system/map", D_8011ED84);


// static const Gfx D_8011ED88[5] = {

//     gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON),
//     gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
//     gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
//     gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0),
//     gsDPPipeSync()

// };

// gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_ON),

INCLUDE_RODATA(const s32, "system/map", D_8011ED88);

INCLUDE_RODATA(const s32, "system/map", D_8011ED8C);

// gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA)

INCLUDE_RODATA(const s32, "system/map", D_8011ED90);

INCLUDE_RODATA(const s32, "system/map", D_8011ED94);

// gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF)

INCLUDE_RODATA(const s32, "system/map", D_8011ED98);

INCLUDE_RODATA(const s32, "system/map", D_8011ED9C);

// gsDPSetCombineMode
// gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0)

INCLUDE_RODATA(const s32, "system/map", D_8011EDA0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDA4);

// gsDPPipeSync()

INCLUDE_RODATA(const s32, "system/map", D_8011EDA8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDAC);


// binary data

INCLUDE_RODATA(const s32, "system/map", D_8011EDB0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDB4);


// static const Gfx D_8011EDC8[3] = {

//     gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2),
//     gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON),
//     gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)

// };

// gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE2)

INCLUDE_RODATA(const s32, "system/map", D_8011EDC8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDCC);

// gsSPTexture(qu016(0.5), qu016(0.5), 0, G_TX_RENDERTILE, G_ON)

INCLUDE_RODATA(const s32, "system/map", D_8011EDD0);

INCLUDE_RODATA(const s32, "system/map", D_8011EDD4);

// gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0)

INCLUDE_RODATA(const s32, "system/map", D_8011EDD8);

INCLUDE_RODATA(const s32, "system/map", D_8011EDDC);
