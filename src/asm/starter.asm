bits 16
extern kernel_main

start:
    mov ax, cs
    mov ds, ax

    call load_gdt
    call init_video_mode
    call enter_protected_mode
    call setup_interrupts

    call 08h:start_kernel

setup_interrupts:
    ret

; Entrando no modo protegido 

load_gdt:
    cli
    lgdt [gdtr - start]

    ret

enter_protected_mode:
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ret

init_video_mode:
    mov ah, 0h
    mov al, 03h ; video mode
    int 10h

    mov ah, 01h
    mov cx, 2000h
    int 10h

    ret

bits 32
start_kernel:
    mov eax, 10h
    mov ds, eax ; data segment
    mov ss, eax ; stack segment

    mov eax, 0h
    mov es, eax
    mov fs, eax
    mov gs, eax

    sti
    call kernel_main


%include "gdt.asm"