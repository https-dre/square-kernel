#include "vga_buffer.h"
#include "scheduler.h"
#include "colors.h"
#include "heap.h"
#include "paging.h"

void processA();
void processB();

void kernel_main() {
    heap_init();
    paging_init();  
    vga_init();
    process_init();
    scheduler_init();
    set_vga_color_code(new_color_code(Black, LightGreen));
    println("running square-kernel");
    setDefaultColor();
    process_create((int*)&processA);
    process_create((int*)&processB);
    
    while( 1 );
}

void processA()
{
    println("Running Process A, adicionando 11 ao eax");

    while ( 1 )
        asm( "mov $11, %eax" );
}

void processB()
{
    println("Running Process B, adicionando 22 ao eax");

    while ( 1 )
        asm( "mov $22, %eax" );
}

void interrupt_handler(int interrupt_number) {
    if(interrupt_number == 32) {
        print(".");
    }
}
