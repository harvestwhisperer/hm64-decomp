#ifndef __log__
#define __log__

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/ultratypes.h>

#define OS_LOG_MAX_ARGS         16
#define OS_LOG_MAGIC            0x20736a73
#define OS_LOG_FLOAT(x)         (*(int *) &(x))
#define OS_LOG_VERSION          1
    
typedef struct {
    u32 magic;          /* log identifier                       */
    u32 len;            /* length of log data + log structure   */
    u32 *base;          /* starting addr array                  */
    s32 startCount;     /* read offset from dataBase            */
    s32 writeOffset;    /* write offset from dataBase           */
} OSLog;

typedef struct {
    u32 magic;
    u32 timeStamp;
    u16 argCount;
    u16 eventID;
} OSLogItem;

typedef struct {
    u32 magic;          /* log identifier */
    u32 version;        /* 1 */
} OSLogFileHdr;

void    osCreateLog(OSLog *log, u32 *base, s32 len);
void    osLogEvent(OSLog *log, s16 code, s16 numArgs, ...);
void    osFlushLog(OSLog *log);
u32	osLogFloat(f32);

extern void osDelay(int count);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif
