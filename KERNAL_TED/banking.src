	.page
	.subttl  'banking'
;***********************************************************************
;
;                 software supporting banking hardware
;
;  *******  this code must fit *entirely* between $fc00-$fcff  *******
;
;
; set up each of the four possible slots, and test if there is a
; device in that slot. if so, store that devices number in the cor-
; responding entry in the physical address translation (pat) table.
; if a device is found to have a number of '1', it will be logged in the
; table, and a jump to that devices cold-start routine will be performed.
;
;***********************************************************************

poll
       ldx #3
       stx xsav
       lda #0
poll10
       sta pat,x       	;first zero out all pat entries
       dex
       bpl poll10

poll20			;set up and test each bank
       ldx xsav
       lda dblx,x      	;set up both upper & lower banks
       tax
       sta bnksel,x
       ldy #2
poll30
       lda $8007,y     	;test for 'cbm' (in ascii)
       cmp cbmmsg,y
       bne poll50      	;no match
       dey
       bpl poll30      	;keep looking

       lda $8006       	;it's 'cbm'...now get device number
       ldx xsav
       sta pat,x
       cmp #1          	;autostart?
       bne poll50      	;no
       stx curbnk      	;yes, give them the bank configuration,
       jsr $8000       	;.. then go to cold start routine.

poll50
       dec xsav
       bpl poll20
       rts

cbmmsg .byte 'CBM'
	.page
;*************************************************
;
; call every active cartridges cold start routine.
;
;*************************************************

phenix
       sei
       ldx #3
phen2
       lda pat,x
       beq phen3       	;no cartridge in this slot if 0

       txa
       pha
       lda dblx,x      	;select low and high rom
       tax
       sta bnksel,x
       stx curbnk
       jsr $8000       	;call it's cold start routine
       pla
       tax
phen3
       dex
       bne phen2       	;do slots 3,2,1 - ext2, ext1, int

       sta bnksel      	;set up system bank
       stx curbnk      	;..as current bank
       cli
       rts

dblx   .byte %00000000,%00000101,%00001010,%00001111
	.page
;***********************************************
; fetch a byte long
;    entry:
;       your bank in acc
;       target bank in x
;       target address in fetptr,fetptr+1
;       offset from address in y
;
;    return with value in a
;***********************************************

fetchl
       sta bnksel,x
       tax
       lda (fetptr),y
       sta bnksel,x
       rts
	.page
;***********************************************************************
; call a subroutine in another bank
;     enter with:
;       your bank in acc
;       target bank in x
;       fetxrg, fetarg, fetsrg loaded with x, a, and s to go into routine
;       address in lngjmp, lngjmp+1
;
;     return with:
;       fetxrg, fetarg, fetsrg loaded with x, a, and s from routine
;***********************************************************************

long
       pha             	;save return bank combo
       stx curbnk      	;set up target bank combo
       sta bnksel,x
       ldx fetxrg
       lda fetsrg
       pha
       lda fetarg
       plp
       jsr lngrt1
       sta fetarg
       php
       pla
       sta fetsrg
       stx fetxrg
       pla
       sta curbnk
       tax
       sta bnksel,x
       rts

lngrt1
       jmp (lngjmp)
	.page
;***********************************************************************
;
; long irq routine.
; called by a bank which has received an interrupt, and wishes to have
; the system roms (kernal/basic) service the interrupt.
;
; the users irq vector should point to the following routine in his rom:
;
;       pha  ;save accum.
;       txa
;       pha  ;save x reg
;       tya
;       pha  ;save y reg
;        .
;        .
;   determine if this is an interrupt to be serviced by the kernal.
;     if so....
;       jmp lngirq
;
; *note* before calling this routine, curbank must contain your bank #
;
;***********************************************************************

puls			;entry for normal irq's
       pha
       txa
       pha
       tya
       pha

lngirq			;entry for irq's passed from banking cartridges
       sta bnksel      	;select system roms
       jmp krnirq      	;kernal irq routine

irqret
       ldx curbnk      	;restore previous bank
       sta bnksel,x
       pla
       tay
       pla
       tax
       pla
       rti
	.page
gobvec
       ldx curbnk
       sta $fdd0,x
       jmp (bnkvec)

	*=$fcf1		;jump table for banking routines
       jmp gobvec
       jmp phenix
       jmp fetchl
       jmp long
       jmp lngirq

;end
