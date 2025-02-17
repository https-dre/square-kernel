#include "gdt.h"

// gdt.c
struct gdt_entry gdt[GDT_SIZE];  // Definição real de gdt
struct gdt_ptr gp;               // Definição real de gp

void gdt_set_entry(int index, unsigned int base, unsigned int limit,
    unsigned char access, unsigned char granularity)
{
    gdt[index].base_low    = base & 0xFFFF;        // Parte baixa da base
    gdt[index].base_middle = (base >> 16) & 0xFF; // Parte média da base
    gdt[index].base_high   = (base >> 24) & 0xFF; // Parte alta da base

    gdt[index].limit_low   = limit & 0xFFFF;       // Parte baixa do limite
    gdt[index].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0); // Parte alta do limite + flags

    gdt[index].access      = access; // Permissões e tipo do segmento
}

void load_gdt() {
    asm("cli");
    asm volatile("lgdt %0" : : "m" (gp)); // Carrega a GDT
}

void init_gdt(void) {
   
    gdt_set_entry(0, 0, 0, 0, 0);  // Entrada nula
    gdt_set_entry(1, 0xffff, 0x0000, 0x9A, 0xCF); // Kernel Code Segment
    gdt_set_entry(2, 0xffff, 0x0000, 0x92, 0xCF); // Kernel Data Segment
    gdt_set_entry(3, 0xffff, 0x0000, 0xFA, 0xCF); // User Space Code Segment
    gdt_set_entry(4, 0xffff, 0x0000, 0xF2, 0xCF); // User Space Data Segment
 
    gp.limit = (sizeof(gdt) - 1);  // O limite da GDT é o tamanho da GDT - 1
    gp.base = (unsigned long)&gdt; // O endereço base da GDT é o endereço de 'gdt'

    load_gdt();
}
