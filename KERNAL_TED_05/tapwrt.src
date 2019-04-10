	.page
	.subttl 'tape write'
;   *** tape write routines ***
;
; primitives first...
; <w>ait for <t>imer to <t>oggle <t>wice

wttt
       sec
       bcs   wtts

; <w>ait for <t>imer to <t>oggle

wtt
       clc
wtts
       sty  t1pipe+2   	; save x,y
       stx  t1pipe+3
       ldy  t1pipe     	; pre-load for spipe
       ldx  t1pipe+1
       lda   #$10
w1     			; wait for timeout
       bit   tedirq
       beq   w1
       sty  timr2l     	; reload timer with latch
       stx  timr2h
       sta   tedirq    	; clear flag
       lda   port      	; toggle write line
       eor   #$02
       sta   port

       php             	; preserve carry (pass indicator)
       jsr  balout     	; do stopkey check
       plp             	; restore carry

       ldy  t1pipe+2   	; restore x,y
       ldx  t1pipe+3
       bcs   wtt       	; if c=1 then do twice
       rts


tshort=212-4           	; 240us / 8 cycles(4us) to setup
tlong= 424-4           	; 480us /       "       "
tbyte= 850-4           	; 960us /       "       "


; setup >byte< dipole time: 306 cycles

setupb
       lda   #<tbyte
       sta   t1pipe
       lda   #>tbyte
       sta   t1pipe+1
       rts

; setup >short< dipole time: 163 cycles

setups
       lda   #<tshort
       sta   t1pipe
       lda   #>tshort
       sta   t1pipe+1
       rts

; setup >long< dipole time: 234 cycles

setupl
       lda   #<tlong
       sta   t1pipe
       lda   #>tlong
       sta   t1pipe+1
       rts

; write a 0 on tape; ie: s,l

write0
       jsr   setups
       jsr   wttt
       jsr   setupl
       jmp   wttt       	; does rts

; write a 1 on tape; ie: l,s

write1
       jsr   setupl
       jsr   wttt
       jsr   setups
       jmp   wttt       	; does rts

; write a word marker on tape; ie: b,l

writew
       jsr   setupb
       jsr   wttt
       jsr   setupl
       jmp   wttt       	; does rts

; write the byte in (tpbyte) to the tape with leading word marker,
; and ending odd parity.

wrbyte
       sta   tpbyte    	; save byte to be written
       lda   #1
       sta   parity
       jsr   writew
       ldx   #8        	; #of bits
twloop
       ror   tpbyte    	; lsb first
       bcs   aone
       inc   parity    	; count the zeros
       jsr   write0
       jmp   wjoin
aone
       jsr   write1
wjoin
       dex             	; done yet?
       bne   twloop    	; nope
       ror   parity    	; yep, get parity
       bcs   aoneb
       jsr   write0
       jmp   wjoin2
aoneb
       jsr   write1
wjoin2
       rts


; write elemental block to tape as defined by:
; (wrbase{+1})::=@beg of data to be written
; (wrlen{+1})::= 2's compl of numeric length of data
; pass::= {=0 for pass1}, {=$80 for pass2}

welemb
       tsx             	; save stack mark
       stx  srecov     	; ...for stopkey

       lda   port      	; assert external line low
       ora   #$02
       sta   port

       jsr   setups    	; get ready for short*
       ldy  #1
       sty  timr2h     	; prime t2 to use as one-shot

       lda   #$10      	; clear any flag
       sta   tedirq

; lots & lots of shorts...

       bit   pass      	; what pass are we doing
       bpl   l1loop    	

; y=1...   shorter shorts for second pass

       ldy   #$40      	; hi loop index...for first pass
       ldx   #$fe      	; low loop index...for both passes

; write leader 1

l1loop
       jsr   wttt
       dex
       bne   l1loop
       dey
       bne   l1loop

; now write countdown loop

       ldy   #9
cdloop
       tya
       ora   pass      	; pass modifies b(7) of data
       jsr   wrbyte
       dey
       bne   cdloop

; init checksum

       lda   type
       sta   chksum

; now write block type

       beq   wdloop     ; if=0 then no type in block
       jsr   wrbyte

; write data block

