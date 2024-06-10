#define PDE_NUM 3
#define PTE_NUM 1024

// loads the address of the kernel's page directory in the register CR3
extern void load_page_directory();
extern void enable_paging();

extern unsigned int *page_directory;

void paging_init();
int create_page_entry(int, char, char, char, char, char, char, char, char);