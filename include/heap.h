typedef enum {
  FREE,
  BUSY
} block_state;

#define NULL ((void*)0)

typedef struct block_t {
  unsigned int start_address;
  unsigned int size;
  block_state state;
  int has_next;
  struct block_t *next;
} block_t;

extern unsigned int heap_base;
extern int blocks_count;
extern block_t *head;

void heap_init();
block_t *push_block(unsigned int, unsigned int, block_t*);
void kfree(int);
int* malloc(int);
