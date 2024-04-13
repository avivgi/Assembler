; All rights reserved to the open university of israel. (c)
MAIN: add r3,LIST
LOOP: prn #48
lea W,r6
inc r6
mov r3,K
sub r1,r4
bne END
cmp K, #-6
bne END
dec W
;Comment0
.entry MAIN
jmp LOOP
add L3,L3
END: hlt
STR: .string "abcd"
;Comment1
LIST: .data 6,-9
x: .data -100
.data 31
.entry END
.extern W
.extern L3