/* NOTE THESE FUNCTIONS ARE ENTERED WITH THE POINTER ALREADY BEING ADVANCED PAST
   THE COMMAND <cmd> VALUE */

/* Fstop - stop data processing   
 * Format: <cmd>
 */
static unsigned char *Fstop(channel_t *cp, unsigned char *ptr)
{
  cp->pvolume = NULL;
  cp->ppitchbend = NULL;
  cp->song_addr = NULL;
  cp->IsFX = 0;
  cp->handle = 0;
  return (NULL);
}

/* Fwave - set wave number to be played
 * Format: <cmd> <wave> or <cmd> <wave&0x7F00> <wave&0xFF)
 */
static unsigned char *Fwave(channel_t *cp, unsigned char *ptr)
{
  unsigned short wave;

  wave = *ptr++;
  if(wave&0x80)
  {
    wave &= 0x7f;
    wave <<= 8;
    wave |= *ptr++;
  }
  cp->wave = wave;
  return (ptr);
}

/* Fport - enable portamento
 * Format: <cmd> <portamento>
 */
static unsigned char *Fport(channel_t *cp, unsigned char *ptr)
{
  cp->port = *ptr++;
  if (cp->port)
    cp->port_base = cp->base_note;
  return (ptr);
}

/* Fportoff - disable portamento
 * Format: <cmd>
 */
static unsigned char *Fportoff(channel_t *cp, unsigned char *ptr)
{
  cp->port = 0;
  return (ptr);
}

/* Fdefa - set envelope
 * Format: <cmd> <speed> <initial volume> <attack speed> <peak volume> 
 *               <decay speed> <sustain volume> <release speed>
 */
static unsigned char *Fdefa(channel_t *cp, unsigned char *ptr)
{
  unsigned char value;

  // get envelope speed...
  value = *ptr++;
  if (value==0)	// cannot be zero!!!
    value=1;
  cp->env_speed = value;
  cp->env_speed_calc = 1024/value;
  
  // get envelope initial volume level...
  cp->env_init_vol = *ptr++;
  
  // get attack speed...
  value = *ptr++;
#ifdef _AUDIODEBUG
  if (value==0)
  {
    osSyncPrintf("PLAYER_COMMANDS.C: Fdefa attempting to set speed of zero.\n");
    value=1;      
  }
#endif
  cp->env_attack_speed = value;

  // get peak volume...
  cp->env_max_vol = *ptr++;

  // get attack precalc value...
  cp->env_attack_calc  = (1.0 / ((float)value)) * ((float)(cp->env_max_vol-cp->env_init_vol));

  // get decay speed...
  value = *ptr++;  
#ifdef _AUDIODEBUG
  if (value==0)
  {
    osSyncPrintf("PLAYER_COMMANDS.C: Fdefa attempting to set decay speed of zero.\n");
    value=1;      
  }
#endif
  cp->env_decay_speed = value;

  // get sustain volume level...
  cp->env_sustain_vol = *ptr++;

  // get sustain precalc value...
  cp->env_decay_calc = (1.0 / ((float)value)) * ((float)(cp->env_sustain_vol-cp->env_max_vol));

  // get release speed...
  value = *ptr++;
#ifdef _AUDIODEBUG
  if (value==0)
  {
    osSyncPrintf("PLAYER_COMMANDS.C: Fdefa attempting to set release speed of zero.\n");
    value=1;      
  }
#endif
  cp->env_release_speed = value;
  cp->env_release_calc = 1.0 / ((float)value);

     
  return (ptr);
}	

/* Ftempo - set tempo
 * Format: <cmd> <tempo>
 */
