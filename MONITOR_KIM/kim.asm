;- - - - - - - - - - - - - - KIM.ASM - - - - - - - - - - -
; COPYRIGHT MOS TECHNOLOGY, INC
; DATE: OCT 18, 1975 REV-D
;************************ 6530-003 I.C. ******************
; 6530-003 I.C. IS AN AUDIO CASSETTE TAPE RECORDER
; EXTENSION OF THE BASIC KIM MONITOR.  IT FEATURES
; TWO ROUTINES:
;   LOADT - LOAD MEMORY FROM AUDIO TAPE
;     ID=00     IGNORE ID
;     ID=FF     IGNORE ID, USE SA FOR START ADDR
;     ID=01-FE  USE ADDRESS ON TAPE
;
;   DUMPT - STORE MEMORY ONTO AUDIO TAPE
;     ID=00     SHOULD NOT BE USED
;     ID=FF     SHOULD NOT BE USED
;     ID=01-FE  NORMAL ID RANGE
;     SAL       LSB STARTING ADDRESS OF PROGRAM
;     SAH       MSB
;     EAL       ENDING ADDRESS OF PROGRAM
;     EAH       MSB
;
        *=    $1800
SAD     =     $1740     6530 A DATA
PADD    =     $1741     6530 A DATA DIRECTION
SBD     =     $1742     6530 B DATA
PBDD    =     $1743     6530 B DATA DIRECTION
CLK1T   =     $1744     DIV BY 1 TIME
CLK8T   =     $1745     DIV BY 8 TIME
CLK64T  =     $1746     DIV BY 64 TIME
CLKKT   =     $1747     DIV BY 1024 TIME
CLKRDI  =     $1747     READ TIME OUT BIT
CLKRDT  =     $1746     READ TIME
;       ** MPU REG.  SAVX AREA IN PAGE 0 **
PCL     =     $EF       PROGRAM CNT LOW
PCH     =     $F0       PROGRAM CNT HI
PREG    =     $F1       CURRENT STATUS REG
SPUSER  =     $F2       CURRENT STACK POINTER
ACC     =     $F3       ACCUMULATOR
YREG    =     $F4       Y INDEX
XREG    =     $F5       X INDEX
;       ** KIM FIXED AREA IN PAGE 0  **
CHKHI   =     $F6
CHKSUM  =     $F7
INL     =     $F8       INPUT BUFFER
INH     =     $F9       INPUT BUFFER
POINTL  =     $FA       LSB OF OPEN CELL
POINTH  =     $FB       MSB OF OPEN CELL
TEMP    =     $FC
TMPX    =     $FD
CHAR    =     $FE
MODE    =     $FF
;       ** KIM FIXED AREA IN PAGE 23 **
CHKL    =     $17E7
CHKH    =     $17E8     CHKSUM
SAVX    =     $17E9     (3-BYTES)
VEB     =     $17EC     VOLATILE EXEC BLOCK (6-B)
CNTL30  =     $17F2     TTY DELAY
CNTH30  =     $17F3     TTY DELAY
TIMH    =     $17F4
SAL     =     $17F5     LOW STARTING ADDRESS
SAH     =     $17F6     HI STARTING ADDRESS
EAL     =     $17F7     LOW ENDING ADDRESS
EAH     =     $17F8     HI ENDING ADDRESS
ID      =     $17F9     TAPE PROGRAM ID NUMBER
;       ** INTERRUPT VECTORS **
NMIV    =     $17FA     STOP VECTOR (STOP=1C00)
RSTV    =     $17FC     RST VECTOR
IRQV    =     $17FE     IRQ VECTOR (BRK=1C00)

;
;       ** DUMP MEMORY TO TAPE **
DUMPT   LDA   #$AD      LOAD ABSOLUTE INST            1800
        STA   VEB   
        JSR   INTVEB
        LDA   #$27      TURN OFF DATAIN PB5
        STA   SBD   
        LDA   #$BF      CONVERT PB7 TO OUTPUT
        STA   PBDD  
        LDX   #$64      100 CHARS 
DUMPT1  LDA   #$16      SYNC CHARS
        JSR   OUTCHT
        DEX         
        BNE   DUMPT1
        LDA   #$2A      START CHAR
        JSR   OUTCHT
        LDA   ID        OUTPUT ID 
        JSR   OUTBT 
        LDA   SAL       OUTPUT STARTING
        JSR   OUTBTC    ADDRESS 
        LDA   SAH   
        JSR   OUTBTC
