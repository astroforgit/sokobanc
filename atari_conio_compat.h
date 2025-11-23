/*
  atari_conio_compat.h - Compatibility shim for atari_conio.h

  This file provides function declarations for duplicator_game.c.
  It works with both 8x8 and 16x16 modes.
*/

#ifndef ATARI_CONIO_COMPAT_H
#define ATARI_CONIO_COMPAT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned char byte;
typedef unsigned short word;

/* Function declarations that duplicator_game.c needs */
/* These are implemented in either atari_conio.c or duplicator_conio_16x16.c */
void my_cputcxy(byte x, byte y, byte character);
void my_clrscr(void);
void my_cputsxy(byte x, byte y, const char* str);
void my_cprintf_status(byte b, byte t, byte m);
void wait_vblank(void);

/* POKE/PEEK macros */
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))

#endif /* ATARI_CONIO_COMPAT_H */

