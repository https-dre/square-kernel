#define BUFFER_WIDTH 160
#define BUFFER_HEIGHT 25

enum Color {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Pink = 13,
    Yellow = 14,
    White = 15,
};

typedef struct {
    volatile unsigned char buffer[BUFFER_HEIGHT][BUFFER_WIDTH];
} VgaBuffer;

typedef struct {
    int column;
    int row;
    int color_code;
} Writer;

extern volatile VgaBuffer *vga; // Declare vga as extern
extern Writer vgaWriter; // Declare vgaWriter as extern

void vga_init();
void print(char *);
void new_line();
void printi(int);
void clear_row();
void set_vga_color_code(int);
int new_color_code(int, int);