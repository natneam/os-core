#include "vmm.h"
#include "pmm.h"
#include "../../libc/memory.h"
#include "../utils/utils.h"

#define PAGE_SIZE 4096

extern unsigned int KERNEL_START[];
extern unsigned int KERNEL_END[]; 

static inline void load_page_directory(unsigned int* page_directory_phys_addr) {
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(page_directory_phys_addr));
}

static inline void enable_paging() {
    unsigned int cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Set PG bit (bit 31)
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));
}

page_directory_t* kernel_directory = 0;

void vmm_map_page(void* phys_addr, void* virt_addr, unsigned int flags){
    unsigned int pd_index = (unsigned int) virt_addr >> 22;
    unsigned int pt_index = ((unsigned int) virt_addr >> 12) & 0x3FF;

    pte_t* pde = &kernel_directory->entries[pd_index];

    page_table_t* table;

    if((*pde & PTE_PRESENT) != 0) {
        unsigned int table_phys_addr = *pde & PTE_FRAME;
        table = (page_table_t*) table_phys_addr;
    } else {
        unsigned int table_phys_addr = (unsigned int)pmm_alloc_page();
        if(!table_phys_addr){
            panic("Out of memory while creating page table");
            return;
        }

        table = (page_table_t*) table_phys_addr;
        memset(table, 0, PAGE_SIZE);

        *pde = table_phys_addr | PTE_PRESENT | PTE_RW;
    }

    table->entries[pt_index] = (unsigned int)phys_addr | flags | PTE_PRESENT;

    __asm__ __volatile__("invlpg (%0)" : : "r" (virt_addr) : "memory");
}

void vmm_init() {
    kernel_directory = (page_directory_t*)pmm_alloc_page();
    if (!kernel_directory) {
        panic("Could not allocate initial page directory");
    }
    memset(kernel_directory, 0, sizeof(page_directory_t));


    unsigned int current_addr = 0;
    unsigned int max_addr = (unsigned int)&KERNEL_END + (4 * 1024 * 1024);

    if (max_addr < 0x100000 + 0xB8000) {
        max_addr = 0x200000; 
    }

    while ( current_addr < max_addr) {
        vmm_map_page((void*)current_addr, (void*)current_addr, PTE_PRESENT | PTE_RW);
        current_addr += PAGE_SIZE;
    }

    load_page_directory((unsigned int*)kernel_directory);

    enable_paging();   
}