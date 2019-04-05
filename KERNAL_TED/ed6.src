	.page
	.subttl 'ed.6    ted 12/09/83'
; grab a character

get1ch
       ldy pntr        	;get char/color index
       lda (user),y    	;get the color
       sta tcolor
       lda (pnt),y     	;get the character
       rts

;  wrap table subroutines
;
getbit
       ldx tblx
getbt1
       jsr bitpos      	;get byte & bit positions
       and bitabl,x
       cmp #1          	;make carry clear if zero
       jmp bitout

; putbit - set bit according to carry
;
putbit
       ldx tblx
putbt1
       bcs setbit      	;go if to mark as wrappped line

; clrbit - clear wrap bit
;
clrbit
       jsr bitpos      	;get byte & bit positions
       eor #$ff        	;invert bit position
       and bitabl,x    	;clear bit
bitsav
       sta bitabl,x
bitout
       ldx bitmsk
       rts

; setbit  -  set bit to mark as wrapped line
;
setbit
       bit scrdis      	;check for line-link disable
       bvs getbt1
       jsr bitpos      	;get byte & bit position
       ora bitabl,x    	;set wrap bit
       bne bitsav      	;always

; bitpos - get byte & bit position of wrap bit
;          input - x = row number
;          output - x = byte number
;                   a = bit mask

bitpos
       stx bitmsk
       txa
       and #$07        	;get bit position
       tax
       lda scbits,x    	;get bit mask
       pha
       lda bitmsk
       lsr a
       lsr a           	;shift to get byte position
       lsr a
       tax
       pla
       rts

scbits .byte $80,$40,$20,$10,$08,$04,$02,$01

; ****** move to end/start of line
;
fndsol
       ldy sclf        	;will move to start of line...
       sty pntr        	;set to leftmost column


; ****** find beginning of line
;
fistrt
       jsr getbit      	;find start of current line
       bcc fnd0        	;branch if found
       dec tblx        	;up a line
       bpl fistrt      	;always
       inc tblx        	;whoops went too far
fnd0
       jmp stupt       	;set line base adr


; ****** find last non-blank char of line
;
; pntr= column #
; tblx= line #

fndend
       inc tblx
       jsr getbit      	;is this line continued
       bcs fndend      	;branch if so
       dec tblx        	;found it - compensate for inc tblx
       jsr stupt
       ldy scrt        	;get right margin
       sty pntr        	;point to right margin
eloup2
       jsr get1ch
       cmp #$20
       bne endbye      	;yes
       cpy sclf        	;are we at the left margin?
       bne eloup1      	;no- keep going
       jsr getbit      	;are we on a wrapped line?
       bcc endbye      	;no- get out
eloup1
       jsr bakchr
       bcc eloup2      	;ok- not at top left
endbye
       sty indx        	;remember this
       rts


; ****** move to next char
;
; scroll if enabled
; wrap to top if disabled

nxtchr
       pha
       ldy pntr
       cpy scrt        	;are we at the right margin?
       bcc bumpnt      	;branch if not
	.space 1
       jsr nxln        	;point to nextline
       ldy sclf        	;point to first char of 1st line
       dey
       sec             	;set to show moved to new line
bumpnt iny             	;increment char index
       sty pntr
       pla
       rts


; ****** backup one char
;
; wrap up and stop a top left

bakchr
       ldy pntr
       dey
       bmi bakot1
       cpy sclf        	;are we at the left margin
       bcs bakout      	;no - past it
bakot1
       ldy sctop
       cpy tblx        	;are we at top line last character?
       bcs bakot2      	;leave with carry set
       dec tblx        	;else backup a line
       pha
       jsr stupt       	;set line base adr
       pla
       ldy scrt        	;move cursor to right side
bakout
       sty pntr
       cpy scrt        	;set z-flag if moved to new line
       clc             	;always clear
bakot2
       rts


; ****** save row & column position
;
savpos
       ldy pntr
       sty sedt1
       ldx tblx
       stx sedt2
       rts


doblnk
       lda #$20


dspp
       ldy pntr
       sta (pnt),y
       jsr scolor
       lda color
       ora flash
       sta (user),y
       rts


dsppc
       ldy pntr
       sta (pnt),y
       jsr scolor
       lda tcolor
       sta (user),y
       rts

;end
