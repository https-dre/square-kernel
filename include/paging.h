#define PDE_NUM 3 // page directory entries
#define PTE_NUM 1024

extern unsigned int *page_directory;

void paging_init();
int create_page_entry(int, char, char, char, char, char, char, char, char);