DSEG	SEGMENT
	char	DW	1 DUP(0)
	inta	DW	1 DUP(0)
	float	DW	0 DUP(0)
	TEMP	DW	0 DUP(0)
	CONST	DW	1,0
DSEG	ENDS
CSEG	SEGMENT
	ASSUME	CS:CSEG,DS:DSEG
COUT	PROC	NEAR
	MOV	SI,10
	MOV	CX,0
next1:	MOV	DX,0
	DIV	SI
	PUSH	DX
	INC	CX
	CMP	AX,0
	JNZ	next1
next2:	POP	DX
	ADD	DL,30H
	MOV	AH,02H
	INT	21H
	LOOP	next2
	RET
COUT	ENDP
start:	MOV	AX,DSEG
	MOV	DS,AX
	MOV	AX,char[2]
	CALL	COUT
	MOV	AX,CONST[0]
	MOV	char[4],AX
	MOV	AX,char[4]
	MOV	char[0],AX
CSEG	ENDS
	END	START
