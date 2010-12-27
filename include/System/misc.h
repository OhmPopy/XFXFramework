/******************************************************************************/
/*																              */
/* File: misc.h                                                               */
/* Author: bkenwright@xbdev.net                                               */
/* Desc: Misc functions, making our usb/gamepad code lib independent          */
/* Modified and SSE optimized by Halofreak1990								  */
/*                                                                            */
/******************************************************************************/
#ifndef _MISC_H
#define _MISC_H

#include <string.h>

void Sleep(long timemilliseconds);

void *memcpy(void *dest, const void *src, size_t count);

void *malloc(size_t size);

void *memset(void *s, char c, size_t count);

#endif