DUMPT2  LDA   VEB+1     CHECK FOR LAST
        CMP   EAL       DATA BYTE
        LDA   VEB+2 
        SBC   EAH   
        BCC   DUMPT4
        LDA   #'/       OUTPUT END-OF-DATA CHAR
        JSR   OUTCHT
        LDA   CHKL      LAST BYTE HAS BEEN 
        JSR   OUTBT     OUTPUT  NOW OUTPUT
        LDA   CHKH      CHKSUM
        JSR   OUTBT 
        LDX   #$02      2 CHARS
DUMPT3  LDA   #$04      EOT CHAR       
        JSR   OUTCHT 
        DEX          
        BNE   DUMPT3    
        LDA   #$00      DISPLAY 0000
        STA   POINTL    FOR NORMAL EXIT
        STA   POINTH  
        JMP   START
DUMPT4  JSR   VEB       DATA BYTE OUTPUT
        JSR   OUTBTC
        JSR   INCVEB
        JMP   DUMPT2
;
;       ** LOAD MEMORY FROM TAPE **
;
TAB     .WORD LOAD12      'LOAD12' ADDRESS            1871
;
LOADT   LDA   #$8D      INIT VOLATILE EXECUTION       1873
        STA   VEB       BLOCK WITH STA ABS.
        JSR   INTVEB
        LDA   #$4C      JUMP TYPE RTRN
        STA   VEB+3
        LDA   TAB  
        STA   VEB+4
        LDA   TAB+1
        STA   VEB+5
        LDA   #$07      RESET PB5=0 (DATA-IN)
        STA   SBD
SYNC    LDA   #$FF      CLEAR SAVX FOR SYNC CHAR      1891
        STA   SAVX 
SYNC1   JSR   RDBIT     GET A BIT        
        LSR   SAVX      SHIFT BIT INTO CHAR
        ORA   SAVX
        STA   SAVX
        LDA   SAVX      GET NEW CHAR
        CMP   #$16      SYNC CHAR
        BNE   SYNC1
        LDX   #$0A      TEST FOR 10 SYNC CHARS
SYNC2   JSR   RDCHT                      
        CMP   #$16  
        BNE   SYNC      IF NOT 10 CHAR, RE-SYNC
        DEX     
        BNE   SYNC2
LOADT4  JSR   RDCHT     LOOK FOR START OF
        CMP   #$2A      DATA CHAR
        BEQ   LOAD11
        CMP   #$16      IF NOT , SHOULD BE SYNC
        BNE   SYNC
        BEQ   LOADT4
LOAD11  JSR   RDBYT     READ ID FROM TAPE
        CMP   ID        COMPARE WITH REQUESTED ID
        BEQ   LOADT5
        LDA   ID        DEFAULT 00, READ RECORD
        CMP   #$00      ANYWAY
        BEQ   LOADT5
        CMP   #$FF      DEFAULT FF, IGNORE SA ON
        BEQ   LOADT6    TAPE
        BNE   LOADT
LOADT5  JSR   RDBYT     GET SA FROM TAPE
        JSR   CHKT
        STA   VEB+1     SAVX IN VEB+1,2
        JSR   RDBYT
        JSR   CHKT
        STA   VEB+2
        JMP   LOADT7
;
LOADT6  JSR   RDBYT     GET SA BUT IGNORE             18EC
        JSR   CHKT
        JSR   RDBYT
        JSR   CHKT
LOADT7  LDX   #$02      GET 2 CHARS 
LOAD13  JSR   RDCHT     GET CHAR (X)
        CMP   #$2F      LOOK FOR LAST CHAR
        BEQ   LOADT8
        JSR   PACKT     CONVERT TO HEX
        BNE   LOADT9    Y=1 NON-HEX CHAR
        DEX                         
        BNE   LOAD13
        JSR   CHKT      COMPARE CHECKSUM
        JMP   VEB       SAVX DATA IN MEMORY
LOAD12  JSR   INCVEB    INCR DATA POINTER
        JMP   LOADT7
;
LOADT8  JSR   RDBYT     END OF DATA, COMPARE CHKSUM   1915
        CMP   CHKL
        BNE   LOADT9
        JSR   RDBYT
        CMP   CHKH
        BNE   LOADT9
        LDA   #$00      NORMAL EXIT
        BEQ   LOAD10
LOADT9  LDA   #$FF      ERROR EXIT 
LOAD10  STA   POINTL               
        STA   POINTH
        JMP   START
;
;       ** SUBROUTINES BELOW **
INTVEB  LDA   SAL       MOVE SA TO VEB+1,2            1932
        STA   VEB+1
        LDA   SAH
        STA   VEB+2
        LDA   #$60      RTS INST
        STA   VEB+3
        LDA   #$00      CLEAR CHKSUM AREA
        STA   CHKL
        STA   CHKH
        RTS
