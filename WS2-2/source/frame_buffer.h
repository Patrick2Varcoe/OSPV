#ifndef INCLUDE_FRAME_BUFFER_H
#define INCLUDE_FRAME_BUFFER_H

#include "type.h"

/* 
   Global framebuffer state
   (actually defined in driver.c)
*/
extern char *fb;
extern int CURSORPOS;

/* VGA text-mode color definitions */
#define FB_BLACK         0x0
#define FB_BLUE          0x1
#define FB_GREEN         0x2
#define FB_CYAN          0x3
#define FB_RED           0x4
#define FB_MAGENTA       0x5
#define FB_BROWN         0x6
#define FB_LIGHT_GREY    0x7
#define FB_DARK_GREY     0x8
#define FB_LIGHT_BLUE    0x9
#define FB_LIGHT_GREEN   0xA
#define FB_LIGHT_CYAN    0xB
#define FB_LIGHT_RED     0xC
#define FB_LIGHT_MAGENTA 0xD
#define FB_LIGHT_BROWN   0xE
#define FB_WHITE         0xF

/* VGA hardware cursor register ports */
#define FB_COMMAND_PORT      0x3D4
#define FB_DATA_PORT         0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/* Framebuffer API (matching driver.c + loader.asm) */

/* Move cursor using linear position */
void fb_move_cursor(unsigned short pos);

/* Move cursor using x,y coordinates */
void fb_move_cursor2D(int x, int y);

/* Write ONE character cell (pos is byte offset: 0,2,4,...) */
int fb_write_cell(unsigned int pos, char c,
                  unsigned char fg, unsigned char bg);

/* Clear and initialize the framebuffer */
void init_fb(void);

/* Write a string starting at position, returns new pos */
int fb_write_string(unsigned int pos, const char *s,
                    unsigned char fg, unsigned char bg);

/* Write a number at position, returns new pos */
int fb_write_number(unsigned int pos, unsigned int num,
                    unsigned char fg, unsigned char bg);

/* Change colors — from older assignments, no-op in driver.c */
int change_fg_color(int x);
int change_bg_color(int x);

/* Additional functions used by interrupts.c */
void fb_write(const char *str);
void fb_clear();
void fb_newline(void);
void fb_backspace(void);

/* ===== Scrollback Support ===== */
extern int view_offset;
extern int sb_count;

/* Render a scrollback page based on view_offset */
void fb_render_scrollback_page(void);


#endif

