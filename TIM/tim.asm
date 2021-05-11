       .PAGE 'TIM VERSION 1.0 - MEM PAGE 0'
;
;
;
;
;
;
; - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
;
;  PROMPTING CHARACTER IS A PERIOD (.)
;  -----------------------------------
;
;
;  DISPLAY COMMANDS
;  ----------------
;
;  .R          DISPLAY REGISTERS (PC,F,A,X,Y,SP)
;  .M  ADDR    DISPLAY MEMORY ( 8 BYTES BEGINNING AT ADDR )
;
;
;  ALTER COMMAND (:)
;  -----------------
;  .:  DATA      ALTERS PREVIOUSLY DISPLAYED ITEM OR NEXT ITEM
;
;
;  PAPER TAPE I/O COMMANDS
;  ------------------------
;
;  .LH                   LOAD HEX TAPE
;  .WB ADDR1 ADDR2       WRITE BNPF TAPE (FROM LOW ADDR1 TO HIGH ADDR2)
;  .WH ADDR1 ADDR2       WRITE HEX TAPE (FROM LOW ADDR1 TO HIGH ADDR2)
;
;  CONTROL COMMANDS
;  ----------------
;
;  .G                    GO, CONTINUE EXECUTION FROM CIRRENT PC ADDRESS
;
;  .H                    TOGGLES HIGH-SPEED-READER OPTION
;                          (IF ITS ON, TURNS IT OFF; IF OFF, TURNS ON
;
;  BRK AND NMI ENTRY POINTS TO TIM
;  -------------------------------
;
;      TIM IS NORMALLY ENTERED WHEN A 'BRK' INSTRUKTION IS
;          ENCOUNTERED DURING PROGRAM EXECUTION.  AT THAT
;          TIME CPU REGISTERS ARE OUTPUT:    PC F A X Y SP
;          AND CONTROL IS GIVEN TO THE KEYBOARD.
;      USER MAY ENTER TIM BY PROGRAMMED BRK OR INDUCED NMI.  NMI
;          ENTRIES CAUSE A '#' TO PRECEDE THE '.' IN THE CPU REGISTER
;          PRINTOUT FORMAT
;
;  NON-BRK INTRO (EXTERNAL DEVICE) INTERRUPT HANDLING
;  --------------------------------------------------
;
;      A NON-BRK INTRO INTERRUPT CAUSES AN INDIRECT JUMP TO THE ADDRESS
;          LOCATED AT 'UINT' (HEX FFF8).  THIS LOCATION CAN BE SET
;          USING THE ALTER CMD, OR LOADED AUTOMATICALLY IN PAPER TAPE
;          FROM WITH THE LH CMD IF THE USER ASSIGNS HIS INTRO INTERRUPT
;          VECTOR TO $FFF8 IN THE SOURCE ASSEMBLY PROGRAM.
;      IF NOT RESET BY THE USER, UINT IS SET TO CAUSE EXTERNAL
;          DEVICE INTERRUPTS TO ENTER TIM AS NMI'S.  I.E.,
;          IF A NMI OCOURS WITHOUT AN INDUCED NMI SIGNAL, IT IS
;          AN EXTERNAL DEVICE INTERRUPT.
;
;  SETTING AND RESETTING PROGRAM BREAKPOINTS
;  -----------------------------------------
;
;      BREAKPOINTS ARE SET AND RESET USING THE MEMORY DISPLAY
;          AND ALTER COMMANDS.  BRK HAS A '00' OPERATION CODE.
;      TO SET A BREAKPOINT SIMPLY DISPLAY THE MEMORY LOCATION
;          (FIRST INSTRUCTION BYTE) AT WHICH THE BREAKPOINT IS
;          TO BE PLACED THEN ALTER THE LOCATION TO '00'.  THERE IS
;          NO LIMIT TO THE NUMBER OF BREAKPOINTS THAT CAN BE
;          ACTIVE AT ONE TIME.
;      TO RESET A BREAKPOINT, RESTORE THE ALTERED MEMORY LOCATION
;          TO ITS ORIGINAL VALUE.
;      WHEN AND IF A BREAKPOINT IS ENCOUNTERED DURING EXECUTION,
;          THE BREAKPOINT DATA PRECEDEC BY AN ':' IS DISPLAYED.
;          THE PROGRAM COUNTER VALUE DISPLAYED IS THE BRK
;          INSTRUCTION LOCATION + 1.
;
;  -------------------------------------------------------------------
;
MDBK   =%00010110        ; X,X,X,POR,DATA-AVAIL,GOT-DATA,SERIAL-OUT,IN
DAVAIL =$08
GOTDAT =$04
IOBASE =$6E00
MPA    =IOBASE+0
MDA    =IOBASE+1
MPB    =IOBASE+2
MDB    =IOBASE+3
MCLKIT =IOBASE+4
MCLKRD =IOBASE+4
MCLKIF =IOBASE+5
UINT   =$FFF8
NCMDS  =7
MP0    =$7000
MP1    =$7100
MP2    =$7200
MP3    =$7300
;
;  ZERO PAGE MONITOR RESERVE AREA
;
CRDLY  =227              ;DELAY FOR CR IN BIT-TIMES
WRAP   =228              ;ADDRESS WRAP-AROUND FLAG
DIFF   =229
HSPTR  =231
HSROP  =232
PREVC  =233
MAJORT =234
MINORT =235
ACMD   =236
TMP0   =238
TMP2   =240
TMP4   =242
TMP6   =244
PCL    =246
PCH    =247
FLGS   =248
ACC    =249
XR     =250
YR     =251
SP     =252
SAVX   =253
TMPC   =254
TMPC2  =255
RCNT   =TMPC
LCNT   =TMPC2
;
;  64 BYTE RAM MONITOR RESERVE AREA
;
RAM64  =$FFC0
       *=RAM64
       .PAGE 'MP0 TIM PAGE 0'
;
;
;  TIM PAGE 0 (RELATIVE)
       *=MP0
;
NMINT  STA ACC
       LDA #'#           ; SET A=# TO INDICATE NMINT ENTRY
       BNE B3            ; JMP B3
;
RESET  LDA #MDBK         ; INIT DIR REG, PCR IC 1 RELOCATES
;
       STA MDB
;
       LDX #8            ; X=0
R1     LDA INTVEC-1,X    ; INITALIZE INT VECTORS
       STA UINT-1,X
       DEX
       BNE R1
;
       STX MAJORT        ; INIT MAJOR T COUNT To ZERO
       STX HSPTR         ; CLEAR HSPTR FLAGS
       STX HSROP
       DEX               ; X=FF
       TXS               ; SP=FF
;
;                        ; COMPUTE BIT-TIME CONSTANT, X=FF
;
       LDY #1            ; SET TO MEASURE 2 BITS
       STY CRDLY         ;INIT OR DELAY TIME PARAMETER
R0     LDA MPB           ; WAIT FOR START
       LSR A
       BCC R0
;
R2     STX MCLKIT        ; START CLOCK INITIALLY WITH FF
R3     LDA MCLKIF
       BPL R4
       INC MAJORT        ; COUNT MAJOR T
       BNE R2            ; GO RESTART CLOCK WITH X = FF
;
R4     TYA
       EOR MPB
       AND #1
       BEQ R3            ; WAIT FOR Y BIT 0 AND SERIAL-IN NOT EQU
       DEY
       BPL R3            ; LOOP UNTIL START OF BIT 2
;
       LDA MCLKRD
       EOR #$FF          ; COMPLEMENT RESIDOE
R5     LSR A             ; HALF IT
       LSR MAJORT        ; HALF MAJOR
       BCC R6
       ORA #$80          ; PROPAGETE HC TO LC
R6     INY
       BEQ R5
       STA MINORT
;
       CLI               ; ENABLE INTS
       BRK               ; ENTER TIM BY BRK
;
INTRQ  STA ACC           ; SAVE ACC
       PLA               ; FLAGS TO A
       PHA               ; RESTORE STACK STATUS
       AND #$10          ; TEST BRK FLAG
       BEQ BX            ; USER INTERRUPT
;
       ASL A             ; SET A=SPACE (10 X 2 = 2C)
B3     STA TMPC          ; SAVE INT TYPE FLAG
       CLD               ; CLEAR DECIMAL MODE
       LSR A             ; # IS ODD, SPACE IS EVEN
                         ; SET CY FOR PC BRK CORRECTION
;
       STX XR            ; SAVE X
       STY YR            ; Y
       PLA
       STA FLGS          ; FLAGS
       PLA
       ADC #$FF          ; CY SET TO PC-1 FOR BRK
       STA PCL
       PLA
       ADC #$FF
       STA PCH
       TSX
       STX SP            ; SAVE ORIG SP
;
B5     JSR CRLF
       LDX TMPC
;
       LDA #'*
       JSR WRTWO
       LDA #'R           ; SET FOR R DISPLAY TO PERMIT
       BNE S0            ;   IMMEDIATE ALTER FOLLOWING BREAKPOINT.
;
BX     LDA ACC
       JMP (UINT)        ; CONTROL TO USER INTRQ SERVICE ROUTINE
;
START  LDA #0            ;NEXT COMMAND FROM USER
       STA HSPTR         ;CLEAR H. S. PAPER TAPE FLAG
       STA WRAP          ;CLEAR ADDRESS WRAP-AROUND FLAG
       JSR CRLF
       LDA #'.           ; TYPE PROMPTING '.'
       JSR WROC
       JSR RDOC          ; READ CMD, CHAR RETURNED In A
;
S0     LDX #NCMDS-1      ; LOCK-UP CMD
S1     CMP CMDS,X
       BNE S2
;
       LDA SAVX          ; SAVE PRIVIOUS CMD
       STA PREVC
       STX SAVX          ; SAVE CURRENT CMD INDEX
       LDA #MP1/256      ; JMP INDIRECT TO CMD CODE
       STA ACMD+1        ;   ALL CMD CODE BEGINS ON MP1
       LDA ADRS,X
       STA ACMD
       CPX #3            ; IF :, R OR M (0, 1, OR 2) SPACE 2
       BCS IJMP
       JSR SPAC2
;
IJMP   JMP (ACMD)
;
S2     DEX
       BPL S1            ; LOOP FOR ALL CMDS
;
ERROPR LDA #'?           ; OPERATOR ERR, TYPE '?', RESTART
       JSR WROC
       BCC START         ; JMP START (WROC RETURNS CY=0)
;
DCMP   SEC               ; TMP2-TMP0 DOUBLE SUBTRACT
       LDA TMP2
       SBC TMP0
       STA DIFF
       LDA TMP2+1
       SBC TMP0+1
       TAY               ; RETURN HIGH ORDER PART IN Y
       ORA DIFF          ; OR LO FOR EQU TEST
       RTS
;
PUTP   LDA TMP0          ; MOVE TMP0 TO PCH,PCL
       STA PCL
       LDA TMP0+1
       STA PCH
       RTS
;
ZTMP   LDA #0            ; CLEAR REGS
       STA TMP0,X
       STA TMP0+1,X
       RTS
;
;  READ AND STORE BYTE.  NO STORE IF SPACE OR RCNT=0.
;
BYTE   JSR RDOB          ; CHAR IN A, CY=0 IF SP
       BCC BY3           ; SPACE
;
       LDX #0            ; STORE BYTE
       STA (TMP0,X)
;
       CMP (TMP0,X)      ; TEST FOR VALID WRITE (RAM)
       BEQ BY2
       PLA               ; ERR, CLEAR JSR ADR IN STACK
       PLA
       JMP ERROPR
;
BY2    JSR CADD          ; INCR CKSUM
BY3    JSR INCTMP        ; GO INCR TMPC ADR
       DEC RCNT
       RTS
;
SETR   LDA #FLGS         ; SET TO ACCESS REGS
       STA TMP0
       LDA #0
       STA TMP0+1
       LDA #5
       RTS
;
CMDS   .BYTE ':'
       .BYTE 'R'
       .BYTE 'M'
       .BYTE 'G'
       .BYTE 'H'
       .BYTE 'L'
       .BYTE 'W'         ; W MUST BE LAST CMD IN CHAIN
ADRS   .BYTE ALTER-MP1
       .BYTE DSPLYR-MP1
       .BYTE DSPLYM-MP1
       .BYTE GO-MP1
       .BYTE HSP-MP1
       .BYTE LH-MP1
       .BYTE WO-MP1
       .PAGE 'MP1 TIM PAGE 1'
;
;
;  NOTE -- ALL CMD CODE MUST BEGIN ON MP1
;
;  DISPLAY REG CMD - A,P,X,Y, AND SP
;
DSPLYR JSR WRPC          ; WRITE PC
       JSR SETR
       BNE M0            ; USE DSPLYM
;
DSPLYM JSR RDOA          ; READ MEM ADR INTO TMPC
       BCC ERRS1         ; ERR IF NO ADDR
       LDA #8
M0     STA TMPC
       LDY #0
M1     JSR SPACE         ; TYPE 8 BYTES OF MEM
       LDA (TMP0),Y      ; (TMP0) PRESERVED FOR POSS ALTER
       JSR WROB
       INY               ; INCR INDEX
       DEC TMPC
       BNE M1
BEQS1  JMP START
;
ERRS1  JMP ERROPR
;
;  ALTER LAST DISPLAYED ITEM (ADR IN TMPC)
;
ALTER  DEC PREVC         ; R INDEX = 1
       BNE A3
;
       JSR RDOA          ; CY=0 IF SP
       BCC A2            ; SPACE
       JSR PUTP          ; ALTER PC
A2     JSR SETR          ; ALTER R*S
       BNE A4            ; JMP A4 (SETR RETURNS ACC = 5)
A3     JSR WROA          ; ALTER M, TYPE ADR
       LDA #8            ; SET CNT=8
;
A4     STA RCNT
A5     JSR SPACE         ; PRESERVES Y
       JSR BYTE
       BNE A5
A9     BEQ BEQS1
;
GO     LDX SP
       TXS               ; ORIG OR NEW SP VALUE TO SP
       LDA PCH
       PHA
       LDA PCL
       PHA
       LDA FLGS
       PHA
       LDA ACC
       LDX XR
       LDY YR
       RTI
;
HSP    INC HSROP         ; TOGGLE BIT C
       JMP START
;
LH     JSR RDOC          ; READ SECOND CMD CHAR
       JSR CRLF
       LDX HSROP         ; ENABLE PTR OPTION IF SET
       STX HSPTR
LH1    JSR RDOC
       CMP #':           ; FIND NEXT BCD MARK (:)
       BNE LH1
;
       LDX #4
       JSR ZTMP          ; CLEAR CKSUM REGS TMP4
       JSR RDOB
       BNE LH2
;
       LDX #0            ; CLEAR HS ROR FLAG
       STX HSPTR
       BEQ BEQS1         ; FINISHED
;
LH2    STA RCNT          ; RCNT
       JSR CADD          ; BCD LNGH TO CKSUM
       JSR RDOB          ; SA HO TO TMP0+1
       STA TMP0+1
       JSR CADD          ; ADD TO CKSUM
       JSR RDOB          ; SA LO TO TMP0
       STA TMP0
       JSR CADD          ; ADD TO CKSUM
;
LH3    JSR BYTE          ; BYTE SUB/R DECRS RCNT On EXIT
       BNE LH3
       JSR RDOA          ; CKSUM FROM HEX BCD TO TMP0
       LDA TMP4          ; TMP4 TO TMP2 FOR DCMP
       STA TMP2
       LDA TMP4+1
       STA TMP2+1
       JSR DCMP
       BEQ LH1
ERRP1  JMP ERROPR
;
WO     JSR RDOC          ; RD 2ND CMD CHAR
       STA TMPC
       JSR SPACE
       JSR RDOA
       JSR T2T2         ; SA TO TMP2
       JSR SPACE         ; SPACE BEFORE NEXT ADDRESS
       JSR RDOA
       JSR T2T2          ; SA TO TMP0, EA TO TMP2
       JSR RDOC          ; DELAY FOR FINAL CR
       LDA TMPC
;
       CMP #'H
       BNE WB
;
WH0    LDX WRAP          ;IF ADDR HAS WRAPPED AROUND
       BNE BCCST         ;THEN TERMINATE WRITE OPERATION
;
       JSR CRLF
       LDX #24
       STX RCNT          ; RCNT=24
       LDX #4            ; CLEAR CKSUM
       JSR ZTMP
;
       LDA #';
       JSR WROC          ; WR BCD MARK
;
       JSR DCMP          ; EA-SA (TMP0+2-TMP0) DIFF IN LOC DIFF,+1
       TYA               ; MS BYTE OF DIFF
       BNE WH1
       LDA DIFF
       CMP #23
       BCS WH1           ; DIFF GT 24
       STA RCNT          ; INCR LAST RCNT
       INC RCNT
WH1    LDA RCNT
       JSR CADD          ; ADD TO CKSUM
       JSR WROB          ; BCD CNT IN A
       LDA TMP0+1        ; SA HO
       JSR CADD
       JSR WROB
       LDA TMP0          ; SA LO
       JSR CADD
       JSR WROB
;
WH2    LDY #0
       LDA (TMP0),Y
;
       JSR CADD          ; INC CKSUM, PRESERVES A
       JSR WROB
       JSR INCTMP        ; INC SA
       DEC RCNT
       BNE WH2           ; LOOP FOR OP TO 24 BYTE
;
       JSR WROA4         ; WRITE CKSUM
;
       JSR DCMP
       BCS WH0           ; LOOP WHILE EA GT OR = SA
BCCST  JMP START
;
;
WB     INC SAVX          ; SAVX TO = NCMDS FOR ASCII SUB/R
WB1    LDA WRAP          ;IF ADDR HAS WRAPPED AROUND
       BNE BCCST         ;THEN TERMINATE WRITE OPERATION
;
       LDA #4
       STA ACMD
       JSR CRLF
       JSR WROA          ; OUTPUT HEX ADR
;
WBNPF  JSR SPACE
       LDX #9
       STX TMPC          ; LOOP CNT =9
       LDA (TMP0-9,X)
       STA TMPC2         ; BYTE TO TMPC2
       LDA #'B
       BNE WBF2          ; WRITE @
;
WBF1   LDA #'P
       ASL TMPC2
       BCS WBF2
       LDA #'N
;
WBF2   JSR WROC          ; WRIRE N OR R
       DEC TMPC
       BNE WBF1          ; LOOP
       LDA #'F
       JSR WROC          ; WRITE F
;
       JSR INCTMP
;
       DEC ACMD          ; TEST FOR MULTIPLE OF FOUR
       BNE WBNPF
;
       JSR DCMP
       BCS WB1           ; LOOP WHILE EA GT OR = SA
       BCC BCCST
;
CADD   PHA               ; SAVE A
       CLC
       ADC TMP4
       STA TMP4
       LDA TMP4+1
       ADC #0
       STA TMP4+1
       PLA               ; RESTORE A
       RTS
;
CRLF   LDX #$0D
       LDA #$0A
       JSR WRTWO
       LDX CRDLY         ;BIT-TIME COUNT FOR DELAY
CR1    JSR DLY2          ;DELAY OF ONE BIT-TIME
       DEX
       BNE CR1
       RTS
;
;  WRITE ADR FROM TMP0 STORES
;
WROA   LDX #1
       BNE WROA1
WROA4  LDX #5
       BNE WROA1
WROA6  LDX #7
       BNE WROA1
WRPC   LDX #9
WROA1  LDA TMP0-1,X
       PHA
       LDA TMP0,X
       JSR WROB
       PLA
;
;  WRITE BYTE - A = BYTE
;  UNPACK BYTE DATA INTO TWO ASCII CHARS: A=BYTE; X,A=CHARS
;
WROB   PHA
       LSR A
       LSR A
       LSR A
       LSR A
       JSR ASCII         ; CONVERT TO ASCII
       TAX
       PLA
       AND #$0F
       JSR ASCII
;
;  WRITE 2 CHARS - X,A = CHARS
;
WRTWO  PHA
       TXA
       JSR WRT
       PLA
;
;  WRITE SERIAL OUTPUT
;  A = CHAR TO BE OUTPUT
;
WRT    JSR DLY2
       LDX #9
WROC   =WRT
       EOR #$FF          ; COMPLEMENT A
       SEC
;
WRT1   JSR OUT
       JSR DLY2
       LSR A
       DEX
       BNE WRT1
       BEQ RDT5
                         ; *USE BNE?
;
OUT    PHA               ; SAVE A
       LDA MPB           ; OUTPUT BIT FROM CY
       AND #%11111101
       BCC OUT1
       ORA #%00000010
OUT1   STA MPB
       PLA               ; RESTORE A
       RTS
;
;  OUTPUT  RETURNS CHAR IN A
;
RDT    LDA HSPTR         ; TEST HS PTR OPTION
       LSR A
       BCS RDHSR
RDOC   =RDT
       LDX #8
;
RDT1   LDA MPB
       LSR A             ; WAIT FOR START BIT
       BCC RDT1
;
       JSR DLY1
       JSR OUT           ; ECHO START BIT
;
RDT2   JSR DLY2
       LDA MPB           ; CY = NEXT BIT
       LSR A
       JSR OUT           ; ECHO
;
       PHP               ; SAVE BIT
       TYA               ; Y CONTAINS CHAR BEING FORMED
       LSR A
       PLP               ; RECALL BIT
       BCC RDT4
       ORA #$80          ; ADD IN NEXT BIT
RDT4   TAY
       DEX
       BNE RDT2          ; LOOP FOR 8 BITS
       EOR #$FF          ; COMPLEMENT DATA
       AND #$7F          ; CLEAR PARITY
;
       JSR DLY2
RDT5   CLC
       JSR OUT           ;AND DELAY 2 HALF-BIT-TIMES
;
DLY2   JSR DLY1
DLY1   PHA               ; SAVE FLAGS AND A
       PHP
       TXA               ; SAVE X
       PHA
       LDX MAJORT
       LDA MINORT
;
DL2    STA MCLKIT
;
DL3    LDA MCLKIF
       BPL DL3
       DEX
       PHP
       LDA MCLKRD        ; RESET TIMER INT FLAG
       PLP
       BPL DL3
;
       PLA               ; RESTORE REGS
       TAX
       PLP
       PLA
DLX    RTS
;
RDHSR  LDA MPB           ; LOOP ON DATA AVAIL
       AND #DAVAIL
       BEQ RDHSR
;
       LDX MPA           ; READ DATA
       LDA MPB           ; SEND GOT-DATA PULSE
       ORA #GOTDAT
       STA MPB
       AND #%11111011
       STA MPB
       TXA
       AND #$7F
       RTS
;
ASCII  CLC
       ADC #6
       ADC #$F0
       BCC ASC1
       ADC #$06
;
ASC1   ADC #$3A
       PHA               ; TEST FOR LETTER B IN ADR DURING WBNPF
       CMP #'B
       BNE ASCX
       LDA SAVX
       CMP #NCMDS
       BNE ASCX          ; NOT WB CMD
       PLA
       LDA #'            ; FOR WB, BLANK @'S IN ADR
       PHA
ASCX   PLA
       RTS
;
SPAC2  JSR SPACE
SPACE  PHA               ; SAVE A,X,Y
       TXA
       PHA
       TYA
       PHA
       LDA #' 
       JSR WRT           ; TYPE SP
       PLA               ; RESTORE A,X,Y
       TAY
       PLA
       TAX
       PLA
       RTS
;
T2T2   LDX #2
T2T21  LDA TMP0-1,X
       PHA
       LDA TMP2-1,X
       STA TMP0-1,X
       PLA
       STA TMP2-1,X
       DEX
       BNE T2T21
       RTS
;
;INCREMENT (TMP0,TMP0+1) BY 1
INCTMP INC TMP0          ;LOW BYTE
       BEQ INCT1
       RTS
;
INCT1  INC TMP0+1        ;HIGH BYTE
       BEQ SETWRP
       RTS
;
SETWRP INC WRAP          ;POINTER HAS WRAPPED AROUND - SET FLAG
       RTS
;
; READ HEX ADR; RETURN HO IN TMP0; LO IN TMP0+1 AND CY=1
;    IF SP CY=0
;
RDOA   JSR RDOB          ; READ 2 CHAR BYTE
       BCC RDOA2         ; SPACE
;
       STA TMP0+1
RDOA2  JSR RDOB
       BCC RDEXIT        ; SP
       STA TMP0
RDEXIT RTS
;
;  READ HEX BYTE AND RETURN IN A, AND CY=1
;    IF SP CY=0
;    Y REG IS PRESERVED
;
RDOB   TYA               ; SAVE Y
       PHA
       LDA #0            ; SET DATA = 0
       STA ACMD
       JSR RDOC
       CMP #$0D          ; CR?
       BNE RDOB1
       PLA               ;YES - GO TO START
       PLA               ;CLEANING STACK UP FIRST
       PLA
       JMP START
;
RDOB1  CMP #'            ; SPACE
       BNE RDOB2
       JSR RDOC          ; READ NEXT CHAR
       CMP #' 
       BNE RDOB3
       CLC               ; CY=0
       BCC RDOB4
;
RDOB2  JSR HEXIT         ; TO HEX
       ASL A
       ASL A
       ASL A
       ASL A
       STA ACMD
       JSR RDOC          ; 2ND CHAR ASSUMED HEX
RDOB3  JSR HEXIT
       ORA ACMD
       SEC               ; CY=1
RDOB4  TAX
       PLA               ; RESTORE Y
       TAY
       TXA               ;SET Z & N FLAGS FOR RETURN
       RTS
;
HEXIT  CMP #$3A
       PHP               ; SAVE FLAGS
       AND #$0F
       PLP
       BCC HEX09         ; 0-9
       ADC #8            ; ALPHA ACC 8+CY=9
HEX09  RTS
;
       *=MP3+$F8
;
INTVEC .WORD NMINT       ; DEFAULT USER INTRQ TO NMINT
       .WORD NMINT
       .WORD RESET
       .WORD INTRQ
;
       .END