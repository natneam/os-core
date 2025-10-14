#include "keyboard.h"
#include "../../arch/ports.h"

void keyboard_init()
{
    unsigned char current_mask = port_byte_in(0x21);
    port_byte_out(0x21, current_mask & 0xFD);
}