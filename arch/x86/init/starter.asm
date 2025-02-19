section .text
bits 32
extern kernel_main

global start

start:
    mov word [0xb8000], 0x0248 ; H
    call long_mode_paging
    jmp enter_long_mode

long_mode_paging:
    ; Point the first entry of the level 4 page table to the first entry in the
    ; p3 table
    mov eax, p3_table
    or eax, 0b11
    mov dword [p4_table + 0], eax

    ; Point the first entry of the level 3 page table to the first entry in the
    ; p2 table
    mov eax, p2_table
    or eax, 0b11
    mov dword [p3_table + 0], eax

    ; point each page table level two entry to a page
    mov ecx, 0         ; counter variable
.map_p2_table:
    mov eax, 0x200000  ; 2MiB
    mul ecx
    or eax, 0b10000011
    mov [p2_table + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .map_p2_table

    ; move page table address to cr3
    mov eax, p4_table
    mov cr3, eax

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    or eax, 1 << 16
    mov cr0, eax

    ret

enter_long_mode:
    ; Set up the GDT for long mode
    cli
    lgdt [gdt_descriptor]

    ; Enable long mode and paging
    mov eax, cr4
    or eax, 0x20
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    or eax, 0x00000001
    mov cr0, eax

    ; Far jump to 64-bit mode
    jmp 08h:kernel_entry

; 64-bit kernel entry point
bits 64

kernel_entry:
    ; Update segment registers
    mov ax, 10h
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Jump to the 64-bit kernel main function
    call kernel_main
    hlt

section .bss
align 4096

p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096

section .data
align 8

gdt:
    dq 0x0000000000000000 ; Null segment
    dq 0x00af9a000000ffff ; Code segment
    dq 0x00cf92000000ffff ; Data segment
    dq 0x0000000000000000 ; Unused
    dq 0x0000000000000000 ; Unused
    dq 0x0000000000000000 ; TSS segment

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dq gdt

section .text
global gdt_set_entry

gdt_set_entry:
    ; Arguments:
    ; rdi - index
    ; rsi - base
    ; rdx - limit
    ; rcx - access
    ; r8  - flags
    mov rax, rsi
    mov word [gdt + rdi * 8 + 0], dx
    shr rdx, 16
    mov byte [gdt + rdi * 8 + 2], dl
    mov byte [gdt + rdi * 8 + 3], cl
    mov byte [gdt + rdi * 8 + 4], r8b
    shr rsi, 16
    mov byte [gdt + rdi * 8 + 7], al ; movendo para byte e garantindo que seja 8 bits
    mov word [gdt + rdi * 8 + 6], ax ; movendo para word e garantindo que seja 16 bits
    shr rsi, 16
    mov byte [gdt + rdi * 8 + 5], al ; movendo para byte e garantindo que seja 8 bits
    ret