#include "../arch/interrupts/idt.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"

void main()
{
  clear_screen();
  print("Hello World!\n");

  idt_init();
  print("IDT and PIC initialized.\n");

  keyboard_init();
  print("Keyboard initialized (IRQ1 unmasked).\n");

  __asm__ __volatile__("sti");
  print("Interrupts enabled. Waiting for keypress...\n");

  // Echo characters as they are typed
  while(1){
    char str[2] = {getchar(), '\0'};
    print(str);
  }

  while (1)
  {
    __asm__ __volatile__("hlt");
  }
}
