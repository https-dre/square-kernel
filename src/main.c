#include "vga_buffer.h"
#include "scheduler.h"

void kernel_main() {
    vga_init();
    process_init();
    scheduler_init();
    print("Welcome to square-kernel!\n");
    set_vga_color_code(10);
    print("We are in protected mode!\n");

    while( 1 );
}

void interrupt_handler(int interrupt_number) {
    if(interrupt_number == 32) {
        print(".");
    }
}
