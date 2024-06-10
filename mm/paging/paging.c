#include "paging.h"

int create_page_entry( int base_address, char present, char writable, 
char privilege_level, char cache_enabled, char write_through_cache, 
char accessed, char page_size, char dirty ) {
    int entry = 0;

    entry |= present;
    entry |= writable << 1;
    entry |= privilege_level << 2;
    entry |= write_through_cache << 3;
    entry |= cache_enabled << 4;
    entry |= accessed << 5;
    entry |= dirty << 6;
    entry |= page_size << 7;

    return base_address | entry;
}