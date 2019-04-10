	.page
	.subttl  'openchannel'
;***************************************
;* chkin -- open channel for input     *
;*                                     *
;* the number of the logical file to be*
;* opened for input is passed in .x.   *
;* chkin searches the logical file     *
;* to look up device and command info. *
;* errors are reported if the device   *
;* was not opened for input ,(e.g.     *
;* cassette write file), or the logical*
;* file has no reference in the tables.*
;* device 0, (keyboard), and device 3  *
;* (screen), require no table entries  *
;* and are handled separate.           *
;***************************************

nchkin
       jsr lookup      	;see if file known
       beq jx310       	;yup...
       jmp error3      	;no...file not open

jx310
       jsr jz100       	;extract file info (returns w/.a=fa & flags set)
       beq jx320       	;is keyboard...done.
;
; could be screen, keyboard, or serial
;
       cmp #3
       beq jx320       	;is screen...done.
       bcs jx330       	;is serial...address it
       cmp #2
       bne opcasi      	;open cass for input
       jsr aready      	;open rs-232 for input
       bcs excp1       	;carry set if acia not ready
       lda fa

jx320
       sta dfltn       	;all input comes from here
       clc             	;good exit
excp1
       rts
;
; a serial device has to be a talker
;
jx330
       tax             	;device # for dflto
       jsr ttalk       	;tell him to talk
       bit status      	;anybody home? (case of no devices on bus)
       bmi dnpci       	;no...

       lda sa          	;a second?
       bpl jx340       	;yes...send it
       jsr ttkatn      	;no...let go
       jmp jx350

jx340
       jsr ttksa       	;send second
jx350
       txa             	;restore dev#
       bit status      	;secondary address sent ok?
       bpl jx320       	;yep, done
dnpci
       jmp error5      	;input channel device not present


opcasi
       ldx sa          	;open cass for input
       cpx #$60        	;is command a read?
       beq jx320       	;yes
       jmp error6
	.page
;***************************************
;* chkout -- open channel for output   *
;*                                     *
;* the number of the logical file to be*
;* opened for output is passed in .x.  *
;* chkout searches the logical file    *
;* to look up device and command info. *
;* errors are reported if the device   *
;* was not opened for input ,(e.g.     *
;* keyboard), or the logical file has  *
;* reference in the tables.            *
;* device 0, (keyboard), and device 3  *
;* (screen), require no table entries  *
;* and are handled separate.           *
;***************************************

nckout
       jsr lookup      	;is file in table?
       beq ck5         	;yes...
       jmp error3      	;no...file not open

ck5
       jsr jz100       	;extract table info (returns w/.a=fa & flags set)
       bne ck10        	;no...something else.
ck20
       jmp error7      	;yes...not output file
;
; could be screen, serial, or tape
;
ck10
       cmp #3
       beq ck30        	;is screen...done
       bcs ck40        	;is serial...address it
       cmp #2
       bne optapo      	;open tape for output
       jsr aready      	;open rs-232 for output
       bcs excp2       	;carry set if acia not ready
       lda fa

ck30
       sta dflto       	;all output goes here
       clc             	;good exit
excp2
       rts

ck40
       tax             	;save device for dflto
       jsr tlistn      	;tell him to listen
       bit status      	;anybody home? (case of no devices on bus)
       bmi dnpco       	;no...

       lda sa          	;is there a second?
       bpl ck50        	;yes...
       jsr tscatn      	;no...release lines
       bne ck60        	;branch always

ck50
       jsr tsecnd      	;send second...
ck60
       txa
       bit status      	;speaketh ye?
       bpl ck30        	;yep, done
dnpco
       jmp error5      	;device not present


optapo
       ldx sa          	;open tape for output
       cpx #$60        	;read?
       beq ck20        	;yes...bad!
       bne ck30        	;always
	.page
;***** tedisk support routines... {state transition} *****

tstkdy
	pha             ;save .a
       stx wrbase      	;save .x
       ldx #$30        	;starting i/o offset
       lda fa          	;load file address
       cmp #8          	;=8?
       beq tstok
nok8   cmp #9          	;=9?
       bne notprs
       ldx #0          	;must be $fec0
tstok  lda #$55        	;write a pattern to cmd channel
       sta tedrva-48,x
       eor tedrva-48,x 	;is it the same
       bne notprs
       lda tedrvb-48,x 	;i tied a status bit hi
       and #2
       bne notprs      	;br, he is blown away
       stx usekdy      	;store offset into i/o slot
       clc             	;ok
       .byte $24
notprs sec             	;sorry not home
       ldx wrbase      	;restore .x
       pla             	;restore .a
       rts             	;45 bytes

patchb lda tedrvc-48,x 	;wait for rdy ack to go hi
       bpl patchb
       bmi ptchbb      	;bra

patcha lda #0
       sta tedrva-48,x 	;clear cmd channel
ptchbb lda #$40
       sta tedrvc-48,x 	;set dav hi
       ldx wrbase      	;restor .x
       pla             	;restore .a for ciout, data for acptr
       clc             	;ok
       rts             	;*

patchd sta tedrvc
       sta drvc2-48
       sta tedrvc-48
       dex
       stx drva2-48
       jmp ptchdd

;this is in the patch area:
;ptchdd inx             ;.x=0
;       stx drvb2-48
;       stx tedrva-48
;       rts             ;x must = 0 on rts !!!
       
ttalk
       jsr tstkdy      	;who do we talk to???
       bcc kdy1        	;he's out there
       jmp talk        	;serial dev.

kdy1   pha             	;save (a)data
       lda #italk
       sta kdycmd
       lda usekdy
       ora #$40        	;write to kdy
       sta usekdy
       lda #kcmd1
       jmp kdy75       	;finish up

ttkatn
       bit usekdy      	;do an open in with no sa...
       bvs kdy5        	;kdy is pres...do nothing
       jmp tkatn       	;serial

ttksa
       bit usekdy      	;do an open in with sa...
       bvs kdy3        	;kdy is pres
       jmp tksa

kdy3
       pha             	;save (a) data
;      lda sa		; assumne SA in acc from call to TKSA by
       NOP		; application. this is an 05 rev recommended
       NOP		; by D. Siracusa for compatibility with LOGO, etc.
       sta kdycmd
       lda #kcmd2
       jmp kdy75

tlistn
       jsr tstkdy      	;do an open out with fa...
       bcc kdy4        	;he's out there...
       jmp listn

kdy4
       pha             	;save (a) data
       lda #ilstn
       sta kdycmd
       lda usekdy
       ora #$80
       sta usekdy
       lda #kcmd1      	;tell tedisk to listen
       jmp kdy75

tscatn
       bit usekdy      	;do an open out with no sa...
       bmi kdy5
       jmp scatn

kdy5
       rts             	;do nothing for kdy

tsecnd
       bit usekdy      	;do an open out with sa...
       bmi kdy6
       jmp secnd

kdy6
       pha             	;save (a) data
       sta kdycmd
       lda #kcmd2
       jmp kdy75

;end
