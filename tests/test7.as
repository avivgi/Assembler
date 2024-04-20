ABBA: .data -22
.define sz = -2
.define ind = 77
.define ind1 = 1
.define ind2 = 2
.define ind3 = 3
.define ind4 = 4
.define ind5 = 5
.entry LIST
LIST: .data 6, 8, sz




;a: .data 1
;b: .data 2
;c: .data 3


;BIL a,b,c

;lea r1, fasdiu3245dghfgshdsf78dhkj12345
;fasdiu3245dghfgshdsf78dhkj12345: .data 4


;; to success
;; addressing

;rts 
;hlt
;A: rts
;B: hlt
;not sz
;not LIST[21]
;not r0
;not r7
;clr sz
;clr LIST[21]
;clr r0
;clr r7
;inc sz
;inc LIST[21]
;inc r0
;inc r7
;dec sz
;dec LIST[21]
;dec r0
;dec r7
;jmp sz
;jmp r0
;jmp r7
;bne sz
;bne r0
;bne r7
;red sz
;red LIST[21]
;red r0
;red r7
;prn #32
;prn sz
;prn LIST[21]
;prn r3
;jsr sz
;jsr r0
;jsr r7
;a11: not sz
;a12: not LIST[21]
;a13: not r0
;a14: not r7
;a15: clr sz
;a16: clr LIST[21]
;a17: clr r0
;a18: clr r7
;a19: inc sz
;a110: inc LIST[21]
;a111: inc r0
;a112: inc r7
;a113: dec sz
;a114: dec LIST[21]
;a115: dec r0
;a116: dec r7
;a117: jmp sz
;a118: jmp r0
;a119: jmp r7
;a120: bne sz
;a121: bne r0
;a122: bne r7
;a123: red sz
;a124: red LIST[21]
;a125: red r0
;a126: red r7
;a127: prn #32
;a128: prn sz
;a129: prn LIST[21]
;a130: prn r3
;a131: jsr sz
;a132: jsr r0
;a133: jsr r7
;mov #22,sz
;mov #22,LIST[4]
;mov #22,r2
;mov ABBA,sz
;mov ABBA,LIST[4]
;mov ABBA,r2
;mov LIST[0],sz
;mov LIST[1],LIST[4]
;mov LIST[2],r2
;mov r3,sz
;mov r4,LIST[4]
;mov r5,r2
;cmp #11,#32
;cmp #12,sz
;cmp #13,LIST[21]
;cmp #14,r3
;cmp ABBA,#32
;cmp ABBA,sz
;cmp ABBA,LIST[21]
;cmp ABBA,r3
;cmp LIST[0],#32
;cmp LIST[1],sz
;cmp LIST[2],LIST[21]
;cmp LIST[3],r3
;cmp r7,#32
;cmp r6,sz
;cmp r5,LIST[21]
;cmp r4,r3
;add #45,ABBA
;add #45,LIST[3]
;add #45,r2
;add sz,ABBA
;add sz,LIST[3]
;add sz,r2
;add LIST[9],ABBA
;add LIST[8],LIST[3]
;add LIST[5],r2
;add r0,ABBA
;add r1,LIST[3]
;add r2,r2
;sub #23,ABBA
;sub #23,LIST[4]
;sub #23,r4
;sub ABBA,ABBA
;sub ABBA,LIST[4]
;sub ABBA,r4
;sub LIST[0],ABBA
;sub LIST[0],LIST[4]
;sub LIST[0],r4
;sub r4,ABBA
;sub r4,LIST[4]
;sub r4,r4
;lea ABBA,ABBA
;lea ABBA,LIST[ind]
;lea ABBA,r1
;lea LIST[5],ABBA
;lea LIST[5],LIST[ind]
;lea LIST[5],r1
;b21: mov #22,sz
;b22: mov #22,LIST[4]
;b23: mov #22,r2
;b24: mov ABBA,sz
;b25: mov ABBA,LIST[4]
;b26: mov ABBA,r2
;b27: mov LIST[0],sz
;b28: mov LIST[1],LIST[4]
;b29: mov LIST[2],r2
;b210: mov r3,sz
;b211: mov r4,LIST[4]
;b212: mov r5,r2
;b213: cmp #11,#32
;b214: cmp #12,sz
;b215: cmp #13,LIST[21]
;b216: cmp #14,r3
;b217: cmp ABBA,#32
;b218: cmp ABBA,sz
;b219: cmp ABBA,LIST[21]
;b220: cmp ABBA,r3
;b221: cmp LIST[0],#32
;b222: cmp LIST[1],sz
;b223: cmp LIST[2],LIST[21]
;b224: cmp LIST[3],r3
;b225: cmp r7,#32
;b226: cmp r6,sz
;b227: cmp r5,LIST[21]
;b228: cmp r4,r3
;b229: add #45,ABBA
;b230: add #45,LIST[3]
;b231: add #45,r2
;b232: add sz,ABBA
;b233: add sz,LIST[3]
;b234: add sz,r2
;b235: add LIST[9],ABBA
;b236: add LIST[8],LIST[3]
;b237: add LIST[5],r2
;b238: add r0,ABBA
;b239: add r1,LIST[3]
;b240: add r2,r2
;b241: sub #23,ABBA
;b242: sub #23,LIST[4]
;b243: sub #23,r4
;b244: sub ABBA,ABBA
;b245: sub ABBA,LIST[4]
;b246: sub ABBA,r4
;b247: sub LIST[0],ABBA
;b248: sub LIST[0],LIST[4]
;b249: sub LIST[0],r4
;b250: sub r4,ABBA
;b251: sub r4,LIST[4]
;b252: sub r4,r4
;b253: lea ABBA,ABBA
;b254: lea ABBA,LIST[ind]
;b255: lea ABBA,r1
;b256: lea LIST[5],ABBA
;b257: lea LIST[5],LIST[ind]
;b258: lea LIST[5],r1

