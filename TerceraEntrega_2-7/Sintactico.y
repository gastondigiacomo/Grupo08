
%{
#include "funciones.c"
#include "y.tab.h"

FILE  *yyin;

%}


%union {
      int int_val;
      float real_val;
      char* str_val;
}

//OPERADORES ARITMERICOS
%token OP_SUMA OP_RESTA OP_MUL OP_DIV 

//TIPOS DE DATOS
%token <int_val> ENTERO
%token <real_val> REAL
%token <str_val> CADENA
%token <str_val> ID

//CARACTERES
%token COMA PYC D_P
%token ASIG

//OPERADORES LOGICOS
%token OP_L_GT OP_L_LT OP_L_GET OP_L_LET OP_L_E OP_L_NE OP_L_O OP_L_A OP_L_N

//PALABRAS RESERVADAS
%token WHILE IF BETWEEN DEFVAR ENDDEF GET DISPLAY ELSE

//PARENTESIS Y LLAVES Y CORCHETES
%token L_A L_C P_A P_C C_A C_C

//TIPO VARIABLES
%token <str_val> INT FLOAT STRING

%%

start:
      programa
      {
            print2D(arbolPrograma);
            generarASM(arbolPrograma, "ET_0");
            //recorrerArbol(arbolPrograma, "ET_0");
            printf("FIN\n");
      }
      ;

programa: 
      bloque_declaracion bloque 
      {
            
            int i = 0;
            arbolPrograma = desapilarBloque();
            FILE * archivo_arbol;
            archivo_arbol = fopen("intermedia.txt","wt");
            guardarArbolIRD(&arbolPrograma, archivo_arbol);
            printf("COMPILACION CORRECTA\n");
      }
      |bloque
      {
            arbolPrograma = desapilarBloque();
            FILE * archivo_arbol;
            archivo_arbol = fopen("arbol.txt","wt");
            guardarArbolIRD(&arbolPrograma, archivo_arbol);
            printf("COMPILACION CORRECTA\n");
      }
      ;

bloque_declaracion:
      DEFVAR declaraciones ENDDEF
      ;

declaraciones:
      declaraciones declaracion
      |declaracion
      ;

declaracion:
      tipo_variable D_P lista_id
      {
        guardar_variables_ts();
        freeArray(&array_nombres_variables);
        initArray(&array_nombres_variables);
      }
      ;

tipo_variable:
      INT
      {
        strcpy(tipo_dato,$<str_val>1);
      }
      |FLOAT
      {
        strcpy(tipo_dato,$<str_val>1);
      }
      |STRING
      {
        strcpy(tipo_dato,$<str_val>1);
      }
      ;

lista_id:
      lista_id PYC ID
      {
        insertArray(&array_nombres_variables,$<str_val>3);
      }
      |ID
      {
        insertArray(&array_nombres_variables,$<str_val>1);
      }
      ;

bloque: 
      bloque sentencia
      {
            arbolBloque = desapilarBloque();
		arbolBloque = (*crear_nodo("BLOQUE",&arbolBloque, &arbolSentencia));
            apilarBloque(&arbolBloque);
      }
      |sentencia
      {
            apilarBloque(&arbolSentencia);
      }
      ;

sentencia: 
      asignacion
      {
            arbolSentencia = arbolAsignacion;
      }
      |decision
      {
            arbolSentencia = arbolDecision;
      }
      |iteracion
      {
            arbolSentencia = arbolIteracion;
      }
      |salida
      {
            arbolSentencia = arbolDisplay;
      }
      |entrada
      {
            arbolSentencia = arbolGet;
      }
      ;

