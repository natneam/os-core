#include "../../drivers/screen/screen.h"
#include "utils.h"

void panic(const char *message)
{
  print("KERNEL PANIC: ");
  print(message);
  while (1)
  {
    __asm__ __volatile__("hlt");
  }
}

void multiboot_validate(multiboot_info_t *mbd, unsigned int magic) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
      panic("Invalid Multiboot magic number.");
  }

  // Check bit 6 of the flags field, which indicates a valid memory map.
  if (!(mbd->flags & MULTIBOOT_INFO_MEM_MAP)) {
      panic("No memory map provided by GRUB.");
  }
}

void multiboot_print_memory_map(multiboot_info_t *mbd) {
  print("--- Physical Memory Map ---\n");

  // Iterate through the memory map provided by GRUB.
  multiboot_memory_map_t *entry = (multiboot_memory_map_t *)mbd->mmap_addr;
  while ((unsigned int)entry < mbd->mmap_addr + mbd->mmap_length) {
      print(" Addr: ");
      print_hex_ll(entry->addr);
      print(" | Len: ");
      print_hex_ll(entry->len);
      print(" | Type: ");

      switch (entry->type) {
          case MULTIBOOT_MEMORY_AVAILABLE:      print("Available\n"); break;
          case MULTIBOOT_MEMORY_RESERVED:       print("Reserved\n"); break;
          case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: print("ACPI Reclaimable\n"); break;
          case MULTIBOOT_MEMORY_NVS:            print("NVS\n"); break;
          case MULTIBOOT_MEMORY_BADRAM:         print("Bad RAM\n"); break;
          default:                              print("Unknown\n"); break;
      }

      entry = (multiboot_memory_map_t *)((unsigned int)entry + entry->size + sizeof(entry->size));
  }
  print("---------------------------\n");
}
