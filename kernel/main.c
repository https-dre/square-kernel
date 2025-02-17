#include "vga_buffer.h"
#include "colors.h"
#include "paging.h"

void kernel_main() {
    vga_init();
    paging_init();
    set_vga_color_code(color_code(Black, LightGreen));
    println("Kernel Ready!");

    while( 1 );
}