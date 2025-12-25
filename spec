#include <nusys.h>

// ============================================================================
// Boot
// ============================================================================

beginseg
    name    "code"
    flags   BOOT OBJECT
    entry   nuBoot
    address NU_SPEC_BOOT_ADDR
    stack   NU_SPEC_BOOT_STACK
    include "codesegment.o"
    include "$(ROOT)/usr/lib/PR/rspboot.o"
    include "$(ROOT)/usr/lib/PR/n_aspMain.o"
    include "$(ROOT)/usr/lib/PR/gspF3DEX2.fifo.o"
endseg

// ============================================================================
// Buffers
// ============================================================================

beginseg
    name "messageBoxBuffer"
    flags OBJECT
    address 0x80238800
    include "messageBoxBuffer.o"
endseg

beginseg
    name "dialogueIconSpriteBuffer"
    flags OBJECT
    address 0x8023B400
    include "dialogueIconSpriteBuffer.o"
endseg

beginseg
    name "characterAvatarsSpriteBuffer"
    flags OBJECT
    address 0x8023D300
    include "characterAvatarsSpriteBuffer.o"
endseg

beginseg
    name "shadowSpriteBuffer"
    flags OBJECT
    address 0x80240300
    include "shadowSpriteBuffer.o"
endseg

beginseg
    name "playerSpriteBuffer"
    flags OBJECT
    address 0x80240B00
    include "playerSpriteBuffer.o"
endseg

beginseg
    name "namingScreenBuffer"
    flags OBJECT
    address 0x80253B00
    include "namingScreenBuffer.o"
endseg

beginseg
    name "mapDataBuffer"
    flags OBJECT
    address 0x80255000
    include "mapDataBuffer.o"
endseg

beginseg
    name "spriteBuffer"
    flags OBJECT
    address 0x8026F000
    include "spriteBuffer.o"
endseg

beginseg
    name "mapObjectBuffer"
    flags OBJECT
    address 0x802E4000
    include "mapObjectBuffer.o"
endseg

beginseg
    name "cutsceneBytecodeBuffer"
    flags OBJECT
    address 0x802F4000
    include "cutsceneBytecodeBuffer.o"
endseg

beginseg
    name "fontBuffer"
    flags OBJECT
    address 0x802FF000
    include "fontBuffer.o"
endseg

beginseg
    name "textBuffer"
    flags OBJECT
    address 0x8030A800
    include "textBuffer.o"
endseg

beginseg
    name "messageBoxesBuffer"
    flags OBJECT
    address 0x8030B000
    include "messageBoxesBuffer.o"
endseg

beginseg
    name "dialogueBytecodeBuffer"
    flags OBJECT
    address 0x8030C800
    include "dialogueBytecodeBuffer.o"
endseg

beginseg
    name "sramBuffer"
    flags OBJECT
    address 0x8030D000
    include "sramBuffer.o"
endseg

beginseg
    name "farmRankingsBuffer"
    flags OBJECT
    address 0x8030E000
    include "farmRankingsBuffer.o"
endseg

beginseg
    name "unknownBuffer"
    flags OBJECT
    address 0x8030F800
    include "unknownBuffer.o"
endseg

beginseg
    name "frameBuffer"
    flags OBJECT
    address 0x8038F800
    include "frameBuffer.o"
endseg

// ============================================================================
// Cutscene bytecode
// ============================================================================

beginseg
    name "cutsceneBank1"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank1.bin.o"
endseg

beginseg
    name "cutsceneBank2"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank2.bin.o"
endseg

beginseg
    name "cutsceneBank3"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank3.bin.o"
endseg

beginseg
    name "cutsceneBank4"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank4.bin.o"
endseg

beginseg
    name "cutsceneBank5"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank5.bin.o"
endseg

beginseg
    name "cutsceneBank6"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank6.bin.o"
endseg

beginseg
    name "cutsceneBank7"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank7.bin.o"
endseg

beginseg
    name "cutsceneBank8"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank8.bin.o"
endseg

beginseg
    name "cutsceneBank9"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank9.bin.o"
endseg

beginseg
    name "cutsceneBank10"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank10.bin.o"
endseg

beginseg
    name "cutsceneBank11"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank11.bin.o"
endseg

beginseg
    name "cutsceneBank12"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank12.bin.o"
endseg

beginseg
    name "cutsceneBank13"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank13.bin.o"
endseg

beginseg
    name "cutsceneBank14"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank14.bin.o"
endseg

beginseg
    name "fireworksFestival"
    flags OBJECT
    include "bin/cutscenes/fireworksFestival.bin.o"
endseg

beginseg
    name "cutsceneBank16"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank16.bin.o"
endseg

beginseg
    name "cutsceneBank17"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank17.bin.o"
endseg

beginseg
    name "cutsceneBank18"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank18.bin.o"
endseg

beginseg
    name "cutsceneBank19"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank19.bin.o"
endseg

beginseg
    name "cutsceneBank20"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank20.bin.o"
endseg

beginseg
    name "cutsceneBank21"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank21.bin.o"
endseg

beginseg
    name "cutsceneBank22"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank22.bin.o"
endseg

beginseg
    name "cutsceneBank23"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank23.bin.o"
endseg

beginseg
    name "cutsceneBank24"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank24.bin.o"
endseg

beginseg
    name "cutsceneBank25"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank25.bin.o"
endseg

beginseg
    name "cutsceneBank26"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank26.bin.o"
endseg

beginseg
    name "cutsceneBank27"
    flags OBJECT
    include "bin/cutscenes/cutsceneBank27.bin.o"
endseg

// ============================================================================
// Entity sprites
// ============================================================================

beginseg
    name "playerTexture"
    flags RAW
    include "bin/sprites/entity-sprites/player/playerTexture.bin"
endseg

beginseg
    name "playerAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/player/playerAssetsIndex.bin"
endseg

beginseg
    name "playerSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/player/playerSpritesheetIndex.bin"
endseg

beginseg
    name "mariaTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/maria/mariaTexture.bin"
endseg

beginseg
    name "mariaAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/maria/mariaAssetsIndex.bin"
endseg

beginseg
    name "mariaSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/maria/mariaSpritesheetIndex.bin"
endseg

beginseg
    name "popuriTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/popuri/popuriTexture.bin"
endseg

beginseg
    name "popuriAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/popuri/popuriAssetsIndex.bin"
endseg

beginseg
    name "popuriSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/popuri/popuriSpritesheetIndex.bin"
endseg

beginseg
    name "elliTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/elli/elliTexture.bin"
endseg

beginseg
    name "elliAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/elli/elliAssetsIndex.bin"
endseg

beginseg
    name "elliSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/elli/elliSpritesheetIndex.bin"
endseg

beginseg
    name "annTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/ann/annTexture.bin"
endseg

beginseg
    name "annAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/ann/annAssetsIndex.bin"
endseg

beginseg
    name "annSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/ann/annSpritesheetIndex.bin"
endseg

beginseg
    name "karenTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/karen/karenTexture.bin"
endseg

beginseg
    name "karenAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/karen/karenAssetsIndex.bin"
endseg

beginseg
    name "karenSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/karen/karenSpritesheetIndex.bin"
endseg

beginseg
    name "harrisTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harris/harrisTexture.bin"
endseg

beginseg
    name "harrisAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harris/harrisAssetsIndex.bin"
endseg

beginseg
    name "harrisSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harris/harrisSpritesheetIndex.bin"
endseg

beginseg
    name "grayTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/gray/grayTexture.bin"
endseg

beginseg
    name "grayAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/gray/grayAssetsIndex.bin"
endseg

beginseg
    name "graySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/gray/graySpritesheetIndex.bin"
endseg

beginseg
    name "jeffTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/jeff/jeffTexture.bin"
endseg

beginseg
    name "jeffAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/jeff/jeffAssetsIndex.bin"
endseg

beginseg
    name "jeffSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/jeff/jeffSpritesheetIndex.bin"
endseg

beginseg
    name "cliffTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/cliff/cliffTexture.bin"
endseg

beginseg
    name "cliffAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/cliff/cliffAssetsIndex.bin"
endseg

beginseg
    name "cliffSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/cliff/cliffSpritesheetIndex.bin"
endseg

beginseg
    name "kaiTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kai/kaiTexture.bin"
endseg

beginseg
    name "kaiAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kai/kaiAssetsIndex.bin"
endseg

beginseg
    name "kaiSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kai/kaiSpritesheetIndex.bin"
endseg

beginseg
    name "mayorTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mayor/mayorTexture.bin"
endseg

beginseg
    name "mayorAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mayor/mayorAssetsIndex.bin"
endseg

beginseg
    name "mayorSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mayor/mayorSpritesheetIndex.bin"
endseg

beginseg
    name "mayorWifeTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mayorWife/mayorWifeTexture.bin"
endseg

beginseg
    name "mayorWifeAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mayorWife/mayorWifeAssetsIndex.bin"
endseg

beginseg
    name "mayorWifeSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mayorWife/mayorWifeSpritesheetIndex.bin"
endseg

beginseg
    name "liliaTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/lilia/liliaTexture.bin"
endseg

beginseg
    name "liliaAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/lilia/liliaAssetsIndex.bin"
endseg

beginseg
    name "liliaSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/lilia/liliaSpritesheetIndex.bin"
endseg

beginseg
    name "basilTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/basil/basilTexture.bin"
endseg

beginseg
    name "basilAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/basil/basilAssetsIndex.bin"
endseg

beginseg
    name "basilSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/basil/basilSpritesheetIndex.bin"
endseg

beginseg
    name "ellenTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/ellen/ellenTexture.bin"
endseg

beginseg
    name "ellenAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/ellen/ellenAssetsIndex.bin"
endseg

beginseg
    name "ellenSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/ellen/ellenSpritesheetIndex.bin"
endseg

beginseg
    name "dougTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/doug/dougTexture.bin"
endseg

beginseg
    name "dougAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/doug/dougAssetsIndex.bin"
endseg

beginseg
    name "dougSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/doug/dougSpritesheetIndex.bin"
endseg

beginseg
    name "gotzTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/gotz/gotzTexture.bin"
endseg

beginseg
    name "gotzAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/gotz/gotzAssetsIndex.bin"
endseg

beginseg
    name "gotzSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/gotz/gotzSpritesheetIndex.bin"
endseg

beginseg
    name "shipperTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/shipper/shipperTexture.bin"
endseg

beginseg
    name "shipperAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/shipper/shipperAssetsIndex.bin"
endseg

beginseg
    name "shipperSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/shipper/shipperSpritesheetIndex.bin"
endseg

beginseg
    name "sashaTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/sasha/sashaTexture.bin"
endseg

beginseg
    name "sashaAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/sasha/sashaAssetsIndex.bin"
endseg

beginseg
    name "sashaSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/sasha/sashaSpritesheetIndex.bin"
endseg

beginseg
    name "dukeTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/duke/dukeTexture.bin"
endseg

beginseg
    name "dukeAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/duke/dukeAssetsIndex.bin"
endseg

beginseg
    name "dukeSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/duke/dukeSpritesheetIndex.bin"
endseg

beginseg
    name "manaTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mana/manaTexture.bin"
endseg

beginseg
    name "manaAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mana/manaAssetsIndex.bin"
endseg

beginseg
    name "manaSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mana/manaSpritesheetIndex.bin"
endseg

beginseg
    name "rickTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/rick/rickTexture.bin"
endseg

beginseg
    name "rickAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/rick/rickAssetsIndex.bin"
endseg

beginseg
    name "rickSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/rick/rickSpritesheetIndex.bin"
endseg

beginseg
    name "barleyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/barley/barleyTexture.bin"
endseg

beginseg
    name "barleyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/barley/barleyAssetsIndex.bin"
endseg

beginseg
    name "barleySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/barley/barleySpritesheetIndex.bin"
endseg

beginseg
    name "midwifeTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/midwife/midwifeTexture.bin"
endseg

beginseg
    name "midwifeAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/midwife/midwifeAssetsIndex.bin"
endseg

beginseg
    name "midwifeSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/midwife/midwifeSpritesheetIndex.bin"
endseg

beginseg
    name "pastorTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/pastor/pastorTexture.bin"
endseg

beginseg
    name "pastorAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/pastor/pastorAssetsIndex.bin"
endseg

beginseg
    name "pastorSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/pastor/pastorSpritesheetIndex.bin"
endseg

beginseg
    name "saibaraTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/saibara/saibaraTexture.bin"
endseg

beginseg
    name "saibaraAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/saibara/saibaraAssetsIndex.bin"
endseg

beginseg
    name "saibaraSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/saibara/saibaraSpritesheetIndex.bin"
endseg

beginseg
    name "potionShopDealerTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/potionShopDealer/potionShopDealerTexture.bin"
endseg

beginseg
    name "potionShopDealerAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/potionShopDealer/potionShopDealerAssetsIndex.bin"
endseg

beginseg
    name "potionShopDealerSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/potionShopDealer/potionShopDealerSpritesheetIndex.bin"
endseg

beginseg
    name "masterCarpenterTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/masterCarpenter/masterCarpenterTexture.bin"
endseg

beginseg
    name "masterCarpenterAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/masterCarpenter/masterCarpenterAssetsIndex.bin"
endseg

beginseg
    name "masterCarpenterSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/masterCarpenter/masterCarpenterSpritesheetIndex.bin"
endseg

beginseg
    name "assistantCarpentersTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/assistantCarpenters/assistantCarpentersTexture.bin"
endseg

beginseg
    name "assistantCarpentersAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/assistantCarpenters/assistantCarpentersAssetsIndex.bin"
endseg

beginseg
    name "assistantCarpentersSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/assistantCarpenters/assistantCarpentersSpritesheetIndex.bin"
endseg

beginseg
    name "kentTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kent/kentTexture.bin"
endseg

beginseg
    name "kentAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kent/kentAssetsIndex.bin"
endseg

beginseg
    name "kentSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kent/kentSpritesheetIndex.bin"
endseg

beginseg
    name "stuTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/stu/stuTexture.bin"
endseg

beginseg
    name "stuAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/stu/stuAssetsIndex.bin"
endseg

beginseg
    name "stuSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/stu/stuSpritesheetIndex.bin"
endseg

beginseg
    name "mayTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/may/mayTexture.bin"
endseg

beginseg
    name "mayAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/may/mayAssetsIndex.bin"
endseg

beginseg
    name "maySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/may/maySpritesheetIndex.bin"
endseg

beginseg
    name "babyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/baby/babyTexture.bin"
endseg

beginseg
    name "babyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/baby/babyAssetsIndex.bin"
endseg

beginseg
    name "babySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/baby/babySpritesheetIndex.bin"
endseg

beginseg
    name "mariaHarrisBabyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mariaHarrisBaby/mariaHarrisBabyTexture.bin"
endseg

beginseg
    name "mariaHarrisBabyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mariaHarrisBaby/mariaHarrisBabyAssetsIndex.bin"
endseg

beginseg
    name "mariaHarrisBabySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/mariaHarrisBaby/mariaHarrisBabySpritesheetIndex.bin"
endseg

beginseg
    name "popuriGrayBabyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/popuriGrayBaby/popuriGrayBabyTexture.bin"
endseg

beginseg
    name "popuriGrayBabyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/popuriGrayBaby/popuriGrayBabyAssetsIndex.bin"
