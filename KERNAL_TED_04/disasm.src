	.page
	.subttl 'disasm   01/16/84'
disasm
       bcs disa10      	;use a default length from current sa
       jsr t0tot2
       jsr parse
       bcc disa20      	;got sa,ea. use 'em
disa10
       lda #20         	;guess at 1/2 page
       sta t0
       bne disa30
disa20
       jsr sub0m2      	;put ea-sa in t0
disa30
       jsr crlf
       jsr stop
       beq l999        	;stop ('main')
       jsr dis300      	;disassemble 1 line
       inc length
       lda length
       jsr addt2
       lda length
       jsr subt0
       bcs disa30
       bcc l999        	;always done ('main')


dis300
       lda #'.
       jsr bsout
       jsr putspc
dis400
       jsr putt2
       jsr putspc
       ldy #0
       jsr pick1       	;***patch 01/16/84 select fetches from ram or rom
       jsr dset        	;get instr & digest it

       pha             	;dump (length+1) bytes
       ldx length      	;(.y=0 from 'dset' above)
       inx
pradr0
       dex
       bpl pradrl      	;pad non-printers
       jsr primm       	;print 3 spaces
       .byte '   ',0
       jmp pradrm
       nop
pradrl
       jsr pick1       	;***patch 01/16/84 select fetches from ram or rom
       jsr puthxs
pradrm
       iny
       cpy #3
       bcc pradr0
       pla

       ldx #3
       jsr prmne       	;print mnemonic
       ldx #6          	;6 format bits
pradr1
       cpx #3
       bne pradr3      	;if x=3 print adr val
       ldy length
       beq pradr3      	;no print if len=0
pradr2
       lda format
       cmp #$e8        	;relative addressing mode?
       jsr pick1       	;***patch 01/16/84 select fetches from ram or rom
       bcs reladr
       jsr puthex
       dey
       bne pradr2
pradr3
       asl format      	;test next format bit
       bcc pradr4      	;no print if=0
       lda char1-1,x
       jsr bsout
       lda char2-1,x
       beq pradr4
       jsr bsout
pradr4
       dex
       bne pradr1
       rts


reladr
       jsr pcadj3      	;pcl,h + disp + 1 into a,x
       clc             	;add 1
       adc #1
       bne relad2
       inx
relad2
       jmp putwrd


pcadj3
       ldx t2+1
       tay
       bpl pcadj4
       dex
pcadj4
       adc t2
       bcc pcrts
       inx
pcrts
       rts



; disassembler digest routine
;
dset
       tay
       lsr a           	;even/odd test
       bcc ieven
       lsr a           	;test b1
       bcs err         	;xxxxxx11 instr bad
       cmp #$22
       beq err         	;10001001 instr bad
       and #$7         	;mask 3 bits for adr mode
       ora #$80        	;add indexing offset
ieven
       lsr a           	;left/right test
       tax
       lda nmode,x     	;index into mode table
       bcs rtmode      	;if carry set use lsb for
       lsr a           	;print format index
       lsr a
       lsr a           	;if carry clr use msb
       lsr a
rtmode
       and #$0f        	;mask for 4-bit index
       bne getfmt      	;$0 for bad opcodes
err
       ldy #$80        	;sub $80 for bad opcode
       lda #0          	;set format index to zero
getfmt
       tax
       lda nmode2,x    	;index into prt format tab
       sta format      	;save for adr field format
       and #3          	;mask 2-bit length. 0=1byte
       sta length      	;1=2byte,2=3byte
       tya             	;op code
       and #$8f        	;mask for 1xxx1010 test
       tax             	;save in x
       tya             	;op code again
       ldy #3
       cpx #$8a
       beq mnndx3
mnndx1
       lsr a
       bcc mnndx3      	;form index into mnemonic tab
       lsr a
mnndx2
       lsr a           	;1xxx1010->00101xxx
       ora #$20        	;xxxyyy01->00111xxx
       dey             	;xxxyyy10->00110xxx
       bne mnndx2      	;xxxyy100->00100xxx
       iny             	;xxxxx000->000xxxxx
mnndx3
       dey
       bne mnndx1
       rts             	;(.y=0 is assumed!)



; print mnemonic
; enter x=3 characters
;
prmne
       tay
       lda mneml,y     	;fetch 3 char mnemonic
       sta t1
       lda mnemr,y
       sta t1+1
prmn1
       lda #0
       ldy #5
prmn2
       asl t1+1        	;shift 5 bits of char
       rol t1          	;into a
       rol a           	;clear carry
       dey
       bne prmn2
       adc #$3f        	;add '?' offset
       jsr bsout
       dex
       bne prmn1
       jmp putspc      	;finish with space


nmode
       .byte $40,2,$45,3
       .byte $d0,8,$40,9
       .byte $30,$22,$45,$33
       .byte $d0,8,$40,9
       .byte $40,2,$45,$33
       .byte $d0,8,$40,9
       .byte $40,$02,$45,$b3
       .byte $d0,$08,$40,$09
       .byte 0,$22,$44,$33
       .byte $d0,$8c,$44,0
       .byte $11,$22,$44,$33
       .byte $d0,$8c,$44,$9a
       .byte $10,$22,$44,$33
       .byte $d0,8,$40,9
       .byte $10,$22,$44,$33
       .byte $d0,8,$40,9
       .byte $62,$13,$78,$a9
nmode2
       .byte 0,$21,$81,$82
       .byte 0,0,$59,$4d
       .byte $91,$92,$86,$4a
       .byte $85,$9d
char1
       .byte ',),#($'
char2
       .byte 'Y',0,'X$$',0
mneml
       .byte $1c,$8a,$1c,$23
       .byte $5d,$8b,$1b,$a1
       .byte $9d,$8a,$1d,$23
       .byte $9d,$8b,$1d,$a1
       .byte 0,$29,$19,$ae
       .byte $69,$a8,$19,$23
       .byte $24,$53,$1b,$23
       .byte $24,$53,$19,$a1
       .byte 0,$1a,$5b,$5b
       .byte $a5,$69,$24,$24
       .byte $ae,$ae,$a8,$ad
       .byte $29,0,$7c,0
       .byte $15,$9c,$6d,$9c
       .byte $a5,$69,$29,$53
       .byte $84,$13,$34,$11
       .byte $a5,$69,$23,$a0
mnemr
       .byte $d8,$62,$5a,$48
       .byte $26,$62,$94,$88
       .byte $54,$44,$c8,$54
       .byte $68,$44,$e8,$94
       .byte 0,$b4,8,$84
       .byte $74,$b4,$28,$6e
       .byte $74,$f4,$cc,$4a
       .byte $72,$f2,$a4,$8a
       .byte 0,$aa,$a2,$a2
       .byte $74,$74,$74,$72
       .byte $44,$68,$b2,$32
       .byte $b2,0,$22,0
       .byte $1a,$1a,$26,$26
       .byte $72,$72,$88,$c8
       .byte $c4,$ca,$26,$48
       .byte $44,$44,$a2,$c8
regk
       .byte cr,$20,$20,$20

;end
