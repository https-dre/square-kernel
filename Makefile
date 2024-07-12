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
OUTFILE = square-kernel.img
LINKER = linker.ld
LINKER_FILES = $(BUILD_DIR)/starter.o $(KERNEL_OBJECT) $(BUILD_DIR)/vga_buffer.elf $(BUILD_DIR)/process.elf $(BUILD_DIR)/scheduler.elf $(BUILD_DIR)/heap.elf $(BUILD_DIR)/paging.elf $(BUILD_DIR)/ata.elf $(BUILD_DIR)/exceptions.elf
RELEASE_VMDK = square-vmware.vmdk
DEBUG_FLAGS = -g $(KERNEL_FLAGS)

.PHONY: build clean release

build: clean $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	$(ASM) -f bin $(BOOTSTRAP_FILE) -o $(BUILD_DIR)/bootstrap.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o $(BUILD_DIR)/starter.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILES) -o $(KERNEL_OBJECT)
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/vga/vga_buffer.c -o $(BUILD_DIR)/vga_buffer.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/process/process.c -o $(BUILD_DIR)/process.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/sch/scheduler.c -o $(BUILD_DIR)/scheduler.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/mm/heap/heap.c -o $(BUILD_DIR)/heap.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/mm/paging/paging.c -o $(BUILD_DIR)/paging.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/fs/ata.c -o $(BUILD_DIR)/ata.elf
	$(CC) $(KERNEL_FLAGS) $(SRC_DIR)/kernel/exceptions.c -o $(BUILD_DIR)/exceptions.elf

	ld -melf_i386 -T $(LINKER) $(LINKER_FILES) -o $(BUILD_DIR)/square-kernel.elf 
	objcopy -O binary $(BUILD_DIR)/square-kernel.elf $(BUILD_DIR)/square-kernel.bin

	dd if=$(BUILD_DIR)/bootstrap.o of=$(OUTFILE)
	dd seek=1 conv=sync if=$(BUILD_DIR)/square-kernel.bin of=$(OUTFILE) bs=512 count=8
	dd seek=9 conv=sync if=/dev/zero of=$(OUTFILE) bs=512 count=2046

run: $(OUTFILE)
	qemu-system-x86_64 -s -no-reboot -d int -drive format=raw,file=$(OUTFILE)

debug: clean debug_build
	qemu-system-x86_64 -s -S -drive format=raw,file=$(OUTFILE)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(OUTFILE)

release_vmware: $(OUTFILE)
	dd seek=9 conv=sync if=/dev/zero of=$(OUTFILE) bs=512 count=3070
	qemu-img convert -f raw -O vmdk $(OUTFILE) $(RELEASE_VMDK)

debug_build: build clean $(BOOTSTRAP_FILE) $(KERNEL_FILES)
	$(ASM) -f bin $(BOOTSTRAP_FILE) -o $(BUILD_DIR)/bootstrap.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o $(BUILD_DIR)/starter.o
	$(CC) $(DEBUG_FLAGS) $(KERNEL_FILES) -o $(KERNEL_OBJECT)
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/vga/vga_buffer.c -o $(BUILD_DIR)/vga_buffer.elf
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/process/process.c -o $(BUILD_DIR)/process.elf
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/sch/scheduler.c -o $(BUILD_DIR)/scheduler.elf
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/mm/heap/heap.c -o $(BUILD_DIR)/heap.elf
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/mm/paging/paging.c -o $(BUILD_DIR)/paging.elf
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/fs/ata.c -o $(BUILD_DIR)/ata.elf
	$(CC) $(DEBUG_FLAGS) $(SRC_DIR)/kernel/exceptions.c -o $(BUILD_DIR)/exceptions.elf

	ld -melf_i386 -T $(LINKER) $(LINKER_FILES) -o $(BUILD_DIR)/square-kernel.elf 
	objcopy -O binary $(BUILD_DIR)/square-kernel.elf $(BUILD_DIR)/square-kernel.bin

	dd if=$(BUILD_DIR)/bootstrap.o of=$(OUTFILE)
	dd seek=1 conv=sync if=$(BUILD_DIR)/square-kernel.bin of=$(OUTFILE) bs=512 count=8
	dd seek=9 conv=sync if=/dev/zero of=$(OUTFILE) bs=512 count=2046
