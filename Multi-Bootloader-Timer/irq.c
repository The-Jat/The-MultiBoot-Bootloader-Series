#include <system.h> // Include system header for required definitions and functions

// Declare external assembly ISR functions
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

// Array to store custom IRQ handler functions
void *irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

// Function to install a custom IRQ handler
void irq_install_handler(int irq, void *handler) {
	irq_routines[irq] = handler; // Store the handler function in the array
}

// Function to uninstall a custom IRQ handler
void irq_uninstall_handler(int irq) {
	irq_routines[irq] = 0; // Remove the handler function from the array
}

// Function to remap the PIC
void irq_remap() {
	outportb(0x20, 0x11); // Initialize master PIC
	outportb(0xA0, 0x11); // Initialize slave PIC
	outportb(0x21, 0x20); // Remap master PIC IRQs to 0x20-0x27
	outportb(0xA1, 0x28); // Remap slave PIC IRQs to 0x28-0x2F
	outportb(0x21, 0x04); // Tell master PIC about slave PIC at IRQ2
	outportb(0xA1, 0x02); // Tell slave PIC its cascade identity
	outportb(0x21, 0x01); // Set master PIC to 8086 mode
	outportb(0xA1, 0x01); // Set slave PIC to 8086 mode
	outportb(0x21, 0x0);  // Unmask all IRQs on master PIC
	outportb(0xA1, 0x0);  // Unmask all IRQs on slave PIC
}

// Function to set up the IDT entries for IRQs
void irq_install() {
	irq_remap(); // Remap the PIC

	// Set the IDT entries for each IRQ
	idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

// The main IRQ handler function
void irq_handler(struct regs *r) {
	void (*handler)(struct regs *r); // Define a handler function pointer

	// Get the custom handler for this IRQ from the array
	handler = irq_routines[r->int_no - 32];

	// If a custom handler is installed, call it
	if (handler) {
		handler(r);
	}

	// If the interrupt came from the slave PIC (IRQ8-IRQ15)
	if (r->int_no >= 40) {
		outportb(0xA0, 0x20); // Send End of Interrupt (EOI) signal to the slave PIC
	}

	// Send EOI signal to the master PIC
	outportb(0x20, 0x20);
}
