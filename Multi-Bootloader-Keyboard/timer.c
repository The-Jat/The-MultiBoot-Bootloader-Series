#include <system.h>

/* 
 * Configure the Programmable Interval Timer (PIT) to generate interrupts at a specific frequency.
 */
void timer_phase(int hz) {
    // Calculate the divisor needed for the desired frequency
    int divisor = 1193180 / hz;
    
    // Send the command byte to PIT control register
    outportb(0x43, 0x36);
    
    // Send the divisor to the PIT data register
    outportb(0x40, divisor & 0xFF);    // Low byte
    outportb(0x40, divisor >> 8);      // High byte
}

// Global variables to keep track of timer ticks and ticker
int timer_ticks = 0;
unsigned long ticker = 0;

/* 
 * Timer interrupt handler.
 * This function will be called every time the timer interrupt occurs.
 */
void timer_handler(struct regs *r) {
    // Increment the timer ticks
    ++timer_ticks;
    
    // Perform actions every 18 ticks (~1 second if the frequency is 18.222 Hz)
    if (timer_ticks % 18 == 0) {
        ticker++;  // Increment the ticker
        
        // Print a message to indicate a tick
        puts("Tick. ");
        
        // Display a rotating animation to show timer progress
        if     (ticker % 4 == 0) { putch('|'); }
        else if(ticker % 4 == 1) { putch('/'); }
        else if(ticker % 4 == 2) { putch('-'); }
        else if(ticker % 4 == 3) { putch('\\'); }
        
        // New line for readability
        putch('\n');
    }
}

/* 
 * Install the timer interrupt handler.
 * This function sets up the handler for IRQ0 (the timer interrupt).
 */
void timer_install() {
    irq_install_handler(0, timer_handler);
}

/* 
 * Wait for a specified number of timer ticks.
 * This function will block until the specified number of ticks has passed.
 */
void timer_wait(int ticks) {
    // Calculate the end tick count
    unsigned long eticks = timer_ticks + ticks;
    
    // Wait until the current tick count reaches the end tick count
    while(timer_ticks < eticks);
}
