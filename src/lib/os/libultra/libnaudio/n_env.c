#include "n_synthInternals.h"
#include <os.h>
#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#include <assert.h>
#endif

#ifdef AUD_PROFILE
extern u32 cnt_index, env_num, env_cnt, env_max, env_min, lastCnt[];
extern u32 rate_num, rate_cnt, rate_max, rate_min;
extern u32 vol_num, vol_cnt, vol_max, vol_min;
#endif

#define N_EQPOWER_LENGTH 128

s16 n_eqpower[N_EQPOWER_LENGTH] = {
    32767,  32764,  32757,  32744,  32727,  32704,
    32677,  32644,  32607,  32564,  32517,  32464,
    32407,  32344,  32277,  32205,  32127,  32045,
    31958,  31866,  31770,  31668,  31561,  31450,
    31334,  31213,  31087,  30957,  30822,  30682,
    30537,  30388,  30234,  30075,  29912,  29744,
    29572,  29395,  29214,  29028,  28838,  28643,
    28444,  28241,  28033,  27821,  27605,  27385,
    27160,  26931,  26698,  26461,  26220,  25975,
    25726,  25473,  25216,  24956,  24691,  24423,
    24151,  23875,  23596,  23313,  23026,  22736,
    22442,  22145,  21845,  21541,  21234,  20924,
    20610,  20294,  19974,  19651,  19325,  18997,
    18665,  18331,  17993,  17653,  17310,  16965,
    16617,  16266,  15913,  15558,  15200,  14840,
    14477,  14113,  13746,  13377,  13006,  12633,
    12258,  11881,  11503,  11122,  10740,  10357,
    9971,   9584,   9196,   8806,   8415,   8023,
    7630,   7235,   6839,   6442,   6044,   5646,
    5246,   4845,   4444,   4042,   3640,   3237,
    2833,   2429,   2025,   1620,   1216,    810,
    405,      0
};

#ifndef N_MICRO
extern	f64	__pow(f64, f64);
extern f64 _frexpf(f64 value, s32* eptr);
extern f64 _ldexpf(f64 in, s32 ex);
#endif

/*
 * prototypes for private enveloper functions
 */
static  Acmd* _n_pullSubFrame(N_PVoice* pv, s16* inp, s16* outp, s32 outCount,
    Acmd* p);

static  s16 _n_getRate(f64 vol, f64 tgt, s32 count, u16* ratel);

#ifndef N_MICRO
static  f32 _n_getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);
#else
static  s16 _n_getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);
#endif
     
/***********************************************************************
  * Enveloper filter public interfaces
 ***********************************************************************/
