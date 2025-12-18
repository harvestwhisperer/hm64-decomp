#include <ultra64.h>

/* include current configuration settings */
#include "libmus_config.h"

#ifdef	NU_SYSTEM
 #ifdef N_AUDIO
  #include <n_libaudio_sc.h>
  #include <n_libaudio_sn_sc.h>
 #else
  #include <libaudio.h>
 #endif
 #include <nusys.h>
#else
 #include "audio.h"
#endif
#include "player.h"

/* dont define things as static for test program */
#ifndef _AUDIOVISIBLE
#define STATIC static
#else
#define STATIC
#endif

/* define note data */
#define REST		96	/* c8 = rest				*/
#define BASEOFFSET	48	/* c4 = sample base note default	*/

/* convert unsigned char values to floats */
#define U8_TO_FLOAT(c) ((c)&128) ? -(256-(c)) : (c)


/* internal function prototypes */
static ALMicroTime __MusIntMain			(void *node);
static void __MusIntProcessContinuousVolume	(channel_t *cp);
static void __MusIntProcessContinuousPitchBend	(channel_t *cp);
static void __MusIntGetNewNote			(channel_t *cp, int x);
static void __MusIntFlushPending		(channel_t *cp, int x);
static void __MusIntSetVolumeAndPan		(channel_t *cp, int x);
static void __MusIntSetPitch			(channel_t *cp, int x, float offset);
static void __MusIntInitEnvelope		(channel_t *cp);
static void __MusIntProcessEnvelope		(channel_t *cp);
static void __MusIntInitSweep			(channel_t *cp);
static void __MusIntProcessSweep		(channel_t *cp);
static float __MusIntProcessVibrato		(channel_t *cp);
static float __MusIntProcessWobble		(channel_t *cp);

/* Internal utility functions... */
static void __MusIntInitialiseChannel		(channel_t *cp);
static void __MusIntRemapPtrBank		(char *pptr, char *wptr);

static float __MusIntPowerOf2			(float x);
static int  __MusIntRandom			(int range);
static void __MusIntMemSet			(void *dest, unsigned char value, unsigned long length);
static int __MusIntFindChannel                  (song_t *addr, int song_chan);
static unsigned long __MusIntStartEffect        (channel_t *cp, int number, int volume, int pan, int priority);
static void __MusIntRemapPtrs			(void *addr, void *offset, int count);

/* external vars */

#ifdef	NU_SYSTEM
 #ifdef	N_AUDIO
extern N_ALGlobals	auGlobal;
 #else
extern ALGlobals	auGlobal;
 #endif
#else
extern AMAudioMgr      __am;
#endif


/* internal vars */
STATIC ALPlayer        plr_player;                /* synthesizer player        */
#ifdef	NU_SYSTEM
extern ALHeap	       nuAuHeap;                  /* audio heap                */
#else
STATIC ALHeap	       heap;                      /* audio heap                */
#endif
STATIC int             max_channels;              /* number of channels        */
STATIC ALVoice         *mus_voices;               /* audio library voices      */
STATIC channel_t       *mus_channels;             /* music player channels     */
STATIC unsigned char   **mus_effects;             /* address of sound effects  */
STATIC int             *mus_priority;             /* address of sfx prioritys  */
STATIC int             mus_vsyncs_per_second;     /* video refresh rate        */
STATIC ALMicroTime     mus_next_frame_time;	  /* time until next frame     */
STATIC unsigned short  mus_master_volume_effects; /* sound effect master value */
STATIC unsigned short  mus_master_volume_songs;   /* song master volume        */
STATIC unsigned long   mus_current_handle;        /* current handle number     */
STATIC long            mus_random_seed;           /* random number seed value  */
STATIC ptr_bank_t      *mus_init_bank;		  /* sample bank to initialise */
STATIC ptr_bank_t      *mus_default_bank;	  /* sample bank default       */

/* music player control flag */
unsigned long __muscontrol_flag;


/* C files included directly (to avoid lots of global variables in the library)*/
/* these files are separated just for readability                              */
#include "player_commands.c"
#include "player_api.c"

/* C files for SC effect extensions */
#ifdef SUPPORT_EFFECTS
#include "player_effects.c"
#endif

