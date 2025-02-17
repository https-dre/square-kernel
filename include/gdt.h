#ifndef GDT_H
#define GDT_H

struct gdt_entry {
    unsigned short limit_low;   // Limite (16 bits)
    unsigned short base_low;    // Base (16 bits)
    unsigned char  base_middle; // Base (8 bits)
    unsigned char  access;      // Byte de acesso
    unsigned char  granularity; // Granularidade e bits do limite
    unsigned char  base_high;   // Base (8 bits)
} __attribute__((packed)); // evita que o compilador adicione padding (preenchimento extra).

struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

#define GDT_SIZE 5 // Criamos três entradas na GDT, null, códigos e dados

void gdt_set_entry(int index, unsigned int base, unsigned int limit,
    unsigned char access, unsigned char granularity);
void init_gdt();

#endif