; Example assembly code
message: .string "Hello, World"
.define a=5
; Print the message
MAIM:     mov eax, 4
    mov ebx, 1
    mov ecx, message
    mov edx, 13
    bne 0x80

    ; Exit the program
EXIT:   mov eax, 1
    sub ebx, ebx
    bne 0x80