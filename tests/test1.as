.define sz = 2
.define szns=3
MAIN: mov r3, LIST[sz]
LOOP: jmp L1
mcr m_mcr
cmp r3, #sz 
bne END
endmcr
prn #-5
mov STR[5], STR[2]
sub r1, r4
m_mcr
L1: inc K
bne LOOP
mcr m_mcr2
cmp 4, #sz 
bne LOOP
bne LOOP
bne LOOP
bne LOOP
endmcr
END: hlt
.define len=4
m_mcr2
STR: .string "abcdef"
LIST: .data 6,-9,len
K: .data 22
m_mcr2
