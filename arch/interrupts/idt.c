#include "idt.h"
#include "pic.h"
#include "../ports.h"
#include "drivers/screen/screen.h"

struct idt_entry idt[256];
struct idt_ptr idt_p;

extern void load_idt(struct idt_ptr *idt_p);

extern void *interrupt_handlers_stub_table[];

void idt_set_gate(int num, unsigned long base, unsigned short selector, unsigned char flags)
{
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_init()
{
    idt_p.limit = sizeof(struct idt_entry) * 256;
    idt_p.base = (unsigned int)&idt;

    for (int i = 0; i < 256; i++)
    {
        idt_set_gate(i, (int)interrupt_handlers_stub_table[i], 0x08, 0x8E);
    }

    pic_remap();

    port_byte_out(PIC1_DATA, 0xFF); // Mask all interrupts on Master
    port_byte_out(PIC2_DATA, 0xFF); // Mask all interrupts on Slave

    load_idt(&idt_p);
}