/* timing workspace */
#ifdef SUPPORT_PROFILER
unsigned long _mus_cpu_last = 0;
unsigned long _mus_cpu_worst = 0;
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusExtPlayerDelete(player)

  [Parameters]
  player	address of player structure

  [Explanation]
  Called by synthesis driver to remove the music player. This should never be
  called!

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#if 0 /* never called */
void __MusExtPlayerDelete(ALPlayer *player)
{
#ifdef	NU_SYSTEM
  alSynRemovePlayer(&auGlobal.drvr, player);
#else
  alSynRemovePlayer(&__am.g.drvr, player);
#endif
  player->handler = NULL;
}
#endif

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [EXTERNAL FUNCTION]
  __MusIntMain(node)

  [Parameters]
  node		ignored

  [Explanation]
  Called by synthesis driver to generate hardware output, this is the music
  players main loop.

  [Return value]
  amount of time before next callback
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static ALMicroTime __MusIntMain(void *node)
{
  int	x;
  channel_t *cp;
  unsigned long calc;
#ifdef SUPPORT_PROFILER
  unsigned long start=osGetCount();
#endif

  for(x=0, cp=mus_channels; x<max_channels; x++, cp++)
  {
    if(cp->pdata == NULL)
      continue;

    if (cp->pending)
      __MusIntFlushPending(cp,x);

    cp->channel_frame+=cp->channel_tempo;
    /* process until note or stop for this frame is found */
    if(cp->length != 0x7fff)
    {
      while (cp->note_end_frame<cp->channel_frame && cp->pdata!=NULL)
	__MusIntGetNewNote(cp, x);
      /* cancel processing if channel stopped */
      if(!cp->pdata)
	continue;
    }
    /* process volume and pitchbend continous data streams */
    if(cp->pvolume && cp->pitchbend_frame<cp->channel_frame)
      __MusIntProcessContinuousVolume(cp);
    if(cp->ppitchbend && cp->pitchbend_frame<cp->channel_frame)
      __MusIntProcessContinuousPitchBend(cp);

    /* process fade down and stop API command */
    if (cp->stopping != -1)
    {
      cp->stopping--;
      if (cp->stopping==-1)
      {
	cp->pdata = Fstop(cp, NULL);      
	if(cp->playing)
	{
	  cp->playing = 0;
#ifdef	NU_SYSTEM
	  alSynStopVoice(&auGlobal.drvr, mus_voices+x);
#else
	  alSynStopVoice(&__am.g.drvr, mus_voices+x);
#endif
	}
      }
    }

    /* process current note to generate sound */
    if(cp->playing)
    {
      float total;
      
      if (cp->env_phase)
	__MusIntProcessEnvelope(cp);
      if (cp->sweep_speed && cp->sweep_frame<cp->channel_frame)
	__MusIntProcessSweep(cp);

      total = cp->freqoffset;
#ifdef SUPPORT_EFFECTS
      if (cp->vib_speed || cp->specialvib_speed)
#else
      if (cp->vib_speed)
#endif
	total += __MusIntProcessVibrato(cp);
      if (cp->wobble_on_speed)
	total += __MusIntProcessWobble(cp);
      if (!cp->pending)
      {
	__MusIntSetPitch(cp, x, total);
	__MusIntSetVolumeAndPan(cp, x);
      }
    }
    /* increment channel counters */
    cp->count= (cp->channel_frame-cp->note_start_frame)>>8;
  }

#ifdef SUPPORT_PROFILER
  _mus_cpu_last = osGetCount()-start;
  if (_mus_cpu_last>_mus_cpu_worst)
    _mus_cpu_worst = _mus_cpu_last;
#endif
  
  /* set to call back next video frame */
  return (mus_next_frame_time);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntGetNewNote(cp,x)

  [Parameters]
  cp		address of music player channel
  x		channel number

  [Explanation]
  Get the next note from the sound data, processing any commands before a note is
  found.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntGetNewNote(channel_t *cp,  int x)
{
  unsigned char *ptr;
  unsigned char command;

  ptr = cp->pdata;
  while(ptr && (command=*ptr)>127)
  {
#ifdef _AUDIODEBUG
    if(command >= Clast)
    {
      osSyncPrintf("PLAYER.C: Channel %d is corrupt (command=%02x)\n",x,command);
      cp->pdata = NULL;
      break;
    }    
#endif
    /* Execute the relevant code for the token.  */
    ptr = (jumptable[command&0x7f].func)(cp, ptr+1);
  }
  cp->pdata = ptr;

  /* new note */
  if(ptr)
  {
    int note;

    cp->last_note = cp->port_base;
    note = *(cp->pdata++);

    if(cp->velocity_on)
      cp->velocity = /*velocity_map[*/*(cp->pdata++)/*]*/;
    else
      cp->velocity = cp->default_velocity;

    if (cp->fixed_length)
    {
      if (!cp->ignore)
	cp->length = cp->fixed_length;
      else
      {
	cp->ignore = 0;
	command = *(cp->pdata++);
	if (command < 128)
	  cp->length = command;
	else
	  cp->length = ((int)(command&0x7f)<<8)+*(cp->pdata++);
      }
    }
    else
    {
      command = *(cp->pdata++);
      if(command < 128)
	cp->length = command;
      else
	cp->length = ((int)(command&0x7f)<<8)+*(cp->pdata++);
    }

    /* set length and timer */
    cp->note_start_frame = cp->note_end_frame;
    cp->note_end_frame += cp->length*256;
    cp->count = 0;
    /* initialise wobble */
    cp->wobble_count = cp->wobble_off_speed;
    cp->wobble_current = 0;

#ifdef SUPPORT_SONGWAVELOOKUP
    /* if wave look up is -1 sample is silent - replace note with a rest */
    if (!cp->pdrums)
    {
      int wave;
      
      wave = cp->wave;
      if (cp->song_addr)
	wave = cp->song_addr->WaveLookup[wave];
      if (wave==-1)
	note = REST;
    }
#endif

    if (note!=REST)
    {
      int wave;
      ptr_bank_t *bank;

      /* get current sample bank */
      bank = cp->sample_bank;

      /* check for drums */
      if(cp->pdrums != NULL)
      {
	cp->wave = (*(cp->pdrums+(note)*4));
	cp->pan = (*(cp->pdrums+(note)*4+2))/2;
	(void) Fdefa(cp, &cp->song_addr->EnvelopeData[(*(cp->pdrums+(note)*4+1))*7]);
	note = (*(cp->pdrums+(note)*4+3));
      }

      /* initialise envelope */
      if(!cp->env_trigger_off)
	__MusIntInitEnvelope(cp);

      /* initialise sweep */
      if (cp->sweep_speed)
	__MusIntInitSweep(cp);

#ifdef _AUDIODEBUG
      /* validate wave number with sample bank */
      if (cp->wave>=bank->count)
      {
	osSyncPrintf("PLAYER.C: Wave number overflow!\n");
	cp->wave = 0;
      }
#endif
      /* get current wave number */
      wave = cp->wave;

#ifdef SUPPORT_SONGWAVELOOKUP
      /* lookup wave in songs lookup table */
      if (cp->song_addr)
	wave = cp->song_addr->WaveLookup[wave];
#endif

      /* start relevant sample if required */
      if(!cp->trigger_off)
      {
	ALWaveTable *wave_addr;

	wave_addr = bank->wave_list[wave];
#ifdef SUPPORT_WAVEMONITORING
	wave_addr->flags++; /* flags isn't needed by ALWaveTable honest! */
#endif
	cp->pending = wave_addr;
	if (cp->playing && cp->old_volume)
	{
	  cp->old_volume = 0;
#ifdef	NU_SYSTEM
	  alSynSetVol(&auGlobal.drvr, mus_voices+x, 0, mus_next_frame_time);
#else
	  alSynSetVol(&__am.g.drvr, mus_voices+x, 0, mus_next_frame_time);
#endif
	}
	else
	  __MusIntFlushPending(cp,x);
      }
      cp->base_note = (float)note+bank->detune[wave];

      /* set reverb level if required */
      if (cp->reverb!=cp->old_reverb)
      {
	unsigned char work;

	work = cp->reverb_base;
	work += ((128-work)*cp->reverb)>>7;
	cp->old_reverb = cp->reverb;
#ifdef	NU_SYSTEM
	alSynSetFXMix(&auGlobal.drvr, mus_voices+x, work);
#else
	alSynSetFXMix(&__am.g.drvr, mus_voices+x, work);
#endif
      }
    }
    else
    {
      /* rest allows previous notes release to finish */
      if (cp->env_phase<4)
      {
	cp->env_phase = 4; /* Start Release */
	cp->release_frame = cp->channel_frame;
	cp->env_count=1;
	cp->release_start_vol = cp->env_current;
      }
    }
  }
  else	/* must have hit a Cstop so stop its voice */
  {
    if(cp->playing)
    {
      cp->playing = 0;
#ifdef	NU_SYSTEM
      alSynSetVol(&auGlobal.drvr, mus_voices+x, 0, mus_next_frame_time);
      alSynStopVoice(&auGlobal.drvr, mus_voices+x);
#else
      alSynSetVol(&__am.g.drvr, mus_voices+x, 0, mus_next_frame_time);
      alSynStopVoice(&__am.g.drvr, mus_voices+x);
#endif
    }
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntFlushPending(cp,x)

  [Parameters]
  cp		address of music player channel
  x		channel number

  [Explanation]
  Start the sample waiting to play and, if necessary, stop the previous one.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntFlushPending(channel_t *cp, int x)
{
  if (cp->playing)
#ifdef	NU_SYSTEM
    alSynStopVoice(&auGlobal.drvr, mus_voices+x);
#else
    alSynStopVoice(&__am.g.drvr, mus_voices+x);
#endif
  cp->playing = 1;
  /* start sample */
#ifdef	NU_SYSTEM
  alSynStartVoice(&auGlobal.drvr, mus_voices+x, cp->pending);
#else
  alSynStartVoice(&__am.g.drvr, mus_voices+x, cp->pending);
#endif
  cp->pending = NULL;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntSetVolumeAndPan(cp,x)

  [Parameters]
  cp		address of music player channel
  x		channel number

  [Explanation]
  Output the volume level and pan position of the current channel to the hardware 
  (if necessary).

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntSetVolumeAndPan(channel_t *cp, int x)
{
  u32	volume;

  /* process volume */
  volume = ((u32)(cp->volume)*(u32)(cp->env_current)*(u32)(cp->velocity)*(u32)(cp->volscale))>>13;
  if (volume>32767)
    volume = 32767;

  if (!cp->IsFX)
    volume *= mus_master_volume_songs;
  else
    volume *= mus_master_volume_effects;
  volume >>= 15;
  if (cp->stopping != -1)
    volume = (volume*cp->stopping)/cp->stopping_speed;

  if (volume!=cp->old_volume)
  {
    cp->old_volume = volume;
#ifdef	NU_SYSTEM
    alSynSetVol(&auGlobal.drvr, mus_voices+x, volume, mus_next_frame_time);
#else
    alSynSetVol(&__am.g.drvr, mus_voices+x, volume, mus_next_frame_time);
#endif
  }

  /* process pan */
  if (cp->pan!=cp->old_pan)
  {
    volume = ((cp->pan*cp->panscale)>>7)&0x7f;
    cp->old_pan = volume;
#ifdef	NU_SYSTEM
    alSynSetPan(&auGlobal.drvr, mus_voices+x, volume);
#else
    alSynSetPan(&__am.g.drvr, mus_voices+x, volume);
#endif
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntSetPitch(cp,x)

  [Parameters]
  cp		address of music player channel
  x		channel number

  [Explanation]
  Output the frequency of the current channel to the hardware (if necessary).

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntSetPitch(channel_t *cp, int x, float offset)
{
  float   frequency, temp;
  int     calc;

  /* base frequency */
  frequency = cp->base_note;

  /* incorporate portamento */
  if(cp->port != 0)	
  {
    if(cp->count <= cp->port)
    {
      temp = (frequency - cp->last_note)/(float)(cp->port);
      temp *= (float)cp->count;
      frequency = cp->last_note + temp;
    }
    cp->port_base = frequency;
  }  
  /* get transpose value */
  calc = cp->transpose*(1-cp->ignore_transpose);
  cp->ignore_transpose = 0;

  /* incorporate offsets */
  frequency += offset+cp->pitchbend_precalc+(float)calc;

  /* only output it if it's changed! */
  if (frequency==cp->old_frequency)
    return;
  cp->old_frequency = frequency;  
  frequency = __MusIntPowerOf2(frequency*(1.0/12.0));
#ifdef _AUDIODEBUG
  if(frequency <= 0)
  {
    osSyncPrintf("PLAYER.C: frequency underflow.\n");
    frequency= 1.0;  
  }
#endif
  if(frequency > 2.0)
  {
    frequency = 2.0;
    cp->velocity = 0;    
  }  
#ifdef	NU_SYSTEM
  alSynSetPitch(&auGlobal.drvr, mus_voices+x, frequency);
#else
  alSynSetPitch(&__am.g.drvr, mus_voices+x, frequency);
#endif
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntInitEnvelope(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Initialise the envelope settings.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntInitEnvelope(channel_t *cp)
{   
  if(cp->length != 0x7fff)
  {
    if(cp->cutoff != 0) /* release time is from start of note */
      cp->release_frame = cp->note_start_frame + (cp->cutoff<<8);
    else		/* release time is from end of note */
      cp->release_frame = cp->note_end_frame - (cp->endit<<8);
  }
  else
  {
    cp->release_frame = 0x7fffffff; /* never release (well not this century) */
  }    
  cp->env_current = cp->env_init_vol;
  cp->env_count = cp->env_speed;
  cp->env_phase = 1;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntProcessEnvelope(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  If necessary process the envelope settings to calculate the current value.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntProcessEnvelope(channel_t *cp)
{
  float	temp;
  int env_phase_count;
 
  if(cp->channel_frame >= cp->release_frame && cp->env_phase<4)
  {
    /* Start Release */
    cp->env_phase = 4;
    cp->env_count=1;
    cp->release_start_vol = cp->env_current;
  }
  cp->env_count--;
  if(!cp->env_count)
  {
    cp->env_count = cp->env_speed;

    switch(cp->env_phase)
    {
	case 1:
	  {
	    /* Attack */
            env_phase_count = (((cp->channel_frame - cp->note_start_frame)>>8)*cp->env_speed_calc)>>10;
	    if (env_phase_count<cp->env_attack_speed)
	    {
	      cp->env_current = (int)cp->env_init_vol+(int)((float)(cp->env_attack_calc*(float)env_phase_count));
	      return;
	    }
	    else
	    {
	      cp->env_phase++;
	      cp->env_current = cp->env_max_vol;
	      return;
	    }
	  }
	case 2:
	  {
	    /* Decay */
	    env_phase_count = ((((cp->channel_frame - cp->note_start_frame)>>8) - cp->env_attack_speed)*cp->env_speed_calc)>>10;
	    if (env_phase_count<cp->env_decay_speed)
	    {
	      cp->env_current = (int)cp->env_max_vol+(int)((float)(cp->env_decay_calc*(float)env_phase_count));
	      return;
	    }
	    else
	    {
	      cp->env_phase++;
	      cp->env_current = cp->env_sustain_vol;
	      return;
	    }
	  }
	case 3:
	  {
	    /* Sustain */
	    return;
	  }
	case 4:
	  {
	    /* Do Release */
            env_phase_count = (((cp->channel_frame - cp->release_frame)>>8)*cp->env_speed_calc)>>10;
	    if (env_phase_count < cp->env_release_speed)
	    {
	      cp->env_current = (int)cp->release_start_vol-(int)((float)(cp->env_release_calc*(float)env_phase_count*(float)cp->release_start_vol));
	      return;
	    }
	    else
	    {
	      cp->env_phase++;
	      cp->env_current = 0;
	      return;
	    }
	  }
    }	
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntInitSweep(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Initialise settings for sweep pan processing.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntInitSweep(channel_t *cp)
{
  cp->sweep_frame = cp->note_start_frame;
  cp->sweep_timer = 0;
  cp->sweep_dir = cp->pan&0x40;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntProcessSweep(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Process the current sweep settings to move the pan position.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void __MusIntProcessSweep(channel_t *cp)
{
  unsigned long calc;

  do
  {
    cp->sweep_frame += 256;
    calc = cp->sweep_timer+cp->sweep_speed;
    if (calc<64)
    {
      cp->sweep_timer=calc;
      continue;
    }
    cp->sweep_timer=calc&63;
    calc>>=6;
    if (!cp->sweep_dir)
    {
      cp->pan += calc;
      if (cp->pan>0x7f)
      {
	cp->pan=0x7f;
	cp->sweep_dir = 1;
      }
    }
    else
    {
      cp->pan -= calc;
      if (cp->pan>=0x80 || cp->pan==0)
      {
	cp->pan=0;
	cp->sweep_dir = 0;
      }    
    }
  } while (cp->sweep_frame<cp->channel_frame);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntProcessWobble(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Process the wobble settings to calculate the current value.

  [Return value]
  Current wobble offset
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static float __MusIntProcessWobble(channel_t *cp)
{
  cp->wobble_count--;
  if(!cp->wobble_count)
  {
    if(cp->wobble_current==0)
    {
      cp->wobble_current=cp->wobble_amount;
      cp->wobble_count=cp->wobble_on_speed;
    }
    else
    {
      cp->wobble_current=0;
      cp->wobble_count=cp->wobble_off_speed;
    }
  }
  return ((float)cp->wobble_current);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntProcessVibrato(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Process the vibrato settings to calculate the current value.

  [Return value]
  Current vibrato offset
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static float __MusIntProcessVibrato (channel_t *cp)
{
  int temp;
  float temp1;

#ifdef SUPPORT_EFFECTS
  if (cp->specialvib_speed)
  {
    if((temp = cp->count-cp->specialvib_delay) > 0)
    {
      temp1 = sinf( (float) temp/cp->specialvib_speed*2*3.1415926)*cp->specialvib_amount/*/50*/;
      cp->vibrato = temp1;
    }    
  }
  else
  {
#endif

  temp = cp->count-cp->vib_delay ;
  if(temp > 0)
  {
    temp1 = sinf( (float) temp/cp->vib_speed*2*3.1415926)*cp->vib_amount/*/50*/;
    cp->vibrato = temp1;
  }
#ifdef SUPPORT_EFFECTS
  }
#endif
  return (cp->vibrato);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntProcessContinuousVolume(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Process the continuous RLE volume data to set the current value.

  ------------------------
  Current RLE data format:
  ------------------------
  If the top bit of the value is set then the value is to be repeated and the count
  byte follows. If the top bit of the count byte is set then it represents the top
  seven bits of the count and the following byte is the lower eight bits. The count
  is stored with two subtracted from it (0=2, 1=3 etc.etc.) because it is not worth
  compressing runs of less than two. The processing of this data can be show as
  follows:

       if count = 0
          byte = next byte from data stream
          if byte>128
             byte = byte & 127
             count = next byte from data stream
             if count>128
                count = count & 127
                count = count * 256
                count = count + next byte from the data stream
             endif
             count = count + 2
          endif
       else
          count = count - 1
       endif

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntProcessContinuousVolume(channel_t *cp)
{
  unsigned char work_vol;

  do
  {    
    cp->volume_frame += 256;
    cp->cont_vol_repeat_count--;
    if(cp->cont_vol_repeat_count == 0)   /* already repeating? */
    {
      work_vol = *(cp->pvolume++);
      if(work_vol > 127)		    /* does count follow? */
      {
	/* yes  volume is followed by run length data */
#ifdef SUPPORT_EFFECTS
	cp->last_volume = 
#endif
	cp->volume = work_vol &0x7f;
	work_vol = *(cp->pvolume++);
	if(work_vol > 127)
	{
	  cp->cont_vol_repeat_count = ((int)(work_vol&0x7f)*256);
	  cp->cont_vol_repeat_count += (int)*(cp->pvolume++)+2;
	}
	else
	  cp->cont_vol_repeat_count = (int)work_vol+2;
      }
      else
      {
#ifdef SUPPORT_EFFECTS
	cp->last_volume = 
#endif
	cp->volume = work_vol;
	cp->cont_vol_repeat_count=1;
      }
    }
  } while (cp->volume_frame < cp->channel_frame);

#ifdef SUPPORT_EFFECTS
  __MusIntSpecialVol(cp);
#endif
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntProcessContinuousPitchBend(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Process the continuous RLE pitchbend data to set the current value.

  ------------------------
  Current RLE data format:
  ------------------------
  If the top bit of the value is set then the value is to be repeated and the count
  byte follows. If the top bit of the count byte is set then it represents the top
  seven bits of the count and the following byte is the lower eight bits. The count
  is stored with two subtracted from it (0=2, 1=3 etc.etc.) because it is not worth
  compressing runs of less than two. The processing of this data can be show as
  follows:

       if count = 0
          byte = next byte from data stream
          if byte>128
             byte = byte & 127
             count = next byte from data stream
             if count>128
                count = count & 127
                count = count * 256
                count = count + next byte from the data stream
             endif
             count = count + 2
          endif
       else
          count = count - 1
       endif

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntProcessContinuousPitchBend(channel_t *cp)
{
  unsigned char work_pb;

  do 
  {    
    cp->pitchbend_frame += 256;
    cp->cont_pb_repeat_count--;
    if(cp->cont_pb_repeat_count == 0)   /* already repeating? */
    {
      work_pb = *(cp->ppitchbend++);
      if(work_pb > 127)		    /* does count follow? */
      {
	/* yes  pitchbend is followed by run length data */
	cp->pitchbend = ((float)(work_pb &0x7f))-64.0;
	cp->pitchbend_precalc = cp->pitchbend*cp->bendrange;
	work_pb = *(cp->ppitchbend++);
	if(work_pb > 127)
	{
	  cp->cont_pb_repeat_count = ((int)(work_pb&0x7f)*256);
	  cp->cont_pb_repeat_count += (int)*(cp->ppitchbend++)+2;
	}
	else
	  cp->cont_pb_repeat_count = (int)work_pb+2;
      }
      else
      {
	cp->pitchbend = ((float)work_pb)-64.0;
	cp->pitchbend_precalc = cp->pitchbend*cp->bendrange;
	cp->cont_pb_repeat_count=1;
      }
    }
  } while (cp->pitchbend_frame < cp->channel_frame);

#ifdef SUPPORT_EFFECTS
  __MusIntSpecialPitchBend(cp);
#endif
}


/***********************************************************************************
************************************************************************************

               I N T E R N A L   U T I L I T Y   F U N C T I O N S

************************************************************************************
***********************************************************************************/


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntPowerOf2(x)

  [Parameters]
  x		value

  [Explanation]
  Calculate the power of 2 which represents the specified value.

  [Return value]
  power of 2
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static float __MusIntPowerOf2(float x)
{
  float x2;
  
  if(x == 0)
    return 1;
  
  if(x > 0)
  {
    x2 = x*x;
    return (1+(x*.693147180559945)
	    +(x2*.240226506959101)
	    +(x2*x* 5.55041086648216E-02)
	    +(x2*x2* 9.61812910762848E-03)
	    +(x2*x2*x* 1.33335581464284E-03)
	    +(x2*x2*x2* 1.54035303933816E-04)
	    );
  }	
  else	
  {
    x = -x;
    x2 = x*x;
    return (1/(1+(x*.693147180559945)
	       +(x2*.240226506959101)
	       +(x2*x* 5.55041086648216E-02)
	       +(x2*x2* 9.61812910762848E-03)
	       +(x2*x2*x* 1.33335581464284E-03)
	       +(x2*x2*x2* 1.54035303933816E-04)
	       ));
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntRemapPtrBank(pptr, wptr, remap)

  [Parameters]
  pptr		address of PTR file in RAM
  wptr		address of WBK file in ROM
  remap		address of structure to store info about sample bank

  [Explanation]
  Change the offsets stored within the pointer bank file (PTR) to pointers.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntRemapPtrBank(char *pptr, char *wptr)
{
  int i,bankLen;
  u32 l;
  ptr_bank_t *ptrfile_addr;
  unsigned char *chardetune, charwork;
  float *floatdetune, floatwork;
  unsigned long base;

  ptrfile_addr = (ptr_bank_t *)pptr;
  /* return if already remapped */
  if (ptrfile_addr->flags&PTRFLAG_REMAPPED)
    return;
  /* set remapped flag */
  ptrfile_addr->flags |= PTRFLAG_REMAPPED;

  /* remap first set of pointers */
  __MusIntRemapPtrs(&ptrfile_addr->basenote, pptr, 3);
  /* remap wave list pointers */
  __MusIntRemapPtrs(&ptrfile_addr->wave_list[0], pptr, ptrfile_addr->count);

  /* now calculate detune values and remap wave list */
  for (i=0; i<ptrfile_addr->count; i++)
  {
    floatdetune = &ptrfile_addr->detune[i];
    chardetune = (unsigned char *)floatdetune;
    charwork = *chardetune;
    floatwork = U8_TO_FLOAT(charwork);
    *floatdetune = floatwork/100.0;

    charwork = ptrfile_addr->basenote[i]-BASEOFFSET;
    floatwork = U8_TO_FLOAT(charwork);
    *floatdetune += floatwork;

    /* remap pointers inside ALWaveTable structures */
    if(!ptrfile_addr->wave_list[i]->flags)
    {
      base = (unsigned long)ptrfile_addr->wave_list[i]->base;
      if ((base&0xff000000)!=0xff000000) /* not n64dd sample */
      {
	base += (unsigned long)wptr;
	ptrfile_addr->wave_list[i]->base = (u8 *)base;	
      }
      ptrfile_addr->wave_list[i]->flags = 1;
      
      if(ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop)			
	ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop = (ALADPCMloop *)((u32)(ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.loop)+(u32)(pptr));
      if(ptrfile_addr->wave_list[i]->type == AL_ADPCM_WAVE)			
	ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.book = (ALADPCMBook *)((u32)(ptrfile_addr->wave_list[i]->waveInfo.adpcmWave.book)+(u32)(pptr));			
    }	
  }
  /* flush data cache so the new sample pointers are visible to the RSP */
  osWritebackDCacheAll();
}



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntRandom(range)

  [Parameters]
  range		upper limit of random number limit (0 to range-1)

  [Explanation]
  Calculate a random(ish) number.

  [Return value]
  random number
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static int __MusIntRandom (int range)
{
  long temp;
  int x;
  float f;
  
  for (x=0;x<8;x++)
  {
    temp = mus_random_seed & 0x48000000;
    mus_random_seed = mus_random_seed<<1;
    if (temp==0x48000000 || temp == 0x08000000)
      mus_random_seed = mus_random_seed | 1;
  }
  f = (float)(mus_random_seed)/(1<<16);
  f /= (1<<16);
  return ((int)((float)(range)*f));
}	

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntInitialiseChannel(cp)

  [Parameters]
  cp		address of music player channel

  [Explanation]
  Initialise a music player channel to it's default state whilst preserving it's
  playing status.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntInitialiseChannel(channel_t *cp)
{
  unsigned char old_playing, *work_ptr;
  int i;

  /* disable channel processing 1st!!! */
  cp->pdata = NULL;
  old_playing = cp->playing;

  /* zero out channel */
  work_ptr = (unsigned char *)cp;
  for (i=0; i<sizeof(channel_t); i++)
    *work_ptr++ = 0;

  /* set none zero values */
  cp->old_volume    	= 0xffff;
  cp->old_reverb    	= 0xff;
  cp->old_pan       	= 0xff;
  cp->old_frequency 	= 99.9;

  cp->channel_tempo = cp->channel_tempo_save = 96*256/mus_vsyncs_per_second;

  cp->length        	= 1;

  cp->velocity_on   	= 0; /* now defaults to off! (was 1) */;
  cp->default_velocity 	= /*velocity_map[*/127/*]*/;
  cp->volume		= 127;
  cp->bendrange		= 2*(1.0/64.0);
  cp->pan		= 64;

  cp->cont_vol_repeat_count = 1;
  cp->cont_pb_repeat_count = 1;

  cp->stopping  = -1;

  /* new volume and pan scales */
  cp->volscale = 0x80;
  cp->panscale = 0x80;
  cp->temscale = 0x80;
  
  /* setup a default envelope */
  cp->env_speed = 1;
  cp->env_attack_speed = 1;
  cp->env_attack_calc = 1.0F;
  cp->env_max_vol = 127;
  cp->env_decay_speed = 255;
  cp->env_decay_calc = 1.0/255.0;
  cp->env_sustain_vol = 127;
  cp->env_release_speed = 15;
  cp->env_release_calc = 1.0/15.0;

  /* set current sample bank */
  cp->sample_bank = mus_init_bank;

  /* restore channel status flag */
  cp->playing = old_playing;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntFindChannel(addr, song_chan)

  [Parameters]
  addr		address of song data
  song_chan	song channel number

  [Explanation]
  Find a music player channel for the specified song channel. 

  [Return value]
  channel found for music channel
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static int __MusIntFindChannel(song_t *addr, int song_chan)
{
  channel_t *cp;
  int i,current,current_channel;

  /* 1st scan for empty channel */
  for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
    if (!cp->pdata)
      return (i);

  /* 2nd scan for sfx channel to override */
  for (i=0, cp=mus_channels, current=0x7fffffff, current_channel=-1; i<max_channels; i++, cp++)
  {
    if (cp->IsFX)
    {
      if (cp->priority<=current)
      {
	current=cp->priority;
	current_channel=i;
      }
    }
  }
  if (current_channel!=-1)
    return (current_channel);

  /* 3rd scan for song channel (not this song) */
  for (i=0, cp=mus_channels; i<max_channels; i++)
    if (!cp->IsFX && cp->song_addr!=addr)
      return (i);

  /* 4th scan for same tune, same channel */
  for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
    if (cp->song_addr==addr && addr->ChannelData[song_chan]==cp->pbase)
      return (i);

  /* get any channel */
  return (song_chan%max_channels);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntMemSet(dest, value, length)

  [Parameters]
  dest		address of memory to set
  value		value to store
  length	number of bytes to store

  [Explanation]
  Set a block of memory to a specific byte value.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntMemSet(void *dest, unsigned char value, unsigned long length)
{
  unsigned char *a;

  a = dest;
  while (length--)
    *a++=value;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntRemapPtrs(addr, offset, count)

  [Parameters]
  addr		address of pointer list (zero pointer is kept as a NULL pointer)
  offset	offset to add to pointer
  count		number of pointers to remap

  [Explanation]
  Convert a list of offsets into pointers.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static void __MusIntRemapPtrs(void *addr, void *offset, int count)
{
  unsigned long *dest, add;
  int i;

  dest = (unsigned long *)addr;
  add = (unsigned long)offset;
  for (i=0; i<count; i++)
    if (dest[i])
      dest[i]+=add;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  __MusIntStartEffect(cp, number, volume, pan, priority)

  [Parameters]
  cp		address of music player channel
  number	sound effect number to start
  volume	volume scale
  pan		pan scale
  priority	priority level

  [Explanation]
  Start the specified sound effect on the specified sound channel with the specified
  settings.

  [Return value]
  sound handle
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static unsigned long __MusIntStartEffect (channel_t *cp, int number, int volume, int pan, int priority)
{
  u8 old_playing;

  __MusIntInitialiseChannel(cp);

  cp->IsFX	= number;
  cp->volscale	= volume;
  cp->panscale	= pan;
  cp->handle	= mus_current_handle++;
  cp->priority	= priority;
  
  /* pdata must be set last to avoid processing clash */
  cp->pdata = cp->pbase = mus_effects[number];

  return (cp->handle);
}


/* end of file */