static unsigned char *Ftempo(channel_t *cp, unsigned char *ptr)
{
// tempo   = bpm
// fps     = mus_vsyncs_per_second
// 120 bpm = 96 fps
// therefore tempo = bmp(required)/120*96/mus_vsyncs_per_second

  channel_t *sp;
  int	i;
  int	temp, temp2;  

  temp	= (*ptr++)*256*96/120/mus_vsyncs_per_second;	
  temp2  = (temp*cp->temscale)>>7;
  if (cp->IsFX)
  {
    cp->channel_tempo=temp;
  }
  else
  {
    for	(i=0, sp=mus_channels;i<max_channels;i++,sp++)
    {
      if (sp->song_addr==cp->song_addr)
      {
	sp->channel_tempo_save=temp;
	sp->channel_tempo=temp2;
      }
    }
  }
  return (ptr);
}

/* Ftempo - set endit value
 * Format: <cmd> <endit>
 */
static unsigned char *Fendit(channel_t *cp, unsigned char *ptr)
{
  cp->endit=*ptr++;
  cp->cutoff=0;
  return (ptr);
}

/* Fcutoff - set cutoff value
 * Format: <cmd> <cutoff&0xFF00> <cutoff&0xFF>
 */
static unsigned char *Fcutoff(channel_t *cp, unsigned char *ptr)
{
  short tmp;

  tmp = (*ptr++)<<8;
  tmp |= *ptr++;

  cp->cutoff = tmp;
  cp->endit = 0;
  return (ptr);
}

/* Fvibup - set positive vibrato
 * Format: <cmd> <delay> <speed> <amount>
 */
static unsigned char *Fvibup(channel_t *cp, unsigned char *ptr)
{

  cp->vib_delay = *ptr++;
  cp->vib_speed = *ptr++;
  cp->vib_amount = ((float)*ptr++)/50.0;
  return (ptr);
}

/* Fvibdown - set negative vibrato
 * Format: <cmd> <delay> <speed> <amount>
 */
static unsigned char *Fvibdown(channel_t *cp, unsigned char *ptr)
{
  cp->vib_delay = *ptr++;
  cp->vib_speed = *ptr++;
  cp->vib_amount = (-((float)*ptr++))/50.0;
  return (ptr);
}

/* Fviboff - disable vibrato
 * Format: <cmd>
 */
static unsigned char *Fviboff(channel_t *cp, unsigned char *ptr)
{
  cp->vib_speed=0;
  cp->vibrato=0;        // if its been switched off you want it back to a normal note
  return (ptr);
}

/* Flength - set fixed length
 * Format: <cmd> <length> or <cmd> <length&0x7F00> <length&0xFF)
 */
static unsigned char *Flength(channel_t *cp, unsigned char *ptr)
{
  unsigned char value;

  value = *ptr++;
  if(value<128)
  {
    cp->fixed_length = value;
  }
  else
  {
    cp->fixed_length = ((int)(value&0x7f)*256);
    cp->fixed_length += (int)*ptr++;
  }
  return (ptr);
}

/* Fignore - set ignore length flag
 * Format: <cmd>
 */
static unsigned char *Fignore(channel_t *cp, unsigned char *ptr)
{
  cp->ignore = 1;
  return (ptr);
}

/* Ftranspose - set transpose value
 * Format: <cmd> <transpose>
 */
static unsigned char *Ftrans(channel_t *cp, unsigned char *ptr)
{
  cp->transpose = *ptr++;
  return (ptr);
}

/* Fignore_trans - set ignore transpose flag
 * Format: <cmd>
 */
static unsigned char *Fignore_trans(channel_t *cp, unsigned char *ptr)
{
  cp->ignore_transpose = 1;
  return (ptr);
}

/* Fdistort - set distortion value
 * Format: <cmd> <distort>
 */
static unsigned char *Fdistort(channel_t *cp, unsigned char *ptr)
{
  int c;
  float f;
  
  c = (int)(*ptr++);
  if(c&0x80)
    c |= 0xffffff00;	// signed chars don't work
  f = (float)(c)/100.0;

  cp->freqoffset -= cp->distort;
  cp->freqoffset += f;
  cp->distort = f;
  return (ptr);
}

