#include "pic.h"
#include "../ports.h"

void pic_remap()
{
    // Save current masks
    unsigned char mask1 = port_byte_in(PIC1_DATA);
    unsigned char mask2 = port_byte_in(PIC2_DATA);

    // --- Start Initialization Sequence (ICW1) ---
    port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // --- Set Vector Offsets (ICW2) ---
    // Master PIC: IRQs 0-7 -> Interrupts 32-39 (0x20)
    // Slave PIC:  IRQs 8-15 -> Interrupts 40-47 (0x28)
    port_byte_out(PIC1_DATA, 0x20);
    port_byte_out(PIC2_DATA, 0x28);

    // --- Configure Chaining (ICW3) ---
    port_byte_out(PIC1_DATA, 4); // Tell Master that Slave is at IRQ2
    port_byte_out(PIC2_DATA, 2); // Tell Slave its cascade identity is 2

    // --- Set Mode (ICW4) ---
    port_byte_out(PIC1_DATA, ICW4_8086);
    port_byte_out(PIC2_DATA, ICW4_8086);

    // Restore saved masks
    port_byte_out(PIC1_DATA, mask1);
    port_byte_out(PIC2_DATA, mask2);
}

void pic_unmask_irq(int irq_line){
    if (irq_line < 0 || irq_line > 7){
        return;
    }

    unsigned char mask = port_byte_in(PIC1_DATA) & ~(1 << irq_line);
    port_byte_out(PIC1_DATA, mask);
}

void pic_mask_irq(int irq_line){
    if (irq_line < 0 || irq_line > 7){
        return;
    }
    
    unsigned char mask = port_byte_in(PIC1_DATA) | (1 << irq_line);
    port_byte_out(PIC1_DATA, mask);
}