salida:
      DISPLAY ENTERO
      {
            guardar_cte_int($<int_val>2);
            char aux[100];
            sprintf(aux,"%d",$<int_val>1);
            arbolDisplay = (*crear_hoja(aux));
            arbolDisplay = (*crear_nodo("DISPLAY", &arbolDisplay, NULL));
      }
      |DISPLAY REAL
      {
            guardar_cte_float($<real_val>2);
            char aux[100];
            sprintf(aux,"%f",$<int_val>1);
            arbolDisplay = (*crear_hoja(aux));
            arbolDisplay = (*crear_nodo("DISPLAY", &arbolDisplay, NULL));
      }
      |DISPLAY CADENA
      {
            char* nombre_cte_string = guardar_cte_string($<str_val>2);
            arbolDisplay = (*crear_hoja(nombre_cte_string));
            arbolDisplay = (*crear_nodo("DISPLAY", &arbolDisplay, NULL));
            free(nombre_cte_string);
      }
      |DISPLAY ID
      {
            if(!existe_simbolo($<str_val>2)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                  yyerror();
            }else{
                  arbolDisplay = (*crear_hoja($<str_val>2));
                  arbolDisplay = (*crear_nodo("DISPLAY", &arbolDisplay, NULL));
            }
      }
      ;

entrada:
      GET ID
      {
            if(!existe_simbolo($<str_val>2)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                  yyerror();
            }
            else{
                  arbolGet = (*crear_hoja($<str_val>2));
                  arbolGet = (*crear_nodo("GET", &arbolGet, NULL));
            }
      }
      ;

asignacion: 
      ID ASIG expresion
      {
            t_arbol aux_expresion;
            switch(verificar_asignacion($<str_val>1)){
                  case 1:     printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>1);
                              yyerror();
                              break;
                  case 2:     aux_expresion = desapilar();
                              arbolAsignacion = (*crear_hoja($<str_val>1));
                              arbolAsignacion = (*crear_nodo("=", &arbolAsignacion, &aux_expresion));
                              break;
                  case 3:     printf("ERROR DE SINTAXIS, ASIGNACION ERRONEA, TIPOS DE DATOS INCORRECTOS.\n"); 
                              printf("USTED ESTA INTENTANDO ASIGNAR UNA CONSTANTE %s A UNA VARIABLE %s \n", ultima_expresion, simbolo_busqueda.tipo_dato);
                              yyerror();
                              break;
            }
      }
      |ID operador ASIG expresion
      {
            t_arbol aux_expresion = desapilar();
            arbolAsignacion = (*crear_hoja($<str_val>1));
            arbolAsignacionEspecial = (*crear_nodo(ultimo_operador, &arbolAsignacion,&aux_expresion));
            arbolAsignacion = (*crear_hoja($<str_val>1));
            arbolAsignacion = (*crear_nodo("=",&arbolAsignacion,&arbolAsignacionEspecial));
      }
      |ID ASIG CADENA
      {
            char * nombre_cte_string = guardar_cte_string($<str_val>3);
            ultima_expresion = "string";
            switch(verificar_asignacion($<str_val>1)){
                  case 1:   printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>1);
                              free(nombre_cte_string);
                              yyerror();
                              break;
                  case 2:   arbolAsignacion = (*crear_hoja($<str_val>1));
                              arbolAsignacionCadena = (*crear_hoja(nombre_cte_string));
                              arbolAsignacion = (*crear_nodo("=",&arbolAsignacion,&arbolAsignacionCadena));
                              free(nombre_cte_string);
                              break;
                  case 3:   printf("ERROR DE SINTAXIS, ASIGNACION ERRONEA, TIPOS DE DATOS INCORRECTOS.\n"); 
                              printf("USTED ESTA INTENTANDO ASIGNAR UNA CONSTANTE %s A UNA VARIABLE %s \n", ultima_expresion, simbolo_busqueda.tipo_dato);
                              free(nombre_cte_string);
                              yyerror();
                              break;
            }
      }
      ;

operador:
      OP_SUMA
      {
            ultimo_operador = "+";
      }
      |OP_RESTA
      {
            ultimo_operador = "-";
      }
      |OP_MUL
      {
            ultimo_operador = "*";
      }
      |OP_DIV
      {
            ultimo_operador = "/";
      }
      ;

iteracion:
      WHILE P_A condicion P_C L_A bloque L_C
      {
            t_arbol aux_arbol_condicion = desapilar();
            t_arbol bloque_then = desapilarBloque();

            arbolIteracion = (*crear_nodo("WHILE", &aux_arbol_condicion, &bloque_then));
      }
      ;

