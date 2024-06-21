#include "vga_buffer.h"
#include "scheduler.h"
#include "colors.h"
#include "heap.h"
#include "paging.h"
#include "exceptions.h"

void systemroot();

void kernel_main() {
    heap_init();
    paging_init();
    //process_init();
    //scheduler_init();
    vga_init();
    set_vga_color_code(new_color_code(Black, LightGreen));
    println("running square-kernel");
    //int teste_alloc = kalloc(sizeof(int));
    //printi(teste_alloc);
    //setDefaultColor();
    process_create((int*)&systemroot);
    //asm("int $49");
    
    while(1);
}

void systemroot()
{
    println("\nrunning systemroot -->");
    while(1);
}

void interrupt_handler(int interrupt_number) {
  if(interrupt_number <= 31) {
    exception_handler(interrupt_number);
    return;
  }
  switch (interrupt_number)
  {
  case 49:
    println("KILL PROCESS INTERRUPT");
    break;
  
  default:
    println("CPU INTERRUPT NOT IN RANGE");
    break;
  }
}
