#include <system.h>

/*
 * memcpy
 * Copy from source to destination. Assumes that
 * source and destination are not overlapping.
 */
unsigned char *
memcpy(
		unsigned char *dest,
		const unsigned char *src,
		int count
	  ) {
	int i;
	i = 0;
	for ( ; i < count; ++i ) {
		dest[i] = src[i];
		
	}
	return dest;
}

/*
 * memset
 * Set `count` bytes to `val`.
 */
unsigned char *
memset(
		unsigned char *dest,
		unsigned char val,
		int count
	  ) {
	int i;
	i = 0;
	for ( ; i < count; ++i ) {
		dest[i] = val;
	}
	return dest;
}

/*
 * memsetw
 * Set `count` shorts to `val`.
 */
unsigned short *
memsetw(
		unsigned short *dest,
		unsigned short val,
		int count
	  ) {
	int i;
	i = 0;
	for ( ; i < count; ++i ) {
		dest[i] = val;
	}
	return dest;
}

/*
 * strlen
 * Returns the length of a given `str`.
 */
int
strlen(
		const char *str
	  ) {
	int i = 0;
	while (str[i] != (char)0) {
		++i;
	}
	return i;
}

/*
 * inportb
 * Read from an I/O port.
 */
unsigned char
inportb(
		unsigned short _port
	   ) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

/*
 * outportb
 * Write to an I/O port.
 */
void
outportb(
		unsigned short _port,
		unsigned char _data
		) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/*
 * Kernel Entry Point
 */
int
main() {
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();

	// Install the timer handler
	timer_install();
    	// Set the timer phase to the desired frequency (e.g., 18.222 Hz)
	timer_phase(18);    
	// Enable interrupts
	__asm__ __volatile__("sti");
	// Test the timer by waiting for a few ticks
	timer_wait(100);

	init_video();
	puts("TheJat!\n");
	for (;;);
	return 0;
}
