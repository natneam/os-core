#include "../arch/interrupts/idt.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/timer/timer.h"
#include "utils/utils.h"
#include "shell/shell.h"
#include "memory/pmm.h"
#include "memory/vmm.h"

#define TIMER_FREQUENCY 100

void main(multiboot_info_t *mbd, unsigned int magic)
{
  multiboot_validate(mbd, magic);

  pmm_init(mbd);

  vmm_init();

  idt_init();

  timer_init(TIMER_FREQUENCY);

  keyboard_init();

  clear_screen();

  __asm__ __volatile__("sti");

  shell();

  while (1)
  {
    __asm__ __volatile__("hlt");
  }
}