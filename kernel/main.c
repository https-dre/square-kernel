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
    process_create((int*)&systemroot);
    while(1);
}

void systemroot()
{
    println("\nrunning systemroot -->");
    println("   square-kernel :)");
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