;       ** COMPUTE CHKSUM FOR TAPE LOAD **
CHKT    TAY                                           194C
        CLC     
        ADC   CHKL
        STA   CHKL
        LDA   CHKH
        ADC   #$00
        STA   CHKH
        TYA       
        RTS 
;       ** OUTPUT ONE BYTE **
OUTBTC  JSR   CHKT      COMPARE CHKSUM                195E
OUTBT   TAY             SAVX DATA BYTE
        LSR   A         SHIFT OFF LSD
        LSR   A
        LSR   A
        LSR   A
        JSR   HEXOUT    OUTPUT MSD
        TYA 
        JSR   HEXOUT    OUTPUT LSD
        TYA  
        RTS 
;       ** CONVERT LSD OF A TO ASCII, OUTPUT TO TAPE **
HEXOUT  AND   #$0F                                    196F
        CMP   #$0A      
        CLC   
        BMI   HEX1
        ADC   #$07
HEX1    ADC   #$30
;       ** OUTPUT TO TAPE ONE ASCII CHAR **
OUTCHT  STX   SAVX                                    197A
        STY   SAVX+1
        LDY   #$08      START BIT
CHT1    JSR   ONE                  
        LSR   A         GET DATA BIT
        BCS   CHT2 
        JSR   ONE       DATA BIT=1
        JMP   CHT3
CHT2    JSR   ZRO       DATA BIT=0 
CHT3    JSR   ZRO                  
        DEY
        BNE   CHT1
        LDX   SAVX
        LDY   SAVX+1
        RTS
;       ** OUTPUT 1 TO TAPE, 9 PULSES, 138 US EACH **
ONE     LDX   #$09                                    199E
        PHA             SAVX A
ONE1    BIT   CLKRDI    WAIT FOR TIME OUT             19A1
        BPL   ONE1
        LDA   #126
        STA   CLK1T
        LDA   #$A7 
        STA   SBD       SET PB7 = 1
ONE2    BIT   CLKRDI                                  19B0
        BPL   ONE2 
        LDA   #126
        STA   CLK1T
        LDA   #$27 
        STA   SBD       RESET PB7=0
        DEX   
        BNE   ONE1
        PLA  
        RTS  
;       ** OUTPUT 0 TO TAPE, 6 PULSES, 207 US EACH **
ZRO     LDX   #$06                                    19C4
        PHA             SAVX A
ZRO1    BIT   CLKRDI                                  19C7
        BPL   ZRO1 
        LDA   #$C3 
        STA   CLK1T
        LDA   #$A7 
        STA   SBD       SET PB7=1
ZRO2    BIT   CLKRDI
        BPL   ZRO2 
        LDA   #195 
        STA   CLK1T
        LDA   #$27 
        STA   SBD       RESET PB7=0
        DEX        
        BNE   ZRO1
        PLA             RESTORE A
        RTS
;       ** SUB TO INC VEB+1,2 **
INCVEB  INC   VEB+1                                   19EA
        BNE   INCVE1
        INC   VEB+2
INCVE1  RTS     
;       ** SUB TO READ BYTE FROM TAPE **
RDBYT   JSR   RDCHT                                   19F3
        JSR   PACKT
        JSR   RDCHT
        JSR   PACKT
        RTS
;       ** PACK A=ASCII INTO SAVX AS HEX DATA **
PACKT   CMP   #$30                                    1A00
        BMI   PACKT3
        CMP   #$47
        BPL   PACKT3
        CMP   #$40  
        BMI   PACKT1
        CLC         
        ADC   #$09  
PACKT1  ROL   A
        ROL   A
        ROL   A
        ROL   A
        LDY   #$04
PACKT2  ROL   A
        ROL   SAVX
        DEY          
        BNE   PACKT2
        LDA   SAVX
        LDY   #$00      Y=0 VALID HEX CHAR
        RTS
PACKT3  INY             Y=1 NOT HEX
        RTS
;       ** GET 1 CHAR FROM TAPE AND RETURN WITH 
;          CHAR IN A.  USE SAVX+1 TO ASM CHAR  **
RDCHT   STX   SAVX+2                                  1A24
        LDX   #$08      READ 8 BITS
RDCHT1  JSR   RDBIT     GET NEXT DATA BIT
        LSR   SAVX+1    RIGHT SHIFT CHAR
        ORA   SAVX+1    OR IN SIGN BIT
        STA   SAVX+1    REPLACE CHAR
        DEX          
        BNE   RDCHT1
        LDA   SAVX+1    MOVE CHAR INTO A
        ROL   A         SHIFT OFF PARITY
        LSR   A
        LDX   SAVX+2
        RTS
