#define PAGE_SIZE 4096
#define TOTAL_PAGES 1024

#define PDE_NUM 3
#define PTE_NUM 1024

#include "stdint.h"
#include "stddef.h"

extern uintptr_t  *page_directory;

#define PAGE_SIZE 4096
#define TOTAL_PAGES 1024

#define BASE_VIRTUAL 0x100000
#define BASE_FISICA  0x00000000  // Endereço real onde começa a memória física

#define HEAP_MAGIC 0x12345678
#define HEAP_MIN_SIZE 0x70000
#define HEAP_INDEX_SIZE 0x20000

extern int init_mmu();
