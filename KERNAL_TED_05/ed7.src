	.page
	.subttl 'ed.7    ted 12/09/83'
keycod 					;keyboard mode 'dispatch'
       .word mode1
       .word mode2
       .word mode3
       .word contrl    			;control keys


mode1				  	;unshifted,no control
	.byte   $14             	; del
	.byte   $0d             	; return
	.byte   $5c             	; pound-l
	.byte   $8c             	; f8 (help)
	.byte   $85             	; f1
	.byte   $89             	; f2
	.byte   $86             	; f3
	.byte   $40             	; @

	.byte   $33             	; 3
	.byte   $57             	; w
	.byte   $41             	; a
	.byte   $34             	; 4
	.byte   $5a             	; z
	.byte   $53             	; s
	.byte   $45             	; e
	.byte   $01             	; shift

	.byte   $35             	; 5
	.byte   $52             	; r
	.byte   $44             	; d
	.byte   $36             	; 6
	.byte   $43             	; c
	.byte   $46             	; f
	.byte   $54             	; t
	.byte   $58             	; x

	.byte   $37             	; 7
	.byte   $59             	; y
	.byte   $47             	; g
	.byte   $38             	; 8
	.byte   $42             	; b
	.byte   $48             	; h
	.byte   $55             	; u
	.byte   $56             	; v

	.byte   $39             	; 9
	.byte   $49             	; i
	.byte   $4a             	; j
	.byte   $30             	; 0
	.byte   $4d             	; m
	.byte   $4b             	; k
	.byte   $4f             	; o
	.byte   $4e             	; n

	.byte   $11             	; crsr down
	.byte   $50             	; p
	.byte   $4c             	; l
	.byte   $91             	; crsr up
	.byte   $2e             	; .
	.byte   $3a             	; :
	.byte   $2d             	; -
	.byte   $2c             	; ,

	.byte   $9d             	; crsr left
	.byte   $2a             	; *
	.byte   $3b             	; ;
	.byte   $1d             	; crsr right
	.byte   $1b             	; escape
	.byte   $3d             	; =
	.byte   $2b             	; +
	.byte   $2f             	; /

	.byte   $31             	; 1
	.byte   $13             	; home
	.byte   $04             	; control
	.byte   $32             	; 2
	.byte   $20             	; space
	.byte   $02             	; commodore key
	.byte   $51             	; q
	.byte   $03             	; stop
	.byte   $ff             	;end of table null


mode2 				 	;shifted,no control
	.byte   $94             	; insert
	.byte   $8d             	; shift return
	.byte   $a9             	; pound-l
	.byte   $88             	; f7
	.byte   $8a             	; f4
	.byte   $87             	; f5
	.byte   $8b             	; f6
	.byte   $ba             	; @

	.byte   $23             	; #
	.byte   $d7             	; w
	.byte   $c1             	; a
	.byte   $24             	; $
	.byte   $da             	; z
	.byte   $d3             	; s
	.byte   $c5             	; e
	.byte   $01             	; shift

	.byte   $25             	; %
	.byte   $d2             	; r
	.byte   $c4             	; d
	.byte   $26             	; &
	.byte   $c3             	; c
	.byte   $c6             	; f
	.byte   $d4             	; t
	.byte   $d8             	; x

	.byte   $27             	; '
	.byte   $d9             	; y
	.byte   $c7             	; g
	.byte   $28             	; (
	.byte   $c2             	; b
	.byte   $c8             	; h
	.byte   $d5             	; u
	.byte   $d6             	; v

	.byte   $29             	; )
	.byte   $c9             	; i
	.byte   $ca             	; j
	.byte   $5e             	; ~
	.byte   $cd             	; m
	.byte   $cb             	; k
	.byte   $cf             	; o
	.byte   $ce             	; n

	.byte   $11             	; crsr down
	.byte   $d0             	; p
	.byte   $cc             	; l
	.byte   $91             	; crsr up
	.byte   $3e             	; >
	.byte   $5b             	; {
	.byte   $dd             	; -
	.byte   $3c             	; <

	.byte   $9d             	; crsr left
	.byte   $c0             	; *
	.byte   $5d             	; }
	.byte   $1d             	; crsr right
	.byte   $1b             	; escape
	.byte   $5f             	; left arrow
	.byte   $db             	; +
	.byte   $3f             	; ?

	.byte   $21             	; !
	.byte   $93             	; clr screen
	.byte   $04             	; control
	.byte   $22             	; "
	.byte   $a0             	; shifted space
	.byte   $02             	; commodore key
	.byte   $d1             	; q
	.byte   $83             	; run
	.byte   $ff             	;end of table null


