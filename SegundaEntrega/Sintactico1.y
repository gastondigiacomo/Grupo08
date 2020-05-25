
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


programa: 
      bloque_declaracion bloque 
      {
            
            int i = 0;
            printf("cantidad_bloques --- %d\n",cantidad_bloques);


            // t_arbol bloque_2 = desapilarBloque();
            // t_arbol bloque_1 = desapilarBloque();
            // arbolBloque = (*crear_nodo("BLOQUE",&bloque_1, &bloque_2));



            // for(i;i<cantidad_bloques-2;i++){
            //       printf("i --- %d\n",i);
            //       t_arbol bloque_1 = desapilarBloque();
            //       arbolBloque = (*crear_nodo("BLOQUE",&bloque_1, &arbolBloque));               
            // }




            FILE * archivo_arbol;
            archivo_arbol = fopen("arbol.txt","wt");
            guardarArbolIRD(&arbolBloque, archivo_arbol);
            printf("COMPILACION CORRECTA\n");
      }
      |bloque
      {
            printf("COMPILACION CORRECTA\n");
            FILE * archivo_arbol;
            archivo_arbol = fopen("arbol.txt","wt");
            guardarArbolIRD(&arbolBloque, archivo_arbol);
      }
      ;

bloque_declaracion:
      DEFVAR declaraciones ENDDEF
      {

      }
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
            //crear nodo auxiliar con NOMBRE AUX para conectar todas las sentencias :DDDD
            // cantidad_bloques++;
            printf("----BLOQUE %s \n", arbolBloque->dato);
            printf("----SENTENCIA %s \n", arbolSentencia->dato);
            arbolBloque = desapilarBloque();
		arbolBloque = (*crear_nodo("BLOQUE",&arbolBloque, &arbolSentencia));
            apilarBloque(&arbolBloque);
      }
      |sentencia
      {
            // cantidad_bloques++;
            printf("----SENTENCIA SOLA %s \n", arbolSentencia->dato);
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
      |between
      {
            arbolSentencia = arbolBetween;
      }
      |salida
      |entrada
      ;

salida:
      DISPLAY ENTERO
      {
            //guardar_cte_int($<int_val>2);
            // printf("DISPLAY %d\n",$<int_val>2);
      }
      |DISPLAY REAL
      {
            // guardar_cte_float($<real_val>2);
            // printf("DISPLAY %f\n",$<real_val>2);
      }
      |DISPLAY CADENA
      {
            //guardar_cte_string($<str_val>2);
            // printf("DISPLAY %s\n",$<str_val>2);
      }
      |DISPLAY ID
      {
            if(!existe_simbolo($<str_val>2)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                  yyerror();
            }
            // printf("DISPLAY %s\n",$<str_val>2);
      }
      ;

entrada:
      GET ID
      {
            if(!existe_simbolo($<str_val>2)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                  yyerror();
            }
            // printf("GET %s\n",$<str_val>2 );
      }
      ;

asignacion: 
      ID ASIG expresion
      {
            
            switch(verificar_asignacion($<str_val>1)){
                  case 1:     printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>1);
                              yyerror();
                              break;
                  case 2:     //printf("ASIGNACION EXITOSA!\n");
                              arbolAsignacion = (*crear_hoja($<str_val>1));
                              arbolAsignacion = (*crear_nodo("=", &arbolAsignacion, &arbolExpresion));
                              break;
                  case 3:     printf("ERROR DE SINTAXIS, ASIGNACION ERRONEA, TIPOS DE DATOS INCORRECTOS.\n"); 
                              printf("USTED ESTA INTENTANDO ASIGNAR UNA CONSTANTE %s A UNA VARIABLE %s \n", ultima_expresion, simbolo_busqueda.tipo_dato);
                              yyerror();
                              break;
            }
      }
      |ID operador ASIG expresion
      {
            arbolAsignacion = (*crear_hoja($<str_val>1));
            arbolAsignacionEspecial = (*crear_nodo(ultimo_operador, &arbolAsignacion,&arbolExpresion));
            arbolAsignacion = (*crear_hoja($<str_val>1));
            arbolAsignacionEspecial = (*crear_nodo("=",&arbolAsignacion,&arbolAsignacionEspecial));
            // printf("ASIGNACION ESPECIAL EXITOSA!\n");
      }
      |ID ASIG CADENA
      {
            guardar_cte_string($<str_val>3);
            ultima_expresion = "string";
            switch(verificar_asignacion($<str_val>1)){
                  case 1:   //printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>1);
                              yyerror();
                              break;
                  case 2:   printf("CONSTANTE STRING: %s\n",$<str_val>3);
                              arbolAsignacion = (*crear_hoja($<str_val>1));
                              arbolAsignacionCadena = (*crear_hoja($<str_val>3));
                              arbolAsignacion = (*crear_nodo("=",&arbolAsignacion,&arbolAsignacionCadena));
                              printf("ASIGNACION EXITOSA!\n");
                              break;
                  case 3:   printf("ERROR DE SINTAXIS, ASIGNACION ERRONEA, TIPOS DE DATOS INCORRECTOS.\n"); 
                              printf("USTED ESTA INTENTANDO ASIGNAR UNA CONSTANTE %s A UNA VARIABLE %s \n", ultima_expresion, simbolo_busqueda.tipo_dato);
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
      ;

decision:
      IF P_A condicion P_C L_A bloque L_C
      {
            if (strcmp(arbolCondicion-> dato, "OR") == 0) {
                  t_arbol cmp_1 = arbolCondicion-> izq;
                  t_arbol cmp_2 = arbolCondicion-> der;
                  
                  arbolDecision = (*crear_nodo("IF", &cmp_2, &arbolBloque));
                  arbolDecision = (*crear_nodo("CUERPO", &arbolBloque, &arbolDecision));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));

            }
            else if (strcmp(arbolCondicion-> dato, "AND") == 0) {
                  t_arbol cmp_1 = arbolCondicion-> izq;
                  t_arbol cmp_2 = arbolCondicion-> der;

                  arbolDecision = (*crear_nodo("IF", &cmp_2, &arbolBloque));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));
            }
            else {
                  arbolDecision = (*crear_nodo("IF",&arbolCondicion,&arbolBloque));
            }
      }
      | IF P_A condicion P_C L_A bloque L_C ELSE L_A bloque L_C
      {
            t_arbol bloque_else = desapilarBloque();
            t_arbol bloque_then = desapilarBloque();

            if (strcmp(arbolCondicion-> dato, "OR") == 0) {
                  
                  t_arbol cmp_1 = arbolCondicion-> izq;
                  t_arbol cmp_2 = arbolCondicion-> der;
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &bloque_else));
                  arbolDecision = (*crear_nodo("IF", &cmp_2, &arbolDecision));
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &arbolDecision));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));
                  
            }
            else if (strcmp(arbolCondicion-> dato, "AND") == 0) {
                  t_arbol cmp_1 = arbolCondicion-> izq;
                  t_arbol cmp_2 = arbolCondicion-> der;

                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &bloque_else));
                  arbolDecision = (*crear_nodo("IF", &cmp_2, &arbolDecision));
                  arbolDecision = (*crear_nodo("CUERPO", &arbolDecision, &bloque_else));
                  arbolDecision = (*crear_nodo("IF", &cmp_1, &arbolDecision));
            }
            else {
                  arbolDecision = (*crear_nodo("CUERPO", &bloque_then, &bloque_else));
                  arbolDecision = (*crear_nodo("IF",&arbolCondicion,&arbolDecision));
            }
      }
      ;

