#include "process.h"
#include "heap.h"
#include "vga_buffer.h"

int processes_count, curr_pid;
process_t *processes[15];

void process_init()
{
    processes_count = 0;
    curr_pid = 0;
    println("Process Manager Initialized");
}

void process_create(int *base_address) {
    process_t *process = (process_t *)kalloc(sizeof(process_t));
    if((int)process == -1) {
        errorPrint("Error allocating memory for process");
        return;
    }
    process->pid = curr_pid++;
    
    process->context.eax = 0;
    process->context.ecx = 0;
    process->context.edx = 0;
    process->context.ebx = 0;
    process->context.esp = 0;
    process->context.ebp = 0;
    process->context.esi = 0;
    process->context.edi = 0;
    process->context.eip = (int)base_address;
    
    process->state = READY;
    process->base_address = base_address;
    
    processes[ process->pid ] = process;
    
    processes_count++;
}