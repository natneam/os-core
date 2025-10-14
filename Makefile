# Toolchain definition
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = nasm

# Directories
C_SOURCES := $(shell find kernel drivers arch libc -name '*.c')
C_OBJECTS := $(patsubst %.c, build/%.o, $(C_SOURCES))
DEPS      := $(patsubst %.c, build/%.d, $(C_SOURCES))

# Kernel entry object
ENTRY_OBJECT := build/kernel/kernel_entry.o

# Flags
CFLAGS := -m32 -g -Wall -Wextra -I. -nostdlib -nostdinc -ffreestanding -fno-builtin -fno-stack-protector -MMD -MP
LDFLAGS := -m elf_i386 -e start -Ttext 0x1000 --oformat binary
ASFLAGS_32 := -f elf32
ASFLAGS_16 := -f bin

TARGET := os-img

all: run

.PHONY: all run clean

run: $(TARGET)
	qemu-system-x86_64 -fda $(TARGET)

$(TARGET): build/boot.bin build/kernel.bin
	cat build/boot.bin build/kernel.bin > $@

# Link the kernel binary
build/kernel.bin: $(ENTRY_OBJECT) build/arch/idt.o $(C_OBJECTS) build/arch/interrupts.o
	$(LD) $(LDFLAGS) -o $@ $^

# Include auto-generated dependency files
-include $(DEPS)

# Compile C source files
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble the 16-bit bootloader
build/boot.bin: boot/boot.asm boot/disk_load.asm boot/print_16.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS_16) $< -o $@

# Assemble the 32-bit kernel entry point
$(ENTRY_OBJECT): kernel/kernel_entry.asm arch/GDT.asm arch/switch_to_pm.asm arch/print_32.asm
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
	rm -rf build $(TARGET)