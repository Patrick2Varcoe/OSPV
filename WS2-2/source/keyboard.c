#include "io.h"
#include "frame_buffer.h"
#include "keyboard.h"

#define KB_BUFFER_SIZE 128

static volatile char kb_buffer[KB_BUFFER_SIZE];
static volatile int kb_head = 0;   // write index
static volatile int kb_tail = 0;   // read index

/* Push one character into the circular buffer */
void kb_buffer_push(char c)
{
    int next = (kb_head + 1) % KB_BUFFER_SIZE;

    if (next != kb_tail) {
        kb_buffer[kb_head] = c;
        kb_head = next;
    }
}

/* Pop one character from the buffer (called by terminal input) */
char kb_buffer_pop()
{
    if (kb_tail == kb_head)
        return 0; // buffer empty

    char c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return c;
}

/* Read raw scancode from keyboard controller */
u8int keyboard_read_scan_code()
{
    return inb(0x60);
}

/* Convert scancode to ASCII (simplified US layout) */
char keyboard_scan_code_to_ascii(u8int sc)
{
    switch (sc)
    {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';

        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';

        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';

        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';

        case 0x1C: return '\n';   // Enter
        case 0x39: return ' ';    // Space
        case 0x0E: return '\b';   // Backspace

        case 0x4A: return '-';
        case 0x4E: return '+';
        case 0x53: return '.';

        default:
            return 0;  // unknown or key release
    }
}

