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
	_cte_string_0   	 db 			 "HOLA",10,13,'$'
	_cte_string_1   	 db 			 "INGRESE TEXTO",10,13,'$'
	_cte_string_2   	 db 			 "INGRESE UN NUM ENTRE 10 y 20",10,13,'$'
	_10             	 dd 			 10.0
	_20             	 dd 			 20.0
	_cte_string_3   	 db 			 "dije un NUM entre 10 y 20",10,13,'$'
	_0              	 dd 			 0.0
	_1              	 dd 			 1.0
	                	  			 
.CODE
START:
MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX

	LEA EAX, _cte_string_0
	MOV variable_string_2, EAX

	DisplayString variable_string_2
	DisplayString NEW_LINE

	DisplayFloat a,0
	DisplayString NEW_LINE

	DisplayString _cte_string_1
	DisplayString NEW_LINE

	GetString variable_string_2

	DisplayString variable_string_2
	DisplayString NEW_LINE

	DisplayString _cte_string_2
	DisplayString NEW_LINE

	GetFloat b
ET_0:
	FLD b
	FCOMP _10
	fstsw ax
	sahf
	JNB X_0
ET_1:
	FLD b
	FCOMP _20
	fstsw ax
	sahf
	JNA X_1

	DisplayString _cte_string_3
	DisplayString NEW_LINE

	GetFloat b
	JMP ET_1
X_1:
ET_2:
	FLD b
	FCOMP _0
	fstsw ax
	sahf
	JNAE X_2

	DisplayFloat b,0
	DisplayString NEW_LINE
	FLD b
	FLD _1
	FSUB
	fstp @aux_1
	ffree
	FLD @aux_1
	fstp b
	ffree
	JMP ET_2
X_2:

	mov ah, 1 ; pausa, espera que oprima una tecla
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h       ; Enviamos la interrupcion 21h

END START
