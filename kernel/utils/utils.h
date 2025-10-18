#ifndef KERNEL_UTILS_H
#define KERNEL_UTILS_H

#include "../boot/multiboot.h"

void panic(const char *message);
void multiboot_validate(multiboot_info_t *mbd, unsigned int magic);
void multiboot_print_memory_map(multiboot_info_t *mbd);

#endif