#ifndef KHEAP
#define KHEAP

typedef struct KHeapBlock {
    struct KHeapBlock *next;
    unsigned int size;
    unsigned int is_free;
} __attribute__((packed)) KHeapBlock;

void kheap_init();
void* kmalloc(unsigned int size);
void kfree(void* ptr);

#endif