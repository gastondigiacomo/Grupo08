include macros2.asm
include number.asm
.MODEL LARGE
.386
.STACK 200h

.CODE

MAIN:

	MOV EAX, @DATA
	MOV DS, EAX
	MOV ES, EAX

START:
	FLD 5
	fstp a
	ffree

	FLD 10
	fstp b
	ffree

	FLD a
	FCOMP _10
	fstsw ax
	sahf
	JNB ET_1

	FLD a
	FLD 1
	FADD
	fstp @aux1
	ffree

	FLD @aux1
	fstp a
	ffree

	JMP ET_2

ET_1:
	FLD a
	FLD 1
	FSUB
	fstp @aux2
	ffree

	FLD @aux2
	fstp a
	ffree

ET_2:
	FLD b
	FCOMP _0
	fstsw ax
	sahf
	JNAE ET_3

	FLD b
	FLD 1
	FSUB
	fstp @aux3
	ffree

	FLD @aux3
	fstp b
	ffree

	JMP ET_2
ET_3:
	FLD b
	FCOMP _10
	fstsw ax
	sahf
	JNBE ET_4

	FLD b
	FLD 1
	FADD
	fstp @aux4
	ffree

	FLD @aux4
	fstp b
	ffree

	FLD b
	FCOMP _5
	fstsw ax
	sahf
	JNE ET_5

	FLD 1
	fstp resul
	ffree

	JMP ET_6

ET_5:
	FLD 0
	fstp resul
	ffree

	JMP ET_3
ET_4:
	FLD 10
	fstp c
	ffree



MOV EAX, 4C00h
INT 21h

.DATA
	r          	DD 			?
	d          	DD 			?
	z          	DD 			?
	x          	DD 			?
	pepe1      	DD 			?
	pepe2      	DD 			?
	a          	DD 			?
	b          	DD 			?
	c          	DD 			?
	var1       	DD 			?
	var2       	DD 			?
	var3       	DD 			?
	var4       	DD 			?
	var5       	DD 			?
	resul      	DD 			?
	pepe       	DD 			?
	pepe3      	DD 			?
	pepe4      	DD 			?
	a2a        	DD 			?
	ssd        	DD 			?
	_5         	DD 			5.0
	_10        	DD 			10.0
	_1         	DD 			1.0
	_0         	DD 			0.0
	NEW_LINE 	DB 			0AH,0DH,'$'

END START