#include "common.h"

#include "system/map.h"

#include "system/graphic.h"

// forward declarations
void func_800366F4(Vec3f*, u16, f32, f32);                  
u16 func_80036880(u32, f32, f32);  

// bss
extern UnknownMapStruct1 D_80158248;
extern UnknownMapStruct2 D_8013DC40;
extern UnknownMapStruct3 D_80143168[];

// convert to 2d array
extern u16 D_80141D28;

extern f32 D_8013D550;
extern f32 D_80170460;
extern f32 D_801FB5D4;
extern f32 D_802226EC;

// struct/array
extern f32 D_80143180;
extern f32 D_80143184;
extern f32 D_80143188;

extern u32 D_80141C98;
extern u32 D_80141C9C;
extern u32 D_80141CA0;

// shared
// ground rgba
extern Vec4f D_801580D8;
extern Vec4f D_801580E8;

// rodata
extern Gfx D_8011ED68[4];
extern Gfx D_8011ED88[5];
extern Gfx D_8011EDC8[3];

INCLUDE_ASM(const s32, "system/map", func_800337D0);

INCLUDE_ASM(const s32, "system/map", func_80033A90);

INCLUDE_ASM(const s32, "system/map", func_80034090);

INCLUDE_ASM(const s32, "system/map", func_8003423C);

INCLUDE_ASM(const s32, "system/map", func_80034298);

INCLUDE_ASM(const s32, "system/map", func_800342F4);

//INCLUDE_ASM(const s32, "system/map", func_80034350);

bool func_80034350(u16 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    
    bool res = 0;
    
    if (!arg0 && (D_80158248.flags & 1)) {
        
        res = 1;

        D_801580D8.r = arg1;
        D_801580D8.g = arg2;
        D_801580D8.b = arg3;
        D_801580D8.a = arg4;
        
        D_801580E8.r = arg1;
        D_801580E8.g = arg2;
        D_801580E8.b = arg3;
        D_801580E8.a = arg4;
    
    }
    
    return res;
}


INCLUDE_ASM(const s32, "system/map", func_800343FC);

INCLUDE_ASM(const s32, "system/map", func_800344E8);

INCLUDE_ASM(const s32, "system/map", func_80034568);

INCLUDE_ASM(const s32, "system/map", func_800345E8);

INCLUDE_ASM(const s32, "system/map", func_80034668);

INCLUDE_ASM(const s32, "system/map", func_80034738);

INCLUDE_ASM(const s32, "system/map", func_80034A6C);

INCLUDE_ASM(const s32, "system/map", func_80034C40);

INCLUDE_ASM(const s32, "system/map", func_80034D64);

INCLUDE_ASM(const s32, "system/map", func_80034DC8);

INCLUDE_ASM(const s32, "system/map", func_80034E64);

INCLUDE_ASM(const s32, "system/map", func_80034EF0);

//INCLUDE_ASM(const s32, "system/map", func_80035004);

bool func_80035004(u16 arg0, u16 arg1, u8 arg2, u8 arg3) {

    bool result;

    result = 0;

    if (!arg0 && (D_80158248.flags & 1)) {
        // convert to 2D array if possible
        *(&D_80141D28 + (arg3 * 0x14 + arg2)) = arg1;
        result = 1;
    }

    return result;
}

INCLUDE_ASM(const s32, "system/map", func_80035054);

// non-matching
/*
u8 func_80035054(u16 arg0, u32 arg1, u16 arg2, f32 arg3, f32 arg4, f32 arg5) {

    u8 res;
    void *temp1;

    if (!arg0) {
        
        if (D_80158248.flags & 1) {
            if (arg2) {
                func_80026E78(&D_80143168[arg1], func_80028888(arg2, D_80141C98), func_800288B8(arg2, D_80141C9C, D_80141CA0));
                res = 1;
                D_80143168->unk_20.x = arg3; 
                D_80143168->unk_20.y = arg4;
                D_80143168->unk_20.z = arg4;
            }
        }
    }
    
    return res;
}
*/

INCLUDE_ASM(const s32, "system/map", func_80035150);

INCLUDE_ASM(const s32, "system/map", func_8003544C);

