	.page
	.subttl 'ed1    ted 07/10/84'

	.byte   >dclver,<dclver	;mark declare version

ldtb2			;screen lines low byte table
       .byte <linz0	;**must** be at $d802...if this is changed, tell basic!
       .byte <linz1
       .byte <linz2
       .byte <linz3
       .byte <linz4
       .byte <linz5
       .byte <linz6
       .byte <linz7
       .byte <linz8
       .byte <linz9
       .byte <linz10
       .byte <linz11
       .byte <linz12
       .byte <linz13
       .byte <linz14
       .byte <linz15
       .byte <linz16
       .byte <linz17
       .byte <linz18
       .byte <linz19
       .byte <linz20
       .byte <linz21
       .byte <linz22
       .byte <linz23
       .byte <linz24
ldtb1			;screen lines high byte table
       .byte >linz0
       .byte >linz1
       .byte >linz2
       .byte >linz3
       .byte >linz4
       .byte >linz5
       .byte >linz6
       .byte >linz7
       .byte >linz8
       .byte >linz9
       .byte >linz10
       .byte >linz11
       .byte >linz12
       .byte >linz13
       .byte >linz14
       .byte >linz15
       .byte >linz16
       .byte >linz17
       .byte >linz18
       .byte >linz19
       .byte >linz20
       .byte >linz21
       .byte >linz22
       .byte >linz23
       .byte >linz24
	.page
scrorg			;return max. # rows, cols of screen
       ldx #llen
       ldy #nlines
       rts


