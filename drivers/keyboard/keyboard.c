#include "keyboard.h"
#include "../../arch/ports.h"
#include "../screen/screen.h"

// US QWERTY keyboard scan code map
static const char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* Backspace */
    '\t',                                                                    /* Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',        /* Enter key */
    0,                                                                       /* Left Ctrl */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, /* Left Shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, /* Right Shift */
    '*',
    0,   /* Alt */
    ' ', /* Space bar */
    0,   /* Caps lock */
    0,   /* F1 ... > */
    0, 0, 0, 0, 0, 0, 0, 0,
    0, /* < ... F10 */
    0, /* Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up Arrow */
    0, /* Page Up */
    '-',
    0, /* Left Arrow */
    0,
    0, /* Right Arrow */
    '+',
    0, /* End key */
    0, /* Down Arrow */
    0, /* Page Down */
    0, /* Insert Key */
    0, /* Delete Key */
    0, 0, 0,
    0, /* F11 */
    0, /* F12 */
    0, /* All other keys are undefined */
};

void keyboard_init()
{
    unsigned char current_mask = port_byte_in(0x21);
    port_byte_out(0x21, current_mask & 0xFD);
}

void handle_keyboard_interrupt()
{
    unsigned char scancode = port_byte_in(KEYBOARD_DATA_PORT);
    if ((scancode & 0x80) == 0) { // Key press
        if (scancode_map[scancode] != 0) {
            char msg[2] = {scancode_map[scancode], '\0'};
            print(msg);
        }
    }
}