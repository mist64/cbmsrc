	.page
	.subttl 'close'
;***************************************
;* close -- close logical file         *
;*                                     *
;* the logical file number of the      *
;* file to be closed is passed in .a.  *
;* keyboard, screen, and files not     *
;* open pass straight through. tape    *
;* files open for write are closed by  *
;* dumping the last buffer and         *
;* conditionally writing an end of     *
;* tape block.serial files are closed  *
;* by sending a close file command if  *
;* a secondary address was specified   *
;* in its open command.                *
;***************************************

nclose
       ror wrbase      	;save serial close flag
       jsr jltlk       	;look file up
       beq jx050       	;open...
       clc             	;else return
       rts

jx050
       jsr jz100       	;extract table data
       txa             	;save table index
       pha

       lda fa          	;check device number
       beq jx150       	;is keyboard...done
       cmp #3
       beq jx150       	;is screen...done

       bcs jx120       	;is serial address it
       cmp #2          	;rs232?
       bne jx115       	;no, was tape
;
; rs232 close ;...by brute force
;
       php             	;!mutex
       sei
       jsr ainit       	;reset pointers, variables, & acia
       plp             	;!mutex
       beq jx150       	;always
;
; close tape data file
;
jx115
       lda sa          	;was it a tape read?
       and #$f
       beq jx150       	;yes
       ldy tptr        	;else a write
       cpy #bufmax
       bcc jx116       	;buf not full
       jsr wfblok      	;else write out a full block first
       bcs jx117       	;write exception
       lda #bdf        	;setup new block
       sta type
       ldy #0
       sty tptr
jx116
       lda #0
       sta (tapebs),y
       jsr wfblok      	;write out final block
       bcc jx118       	;ok
jx117
       pla             	;get index off stack
       lda #0          	;error exit (stop key pressed)
       rts

jx118
       lda sa          	;write eot-block?
       cmp #$62
       bne jx150       	;no
       jsr wreot       	;yes
       jmp jx150
;
; close a serial file
;
jx120
       bit wrbase      	;do a real close?
       bpl ropen       	;yep
       lda fa          	;no if a disk & sa=$f
       cmp #8
       bcc ropen       	;>8 ==>not a disk, do real close
       lda sa
       and #$f
       cmp #$f
       beq jx150       	;sa=$f, no real close
ropen
       jsr clsei       	;else do a real close


; entry to remove a give logical file
; from table of logical, primary,
; and secondary addresses

jx150
       pla             	;get table index off stack
       tax
       dec ldtnd
       cpx ldtnd       	;is deleted file at end?
       beq jx170       	;yes...done

; delete entry in middle by moving
; last entry to that position.

       ldy ldtnd
       lda lat,y
       sta lat,x
       lda fat,y
       sta fat,x
       lda sat,y
       sta sat,x

jx170
       clc             	;close exit
jx175
       rts



; lookup tablized logical file data
;
lookup
       lda #0
       sta status
       txa
jltlk
       ldx ldtnd
jx600
       dex
       bmi jz101
       cmp lat,x
       bne jx600
       rts
;
; routine to fetch table entries
;
jz100
       lda lat,x
       sta la
       lda sat,x
       sta sa
       lda fat,x       	;must return w/.a=fa & flags set!
       sta fa
jz101
       rts

;rsr 5/12/82 - modify for cln232
;end

