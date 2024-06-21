typedef enum {
  FREE,
  BUSY
} block_state;

typedef struct block {
  unsigned int *start_address;
  unsigned int size;
  block_state state;
  int has_next;
  struct block *next;
} block_t;

extern unsigned int heap_base;
extern int blocks_count;
extern block_t head;
extern block_t *last_block;

void heap_init();
block_t *push_block(unsigned int*, unsigned int);
int kalloc(int);
void kfree(int);
int malloc(int);