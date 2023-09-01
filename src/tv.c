#include "common.h"

#include "system/controller.h"
#include "system/map.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/mathUtils.h"

#include "game.h"
#include "gameAudio.h"
#include "overlayScreens.h"
#include "weather.h"
       
// forward declarations   
void func_800D879C();                                  
void func_800D897C();       
void func_800D9298();            
void func_800D8540();      

// struct
// likely bss
extern u16 D_80170270;
extern u8 D_80170272[1];
extern u8 D_80170273;
extern u8 D_80170274[1];
extern u8 D_80170275;
extern u8 D_80170277;

// rodata
extern void *tvSprites_romTextureStart;
extern void *tvSprites_romTextureEnd;
extern void *tvSprites_romPaletteStart;
extern void *tvSprites_romPaletteEnd;
extern void *tvSprites_romIndexStart;
extern void *tvSprites_romIndexEnd;

// convert to array?
extern u8 D_80170276;
extern u8 D_80170277;
extern u8 D_80170278;
extern u8 D_80170279;
extern u8 D_8017027A;
extern u8 D_8017027B;
extern u8 D_8017027C;


//INCLUDE_ASM(const s32, "tv", func_800D8540);

void func_800D8540(void) {

    func_800D879C();
    func_800D897C();
    func_800D9298();
    func_80034E64(0, 9);

    func_8002B138(0x6A, &tvSprites_romTextureStart, &tvSprites_romTextureEnd, &tvSprites_romPaletteStart, &tvSprites_romPaletteEnd, &tvSprites_romIndexStart, &tvSprites_romIndexEnd, 0x802EB800, 0x802EC800, 0x802ED800, 0x802EDB00, 0x802EDE00, 0x802EDF00, 1, 1);
    
    func_8002BD90(0x6A, 2.0f, 2.0f, 1.0f);
    func_8002BE14(0x6A, 45.0f, -45.0f, 0.0f);
    func_8002C7EC(0x6A, 3);
    func_8002C914(0x6A, 0xFF, 0xFF, 0xFF, 0xFF);

    func_80034C40(0, 9, 0x6A, D_80170273, -12.0f, 36.0f, -154.0f, 0xFF, 0xFE, 0, 0);

    D_80170274[0] = 0;
}

// jtbl_80122760
//INCLUDE_ASM(const s32, "tv", func_800D86D8);

void func_800D86D8(void) {
    
    u8 *ptr;
    u8 temp;
    
    switch (gDayOfWeek) {
        case TUESDAY:
            ptr = &D_80170276;
            break;
        
        case WEDNESDAY:
            ptr = &D_80170277;
            break;
        
        case THURSDAY:
            ptr = &D_80170278;
            break;
        
        case FRIDAY:
            ptr = &D_80170279;
            break;

        case SATURDAY:
            ptr = &D_8017027A;
            break;
        
        case SUNDAY:
            ptr = &D_8017027B;
            temp = *ptr+1;
            *ptr = temp;
            if (!(temp < 20)) {
                *ptr = 0;
            }
            return;
        
        case MONDAY:
            ptr = &D_8017027C;
            break;

        default:
            return;
    }

    temp = *ptr+1;
    *ptr = temp;
    
check:
    if (!(temp < 10)) {
        *ptr = 0;
    }
}

// jtbl_80122780
//INCLUDE_ASM(const s32, "tv", func_800D879C);

void func_800D879C(void) {

    D_80170272[0] = 0xFF;
    
    if (gHour < 6) {
        D_80170272[0] = 0;
        return;
    }

    if (gWeather != TYPHOON) {
        switch (D_80170275) {
            case 0:
                switch (gForecast) {
                    case SUNNY:
                        D_80170272[0] = 1;
                        return;
                    case RAIN:
                        D_80170272[0] = 2;
                        break;
                    case SNOW:
                        D_80170272[0] = 3;
                        break;
                    case TYPHOON:
                        D_80170272[0] = 4;
                        break;
                }
                break;
            case 1:
                switch (gSeason) {
                    case SPRING:
                        D_80170272[0] = 5;
                        return;
                    case SUMMER:
                        D_80170272[0] = 6;
                        return;
                    case AUTUMN:
                        D_80170272[0] = 7;
                        return;
                    case WINTER:
                        D_80170272[0] = 8;
                        return;
                }
                break;
            case 2:
                switch (gDayOfWeek) {
                    case SUNDAY:
                       switch (getRandomNumberInRange(0, 2)) {
                           case 0:
                               D_80170272[0] = 9;
                               return;
                           case 1:
                               D_80170272[0] = 10;
                               return;
                           case 2:    
                               break;
                           default:
                               return;
                       } 
                        break;
                    case MONDAY:
                    case TUESDAY:
                        D_80170272[0] = 9;
                        return;
                    case WEDNESDAY:
                    case THURSDAY:
                       D_80170272[0] = 10;
                       return;
                    case FRIDAY:
                    case SATURDAY:
                        break;
                    default:
                        return;
                }
                
                D_80170272[0] = 11;
                break;
            
            case 3:
                D_80170272[0] = 0x13;
                break;
        }
    } else {
        D_80170272[0] = 19;
    }
}

// jtbl_801227A0
// jtbl_801227F0
INCLUDE_ASM(const s32, "tv", func_800D897C);

// jtbl_80122808
//INCLUDE_ASM(const s32, "tv", func_800D9298);

void func_800D9298(void) {

    switch (D_80170272[0]) {
        case 0:
            D_80170273 = 1;
            break;
        case 19:
            D_80170273 = 0;
            break;
        case 1:
            D_80170273 = 3;
            break;
        case 2:
            D_80170273 = 4;
            break;
        case 3:
            D_80170273 = 5;
            break;
        case 4:
            D_80170273 = 6;
            break;
        case 5:
            D_80170273 = 7;
            break;
        case 6:
            D_80170273 = 8;
            break;
        case 7:
            D_80170273 = 9;
            break;
        case 8:
            D_80170273 = 10;
            break;
        case 9:
            D_80170273 = 2;
            break;
        case 10:
            D_80170273 = 2;
            break;
        case 11:
            D_80170273 = 2;
            break;
        case 12:
            D_80170273 = 11;
            break;
        case 13:
            if (D_80170277 == 3 || D_80170277 == 6) {
                D_80170273 = 13;
                break;
            }
            if (D_80170277 == 7) {
                D_80170273 = 13;
                break;
            }
            D_80170273 = 12;
            break;
        case 14:
            D_80170273 = 14;
            break;        
        case 15:
            D_80170273 = 15;
            break;
        case 16:
            if (!getRandomNumberInRange(0, 1)) {
                D_80170273 = 17;
                break;
            }
            D_80170273 = 16;
            break;
        case 17:
            D_80170273 = 18;
            break;
        case 18:
            D_80170273 = 19;
            break;
        default:
            break;
    }
}

INCLUDE_ASM(const s32, "tv", func_800D93CC);
