# os-core: A Simple 32-bit x86 Operating System

Welcome to os-core! This project is an educational operating system for the x86 architecture, built from scratch. It documents the journey from a simple 16-bit bootloader to a functional 32-bit kernel running on the industry-standard GRUB bootloader.

![Kernel Demo](https://natneam.github.io/kernel-at-work.gif)

## Features

*   **GRUB/Multiboot Compliant:** Boots from a standard ISO using the GRUB bootloader and the Multiboot v1 specification.
*   **Memory Map Awareness:** Parses the memory map provided by GRUB to understand the physical memory layout of the machine.
*   **32-bit Protected Mode:** The kernel runs entirely in 32-bit protected mode.
*   **Global Descriptor Table (GDT):** Sets up its own segment descriptors for kernel code and data.
*   **Interrupt Handling:**
    *   **Interrupt Descriptor Table (IDT):** Manages hardware and software interrupts, including CPU exceptions.
    *   **PIC Remapping:** Remaps the Programmable Interrupt Controller to avoid conflicts.
*   **System Timer (PIT):** Uses the Programmable Interval Timer to provide a steady system tick.
*   **Memory Management:**
    *   **Physical Memory Manager (PMM):** Manages the allocation and deallocation of physical memory pages.
    *   **Virtual Memory Manager (VMM):** Handles virtual memory mapping and paging.
    *   **Kernel Heap:** Provides dynamic memory allocation through `kmalloc()` and `kfree()` functions.
*   **Basic I/O Drivers:**
    *   **Screen Driver:** For printing text, clearing the screen, and handling scrolling.
    *   **Keyboard Driver:** For reading character input via a circular buffer.
    *   **Timer Driver:** Uses the Programmable Interval Timer (PIT) for system timing.
*   **Simple Interactive Shell:** A command-line interface with support for various commands:
    *   `clear`: Clear the screen
    *   `ticks`: Display system timer ticks
    *   `sleep <ms>`: Sleep for a specified number of milliseconds
    *   `testpmm`: Test physical memory manager allocation and deallocation
    *   `testheap`: Test kernel heap memory allocation and deallocation
*   **C Library:** Includes essential string and memory functions: `strcmp()`, `strcpy()`, `atoi()`, and memory manipulation utilities.

## Getting Started

### Prerequisites

To build and run this OS, you will need the following tools:

*   A cross-compiler toolchain for `i686-elf` or `x86_64-elf`.
*   The `grub-mkrescue` utility (part of the GRUB toolchain).
*   `nasm` (The Netwide Assembler).
*   `qemu-system-i386` or `qemu-system-x86_64` for emulation.

### Building and Running

1.  **Build the OS image and run it in QEMU:**
    ```bash
    make
    ```
    This command compiles the source files, creates a bootable `os-img.iso` using GRUB, and launches QEMU to run the operating system automatically.

2.  **Clean up build files:**
    ```bash
    make clean
    ```
    This will remove all generated object files, binaries, and the OS image.

## Project Structure

```
.
├── Makefile              # Build script for the project
├── arch/                 # Architecture-specific code (x86)
│   ├── GDT.asm
│   ├── interrupts/
│   └── ports.c
├── boot/                 # (Historical) Original 16-bit bootloader code
├── drivers/              # Device drivers (screen, keyboard, timer)
├── kernel/               # The main OS kernel
│   ├── kernel.c          # Main kernel entry and initialization
│   ├── kernel_entry.asm  # 32-bit entry point with Multiboot header
│   ├── boot/             # Boot-related headers (e.g., multiboot)
│   ├── memory/           # Memory management subsystems
│   │   ├── pmm.c         # Physical memory manager
│   │   ├── vmm.c         # Virtual memory manager
│   │   ├── heap.c        # Kernel heap allocator
│   ├── shell/            # Interactive shell subsystem
│   ├── time/             # Time-related functions (e.g., sleep)
│   └── utils/            # Kernel utilities (panic, boot validation)
└── libc/                 # A basic C library implementation
```

*   **`arch/`**: Contains architecture-specific code for the i386 platform, including GDT setup, interrupt handling (IDT, PIC), and port I/O.
*   **`boot/`**: **(Historical Archive)** This directory contains the original 16-bit assembly code for the custom bootloader. It is no longer compiled but is preserved to document the project's evolution.
*   **`drivers/`**: Contains device drivers for the screen, keyboard, and system timer (PIT).
*   **`kernel/`**: The core of the operating system.
    *   `kernel_entry.asm` is the 32-bit entry point, which contains the Multiboot header and calls `main()`.
    *   `kernel.c` contains the `main()` function, which initializes all kernel subsystems.
    *   The `memory/` subdirectory includes the Physical Memory Manager (PMM), Virtual Memory Manager (VMM), and Kernel Heap allocator.
    *   The `shell/`, `time/`, and `utils/` subdirectories organize the kernel into logical modules.
*   **`libc/`**: Provides basic C library functions.
*   **`Makefile`**: Defines the entire build process, from compilation and assembly to creating the final bootable ISO image with GRUB.

## How It Works

The OS boot process has evolved from its original custom bootloader to a more standard, robust architecture.

1.  **Booting (GRUB):** The system boots from a bootable ISO image. The GRUB bootloader locates the kernel ELF file (`/boot/kernel.elf`) within the ISO.
2.  **Protected Mode & Kernel Entry:** GRUB switches the CPU to 32-bit protected mode, sets up a temporary GDT, and loads the kernel into memory at address `0x100000`. It then jumps to the kernel's `start` label, passing a pointer to the Multiboot information structure in the `EBX` register.
3.  **Kernel Initialization:**
    *   The assembly entry point (`kernel_entry.asm`) sets up a stack and loads the kernel's own GDT.
    *   It then calls the `main` function in `kernel.c`, passing the Multiboot structure pointer.
4.  **Memory System Initialization:** The `main` function initializes memory management subsystems:
    *   **PMM (Physical Memory Manager):** Initializes the physical page allocator using the memory map from GRUB.
    *   **VMM (Virtual Memory Manager):** Sets up virtual memory paging.
    *   **Kernel Heap:** Initializes the dynamic memory allocator (`kmalloc`/`kfree`).
5.  **Driver and Interrupt Initialization:** Initializes the Interrupt Descriptor Table (IDT), Programmable Interrupt Controller (PIC), and device drivers.
6.  **Interactive Shell:** Once initialization is complete, the kernel enables interrupts and launches a simple interactive shell, which waits for and processes user commands.