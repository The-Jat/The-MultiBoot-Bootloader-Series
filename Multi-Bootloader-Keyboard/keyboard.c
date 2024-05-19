#include <system.h>

/* 
 * US Keyboard layout mapping scancodes to ASCII characters.
 * This array maps the scancodes from the keyboard to the corresponding characters.
 */
char kbd_us[128] = {
    0, 27,            // 0x00: No key, 0x01: ESC
    '1','2','3','4','5','6','7','8','9','0', // 0x02-0x0B: Number keys
    '-','=','\b',     // 0x0C-0x0E: '-', '=', Backspace
    '\t',             // 0x0F: Tab
    'q','w','e','r','t','y','u','i','o','p', // 0x10-0x19: Alphabet keys
    '[',']','\n',     // 0x1A-0x1C: '[', ']', Enter
    0,                // 0x1D: Control
    'a','s','d','f','g','h','j','k','l',';', // 0x1E-0x26: Alphabet keys
    '\'', '`',        // 0x27-0x28: '\'', '`'
    0,                // 0x29: Left shift
    '\\','z','x','c','v','b','n','m',',','.','/', // 0x2A-0x35: Alphabet keys and symbols
    0,                // 0x36: Right shift
    '*',              // 0x37: Keypad '*'
    0,                // 0x38: Alt
    ' ',              // 0x39: Space
    0,                // 0x3A: Caps lock
    0,                // 0x3B: F1
    0, 0, 0, 0, 0, 0, 0, 0, // 0x3C-0x43: F2-F10 (not mapped)
    0,                // 0x44: Num lock
    0,                // 0x45: Scroll lock
    0,                // 0x46: Home
    0,                // 0x47: Up arrow
    0,                // 0x48: Page up
    '-',              // 0x49: Keypad '-'
    0,                // 0x4A: Left arrow
    0,                // 0x4B: (empty)
    0,                // 0x4C: Right arrow
    '+',              // 0x4D: Keypad '+'
    0,                // 0x4E: End
    0,                // 0x4F: Down arrow
    0,                // 0x50: Page down
    0,                // 0x51: Insert
    0,                // 0x52: Delete
    0, 0, 0,          // 0x53-0x55: (empty)
    0,                // 0x56: F11
    0,                // 0x57: F12
    0                 // 0x58-0x7F: (everything else)
};

/*
 * Keyboard interrupt handler.
 * This function handles keyboard interrupts, reads the scancode from the keyboard,
 * and translates it into an ASCII character using the kbd_us array.
 */
void keyboard_handler(struct regs *r) {
    unsigned char scancode;

    // Read the scancode from the keyboard's data buffer
    scancode = inportb(0x60);

    if (scancode & 0x80) {
        // If the highest bit of the scancode is set, it's a key release event (key up).
        // We ignore these events in this simple implementation.
    } else {
        // If the highest bit is not set, it's a key press event (key down).
        // We convert the scancode to an ASCII character and print it to the screen.
        putch(kbd_us[scancode]);
    }
}

/*
 * Install the keyboard handler.
 * This function installs the keyboard_handler function as the IRQ1 handler.
 */
void keyboard_install() {
    // Install the keyboard handler for IRQ1 (keyboard interrupt)
    irq_install_handler(1, keyboard_handler);
}

/*
 * Wait for the keyboard controller to be ready.
 * This function waits until the keyboard controller is ready to send or receive data.
 */
void keyboard_wait() {
    // Wait until the keyboard controller's input buffer is clear
    while(inportb(0x64) & 2);
}

