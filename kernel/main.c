#include "vga_buffer.h"
#include "scheduler.h"
#include "colors.h"
#include "heap.h"
#include "paging.h"

void processA();
void processB();

void kernel_main() {
    vga_init();
    println("VGA Initialized");
    heap_init();
    paging_init();  
    /* process_init();
    scheduler_init();
    process_create(&processA);
    process_create(&processB); */

    set_vga_color_code(new_color_code(Black, LightGreen));
    println("Kernel Ready!");
    setDefaultColor();

    while( 1 );
}

void processA()
{
    while ( 1 )
        asm( "mov $11, %eax" );
}

void processB()
{
    while ( 1 )
        asm( "mov $22, %eax" );
}

void interrupt_handler(int interrupt_number) {
    if(interrupt_number == 32) {
        print(".");
    }
}