The main body of this worksheet is loader.asm, however it draws many functions from other files, primarily c functions.

## Included C functions

- increment.c
  A very basic c function which takes an integer as a parameter and returns the value of that integer + 2

- func2.c
  A function which takes an integer as a parameter and returns the value of that integer multiplied by itself (squared)

- func3.c
  A function which takes an integer as a parameter and returns the value of the integer divided by 2, however as to avoid any confusion with decimals if the integer provided is odd the returned result is that number -1 divided by two, this provides a truncated result.

- sumofthree.c
  A function which takes three integers as parameters and returns the summed result of all three integers.

## C Driver

This is the main C file containing the code used to interact directly with the framebuffer, cursor, and other ports. This is all done in conjunction with the io.s and io.h of which the io.s assembly code contains the actual function which interacts with the IO port.

- io.s 
  io.s contains a singular function, to write a byte to the IO port.

  mov al, [esp + 8]
  This moves the value to be written into the register al

  mov dx, [esp + 4]
  This moves the port number into the register dx

  out dx, al
  Sends the data stored in the al register to the port referenced by the dx register

  ret
  Returns to the point which the function was called from


- driver.c

- char *fb = (char *) 0x000B8000;
  0x000B8000 is the memory address for the VGA text buffer (color)

- #define FB_BLACK 0
  Defining names to relate to the values used to dictate both foreground and background colour. 
  #define FB_COMMAND_PORT 0x3D4
  #define FB_HIGH_BYTE_COMMAND 14
  Defining details used for interacting with the ports, by doing this it makes the code more readable and in the case of adapting the code for another system makes it more easily adaptable

- fb_move_cursor


- fb_move_cursor2D
  Takes two integer parameters, representing horizontal and vertical positions, and moves the cursor using the previous function. In essence this function translates a 2 dimensional position into a position readable by fb_move_cursor.

- fb_write_cell
  This function is used to write a character to a text cell in the framebuffer, this is a one to one interaction and will have to be preformed for each character in a string.
  
  fb[i] = c;
  Puts the character to be written in the framebuffer
  
  fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
  Builds the byte used to dictate the colours used in both the background and foreground for the character written

  CURSORPOS = i;
  fb_move_cursor(CURSORPOS);
  Moves the cursor to follow the character being written
  
  i = i + 2;
  Increments the cursors position 
  
  return i;
  Returns the cursor position as an integer

- init_fb()
  This is just an example function to demonstrate the write cell function

- fb_write_string
  This function utilizes the write cell function to be able to take string arguments and write them to the framebuffer. 
  while (*s) {
        fb_write_cell(pos, *s, fg, bg);
        s++;
        pos += 2;}

  This loop points to every character in the string given as a parameter to the function, and for each character uses the write cell function to write the character to the frame buffer
  
  The cursor is then moved to accurately follow the writing of the string.

- fb_write_number
  This function is used to take an integer and convert it into an ASCII format to be able to write it to the framebuffer using the previously implemented write cell function.

  buffer[i++] = '0' + (num % 10);
  num /= 10;
  By taking the modulus of the number by 10, we are left with only the 'digits' part of the number, the right most character, and add this to the buffer storing our ascii number. Then by dividing our number by 10 we are able to move onto the next digit, this is due to the truncating effect of dividing with integers as they are unable to have decimal parts.
  By using this within a loop the number can be separated into its individual digits, although they are in the reverse order.

  Then by looping through the buffer backwards and using write cell to write the current character to the frame buffer we are able to output the whole number in an ASCII format.
  
That is an overview of the main functionality found within driver.c, this will be essential to understanding the loader.asm file as it relies heavily on the functions contained here.

## Loader.asm

- Multiboot header
  MAGIC_NUMBER      equ 0x1BADB002
  FLAGS             equ 0
  CHECKSUM          equ -(MAGIC_NUMBER + FLAGS)
  This section is used to define values required by the bootloader we are using (GRUB). We also define in this section the size we will be using for the stack.

- External Functions
  extern sumofthree
  Also declared are the names of the external functions used in this file, these are the C functions we will be using within this program.

 - mov esp, kernel_stack + KERNEL_STACK_SIZE
   Moves the pointer for the top of the stack to the register esp
   
- Calling a C function (sum of three)
  push dword 3
  push dword 2
  push dword 1
  Pushing the arguments used in the function, they are pushed in reverse order to the C function although in this use case its not important it will be later on.
  call sumofthree
  This actually calls the C function, using the arguments pushed
  add esp, 12
  As each argument is using 4 bytes we move the pointer back to the top of the stack by adding 12 (3x4)
  The result of this function is now held in the EAX register

  As all C functions are called in a similar way I wont go over the details of it again unless there is something important to highlight as I've gone over their internal workings already
  
- The rest of this program focuses on displaying implementation of the various C functions, the only notable difference in the following functions is when we collect the return value such as in this section
  push dword [FG_COL]
  push dword [BG_COL]
  push dword 1234
  push dword [WRITEPOS]
  call fb_write_number
  mov [WRITEPOS], EAX
  add esp, 16
  where after calling the C function we move the value from EAX to the memory referenced by WRITEPOS, this is to record the new position in the framebuffer we are writing to.
  Additionally in this section we can see the importance of pushing arguments in the right order, as whilst the related C function requires the arguments in order of 
  pos num bg fg
  to achieve this we need to push the arguments in the reverse order as seen
  fg bg num pos
  
  

Overall this program demonstrates interacting directly with the framebuffer and implementing external C functions into assembly programs.