endseg

beginseg
    name "popuriGrayBabySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/popuriGrayBaby/popuriGrayBabySpritesheetIndex.bin"
endseg

beginseg
    name "elliJeffBabyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/elliJeffBaby/elliJeffBabyTexture.bin"
endseg

beginseg
    name "elliJeffBabyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/elliJeffBaby/elliJeffBabyAssetsIndex.bin"
endseg

beginseg
    name "elliJeffBabySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/elliJeffBaby/elliJeffBabySpritesheetIndex.bin"
endseg

beginseg
    name "annCliffBabyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/annCliffBaby/annCliffBabyTexture.bin"
endseg

beginseg
    name "annCliffBabyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/annCliffBaby/annCliffBabyAssetsIndex.bin"
endseg

beginseg
    name "annCliffBabySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/annCliffBaby/annCliffBabySpritesheetIndex.bin"
endseg

beginseg
    name "karenKaiBabyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/karenKaiBaby/karenKaiBabyTexture.bin"
endseg

beginseg
    name "karenKaiBabyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/karenKaiBaby/karenKaiBabyAssetsIndex.bin"
endseg

beginseg
    name "karenKaiBabySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/karenKaiBaby/karenKaiBabySpritesheetIndex.bin"
endseg

beginseg
    name "harvestSpritesTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harvestSprites/harvestSpritesTexture.bin"
endseg

beginseg
    name "harvestSpritesAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harvestSprites/harvestSpritesAssetsIndex.bin"
endseg

beginseg
    name "harvestSpritesSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harvestSprites/harvestSpritesSpritesheetIndex.bin"
endseg

beginseg
    name "harvestGoddessTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harvestGoddess/harvestGoddessTexture.bin"
endseg

beginseg
    name "harvestGoddessAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harvestGoddess/harvestGoddessAssetsIndex.bin"
endseg

beginseg
    name "harvestGoddessSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/harvestGoddess/harvestGoddessSpritesheetIndex.bin"
endseg

beginseg
    name "kappaTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kappa/kappaTexture.bin"
endseg

beginseg
    name "kappaAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kappa/kappaAssetsIndex.bin"
endseg

beginseg
    name "kappaSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/kappa/kappaSpritesheetIndex.bin"
endseg

beginseg
    name "dadTexture"
    flags RAW
    include "bin/sprites/entity-sprites/npc/dadTexture.bin"
endseg

beginseg
    name "dadAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/dadAssetsIndex.bin"
endseg

beginseg
    name "dadSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/npc/dadSpritesheetIndex.bin"
endseg

beginseg
    name "dogTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/dogTexture.bin"
endseg

beginseg
    name "dogAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/dogAssetsIndex.bin"
endseg

beginseg
    name "dogSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/dogSpritesheetIndex.bin"
endseg

beginseg
    name "dogTitleTexture"
    flags RAW
    include "bin/sprites/entity-sprites/title/dogTitleTexture.bin"
endseg

beginseg
    name "dogTitleAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/title/dogTitleAssetsIndex.bin"
endseg

beginseg
    name "dogTitleSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/title/dogTitleSpritesheetIndex.bin"
endseg

beginseg
    name "horsePonyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horsePonyTexture.bin"
endseg

beginseg
    name "horsePonyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horsePonyAssetsIndex.bin"
endseg

beginseg
    name "horsePonySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horsePonySpritesheetIndex.bin"
endseg

beginseg
    name "horseGrownTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseGrownTexture.bin"
endseg

beginseg
    name "horseGrownAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseGrownAssetsIndex.bin"
endseg

beginseg
    name "horseGrownSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseGrownSpritesheetIndex.bin"
endseg

beginseg
    name "horseUnbridledTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseUnbridledTexture.bin"
endseg

beginseg
    name "horseUnbridledAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseUnbridledAssetsIndex.bin"
endseg

beginseg
    name "horseUnbridledSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseUnbridledSpritesheetIndex.bin"
endseg

beginseg
    name "horseRacerTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseRacerTexture.bin"
endseg

beginseg
    name "horseRacerAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseRacerAssetsIndex.bin"
endseg

beginseg
    name "horseRacerSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/horseRacerSpritesheetIndex.bin"
endseg

beginseg
    name "chickTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/chickTexture.bin"
endseg

beginseg
    name "chickAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/chickAssetsIndex.bin"
endseg

beginseg
    name "chickSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/chickSpritesheetIndex.bin"
endseg

beginseg
    name "chickenTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/chickenTexture.bin"
endseg

beginseg
    name "chickenAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/chickenAssetsIndex.bin"
endseg

beginseg
    name "chickenSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/chickenSpritesheetIndex.bin"
endseg

beginseg
    name "calfTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/calfTexture.bin"
endseg

beginseg
    name "calfAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/calfAssetsIndex.bin"
endseg

beginseg
    name "calfSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/calfSpritesheetIndex.bin"
endseg

beginseg
    name "youngCowTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/youngCowTexture.bin"
endseg

beginseg
    name "youngCowAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/youngCowAssetsIndex.bin"
endseg

beginseg
    name "youngCowSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/youngCowSpritesheetIndex.bin"
endseg

beginseg
    name "cowTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cowTexture.bin"
endseg

beginseg
    name "cowAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cowAssetsIndex.bin"
endseg

beginseg
    name "cowSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cowSpritesheetIndex.bin"
endseg

beginseg
    name "cowIndoorsTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cowIndoorsTexture.bin"
endseg

beginseg
    name "cowIndoorsAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cowIndoorsAssetsIndex.bin"
endseg

beginseg
    name "cowIndoorsSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cowIndoorsSpritesheetIndex.bin"
endseg

beginseg
    name "lambTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/lambTexture.bin"
endseg

beginseg
    name "lambAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/lambAssetsIndex.bin"
endseg

beginseg
    name "lambSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/lambSpritesheetIndex.bin"
endseg

beginseg
    name "sheepTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/sheepTexture.bin"
endseg

beginseg
    name "sheepAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/sheepAssetsIndex.bin"
endseg

beginseg
    name "sheepSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/sheepSpritesheetIndex.bin"
endseg

beginseg
    name "catTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/catTexture.bin"
endseg

beginseg
    name "catAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/catAssetsIndex.bin"
endseg

beginseg
    name "catSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/catSpritesheetIndex.bin"
endseg

beginseg
    name "frogTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/frogTexture.bin"
endseg

beginseg
    name "frogAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/frogAssetsIndex.bin"
endseg

beginseg
    name "frogSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/frogSpritesheetIndex.bin"
endseg

beginseg
    name "snakeTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/snakeTexture.bin"
endseg

beginseg
    name "snakeAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/snakeAssetsIndex.bin"
endseg

beginseg
    name "snakeSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/snakeSpritesheetIndex.bin"
endseg

beginseg
    name "crabTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/crabTexture.bin"
endseg

beginseg
    name "crabAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/crabAssetsIndex.bin"
endseg

beginseg
    name "crabSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/crabSpritesheetIndex.bin"
endseg

beginseg
    name "birdTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/birdTexture.bin"
endseg

beginseg
    name "birdAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/birdAssetsIndex.bin"
endseg

beginseg
    name "birdSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/birdSpritesheetIndex.bin"
endseg

beginseg
    name "hornedBeetleTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/hornedBeetleTexture.bin"
endseg

beginseg
    name "hornedBeetleAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/hornedBeetleAssetsIndex.bin"
endseg

beginseg
    name "hornedBeetleSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/hornedBeetleSpritesheetIndex.bin"
endseg

beginseg
    name "stagBeetleTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/stagBeetleTexture.bin"
endseg

beginseg
    name "stagBeetleAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/stagBeetleAssetsIndex.bin"
endseg

beginseg
    name "stagBeetleSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/stagBeetleSpritesheetIndex.bin"
endseg

beginseg
    name "bellRingCricketTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/bellRingCricketTexture.bin"
endseg

beginseg
    name "bellRingCricketAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/bellRingCricketAssetsIndex.bin"
endseg

beginseg
    name "bellRingCricketSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/bellRingCricketSpritesheetIndex.bin"
endseg

beginseg
    name "cicadaTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cicadaTexture.bin"
endseg

beginseg
    name "cicadaAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cicadaAssetsIndex.bin"
endseg

beginseg
    name "cicadaSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/cicadaSpritesheetIndex.bin"
endseg

beginseg
    name "ladybugTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/ladybugTexture.bin"
endseg

beginseg
    name "ladybugAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/ladybugAssetsIndex.bin"
endseg

beginseg
    name "ladybugSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/ladybugSpritesheetIndex.bin"
endseg

beginseg
    name "dragonflyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/dragonflyTexture.bin"
endseg

beginseg
    name "dragonflyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/dragonflyAssetsIndex.bin"
endseg

beginseg
    name "dragonflySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/dragonflySpritesheetIndex.bin"
endseg

beginseg
    name "grasshopperTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/grasshopperTexture.bin"
endseg

beginseg
    name "grasshopperAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/grasshopperAssetsIndex.bin"
endseg

beginseg
    name "grasshopperSpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/grasshopperSpritesheetIndex.bin"
endseg

beginseg
    name "purpleButterflyTexture"
    flags RAW
    include "bin/sprites/entity-sprites/animals/purpleButterflyTexture.bin"
endseg

beginseg
    name "purpleButterflyAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/purpleButterflyAssetsIndex.bin"
endseg

beginseg
    name "purpleButterflySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/animals/purpleButterflySpritesheetIndex.bin"
endseg

beginseg
    name "shadowsTexture"
    flags RAW
    include "bin/sprites/shadows/shadowsTexture.bin"
endseg

beginseg
    name "shadowsIndex"
    flags RAW
    include "bin/sprites/shadows/shadowsIndex.bin"
endseg

beginseg
    name "expressionBubblesTexture"
    flags RAW
    include "bin/sprites/expression-bubbles/expressionBubblesTexture.bin"
endseg

beginseg
    name "expressionBubblesAssetsIndex"
    flags RAW
    include "bin/sprites/expression-bubbles/expressionBubblesAssetsIndex.bin"
endseg

beginseg
    name "expressionBubblesSpritesheetIndex"
    flags RAW
    include "bin/sprites/expression-bubbles/expressionBubblesSpritesheetIndex.bin"
endseg

beginseg
    name "emptyEntityTexture"
    flags RAW
    include "bin/sprites/entity-sprites/emptyEntityTexture.bin"
endseg

beginseg
    name "emptyEntityAssetsIndex"
    flags RAW
    include "bin/sprites/entity-sprites/emptyEntityAssetsIndex.bin"
endseg

beginseg
    name "emptyEntitySpritesheetIndex"
    flags RAW
    include "bin/sprites/entity-sprites/emptyEntitySpritesheetIndex.bin"
endseg

beginseg
    name "ranchSpringMap"
    flags RAW
    include "bin/maps/ranch/ranchSpringMap.bin"
endseg

beginseg
    name "ranchSummerMap"
    flags RAW
    include "bin/maps/ranch/ranchSummerMap.bin"
endseg

beginseg
    name "ranchFallMap"
    flags RAW
    include "bin/maps/ranch/ranchFallMap.bin"
endseg

beginseg
    name "ranchWinterMap"
    flags RAW
    include "bin/maps/ranch/ranchWinterMap.bin"
endseg

beginseg
    name "annsRoomMap"
    flags RAW
    include "bin/maps/ranch/annsRoomMap.bin"
endseg

beginseg
    name "ranchBarnMap"
    flags RAW
    include "bin/maps/ranch/ranchBarnMap.bin"
endseg

beginseg
    name "ranchStoreMap"
    flags RAW
    include "bin/maps/ranch/ranchStoreMap.bin"
endseg

beginseg
    name "ranchHouseMap"
    flags RAW
    include "bin/maps/ranch/ranchHouseMap.bin"
endseg

beginseg
    name "emptyMap1"
    flags RAW
    include "bin/maps/unknown/emptyMap1.bin"
endseg

beginseg
    name "beachSpringMap"
    flags RAW
    include "bin/maps/beach/beachSpringMap.bin"
endseg

beginseg
    name "beachSummerMap"
    flags RAW
    include "bin/maps/beach/beachSummerMap.bin"
endseg

beginseg
    name "beachFallMap"
    flags RAW
    include "bin/maps/beach/beachFallMap.bin"
endseg

beginseg
    name "beachWinterMap"
    flags RAW
    include "bin/maps/beach/beachWinterMap.bin"
endseg

beginseg
    name "raceTrackSpringMap"
    flags RAW
    include "bin/maps/race-track/raceTrackSpringMap.bin"
endseg

beginseg
    name "raceTrackSummerMap"
    flags RAW
    include "bin/maps/race-track/raceTrackSummerMap.bin"
endseg

beginseg
    name "raceTrackFallMap"
    flags RAW
    include "bin/maps/race-track/raceTrackFallMap.bin"
endseg

beginseg
    name "raceTrackWinterMap"
    flags RAW
    include "bin/maps/race-track/raceTrackWinterMap.bin"
endseg

beginseg
    name "mountain1SpringMap"
    flags RAW
    include "bin/maps/mountain/mountain-1/mountain1SpringMap.bin"
endseg

beginseg
    name "mountain1SummerMap"
    flags RAW
    include "bin/maps/mountain/mountain-1/mountain1SummerMap.bin"
endseg

beginseg
    name "mountain1FallMap"
    flags RAW
    include "bin/maps/mountain/mountain-1/mountain1FallMap.bin"
endseg

beginseg
    name "mountain1WinterMap"
    flags RAW
    include "bin/maps/mountain/mountain-1/mountain1WinterMap.bin"
endseg

beginseg
    name "mountain2SpringMap"
    flags RAW
    include "bin/maps/mountain/mountain-2/mountain2SpringMap.bin"
endseg

beginseg
    name "mountain2SummerMap"
    flags RAW
    include "bin/maps/mountain/mountain-2/mountain2SummerMap.bin"
endseg

beginseg
    name "mountain2FallMap"
    flags RAW
    include "bin/maps/mountain/mountain-2/mountain2FallMap.bin"
endseg

beginseg
    name "mountain2WinterMap"
    flags RAW
    include "bin/maps/mountain/mountain-2/mountain2WinterMap.bin"
endseg

beginseg
    name "topOfMountain1SpringMap"
    flags RAW
    include "bin/maps/mountain/top-of-mountain-1/topOfMountain1SpringMap.bin"
endseg

beginseg
    name "topOfMountain1SummerMap"
    flags RAW
    include "bin/maps/mountain/top-of-mountain-1/topOfMountain1SummerMap.bin"
endseg

beginseg
    name "topOfMountain1FallMap"
    flags RAW
    include "bin/maps/mountain/top-of-mountain-1/topOfMountain1FallMap.bin"
endseg

beginseg
    name "topOfMountain1WinterMap"
    flags RAW
    include "bin/maps/mountain/top-of-mountain-1/topOfMountain1WinterMap.bin"
endseg

beginseg
    name "moonMountainSpringMap"
    flags RAW
    include "bin/maps/mountain/moon-mountain/moonMountainSpringMap.bin"
endseg

beginseg
    name "moonMountainSummerMap"
    flags RAW
    include "bin/maps/mountain/moon-mountain/moonMountainSummerMap.bin"
endseg

beginseg
    name "moonMountainFallMap"
    flags RAW
    include "bin/maps/mountain/moon-mountain/moonMountainFallMap.bin"
