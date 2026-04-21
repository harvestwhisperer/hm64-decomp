#include "common.h"
#include "yay0.h"

typedef struct {
    u32 start;
} CompressedRange;

// The range table is auto-generated at build time by
// tools/libhm64/yay0/generate_ranges.py — it scans the spec files for
// any segment whose `include` path ends in `.yay0.o` and emits an entry
// for each. See Makefile: $(BUILD_DIR)/lib/yay0/compressed_ranges.h.
#include "compressed_ranges.h"

#define COMPRESSED_RANGE_COUNT (sizeof(sCompressedRanges) / sizeof(sCompressedRanges[0]))

static bool isCompressedRom(u32 romAddr) {
    u32 i;
    for (i = 0; i < COMPRESSED_RANGE_COUNT; i++) {
        if (sCompressedRanges[i].start == romAddr) {
            return TRUE;
        }
    }
    return FALSE;
}

static u32 readBE32(const u8 *p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

u64 yay0LastDecodeCycles = 0;
u32 yay0LastDecodeBytes  = 0;

void dmaReadRom(u32 romAddr, void *vaddr, u32 length) {
    u32 decompressedSize;
    OSTime t0, t1;

    if (!isCompressedRom(romAddr)) {
        nuPiReadRom(romAddr, vaddr, length);
        return;
    }

    nuPiReadRom(romAddr, yay0Scratch, length);
    decompressedSize = readBE32(yay0Scratch + 4);

    t0 = osGetTime();
    yay0Decode(yay0Scratch, (u8 *)vaddr);
    t1 = osGetTime();

    yay0LastDecodeCycles = t1 - t0;
    yay0LastDecodeBytes  = decompressedSize;

    osWritebackDCache(vaddr, (s32)decompressedSize);
}
