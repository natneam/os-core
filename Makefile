# Define source directories and object files
C_SOURCES := $(shell find kernel drivers -name '*.c')
C_OBJECTS := $(patsubst %.c, build/%.o, $(C_SOURCES))
ENTRY_OBJECT := build/kernel_entry.o

# GCC and Linker flags
CFLAGS := -ffreestanding -m32 -c
LDFLAGS := -m elf_i386 -e start -Ttext 0x1000 --oformat binary

all: run

.PHONY: all run clean

run: os-img
	qemu-system-x86_64 -fda os-img

os-img: build/boot.bin build/kernel.bin
	cat build/boot.bin build/kernel.bin > os-img

# Link the kernel binary
build/kernel.bin: $(ENTRY_OBJECT) $(C_OBJECTS)
	x86_64-elf-ld $(LDFLAGS) -o $@ $^

# Compile C source files using the CFLAGS
build/%.o: %.c
	@mkdir -p $(dir $@)
	x86_64-elf-gcc $(CFLAGS) $< -o $@

# Assemble the 16-bit bootloader
build/boot.bin: boot/boot.asm utils/*.asm
	@mkdir -p build
	nasm -f bin boot/boot.asm -o build/boot.bin

# Assemble the 32-bit kernel entry point
$(ENTRY_OBJECT): kernel/kernel_entry.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

# Clean up build artifacts
clean:
	rm -rf build os-img
