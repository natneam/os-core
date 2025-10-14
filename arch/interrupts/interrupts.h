#ifndef INTERRUPTS_H
#define INTERRUPTS_H

struct registers_t {
    // Manually pushed segment registers
    unsigned int gs, fs, es, ds;

    // Pushed by the 'pushad' instruction
    unsigned int edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax;

    // Pushed by our interrupt stubs
    unsigned int int_no, err_code;

    // Pushed by the CPU automatically on interrupt
    unsigned int eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed));

void interrupt_handler(struct registers_t* regs);

#endif