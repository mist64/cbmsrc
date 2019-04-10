	.page
	.subttl 'time   01/17/84'
;***********************************
;*                                 *
;* time                            *
;*                                 *
;*consists of three functions:     *
;* (1) udtim-- update time. usually*
;*     called every 60th second.   *
;* (2) settim-- set time. .y=msd,  *
;*     .x=next significant,.a=lsd  *
;* (3) rdtim-- read time. .y=msd,  *
;*     .x=next significant,.a=lsd  *
;*                                 *
;***********************************

; interrupts are coming from timer 1
; here we proceed with an increment
; of the time register.

udtim
       inc time+2
       bne ud30
       inc time+1
       bne ud30
       inc time

; here we check for roll-over 23:59:59
; and reset the clock to zero if true

ud30
       sec
       lda time+2
       sbc #$01
       lda time+1
       sbc #$1a
       lda time
       sbc #$4f
       bcc ud60
;
; time has rolled--zero register
;
       ldx #0
       stx time
       stx time+1
       stx time+2
;
; set stop key flag here
;
ud60
       lda #$7f        	;debounce  (** 01/17/84 mod for new keyboard ports)
       jsr keyscn
       sta tmpkey
       lda #$7f
       jsr keyscn
       cmp tmpkey
       bne ud60
       ora #$7f
       sta stkey       	;stkey is now either $ff (no stop) or $7f (stop)
       rts



rdtim
       sei             	;keep time from rolling
       lda time+2      	;get lsd
       ldx time+1      	;get next most sig.
       ldy time        	;get msd



settim
       sei             	;keep time from changing
       sta time+2      	;store lsd
       stx time+1      	;next most significant
       sty time        	;store msd
       cli
       rts

;end