decision:
      IF P_A condicion P_C L_A bloque L_C
      {
            t_arbol bloque_then = desapilarBloque();
            t_arbol aux_arbol_condicion = desapilar();

            if (strcmp(aux_arbol_condicion-> dato, "OR") == 0) {
                  t_arbol cmp_1 = aux_arbol_condicion-> izq;
                  t_arbol cmp_2 = aux_arbol_condicion-> der;
                  
                  arbolDecision = (*crear_nodo("IF", &cmp_2, &bloque_then));
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &arbolDecision));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));

            }
            else if (strcmp(aux_arbol_condicion-> dato, "AND") == 0) {
                  t_arbol cmp_1 = aux_arbol_condicion-> izq;
                  t_arbol cmp_2 = aux_arbol_condicion-> der;

                  arbolDecision = (*crear_nodo("IF", &cmp_2, &bloque_then));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));
            }
            else {
                  arbolDecision = (*crear_nodo("IF",&aux_arbol_condicion,&bloque_then));
            }
            if(between_flag == 1) {
                  arbolDecision = (*crear_nodo("BLOQUE",&arbolBetween,&arbolDecision));
                  between_flag = 0;
            }
      }
      | IF P_A condicion P_C L_A bloque L_C ELSE L_A bloque L_C
      {
            t_arbol bloque_else = desapilarBloque();
            t_arbol bloque_then = desapilarBloque();
            t_arbol aux_arbol_condicion = desapilar();
            if (strcmp(aux_arbol_condicion-> dato, "OR") == 0) {
                  
                  t_arbol cmp_1 = aux_arbol_condicion-> izq;
                  t_arbol cmp_2 = aux_arbol_condicion-> der;
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &bloque_else));
                  arbolDecision = (*crear_nodo("IF", &cmp_2, &arbolDecision));
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &arbolDecision));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));
                  
            }
            else if (strcmp(aux_arbol_condicion-> dato, "AND") == 0) {
                  t_arbol cmp_1 = aux_arbol_condicion-> izq;
                  t_arbol cmp_2 = aux_arbol_condicion-> der;

                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &bloque_else));
                  arbolDecision = (*crear_nodo("IF", &cmp_2, &arbolDecision));
                  arbolDecision = (*crear_nodo("CUERPO", &arbolDecision, &bloque_else));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));
            }
            else {
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &bloque_else));
                  arbolDecision = (*crear_nodo("IF",&aux_arbol_condicion,&arbolDecision));
            }
            if(between_flag == 1) {
                  arbolDecision = (*crear_nodo("BLOQUE",&arbolBetween,&arbolDecision));
                  between_flag = 0;
            }
      }
      ;

condicion: 
      comparacion OP_L_O comparacion
      {
            t_arbol cmp2 = desapilar();
            t_arbol cmp1 = desapilar();
            arbolCondicion = (*crear_nodo("OR", &cmp1, &cmp2));
            apilar(&arbolCondicion);
      }
      |comparacion OP_L_A comparacion
      {
            t_arbol cmp2 = desapilar();
            t_arbol cmp1 = desapilar();
            arbolCondicion = (*crear_nodo("AND", &cmp1, &cmp2));
            apilar(&arbolCondicion);
      }
      |comparacion
      {
      }
      |OP_L_N comparacion
      {
            arbolCondicion = desapilar();
            arbolCondicion = negarComparacion(&arbolCondicion);
            apilar(&arbolCondicion);
      }
      |between
      {
            between_flag = 1;
            arbolCondicion = (*crear_hoja("0"));
            arbolCondicion = (*crear_nodo("!=",&id_aux,&arbolCondicion));
            apilar(&arbolCondicion);
      }
      ;

comparacion: 
      expresion comparador expresion
      {
            t_arbol expresion_2 = desapilar();
            t_arbol expresion_1 = desapilar();
            arbolComparacion = (*crear_nodo(ultimo_comparador,&expresion_1,&expresion_2));
            apilar(&arbolComparacion);
      }
      |expresion
      {
            t_arbol expresion_1 = desapilar();
            arbolComparacion = (*crear_hoja("0"));
            arbolComparacion = (*crear_nodo("!=",&expresion_1,&arbolComparacion));
            apilar(&arbolComparacion);
      }
      ;

