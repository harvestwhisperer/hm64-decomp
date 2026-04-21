#include "common.h"
#include "yay0.h"

static u32 readBE32(const u8 *p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

#ifdef DEBUG
u64 yay0LastDecodeCycles = 0;
u32 yay0LastDecodeBytes  = 0;
#endif

// Stages `compressedLen` bytes of Yay0-compressed data from `romAddr` into
// yay0Scratch, decompresses into `vaddr`, and flushes the writeback cache
// for the decompressed range. Caller is responsible for knowing the blob
// is compressed — there is no runtime detection.
void dmaReadRom(u32 romAddr, void *vaddr, u32 compressedLen) {
    u32 decompressedSize;
#ifdef DEBUG
    OSTime t0, t1;
#endif

    nuPiReadRom(romAddr, yay0Scratch, compressedLen);

#ifdef DEBUG
    if (readBE32(yay0Scratch) != YAY0_MAGIC) {
        osSyncPrintf("dmaReadRom: not Yay0 at %08x\n", romAddr);
        while (1) {}
    }
    t0 = osGetTime();
#endif

    yay0Decode(yay0Scratch, (u8 *)vaddr);
    decompressedSize = readBE32(yay0Scratch + 4);

#ifdef DEBUG
    t1 = osGetTime();
    yay0LastDecodeCycles = t1 - t0;
    yay0LastDecodeBytes  = decompressedSize;
#endif

    osWritebackDCache(vaddr, (s32)decompressedSize);
}
