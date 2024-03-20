.define sz = 2
MAIN: mov r3, LIST[sz]
LOOP: jmp L1
mcr m_mcr1
cmp r4, #sz
bne END
endmcr
prn #-5
mov STR[5], STR[2]
sub r1, r4
m_mcr1
L1: bne LOOP
END: hlt
.define len = 4
mcr m_mcr2
cmp r6, #sz
cmp r7, #sz
endmcr
STR: .string "abcdef"
LIST: .data 6, -9, len
K: .data 22
m_mcr2
m_mcr2
K: .data 22
