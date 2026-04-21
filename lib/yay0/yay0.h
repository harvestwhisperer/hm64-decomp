#ifndef YAY0_H
#define YAY0_H

#include <ultra64.h>

// "Yay0" big-endian
#define YAY0_MAGIC 0x59617930

#define YAY0_HEADER_SIZE 16

// Scratch buffer for staging compressed blobs. Sized to comfortably hold the largest compressed asset.
#define YAY0_SCRATCH_SIZE 0x10000

extern u8 yay0Scratch[YAY0_SCRATCH_SIZE];

// Stage `compressedLen` bytes of Yay0-compressed data from `romAddr` into
// yay0Scratch, decode into `vaddr`, and flush the writeback cache for the
// decompressed range. Caller must know the blob is compressed — use
// nuPiReadRom directly for raw data.
void dmaReadRom(u32 romAddr, void *vaddr, u32 compressedLen);

// Memory-to-memory Yay0 decoder. `dst` must have room for the
// decompressed size declared in the `src` header.
void yay0Decode(const u8 *src, u8 *dst);

#endif
