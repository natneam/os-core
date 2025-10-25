#ifndef PMM_H
#define PMM_H

#include "../boot/multiboot.h"

#define PAGE_SIZE 4096

void pmm_init(multiboot_info_t *mbd);
void *pmm_alloc_page();
void pmm_free_page(void *p);

#endif