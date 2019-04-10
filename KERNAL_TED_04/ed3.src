	.page
	.subttl  'ed.3  ted 01/18/84'
; ****** general keyboard scan ******
;
scnkey
       lda #0
       sta shflag
       ldy #64         	;last key index
       sty sfdx        	;null key found
       jsr keyscn      	;raise all lines  (**01/17/84 mod)
       tax             	;check for a key down
       cpx #$ff        	;no keys down?
       bne scn10       	;branch if somthing down
       jmp ckit2
scn10
       ldy #0          	;init key counter
       lda #<mode1
       sta keytab
       lda #>mode1
       sta keytab+1
       lda #$fe        	;start with 1st column
scn20
       ldx #8          	;8 row keybrd
       pha
scn22
       pla
       pha
       jsr keyscn      	;debounce (**01/17/84 mod)
       sta tmpkey
       pla
       pha
       jsr keyscn      	;(**01/17/84 mod)
       cmp tmpkey
       bne scn22
scn30
       lsr a           	;look for key down
       bcs ckit        	;none
       pha
       lda (keytab),y  	;get char code
       cmp #$05
       bcs spck2       	;if not special key go on
       cmp #$03        	;could it be a stop key?
       beq spck2       	;branch if so
       ora shflag
       sta shflag      	;put shift bit in flag byte
       bpl ckut
spck2
       sty sfdx        	;save key number
ckut
       pla
ckit
       iny
       cpy #65
       bcs ckit1       	;branch if finished
       dex
       bne scn30
       sec
       pla             	;reload column info
       rol a
       bne scn20       	;always branch
ckit1
       pla             	;dump column output...all done
       lda sfdx        	;check function keys
       jmp (keylog)    	;sent to 'shflog' at init



keyscn			;** 01/17/84 mod for new keyboard port
       sta $fd30
       sta keybrd
       lda keybrd
       rts
	.page
; shift logic
;
shflog
       lda shflag
       cmp #$03        	;commodore shift combination?
       bne keylg2      	;branch if not
       lda mode
       bmi shfout      	;don't shift if it's locked out
       lda lstshf      	;was it done recently?
       bne shfout      	;yes- ignore it

       lda tedcbr      	;character base register
       eor #cbrlsb     	;lsb of char base in cbr
       sta tedcbr      	;get characters from new area
       lda #%00001000  	
			;disallow cbm/shift again until 'lstshf'
       sta lstshf      	;...is reset by 'ckit2' shifting it right
       bne shfout      	;always

keylg2
       asl a
       cmp #8          	;was it a control key
       bcc nctrl       	;branch if not
       lda #6          	;else use table #4

       ldx stpdsb      	;is the pause disable set?
       bne nctrl       	;branch if so
       ldx sfdx        	;get the number of the key pressed
       cpx #13         	;'s'?
       bne nctrl
       stx stpflg      	;flag 'pause'
       rts

nctrl
       tax
       lda keycod,x
       sta keytab
       lda keycod+1,x
       sta keytab+1

; fall into shfout
;
shfout
       ldy sfdx        	;get key index
       lda (keytab),y  	;get char code
       tax             	;save the char
       cpy lstx        	;same as prev char index?
       beq rpt10       	;yes
       ldy #$10        	;no - reset delay before repeat
       sty delay
       bne ckit2       	;always
rpt10
       and #$7f        	;unshift it
       bit rptflg      	;check for repeat disable
       bmi rpt20       	;yes
       bvs scnrts
       cmp #$7f        	;no keys ?
       beq ckit2       	;yes - get out
       cmp #$14        	;an inst/del key ?
       beq rpt20       	;yes - repeat it
       cmp #$20        	;a space key ?
       beq rpt20       	;yes
       cmp #$1d        	;a crsr left/right ?
       beq rpt20       	;yes
       cmp #$11        	;a crsr up/dwn ?
       bne scnrts      	;no - exit
rpt20
       ldy delay       	;time to repeat ?
       beq rpt40       	;yes
       dec delay
       bne scnrts
rpt40
       dec kount       	;time for next repeat ?
       bne scnrts      	;no
       ldy #4          	;yes - reset ctr
       sty kount
       ldy ndx         	;no repeat if queue full
       dey
       bpl scnrts
ckit2
       .byte $ea,$ea   	;**mod to save bytes 01/13/84 fab.
       lsr lstshf
ckit3
       ldy sfdx        	;get index of key
       sty lstx        	;save this index to key found
       cpx #$ff        	;a null key or no key ?
       beq scnrts      	;branch if so
       txa             	;need x as index so...
       ldx #0
       stx stpflg      	;clear pause flag- we have a key
       ldx #7
dokey1
       cmp funtab,x    	;is it a function key?
       beq dopfky      	;yes
       dex
       bpl dokey1
       ldx ndx         	;get # of chars in key queue
       cpx xmax        	;irq buffer full ?
       bcs scnrts      	;yes - no more insert
       sta keyd,x      	;put raw data here
       inx
       stx ndx         	;update key queue count
scnrts
       rts



dopfky
       lda keybuf,x    	;find length of function key string
       sta kyndx
       lda #0          	;find index to start of string
fndky1
       dex
       bmi fndky2
       clc
       adc keybuf,x
       bcc fndky1      	;always
fndky2
       sta keyidx
       rts

funtab
	.byte   $85,$89,$86,$8a
	.byte   $87,$8b,$88,$8c

;end
