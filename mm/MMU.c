#include <memory_manager.h>
#include <paging.h>
#include <liballoc.h>
#include <page_allocator.h>
#include <vga_buffer.h>

int init_mmu() {
    paging_init();
    uintptr_t heap_start = alloc_page();
    if(heap_start == 0) {
        errorPrint("Failed to allocate heap start page");
    } else {
        heap_init(heap_start);
    }

    #ifdef DEBUG
    uint16_t *number = (uint16_t*)kmalloc(sizeof(uint16_t));
    *number = 18;
    kfree((uintptr_t)number);
    #endif
}