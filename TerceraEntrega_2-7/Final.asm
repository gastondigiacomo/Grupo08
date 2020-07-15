include macros2.asm
include number.asm
.MODEL LARGE

.386
.STACK 200h


.DATA
	@aux_0          	 dd 			 ?
	@aux_1          	 dd 			 ?

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
	_cte_string_0   	 db 			 "A VER SI SALE",10,13,'$'
	_cte_string_1   	 db 			 "INGRESE TEXTO",10,13,'$'
	_4              	 dd 			 4.0
	_5              	 dd 			 5.0
	                	  			 
.CODE
START:
MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX

	LEA EAX, _cte_string_0
	MOV variable_string_1, EAX

	DisplayString variable_string_1
	newline

	DisplayString _cte_string_1
	newline

	GetString variable_string_2

	DisplayString variable_string_2
	newline
	FLD _4
	FLD _5
	FADD
	fstp @aux_1
	ffree
	FLD @aux_1
	fstp a
	ffree

	DisplayFloat a,0
	newline

	mov ah, 1 ; pausa, espera que oprima una tecla
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h       ; Enviamos la interrupcion 21h

END START
