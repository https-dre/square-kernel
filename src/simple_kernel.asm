org 0900h
start:
  mov ax, cs
  mov ds, ax
    
  mov si, hello_string
  call print_string
    
  jmp $

print_string:
  mov ah, 0Eh

print_char:
  lodsb
    
  cmp al, 0
  je done
    
  int 10h
    
  jmp print_char

done:
  ret
    
hello_string db '0', 0
