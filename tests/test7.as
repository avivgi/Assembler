K: .data -22
.define sz = -2
.define ind = 5
.entry LIST
LIST: .data 6, 8, sz

; zero operands
;rts
;END: hlt

; 1 operand
;prn #+3
;prn #ind
;prn #sz
;prn #78
;prn #-4
;prn r5
;prn K
;prn BB
;prn LIST[ 0]
;prn LIST[3 ]
;prn LIST[ind]
;prn LIST[ sz ]



; 2 operands

;comma with all
;cmp #1,#ind
;cmp kk,qq
;cmp LIST[1],LIST[ind]
;cmp r1,r7
;cmp qq,r7
;cmp r7,qq

;comma as a word
;cmp LIST[0] , LIST[ind]


;comma with an operand
;cmp LIST[0] ,LIST[ind]
;cmp LIST[ 0] ,LIST[ind]
;cmp LIST[0 ] ,LIST[ind]
;cmp LIST[ 0 ] ,LIST[ind]
;cmp LIST[0] ,LIST[ind]
;cmp LIST[0] ,LIST[ ind]
;cmp LIST[0] ,LIST[ind ]
;cmp LIST[0] ,LIST[ ind ]
;cmp LIST[ 0 ] ,LIST[ ind ]
;cmp LIST[ 0], LIST[ind]
;cmp LIST[0 ], LIST[ind]
;cmp LIST[ 0 ], LIST[ind]
;cmp LIST[0], LIST[ind]
;cmp LIST[0], LIST[ ind]
;cmp LIST[0], LIST[ind ]
;cmp LIST[0], LIST[ ind ]


; comma with some of both
;cmp LIST[0 ],LIST[ind]
;cmp LIST[ 0],LIST[ind]
;cmp LIST[ 0 ],LIST[ind]
;cmp LIST[0],LIST[ ind]
;cmp LIST[0],LIST[ind ]
;cmp LIST[0],LIST[ ind ]
;cmp LIST[ 0],LIST[ ind ]
;cmp LIST[0 ],LIST[ ind ]
;cmp LIST[ 0 ],LIST[ ind ]


