/*
  atari_conio_compat.h - Compatibility shim for atari_conio.h

  This file provides function declarations for duplicator_game.c.
  It works with both 8x8 and 16x16 modes.

  - For 8x8 mode (duplicator.c): includes atari_conio.h
  - For 16x16 mode (duplicator_16x16.c): provides declarations only
*/

#ifndef ATARI_CONIO_COMPAT_H
#define ATARI_CONIO_COMPAT_H

/* Check if we're in 16x16 mode by seeing if duplicator_conio_16x16.h was included */
#ifndef DUPLICATOR_CONIO_16X16_H
    /* 8x8 mode - just include the regular header */
    #include "atari_conio.h"
#else
    /* 16x16 mode - declarations are already provided by duplicator_conio_16x16.h */
    /* Nothing to do here */
#endif

#endif /* ATARI_CONIO_COMPAT_H */

