#ifndef IDT_H
#define IDT_H

// An entry in the Interrupt Descriptor Table
struct idt_entry {
    unsigned short base_low;
    unsigned short selector;
    unsigned char  zero;
    unsigned char  flags;
    unsigned short base_high;
} __attribute__((packed));

// A pointer to the IDT, used by the lidt instruction
struct idt_ptr {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

void idt_set_gate(int num, unsigned long base, unsigned short selector, unsigned char flags);

void idt_init();

#endif