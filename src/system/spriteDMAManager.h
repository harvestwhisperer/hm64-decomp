#ifndef _SPRITE_DMA_MANAGER_H_
#define _SPRITE_DMA_MANAGER_H_

#define MAX_SPRITE_ASSET_DESCRIPTORS 40

/* spriteDMAManager flags */
#define NEED_DMA 1

// info for DMA
typedef struct {
	void *romAddr;
	void *vaddr;
	u32 length;
	u16 flags;
} SpriteAssetDescriptor;

extern void initializeSpriteAssetDescriptorFlags(void);
extern bool setSpriteAssetDescriptors(u8 *romAddr, u8 *vaddr, u32 length);
extern void dmaSprites();

#endif