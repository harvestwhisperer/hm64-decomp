#include "common.h"
#include "controller.h"

extern OSPifRam D_80170400;
extern u8 __osMaxControllers;

INCLUDE_ASM(const s32, "lib/os/libultra/io/contreaddata", osContStartReadData);

//INCLUDE_ASM(const s32, "lib/os/libultra/io/contreaddata", osContGetReadData);

void osContGetReadData(OSContPad* data) {
    u8 *ptr;
    __OSContReadFormat readformat;
    int i;
    ptr = (u8*)&D_80170400.ramarray;
    for (i = 0; i < __osMaxControllers; i++, ptr += sizeof(__OSContReadFormat), data++) {
        readformat = *(__OSContReadFormat *)ptr;
        data->errno = CHNL_ERR(readformat);
        if (data->errno == 0) {
            data->button = readformat.button;
            data->stick_x = readformat.stick_x;
            data->stick_y = readformat.stick_y;
        }
    }
}

INCLUDE_ASM(const s32, "lib/os/libultra/io/contreaddata", contreaddata_text_0124);
