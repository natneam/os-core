#include "../ports.h"
#include "pic.h"
#include "interrupts.h"
#include "drivers/screen/screen.h"
#include "drivers/keyboard/keyboard.h"

#define PIC_EOI		0x20
#define KEYBOARD_DATA_PORT 0x60


void interrupt_handler(struct registers_t* regs) {
    switch (regs->int_no)
    {
    case 0:
        print("DIV by 0");
        break;
    case 33:
        handle_keyboard_interrupt();
        break;
    default:
        print("Unhandled Interrupt: ");
        print_int(regs->int_no);
        print("\n");
        break;
    }

    // If it was a hardware interrupt, send EOI
    if (regs->int_no >= 32 && regs->int_no <= 47) {
        port_byte_out(PIC1_COMMAND, PIC_EOI); // EOI to Master PIC
        if (regs->int_no >= 40) {
            port_byte_out(PIC2_COMMAND, PIC_EOI); // EOI to Slave PIC if needed
        }
    }
}