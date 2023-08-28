#include "common.h"
       
u8 func_8002B6B8(u16);                                 
u8 func_8002B80C(u16, u16, u8);                           
u8 func_8002BAD8(u16);                                    
u8 func_8002BD0C(u16 index, f32 x, f32 y, f32 z);       
u8 func_8002BE98(u16, f32, f32, f32);                        
u8 func_8002C1C0(u16, u8, u8, u8, s32, s32);              
u8 func_8002CBF8(u16);                               
u32 func_8004D380(u8, u32);                            
u32 func_8004D3C8(u8, u32);                            
void func_80055F08(u16, u8, u8);                           
void func_800ACB5C(u16);                                 
u8 func_800ACBB8(u8);                               
void func_800ACC50(u16);                                 
void func_800E0ED4();                                  
void func_800E0FEC();                                  
void func_800E10E8();                                  
void func_800E1380(u8);            

extern f32 D_80126578;
extern f32 D_80200AC4;
extern f32 D_80200B60;
extern f32 D_80200BFC;
extern f32 D_80200C98;
extern f32 D_80200D34;
extern f32 D_80200DD0;

extern u8 D_80126570;
extern u8 D_80126571;
extern u16 D_80126574;
extern u32 D_8012657C;
extern u8 D_801C3F50;

              

INCLUDE_ASM(const s32, "title", func_800DE3C0);

INCLUDE_ASM(const s32, "title", func_800DFF64);

// jtbl_80123658
//INCLUDE_ASM(const s32, "title", func_800E00A4);