//INCLUDE_ASM(const s32, "system/map", func_80035914);

bool func_80035914(u16 arg0, f32 arg1, f32 arg2) {

    Vec3f vec;
    
    bool result = 0;


    if (!arg0 && (D_80158248.flags & 1)) {

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

u8 func_80035DA4(UnknownMapStruct2 *arg0, u8 arg1, u8 arg2) {
    
  u8 res = 0;

  if (-1.0f <= (((arg0->unk_3D54 * (arg0->unk_3D58 - arg1)) + (arg0->unk_3D60 * (arg1 - arg0->unk_3D4C))) + (arg2 * D_802226EC))) {
    if (-1.0f <= (((arg0->unk_3D60 * (arg0->unk_3D64 - arg1)) + (arg0->unk_3D6C * (arg1 - arg0->unk_3D58))) + (arg2 * D_80170460))) {
      if (-1.0f <= (((arg0->unk_3D6C * (arg0->unk_3D70 - arg1)) + (arg0->unk_3D78 * (arg1 - arg0->unk_3D64))) + (arg2 * D_8013D550))) {
        if (-1.0f <= (((arg0->unk_3D78 * (arg0->unk_3D4C - arg1)) + (arg0->unk_3D54 * (arg1 - arg0->unk_3D70))) + (arg2 * D_801FB5D4))) {
          res = 1;
        }
      }
    }
  }
    
  return res;
  
}

INCLUDE_ASM(const s32, "system/map", func_80035EE0);

INCLUDE_ASM(const s32, "system/map", func_800360BC);

// returns index for interactable object/exit from rodata array per level
INCLUDE_ASM(const s32, "system/map", func_80036318);

INCLUDE_ASM(const s32, "system/map", func_80036490);

//INCLUDE_ASM(const s32, "system/map", func_80036610);

Vec3f *func_80036610(Vec3f *arg0, u16 arg1, f32 arg2, f32 arg3) {

    Vec3f vec;
    Vec3f vec2;

    vec.x = 0.0f;
    vec.z = 0.0f;
    vec.y = MAX_UNSIGNED_SHORT;

    if (!arg1 && D_80158248.flags & 1) {
        
        vec2.y = 0;
        vec2.x = (arg2 + D_80158248.unk_0) / D_8013DC40.unk_2C;
        vec2.z = (arg3 + D_80158248.unk_4) / D_8013DC40.unk_2D;

        vec = vec2;
    }

    *arg0 = vec;
    
    return arg0;
}

INCLUDE_ASM(const s32, "system/map", func_800366F4);

INCLUDE_ASM(const s32, "system/map", func_80036880);

INCLUDE_ASM(const s32, "system/map", func_80036980);

//INCLUDE_ASM(const s32, "system/map", func_80036A84);

bool func_80036A84(u16 arg0) {

    bool result = 0;
    
    if (!arg0 && (D_80158248.flags & 1)) {
        // bit 4
        result = (D_80158248.flags >> 3) & 1;
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

INCLUDE_ASM(const s32, "system/map", func_80037650);

INCLUDE_ASM(const s32, "system/map", func_80037668);

INCLUDE_ASM(const s32, "system/map", func_8003797C);

INCLUDE_ASM(const s32, "system/map", func_80037BC4);

INCLUDE_ASM(const s32, "system/map", func_80037DF0);

INCLUDE_ASM(const s32, "system/map", func_80037F08);

INCLUDE_ASM(const s32, "system/map", func_800383B0);

void func_80038514(void) {}

INCLUDE_ASM(const s32, "system/map", func_8003851C);

INCLUDE_ASM(const s32, "system/map", func_80038630);

INCLUDE_ASM(const s32, "system/map", func_800386C0);

INCLUDE_ASM(const s32, "system/map", func_800386D4);

INCLUDE_ASM(const s32, "system/map", func_80038728);

INCLUDE_ASM(const s32, "system/map", func_800387E0);

INCLUDE_ASM(const s32, "system/map", func_800387F8);

INCLUDE_ASM(const s32, "system/map", func_80038810);

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

void func_8003AC14(Gfx* dl, UnknownMapStruct1* arg1) {
    
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
