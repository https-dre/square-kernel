#include "heap.h"
#include "vga_buffer.h"

Block* heap_base = (Block*)HEAP_START;
Block* last_block = NULL;
#define KB 1024

void heap_init() {
    heap_base->size = 8;
    heap_base->next = NULL;
    heap_base->free = 1;
    last_block = heap_base;
    println("Memory: Heap Initialized!");
}

int push_block(int size) {
    if (last_block == NULL) {
        errorPrint("Erro: heap não inicializada corretamente!");
        return -1;
    }

    // Calcula o endereço do novo bloco
    Block* new_block = (Block*)((unsigned int)last_block + sizeof(Block) + last_block->size);

    // Verifica se o novo bloco está dentro do espaço de heap esperado
    if ((unsigned int)new_block + sizeof(Block) + size > HEAP_LIMIT) {
        errorPrint("Erro: heap cheia!");
        return -1;
    }

    // Inicializa o novo bloco
    new_block->size = size;
    new_block->free = 1;
    new_block->next = NULL;

    // Atualiza o ponteiro do último bloco
    last_block->next = new_block;
    last_block = new_block;
    return (int)new_block;
}

int kalloc(int bytes) {
    Block* current = heap_base;

    while (current->next != NULL) {
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
            return (int)current + sizeof(Block);
        }
        current = current->next;
    }

    // Aqui não há blocos disponíveis. Ou seja, devemos criar um
    int newBlockAddr = push_block(bytes);

    if(newBlockAddr == -1) {
        errorPrint("Erro ao alocar memória, falha ao criar novo bloco");
        return -1;
    }
    Block* newBlock = (Block*)newBlockAddr;
    newBlock->free = 0;
    return (int)newBlock + sizeof(Block);

    if(current == NULL) {
        println("Erro ao alocar memória");
    }

    if(current->next == NULL) {
        errorPrint("Sem blocos disponiveis");
    }

    return -1;
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