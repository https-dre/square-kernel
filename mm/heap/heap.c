#include "heap.h"
#include "vga_buffer.h"

Block* heap_base = (Block*)HEAP_START;
#define KB 1024

void heap_init() {
    heap_base->size = 8;
    heap_base->next = NULL;
    heap_base->free = 1;
    println("Heap initialized");
}

void* kalloc(int bytes) {
    Block* current = heap_base;

    while (current) {
        if (current->free == 1 && current->size >= bytes) {
            // Dividir o bloco se sobrar espaço suficiente para um novo bloco
            if (current->size > bytes + sizeof(Block)) {
                Block* new_block = (Block*)((unsigned int)current + sizeof(Block) + bytes);
                new_block->size = current->size - bytes - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;
                current->next = new_block;
            }

            current->size = bytes;
            current->free = 0;
            println("Mais memória alocada!");
            return (void*)((unsigned int)current + sizeof(Block));
        }
        current = current->next;
    }

    return (void*)-1;
}

int kfree(void* ptr) {
    if (!ptr) return -1;

    Block* block = (Block*)((unsigned int)ptr - sizeof(Block));
    block->free = 1;

    if (block->next && block->next->free) {
        block->size += block->next->size + sizeof(Block);
        block->next = block->next->next;
    }
}