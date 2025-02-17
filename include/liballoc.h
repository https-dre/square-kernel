#ifndef HEAP_H
#define HEAP_H

#include "stddef.h"
#include "stdint.h"

typedef struct {
    unsigned int magic;    // Magic number for error checking and identification.
    unsigned char is_hole; // 1 if this is a hole. 0 if this is a block.
    size_t size;           // Size of the block, including the end footer.
} header_t;

typedef struct {
    unsigned int magic;    // Magic number, same as in header_t.
    header_t *header;      // Pointer to the block header.
} footer_t;

typedef struct {
    void *start_address;   // The start of our allocated space.
    size_t size;           // The size of our allocated space.
} heap_t;

heap_t *create_heap(uintptr_t start, uintptr_t end, uintptr_t max);
void *malloc(size_t size, heap_t *heap);
void free(void *p, heap_t *heap);
void *realloc(void *p, size_t size, heap_t *heap);

#endif