;       ** THIS SUB GETS ONE BIT FROM TAPE AND
;          RETURNS IT IN SIGN OF A  **
RDBIT   BIT   SBD       WAIT FOR END OF START BIT     1A41
        BPL   RDBIT
        LDA   CLKRDT    GET START BIT TIME
        LDY   #$FF      A=256-T1    
        STY   CLK64T    SET UP TIMER
        LDY   #$14  
RDBIT3  DEY             DELAY 100 MICRO SEC
        BNE   RDBIT3
RDBIT2  BIT   SBD
        BMI   RDBIT2    WAIT FOR NEXT START BIT
        SEC   
        SBC   CLKRDT    (256-T1)-(256-T2)=T2-T1
        LDY   #$FF
        STY   CLK64T    SET UP TIMER FOR NEXT BIT
        LDY   #$07
RDBIT4  DEY             DELAY 50 MICROSEC
        BNE   RDBIT4
        EOR   #$FF      COMPLEMENT SIGN OF A
        AND   #$80      MASK ALL EXCEPT SIGN
        RTS 
;       ** PLLCAL OUTPUT 166 MICROSEC (6024 HZ)
;          PULSE STRING 
PLLCAL  LDA   #$27                                    1A6B
        STA   SBD       TURN OFF DATIN PB5=1
        LDA   #$BF      CONVERT PB7 TO OUTPUT
        STA   PBDD
PLL1    BIT   CLKRDI
        BPL   PLL1  
        LDA   #154      WAIT 166 MICROSEC
        STA   CLK1T
        LDA   #$A7      OUTPUT PB7=1
        STA   SBD
PLL2    BIT   CLKRDI
        BPL   PLL2 
        LDA   #154
        STA   CLK1T
        LDA   #$27      PB7=0
        STA   SBD
        JMP   PLL1
;
;       ** INTERRUPTS **
        *=    $1BFA
NMIP27  .WORD PLLCAL
RSTP27  .WORD PLLCAL
IRQP27  .WORD PLLCAL
;
;******************* 6530-002 I.C. *****************
;       ** COPYRIGHT MOS TECHNOLOGY INC.
;          DATE OCT 13, 1975 REV E
;
;       ** KIM **
;         TTY INTERFACE   6530-002
;         KEYBOARD INTERFACE,
;         7-SEGMENT 6-DIGIT DISPLAY
;
;       TTY COMANDS:
;         G    GOEXEC
;         CR   OPEN NEXT CELL
;         LF   OPEN PREVIOUS CELL
;         .    MODIFY OPEN CELL
;         SP   OPEN NEW CELL
;         L    LOAD (OBJECT FORMAT)
;         Q    DUMP FROM OPEN CELL ADDR TO HI LIMIT
;         RO   RUB OUT - RETURN TO START KIM
;               (ALL ILLEGAL CHARS ARE IGNORED)
;
;       KEYBOARD COMMANDS:
;         ADDR  SETS MODE TO MODIFY CELL ADDRESS
;         DATA  SETS MODE TO MODIFY DATA IN OPEN CELL
;         STEP  INCREMENTS TO NEXT CELL
;         RST   SYSTEM RESET
;         RUN   GOEXEC
;         STOP  $1C00 CAN BE LOADED INTO NMIV TO USE
;         PC    DISPLAY PC (PROGRAM COUNTER)
;         
        *=      $1C00
SAVE    STA   ACC       KIM ENTRY VIA STOP (NMI)      1C00
        PLA             OR BRK (IRQ)
        STA   PREG
        PLA             KIM ENTRY VIA JSR (A LOST)    1C05
        STA   PCL
        STA   POINTL
        PLA         
        STA   PCH
        STA   POINTH
        STY   YREG
        STX   XREG
        TSX   
        STX   SPUSER
        JSR   INITS
        JMP   START
;
NMIT    JMP   (NMIV)    NON-MASKABLE INTERRUPT TRAP   1C1C
IRQT    JMP   (IRQV)    INTERRUPT TRAP                1C1F
RST     LDX   #$FF      KIM ENTRY VIA RST             1C22
        TXS
        STX   SPUSER
        JSR   INITS
        LDA   #$FF      COUNT START BIT
        STA   CNTH30    ZERO CNTH30
        LDA   #$01      MASK HI ORDER BITS
DET1    BIT   SAD       TEST                          1C31
        BNE   START     KEYBD SSW TEST
        BMI   DET1      START BIT TEST
        LDA   #$FC
DET3    CLC             THIS LOOP COUNTS              1C3A
        ADC   #$01      THE START BIT TIME
        BCC   DET2
        INC   CNTH30
