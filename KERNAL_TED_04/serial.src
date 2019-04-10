	.page
	.subttl 'serial'
;  command serial bus device to talk
;
talk
       ora #$40        	;make a talk adr
       .byte $2c


;  command serial bus device to listen
;
listn
       ora #$20        	;make a listen adr

list1
       pha
       bit c3p0        	;character left in buf?
       bpl list2       	;no...

;  send buffered character
;
       sec             	;initialize eoi flag
       ror r2d2
       jsr isour       	;send last character
       lsr c3p0        	;buffer clear flag
       lsr r2d2        	;clear eoi flag

list2
       pla             	;talk/listen address
       sta bsour
       sei
       jsr datahi
       jsr clklo

list5
       lda port        	;assert attention
       ora #%00000100
       sta port


isoura
       sei
       jsr clklo       	;set clock line low
       jsr datahi
       jsr w1ms        	;delay 1 ms


isour
       sei             	;no irq's allowed
       jsr datahi      	;make sure data is released
       jsr debpia      	;data should be low
       bcs nodev       	;%%
       jsr clkhi       	;clock line high
       bit r2d2        	;eoi flag test
       bpl noeoi

;  do the eoi
;
isr02
       jsr debpia      	;wait for data to go high
       bcc isr02       	;%%

isr03
       lda port        	;wait for data to go low
       cmp port
       bne isr03
       asl a
       bcs isr03

noeoi
       jsr debpia      	;wait for data high
       bcc noeoi       	;%%
       jsr clklo       	;set clock low

;  set to send data
;
       lda #$08        	;count 8 bits
       sta dcount
isr01
       jsr debpia      	;debounce the bus & shift data-in bit to carry bit
       bcc frmerr      	;...if data=low then, frame error

       ror bsour       	;next bit into carry
       bcs isrhi
       jsr datalo
       bne isrclk
isrhi
       jsr datahi
isrclk
       jsr dilly
       jsr clkhi       	;clock hi
       jsr dilly       	;mod for ted 1.7 mhz clk
       lda port
       and #%11111110  	;data high
       ora #%00000010  	;clock low
       sta port
       dec dcount
       bne isr01

isr04
       txa
       pha
       ldx #120
isr04a
       lda port
       cmp port
       bne isr04a
       asl a
       bcc isr04b
       dex
       bne isr04a
       pla
       tax
       bcs frmerr      	;always

isr04b
       pla
       tax
       cli             	;irq's ok now
       rts

nodev
       lda #$80
       jmp csberr
frmerr
       lda #$03        	;framing error
csberr
       jsr udst        	;commodore serial bus error entry
       cli             	;irq's were off...turn on
       clc             	;make sure no kernal error returned
       bcc dlabye      	;turn atn off ,release all lines


;  send secondary address after listen
;
secnd
       sta bsour       	;buffer character
       jsr isoura      	;send it


;  release attention after listen
;
scatn
       lda port
       and #%11111011
       sta port        	;release attention
       rts


;  talk second address
;
tksa
       sta bsour       	;buffer character
       jsr isoura      	;send second addr
       bit status      	;sa sent ok?
       bmi dlabye      	;no, get out!


tkatn
       sei             	;shift over to listener. no irq's
       jsr datalo      	;data line low
       jsr scatn
       jsr clkhi       	;clock line high jsr/rts
tkatn1
       bit port        	;wait for clock to go low ( use bit ( timing shit ))
       bvs tkatn1      	;%%
       cli             	;irq's okay now
       rts
       .byte $ff        ; free


;  buffered output to serial bus
;
ciout
       bit c3p0        	;buffered char?
       bmi ci2         	;yes...send last

       sec             	;no...
       ror c3p0        	;set buffered char flag
       bne ci4         	;branch always

ci2
       pha             	;save current char
       jsr isour       	;send last char
       pla             	;restore current char
ci4
       sta bsour       	;buffer current char
       clc             	;carry-good exit
       rts


