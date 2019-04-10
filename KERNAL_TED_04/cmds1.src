	.page
	.subttl 'cmds1   02/17/84'
;**********************************************
;
;   monitor with mini-assembler/disassembler
;
;**********************************************

entry
       ldx #0          	;call entry***************
       stx flgs
       beq ent010      	;always (.x=msgmon=0)

entbrk
       cld             	;break entry***************
       ldx #5
ent005
       pla             	;save regs & flags
       sta pch,x
       dex
       bpl ent005
       ldx #msgbrk

ent010
       stx syreg
       lda #$c0
       sta msgflg      	;enable kernal msgs

       tsx
       stx sp
       ldx syreg
       jsr msgxxx

       lda tedvcr      	;make sure screen is enabled
       ora #$10
       sta tedvcr

       lda #0          	;zero out 'last address' reg
       sta t2
       sta t2+1
       cli             	;be sure to allow these!


dspreg
       ldx #msgreg
       jsr msgxxx
       lda pch
       jsr puthex
       ldy #0
dspr10
       lda pcl,y
       jsr puthxs
       iny
       cpy #6
       bcc dspr10
       bcs main        	;always


error
       jsr putqst


main
       jsr crlf
       ldx #0          	;'getbuf' now in-line!
       stx chrptr
main00
       jsr basin
       sta buf,x
       inx
       cmp #cr        	;read one line into buffer
       bne main00
       dex
       stx bufend

main01
       jsr gnc         	;get a character from buffer
       beq main        	;end of line
       cmp #' '        	;skip leading spaces
       beq main01

       ldx #cmdqty-1
main05
       cmp cmdchr,x
       beq main10
       dex
       bpl main05
       bmi error
main10
       cpx #cmdls
       bcs main30      	;load/save don't use parse, handle seperatly
       txa
       asl a
       tax
       lda cmdtbl+1,x
       pha
       lda cmdtbl,x
       pha
       jmp parse       	;cute but effective
main30
       sta cmpflg
       jmp lodsav


dspmem
       bcs dspm20      	;no range, do 1/2 screen
       jsr t0tot2      	;else move 'from' value into place
       jsr parse
       bcc dspm30      	;got 'to', go dump
dspm20
       lda #11         	;do 12 lines
       sta t0
       bne dspm40
dspm30 			;calculate # of lines
       jsr sub0m2      	;calculate bytes
       lsr a
       ror t0          	;divide by 8
       lsr a
       ror t0
       lsr a
       ror t0
       sta t0+1
dspm40
       jsr stop        	;is stop key down?
       beq dspm70
       jsr dmpone
       lda #8          	;add 8 to 'starting address'
       jsr addt2
dspm60
       jsr dect0       	;test if dump finished
       bcs dspm40
dspm70
       jmp main


setreg
       bcs dspm70      	;no arg's, done
       lda t0
       ldy t0+1
       sta pcl
       sty pch
       ldy #0
setr10
       jsr parse
       bcs dspm70      	;quit anytime arg list is empty
       lda t0
       sta flgs,y
       iny
       cpy #5
       bcc setr10
       bcs dspm70      	;always ('main')


setmem
       bcs setm20
       jsr t0tot2
       ldy #0
setm10
       jsr parse
       bcs setm20
       lda t0
       sta (t2),y
       iny
       cpy #8
       bcc setm10
setm20 			;**(02/06/84 fix: fab)
       jsr primm       	;clear all modes & cursor up
       .byte $1b,$4f,$91,0

       jsr dmpone
       jmp main


go
       bcs go10
       lda t0
       sta pcl
       lda t0+1
       sta pch
go10
       ldx sp
       txs
       ldx #0
go15
       lda pch,x
       pha
       inx
       cpx #3
       bne go15
       ldx xr
       ldy yr
       lda acc
       rti



CMDCHR
       .BYTE 'X'        	;JUMP TO BASIC WARM START
       .BYTE 'M'        	;MEMORY DUMP
       .BYTE 'R'        	;DISPLAY REGS
       .BYTE 'G'        	;GO
       .BYTE 'T'        	;TRANSFER
       .BYTE 'C'        	;COMPARE
       .BYTE 'D'        	;DISASSEMBLE
       .BYTE 'A'        	;ASSEMBLE
       .BYTE '.'        	;ASSEMBLE, ALSO
       .BYTE 'H'        	;HUNT
       .BYTE 'F'        	;FILL
       .BYTE '>'        	;SET MEMORY
       .BYTE ';'		;SET REGS

CMDLS=*-CMDCHR			;L,S & V MUST BE LAST IN TABLE
       .BYTE 'L'        	;LOAD MEMORY
       .BYTE 'S'        	;SAVE MEMORY
       .BYTE 'V'        	;VERIFY MEMORY
CMDQTY=*-CMDCHR

cmdtbl
       .wor $8003-1    		;use basic's warm start vector
       .wor dspmem-1
       .wor dspreg-1
       .wor go-1
       .wor trnsfr-1
       .wor compar-1
       .wor disasm-1
       .wor assem-1
       .wor assem-1
       .wor hunt-1
       .wor fill-1
       .wor setmem-1
       .wor setreg-1


dmpone
       jsr crlf
       lda #'>'        	;flag as dump
       jsr bsout
       jsr putt2       	;print address
       ldy #0
dmp150
       jsr pick1       	;***patch 01/16/84 to select ram/rom
       jsr puthxs      	;print hex byte, space
       iny
       cpy #8
       bcc dmp150

       jsr primm       	;block off ascii dump & turn rvs on
       .byte ':',18,0

       ldy #0
dmp155
       jsr pick1       	;***patch 01/16/84 to select ram/rom
       and #$7f        	;no weird stuff
       cmp #$20
       bcs dmp156
       lda #'.'
dmp156
       jsr bsout
       iny
       cpy #8
       bcc dmp155
       rts


compar
       lda #0          	;flag 'compare'
       .byte $2c
trnsfr
       lda #$80        	;flag 'transfer'
tnsf10
       sta wrbase+1    	;**02/17
       jsr range       	;get sa in t2, len in t1
       bcs tnsf99      	;no defaults, please
       jsr parse       	;get new addr.
       bcs tnsf99

       jsr crlf
       ldy #0
tnsf15
       jsr pick1       	;**02/17/84 mod to fetch from ram or rom
       bit wrbase+1    	;**02/17
       bpl tnsf20      	;branch if compare
       sta (t0),y       	;else transfer
tnsf20
       cmp (t0),y       	;correct?
       beq tnsf30
       jsr stop
       beq tnsf98
       jsr putt2
tnsf30
       inc t0
       bne tnsf35
       inc t0+1
tnsf35
       jsr inct2
       jsr dect1
       bcs tnsf15
tnsf98
       jmp main
tnsf99
       jmp error
       nop             	;**02/17/84 placeholder

;02/06/84 fab: fixes ascii display @ 'dmpone' should quote mode be set
;02/17/84 tvr: transfer & compare now use ram/rom switch for fetches
;end

