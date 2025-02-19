#include <memory/memory_manager.h>
#include <memory/liballoc.h>
#include <memory/page_allocator.h>
#include <video/vga_buffer.h>

int init_mmu() {
    uintptr_t heap_start = (uintptr_t)alloc_page();
    if(heap_start == NULL) {
        errorPrint("HEAP_START == NULL");
        println("MMU error!");
        return -1;
    }
    heap_init(heap_start);
    println("Memory Ready!");
}