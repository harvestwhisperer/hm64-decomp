#ifndef _LIBMUS_H_
#define _LIBMUS_H_

#include <ultra64.h>
#ifdef	NU_SYSTEM
#include <nusys.h>
#else
#include <sched.h>
#endif

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

typedef unsigned long	musHandle;

typedef struct 
{
  unsigned long control_flag;

  int 		channels;
#ifdef	NU_SYSTEM
  NUSched	*sched;
#else
  OSSched	*sched;
#endif
  int		thread_priority;
  unsigned char	*heap;
  int		heap_length;

  unsigned char	*ptr;
  unsigned char	*wbk;

  char		**fxs;
  int		*priority;

  int		syn_updates;
  int		syn_output_rate;
  int		syn_rsp_cmds;
  int 		syn_retraceCount;
  int		syn_num_dma_bufs;
  int		syn_dma_buf_size;
} musConfig;

#define MUSCONTROL_RAM		(1<<0)

#define MUSFLAG_EFFECTS	1
#define MUSFLAG_SONGS	2

int MusInitialize                    (musConfig *config);

extern void MusSetMasterVolume       (unsigned long flags, int volume);

extern unsigned long MusStartSong    (void *addr);
extern unsigned long MusStartEffect  (int number);
extern unsigned long MusStartEffect2 (int number, int volume, int pan , int restartflag, int priority);

extern void MusStop                  (unsigned long flags, int speed);
extern int  MusAsk                   (unsigned long flags);

extern int MusHandleStop             (unsigned long handle, int speed);
extern int MusHandleAsk              (unsigned long handle);
extern int MusHandleSetVolume        (unsigned long handle, int volume);
extern int MusHandleSetPan           (unsigned long handle, int pan);
extern int MusHandleSetFreqOffset    (unsigned long handle, float offset);
extern int MusHandleSetTempo         (unsigned long handle, int tempo);
extern int MusHandleSetReverb	     (unsigned long handle, int reverb);

extern void MusBankInitialize	        (void *pbank, void *wbank);
extern unsigned long MusBankStartSong   (void *ipbank, void *addr);
extern unsigned long MusBankStartEffect (void *ipbank, int number);
extern unsigned long MusBankStartEffect2(void *ipbank, int number, int volume, int pan , int restartflag, int priority);
extern void *MusHandleGetPtrAddr	(int handle);

#ifdef SUPPORT_SONGWAVELOOKUP
extern int    MusHandleWaveCount   (int handle);
extern short *MusHandleWaveAddress (int handle);
#endif


#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* _LIBMUS_H_ */
