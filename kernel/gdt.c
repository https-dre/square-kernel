#include "gdt.h"
#include <vga_buffer.h>
#include <thread.h>
#include <stddef.h>

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

// Função para adicionar a entrada do TSS na GDT
void gdt_set_tss(int index, unsigned int base, unsigned int limit) {
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].granularity = 0x89;  // Granularidade do TSS (valores típicos para TSS)
    gdt[index].access = 0xE9;        // Acesso ao TSS (0xE9 é o valor comum para TSS)
}

void load_gdt() {
    asm("cli");
    asm volatile("lgdt %0" : : "m" (gp)); // Carrega a GDT
}

void load_tss(uint16_t tss_selector) {
    __asm__ volatile("ltr %0" : : "r" (tss_selector));
}

void init_gdt(void) {
    init_tss();
    gdt_set_entry(0, 0, 0, 0, 0);  // Entrada nula
    gdt_set_entry(1, 0xffff, 0x0000, 0x9A, 0xCF); // Kernel Code Segment
    gdt_set_entry(2, 0xffff, 0x0000, 0x92, 0xCF); // Kernel Data Segment
    gdt_set_entry(3, 0xffff, 0x0000, 0xFA, 0xCF); // User Space Code Segment
    gdt_set_entry(4, 0xffff, 0x0000, 0xF2, 0xCF); // User Space Data Segment

    uint32_t tss_base = (uint32_t)&tss;  // Base da estrutura TSS
    uint32_t tss_limit = sizeof(tss_t) - 1;  // Limite do TSS (tamanho da estrutura)

    gdt_set_tss(5, tss_base, tss_limit);  // Adiciona o TSS na entrada 5 da GDT
 
    gp.limit = (sizeof(gdt) - 1);  // O limite da GDT é o tamanho da GDT - 1
    gp.base = (unsigned long)&gdt; // O endereço base da GDT é o endereço de 'gdt'

    load_gdt();
    println("->GDT Loaded!");
}
