/*
  atari_conio_wrapper.h - Wrapper to redirect conio calls to 16x16 versions
*/

#ifndef ATARI_CONIO_WRAPPER_H
#define ATARI_CONIO_WRAPPER_H

#include "duplicator_conio_16x16.h"

// Redirect my_cputcxy to the 16x16 version
#define my_cputcxy(x, y, c) my_cputcxy_16x16(x, y, c)

#endif

