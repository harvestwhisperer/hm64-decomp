#include "common.h"

#include "cutscene.h"

#include "system/map.h"
#include "system/sprite.h"

// forward declarations
void func_800471B0(u16);                               
void func_80047640(u16);                               
void func_80047E34(u16);                               

// bss
// cutscene handler callbacks
extern void (*D_801133D0[])(u16);
 
extern CutsceneMap D_801808B0[42];

//INCLUDE_ASM(const s32, "system/cutscene", initializeCutsceneMaps);

void initializeCutsceneMaps(void) {

    u16 i;

    for (i = 0; i < 42; i++) {
        
        D_801808B0[i].unk_64 = 0;
        D_801808B0[i].unk_66 = 0;
        D_801808B0[i].unk_68 = 0;
        D_801808B0[i].unk_69 = 0;
        D_801808B0[i].unk_6A = 0;

        D_801808B0[i].offsets.x = 0;
        D_801808B0[i].offsets.y = 0;
        D_801808B0[i].offsets.z = 0;

        D_801808B0[i].unk_58.x = 0;
        D_801808B0[i].unk_58.y = 0;
        D_801808B0[i].unk_58.z = 0;

        D_801808B0[i].flags = 0;

        D_801808B0[i].unk_20 = 0;
        D_801808B0[i].unk_22 = 0;
        D_801808B0[i].unk_24 = 0;
        D_801808B0[i].unk_26 = 0;
        D_801808B0[i].unk_28 = 0;
        D_801808B0[i].unk_2A = 0;

        D_801808B0[i].unk_1C = 0xFFFF;

        D_801808B0[i].unk_C.x = 0;
        D_801808B0[i].unk_C.y = 0;
        D_801808B0[i].unk_C.z = 0;

        D_801808B0[i].cameraFlags = 0;
    }
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800469A8);

bool func_800469A8(u16 index, Cutscene *cutscenePointer) {

    bool result = 0;
    
    if (index < 42) {

        if (!(D_801808B0[index].flags & 1)) {
            
            result = 1;
            
            D_801808B0[index].cutscenePointer = cutscenePointer;
            
            D_801808B0[index].unk_66 = 0;
            
            D_801808B0[index].offsets.x = 0;
            D_801808B0[index].offsets.y = 0;
            D_801808B0[index].offsets.z = 0;

            D_801808B0[index].unk_58.x = 0;
            D_801808B0[index].unk_58.y = 0;
            D_801808B0[index].unk_58.z = 0;

            D_801808B0[index].flags = 1;
            
        }
    }

    return result;
    
}

//INCLUDE_ASM(const s32, "system/cutscene", func_80046A58);

