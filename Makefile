ASM = nasm
CC = gcc
SRC_DIR = src
ASM_DIR = $(SRC_DIR)/asm
BUILD_DIR = build
BOOTSTRAP_FILE = $(ASM_DIR)/bootstrap.asm
INIT_KERNEL_FILES = $(ASM_DIR)/starter.asm
KERNEL_FILES = $(SRC_DIR)/main.c
KERNEL_FLAGS = -Wall -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie -Wint-conversion
KERNEL_OBJECT = $(BUILD_DIR)/kernel.o
OUTFILE = square-kernel.iso
LINKER = linker.ld

build_and_run: $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	$(ASM) -f bin $(BOOTSTRAP_FILE) -o $(BUILD_DIR)/bootstrap.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o $(BUILD_DIR)/starter.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILES) -o $(KERNEL_OBJECT)

	ld -melf_i386 -T$(LINKER) $(BUILD_DIR)/starter.o $(KERNEL_OBJECT) -o $(BUILD_DIR)/square-kernel.elf
	objcopy -O binary $(BUILD_DIR)/square-kernel.elf $(BUILD_DIR)/square-kernel.bin

	dd if=$(BUILD_DIR)/bootstrap.o of=$(OUTFILE)
	dd seek=1 conv=sync if=$(BUILD_DIR)/square-kernel.bin of=$(OUTFILE) bs=512 count=5
	dd seek=6 conv=sync if=/dev/zero of=$(OUTFILE) bs=512 count=2046

	qemu-system-x86_64 -s -drive format=raw,file=$(OUTFILE)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OUTFILE)
