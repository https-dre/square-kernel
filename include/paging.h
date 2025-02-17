#define PDE_NUM 3 // page directory entries
#define PTE_NUM 1024
#include "stdint.h"

extern uintptr_t  *page_directory;

void paging_init();
int create_page_entry(int, char, char, char, char, char, char, char, char);