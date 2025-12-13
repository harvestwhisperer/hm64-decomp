#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include "common.h"

#include "system/audio.h"

#define OPENING 0
#define TITLE_THEME 1
#define SPORTS_FESTIVAL 2
#define GOODBYE 3
#define SPRING_THEME 4
#define SUMMER_THEME 5
#define AUTUMN_THEME 6
#define WINTER_THEME 7
#define NAMING_SCREEN_THEME 8
#define FESTIVAL_THEME 9
#define SPORTS_FESTIVAL_THEME_2 10
#define SPIRIT_FESTIVAL_THEME 11
#define DOG_RACE_THEME 12
#define RACING_FESTIVAL_THEME 13
#define FESTIVAL_DANCE_THEME 14
#define MOUNTAIN_THEME 15
#define VILLAGE_THEME 16
#define POND_THEME 17
#define CHURCH_THEME 18 
#define TAVERN_THEME 19
#define WEDDING_THEME 20
#define NEW_YEAR_SUNRISE_THEME 21
#define CREDITS_THEME 22
#define BROKEN_MUSIC_BOX_THEME 23
#define MUSIC_BOX_THEME 24
#define FLUTE_PERFORMANCE 25
#define ORGAN_PERFORMANCE 26
#define OCARINA_PERFORMANCE 27
#define SLEIGH_BELLS 28
#define BAD_ENDING_THEME 29
#define VOCAL_PERFORMANCE 30
#define ENSEMBLE_PERFORMANCE 31
#define DRUM_PERFORMANCE 32
#define TYPHOON_AMBIENCE_1 33
#define TYPHOON_AMBIENCE_2 34
#define NIGHT_AMBIENCE_SPRING 35
#define NIGHT_AMBIENCE_SUMMER 36
#define NIGHT_AMBIENCE_AUTUMN 37
#define STARRY_NIGHT_FESTIVAL_THEME 38
#define BEACH_AMBIENCE_1 39
#define BEACH_AMBIENCE_2 40

// TODO: finish adding sfx
#define SELECT_1 0
#define CLOSE 1
#define MOVE_CURSOR 2
#define INVALID_BUZZ 3
// shipping bin
#define ITEM_PLUCK_SFX 4
#define WALKING_SFX 5
// 5 and 6 = gallop
#define SELECT_2 7
// pause screen, dialogue box
#define MENU_OPEN_SFX 8
#define WHISTLE 10
#define SICKLE_SFX 11
#define SILVER_SICKLE_SFX 12
#define GOLDEN_SICKLE_SFX 13
#define HOE_SFX 14
#define AX_SFX 17
#define SILVER_AX_SFX 18
#define GOLDEN_AX_SFX 19
#define HAMMER_SFX 20
#define SILVER_HAMMER_SFX 21
#define GOLDEN_HAMMER_SFX 22
#define WATERING_CAN_SFX 23
#define SILVER_WATERING_CAN_SFX 24
#define GOLDEN_WATERING_CAN_SFX 25
#define SEEDS_SFX 29
#define MILKER_SFX 30
#define RAIN_SFX 32
#define TYPHOON_SFX 34
#define PICKING_UP_SFX 36
#define TV_OFF_SFX 47
#define DOOR_OPEN_SFX 50
// 0x3F set by func_80056030
#define ROOSTER 63
#define CHICKEN_CLUCK 64
#define BIRD_CHIRP 66
#define CAT_MEOW 67
#define BIRD_CHIRP_2 70
#define DIALOGUE_BEEP 87
#define RUMBLE 89
#define TOOL_ACQUISITION_FANFARE 90


extern void initializeWaveTable(u16 waveTableIndex);
extern void setCurrentAudioSequence(u16 sequenceIndex);
extern void stopCurrentAudioSequence(u16 sequenceIndex);
extern void stopAudioSequenceWithDefaultFadeOut(u16 sequenceIndex);
extern u8 checkDefaultSequenceChannelOpen(u16 sequenceIndex);
extern void setAudioSequenceVolume(u16 sequenceIndex, u32 arg1);
extern void func_800ACC1C(u16 sequenceIndex);
extern void playSfx(u16 index);
extern u8 checkAllSfxInactive(void);

#endif