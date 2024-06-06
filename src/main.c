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

void kernel_main() {
    print("Welcome to square-kernel!\n");
    print("We are now in protected mode");

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
        
        vga->buffer[vgaWriter.row][vgaWriter.column] = c;
        vga->buffer[vgaWriter.row][vgaWriter.column+1] = vgaWriter.color_code;
        vgaWriter.column += 2;

        str++;
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
    print("\nInterrupt Received ");
    printi(interrupt_number);
}
