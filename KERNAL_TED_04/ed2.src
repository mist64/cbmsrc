	.page
	.subttl   'ed2    ted 01/17/84'
; ****** scroll routines ******
;
movlin
       lda ldtb2,x
       sta sedeal
       sta sedsal
       lda ldtb1,x
       sta sedsal+1
       and #$03
       ora #>tedatr
       sta sedeal+1
movl10
       lda (sedsal),y  	;move character byte
       sta (pnt),y
       lda (sedeal),y  	;move color byte
       sta (user),y
       cpy scrt        	;done a whole line ?
       iny
       bcc movl10      	;no
       rts


; ****** scroll down ******
;
scrdwn
       ldx lsxp
       bmi scd30       	;skip if new line flag already set
       cpx tblx
       bcc scd30       	;skip if old line is below scroll area
       inc lsxp        	;else inc start line number
scd30
       ldx scbot       	;scroll down, start bottom
scd10
       jsr scrset      	;set 'pnt' to line
       ldy sclf
       cpx tblx        	;test if at destination line
       beq scd20       	;done if yes
       dex             	;point to previous line as source
       jsr getbt1
       inx
       jsr putbt1      	;move continuation byte
       dex
       jsr movlin      	;move one line
       bcs scd10       	;always
scd20
       jsr clrln       	;set line to blanks
       jmp setbit      	;mark as continuation line


; ****** scroll up ******
;
scrup
       ldx sctop
scru00
       inx
       jsr getbt1      	;find first non-continued line
       bcc scru15
       cpx scbot
       bcc scru00
       ldx sctop
       inx
       jsr clrbit      	;clear to only scroll 1 line
scru15
       dec tblx
       bit lsxp
       bmi scru20      	;no change if already new line
       dec lsxp        	;move input up one
scru20
       ldx sctop
       cpx sedt2
       bcs scru30
       dec sedt2       	;in case doing insert
scru30
       jsr scr10       	;scroll
       ldx sctop
       jsr getbt1
       php
       jsr clrbit      	;make sure top line is not continuation
       plp
       bcc scru10      	;done if top line off
       bit logscr      	;logical scroll ?
       bmi scrup       	;no - keep scrolling
scru10
       rts


scr10
       jsr scrset
       ldy sclf
       cpx scbot       	;at last line ?
       bcs scr40       	;yes
       inx             	;point to next line
       jsr getbt1
       dex
       jsr putbt1      	;move continuation byte
       inx
       jsr movlin      	;move one line
       bcs scr10

scr40
       jsr clrln       	;make last line blank
       lda #$7f        	;check for slo-scroll screen
       jsr keyscn      	;**01/17/84 mod
       cmp #$df
       bne scr80       	;no commodore key
       ldy #0          	;delay
scr60
       nop
       dex
       bne scr60
       dey
       bne scr60
scr80
       rts
	.byte   $ea,$ea,$ea



;****************************************************
; clear one line subroutine
; enter with x=line number
;     entry at clrln - clear entire line
;              clrprt - y=starting column number
;****************************************************
;
clrln
       ldy sclf
       jsr clrbit      	;make sure non-continued line
clrprt
       jsr scrset
       dey
clr10
       iny
       lda #' '
       sta (pnt),y     	;print space
       lda color
       sta (user),y    	;update color ram
       cpy scrt
       bne clr10
       rts

;end