/* Fenvelope - set envelope from envelope table
 * Format: <cmd> <index> or <cmd> <index&0x7F00> <index&0xFF)
 */
static unsigned char *Fenvelope(channel_t *cp, unsigned char *ptr)
{
  int tmp;

  tmp = *ptr++;
  if(tmp&0x80)
  {
    tmp &= 0x7f;
    tmp <<= 8;
    tmp |= *ptr++;
  }
  (void)Fdefa(cp, &cp->song_addr->EnvelopeData[tmp*7]);
  return (ptr);
}

/* Fenvoff - disable envelopes
 * Format: <cmd>
 */
static unsigned char *Fenvoff(channel_t *cp, unsigned char *ptr)
{
  cp->env_trigger_off = 1;
  return (ptr);
}

/* Fenvoff - enable envelopes
 * Format: <cmd>
 */
static unsigned char *Fenvon(channel_t *cp, unsigned char *ptr)
{
  cp->env_trigger_off = 0;
  return (ptr);
}

/* Ftroff - disable triggering
 * Format: <cmd>
 */
static unsigned char *Ftroff(channel_t *cp, unsigned char *ptr)
{
  cp->trigger_off = 1;
  return (ptr);
}

/* Ftron - enable triggering
 * Format: <cmd>
 */
static unsigned char *Ftron(channel_t *cp, unsigned char *ptr)
{
  cp->trigger_off = 0;
  return (ptr);
}

/* Ffor - start for-next loop (count of 0xFF is infinite)
 * Format: <cmd> <count>
 */
static unsigned char *Ffor(channel_t *cp, unsigned char *ptr)
{
  int index;

  index = cp->for_stack_count;
  cp->for_count[index] = *ptr++;
  cp->for_stack[index] = ptr;
  cp->for_stackvol[index] = cp->pvolume;
  cp->for_stackpb[index] = cp->ppitchbend;
  cp->for_volume[index] = cp->volume;
  cp->for_pitchbend[index] = cp->pitchbend;
  cp->for_vol_count[index] = cp->cont_vol_repeat_count;
  cp->for_pb_count[index] = cp->cont_pb_repeat_count;
  cp->for_stack_count++;
  return (ptr);
}

/* Fnext - end for-next loop (count of 0xFF is infinite)
 * Format: <cmd>
 */
static unsigned char *Fnext (channel_t *cp, unsigned char *ptr)
{
  int index;

  index = cp->for_stack_count-1;
  /* infinite loop? */
  if (cp->for_count[index]!=0xff)
  { /* still looping? */
    if (--(cp->for_count[index])==0)
    {
      cp->for_stack_count = index;
      index = -1;
    }
  }
  /* unstack pointers if necessary */
  if (index>-1)
  {
    ptr = cp->for_stack[index];
    cp->pvolume = cp->for_stackvol[index];
    cp->ppitchbend = cp->for_stackpb[index];
    cp->volume = cp->for_volume[index];
    cp->pitchbend = cp->for_pitchbend[index];
    cp->cont_vol_repeat_count = cp->for_vol_count[index];
    cp->cont_pb_repeat_count = cp->for_pb_count[index];
    cp->pitchbend_precalc = cp->pitchbend*cp->bendrange;
  }
  return (ptr);
}

/* Fwobble - define wobble settings
 * Format: <cmd> <amount> <on speed> <off speed>
 */
static unsigned char *Fwobble(channel_t *cp, unsigned char *ptr)
{
  cp->wobble_amount = *ptr++;
  cp->wobble_on_speed = *ptr++;
  cp->wobble_off_speed = *ptr++;
  return(ptr);
}

/* Fwobble - disable wobble
 * Format: <cmd>
 */
static unsigned char *Fwobbleoff(channel_t *cp, unsigned char *ptr)
{
  cp->wobble_on_speed = 0;
  return (ptr);
}

