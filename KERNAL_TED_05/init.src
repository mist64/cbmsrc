	.page
	.subttl  'init  (kernal)  02/17/84'
;****************************************
;
;   start  -  all kernal initialization
;             is done here.
;
;****************************************

start
       ldx #$ff
       sei
       txs
       cld
       jsr spkpat      	;see which cartridge slots are occupied (and go if autostart)
       jsr ioinit      	;initialize ted, i/o

       jsr ud60        	;test for run/stop key down <and> 'cold' reset
       php             	;(save for 'jmp to monitor' test later)
       bmi start1      	;key not down, do full init
       lda #$a5
       cmp dejavu      	;test for 'cold' or 'warm' reset
       beq start2      	;it is 'warm' reset w/ run/stop down: skip 'ramtas'

start1
       jsr ramtas      	;find mem top, init system storage, download ram code
start2
       jsr restor      	;set up indirects
       jsr cint        	;initialize screen & editor
       plp
       bmi basic       	;branch to basic if no run/stop key

       jmp entry       	;go to monitor
basic
       jmp $8000       	;go to basic



;********************************************
;
;  restor - set kernal indirects and vectors
;
;********************************************

restor
       ldx #<vectss
       ldy #>vectss
       clc

;  vector - set kernal indirect and vectors (user)
;
vector
       stx tmp2
       sty tmp2+1
       ldy #vectse-vectss-1
movos1
       lda itime,y     	;get from storage
       bcs movos2      	;c...want storage to user
       lda (tmp2),y     	;...want user to storage
movos2
       sta itime,y     	;put in storage
       bcc movos3
       sta (tmp2),y     	;put in user
movos3
       dey
       bpl movos1
       rts

vectss
       .wor ntime,srvirq,entbrk
       .wor nopen,nclose,nchkin
       .wor nckout,nclrch,nbasin
       .wor nbsout,nstop,ngetin
       .wor nclall,entbrk
       .wor nload,nsave
vectse
	.page
;************************************
;
;  ioinit - initialize i/o
;
;************************************

ioinit
       lda #%00001111  	;set up 6510 port
       sta pdir
       lda #%00001000
       sta port

       ldx #$ff
       stx xport       	;init 6529
       stx drva2       	;cmd channel all outputs
       inx             	;.x=0
       stx drvb2       	;inputs
       stx tedrva      	;clear cmd

       lda #%01000000  	;($40)
       sta drvc2       	;set up handshake line
;	sta tedrvc	;init line to '1'
       jsr patchd

initlp
       lda table,x     	;(.x=0 initially)
       sta ted,x       	;init ted registers
       inx
       cpx #26
       bne initlp

       jmp ainit       	;init rs-232 buffers & rts


