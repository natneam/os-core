# Toolchain definition
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = nasm

# ISO Configuration
ISO_NAME := os-img.iso
ISO_DIR  := iso
ISO_GRUB_DIR := $(ISO_DIR)/boot/grub

# Kernel entry object
ENTRY_OBJECT := build/kernel/kernel_entry.o

# Directories
C_SOURCES := $(shell find kernel drivers arch libc -name '*.c')
C_OBJECTS := $(patsubst %.c, build/%.o, $(C_SOURCES))
ASM_OBJECTS := $(ENTRY_OBJECT) build/arch/idt.o build/arch/interrupts.o
DEPS      := $(patsubst %.c, build/%.d, $(C_SOURCES))

# Flags
CFLAGS := -m32 -g -Wall -Wextra -I. -nostdlib -nostdinc -ffreestanding -fno-builtin -fno-stack-protector -MMD -MP
LDFLAGS := -m elf_i386 -T linker.ld
ASFLAGS_32 := -f elf32

all: run

.PHONY: all run clean

run: $(ISO_NAME)
	qemu-system-i386 -cdrom $(ISO_NAME)

# Rule to create the final bootable ISO
$(ISO_NAME): build/kernel.elf $(ISO_GRUB_DIR)/grub.cfg
	@echo "Creating bootable ISO image..."
	cp build/kernel.elf $(ISO_DIR)/boot/kernel.elf
	i686-elf-grub-mkrescue -o $(ISO_NAME) $(ISO_DIR)

# Rule to generate the grub.cfg file
$(ISO_GRUB_DIR)/grub.cfg:
	@echo "Creating GRUB config..."
	@mkdir -p $(ISO_GRUB_DIR)
	@echo 'set timeout=0' > $@
	@echo 'set default=0' >> $@
	@echo '' >> $@
	@echo 'menuentry "My OS" {' >> $@
	@echo '    multiboot /boot/kernel.elf' >> $@
	@echo '    boot' >> $@
	@echo '}' >> $@

# Link the kernel binary
build/kernel.elf: $(ASM_OBJECTS) $(C_OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o $@ $^

# Include auto-generated dependency files
-include $(DEPS)

# Compile C source files
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble the 32-bit kernel entry point
$(ENTRY_OBJECT): kernel/kernel_entry.asm kernel/multiboot_header.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS_32) $< -o $@

# Assemble the interrupt handlers
build/arch/interrupts.o: arch/interrupts/interrupts.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS_32) $< -o $@

build/arch/idt.o: arch/interrupts/idt.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS_32) $< -o $@

clean:
	@echo "Cleaning build artifacts..."
	rm -rf build $(ISO_NAME) $(ISO_DIR)