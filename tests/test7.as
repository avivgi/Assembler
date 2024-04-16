;K: .data -22
.define sz = -2
.define ind = 5
;.entry LIST
LIST: .data 6, 8, sz


; to fail
; zero operands
; 1 operand
; 2 operands

lea r1, fasdiu3245dghfgshdsf78dhkj12345
fasdiu3245dghfgshdsf78dhkj12345: .data 3

; to success
; zero operands
;rts
;A: rts
;hlt
;B: hlt

; 1 operand
;prn #+3
;prn #ind
;prn #sz
;prn #78
;prn #-4
;prn K
;prn LIST[ 0]
;prn LIST[3 ]
;prn LIST[ind]




; 2 operands

cmp #1,#ind
cmp LIST[1],LIST[ind]
cmp r1,r7
cmp LIST[0] , LIST[ind]
cmp LIST[0] ,LIST[ind]
cmp LIST[ 0] ,LIST[ind]
cmp LIST[0 ] ,LIST[ind]
cmp LIST[ 0 ] ,LIST[ind]
cmp LIST[0] ,LIST[ind]
cmp LIST[0] ,LIST[ ind]
cmp LIST[0] ,LIST[ind ]
cmp LIST[0] ,LIST[ ind ]
cmp LIST[ 0 ] ,LIST[ ind ]
cmp LIST[ 0], LIST[ind]
cmp LIST[0 ], LIST[ind]
cmp LIST[ 0 ], LIST[ind]
cmp LIST[0], LIST[ind]
cmp LIST[0], LIST[ ind]
cmp LIST[0], LIST[ind ]
cmp LIST[0], LIST[ ind ]
cmp LIST[0 ],LIST[ind]
cmp LIST[ 0],LIST[ind]
cmp LIST[ 0 ],LIST[ind]
cmp LIST[0],LIST[ ind]
cmp LIST[0],LIST[ind ]
cmp LIST[0],LIST[ ind ]
cmp LIST[ 0],LIST[ ind ]
cmp LIST[0 ],LIST[ ind ]
cmp LIST[ 0 ],LIST[ ind ]