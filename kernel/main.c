#include <video/vga_buffer.h>
#include <video/colors.h>
#include <memory/memory_manager.h>
#include <idt.h>
#include <thread.h>

#ifndef DEBUG
//#define DEBUG
#endif

void kernel_main() {
    vga_init();
    println("64-Bit Mode");
    println("Paging initialized!");
    println("GDT Loaded!");
    init_mmu();

    set_vga_color_code(color_code(Black, LightGreen));
    println("Kernel Ready!");
    
    while( 1 );
}
