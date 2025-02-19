#include <memory/paging.h>
#include <memory/memory_manager.h>
#include <memory/page_allocator.h>
#include <video/vga_buffer.h>
#include "stdint.h"
#include "stddef.h"
#include "string.h"


uintptr_t *page_directory;

void flush_tlb() {
    uintptr_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));
    asm volatile("mov %0, %%cr3" :: "r"(cr3));
}

void enable_paging(uintptr_t page_directory) {
    uintptr_t cr0;

    // Lê o valor atual de cr0
    asm volatile("mov %%cr0, %0" : "=r"(cr0));  // O operando %0 deve ser um registrador de 32 bits.

    // Ativa a paginação (bit 31)
    cr0 |= 0x80000000;  // Define o bit 31 de cr0 para ativar a paginação

    // Atualiza cr0 com o novo valor
    asm volatile("mov %%cr0, %0" :: "r"(cr0));  // O operando %0 deve ser um registrador de 32 bits

    // Carrega o diretório de páginas em cr3
    asm volatile("mov %%cr3, %0" :: "r"(page_directory));  // A instrução mov deve ter a sintaxe correta

    println("MMU: Paging Enabled!");  // Exibe uma mensagem indicando que a paginação foi ativada
}


int create_page_entry(int base_address, char present, char writable, 
        char privilege_level, char cache_enabled, 
        char write_through_cache, char accessed, 
        char page_size, char dirty) {
    int entry = (base_address & ~0xFFF); // Ensure 4 KB alignment
    entry |= present;
    entry |= writable << 1;
    entry |= privilege_level << 2;
    entry |= write_through_cache << 3;
    entry |= cache_enabled << 4;
    entry |= accessed << 5;
    entry |= dirty << 6;
    entry |= page_size << 7;
    return entry;
}

void paging_init() {
    unsigned int curr_page_frame = 0;
    init_page_allocator();

    page_directory = (uintptr_t*)alloc_page();

    if(page_directory == NULL) {
        errorPrint("Error: Unable to allocate page directory!");
        int c = debug_pages();
        printi((int)page_directory);
        return;
    }

    memset(page_directory, 0, 1024 * sizeof(uintptr_t));

    for(int currPDE = 0; currPDE < PDE_NUM; currPDE++) {
        uintptr_t *page_table = (uintptr_t*)alloc_page();
        if(page_table == NULL) {
            errorPrint("Error: Unable to allocate page table!");
            return;
        }
        memset(page_table, 0, 1024 * sizeof(uintptr_t));

        for(int currPTE = 0; currPTE < PTE_NUM; currPTE++) {
            page_table[currPTE] = 
            create_page_entry(curr_page_frame * PAGE_SIZE, 1, 0, 0, 1, 1, 0, 0, 0);
        }
        page_directory[currPDE] =
            create_page_entry((uintptr_t)page_table, 1, 0, 0, 1, 1, 0, 0, 0);
    }

    uintptr_t physical_page_directory = virtual_to_physical((uintptr_t)page_directory);
    print("Page Directory: ");
    printi((int)page_directory);
    print("\nPhysical Page Directory: ");
    printi((int)physical_page_directory);
    print("\n");
    //printi((int)physical_page_directory);

    enable_paging(physical_page_directory);
}