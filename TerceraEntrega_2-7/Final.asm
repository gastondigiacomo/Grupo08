include macros2.asm
include number.asm
 .MODEL LARGE
.386
.STACK 200h
 
.DATA

.CODE
START:
	JMP ET_1

ET_1:
	FLD 5
	fstp var2
mov ax, 4C00h
int 21h 

END START 4C00h
int 21h 

END START