Acmd *n_alEnvmixerPull(N_PVoice *filter, s32 sampleOffset, Acmd *p) 
{
  Acmd        *ptr = p;

  
  N_PVoice	*e = (N_PVoice *)filter;
  
  s16         inp;
  s32         lastOffset;
  s32         thisOffset = sampleOffset;
  s32         samples;
  s16         loutp = 0;
  s32         fVol;
  ALParam     *thisParam;
  s32 outCount = FIXED_SAMPLE;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * Force the input to be the resampler output
   */
#ifndef N_MICRO
  inp = AL_RESAMPLER_OUT;
#else
  inp = N_AL_RESAMPLER_OUT;
#endif
  
  while (e->em_ctrlList != 0) {
    
    lastOffset = thisOffset;
    thisOffset = e->em_ctrlList->delta;

#ifdef FINAL_ROUND
    samples    = SAMPLE184(thisOffset - lastOffset);
    if (!samples)
      thisOffset = lastOffset;
#else
    samples    = thisOffset - lastOffset;
#endif

    if (samples > outCount)
      break;
    
#ifdef _DEBUG
    assert(samples >= 0);
#ifndef N_MICRO
    assert(samples <= AL_MAX_RSP_SAMPLES);
#else
    assert(samples <= FIXED_SAMPLE);
#endif
#endif /* _DEBUG */

    switch (e->em_ctrlList->type) {
    case (AL_FILTER_START_VOICE_ALT):
      {                  
	ALStartParamAlt *param = (ALStartParamAlt *)e->em_ctrlList;
	ALFilter     *f = (ALFilter *) e;
	s32 tmp;
	
	if (param->unity) {
	  e->rs_upitch = 1;
	}
	
	n_alLoadParam(e, AL_FILTER_SET_WAVETABLE, param->wave);
	e->em_motion = AL_PLAYING;
	e->em_first  = 1;
	e->em_delta  = 0;

#ifdef FINAL_ROUND
	e->em_segEnd = SAMPLE184(param->samples);
#else
	e->em_segEnd = param->samples;
#endif

	tmp = ((s32)param->volume * (s32)param->volume) >> 15;


	e->em_volume = (s16) tmp;
	e->em_pan    = param->pan;
	e->em_dryamt = n_eqpower[param->fxMix];
	e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - param->fxMix - 1];
	
	if (param->samples) {
	  e->em_cvolL  = 1;
	  e->em_cvolR  = 1;
	} else {
	  /*
	   * Attack time is zero. Simply set the
	   * volume. We don't want an attack segment.
	   */
	  e->em_cvolL = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
	  e->em_cvolR = (e->em_volume *
			 n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
	}
	e->rs_ratio = param->pitch;
      }
      
      break;
      
    case (AL_FILTER_SET_FXAMT):
    case (AL_FILTER_SET_PAN):
    case (AL_FILTER_SET_VOLUME):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      
      if (e->em_delta >= e->em_segEnd){

	/*
	 * We should have reached our target, calculate
	 * target in case e->em_segEnd was 0
	 */
	e->em_ltgt = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
	e->em_rtgt = (e->em_volume *
		      n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
	e->em_delta = e->em_segEnd;   /* To prevent overflow */
	e->em_cvolL = e->em_ltgt;
	e->em_cvolR = e->em_rtgt;
      } else {
	/* 
	 * Estimate the current volume
	 */
	e->em_cvolL = _n_getVol(e->em_cvolL, e->em_delta, e->em_lratm, e->em_lratl);
	e->em_cvolR = _n_getVol(e->em_cvolR, e->em_delta, e->em_rratm, e->em_rratl);
      }
      
      /*
       * We can't have volume of zero, because the envelope
       * would never go anywhere from there
       */
      if ( e->em_cvolL == 0 ) e->em_cvolL = 1;
      if ( e->em_cvolR == 0 ) e->em_cvolR = 1;
      
      if (e->em_ctrlList->type == AL_FILTER_SET_PAN)
	
	/*
	 * This should result in a change to the current
	 * segment rate and target
	 */
	e->em_pan = (s16) e->em_ctrlList->data.i;
      
      if (e->em_ctrlList->type == AL_FILTER_SET_VOLUME){
	
	/*
	 * Switching to a new segment
	 */
	e->em_delta = 0;
	
	/*
	 * Map volume non-linearly to give something close to
	 * loudness
	 */
	fVol = (e->em_ctrlList->data.i);
	fVol = (fVol*fVol)>>15;
	e->em_volume = (s16) fVol;
	
#ifdef FINAL_ROUND
	e->em_segEnd = SAMPLE184(e->em_ctrlList->moredata.i);
#else
	e->em_segEnd = e->em_ctrlList->moredata.i;
#endif
	
      }
      
      if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT){
	e->em_dryamt = n_eqpower[e->em_ctrlList->data.i];
	e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - e->em_ctrlList->data.i - 1];
      }
      
      /*
       * Force a volume update
       */
      e->em_first = 1;
      break;
      
    case (AL_FILTER_START_VOICE):
      {
	ALStartParam *p = (ALStartParam *)e->em_ctrlList;
	
	/*
	 * Changing to PLAYING (since the previous state was
	 * persumable STOPPED, we'll just bump the output
	 * pointer rather than pull a subframe of zeros).
	 */
	if (p->unity) {
	  e->rs_upitch = 1;
	}
	
	n_alLoadParam(e, AL_FILTER_SET_WAVETABLE, p->wave);
	e->em_motion = AL_PLAYING;
      }
      break;
    case (AL_FILTER_STOP_VOICE):
      {
	/*
	 * Changing to STOPPED and reset the filter
	 */
	ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
	n_alEnvmixerParam(e, AL_FILTER_RESET, 0);
      }
      break;
      
    case (AL_FILTER_FREE_VOICE):
      {
	N_ALFreeParam *param = (N_ALFreeParam *)e->em_ctrlList;
	param->pvoice->offset = 0;
	_n_freePVoice((N_PVoice *)param->pvoice);
      }
      break;
#if 1
    case (AL_FILTER_SET_PITCH):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      e->rs_ratio = e->em_ctrlList->data.f;
      break;
    case (AL_FILTER_SET_UNITY_PITCH):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      e->rs_upitch = 1;
      break;
    case (AL_FILTER_SET_WAVETABLE):
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      n_alLoadParam(e, AL_FILTER_SET_WAVETABLE,
		       (void *)e->em_ctrlList->data.i);
      break;
#endif
    default:
      /*
       * Pull the reuired number of samples and then pass the message
       * on down the chain
       */
      ptr = _n_pullSubFrame(e, &inp, &loutp, samples, ptr);
      n_alEnvmixerParam(e, e->em_ctrlList->type,
		      (void *) e->em_ctrlList->data.i);
      break;
    }
    loutp  += (samples<<1);
    outCount -= samples;
    
    /*
     * put the param record back on the free list
     */
    thisParam = e->em_ctrlList;
    e->em_ctrlList = e->em_ctrlList->next;
    if (e->em_ctrlList == 0)
      e->em_ctrlTail = 0;
    
    _n_freeParam(thisParam);
    
  }
  
  ptr = _n_pullSubFrame(e, &inp, &loutp, outCount, ptr);
  
  /*
   * Prevent overflow in e->em_delta
   */
  if (e->em_delta > e->em_segEnd)
    e->em_delta = e->em_segEnd;
  