;  send untalk command on serial bus
;
untlk
       sei
       jsr clklo
       lda port        	;pull atn
       ora #%00000100
       sta port
       lda #$5f        	;untalk command
       bne untlk2      	;!bra


;  send unlisten command on serial bus
;
unlsn
       lda #$3f        	;unlisten command
untlk2
       jsr list1       	;send it

;  release all lines
;
dlabye
       jsr scatn       	;always release atn

;  delay then release clock and data
;
dladlh
       txa             	;delay at least 60 us (max=120us w. 2x ted clock)
       ldx #20
dlad00
       dex
       bne dlad00
       tax
       jsr clkhi
       jmp datahi


;  input a byte from serial bus
;
acptr
       sei             	;no irq allowed
       lda #$00        	;set eoi/error flag
       sta dcount
       jsr clkhi       	;make sure clock line is released

acp00a
       txa
       pha
acpz
       jsr debpia
       bpl acpz
eoiacp
       ldx #32
       jsr datahi
acpy
       lda port
       cmp port
       bne acpy
       asl a
       bpl acp01
       dex
       bne acpy
       lda dcount
       beq acp00c
       pla
       tax
       lda #2
       jmp csberr


;  timer ran out do an eoi thing
;
acp00c
       jsr datalo
       ldx #64
hohum
       dex
       bne hohum
       lda #$40
       jsr udst
       inc dcount
       bne eoiacp

;  do the byte transfer from serial bus
;
acp01
       ldx #$8         	;set up counter

acp03
       lda port        	;%% wait for clk hi
       asl a           	;shift clk to d7, data into carry
       bpl acp03       	;clock still low...

       ror bsour1      	;rotate data bit in

acp03a
       lda port        	;debounce, then wait for clk low
       cmp port
       bne acp03a
       asl a
       bmi acp03a      	;clk still hi...wait
       dex             	;another bit ???
       bne acp03       	;more bits.....
       stx dcount      	;update dcount
       pla             	;restore x
       tax             	;from stack

;  ...exit...
;
       jsr datalo      	;set data low to do frame handshake
       lda #%01000000
       bit status      	;check for eoi
       bvc acp04       	;none...

       jsr dladlh      	;delay then set data high

acp04
       lda bsour1      	;return constructed byte in acc
       cli             	;irq is ok now
       clc             	;good exit
       rts

clkhi
       lda port        	;set clock line high (inverted)
       and #%11111101
       sta port
       rts

clklo
       lda port        	;set clock line low (inverted)
       ora #%00000010
       sta port
       rts

datahi
       lda port        	;set data line high (inverted)
       and #%11111110
       sta port
       rts

datalo
       lda port        	;set data line low (inverted)
       ora #%00000001
       sta port
       rts

debpia
       lda port        	;debounce the pia
       cmp port
       bne debpia
       asl a           	;%% data bit->carry, clock bit->d7
       rts

w1ms
       jsr twait1      	;delay 1ms using t2
       lda #$10
bzywt
       bit tedirq
       beq bzywt       	;wait until t2 times out
       sta tedirq      	;reset t2 interrupt
       rts

w16ms
       jsr twait3      	;delay 16ms using t2
       lda #$10
bzywt2
       bit tedirq
       beq bzywt2      	;wait until t2 times out
       sta tedirq      	;reset t2 interrupt
       rts


;  timer timeout setup routine for 16ms, 1ms & 256us deltas
;
twait1
       lda #$04        	;entry for 1ms time delay
       bne stime
twait3
       lda #$40        	;entry for 16ms time delay
stime
       php             	;to save int enb bit
       pha             	;save high time val
       sei             	;can't be interrupted here
       lda #0
       sta timr2l
       pla
       sta timr2h      	;start timer running
       lda #$10
       sta tedirq      	;clear any prior flag
       plp             	;restore int enb bit
       rts


;  dilly dally for the data to setup to clock pos tran
;
dilly
       txa
       ldx #5
dally
       dex
       bne dally
       tax
       rts

;end
