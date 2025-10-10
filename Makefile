all: run

.PHONY: all run clean

run: os-img
	qemu-system-x86_64 -fda os-img

os-img: build/boot.bin build/kernel.bin
	cat build/boot.bin build/kernel.bin > os-img

build/boot.bin:boot/boot.asm kernel/kernel_entry.asm utils/* 
	@mkdir -p build
	nasm -f bin boot/boot.asm -o build/boot.bin
	nasm -f elf64 kernel/kernel_entry.asm -o build/kernel_entry.o

build/kernel.bin: kernel/kernel.c
	@mkdir -p build
	x86_64-elf-gcc -ffreestanding -c kernel/kernel.c -o build/kernel.o
	x86_64-elf-ld -o build/kernel.bin -Ttext 0x1000 build/kernel_entry.o build/kernel.o  --oformat binary

clean:
	rm -r build
	rm os-img
