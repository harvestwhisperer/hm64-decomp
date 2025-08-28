/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetTempo(config)

  [Parameters]
  config	address of configuration structure

  [Explanation]
  Initialise the music player and the audio manager.

  [Return value]
  amount of memory used from the audio heap
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusInitialize(musConfig *config)
{
#ifdef	NU_SYSTEM
  ALVoiceConfig vc;  
  int i;

  /* main control flag */
  __muscontrol_flag = config->control_flag;

  /* copy player settings first */
  max_channels = config->channels;
  mus_effects  = config->fxs;
  mus_priority = config->priority;

  /* get video refresh rate */
  if (osTvType==0)
    mus_vsyncs_per_second = 50;
  else
    mus_vsyncs_per_second = 60;
  mus_next_frame_time = 1000000/mus_vsyncs_per_second;

  /* claim and clear memory for syn voices and mus channels */
  mus_voices = alHeapAlloc(&nuAuHeap, 1, max_channels*sizeof(ALVoice));
  mus_channels = alHeapAlloc(&nuAuHeap, 1, max_channels*sizeof(channel_t));
  __MusIntMemSet(mus_voices,0,max_channels*sizeof(ALVoice));
  __MusIntMemSet(mus_channels,0,max_channels*sizeof(channel_t));

  /* set volumes to maxiumum level */
  MusSetMasterVolume(MUSFLAG_EFFECTS|MUSFLAG_SONGS, 0x7fff);
  /* initialise player vars */
  mus_current_handle = 1;
  mus_random_seed = 0x12345678;

  /*
   * Sign into the synthesis driver so that the next time
   * alAudioFrame is called, the __MusIntMain function will be
   * called.
   */

  plr_player.next       = NULL;
  plr_player.handler    = __MusIntMain;
  plr_player.clientData = &plr_player;
  alSynAddPlayer(&auGlobal.drvr, &plr_player);
  
  for(i=0; i<max_channels; i++)
  {    
    mus_channels[i].playing = 0;
    __MusIntInitialiseChannel(&mus_channels[i]);
    
    vc.unityPitch = 0;
    vc.priority = config->thread_priority;
    vc.fxBus = 0;
    
    alSynAllocVoice(&auGlobal.drvr, &mus_voices[i], &vc);
  }
  return (nuAuHeap.cur-nuAuHeap.base);

#else

  ALVoiceConfig vc;  
  ALSynConfig c;
  amConfig amc;
  int i;

  /* main control flag */
  __muscontrol_flag = config->control_flag;

  /* copy player settings first */
  max_channels = config->channels;
  mus_effects  = config->fxs;
  mus_priority = config->priority;

  /* get video refresh rate */
  if (osTvType==0)
    mus_vsyncs_per_second = 50;
  else
    mus_vsyncs_per_second = 60;
  mus_next_frame_time = 1000000/mus_vsyncs_per_second;

  /* initialise heap */
  __MusIntMemSet(config->heap, 0, config->heap_length);
  alHeapInit(&heap, config->heap, config->heap_length);

  /* claim and clear memory for syn voices and mus channels */
  mus_voices = alHeapAlloc(&heap, 1, max_channels*sizeof(ALVoice));
  mus_channels = alHeapAlloc(&heap, 1, max_channels*sizeof(channel_t));
  __MusIntMemSet(mus_voices,0,max_channels*sizeof(ALVoice));
  __MusIntMemSet(mus_channels,0,max_channels*sizeof(channel_t));

  /* initialse samples */
  __MusIntRemapPtrBank(config->ptr, config->wbk);
  mus_default_bank = mus_init_bank = (ptr_bank_t *)config->ptr;

  /*
   * Create the Audio Manager
   */

  c.maxVVoices = max_channels;
  c.maxPVoices = max_channels;
  c.maxUpdates = config->syn_updates;
  c.dmaproc    = 0;                  /* audio mgr will fill this in */
  c.fxType     = AL_FX_BIGROOM;
  c.outputRate = 0;                  /* audio mgr will fill this in */
  c.heap       = &heap;

  amc.outputRate = config->syn_output_rate;
  amc.framesPerField = config->syn_retraceCount;
  amc.maxACMDSize = config->syn_rsp_cmds;

  amCreateAudioMgr(config->sched, &c, config->thread_priority, &amc, 
		   config->syn_num_dma_bufs, config->syn_dma_buf_size, 
		   mus_vsyncs_per_second);

  /* set volumes to maxiumum level */
  MusSetMasterVolume(MUSFLAG_EFFECTS|MUSFLAG_SONGS, 0x7fff);
  /* initialise player vars */
  mus_current_handle = 1;
  mus_random_seed = 0x12345678;

  /*
   * Sign into the synthesis driver so that the next time
   * alAudioFrame is called, the __MusIntMain function will be
   * called.
   */

  plr_player.next       = NULL;
  plr_player.handler    = __MusIntMain;
  plr_player.clientData = &plr_player;
  alSynAddPlayer(&__am.g.drvr, &plr_player);
  
  for(i=0; i<max_channels; i++)
  {    
    mus_channels[i].playing = 0;
    __MusIntInitialiseChannel(&mus_channels[i]);
    
    vc.unityPitch = 0;
    vc.priority = config->thread_priority;
    vc.fxBus = 0;
    
    alSynAllocVoice(&__am.g.drvr, &mus_voices[i], &vc);
  }
  return (heap.cur-heap.base);

#endif	/* NU_SYSTEM */
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusSetAllMasterVolumes(flags,volume)

  [Parameters]
  flags		bit pattern indicating which types to stop
  volume	volume level (0-0x7FFF)

  [Explanation]
  Set the song and sound effect master volume level.

  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusSetMasterVolume(unsigned long flags, int volume)
{
  if (flags&MUSFLAG_EFFECTS)
    mus_master_volume_effects = volume;
  if (flags&MUSFLAG_SONGS)
    mus_master_volume_songs = volume;
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartSong(addr)

  [Parameters]
  addr		address of binary song data in RAM (must be 32-bit aligned)

  [Explanation]
  This function will start the song at the given address in RAM.

  [Return value]
  sound handle
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned long MusStartSong(void *addr)
{
  song_t *song_addr;
  unsigned long handle, *work_addr;
  int i, count;
  channel_t *cp;


  song_addr = addr;
  work_addr = addr;
  count = *work_addr++;
#ifdef SUPPORT_SONGWAVELOOKUP
  work_addr++;
#endif
  /* remap song if necessary */
  if (*work_addr<0x400)
  {
    /* convert main offsets to pointers */
    __MusIntRemapPtrs(work_addr, addr, SONGTPTRS);
    /* convert channel, volume and pitch bend offsets to pointers */
    __MusIntRemapPtrs(song_addr->ChannelData,    addr, count);
    __MusIntRemapPtrs(song_addr->VolumeData,     addr, count);
    __MusIntRemapPtrs(song_addr->PitchBendData,  addr, count);
  }

  /* get next handle */
  handle = mus_current_handle++;

  for(i=0; i<count; i++)
  {
    if(song_addr->ChannelData[i] != NULL)
    {
      cp=mus_channels+__MusIntFindChannel(song_addr,i);
      __MusIntInitialiseChannel(cp);
      cp->song_addr = song_addr;
      cp->pvolume = cp->pvolumebase = song_addr->VolumeData[i];
      cp->ppitchbend = cp->ppitchbendbase = song_addr->PitchBendData[i];
      /* pdata must be set last to avoid processing clash */
      cp->pdata = cp->pbase = song_addr->ChannelData[i];
      cp->handle = handle;

    }
  }
  return (handle);
}


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartEffect(number)

  [Parameters]
  number	sound effect number to start

  [Explanation]
  Start the specified sound effect with default parameters.

  [Return value]
  sound effect handle (0=sound effect not triggered)
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned long MusStartEffect(int number)
{
  channel_t *cp, *current_cp;
  int i, priority, current_priority;
	
  priority = mus_priority[number];
  current_priority = priority+1;
  for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
  {
    if (cp->pdata==NULL)
      return (__MusIntStartEffect(cp, number, 0x80, 0x80, priority));
    if (cp->IsFX && cp->priority<current_priority)
    {
      current_priority = cp->priority;
      current_cp = cp;
    }
  }
  if (current_priority<priority)
      return (__MusIntStartEffect(current_cp, number, 0x80, 0x80, priority));
  return (0);
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStartEffect2(number, volume, pan, restartflag, priority)

  [Parameters]
  number	sound effect number to start
  volume	volume scale
  pan		pan scale
  restartflag	flag to control if sound effect should overwrite effects with
                the same number (0=no, <>0=yes)
  priority	sound effects priority (-1 = use Music Tools priority level)

  [Explanation]
  Start the specified sound effect with given parameters.

  [Return value]
  sound effect handle (0=sound effect not triggered)
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned long MusStartEffect2(int number, int volume, int pan , int restartflag, int priority)
{
  channel_t *cp, *current_cp;
  int i, current_priority;
	
  if (priority==-1)
    priority = mus_priority[number];

  if (restartflag)
  {
    for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
      if (cp->IsFX == number)
	return (__MusIntStartEffect(cp, number, volume, pan, priority));
  }
  current_priority = priority+1;
  for(i=0, cp=mus_channels; i<max_channels; i++, cp++)
  {
    if (cp->pdata==NULL)
      return (__MusIntStartEffect(cp, number, volume, pan, priority));
    if (cp->IsFX && cp->priority<current_priority)
    {
      current_priority = cp->priority;
      current_cp = cp;
    }
  }
  if (current_priority<priority)
      return (__MusIntStartEffect(current_cp, number, volume, pan, priority));
  return (0);
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusStop(flags, speed)

  [Parameters]
  flags		bit pattern indicating which types to stop
  speed		number of video frames to stop in

  [Explanation]
  Stop all the specified sound types in the specified number of video frames.

  [Return value]
  NONE
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusStop(unsigned long flags, int speed)
{
  int i, speed2;
  channel_t *cp;

#ifdef _AUDIODEBUG
  if (speed<0)
    osSyncPrintf("PLAYER.C: Stop all function called with invalid speed setting.\n");
  speed = 0;
#endif
  speed2 = speed ? speed : 1;
  for (i=0, cp=mus_channels; i<max_channels; i++, cp++)
  {
    if ((cp->IsFX && flags&MUSFLAG_EFFECTS) || (!cp->IsFX && flags&MUSFLAG_SONGS))
    {
      cp->stopping = speed;
      cp->stopping_speed = speed2;
    }
  }
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusAsk(flags)

  [Parameters]
  flags		bit pattern indicating which types to test

  [Explanation]
  Count the number of channels that are currently playing.

  [Return value]
  number of channels currently playing
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusAsk(unsigned long flags)
{
  int i, count;
  channel_t *cp;

  for (i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->pdata)
    {
      if ((cp->IsFX && flags&MUSFLAG_EFFECTS) || (!cp->IsFX && flags&MUSFLAG_SONGS))
	count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleStop(handle, speed)

  [Parameters]
  handle	sound handle
  speed		number of video frames to stop in

  [Explanation]
  Stop the channels associated with the sound handle in a specified number of video 
  frames.

  [Return value]
  number of channels set to stop
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleStop(unsigned long handle, int speed)
{
  int i,speed2, count;
  channel_t *cp;

  if (!handle)
    return(0);

#ifdef _AUDIODEBUG
  if (speed<0)
    osSyncPrintf("PLAYER.C: MusHandleStop called with invalid speed setting.\n");
  speed = 0;
#endif
  speed2 = speed ? speed : 1;
  for (i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->stopping = speed;
      cp->stopping_speed = speed2;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleAsk(handle)

  [Parameters]
  handle	sound handle

  [Explanation]
  Count the number of channels associated with the specified sound handle that are 
  currently playing.

  [Return value]
  number of channels currently playing
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleAsk(unsigned long handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);
  
  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
    if (cp->handle==handle)
      count++;
  return(count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetVolume(handle, volume)

  [Parameters]
  handle	sound handle
  volume	volume scale

  [Explanation]
  Set the volume scale level of the channels associated with the specified sound
  handle. This value scales the channels volume level (from the sound data) in the 
  following mannor:
          volume = (volume*volumescale)>>7

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetVolume(unsigned long handle, int volume)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->volscale = volume;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetPan(handle, pan)

  [Parameters]
  handle	sound effect handle
  pan		pan scale

  [Explanation]
  Set the pan scale of the channels associated with the specified sound handle. This
  value scales the channels pan value (from the sound data) in the following mannor:
          pan = (pan*panscale)>>7

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetPan (unsigned long handle, int pan)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->panscale = pan;
      cp->old_pan = 0xff;
      count++;
    }
  }
  return (count);
}


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetFreqOffset(handle, offset)

  [Parameters]
  handle	sound handle
  offset	frequency offset

  [Explanation]
  Set the frequency offset value of the channels associated with the specified sound 
  handle. This value is added to the frequency that is to be output.

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetFreqOffset(unsigned long handle, float offset)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->freqoffset = offset+cp->distort;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetTempo(handle, tempo)

  [Parameters]
  handle	sound effect handle
  tempo		tempo scale

  [Explanation]
  Set the tempo scale value of the channels associated with the specified sound 
  handle. This value scales the channels tempo value (from the sound data) in 
  the following mannor:
          tempo = (tempo*temposcale)>>7

  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetTempo(unsigned long handle, int tempo)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  if (tempo<1)
    tempo=1;
  else if (tempo>256)
    tempo=256;

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->temscale = tempo;
      cp->channel_tempo = (cp->channel_tempo_save*tempo)>>7;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleSetReverb(handle, reverb)

  [Parameters]
  handle	sound effect handle
  reverb       	reverb offset

  [Explanation]
  Set the reverb offset value of the channels associated with the specified sound
  handle. This value is used as a base level with the levels defined in the sound
  data being scaled (with 128-base level) and added to the offset. This can be shown as
  follows:
          channelreverb = reverb+(((128-reverb)*datareverb)>>7)

  NOTE: the change in the base level will only become apparent when a new note is
  fetched on the channels associated with the sound handle
  
  [Return value]
  number of channels which have been changed
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

int MusHandleSetReverb(unsigned long handle, int reverb)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  if (reverb<0)
    reverb=0;
  else if (reverb>127)
    reverb=127;

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      cp->reverb_base = reverb;
      cp->old_reverb = 0xff;
      count++;
    }
  }
  return (count);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusBankInitialise(pbank, wbank)

  [Parameters]
  pbank		address of pointer bank
  wbank		address of wave bank

  [Explanation]
  Initialise a sample bank. This function remaps various pointers within the sample 
  pointer bank referencing both itself and the sample wave bank.

  NOTE: The address of the sample pointer bank must be 32-bit aligned although it is
  recommended it be cache aligned.
  
  [Return value]
  none
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void MusBankInitialize(void *pbank, void *wbank)
{
  __MusIntRemapPtrBank(pbank, wbank);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusBankStartSong(ipbank, addr)

  [Parameters]
  ipbank	address of an initialised sample bank
  addr		address of binary song data in RAM (must be 32-bit aligned)

  [Explanation]
  This function will start the song at the given address in RAM and the given sample
  bank.

  [Return value]
  sound handle
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned long MusBankStartSong(void *ipbank, void *addr)
{
  unsigned long handle;
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
#ifdef _AUDIODEBUG
    else
      osSyncPrintf("PLAYER_API.C: Calling MusBankStartSong with invalid pointer bank.\n");
#endif
  }
  handle = MusStartSong(addr);
  mus_init_bank = mus_default_bank;
  return (handle);
}


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusBankStartEffect(ipbank, number)

  [Parameters]
  ipbank	address of an initialised sample bank
  number	sound effect number to start

  [Explanation]
  Start the specified sound effect with default parameters and the given sample bank.

  [Return value]
  sound effect handle (0=sound effect not triggered)
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned long MusBankStartEffect(void *ipbank, int number)
{
  unsigned long handle;
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
  }
  
  handle = MusStartEffect(number);
  mus_init_bank = mus_default_bank;
  return (handle);
}	

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusBankStartEffect2(number, volume, pan, restartflag, priority)

  [Parameters]
  ipbank	address of an initialised sample bank
  number	sound effect number to start
  volume	volume scale
  pan		pan scale
  restartflag	flag to control if sound effect should overwrite effects with
                the same number (0=no, <>0=yes)
  priority	sound effects priority (-1 = use Music Tools priority level)

  [Explanation]
  Start the specified sound effect with given parameters and the given sample bank.

  [Return value]
  sound effect handle (0=sound effect not triggered)
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

unsigned long MusBankStartEffect2(void *ipbank, int number, int volume, int pan , int restartflag, int priority)
{
  unsigned long handle;
  ptr_bank_t *pptr;

  if (ipbank)
  {
    pptr = (ptr_bank_t *)ipbank;
    if (pptr->flags&PTRFLAG_REMAPPED)
      mus_init_bank = pptr;    
  }
  
  handle = MusStartEffect2(number, volume, pan, restartflag, priority);
  mus_init_bank = mus_default_bank;
  return (handle);
}	


/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION]
  MusHandleGetPtrAddr(handle)

  [Parameters]
  handle	song handle

  [Explanation]
  Return the address of the sample pointer bank the first channel related to the 
  specified handle uses.
  
  [Return value]
  address of sample pointer bank or NULL if handle is not found
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

void *MusHandleGetPtrAddr(int handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
    if (cp->handle==handle)
      return (cp->sample_bank);
  return (0);
}

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION - DEVELOPMENT]
  MusHandleWaveCount(handle)

  [Parameters]
  handle	song handle

  [Explanation]
  Return the number of waves the specified song uses. This function is only available
  if the library has been compiled with SUPPORT_SONGWAVELOOKUP defined and the songs 
  have been saved with the additional data in the header.
  
  [Return value]
  number of entries in the song wave lookup table
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

#ifdef SUPPORT_SONGWAVELOOKUP
int MusHandleWaveCount(int handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (0);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      if (cp->song_addr)
	return (cp->song_addr->number_of_waves);
    }
  }   
  return (0);
}
#endif

/*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
  [API FUNCTION - DEVELOPMENT]
  MusHandleWaveAddress(handle)

  [Parameters]
  handle	song handle

  [Explanation]
  Return the address of the wave lookup table associated with the specified song. This
  function is only available if the library has been compiled with SUPPORT_SONGWAVELOOKUP
  defined and the songs have been saved with the additional data in the header.
  
  [Return value]
  address of the song wave lookup table
*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*/

#ifdef SUPPORT_SONGWAVELOOKUP
short *MusHandleWaveAddress(int handle)
{
  channel_t *cp;
  int i, count;

  if (!handle)
    return (NULL);

  for(i=0, cp=mus_channels, count=0; i<max_channels; i++, cp++)
  {
    if (cp->handle==handle)
    {
      if (cp->song_addr)
	return (cp->song_addr->WaveLookup);
    }
  }   
  return (NULL);
}
#endif



/* end of file */
