	.page
	.subttl  'tapred   01/17/84'
;  *** tape read routines ***
;
; primitives first...
;
; read a dipole from tape
;
; if c=1 then error
;    else if v=1 then short
;            else if n=0 then long
;                    else word
;                    end
;            end
;    end
;                                            n : v
; local parms...                             -----

tshrtd .byte $40        	; token for a short   0 : 1
tlongd .byte $00        	; and a long          0 : 0
twordd .byte $80        	; and word dipole     1 : 0


; trigger on negative edge (beginning) of dipole
;
rddipl
       ldx  dsamp1     	; setup x,y with 1st sample point
       ldy  dsamp1+1
badeg1
       lda  dsamp2+1   	; put 2nd samp value on stack in reverse order
       pha
       lda  dsamp2
       pha

       lda  #$10
rwtl   			; wait till rd line is high
       bit  port
       beq  rwtl       	; !ls!

rwth   			;it's high...now wait till it's low
       bit  port
       bne  rwth	; caught the edge

       stx  timr2l
       sty  timr2h     	

; go! ...ta

       pla		;go! ...ta
       sta  timr3l
       pla
       sta  timr3h     	;go! ...tb

; clear timer flags

       lda  #$50       	; clr ta,tb
       sta  tedirq

; um...check that edge again

casdb1
       lda  port
       cmp  port
       bne  casdb1     	; something is going on here...
       and  #$10       	; a look at that edge again
       bne  badeg1     	; woa! got a bad edge trigger  !ls!

; must have been a valid edge
;
; do stop key check here

       jsr  balout
       lda  #$10
wata   			; wait for ta to timeout
       bit  port       	; kuldge, kludge, kludge !!! <<><>>
       bne  rshort     	; kuldge, kludge, kludge !!! <<><>>
       bit  tedirq
       beq  wata

; now do the dipole sample #1

casdb2
       lda  port
       cmp  port
       bne  casdb2
       and  #$10
       bne  rshort     	; shorts anyone?

; perhaps a long or a word?

       lda  #$40
watb
       bit  tedirq
       beq  watb       	

; wait for tb to timeout
; now do the dipole sample #2

casdb3
       lda  port
       cmp  port
       bne  casdb3
       and  #$10
       bne  rlong      	; looks like a long from here !ls!
			; or could it be a word?
       lda  zcell
       sta  timr2l
       lda  zcell+1
       sta  timr2h     	
			; go! z-cell check
			; clear ta flag
       lda  #$10
       sta  tedirq	; verify +180 half of word dipole
       lda  #$10
wata2
       bit  tedirq
       beq  wata2	; check z-cell is low
casdb4
       lda  port
       cmp  port
       bne  casdb4
       and  #$10
       beq  rderr1     	; !ls!
       bit  twordd     	; got a word dipole
       bmi  dipok      	; !bra

rshort
       bit  tshrtd     	; got a short
       bvs  dipok      	; !bra

rlong
       bit  tlongd     	; got a long

dipok
       clc             	; everything's fine
       rts

rderr1
       sec             	; i'm confused
       rts



; read a bit
; emulate a simple deterministic finite automaton

tzerob .byte $40        	; zero bit
toneb  .byte $00        	; one bit
twordm .byte $80        	; word marker


;     bool      cond  equ d(t) d(f)
; ---------------------------------
;     c=1=>     error ::: bcs  bcc
;     v=1=>     short ::: bvs  bvc
; (n=0)&(v=0)=> long  ::: bpl  bmi
; (n=1)&(v=0)=> word  ::: bmi  bpl

rdbit
s0
       jsr  rddipl
       bcs  s8
       bvs  s3
       bpl  s1
       bmi  s6
s1     			; d(c,l,w)=s10
       jsr  rddipl
       bcs  s8
       bvs  s2
       bvc  s8
s2     			; final state {1}
       bit  toneb
       clc
       rts
s3
       jsr  rddipl
       bvs  s4
       bpl  s5
       bmi  s8
s4
       jsr  rddipl
       bcs  s8
       bvs  s4
       bpl  s8
       bmi  s6
s5     			; final state {0}
       bit  tzerob
       clc
       rts
s6
       jsr  rddipl
       bcs  s8
       bvs  s8
       bpl  s7
       bmi  s8
s7     			; final state {w}
       bit  twordm
       clc
       rts
