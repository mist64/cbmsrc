	.page
	.subttl 'rs232'
; interrupt handler
;
aout
       lda  astat
       and  #$10
       beq  txnmt      	; tx reg is busy
       lda xport       	;hardware xoff?
       and #$02
       beq txnmt       	;yes!
       ldx  #0         	; preload
       bit  soutfg     	; got a system char to send
       bpl  nosysc     	; nope

; yep, send sys char

       lda  soutq      	; get sys char
       stx  soutfg     	; reset flag
       jmp  aoutc      	; send it

nosysc
       bit  uoutfg     	; have a user char to send?
       bpl  txnmt      	; nope

; yes, send user char

       bit  alstop     	; are we ~s'ed?
       bmi  txnmt      	; yep...
       lda  uoutq      	; else...get char
       stx  uoutfg     	; reset flag

; send some char

aoutc
       sta  acia

       lda  astat      	; update status
       and  #$ef
       sta  astat
txnmt 		 	; just passing thru exit
       rts


ain
       lda  astat
       and  #$8
       beq  rxfull     	; no char has been received
       lda  astat      	; got one...reset stat bit
       and  #$f7
       sta  astat
       lda  acia
       beq  notacc     	

; it's a null, don't let thru for x-disable

       sta  aintmp     	; save char
       cmp  xon        	; is it a ~q
       bne  trycs      	; nope

; got a ~q

       lda  #0
       sta  alstop     	; tell local xmit to go
       beq  rxfull     	; !bra, what character?

trycs
       cmp  xoff       	; is it a ~s
       bne  notacc     	; nope

; got a ~s

       lda  #$ff
       sta  alstop     	; tell local xmit to stop
       bne  rxfull     	; !bra, i didn't see that...

notacc
       lda  inqcnt
       cmp  #inpqln-1  	; is queue full
       beq  rxfull     	; yep
       cmp  #hiwatr    	; high water mark
       bne  nohw       	; nope

; hit high water mark, tell sender to stop

       lda  xoff       : x-sw is off
       beq  nohw
       sta  soutq      	; ~s
       lda  #$ff
       sta  soutfg     	; flag it present
       sta  arstop     	; flag remote stopped

nohw
			; not full, insert char
       ldx  inqfpt     	; do: inqfpt <- inqfpt+1 mod 64
       inx
       txa
       and  #$3f
       sta  inqfpt
       tax
       lda  aintmp     	; get char to insert
       sta  inpque,x   	; insert it
       inc  inqcnt     	; another drop in the bucket
rxfull 			; error exit
       rts             	; all ok



; get a char out of the input buffer

agetch
       lda  inqcnt     	; got any?
       beq  bukbuk     	; nope, inqcnt=0 then return a $00

; not empty

       php             	; save int enb bit

; { mut-ex begin

       sei
       ldx  inqrpt     	; do: inqrpt <- inqrpt+1 mod 64
       inx
       txa
       and  #$3f
       sta  inqrpt
       plp

; mut-ex end }

       tax
       lda  inpque,x   	; get char from queue
       pha             	; save it
       dec  inqcnt     	; one less
       lda  inqcnt
       cmp  #lowatr    	; low watermark?
       bne  notlow     	; nope

; hit low water mark

       bit  arstop     	; is remote ~s'ed
       bpl  notlow     	; nope, then don't ~q
       lda  xon
       beq  notlow     	; x-sw is off
       sta  soutq      	; send a ~q
       sec
       ror  soutfg     	; flag it
       lsr  arstop     	; remote now not ~s'ed (msb cleared)


aready 			; entry for chkin & chkout
       bit  apres      	; is he there?
       bpl  anrdy      	; no, don't even bother to look
bukbuk 			; i'll just be jumping in here
       pha
notlow 			; here too
       lda  astat      	; get status definition
       and  #$4f       	; use bits 0..3, and 6
       eor  #$40       	; invert dsr
       sta  status
       pla
anrdy
       clc
       rts


; put a character in the output queue

aputch
       bit  uoutfg
       bmi  aputch     	; busy wait for buffer empty
       sta  uoutq      	; now, put it in the buf
       sec             	; flag buf as full
       ror  uoutfg
       jmp  bukbuk     	; update status exit


; initialize rs-232 variables & pointers
; reset to 0:
;  uoutfg     - user out buf is empty
;  inqfpt     - make front=rear
;  inqrpt
;  inqcnt     - no chars in queue
;  soutfg     - sysm out buf is empty
;  alstop     - we're not ~s'ed
;  arstop     - i'll assume remote isn't either
;  apres      - acia assumed not to be present yet
;  xon        - x-on is deselected
;  xoff       - x-off is deselected


ainit
       lda  #0
       ldx  #11
gerber
       sta  uoutq,x    	; clear above flags (and some others)
       dex
       bpl  gerber

       sta  acia+1     	; programmed reset of the acia
       sta  xon        	; deselect handshake protocol
       sta  xoff
       rts

;end

