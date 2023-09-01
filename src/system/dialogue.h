
#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "common.h"

typedef struct {
	void *romPtr1;
	void *romPtr2;
	void *romPtr3;
	void *romPtr4;
	void *romPtr5;
	void *romPtr6;
	void *vaddr1;
	void *vaddr2;
	void *vaddr3;
	void *vaddr4;
	void *vaddr5;
	void *vaddr6;
	Vec3f unk_30;
	u16 unk_3C;
	u16 unk_3E;
	u32 unk_40;
	u16 unk_44;
    u16 unk_46;
	Vec4f unk_48;
} DialogueIcon;

extern void func_80042F60();   
extern void func_80043430(u16, u16, u16, u16);
extern u8 func_80043A88();  

extern DialogueIcon D_801806D0;

#endif