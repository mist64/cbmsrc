	.page
	.subttl 'split  01/24/84'
;**********************************************************
;
;      split   -     service split screen
;
; test for 3 cases:
;  1) raster before line 20 (wait for 20, then flip if ss)
;  2) after 20, but before end of screen (exit)
;  3) after end of screen ( flip to graphics)
;
;**********************************************************

srvspl
       lda ted+28      	;check bit 8 of raster value- if set, def. eos
       and #1
       bne spls55      	;end of screen
       lda ted+29      	;now test lower 8 bits of raster value
       cmp #$a3        	;is this line 20 ($a1) or end of screen ($cc)
       bcs spls50      	;it's past line 20

       bit graphm      	;is current mode split screen?
       bvc splrts      	;no, we have no need for this interrupt

       lda #%00001000  	;point vm base at real vm area ($0800)
       sta ted+20

       lda ted+6       	;set up to display text screen
       and #%11011111  	;clear bmm
       tay             	;wait...

       lda ted+7
       and #%11101111  	;clear mcm
       tax             	;wait...

       lda ted+18      	;set 'fetch from rom' bit on.
       ora ffrmsk
       pha             	;wait...

spls30 			;wait for edge of visible screen
       lda ted+29
       cmp #$a3
       bcc spls30

       pla
       sta ted+18
       sty ted+6
       stx ted+7
       rts



spls50
       cmp #$cc        	;is this the end of screen?
       bcc splrts      	;not yet
spls55
       ldx graphm      	;what is current mode?
       beq splrts      	;branch if text
       bpl spls60      	;branch if not mcm

       lda ted+7
       ora #%00010000  	;set mcm
       sta ted+7

spls60
       lda ted+6
       ora #%00100000  	;set bmm
       sta ted+6

       lda ted+18      	;turn off 'fetch from rom' bit
       and #%11111011
       sta ted+18

       lda vmbmsk      	;point vmbase at bit map vm
       sta ted+20
splrts
       rts

	.byte   $ea,$ea,$ea,$ea,$ea,$ea,$ea,$ea

;end

