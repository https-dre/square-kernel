extern kernel_main
extern interrupt_handler

extern scheduler
extern run_next_process

bits 16
start:
    mov ax, cs
    mov ds, ax

    call load_gdt
    call init_video_mode
    call enter_protected_mode
    call setup_interrupts
    call load_task_register

    call 08h:start_kernel

setup_interrupts:
    call remap_pic
    call load_idt
    
    ret
load_idt:
    lidt [idtr - start]
    ret

load_gdt:
    cli
    lgdt [gdtr - start]

    ret

; Entrando no modo protegido 
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
remap_pic:
    mov al, 0x11        ; Início da inicialização do PIC mestre
    out 0x20, al
    mov al, 0x11        ; Início da inicialização do PIC escravo
    out 0xA0, al

    mov al, 0x20        ; Remapear o PIC mestre para 0x20-0x27
    out 0x21, al
    mov al, 0x28        ; Remapear o PIC escravo para 0x28-0x2F
    out 0xA1, al

    mov al, 0x04        ; Informar ao PIC mestre sobre o PIC escravo na linha IRQ2
    out 0x21, al
    mov al, 0x02        ; Informar ao PIC escravo sobre a linha de ligação com o PIC mestre
    out 0xA1, al

    mov al, 0x01        ; Modo 8086/88 (normal)
    out 0x21, al
    mov al, 0x01        ; Modo 8086/88 (normal)
    out 0xA1, al

    ; Mascarar todas as interrupções exceto a do timer (IRQ0)
    mov al, 0xFE
    out 0x21, al

    ; Mascarar todas as interrupções do PIC escravo
    mov al, 0xFF
    out 0xA1, al

    ret

set_pit_frequency:
    ; Parâmetro: frequência desejada (em Hz) é passado em eax
    mov ecx, 1193182       ; Frequência base do PIT
    mov ebx, eax           ; Frequência desejada (em Hz)
    div ebx                ; Calcula o divisor
    ; O divisor é agora armazenado em eax

    ; Configura o PIT
    mov al, 0x36           ; Command byte: Channel 0, Access mode lobyte/hibyte, Mode 3 (square wave generator)
    out 0x43, al           ; Envia o comando para a porta de controle do PIT (0x43)

    mov al, al             ; Low byte do divisor
    out 0x40, al           ; Envia o low byte para a porta de dados do canal 0 (0x40)

    mov al, ah             ; High byte do divisor
    out 0x40, al           ; Envia o high byte para a porta de dados do canal 0 (0x40)

    ret                    ; Retorna da função

load_task_register:
    mov ax, 40d
    ltr ax

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

%include "arch/x86/gdt.asm"
%include "arch/x86/idt.asm"

tss:
    dd 0       ; Segmento de ligação de backlink (reservado no x86)
    dd 0       ; Reservado
    dd 0       ; Segmento de pilha de kernel
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
    dd 0       ; Reservado
