#ifndef IDT_H
#define IDT_H

// Definição do descritor da IDT
typedef struct {
    unsigned short base_low;       // Parte baixa do endereço da função
    unsigned short sel;            // Seletor de segmento
    unsigned char  always0;        // Sempre 0
    unsigned char  flags;          // Flags (0x8E para interrupções do anel 0)
    unsigned short base_high;      // Parte alta do endereço da função
} __attribute__((packed)) idt_entry;

#define NUM_INTERRUPTS 256

extern idt_entry idt[256];

typedef struct {
    unsigned short limit;  // Tamanho da IDT
    unsigned int base;     // Endereço base da IDT
} __attribute__((packed)) idt_ptr_t;

extern idt_ptr_t idt_ptr;

void set_idt_gate(int num, unsigned int base, unsigned short sel, unsigned char flags);
void set_idt();

#endif