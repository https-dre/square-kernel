global start

section .text
bits 32
extern kernel_main
start:
    mov word [0xb8000], 0x0248 ; H
    call kernel_main
    hlt