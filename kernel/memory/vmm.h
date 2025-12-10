#ifndef VMM_H
#define VMM_H

// Page Table/Directory Entry Flags
#define PTE_PRESENT   0x1  // Is the page present in memory?
#define PTE_RW        0x2  // Is the page writable? (0 = Read-only)
#define PTE_USER      0x4  // Is it accessible by User Mode (Ring 3)?
#define PTE_WRITETHROUGH 0x8
#define PTE_CACHE_DISABLE 0x10
#define PTE_ACCESSED  0x20
#define PTE_DIRTY     0x40 // Has the page been written to?
#define PTE_FRAME     0xFFFFF000 // Mask to get the physical address

// A Page Table Entry is just a 32-bit integer
typedef unsigned int pte_t;

// A Page Directory is an array of 1024 entries
typedef struct {
    pte_t entries[1024];
} page_directory_t;

// A Page Table is also an array of 1024 entries
typedef struct {
    pte_t entries[1024];
} page_table_t;

// Functions
void vmm_init();
void vmm_map_page(void* phys_addr, void* virt_addr, unsigned int flags);

#endif