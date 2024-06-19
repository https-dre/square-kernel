#include "exceptions.h"
#include "vga_buffer.h"
#include "colors.h"


void exception_handler(int code) {
  set_vga_color_code(new_color_code(Black, Red));
  print("\nFAULT OCURRED: ");
  printi(code);
  setDefaultColor();
  println("\n CPU Halted!");
  asm("cli");
  asm("hlt");
}
