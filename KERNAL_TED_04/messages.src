	.page
	.subttl 'messages'
MS1    .BYTE CR,'I/O ERROR ',$A3
MS5    .BYTE CR,'SEARCHING',$A0
MS6    .BYTE 'FOR',$A0
MS7    .BYTE CR,'PRESS PLAY ON TAP',$C5
MS8    .BYTE 'PRESS RECORD & PLAY ON TAP',$C5
MS10   .BYTE CR,'LOADIN',$C7
MS11   .BYTE CR,'SAVING',$A0
MS21   .BYTE CR,'VERIFYIN',$C7
MS17   .BYTE CR,'FOUND',$A0
MS18   .BYTE CR,'OK',$8D


;print message to screen only if
;output enabled

spmsg  bit msgflg      	;printing messages?
	bpl msg10       ;no...
msg	lda ms1,y
	php
	and #$7f
	jsr    bsout
	iny
	plp
	bpl    msg
msg10  clc
	rts

;.end