;; spaces 1 operands
;prn LIST[0]
;prn LIST[ 0]
;prn LIST[0 ]
;prn LIST[ 0 ]

;; spaces 2 operands
;; 6 spaces
;cmp LIST[ 0 ] , LIST[ ind ]
;; 5 spaces
;cmp LIST[ 0 ] , LIST[ ind]
;cmp LIST[ 0 ] , LIST[ind ]
;cmp LIST[ 0 ] ,LIST[ ind ]
;cmp LIST[ 0 ], LIST[ ind ]
;cmp LIST[ 0] , LIST[ ind ]
;cmp LIST[0 ] , LIST[ ind ]
;; 4 spaces
;cmp LIST[ 0 ] , LIST[ind]
;cmp LIST[ 0 ] ,LIST[ ind]
;cmp LIST[ 0 ], LIST[ ind]
;cmp LIST[ 0] , LIST[ ind]
;cmp LIST[0 ] , LIST[ ind]
;cmp LIST[ 0 ] ,LIST[ind ]
;cmp LIST[ 0 ], LIST[ind ]
;cmp LIST[ 0] , LIST[ind ]
;cmp LIST[0 ] , LIST[ind ]
;cmp LIST[ 0 ],LIST[ ind ]
;cmp LIST[ 0] ,LIST[ ind ]
;cmp LIST[0 ] ,LIST[ ind ]
;cmp LIST[ 0], LIST[ ind ]
;cmp LIST[0 ], LIST[ ind ]
;cmp LIST[0] , LIST[ ind ]
;; 3 spaces
;cmp LIST[ 0 ] ,LIST[ind]
;cmp LIST[ 0 ], LIST[ind]
;cmp LIST[ 0 ],LIST[ ind]
;cmp LIST[ 0 ],LIST[ind ]
;cmp LIST[ 0] , LIST[ind]
;cmp LIST[ 0] ,LIST[ ind]
;cmp LIST[ 0] ,LIST[ind ]
;cmp LIST[ 0], LIST[ ind]
;cmp LIST[ 0], LIST[ind ]
;cmp LIST[ 0],LIST[ ind ]
;cmp LIST[0 ] , LIST[ind]
;cmp LIST[0 ] ,LIST[ ind]
;cmp LIST[0 ] ,LIST[ind ]
;cmp LIST[0 ], LIST[ ind]
;cmp LIST[0 ], LIST[ind ]
;cmp LIST[0 ],LIST[ ind ]
;cmp LIST[0] , LIST[ ind]
;cmp LIST[0] , LIST[ind ]
;cmp LIST[0] ,LIST[ ind ]
;cmp LIST[0], LIST[ ind ]
;; 2 spaces
;cmp LIST[ 0 ],LIST[ind]
;cmp LIST[ 0] ,LIST[ind]
;cmp LIST[ 0], LIST[ind]
;cmp LIST[ 0],LIST[ ind]
;cmp LIST[ 0],LIST[ind ]
;cmp LIST[0 ] ,LIST[ind]
;cmp LIST[0 ], LIST[ind]
;cmp LIST[0 ],LIST[ ind]
;cmp LIST[0 ],LIST[ind ]
;cmp LIST[0] , LIST[ind]
;cmp LIST[0] ,LIST[ ind]
;cmp LIST[0] ,LIST[ind ]
;cmp LIST[0], LIST[ ind]
;cmp LIST[0], LIST[ind ]
;cmp LIST[0],LIST[ ind ]
;; 1 spaces
;cmp LIST[ 0],LIST[ind]
;cmp LIST[0 ],LIST[ind]
;cmp LIST[0] ,LIST[ind]
;cmp LIST[0], LIST[ind]
;cmp LIST[0],LIST[ ind]
;cmp LIST[0],LIST[ind ]
;; 0 spaces
cmp LIST[0],LIST[ind]