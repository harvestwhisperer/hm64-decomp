#ifndef _ANIMATON_SCRIPTS_H_
#define _ANIMATON_SCRIPTS_H_

#include "common.h"

#define ANIM_TYPE_ONE_SHOT           0x0000  // type 0xFF
#define ANIM_TYPE_LOOP               0x2000  // type 0xFE
#define ANIM_TYPE_DESTROY_ON_END     0x4000  // type 0xFD

#define FLIP_HORIZONTAL              0x8000

#define PACK_ANIM_DATA(metadata_offset, anim_type, horizontal_flip) \
    ((metadata_offset) | (anim_type) | (horizontal_flip))

extern u16 playerAnimationScripts[];
extern u16 mariaAnimationScripts[];
extern u16 popuriAnimationScripts[];
extern u16 elliAnimationScripts[];
extern u16 annAnimationScripts[];
extern u16 karenAnimationScripts[];
extern u16 babyAnimationScripts[];
extern u16 harrisAnimationScripts[];
extern u16 grayAnimationScripts[];
extern u16 jeffAnimationScripts[];
extern u16 cliffAnimationScripts[];
extern u16 kaiAnimationScripts[];
extern u16 mayorAnimationScripts[];
extern u16 mayorWifeAnimationScripts[];
extern u16 lilliaAnimationScripts[];
extern u16 basilAnimationScripts[];
extern u16 ellenAnimationScripts[];
extern u16 dougAnimationScripts[];
extern u16 gotzAnimationScripts[];
extern u16 sashaAnimationScripts[];
extern u16 potionShopDealerAnimationScripts[];
extern u16 kentAnimationScripts[];
extern u16 stuAnimationScripts[];
extern u16 midwifeAnimationScripts[];
extern u16 mayAnimationScripts[];
extern u16 rickAnimationScripts[];
extern u16 pastorAnimationScripts[];
extern u16 shipperAnimationScripts[];
extern u16 saibaraAnimationScripts[];
extern u16 dukeAnimationScripts[];
extern u16 gregAnimationScripts[];
extern u16 assistantCarpenterAnimationScripts[];
extern u16 masterCarpenterAnimationScripts[];
extern u16 harvestSpriteAnimationScripts[];
extern u16 barleyAnimationScripts[];
extern u16 sydneyAnimationScripts[];
extern u16 entomologistAnimationScripts[];
extern u16 gourmetJudgeAnimationScripts[];
extern u16 photographerAnimationScripts[];
extern u16 shadySalesmanAnimationScripts[];
extern u16 mrsManaSonAnimationScripts[];
extern u16 mrsManaAnimationScripts[];
extern u16 oldManAnimationScripts[];
extern u16 oldWomanAnimationScripts[];
extern u16 touristCoupleManAnimationScripts[];
extern u16 touristCoupleWomanAnimationScripts[];
extern u16 festivalGirl1AnimationScripts[];
extern u16 festivalGirl2AnimationScripts[];
extern u16 festivalGirl3AnimationScripts[];
extern u16 harvestGoddessAnimationScripts[];
extern u16 spiritFestivalOcarinaPerformerAnimationScripts[];
extern u16 kappaAnimationScripts[];
extern u16 princessAnimationScripts[];
extern u16 bunnyAnimationScripts[];
extern u16 mouseAnimationScripts[];
extern u16 foxAnimationScripts[];
extern u16 monkeyAnimationScripts[];
extern u16 hawkAnimationScripts[];
extern u16 squirrelAnimationScripts[];
extern u16 birdAnimationScripts[];
extern u16 catAnimationScripts[];
extern u16 dogVillageAnimationScripts[];
extern u16 dogAnimationScripts[];
extern u16 horsePonyAnimationScripts[];
extern u16 horseGrownAnimationScripts[];
extern u16 chickAnimationScripts[];
extern u16 chickenAnimationScripts[];
extern u16 calfAnimationScripts[];
extern u16 youngCowAnimationScripts[];
extern u16 cowAnimationScripts[];
extern u16 lambAnimationScripts[];
extern u16 sheepAnimationScripts[];
extern u16 crabAnimationScripts[];
extern u16 hornedBeetleAnimationScripts[];
extern u16 stagBeetleAnimationScripts[];
extern u16 bellRingCricketAnimationScripts[];
extern u16 cicadaAnimationScripts[];
extern u16 whiteButterflyAnimationScripts[];
extern u16 ladybugAnimationScripts[];
extern u16 dragonflyAnimationScripts[];
extern u16 snakeAnimationScripts[];
extern u16 cowIndoorsAnimationScripts[];
extern u16 horseUnbridledAnimationScripts[];
extern u16 shopItemsAnimationScripts[];
extern u16 homeItemsAnimationScripts[];
extern u16 mariaHarrisBabyAnimationScripts[];
extern u16 popuriGrayBabyAnimationScripts[];
extern u16 elliJeffBabyAnimationScripts[];
extern u16 annCliffBabyAnimationScripts[];
extern u16 karenKaiBabyAnimationScripts[];
extern u16 holdableItems1AnimationScripts[];
extern u16 holdableItems2AnimationScripts[];
extern u16 holdableItems3AnimationScripts[];
extern u16 toolsAnimationScripts[];
extern u8 characterAvatarCodesToAnimations[];

#endif