table
       .byte $f1,$39    	;0,1:   t1
       .byte 0,0        	;2,3:   t2
       .byte 0,0        	;4,5:   t3
       .byte $1b        	;6:     elm=0, bmm=0, blnk=1, 25 rows, y=3

	.if  palmod
       .byte $08		;7:     rev. video on,pal,freeze=0,mcm=0,40col,x=0
	.else
       .byte $48		;7:     rev. video on,ntsc,freeze=0,mcm=0,40col,x=0
	.endif

       .byte 0,0        	;8,9:   kbd, int read (don't care)
       .byte $02        	;10:    disable all interrupts except raster
       .byte $cc        	;11:    raster compare (end of screen)
       .byte 0,0        	;12,13: cursor position
       .byte 0,0        	;14,15: lsb of sound 1 & 2
       .byte 0          	;16:    msb of sound 2 off
       .byte 0          	;17:    no voice, volume off
       .byte $04        	;18:    bm base, charset from rom, ms bits sound 1 off
       .byte $d0        	;19:    character base @ $d000, single clock, status
       .byte $08        	;20:    vm base @ $c00
       .byte $71        	;21:    bkgd 0, ful lum, white
       .byte $5b        	;22:    bkgd 1, med lum, lt. red
       .byte $75        	;23:    bkgd 2, ful lum, lt. green (not used)
       .byte $77        	;24:    bkgd 3, ful lum, yellow (not used)
       .byte $6e        	;25:    exterior (ful lum)-1, dk. blue
	.page
;*****************************************
;
;  ramtas - memory size check and set
;
;*****************************************

ramtas
       lda #0          	;zero low memory
       tay
ramtz0
       sta $0002,y     	;zero page (skip over 6510 port!)
       sta $0200,y     	;user buffers and vars
       sta $0300,y     	;system space and user space
       sta $0400,y     	;basic stuff
       sta $0700,y     	;editor stuff
       iny
       bne ramtz0

;  set top of memory
;
ramtbt
       ldx #8          	;(.y=0 already)
       stx t1          	;set up a counter
ramtlp			;copy reset vector & code into ram, in case of a reset
       lda gostrt-1,x  	;..while rom is banked out.
       sta gostrt-1,x
       cmp gostrt-$c000-1,x 	;test if write appeared in lowest 16k bank.
       bne ramtzx      	;nope
       iny             	;yes, means write may have bled thru. count # of occurances
ramtzx
       cmp gostrt-$8000-1,x 	;test if write appeared in second 16k bank.
       bne ramtzy      	;nope
       dec t1
ramtzy
       dex
       bne ramtlp      	;go for 8 bytes

       cpy #8          	;did all 8 bytes bleed thru to lowest 16k?
       beq ramtz3      	;yes, means 16k system
       lda t1          	;did all 8 bytes bleed thru to second 16k?
       bne ramtz1      	;must be 64k system
       ldy #$7f        	;32k system, top of ram at $7ff6
       .byte $2c

ramtz3
       ldy #$3f        	;16k system, top of ram at $3ff6
       ldx #$f6
       .byte $2c

ramtz1
       ldy #$fd        	;64k system..top of memory @ $fd00 (*assume x=0*)
ramtz2
       clc
       jsr settop

       lda #$10        	;set bottom of ram at $1000
       sta memstr+1

       ldx #pfkend-pfktbl
pfinit
       lda pfktbl-1,x  	;init prog. function keys
       sta keybuf-1,x
       dex
       bne pfinit
       stx kyndx       	;clear key count

       ldx #ugh-sobsob-1
mmm
       lda sobsob,x    	;download indirect routine for save & verify
       sta kludes,x
       dex
       bpl mmm

       ldx #15
mmmm
       lda collum,x    	;download color default values to mr. f's table
       sta colkey,x    	;now mr. f can choose his own colors!!!
       dex
       bpl mmmm

       lda #$a5
       sta dejavu      	

;  ted is now 'warm': set flag to show ram is init-ed
;
       lda #$04
       sta ffrmsk      	;fetch characters from rom
       lda #$18
       sta vmbmsk      	;set video matrix base pointer
       rts

	.byte   $ea,$ea ;(** 01/24/84 patch to conform to 'ces' release!)


pfktbl
       .byte key2-key1
       .byte key3-key2
       .byte key4-key3
       .byte key5-key4
       .byte key6-key5
       .byte key7-key6
       .byte key8-key7
       .byte pfkend-key8
key1
       .BYTE 'GRAPHIC'
KEY2
       .BYTE 'DLOAD',34
KEY3
       .BYTE 'DIRECTORY',CR
KEY4
       .BYTE 'SCNCLR',CR
KEY5
       .BYTE 'DSAVE',34
KEY6
       .BYTE 'RUN',CR
KEY7
       .BYTE 'LIST',CR
KEY8
       .BYTE 'HELP',CR
pfkend
	.page
setnam
       sta fnlen       	;set up filename
       stx fnadr
       sty fnadr+1
       rts


setlfs
       sta la          	;set up la, fa, sa
       stx fa
       sty sa
       rts


setmsg
       sta msgflg      	;enable/disable kernal messages
readss
       lda status      	;read i/o status
udst
       ora status      	;update i/o status
       sta status
       rts


settmo
       sta timout      	;update timeout flag
       rts


;  manage top of memory
;
memtop
       bcc settop

;  carry set - read top of memory
;
gettop
       ldx msiz
       ldy msiz+1

;  carry clear - set top of memory
;
settop
       stx msiz
       sty msiz+1
       rts

;  manage bottom of memory
;
membot
       bcc setbot

;  carry set - read bottom of memory
;
       ldx memstr
       ldy memstr+1

;  carry clear - set bottom of memory
;
setbot
       stx memstr
       sty memstr+1
       rts

;end
