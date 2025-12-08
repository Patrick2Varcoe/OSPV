all: task1.1 task1.2 task2.1 task2.2 task2.3

task1.1: src/assemblyio.asm src/asm_io.asm src/asm_io.inc src/driver.c
		nasm -f elf src/asm_io.asm -o src/asm_io.o
		gcc -m32 -c src/driver.c -o src/driver.o
		nasm -f elf src/assemblyio.asm -o src/assemblyio.o
		gcc -m32 src/driver.o src/assemblyio.o src/asm_io.o -o src/task1.1.exe
task1.2: src/task1.2.asm src/asm_io.asm src/asm_io.inc src/driver.c
		nasm -f elf src/task1.2.asm -o src/task1.2.o
		nasm -f elf src/asm_io.asm -o src/asm_io.o
		gcc -m32 -c src/driver.c -o src/driver.o
		gcc -m32 src/driver.o src/task1.2.o src/asm_io.o -o src/task1.2.exe
task2.1: src/task2.asm src/asm_io.asm src/asm_io.inc src/driver.c
		nasm -f elf src/asm_io.asm -o src/asm_io.o
		gcc -m32 -c src/driver.c -o src/driver.o
		nasm -f elf src/task2.asm -o src/task2.1.o
		gcc -m32 src/driver.o src/task2.1.o src/asm_io.o -o src/task2.1.exe
task2.2: src/task2.2.asm src/asm_io.asm src/asm_io.inc src/driver.c
		nasm -f elf src/asm_io.asm -o src/asm_io.o
		gcc -m32 -c src/driver.c -o src/driver.o
		nasm -f elf src/task2.2.asm -o src/task2.2.o
		gcc -m32 src/driver.o src/task2.2.o src/asm_io.o -o src/task2.2.exe
task2.3: src/task2.3.asm src/asm_io.asm src/asm_io.inc src/driver.c
		nasm -f elf src/asm_io.asm -o src/asm_io.o
		gcc -m32 -c src/driver.c -o src/driver.o
		nasm -f elf src/task2.3.asm -o src/task2.3.o
		gcc -m32 src/driver.o src/task2.3.o src/asm_io.o -o src/task2.3.exe

