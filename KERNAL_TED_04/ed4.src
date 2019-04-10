	.page
	.subttl   'ed.4    ted 01/16/84'
; print a character on the screen
;
print
       sta datax       	;save a copy of character
       pha             	;save reg's
       txa
       pha
       tya
       pha

print1
       lda stpflg      	;is there a pause going on?
       bne print1      	;if so, loop until the keyboard routine clears it.
       sta crsw

lopwrk = loop2-1

       lda #>lopwrk    	;push 'loop2' onto stack for common return mechanism
       pha
       lda #<lopwrk
       pha

       ldy pntr
       lda datax
       cmp #cr        	;if <cr> or <shift><cr> jump out now
       beq nxt1
       cmp #$8d
       beq nxt1
       ldx lstchr      	;see if last char. was an <escape>
       cpx #$1b
       bne print3      	;no
       jmp escape      	;go service <escape> & rts to loop2
print3
       tax             	;set flags per last char.
       bmi goshft      	;see if last char. was shifted

       cmp #$20		;is this a control chr?	
       bcc ctlchr      	;yes

       cmp #$60        	;lower case?
       bcc print2      	;no
       and #$df        	;yes, convert to pet ascii
       bne print4      	;always
print2
       and #$3f
print4
       jsr qtswc       	;if quote char, toggle quote switch
       jmp nxt3        	;put char on screen, do rts

goshft
       jmp shiftd      	;go service a shifted char.

nxt1
       jsr fndend      	;find the end of the current line
       inx
       jsr clrbit      	;set next line as non-continued
       ldy sclf        	;and point to start of next line
       sty pntr
       jsr nxln        	;set up next line & fall thru to 'togm'


toqm
       lda #0          	;turn off modes
       sta insrt
       sta rvs
       sta qtsw
       sta flash
       rts


ctlchr
       cmp #$1b        	;is char. 'escape'?
       beq chk3        	;yes, ignore & just quit
       ldx insrt       	;are we in insert mode?
       beq ctlc10      	;no.
ctlc05
       jmp nc3         	;yes, go make reverse, and print it

ctlc10
       cmp #$14        	;is char. 'delete'?
       bne ctlc20      	;no
       jmp delete      	;yes...service & rts to loop2

ctlc20
       ldx qtsw        	;are we in quote-mode?
       bne ctlc05      	;yes, make reverse, and print it

       cmp #$12        	;is char 'reverse on'?
       bne ctlc30      	;no.
       sta rvs         	;yes, set reverse flag

ctlc30
       cmp #$13        	;is char. 'home'?
       bne ctlc40      	;no
       cmp lstchr      	;deja vu?
       bne ctlc35      	;no
       jsr sreset      	;yes- reset window to max.
ctlc35
       jmp home        	;yes...service & rts to loop2

ctlc40
       cmp #$1d        	;is char 'cursor right'?
       beq crsrrt      	;yes

       cmp #$11        	;is char. 'cursor down'?
       beq crsrdn      	;yes

ctlc60
       cmp #$0e        	;is char. 'set lower case'?
       beq lower       	;yes

       cmp #$08        	;is char. 'lock in this mode'?
       beq lock        	;yes

       cmp #$09        	;is char. 'unlock keyboard'?
       beq unlock      	;yes

ctlc90
       ldx #15         	;test if one of 16 colors
chk1
       cmp coltab,x
       beq chk2
       dex
       bpl chk1
       rts
chk2
       pha
       jsr chkpat      	;***patch 01/16/84 fetch color & lum from ram (or rom) table
       sta color
       pla
chk3
       rts             	;done, return to loop2


crsrrt
       jsr nxtchr      	;cursor right
       bcs cdn10       	;branch if wrap
       rts


crsrdn
       jsr nxln        	;cursor down
cdn10
       jsr getbit      	;a wrapped line?
       bcs cdrts
       sec
       ror lsxp        	;flag we left line
cdrts
       clc
critgo
       rts             	;return to loop2 (sometimes)


crsrup
       ldx sctop       	;cursor up
       cpx tblx        	;at top of window?
       bcs critgo      	;yes...do nothing