s8     			; final state {error}
       sec
       rts



; sync on a word marker
; ...attempt to do a fixed # of times as defined by value passed
; in accumulator and report failure when it occurs.

wsync
       tsx             	; mark stack
       stx  drecov

       clc
       ror  enext      	; no propagation of errors
       cli             	; enable interrupts

wserr
       jsr  rdbit
       bcs  wserr      	; a dipole error
       bvs  wserr      	; a zero
       bpl  wserr      	; a one
		       	; else got a wm!
       jsr  setd1      	; setup t1 for delta 1
       clc
       rts



; read a byte from tape into (tpbyte)
; c=0=>ok, (c=1)&(a=1)=>parity error, (c=1)&(a=2)=> timing error
;
rdbyte 			; look for a wm, then read a byte
       bit  enext      	; a carry over error?
       bmi  rdbite     	; yep

       jsr  wsync
       bcs  rdbite     	; ok, well

rdbytd 			; read a byte directly, don't look for a wm...
       lda  #1
       sta  parity

       ldx  #8         	; init counter
       stx  rdbits

       sec
       ror  enext      	; assume error propagate

rdbylp
       jsr  rdbit
       bcs  rdbite     	; read bit in error
       bvs  rdzero     	; read a 0
       bpl  rdone      	; read a 1
       bmi  rdbite     	; got a wm!

rdzero
       clc
       ror  tpbyte     	; shift a 0 into tpbyte
       inc  parity
       dec  rdbits
       bne  rdbylp     	; more bits
       beq  rdpary     	; no more, check parity

rdone
       sec
       ror  tpbyte     	; shift a 1 into tpbyte
       dec  rdbits
       bne  rdbylp     	; more bits
		       	; else do parity
rdpary
       jsr  rdbit
       bcs  rdbite     	; bad bit
       bvs  pzero      	; parbit=0
       bpl  pone       	; parbit=1
       bmi  rdbite     	; bad bit

pzero
       lda  parity
       and  #1
       bne  rdbite     	; parity should be a 0 but its not!
       beq  rbytok

pone
       lda  parity
       and  #1
       beq  rdbite     	; parity should be a 1 but its not!
		       	; else ok...
rbytok
       clc             	; good exit
       bcc  mork
rdbite 			; error exit
       sec
mork
       sei
       php             	; save status
       clc
       ror  enext      	; no next error
       plp             	; restore status
       rts


; ###  read first elemental tape block ###
; assume counter & pointer are already specified

rtblok
       tsx             	; save stack mark
       stx  srecov

       lda  verfck     	; fix verify flag
       beq  fload      	; a load =0, leave alone
       sec             	; a verify== msb=1
       ror  verfck
fload
       jsr  moton      	; roll 'em
       jsr  faster

       lda  trsave     	; get counter & pointer values for 1st pass
       sta  tapebs
       lda  trsave+1
       sta  tapebs+1
       lda  trsave+2
       sta  rdcnt
       lda  trsave+3
       sta  rdcnt+1

       jsr   ldsync    	; sync on first leader

       ldy   #0        	; init errlist, #errors, & checksum
       sty   errsp
       sty   fperrs
       sty   chksum
       sty   errsum
       sty   type      	; !assume no type initially

       lda   #tapebs   	; set up kludes in case of verify
       sta   sinner

       bit   typenb    	; does a type exist in this block?
       bpl   rtl1      	; no

       jsr  rdbyte
       bcs  fpterr     	; first pass type error
       lda  tpbyte
       sta  type       	; type is ok
       eor  chksum     	; !type is included in checksum
       sta  chksum
       jmp  rtl1       	; continue with data
fpterr
       sec
       ror  type       	; set msb of type to=1=>error

rtl1
       jsr   rdbyte    	; read a byte from the block
       bcs   rerrfb    	; first eblock read error
			; else ok
       ldy #0          	;  read a byte from tape to tpbyte
       jsr kludes      	; acc <= byte in memory at desired location
       nop
       bit verfck      	; if we are doing a load
       bmi rtl111
       lda tpbyte      	;       acc <= byte from tape
rtl111
       cmp tpbyte      	; if    acc <> byte from tape
       bne rerrfb      	;       go execute code for error!


fpload
       sta   (tapebs),y 	;stash byte

