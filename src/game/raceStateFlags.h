#ifndef _RACE_STATE_FLAGS_H_
#define _RACE_STATE_FLAGS_H_

#define RACE_STATE_BIT_PLAYER_PLACED          0  // resolver finished and player participated (gates post-race NPC dialogue in horseRace)
#define RACE_STATE_BIT_PLAYER_WON             1  // player's mount/dog placed 1st
#define RACE_STATE_BIT_RACE_FINALIZED         2  // race-final cleanup ran (post-race NPC dialogue gate)
#define RACE_STATE_BIT_DOG2_WON               3  // dogRace only: dog 2 placed 1st (with bit 6 = "Taro won")
#define RACE_STATE_BIT_BETWEEN_RACES_SHOWN    4  // announcer's between-races one-shot already played
#define RACE_STATE_BIT_PLAYER_IN_RACE         5  // playerRaceNumber == currentRaceIndex
#define RACE_STATE_BIT_RIVAL_IN_RACE          6  // rivalEntryRaceIndex == currentRaceIndex
#define RACE_STATE_BIT_PLAYER_CEREMONY_DONE  10  // horseRace only: player-race winner ceremony completed

#define RACE_STATE_PLAYER_IN_RACE  0x20  // (1 << RACE_STATE_BIT_PLAYER_IN_RACE)
#define RACE_STATE_RIVAL_IN_RACE   0x40  // (1 << RACE_STATE_BIT_RIVAL_IN_RACE)

#endif
