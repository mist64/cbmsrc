       .page
	.subttl 'assem'
;  simple assembler
;  syntax:      a 1111 lda ($00,x)
;               a 1111 dex:               (':' = terminator)

assem
       bcc as005
       jmp error
as005
       jsr t0tot2
as010
       ldx #0
       stx hulp+1      	;clear left mnemonic
as020
       jsr gnc         	;get a char
       bne as025       	;check for eol
       cpx #0
       bne as025
       jmp main        	;if eol & no mnemonic, exit cleanly
as025
       cmp #$20        	;is it a space ?
       beq as010       	;yes - start again
       sta msal,x      	;no - save char
       inx
       cpx #3          	;got three chars ?
       bne as020       	;no - loop
as030
       dex             	;squished all three ?
       bmi as045       	;yes
       lda msal,x      	;no - first in last out
       sec             	;no borrow
       sbc #$3f        	;normalize
       ldy #5          	;set for 5 shift rights
as040
       lsr a
       ror hulp+1      	;left mnemonic
       ror hulp        	;right mnemonic
       dey             	;done 5 shifts?
       bne as040       	;no-loop
       beq as030       	;always

as045
       ldx #2          	;move index past mnemonic
as050
       jsr gnc         	;get a char
       beq as100       	;done if eol
       cmp #' '        	;a space
       beq as050       	;yes-skip it
       jsr chrtst      	;a hex #?
       bcs as070       	;no-buffer if
       jsr rdob2       	;fin a read byte
       ldy t0          	;shift t0 to t0+1
       sty t0+1
       sta t0          	;save byte
       lda #'0         	;buffer ascii 0
       sta hulp,x
       inx
as070
       sta hulp,x
       inx
       cpx #10         	;watch buffer size
       bcc as050       	;branch if not full
as100
       stx t1          	;save input # of chars
       ldx #0
       stx wrap        	;start trial at zero
as110
       ldx #0
       stx tmpc        	;disa index=0
       lda wrap        	;get trial byte
       jsr dset        	;digest it
       ldx format      	;save format for later
       stx t1+1
       tax             	;index into mnemonic table
       lda mnemr,x     	;get compressed
       jsr tstrx       	;mnemonic and test
       lda mneml,x
       jsr tstrx
       ldx #6          	;six format bits
as210
       cpx #3
       bne as230
       ldy length
       beq as230       	;skip-single byte instr
as220
       lda format
       cmp #$e8        	;a relative instr?
       lda #'0         	;test zeros
       bcs as250       	;no-3 byte
       jsr tst2        	;test a byte,2 chars
       dey
       bne as220
as230
       asl format
       bcc as240
       lda char1-1,x
       jsr tstrx       	;test syntax
       lda char2-1,x
       beq as240
       jsr tstrx       	;test more syntax
as240
       dex
       bne as210
       beq as300
as250
       jsr tst2        	;test a word,4 chars
       jsr tst2
as300
       lda t1          	;check # chars of both
       cmp tmpc
       beq as310       	;match, skip
       jmp tst05       	;fail
as310
       ldy length
       beq as500       	;if only 1 byte instr skip
       lda t1+1        	;get saved format
       cmp #$9d        	;a relative instr?
       bne as400       	;no-skip

       lda t0          	;calculate a relative
       sbc t2          	;(.c=1 already)
       sta diff
       lda t0+1
       sbc t2+1

       bcc as320
       bne aerr        	;out of range
       ldx diff
       bmi aerr
       bpl as340
as320
       tay
       iny             	;out of range,y=$ff
       bne aerr
       ldx diff
       bpl aerr
as340
       dex             	;subtract 2 for instr
       dex
       txa
       ldy length      	;set index to length
       bne as420       	;branch always

as400
       lda t0-1,y      	;no-put byte out there
as420
       sta (t2),y
       dey
       bne as400
as500
       lda wrap        	;get good op code
       sta (t2),y
       jsr cronly      	;get ready to overstrike line
       ldx #msgasm     	;print 'a(sp)'
       jsr msgxxx
       jsr dis400      	;disassemble one line

       inc length
       lda length
       jsr addt2       	;update address

       lda #'a         	;set up next line with 'a nnnn ' for convience
       sta keyd        	;put it in the keyboard buffer
       lda #$20
       sta keyd+1
       sta keyd+6
       lda t2+1
       jsr makhex
       sta keyd+2
       stx keyd+3
       lda t2
       jsr makhex
       sta keyd+4
       stx keyd+5
       lda #7
       sta ndx
       jmp main



;  test char in .a with char in hulp
;
tst2
       jsr tstrx       	;do two tests
tstrx
       stx sxreg
       ldx tmpc        	;get current position
       cmp hulp,x      	;same char
       beq tst10       	;yes-skip
       pla             	;pull jsr off stack
       pla

tst05
       inc wrap        	;try next trial
       beq aerr        	;=0 tried all,sorry
       jmp as110

aerr
       jmp error

tst10
       inx
       stx tmpc
       ldx sxreg       	;restore x
       rts



;  character test
;  test for char between 0-f
;  if 0<=char<=f then carry=0
;
chrtst
       cmp #'a
       bcc chr10       	;must be 0-9
       cmp #'g
       rts
chr10
       cmp #'0
       bcc chr20       	;error
       cmp #':
       rts



rdob2
       jsr hexit
       asl a
       asl a
       asl a
       asl a
       sta bad
       jsr gnc
       jsr hexit
       ora bad
chr20
       sec
       rts



hexit
       cmp #$3a
       php
       and #$0f
       plp
       bcc hex09
       adc #8
hex09
       rts

;end

