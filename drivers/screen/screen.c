#include "screen.h"
#include "../low_level.h"

int get_screen_offset(int col, int row)
{
    int offset = row * MAX_COLS + col;
    return offset * 2;
}

int get_cursor()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int handle_scrolling(int offset)
{
    // check if offset is at the last position
    if (offset == get_screen_offset(MAX_COLS - 1, MAX_ROWS - 1))
    {

        for (int j = 1; j < MAX_ROWS; j++)
        {
            memory_copy((char *)(get_screen_offset(0, j) + VIDEO_ADDRESS), (char *)(get_screen_offset(0, j - 1) + VIDEO_ADDRESS), MAX_COLS * 2);
        }

        char *last_line = (char *)(get_screen_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);

        for (int i = 0; i < MAX_COLS * 2; i++)
        {
            last_line[i] = 0;
        }

        offset = get_screen_offset(0, MAX_ROWS - 1);
    }

    return offset;
}

/**
 * Print a character on the screen at col and row or cursor position
 */
void print_char(char character, int col, int row, char attribute_byte)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;

    if (!attribute_byte)
    {
        attribute_byte = WHITE_ON_BLACK;
    }

    // Get memory offset for the screen location
    int offset;
    if (col >= 0 && row >= 0)
    {
        offset = get_screen_offset(col, row);
    }
    else
    {
        offset = get_cursor();
    }

    // Handle new line
    if (character == '\n')
    {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    }
    // Print the character to the screen
    else
    {
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }

    // Move offset to the next character cell
    offset += 2;

    // Handle scrolling
    offset = handle_scrolling(offset);
    set_cursor(offset);
}

void print_at(char *message, int col, int row)
{
    if (col >= 0 && row >= 0)
    {
        set_cursor(get_screen_offset(col, row));
    }

    col = -1;
    row = -1;

    int i = 0;
    while (message[i] != 0)
    {
        print_char(message[i++], col, row, WHITE_ON_BLACK);
    }
}

void print(char *message)
{
    print_at(message, -1, -1);
}

void clear_screen()
{
    for (int row = 0; row < MAX_ROWS; row++)
    {
        for (int col = 0; col < MAX_COLS; col++)
        {
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }

    set_cursor(get_screen_offset(0, 0));
}