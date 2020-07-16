tasm NUMBERS.ASM
tasm Final.asm
tlink /3 Final.OBJ NUMBERS.OBJ /v /s /m
pause
Grupo08.exe
pause
del Final.obj
del Final.lst
del numbers.obj
del numbers.lst
