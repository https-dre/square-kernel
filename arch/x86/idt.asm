; IRQ (Interruption Request)
; refere-se a uma solicitação de interrupção enviada por um dispositivo
; de hardqare para o processador. Isso indica que o dispositivo precisa de atenção
; imediata do sistema operaional ou do BIOS.

; ISR (Interrupt Service Routine)
; é o código de rotina específico que é executado pelo sistema operacional em resposta a 
; uma interrupção gerada por um IRQ específico. Cada IRQ tem sua própria ISR.

; Cada IRQ está associado a uma ISR específica no sistema operacional. Quando um IRQ é
; acionado, o processador invoca imediatamente a ISR correspondente com a IRQ.

extern interrupt_handler

isr_0:
	cli
	push 0
	jmp isr_basic

isr_1:
	cli
	push 1
	jmp isr_basic
	
isr_2:
	cli
	push 2
	jmp isr_basic
	
isr_3:
	cli
	push 3
	jmp isr_basic
	
isr_4:
	cli
	push 4
	jmp isr_basic
	
isr_5:
	cli
	push 5
	jmp isr_basic
	
isr_6:
	cli
	push 6
	jmp isr_basic
	
isr_7:
	cli
	push 7
	jmp isr_basic
	
isr_8:
	cli
	push 8
	jmp isr_basic
	
isr_9:
	cli
	push 9
	jmp isr_basic
	
isr_10:
	cli
	push 10
	jmp isr_basic
	
isr_11:
	cli
	push 11
	jmp isr_basic
	
isr_12:
	cli
	push 12
	jmp isr_basic
	
isr_13:
	cli
	push 13
	jmp isr_basic
	
isr_14:
	cli
	push 14
	jmp isr_basic
	
isr_15:
	cli
	push 15
	jmp isr_basic
	
isr_16:
	cli
	push 16
	jmp isr_basic
	
isr_17:
	cli
	push 17
	jmp isr_basic
	
isr_18:
	cli
	push 18
	jmp isr_basic
	
isr_19:
	cli
	push 19
	jmp isr_basic
	
isr_20:
	cli
	push 20
	jmp isr_basic
	
isr_21:
	cli
	push 21
	jmp isr_basic

isr_22:
	cli
	push 22
	jmp isr_basic
	
isr_23:
	cli
	push 23
	jmp isr_basic
	
isr_24:
	cli
	push 24
	jmp isr_basic
	
isr_25:
	cli
	push 25
	jmp isr_basic
	
isr_26:
	cli
	push 26
	jmp isr_basic
	
isr_27:
	cli
	push 27
	jmp isr_basic
	
isr_28:
	cli
	push 28
	jmp isr_basic
	
isr_29:
	cli
	push 29
	jmp isr_basic
	
isr_30:
	cli
	push 30
	jmp isr_basic
	
isr_31:
	cli
	push 31
	jmp isr_basic
	
isr_32:
	cli
	push 32
	jmp irq_basic
	
isr_33:
	cli
	push 33
	jmp irq_basic
	
isr_34:
	cli
	push 34
	jmp irq_basic
	
isr_35:
	cli
	push 35
	jmp irq_basic
	
isr_36:
	cli
	push 36
	jmp irq_basic
	
isr_37:
	cli
	push 37
	jmp irq_basic

isr_38:
	cli
	push 38
	jmp irq_basic

isr_39:
	cli
	push 39
	jmp irq_basic

isr_40:
	cli
	push 40
	jmp irq_basic

isr_41:
	cli
	push 41
	jmp irq_basic

isr_42:
	cli
	push 42
	jmp irq_basic

isr_43:
	cli
	push 43
	jmp irq_basic

isr_44:
	cli
	push 44
	jmp irq_basic

isr_45:
	cli
	push 45
	jmp irq_basic

isr_46:
	cli
	push 46
	jmp irq_basic

isr_47:
	cli
	push 47
	jmp irq_basic
	
isr_48:
	cli
	push 48
	jmp irq_basic

isr_49:
  cli
  push 49
  jmp isr_basic

isr_basic:
	call interrupt_handler
	pop rax
    
  	sti
	iret

irq_basic:
    call interrupt_handler

    ; Envia EOI para o PIC mestre
    mov al, 0x20
    out 0x20, al

    ; Se a interrupção veio do PIC escravo (IRQ 8+), precisa limpar os dois PICs
    cmp byte [rsp + 8], 40  ; IRQ >= 40 (decimal) significa que veio do PIC escravo
    jl .irq_basic_end       ; Se for menor, não precisa limpar o PIC escravo

    mov al, 0x20
    out 0xA0, al            ; Envia EOI para o PIC escravo

.irq_basic_end:

    sti    ; Reativa interrupções
    iretq  ; Retorna da interrupção (versão 64 bits do IRET)

global load_idt

load_idt:
	lidt [idtr]
	ret

section .data
align 16
idt:
    dq isr_0, 0x0008000000008E00
    dq isr_1, 0x0008000000008E00
    dq isr_2, 0x0008000000008E00
    dq isr_3, 0x0008000000008E00
    dq isr_4, 0x0008000000008E00
    dq isr_5, 0x0008000000008E00
    dq isr_6, 0x0008000000008E00
    dq isr_7, 0x0008000000008E00
    dq isr_8, 0x0008000000008E00
    dq isr_9, 0x0008000000008E00
    dq isr_10, 0x0008000000008E00
    dq isr_11, 0x0008000000008E00
    dq isr_12, 0x0008000000008E00
    dq isr_13, 0x0008000000008E00
    dq isr_14, 0x0008000000008E00
    dq isr_15, 0x0008000000008E00
    dq isr_16, 0x0008000000008E00
    dq isr_17, 0x0008000000008E00
    dq isr_18, 0x0008000000008E00
    dq isr_19, 0x0008000000008E00
    dq isr_20, 0x0008000000008E00
    dq isr_21, 0x0008000000008E00
    dq isr_22, 0x0008000000008E00
    dq isr_23, 0x0008000000008E00
    dq isr_24, 0x0008000000008E00
    dq isr_25, 0x0008000000008E00
    dq isr_26, 0x0008000000008E00
    dq isr_27, 0x0008000000008E00
    dq isr_28, 0x0008000000008E00
    dq isr_29, 0x0008000000008E00
    dq isr_30, 0x0008000000008E00
    dq isr_31, 0x0008000000008E00
    dq isr_32, 0x0008000000008E00
    dq isr_33, 0x0008000000008E00
    dq isr_34, 0x0008000000008E00
    dq isr_35, 0x0008000000008E00
    dq isr_36, 0x0008000000008E00
    dq isr_37, 0x0008000000008E00
    dq isr_38, 0x0008000000008E00
    dq isr_39, 0x0008000000008E00
    dq isr_40, 0x0008000000008E00
    dq isr_41, 0x0008000000008E00
    dq isr_42, 0x0008000000008E00
    dq isr_43, 0x0008000000008E00
    dq isr_44, 0x0008000000008E00
    dq isr_45, 0x0008000000008E00
    dq isr_46, 0x0008000000008E00
    dq isr_47, 0x0008000000008E00
    dq isr_48, 0x0008000000008E00
    dq isr_49, 0x0008000000008E00

idtr:
    dw (idtr - idt) - 1
    dq idt