endseg

beginseg
    name "moonMountainWinterMap"
    flags RAW
    include "bin/maps/mountain/moon-mountain/moonMountainWinterMap.bin"
endseg

beginseg
    name "carpentersHutMap"
    flags RAW
    include "bin/maps/carpenter-hut/carpentersHutMap.bin"
endseg

beginseg
    name "dumplingHouseMap"
    flags RAW
    include "bin/maps/dumpling-house/dumplingHouseMap.bin"
endseg

beginseg
    name "pondSpringMap"
    flags RAW
    include "bin/maps/pond/pondSpringMap.bin"
endseg

beginseg
    name "pondSummerMap"
    flags RAW
    include "bin/maps/pond/pondSummerMap.bin"
endseg

beginseg
    name "pondFallMap"
    flags RAW
    include "bin/maps/pond/pondFallMap.bin"
endseg

beginseg
    name "pondWinterMap"
    flags RAW
    include "bin/maps/pond/pondWinterMap.bin"
endseg

beginseg
    name "ellisRoomMap"
    flags RAW
    include "bin/maps/bakery/ellisRoomMap.bin"
endseg

beginseg
    name "bakeryMap"
    flags RAW
    include "bin/maps/bakery/bakeryMap.bin"
endseg

beginseg
    name "village1SpringMap"
    flags RAW
    include "bin/maps/village/village-1/village1SpringMap.bin"
endseg

beginseg
    name "village1SummerMap"
    flags RAW
    include "bin/maps/village/village-1/village1SummerMap.bin"
endseg

beginseg
    name "village1FallMap"
    flags RAW
    include "bin/maps/village/village-1/village1FallMap.bin"
endseg

beginseg
    name "village1WinterMap"
    flags RAW
    include "bin/maps/village/village-1/village1WinterMap.bin"
endseg

beginseg
    name "village2SpringMap"
    flags RAW
    include "bin/maps/village/village-2/village2SpringMap.bin"
endseg

beginseg
    name "village2SummerMap"
    flags RAW
    include "bin/maps/village/village-2/village2SummerMap.bin"
endseg

beginseg
    name "village2FallMap"
    flags RAW
    include "bin/maps/village/village-2/village2FallMap.bin"
endseg

beginseg
    name "village2WinterMap"
    flags RAW
    include "bin/maps/village/village-2/village2WinterMap.bin"
endseg

beginseg
    name "popurisRoomMap"
    flags RAW
    include "bin/maps/flower-shop/popurisRoomMap.bin"
endseg

beginseg
    name "flowerShopMap"
    flags RAW
    include "bin/maps/flower-shop/flowerShopMap.bin"
endseg

beginseg
    name "churchMap"
    flags RAW
    include "bin/maps/church/churchMap.bin"
endseg

beginseg
    name "souvenirShopMap"
    flags RAW
    include "bin/maps/souvenir-shop/souvenirShopMap.bin"
endseg

beginseg
    name "squareSpringMap"
    flags RAW
    include "bin/maps/square/squareSpringMap.bin"
endseg

beginseg
    name "squareSummerMap"
    flags RAW
    include "bin/maps/square/squareSummerMap.bin"
endseg

beginseg
    name "squareFallMap"
    flags RAW
    include "bin/maps/square/squareFallMap.bin"
endseg

beginseg
    name "squareWinterMap"
    flags RAW
    include "bin/maps/square/squareWinterMap.bin"
endseg

beginseg
    name "ricksShopMap"
    flags RAW
    include "bin/maps/rick-shop/ricksShopMap.bin"
endseg

beginseg
    name "midwifesHouseMap"
    flags RAW
    include "bin/maps/midwife/midwifesHouseMap.bin"
endseg

beginseg
    name "tavernMap"
    flags RAW
    include "bin/maps/tavern/tavernMap.bin"
endseg

beginseg
    name "libraryMap"
    flags RAW
    include "bin/maps/library/libraryMap.bin"
endseg

beginseg
    name "mariasRoomMap"
    flags RAW
    include "bin/maps/mayors-house/mariasRoomMap.bin"
endseg

beginseg
    name "mayorsHouseMap"
    flags RAW
    include "bin/maps/mayors-house/mayorsHouseMap.bin"
endseg

beginseg
    name "potionShopBackroomMap"
    flags RAW
    include "bin/maps/potion-shop/potionShopBackroomMap.bin"
endseg

beginseg
    name "potionShopMap"
    flags RAW
    include "bin/maps/potion-shop/potionShopMap.bin"
endseg

beginseg
    name "emptyMap2"
    flags RAW
    include "bin/maps/unknown/emptyMap2.bin"
endseg

beginseg
    name "spriteCaveMap"
    flags RAW
    include "bin/maps/cave/spriteCaveMap.bin"
endseg

beginseg
    name "caveMap"
    flags RAW
    include "bin/maps/cave/caveMap.bin"
endseg

beginseg
    name "emptyMineMap"
    flags RAW
    include "bin/maps/mine/emptyMineMap.bin"
endseg

beginseg
    name "mineMap"
    flags RAW
    include "bin/maps/mine/mineMap.bin"
endseg

beginseg
    name "karensRoomMap"
    flags RAW
    include "bin/maps/vineyard/karensRoomMap.bin"
endseg

beginseg
    name "vineyardSpringMap"
    flags RAW
    include "bin/maps/vineyard/vineyardSpringMap.bin"
endseg

beginseg
    name "vineyardSummerMap"
    flags RAW
    include "bin/maps/vineyard/vineyardSummerMap.bin"
endseg

beginseg
    name "vineyardFallMap"
    flags RAW
    include "bin/maps/vineyard/vineyardFallMap.bin"
endseg

beginseg
    name "vineyardWinterMap"
    flags RAW
    include "bin/maps/vineyard/vineyardWinterMap.bin"
endseg

beginseg
    name "vineyardHouseMap"
    flags RAW
    include "bin/maps/vineyard/vineyardHouseMap.bin"
endseg

beginseg
    name "vineyardCellar1Map"
    flags RAW
    include "bin/maps/vineyard/vineyardCellar1Map.bin"
endseg

beginseg
    name "vineyardCellar2Map"
    flags RAW
    include "bin/maps/vineyard/vineyardCellar2Map.bin"
endseg

beginseg
    name "roadSpringMap"
    flags RAW
    include "bin/maps/road/roadSpringMap.bin"
endseg

beginseg
    name "roadSummerMap"
    flags RAW
    include "bin/maps/road/roadSummerMap.bin"
endseg

beginseg
    name "roadFallMap"
    flags RAW
    include "bin/maps/road/roadFallMap.bin"
endseg

beginseg
    name "roadWinterMap"
    flags RAW
    include "bin/maps/road/roadWinterMap.bin"
endseg

beginseg
    name "farmSpringMap"
    flags RAW
    include "bin/maps/farm/farmSpringMap.bin"
endseg

beginseg
    name "farmSummerMap"
    flags RAW
    include "bin/maps/farm/farmSummerMap.bin"
endseg

beginseg
    name "farmFallMap"
    flags RAW
    include "bin/maps/farm/farmFallMap.bin"
endseg

beginseg
    name "farmWinterMap"
    flags RAW
    include "bin/maps/farm/farmWinterMap.bin"
endseg

beginseg
    name "greenhouseMap"
    flags RAW
    include "bin/maps/greenhouse/greenhouseMap.bin"
endseg

beginseg
    name "houseMap"
    flags RAW
    include "bin/maps/house/houseMap.bin"
endseg

beginseg
    name "barnMap"
    flags RAW
    include "bin/maps/farm/barnMap.bin"
endseg

beginseg
    name "coopMap"
    flags RAW
    include "bin/maps/farm/coopMap.bin"
endseg

beginseg
    name "kitchenMap"
    flags RAW
    include "bin/maps/house/kitchenMap.bin"
endseg

beginseg
    name "bathroomMap"
    flags RAW
    include "bin/maps/house/bathroomMap.bin"
endseg

// ============================================================================
// Map object sprites
// ============================================================================

beginseg
    name "wavesTexture"
    flags RAW
    include "bin/maps/beach/sprites/wavesTexture.bin"
endseg

beginseg
    name "wavesAssetsIndex"
    flags RAW
    include "bin/maps/beach/sprites/wavesAssetsIndex.bin"
endseg

beginseg
    name "wavesSpritesheetIndex"
    flags RAW
    include "bin/maps/beach/sprites/wavesSpritesheetIndex.bin"
endseg

beginseg
    name "waterTilesTexture"
    flags RAW
    include "bin/maps/beach/sprites/waterTilesTexture.bin"
endseg

beginseg
    name "waterTilesAssetsIndex"
    flags RAW
    include "bin/maps/beach/sprites/waterTilesAssetsIndex.bin"
endseg

beginseg
    name "waterTilesSpritesheetIndex"
    flags RAW
    include "bin/maps/beach/sprites/waterTilesSpritesheetIndex.bin"
endseg

beginseg
    name "boulderTexture"
    flags RAW
    include "bin/maps/mountain/sprites/boulderTexture.bin"
endseg

beginseg
    name "boulderAssetsIndex"
    flags RAW
    include "bin/maps/mountain/sprites/boulderAssetsIndex.bin"
endseg

beginseg
    name "boulderSpritesheetIndex"
    flags RAW
    include "bin/maps/mountain/sprites/boulderSpritesheetIndex.bin"
endseg

beginseg
    name "bridgeTexture"
    flags RAW
    include "bin/maps/mountain/sprites/bridgeTexture.bin"
endseg

beginseg
    name "bridgeAssetsIndex"
    flags RAW
    include "bin/maps/mountain/sprites/bridgeAssetsIndex.bin"
endseg

beginseg
    name "bridgeSpritesheetIndex"
    flags RAW
    include "bin/maps/mountain/sprites/bridgeSpritesheetIndex.bin"
endseg

beginseg
    name "stonePillarTexture"
    flags RAW
    include "bin/maps/mountain/sprites/stonePillarTexture.bin"
endseg

beginseg
    name "stonePillarAssetsIndex"
    flags RAW
    include "bin/maps/mountain/sprites/stonePillarAssetsIndex.bin"
endseg

beginseg
    name "stonePillarSpritesheetIndex"
    flags RAW
    include "bin/maps/mountain/sprites/stonePillarSpritesheetIndex.bin"
endseg

beginseg
    name "emptyMountainSprite1Texture"
    flags RAW
    include "bin/maps/mountain/sprites/emptyMountainSprite1Texture.bin"
endseg

beginseg
    name "emptyMountainSprite2Texture"
    flags RAW
    include "bin/maps/mountain/sprites/emptyMountainSprite2Texture.bin"
endseg

beginseg
    name "mountainPathTilesTexture"
    flags RAW
    include "bin/maps/mountain/sprites/mountainPathTilesTexture.bin"
endseg

beginseg
    name "mountainPathTilesAssetsIndex"
    flags RAW
    include "bin/maps/mountain/sprites/mountainPathTilesAssetsIndex.bin"
endseg

beginseg
    name "mountainPathTilesSpritesheetIndex"
    flags RAW
    include "bin/maps/mountain/sprites/mountainPathTilesSpritesheetIndex.bin"
endseg

beginseg
    name "steamTexture"
    flags RAW
    include "bin/maps/mountain/sprites/steamTexture.bin"
endseg

beginseg
    name "steamAssetsIndex"
    flags RAW
    include "bin/maps/mountain/sprites/steamAssetsIndex.bin"
endseg

beginseg
    name "steamSpritesheetIndex"
    flags RAW
    include "bin/maps/mountain/sprites/steamSpritesheetIndex.bin"
endseg

beginseg
    name "pondWaterTexture"
    flags RAW
    include "bin/maps/pond/sprites/pondWaterTexture.bin"
endseg

beginseg
    name "pondWaterAssetsIndex"
    flags RAW
    include "bin/maps/pond/sprites/pondWaterAssetsIndex.bin"
endseg

beginseg
    name "pondWaterSpritesheetIndex"
    flags RAW
    include "bin/maps/pond/sprites/pondWaterSpritesheetIndex.bin"
endseg

beginseg
    name "festivalFlowersTexture"
    flags RAW
    include "bin/maps/square/sprites/festivalFlowersTexture.bin"
endseg

beginseg
    name "festivalFlowersAssetsIndex"
    flags RAW
    include "bin/maps/square/sprites/festivalFlowersAssetsIndex.bin"
endseg

beginseg
    name "festivalFlowersSpritesheetIndex"
    flags RAW
    include "bin/maps/square/sprites/festivalFlowersSpritesheetIndex.bin"
endseg

beginseg
    name "squareFountainTexture"
    flags RAW
    include "bin/maps/square/sprites/squareFountainTexture.bin"
endseg

beginseg
    name "squareFountainAssetsIndex"
    flags RAW
    include "bin/maps/square/sprites/squareFountainAssetsIndex.bin"
endseg

beginseg
    name "squareFountainSpritesheetIndex"
    flags RAW
    include "bin/maps/square/sprites/squareFountainSpritesheetIndex.bin"
endseg

beginseg
    name "village2WaterTexture"
    flags RAW
    include "bin/maps/square/sprites/village2WaterTexture.bin"
endseg

beginseg
    name "village2WaterAssetsIndex"
    flags RAW
    include "bin/maps/square/sprites/village2WaterAssetsIndex.bin"
endseg

beginseg
    name "village2WaterSpritesheetIndex"
    flags RAW
    include "bin/maps/square/sprites/village2WaterSpritesheetIndex.bin"
endseg

beginseg
    name "emptyMapSprite1Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMapSprite1Texture.bin"
endseg

beginseg
    name "emptyMapSprite2Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMapSprite2Texture.bin"
endseg

beginseg
    name "emptyMapSprite3Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMapSprite3Texture.bin"
endseg

beginseg
    name "emptyMapSprite4Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMapSprite4Texture.bin"
endseg

beginseg
    name "emptyMapSprite5Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMapSprite5Texture.bin"
endseg

beginseg
    name "emptyMapSprite6Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMapSprite6Texture.bin"
endseg

beginseg
    name "stoneTexture"
    flags RAW
    include "bin/maps/bathroom/sprites/stoneTexture.bin"
endseg

beginseg
    name "stoneAssetsIndex"
    flags RAW
    include "bin/maps/bathroom/sprites/stoneAssetsIndex.bin"
endseg

beginseg
    name "stoneSpritesheetIndex"
    flags RAW
    include "bin/maps/bathroom/sprites/stoneSpritesheetIndex.bin"
endseg

beginseg
    name "vineyardTreeTexture"
    flags RAW
    include "bin/maps/vineyard/sprites/vineyardTreeTexture.bin"
endseg

beginseg
    name "vineyardTreeAssetsIndex"
    flags RAW
    include "bin/maps/vineyard/sprites/vineyardTreeAssetsIndex.bin"
endseg

beginseg
    name "vineyardTreeSpritesheetIndex"
    flags RAW
    include "bin/maps/vineyard/sprites/vineyardTreeSpritesheetIndex.bin"
endseg

beginseg
    name "emptyMap1Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMap1Texture.bin"
endseg

beginseg
    name "emptyMap2Texture"
    flags RAW
    include "bin/maps/unknown/sprites/emptyMap2Texture.bin"
endseg

beginseg
    name "farmPondTexture"
    flags RAW
    include "bin/maps/farm/sprites/farmPondTexture.bin"
