#include "../../drivers/screen/screen.h"
#include "../../drivers/keyboard/keyboard.h"
#include "../../libc/string.h"
#include "../time/time.h"

#include "shell.h"

#define MAX_COMMAND_BUFFER_SIZE 256

char cmd_buffer[MAX_COMMAND_BUFFER_SIZE];
int buffer_idx = 0;

void process_command(char *buffer)
{
    // preprocess the command buffer
    unsigned int command_end = 0;

    while (buffer[command_end] != '\0' && buffer[command_end] != ' ')
    {
        command_end++;
    }

    buffer[command_end] = '\0';
    char *arg = &(buffer[command_end + 1]);

    if (strcmp(buffer, "clear") == 0)
    {
        clear_screen();
    }
    else if (strcmp(buffer, "ticks") == 0)
    {
        print("System ticks: ");
        print_int(timer_ticks);
        print("\n");
    }
    else if (strcmp(buffer, "sleep") == 0)
    {
        int ms = atoi(arg);
        print("waiting for ");
        print_int(ms / 1000);
        print(" seconds\n");
        sleep(ms);
        print("DONE\n");
    }
    else
    {
        print(buffer);
        print(" is not a valid command");
        print("\n");
    }
}

void draw_shell_prompt()
{
    print(">");
}

void shell()
{
    draw_shell_prompt();
    while (1)
    {
        char received_char = getchar();
        char str[2] = {received_char, '\0'};

        if (received_char == '\n')
        {
            print(str);
            cmd_buffer[buffer_idx] = '\0';

            process_command(cmd_buffer);

            draw_shell_prompt();
            buffer_idx = 0;
        }
        else if (received_char == '\b')
        {
            if (buffer_idx > 0){
                buffer_idx--;
                print(str);
            }
        }
        else
        {
            // Only add chars if there is space
            if (buffer_idx < MAX_COMMAND_BUFFER_SIZE - 1)
            {
                cmd_buffer[buffer_idx] = received_char;
                buffer_idx++;
                print(str);
            }
        }
    }
}