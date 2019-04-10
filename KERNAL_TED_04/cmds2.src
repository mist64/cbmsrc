	.page
	.subttl 'cmds2   02/07/84'
; hunt for bytes or string
; syntax:  h 0000 1111 'af...   <or>   h 0000 1111 22 33 44 ...

hunt
       jsr range       	;get sa in t2, calculate length, put in t1
       bcs tnsf99      	;error if eol
       ldy #0
       jsr gnc         	;get first char
       cmp #$27        	;is it a '
       bne ht50        	;no-must be hex
       jsr gnc         	;yes-get first string chr
ht30
       sta xcnt,y      	;save in buf (** 02/07/84 fix: was 'hulp')
       iny
       jsr gnc         	;get next
       beq ht80        	;yes-end of string
       cpy #$20        	;no-32 char yet?
       bne ht30        	;no-get more
       beq ht80        	;yes-go look for it
ht50
       sty bad         	;zero for rdob
       jsr pargot      	;finish hex read
ht60
       lda t0
       sta xcnt,y      	;save in buf (** 02/07/84 fix)
       iny
       jsr parse       	;get next character
       bcs ht80        	;no more -go look for bytes
       cpy #$20        	;32 bytes yet?
       bne ht60        	;no-get more
ht80
       sty cmpflg      	;yes-start search
       jsr crlf        	;next line
ht85
       ldx #0
       ldy #0
ht90
       jsr pick1       	;***patch 01/16/84 to select fetches from ram or rom
       cmp xcnt,x      	;same? (** 02/07/84 fix)
       bne ht100       	;no-move on
       iny
       inx
       cpx cmpflg      	;checked full string?
       bne ht90        	;no-check on

       jsr stop
       beq tnsf98      	;stop (goto 'main')
       jsr putt2       	;print address found
ht100
       jsr inct2       	;increment t2
       jsr dect1       	;decrement byte counter
       bcs ht85        	;loop if not done
       bcc tnsf98      	;goto 'main'
	.page
; load/save/verify
;
; l {"name"} {,device-number}
; v {"name"} {,device-number}
; s "name",device-number,starting-address,ending-address

lodsav
       ldy #1
       sty fa
       sty sa
       dey
       sty fnlen       	;(.y=0)
       sty status
       sty verfck
       lda #>xcnt      	;(** 02/07/84 fix: was 'hulp')
       sta fnadr+1
       lda #<xcnt
       sta fnadr
l1
       jsr gnc         	;look for name
       beq l5          	;branch if no name (must be default load)
       cmp #' '
       beq l1          	;skip spaces
       cmp #'"'
       bne errl
       ldx chrptr
l3
       cpx bufend
       bcs l5          	;eol, must be load
       lda buf,x       	;get chr
       inx
       cmp #'"'        	;pass everything up to closing quote
       beq l8
       sta (fnadr),y
       inc fnlen
       iny
       cpy #17         	;check length of name (02/07/84 fix: 16 max.)
       bcc l3
errl
       jmp error
       nop

l8
       stx chrptr
       jsr gnc         	;trash delimitor
       jsr parse       	;get device #
       bcs l5          	;use default
       lda t0          	;(** 02/07/84 fix: removed 'and #$0f')
       beq errl        	;can't be device 0,
       cmp #3
       beq errl        	;..or device 3
       sta fa

       jsr parse       	;get starting address
       bcs l5          	;none, must be load
       jsr t0tot2      	;save sa in t2

       jsr parse       	;get ending address
       bcs errl        	;can't default now!
       jsr crlf        	;prep for 'saving...' msg
       ldx t0          	;pickup end addr
       ldy t0+1
       lda cmpflg
       cmp #'s         	;check that this is a save
       bne errl
       lda #0
       sta sa
       lda #<t2        	;pointer to start. addr
       jsr $ffd8
l999
       jmp main

l5
       lda cmpflg      	;check for load
       cmp #'v         	;..or verify
       beq l6
       cmp #'l
       bne errl
       lda #0          	;flag load
l6
       jsr $ffd5
       lda status
       and #$10
       beq l999        	;ok to cont.
       lda cmpflg      	;l & v have diff. err. msgs
       cmp #'l
       beq errl
       ldx #msgver
       jsr msgxxx
       bmi l999        	;always


fill
       jsr range       	;sa in t2, len in t1
       bcs errl        	;error if eol
       jsr parse       	;get fill value
       bcs errl
       ldy #0
fill10
       lda t0
       sta (t2),y
       jsr inct2
       jsr dect1
       bcs fill10
       bcc l999

;02/07/84 fix: move tedmon string buffer from 'hulp' to 'xcnt'
;02/07/84 fix: remove load/save/ver masking of 't0' to 4 bits!
;02/07/84 fix: allow 16 char. max. file name length
;end

