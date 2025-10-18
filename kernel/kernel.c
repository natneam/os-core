#include "../arch/interrupts/idt.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/timer/timer.h"
#include "utils/utils.h"
#include "shell/shell.h"

#define TIMER_FREQUENCY 100

void main(multiboot_info_t *mbd, unsigned int magic)
{
  multiboot_validate(mbd, magic);

  // clear_screen();

  idt_init();

  timer_init(TIMER_FREQUENCY);

  keyboard_init();

  multiboot_print_memory_map(mbd);

  __asm__ __volatile__("sti");

  // Simple shell
  shell();

  while (1)
  {
    __asm__ __volatile__("hlt");
  }
}
