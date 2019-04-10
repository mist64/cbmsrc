	.page
	.subttl 'interrupt   01/16/84'
;*******************************
;
;  irq / break service routine
;
;*******************************

krnirq 			;enter from 'puls' in non-banking rom
       tsx
       lda $104,x      	;get old status
       and #$10        	;test if irq or break
       bne puls1       	;branch if break
       jmp (cinv)      	;usually goes to srvirq
puls1
       jmp (cbinv)     	;usually goes to monitor



srvirq
       lda tedirq
       and #%00000010  	;is this a raster interrupt?
       beq srv010      	;no, go test for acia
       jsr srvspl      	;service split screen

srv010 			;test the acia
       bit apres       	;see if acia is present
       bpl srv020      	;no, don't poll
       lda acia+1
       sta astat       	;read sensitive device
       bpl srv020      	;no acia interrupt
       jsr ain         	;did we get a char?
       jsr aout        	;can we send a char?

srv020
       jsr dtimeo      	;see if cass interrupt
       lda tedirq
       and #%00000010  	;is this a raster interrupt?
       beq srv080      	;nope

       sta tedirq      	;turn off interrupt
       bit ted+11      	;line 20 or end of screen?
       lda #$cc        	;assume line 20
       bvc srv070      	;it was line 20! just reload raster compare reg.

       jmp (itime)
ntime
       jsr judt2       	;update jiffy clock  +++
       jsr domus       	;service sound generators

;   *** 01/16/84 fix: remove function key handler from irq
;                (let screen editor do it all)

       lda curbnk      	;save old bank configuration,
       pha
       lda #0
       sta curbnk      	;make kernal (temporarily) the new configuration,
       php
       cli             	;key scan must be interruptable
       jsr scnkey      	;scan keyboard
       plp             	;protect our retreat
       pla             	;get old 'current bank'
       sta curbnk
srv060
       lda #$a1        	;set up to load raster compare reg for line 20
srv070
       sta ted+11
srv080
       jmp irqret      	;restore registers, exit from irq

;end

