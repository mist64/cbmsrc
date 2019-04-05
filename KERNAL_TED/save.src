	.page
	.subttl 'save'
;***********************************
;*             save                *
;*                                 *
;*   fa:   0= invalid              *
;*         1= cassette             *
;*         2= invalid              *
;*         4-31= serial device     *
;*                                 *
;*  start of save in indirect @.a  *
;*  end of save is @ .x, .y        *
;*                                 *
;***********************************

savesp
       stx eal
       sty eah
       tax             	;set up start
       lda $00,x
       sta stal
       lda $01,x
       sta stah

save
       jmp (isave)
nsave
       lda fa          	;***monitor entry
       bne sv20

sv10
       jmp error9      	;bad device # eg: screen or kbd

sv20
       cmp #3
       beq sv10
       cmp #2
       beq sv10        	;beneath my dignity to save to rs232
       bcc stape       	;save tape

wombat
       lda #$61
       sta sa
       ldy fnlen
       bne sv25
       jmp error8      	;missing file name
;
;  choose the serial bus
;
sv25
       jsr openi
       jsr saving
       lda fa
       jsr tlistn      	;%%
       lda sa
       jsr tsecnd      	;%%
       ldy #0
       lda stah
       sta sah
       lda stal
       sta sal
       lda sal
       jsr tciout      	;%%
       lda sah
       jsr tciout      	;%%
sv30
       sec             	;compare start to end
       lda sal
       sbc eal
       lda sah
       sbc eah
       bcs sv50        	;have reached end
       lda #sal
       sta sinner
       jsr kludes
       jsr tciout      	;%%
       jsr stop
       bne sv40

break
       jsr clsei
       lda #0
       sec
       rts

sv40
       inc sal         	;increment current address
       bne sv30
       inc sal+1
       bne sv30
sv50
       jsr tunlsn      	;%%

clsei
       bit sa
       bmi clsei2
       lda fa
       jsr tlistn
       lda sa
       and #$ef
       ora #$e0
       jsr tsecnd

cunlsn
       jsr tunlsn      	;entry for openi

clsei2
       clc
       rts


;  subroutine to output:
;  'saving <file name>'
;
saving
       lda msgflg
       bpl svrts       	;no print
       ldy #ms11-ms1   	;'saving'
       jsr msg
       jmp outfn       	;<file name>
;
;  save to tape
;
stape
       jsr tstrec      	;tell play&rec
       bcs sabort      	;stop key pressed
       jsr saving      	;tell user
       ldx #plf
       lda sa
       and #1
       bne svs         	;1->plf, 0->bdf
       ldx #blf
svs
       stx type
       jsr tphead      	;write tape header
       bcs sabort
       lda #0
       sta type
       jsr wvblok      	;write actual tape data
       bcs sabort
       lda sa
       and #2          	;write eot?
       beq sgex        	;no
       jsr wreot       	;yep
       bcs sabort
sgex
       clc             	;good exit
sabort 	;bad exit
       lda #0          	;break error
svrts
       rts

;end

