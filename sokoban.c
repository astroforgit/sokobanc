/*
  A simple Sokoban game with a custom character set.
  DEFINITIVE WORKING VERSION - Fixes all animation bugs by updating both
  player characters and synchronizing the game loop to the screen refresh.
*/

#include <stdlib.h>
#include <string.h>
#include <joystick.h>
#include <conio.h>
#include <atari.h>
#include <peekpoke.h>
#include <stdio.h>

// Data types for clarity
typedef unsigned char byte;
typedef unsigned short word;

// Screen dimensions & Manual Memory Layout
#define COLS 40
#define ROWS 24
#define CHARSET_MEM ((byte*)0x7000)
#define DLIST_MEM   ((byte*)0x8000)
#define SCREEN_MEM  ((byte*)0x9000)
#define ROM_CHARSET_ADDRESS 0xE000

// Game Tile Definitions
#define TILE_EMPTY       0
#define TILE_WALL       '#'
#define TILE_GOAL       '.'
#define TILE_BOX        '$'
#define TILE_BOX_ON_GOAL '*'
#define TILE_PLAYER     '@'
#define TILE_PLAYER_ON_GOAL '+'

// Game State & Data
char level[ROWS][COLS];
byte player_x, player_y, total_boxes = 0, boxes_on_goals = 0, moves = 0;
byte frame_counter = 0;
byte anim_state = 0;

// Graphics Data with Animation Frames
unsigned char custom_graphics[] = {
    /*  0 */ 0xFF,0x81,0xBD,0xA5,0xA5,0xBD,0x81,0xFF, // # Wall
    /*  8 */ 0x7E,0x42,0x5A,0x5A,0x5A,0x5A,0x42,0x7E, // $ Box
    /* 16 */ 0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x00, // . Goal
    /* 24 */ 0x7E,0x42,0x42,0x5A,0x5A,0x42,0x42,0x7E, // * Box on Goal
    
    // Player Animation Frames
    /* 32: Player Standing */
    0x00,0x18,0x18,0x7E,0x18,0x18,0x24,0x66,
    /* 40: Player Blinking */
    0x00,0x18,0x18,0x7E,0x18,0x00,0x24,0x66,
    /* 48: Player Walking */
    0x00,0x18,0x18,0x7E,0x18,0x18,0x66,0x24
};

const char* level_1[]={"    #####","    #   #","    #$  #","  ###  $##","  #  $ $ #","### # ## #   ######","#   # ## #####  ..#","# $  $          ..#","##### ### #@##  ..#","    #     #########","    #######"};

//----------------------------------------------------------------------
// ** RELIABLE SCREEN & TIMING FUNCTIONS **
//----------------------------------------------------------------------
void my_clrscr(void) { memset(SCREEN_MEM, TILE_EMPTY, COLS * ROWS); }
void my_cputcxy(byte x, byte y, byte character) { word offset=(word)y*COLS+x; SCREEN_MEM[offset]=character; }
void my_cputsxy(byte x, byte y, const char* str) { word offset=(word)y*COLS+x; while(*str){byte v=*str++;if(v>=0x20&&v<=0x5f)SCREEN_MEM[offset++]=v-0x20;else if(v>=0x60&&v<=0x7f)SCREEN_MEM[offset++]=v-0x60;}}
void my_cprintf_status(byte b, byte t, byte m) { char buf[41]; sprintf(buf,"Boxes: %d/%d  Moves: %d",b,t,m); my_cputsxy(0,23,buf); }

void wait_vblank(void) { asm("lda $14"); __wait: asm("cmp $14"); asm("beq %g", __wait); }
//----------------------------------------------------------------------

void setup_graphics(void) {
    byte i;
    word screen_addr=(word)SCREEN_MEM; word dlist_addr=(word)DLIST_MEM; word charset_addr=(word)CHARSET_MEM;
    DLIST_MEM[0]=0x70; DLIST_MEM[1]=0x70; DLIST_MEM[2]=0x70;
    DLIST_MEM[3]=0x42; DLIST_MEM[4]=(byte)screen_addr; DLIST_MEM[5]=(byte)(screen_addr>>8);
    for (i=0; i<23; ++i) { DLIST_MEM[6+i] = 0x02; }
    DLIST_MEM[29]=0x41; DLIST_MEM[30]=(byte)dlist_addr; DLIST_MEM[31]=(byte)(dlist_addr>>8);
    POKE(756, (byte)(charset_addr>>8));
    memcpy(CHARSET_MEM, (void*)ROM_CHARSET_ADDRESS, 1024);
    memcpy(CHARSET_MEM+('#'*8), &custom_graphics[0],  8);
    memcpy(CHARSET_MEM+('$'*8), &custom_graphics[8],  8);
    memcpy(CHARSET_MEM+('.'*8), &custom_graphics[16], 8);
    memcpy(CHARSET_MEM+('*'*8), &custom_graphics[24], 8);
    // Initially set both player chars to the standing graphic
    memcpy(CHARSET_MEM+('@'*8), &custom_graphics[32], 8);
    memcpy(CHARSET_MEM+('+'*8), &custom_graphics[32], 8);
    POKE(709,0); POKE(710,14);
    POKEW(560, dlist_addr);
    POKE(752,0);
}

