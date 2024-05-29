ASM = nasm
CC = gcc
SRC_DIR = src
ASM_DIR = $(SRC_DIR)/asm
TARGET_DIR = target
BOOSTRAP_FILE = $(ASM_DIR)/bootstrap.asm
INIT_KERNEL_FILES= $(ASM_DIR)/starter.asm
C_DIR = $(SRC_DIR)/c
KERNEL_FILES = $(C_DIR)/main.c
KERNEL_FLAGS = -Wall -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie
KERNEL_OBJECT = -o $(TARGET_DIR)/kernel.elf
OUTFILE = kernel.img
LINKER = linker.ld

build: $(BOOSTRAP_FILE) $(KERNEL_FILES)
	$(ASM) -f bin $(BOOSTRAP_FILE) - $(TARGET_DIR)/bootstrap.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o $(TARGET_DIR)/starter.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILES) $(KERNEL_OBJECT)

	ld -melf_i386 -Tlinker.ld $(TARGET_DIR)/starter.o $(KERNEL_OBJECT) 539kernel.bin
	dd if=$(TARGET_DIR)/bootstrap.o of=$(OUTFILE)
	dd seek=1 conv=sync if=539kernel.bin of=$(OUTFILE) bs=512 count=5
	dd seek=6 conv=sync if=/dev/zero of=$(OUTFILE) bs=512 count=2046
	qemu-system-x86_64 -s $(OUTFILE)