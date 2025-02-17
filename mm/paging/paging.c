#include "paging.h"
#include "heap.h"
#include "vga_buffer.h"

unsigned int *page_directory;

// loads the address of the kernel's page directory in the register CR3
void load_page_directory(unsigned int page_directory) {
    printi((int)page_directory);  // Print the address of the page directory
    asm volatile("mov %0, %%cr3" :: "r"(page_directory));
}

void enable_paging() {
    unsigned int cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging (bit 31)
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

int create_page_entry(int base_address, char present, char writable, 
        char privilege_level, char cache_enabled, 
        char write_through_cache, char accessed, 
        char page_size, char dirty) {
    int entry = (base_address & ~0xFFF); // Ensure 4 KB alignment
    entry |= present;
    entry |= writable << 1;
    entry |= privilege_level << 2;
    entry |= write_through_cache << 3;
    entry |= cache_enabled << 4;
    entry |= accessed << 5;
    entry |= dirty << 6;
    entry |= page_size << 7;
    return entry;
}

unsigned int virtual_to_physical(unsigned int virtual_addr) {
    unsigned int dir_index = (virtual_addr >> 22) & 0x3FF;  // Directory index
    unsigned int table_index = (virtual_addr >> 12) & 0x3FF; // Table index
    unsigned int offset = virtual_addr & 0xFFF; // Offset

    // Check if page directory is present
    if (!(page_directory[dir_index] & 1)) {
        errorPrint("Error: Page directory not mapped!");
        return 0;  // Invalid address
    }

    // Get the address of the page table
    unsigned int *page_table = (unsigned int*)kalloc(4 * 1024);

    // Check if the page is present in the page table
    if (!(page_table[table_index] & 1)) {
        errorPrint("Error: Page not allocated!");
        return 0;  // Invalid address
    }

    // Return the physical address by combining the page table entry with the offset
    return (page_table[table_index] & ~0xFFF) + offset;
}

void paging_init() {
    unsigned int curr_page_frame = 0;

    // Allocate memory for the page directory and align to 4KB
    page_directory = (unsigned int *)((kalloc(4 * 1024) + 0xFFF) & ~0xFFF);

    if ((int)page_directory == -1) {
        errorPrint("Error allocating memory for page directory");
        return;
    }

    // Initialize the page directory entries
    for (int currPDE = 0; currPDE < PDE_NUM; currPDE++) {
        unsigned int *pagetable = (unsigned int *)kalloc(PTE_NUM * sizeof(unsigned int));
        if ((int)pagetable == -1) {
            errorPrint("Error allocating memory for page table");
            printi(1010);
            return;
        }

        // Initialize the page table entries
        for (int currPTE = 0; currPTE < PTE_NUM; currPTE++, curr_page_frame++) {
            pagetable[currPTE] = create_page_entry(curr_page_frame * 4096, 1, 0, 0, 1, 1, 0, 0, 0);
        }

        // Update the page directory with the physical address of the page table
        page_directory[currPDE] = create_page_entry(
            virtual_to_physical((unsigned int)pagetable),  // Convert the page table address to physical
            1, 0, 0, 1, 1, 0, 0, 0
        );
    }

    // Get the physical address of the page directory
    unsigned int physical_page_directory = virtual_to_physical((unsigned int)page_directory);
    printi((int)physical_page_directory);  // Print the physical address of the page directory

    if ((int)physical_page_directory == -1) {
        errorPrint("Error converting page directory to physical address");
        return;
    }

    // Load the page directory into CR3
    //load_page_directory(physical_page_directory);

    // Enable paging
    //enable_paging();
    println("Memory: Paging Initialized!");
}