bool func_80046A58(u16 arg0, Cutscene* arg1) {
    
    bool result = 0;

    if (arg0 < 0x2A) {
        
        if (D_801808B0[arg0].flags & 1) {
            result = 1;
            D_801808B0[arg0].unk_66 = 0;
            D_801808B0[arg0].cutscenePointer = arg1;
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "system/cutscene", func_80046AB0);

//INCLUDE_ASM(const s32, "system/cutscene", func_80046BB8);

void func_80046BB8(void) {
    u16 i;

    for (i = 0; i < 0x2A; i++) {
        func_80046AB0(i);
    }
}

INCLUDE_ASM(const s32, "system/cutscene", func_80046BF8);

INCLUDE_ASM(const s32, "system/cutscene", func_80046C48);

INCLUDE_ASM(const s32, "system/cutscene", func_80046C98);

INCLUDE_ASM(const s32, "system/cutscene", func_80046CF4);

INCLUDE_ASM(const s32, "system/cutscene", func_80046D50);

#ifdef PERMUTER
void func_80046D78(void) {

    u16 i;

    for (i = 0; i < 0x2A; i++) {

        if (D_801808B0[i].flags & 1 && !(D_801808B0[i].flags & 0x20)) {

            if (D_801808B0[i].flags & 8) {
                
                D_801808B0[i].coordinates.x = renderedSprites[D_801808B0[i].unk_64].startingCoordinates.x;
                D_801808B0[i].coordinates.y = renderedSprites[D_801808B0[i].unk_64].startingCoordinates.y;
                D_801808B0[i].coordinates.z = renderedSprites[D_801808B0[i].unk_64].startingCoordinates.z;
                
                if (D_801808B0[i].unk_1C != 0xFFFF && renderedSprites[D_801808B0[i].unk_64].unk_58 == D_801808B0[i].unk_1C && D_801808B0[i].unk_1E == renderedSprites[D_801808B0[i].unk_64].unk_5A) {
                    D_801808B0[i].unk_1C = 0xFFFF;
                    D_801808B0[i].unk_66 = 0;
                    D_801808B0[i].cutscenePointer = D_801808B0[i].sfxIndex;
                
                }
            }
            
            if (D_801808B0[i].flags & 0x10) {
                D_801808B0[i].coordinates.x = gTileContext[D_801808B0[i].unk_64].unk_4.x;
                D_801808B0[i].coordinates.y = gTileContext[D_801808B0[i].unk_64].unk_4.y;
                D_801808B0[i].coordinates.z = gTileContext[D_801808B0[i].unk_64].unk_4.z;
            }

            if (D_801808B0[i].unk_66) {
                 D_801808B0[i].unk_66--;
            }
            
            if (D_801808B0[i].unk_66) {
                goto skip_callback;
            }

            while (!D_801808B0[i].unk_66) {

                D_801808B0[i].offsets.x = 0;
                D_801808B0[i].offsets.y = 0;
                D_801808B0[i].offsets.z = 0;

                D_801133D0[*(D_801808B0[i].cutscenePointer)](i);
                
skip_callback:
                D_801808B0[i].coordinates.x += D_801808B0[i].offsets.x;
                D_801808B0[i].coordinates.y += D_801808B0[i].offsets.y;
                D_801808B0[i].coordinates.z += D_801808B0[i].offsets.z;
        
            }
            
            if (D_801808B0[i].flags & 0x10) {
                func_80047E34(i);
            } 
            
            if (D_801808B0[i].flags & 2) {
                func_800471B0(i);
            }

            if (D_801808B0[i].flags & 8) {
                func_80047640(i);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "system/cutscene", func_80046D78);
#endif

INCLUDE_ASM(const s32, "system/cutscene", func_800471B0);

//INCLUDE_ASM(const s32, "system/cutscene", func_800475B4);

void func_800475B4(u16 arg0) {
    D_801808B0[arg0].unk_68 = 1;
    D_801808B0[arg0].flags &= ~2;
}

//INCLUDE_ASM(const s32, "system/cutscene", func_800475F8);

void func_800475F8(u16 arg0) {

    int temp;
    D_801808B0[arg0].unk_4 += 1;
    temp = D_801808B0[arg0].unk_4;
    D_801808B0[arg0].unk_4 = temp + (*D_801808B0[arg0].unk_4);
    
}
 
INCLUDE_ASM(const s32, "system/cutscene", func_80047640);

INCLUDE_ASM(const s32, "system/cutscene", func_80047E34);

INCLUDE_ASM(const s32, "system/cutscene", func_80047E94);

INCLUDE_ASM(const s32, "system/cutscene", func_80047F20);

INCLUDE_ASM(const s32, "system/cutscene", func_80047F90);

INCLUDE_ASM(const s32, "system/cutscene", func_80048124);

INCLUDE_ASM(const s32, "system/cutscene", func_80048258);

// void func_80048258(u16 arg0) {

//     D_801808B0[arg0].cutscenePointer += 1;
//     D_801808B0[arg0].unk_66 = *(u16*)D_801808B0[arg0].cutscenePointer;
//     D_801808B0[arg0].cutscenePointer++;
// }

//INCLUDE_ASM(const s32, "system/cutscene", func_800482B8);

void func_800482B8(u16 arg0) {
    D_801808B0[arg0].unk_66 = 1;
    func_80046AB0(arg0);
}


#ifdef PERMUTER
void func_800482F8(u16 arg0) {

    s32 temp;

    temp = D_801808B0[arg0].cutscenePointer;
    D_801808B0[arg0].cutscenePointer++;
    D_801808B0[arg0].unk_8 = temp+4;
    D_801808B0[arg0].cutscenePointer = temp + *(D_801808B0[arg0].cutscenePointer);
    
}
#else
INCLUDE_ASM(const s32, "system/cutscene", func_800482F8);
#endif

INCLUDE_ASM(const s32, "system/cutscene", func_8004835C);

// void func_8004835C(u16 arg0) {
//     D_801808B0[arg0].cutscenePointer = D_801808B0[arg0].unk_8;
// }

INCLUDE_ASM(const s32, "system/cutscene", func_8004838C);

INCLUDE_ASM(const s32, "system/cutscene", func_80048430);

INCLUDE_ASM(const s32, "system/cutscene", func_800484D4);

INCLUDE_ASM(const s32, "system/cutscene", func_80048578);

INCLUDE_ASM(const s32, "system/cutscene", func_800485EC);


#ifdef PERMUTER
void func_80048694(u16 arg0) {

    s16 *temp = D_801808B0[arg0].cutscenePointer; 
    D_801808B0[arg0].cutscenePointer += 1;
    D_801808B0[arg0].cutscenePointer = (u16)*(temp+1) + 4;
    func_80046AB0(D_801808B0[arg0].cutscenePointer);
    
}
#else
INCLUDE_ASM(const s32, "system/cutscene", func_80048694);
#endif 

INCLUDE_ASM(const s32, "system/cutscene", func_800486F4);

INCLUDE_ASM(const s32, "system/cutscene", func_800488CC);

INCLUDE_ASM(const s32, "system/cutscene", func_80048B08);

INCLUDE_ASM(const s32, "system/cutscene", func_80048B90);

INCLUDE_ASM(const s32, "system/cutscene", func_80048BEC);

INCLUDE_ASM(const s32, "system/cutscene", func_80048C48);

INCLUDE_ASM(const s32, "system/cutscene", func_80048CA4);

INCLUDE_ASM(const s32, "system/cutscene", func_80048DA8);

INCLUDE_ASM(const s32, "system/cutscene", func_80048E98);

// set bit on cutscene flags
INCLUDE_ASM(const s32, "system/cutscene", func_80048F88);

// toggle bit
INCLUDE_ASM(const s32, "system/cutscene", func_80048FF4);

INCLUDE_ASM(const s32, "system/cutscene", func_80049064);

INCLUDE_ASM(const s32, "system/cutscene", func_8004910C);

INCLUDE_ASM(const s32, "system/cutscene", func_80049228);

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

INCLUDE_ASM(const s32, "system/cutscene", func_8004A000);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A0A8);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A0F4);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A140);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A1DC);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A234);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A320);

INCLUDE_ASM(const s32, "system/cutscene", func_8004A38C);

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

INCLUDE_ASM(const s32, "system/cutscene", func_8004B0E8);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B2FC);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B410);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B498);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B538);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B5F0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B700);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B7B8);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B920);

INCLUDE_ASM(const s32, "system/cutscene", func_8004B9A0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BA34);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BB84);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BD00);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BD6C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BE14);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BE88);

INCLUDE_ASM(const s32, "system/cutscene", func_8004BFC4);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C048);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C0D0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C148);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C258);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C34C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C3D0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C454);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C4E0);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C56C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C5D8);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C6CC);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C770);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C7E4);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C870);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C904);

INCLUDE_ASM(const s32, "system/cutscene", func_8004C994);

INCLUDE_ASM(const s32, "system/cutscene", func_8004CA80);

INCLUDE_ASM(const s32, "system/cutscene", func_8004CB1C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004CB88);

INCLUDE_ASM(const s32, "system/cutscene", func_8004CC3C);

INCLUDE_ASM(const s32, "system/cutscene", func_8004CCF0);
