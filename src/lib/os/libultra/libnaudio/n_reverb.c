#include "n_synthInternals.h"
#include <ultraerror.h>

#include <os.h>
#include <os_internal.h>
#include <stdio.h>
#define RANGE 2.0

#ifdef AUD_PROFILE
extern u32 cnt_index, reverb_num, reverb_cnt, reverb_max, reverb_min, lastCnt[];
extern u32 load_num, load_cnt, load_max, load_min, save_num, save_cnt, save_max, save_min;
#endif

/*
 * macros
 */
#define SWAP(in, out)	\
{			\
			  s16 t = out;	\
			    out = in;		\
			      in = t;		\
			      }

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p);
Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p);
Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, Acmd *p);

extern f32  _doModFunc(ALDelay *d, s32 count);
extern s32 L_INC[];

/***********************************************************************
 * Reverb filter public interfaces
 ***********************************************************************/
Acmd *n_alFxPull(s32 sampleOffset, Acmd *p) 
{
  Acmd        *ptr = p;
  ALFx	*r = (ALFx *)n_syn->auxBus->fx;
  s16		i, buff1, buff2, input, output;
  s16		*in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
  ALDelay	*d, *pd;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * pull channels going into this effect first
   */
  ptr = n_alAuxBusPull(sampleOffset, p);
  
#ifndef N_MICRO
  input  = AL_AUX_L_OUT;
  output = AL_AUX_R_OUT;
  buff1  = AL_TEMP_0;
  buff2  = AL_TEMP_1;
#else
  input  = N_AL_AUX_L_OUT;
  output = N_AL_AUX_R_OUT;
  buff1  = N_AL_TEMP_0;
  buff2  = N_AL_TEMP_1;
#endif
  
#ifndef N_MICRO
  aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);  /* set the buffer size */
  aMix(ptr++, 0, 0xda83, AL_AUX_L_OUT, input); /* .707L = L - .293L */
  aMix(ptr++, 0, 0x5a82, AL_AUX_R_OUT, input); /* mix the AuxL and AuxR into the AuxL */
#else
  aMix(ptr++, 0, 0xda83, N_AL_AUX_L_OUT, input);
  aMix(ptr++, 0, 0x5a82, N_AL_AUX_R_OUT, input);
#endif

  /* and write the mixed value to the delay line at r->input */
  ptr = _n_saveBuffer(r, r->input, input, ptr);
  
  aClearBuffer(ptr++, output, FIXED_SAMPLE<<1); /* clear the AL_AUX_R_OUT */
  
  for (i = 0; i < r->section_count; i++) {
    d  = &r->delay[i];  /* get the ALDelay structure */
    in_ptr  = &r->input[-d->input];
    out_ptr = &r->input[-d->output];
    
    if (in_ptr == prev_out_ptr) {
      SWAP(buff1, buff2);
    } else {  /* load data at in_ptr into buff1 */
      ptr = _n_loadBuffer(r, in_ptr, buff1, FIXED_SAMPLE, ptr);
    }
    ptr = _n_loadOutputBuffer(r, d, buff2, ptr);
    
    if (d->ffcoef) {
      aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
      if (!d->rs && !d->lp) {
	ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
      }
    }
    
    if (d->fbcoef) {
      aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
      ptr = _n_saveBuffer(r, in_ptr, buff1, ptr);
    }
    
    if (d->lp)
      ptr = _n_filterBuffer(d->lp, buff2, ptr);
    
    if (!d->rs)
      ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
    
    if (d->gain)
      aMix(ptr++, 0, (u16)d->gain, buff2, output);
    
    prev_out_ptr = &r->input[d->output];
  }
  
  /*
   * bump the master delay line input pointer
   * modulo the length
   */
  r->input += FIXED_SAMPLE;
  if (r->input > &r->base[r->length])
    r->input -= r->length;
  
  /*
   * output already in AL_AUX_R_OUT
   *      just copy to AL_AUX_L_OUT
   */
#ifndef N_MICRO
  aDMEMMove(ptr++, output, AL_AUX_L_OUT, FIXED_SAMPLE<<1);