#ifdef AUD_PROFILE
  PROFILE_AUD(env_num, env_cnt, env_max, env_min);
#endif
  return ptr;
}

s32
  n_alEnvmixerParam(N_PVoice *filter, s32 paramID, void *param)
{
  N_PVoice	*e =  filter;
  
  switch (paramID) {
  case (AL_FILTER_ADD_UPDATE):
    if (e->em_ctrlTail) {
      e->em_ctrlTail->next = (ALParam *)param;
    } else {
      e->em_ctrlList = (ALParam *)param;
    }            
    e->em_ctrlTail = (ALParam *)param;
    break;
  case (AL_FILTER_RESET):
    e->em_first = 1;
    e->em_motion = AL_STOPPED;
    e->em_volume = 1;
    e->em_segEnd = 0;
    e->rs_delta  = 0.0;
    e->rs_first  = 1;
    e->rs_upitch = 0;
    n_alLoadParam(e, AL_FILTER_RESET, param);
    break;
  case (AL_FILTER_START):
    e->em_motion = AL_PLAYING;
    break;
  default:
#if 1
    n_alLoadParam(e, paramID, param);
#else
    n_alResampleParam(e, paramID, param);
#endif
    break;
  }
  return 0;
}

static
  Acmd* _n_pullSubFrame(N_PVoice *filter, s16 *inp, s16 *outp, s32 outCount,
		      Acmd *p) 
{
  Acmd        *ptr = p;
  N_PVoice	*e = filter;
#ifndef N_MICRO
  s16 *inpp = AL_RESAMPLER_OUT;
#else
  s16 *inpp = N_AL_RESAMPLER_OUT;
#endif

  /* filter must be playing and request non-zero output samples to pull. */
  if (e->em_motion != AL_PLAYING || !outCount)
    return ptr;
  
  /*
   * ask all filters upstream from us to build their command
   * lists.
   */
  
  ptr = n_alResamplePull(e,inp, p);
  
  /*
   * construct our portion of the command list
   */
#ifndef N_MICRO
  aSetBuffer(ptr++, A_MAIN, *inp, AL_MAIN_L_OUT, FIXED_SAMPLE<<1);
  aSetBuffer(ptr++, A_AUX, AL_MAIN_R_OUT , AL_AUX_L_OUT ,
	     AL_AUX_R_OUT );
#endif
  
  if (e->em_first){
    e->em_first = 0;
    
    /*
     * Calculate derived parameters
     */
    e->em_ltgt = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
    e->em_lratm = _n_getRate((f64)e->em_cvolL, (f64)e->em_ltgt,
			   e->em_segEnd, &(e->em_lratl));
    e->em_rtgt = (e->em_volume *
		  n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
    e->em_rratm = _n_getRate((f64)e->em_cvolR, (f64)e->em_rtgt, e->em_segEnd,
			   &(e->em_rratl));
    
#ifndef N_MICRO
    aSetVolume(ptr++, A_LEFT | A_VOL, e->em_cvolL, 0, 0);
    aSetVolume(ptr++, A_RIGHT | A_VOL, e->em_cvolR, 0, 0);
    aSetVolume(ptr++, A_LEFT  | A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);
    aSetVolume(ptr++, A_RIGHT | A_RATE, e->em_rtgt, e->em_rratm, e->em_rratl);
    aSetVolume(ptr++, A_AUX, e->em_dryamt, 0, e->em_wetamt);
    aEnvMixer (ptr++, A_INIT | A_AUX, osVirtualToPhysical(e->em_state));
  }
  else
    aEnvMixer(ptr++, A_CONTINUE | A_AUX, osVirtualToPhysical(e->em_state));
#else
//Begin #include "n_env_add01.c"
    n_aSetVolume(ptr++, A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);
    n_aSetVolume(ptr++, A_LEFT  | A_VOL, e->em_cvolL, e->em_dryamt, e->em_wetamt);
    n_aSetVolume(ptr++, A_RIGHT | A_VOL, e->em_rtgt, e->em_rratm,  e->em_rratl);
    n_aEnvMixer (ptr++, A_INIT, e->em_cvolR, osVirtualToPhysical(e->em_state));
  }
  else
    n_aEnvMixer(ptr++, A_CONTINUE, 0, osVirtualToPhysical(e->em_state));
//End #include "n_env_add01.c"
#endif  
  /*
   * bump the input buffer pointer
   */
  
  *inp += (FIXED_SAMPLE<<1);
  e->em_delta += FIXED_SAMPLE;
  
  return ptr;
}

