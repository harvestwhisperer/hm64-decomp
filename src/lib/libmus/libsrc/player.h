#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "libmus/libmus.h"
#include "libmus/libmus_data.h"

#define FORNEXT_DEPTH	4

#define MAX_SONGS		4

typedef struct
{
	unsigned short	wave;
	unsigned short	adsr;
	unsigned char	pan;
	unsigned char	pitch;	
} drum_t;

typedef struct 
{
	unsigned long	version;
	long			num_channels;	
	long			num_waves;		

#define SONGHDR_COUNT	7				
#define SONGHDR_ADR(x)	(&(x)->data_list)	

	/* channel pointers */
	unsigned char 	**data_list;	
	unsigned char	**volume_list;
	unsigned char	**pbend_list;	
	/* table pointers */
	unsigned char	*env_table;	
	drum_t			*drum_table;	
	unsigned short	*wave_table;	
	unsigned char	*master_track;	

	unsigned long	flags;
	unsigned long 	reserved1;
	unsigned long	reserved2;
	unsigned long	reserved3;
} song_t;

typedef struct
{
	unsigned char *fxdata;
	int		priority;
} fx_t;

typedef struct
{
	int			number_of_components;
	int			number_of_effects;
	int			num_waves;
	unsigned long 	flags;
	ptr_bank_t		*ptr_addr;
	unsigned short	*wave_table;
	fx_t     		effects[1];
} fx_header_t;


#define PTRFLAG_REMAPPED	(1<<31)

#define CHFLAG_PAUSE		(1<<0)
#define CHFLAG_MASTERTRACK	(1<<1)

#define FXFLAG_INITIALISED	(1<<0)

#define SONGFLAG_INITIALISED	(1<<0)



typedef struct
{
  unsigned long		channel_flag;
  unsigned char		*pdata;
  ALWaveTable		*pending;
  unsigned long		channel_frame;

  int     			stopping;
  unsigned long		volume_frame;
  unsigned long		pitchbend_frame;
  int    			stopping_speed;

  float				vib_amount;
  float				pitchbend_precalc;
  float		        old_frequency;
  float				base_note;

  float				freqoffset;
  unsigned char		*ppitchbend;
  unsigned char		*pvolume;
  unsigned long		note_end_frame;

  unsigned long		note_start_frame;
  unsigned long		handle;
  int  				priority;
  float				last_note;

  float				port_base;
  unsigned long		release_frame;
  float				env_attack_calc;
  float				env_decay_calc;

  float				env_release_calc;
  int				env_speed_calc;
  float				vibrato;
  float				bendrange;

  float				pitchbend;
  song_t			*song_addr;
  fx_header_t		*fx_addr;
  ptr_bank_t		*sample_bank;

  unsigned char		*pbase;
  drum_t 			*pdrums;
  unsigned char		*ppitchbendbase;
  unsigned char		*pvolumebase;

  float				distort;
  unsigned long		sweep_frame;  

  short				temscale;
  unsigned short	length;
  unsigned short	channel_tempo;
  short				volscale;

  unsigned short	old_volume;
  unsigned short	cont_vol_repeat_count;
  unsigned short	cont_pb_repeat_count;
  unsigned short	fx_number;
  unsigned short	channel_tempo_save;
  unsigned short	count;
  unsigned short	fixed_length;
  unsigned short	wave;

  short				panscale;
  unsigned short	cutoff;
  unsigned short	endit;
  // 8bit values...
  unsigned char		vib_delay;
  unsigned char		ignore;

  unsigned char		port;
  unsigned char		transpose;
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
  unsigned char		reverb;
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
  unsigned char		vib_speed;
  unsigned char		env_trigger_off;
  unsigned char		trigger_off;

  signed char		wobble_amount;
  unsigned char		sweep_timer;
  unsigned char		sweep_dir;
  unsigned char		for_stack_count;

  float			vib_precalc;

  unsigned char		*for_stack	[FORNEXT_DEPTH];
  unsigned char		*for_stackvol	[FORNEXT_DEPTH];
  unsigned char		*for_stackpb	[FORNEXT_DEPTH];
  unsigned short	for_vol_count	[FORNEXT_DEPTH];
  unsigned short	for_pb_count	[FORNEXT_DEPTH];
  unsigned char		for_count	[FORNEXT_DEPTH];	
  unsigned char		for_volume	[FORNEXT_DEPTH];
  float		        for_pitchbend	[FORNEXT_DEPTH];

#ifndef SUPPORT_EFFECTS
  unsigned char padding[4];
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


typedef	struct
{
  unsigned char *(*func)(channel_t *cp, unsigned char *ptr);
} command_func_t;



#endif