// main loop callback
void func_800E00A4(void) {

    u8 set = 0;
    
    switch (D_80126570) {
        case 0:
            D_8012657C++;
            if (D_8012657C == 0x1F4) {
                func_8002C1C0(0x42, 0, 0, 0, 0, 8);
                func_8002C1C0(0x43, 0, 0, 0, 0, 8);
                func_8002C1C0(0x44, 0, 0, 0, 0, 8);
                func_8002C1C0(0x45, 0, 0, 0, 0, 8);
                func_8002C1C0(0x46, 0, 0, 0, 0, 8);
                func_8002C1C0(0x47, 0, 0, 0, 0, 8);
                func_8002C1C0(0x48, 0, 0, 0, 0, 8);
                func_8002C1C0(0x49, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4A, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4B, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4C, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4D, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4E, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4F, 0, 0, 0, 0, 8);
                func_8002C1C0(0x50, 0, 0, 0, 0, 8);
                func_8002C1C0(0x51, 0, 0, 0, 0, 8);
                func_8002C1C0(0x52, 0, 0, 0, 0, 8);
                func_8002C1C0(0x53, 0, 0, 0, 0, 8);
                func_8002C1C0(0x54, 0, 0, 0, 0, 8);
                func_8002C1C0(0x55, 0, 0, 0, 0, 8);
                func_8002C1C0(0x56, 0, 0, 0, 0, 8);
                func_8002C1C0(0x57, 0, 0, 0, 0, 8);
                func_8002C1C0(0x58, 0, 0, 0, 0, 8);
                func_8002C1C0(0x59, 0, 0, 0, 0, 8);
                func_8002C1C0(0x5A, 0, 0, 0, 0, 8);
                func_8002C1C0(0x5B, 0, 0, 0, 0, 8);
                func_800ACB5C(1);
                set = 1;
                D_80126570 = 5;
            }
            if (func_8004D380(0, 0x1000) && !set) {
                func_8002BAD8(0x46);
                func_8002B80C(0x46, 4, 0xFF);
                func_8002BAD8(0x44);
                func_8002BAD8(0x45);
                func_8002B80C(0x44, 8, 0xFE);
                D_80126570 = 1;
                func_800ACC50(0);
            }
            func_800E0ED4();
            break;
        
        case 1:
            if (D_80126574 < 0x40) {
                func_8002BE98(0x46, 5.0f, 0.0f, 0.0f);
                func_8002BE98(0x47, 5.0f, 0, 0);
                func_8002BE98(0x48, 5.0f, 0, 0);
                func_8002BE98(0x49, 5.0f, 0, 0);
                func_8002BE98(0x4A, 5.0f, 0, 0);
                func_8002BE98(0x4B, 5.0f, 0, 0);
                func_8002BE98(0x52, 5.0f, 0, 0);
                func_8002BE98(0x53, 5.0f, 0, 0);
                func_8002BE98(0x4E, 2.0f, 0, 0);
                func_8002BE98(0x4D, 3.0f, 0, 0);
                func_8002BE98(0x4C, 5.0f, 0, 0);
                func_8002BE98(0x51, 2.0f, 0, 0);
                func_8002BE98(0x50, 3.0f, 0, 0);
                func_8002BE98(0x4F, 5.0f, 0, 0);
                func_8002BE98(0x54, 1.0f, 0, 0);
                func_8002BE98(0x55, 1.0f, 0, 0);
                func_8002BE98(0x56, 1.0f, 0, 0);
                func_8002BE98(0x57, 1.0f, 0, 0);
                func_8002BE98(0x58, 1.0f, 0, 0);
                func_8002BE98(0x59, 1.0f, 0, 0);
                func_8002BE98(0x5A, 1.0f, 0, 0);
                func_8002BE98(0x5B, 1.0f, 0, 0);
            } else {
                D_80126570 = 2;
                D_80126574 = 0;
            }
            D_80126574 += 1;
            break;
        
        case 2:
            func_8002BAD8(0x44);
            func_8002B80C(0x44, 0x1D, 0xFE);
            func_8002BD0C(0x45, -56.0f, -20.0f, 56.0f);
            func_8002B80C(0x45, 3, 0xFE);
            D_80126570 = 3;
            D_80126571 = 0;
            break;
        
        case 3:
            if (func_8004D3C8(0, 0x10000)) {         
                if (!set) {
                    if (D_80126571 != 1) {
                        func_8002BD0C(0x45, -56.0f, -52.0f, 56.0f);
                        D_80126571 = 1;
                        func_800ACC50(2);
                    }
                    set = 1;
                }
            }
            
            if (func_8004D3C8(0, 0x100000)) {
                if (!set) {
                    if (D_80126571) {
                        func_8002BD0C(0x45, -56.0f, -20.0f, 56.0f);
                        D_80126571 = 0;
                        func_800ACC50(2);
                    }
                    set = 1;
                }
            }
            
            if (func_8004D380(0, 0x9000) && !set) {
                
                if (!D_80126571) {
                    func_8002BAD8(0x47);
                    func_8002B80C(0x47, 6, 0xFF);
                } else {
                    func_8002BAD8(0x48);
                    func_8002B80C(0x48, 8, 0xFF);
                }
                  
                func_8002C1C0(0x42, 0, 0, 0, 0, 8);
                func_8002C1C0(0x43, 0, 0, 0, 0, 8);
                func_8002C1C0(0x44, 0, 0, 0, 0, 8);
                func_8002C1C0(0x45, 0, 0, 0, 0, 8);
                func_8002C1C0(0x46, 0, 0, 0, 0, 8);
                func_8002C1C0(0x47, 0, 0, 0, 0, 8);
                func_8002C1C0(0x48, 0, 0, 0, 0, 8);
                func_8002C1C0(0x49, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4A, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4B, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4C, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4D, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4E, 0, 0, 0, 0, 8);
                func_8002C1C0(0x4F, 0, 0, 0, 0, 8);
                func_8002C1C0(0x50, 0, 0, 0, 0, 8);
                func_8002C1C0(0x51, 0, 0, 0, 0, 8);
                func_8002C1C0(0x52, 0, 0, 0, 0, 8);
                func_8002C1C0(0x53, 0, 0, 0, 0, 8);
                func_8002C1C0(0x54, 0, 0, 0, 0, 8);
                func_8002C1C0(0x55, 0, 0, 0, 0, 8);
                func_8002C1C0(0x56, 0, 0, 0, 0, 8);
                func_8002C1C0(0x57, 0, 0, 0, 0, 8);
                func_8002C1C0(0x58, 0, 0, 0, 0, 8);
                func_8002C1C0(0x59, 0, 0, 0, 0, 8);
                func_8002C1C0(0x5A, 0, 0, 0, 0, 8);
                func_8002C1C0(0x5B, 0, 0, 0, 0, 8);
                D_80126570 = 4;
                func_800ACC50(0);
            }

        func_800E0ED4();
        break;

        case 4:
            
            // check sprite flag
            if (func_8002CBF8(0x42)) {

                // toggle sprite flags
                func_8002B6B8(0x42);
                func_8002B6B8(0x43);
                func_8002B6B8(0x44);
                func_8002B6B8(0x45);
                func_8002B6B8(0x46);
                func_8002B6B8(0x47);
                func_8002B6B8(0x48);
                func_8002B6B8(0x49);
                func_8002B6B8(0x4A);
                func_8002B6B8(0x4B);
                func_8002B6B8(0x4C);
                func_8002B6B8(0x4D);
                func_8002B6B8(0x4E);
                func_8002B6B8(0x4F);
                func_8002B6B8(0x50);
                func_8002B6B8(0x51);
                func_8002B6B8(0x52);
                func_8002B6B8(0x53);
                func_8002B6B8(0x54);
                func_8002B6B8(0x55);
                func_8002B6B8(0x56);
                func_8002B6B8(0x57);
                func_8002B6B8(0x58);
                func_8002B6B8(0x59);
                func_8002B6B8(0x5A);
                func_8002B6B8(0x5B);
                
                // row index
                if (!D_80126571) {
                    // load select game screen
                    func_800E1380(0);
                } else {
                    func_80055F08(0x640, 0, 0);
                }
            }

            break;

        case 5:
            
            if (func_8002CBF8(0x42) && func_800ACBB8(1)) {
                
                // reset sprite flags
                func_8002B6B8(0x42);
                func_8002B6B8(0x43);
                func_8002B6B8(0x44);
                func_8002B6B8(0x45);
                func_8002B6B8(0x46);
                func_8002B6B8(0x47);
                func_8002B6B8(0x48);
                func_8002B6B8(0x49);
                func_8002B6B8(0x4A);
                func_8002B6B8(0x4B);
                func_8002B6B8(0x4C);
                func_8002B6B8(0x4D);
                func_8002B6B8(0x4E);
                func_8002B6B8(0x4F);
                func_8002B6B8(0x50);
                func_8002B6B8(0x51);
                func_8002B6B8(0x52);
                func_8002B6B8(0x53);
                func_8002B6B8(0x54);
                func_8002B6B8(0x55);
                func_8002B6B8(0x56);
                func_8002B6B8(0x57);
                func_8002B6B8(0x58);
                func_8002B6B8(0x59);
                func_8002B6B8(0x5A);
                func_8002B6B8(0x5B);

                // demo cutscene index
                switch (D_801C3F50) {
                    case 0:
                        func_80055F08(0x5DC, 0, 1);
                        break;
                    case 1:
                        func_80055F08(0x5DD, 0x1D, 1);
                        break;
                    case 2:
                        func_80055F08(0x5DF, 0x32, 1);
                        break;
                    default:
                        break;
                }

                D_801C3F50++;

                if (D_801C3F50 > 2) {
                    D_801C3F50 = 0;
                }   
            }
        }  
            
    func_800E0FEC();
    func_800E10E8();
}


