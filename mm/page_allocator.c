#include <memory/page_allocator.h>
#include "stdint.h"
#include "stddef.h"
#include <video/vga_buffer.h>
#include <memory/memory_manager.h>

static uint8_t *bitmap = (uint8_t*)0xffff;

void* virtual_to_physical(void* virtual_address) {
    uintptr_t v_addr = (uintptr_t)virtual_address;

    // Verifica se está dentro da faixa de endereços válidos
    if (v_addr < BASE_VIRTUAL) {
        return (void*)v_addr;  // Endereço já é físico
    }

    return (void*)(v_addr - BASE_VIRTUAL + BASE_FISICA);
}

void init_page_allocator() {
    // Marca todas as páginas como livres
    for (size_t i = 0; i < sizeof(bitmap); i++) {
        bitmap[i] = 0;
    }
    alloc_page();
}

static void set_bit(size_t index) {
    bitmap[index / 8] |= (1 << (index % 8));
}

static void clear_bit(size_t index) {
    bitmap[index / 8] &= ~(1 << (index % 8));
}

static int is_bit_set(size_t index) {
    return bitmap[index / 8] & (1 << (index % 8));
}

void* alloc_page() {
    for (size_t i = 0; i < TOTAL_PAGES; i++) {
        if (!is_bit_set(i)) {
            set_bit(i);
            return (void*)(i * PAGE_SIZE);
        }
    }
    // Nenhuma página livre disponível
    errorPrint("Error: No free pages available!");
    return NULL;
}

int debug_pages() {
    int count = 0;
    for (size_t i = 0; i < TOTAL_PAGES; i++) {
        if (is_bit_set(i)) {
            count++;
        } 
    }
    return count;
}

void free_page(void* page) {
    size_t index = ((uintptr_t)page) / PAGE_SIZE;
    clear_bit(index);
}