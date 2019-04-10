	.page
	.subttl 'music'
domus
       ldx #1          	;service tone generators
domus0
       lda mtimlo,x
       ora mtimhi,x    	;check each voices's duration counter
       beq domus1      	;not set, do next

       inc mtimlo,x    	;stored as 2's complement
       bne domus1
       inc mtimhi,x
       bne domus1

       lda domtab,x    	;timer timed out..turn off voice
       and tedvoi
       sta tedvoi
domus1
       dex
       bpl domus0
       rts

domtab
       .byte $ef,$9f    	;if voice 2, turn off 2 & 3

;end

