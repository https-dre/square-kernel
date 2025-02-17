#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <stddef.h>

#define PAGE_SIZE 4096
#define TOTAL_PAGES 1024

void init_page_allocator();
void* alloc_page();
void free_page(void* page);
int debug_pages();

#endif // MEMORY_ALLOCATOR_H