DET2    LDY   SAD       CHECK FOR END OF START BIT    1C42
        BPL   DET3
        STA   CNTL30
        LDX   #$08
        JSR   GET5      GET REST OF THE CHAR, TEST CHAR
;       ** MAKE TTY/KB SELECTION **
START   JSR   INIT1                                   1C4F
        LDA   #$01
        BIT   SAD
        BNE   TTYKB
        JSR   CRLF      PRT CR LF
        LDX   #$0A      TYPE OUT KIM
        JSR   PRTST
        JMP   SHOW1
;
CLEAR   LDA   #$00
        STA   INL       CLEAR INPUT BUFFER
        STA   INH
READ    JSR   GETCH     GET CHAR
        CMP   #$01      
        BEQ   TTYKB
        JSR   PACK
        JMP   SCAN
;       ** MAIN ROUTINE FOR KEYBOARD AND DISPLAY **
TTYKB   JSR   SCAND     IF A=0 NO KEY                 1C77
        BNE   START
TTYKB1  LDA   #$01
        BIT   SAD 
        BEQ   START
        JSR   SCAND
        BEQ   TTYKB1
        JSR   SCAND
        BEQ   TTYKB1
        JSR   GETKEY
        CMP   #$15  
        BPL   START
        CMP   #$14
        BEQ   PCCMD     DISPLAY PC
        CMP   #$10      ADDR MODE=1
        BEQ   ADDRM
        CMP   #$11      DATA MODE=1
        BEQ   DATAM
        CMP   #$12      STEP
        BEQ   STEP
        CMP   #$13      RUN
        BEQ   GOV
        ASL   A         SHIFT CHAR INTO HIGH
        ASL   A         ORDER NIBBLE
        ASL   A
        ASL   A
        STA   TEMP      STORE IN TEMP
        LDX   #$04
DATA1   LDY   MODE      TEST MODE 1=ADDR
        BNE   ADDR      MODE=0 DATA
        LDA   (POINTL),Y   GET DATA
        ASL   TEMP      SHIFT CHAR
        ROL   A         SHIFT DATA
        STA   (POINTL),Y  STORE OUT DATA
        JMP   DATA2
;
ADDR    ASL   A         SHIFT CHAR
        ROL   POINTL    SHIFT ADDR
        ROL   POINTH    SHIFT ADDR HI
DATA2   DEX   
        BNE   DATA1     DO 4 TIMES
        BEQ   DATAM2    EXIT HERE
ADDRM   LDA   #$01
        BNE   DATAM1
DATAM   LDA   #$00 
DATAM1  STA   MODE 
DATAM2  JMP   START
;
STEP    JSR   INCPT                                   1CD3
        JMP   START
;
GOV     JMP   GOEXEC                                  1CD9
;       ** DISPLAY PC BY MOVING PC TO POINT **
PCCMD   LDA   PCL                                     1CDC
        STA   POINTL
        LDA   PCH
        STA   POINTH
        JMP   START
;       ** LOAD PAPER TAPE FROM TTY **
LOAD    JSR   GETCH     LOOK FOR FIRST CHAR           1CE7
        CMP   #$3B      SEMICOLON
        BNE   LOAD
        LDA   #$00
        STA   CHKSUM
        STA   CHKHI
        JSR   GETBYT    GET BYTE COUNT
        TAX             SAVE IN X INDEX
        JSR   CHK       COMPUTE CHECKSUM
        JSR   GETBYT    GET ADDRESS HI
        STA   POINTH
        JSR   CHK
        JSR   GETBYT    GET ADDRESS LO
        STA   POINTL
        JSR   CHK
        TXA             IF CNT=0 DONT
        BEQ   LOAD3     GET ANY DATA
LOAD2   JSR   GETBYT    GET DATA
        STA   (POINTL),Y  STORE DATA
        JSR   CHK
        JSR   INCPT     NEXT ADDRESS
        DEX    
        BNE   LOAD2
        INX             X=1 DATA RCD X=0 LAST RCD
LOAD3   JSR   GETBYT    COMPARE CHKSUM
        CMP   CHKHI
        BNE   LOADE1
        JSR   GETBYT
        CMP   CHKSUM
        BNE   LOADER
        TXA             X=0 LAST RECORD
        BNE   LOAD
        LDX   #$0C      X-OFF KIM
LOAD8   LDA   #$27
        STA   SBD       DISABLE DATA IN
        JSR   PRTST
        JMP   START
;
LOADE1  JSR   GETBYT    DUMMY
LOADER  LDX   #$11      X-OFF ERR KIM
        BNE   LOAD8
