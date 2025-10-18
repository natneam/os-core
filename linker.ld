ENTRY(start) /* The entry point of the kernel */

SECTIONS
{
    . = 0x100000; /* Start loading the kernel at 1MB */
    
    KERNEL_START = .;

    .text :
    {
        *(.multiboot) /* First, place the multiboot header */
        *(.text)     /* Then, all the compiled code */
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    KERNEL_END = .;
}