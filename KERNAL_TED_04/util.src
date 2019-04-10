	.page
	.subttl 'util'
;  parse entry when 1st char has already been read
;
pargot
       dec chrptr

;  get a number in t0,t0+1. z set if no value found,
;  c set if end of line. y is preserved

parse
       lda #0
       sta t0
       sta t0+1
       sta syreg       	;flag for valid number

par005
       jsr gnc
       beq par040
       cmp #$20
       beq par005
par006
       cmp #$20
       beq par030
       cmp #',
       beq par030

       cmp #'0         	;check if hex, and convert
       bcc parerr
       cmp #'g
       bcs parerr
       cmp #':         	;'9'+1
       bcc par010
       cmp #'a
       bcc parerr
       sbc #8          	;adjust if in a..f
par010
       sbc #$2f        	;adjust to 00..0f
       asl a
       asl a           	;shift nibble left
       asl a
       asl a
       ldx #4          	;mult old val by 16,add new
par015
       asl a
       rol t0
       rol t0+1
       dex
       bne par015

       inc syreg       	;make syreg non-zero
       jsr gnc
       bne par006      	;keep going if not e-o-l
par030
       lda syreg       	;set z flag if a real number
       clc             	;flag as number
par040
       rts
parerr
       pla             	;pop this call
       pla
       jmp error



;  print t2 as 4 hex digits: .x destroyed, .y preserved
;
putt2
       lda t2
       ldx t2+1
putwrd
       pha
       txa
       jsr puthex
       pla
puthxs
       jsr puthex
putspc
       lda #$20
       .byte $2c
putqst
       lda #'?
       jmp bsout



;  print .a as 2 hex digits
;
puthex
       stx sxreg
       jsr makhex
       jsr bsout
       txa
       ldx sxreg
       jmp bsout



;  convert .a to 2 hex digits & put msb in .a, lsb in .x
;
makhex
       pha
       jsr makhx1
       tax
       pla
       lsr a
       lsr a
       lsr a
       lsr a
makhx1
       and #$0f
       cmp #$0a
       bcc makhx2
       adc #6
makhx2
       adc #'0
       rts



cronly
       lda #145        	;cursor up
       jsr bsout
crlf
       lda #$0d
       jmp bsout



;  get next character: return in .a (return $00 if buffer empty)
;
gnc
       stx sxreg
       ldx chrptr
       cpx bufend
       bcs gnc99
       lda buf,x
       cmp #':         	;eol-return with z=1
       beq gnc99
       inc chrptr
gnc98
       php
       ldx sxreg
       plp
       rts
gnc99
       lda #0
       beq gnc98



;  move t0,t0+1 to t2,t2+1
;
t0tot2
       lda t0
       sta t2
       lda t0+1
       sta t2+1
       rts



;  subtract t2 from t0, result in t0
;
sub0m2
       sec
       lda t0
       sbc t2
       sta t0
       lda t0+1
       sbc t2+1
       sta t0+1
       rts



;  decrement t0
;
dect0
       lda #1
subt0
       sta sxreg       	;subtract .a from t2
       sec
       lda t0
       sbc sxreg
       sta t0
       lda t0+1
       sbc #0
       sta t0+1
       rts

;  decrement t1
;
dect1
       sec
       lda t1
       sbc #1
       sta t1
       lda t1+1
       sbc #0
       sta t1+1
       rts


inct2
       lda #1          	;increment t2
addt2
       clc             	;add .a to t2
       adc t2
       sta t2
       bcc addt2r
       inc t2+1
addt2r
       rts


;  read a range - put sa in t2, count in t1
;
range
       bcs rang99      	;no defaults
       jsr t0tot2
       jsr parse       	;get ea
       bcs rang99
       jsr sub0m2
       lda t0          	;move t0 to t1
       sta t1
       lda t0+1
       sta t1+1
       clc             	;flag ok
rang99
       rts


savaxy
       sta savea
savxy
       stx savex
       sty savey
       rts

rstaxy
       lda savea
rstxy
       ldx savex
       ldy savey
       rts
	.page
;  test stop key in a sane manner & cause no reg alterations...
;
;  .c=0 => no stopkey,     .c=1 => stopkey pressed

tstsky
       stx xstop       	;save
       jsr ud60        	;do actual stopkey test
       ldx xstop       	;restore
       eor #$80
       asl a
       lda #0
       rts



;     *** print immediate ***
;  a jsr to this routine is followed by a immediate ascii string,
;  terminated by a $00. the immediate string must not be longer
;  than 255 characters including the terminator.
;
primm
       pha             	;protect a,x,y
       tya
       pha
       txa
       pha
       tsx             	;get sp
       inx             	;make it point to return addr low
       inx
       inx
       inx

       lda $0100,x     	;get it
       sta imparm      	;in base ptr.
       inx             	;point to hi
       lda $0100,x     	;get it
       sta imparm+1    	;in base ptr.
       inc imparm      	;this was actually a return addr minus 1
       bne ipskip      	;...note that a string of only a $0 will work
       inc imparm+1
ipskip
       ldy #0          	;we're pointing to 1st byte of string
emsg
       lda (imparm),y  	;loop to output string
       beq priend
       jsr bsout
       iny
       bne emsg

priend 			;shove true return addr in the stack
       tya             	;y has offset to add onto imparm base
       tsx
       inx             	;x points to ret adr lo
       inx
       inx
       inx

       clc
       adc imparm
       sta $0100,x     	;new lo ret adr
       lda #0
       adc imparm+1
       inx
       sta $0100,x     	;new hi ret adr

       pla
       tax
       pla
       tay
       pla
       rts


iobase
       ldx #$00        	;return sa of i/o page
       ldy #$fd
       rts

;end

