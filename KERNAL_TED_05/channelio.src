	.page
	.subttl 'channelio'
;***************************************
;* getin -- get character from channel *
;* channel is determined by dfltn.     *
;* if device is 0, keyboard queue is   *
;* examined and a character removed if *
;* available. if queue is empty, z     *
;* flag is returned set. devices 1-31  *
;* advance to basin.                   *
;***************************************

ngetin
       lda dfltn       	;check device
       bne bn10        	;not keyboard

       lda ndx         	;queue index
       ora kyndx
       beq gn20        	;nobody there...exit
       sei
       jmp lp2         	;go remove a character



;***************************************
;* basin-- input character from channel*
;* input differs from get on device    *
;* #0 function which is keyboard. the  *
;* screen editor makes ready an entire *
;* line which is passed char by char   *
;* up to the carriage return. other    *
;* devices are:                        *
;*          0 -- keyboard              *
;*          3 -- screen                *
;*          1 -- cassette              *
;*          2 -- rs-232                *
;*          4-31 -- serial bus         *
;***************************************

nbasin
       lda dfltn       	;check device
       bne bn10        	;is not keyboard...
;
;  input from keyboard
;
       lda pntr        	;save current...
       sta lstp        	;... cursor column
       lda tblx        	;save current...
       sta lsxp        	;... line number
       jmp loop5       	;blink cursor until return

bn10
       cmp #3          	;is input from screen?
       bne bn30        	;nope, try cassette, 232 or serial

       ora crsw
       sta crsw        	;fake a carriage return
       lda scrt        	;say we ended...
       sta indx        	;...up on this line
       jmp loop5       	;pick up characters

casi
       jsr savxy
       cmp #1
       bne rs232i
       jsr getcas
       jmp rstxy

rs232i
       jsr agetch
       jmp rstxy
;
;  input from serial bus
;
bn30
       bcc casi        	;<3
       lda status      	;status from last
       beq tacptr      	;was good
       lda #$0d        	;bad...all done
gn20
       clc             	;valid data, good return
       rts



getcas 			;get a chr from the cassette buffer
       ldy tptr        	;test for bufr empty
       cpy #bufmax
       bcc notmt1      	;-mt
       jsr rdblok      	;mt, read another block
       bcc getcas      	;!bra, try again
       rts             	;bad exit, carry should be set

notmt1
       ldy tptr        	;get bufptr
       lda (tapebs),y  	;get chr
       pha             	;save it
       iny             	;try to look at next chr
       cpy #bufmax
       bcs notovr      	;no next chr
       lda (tapebs),y  	;get next chr
       bne notovr      	;..ok, not a #00
       lda #64         	;wooops!, eof
       jsr udst        	;flag it so

notovr
       inc tptr        	;advance buf ptr
       pla             	;chr to be returned
       clc             	;sucess flag
       rts
	.page
;***************************************
;* bsout -- out character to channel   *
;* determined by variable dflto:       *
;*           0 -- invalid (rs232?)     *
;*           1 -- cassette             *
;*           2 -- rs-232               *
;*           3 -- screen               *
;*        4-31 -- serial bus           *
;***************************************

nbsout
       pha             	;preserve .a
       lda dflto       	;check device
       cmp #3          	;is it the screen?
       bne bo10        	;nope, try somethin else...
;
;  print to crt
;
       pla             	;restore data
       jmp print       	;print on crt

bo10
       bcc bo9         	;<3
       pla
       jmp tciout      	;print to serial bus

bo9
       jsr savaxy
       cmp #1          	;is it cass?
       bne boa         	;nope, try 232
       ldy tptr        	

;char to write is on top of stack...how much we got?

       cpy #bufmax
       bcc less1       	;ok, less than the max#
       jsr wfblok      	

;buf is full, write the block out to tape

       bcs wrerr1      	;somebody goofed...
       lda #bdf        	;set up a new output buffer...
       sta type
       ldy #0          	;reset tape buf ptr
less1
       pla             	;get the chr to write...
       sta (tapebs),y  	;into buffer
       iny             	;advance buf ptr
       sty tptr        	;save it (or reset it...)
       bcc boa1        	;good exit, tell 'em  (.c=0 always!)

wrerr1
       pla             	;ditch data
       lda #0
       jmp rstxy       	;(.c=1)


boa
       pla
       jsr aputch      	;input a char to queue
boa1
       jmp rstaxy
	.page
;  ***** tedisk support routines... {channel i/o} *****
;
tacptr
       stx wrbase      	;save .x
       bit usekdy      	;use kdy for rd?
       bvs kdy8        	;yes b.6=1
       ldx wrbase      	;restore .x
       jmp acptr       	;...else serial

kdy8
       lda usekdy      	;get i/o slot
       and #%00110000
       tax             	;$fec0, $fef0
       lda #kcmd4
       sta tedrva-48,x 	;write command
kdy85
       lda tedrvc-48,x 	;wait for rdy to go low
       bmi kdy85
       lda #$00
       sta drva2-48,x  	;setup dat/dir for inputs
       sta tedrvc-48,x 	;drop rdy low
kdy86
       lda tedrvc-48,x 	;wait for rdy to go high
       bpl kdy86
       lda tedrvb-48,x 	;retr status
       and #3          	;mask status bits
       cmp #3          	;eoi ?
       bne kdy88
       lda #$40
kdy88
       jsr udst        	;update status for basic
       lda tedrva-48,x 	;get data
       pha             	;*
       lda #$40
       sta tedrvc-48,x 	;set rdy high
kdy89
       lda tedrvc-48,x 	;wait for dav to go low
       bmi kdy89
       lda #$ff
       sta drva2-48,x  	;turn ports around i got data & status
       lda #$00
       sta tedrva-48,x 	;clear cmd chnl
       sta tedrvc-48,x 	;drop rdy low
kdy90
       jmp patchb      	;finish up
       nop             	;extra byte


tciout
       bit usekdy      	;use kdy for wr?
       bmi kdy7        	;yes b.7=1
       jmp ciout       	;else serial

kdy7
       pha             	;save (a)
       sta kdycmd
       lda #kcmd3
kdy75  stx wrbase
       pha             	;save cmd
       lda usekdy
       and #%00110000
       tax             	;get i/o offset
       pla             	;retr. cmd
       sta tedrva-48,x 	;send cmd
kdy76
       lda tedrvc-48,x 	;wait for dav ack
       bmi kdy76
       lda kdycmd      	;ret state cmd
       sta tedrva-48,x
       lda #$00
       sta tedrvc-48,x 	;drop dav low
kdy77
       lda tedrvc-48,x 	;wait for rdy to go high
       bpl kdy77
       lda tedrvb-48,x 	;retr status
       and #3          	;mask status
kdy79
       jsr udst        	;update basic
       jmp patcha      	;finish up

;end