plot			;set or read cursor position
       bcs plot10
       stx tblx		;.c=0 means set it
       stx lsxp
       sty pntr
       sty lstp
       jsr sreset	;(in case it's outside window)
       jsr stupt
plot10
       ldx tblx		;.c=1 means read it
       ldy pntr
       rts



;****************************************
;
; cint:  initialize screen & editor
;
;****************************************

cint
       lda #$0c		;set up base of screen
       sta hibase

       lda #3
       sta dflto
       lda #0
       sta dfltn

       sta mode		;always pet mode
       sta graphm	;always text mode
       sta ndx		;no keys in buffer yet
       sta stpflg	;flag 'no ctl-s yet'

       lda #<shflog	;set shift logic indirects
       sta keylog
       lda #>shflog
       sta keylog+1
       lda #10
       sta xmax		;maximum type ahead buffer size
       sta rolflg	;flag 'roll ok, if in basic, and if in direct mode'
       sta delay
       lda #$80
       sta rptflg	;make all keys repeat
       lda #16		;init color to gk. blue
       sta color
       lda #4
       sta kount	;delay between key repeats

setbig			;setup full screen window, clear it & clear wrap table
       jsr sreset
clsr			;clear screen
       jsr home		;start at top of window
cls10
       jsr scrset	;point to line
       jsr clrln	;clear the line
       cpx scbot	;done?
       inx
       bcc cls10	;no

home			;home cursor
       ldx sctop	;move to top of window
       stx tblx
       stx lsxp        	;(for input after home or clear)
stu10
       ldy sclf        	;move to left side of window
       sty pntr
       sty lstp
stupt
       ldx tblx        	;set pointers to beginning of line

scrset
       lda ldtb2,x     	;.x=line # to set up
       sta pnt
       lda ldtb1,x     	;pointer to screen ram
       sta pnt+1       	

;pointer to screen ram, fall into 'scolor'
;
scolor
       lda pnt         	;generate color pointer
       sta user
       lda pnt+1
       and #$03
       ora #>tedatr    	;address of ted attribute byte area
       sta user+1
       rts


;  remove character from queue
;
; ********** commented code here replaced with that following
;	this is a REV  5 change.
;
;	10 getkey a$:print a$:goto 10
;	rem this line reacted horribly when function keys depressed.
;
;lp2
;       ldy kyndx       	;are there any pf keys?
;       beq lp3         	;branch if not
;       ldy keyidx      	;get index to current char
;       lda pkybuf,y    	;get current byte
;       dec kyndx       	;1 byte down
;       inc keyidx      	;bump index to next char
;       cli
;       rts
;lp3
;       ldy keyd        	;get key from irq buffer
;       ldx #0
;
;
lp2
       LDX KYNDX       	;are there any pf keys?
       BEQ LP3         	;branch if not
       LDY KEYIDX      	;get index to current char
       LDA PKYBUF,Y    	;get current byte
       DEC KYNDX       	;1 byte down
       INC KEYIDX      	;bump index to next char
       CLI
       CLC
       RTS
LP3
       LDY KEYD        	;get key from irq buffer
       NOP
;
;*********	end of rev 5 change
;
lp1
       lda keyd+1,x
       sta keyd,x
       inx
       cpx ndx
       bne lp1
       dec ndx
       tya
       cli
       clc             	;always good return from keybd!
       rts

loop4
       jsr print
loop3			;turn on cursor
       jsr scolor      	;set up (user) to point to start of current attr bytes row
       ldy pntr        	;get column
       lda (user),y     	;get old color
       pha             	;save it

       lda color       	;make cursor flash in current color
       sta (user),y

       tya             	;calculate current screen position
       clc
       adc pnt
       sta tedcrl      	;point cursor there
       lda pnt+1
       adc #0          	;(.c will be clear for sbc following!)
       sbc #>tedscn-1  	;ted crsr posn. is relative to start of screen
       sta tedcrh

waitky
       lda ndx         	;are there any keys ready?
       ora kyndx       	;or any in the pf key buffer?
       beq waitky      	;loop if not

;  turn off cursor
;
       pla             	;get saved color
       sta (user),y     	;put it in
       lda #$ff        	;point cursor to cuba
       sta tedcrh      	;(it's the only way to turn it off)
       sta tedcrl
       jsr lp2         	;get key input

       cmp #$83        	;<shift><run/stop>?
       bne lp22
       ldx #9
       sei             	;fill buffer with load/run cmds
       stx ndx
lp21
       lda runtb-1,x
       sta keyd-1,x
       dex
       bne lp21
lp211
       beq loop3

lp22
       cmp #cr       	;<cr>?
       bne loop4
       sta crsw        	;flag - we pass chars now
       jsr fndend      	;check nxt line for cont
       stx lintmp      	;save last line number of sentence
       jsr fistrt      	;find begining of line
       lda #0
       sta qtsw        	;clear quote mode
       ldy sclf        	;retrieve from line start if left it
       lda lsxp        	;input started row
       bmi lp80        	;flag we left start line
       cmp tblx
       bcc lp80
       ldy lstp        	;input started column
       cmp lintmp      	;on start line
       bne lp70
       cpy indx        	;past start column
       beq lp75        	;ok if the same
lp70
       bcs clp2        	;yes - null input
lp75
       sta tblx        	;start from here on input
lp80
       sty pntr
       jmp lop5


;  input a line until carriage return
;
loop5
       tya
       pha
       txa
       pha
       lda crsw        	;passing chars to input
       beq lp211       	;no - buffer on screen (jmp's to loop3)
       bpl lop5        	;not done - get next char
clp2
       lda #0          	;input done clear flag
       sta crsw
       jmp clppat      	;**patch 01/03/84 fab
       nop             	;**
lop5
       jsr stupt       	;set pnt and user
       jsr get1ch      	;get a screen char
       sta datax
       and #$3f
       asl datax
       bit datax
       bpl lop54
       ora #$80
lop54
       bcc lop52
       ldx qtsw
       bne lop53
lop52
       bvs lop53
       ora #$40
lop53
       jsr qtswc
       ldy tblx        	;on input end line ?
       cpy lintmp
       bcc clp00       	;no
       ldy pntr        	;on input end column ?
       cpy indx
       bcc clp00       	;no
       ror crsw        	;c=1 minus flags last char sent
       bmi clp1        	;always

clp00
       jsr nxtchr      	;at next char
clp1
       cmp #$de        	;a pi ?
       bne clp7        	;no
       lda #$ff        	;translate
clp7
       sta datax
       pla
       tax
       pla
       tay
       lda datax
       clc             	;**patch 12/22/83 tvr
       rts


qtswc
       cmp #$22
       bne qtswl
       lda qtsw
       eor #$1
       sta qtsw
       lda #$22
qtswl
       rts


loop2
       lda datax
       sta lstchr      	;save for next escape test
       pla
       tay
       lda insrt
       beq lop2
       lsr qtsw
lop2
       pla
       tax
       pla
       clc             	;good return
;      cli		;shouldn't be necessary here
       rts


nxt33
       ora #$40
nxt3
       ldx rvs
       beq nvs
nc3
       ora #$80
nvs
       ldx insrt
       beq nvsa
       dec insrt
nvsa
       bit insflg      	;are we in auto insert?
       bpl nvs1        	;branch if not
       pha             	;save char.
       jsr insert      	;make room
       ldx #0
       stx insrt       	;make sure insert flag is off
       pla
nvs1
       jsr dspp        	;fall thru to movchr! (will return to 'loop2')
	.page
; movchr - move to next char position
; insert blank line if at end of line
; y = column position
; on exit - carry set = abort - scroll disabled
;
movchr
       cpy scrt
       bcc movc10      	;easy if not at end of line
       ldx tblx
       cpx scbot
       bcc movc10      	;skip if not last line of screen
       bit scrdis
       bmi movc30      	;abort if scrolling disabled
movc10
       jsr stupt       	;set pnt address
       jsr nxtchr      	;move to next char position
       bcc movc30      	;done if not move to new line
       jsr getbit      	;check if on a continued line
       bcs movc20      	;skip ahead if not
       sec             	;incase we abort
       bit scrdis
       bvs movc30
       jsr scrdwn      	;else insert a blank line
movc20
       clc             	;for clean exit
movc30
       rts


; skip to next line
; wrap to top if scroll disabled
;
nxln
       ldx tblx
       cpx scbot       	;of the bottom of window ?
       bcc nxln1       	;no
       bit scrdis      	;what if scrolling is disabled?
       bpl doscrl      	;branch if scroll is enabled
       lda sctop       	;wrap to top
       sta tblx
       bcs nowhop      	;always

doscrl
       jsr scrup       	;scroll it all
       clc             	;indicate scroll ok
nxln1
       inc tblx
nowhop
       jmp stupt       	;set line base adr

;end
;(07/10/84) tvr & fab:  pressing a fct key during a 'getkey' cmd returned
;			a error and shouldn't have. code needed a 'clc'.
