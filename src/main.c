extern void enable_interrupts(void);
extern void disable_interrupts(void);

volatile unsigned char *video = 0xb8000;

#define BUFFER_WIDTH 80
#define BUFFER_HEIGHT 25

typedef struct {
    volatile unsigned char buffer[BUFFER_HEIGHT][BUFFER_WIDTH * 2];
} VgaBuffer;

typedef struct {
    int column;
    int row;
    int color_code;
} Writer;

volatile VgaBuffer *vga = (volatile VgaBuffer *)0xb8000;
Writer vgaWriter = {
    column: 0,
    row: 0,
    color_code: 15
};

void print(char *);
void new_line();
void printi( int );
void clear_row();

void kernel_main() {
    print("Welcome to square-kernel!\n");
    print("We are in protected mode!\n");

    while( 1 );
}

void print(char *str) {
    while(*str != '\0') {
        char c = *str;
        if (c == '\n') {
            new_line();
            str++;
            continue;
        }

        if(vgaWriter.row > BUFFER_HEIGHT || vgaWriter.column == BUFFER_WIDTH*2) {
            disable_interrupts();
            clear_row();
            enable_interrupts();
        }
        
        vga->buffer[vgaWriter.row][vgaWriter.column] = c;
        vga->buffer[vgaWriter.row][vgaWriter.column+1] = vgaWriter.color_code;
        vgaWriter.column += 2;

        str++;
    }
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

void interrupt_handler(int interrupt_number) {
    /* if(interrupt_number == 32) {
        print(".");
    } */
}
