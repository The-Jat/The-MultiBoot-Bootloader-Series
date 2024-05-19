.PHONY: all clean run install

# Compiler and assembler flags
CC = gcc
CFLAGS = -Wall -m32 -fno-pie -O0 -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include
AS = nasm
ASFLAGS = -f elf

# Output files and directories
ISO_DIR = iso
BOOT_DIR = $(ISO_DIR)/boot
GRUB_DIR = $(BOOT_DIR)/grub
ISO = my_os.iso

# Source files and objects
C_SOURCES = main.c vga.c gdt.c idt.c isr.c
ASM_SOURCES = start.asm gdt_asm.asm idt_asm.asm isr_asm.asm
OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)

# Linker script
LINKER_SCRIPT = link.ld

# Kernel binary
KERNEL = kernel.bin

all: $(KERNEL)

# Build the ISO and run it with QEMU
run: install
	qemu-system-x86_64 -cdrom $(ISO)

# Install the kernel and GRUB configuration into the ISO directory and create the ISO image
install: $(KERNEL)
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL) $(BOOT_DIR)/kernel.bin
	cp grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Link the kernel binary
$(KERNEL): $(OBJ) $(LINKER_SCRIPT)
	ld -m elf_i386 -T $(LINKER_SCRIPT) -o $(KERNEL) $(OBJ)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Assemble assembly source files
%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(KERNEL) $(ISO)
	rm -rf $(ISO_DIR)

# Dependencies
main.o: main.c
vga.o: vga.c
gdt.o: gdt.c
idt.o: idt.c
isr.o: isr.c
start.o: start.asm
gdt_asm.o: gdt_asm.asm
idt_asm.o: idt_asm.asm
isr_asm.o: isr_asm.asm
