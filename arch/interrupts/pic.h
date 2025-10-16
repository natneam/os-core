#ifndef PIC_H
#define PIC_H

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// Initialization Command Words
#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

void pic_remap();
void pic_unmask_irq(int irq_line);
void pic_mask_irq(int irq_line);

#endif