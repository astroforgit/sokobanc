/* duplicator_font_16x16.c - 16x16 tile graphics for Duplicator game */
/* Runtime scaling of 8x8 graphics to 16x16 by doubling each pixel */

#include <stdlib.h>
#include <string.h>
#include "duplicator_font_16x16.h"

typedef unsigned char byte;

/* Scale one 8x8 tile (8 bytes) to 16x16 (32 bytes = 4 chars) */
/* Each pixel becomes 2x2 pixels */
void scale_8x8_to_16x16(const byte* src_8x8, byte* dest_16x16) {
    byte row, bit;

    byte* tl = dest_16x16;       // Top-left char (8 bytes)
    byte* tr = dest_16x16 + 8;   // Top-right char (8 bytes)
    byte* bl = dest_16x16 + 16;  // Bottom-left char (8 bytes)
    byte* br = dest_16x16 + 24;  // Bottom-right char (8 bytes)

    // Process each row of the 8x8 source
    for (row = 0; row < 8; row++) {
        byte src_byte = src_8x8[row];

        // Scale horizontally: each bit becomes 2 bits
        // Left half (bits 7-4) and right half (bits 3-0)
        byte left_half = 0;
        byte right_half = 0;

        for (bit = 0; bit < 8; bit++) {
            if (src_byte & (1 << (7 - bit))) {
                // Pixel is set - set 2 bits in output
                if (bit < 4) {
                    // Left half of source -> left char
                    left_half |= (3 << (6 - bit * 2));
                } else {
                    // Right half of source -> right char
                    right_half |= (3 << (6 - (bit - 4) * 2));
                }
            }
        }

        // Each source row becomes 2 destination rows (vertical doubling)
        if (row < 4) {
            // Top half -> top-left and top-right chars
            tl[row * 2] = left_half;
            tl[row * 2 + 1] = left_half;
            tr[row * 2] = right_half;
            tr[row * 2 + 1] = right_half;
        } else {
            // Bottom half -> bottom-left and bottom-right chars
            bl[(row - 4) * 2] = left_half;
            bl[(row - 4) * 2 + 1] = left_half;
            br[(row - 4) * 2] = right_half;
            br[(row - 4) * 2 + 1] = right_half;
        }
    }
}

