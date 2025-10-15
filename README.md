# os-core: A Simple 32-bit x86 Operating System

Welcome to os-core! This project is an educational operating system for the x86 architecture, built from scratch. It documents the journey from a simple 16-bit bootloader to a functional 32-bit protected-mode kernel with basic driver support and interrupt handling.

## Features

*   **16-bit Real Mode Bootloader:** Loads the kernel from a floppy disk image.
*   **32-bit Protected Mode:** Switches the CPU from 16-bit real mode to 32-bit protected mode.
*   **Global Descriptor Table (GDT):** Sets up segment descriptors for code and data.
*   **Interrupt Handling:**
    *   **Interrupt Descriptor Table (IDT):** Manages hardware and software interrupts.
    *   **PIC Remapping:** Remaps the Programmable Interrupt Controller to avoid conflicts with CPU exceptions.
*   **Basic I/O Drivers:**
    *   **Screen Driver:** For printing text, clearing the screen, and handling scrolling.
    *   **Keyboard Driver:** For reading character input from the keyboard.
*   **Simple C Library:** Includes a basic `memory_copy` function.

## Getting Started

### Prerequisites

To build and run this OS, you will need the following tools:

*   A cross-compiler toolchain for `x86_64-elf`.
*   `nasm` (The Netwide Assembler)
*   `qemu-system-x86_64` for emulation.

### Building and Running

1.  **Build the OS image and run it in QEMU:**
    ```bash
    make
    ```
    This command compiles the source files, creates the `os-img` floppy disk image, and launches QEMU to run the operating system automatically.
2.  **Clean up build files:**
    ```bash
    make clean
    ```
    This will remove all generated object files, binaries, and the OS image.

## Project Structure

```
.
├── Makefile              # Build script for the project
├── arch                  # Architecture-specific code (x86)
│   ├── GDT.asm
│   ├── interrupts
│   ├── ports.c
│   └── ...
├── boot                  # 16-bit bootloader code
│   ├── boot.asm
│   └── ...
├── drivers               # Device drivers
│   ├── keyboard
│   └── screen
├── kernel                # The main OS kernel
│   ├── kernel.c
│   └── kernel_entry.asm
└── libc                  # A basic C library implementation
    └── memory.c
```

*   **`arch/`**: Contains architecture-specific code for the x86 platform, including GDT setup, interrupt handling (IDT, PIC), and port I/O.
*   **`boot/`**: Holds the 16-bit assembly code for the bootloader. `boot.asm` is the entry point.
*   **`drivers/`**: Contains device drivers, currently for the screen and keyboard.
*   **`kernel/`**: The core of the operating system. `kernel_entry.asm` is the 32-bit entry point which calls `main()` in `kernel.c`.
*   **`libc/`**: Provides basic C library functions like `memory_copy`.
*   **`Makefile`**: Defines the build process, including compilation, assembly, and linking.

## How it Works

1.  **Booting:** The BIOS loads `boot.asm` into memory and executes it. The bootloader then loads the kernel from the disk into memory at address `0x1000`.
2.  **Protected Mode:** The bootloader switches the CPU into 32-bit protected mode and jumps to the kernel's 32-bit entry point (`kernel_entry.asm`).
3.  **Kernel Initialization:** The kernel entry code sets up the stack and calls the `main` function in `kernel.c`.
4.  **Driver and Interrupt Initialization:** The `main` function initializes the screen, sets up the IDT and PIC, and enables keyboard interrupts.
5.  **User Interaction:** The kernel enters a loop, echoing any typed characters to the screen.