/* Fvelon - enable velocity mode
 * Format: <cmd>
 */
static unsigned char *Fvelon(channel_t *cp, unsigned char *ptr)
{  
  cp->velocity_on = 1;
  return (ptr);
}

/* Fvelon - disable velocity mode
 * Format: <cmd>
 */
static unsigned char *Fveloff(channel_t *cp, unsigned char *ptr)
{
  cp->velocity_on = 0;
  return (ptr);
}

/* Fvelon - enable default velocity mode
 * Format: <cmd> <default velocity>
 */

/* default velocity map */
static unsigned char *Fvelocity(channel_t *cp, unsigned char *ptr)
{
  cp->default_velocity 	= /*velocity_map[*/*ptr++/*]*/;
  cp->velocity_on = 0;
  return (ptr);
}

/* Fpan - set pan position
 * Format: <cmd> <pan*2>
 */
static unsigned char *Fpan(channel_t *cp, unsigned char *ptr)
{
  cp->pan = (*ptr++)/2;
  return (ptr);
}

/* Fstereo - OUTDATED
 * Format: <cmd> <ignored> <ignored>
 */
static unsigned char *Fstereo(channel_t *cp, unsigned char *ptr)
{
  return (ptr+2);
}

/* Fdrums - enable drum data
 * Format: <cmd> <index>
 */
static unsigned char *Fdrums(channel_t *cp, unsigned char *ptr)
{
  unsigned long addr;

  addr = (unsigned long)cp->song_addr+(cp->song_addr->DrumData[*ptr++]);
  cp->pdrums = (unsigned char *)addr;
  return (ptr);
}

/* Fdrumsoff - disable drum data
 * Format: <cmd>
 */
static unsigned char *Fdrumsoff(channel_t *cp, unsigned char *ptr)
{
  cp->pdrums = NULL;
  return (ptr);
}

/* Fprint - display debugging info (DEBUG MODE ONLY!!!)
 * Format: <cmd> <data to print>
 */
static unsigned char *Fprint(channel_t *cp, unsigned char *ptr)
{
#ifdef _AUDIODEBUG
  osSyncPrintf("PLAYER_COMMANDS.C: Fprint -  %d (channel frame=%d)\n", *ptr++,  cp->channel_frame);
  return (ptr);
#else
  ptr++;
  return (ptr);
#endif
}

/* Fgoto - sound data ptr relative jump
 * Format: <cmd> <song data offset&0xFF00> <song data offset&0xFF>
 *               <volume data offset&0xFF00> <volume data offset&0xFF>
 *               <pitchbend data offset&0xFF00> <pitchbend data offset&0xFF>
 */
static unsigned char *Fgoto(channel_t *cp, unsigned char *ptr)
{
  int off, off1;

  /* 2 bytes for song offset */
  off1 = *ptr++<<8;
  off1 += *ptr++;

  /* get volume offset BEFORE updating pointer */
  /* 2 bytes for volume offset (never inside a run length bit) */
  off = *ptr++<<8;
  off += *ptr++;
  cp->pvolume = cp->pvolumebase+off;
  cp->cont_vol_repeat_count = 1;
  
  /* get pitchbend offset BEFORE updating pointer */
  /* 2 bytes for pitchbend offset (never inside a run length bit) */
  off = *ptr++<<8;
  off += *ptr++;
  cp->ppitchbend = cp->ppitchbendbase+off;
  cp->cont_pb_repeat_count = 1;
  
  return (cp->pbase+off1);
}

/* Freverb - set reverb value
 * Format: <cmd> <reverb>
 */
static unsigned char *Freverb(channel_t *cp, unsigned char *ptr)
{
  cp->reverb = *ptr++;
  return (ptr);
}

/* FrandNote - set random transpose (within a range)
 * Format: <cmd> <range> <offset>
 */