;       ** DUMP TO TTY FROM OPEN CELL ADDRESS TO 
;          LIMHL, LIMHH **
DUMP    LDA   #$00                                    1D42
        STA   INL
        STA   INH       CLEAR RECORD COUNT
DUMP0   LDA   #$00
        STA   CHKHI     CLEAR CHKSUM
        STA   CHKSUM
        JSR   CRLF      PRINT CR LF
        LDA   #$3B      PRINT SEMICOLON
        JSR   OUTCH
        LDA   POINTL    TEST POINT GT OR ET
        CMP   EAL       HI LIMIT GOTO EXIT
        LDA   POINTH
        SBC   EAH
        BCC   DUMP4
        LDA   #$00      PRINT LAST RECORD
        JSR   PRTBYT    0 BYTES
        JSR   OPEN
        JSR   PRTPNT
        LDA   CHKHI     PRINT CHKSUM
        JSR   PRTBYT    FOR LAST RECORD
        LDA   CHKSUM
        JSR   PRTBYT
        JMP   CLEAR
;
DUMP4   LDA   #$18      PRINT 24 BYTE COUNT           1D7A
        TAX             SAVE AS INDEX
        JSR   PRTBYT
        JSR   CHK
        JSR   PRTPNT
DUMP2   LDY   #$00      PRINT 24 BYTES
        LDA   (POINTL),Y  GET DATA
        JSR   PRTBYT    PRINT DATA
        JSR   CHK       COMPUTE CHKSUM
        JSR   INCPT     INCREMENT POINT
        DEX
        BNE   DUMP2
        LDA   CHKHI     PRINT CHKSUM
        JSR   PRTBYT
        LDA   CHKSUM
        JSR   PRTBYT
        INC   INL       INCR RECORD COUNT
        BNE   DUMP3
        INC   INH
DUMP3   JMP   DUMP0
;
SPACE   JSR   OPEN      OPEN NEW CELL                 1DA9
SHOW    JSR   CRLF      PRINT CR LF
SHOW1   JSR   PRTPNT
        JSR   OUTSP     PRINT SPACE
        LDY   #$00      PRINT DATA SPECIFIED
        LDA   (POINTL),Y  BY POINT AD=LDA EXT
        JSR   PRTBYT
        JSR   OUTSP     PRINT SPACE
        JMP   CLEAR
;
RTRN    JSR   INCPT     OPEN NEXT CELL                1DC2
        JMP   SHOW
;
GOEXEC  LDX   SPUSER                                  1DC8
        TXS   
        LDA   POINTH    PROGRAM RUNS FROM
        PHA             OPEN CELL ADDRESS
        LDA   POINTL
        PHA      
        LDA   PREG
        PHA       
        LDX   XREG      RESTORE REGS
        LDY   YREG
        LDA   ACC
        RTI
;
SCAN    CMP   #$20      OPEN CELL
        BEQ   SPACE
        CMP   #$7F      RUB OUT (KIM)
        BEQ   STV
        CMP   #$0D      NEXT CELL
        BEQ   RTRN
        CMP   #$0A      PREV CELL
        BEQ   FEED
        CMP   #'.       MODIFY CELL
        BEQ   MODIFY
        CMP   #'G       GO EXEC
        BEQ   GOEXEC
        CMP   #'Q       DUMP FROM OPEN CELL TO HI LIMIT
        BEQ   DUMPV
        CMP   #'L       LOAD TAPE
        BEQ   LOADV
        JMP   READ      IGNORE ILLEGAL CHAR
;
STV     JMP   START
DUMPV   JMP   DUMP
LOADV   JMP   LOAD 
;
FEED    SEC                                           1E07
        LDA   POINTL    DEC DOUBLE BYTE
        SBC   #$01      AT POINTL AND POINTH
        STA   POINTL
        BCS   FEED1
        DEC   POINTH
FEED1   JMP   SHOW
;
MODIFY  LDY   #$00      GET CONTENTS OF INPUT BUFF 
        LDA   INL       INL AND STORE IN LOC
        STA   (POINTL),Y  SPECIFIED BY POINT
        JMP   RTRN 
;
;       ** SUBROUTINES FOLLOW **
PRTPNT  LDA   POINTH    PRINT POINTL, POINTH          1E1E
        JSR   PRTBYT
        JSR   CHK
        LDA   POINTL
        JSR   PRTBYT
        JSR   CHK
        RTS
;       **PRINT STRING OF ASCII CHARS FROM TOP+X TO TOP
CRLF    LDX   #$07    
PRTST   LDA   TOP,X 
        JSR   OUTCH
        DEX 
        BPL   PRTST     STOP ON INDEX ZERO
        RTS