endseg

beginseg
    name "farmPondAssetsIndex"
    flags RAW
    include "bin/maps/farm/sprites/farmPondAssetsIndex.bin"
endseg

beginseg
    name "farmPondSpritesheetIndex"
    flags RAW
    include "bin/maps/farm/sprites/farmPondSpritesheetIndex.bin"
endseg

// ============================================================================
// Misc. sprites
// ============================================================================

beginseg
    name "funeralSpritesTexture"
    flags RAW
    include "bin/sprites/cutscenes/funeral/funeralSpritesTexture.bin"
endseg

beginseg
    name "funeralSpritesIndex"
    flags RAW
    include "bin/sprites/cutscenes/funeral/funeralSpritesIndex.bin"
endseg

beginseg
    name "moonMountainBackgroundTexture"
    flags RAW
    include "bin/sprites/cutscenes/moonMountainBackgroundTexture.bin"
endseg

beginseg
    name "moonMountainBackgroundAssetsIndex"
    flags RAW
    include "bin/sprites/cutscenes/moonMountainBackgroundAssetsIndex.bin"
endseg

beginseg
    name "horseRaceTexture"
    flags RAW
    include "bin/sprites/horse-race/horseRaceTexture.bin"
endseg

beginseg
    name "horseRaceAssetsIndex"
    flags RAW
    include "bin/sprites/horse-race/horseRaceAssetsIndex.bin"
endseg

beginseg
    name "emptyOverlayScreen1Texture"
    flags RAW
    include "bin/sprites/unknown/emptyOverlayScreen1Texture.bin"
endseg

beginseg
    name "splashTexture"
    flags RAW
    include "bin/sprites/effects/splashTexture.bin"
endseg

beginseg
    name "splashAssetsIndex"
    flags RAW
    include "bin/sprites/effects/splashAssetsIndex.bin"
endseg

beginseg
    name "sparklersTexture"
    flags RAW
    include "bin/sprites/effects/sparklersTexture.bin"
endseg

beginseg
    name "sparklersAssetsIndex"
    flags RAW
    include "bin/sprites/effects/sparklersAssetsIndex.bin"
endseg

beginseg
    name "chimneySmokeTexture"
    flags RAW
    include "bin/sprites/effects/chimneySmokeTexture.bin"
endseg

beginseg
    name "chimneySmokeAssetsIndex"
    flags RAW
    include "bin/sprites/effects/chimneySmokeAssetsIndex.bin"
endseg

beginseg
    name "balloonsTexture"
    flags RAW
    include "bin/sprites/effects/balloonsTexture.bin"
endseg

beginseg
    name "balloonsAssetsIndex"
    flags RAW
    include "bin/sprites/effects/balloonsAssetsIndex.bin"
endseg

beginseg
    name "smallCloudTexture"
    flags RAW
    include "bin/sprites/effects/smallCloudTexture.bin"
endseg

beginseg
    name "smallCloudAssetsIndex"
    flags RAW
    include "bin/sprites/effects/smallCloudAssetsIndex.bin"
endseg

beginseg
    name "fairiesTexture"
    flags RAW
    include "bin/sprites/effects/fairiesTexture.bin"
endseg

beginseg
    name "fairiesTextureAssetsIndex"
    flags RAW
    include "bin/sprites/effects/fairiesTextureAssetsIndex.bin"
endseg

beginseg
    name "firefliesTexture"
    flags RAW
    include "bin/sprites/items/firefliesTexture.bin"
endseg

beginseg
    name "firefliesAssetsIndex"
    flags RAW
    include "bin/sprites/items/firefliesAssetsIndex.bin"
endseg

// ============================================================================
// Overlay screen sprites
// ============================================================================

beginseg
    name "bulletBlinkingTexture"
    flags RAW
    include "bin/sprites/overlay-screens/bulletBlinkingTexture.bin"
endseg

beginseg
    name "bulletBlinkingAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/bulletBlinkingAssetsIndex.bin"
endseg

beginseg
    name "bulletNumeralTexture"
    flags RAW
    include "bin/sprites/overlay-screens/bulletNumeralTexture.bin"
endseg

beginseg
    name "bulletNumeralAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/bulletNumeralAssetsIndex.bin"
endseg

beginseg
    name "checkerboardBackgroundTexture"
    flags RAW
    include "bin/sprites/overlay-screens/checkerboardBackgroundTexture.bin"
endseg

beginseg
    name "checkerboardBackgroundAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/checkerboardBackgroundAssetsIndex.bin"
endseg

beginseg
    name "emptyOverlayScreen2Texture"
    flags RAW
    include "bin/sprites/overlay-screens/emptyOverlayScreen2Texture.bin"
endseg

beginseg
    name "emptyOverlayScreen3Texture"
    flags RAW
    include "bin/sprites/overlay-screens/emptyOverlayScreen3Texture.bin"
endseg

beginseg
    name "namingScreenBackgroundTexture"
    flags RAW
    include "bin/sprites/overlay-screens/namingScreenBackgroundTexture.bin"
endseg

beginseg
    name "namingScreenBackgroundAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/namingScreenBackgroundAssetsIndex.bin"
endseg

beginseg
    name "bookTexture"
    flags RAW
    include "bin/sprites/overlay-screens/bookTexture.bin"
endseg

beginseg
    name "bookAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/bookAssetsIndex.bin"
endseg

beginseg
    name "photoSmallTexture"
    flags RAW
    include "bin/sprites/overlay-screens/photoSmallTexture.bin"
endseg

beginseg
    name "photoSmallAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/photoSmallAssetsIndex.bin"
endseg

beginseg
    name "snapshotLargeTexture"
    flags RAW
    include "bin/sprites/overlay-screens/snapshotLargeTexture.bin"
endseg

beginseg
    name "snapshotLargeAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/snapshotLargeAssetsIndex.bin"
endseg

beginseg
    name "namingScreen1Texture"
    flags RAW
    include "bin/sprites/naming-screen/namingScreen1Texture.bin"
endseg

beginseg
    name "namingScreen1AssetsIndex"
    flags RAW
    include "bin/sprites/naming-screen/namingScreen1AssetsIndex.bin"
endseg

beginseg
    name "calendarTexture"
    flags RAW
    include "bin/sprites/overlay-screens/calendar/calendarTexture.bin"
endseg

beginseg
    name "calendarAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/calendar/calendarAssetsIndex.bin"
endseg

beginseg
    name "calenderJapanTexture"
    flags RAW
    include "bin/sprites/overlay-screens/calenderJapanTexture.bin"
endseg

beginseg
    name "calenderJapanAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/calenderJapanAssetsIndex.bin"
endseg

beginseg
    name "bookScreenLabelsTexture"
    flags RAW
    include "bin/sprites/overlay-screens/bookScreenLabelsTexture.bin"
endseg

beginseg
    name "bookScreenLabelsAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/bookScreenLabelsAssetsIndex.bin"
endseg

beginseg
    name "japaneseCharacterSelectTexture"
    flags RAW
    include "bin/sprites/overlay-screens/japaneseCharacterSelectTexture.bin"
endseg

beginseg
    name "japaneseCharacterSelectAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/japaneseCharacterSelectAssetsIndex.bin"
endseg

beginseg
    name "overlayScreenNamesTexture"
    flags RAW
    include "bin/sprites/overlay-screens/overlayScreenNamesTexture.bin"
endseg

beginseg
    name "overlayScreenNamesAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/overlayScreenNamesAssetsIndex.bin"
endseg

beginseg
    name "moneyTexture"
    flags RAW
    include "bin/sprites/overlay-screens/moneyTexture.bin"
endseg

beginseg
    name "moneyAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/moneyAssetsIndex.bin"
endseg

beginseg
    name "powerNutTreeDrawingTexture"
    flags RAW
    include "bin/sprites/overlay-screens/powerNutTreeDrawingTexture.bin"
endseg

beginseg
    name "powerNutTreeDrawingAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/powerNutTreeDrawingAssetsIndex.bin"
endseg

beginseg
    name "inventoryScreenTexture"
    flags RAW
    include "bin/sprites/overlay-screens/inventoryScreenTexture.bin"
endseg

beginseg
    name "inventoryScreenAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/inventoryScreenAssetsIndex.bin"
endseg

beginseg
    name "prizeScreenTexture"
    flags RAW
    include "bin/sprites/overlay-screens/prizeScreenTexture.bin"
endseg

beginseg
    name "prizeScreenAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/prizeScreenAssetsIndex.bin"
endseg

beginseg
    name "loadGameScreenTexture"
    flags RAW
    include "bin/sprites/overlay-screens/loadGameScreenTexture.bin"
endseg

beginseg
    name "loadGameScreenAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/loadGameScreenAssetsIndex.bin"
endseg

beginseg
    name "messageBoxTexture"
    flags RAW
    include "bin/sprites/dialogue-box/messageBoxTexture.bin"
endseg

beginseg
    name "messageBoxAssetsIndex"
    flags RAW
    include "bin/sprites/dialogue-box/messageBoxAssetsIndex.bin"
endseg

beginseg
    name "namingScreen2Texture"
    flags RAW
    include "bin/sprites/naming-screen/namingScreen2Texture.bin"
endseg

beginseg
    name "namingScreen2AssetsIndex"
    flags RAW
    include "bin/sprites/naming-screen/namingScreen2AssetsIndex.bin"
endseg

beginseg
    name "estimateScreenTexture"
    flags RAW
    include "bin/sprites/overlay-screens/estimateScreen/estimateScreenTexture.bin"
endseg

beginseg
    name "estimateScreenAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/estimateScreen/estimateScreenAssetsIndex.bin"
endseg

beginseg
    name "raceBettingUiTexture"
    flags RAW
    include "bin/sprites/overlay-screens/races/raceBettingUiTexture.bin"
endseg

beginseg
    name "raceBettingUiAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/races/raceBettingUiAssetsIndex.bin"
endseg

beginseg
    name "emptyOverlayScreen4Texture"
    flags RAW
    include "bin/sprites/overlay-screens/emptyOverlayScreen4Texture.bin"
endseg

beginseg
    name "timeUiTexture"
    flags RAW
    include "bin/sprites/overlay-screens/timeUiTexture.bin"
endseg

beginseg
    name "timeUiAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/timeUiAssetsIndex.bin"
endseg

beginseg
    name "houseExtensionSelectionTexture"
    flags RAW
    include "bin/sprites/overlay-screens/houseExtensionSelectionTexture.bin"
endseg

beginseg
    name "houseExtensionSelectionAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/houseExtensionSelectionAssetsIndex.bin"
endseg

beginseg
    name "shopIconsTexture"
    flags RAW
    include "bin/sprites/overlay-screens/shop/shopIconsTexture.bin"
endseg

beginseg
    name "shopIconsAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/shop/shopIconsAssetsIndex.bin"
endseg

beginseg
    name "rankingsTexture"
    flags RAW
    include "bin/sprites/overlay-screens/rankingsTexture.bin"
endseg

beginseg
    name "rankingsAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/rankingsAssetsIndex.bin"
endseg

beginseg
    name "guidebookTexture"
    flags RAW
    include "bin/sprites/overlay-screens/guidebookTexture.bin"
endseg

beginseg
    name "guidebookAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/guidebookAssetsIndex.bin"
endseg

beginseg
    name "dialogueIconsTexture"
    flags RAW
    include "bin/sprites/dialogue-icons/dialogueIconsTexture.bin"
endseg

beginseg
    name "dialogueIconsAssetsIndex"
    flags RAW
    include "bin/sprites/dialogue-icons/dialogueIconsAssetsIndex.bin"
endseg

beginseg
    name "tvContentTexture"
    flags RAW
    include "bin/sprites/tv/tvContentTexture.bin"
endseg

beginseg
    name "tvContentAssetsIndex"
    flags RAW
    include "bin/sprites/tv/tvContentAssetsIndex.bin"
endseg

beginseg
    name "tvContentSpritesheetIndex"
    flags RAW
    include "bin/sprites/tv/tvContentSpritesheetIndex.bin"
endseg

beginseg
    name "toolsTexture"
    flags RAW
    include "bin/sprites/tools/toolsTexture.bin"
endseg

beginseg
    name "toolsAssetsIndex"
    flags RAW
    include "bin/sprites/tools/toolsAssetsIndex.bin"
endseg

beginseg
    name "toolsSpritesheetIndex"
    flags RAW
    include "bin/sprites/tools/toolsSpritesheetIndex.bin"
endseg

beginseg
    name "holdableItemsTexture"
    flags RAW
    include "bin/sprites/objects/holdableItemsTexture.bin"
endseg

beginseg
    name "holdableItemsAssetsIndex"
    flags RAW
    include "bin/sprites/objects/holdableItemsAssetsIndex.bin"
endseg

beginseg
    name "holdableItemsSpritesheetIndex"
    flags RAW
    include "bin/sprites/objects/holdableItemsSpritesheetIndex.bin"
endseg

beginseg
    name "extraFestivalTexture"
    flags RAW
    include "bin/sprites/festivals/extraFestivalTexture.bin"
endseg

beginseg
    name "extraFestivalAssetsIndex"
    flags RAW
    include "bin/sprites/festivals/extraFestivalAssetsIndex.bin"
endseg

beginseg
    name "extraFestivalSpritesheetIndex"
    flags RAW
    include "bin/sprites/festivals/extraFestivalSpritesheetIndex.bin"
endseg

beginseg
    name "homeItemsTexture"
    flags RAW
    include "bin/sprites/objects/homeItemsTexture.bin"
endseg

beginseg
    name "homeItemsAssetsIndex"
    flags RAW
    include "bin/sprites/objects/homeItemsAssetsIndex.bin"
endseg

beginseg
    name "homeItemsSpritesheetIndex"
    flags RAW
    include "bin/sprites/objects/homeItemsSpritesheetIndex.bin"
endseg

beginseg
    name "groundObjectsTexture"
    flags RAW
    include "bin/sprites/objects/groundObjectsTexture.bin"
endseg

beginseg
    name "groundObjectsIndex"
    flags RAW
    include "bin/sprites/objects/groundObjectsIndex.bin"
endseg

beginseg
    name "rainTexture"
    flags RAW
    include "bin/sprites/weather/rainTexture.bin"
endseg

beginseg
    name "rainAssetsIndex"
    flags RAW
    include "bin/sprites/weather/rainAssetsIndex.bin"
endseg

beginseg
    name "characterDialogueIconsTexture"
    flags RAW
    include "bin/sprites/dialogue-icons/characterDialogueIconsTexture.bin"
endseg

beginseg
    name "characterDialogueIconsAssetsIndex"
    flags RAW
    include "bin/sprites/dialogue-icons/characterDialogueIconsAssetsIndex.bin"
endseg

beginseg
    name "characterDialogueIconsSpritesheetIndex"
    flags RAW
    include "bin/sprites/dialogue-icons/characterDialogueIconsSpritesheetIndex.bin"
endseg

beginseg
    name "titleSpritesTexture"
    flags RAW
    include "bin/sprites/title/titleSpritesTexture.bin"
endseg

beginseg
    name "titleSpritesAssetsIndex"
    flags RAW
    include "bin/sprites/title/titleSpritesAssetsIndex.bin"
endseg

beginseg
    name "controllerIconsTexture"
    flags RAW
    include "bin/sprites/overlay-screens/controller/controllerIconsTexture.bin"
