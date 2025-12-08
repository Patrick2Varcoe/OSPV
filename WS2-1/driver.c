char *fb = (char *) 0x000B8000;  //vga text buffer address

#define FB_BLACK 0
#define FB_BLUE 1
#define FB_GREEN 2
#define FB_CYAN 3
#define FB_RED 4
#define FB_MAGENTA 5
#define FB_BROWN 6
#define FB_LIGHT_GREY 7
#define FB_DARK_GREY 8
#define FB_LIGHT_BLUE 9
#define FB_LIGHT_GREEN 10
#define FB_LIGHT_CYAN 11
#define FB_LIGHT_RED 12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN 14
#define FB_WHITE 15

#include "io.h"
/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
/** fb_move_cursor:
* Moves the cursor of the framebuffer to the given position

*
* @param pos The new position of the cursor
*/
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

void fb_move_cursor2D(int x, int y)
{
  int newpos;
  y = y * 80;
  newpos = x + y;
  fb_move_cursor(newpos);
}

int CURSORPOS = 0;

/**
 * Writes a character with the given foreground and background to position i
 * in the framebuffer.
 *
 * @param i The location in the framebuffer (cell index * 2)
 * @param c The character
 * @param fg The foreground color
 * @param bg The background color
 */
int fb_write_cell(unsigned int i, char c, unsigned char bg, unsigned char fg)
{
    //i = i *2;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
    CURSORPOS = i;
    fb_move_cursor(CURSORPOS);
    i = i + 2;
    return i;
    
}

void init_fb()
{
    // Example write
    fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);  // green A on dark grey
}

int fb_write_string(unsigned int pos, const char *s, unsigned char bg, unsigned char fg)
{
  //pos = pos *2;
  while (*s) {
        fb_write_cell(pos, *s, fg, bg);
        s++;
        pos += 2;

    }
    CURSORPOS = pos/2;
    fb_move_cursor(CURSORPOS);
    return pos;
}

int fb_write_number(unsigned int pos, unsigned int num,
                     unsigned char bg, unsigned char fg)
{
    char buffer[12];   
    int i = 0;
    //pos = pos *2;

    // Convert number to ASCII (reverse order)
    if (num == 0) {
        buffer[i++] = '0';
    } else {
        while (num > 0) {
            buffer[i++] = '0' + (num % 10);
            num /= 10;
        }
    }
    
    // Now print in correct order
    while (i > 0) {
        char c = buffer[--i];
        fb_write_cell(pos, c, fg, bg);
        pos += 2;  // next framebuffer cell
    }
    CURSORPOS = pos/2;
    fb_move_cursor(CURSORPOS);
    return pos;
}

int change_fg_color(int x)
{

  return(x);

}

int change_bg_color(int x)
{

  return(x);

}


int write(char *buf, unsigned int len);
