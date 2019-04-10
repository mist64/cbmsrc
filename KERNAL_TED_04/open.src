	.page
	.subttl 'open'
;***********************************
;*                                 *
;* open function                   *
;*                                 *
;* creates an entry in the logical *
;* files tables consisting of      *
;* logical file number--la, device *
;* number--fa, and secondary cmd-- *
;* sa.                             *
;*                                 *
;* a file name descriptor, fnadr & *
;* fnlen are passed to this routine*
;*                                 *
;* on entry c=0==>normal open      *
;* with file tables used           *
;* ...c=1==> xmit open only        *
;*                                 *
;***********************************

nopen
       ldx la          	;check file #

op98
       jsr lookup      	;see if in table
       bne op100       	;not found...o.k.
       jmp error2      	;file open

op100
       ldx ldtnd       	;logical device table end
       cpx #10         	;maximum # of open files
       bcc op110       	;less than 10...o.k.
       jmp error1      	;too many files

op110
       inc ldtnd       	;new file
       lda la
       sta lat,x       	;store logical file #
       lda sa
       ora #$60        	;make sa an serial command
       sta sa
       sta sat,x       	;store command #
       lda fa
       sta fat,x       	;store device #
;
;  perform device specific open tasks
;
       beq op300       	;is keyboard...done.
       cmp #3
       beq op300       	;is screen...done.
       bcc op150       	;devices 1 or 2

       jsr openi       	;is on serial...open it
op300
       clc             	;done
       rts

op150
       cmp #2
       bne op152
;
;  open for rs-232
;
       jsr ainit       	;init
       tax             	;x=0
avery
       inx
       beq ahere       	;i think he's home
       stx acia+3      	;write to control reg
       cpx acia+3      	;verify control reg
       beq avery       	;so far, so good
       jmp error5      	;verify error

ahere
       sec
       ror apres       	;indicate to sysm that acia is present
       lda #fnadr      	;set up indirect subby
       sta sinner
       ldy #0
       jsr kludes      	;get control byte
       sta acia+3
       iny
       jsr kludes      	;get command byte
       sta acia+2

       clc             	;don't shoot.
       rts
;
;  open for cassette
;
op152
       lda sa
       and #$f
       bne op200       	;non-zero =>tape write
       jsr tstply      	;tell user to press play for tape read
       bcs op180       	;doesn't want to play
       jsr luking      	;tell: searching

       lda fnlen
       beq op170       	;=0=>looking for any file
       jsr faf         	;looking for named file
       bcc op171       	;found it!
       beq op180       	;stop key pressed...
op160
       jmp error4      	;file not found (.a=0 .c=1)

op170
       jsr fah
       beq op180       	;get any header
       bcs op160       	;file not found
       cmp #eot
       beq op160       	;nothin' left
op171
       ldy #bufmax     	;force a read upon first input from buffer
       sty tptr
       lda #bdf        	;tell sysm type of cominng data
       sta type

op175
       clc             	;everything's fine
op180
       rts


op200
       jsr tstrec      	;tell: press play & record for tape write
       bcs op180
       lda #bdfh
       sta type
       jsr tphead      	;write out data block header
       bcs op201       	;woops!

       lda #bdf        	;!c=0 /setup type for next data block
       sta type
       ldy #0
       sty tptr
       sty ctally
op201
       rts             	;(.c=0)


openi
       lda sa
       bmi op175       	;no sa...done

       ldy fnlen
       beq op175       	;no file name...done

       lda #0          	;clear the serial status
       sta status

       lda fa
       jsr tlistn      	;device la to listen
       bit status      	;anybody home
       bmi unp         	;nope

       lda sa
       ora #$f0
       jsr tsecnd

       lda status      	;anybody home?...get a dev -pres?
       bpl op35        	;yes...continue

;  this routine is called by other
;  kernal routines which are called
;  directly by os. kill return
;  address to return to os.

unp
       pla
       pla
       jmp error5      	;device not present

op35
       lda fnlen
       beq op45        	;no name...done sequence
;
;  send file name over serial
;
       ldy #0
op40
       lda #fnadr
       sta sinner
       jsr kludes
       jsr tciout
       iny
       cpy fnlen
       bne op40

op45
       jmp cunlsn      	;jsr unlsn: clc: rts

;end

