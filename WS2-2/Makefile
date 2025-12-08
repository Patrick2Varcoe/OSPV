ISO_DIR = iso
BOOTDIR = iso/boot
GRUBDIR = iso/boot/grub

CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc \
         -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra

all: kernel os

# --------------------------
# Build the C object
# --------------------------
sumofthree.o: source/sumofthree.c
	gcc $(CFLAGS) -c source/sumofthree.c -o sumofthree.o

driver.o: source/driver.c source/io.h
	gcc $(CFLAGS) -c source/driver.c -o driver.o
	
funcs: source/func2.c source/func3.c source/increment.c
	gcc $(CFLAGS) -c source/func2.c -o func2.o
	gcc $(CFLAGS) -c source/func3.c -o func3.o
	gcc $(CFLAGS) -c source/increment.c -o increment.o
	
cursor: source/io.s 
	nasm -f elf source/io.s -o ios.o
# --------------------------
# Build the assembler part
# --------------------------
loader.o: source/loader.asm
	nasm -f elf source/loader.asm -o loader.o

interrupts: source/interrupts.c source/interrupts.h source/keyboard.c source/keyboard.h source/pic.c source/interrupt_asm.s source/interrupt_handlers.s source/hardware_interrupt_enabler.h source/hardware_interrupt_enabler.s
	gcc $(CFLAGS) -c source/interrupts.c -o interrupts.o
	gcc $(CFLAGS) -c source/keyboard.c -o keyboard.o
	gcc $(CFLAGS) -c source/pic.c -o pic.o
	nasm -f elf source/interrupt_asm.s -o interrupt_asm.o
	nasm -f elf source/interrupt_handlers.s -o interrupt_handlers.o
	nasm -f elf source/hardware_interrupt_enabler.s -o hardware_interrupt_enabler.o
	

terminal:  source/terminal.c source/terminal.h
	gcc $(CFLAGS) -c source/terminal.c -o terminal.o

# --------------------------
# Link kernel
# --------------------------
kernel: loader.o sumofthree.o driver.o funcs cursor terminal interrupts source/link.ld
	ld -T source/link.ld -melf_i386 loader.o driver.o terminal.o ios.o increment.o func2.o func3.o sumofthree.o interrupts.o keyboard.o pic.o interrupt_asm.o interrupt_handlers.o hardware_interrupt_enabler.o -o kernel.elf
	mkdir -p $(BOOTDIR)
	cp kernel.elf $(BOOTDIR)

# --------------------------
# Build ISO using GRUB Legacy stage2_eltorito
# --------------------------
os: kernel
	mkdir -p $(GRUBDIR)
	cp source/menu.lst iso/boot/grub
	cp source/stage2_eltorito iso/boot/grub




	genisoimage -R \
	 -b boot/grub/stage2_eltorito \
	 -no-emul-boot \
	 -boot-load-size 4 \
	 -boot-info-table \
	 -quiet \
	 -o os.iso $(ISO_DIR)

# --------------------------
# Run in QEMU
# --------------------------
run: os.iso
	qemu-system-i386 -display curses -monitor telnet::45454,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

clean:
	rm -f *.o *.elf os.iso
	rm -rf iso


