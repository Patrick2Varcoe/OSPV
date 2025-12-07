# README — OS WS1 Patrick Varcoe

### **Driver.c**

This C driver is very simple however it allows us to make these programs without worrying about setting up a stack and creating a entry point for our programs.

## 1. `assemblyio.asm` — Low-Level Integer Addition

This program takes two integer values and prints their sum. To do this it interacts directly with memory registers.

- %include "src/asm_io.inc"
  Used to include the functions inside the file asm_io, this will be used to preform input and output functions within the programs.

- integer1        dd      15
  Defines 'integer1' as a reference to a memory address, containing a data of type double word (4 bytes) and assigns it the value 15

- `mov eax, [integer1]`  
  Loads the value from integer1 into the eax register.

- `add eax, [integer2]`  
  Preforms an arithmetic addition of the value of integer 2 and the value held in eax

- `mov [result], eax`  
  Stores the result of the addition in result

- `call print_int`  
  Uses the function included from asm_io to print the value to the user, this value actually comes from eax and not the value in result

![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/1-1-Terminal.PNG)

## 2. `task1.2.asm` — Reading Two Integers and Summing

In this program we start taking user input and then implement the same ideas as the first task with simple addition and output, this lets us take two integer values from the user and produce the sum.

### **Key Low-Level Interactions**


- msg1 db "Enter a number: ", 0 
  Declaring memory references and assigning values, in this case the data is actually split up and the reference is to the first memory address containing this data.

- Printing a string
  `mov eax, msg1` 
  loads the address of a string (the address of the first character in the string) into `EAX`.  
  `call print_string` treats EAX as a pointer and prints each character, taking the data from each separate address.

- User input 
  call read_int 
  Calls the function included from asm_io, to allow the user to enter an integer
  mov [integer2], eax 
  Stores the value inputted by the user using the address referenced by integer2

- Integer addition
  mov eax, [integer1]
  add eax, [integer2]
  mov [result], eax
  This is very similar to the first task, and just preforms simple addition and stores the result in memory.

- **Output construction**  
  mov eax, msg2
  call print_string
  mov eax, [integer1]
  call print_int
  The program then finishes by writing multiple outputs to the same line, constructing a sensible output from multiple memory references


![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/1-2-Terminal.PNG)

## 3. `task2.asm` — Welcome loop

In this program we take multiple user inputs and use these to preform a simple loop as well as exploring simple comparisons.
The user is asked to input a string and an integer, the integer is then compared to check whether it is within the bounds of 50 and 100, if this is correct the program then prints a welcome message including the users inputted string a number of times equal to the integer from the user.



- segment .data   -   segment .bss
  These sections continues to setup memory references and assign values to memory, as the actions preformed in this section are generally similar between tasks I wont go over anything here in detail unless there is a new process happening.


- Setup for registers
  mov edi, name_buffer
  mov ecx, 99
  Here we are setting up some registers for later use, specifically we are setting the value of edi to the first memory address reserved by name_buffer and setting the ecx to 99, this is used for the internal counter with loops, when loop read_name_loop is called the this register is used, with its value being decremented and if its value is 0 when called the loop instruction does not jump to its target location. This does show a limitation of the program not allowing for strings to be entered longer than a certain threshold, in this case 99 characters however for this program this should be more than enough.

- Reading user inputted string - read_name_loop
  call read_char
  Using the function provided by asm_io to read the current character

  cmp al, 10
  je end_read_name
  Compare the character read to a newline value (10), indicating whether the string ends here or not, if the comparison returns that the character is a newline the program jumps to the section end_read_name, ending the current loop
  
  mov [edi], al
  inc edi
  stores the character read from the user in memory and increments the pointer so the next character is stored in the next memory address

  loop read_name_loop
  Loops back to the start of read_name_loop to repeat the previous steps to read and store each character of the string entered by the user until a newline character is read.


