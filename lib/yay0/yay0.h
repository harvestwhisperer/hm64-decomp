#ifndef YAY0_H
#define YAY0_H

#include <ultra64.h>

// "Yay0" big-endian
#define YAY0_MAGIC 0x59617930

// Size of the Yay0 header, in bytes
#define YAY0_HEADER_SIZE 16

// Scratch buffer for staging compressed blobs. Lives in its own dedicated
// spec segment (see src/buffers/yay0ScratchBuffer.c). Sized to comfortably
// hold the largest compressed asset.
#define YAY0_SCRATCH_SIZE 0x10000

extern u8 yay0Scratch[YAY0_SCRATCH_SIZE];

// DMA `length` bytes from `romAddr` into `vaddr`.
//
// If `romAddr` is listed in the compressed-range table, the blob is staged
// into yay0Scratch, decoded into vaddr, and the cache is flushed. Otherwise
// this falls through to nuPiReadRom.
void dmaReadRom(u32 romAddr, void *vaddr, u32 length);

// Memory-to-memory Yay0 decoder. `dst` must have room for the
// decompressed size declared in the `src` header.
void yay0Decode(const u8 *src, u8 *dst);

#endif
