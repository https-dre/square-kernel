#include "idt.h"

idt_ptr_t idt_ptr;
idt_entry idt[256];

void set_idt_gate(int num, unsigned int base, unsigned short sel, unsigned char flags) {
    idt[num].base_low = base & 0xFFFF;          // Parte baixa do endereço
    idt[num].base_high = (base >> 16) & 0xFFFF; // Parte alta do endereço
    idt[num].sel = sel;                        // Seletor de segmento
    idt[num].always0 = 0;                      // Campo sempre 0
    idt[num].flags = flags;                    // Flags (0x8E)
}

void interrupt_handler(int interrupt) {
    asm("hlt");
}

void set_idt() {
    idt_ptr.limit = sizeof(idt_entry) * NUM_INTERRUPTS - 1;  // Tamanho da tabela
    idt_ptr.base = (unsigned int)&idt;                           // Endereço da base da IDT

    // Inicializa todas as entradas da IDT
    for (int i = 0; i < NUM_INTERRUPTS; i++) {
        set_idt_gate(i, (unsigned int)interrupt_handler, 0x08, 0x8E);
    }

    // Carregar a IDT utilizando a instrução 'lidt'
    asm("cli");
    __asm__ volatile("lidt (%0)" : : "r" (&idt_ptr));
    println("->IDT Loaded!");
}
