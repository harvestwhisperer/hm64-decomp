#ifndef _SET_CUTSCENES_H_
#define _SET_CUTSCENES_H_

#define DAYTIME (gHour - 6) < 12U
#define NIGHTTIME (gHour - 18) < 6U
#define NOT_WINTER (gSeason - 2) < 2U
#define RAINING (gWeather - 2) < 2U

#define MARRIED_TO_POPURI checkLifeEventBit(MARRIED) && gWife == POPURI
#define MARRIED_TO_ANN checkLifeEventBit(MARRIED) && gWife == ANN
#define MARRIED_TO_KAREN checkLifeEventBit(MARRIED) && gWife == KAREN
#define ARE_MARRIED checkLifeEventBit(MARRIED)
#define NOT_MARRIED !checkLifeEventBit(MARRIED)

/* cutscene indices */
#define MOUNTAIN_FIREWORKS 0x130
#define ELLI_SPRAINED_ANKLE 0x157
#define ANN_AND_RICK 0x1A6
#define STARRY_NIGHT_FESTIVAL 0x1AC 
#define CHURCH_NEW_YEARS 0x1B3
#define POPURI_SICK 0x1B9
#define KAREN_YELLOW_HEART_EVENT_1 0x2C1
#define KAREN_YELLOW_HEART_EVENT_4 0x2C4
#define KAREN_SICK 0x2C7
#define BEACH_FIREWORKS 0x2EE
#define KAREN_AND_ELLI 0x2F2
#define KAREN_PINK_HEART_EVENT 0x2F3
#define SHADY_SALESMAN_AND_MARIA_1 0x322
#define SHADY_SALESMAN_AND_MARIA_2 0x323
#define ANN_SPRAINED_ANKLE 0x324
#define FIREFLY_FESTIVAL 0x41A
#define SEA_FESTIVAL 0x44C
#define INTRO 0x5B0

#endif