#ifndef N_MICRO
#define EXP_MASK  0x7f800000
#define MANT_MASK 0x807fffff

static
  s16 _n_getRate(f64 vol, f64 tgt, s32 count, u16* ratel)
{
  s16         s;
  
  f64         invn = 1.0/count, eps, a, fs, mant;
  s32         i_invn, ex, indx;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  if (count == 0){
    if (tgt >= vol){
      *ratel = 0xffff;
      return 0x7fff;
    }
    else{
      *ratel = 0;
      return 0;
    }
  }
  
  if (tgt < 1.0)
    tgt = 1.0;
  if (vol <= 0) vol = 1;	/* zero and neg values not allowed */
  
#define NBITS (3)
#define NPOS  (1<<NBITS)
#define NFRACBITS (30)
#define M_LN2		0.69314718055994530942
  /*
   * rww's parametric pow()
   Goal: compute a = (tgt/vol)^(1/count)
   
   Approach:
   (tgt/vol)^(1/count) =
   ((tgt/vol)^(1/2^30))^(2^30*1/count)
   
   (tgt/vol)^(1/2^30) ~= 1 + eps
   
   where
   
   eps ~= ln(tgt/vol)/2^30
   
   ln(tgt/vol) = ln2(tgt/vol) * ln(2)
   
   ln2(tgt/vol) = fp_exponent( tgt/vol ) +
   ln2( fp_mantissa( tgt/vol ) )
   
   fp_mantissa() and fp_exponent() are
   calculated via tricky bit manipulations of
   the floating point number. ln2() is
   approximated by a look up table.
   
   Note that this final (1+eps) value needs
   to be raised to the 2^30/count power. This
   is done by operating on the binary representaion
   of this number in the final while loop.
   
   Enjoy!
   */
  {
    f64 logtab[] = { -0.912537, -0.752072, -0.607683, -0.476438,
		       -0.356144, -0.245112, -0.142019, -0.045804  };
    
    i_invn = (s32) _ldexpf( invn, NFRACBITS );
    mant = _frexpf( tgt/vol, &ex );
    indx = (s32) (_ldexpf( mant, NBITS+1 ) ); /* NPOS <= indx < 2*NPOS */
    eps = (logtab[indx - NPOS] + ex) * M_LN2;
    eps /= _ldexpf( 1, NFRACBITS ); /* eps / 2^NFRACBITS */
    fs = (1.0 + eps);
    a = 1.0;
    while( i_invn ) {
      if ( i_invn & 1 )
	a = a * fs;
      fs *= fs;
      i_invn >>= 1;
    }
  }
  
  a *= (a *= (a *= a));
  s = (s16) a;
  *ratel = (s16)(0xffff * (a - (f32) s));
  
#ifdef AUD_PROFILE
  PROFILE_AUD(rate_num, rate_cnt, rate_max, rate_min);
#endif
  return (s16)a;
  
}
#else
static
s16 _n_getRate(f64 vol, f64 tgt, s32 count, u16* ratel)
{
    s16         s, tmp;
    f64         invn, a, f;

#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif
    
    if (count == 0){
        if (tgt >= vol){
            *ratel = 0xffff;
            return 0x7fff;
        }
        else{
 	    *ratel = 0;
            return 0x8000;
        }
    }

    invn = 1.0 / count;

    if (tgt < 1.0)
        tgt = 1.0;
    if (vol <= 0.0) vol = 1.0;	/* zero and neg values not allowed */

    a = (tgt - vol) * invn * 8.0;
    s = (s16)a;
    f = a - (f64)s;
    s -= 1;
    f += 1.0;
    tmp = (s16)f;
    s += tmp;
    f -= (f64)tmp;

#ifdef AUD_PROFILE
	PROFILE_AUD( rate_num, rate_cnt, rate_max, rate_min);
#endif

    *ratel = (u16)(0xffff * f);
    return s;
}
#endif

#ifndef N_MICRO
static
  f32 _n_getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel)
{
  f32	        r, a;
  s32	      	i;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * Rate values are actually rate^8
   */
  samples >>=3;
  if (samples == 0){
    return ivol;
  }
  r = ((f32) (ratem<<16) + (f32) ratel)/65536;
  
  a = 1.0;
  for (i=0; i<32; i++){
    if ( samples & 1 )
      a *= r;
    samples >>= 1;
    if (samples == 0)
      break;
    r *= r;
  }
  ivol *= a;
#ifdef AUD_PROFILE
  PROFILE_AUD(vol_num, vol_cnt, vol_max, vol_min);
#endif
  return ivol;
}
#else
static
s16 _n_getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel)
{
    s32 tmp1;
#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif
    
    samples >>= 3;
    if (samples == 0){
        return ivol;
    }

    tmp1 = ratel * samples;
    tmp1 >>= 16;
    tmp1 += ratem * samples;
    ivol += tmp1;

#ifdef AUD_PROFILE
    PROFILE_AUD(vol_num, vol_cnt, vol_max, vol_min);
#endif
    return ivol;
}
#endif
