#include <system.h>

/*
 * Global Descriptor Table (GDT) Entry
 * Each entry in the GDT is 8 bytes long.
 */
struct gdt_entry {
    unsigned short limit_low;  // The lower 16 bits of the limit.
    unsigned short base_low;   // The lower 16 bits of the base address.
    unsigned char base_middle; // The next 8 bits of the base address.
    unsigned char access;      // Access flags, determine what ring this segment can be used in.
    unsigned char granularity; // Granularity, and upper 4 bits of the limit.
    unsigned char base_high;   // The last 8 bits of the base address.
} __attribute__((packed));

/*
 * GDT pointer structure
 * Points to the start of our array of GDT entries
 * and the size of the array (limit).
 */
struct gdt_ptr {
    unsigned short limit; // The upper 16 bits of all selector limits.
    unsigned int base;    // The address of the first gdt_entry struct.
} __attribute__((packed));

struct gdt_entry gdt[3]; // Define 3 entries in our GDT.
struct gdt_ptr gp;       // Define the pointer to the GDT.

/*
 * (ASM) gdt_flush
 * This will be defined in assembly, it reloads the segment registers.
 */
extern void gdt_flush();

/*
 * gdt_set_gate
 * Set a GDT descriptor.
 * @param num: The index of the GDT entry to set.
 * @param base: The base address of the segment.
 * @param limit: The limit of the segment.
 * @param access: The access flags for the segment.
 * @param gran: The granularity flags for the segment.
 */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    /* Base Address */
    gdt[num].base_low = (base & 0xFFFF);           // Set the lower 16 bits of the base.
    gdt[num].base_middle = (base >> 16) & 0xFF;    // Set the next 8 bits of the base.
    gdt[num].base_high = (base >> 24) & 0xFF;      // Set the high 8 bits of the base.
    /* Limits */
    gdt[num].limit_low = (limit & 0xFFFF);         // Set the lower 16 bits of the limit.
    gdt[num].granularity = (limit >> 16) & 0x0F;   // Set the high 4 bits of the limit in granularity.
    /* Granularity */
    gdt[num].granularity |= (gran & 0xF0);         // Combine the granularity flags.
    /* Access flags */
    gdt[num].access = access;                      // Set the access flags.
}

/*
 * gdt_install
 * Install the kernel's GDT.
 */
void gdt_install() {
    /* GDT pointer and limits */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1; // Calculate the total size of the GDT.
    // Set the base address of the GDT
    // Without cast: some compilers may issue a warning or error.
    // gp.base = &gdt;
    // With explicit cast: ensures the address is treated as an integer.
    gp.base = (unsigned int)&gdt;                                // Set the base address of the GDT.

    /* NULL segment: required by x86 architecture */
    gdt_set_gate(0, 0, 0, 0, 0);                   // Set the first GDT entry (null descriptor).

    /* Code segment: base=0, limit=4GB, accessed from ring 0 */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);    // Set the code segment descriptor.

    /* Data segment: base=0, limit=4GB, accessed from ring 0 */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);    // Set the data segment descriptor.

    /* Load the new GDT using the gdt_flush function */
    gdt_flush();
}
