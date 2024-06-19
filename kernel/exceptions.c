#include "exceptions.h"
#include "vga_buffer.h"
#include "colors.h"


void exception_handler(int code) {
  set_vga_color_code(new_color_code(Red, Black));
  println("   FAULT OCURRED: ");
  printi(code);
  setDefaultColor();
  asm("hlt");
}
