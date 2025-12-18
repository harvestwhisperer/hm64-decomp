#ifndef _LIBMUS_CONFIG_H_
#define _LIBMUS_CONFIG_H_

/*#define _AUDIODEBUG */
/* 
   Includes limited validation code into the library. Reports
   various types of incorrect data or parameters via the 
   osSyncPrintf() function.
*/

#define SUPPORT_N64DD
/*
  Development code to support samples played from the N64DD
  units ROM.
  This setting should be set by default.
*/


/* NOTE:

   The following defines control various functions that may
   be included in future versions of the library. These flags
   should not be used in a standard version of the library.
*/

/*#define SUPPORT_PROFILER*/
/*
   This enables CPU performance monitoring for the main driver
   code. The variables "_mus_cpu_last" and "_mus_cpu_worst"
   become available as externals. These variables are calculated 
   using the osGetCount() function.
*/
    
/*#define SUPPORT_EFFECTS*/
/*
  Currently a Software Creations only extension to the library
  which allows the programmer to apply specific effects to the
  continuous volume and pitch bend data.
  This setting should not be used.
*/

/*#define SUPPORT_SONGWAVELOOKUP*/
/*
  Currently a Software Creations only extension which requires
  a custom version of the Music Tools SQUISH program to insert 
  wave lists into song headers. This is used to allow sample
  replacement in songs.
  This setting should not be used.
*/

/*#define SUPPORT_WAVEMONITORING*/
/*
  Currently a Software Creations only extension which counts
  the number of times a particular sample is used (8-bit only).
*/


#endif /* _LIBMUS_CONFIG_H_ */

/* end of file */
