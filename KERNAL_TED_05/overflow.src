	.page
	.subttl 'overflow'
msgs=*
msgmon=*-msgs
       .BYTE CR,'MONITOR',$8D 
MSGBRK=*-MSGS
       .BYTE CR,'BREA',$CB
MSGREG=*-MSGS
       .BYTE CR,'   PC  SR AC XR YR SP',CR,';',$A0
MSGASM=*-MSGS
       .BYTE 'A',$A0
MSGVER=*-MSGS
       .BYTE ' ERRO',$D2

msgxxx
       lda msgs,x
       php
       and #$7f
       jsr bsout
       inx
       plp
       bpl msgxxx
       rts

;end
