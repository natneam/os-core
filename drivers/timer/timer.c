#include "timer.h"
#include "../../arch/interrupts/pic.h"
#include "../../arch/ports.h"

void timer_init(int frequency)
{

    // Set the PIT frequency to 100Hz
    int divisor = PIT_BASE_FREQUENCY / frequency;

    port_byte_out(PIT_COMMAND_PORT, 0x36);

    unsigned char low_byte = (unsigned char)(divisor & 0xFF);
    unsigned char high_byte = (unsigned char)((divisor >> 8) & 0xFF);

    port_byte_out(PIT_CHANNEL0_DATA_PORT, low_byte);
    port_byte_out(PIT_CHANNEL0_DATA_PORT, high_byte);

    // Unmask IRQ0
    pic_unmask_irq(0);
}