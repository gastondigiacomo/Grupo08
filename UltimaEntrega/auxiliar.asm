include macros2.asm
include number.asm
.MODEL LARGE

.386
.STACK 200h


.DATA
	@aux_0          	 dd 			 ?

	NEW_LINE        	 db 			 0AH,0DH,'$'
	@ID_AUX         	 dd 			 ?
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
	_cte_string_0   	 db 			 "INGRESE UN NUM",10,13,'$'
	_3              	 dd 			 3.0
	_10             	 dd 			 10.0
	_1              	 dd 			 1.0
	_0              	 dd 			 0.0
	_cte_string_1   	 db 			 "APROBAMOS",10,13,'$'
	_cte_string_2   	 db 			 "NO APROBAMOS",10,13,'$'
	                	  			 
.CODE
START:
MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX


	DisplayString _cte_string_0
	DisplayString NEW_LINE

	GetFloat b
ET_0:
	FLD b
	FCOMP _3
	fstsw ax
	sahf
	JNAE X_0
ET_1:
	FLD b
	FCOMP _10
	fstsw ax
	sahf
	JNBE X_1
	FLD _1
	fstp @ID_AUX
	ffree
	JMP X_2
X_1:
	FLD _0
	fstp @ID_AUX
	ffree
X_2:
	JMP X_3
X_0:
	FLD _0
	fstp @ID_AUX
	ffree
X_3:
ET_2:
	FLD @ID_AUX
	FCOMP 0
	fstsw ax
	sahf
	JE X_4

	DisplayString _cte_string_1
	DisplayString NEW_LINE
	JMP X_5
X_4:

	DisplayString _cte_string_2
	DisplayString NEW_LINE
X_5:

	mov ah, 1 ; pausa, espera que oprima una tecla
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h       ; Enviamos la interrupcion 21h

END START
