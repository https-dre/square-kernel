#include "heap.h"

unsigned int heap_base;
int blocks_count;
int MAX_HEAP_SIZE = 3000 * 4096;
int HEAP_START_ADDRESS = 0x100000;
block_t* init_region = (block_t*)HEAP_START_ADDRESS;
block_t *head;
block_t *last_block;

block_t* map_heap() {
  // Inicializa o primeiro bloco
  init_region->start_address = HEAP_START_ADDRESS;
  init_region->state = FREE;
  init_region->size = 16;
  init_region->has_next = 0; 
  heap_base = init_region->start_address + init_region->size;
  return init_region;
}

void heap_init()
{
    heap_base = HEAP_START_ADDRESS;
    head = map_heap();
}

// Mapeia a memória Heap por uma Grande Linked List
block_t* push_block(unsigned int address, unsigned int size, block_t *heap_head) {
  // Cria novo bloco
  block_t *new_block = (block_t*)address;
  new_block->start_address = address;
  new_block->size = size;
  new_block->state = FREE; 
  new_block->has_next = 0;

  block_t *curr = heap_head;
  while (curr->next != NULL) {
    curr = curr->next;  
  }

  // Agora curr aponta para o último bloco da lista
  curr->has_next = 1;
  curr->next = new_block;
  blocks_count++;

  return new_block;
}

/*
  .: Acessa membros de uma estrutura diretamente.
  ->: Acessa membros de uma estrutura através de um ponteiro para a estrutura.
*/

int* malloc(int bytes) {
  // Verifica se há memória suficiente disponível
  if (heap_base + bytes > HEAP_START_ADDRESS + MAX_HEAP_SIZE) {
    return NULL;
  }

  block_t *curr = head;

  while (curr != NULL) {
    if (curr->state == FREE && curr->size >= bytes) {
        curr->state = BUSY;
        return (int*)curr->start_address;
    }
    curr = curr->has_next ? curr->next : NULL; // Move para o próximo bloco, se houver
  }

  if(curr->state == FREE && curr->size >= bytes) {
    curr->state = BUSY;      
    return (int*)curr->start_address;
  }

  // Não encontrou um bloco livre e adequado, aloca um novo bloco
    
  block_t *new_block = push_block(curr->start_address+curr->size, bytes, head);
  new_block->state = BUSY;
  return (int*)new_block->start_address;
}

void kfree(int address) {
  if(address <= HEAP_START_ADDRESS) 
  {
    return;
  }

  block_t *curr = head;
  while (curr->has_next == 1) {
    if((int)curr->start_address == address) {
      curr->state = FREE;
      return;
    }
    curr = curr->next;
  }

  asm("int $0x3");  // Gera um erro de segmentação se o endereço não for válido
}