endseg

beginseg
    name "controllerIconsAssetsIndex"
    flags RAW
    include "bin/sprites/overlay-screens/controller/controllerIconsAssetsIndex.bin"
endseg

beginseg
    name "logoTextures"
    flags RAW
    include "bin/sprites/logo/logoTextures.bin"
endseg

beginseg
    name "logoAssetsIndex"
    flags RAW
    include "bin/sprites/logo/logoAssetsIndex.bin"
endseg

beginseg
    name "creditsTexture"
    flags RAW
    include "bin/sprites/credits/creditsTexture.bin"
endseg

beginseg
    name "creditsAssetsIndex"
    flags RAW
    include "bin/sprites/credits/creditsAssetsIndex.bin"
endseg

beginseg
    name "albumPhotosTexture"
    flags RAW
    include "bin/sprites/album-photos/albumPhotosTexture.bin"
endseg

beginseg
    name "albumPhotosAssetsIndex"
    flags RAW
    include "bin/sprites/album-photos/albumPhotosAssetsIndex.bin"
endseg

beginseg
    name "albumPhotosSpritesheetIndex"
    flags RAW
    include "bin/sprites/album-photos/albumPhotosSpritesheetIndex.bin"
endseg

beginseg
    name "villageMapScreenTexture"
    flags RAW
    include "bin/sprites/misc/villageMapScreenTexture.bin"
endseg

beginseg
    name "villageMapScreenAssetsIndex"
    flags RAW
    include "bin/sprites/misc/villageMapScreenAssetsIndex.bin"
endseg

// ============================================================================
// Fonts
// ============================================================================

beginseg
    name "fontTexture"
    flags RAW
    include "bin/sprites/font/fontTexture.bin"
endseg

beginseg
    name "fontPalette1"
    flags RAW
    include "bin/sprites/font/fontPalette1.bin"
endseg

beginseg
    name "fontPalette2"
    flags RAW
    include "bin/sprites/font/fontPalette2.bin"
endseg

beginseg
    name "fontPalette3"
    flags RAW
    include "bin/sprites/font/fontPalette3.bin"
endseg

// ============================================================================
// Texts
// ============================================================================

beginseg
    name "text1Text"
    flags RAW
    include "bin/text/text1Text.bin"
endseg

beginseg
    name "text1TextIndex"
    flags RAW
    include "bin/text/text1TextIndex.bin"
endseg

beginseg
    name "libraryText"
    flags RAW
    include "bin/text/libraryText.bin"
endseg

beginseg
    name "libraryTextIndex"
    flags RAW
    include "bin/text/libraryTextIndex.bin"
endseg

beginseg
    name "diaryText"
    flags RAW
    include "bin/text/diaryText.bin"
endseg

beginseg
    name "diaryTextIndex"
    flags RAW
    include "bin/text/diaryTextIndex.bin"
endseg

beginseg
    name "text4Text"
    flags RAW
    include "bin/text/text4Text.bin"
endseg

beginseg
    name "text4TextIndex"
    flags RAW
    include "bin/text/text4TextIndex.bin"
endseg

beginseg
    name "festivalOverlaySelectionsText"
    flags RAW
    include "bin/text/festivalOverlaySelectionsText.bin"
endseg

beginseg
    name "festivalOverlaySelectionsTextIndex"
    flags RAW
    include "bin/text/festivalOverlaySelectionsTextIndex.bin"
endseg

beginseg
    name "lettersText"
    flags RAW
    include "bin/text/lettersText.bin"
endseg

beginseg
    name "lettersTextIndex"
    flags RAW
    include "bin/text/lettersTextIndex.bin"
endseg

beginseg
    name "shopText"
    flags RAW
    include "bin/text/shopText.bin"
endseg

beginseg
    name "shopTextIndex"
    flags RAW
    include "bin/text/shopTextIndex.bin"
endseg

beginseg
    name "animalInteractionsText"
    flags RAW
    include "bin/text/animalInteractionsText.bin"
endseg

beginseg
    name "animalInteractionsTextIndex"
    flags RAW
    include "bin/text/animalInteractionsTextIndex.bin"
endseg

beginseg
    name "tvText"
    flags RAW
    include "bin/text/tvText.bin"
endseg

beginseg
    name "tvTextIndex"
    flags RAW
    include "bin/text/tvTextIndex.bin"
endseg

beginseg
    name "text10Text"
    flags RAW
    include "bin/text/text10Text.bin"
endseg

beginseg
    name "text10TextIndex"
    flags RAW
    include "bin/text/text10TextIndex.bin"
endseg

beginseg
    name "namingScreenText"
    flags RAW
    include "bin/text/namingScreenText.bin"
endseg

beginseg
    name "namingScreenTextIndex"
    flags RAW
    include "bin/text/namingScreenTextIndex.bin"
endseg

beginseg
    name "elliText"
    flags RAW
    include "bin/text/elliText.bin"
endseg

beginseg
    name "elliTextIndex"
    flags RAW
    include "bin/text/elliTextIndex.bin"
endseg

beginseg
    name "kaiText"
    flags RAW
    include "bin/text/kaiText.bin"
endseg

beginseg
    name "kaiTextIndex"
    flags RAW
    include "bin/text/kaiTextIndex.bin"
endseg

beginseg
    name "karenText"
    flags RAW
    include "bin/text/karenText.bin"
endseg

beginseg
    name "karenTextIndex"
    flags RAW
    include "bin/text/karenTextIndex.bin"
endseg

beginseg
    name "gotzText"
    flags RAW
    include "bin/text/gotzText.bin"
endseg

beginseg
    name "gotzTextIndex"
    flags RAW
    include "bin/text/gotzTextIndex.bin"
endseg

beginseg
    name "sashaText"
    flags RAW
    include "bin/text/sashaText.bin"
endseg

beginseg
    name "sashaTextIndex"
    flags RAW
    include "bin/text/sashaTextIndex.bin"
endseg

beginseg
    name "cliffText"
    flags RAW
    include "bin/text/cliffText.bin"
endseg

beginseg
    name "cliffTextIndex"
    flags RAW
    include "bin/text/cliffTextIndex.bin"
endseg

beginseg
    name "jeffText"
    flags RAW
    include "bin/text/jeffText.bin"
endseg

beginseg
    name "jeffTextIndex"
    flags RAW
    include "bin/text/jeffTextIndex.bin"
endseg

beginseg
    name "kentText"
    flags RAW
    include "bin/text/kentText.bin"
endseg

beginseg
    name "kentTextIndex"
    flags RAW
    include "bin/text/kentTextIndex.bin"
endseg

beginseg
    name "harrisText"
    flags RAW
    include "bin/text/harrisText.bin"
endseg

beginseg
    name "harrisTextIndex"
    flags RAW
    include "bin/text/harrisTextIndex.bin"
endseg

beginseg
    name "popuriText"
    flags RAW
    include "bin/text/popuriText.bin"
endseg

beginseg
    name "popuriTextIndex"
    flags RAW
    include "bin/text/popuriTextIndex.bin"
endseg

beginseg
    name "mariaText"
    flags RAW
    include "bin/text/mariaText.bin"
endseg

beginseg
    name "mariaTextIndex"
    flags RAW
    include "bin/text/mariaTextIndex.bin"
endseg

beginseg
    name "mayText"
    flags RAW
    include "bin/text/mayText.bin"
endseg

beginseg
    name "mayTextIndex"
    flags RAW
    include "bin/text/mayTextIndex.bin"
endseg

beginseg
    name "annText"
    flags RAW
    include "bin/text/annText.bin"
endseg

beginseg
    name "annTextIndex"
    flags RAW
    include "bin/text/annTextIndex.bin"
endseg

beginseg
    name "dougText"
    flags RAW
    include "bin/text/dougText.bin"
endseg

beginseg
    name "dougTextIndex"
    flags RAW
    include "bin/text/dougTextIndex.bin"
endseg

beginseg
    name "grayText"
    flags RAW
    include "bin/text/grayText.bin"
endseg

beginseg
    name "grayTextIndex"
    flags RAW
    include "bin/text/grayTextIndex.bin"
endseg

beginseg
    name "basilText"
    flags RAW
    include "bin/text/basilText.bin"
endseg

beginseg
    name "basilTextIndex"
    flags RAW
    include "bin/text/basilTextIndex.bin"
endseg

beginseg
    name "liliaText"
    flags RAW
    include "bin/text/liliaText.bin"
endseg

beginseg
    name "liliaTextIndex"
    flags RAW
    include "bin/text/liliaTextIndex.bin"
endseg

beginseg
    name "dukeText"
    flags RAW
    include "bin/text/dukeText.bin"
endseg

beginseg
    name "dukeTextIndex"
    flags RAW
    include "bin/text/dukeTextIndex.bin"
endseg

beginseg
    name "shipperText"
    flags RAW
    include "bin/text/shipperText.bin"
endseg

beginseg
    name "shipperTextIndex"
    flags RAW
    include "bin/text/shipperTextIndex.bin"
endseg

beginseg
    name "harvestSpritesText"
    flags RAW
    include "bin/text/harvestSpritesText.bin"
endseg

beginseg
    name "harvestSpritesTextIndex"
    flags RAW
    include "bin/text/harvestSpritesTextIndex.bin"
endseg

beginseg
    name "assistantCarpentersText"
    flags RAW
    include "bin/text/assistantCarpentersText.bin"
endseg

beginseg
    name "assistantCarpentersTextIndex"
    flags RAW
    include "bin/text/assistantCarpentersTextIndex.bin"
endseg

beginseg
    name "masterCarpenterText"
    flags RAW
    include "bin/text/masterCarpenterText.bin"
endseg

beginseg
    name "masterCarpenterTextIndex"
    flags RAW
    include "bin/text/masterCarpenterTextIndex.bin"
endseg

beginseg
    name "mayorText"
    flags RAW
    include "bin/text/mayorText.bin"
endseg

beginseg
    name "mayorTextIndex"
    flags RAW
    include "bin/text/mayorTextIndex.bin"
endseg

beginseg
    name "gregText"
    flags RAW
    include "bin/text/gregText.bin"
endseg

beginseg
    name "gregTextIndex"
    flags RAW
    include "bin/text/gregTextIndex.bin"
endseg

beginseg
    name "rickText"
    flags RAW
    include "bin/text/rickText.bin"
endseg

beginseg
    name "rickTextIndex"
    flags RAW
    include "bin/text/rickTextIndex.bin"
endseg

beginseg
    name "barleyText"
    flags RAW
    include "bin/text/barleyText.bin"
endseg

beginseg
    name "barleyTextIndex"
    flags RAW
    include "bin/text/barleyTextIndex.bin"
endseg

beginseg
    name "sydneyText"
    flags RAW
    include "bin/text/sydneyText.bin"
endseg

beginseg
    name "sydneyTextIndex"
    flags RAW
    include "bin/text/sydneyTextIndex.bin"
endseg

beginseg
    name "potionShopDealerText"
    flags RAW
    include "bin/text/potionShopDealerText.bin"
endseg

beginseg
    name "potionShopDealerTextIndex"
    flags RAW
    include "bin/text/potionShopDealerTextIndex.bin"
endseg

beginseg
    name "mayorWifeText"
    flags RAW
    include "bin/text/mayorWifeText.bin"
endseg

beginseg
    name "mayorWifeTextIndex"
    flags RAW
    include "bin/text/mayorWifeTextIndex.bin"
endseg

beginseg
    name "ellenText"
    flags RAW
    include "bin/text/ellenText.bin"
endseg

beginseg
    name "ellenTextIndex"
    flags RAW
    include "bin/text/ellenTextIndex.bin"
endseg

beginseg
    name "stuText"
    flags RAW
    include "bin/text/stuText.bin"
endseg

beginseg
    name "stuTextIndex"
    flags RAW
    include "bin/text/stuTextIndex.bin"
endseg

beginseg
    name "midwifeText"
    flags RAW
    include "bin/text/midwifeText.bin"
endseg

beginseg
    name "midwifeTextIndex"
    flags RAW
    include "bin/text/midwifeTextIndex.bin"
endseg

beginseg
    name "pastorText"
    flags RAW
    include "bin/text/pastorText.bin"
endseg

beginseg
    name "pastorTextIndex"
    flags RAW
    include "bin/text/pastorTextIndex.bin"
endseg

beginseg
    name "saibaraText"
    flags RAW
    include "bin/text/saibaraText.bin"
endseg

beginseg
    name "saibaraTextIndex"
    flags RAW
    include "bin/text/saibaraTextIndex.bin"
endseg

beginseg
    name "cutscenes1Text"
    flags RAW
    include "bin/text/cutscenes1Text.bin"
endseg

beginseg
    name "cutscenes1TextIndex"
    flags RAW
    include "bin/text/cutscenes1TextIndex.bin"
endseg

beginseg
    name "funeralIntroText"
    flags RAW
    include "bin/text/funeralIntroText.bin"
endseg

beginseg
    name "funeralIntroTextIndex"
    flags RAW
    include "bin/text/funeralIntroTextIndex.bin"
endseg

beginseg
    name "text48Text"
    flags RAW
    include "bin/text/text48Text.bin"
endseg

beginseg
    name "text48TextIndex"
    flags RAW
    include "bin/text/text48TextIndex.bin"
endseg

beginseg
    name "text49Text"
    flags RAW
    include "bin/text/text49Text.bin"
endseg

beginseg
    name "text49TextIndex"
    flags RAW
    include "bin/text/text49TextIndex.bin"
endseg

beginseg
    name "text50Text"
    flags RAW
    include "bin/text/text50Text.bin"
endseg

beginseg
    name "text50TextIndex"
    flags RAW
    include "bin/text/text50TextIndex.bin"
endseg

beginseg
    name "text51Text"
    flags RAW
    include "bin/text/text51Text.bin"
endseg

beginseg
    name "text51TextIndex"
    flags RAW
    include "bin/text/text51TextIndex.bin"
endseg

beginseg
    name "text52Text"
    flags RAW
    include "bin/text/text52Text.bin"
endseg

beginseg
    name "text52TextIndex"
    flags RAW
    include "bin/text/text52TextIndex.bin"
endseg

beginseg
    name "dogRaceText"
    flags RAW
    include "bin/text/dogRaceText.bin"
endseg

beginseg
    name "dogRaceTextIndex"
    flags RAW
    include "bin/text/dogRaceTextIndex.bin"
endseg

beginseg
    name "text54Text"
    flags RAW
    include "bin/text/text54Text.bin"
endseg

beginseg
    name "text54TextIndex"
    flags RAW
    include "bin/text/text54TextIndex.bin"
endseg

beginseg
    name "text55Text"
    flags RAW
    include "bin/text/text55Text.bin"
endseg

beginseg
    name "text55TextIndex"
    flags RAW
    include "bin/text/text55TextIndex.bin"
endseg

beginseg
    name "text56Text"
    flags RAW
    include "bin/text/text56Text.bin"
endseg

beginseg
    name "text56TextIndex"
    flags RAW
    include "bin/text/text56TextIndex.bin"
endseg

beginseg
    name "newYearFestivalText"
    flags RAW
    include "bin/text/newYearFestivalText.bin"
endseg

beginseg
    name "newYearFestivalTextIndex"
    flags RAW
    include "bin/text/newYearFestivalTextIndex.bin"
endseg

beginseg
    name "spiritFestivalText"
    flags RAW
    include "bin/text/spiritFestivalText.bin"
