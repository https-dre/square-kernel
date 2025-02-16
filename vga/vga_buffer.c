#include "vga_buffer.h"
#include "colors.h"

volatile VgaBuffer *vga; // Define vga
Writer vgaWriter; // Define vgaWriter

void vga_init() {
    vga = (volatile VgaBuffer *)0xb8000;
    vgaWriter.column = 0;
    vgaWriter.row = 0;
    vgaWriter.color_code = 15;
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
    set_vga_color_code(new_color_code(Red, Black));
    println(str);
    setDefaultColor();
}

void set_vga_color_code(int color) {
    vgaWriter.color_code = color;
}

int new_color_code(int background, int color) {
    return ((background << 4) | color);
}

void setDefaultColor() {
    set_vga_color_code(new_color_code(Black, White));
}