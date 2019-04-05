	.page
	.subttl 'load'
;**********************************
;*             load               *
;*                                *
;* fa:   0= invalid               *
;*       1= cassette              *
;*       2= invalid               *
;*       3-31= serial             *
;*                                *
;* sa:   0= alternate load        *
;*          .x,.y= load address   *
;*       1= normal load           *
;*                                *
;* .a:   0= load                  *
;*       >0= verify only          *
;*                                *
;* ending address returned in x,y *
;*                                *
;**********************************

loadsp
       stx memuss      	;.x has low alt start
       sty memuss+1
load
       jmp (iload)     	;monitor load entry

nload
       sta verfck      	;store verify flag
       lda #0
       sta status      	;clear status
       lda fa          	;check device number
       bne ld20

ld10
       jmp error9      	;bad device #-keyboard

ld20
       cmp #3
       beq ld10        	;disallow screen load
       bcs ldieee      	;>=4
       cmp #2
       beq ld10        	;no load from rs232
       jmp ldcass      	;some of our people listen to them when they program
;
;  load from serial
;
ldieee
       ldy fnlen       	;must have file name
       bne ld25        	;yes...ok
       jmp error8      	;missing file name

ld25
       ldx sa          	;save sa in .x
       jsr luking      	;tell user looking
       lda #$60        	;special load command
       sta sa
       jsr openi       	;open the file

       lda fa
       jsr ttalk       	;%% establish the channel
       lda sa
       jsr ttksa       	;%% tell it to load

       jsr tacptr      	;%% get first byte
       sta eal

       lda status      	;test status for error
       lsr a
       lsr a
       bcs ld90        	;file not found...actually i'm just too lazy
       jsr tacptr      	;%%
       sta eah

       txa             	;find out old sa
       bne ld30        	;sa<>0 use disk address
       lda memuss      	;else load where user wants
       sta eal
       lda memuss+1
       sta eah
ld30
       jsr loding      	;tell user loading

ld40
       lda #$fd        	;mask off timeout
       and status
       sta status

       jsr stop        	;stop key?
       bne ld45        	;no...

       jmp break       	;stop key pressed

ld45
       jsr tacptr      	;%% get byte off ieee
       tax
       lda status      	;was there a timeout?
       lsr a
       lsr a
       bcs ld40        	;yes...try again
       txa
       ldy verfck      	;performing verify?
       beq ld50        	;no...load
       ldy #0
       sta vsave       	;temp for check
       lda #eal        	;verify it
       sta sinner
       jsr kludes
       cmp vsave
       beq ld60        	;o.k....
       lda #sperr      	;no good...verify error
       jsr udst        	;update status
       .byte $2c        	;skip next store

ld50
       sta (eal),y
ld60
       inc eal         	;increment store addr
       bne ld64
       inc eah
ld64
       bit status      	;eoi?
       bvc ld40        	;no...continue load

       jsr tuntlk      	;%% close channel
       jsr clsei       	;close the file
       bcc ld180       	;branch always

ld90
       jmp error4      	;file not found
;
;  set end load addresses
;
ld180
       ldx eal         	;.c=0 already!
       ldy eah
ld190
       rts
;
;  load from cassette
;
ldcass
       jsr tstply      	;tell user to press the little buttons
       bcs ld190       	;stop key pressed
       jsr luking      	;tell user searching
ld101
       lda fnlen       	;ark!, have we a name for this creature?
       beq ld150       	;no
       jsr faf         	;yes get a named file
       bcc ld170
       beq ld190       	;stopkey again
       bcs ld90        	;error

ld150
       jsr fah         	;nothing in particular (any header)
       beq ld190       	;stop key
       bcs ld90        	;no header

ld170
       lda type        	;is it a movable program?
       cmp #blf
       beq ld175       	;basic load file

       cmp #plf        	;perhaps a fixed file?
       bne ld101       	;another throw of the dice
ld173
       ldy #0          	;fixed load
       lda (tapebs),y  	;get starting address from header
       sta memuss      	;into memuss
       iny
       lda (tapebs),y
       sta memuss+1
       jmp ld179

ld175
       lda sa          	;check for a monitor load
       bne ld173       	;i'm sorry, excuse me...
ld179
       sec             	;calculate tape ea-sa
       ldy #2
       lda (tapebs),y
       ldy #0
       sbc (tapebs),y
       tax             	;low to .x
       ldy #3
       lda (tapebs),y
       ldy #1
       sbc (tapebs),y
       tay             	;high to .y

       clc             	;ea = sa + (tape ea-sa)
       txa
       adc memuss
       sta eal
       tya
       adc memuss+1
       sta eah

       lda memuss      	;set up starting address
       sta stal
       lda memuss+1
       sta stah

       jsr loding      	;tell user loading
       jsr rvblok      	;read a variable (program) block
       bcc ld180       	;all correct
       lda #29         	;load error
       bit verfck
       bpl ld190       	;it really was
       lda #28         	;it was a verify error
       bne ld190       	;always


;  subroutine to print to console:
;  searching {for filename}
;
luking
       lda msgflg      	;supposed to print?
       bpl ld115       	;...no
       ldy #ms5-ms1    	;"searching"
       jsr msg
       lda fnlen
       beq ld115
       ldy #ms6-ms1    	;"for"
       jsr msg
;
;  subroutine to output file name
;
outfn
       ldy fnlen       	;is there a name?
       beq ld115       	;no...done
       ldy #0
ld110
       lda #fnadr
       sta sinner
       jsr kludes
       jsr bsout
       iny
       cpy fnlen
       bne ld110
ld115
       rts
;
;  subroutine to print:
;  loading/verifing
;
loding
       ldy #ms10-ms1   	;assume 'loading'
       lda verfck      	;check flag
       beq ld410       	;are doing load
       ldy #ms21-ms1   	;are 'verifying'
ld410
       jmp spmsg

;end
