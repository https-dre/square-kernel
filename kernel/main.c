#include <video/vga_buffer.h>
#include <video/colors.h>
#include <memory/memory_manager.h>
#include <thread.h>

#ifndef DEBUG
//#define DEBUG
#endif

extern void load_idt(void);

void kernel_main() {
    vga_init();
    println("64-Bit Mode");
    println("Paging initialized!");
    println("GDT Loaded!");
    load_idt();
    println("IDT Loaded!");
    init_mmu();

    set_vga_color_code(color_code(Black, LightGreen));
    println("Kernel Ready!");
    
    while( 1 );
}