;       ** PRINT 1 HEX BYTE AS 2 ASCII CHARS **
PRTBYT  STA   TEMP                                    1E3B
        LSR   A         SHIFT CHAR RIGHT 4 BITS
        LSR   A
        LSR   A
        LSR   A
        JSR   HEXTA     CONVERT TO HEX AND PRINT
        LDA   TEMP      GET OTHER HALF
        JSR   HEXTA     CONVERT TO HEX AND PRINT
        LDA   TEMP      RESTORE BYTE IN A AND RETURN
        RTS   
HEXTA   AND   #$0F      MASK HI 4 BITS
        CMP   #$0A
        CLC       
        BMI   HEXTA1
        ADC   #$07      ALPHA HEX
HEXTA1  ADC   #$30      DEC HEX
        JMP   OUTCH     PRINT CHAR
;       ** GET 1 CHAR FROM TTY, CHAR IN A
GETCH   STX   TMPX      SAVE X REG     1E5A
        LDX   #$08      SET UP 8-BIT COUNT
        LDA   #$01
GET1    BIT   SAD
        BNE   GET6 
        BMI   GET1      WAIT FOR START BIT
        JSR   DELAY     DELAY 1 BIT
GET5    JSR   DEHALF    DELAY 1/2 BIT TIME
GET2    LDA   SAD       GET 8 BITS
        AND   #$80      MASK OFF LOW ORDER BITS
        LSR   CHAR      SHIFT RIGHT CHAR
        ORA   CHAR
        STA   CHAR
        JSR   DELAY     DELAY 1 BIT TIME
        DEX  
        BNE   GET2      GET NEXT CHAR
        JSR   DEHALF    EXIT THIS ROUTINE
        LDX   TMPX
        LDA   CHAR
        ROL   A SHIFT OFF PARITY
        LSR   A
GET6    RTS   
;       ** INITIALIZATION FOR SIGMA **
INITS   LDX   #$01      SET KB MODE TO ADDR           1E88
        STX   MODE
INIT1   LDX   #$00      
        STX   PADD      FOR SIGMA USE SADD
        LDX   #$3F
        STX   PBDD      FOR SIGMA USE SBDD
        LDX   #$07      ENABLE DATA IN
        STX   SBD       OUTPUT
        CLD    
        SEI    
        RTS
;       ** PRINT ONE CHAR IN A **
OUTSP   LDA   #$20      PRINT SPACE                   1E9E
OUTCH   STA   CHAR
        STX   TMPX
        JSR   DELAY     DELAY 10/11 BIT CODE SYNC
        LDA   SBD       START BIT
        AND   #$FE
        STA   SBD
        JSR   DELAY
        LDX   #$08 
OUT1    LDA   SBD       DATA BIT
        AND   #$FE 
        LSR   CHAR
        ADC   #$00
        STA   SBD  
        JSR   DELAY
        DEX  
        BNE   OUT1
        LDA   SBD       STOP BIT
        ORA   #$01
        STA   SBD
        JSR   DELAY     STOP BIT
        LDX   TMPX      RESTORE INDEX
        RTS
;       ** DELAY 1 BIT TIME **
DELAY   LDA   CNTH30                                  1ED4
        STA   TIMH
        LDA   CNTL30
DE2     SEC   
DE4     SBC   #$01 
        BCS   DE3  
        DEC   TIMH
DE3     LDY   TIMH
        BPL   DE2
        RTS
;       ** DELAY 1/2 BIT TIME **
DEHALF  LDA   CNTH30                                  1EEB  
        STA   TIMH
        LDA   CNTL30
        LSR   A
        LSR   TIMH
        BCC   DE2
        ORA   #$80
        BCS   DE4
;       ** SUB TO DETERMINE IF KEY IS DEPRESSED OR 
;          CONDITION OF SSW KEY NOT DEPRESSED OR
;          TTY MODE  A=0
;          KEY DEPRESSED OR KB MODE  A NOT ZERO
AK      LDY   #$03      3 ROWS
        LDX   #$01      DIGIT 0
ONEKEY  LDA   #$FF
AK1     STX   SBD       OUTPUT DIGIT
        INX             GET NEXT DIGIT
        INX 
        AND   SAD       INPUT SEGMENTS
        DEY 
        BNE   AK1
        LDY   #$07
        STY   SBD
        ORA   #$80
        EOR   #$FF
        RTS 
;       ** OUTPUT TO 7-SEGMENT DISPLAY **
SCAND   LDY   #$00      GET DATA                      1F19 
        LDA   (POINTL),Y  SPECIFIED BY POINT
        STA   INH       SET UP DISPLAY BUFFER
        LDA   #$7F      CHANGE SEG
        STA   PADD      TO OUTPUT
        LDX   #$09      INIT DIGIT NUMBER
        LDY   #$03      OUTPUT 3 BYTES