endseg

beginseg
    name "spiritFestivalTextIndex"
    flags RAW
    include "bin/text/spiritFestivalTextIndex.bin"
endseg

beginseg
    name "horseRaceText"
    flags RAW
    include "bin/text/horseRaceText.bin"
endseg

beginseg
    name "horseRaceTextIndex"
    flags RAW
    include "bin/text/horseRaceTextIndex.bin"
endseg

beginseg
    name "marriedDialoguesText"
    flags RAW
    include "bin/text/marriedDialoguesText.bin"
endseg

beginseg
    name "marriedDialoguesTextIndex"
    flags RAW
    include "bin/text/marriedDialoguesTextIndex.bin"
endseg

beginseg
    name "text61Text"
    flags RAW
    include "bin/text/text61Text.bin"
endseg

beginseg
    name "text61TextIndex"
    flags RAW
    include "bin/text/text61TextIndex.bin"
endseg

beginseg
    name "karenCutscenes1Text"
    flags RAW
    include "bin/text/karenCutscenes1Text.bin"
endseg

beginseg
    name "karenCutscenes1TextIndex"
    flags RAW
    include "bin/text/karenCutscenes1TextIndex.bin"
endseg

beginseg
    name "text63Text"
    flags RAW
    include "bin/text/text63Text.bin"
endseg

beginseg
    name "text63TextIndex"
    flags RAW
    include "bin/text/text63TextIndex.bin"
endseg

beginseg
    name "text64Text"
    flags RAW
    include "bin/text/text64Text.bin"
endseg

beginseg
    name "text64TextIndex"
    flags RAW
    include "bin/text/text64TextIndex.bin"
endseg

beginseg
    name "text65Text"
    flags RAW
    include "bin/text/text65Text.bin"
endseg

beginseg
    name "text65TextIndex"
    flags RAW
    include "bin/text/text65TextIndex.bin"
endseg

beginseg
    name "eggFestivalText"
    flags RAW
    include "bin/text/eggFestivalText.bin"
endseg

beginseg
    name "eggFestivalTextIndex"
    flags RAW
    include "bin/text/eggFestivalTextIndex.bin"
endseg

beginseg
    name "karenCutscenes2Text"
    flags RAW
    include "bin/text/karenCutscenes2Text.bin"
endseg

beginseg
    name "karenCutscenes2TextIndex"
    flags RAW
    include "bin/text/karenCutscenes2TextIndex.bin"
endseg

beginseg
    name "vegetableFestivalText"
    flags RAW
    include "bin/text/vegetableFestivalText.bin"
endseg

beginseg
    name "vegetableFestivalTextIndex"
    flags RAW
    include "bin/text/vegetableFestivalTextIndex.bin"
endseg

beginseg
    name "babyText"
    flags RAW
    include "bin/text/babyText.bin"
endseg

beginseg
    name "babyTextIndex"
    flags RAW
    include "bin/text/babyTextIndex.bin"
endseg

beginseg
    name "mrsManaAndJohnText"
    flags RAW
    include "bin/text/mrsManaAndJohnText.bin"
endseg

beginseg
    name "mrsManaAndJohnTextIndex"
    flags RAW
    include "bin/text/mrsManaAndJohnTextIndex.bin"
endseg

beginseg
    name "additionalNpcsText"
    flags RAW
    include "bin/text/additionalNpcsText.bin"
endseg

beginseg
    name "additionalNpcsTextIndex"
    flags RAW
    include "bin/text/additionalNpcsTextIndex.bin"
endseg

beginseg
    name "howToPlayText"
    flags RAW
    include "bin/text/howToPlayText.bin"
endseg

beginseg
    name "howToPlayTextIndex"
    flags RAW
    include "bin/text/howToPlayTextIndex.bin"
endseg

// ============================================================================
// Dialogue bytecode
// ============================================================================

beginseg
    name "dialogueBytecodeSegment1"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment1.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment1Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment1Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment2"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment2.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment2Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment2Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment3"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment3.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment3Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment3Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment4"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment4.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment4Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment4Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment5"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment5.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment5Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment5Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment6"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment6.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment6Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment6Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment7"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment7.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment7Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment7Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment8"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment8.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment8Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment8Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment9"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment9.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment9Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment9Index.bin.o"
endseg

beginseg
    name "mariaDialogueBytecode"
    flags OBJECT
    include "bin/dialogues/bytecode/mariaDialogueBytecode.bin.o"
endseg

beginseg
    name "mariaDialogueBytecodeIndex"
    flags OBJECT
    include "bin/dialogues/bytecode/mariaDialogueBytecodeIndex.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment11"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment11.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment11Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment11Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment12"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment12.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment12Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment12Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment13"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment13.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment13Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment13Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment14"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment14.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment14Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment14Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment15"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment15.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment15Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment15Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment16"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment16.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment16Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment16Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment17"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment17.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment17Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment17Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment18"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment18.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment18Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment18Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment19"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment19.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment19Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment19Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment20"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment20.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment20Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment20Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment21"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment21.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment21Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment21Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment22"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment22.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment22Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment22Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment23"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment23.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment23Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment23Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment24"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment24.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment24Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment24Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment25"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment25.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment25Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment25Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment26"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment26.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment26Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment26Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment27"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment27.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment27Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment27Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment28"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment28.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment28Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment28Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment29"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment29.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment29Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment29Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment30"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment30.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment30Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment30Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment31"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment31.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment31Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment31Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment32"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment32.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment32Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment32Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment33"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment33.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment33Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment33Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment34"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment34.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment34Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment34Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment35"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment35.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment35Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment35Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment36"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment36.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment36Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment36Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment37"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment37.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment37Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment37Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment38"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment38.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment38Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment38Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment39"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment39.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment39Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment39Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment40"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment40.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment40Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment40Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment41"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment41.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment41Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment41Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment42"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment42.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment42Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment42Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment43"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment43.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment43Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment43Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment44"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment44.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment44Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment44Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment45"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment45.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment45Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment45Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment46"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment46.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment46Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment46Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment47"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment47.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment47Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment47Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment48"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment48.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment48Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment48Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment49"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment49.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment49Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment49Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment50"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment50.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment50Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment50Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment51"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment51.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment51Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment51Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment52"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment52.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment52Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment52Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment53"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment53.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment53Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment53Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment54"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment54.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment54Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment54Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment55"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment55.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment55Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment55Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment56"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment56.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment56Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment56Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment57"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment57.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment57Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment57Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment58"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment58.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment58Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment58Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment59"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment59.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment59Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment59Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment60"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment60.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment60Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment60Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment61"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment61.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment61Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment61Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment62"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment62.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment62Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment62Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment63"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment63.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment63Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment63Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment64"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment64.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment64Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment64Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment65"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment65.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment65Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment65Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment66"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment66.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment66Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment66Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment67"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment67.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment67Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment67Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment68"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment68.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment68Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment68Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment69"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment69.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment69Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment69Index.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment70"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment70.bin.o"
endseg

beginseg
    name "dialogueBytecodeSegment70Index"
    flags OBJECT
    include "bin/dialogues/bytecode/dialogueBytecodeSegment70Index.bin.o"
endseg

// ============================================================================
// Wave table
// ============================================================================

beginseg
    name "waveTablePointer"
    flags RAW
    include "bin/audio/waveTablePointer.bin.o"
endseg

beginseg
    name "waveTableData"
    flags RAW
    include "bin/audio/waveTableData.bin.o"
endseg

// ============================================================================
// Sequences
// ============================================================================

beginseg
    name "seq0"
    flags RAW
    include "bin/audio/sequences/opening.bin"
endseg

beginseg
    name "seq1"
    flags RAW
    include "bin/audio/sequences/title.bin"
endseg

beginseg
    name "seq2"
    flags RAW
    include "bin/audio/sequences/sports-festival-1.bin"
endseg

beginseg
    name "seq3"
    flags RAW
    include "bin/audio/sequences/goodbye.bin"
endseg

beginseg
    name "seq4"
    flags RAW
    include "bin/audio/sequences/spring.bin"
endseg

beginseg
    name "seq5"
    flags RAW
    include "bin/audio/sequences/summer.bin"
endseg

beginseg
    name "seq6"
    flags RAW
    include "bin/audio/sequences/autumn.bin"
endseg

beginseg
    name "seq7"
    flags RAW
    include "bin/audio/sequences/winter.bin"
endseg

beginseg
    name "seq8"
    flags RAW
    include "bin/audio/sequences/naming-screen.bin"
endseg

beginseg
    name "seq9"
    flags RAW
    include "bin/audio/sequences/festival-theme.bin"
endseg

beginseg
    name "seq10"
    flags RAW
    include "bin/audio/sequences/sports-festival-2.bin"
endseg

beginseg
    name "seq11"
    flags RAW
    include "bin/audio/sequences/spirit-festival.bin"
endseg

beginseg
    name "seq12"
    flags RAW
    include "bin/audio/sequences/dog-race.bin"
endseg

beginseg
    name "seq13"
    flags RAW
    include "bin/audio/sequences/racing-festival.bin"
endseg

beginseg
    name "seq14"
    flags RAW
    include "bin/audio/sequences/festival-dance.bin"
endseg

beginseg
    name "seq15"
    flags RAW
    include "bin/audio/sequences/mountain.bin"
endseg

beginseg
    name "seq16"
    flags RAW
    include "bin/audio/sequences/village.bin"
endseg

beginseg
    name "seq17"
    flags RAW
    include "bin/audio/sequences/pond.bin"
endseg

beginseg
    name "seq18"
    flags RAW
    include "bin/audio/sequences/church.bin"
endseg

beginseg
    name "seq19"
    flags RAW
    include "bin/audio/sequences/tavern.bin"
endseg

beginseg
    name "seq20"
    flags RAW
    include "bin/audio/sequences/wedding.bin"
endseg

beginseg
    name "seq21"
    flags RAW
    include "bin/audio/sequences/new-year-sunrise.bin"
endseg

beginseg
    name "seq22"
    flags RAW
    include "bin/audio/sequences/credits.bin"
endseg

beginseg
    name "seq23"
    flags RAW
    include "bin/audio/sequences/broken-music-box.bin"
endseg

beginseg
    name "seq24"
    flags RAW
    include "bin/audio/sequences/music-box.bin"
endseg

beginseg
    name "seq25"
    flags RAW
    include "bin/audio/sequences/flute-performance.bin"
endseg

beginseg
    name "seq26"
    flags RAW
    include "bin/audio/sequences/organ-performance.bin"
endseg

beginseg
    name "seq27"
    flags RAW
    include "bin/audio/sequences/ocarina-performance.bin"
endseg

beginseg
    name "seq28"
    flags RAW
    include "bin/audio/sequences/sleigh-bells.bin"
endseg

beginseg
    name "seq29"
    flags RAW
    include "bin/audio/sequences/bad-ending.bin"
endseg

beginseg
    name "seq30"
    flags RAW
    include "bin/audio/sequences/vocal-performance.bin"
endseg

beginseg
    name "seq31"
    flags RAW
    include "bin/audio/sequences/ensemble-performance.bin"
endseg

beginseg
    name "seq32"
    flags RAW
    include "bin/audio/sequences/drum-performance.bin"
endseg

beginseg
    name "seq33"
    flags RAW
    include "bin/audio/sequences/typhoon-ambience-1.bin"
endseg

beginseg
    name "seq34"
    flags RAW
    include "bin/audio/sequences/typhoon-ambience-2.bin"
endseg

beginseg
    name "seq35"
    flags RAW
    include "bin/audio/sequences/night-ambience-spring.bin"
endseg

beginseg
    name "seq36"
    flags RAW
    include "bin/audio/sequences/night-ambience-summer.bin"
endseg

beginseg
    name "seq37"
    flags RAW
    include "bin/audio/sequences/night-ambience-autumn.bin"
endseg

beginseg
    name "seq38"
    flags RAW
    include "bin/audio/sequences/starry-night-festival.bin"
endseg

beginseg
    name "seq39"
    flags RAW
    include "bin/audio/sequences/beach-ambience-1.bin"
endseg

beginseg
    name "seq40"
    flags RAW
    include "bin/audio/sequences/beach-ambience-2.bin"
endseg

beginseg
    name "seq41"
    flags RAW
    include "bin/audio/sequences/bird-1.bin"
endseg

beginseg
    name "seq42"
    flags RAW
    include "bin/audio/sequences/bird-2.bin"
endseg

beginseg
    name "seq43"
    flags RAW
    include "bin/audio/sequences/cicada.bin"
endseg

beginseg
    name "seq44"
    flags RAW
    include "bin/audio/sequences/seq44.bin"
endseg

beginseg
    name "seq45"
    flags RAW
    include "bin/audio/sequences/fireworksSfx.bin"
endseg

beginseg
    name "seq46"
    flags RAW
    include "bin/audio/sequences/horse-gallop-fast.bin"
endseg

beginseg
    name "seq47"
    flags RAW
    include "bin/audio/sequences/horse-gallop-1.bin"
endseg

beginseg
    name "seq48"
    flags RAW
    include "bin/audio/sequences/horse-gallop-2.bin"
endseg

beginseg
    name "seq49"
    flags RAW
    include "bin/audio/sequences/horse-gallop-3.bin"
endseg

beginseg
    name "seq50"
    flags RAW
    include "bin/audio/sequences/water-1.bin"
endseg

beginseg
    name "seq51"
    flags RAW
    include "bin/audio/sequences/water-2.bin"
endseg

beginseg
    name "seq52"
    flags RAW
    include "bin/audio/sequences/hammer.bin"
endseg

beginseg
    name "seq53"
    flags RAW
    include "bin/audio/sequences/seq53.bin"
endseg

beginseg
    name "seq54"
    flags RAW
    include "bin/audio/sequences/seq54.bin"
endseg

beginseg
    name "seq55"
    flags RAW
    include "bin/audio/sequences/seq55.bin"
endseg

beginseg
    name "seq56"
    flags RAW
    include "bin/audio/sequences/seq56.bin"
endseg

beginseg
    name "seq57"
    flags RAW
    include "bin/audio/sequences/seq57.bin"
endseg

beginseg
    name "seq58"
    flags RAW
    include "bin/audio/sequences/seq58.bin"
endseg

beginseg
    name "seq59"
    flags RAW
    include "bin/audio/sequences/seq59.bin"
endseg

// ============================================================================
// WAVE SECTION
// ============================================================================

