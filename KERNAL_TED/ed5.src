	.page
	.subttl 'ed.5    ted 12/09/83'
;************************************************************
;*
;*  routines involved in executing escape functions
;*
;************************************************************

;  escape sequence handler
;  entry: character following escape character in .a

escape
       and #$7f
       sec
       sbc #'a         	;table begins at ascii 'a' & ends at 'w'
       cmp #$17        	;'w'-'a'+1
       bcs none        	;invalid char...ignore it!

escgo		  	;get address of escape routine, push it, & rts to it
       asl a           	;multiply index by 2
       tax
       lda escvct+1,x  	;get high byte
       pha
       lda escvct,x    	;and low
       pha
none
       rts             	;and go to that address, if any


escvct
       .word auton-1   	;a auto insert
       .word sethtb-1  	;b set bottom
       .word autoff-1  	;c cancel auto insert
       .word dline-1   	;d delete line
       .word none-1    	;e select non-flashing cursor
       .word none-1    	;f flashing cursor
       .word none-1    	;g enable bell
       .word none-1    	;h disable bell
       .word iline-1   	;i insert line
       .word fndsol-1  	;j move to start of line
       .word fndend-1  	;k move to end of line
       .word scrsw0-1  	;l enable scrolling
       .word scrsw1-1  	;m disable scrolling
       .word setbig-1  	;n normal size screen (40 x 25)
       .word toqm-1    	;o cancel insert,quote, and reverse
       .word etstol-1  	;p erase to start of line
       .word etol-1    	;q erase to end of line
       .word setsml-1  	;r reduced size screen (38 x 23)
       .word none-1    	;s solid cursor (not underscore)
       .word sethtt-1  	;t set top of page
       .word none-1    	;u underscore cursor
       .word suup-1    	;v scroll up
       .word sddn-1    	;w scroll down
	.page
;*****************************
;
;  window modes
;
;*****************************

setsml		 	;set small screen (38 x 23)
       jsr sreset      	;first be sure entire...
       jsr clsr        	;...screen is clear & un-wrapped
       lda #1
       tax
       jsr sttop       	;set top at 1,1
       lda #nlines-2
       ldx #llen-2
       jsr stbot       	;set bottom at 39,23
       jmp home        	;put cursor within window & rts

sethtt
       clc             	;set top of window
       .byte $24
sethtb
       sec             	;set bottom of window
       ldx pntr
       lda tblx
       bcc sttop

stbot
       sta scbot
       stx scrt
       jmp rewrap

sreset		 	;reset screen to full window
       lda #nlines-1   	;max # of rows
       ldx #llen-1     	;max # of columns
       jsr stbot
       lda #0
       tax             	;fall thru to set top

sttop
       sta sctop       	;set top of window
       stx sclf

rewrap
       lda #0          	;make all lines non-continued
       ldx #4
rewra1
       sta bitabl-1,x
       dex
       bne rewra1
       rts



;*****************************
;
;  insert line
;
;*****************************

iline
       jsr scrdwn      	;insert a blank line
       jsr stu10       	;move to start of line
       inx
       jsr getbt1
       php
       jsr putbit      	;set continuation same as in previous line
       plp
       bcs linrts      	;skip if was wrapped
       sec
       ror lsxp        	;set flag - new line
linrts
       rts



;**************************
;
; delete line
;
;**************************

dline
       jsr fistrt      	;find start of line
       lda sctop       	;save current of window
       pha
       lda tblx        	;make 1st display line top of window
       sta sctop
       lda logscr      	;make sure logical scrl is off
       pha
       lda #$80
       sta logscr
       jsr scru15      	;scroll the top line away
       pla
       sta logscr
       lda sctop       	;make old 1st line of this 1 current
       sta tblx
       pla
       sta sctop
       sec
       ror lsxp        	;set flag - new line
       jmp stu10       	;make this line the current one



;******************************
;
; erase to end of line
;
;******************************

etol
       jsr savpos
etol2
       jsr clrprt      	;blank rest of line
       inc tblx        	;move to next line
       jsr stupt
       ldy sclf
       jsr getbit      	;check if next is wrapped line
       bcs etol2       	;yes - blank next line
etout
       jmp delout      	;exit and restore original position



;*****************************
;
; erase to start of line
;
;*****************************

etstol
       jsr savpos
etsto2
       jsr doblnk      	;do a blank
       cpy sclf        	;done a line ?
       bne ets100      	;no
       jsr getbit      	;at top of line
       bcc etout       	;yes - exit
ets100 jsr bakchr      	;back up
       bcc etsto2      	;always



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
       jmp etout       	;always



;*****************************
;
; scroll down
;
;*****************************

sddn
       jsr savpos
       jsr getbit
       bcs sddn2
       sec
       ror lsxp        	;set flag - left line
sddn2
       lda sctop
       sta tblx        	;scroll from screen top
       jsr scrdwn
       jsr clrbit      	;make first line non-continued
       jmp etout       	;always



;********************************
;
; scrolling enable/disable
;
;********************************

scrsw0
       lda #0          	;enable scrolling
       .byte $2c
scrsw1
       lda #$80        	;disable scrolling
       sta scrdis
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

;end