mc6809
       eor   chksum    	; update checksum
       sta   chksum
       jmp   rincfp

rerrfb 			; ***  read error
       ldy   errsp
       cpy   #estksz
       bcs   errovf    	; bra if errsp>=estksz
       lda   tapebs    	; put address in table
       sta   estakl,y
       lda   tapebs+1
       sta   estakh,y
       inc   errsp     	; another entry
       inc   errsum    	; another error
       jmp   rincfp

errovf 			; error table overflow
       lda   #$ff
       sta   errsp     	

; special case of ptr, simulate errors>>>30

rincfp
       inc   tapebs
       bne   incnt1
       inc   tapebs+1
incnt1
       inc   rdcnt
       bne   rtl1
       inc   rdcnt+1
       bne   rtl1	; done with first eblock

       lda  errsp      	; errsp=>fperrs on entry to 2nd pass
       sta  fperrs
       jsr   rdbyte    	; get chksum
       lda   fperrs
       bne   errfp     	; we have some fp errors

; else no errors detected, test checksum

       lda   tpbyte    	; check checksum
       cmp   chksum
       bne   ckerr     	; branch if bad; else good
errfp
       jmp   rtebk2    	; no checksum error & 0<=errors<=30

ckerr
       lda   pass      	; first eblock fatal, but was this the second pass?
       bmi   rtebk2    	; no, so try to read 2nd pass
       jmp   reberr    	; read fatal error exit #1


; ###  read elemental tape block for second pass  ###
;
rtebk2
       lda   pass
       bmi   wasfp     	; that was the fp
       lda   fperrs    	; was 2nd pass, any errors?
       beq   nerrfp    	; nope
       jmp  reberr     	; yep, read fatal error exit #2
nerrfp
       jmp  rebgd      	; good exit

wasfp  			; have read fp & 0<=errors<=30
       lda   #0
       sta   errsp     	; reset error pointer
       sta   chksum

       lda   trsave    	; restore pointers & counters
       sta   tapebs
       lda   trsave+1
       sta   tapebs+1
       lda   trsave+2
       sta   rdcnt
       lda   trsave+3
       sta   rdcnt+1

;      clc              ; tell no servo calc
       jsr   ldsync    	; sync on second block hopefuly

       bit   typenb    	; is there a type?
       bpl   rtl2      	; no

       jsr  rdbyte     	; get sp type
       bit  type       	; how was fp type ?
       bpl  udcks      	; ok--use it
       lda  tpbyte     	; fp was bad--rely on sp
       sta  type
       bcc  udcks      	; sp was good, don't ror in error flag
       ror  type
udcks
       lda  type
       eor  chksum     	; !type is included in checksum
       sta  chksum

rtl2
       jsr   rdbyte
       ror   rdetmp    	; save returned status
       lda  tpbyte     	; compute checksum
       eor  chksum
       sta  chksum

; was this a bad byte in fp?

       bit  fperrs     	; errors >>>30?
       bmi  notbad     	; yes, don't try to fix
       ldy   errsp
       cpy   fperrs
       beq   notbad    	; no errors in stack
       lda   estakl,y
       cmp   tapebs
       bne   notbad    	; nope
       lda   estakh,y
       cmp   tapebs+1
       bne   notbad    	; nope
       inc   errsp     	; byte was bad, advance ptr

; but is this one ok?

       lda   rdetmp
       bmi   e2both    	; fp & sp errors

; else sp is good, replace it

       ldy   #0
       jsr   kludes    	

; acc <- byte in ram ( maybe from behind rom )

       nop             	; make mem addresses come out the same
       bit  verfck     	; if this is a load
       bmi  rtl222
       lda  tpbyte     	; acc <- byte read from tape
rtl222
       cmp  tpbyte     	; if    acc <> byte from tape
       bne  e2both     	; go commit error code

spload
       dec  errsum     	; 1 less error
       sta  (tapebs),y 	; stash byte ( in case of load )

e2both 			; have an unrecoverable fp & sp error
		       	; just ignore...it'll get caught
notbad 			; advance pointer & counter
       inc   tapebs
       bne   inccn2
       inc   tapebs+1
inccn2
       inc   rdcnt
       bne   rtl2
       inc   rdcnt+1
       bne   rtl2

; done with sp read

       jsr   rdbyte    	; read chksum byte
