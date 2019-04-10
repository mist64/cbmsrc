	.page
	.subttl 'close all files'
;*************************************
;* clall -- close all logical files  *
;* deletes all table entries and     *
;* restores default i/o channels     *
;* and clears serial port devices.   *
;*************************************

nclall
       lda #0
       sta ldtnd       	;forget all files



;****************************************
;* clrch -- clear channels              *
;* unlisten or untalk serial devcs, but *
;* leave others alone. default channels *
;* are restored.                        *
;****************************************

nclrch
       ldx #3
       cpx dflto       	;is output channel serial?
       bcs jx750       	;no...
       jsr tunlsn      	;yes...unlisten it
jx750
       cpx dfltn       	;is input channel serial?
       bcs clall2      	;no...
       jsr tuntlk      	;yes...untalk it
;
; restore default values
;
clall2
       stx dflto       	;output chan=3=screen
       lda #0
       sta dfltn       	;input chan=0=keyboard
       rts


;***** tedisk support routines {state change} *****
;
tunlsn
       bit usekdy      	;unlisten patch
       bmi kdy9        	;kdy was listening...
       jmp unlsn       	;serial was...

kdy9
       pha             	;save (a)
       lda #iulstn
       sta kdycmd
       lda usekdy      	;status shows unlisten
       and #$7f
       sta usekdy
       lda #kcmd1
       jmp kdy75       	;send cmd

tuntlk
       bit usekdy      	;untalk patch
       bvs kdya
       jmp untlk

kdya
       pha             	;save (a)
       lda #iutalk
       sta kdycmd
       lda usekdy
       and #$bf
       sta usekdy
       lda #kcmd1
       jmp kdy75       	;send cmd

;end