// --- **NEW**: Animation Function ---
void animate_player() {
    // Get pointers to BOTH player character locations in the character set
    byte* player_char_data = CHARSET_MEM + ('@' * 8);
    byte* player_goal_char_data = CHARSET_MEM + ('+' * 8);

    if (anim_state == 1) { // Walking animation
        // Alternate between standing and walking frames based on the timer
        if (frame_counter < 4) {
            memcpy(player_char_data, &custom_graphics[32], 8); // Standing
            memcpy(player_goal_char_data, &custom_graphics[32], 8);
        } else {
            memcpy(player_char_data, &custom_graphics[48], 8); // Walking
            memcpy(player_goal_char_data, &custom_graphics[48], 8);
        }
        if (frame_counter > 8) anim_state = 0; // Stop walking anim after 8 frames
    } else { // Idle animation (blinking)
        // Default to the standing frame
        memcpy(player_char_data, &custom_graphics[32], 8);
        memcpy(player_goal_char_data, &custom_graphics[32], 8);
        // But for a short time, show the blinking frame
        if (frame_counter > 50 && frame_counter < 53) {
            memcpy(player_char_data, &custom_graphics[40], 8); // Blinking
            memcpy(player_goal_char_data, &custom_graphics[40], 8);
        }
    }
}


void load_level(const char**d, byte r_num){byte r,c;char t;total_boxes=0;boxes_on_goals=0;moves=0;for(r=0;r<ROWS;++r)for(c=0;c<COLS;++c)level[r][c]=TILE_EMPTY;for(r=0;r<r_num;++r){c=0;while(d[r][c]){t=d[r][c];if(t==' ')level[r][c]=TILE_EMPTY;else if(t==TILE_PLAYER){player_x=c;player_y=r;level[r][c]=TILE_EMPTY;}else if(t==TILE_BOX)total_boxes++,level[r][c]=t;else level[r][c]=t;c++;}}}
void draw_level(){byte r,c;char t,put;my_clrscr();for(r=0;r<ROWS;++r)for(c=0;c<COLS;++c){t=level[r][c];if(t==TILE_BOX_ON_GOAL||t==TILE_PLAYER_ON_GOAL)my_cputcxy(c,r,TILE_GOAL);if(t!=TILE_EMPTY)my_cputcxy(c,r,t);}put=level[player_y][player_x];if(put==TILE_GOAL)my_cputcxy(player_x,player_y,TILE_PLAYER_ON_GOAL);else my_cputcxy(player_x,player_y,TILE_PLAYER);}
void update_status(){my_cputsxy(0,23,"                                        ");my_cprintf_status(boxes_on_goals,total_boxes,moves);}

void try_move_player(signed char dx, signed char dy) {
    byte tx,ty,bx,by; char tt,bt,put;
    tx=player_x+dx; ty=player_y+dy; tt=level[ty][tx];
    if(tt==TILE_WALL)return;
    if(tt==TILE_BOX||tt==TILE_BOX_ON_GOAL){
        bx=tx+dx; by=ty+dy; bt=level[by][bx];
        if(bt==TILE_WALL||bt==TILE_BOX||bt==TILE_BOX_ON_GOAL)return;
        if(tt==TILE_BOX_ON_GOAL){level[ty][tx]=TILE_GOAL;boxes_on_goals--;}
        else{level[ty][tx]=TILE_EMPTY;}
        my_cputcxy(tx,ty,level[ty][tx]);
        if(bt==TILE_GOAL){level[by][bx]=TILE_BOX_ON_GOAL;boxes_on_goals++;}
        else{level[by][bx]=TILE_BOX;}
        my_cputcxy(bx,by,level[by][bx]);
    }
    my_cputcxy(player_x,player_y,level[player_y][player_x]);
    player_x=tx; player_y=ty;
    put=level[player_y][player_x];
    if(put==TILE_GOAL)my_cputcxy(player_x,player_y,TILE_PLAYER_ON_GOAL);
    else my_cputcxy(player_x,player_y,TILE_PLAYER);
    moves++; update_status();
    
    // Trigger the walking animation
    anim_state = 1;
    frame_counter = 0;
}

void main() {
    byte joy, last_joy = 0;
    joy_install(joy_static_stddrv);
    setup_graphics();
    load_level(level_1, sizeof(level_1)/sizeof(level_1[0]));
    draw_level();
    update_status();
    while (1) {
        joy = joy_read(0);
        if (joy != last_joy) {
            if(JOY_UP(joy)) try_move_player(0,-1);
            if(JOY_DOWN(joy)) try_move_player(0,1);
            if(JOY_LEFT(joy)) try_move_player(-1,0);
            if(JOY_RIGHT(joy)) try_move_player(1,0);
        }
        last_joy = joy;
        if (boxes_on_goals == total_boxes) break;
        
        // **FIXES APPLIED HERE**
        // Animate the player every loop
        animate_player();
        // Wait for the screen to refresh, slowing the game to a playable speed
        wait_vblank();
        // Increment our timer only once per screen refresh
        frame_counter++;
    }
    my_cputsxy(15, 12, "YOU WIN!");
    cgetc();
    while(1);
}