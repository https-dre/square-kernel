#include <thread.h>
#include <stdint.h>
#include <stddef.h>

tss_t tss = {};

void init_tss() {
    // Configura a estrutura TSS com valores padrão
    tss.prev_task_link = 0;      // Nenhuma tarefa anterior
    tss.esp0 = 0x0;              // Ponteiro de pilha do modo kernel
    tss.ss0 = 0x10;              // SS do modo kernel (segmento de dados do kernel)
    tss.esp1 = 0;                // Não usamos esp1 e ss1 (opcional)
    tss.ss1 = 0;
    tss.esp2 = 0;                // Não usamos esp2 e ss2 (opcional)
    tss.ss2 = 0;
    tss.cr3 = 0;                 // CR3 (para a tabela de páginas, 0 por padrão)
    tss.eip = 0;                 // Ponteiro de instrução (inicialmente 0)
    tss.eflags = 0x202;          // Flags de interrupção (habilita interrupções)
    tss.eax = 0;                 // Registradores zerados
    tss.ecx = 0;
    tss.edx = 0;
    tss.ebx = 0;
    tss.esp = 0;
    tss.ebp = 0;
    tss.esi = 0;
    tss.edi = 0;

    // Segmentos
    tss.es = 0x10;               // Segmento de dados do kernel
    tss.cs = 0x08;               // Segmento de código do kernel
    tss.ss = 0x10;               // Segmento de dados do kernel
    tss.ds = 0x10;               // Segmento de dados do kernel
    tss.fs = 0x10;               // Segmento de dados do kernel
    tss.gs = 0x10;               // Segmento de dados do kernel

    tss.ldtr = 0;                // Tabela de segmentos local (não usada)
    tss.iomap_base = sizeof(tss); // Endereço base do mapa de I/O
}