#include "vga_buffer.h"
#include "colors.h"

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5

volatile VgaBuffer *vga; // Define vga
Writer vgaWriter; // Define vgaWriter

// Escreve um byte na porta de E/S
static inline void outb(unsigned short port, unsigned char data) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

// Lê um byte de uma porta de E/S
static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile ("inb %1, %0" : "=a" (data) : "dN" (port));
    return data;
}

// Move o cursor para a posição (row, col)
void move_cursor(int row, int col) {
    if (row < 0 || row >= BUFFER_HEIGHT || col < 0 || col >= BUFFER_WIDTH) {
        return; // Evita posições inválidas
    }

    unsigned short position = (row * BUFFER_WIDTH) + col;

    // DEBUG: Imprime a posição calculada
    // (Se tiver um método de print, use-o para ver a posição calculada)
    // printf("Cursor Pos: %d\n", position);

    outb(VGA_COMMAND_PORT, 0x0E); // Byte alto do cursor
    outb(VGA_DATA_PORT, (position >> 8) & 0xFF);

    outb(VGA_COMMAND_PORT, 0x0F); // Byte baixo do cursor
    outb(VGA_DATA_PORT, position & 0xFF);
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void vga_init() {
    vga = (volatile VgaBuffer *)0xb8000;
    vgaWriter.column = 0;
    vgaWriter.row = 0;
    vgaWriter.color_code = 15;
    disable_cursor();
    println("Video: VGA Buffer Initialized!");
}

void print(char *str) {
    while(*str != '\0') {
        char c = *str;
        if (c == '\n') {
            new_line();
            str++;
            continue;
        }

        if(vgaWriter.row == BUFFER_HEIGHT) {
            clear_row();
        }

        if(vgaWriter.column == BUFFER_WIDTH) {
            new_line();
        }
        //move_cursor(vgaWriter.row, vgaWriter.column);
        vga->buffer[vgaWriter.row][vgaWriter.column] = c; 
        vga->buffer[vgaWriter.row][vgaWriter.column+1] = vgaWriter.color_code;
        vgaWriter.column += 2;
        str++;
    }
}

void println(char *str) {
    print(str);
    print("\n");
}

void clear_row() {
    for(int row = 0; row < BUFFER_HEIGHT; row++) {
        for(int col = 0; col < BUFFER_WIDTH; col++) {
            char *character = vga->buffer[row][col];
            vga->buffer[row - 1][col] = *character;
        }
    }
}

void new_line()
{
    vgaWriter.row += 1;
    vgaWriter.column = 0;
}

void printi(int number) {
    char* digitToStr[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

    if (number >= 0 && number <= 9) {
        print(digitToStr[number]);
        return;
    } else {
        int remaining = number % 10;
        number = number / 10;

        printi(number);
        printi(remaining);
    }
}

void errorPrint(char *str) {
    set_vga_color_code(color_code(Red, Black));
    println(str);
    setDefaultColor();
}

void set_vga_color_code(int color) {
    vgaWriter.color_code = color;
}

int color_code(int background, int color) {
    return ((background << 4) | color);
}

void setDefaultColor() {
    set_vga_color_code(color_code(Black, White));
}