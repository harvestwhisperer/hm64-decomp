#define ANIM_TYPE_ONE_SHOT           0x0000  // type 0xFF
#define ANIM_TYPE_LOOP               0x2000  // type 0xFE
#define ANIM_TYPE_DESTROY_ON_END     0x4000  // type 0xFD

#define FLIP_HORIZONTAL              0x8000

#define PACK_ANIM_DATA(metadata_offset, anim_type, horizontal_flip) \
    ((metadata_offset) | (anim_type) | (horizontal_flip))