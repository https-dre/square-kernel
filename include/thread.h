#include <stdint.h>

// Definindo o descritor de TSS
typedef struct {
    uint32_t prev_task_link;    // Link para a tarefa anterior
    uint32_t esp0;              // Ponteiro para a pilha no modo kernel
    uint32_t ss0;               // Segmento de pilha no modo kernel
    uint32_t esp1;              // Ponteiro para a pilha do nível 1 (opcional)
    uint32_t ss1;               // Segmento de pilha do nível 1 (opcional)
    uint32_t esp2;              // Ponteiro para a pilha do nível 2 (opcional)
    uint32_t ss2;               // Segmento de pilha do nível 2 (opcional)
    uint32_t cr3;               // CR3 (para a tabela de páginas)
    uint32_t eip;               // Ponteiro para a instrução
    uint32_t eflags;            // Flags do processador
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi; // Registradores
    uint32_t es, cs, ss, ds, fs, gs; // Segmentos
    uint32_t ldtr;              // Local Descriptor Table Register (opcional)
    uint32_t iomap_base;        // Endereço base para o mapa de I/O (opcional)
} __attribute__((packed)) tss_t;

extern tss_t tss;

extern void init_tss();