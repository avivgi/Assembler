.define sz = 2
.define abc = 4444
MAIN: mov r3, LIST[sz]
LOOP: jmp L1
prn #-5
mov STR[5], STR[2]
sub r1, r4
cmp r3, #sz 
bne END
L1: inc K
bne LOOP
END: hlt
.define len = 4
cmp 4, #sz 
bne LOOP
bne LOOP
bne LOOP
bne LOOP
STR: .string "abcdef"
LIST: .data 6,-9,len
K: .data 22
cmp 4, #sz 
bne LOOP
bne LOOP
bne LOOP
bne LOOP