// jtbl_80123670
INCLUDE_ASM(const s32, "title", func_800E0ED4);

INCLUDE_ASM(const s32, "title", func_800E0FEC);

//INCLUDE_ASM(const s32, "title", func_800E10E8);

void func_800E10E8(void) {

    if (D_80200AC4 > 320.0f) {
        func_8002BD0C(0x56, D_80126578 - 320.0f, 64.0f, 64.0f);
    }

    if (D_80200B60 > 320.0f) {
        func_8002BD0C(0x57, D_80126578 - 320.0f, 96.0f, 64.0f);
    }

    if (D_80200BFC > 320.0f) {
        func_8002BD0C(0x58, D_80126578 - 320.0f, 80.0f, 64.0f);
    }

    if (D_80200C98 > 320.0f) {
        func_8002BD0C(0x59, D_80126578 - 320.0f, 72.0f, 64.0f);
    }

    if (D_80200D34 > 320.0f) {
        func_8002BD0C(0x5A, D_80126578 - 320.0f, 108.f, 64.0f);
    }

    if (D_80200DD0 > 320.0f) {
        func_8002BD0C(0x5B, D_80126578 - 320.0f, 88.0f, 64.0f);
    }

    func_8002BE98(0x56, 0.2f, 0.0f, 0.0f);
    func_8002BE98(0x57, 0.1f, 0, 0);
    func_8002BE98(0x58, 0.3f, 0, 0);
    func_8002BE98(0x59, 0.4f, 0, 0);
    func_8002BE98(0x5A, 0.1f, 0, 0);
    func_8002BE98(0x5B, 0.2f, 0, 0);
    
}