#else
  aDMEMMove(ptr++, output, N_AL_AUX_L_OUT, FIXED_SAMPLE<<1);
#endif
  
#ifdef AUD_PROFILE
  PROFILE_AUD(reverb_num, reverb_cnt, reverb_max, reverb_min);
#endif
  return ptr;
}

/*
 * This routine gets called by alSynSetFXParam. No checking takes place to 
 * verify the validity of the paramID or the param value. input and output 
 * values must be 8 byte aligned, so round down any param passed. 
 */
s32 n_alFxParamHdl(void *filter, s32 paramID, void *param)
{
  ALFx   *f = (ALFx *) filter;    
  s32    p = (paramID - 2) % 8; 
  s32    s = (paramID - 2) / 8;
  s32    val = *(s32*)param;
  
#define INPUT_PARAM         0
#define OUTPUT_PARAM        1
#define FBCOEF_PARAM        2
#define FFCOEF_PARAM        3
#define GAIN_PARAM          4
#define CHORUSRATE_PARAM    5
#define CHORUSDEPTH_PARAM   6
#define LPFILT_PARAM        7
  
  switch(p)
    {
    case INPUT_PARAM:
      f->delay[s].input = (u32)val & 0xFFFFFFF8;
      break;
    case OUTPUT_PARAM:
      f->delay[s].output = (u32)val & 0xFFFFFFF8;
      break;
    case FFCOEF_PARAM:
      f->delay[s].ffcoef = (s16)val;
      break;
    case FBCOEF_PARAM:
      f->delay[s].fbcoef = (s16)val;
      break;
    case GAIN_PARAM:
      f->delay[s].gain = (s16)val;
      break;
    case CHORUSRATE_PARAM:
      /* f->delay[s].rsinc = ((f32)val)/0xffffff; */
      f->delay[s].rsinc
	= ((((f32)val)/1000) * RANGE)/n_syn->outputRate; 
      break;
      
      /*
       * the following constant is derived from:
       *
       *      ratio = 2^(cents/1200)
       *
       * and therefore for hundredths of a cent
       *                     x
       *      ln(ratio) = ---------------
       *              (120,000)/ln(2)
       * where
       *      120,000/ln(2) = 173123.40...
       */
#define CONVERT 173123.404906676
#define LENGTH  (f->delay[s].output - f->delay[s].input)
      
    case CHORUSDEPTH_PARAM:
      /*f->delay[s].rsgain = (((f32)val) / CONVERT) * LENGTH; */
      f->delay[s].rsgain = (((f32)val) / CONVERT) * LENGTH;
      break;
    case LPFILT_PARAM:
      if(f->delay[s].lp)
	{
	  f->delay[s].lp->fc = (s16)val;
#ifdef _OLD_AUDIO_LIBRARY
	  init_lpfilter(f->delay[s].lp);
#else
	  _init_lpfilter(f->delay[s].lp);
#endif
	}
      break;
    }
  return 0;
}

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p)
{
  Acmd        *ptr = p;
#ifndef N_MICRO
  s32         ratio, count, rbuff = AL_TEMP_2;
#else
  s32         ratio, count, rbuff = N_AL_TEMP_2;
#endif
  s16         *out_ptr;
  f32         fincount, fratio, delta;
  s32         ramalign = 0, length;
  static f32  val=0.0, lastval=-10.0;
  static f32  blob=0;
  s32 incount = FIXED_SAMPLE;

  if (d->rs) {
    length = d->output - d->input;
    delta = _doModFunc(d, incount);
    delta /= length;
    delta = (s32)(delta * UNITY_PITCH);
    delta = delta / UNITY_PITCH;
    fratio = 1.0 - delta;
    fincount = d->rs->delta + (fratio * (f32)incount);
    count = (s32) fincount;
    d->rs->delta = fincount - (f32)count;
    out_ptr = &r->input[-(d->output - d->rsdelta)];
    ramalign = ((s32)out_ptr & 0x7) >> 1;
    ptr = _n_loadBuffer(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
    
    ratio = (s32)(fratio * UNITY_PITCH);
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, rbuff + (ramalign<<1), buff, incount<<1);
    aResample(ptr++, d->rs->first, ratio, osVirtualToPhysical(d->rs->state));
#else
//Begin #include "n_reverb_add04.c"
    {
        s16 tmp;

        tmp = buff >> 8;
        n_aResample(ptr++, osVirtualToPhysical(d->rs->state), d->rs->first, ratio, rbuff + (ramalign << 1), tmp);
    }
//End #include "n_reverb_add04.c"
#endif    
    d->rs->first = 0;
    d->rsdelta += count - incount;
  } else {
    out_ptr = &r->input[-d->output];
    ptr = _n_loadBuffer(r, out_ptr, buff, FIXED_SAMPLE, ptr);
  }
  
  return ptr;
}

Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff,s32 count, Acmd *p)
{
    Acmd    *ptr = p;
    s32     after_end, before_end;
    s16     *updated_ptr, *delay_end;

#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

    delay_end = &r->base[r->length];

#ifdef _DEBUG
    if(curr_ptr > delay_end)
        __osError(ERR_ALMODDELAYOVERFLOW, 1, delay_end - curr_ptr);
#endif

    if (curr_ptr < r->base)
    curr_ptr += r->length;
    updated_ptr = curr_ptr + count;
    
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        
#ifndef N_MICRO
        aSetBuffer(ptr++, 0, buff, 0, before_end<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, buff+(before_end<<1), 0, after_end<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(r->base));
    } else {
        aSetBuffer(ptr++, 0, buff, 0, count<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }

    aSetBuffer(ptr++, 0, 0, 0, count<<1);
#else
//Begin #include "n_reverb_add01.c"
        n_aLoadBuffer(ptr++, before_end<<1, buff, osVirtualToPhysical(curr_ptr));
        n_aLoadBuffer(ptr++, after_end<<1, buff+(before_end<<1), osVirtualToPhysical(r->base));
   } else {
        n_aLoadBuffer(ptr++, count<<1, buff, osVirtualToPhysical(curr_ptr));
   }
