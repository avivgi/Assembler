.define sz = 2
K: .data 22
LIST: .data 6, 8, sz

; zero operands
rts
rts r3
hlt
hlt ,


; 1 operand
inc r3
inc sz
inc #3
inc LIST[0]

jmp r1,
jmp ,r2
jmp r4 r5
jmp 


; 2 operands
mov r0, r1
mov r2 ,r3
mov r4,r5
mov r6 , r7