between:
      BETWEEN P_A ID COMA C_A expresion PYC expresion C_C P_C
      {
            t_arbol limite_sup = desapilar();
            t_arbol limite_inf = desapilar();

            char* ID_AUX = "ID_AUX";
            t_arbol aux;

            id_aux = (*crear_hoja(ID_AUX));
            
            t_arbol verdadero = (*crear_hoja("1"));
            t_arbol falso = (*crear_hoja("0"));

            verdadero = (*crear_nodo("=", &id_aux, &verdadero));
            falso = (*crear_nodo("=", &id_aux, &falso));

            arbolBetween = (*crear_nodo("CUERPO", &verdadero, &falso));

            aux = (*crear_hoja($<str_val>3));
            aux = (*crear_nodo("<=", &aux, &limite_sup));

            arbolBetween = (*crear_nodo("IF", &aux, &arbolBetween));

            arbolBetween = (*crear_nodo("CUERPO", &arbolBetween, &falso));

            aux = (*crear_hoja($<str_val>3));
            aux = (*crear_nodo(">=", &aux, &limite_inf));
            
            arbolBetween = (*crear_nodo("IF", &aux, &arbolBetween));
      }
      ;

comparador:
      OP_L_GT
      {
            ultimo_comparador = ">";
      }
      |OP_L_GET
      {
            ultimo_comparador = ">=";
      }
      |OP_L_LET
      {
            ultimo_comparador = "<=";
      }
      |OP_L_LT
      {
            ultimo_comparador = "<";
      }
      |OP_L_E
      {
            ultimo_comparador = "==";
      }
      |OP_L_NE
      {
            ultimo_comparador = "!=";
      }
      ;

		
expresion:
      termino
      {
      }
      |expresion OP_RESTA termino
      {
            arbolTermino = desapilar();
            arbolExpresion = desapilar();
            arbolExpresion = (*crear_nodo("-", &arbolExpresion, &arbolTermino));
            apilar(&arbolExpresion);
      }
      |expresion OP_SUMA termino
      {
            arbolTermino = desapilar();
            arbolExpresion = desapilar();
            arbolExpresion = (*crear_nodo("+", &arbolExpresion, &arbolTermino));
            apilar(&arbolExpresion);
      }
      ;

termino: 
      factor
      {
      }
      |termino
      {
      }
      OP_MUL factor
      {
            t_arbol factor = desapilar();
            t_arbol termino = desapilar();
            arbolTermino = (*crear_nodo("*", &termino, &factor));
            apilar(&arbolTermino);
      }
      |termino
      {
      }
      OP_DIV factor
      {
            t_arbol factor = desapilar();
            t_arbol termino = desapilar();
            arbolTermino = (*crear_nodo("*", &termino, &factor));
            apilar(&arbolTermino);
      }
      ;

factor: 
      ID
      {
            if(!existe_simbolo($<str_val>1)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>1);
                  yyerror();
            }
            ultima_expresion = simbolo_busqueda.tipo_dato;
            arbolFactor = (*crear_hoja($<str_val>1));
            apilar(&arbolFactor);
      }
      |ENTERO 
      {
            guardar_cte_int($<int_val>1);
            ultima_expresion = "int";
            char aux[100];
            sprintf(aux,"%d",$<int_val>1);
            arbolFactor = (*crear_hoja(aux));
            apilar(&arbolFactor);
      }
      |REAL
      {
            float valor = $<real_val>1;
            guardar_cte_float(valor);
            ultima_expresion = "float";  
            char aux[100];
            sprintf(aux,"%f",$<real_val>1);
            arbolFactor = (*crear_hoja(aux));
            apilar(&arbolFactor);
      }
      |P_A expresion P_C
      {
            arbolFactor = arbolExpresion;
            apilar(&arbolFactor);
      }
      ;

%%
int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL){
	  printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else{
    initArray(&array_nombres_variables);
    crearTabla();
    yyparse();
    guardar_ts();
    freeArray(&array_nombres_variables);
  }
  fclose(yyin);
  return 0;
}
int yyerror(void){
  printf("Syntax Error\n");
  system ("Pause");
  exit (1);
}