//End #include "n_reverb_add01.c"
#endif

#ifdef AUD_PROFILE
    PROFILE_AUD(load_num, load_cnt, load_max, load_min);
#endif
    return ptr;
}

Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p)
{
    Acmd    *ptr = p;
    s32     after_end, before_end;
    s16     *updated_ptr, *delay_end;
#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)      /* probably just security */
        curr_ptr += r->length;   /* shouldn't occur */
    updated_ptr = curr_ptr + FIXED_SAMPLE;

    if (updated_ptr > delay_end) { /* if the data wraps past end of r->base */
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;

#ifndef N_MICRO
        aSetBuffer(ptr++, 0, 0, buff, before_end<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, 0, buff+(before_end<<1), after_end<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(r->base));
        aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);
    } else {
        aSetBuffer(ptr++, 0, 0, buff, FIXED_SAMPLE<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }
#else
//Begin #include "n_reverb_add02.c"
        n_aSaveBuffer(ptr++, before_end << 1, buff, osVirtualToPhysical(curr_ptr));
        n_aSaveBuffer(ptr++, after_end << 1, buff + (before_end << 1), osVirtualToPhysical(r->base));
    }
    else {
        n_aSaveBuffer(ptr++, FIXED_SAMPLE << 1, buff, osVirtualToPhysical(curr_ptr));
    }

//End #include "n_reverb_add02.c"
#endif

#ifdef AUD_PROFILE
    PROFILE_AUD(save_num, save_cnt, save_max, save_min);
#endif
    return ptr;

}

Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, Acmd *p)
{
    Acmd	*ptr = p;
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, buff, buff, FIXED_SAMPLE<<1);
    aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
    aPoleFilter(ptr++, lp->first, lp->fgain, osVirtualToPhysical(lp->fstate));
#else
//Begin #include "n_reverb_add03.c"
    {
        s16  tmp;

        tmp = buff >> 8;
        n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
        n_aPoleFilter(ptr++, lp->first, lp->fgain, tmp, osVirtualToPhysical(lp->fstate));
    }
//End #include "n_reverb_add03.c"
#endif

    lp->first = 0;

    return ptr;
}