wdloop
       ldy   #0
       lda   #wrbase    ; fetch byte ( may be under rom )
       sta   sinner
       jsr   kludes
       pha             	;save
       eor   chksum
       sta   chksum
       pla

       jsr   wrbyte

       inc  wrbase
       bne  okeefe
       inc  wrbase+1
okeefe
       inc   wrlen     	; one more byte
       bne   wdloop
       inc   wrlen+1
       bne   wdloop

; data written, now do checksum

       lda   chksum
       jsr   wrbyte

; do block end marker; ie: l

       jsr   setupl
       jsr   wttt

; do end leader; ie: l*450

       jsr   setups
       ldy   #1        	; loop hi
       ldx   #$c2      	; loop lo
l2loop
       jsr   wttt
       dex
       bne   l2loop
       dey
       bne   l2loop	; done with elemental block
       rts


; write a >fixed length data< block
; assumed data is in the pre-allocated tape buffer of 192 bytes.
; *** type must be specified externally !!! ***

wfblok
       jsr   tstrec
       jsr   faster     ; 1.7mhz/get timer1/no irq's
       jsr   moton      ; get em goin
       bcs   wdabor     ; stop key pressed

       lda   #$80
       sta   pass
web
       lda   tapebs    	; tapebs->wrbase
       sta   wrbase
       lda   tapebs+1
       sta   wrbase+1

; setup length & type

       lda   #$41      	; 2's compl of #191
       sta   wrlen
       lda   #$ff
       sta   wrlen+1

       jsr   welemb
       bcs   wdabor     	; stop key pressed

       lda   pass
       bpl   wdone     	; if second pass
       lda   #0
       sta   pass
       bpl   web       	; else, do second pass

; done with both elemental blocks

wdone  			; good exit
       clc
wdabor 			; bad exit
       jsr   motoff     ; stop em
       jmp   slower     ; whatever clk/give up timer1/ok irq's & RTS


; write a tape header
; ...write starting, ending address, and filename to tape.
; *** type must be specified externally !!! ***

tphead
       jsr  bufini
       jsr  blkbuf
       ldy  #0
       lda  stal
       sta  (tapebs),y
       iny
       lda  stah
       sta  (tapebs),y
       iny
       lda  eal
       sta  (tapebs),y
       iny
       lda  eah
       sta  (tapebs),y
       iny             	; y=4      ; y@ beg of filename

       sty  tt2        	; pointer to tape buffer (dest)
       ldy  #0
       sty  tt1        	; pointer to filename (source)
tfname
       ldy  tt1
       cpy  fnlen
       beq  fnisin     	; all done !
       lda  #fnadr     	; get filename from under rom
       sta  sinner
       jsr  kludes
       ldy  tt2
       sta  (tapebs),y
       inc  tt1
       inc  tt2
       jmp  tfname
fnisin			; header data area is complete, now write it to tape...
       jmp  wfblok	; does rts        ; c=0=>ok, else error



; write a >variable length data< block (ie: program type).
; block to be written is defined by: (stah/stal,eah/eal)
; *** type must be specified externallly !!! ***

wvblok
       jsr   tstrec
       jsr   faster     ; 1.7mhz/get timer1/no irq's
       jsr   moton
       bcs   wpabor     ; stop key pressed

       lda   #$80
       sta   pass

wepb
       lda   stal       ; starting address -> wrbase
       sta   wrbase
       lda   stah
       sta   wrbase+1

; compute: ((( end-start ) <xor> $ffff)+1) -> wrlen
; by: ((end-start)-1)) <xor> $ffff) -> wrlen
;
       clc
       lda   eal
       sbc   stal
       eor   #$ff
       sta   wrlen
       lda   eah
       sbc   stah
       eor   #$ff
       sta   wrlen+1

       jsr   welemb     ; write a elem block
       bcs   wpabor     ; ooops!
       lda   pass
       bpl   wpdone     ; done with both blocks

       lda   #0
       sta   pass
       bpl   wepb       ; write second block

; done with both elem var blocks

wpdone 			; good exit
       clc
wpabor 			; bad exit
       jsr   motoff
       jmp   slower     ; whatever clk/give up timer1/ok irq's & RTS



; write end-of-tape block

wreot
       jsr   blkbuf
       lda   #eot
       sta   type
       jmp   wfblok	;& RTS

;end
