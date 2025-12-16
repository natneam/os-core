#include "heap.h"
#include "vmm.h"
#include "pmm.h"
#include "../utils/utils.h"

#define KHEAP_START 0xD0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define PAGE_SIZE 4096

KHeapBlock *kheap_head = 0;

void kheap_init()
{
    unsigned int total_pages = KHEAP_INITIAL_SIZE / PAGE_SIZE;
    unsigned int virt_addr = KHEAP_START;

    for (unsigned int i = 0; i < total_pages; i++)
    {
        void *phys_addr = pmm_alloc_page();

        if (phys_addr == 0)
        {
            panic("HEAP out of memory during initialization");
        }

        vmm_map_page(phys_addr, (void *)virt_addr, PTE_PRESENT | PTE_RW);

        virt_addr += PAGE_SIZE;
    }

    kheap_head = (KHeapBlock *)KHEAP_START;
    kheap_head->size = KHEAP_INITIAL_SIZE - sizeof(KHeapBlock);
    kheap_head->is_free = 1;
    kheap_head->next = 0;
}

void *kmalloc(unsigned int size)
{
    if (size == 0) return 0;

    unsigned int aligned_size = (size + 3) & ~3;

    KHeapBlock *current = kheap_head;

    while (current != 0)
    {
        if (current->is_free && current->size >= aligned_size)
        {
            if (current->size - aligned_size > (sizeof(KHeapBlock) + 4))
            {
                KHeapBlock* new_block = (KHeapBlock*) ((unsigned int)current + sizeof(KHeapBlock) + aligned_size);
                
                new_block->is_free = 1;
                new_block->size = current->size - aligned_size - sizeof(KHeapBlock);
                new_block->next = current->next;

                current->size = aligned_size;
                current->next = new_block;
            }

            current->is_free = 0;

            return (void *)((unsigned int)current + sizeof(KHeapBlock));
        }

        current = current->next;
    }

    return 0;
}

void kfree(void *ptr)
{
    KHeapBlock *header = (KHeapBlock *)((unsigned int)ptr - sizeof(KHeapBlock));
    header->is_free = 1;
    while (header->next != 0 && header->next->is_free)
    {
        header->size += header->next->size + sizeof(KHeapBlock);
        header->next = header->next->next;
    }
}