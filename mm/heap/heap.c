#include "heap.h"

unsigned int heap_base;
int blocks_count;
int MAX_HEAP_SIZE = 3000 * 4096;
int HEAP_START_ADDRESS = 0x100000;

block_t head;
block_t *last_block;

void map_heap() {
  // Inicializa o primeiro bloco
  head.start_address = heap_base;
  head.state = FREE;
  head.size = 16; // Define um tamanho inicial maior
  head.has_next = 0; 
  last_block = &head;
  heap_base += head.size;
}

void heap_init()
{
    heap_base = 0x100000;
    map_heap();
}

// Mapeia a memória Heap por uma Grande Linked List
block_t* push_block(unsigned int address, unsigned int size) {
  // Cria novo bloco
  block_t *new_block;
  new_block->start_address = address;
  new_block->size = size;
  new_block->state = FREE; 
  new_block->has_next = 0;

  block_t *curr = &head;
  while(curr->has_next == 1) {
    curr = curr->next;
  }

  // Agora curr aponta para o último bloco da lista
  curr->has_next = 1;
  curr->next = new_block;
  last_block = new_block; // Atualiza last_block
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
    return -1;
  }

  block_t *curr = &head;

  while(curr->has_next == 1) {
    if(curr->state == FREE && curr->size >= bytes) {
      // Encontrou um bloco livre e grande o suficiente
      curr->state = BUSY;
      return (int*)curr->start_address;
    }
      curr = curr->next;
    }

    
    if(curr->state == FREE && curr->size >= bytes) {
      curr->state = BUSY;
      return (int*)curr->start_address;
    }

    // Não encontrou um bloco livre e adequado, aloca um novo bloco
    unsigned int new_object_address = heap_base;
    heap_base += bytes+1;

    block_t *new_block = push_block(new_object_address, bytes);
    new_block->state = BUSY;

    return (int*)new_block->start_address;
}

int kalloc(int bytes) {
  unsigned int new_object_address = heap_base;
  heap_base += bytes;

  return new_object_address;
}

void kfree(int address) {
  if(address <= HEAP_START_ADDRESS) 
  {
    return;
  }

  block_t *curr = &head;
  while (curr->has_next) {
    if((int)curr->start_address == address) {
      curr->state = FREE;
      return;
    }
    curr = curr->next;
  }

  asm("int $0x3");  // Gera um erro de segmentação se o endereço não for válido
}
