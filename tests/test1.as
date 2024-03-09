.define sz = 2 MAIN: LOOP:
L1:
mov r3, LIST[sz] jmp L1
mcr m_mcr
cmp r3, #sz bne END
endmcr
prn #-5
mov STR[5], STR[2] sub r1, r4
m_mcr
inc K
bne LOOP
END: hlt .define len = 4
STR:
LIST: .data K:
.string “abcdef” 6, -9, len
.data 22
