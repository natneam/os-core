#include "../ports.h"
#include "interrupts.h"
#include "drivers/screen/screen.h"

void interrupt_handler(struct registers_t* regs) {
    if (regs->int_no == 33) {
        print("Keypress detected!\n");
    }

    // If it was a hardware interrupt, send EOI
    if (regs->int_no >= 32) {
        port_byte_out(0x20, 0x20); // EOI to Master PIC
    }
}