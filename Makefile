ASM = nasm
CC = gcc
SRC_DIR = .
ASM_DIR = $(SRC_DIR)/arch/x86
BUILD_DIR = build
BOOTSTRAP_FILE = $(ASM_DIR)/init/bootstrap.asm
INIT_KERNEL_FILES = $(ASM_DIR)/init/starter.asm
KERNEL_FILES = $(SRC_DIR)/kernel/main.c
INCLUDE_DIR = $(SRC_DIR)/include
KERNEL_FLAGS = -Wno-error=incompatible-pointer-types  -Wno-error=int-conversion -fno-stack-protector -m32 -c -ffreestanding -fno-asynchronous-unwind-tables -fno-pie -Wint-conversion $(addprefix -I, $(INCLUDE_DIR))
KERNEL_OBJECT = $(BUILD_DIR)/kernel.elf
OUTFILE = square-kernel.iso
LINKER = linker.ld
LINKER_FILES = $(BUILD_DIR)/starter.o $(KERNEL_OBJECT) $(BUILD_DIR)/vga_buffer.elf $(BUILD_DIR)/process.elf $(BUILD_DIR)/scheduler.elf $(BUILD_DIR)/heap.elf $(BUILD_DIR)/paging.elf $(BUILD_DIR)/ata.elf

build_: $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	$(ASM) -f bin $(BOOTSTRAP_FILE) -o $(BUILD_DIR)/bootstrap.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o $(BUILD_DIR)/starter.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILES) -o $(KERNEL_OBJECT)
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/vga/vga_buffer.c -o $(BUILD_DIR)/vga_buffer.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/process/process.c -o $(BUILD_DIR)/process.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/sch/scheduler.c -o $(BUILD_DIR)/scheduler.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/mm/heap/heap.c -o $(BUILD_DIR)/heap.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/mm/paging/paging.c -o $(BUILD_DIR)/paging.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/fs/ata.c -o $(BUILD_DIR)/ata.elf

	ld -melf_i386 -T $(LINKER) $(LINKER_FILES) -o $(BUILD_DIR)/square-kernel.elf 
	objcopy -O binary $(BUILD_DIR)/square-kernel.elf $(BUILD_DIR)/square-kernel.bin

	dd if=$(BUILD_DIR)/bootstrap.o of=$(OUTFILE)
	dd seek=1 conv=sync if=$(BUILD_DIR)/square-kernel.bin of=$(OUTFILE) bs=512 count=8
	dd seek=9 conv=sync if=/dev/zero of=$(OUTFILE) bs=512 count=2046

run: $(OUTFILE)
	qemu-system-x86_64 -s -drive format=raw,file=$(OUTFILE)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OUTFILE)
