	.page
	.subttl  ed5  escape handler  (05/06/85)

;/////////////   E S C A P E     F U N C T I O N S   \\\\\\\\\\\\\\

;  escape sequence handler.   entry: character following <escape> in .a

escchr	jmp (escvec)	;'escape' indirect

escape	cmp #esc
	bne 1$		;...branch if not double <escape>
	lsr datax	;...else cancel <escape> sequencer by fucking up lstchr
	jmp toqm	;...and exit via 'toqm' to cancel all other modes too

1$	and #$7f
	sec
	sbc #'@'	;table begins at ascii '@' & ends at 'z'
	cmp #27		;'z'-'@'+1
	bcs escrts	;invalid char...ignore it!

	asl a		;character is index to dispatch table
	tax
	lda escvct+1,x	;get address of escape routine, push it, & rts to it
	pha
	lda escvct,x
	pha
escrts	rts

escvct	.word eeos-1	;@ erase to end of screen
	.word auton-1	;a auto insert
	.word sethtb-1	;b set bottom
	.word autoff-1	;c cancel auto insert
	.word dline-1	;d delete line
	.word crsrnf-1	;e select non-flashing cursor
	.word crsrbl-1	;f flashing cursor
	.word bellon-1	;g enable bell
	.word bellof-1	;h disable bell
	.word iline-1	;i insert line
	.word fndsol-1	;j move to start of line
	.word fndeol-1	;k move to end of line
	.word scrsw0-1	;l enable scrolling
	.word scrsw1-1	;m disable scrolling
	.word dspnml-1	;n un-reverse 8563 screen
	.word toqm-1	;o cancel insert, quote, reverse, ul & flash modes
	.word etstol-1	;p erase to start of line
	.word etol-1	;q erase to end of line
	.word dsprvs-1	;r reverse 8563 screen
	.word crsral-1	;s 8563 block cursor
	.word sethtt-1	;t set top of page
	.word crsrul-1	;u 8563 underscore cursor
	.word suup-1	;v scroll up
	.word sddn-1	;w scroll down
	.word swap-1	;x switch between 40/80 display modes
	.word taball-1	;y set default tab stops
	.word tabclr-1	;z clear all tab stops
	.page
;*****************************
;
;  window modes
;
;*****************************

sethtt
	clc		;set top of window
	.byte $24
sethtb
	sec		;set bottom of window
	ldx pntr
	lda tblx
window	bcc sttop	;//////entry from editor jump table here!


stbot
	sta scbot
	stx scrt
	jmp rewrap


sreset			;reset screen to full window
	lda lines	;max # of rows
	ldx columns	;max # of columns
	jsr stbot
	lda #0
	tax		;fall thru to set top


sttop
	sta sctop	;set top of window
	stx sclf


rewrap
	lda #0		;make all lines non-continued
	ldx #4
1$
	sta bitabl-1,x
	dex
	bne 1$
	rts

	.page
;*****************************
;
;  insert line
;
;*****************************

iline
	jsr scrdwn	;insert a blank line
	jsr stu10	;move to start of line
	inx
	jsr getbt1
	php
	jsr putbit	;set continuation same as in previous line
	plp
	bcs 1$		;skip if was wrapped
	sec
	ror lsxp	;set flag - new line
1$
	rts



;**************************
;
; delete line
;
;**************************

dline
	jsr fistrt      ;find start of line
	lda sctop       ;save current of window
	pha
	lda tblx	;make 1st display line top of window
	sta sctop
	lda scroll	;make sure logical scroll is off
	pha
	lda #$80
	sta scroll
	jsr scru15      ;scroll the top line away
	pla
	sta scroll
	lda sctop       ;make old 1st line of this 1 current
	sta tblx
	pla
	sta sctop
	sec
	ror lsxp	;set flag - new line
	jmp stu10       ;make this line the current one

	.page
;******************************
;
; erase to end of line
;
;******************************

etol
	jsr savpos
1$
	jsr clrprt      ;blank rest of line
	inc tblx	;move to next line
	jsr stupt
	ldy sclf
	jsr getbit      ;check if next is wrapped line
	bcs 1$		;yes - blank next line
etout
	jmp delout      ;exit and restore original position



;*****************************
;
; erase to start of line
;
;*****************************

etstol
	jsr savpos
1$
	jsr blank	;do a blank
	cpy sclf	;done a line?
	bne 2$		;no
	jsr getbit      ;at top of line
	bcc etout       ;yes - exit
2$	jsr bakchr	;back up
	bcc 1$		;always



;******************************
;
; erase to end of screen
;
;******************************

eeos
	jsr savpos
1$
	jsr clrprt      ;blank rest of line
	inc tblx	;move to next line
	jsr stupt
	ldy sclf
	jsr getbit      ;check if next is wrapped line
	bcs 1$		;yes - blank next line

	lda tblx
	cmp scbot	;check if at end of screen
	bcc 1$
	beq 1$

	jmp delout      ;exit and restore original position

	.page
;*****************************
;
; scroll up
;
;*****************************

suup
	jsr savpos
	txa
	pha
	jsr scrup
	pla
	sta sedt2
	jmp delout



;*****************************
;
; scroll down
;
;*****************************

sddn
	jsr savpos
	jsr getbit
	bcs 1$
	sec
	ror lsxp	;set flag - left line
1$
	lda sctop
	sta tblx	;scroll from screen top
	jsr scrdwn
	jsr clrbit      ;make first line non-continued
	jmp delout

	.page
;********************************
;
; scrolling enable/disable
;
;********************************

scrsw0
	lda #0  	;enable scrolling
	.byte $2c
scrsw1
	lda #$80	;disable scrolling
	sta scroll
	rts



;*******************************
;
;  auto insert on/off
;
;*******************************

autoff
	lda #0
	.byte $2c
auton
	lda #$ff
	sta insflg
	rts

	.page
;*******************************
;
;  cursor modes
;
;*******************************

crsral
	bit mode
	bpl crsr_c
	lda curmod	;8563 solid cursor
	and #$e0
	jmp crsr_a	;bra


crsrul
	bit mode
	bpl crsr_c
	lda curmod	;8563 underline cursor
	and #$e0
	ora #$07
	bne crsr_a	;bra


crsrnf
	bit mode	;select non-blinking cursor
	bpl crsr_d

	lda curmod	;8563 hardware cursor
	and #$1f
crsr_a	sta curmod
	jmp crsrdo

crsr_d	lda blnon	;vic cursor
	ora #$40
	bne crsr_b	;bra



crsrbl
	bit mode	;select blinking cursor
	bpl 1$

	lda curmod	;8563 hardware cursor
	and #$1f
	ora #$60
	bne crsr_a	;bra

1$	lda blnon	;vic cursor
	and #$bf
crsr_b	sta blnon
crsr_c	rts

	.page
;*******************************
;
;   bell (ctrl-g) on/off
;
;*******************************

bellon
	lda #0
	.byte $2c

bellof
	lda #$80
	sta beeper
	rts







dsprvs
	ldx #24		;reverse 8563 display
	jsr vdcin
	ora #$40
	bne dspxxx

dspnml
	ldx #24		;normal 8563 display
	jsr vdcin
	and #$bf
dspxxx	jmp vdcout




fndeol
	jsr fndend	;move to last character in line
	jmp movchr	;move past it & start a wrapped line if necessary

;.end