mode3				  	;commodore key (left window graphics)
	.byte   $94             	; insert
	.byte   $8d             	; shifted return
	.byte   $a8             	; pound-l
	.byte   $88             	; f7
	.byte   $8a             	; f4
	.byte   $87             	; f5
	.byte   $8b             	; f6
	.byte   $a4             	; @

	.byte   $96             	; color 10
	.byte   $b3             	; w
	.byte   $b0             	; a
	.byte   $97             	; color 11
	.byte   $ad             	; z
	.byte   $ae             	; s
	.byte   $b1             	; e
	.byte   $01             	; shift

	.byte   $98             	; color 12
	.byte   $b2             	; r
	.byte   $ac             	; d
	.byte   $99             	; color 13
	.byte   $bc             	; c
	.byte   $bb             	; f
	.byte   $a3             	; t
	.byte   $bd             	; x

	.byte   $9a             	; color 14
	.byte   $b7             	; y
	.byte   $a5             	; g
	.byte   $9b             	; color 15
	.byte   $bf             	; b
	.byte   $b4             	; h
	.byte   $b8             	; u
	.byte   $be             	; v

	.byte   $29             	; )
	.byte   $a2             	; i
	.byte   $b5             	; j
	.byte   $30             	; 0
	.byte   $a7             	; m
	.byte   $a1             	; k
	.byte   $b9             	; o
	.byte   $aa             	; n

	.byte   $11             	; crsr down
	.byte   $af             	; p
	.byte   $b6             	; l
	.byte   $91             	; crsr up
	.byte   $3e             	; >
	.byte   $5b             	; {
	.byte   $dc             	; -
	.byte   $3c             	; <

	.byte   $9d             	; crsr left
	.byte   $df             	; *
	.byte   $5d             	; }
	.byte   $1d             	; crsr right
	.byte   $1b             	; escape
	.byte   $de             	; pi
	.byte   $a6             	; +
	.byte   $3f             	; ?

	.byte   $81             	; color 8
	.byte   $93             	; cls
	.byte   $04             	; control
	.byte   $95             	; color 9
	.byte   $a0             	; shifted space
	.byte   $02             	; commodore key
	.byte   $ab             	; q
	.byte   $83             	; run
	.byte   $ff             	;end of table null


contrl
	.byte   $ff             	; del
	.byte   $ff             	; return
	.byte   $1c             	; pound-l
	.byte   $ff             	; f8 (help)
	.byte   $ff             	; f1
	.byte   $ff             	; f2
	.byte   $ff             	; f3
	.byte   $ff             	; @

	.byte   $1c             	; 3
	.byte   $17             	; w
	.byte   $01             	; a
	.byte   $9f             	; 4
	.byte   $1a             	; z
	.byte   $13             	; s
	.byte   $05             	; e
	.byte   $ff             	; shift

	.byte   $9c             	; 5
	.byte   $12             	; r
	.byte   $04             	; d
	.byte   $1e             	; 6
	.byte   $03             	; c
	.byte   $06             	; f
	.byte   $14             	; t
	.byte   $18             	; x

	.byte   $1f             	; 7
	.byte   $19             	; y
	.byte   $07             	; g
	.byte   $9e             	; 8
	.byte   $02             	; b
	.byte   $08             	; h
	.byte   $15             	; u
	.byte   $16             	; v

	.byte   $12             	; 9
	.byte   $09             	; i
	.byte   $0a             	; j
	.byte   $92             	; 0
	.byte   $0d             	; m
	.byte   $0b             	; k
	.byte   $0f             	; o
	.byte   $0e             	; n

	.byte   $ff             	; crsr down
	.byte   $10             	; p
	.byte   $0c             	; l
	.byte   $ff             	; crsr up
	.byte   $84             	; flash off
	.byte   $1b             	; :
	.byte   $ff             	; -
	.byte   $82             	; flash on

	.byte   $ff             	; crsr left
	.byte   $ff             	; *
	.byte   $1d             	; ;
	.byte   $ff             	; crsr right
	.byte   $1b             	; escape
	.byte   $06             	; =
	.byte   $ff             	; +
	.byte   $ff             	; /

	.byte   $90             	; 1
	.byte   $ff             	; home
	.byte   $ff             	; control
	.byte   $05             	; 2
	.byte   $ff             	; space
	.byte   $ff             	; commodore key
	.byte   $11             	; q
	.byte   $ff             	; stop
	.byte   $ff             	;end of table null

runtb  .byte $44,$cc,$22,$2a,cr,'RUN',cr 	;dload '* : run

coltab
       .byte $90,$05,$1c,$9f,$9c,$1e,$1f,$9e
       .byte $81,$95,$96,$97,$98,$99,$9a,$9b
collum
       .byte $00,$71,$32,$63,$44,$35,$46,$77
       .byte $48,$29,$5a,$6b,$5c,$6d,$2e,$5f

linz0  = tedscn
linz1  = linz0+llen
linz2  = linz1+llen
linz3  = linz2+llen
linz4  = linz3+llen
linz5  = linz4+llen
linz6  = linz5+llen
linz7  = linz6+llen
linz8  = linz7+llen
linz9  = linz8+llen
linz10 = linz9+llen
linz11 = linz10+llen
linz12 = linz11+llen
linz13 = linz12+llen
linz14 = linz13+llen
linz15 = linz14+llen
linz16 = linz15+llen
linz17 = linz16+llen
linz18 = linz17+llen
linz19 = linz18+llen
linz20 = linz19+llen
linz21 = linz20+llen
linz22 = linz21+llen
linz23 = linz22+llen
linz24 = linz23+llen

;end
