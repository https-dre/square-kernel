#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

void init_page_allocator();
void* alloc_page();
void free_page(void* page);
int debug_pages();

#endif