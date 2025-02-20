#include <video/vga_buffer.h>

void interrupt_handler(int interrupt) {
    printf(". %d ", interrupt);
}__attribute__((interrupt));
