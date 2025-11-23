/*
  duplicator_game_16x16.c - Wrapper for duplicator_game.c in 16x16 mode
  
  This file includes duplicator_conio_16x16.h first to define the function
  declarations, then includes duplicator_game.c.
*/

/* Include 16x16 conio header first to define DUPLICATOR_CONIO_16X16_H */
#include "duplicator_conio_16x16.h"

/* Now include the game logic */
#include "duplicator_game.c"

