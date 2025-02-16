#include "vga_buffer.h"
#include "colors.h"
#include "heap.h"
#include "paging.h"

void kernel_main() {
    vga_init();
    set_vga_color_code(new_color_code(Black, LightGreen));
    println("Kernel Ready!");
    setDefaultColor();

    while( 1 );
}