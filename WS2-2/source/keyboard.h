#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H
#include "driver.h"
#include "type.h"

#define KEYBOARD_MAX_ASCII 83

void kb_buffer_push(char c);
char getc(void);

char kb_buffer_pop();
void kb_buffer_push(char c);

u8int keyboard_read_scan_code(void);
char keyboard_scan_code_to_ascii(u8int sc);


#endif

