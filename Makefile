ASM = nasm
CC = gcc
SRC_DIR = .
ASM_DIR = $(SRC_DIR)/arch/x86
BUILD_DIR = build
BOOTSTRAP_FILE = $(ASM_DIR)/init/bootstrap.asm
INIT_KERNEL_FILES = $(ASM_DIR)/init/starter.asm 
MULTIBOOT_FILE = $(ASM_DIR)/init/multiboot_header.asm
KERNEL_FILES = $(SRC_DIR)/kernel/main.c
INCLUDE_DIR = $(SRC_DIR)/include
KERNEL_FLAGS = -g -Wno-error=incompatible-pointer-types  -Wno-error=int-conversion -fno-stack-protector -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie -Wint-conversion $(addprefix -I, $(INCLUDE_DIR))
KERNEL_OBJECT = $(BUILD_DIR)/kernel.elf
OUTFILE = square-kernel.iso
LINKER = linker.ld
LINKER_FILES = $(BUILD_DIR)/multiboot_header.o $(BUILD_DIR)/starter.o $(BUILD_DIR)/gdt.o $(BUILD_DIR)/idt.o $(KERNEL_OBJECT) $(BUILD_DIR)/vga_buffer.elf 
DEBUG_FLAGS = -g

build_: $(INIT_KERNEL_FILES) $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	$(ASM) -f elf32 -g $(INIT_KERNEL_FILES) -o $(BUILD_DIR)/starter.o
	$(ASM) -f elf32 -g $(MULTIBOOT_FILE) -o $(BUILD_DIR)/multiboot_header.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILES) -o $(KERNEL_OBJECT)
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/kernel/gdt.c -o $(BUILD_DIR)/gdt.o
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/kernel/idt.c -o $(BUILD_DIR)/idt.o
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/vga/vga_buffer.c -o $(BUILD_DIR)/vga_buffer.elf
	
	ld -melf_i386 --nmagic --output=$(BUILD_DIR)/square-kernel.elf --script=linker.ld $(LINKER_FILES)
	objcopy -O binary $(BUILD_DIR)/square-kernel.elf $(BUILD_DIR)/square-kernel.bin

	cp build/square-kernel.elf root_iso/boot/

	grub-mkrescue -o square-kernel.iso root_iso

run: $(OUTFILE)
	qemu-system-i386 -drive format=raw,file=$(OUTFILE)

debug: $(OUTFILE)
	qemu-system-i386 -drive format=raw,file=$(OUTFILE) -s -S -d int

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OUTFILE)