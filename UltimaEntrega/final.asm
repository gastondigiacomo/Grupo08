include macros2.asm
include number.asm
.MODEL LARGE

.386
.STACK 200h


.DATA
	@aux_0          	 dd 			 ?
	@aux_1          	 dd 			 ?

	NEW_LINE        	 db 			 0AH,0DH,'$'
	r               	 dd 			 ?
	d               	 dd 			 ?
	z               	 dd 			 ?
	x               	 dd 			 ?
	pepe1           	 dd 			 ?
	pepe2           	 dd 			 ?
	a               	 dd 			 ?
	b               	 dd 			 ?
	c               	 dd 			 ?
	var1            	 dd 			 ?
	var2            	 dd 			 ?
	var3            	 dd 			 ?
	var4            	 dd 			 ?
	var5            	 dd 			 ?
	resul           	 dd 			 ?
	pepe            	 dd 			 ?
	pepe3           	 dd 			 ?
	pepe4           	 dd 			 ?
	variable_string_1 	 dd 			 ?
	variable_string_2 	 dd 			 ?
	_0              	 dd 			 0.0
	_4              	 dd 			 4.0
	_2              	 dd 			 2.0
	_cte_string_0   	 db 			 "ENTRO EN LA OTRA",10,13,'$'
	_1              	 dd 			 1.0
	_3              	 dd 			 3.0
	_cte_string_1   	 db 			 "a igual a 3",10,13,'$'
	_cte_string_2   	 db 			 "a distinto de 3",10,13,'$'
	                	  			 
.CODE
START:
MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX

	FLD _0
	fstp a
	ffree
ET_0:
	FLD a
	FCOMP _4
	fstsw ax
	sahf
	JNB X_0
	FLD a
	FCOMP _2
	fstsw ax
	sahf
	JNB X_0

	DisplayString _cte_string_0
	DisplayString NEW_LINE

	DisplayFloat a,0
	DisplayString NEW_LINE
	FLD a
	FLD _1
	FADD
	fstp @aux_1
	ffree
	FLD @aux_1
	fstp a
	ffree
	JMP ET_0
X_0:
ET_1:
	FLD a
	FCOMP _3
	fstsw ax
	sahf
	JNE X_1

	DisplayString _cte_string_1
	DisplayString NEW_LINE
	JMP X_2
X_1:

	DisplayString _cte_string_2
	DisplayString NEW_LINE
X_2:

	mov ah, 1 ; pausa, espera que oprima una tecla
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h       ; Enviamos la interrupcion 21h

END START