spserr
       lda  #0         	; make system staus word say ok.
       sta  status
       lda  type       	; good return with type in accumilaator
       ldx errsum      	; if    no errors to speak of
       beq rebgd       	;       go exit with carry clear
       bit verfck      	; if    verify command
       bmi mvexit      	;       exit with a bad verify
			; fall through to irrecoverable error

reberr 			; read an block in error
       lda  #$60       	; indicate unrecoverable error.
       sta  status
       sec
       jmp  bweep      	; !bra


mvexit
       lda  #$10       	; indicate verify error
       sta  status
       sec
       jmp  bweep

rebgd  			; good exit
       clc
bweep
       jsr  motoff
       jsr  slower
       rts


; $$$  read a fixed length data block  $$$
;
fldbtb .byte       <tapbuf,>tapbuf,$41,$ff

rdblok
       ldy  #3         	; 4-bytes
ateam
       lda  fldbtb,y
       sta  trsave,y
       dey
       bpl  ateam

       sty  typenb     	; enable type read of tape(msb=1) y=$ff

; fixed block must be a write

       lda  verfck     	; save intended flag
       pha
       iny             	; y=0
       sty  verfck     	; enable write (=0)
       sty  tptr       	; reset fixed buffer pointer (=0)

       jsr  rtblok     	; read header

       pla
       sta  verfck     	; restore flag

; point to beg of buffer with tapebs

       jmp  bufini	; does rts       ; !carry preserved



; $$$  read a variable length block  $$$
;
rvblok
       lda  stal       	; sta(l,h) => trsave(0,1)
       sta  trsave
       lda  stah
       sta  trsave+1
       clc
       lda  eal        	; ((((sta-ea)+1)<xor> $ffff)+1) => trsave(2,3)
       sbc  stal
       eor  #$ff
       sta  trsave+2
       lda  eah
       sbc  stah
       eor  #$ff
       sta  trsave+3

       clc             	; disable type read
       ror  typenb
       jmp  rtblok	; does rts



; $$$  sync on leader  $$$

dstab  .byte       <ids1,>ids1,<ids2,>ids2,<izcell,>izcell

sparms= dsamp1		; keep these samp vals contiguous !!!!!!



; *** note: these values not changed whenn tape speed changed
;      ( see definitins for  tshort,tlong, and tbyte )

ids1=  268-10          	; dipole sample #1 initial delta
ids2=  536-22          	; dipole sample #2 initial delta
izcell=536-11          	; initial delta for a z-cell verify

ldsync
       ldx  #5
fweep
       lda  dstab,x
       sta  sparms,x
       dex
       bpl  fweep

molson
       lda  #10        	; find 10 consecutive good shorts
       sta  ldrscn
ale
       jsr  rddipl
       bcs  molson     	; dipole read in error
       bvc  molson     	; read a long or a word
			; got a short
       dec  ldrscn
       bne  ale        	; counting down


; measure dipole down-time via skewed samples to minimize
; sample-point granularity.
;
; ...this is as good as it gets folks:

sigma  = 16            	; number of skewed samples (x3)
accm=wrbase		; use wrbase for servo accumulator


servo
       lda  #0
       sta  accm
       sta  accm+1     	; init accum
       ldy  #sigma     	; for 2:1 sample ratio
sum
       ldx  #0
       lda  #$10
swhi
       bit  port       	; wait till high
       beq  swhi
swlo
       bit  port
       bne  swlo

; caught neg. edge:
;**************************************************************************
; caution!!!, the following loop is sensitive to crossing page boundaries:
;**************************************************************************

srvlow
       inx
       beq  servo      	; error, re-try
       bit  port       	; you figure it out...
       beq  srvlow     	; keep counting
srvhi
       inx
       beq  servo
       bit  port
       bne  srvhi

;*************************************************************************
;
; add current count to accm

       txa
       clc
       adc  accm
       sta  accm
       lda  #0
       adc  accm+1
       sta  accm+1
       dey
       bne  sum

; do *3 ---inherent in #of samples taken

       lsr  accm+1     	; do /4
       ror  accm
       lsr  accm+1
       ror  accm

       lda  accm       	; 3/4 absolute good for:
       sta  dsamp1     	; d1
       asl  a          	; 6/4 for:
       sta  dsamp2     	; d2, and
       sta  zcell      	; z-cell

       lda  accm+1
       sta  dsamp1+1
       rol  a
       sta  dsamp2+1
       sta  zcell+1