- end_read_name

  mov byte [edi], 0 
  Stores a value of 0 at the last memory address representing the string, 0 in this case represents the termination of the string

  mov eax, prompt_count
  call print_string
  call read_int
  mov [count], eax
  Prints the prompt for the user to enter their integer, reads the value entered and stores it in the memory referenced by count

  cmp eax, 50
  jle invalid
  cmp eax, 100
  jge invalid
  Compares the value held in eax with the value 50 and in the case it is equal or less jumps to the section labelled invalid, then similarly compares the value in eax with 100 and if greater than or equal jumps to invalid.

  mov ecx, [count]
  In the case where neither of the previous conditions were met, the program continues and stores the inputted value in the ecx register, as explained earlier this will be used to loop the amount of times equal to the value entered by the user.


- print_loop

  mov eax, welcome_prefix
  call print_string
  mov eax, name_buffer
  call print_string
  call print_nl
  Here the program is printing the strings stored in memory similar to how this is done in previous tasks, the new part here is the call to print_nl which is another function from asm_io, which simply prints a new line.

  loop print_loop
  jmp done
  Loops by jumping back to the start of print_loop, repeating the previous section, until the value in the register ecx is 0, in which case the program continues and jumps to the section labelled done

- invalid
  mov eax, error_msg
  call print_string
  call print_nl
  Some simple printing commands to display an error message, only called if the user enters an integer outside of the allowed range

- done
  jumped to when the program completes without expected errors, signals the end of the program
  
---
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/2-1-Terminal.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/2-1-TerminalEdgeCases.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/2-1-TerminalFailureCase.PNG)



## 4. `task2.2.asm` — Summing a 100-Element Array

In this program we setup an array and loop through it preforming an addition each time.

- Setup
  array dd 1,2,3,4 .......
  We set up an array containing 100 elements, this is done similarly to how strings are stored with the array pointing to the first memory address containing the first element of the array.

  mov esi, array
  mov ecx, 100
  xor eax, eax
  Setting esi to point to the first element of the array, setting ecx to 100 so the loop will preform 100 times and setting eax to 0 initially.


- sum_loop

  add eax, [esi]
  add esi, 4
  loop sum_loop
  Taking the value of the element pointed to by esi (initially the first element of the array) and preforming an addition to the value in eax and storing it in eax. 
  Then we add 4 to esi to point to the next element of the array.
  Then we loop back to the start of this section, this is preformed until ecx is 0 as covered in the previous task, this means it will loop 100 times.

![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/2-2-Terminal.PNG)
## 5. `task2.3.asm` — Summing a given range
In this task we sum a range given by the user between 1 and 100.

This program essentially combines elements from the previous programs and combines them together, user input and output, basic arithmetic, loops, comparisons and error handling.

The only new implementation seen in this program is having a variable loop length
    mov eax, [end_range]
    sub eax, [start_range]
    inc eax
    mov ecx, eax
This code finds the length of the range being summed and places that value in the ecx register which is used when handling loops to determine their length.

![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/2-3-Terminal.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/2-3-TerminalErrors.PNG)

## 6. Makefile and build commands

The makefile uses some simple rules to build the object files and executables seen here.

- all: task1.1 task1.2 task2.1 task2.2 task2.3
  This line dictates that when the command make all is run, the makefile will indicate that all the sub processes, task1.1, task1.2 .... should be run.

- task1.1: src/assemblyio.asm src/asm_io.asm src/asm_io.inc src/driver.c
  This shows that when task1.1 is run such as in make all, the following files will be needed e.g src/driver.c

  nasm -f elf src/asm_io.asm -o src/asm_io.o
  This is a command used to build an object file when using an assembly file (nasm syntax), the output format is of an elf file with name asm_io.o in the src directory.
  The file being used to build this object file is asm_io.asm in src directory.

  gcc -m32 -c src/driver.c -o src/driver.o
  This is very similar to the above command in terms of source file and output file, however in this instance we are compiling with gcc, a C compiler, and dictating that the output file should be of a 32 bit format.
  
  gcc -m32 src/driver.o src/assemblyio.o src/asm_io.o -o src/task1.1.exe
  This is the last line we will look at, this line builds an executable, instead of an object file, using multiple sources. Once again we are using gcc here and a 32 bit format.

Overall this make file is used to efficiently build all the object files and subsequently executables out of the source files detailed above.

![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/MF-Terminal1.1.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/MF-TerminalAll1.PNG)
![alt text](https://github.com/Patrick2Varcoe/OSPV/blob/master/WS1/MF-TerminalAll2.PNG)