beginwave
    name    "game"

    include "code"

    include "messageBoxBuffer"
    include "dialogueIconSpriteBuffer"
    include "characterAvatarsSpriteBuffer"
    include "shadowSpriteBuffer"
    include "playerSpriteBuffer"
    include "namingScreenBuffer"
    include "mapDataBuffer"
    include "spriteBuffer"
    include "mapObjectBuffer"
    include "cutsceneBytecodeBuffer"
    include "fontBuffer"
    include "textBuffer"
    include "messageBoxesBuffer"
    include "dialogueBytecodeBuffer"
    include "sramBuffer"
    include "farmRankingsBuffer"
    include "unknownBuffer"
    include "frameBuffer"

    include "cutsceneBank1"
    include "cutsceneBank2"
    include "cutsceneBank3"
    include "cutsceneBank4"
    include "cutsceneBank5"
    include "cutsceneBank6"
    include "cutsceneBank7"
    include "cutsceneBank8"
    include "cutsceneBank9"
    include "cutsceneBank10"
    include "cutsceneBank11"
    include "cutsceneBank12"
    include "cutsceneBank13"
    include "cutsceneBank14"
    include "fireworksFestival"
    include "cutsceneBank16"
    include "cutsceneBank17"
    include "cutsceneBank18"
    include "cutsceneBank19"
    include "cutsceneBank20"
    include "cutsceneBank21"
    include "cutsceneBank22"
    include "cutsceneBank23"
    include "cutsceneBank24"
    include "cutsceneBank25"
    include "cutsceneBank26"
    include "cutsceneBank27"

    include "playerTexture"
    include "playerAssetsIndex"
    include "playerSpritesheetIndex"

    include "mariaTexture"
    include "mariaAssetsIndex"
    include "mariaSpritesheetIndex"
    include "popuriTexture"
    include "popuriAssetsIndex"
    include "popuriSpritesheetIndex"
    include "elliTexture"
    include "elliAssetsIndex"
    include "elliSpritesheetIndex"
    include "annTexture"
    include "annAssetsIndex"
    include "annSpritesheetIndex"
    include "karenTexture"
    include "karenAssetsIndex"
    include "karenSpritesheetIndex"

    include "harrisTexture"
    include "harrisAssetsIndex"
    include "harrisSpritesheetIndex"
    include "grayTexture"
    include "grayAssetsIndex"
    include "graySpritesheetIndex"
    include "jeffTexture"
    include "jeffAssetsIndex"
    include "jeffSpritesheetIndex"
    include "cliffTexture"
    include "cliffAssetsIndex"
    include "cliffSpritesheetIndex"
    include "kaiTexture"
    include "kaiAssetsIndex"
    include "kaiSpritesheetIndex"

    include "mayorTexture"
    include "mayorAssetsIndex"
    include "mayorSpritesheetIndex"
    include "mayorWifeTexture"
    include "mayorWifeAssetsIndex"
    include "mayorWifeSpritesheetIndex"
    include "liliaTexture"
    include "liliaAssetsIndex"
    include "liliaSpritesheetIndex"
    include "basilTexture"
    include "basilAssetsIndex"
    include "basilSpritesheetIndex"
    include "ellenTexture"
    include "ellenAssetsIndex"
    include "ellenSpritesheetIndex"
    include "dougTexture"
    include "dougAssetsIndex"
    include "dougSpritesheetIndex"
    include "gotzTexture"
    include "gotzAssetsIndex"
    include "gotzSpritesheetIndex"
    include "shipperTexture"
    include "shipperAssetsIndex"
    include "shipperSpritesheetIndex"
    include "sashaTexture"
    include "sashaAssetsIndex"
    include "sashaSpritesheetIndex"
    include "dukeTexture"
    include "dukeAssetsIndex"
    include "dukeSpritesheetIndex"
    include "manaTexture"
    include "manaAssetsIndex"
    include "manaSpritesheetIndex"
    include "rickTexture"
    include "rickAssetsIndex"
    include "rickSpritesheetIndex"
    include "barleyTexture"
    include "barleyAssetsIndex"
    include "barleySpritesheetIndex"
    include "midwifeTexture"
    include "midwifeAssetsIndex"
    include "midwifeSpritesheetIndex"
    include "pastorTexture"
    include "pastorAssetsIndex"
    include "pastorSpritesheetIndex"
    include "saibaraTexture"
    include "saibaraAssetsIndex"
    include "saibaraSpritesheetIndex"
    include "potionShopDealerTexture"
    include "potionShopDealerAssetsIndex"
    include "potionShopDealerSpritesheetIndex"
    include "masterCarpenterTexture"
    include "masterCarpenterAssetsIndex"
    include "masterCarpenterSpritesheetIndex"
    include "assistantCarpentersTexture"
    include "assistantCarpentersAssetsIndex"
    include "assistantCarpentersSpritesheetIndex"

    include "kentTexture"
    include "kentAssetsIndex"
    include "kentSpritesheetIndex"
    include "stuTexture"
    include "stuAssetsIndex"
    include "stuSpritesheetIndex"
    include "mayTexture"
    include "mayAssetsIndex"
    include "maySpritesheetIndex"

    include "babyTexture"
    include "babyAssetsIndex"
    include "babySpritesheetIndex"
    include "mariaHarrisBabyTexture"
    include "mariaHarrisBabyAssetsIndex"
    include "mariaHarrisBabySpritesheetIndex"
    include "popuriGrayBabyTexture"
    include "popuriGrayBabyAssetsIndex"
    include "popuriGrayBabySpritesheetIndex"
    include "elliJeffBabyTexture"
    include "elliJeffBabyAssetsIndex"
    include "elliJeffBabySpritesheetIndex"
    include "annCliffBabyTexture"
    include "annCliffBabyAssetsIndex"
    include "annCliffBabySpritesheetIndex"
    include "karenKaiBabyTexture"
    include "karenKaiBabyAssetsIndex"
    include "karenKaiBabySpritesheetIndex"

    include "harvestSpritesTexture"
    include "harvestSpritesAssetsIndex"
    include "harvestSpritesSpritesheetIndex"
    include "harvestGoddessTexture"
    include "harvestGoddessAssetsIndex"
    include "harvestGoddessSpritesheetIndex"
    include "kappaTexture"
    include "kappaAssetsIndex"
    include "kappaSpritesheetIndex"
    include "dadTexture"
    include "dadAssetsIndex"
    include "dadSpritesheetIndex"

    include "dogTexture"
    include "dogAssetsIndex"
    include "dogSpritesheetIndex"
    include "dogTitleTexture"
    include "dogTitleAssetsIndex"
    include "dogTitleSpritesheetIndex"
    include "horsePonyTexture"
    include "horsePonyAssetsIndex"
    include "horsePonySpritesheetIndex"
    include "horseGrownTexture"
    include "horseGrownAssetsIndex"
    include "horseGrownSpritesheetIndex"
    include "horseUnbridledTexture"
    include "horseUnbridledAssetsIndex"
    include "horseUnbridledSpritesheetIndex"
    include "horseRacerTexture"
    include "horseRacerAssetsIndex"
    include "horseRacerSpritesheetIndex"
    include "chickTexture"
    include "chickAssetsIndex"
    include "chickSpritesheetIndex"
    include "chickenTexture"
    include "chickenAssetsIndex"
    include "chickenSpritesheetIndex"
    include "calfTexture"
    include "calfAssetsIndex"
    include "calfSpritesheetIndex"
    include "youngCowTexture"
    include "youngCowAssetsIndex"
    include "youngCowSpritesheetIndex"
    include "cowTexture"
    include "cowAssetsIndex"
    include "cowSpritesheetIndex"
    include "cowIndoorsTexture"
    include "cowIndoorsAssetsIndex"
    include "cowIndoorsSpritesheetIndex"
    include "lambTexture"
    include "lambAssetsIndex"
    include "lambSpritesheetIndex"
    include "sheepTexture"
    include "sheepAssetsIndex"
    include "sheepSpritesheetIndex"

    include "catTexture"
    include "catAssetsIndex"
    include "catSpritesheetIndex"
    include "frogTexture"
    include "frogAssetsIndex"
    include "frogSpritesheetIndex"
    include "snakeTexture"
    include "snakeAssetsIndex"
    include "snakeSpritesheetIndex"
    include "crabTexture"
    include "crabAssetsIndex"
    include "crabSpritesheetIndex"
    include "birdTexture"
    include "birdAssetsIndex"
    include "birdSpritesheetIndex"

    include "hornedBeetleTexture"
    include "hornedBeetleAssetsIndex"
    include "hornedBeetleSpritesheetIndex"
    include "stagBeetleTexture"
    include "stagBeetleAssetsIndex"
    include "stagBeetleSpritesheetIndex"
    include "bellRingCricketTexture"
    include "bellRingCricketAssetsIndex"
    include "bellRingCricketSpritesheetIndex"
    include "cicadaTexture"
    include "cicadaAssetsIndex"
    include "cicadaSpritesheetIndex"
    include "ladybugTexture"
    include "ladybugAssetsIndex"
    include "ladybugSpritesheetIndex"
    include "dragonflyTexture"
    include "dragonflyAssetsIndex"
    include "dragonflySpritesheetIndex"
    include "grasshopperTexture"
    include "grasshopperAssetsIndex"
    include "grasshopperSpritesheetIndex"
    include "purpleButterflyTexture"
    include "purpleButterflyAssetsIndex"
    include "purpleButterflySpritesheetIndex"

    include "shadowsTexture"
    include "shadowsIndex"
    include "expressionBubblesTexture"
    include "expressionBubblesAssetsIndex"
    include "expressionBubblesSpritesheetIndex"
    include "emptyEntityTexture"
    include "emptyEntityAssetsIndex"
    include "emptyEntitySpritesheetIndex"

    include "ranchSpringMap"
    include "ranchSummerMap"
    include "ranchFallMap"
    include "ranchWinterMap"
    include "annsRoomMap"
    include "ranchBarnMap"
    include "ranchStoreMap"
    include "ranchHouseMap"
    include "emptyMap1"

    include "beachSpringMap"
    include "beachSummerMap"
    include "beachFallMap"
    include "beachWinterMap"

    include "raceTrackSpringMap"
    include "raceTrackSummerMap"
    include "raceTrackFallMap"
    include "raceTrackWinterMap"

    include "mountain1SpringMap"
    include "mountain1SummerMap"
    include "mountain1FallMap"
    include "mountain1WinterMap"
    include "mountain2SpringMap"
    include "mountain2SummerMap"
    include "mountain2FallMap"
    include "mountain2WinterMap"
    include "topOfMountain1SpringMap"
    include "topOfMountain1SummerMap"
    include "topOfMountain1FallMap"
    include "topOfMountain1WinterMap"
    include "moonMountainSpringMap"
    include "moonMountainSummerMap"
    include "moonMountainFallMap"
    include "moonMountainWinterMap"
    include "carpentersHutMap"
    include "dumplingHouseMap"

    include "pondSpringMap"
    include "pondSummerMap"
    include "pondFallMap"
    include "pondWinterMap"

    include "ellisRoomMap"
    include "bakeryMap"

    include "village1SpringMap"
    include "village1SummerMap"
    include "village1FallMap"
    include "village1WinterMap"
    include "village2SpringMap"
    include "village2SummerMap"
    include "village2FallMap"
    include "village2WinterMap"
    include "popurisRoomMap"
    include "flowerShopMap"
    include "churchMap"
    include "souvenirShopMap"

    include "squareSpringMap"
    include "squareSummerMap"
    include "squareFallMap"
    include "squareWinterMap"
    include "ricksShopMap"
    include "midwifesHouseMap"
    include "tavernMap"
    include "libraryMap"

    include "mariasRoomMap"
    include "mayorsHouseMap"

    include "potionShopBackroomMap"
    include "potionShopMap"
    include "emptyMap2"

    include "spriteCaveMap"
    include "caveMap"
    include "emptyMineMap"
    include "mineMap"

    include "karensRoomMap"
    include "vineyardSpringMap"
    include "vineyardSummerMap"
    include "vineyardFallMap"
    include "vineyardWinterMap"
    include "vineyardHouseMap"
    include "vineyardCellar1Map"
    include "vineyardCellar2Map"

    include "roadSpringMap"
    include "roadSummerMap"
    include "roadFallMap"
    include "roadWinterMap"

    include "farmSpringMap"
    include "farmSummerMap"
    include "farmFallMap"
    include "farmWinterMap"
    include "greenhouseMap"
    include "houseMap"
    include "barnMap"
    include "coopMap"
    include "kitchenMap"
    include "bathroomMap"

    include "wavesTexture"
    include "wavesAssetsIndex"
    include "wavesSpritesheetIndex"
    include "waterTilesTexture"
    include "waterTilesAssetsIndex"
    include "waterTilesSpritesheetIndex"

    include "boulderTexture"
    include "boulderAssetsIndex"
    include "boulderSpritesheetIndex"
    include "bridgeTexture"
    include "bridgeAssetsIndex"
    include "bridgeSpritesheetIndex"
    include "stonePillarTexture"
    include "stonePillarAssetsIndex"
    include "stonePillarSpritesheetIndex"
    include "emptyMountainSprite1Texture"
    include "emptyMountainSprite2Texture"
    include "mountainPathTilesTexture"
    include "mountainPathTilesAssetsIndex"
    include "mountainPathTilesSpritesheetIndex"
    include "steamTexture"
    include "steamAssetsIndex"
    include "steamSpritesheetIndex"

    include "pondWaterTexture"
    include "pondWaterAssetsIndex"
    include "pondWaterSpritesheetIndex"

    include "festivalFlowersTexture"
    include "festivalFlowersAssetsIndex"
    include "festivalFlowersSpritesheetIndex"
    include "squareFountainTexture"
    include "squareFountainAssetsIndex"
    include "squareFountainSpritesheetIndex"
    include "village2WaterTexture"
    include "village2WaterAssetsIndex"
    include "village2WaterSpritesheetIndex"

    include "emptyMapSprite1Texture"
    include "emptyMapSprite2Texture"
    include "emptyMapSprite3Texture"
    include "emptyMapSprite4Texture"
    include "emptyMapSprite5Texture"
    include "emptyMapSprite6Texture"
    include "stoneTexture"
    include "stoneAssetsIndex"
    include "stoneSpritesheetIndex"
    include "vineyardTreeTexture"
    include "vineyardTreeAssetsIndex"
    include "vineyardTreeSpritesheetIndex"
    include "emptyMap1Texture"
    include "emptyMap2Texture"
    include "farmPondTexture"
    include "farmPondAssetsIndex"
    include "farmPondSpritesheetIndex"

    include "funeralSpritesTexture"
    include "funeralSpritesIndex"
    include "moonMountainBackgroundTexture"
    include "moonMountainBackgroundAssetsIndex"
    include "horseRaceTexture"
    include "horseRaceAssetsIndex"
    include "emptyOverlayScreen1Texture"

    include "splashTexture"
    include "splashAssetsIndex"
    include "sparklersTexture"
    include "sparklersAssetsIndex"
    include "chimneySmokeTexture"
    include "chimneySmokeAssetsIndex"
    include "balloonsTexture"
    include "balloonsAssetsIndex"
    include "smallCloudTexture"
    include "smallCloudAssetsIndex"
    include "fairiesTexture"
    include "fairiesTextureAssetsIndex"
    include "firefliesTexture"
    include "firefliesAssetsIndex"

    include "bulletBlinkingTexture"
    include "bulletBlinkingAssetsIndex"
    include "bulletNumeralTexture"
    include "bulletNumeralAssetsIndex"
    include "checkerboardBackgroundTexture"
    include "checkerboardBackgroundAssetsIndex"
    include "emptyOverlayScreen2Texture"
    include "emptyOverlayScreen3Texture"
    include "namingScreenBackgroundTexture"
    include "namingScreenBackgroundAssetsIndex"
    include "bookTexture"
    include "bookAssetsIndex"
    include "photoSmallTexture"
    include "photoSmallAssetsIndex"
    include "snapshotLargeTexture"
    include "snapshotLargeAssetsIndex"
    include "namingScreen1Texture"
    include "namingScreen1AssetsIndex"
    include "calendarTexture"
    include "calendarAssetsIndex"
    include "calenderJapanTexture"
    include "calenderJapanAssetsIndex"
    include "bookScreenLabelsTexture"
    include "bookScreenLabelsAssetsIndex"
    include "japaneseCharacterSelectTexture"
    include "japaneseCharacterSelectAssetsIndex"
    include "overlayScreenNamesTexture"
    include "overlayScreenNamesAssetsIndex"
    include "moneyTexture"
    include "moneyAssetsIndex"
    include "powerNutTreeDrawingTexture"
    include "powerNutTreeDrawingAssetsIndex"
    include "inventoryScreenTexture"
    include "inventoryScreenAssetsIndex"
    include "prizeScreenTexture"
    include "prizeScreenAssetsIndex"
    include "loadGameScreenTexture"
    include "loadGameScreenAssetsIndex"
    include "messageBoxTexture"
    include "messageBoxAssetsIndex"
    include "namingScreen2Texture"
    include "namingScreen2AssetsIndex"
    include "estimateScreenTexture"
    include "estimateScreenAssetsIndex"
    include "raceBettingUiTexture"
    include "raceBettingUiAssetsIndex"
    include "emptyOverlayScreen4Texture"
    include "timeUiTexture"
    include "timeUiAssetsIndex"
    include "houseExtensionSelectionTexture"
    include "houseExtensionSelectionAssetsIndex"
    include "shopIconsTexture"
    include "shopIconsAssetsIndex"
    include "rankingsTexture"
    include "rankingsAssetsIndex"
    include "guidebookTexture"
    include "guidebookAssetsIndex"
    include "dialogueIconsTexture"
    include "dialogueIconsAssetsIndex"
    include "tvContentTexture"
    include "tvContentAssetsIndex"
    include "tvContentSpritesheetIndex"

    include "toolsTexture"
    include "toolsAssetsIndex"
    include "toolsSpritesheetIndex"
    include "holdableItemsTexture"
    include "holdableItemsAssetsIndex"
    include "holdableItemsSpritesheetIndex"
    include "extraFestivalTexture"
    include "extraFestivalAssetsIndex"
    include "extraFestivalSpritesheetIndex"
    include "homeItemsTexture"
    include "homeItemsAssetsIndex"
    include "homeItemsSpritesheetIndex"
    include "groundObjectsTexture"
    include "groundObjectsIndex"
    include "rainTexture"
    include "rainAssetsIndex"

    include "characterDialogueIconsTexture"
    include "characterDialogueIconsAssetsIndex"
    include "characterDialogueIconsSpritesheetIndex"
    include "titleSpritesTexture"
    include "titleSpritesAssetsIndex"
    include "controllerIconsTexture"
    include "controllerIconsAssetsIndex"
    include "logoTextures"
    include "logoAssetsIndex"
    include "creditsTexture"
    include "creditsAssetsIndex"
    include "albumPhotosTexture"
    include "albumPhotosAssetsIndex"
    include "albumPhotosSpritesheetIndex"
    include "villageMapScreenTexture"
    include "villageMapScreenAssetsIndex"

    include "fontTexture"
    include "fontPalette1"
    include "fontPalette2"
    include "fontPalette3"

    include "text1Text"
    include "text1TextIndex"
    include "libraryText"
    include "libraryTextIndex"
    include "diaryText"
    include "diaryTextIndex"
    include "text4Text"
    include "text4TextIndex"
    include "festivalOverlaySelectionsText"
    include "festivalOverlaySelectionsTextIndex"
    include "lettersText"
    include "lettersTextIndex"
    include "shopText"
    include "shopTextIndex"
    include "animalInteractionsText"
    include "animalInteractionsTextIndex"
    include "tvText"
    include "tvTextIndex"
    include "text10Text"
    include "text10TextIndex"
    include "namingScreenText"
    include "namingScreenTextIndex"
    include "elliText"
    include "elliTextIndex"
    include "kaiText"
    include "kaiTextIndex"
    include "karenText"
    include "karenTextIndex"
    include "gotzText"
    include "gotzTextIndex"
    include "sashaText"
    include "sashaTextIndex"
    include "cliffText"
    include "cliffTextIndex"
    include "jeffText"
    include "jeffTextIndex"
    include "kentText"
    include "kentTextIndex"
    include "harrisText"
    include "harrisTextIndex"
    include "popuriText"
    include "popuriTextIndex"
    include "mariaText"
    include "mariaTextIndex"
    include "mayText"
    include "mayTextIndex"
    include "annText"
    include "annTextIndex"
    include "dougText"
    include "dougTextIndex"
    include "grayText"
    include "grayTextIndex"
    include "basilText"
    include "basilTextIndex"
    include "liliaText"
    include "liliaTextIndex"
    include "dukeText"
    include "dukeTextIndex"
    include "shipperText"
    include "shipperTextIndex"
    include "harvestSpritesText"
    include "harvestSpritesTextIndex"
    include "assistantCarpentersText"
    include "assistantCarpentersTextIndex"
    include "masterCarpenterText"
    include "masterCarpenterTextIndex"
    include "mayorText"
    include "mayorTextIndex"
    include "gregText"
    include "gregTextIndex"
    include "rickText"
    include "rickTextIndex"
    include "barleyText"
    include "barleyTextIndex"
    include "sydneyText"
    include "sydneyTextIndex"
    include "potionShopDealerText"
    include "potionShopDealerTextIndex"
    include "mayorWifeText"
    include "mayorWifeTextIndex"
    include "ellenText"
    include "ellenTextIndex"
    include "stuText"
    include "stuTextIndex"
    include "midwifeText"
    include "midwifeTextIndex"
    include "pastorText"
    include "pastorTextIndex"
    include "saibaraText"
    include "saibaraTextIndex"
    include "cutscenes1Text"
    include "cutscenes1TextIndex"
    include "funeralIntroText"
    include "funeralIntroTextIndex"
    include "text48Text"
    include "text48TextIndex"
    include "text49Text"
    include "text49TextIndex"
    include "text50Text"
    include "text50TextIndex"
    include "text51Text"
    include "text51TextIndex"
    include "text52Text"
    include "text52TextIndex"
    include "dogRaceText"
    include "dogRaceTextIndex"
    include "text54Text"
    include "text54TextIndex"
    include "text55Text"
    include "text55TextIndex"
    include "text56Text"
    include "text56TextIndex"
    include "newYearFestivalText"
    include "newYearFestivalTextIndex"
    include "spiritFestivalText"
    include "spiritFestivalTextIndex"
    include "horseRaceText"
    include "horseRaceTextIndex"
    include "marriedDialoguesText"
    include "marriedDialoguesTextIndex"
    include "text61Text"
    include "text61TextIndex"
    include "karenCutscenes1Text"
    include "karenCutscenes1TextIndex"
    include "text63Text"
    include "text63TextIndex"
    include "text64Text"
    include "text64TextIndex"
    include "text65Text"
    include "text65TextIndex"
    include "eggFestivalText"
    include "eggFestivalTextIndex"
    include "karenCutscenes2Text"
    include "karenCutscenes2TextIndex"
    include "vegetableFestivalText"
    include "vegetableFestivalTextIndex"
    include "babyText"
    include "babyTextIndex"
    include "mrsManaAndJohnText"
    include "mrsManaAndJohnTextIndex"
    include "additionalNpcsText"
    include "additionalNpcsTextIndex"
    include "howToPlayText"
    include "howToPlayTextIndex"

    include "dialogueBytecodeSegment1"
    include "dialogueBytecodeSegment1Index"
    include "dialogueBytecodeSegment2"
    include "dialogueBytecodeSegment2Index"
    include "dialogueBytecodeSegment3"
    include "dialogueBytecodeSegment3Index"
    include "dialogueBytecodeSegment4"
    include "dialogueBytecodeSegment4Index"
    include "dialogueBytecodeSegment5"
    include "dialogueBytecodeSegment5Index"
    include "dialogueBytecodeSegment6"
    include "dialogueBytecodeSegment6Index"
    include "dialogueBytecodeSegment7"
    include "dialogueBytecodeSegment7Index"
    include "dialogueBytecodeSegment8"
    include "dialogueBytecodeSegment8Index"
    include "dialogueBytecodeSegment9"
    include "dialogueBytecodeSegment9Index"
    include "mariaDialogueBytecode"
    include "mariaDialogueBytecodeIndex"
    include "dialogueBytecodeSegment11"
    include "dialogueBytecodeSegment11Index"
    include "dialogueBytecodeSegment12"
    include "dialogueBytecodeSegment12Index"
    include "dialogueBytecodeSegment13"
    include "dialogueBytecodeSegment13Index"
    include "dialogueBytecodeSegment14"
    include "dialogueBytecodeSegment14Index"
    include "dialogueBytecodeSegment15"
    include "dialogueBytecodeSegment15Index"
    include "dialogueBytecodeSegment16"
    include "dialogueBytecodeSegment16Index"
    include "dialogueBytecodeSegment17"
    include "dialogueBytecodeSegment17Index"
    include "dialogueBytecodeSegment18"
    include "dialogueBytecodeSegment18Index"
    include "dialogueBytecodeSegment19"
    include "dialogueBytecodeSegment19Index"
    include "dialogueBytecodeSegment20"
    include "dialogueBytecodeSegment20Index"
    include "dialogueBytecodeSegment21"
    include "dialogueBytecodeSegment21Index"
    include "dialogueBytecodeSegment22"
    include "dialogueBytecodeSegment22Index"
    include "dialogueBytecodeSegment23"
    include "dialogueBytecodeSegment23Index"
    include "dialogueBytecodeSegment24"
    include "dialogueBytecodeSegment24Index"
    include "dialogueBytecodeSegment25"
    include "dialogueBytecodeSegment25Index"
    include "dialogueBytecodeSegment26"
    include "dialogueBytecodeSegment26Index"
    include "dialogueBytecodeSegment27"
    include "dialogueBytecodeSegment27Index"
    include "dialogueBytecodeSegment28"
    include "dialogueBytecodeSegment28Index"
    include "dialogueBytecodeSegment29"
    include "dialogueBytecodeSegment29Index"
    include "dialogueBytecodeSegment30"
    include "dialogueBytecodeSegment30Index"
    include "dialogueBytecodeSegment31"
    include "dialogueBytecodeSegment31Index"
    include "dialogueBytecodeSegment32"
    include "dialogueBytecodeSegment32Index"
    include "dialogueBytecodeSegment33"
    include "dialogueBytecodeSegment33Index"
    include "dialogueBytecodeSegment34"
    include "dialogueBytecodeSegment34Index"
    include "dialogueBytecodeSegment35"
    include "dialogueBytecodeSegment35Index"
    include "dialogueBytecodeSegment36"
    include "dialogueBytecodeSegment36Index"
    include "dialogueBytecodeSegment37"
    include "dialogueBytecodeSegment37Index"
    include "dialogueBytecodeSegment38"
    include "dialogueBytecodeSegment38Index"
    include "dialogueBytecodeSegment39"
    include "dialogueBytecodeSegment39Index"
    include "dialogueBytecodeSegment40"
    include "dialogueBytecodeSegment40Index"
    include "dialogueBytecodeSegment41"
    include "dialogueBytecodeSegment41Index"
    include "dialogueBytecodeSegment42"
    include "dialogueBytecodeSegment42Index"
    include "dialogueBytecodeSegment43"
    include "dialogueBytecodeSegment43Index"
    include "dialogueBytecodeSegment44"
    include "dialogueBytecodeSegment44Index"
    include "dialogueBytecodeSegment45"
    include "dialogueBytecodeSegment45Index"
    include "dialogueBytecodeSegment46"
    include "dialogueBytecodeSegment46Index"
    include "dialogueBytecodeSegment47"
    include "dialogueBytecodeSegment47Index"
    include "dialogueBytecodeSegment48"
    include "dialogueBytecodeSegment48Index"
    include "dialogueBytecodeSegment49"
    include "dialogueBytecodeSegment49Index"
    include "dialogueBytecodeSegment50"
    include "dialogueBytecodeSegment50Index"
    include "dialogueBytecodeSegment51"
    include "dialogueBytecodeSegment51Index"
    include "dialogueBytecodeSegment52"
    include "dialogueBytecodeSegment52Index"
    include "dialogueBytecodeSegment53"
    include "dialogueBytecodeSegment53Index"
    include "dialogueBytecodeSegment54"
    include "dialogueBytecodeSegment54Index"
    include "dialogueBytecodeSegment55"
    include "dialogueBytecodeSegment55Index"
    include "dialogueBytecodeSegment56"
    include "dialogueBytecodeSegment56Index"
    include "dialogueBytecodeSegment57"
    include "dialogueBytecodeSegment57Index"
    include "dialogueBytecodeSegment58"
    include "dialogueBytecodeSegment58Index"
    include "dialogueBytecodeSegment59"
    include "dialogueBytecodeSegment59Index"
    include "dialogueBytecodeSegment60"
    include "dialogueBytecodeSegment60Index"
    include "dialogueBytecodeSegment61"
    include "dialogueBytecodeSegment61Index"
    include "dialogueBytecodeSegment62"
    include "dialogueBytecodeSegment62Index"
    include "dialogueBytecodeSegment63"
    include "dialogueBytecodeSegment63Index"
    include "dialogueBytecodeSegment64"
    include "dialogueBytecodeSegment64Index"
    include "dialogueBytecodeSegment65"
    include "dialogueBytecodeSegment65Index"
    include "dialogueBytecodeSegment66"
    include "dialogueBytecodeSegment66Index"
    include "dialogueBytecodeSegment67"
    include "dialogueBytecodeSegment67Index"
    include "dialogueBytecodeSegment68"
    include "dialogueBytecodeSegment68Index"
    include "dialogueBytecodeSegment69"
    include "dialogueBytecodeSegment69Index"
    include "dialogueBytecodeSegment70"
    include "dialogueBytecodeSegment70Index"

    include "waveTablePointer"
    include "waveTableData"

    include "seq0"
    include "seq1"
    include "seq2"
    include "seq3"
    include "seq4"
    include "seq5"
    include "seq6"
    include "seq7"
    include "seq8"
    include "seq9"
    include "seq10"
    include "seq11"
    include "seq12"
    include "seq13"
    include "seq14"
    include "seq15"
    include "seq16"
    include "seq17"
    include "seq18"
    include "seq19"
    include "seq20"
    include "seq21"
    include "seq22"
    include "seq23"
    include "seq24"
    include "seq25"
    include "seq26"
    include "seq27"
    include "seq28"
    include "seq29"
    include "seq30"
    include "seq31"
    include "seq32"
    include "seq33"
    include "seq34"
    include "seq35"
    include "seq36"
    include "seq37"
    include "seq38"
    include "seq39"
    include "seq40"

    include "seq41"
    include "seq42"
    include "seq43"
    include "seq44"
    include "seq45"
    include "seq46"
    include "seq47"
    include "seq48"
    include "seq49"
    include "seq50"
    include "seq51"
    include "seq52"
    include "seq53"
    include "seq54"
    include "seq55"
    include "seq56"
    include "seq57"
    include "seq58"
    include "seq59"

endwave
