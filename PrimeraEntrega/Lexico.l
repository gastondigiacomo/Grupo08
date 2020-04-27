%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <float.h>
#include "y.tab.h"
FILE  *yyin;

int show_error(char*);

%}
%option noyywrap  
%option yylineno 


DIGITO		[0-9]
LETRA		[a-zA-Z]
ID			{LETRA}({LETRA}|{DIGITO}|_)*
COM_A       \*\*\*\/
COM_C       \/\*\*\*
COM         ({LETRA}|{DIGITO}|{IGNORA})*
COM_COMP    {COM_A}(({COM}?({COM_C}{COM}?{COM_A})?{COM}?)|({COM}?({COM_A}{COM}?{COM_C})?{COM}?)){COM_C}
IGNORA		(" "|\t|\n|\r)
REAL        ({DIGITO}*"."{DIGITO}+)|({DIGITO}+"."{DIGITO}*)
ENTERO      {DIGITO}+
CADENA      \"({LETRA}|{DIGITO}|" "|"."|",")*\"


%%

","         { return COMA;}
";"         { return PYC;}
"["         { return C_A;}
"]"         { return C_C;}
":"         { return D_P;}

"="		    { return ASIG;}
"+"			{ return OP_SUMA;}
"-"			{ return OP_RESTA;}
"*"			{ return OP_MUL;}
"/"			{ return OP_DIV;}

"("			{ return P_A;}
")"			{ return P_C;}
"{"         { return L_A;}
"}"         { return L_C;}

"while"     { return WHILE;}
"if"        { return IF;}
"between"   { return BETWEEN;}
"DEFVAR"    { return DEFVAR;}
"ENDDEF"    { return ENDDEF;}
"display"   { return DISPLAY;}
"get"       { return GET;}
"int"       { yylval.str_val = strdup(yytext); return INT;}
"string"    { yylval.str_val = strdup(yytext); return STRING;}
"float"     { yylval.str_val = strdup(yytext); return FLOAT;}



">"         { return OP_L_GT;}  //GREATER_THAN
"<"         { return OP_L_LT;}  //LESS_THAN
">="        { return OP_L_GET;} //GREATER_EQUAL_THAN
"<="        { return OP_L_LET;} //LESS_EQUAL_THAN
"=="        { return OP_L_E;}   //EQUAL
"!="        { return OP_L_NE;}  //NOT_EQUAL
"||"        { return OP_L_O;}   //OR
"&&"        { return OP_L_A;}   //AND
"!"         { return OP_L_N;}   //NOT


{ID}			{
                    if(yyleng <= 30){ 
                        yylval.str_val = strdup(yytext);
                        return ID;
                    }
                    show_error("ERROR LEXICO. NOMBRE DE VARIABLE MAYOR a 30 caracteres\n");
                }

{ENTERO}	    { 
                    if(atoi( yytext ) <= 32767 /*&& atoi( yytext ) >= -32768*/){
                        yylval.int_val = atoi(yytext);
                        return ENTERO;
                    }
                    show_error("ERROR LEXICO. ENTERO FUERA DE RANGO\n");
                }

{REAL}          {
                    if(atof( yytext ) < FLT_MAX && atof( yytext ) > FLT_MIN ){
                        yylval.real_val = atof(yytext);
                        return REAL;
                    }
                    show_error("ERROR LEXICO. REAL FUERA DE RANGO\n");
                }

{CADENA}        {
                    if(yyleng <= 32){ //ya que las comillas ocupan dos caracteres
                        yylval.str_val = strdup(yytext);
                        return CADENA;
                    }
                    show_error("ERROR LEXICO. STRING MAYOR a 30 caracteres\n");
                }


{IGNORA}        
{COM_COMP}	    

.               { show_error("Caracter invalido!\n"); }
%%

int show_error(char* mensaje) {
	printf("\nError lexico en linea %d: %s\n", yylineno, mensaje);
	system("Pause");
	exit(1);
}


