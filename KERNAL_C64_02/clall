.PAG 'CLOSE ALL FILES'
;***************************************
;* CLALL -- CLOSE ALL LOGICAL FILES  *
;*      DELETES ALL TABLE ENTRIES AND*
;* RESTORES DEFAULT I/O CHANNELS     *
;* AND CLEARS IEEE PORT DEVICES      *
;*************************************
;
NCLALL	LDA #0
	STA LDTND       ;FORGET ALL FILES
.SKI 3
;********************************************
;* CLRCH -- CLEAR CHANNELS                  *
;*   UNLISTEN OR UNTALK IEEE DEVICES, BUT   *
;* LEAVE OTHERS ALONE.  DEFAULT CHANNELS    *
;* ARE RESTORED.                            *
;********************************************
;
NCLRCH	LDX #3
	CPX DFLTO       ;IS OUTPUT CHANNEL IEEE?
	BCS JX750       ;NO...
;
	JSR UNLSN       ;YES...UNLISTEN IT
;
JX750	CPX DFLTN       ;IS INPUT CHANNEL IEEE?
	BCS CLALL2      ;NO...
;
	JSR UNTLK       ;YES...UNTALK IT
;
;RESTORE DEFAULT VALUES
;
;
CLALL2	STX DFLTO       ;OUTPUT CHAN=3=SCREEN
	LDA #0
	STA DFLTN       ;INPUT CHAN=0=KEYBOARD
	RTS
.END
