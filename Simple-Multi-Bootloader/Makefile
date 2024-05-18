
all: kernel

run: install
	qemu-system-x86_64 -cdrom my_os.iso

install: kernel
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	
	grub-mkrescue -o my_os.iso iso

kernel: start.o link.ld main.o vga.o
	ld -m elf_i386 -T link.ld -o kernel.bin start.o main.o vga.o

%.o: %.c
	gcc -fno-pie -Wall -m32 -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I. -c -o $@ $<

start.o: start.asm
	nasm -f elf -o start.o start.asm

clean:
	rm -f *.o kernel.bin
