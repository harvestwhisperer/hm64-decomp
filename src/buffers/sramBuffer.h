#ifndef _SRAM_BUFFER_H_
#define _SRAM_BUFFER_H_

#define SRAM_BUFFER 0x8030D000

typedef struct {
    // Save file signature "PACKINSOFT FARM2"
    u8 signature[16];

    u8 year;                    // 0x10
    u8 season;                  // 0x11
    u8 dayOfWeek;               // 0x12
    u8 dayOfMonth;              // 0x13

    u8 playerName[6];           // 0x14-0x19
    u8 farmName[6];             // 0x1A-0x1F

    // 0x020: Currency and resources
    u32 gold;                   // 0x20-0x23
    u8 weather;                 // 0x24
    u8 forecast;                // 0x25
    u8 playerBirthdaySeason;    // 0x26
    u8 wateringCanUses;         // 0x27
    u16 lumber;                 // 0x28-0x29
    u16 fodderQuantity;         // 0x2A-0x2B
    u8 seasonTomorrow;          // 0x2C

    u8 unk_2D;                  // D_801FC154
    u8 unk_2E;                  // D_80204DF4
    u8 unk_2F;                  // D_80237458
    u8 unk_30;                  // D_8018A725
    u8 unk_31;                  // D_802373E8
    u8 unk_32;                  // D_8013DC2C
    u8 unk_33;                  // D_801FAD91
    u8 unk_34;                  // D_801C3E28
    u8 unk_35;                  // D_801C3F70
    u8 unk_36;                  // D_80205636
    u8 unk_37;                  // D_802373A8

    u8 maximumStamina;          // 0x38
    u8 currentStamina;          // 0x39
    u8 fatigueCounter;          // 0x3A
    u8 staminaExhaustionLevel;  // 0x3B

    u8 alcoholTolerance;        // 0x3C
    u8 houseExtensionSelection; // 0x3D
    u8 houseExtensionConstructionCounter; // 0x3E
    u8 unk_3F;
    u16 chickenFeedQuantity;    // 0x40-0x41
    u8 unk_42;                  // D_8021E6D0

    u8 harvestKing;             // 0x43
    u8 harvestCoinFinder;       // 0x44
    u8 flowerFestivalGoddess;   // 0x45
    u8 cowFestivalStall;        // 0x46 (D_80189054)
    u8 voteForFlowerFestivalGoddess; // 0x47

    u8 deadAnimalName[6];       // 0x48-0x4D

    u8 happiness;               // 0x4E
    u8 blueMistFlowerPlot;      // 0x4F
    u8 anniversarySeason;       // 0x50
    u8 anniversaryDate;         // 0x51
    u8 babyBirthdaySeason;      // 0x52
    u8 babyBirthdayDate;        // 0x53

    u32 powerNutBits;           // 0x54-0x57
    u32 unk_58;                 // 0x58-0x5B (D_80205204)
    u8 flowerShopPoints;        // 0x5C
    u8 bakeryCardPoints;        // 0x5D
    u8 elliGrievingCounter;    // 0x5E
    u8 unk_5F;

    u32 unk_60;                 // D_801654F4
    u32 unk_64;                 // D_80237414
    u32 unk_68;                 // D_801FB6FC
    u32 unk_6C;                 // D_801C3F80
    u32 unk_70;                 // D_801806C0
    u32 unk_74;                 // D_80188F60
    u32 unk_78;                 // D_801FB5D0

    u32 totalMilkShipped;       // 0x7C-0x7F
    u32 totalEggsShipped;       // 0x80-0x83
    u32 totalCropsShipped;      // 0x84-0x87
    u32 totalFishCaught;        // 0x88-0x8B

    u8 babyName[6];             // 0x8C-0x91

    u16 sickDays;               // 0x92-0x93
    u16 deadAnimalCount;        // 0x94-0x95 (D_8013DC2E)

    u8 mrsManaCow1Index;        // 0x96
    u8 mrsManaCow2Index;        // 0x97
    u8 mrsManaCow3Index;        // 0x98

    u8 padding_99[0x27];        // 0x99-0xBF

    u16 toolUseCounters[5];     // 0xC0-0xC9
    u8 toolLevels[5];           // 0xCA-0xCE
    u8 bottleContents;          // 0xCF

    u8 tvChannelIndex;          // 0xD0
    u8 varietyShowEpisodeCounters[7]; // 0xD1-0xD7

    u32 recipesBits;            // 0xD8-0xDB
    u32 unk_DC;                 // 0xDC-0xDF (D_8016FB00)
    u32 albumBits;              // 0xE0-0xE3

    u8 bornChickenIndex;        // 0xE4
    u8 bornAnimalIndex;         // 0xE5

    u8 wife;                    // 0xE6
    u8 babyAge;                 // 0xE7
    u8 wifeConceptionCounter;   // 0xE8
    u8 wifePregnancyCounter;    // 0xE9
    u8 mariaHarrisBabyAge;      // 0xEA
    u8 mariaHarrisNewlywedCounter; // 0xEB
    u8 mariaHarrisPregnancyCounter; // 0xEC
    u8 popuriGrayBabyAge;       // 0xED
    u8 popuriGrayNewlywedCounter; // 0xEE
    u8 popuriGrayPregnancyCounter; // 0xEF
    u8 elliJeffBabyAge;         // 0xF0
    u8 elliJeffNewlywedCounter; // 0xF1
    u8 elliJeffPregnancyCounter; // 0xF2
    u8 annCliffBabyAge;         // 0xF3
    u8 annCliffNewlywedCounter; // 0xF4
    u8 annPregnancyCounter;     // 0xF5
    u8 karenKaiBabyAge;         // 0xF6
    u8 karenKaiNewlywedCounter; // 0xF7
    u8 karenPregnancyCounter;   // 0xF8

    u8 padding_F9[3];           // 0xF9-0xFB

    u32 dailyShippingBinValue;  // 0xFC-0xFF
    u32 readMailBits;           // 0x100-0x103
    u32 unk_104;                // 0x104-0x107 (D_801C3F3C)
    u32 unk_108;                // 0x108-0x10B (D_801C3F40)
    u32 mailboxBits;            // 0x10C-0x10F
    u32 unk_110;                // 0x110-0x113 (D_8016FFF0)
    u32 unk_114;                // 0x114-0x117 (D_8016FFF4)

    u8 padding_118[0x268];      // 0x118-0x37F

    u8 toolboxSlots[32];      // 0x380-0x39F
    u8 unk_3A0[32];             // 0x3A0-0x3BF (D_80237420)
    u8 unk_3C0[32];             // 0x3C0-0x3DF (D_801890E8)

    u8 padding_3E0[0x20];       // 0x3E0-0x3FF

    u8 belongingsSlots[9];      // 0x400-0x408
    u8 padding_409[7];          // 0x409-0x40F
    u8 toolSlots[9];            // 0x410-0x418
    u8 padding_419[7];          // 0x419-0x41F
    u8 keyItemSlots[24];        // 0x420-0x437

    u8 padding_438[0x48];       // 0x438-0x47F

    u8 npcAffection[48];        // 0x480-0x4AF (MAX_NPCS)

    u8 padding_4B0[0x10];       // 0x4B0-0x4BF

    // 0x4C0: (D_80189108[5][7])
    u8 D_80189108[35];     // 0x4C0-0x4E2

    u8 padding_4E3[0x1D];       // 0x4E3-0x4FF

    u8 dogName[6];              // 0x500-0x505
    u8 dogAffection;            // 0x506
    u8 dogLocation;             // 0x507
    f32 dogCoordX;              // 0x508-0x50B
    f32 dogCoordZ;              // 0x50C-0x50F
    u8 dogUnk_1C;               // 0x510

    u8 padding_511[0x6F];       // 0x511-0x57F

    u8 horseName[6];            // 0x580-0x585
    u8 horseAffection;          // 0x586
    u8 horseLocation;           // 0x587
    f32 horseCoordX;            // 0x588-0x58B
    f32 horseCoordZ;            // 0x58C-0x58F
    u8 horseGrown;              // 0x590
    u8 horseAge;                // 0x591
    u8 horseUnk_1E;             // 0x592
    u8 padding_593;             // 0x593
    u16 horseFlags;             // 0x594-0x595

    u8 padding_596[0x6A];       // 0x596-0x5FF

    struct {
        u8 name[6];             // 0x00-0x05
        u8 affection;           // 0x06 (unused for chickens but same layout)
        u8 location;            // 0x07
        f32 coordX;             // 0x08-0x0B
        f32 coordZ;             // 0x0C-0x0F
        u8 type;                // 0x10
        u8 condition;           // 0x11
        u8 typeCounter;         // 0x12
        u8 conditionCounter;    // 0x13
        u16 flags;               // 0x14
        u8 padding[10];         // 0x16-0x1F
    } chickens[12];             // 0x600-0x77F

    u8 padding_780[0x80];       // 0x780-0x7FF

    struct {
        u8 name[6];             // 0x00-0x05
        u8 affection;           // 0x06
        u8 location;            // 0x07
        f32 coordX;             // 0x08-0x0B
        f32 coordZ;             // 0x0C-0x0F
        u8 type;                // 0x10
        u8 condition;           // 0x11
        u8 typeCounter;         // 0x12
        u8 conditionCounter;    // 0x13
        u16 flags;              // 0x14-0x15
        u8 unk_16;              // 0x16
        u8 unk_17;              // 0x17
        u8 unk_18;              // 0x18
        u8 unk_19;              // 0x19
        u8 unk_1A;              // 0x1A
        u8 unk_1B;              // 0x1B
        u8 unk_1C;              // 0x1C
        u8 unk_1D;              // 0x1D
        u8 normalMilk;          // 0x1E
        u8 padding;             // 0x1F
    } farmAnimals[8];           // 0x800-0x8FF

    u8 padding_900[0x100];      // 0x900-0x9FF

    u8 farmFieldTiles[24][20];  // 0xA00-0xBDF

    u8 padding_BE0[0x20];       // 0xBE0-0xBFF

    u8 greenhouseFieldTiles[24][20]; // 0xC00-0xDDF

    u8 padding_DE0[0x120];      // 0xDE0-0xEFF

    u32 lifeEventBits[16];      // 0xF00-0xF3F

    u8 padding_F40[0x40];       // 0xF40-0xF7F

    u32 specialDialogueBits[16]; // 0xF80-0xFBF

    u8 padding_FC0[0x3E];       // 0xFC0-0xFFD

    u16 checksum;               // 0xFFE-0xFFF

} SramBuffer;

extern SramBuffer sramBuffer;

#endif