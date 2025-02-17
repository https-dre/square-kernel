#include "liballoc.h"
#include "stdint.h"
#include "stddef.h"

static void expand(uintptr_t new_size, heap_t *heap);
static uint32_t contract(uintptr_t new_size, heap_t *heap);
static int find_smallest_hole(uintptr_t size, uint8_t page_align, heap_t *heap);

heap_t *create_heap(uintptr_t start, uintptr_t end, uintptr_t max) {
    heap_t *heap = (heap_t *)start;
    heap->start_address = start + sizeof(heap_t);
    heap->size = (size_t)((uintptr_t)end - (uintptr_t)start);
    return heap;
}