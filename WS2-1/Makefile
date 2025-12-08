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
sumofthree.o: sumofthree.c
	gcc $(CFLAGS) -c sumofthree.c -o sumofthree.o

driver.o: driver.c io.h
	gcc $(CFLAGS) -c driver.c -o driver.o
	
funcs: func2.c func3.c increment.c
	gcc $(CFLAGS) -c func2.c -o func2.o
	gcc $(CFLAGS) -c func3.c -o func3.o
	gcc $(CFLAGS) -c increment.c -o increment.o	
	
cursor: io.s 
	nasm -f elf io.s -o ios.o
	
# --------------------------
# Build the assembler part
# --------------------------
loader.o: loader.asm
	nasm -f elf loader.asm -o loader.o

# --------------------------
# Link kernel
# --------------------------
kernel: loader.o sumofthree.o driver.o funcs cursor source/link.ld
	ld -T source/link.ld -melf_i386 loader.o driver.o ios.o increment.o func2.o func3.o sumofthree.o -o kernel.elf
	mkdir -p $(BOOTDIR)
	cp kernel.elf $(BOOTDIR)

# --------------------------
# Build ISO using GRUB Legacy stage2_eltorito
# --------------------------
os: kernel
	mkdir -p $(GRUBDIR)
	cp source/grub.cfg $(GRUBDIR)



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
	qemu-system-i386 -display curses -monitor telnet::42986,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

clean:
	rm -f *.o *.elf os.iso
	rm -rf iso

