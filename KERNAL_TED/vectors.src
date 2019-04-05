	.page
	.subttl  'vectors  02/17/84'
	*=$ff4c
       jmp print       	;**must be here**... basic needs this jump
       jmp primm       	;**must be here**... basic needs this jump
       jmp entry       	;**must be here**... basic needs this jump

	*=$ff80
	.if   palmod
	.byte revnum+$80 ;release number of ted kernal (msb=1=pal version)
	.else
	.byte revnum	;release number of ted kernal (msb=0=ntsc version)
	.endif

       jmp cint
       jmp ioinit
       jmp ramtas
       jmp restor      	;restore vectors to initial system
       jmp vector      	;change vectors for user
       jmp setmsg      	;control o.s. messages
       jmp tsecnd      	;send sa after listen          /cheap/
       jmp ttksa       	;send sa after talk            /cheap/
       jmp memtop      	;set/read top of memory
       jmp membot      	;set/read bottom of memory
       jmp scnkey      	;scan keyboard
       jmp settmo      	;set timeout in ieee
       jmp tacptr      	;handshake ieee byte in         /cheap/
       jmp tciout      	;handshake ieee byte out        /cheap/
       jmp tuntlk      	;send untalk out ieee           /cheap/
       jmp tunlsn      	;send unlisten out ieee         /cheap/
       jmp tlistn      	;send listen out ieee           /cheap/
       jmp ttalk       	;send talk out ieee             /cheap/
       jmp readss      	;return i/o status byte
       jmp setlfs      	;set la, fa, sa
       jmp setnam      	;set length and fn adr
open   jmp (iopen)     	;open logical file
close  jmp (iclose)    	;close logical file
chkin  jmp (ichkin)    	;open channel in
ckout  jmp (ickout)    	;open channel out
clrch  jmp (iclrch)    	;close i/o channel
basin  jmp (ibasin)    	;input from channel
bsout  jmp (ibsout)    	;output to channel
       jmp loadsp      	;load from file
       jmp savesp      	;save to file
       jmp settim      	;set internal clock
       jmp rdtim       	;read internal clock
stop   jmp (istop)     	;scan stop key
getin  jmp (igetin)    	;get char from q
clall  jmp (iclall)    	;close all files
judtim jmp udtim       	;increment clock
jscrog jmp scrorg      	;screen org
jplot  jmp plot        	;read/set x,y coord
       jmp iobase
	.page
; the following code is necessary to prevent the problem where
; the reset button is pressed while the rom is banked out. since
; the ted chip has no reset pin, the processor will attempt to
; fetch the reset vectors without banking in rom, and will get
; garbage. this code is copied into ram behind the reset vectors,
; and will switch the rom back on before transferring execution
; to the reset routine.

gostrt
       sta romon
       jmp start
	.wor   gostrt          	;initialization code
	.wor   puls            	;interrupt handler

;end
