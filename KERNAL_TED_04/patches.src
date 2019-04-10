	.page
	.subttl  'patches   02/17/84'
;  ** clppat **   fixes:  open4,4 <cr> cmd4 <cr> list <cr> problem
;                 from 'ed.1'
;
clppat
       lda #cr        	;pass a <return>
       ldx dfltn
       cpx #3          	;is input from screen?
       beq clppa1      	;yes
       ldx dflto
       cpx #3          	;is output to the screen?
       beq clppa2      	;yes
clppa1
       jsr print       	;force it
clppa2
       lda #cr
       jmp clp7        	;return to in-line code



;  ** chkpat **   fixes:  allows selection between rom or ram color
;                 tables in 'ed.4'.
;
chkpat
       lda colkey,x    	;assume fetch is from ram (default)
       bit colsw
       bpl chkpa1      	;it is from ram
       lda collum,x    	;no, fetch from rom table
chkpa1
       rts



;  ** pick1 **    fixes:  allows selection between rom or ram reads
;                 in the monitor.
;
pick1
       bit ramrom
       bmi pick2       	;branch if ram
       lda (t2),y      	;fetch byte from rom (default)
       rts
pick2
       lda #t2         	;use subbie to fetch from ram
       sta sinner
       jmp kludes



;  ** spkpat **   fixes:  babbling speech module
;
spkpat
       lda #$09
       sta $fd20       	;shut up
       ora #$80
       sta $fd20       	;shut up now (texans think this way...)
       jmp poll



; ** sobsob, wimper **  code to download for ram fetch routine
;
sobsob
       php
       sei
       sta romoff
wimper
       lda ($00),y
       sta romon
       plp
       rts
ugh



;  +++ patches for cassette
;
judt2
       lda  xport
       and  #4
       bne  supz
			; else sw is down
       bit  lsem
       bmi  exitz
       lda  port
       and  #$f7       	;***patch 16mar84 tvr
       sta  port
exitz
	.if   palmod
       dec palcnt      	;test if 5th jiffy (pal system only)
       bpl pal001      	;it's not
       lda #4
       sta palcnt      	;reset counter
       jsr udtim       	;give one extra tick to correct
pal001
	.endif
       jmp  udtim
supz
       sta  lsem       	; ac.7=0 from jsr castst
       jsr  motoff
       jmp  exitz



ptchdd			;84.2.13
       inx             	;x=0
       stx  drvb2-48
       stx  tedrva-48

       lda #$80        	
			;+++patch 2mar84 - reset white noise generator
       sta ted+17

       rts             	;x must =0 on rts !!! ******


;**patch 16mar84 fix to eliminate cassette/serial bus interference
;end