cup10
       jsr cdn10       	;about to wrap?
       dec tblx        	;up a line
       jmp stupt       	;& rts to loop2


cleft
       jsr bakchr      	;move back
       bcs critgo      	;abort if at top left
       bne cdrts       	;no - exit
       inc tblx
       bne cup10       	;go set flag if needed


lower			;set lower character set
       lda tedcbr      	;get character base reg.
       ora #cbrlsb     	;set lsb (point to lower char set)
       bne upper2      	;always


lock
       lda #$80        	;lock keyboard in current mode
       ora mode        	;set lock bit on
       bmi ulock2      	;always


unlock
       lda #$7f        	;unlock keyboard
       and mode
ulock2
       sta mode
       rts             	;return to loop2


upper			;set upper charscter set
       lda tedcbr      	;get character base reg.
       and #$ff-cbrlsb 	;clear lsb (point to upper case)
upper2
       sta tedcbr
       rts             	;return to loop2



shiftd			;process shifted keycodes
       and #$7f        	;for some obscure reason
       cmp #$7f
       bne shft10
       lda #$5e

shft10
       cmp #$20        	;is it a function key?
       bcc shft15      	;yes
       jmp nxt33       	;no, print it

shft15
       ldx qtsw        	;are we in quote mode?
       beq shft25      	;no
shft21
       ora #$40        	;make reverse
       jmp nc3         	;...and print it.

shft25
       cmp #$14        	;is char. 'insert'?
       beq insert      	;yes...service & rts to loop2

shft30
       ldx insrt       	;are we in insert mode?
       bne shft21      	;yes, make reverse, and print.

       cmp #$11        	;is char. 'cursor up'?
       beq crsrup      	;yes

       cmp #$12        	;is char. 'reverse off'?
       bne shft40      	;no
       lda #0
       sta rvs

shft40
       cmp #$1d        	;is char. 'cursor left'?
       beq cleft       	;yes

       cmp #$13        	;is char. 'clear screen'?
       bne shft50
       jmp clsr        	;yes...service & rts to loop2

shft50
       cmp #$02        	;is char. 'flash on'?
       bne shft55      	;no
       lda #$80
       sta flash

shft55
       cmp #$04        	;is char. 'flash off'?
       bne shft60      	;no
       lda #0
       sta flash

shft60
       cmp #$0e        	;is char. 'set upper char set'?
       beq upper       	;yes

       ora #$80        	;restore msb
       jmp ctlc90      	;test for color, return


delete
       jsr cleft       	;move back 1 position
       jsr savpos      	;save column & row positions
       bcs delout      	;abort if at top left corner
deloop
       cpy scrt        	;at right margin?
       bcc delop1      	;no - skip ahead
       ldx tblx
       inx
       jsr getbt1      	;is next line a wrapped line?
       bcs delop1      	;yes - continue with delete
       jsr doblnk      	;no - blank last character
delout
       lda sedt1       	;restore column and row positions
       sta pntr
       lda sedt2
       sta tblx
       jmp stupt       	;restore pnt & rts to loop2
delop1
       jsr nxtchr
       jsr get1ch      	;get next character
       jsr bakchr
       jsr dsppc       	;move it back 1 position
       jsr nxtchr      	;move up 1 position
       jmp deloop      	;loop until at end of line


; insert a character
;
insert
       jsr savpos      	;save column & row positions
       jsr fndend      	;move to last char on the line
       cpx sedt2       	;last row equal to starting row?
       bne ins10       	;no - skip ahead
       cpy sedt1       	;is last position before starting position?
ins10
       bcc ins50       	;yes - no need to move anything
       jsr movchr      	;move to next char position
       bcs insout      	;abort if scroll needed but disabled
ins30
       jsr bakchr
       jsr get1ch      	;move char forward 1 position
       jsr nxtchr
       jsr dsppc
       jsr bakchr
       ldx tblx
       cpx sedt2       	;at original position
       bne ins30
       cpy sedt1
       bne ins30       	;no - loop till we are
       jsr doblnk      	;insert a blank
ins50
       inc insrt       	;inc insert count
       bne insout      	;only allow up to 255
       dec insrt
insout
       jmp delout      	;restore original position

;end
