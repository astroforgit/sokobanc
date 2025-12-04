/*
  test_conio.c - Mock console I/O library for testing duplicator game
  Implementation file
*/

#include "test_conio.h"

// Screen buffer (simulated screen memory)
byte test_screen_buffer[ROWS][COLS];

void my_clrscr(void) {
    memset(test_screen_buffer, ' ', sizeof(test_screen_buffer));
}

void my_cputcxy(byte x, byte y, byte character) {
    if (x < COLS && y < ROWS) {
        test_screen_buffer[y][x] = character;
    }
}

void my_cputsxy(byte x, byte y, const char* str) {
    byte offset = 0;
    while (*str && (x + offset) < COLS && y < ROWS) {
        test_screen_buffer[y][x + offset] = *str;
        str++;
        offset++;
    }
}

void my_cprintf_status(byte b, byte t, byte m) {
    char buf[41];
    sprintf(buf, "Boxes: %d/%d  Moves: %d", b, t, m);
    my_cputsxy(0, 23, buf);
}

void wait_vblank(void) {
    // Do nothing in test mode
}

void print_screen(void) {
    byte x, y;
    printf("\n");
    for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLS; x++) {
            byte ch = test_screen_buffer[y][x];
            if (ch == 0 || ch == ' ') {
                putchar(' ');
            } else {
                putchar(ch);
            }
        }
        printf("\n");
    }
}

byte get_screen_char(byte x, byte y) {
    if (x < COLS && y < ROWS) {
        return test_screen_buffer[y][x];
    }
    return 0;
}

