#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "libmus.h"
#include "libmus_data.h"

/* maximum depth of for-next loops */
#define FORNEXT_DEPTH	4

/* standard Music Tools binary song header */
typedef struct 
{ 
  long		number_of_channels;
#ifdef SUPPORT_SONGWAVELOOKUP
  long 		number_of_waves;
#endif
  char 		**ChannelData;
  char 		**VolumeData;
  char 		**PitchBendData;
  char 		*EnvelopeData;
  unsigned long *DrumData;
#ifdef SUPPORT_SONGWAVELOOKUP
  short 	*WaveLookup;
#endif
} song_t;
	
#ifndef SUPPORT_SONGWAVELOOKUP
#define SONGTPTRS	5
#else
#define SONGTPTRS	6
#endif

/* flags for sample pointer bank file */
#define PTRFLAG_REMAPPED	(1<<31)


//#define SUPPORT_EFFECTS

/* music player channel structure */
typedef struct
{
  /* 32bit values... */
  unsigned char		*pdata;
  ALWaveTable		*pending;
  unsigned long		channel_frame;
  int     		stopping;

  unsigned long		volume_frame;
  unsigned long		pitchbend_frame;
  int    		stopping_speed;
  float			vib_amount;

  float			pitchbend_precalc;
  float		        old_frequency;
  float			base_note;
  float			freqoffset;

  unsigned char		*ppitchbend;
  unsigned char		*pvolume;
  unsigned long		note_end_frame;
  unsigned long		note_start_frame;

  long			handle;
  int  			priority;
  float			last_note;
  float			port_base;

  unsigned long		release_frame;
  float			env_attack_calc;
  float			env_decay_calc;
  float			env_release_calc;

  int			env_speed_calc;
  float			vibrato;
  float			bendrange;
  float			pitchbend;

  song_t		*song_addr;
  ptr_bank_t		*sample_bank;
  unsigned char		*pbase;
  unsigned char		*pdrums;

  unsigned char		*ppitchbendbase;
  unsigned char		*pvolumebase;
  float			distort;
  unsigned long		sweep_frame;
  
  // 16bit values...
  short			temscale;
  unsigned short	length;
  unsigned short	channel_tempo;
  short			volscale;
  unsigned short	old_volume;
  unsigned short	cont_vol_repeat_count;
  unsigned short	cont_pb_repeat_count;
  unsigned short	IsFX;

  unsigned short	channel_tempo_save;
  unsigned short	count;
  unsigned short	fixed_length;
  unsigned short       	wave;
  short			panscale;
  unsigned short	cutoff;
  unsigned short	endit;

  // 8bit values...
  unsigned char		vib_delay;
  unsigned char		ignore;

  unsigned char		port;
  signed char		transpose;
  unsigned char		ignore_transpose;
  unsigned char		velocity;
  unsigned char		volume;
  unsigned char		pan;
  unsigned char		old_pan;
  unsigned char		env_speed;
  unsigned char		env_init_vol;
  unsigned char		env_max_vol;
  unsigned char		env_sustain_vol;
  unsigned char		env_phase;
  unsigned char		env_current;
  unsigned char		env_count;
  unsigned char		env_attack_speed;
  unsigned char		env_decay_speed;

  unsigned char		env_release_speed;
  unsigned char		playing;
  unsigned char		reverb;			// wet/dry mix for alSynSetMix()
  unsigned char		reverb_base;
  unsigned char		old_reverb;
  unsigned char		release_start_vol;
  unsigned char		wobble_on_speed;
  unsigned char		wobble_off_speed;
  unsigned char		wobble_count;
  signed char		wobble_current;
  unsigned char		velocity_on;
  unsigned char		default_velocity;
  unsigned char		sweep_speed;
  unsigned char		vib_speed;/* zero if not active*/
  unsigned char		env_trigger_off;
  unsigned char		trigger_off;

  signed char		wobble_amount;
  unsigned char		sweep_timer;
  unsigned char		sweep_dir;
  unsigned char		for_stack_count;

  /* for-next stuff all together */
  unsigned char		*for_stack	[FORNEXT_DEPTH];
  unsigned char		*for_stackvol	[FORNEXT_DEPTH];
  unsigned char		*for_stackpb	[FORNEXT_DEPTH];
  unsigned short	for_vol_count	[FORNEXT_DEPTH];
  unsigned short	for_pb_count	[FORNEXT_DEPTH];
  unsigned char		for_count	[FORNEXT_DEPTH];	
  unsigned char		for_volume	[FORNEXT_DEPTH];
  unsigned char		for_pitchbend	[FORNEXT_DEPTH];

#ifndef SUPPORT_EFFECTS
//  char padding[4]; /* pad for data cache */
#else
  unsigned long		effect_type;
  float			specialvib_amount;
  unsigned char		specialvib_speed;
  unsigned char		specialvib_delay;
  unsigned char		last_volume;
  unsigned char		vol_effect[4];
  unsigned char		pitch_effect[4];
#endif
} channel_t;


/* sound data command function prototype */
typedef	struct
{
  unsigned char *(*func)(channel_t *cp, unsigned char *ptr);
} command_func_t;



#endif /* _PLAYER_H_ */



