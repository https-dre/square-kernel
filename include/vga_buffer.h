#define BUFFER_WIDTH 160
#define BUFFER_HEIGHT 25

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
int color_code(int, int);
void println(char *);
void errorPrint(char *);
void setDefaultColor();