static unsigned char *FrandNote(channel_t *cp, unsigned char *ptr)
{	
  // rand_amount,rand_base  -- 20,-3 would give -3 to 16 as the value
  cp->transpose = __MusIntRandom(*ptr++);
  cp->transpose += *ptr++;
  return (ptr);
}

/* FrandVolume - set random volume level
 * Format: <cmd> <range> <offset>
 */
static unsigned char *FrandVolume(channel_t *cp, unsigned char *ptr)
{
  // rand_amount,base
  cp->volume = __MusIntRandom(*ptr++);
  cp->volume += *ptr++;
  return (ptr);
}

/* FrandPan - set random pan position
 * Format: <cmd> <range> <offset>
 */
static unsigned char *FrandPan(channel_t *cp, unsigned char *ptr)
{
  // rand_amount,base
  cp->pan = __MusIntRandom(*ptr++);
  cp->pan += *ptr++;
  return (ptr);
}

/* Fvolume - set volume level
 * Format: <cmd> <volume>
 */
static unsigned char *Fvolume(channel_t *cp, unsigned char *ptr)
{
  cp->volume = *ptr++;
  return (ptr);
}

/* Fstartfx - start sound effect
 * Format: <cmd> <number>
 */
static unsigned char *Fstartfx(channel_t *cp, unsigned char *ptr)
{
  int	i;
  int	num;
  channel_t *sp;
  long	search_handle;
  long	old_handle;

  num = *ptr++;
  if (num>=0x80)
    num = ((num&0x7f)<<8)+*ptr++;
  old_handle=cp->handle;
  /* change priority so child fx can never overwrite parent */
  search_handle = MusStartEffect2(num,cp->volume,cp->pan,0,cp->priority++);
  cp->priority--; /* change priority back! */
  if (!search_handle)
    return(ptr);
  for(i=0, sp=mus_channels; i<max_channels; i++, sp++)
  {
    if (sp->handle == search_handle)
    {
      sp->handle=cp->handle;
      sp->sample_bank = cp->sample_bank;
    }
  }
  return (ptr);
}

/* Fbendrange - set bend range
 * Format: <cmd> <bend range>
 */
static unsigned char *Fbendrange(channel_t *cp, unsigned char *ptr)
{
  cp->bendrange = (float)(*ptr++)*(1.0/64.0);
  cp->pitchbend_precalc = cp->pitchbend*cp->pitchbend;
  return (ptr);
}

/* Command : Setup pan sweeping
   Format  : Csweep, speed */
static unsigned char *Fsweep(channel_t *cp, unsigned char *ptr)
{
  cp->sweep_speed = *ptr++;
  return (ptr);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/* array of functions for music data commands */

command_func_t jumptable[]=
{
  Fstop,		//80
  Fwave,		//81
  Fport,		//82
  Fportoff,		//83
  Fdefa,		//84
  Ftempo,		//85
  Fcutoff,		//86
  Fendit, 		//87
  Fvibup,		//88
  Fvibdown,		//89
  Fviboff,		//8a
  Flength, 		//8b
  Fignore, 		//8c
  Ftrans, 		//8d
  Fignore_trans, 	//8e
  Fdistort,		//8f
  Fenvelope,		//90
  Fenvoff,		//91
  Fenvon, 		//92
  Ftroff, 		//93
  Ftron,  		//94
  Ffor, 		//95
  Fnext,		//96
  Fwobble, 		//97
  Fwobbleoff, 		//98
  Fvelon, 		//99
  Fveloff, 		//9a
  Fvelocity, 		//9b
  Fpan, 		//9c
  Fstereo, 		//9d
  Fdrums, 		//9e
  Fdrumsoff, 		//9f
  Fprint, 		//a0
  Fgoto, 		//a1
  Freverb, 		//a2
  FrandNote, 		//a3
  FrandVolume,		//a4
  FrandPan,		//a5
  Fvolume,		//a6
  Fstartfx,		//a7
  Fbendrange,		//a8
  Fsweep,		//a8
};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/


/* end of file */
