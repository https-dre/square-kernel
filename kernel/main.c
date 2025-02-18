#include "vga_buffer.h"
#include "colors.h"
#include <memory_manager.h>
#include <gdt.h>
#include <idt.h>

#ifndef DEBUG
//#define DEBUG
#endif

void kernel_main() {
    vga_init();
    init_gdt();
    set_idt();
    init_mmu();
    set_vga_color_code(color_code(Black, LightGreen));
    println("Kernel Ready!");

    while( 1 );
}