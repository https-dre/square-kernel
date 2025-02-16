SECTIONS
{
  .text 0x09000 : 
  {
    code = .; _code = .; __code = .;
    *(.text)
  } = 0xE0  /* RX (Leitura e Execução) */

  .data : 
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
  } = 0xC0  /* RW (Leitura e Escrita) */

  .bss : 
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
  } = 0xC0  /* RW (Leitura e Escrita) */

  end = .; _end = .; __end = .;
}
