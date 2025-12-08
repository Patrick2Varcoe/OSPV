
# C Files
There are once again many C files used within this program, as there are many different C functions I will try to only highlight the significant parts as well as any interactions I haven't already covered

---

## driver.c


  - Scrollback:
	As we are planning on implementing a scroll back feature we set up a few variables here, I will touch on them more later when covering the implementation

- fb_clear
  This function is used to clear the terminal, to do this the function loops over every cell and writes an empty character, so in essence fills the screen with nothing.

- fb_write
  This function serves as an upgrade to fb_write_string, now implementing the additional functionality of new lines as well as including the important rule of the ability to be disabled when using the scroll back functionality. Otherwise it maintains the same core functionality as before.

- fb_newline
  This new function implements the ability to write a newline to the framebuffer.
  After dealing with saving the current line contents to the scroll back buffer, this function finds which line the cursor is on and advances it to the start of the next line, CURSORPOS = (CURSORPOS / 80 + 1) * 80;
  fb_move_cursor(CURSORPOS);
  It also has some additional checks for the case in which the screen is currently full in which to scroll the screen by moving all the current characters in the framebuffer up by one row and clearing the bottom row similarly to the fb_clear function.


- fb_backspace
  This function implements the ability for the user to enter a backspace and delete the previous character and move the cursor to reflect this change.

- fb_render_scrollback_page
  This function is used to allow the user to scroll back to previous pages in the terminal, to do so it first clears the screen similar to the previous clear function, then it writes lines from the scrollback buffer, this is where some of those variables declared at the start of this file are used.

---

## keyboard.c

- kb_buffer_push
  Function to add a character to the keyboard buffer.

- kb_buffer_pop
  Function to return the tail of the keyboard buffer, advances the tail of the keyboard buffer.

- keyboard_read_scan_code
  A function to read a single characters scancode from the keyboard controller port

- keyboard_scan_code_to_ascii
  A simple switch statement to convert the raw scancode to a standardized ASCII format and return it.

---

## interrupts.c

- interrupt_handler
  Upon recieving an interrupt, checks the interrupt value and if its 33 (the case for keyboard interrupts) proceeds to handle the keyboard input, and if not proceeds.
  Then the function reads the incoming data with the previously explained functions in keyboard.c and checks to see if the incoming character is a special case (PAGEUP or PAGEDOWN) and if not proceeds to converting it to ASCII and pushing it to the keyboard buffer.
  In the case of PAGEUP or PAGEDOWN the function refers to the scrollback functionality.
  No matter the outcome of the keyboard input we acknowledge that it has been handled and return to the code section we came from.

---

## pic.c

- pic_acknowledge
  Checks the sender of the interrupt and sends data signalling that the interrupt has been handled to the sender of the interrupt.

## terminal.c

- cmd_echo
  Takes a pointer to the first character in a string and writes back all the characters in the string and a new line .
- cmd_clear
  Simply runs the fb_clear command
- cmd_version
  Writes the version of the OS to the terminal
- cmd_help
  Writes the available commands within the OS and a short description of their use.

- readline
  Continually reads characters out of the keyboard buffer, and adds them to its buffer, upon reading a newline character returns out of its loop back to the main terminal loop with the full buffer as its return value. Takes the users input and returns it to the main body of the program for use but only when a new line is entered.

- execute_command
  Splits input into command and arguments, this is done by putting any characters before a " " character as the command and any after as the arguments.
  Then the interpreted command is matched against the array of commands to be able to convert it into the proper function name, which is then run with the arguments passed as parameters.
  The function then returns to the main body loop to proceed.

- terminal_run
  - Main loop: prints prompt myos> , reads a line, executes command, repeats forever. This allows for the terminal to continually run whilst executing its main steps in order.

# Assembly Files

## io.s

- inb
  Reads and returns a byte from the IO port given

## interrupt_handlers.s


- **load_idt**
  - Stack: `[esp + 4]` = address of first IDT entry (actually an IDT descriptor containing pointer & size).
  - Calls `lidt [eax]` where `eax` holds the pointer passed in.
  - `ret`
---

## loader.asm

Used to initially set up the kernel and stack as well as to initialise the frame buffer, interrupts and to start the terminal.


![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS2-2/TerminalStart.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS2-2/TerminalCommands1.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS2-2/TerminalNoCommand.PNG)