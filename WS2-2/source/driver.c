#include "frame_buffer.h"
#include "io.h"
#include "driver.h"


/* ===== Scrollback globals (actual definitions) ===== */
int view_offset = 0;     // 0 = live mode
int sb_count = 0;        // total number of stored lines
#define SCROLLBACK_LINES 500
static char scrollback[SCROLLBACK_LINES][81];
static int sb_head = 0;



char *fb = (char*)0x000B8000;
int CURSORPOS = 0;




/* -----------------------------------------------------------
   Internal helper: write a single cell
----------------------------------------------------------- */
int fb_write_cell(unsigned int pos, char c,
                  unsigned char fg, unsigned char bg)
{
    fb[pos]     = c;
    fb[pos + 1] = (bg << 4) | (fg & 0x0F);

    CURSORPOS = pos / 2;
    fb_move_cursor(CURSORPOS);
    return pos + 2;
}

/* -----------------------------------------------------------
   Move cursor
----------------------------------------------------------- */
void fb_move_cursor(unsigned short pos)
{
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void fb_move_cursor2D(int x, int y)
{
    fb_move_cursor(y * 80 + x);
}

/* -----------------------------------------------------------
   Clear the screen
----------------------------------------------------------- */
void fb_clear()
{
    for (int i = 0; i < 80 * 25; i++)
        fb_write_cell(i * 2, ' ', FB_LIGHT_GREY, FB_BLACK);

    CURSORPOS = 0;
    fb_move_cursor(0);
}

/* -----------------------------------------------------------
   Initialize framebuffer
----------------------------------------------------------- */
void init_fb()
{
    fb_clear();
}

/* -----------------------------------------------------------
   Write a whole string
----------------------------------------------------------- */
void fb_write(const char *str)
{
  // If user is in scrollback view, ignore live output
  if (view_offset > 0){
      return; 
  } 
  while (*str)
      {
          if (*str == '\n')
          {
              fb_newline();
          }
          else
          {
              fb_write_cell(CURSORPOS * 2, *str, FB_LIGHT_GREY, FB_BLACK);
              CURSORPOS++;
          }
          str++;
      }
}

/* -----------------------------------------------------------
   Newline
----------------------------------------------------------- */
void fb_newline(void)
{
  
        // Copy current line into scrollback buffer
    int line = CURSORPOS / 80;
    for (int i = 0; i < 80; i++)
    {
        scrollback[sb_head][i] = fb[(line * 160) + (i * 2)];
    }
    scrollback[sb_head][80] = 0;

    sb_head = (sb_head + 1) % SCROLLBACK_LINES;

    if (sb_count < SCROLLBACK_LINES)
        sb_count++;

  
    CURSORPOS = (CURSORPOS / 80 + 1) * 80;

    if (CURSORPOS >= 80 * 25)
    {
        for (int i = 0; i < (80 * 24) * 2; i++)
            fb[i] = fb[i + 160];

        for (int i = (80 * 24) * 2; i < (80 * 25) * 2; i += 2)
            fb_write_cell(i, ' ', FB_LIGHT_GREY, FB_BLACK);

        CURSORPOS = 80 * 24;
    }

    fb_move_cursor(CURSORPOS);
}


/* -----------------------------------------------------------
   Backspace
----------------------------------------------------------- */
void fb_backspace()
{
    if (CURSORPOS == 0)
        return;

    CURSORPOS--;
    fb_write_cell(CURSORPOS * 2, ' ', FB_LIGHT_GREY, FB_BLACK);
    fb_move_cursor(CURSORPOS);
}


void fb_render_scrollback_page()
{
    int start_line = sb_count - (25 + view_offset);
    if (start_line < 0) start_line = 0;

    // Clear screen
    for (int i = 0; i < 80 * 25; i++)
        fb_write_cell(i * 2, ' ', FB_LIGHT_GREY, FB_BLACK);

    // Draw up to 25 lines
    for (int row = 0; row < 25; row++)
    {
        int sb_index = start_line + row;
        if (sb_index < 0 || sb_index >= sb_count)
            continue;

        for (int col = 0; col < 80; col++)
        {
            char c = scrollback[(sb_index) % SCROLLBACK_LINES][col];
            if (c == 0) break;

            fb_write_cell((row * 80 + col) * 2,
                          c, FB_LIGHT_GREY, FB_BLACK);
        }
    }

    fb_move_cursor(80 * 24); // bottom of screen
}

