#include "../ports.h"
#include "pic.h"
#include "interrupts.h"
#include "drivers/screen/screen.h"
#include "drivers/keyboard/keyboard.h"
#include "kernel/utils/utils.h"

#define PIC_EOI		0x20
#define KEYBOARD_DATA_PORT 0x60

volatile unsigned int timer_ticks = 0;


void interrupt_handler(struct registers_t* regs) {
    switch (regs->int_no)
    {
    case 0:
        print("DIV by 0");
        break;
    case 14:{
        unsigned int faulting_address;
        __asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));

        int present = !(regs->err_code & 0x1); // Bit 0: 0 = Not Present
        int rw = regs->err_code & 0x2;         // Bit 1: 1 = Write access
        int us = regs->err_code & 0x4;         // Bit 2: 1 = User mode

        print("\n--- PAGE FAULT EXCEPTION ---\n");
    
        print("Faulting Address: ");
        print_hex_ll(faulting_address);
        print("\n");

        print("Error Code: ");
        print_int(regs->err_code);
        print("\n");

        print("Reason: ");
        if (present) print(" [Page Not Present]");
        else         print(" [Protection Violation]");

        if (rw)      print(" [Write Operation]");
        else         print(" [Read Operation]");

        if (us)      print(" [User Mode]");
        else         print(" [Kernel Mode]");
        
        print("\n");

        panic("System halted.");
        break;
    }
    case 32:
        timer_ticks++;
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