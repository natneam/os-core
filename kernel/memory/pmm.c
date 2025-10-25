#include "pmm.h"
#include "../utils/utils.h"
#include "../../drivers/screen/screen.h"
#include "../../libc/memory.h"

static unsigned char *g_bit_map = 0;
static unsigned int g_bit_map_size = 0;

extern unsigned int KERNEL_START[];
extern unsigned int KERNEL_END[];

static void pmm_set_bit(unsigned int page_index)
{
    unsigned int region = page_index / 8;
    unsigned int bit = page_index % 8;

    g_bit_map[region] |= (1 << bit);
}

static void pmm_clear_bit(unsigned int page_index)
{
    unsigned int region = page_index / 8;
    unsigned int bit = page_index % 8;

    g_bit_map[region] &= ~(1 << bit);
}

static void pmm_set_region(unsigned int start_page, unsigned int num_pages)
{
    for (unsigned int page = start_page; page < start_page + num_pages; page++)
    {
        pmm_set_bit(page);
    }
}

static void pmm_clear_region(unsigned int start_page, unsigned int num_pages)
{
    for (unsigned int page = start_page; page < start_page + num_pages; page++)
    {
        pmm_clear_bit(page);
    }
}

void pmm_init(multiboot_info_t *mbd)
{
    // Find total memory
    unsigned long long total_memory = 0;
    multiboot_memory_map_t *entry = (multiboot_memory_map_t *)mbd->mmap_addr;
    while ((unsigned int)entry < mbd->mmap_addr + mbd->mmap_length)
    {
        if (entry->addr + entry->len > total_memory)
        {
            total_memory = entry->addr + entry->len;
        }

        entry = (multiboot_memory_map_t *)((unsigned int)entry + entry->size + sizeof(entry->size));
    }

    g_bit_map_size = (((total_memory + PAGE_SIZE - 1) / PAGE_SIZE) + 7) / 8;

    // Find address for the bitmap
    entry = (multiboot_memory_map_t *)mbd->mmap_addr;
    while ((unsigned int)entry < mbd->mmap_addr + mbd->mmap_length)
    {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            unsigned int placement_addr = ((unsigned int)&KERNEL_END + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

            unsigned int usable_start = (unsigned int)entry->addr;
            if (usable_start < placement_addr)
            {
                usable_start = placement_addr;
            }

            unsigned int region_end = (unsigned int)entry->addr + (unsigned int)entry->len;
            if (region_end > usable_start)
            {
                unsigned int available_len = region_end - usable_start;

                if (available_len >= g_bit_map_size)
                {
                    g_bit_map = (unsigned char *)usable_start;
                    break;
                }
            }
        }

        entry = (multiboot_memory_map_t *)((unsigned int)entry + entry->size + sizeof(entry->size));
    }

    if (g_bit_map == 0)
    {
        panic("Could not find a suitable memory region for the PMM bitmap!");
    }

    // set everything to used in the bitmap
    memset(g_bit_map, 0xFF, g_bit_map_size);

    // Free unused memory
    entry = (multiboot_memory_map_t *)mbd->mmap_addr;
    while ((unsigned int)entry < mbd->mmap_addr + mbd->mmap_length)
    {
        if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            pmm_clear_region(entry->addr / PAGE_SIZE, entry->len / PAGE_SIZE);
        }

        entry = (multiboot_memory_map_t *)((unsigned int)entry + entry->size + sizeof(entry->size));
    }

    // Reclaim BIOS/hardware memory
    pmm_set_region(0, 256);

    // Reclaim KERNEL memory
    unsigned int kernel_start = ((unsigned int)&KERNEL_START) / PAGE_SIZE;
    unsigned int kernel_end = ((unsigned int)&KERNEL_END + PAGE_SIZE - 1) / PAGE_SIZE;
    pmm_set_region(kernel_start, kernel_end - kernel_start + 1);

    // Reclaim bitmap memory
    unsigned int bitmap_start = (unsigned int)g_bit_map / PAGE_SIZE;
    pmm_set_region(bitmap_start, (g_bit_map_size + PAGE_SIZE - 1) / PAGE_SIZE);
}

void *pmm_alloc_page()
{
    for (unsigned int i = 0; i < g_bit_map_size; i++)
    {
        if (g_bit_map[i] != 0xFF)
        {
            for (unsigned int j = 0; j < 8; j++)
            {
                if ((g_bit_map[i] & (1 << j)) == 0)
                {
                    unsigned int page_idx = i * 8 + j;
                    pmm_set_bit(page_idx);
                    return (void *) (page_idx * PAGE_SIZE);
                }
            }
        }
    }
    return 0;
}


void pmm_free_page(void *p) {
    if (p == 0) {
        return;
    }

    if((unsigned int)p % PAGE_SIZE != 0){
        panic("Invalid Address");
    }

    pmm_clear_bit((unsigned int)p / PAGE_SIZE);
}