; ...i have a feeling we're not in kansas anymore

spansh 			; span all shorts till a wm
       jsr  rddipl
       bcs  spansh     	; error
       bvs  spansh     	; short
       bpl  spansh     	; long

; else got a word (maybe)

       jsr  rddipl
       bcs  spansh     	; error:backup
       bvs  spansh     	; short:backup
       bmi  spansh     	; word:backup

; ok, have seen a bm & have consumed it

       clc             	; boundary case
       ror  enext
       jsr  setd1      	; get ready for first byte
       lda  #3         	; max # of erros allowed in countdown
       sta  cderrm
       jsr  rdbytd     	; gobble up cd.9
       bcc  rdcdok     	; its ok
       dec  cderrm     	; first error
rdcdok
       jsr  rdbyte     	; read a countdown byte
       bcc  rcdok2     	; got a good read
       dec  cderrm     	; another error
       bne  rcdok2     	; not fatal
       jmp  ldsync     	; else==00, start from scratch
rcdok2
       lda  tpbyte
       and  #$f
       cmp  #1         	; is cdbyte=1?
       bne  rdcdok     	; no

; got cdbyte=1

       lda  tpbyte     	; specify pass
       and  #$80       	; msb of type=>pass
       sta  pass
       rts             	; good return


; find any header
; ---read tape until one of the following block types is found:
;  bdfh - basic data file header
;  blf  - basic load file
;
;  if c=0 then success  /* ac=type of header found */
;         else if ac=0 then stop/key/pressed
;                      else end/of/tape  /* ac=5 */
;              fi
;  fi


fah
       jsr  rdblok     	; read a data-type block
       bcs  fhderr     	; hey! stop key???  if not keep looking

       lda  type
       cmp  #eot
       beq  fah40      	; no more blocks
       cmp  #blf
       beq  gothed     	; good nuf
       cmp  #plf
       beq  gothed     	; good
       cmp  #bdfh
       bne  fah

gothed
       tax             	; is this necessary???
       bit  msgflg     	; r we printing messages?
       bpl  ctlo       	; nope
       jsr  primm
       .byte $d,'FOUND ',0

       ldy  #4
fah55  			; output filename
       lda  (tapebs),y
       jsr  bsout
       iny
       cpy  #21
       bne  fah55

; <wait 8 seconds> or <wait for <<run/stop> or <commodore> key>>

fah45
       ldx  #$ff
tapewt
       jsr  w16ms
       jsr  w16ms
       dex
       beq  ctlo

       lda  #$7f
       jsr  keyscn     	;** 01/17/84 mod for new keyboard port
;	cmp  #$ef       ; was this the space key?
;	beq  fah        ; yep, skip this file
       cmp  #$7f       	; was it the run/stop key
       beq  fhderr     	; yep
       cmp  #$df       	; how about the commodore key?
       bne  tapewt     	; no, continue with timeout loop

; else: yes, load this file

ctlo   			; got a timeout or user pressed commodore key
       clc
       lda  type
fah40
       rts
       .byte $ea,$ea,$ea
fhderr
       lda  #0
       rts


; find a file
;
; if c=0 then sucess   /* ac=type of header found */
;        else if ac=0 then stop/key/pressed
;                     else end/of/tape  /* ac=5 */
;             fi
;  fi

faf
       jsr  fah        	; pick any card
       bcs  faf35      	; ...not that one
       cmp  #eot        ;       if      end of tape
       beq  faf30      	;               go quit
			; ok let's take a look
       ldy  #$ff
faf20
       iny             	;       if      filename matches
       cpy  fnlen
       beq  faf40      	;               goto faf40
       lda  #fnadr
       sta sinner
       jsr kludes
       cmp tapbuf+4,y
       beq faf20
       lsr type       	;       if      type <> 1 or 3
       bcc faf         	;               go load next header
       ldy #$ff           	

;      read two bytes of prog into tape buffer

       sty trsave+3   	;       and go find next file
       dey
       sty trsave+2
       ldy #1
       jsr ateam
       jmp faf

faf35
       lda  #0         	; stop key pressed exit
faf30  			; forced error exit
       sec
       rts

faf40
       clc
       lda  type       	; ac returns type of header found
       rts

;end