condicion: 
      comparacion OP_L_O comparacion
      {
            t_arbol cmp2 = desapilar();
            t_arbol cmp1 = desapilar();
            arbolCondicion = (*crear_nodo("OR", &cmp1, &cmp2));
      }
      |comparacion OP_L_A comparacion
      {
            t_arbol cmp2 = desapilar();
            t_arbol cmp1 = desapilar();
            arbolCondicion = (*crear_nodo("AND", &cmp1, &cmp2));
      }
      |comparacion
      {
            arbolCondicion = arbolComparacion;
      }
      |OP_L_N comparacion
      {
            arbolCondicion = negarComparacion(&arbolComparacion);
      }
      |between
      {
            arbolComparacion = (*crear_hoja("0"));
            arbolComparacion = (*crear_nodo("!=",&id_aux,&arbolComparacion));
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
            // printf("3.1\n");
            arbolExpresion = arbolTermino;
            apilar(&arbolExpresion);
      }
      |expresion OP_RESTA termino
      {
            // printf("3.2\n");
            // printf("RESTA\n");
            arbolExpresion = (*crear_nodo("-", &arbolExpresion, &arbolTermino));
            apilar(&arbolExpresion);
      }
      |expresion OP_SUMA termino
      {
            // printf("3.3\n");
            // printf("SUMA\n");
            arbolExpresion = (*crear_nodo("+", &arbolExpresion, &arbolTermino));
            apilar(&arbolExpresion);
      }
      ;

termino: 
      factor
      {
            // printf("2.1\n");
            arbolTermino = desapilar();
      }
      |termino
      {
            apilar(&arbolTermino);
      }
      OP_MUL factor
      {
            // printf("2.2.2\n");
            // printf("MULTIPLICACION\n");
            t_arbol factor = desapilar();
            t_arbol termino = desapilar();
            arbolTermino = (*crear_nodo("*", &termino, &factor));
      }
      |termino
      {
            apilar(&arbolTermino);
      }
      OP_DIV factor
      {
            // printf("2.2.2\n");
            // printf("MULTIPLICACION\n");
            t_arbol factor = desapilar();
            t_arbol termino = desapilar();
            arbolTermino = (*crear_nodo("*", &termino, &factor));
      }
      ;

factor: 
      ID
      {
            // printf("1.1\n");
            if(!existe_simbolo($<str_val>1)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>1);
                  yyerror();
            }
            ultima_expresion = simbolo_busqueda.tipo_dato;
            // printf("VARIABLE USADA: %s\n", $<str_val>1);
            arbolFactor = (*crear_hoja($<str_val>1));
            apilar(&arbolFactor);
      }
      |ENTERO 
      {
            // printf("1.2\n");
            guardar_cte_int($<int_val>1);
            ultima_expresion = "int";
            // printf("CONSTANTE ENTERA: %d\n",$<int_val>1);
            char aux[100];
            sprintf(aux,"%d",$<int_val>1);
            arbolFactor = (*crear_hoja(aux));
            apilar(&arbolFactor);
      }
      |REAL
      {
            // printf("1.3\n");
            float valor = $<real_val>1;
            guardar_cte_float(valor);
            ultima_expresion = "float";  
            // printf("CONSTANTE REAL: %f\n",valor);
            char aux[100];
            sprintf(aux,"%f",$<real_val>1);
            arbolFactor = (*crear_hoja(aux));
            apilar(&arbolFactor);
      }
      |P_A expresion P_C
      {
            // printf("1.4\n");
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


