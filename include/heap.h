#define HEAP_START  0x100000
#define HEAP_LIMIT  0x200000  // limite para a heap
#define NULL ((void*)0)

typedef struct Block {
    int size;         // Tamanho do bloco (excluindo a estrutura)
    struct Block* next;  // Próximo bloco na lista
    int free;            // 1 = Livre, 0 = Ocupado
} Block;

extern Block* heap_base;
void heap_init();
void* kalloc(int);