all: run

.PHONY: all run clean

run: build/boot.bin
	qemu-system-i386 -fda build/boot.bin

build/boot.bin:boot/boot.asm
	@mkdir -p build
	nasm -f bin boot/boot.asm -o build/boot.bin

clean:
	rm -r build
