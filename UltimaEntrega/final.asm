include macros2.asm
include number.asm
.MODEL LARGE

.386
.STACK 200h


.DATA
	@aux_0          	 dd 			 ?
	@aux_1          	 dd 			 ?
	@aux_2          	 dd 			 ?

	NEW_LINE        	 db 			 0AH,0DH,'$'
	@ID_AUX         	 dd 			 ?
	r               	 dd 			 ?
	d               	 dd 			 ?
	z               	 dd 			 ?
	a               	 dd 			 ?
	b               	 dd 			 ?
	c               	 dd 			 ?
	alumnos         	 dd 			 ?
	nota            	 dd 			 ?
	nota2           	 dd 			 ?
	flagn1          	 dd 			 ?
	flagn2          	 dd 			 ?
	float1          	 dd 			 ?
	variable_string_1 	 dd 			 ?
	variable_string_2 	 dd 			 ?
	_cte_string_0   	 db 			 "COMPILADOR GRUPO 08",10,13,'$'
	_0              	 dd 			 0.0
	_cte_string_1   	 db 			 "Ingrese cantidad de alumnos",10,13,'$'
	_cte_string_2   	 db 			 "ALUMNOS IGUAL A CERO",10,13,'$'
	_cte_string_3   	 db 			 "NO SE EJECUTA EL PROGRAMA",10,13,'$'
	_cte_string_4   	 db 			 "Ingrese nota primer parcial",10,13,'$'
	_3              	 dd 			 3.0
	_6              	 dd 			 6.0
	_cte_string_5   	 db 			 "Falto muy poquito",10,13,'$'
	_7              	 dd 			 7.0
	_10             	 dd 			 10.0
	_1              	 dd 			 1.0
	_cte_string_6   	 db 			 "Ingrese nota segundo parcial",10,13,'$'
	_cte_string_7   	 db 			 "Falto muy poquito",10,13,'$'
	_2              	 dd 			 2.0
	_cte_string_8   	 db 			 "El alumno esta PROMOCIONADO",10,13,'$'
	_cte_string_9   	 db 			 "El alumno NO esta PROMOCIONADO",10,13,'$'
	_cte_string_10  	 db 			 "IOIOIOIOIOIOIOIOIOIO",10,13,'$'
	                	  			 
.CODE
START:
MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX


	DisplayString _cte_string_0
	DisplayString NEW_LINE
	FLD _0
	fstp a
	ffree

	DisplayString _cte_string_1
	DisplayString NEW_LINE

	GetFloat alumnos
ET_0:
	FLD alumnos
	FCOMP _0
	fstsw ax
	sahf
	JNBE X_0

	DisplayString _cte_string_2
	DisplayString NEW_LINE

	DisplayString _cte_string_3
	DisplayString NEW_LINE
	JMP X_1
X_0:
ET_1:
	FLD a
	FCOMP alumnos
	fstsw ax
	sahf
	JNB X_2

	DisplayString _cte_string_4
	DisplayString NEW_LINE

	GetFloat nota
ET_2:
	FLD nota
	FCOMP _3
	fstsw ax
	sahf
	JNE X_3

	DisplayString _cte_string_5
	DisplayString NEW_LINE
	JMP X_4
X_3:
ET_3:
	FLD nota
	FCOMP _6
	fstsw ax
	sahf
	JNE X_5

	DisplayString _cte_string_5
	DisplayString NEW_LINE
X_5:
X_4:
ET_4:
	FLD nota
	FCOMP _7
	fstsw ax
	sahf
	JNAE X_6
ET_5:
	FLD nota
	FCOMP _10
	fstsw ax
	sahf
	JNBE X_7
	FLD _1
	fstp @ID_AUX
	ffree
	JMP X_8
X_7:
	FLD _0
	fstp @ID_AUX
	ffree
X_8:
	JMP X_9
X_6:
	FLD _0
	fstp @ID_AUX
	ffree
X_9:
ET_6:
	FLD @ID_AUX
	FCOMP _0
	fstsw ax
	sahf
	JE X_10
	FLD _1
	fstp flagn1
	ffree
	JMP X_11
X_10:
	FLD _0
	fstp flagn1
	ffree
X_11:

	DisplayString _cte_string_6
	DisplayString NEW_LINE

	GetFloat nota2
ET_7:
	FLD nota2
	FCOMP _3
	fstsw ax
	sahf
	JNE X_12

	DisplayString _cte_string_7
	DisplayString NEW_LINE
	JMP X_13
X_12:
ET_8:
	FLD nota2
	FCOMP _6
	fstsw ax
	sahf
	JNE X_14

	DisplayString _cte_string_7
	DisplayString NEW_LINE
X_14:
X_13:
ET_9:
	FLD nota2
	FCOMP _7
	fstsw ax
	sahf
	JNAE X_15
ET_10:
	FLD nota2
	FCOMP _10
	fstsw ax
	sahf
	JNBE X_16
	FLD _1
	fstp @ID_AUX
	ffree
	JMP X_17
X_16:
	FLD _0
	fstp @ID_AUX
	ffree
X_17:
	JMP X_18
X_15:
	FLD _0
	fstp @ID_AUX
	ffree
X_18:
ET_11:
	FLD @ID_AUX
	FCOMP _0
	fstsw ax
	sahf
	JE X_19
	FLD _1
	fstp flagn2
	ffree
	JMP X_20
X_19:
	FLD _0
	fstp flagn2
	ffree
X_20:
	FLD a
	FLD _1
	FADD
	fstp @aux_1
	ffree
	FLD @aux_1
	fstp a
	ffree
	FLD _2
	fstp b
	ffree
ET_12:
	FLD flagn1
	FCOMP _1
	fstsw ax
	sahf
	JNE X_21
ET_13:
	FLD flagn2
	FCOMP _1
	fstsw ax
	sahf
	JNE X_22

	DisplayString _cte_string_8
	DisplayString NEW_LINE
	JMP X_23
X_22:

	DisplayString _cte_string_9
	DisplayString NEW_LINE
X_23:
	JMP X_24
X_21:

	DisplayString _cte_string_9
	DisplayString NEW_LINE
X_24:
ET_14:
	FLD b
	FCOMP _3
	fstsw ax
	sahf
	JNB X_25

	DisplayString _cte_string_10
	DisplayString NEW_LINE
	FLD b
	FLD _1
	FADD
	fstp @aux_2
	ffree
	FLD @aux_2
	fstp b
	ffree
	JMP ET_14
X_25:
	JMP ET_1
X_2:
X_1:

	mov ah, 1 ; pausa, espera que oprima una tecla
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h       ; Enviamos la interrupcion 21h

END START