SCAND1  LDA   INL,Y     GET BYTE
        LSR   A         GET MSD
        LSR   A
        LSR   A
        LSR   A
        JSR   CONVD     OUTPUT CHAR
        LDA   INL,Y     GET BYTE AGAIN
        AND   #$0F      GET LSD
        JSR   CONVD     OUTPUT CHAR
        DEY             SET UP FOR NEXT BYTE
        BNE   SCAND1
        STX   SBD       ALL DIGITS OFF
        LDA   #$00      CHANGE SEGMENT
        STA   PADD      TO INPUTS
        JMP   AK        GET ANY KEY
;       ** CONVERT AND DISPLAY HEX (USED BY SCAND ONLY)**
CONVD   STY   TEMP
        TAY             SAVE Y
        LDA   TABLE,Y   USE CHAR AS INDEX
        LDY   #$00      LOOKUP CONVERSION
        STY   SAD       TURN OFF SEGMENTS
        STX   SBD       OUTPUT DIGIT ENABLE
        STA   SAD       OUTPUT SEGMENTS
        LDY   #$7F      DELAY 500 CYCLES
CONVD1  DEY   
        BNE   CONVD1
        INX             GET NEXT DIGIT NUMBER
        INX             ADD 2
        LDY   TEMP      RESTORE Y
        RTS
;       ** INCREMENT POINT **
INCPT   INC   POINTL
        BNE   INCPT2
        INC   POINTH
INCPT2  RTS   
;       ** GET KEY FROM KEYPAD A=KEYVALUE **
GETKEY  LDX   #$21      START AT DIGIT 0              1F6A 
GETKE5  LDY   #$01      GET 1 ROW
        JSR   ONEKEY
        BNE   KEYIN     A=0 NO KEY
        CPX   #$27      TEST FOR DIGIT 2
        BNE   GETKE5
        LDA   #$15      15=NOKEY
        RTS
KEYIN   LDY   #$FF      
KEYIN1  ASL   A         SHIFT LEFT
        BCS   KEYIN2    UNTIL Y=KEY NO
        INY   
        BPL   KEYIN1
KEYIN2  TXA  
        AND   #$0F      MASK MSD
        LSR   A         DIVIDE BY 2
        TAX   
        TYA   
        BPL   KEYIN4
KEYIN3  CLC   
        ADC   #$07      MULT (X-1 TIMES A
KEYIN4  DEX   
        BNE   KEYIN3
        RTS
;       ** COMPUTE CHECKSUM **
CHK     CLC   
        ADC   CHKSUM
        STA   CHKSUM
        LDA   CHKHI
        ADC   #$00
        STA   CHKHI
        RTS
;       ** GET 2 HEX CHARS AND PACK INTO INL AND INH **
GETBYT  JSR   GETCH
        JSR   PACK
        JSR   GETCH
        JSR   PACK
        LDA   INL
        RTS
;       ** SHIFT CHAR IN A INTO INL AND INH **
PACK    CMP   #$30      CHECK FOR HEX  1FAC
        BMI   UPDAT2
        CMP   #$47      NOT HEX EXIT
        BPL   UPDAT2
        CMP   #$40      CONVERT TO HEX
        BMI   UPDATE
        CLC   
        ADC   #$09
UPDATE  ROL   A
        ROL   A
        ROL   A
        ROL   A
        LDY   #$04      SHIFT INTO I/O BUFFER
UPDAT1  ROL   A 
        ROL   INL
        ROL   INH
        DEY   
        BNE   UPDAT1
        LDA   #$00      A=0 IF HEX NUM
UPDAT2  RTS   
;
OPEN    LDA   INL       MOVE I/O BUFFER TO POINT
        STA   POINTL
        LDA   INH       TRANSFER INH- POINTH
        STA   POINTH
        RTS
;
;       ** TABLES **
TOP     .DBYTE $0000,$0000,$0000,$0A0D                1FD5
        .BYTE  'M','I','K',$20,$13         KIM
        .BYTE  'R','R','E',$20,$13         ERR
TABLE   .DBYTE $BF86,$DBCF,$E6ED,$FD87 ;0-7           1FE7
        .DBYTE $FFEF,$F7FC,$B9DE,$F9F1 ;8-F HEX TO 7-SEG
;
;       ** INTERRUPT VECTORS **
        *=    $1FFA
NMIENT  .WORD NMIT
RSTENT  .WORD RST
IRQENT  .WORD IRQT
        .END

