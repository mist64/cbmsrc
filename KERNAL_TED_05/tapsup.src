	.page
	.subttl 'tape support'
; if cas plat sw is not pressed give message & wait until pressed
; entry to test sw for record:
;
tstrec			;i just have to keep thinking about the 68000...
       sec
       .byte $24
tstply
       clc
       lda  xport
       and  #4
       beq  pok
       php
       jsr  primm
       .byte cr,'PRESS PLAY ',0
       plp
       bcc  qq2
       jsr  primm
       .byte '& RECORD ',0
qq2
       jsr  primm
       .byte 'ON TAPE',0

waitpl
       jsr  tstsky     	; does user want to play some more?
       bcs  pabort     	; nope, boo...
       lda  xport
       and  #4
       bne  waitpl     	; play still not down
       jsr  primm      	; finally!
       .byte cr,'OK',0
pok
       clc             	; everything's fine
pabort 			; ooops
       rts             	; exit


; prep system for cassette useage...
; stop dma's by blanking screen & disable interrupts
;
; call motor/on after this routine to allow ds clk to setup
;

faster
       sei             	; pass on the irq's

; select screen blanking: 1.7mhz clock & no dma's
;
       lda  tedvcr     	; have mutex from above <sei>
       and  #$ef
       sta  tedvcr

       lda  tedicr
       and  #$fd       	; raster int off
       ora  #$08       	; t1 int on
       sta  tedicr
       rts

; restore system to normal(?) mode of operation...
; restore timer t1, re-enable dma's & irq's

slower		 	; !carry preserved
       sei
       lda  tedvcr     	; de-select blanking
       ora  #$10
       sta  tedvcr

       lda  tedicr
       and  #$f7       	; t1 int off
       ora  #$02       	; raster int on
       sta  tedicr
       cli             	; yes, you too
       rts             	; !carry preserved


; turn motor on & delay
; modifies a,x,y
;
moton
       php              	; !preserve carry
       sec
       ror   lsem
       lda   port
       and   #$f5       	; motor on & init write line to high
       sta   port       	; ...have to write inverted
       ldx   #30        	; wait 480ms for motor to come up to speed
md1				;  to do skewed servo
       jsr   w16ms      	; ...wait 16ms
       dex
       bne   md1
       plp              	; !restore carry
       rts


       .byte  'C1984COMMODORE'


; turn motor off

motoff 				; !carry preserved
       lda   port
       ora   #$08
       sta   port       	; !carry preserved
       rts


; write blanks to tape buffer
; modifies a,x

blkbuf
       ldy   #0
       lda   #$20
blklop
       sta   (tapebs),y
       iny
       cpy   #bufmax+1
       bne   blklop
       rts


; setup tape pointer to reserved buffer

bufini
       pha             	; !save ac
       lda  #<tapbuf
       sta  tapebs
       lda  #>tapbuf
       sta  tapebs+1
       pla             	; !restore ac
       rts



balout		 	; error recovery for stop-key pressed
       jsr  tstsky
       bcc  cont
       jsr  motoff     	; restore system
       jsr  slower

       ldx  srecov
       txs
       lda  #0
       sta  srecov
       sec
cont
       rts             	; blind faith


d2     = 13200         	; dead man-2

dtimeo
       lda  tedirq     	; was this a t1 int
       and  tedicr
       and  #$08
       bne  nott1      	; no return
       rts
nott1
       sta  tedirq     	; yep, clear it
       sei

       lda  #<d2       	; set up d2
       sta  timr1l     	; timeout sets up d2
       lda  #>d2
       sta  timr1h

       ldx  drecov     	; get stack mark
       txs
       sec             	; indicate error
       rts             	; return to upper lexical level


; word marker sets this time up

d1     = 18600         	; dead man-1

setd1 		 	; setup delta #1 for byte read
       lda  #<d1
       sta  timr1l
       lda  #>d1
       sta  timr1h

       lda  #$08       	; clear any pending t1 int's
       sta  tedirq
       rts

;end
