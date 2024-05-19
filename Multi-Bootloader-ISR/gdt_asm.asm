; Declare that the gdt_flush function is global, so it can be used outside this file.
global gdt_flush

; Declare that the gp (GDT pointer) is defined in another file.
extern gp

; Define the gdt_flush function.
gdt_flush:
    ; Load the GDT pointer (gp) into the GDTR register using the lgdt instruction.
    lgdt [gp]

    ; Set up segment registers with the data segment selector.
    ; 0x10 is the selector for the data segment, which is the second entry in the GDT.
    mov ax, 0x10
    mov ds, ax   ; Load the data segment (ds) register.
    mov es, ax   ; Load the extra segment (es) register.
    mov fs, ax   ; Load the fs segment register.
    mov gs, ax   ; Load the gs segment register.
    mov ss, ax   ; Load the stack segment (ss) register.

    ; Perform a far jump to reload the code segment register (cs) with the code segment selector.
    ; 0x08 is the selector for the code segment, which is the first entry in the GDT.
    jmp 0x08:flush2

; Label for the code that is executed after the far jump.
flush2:
    ; Return from the function.
    ret
