#include "funciones.c"
#include "cgi.c"

#define NO_OPERADOR                     99
#define ES_SUMA                         100
#define ES_RESTA                        101
#define ES_DIVISION                     102
#define ES_MULTIPLICACION               103
#define ES_ASIGNACION                   104
#define NO_ES_COMPARADOR                199
#define ES_COMP_IGUAL                   200
#define ES_COMP_DISTINTO                201
#define ES_COMP_MAYOR                   202
#define ES_COMP_MAYOR_IGUAL             203
#define ES_COMP_MENOR                   204
#define ES_COMP_MENOR_IGUAL             205
#define NO_ES_ITERACION                 299
#define ES_ITER_IF                      300
#define ES_ITER_WHILE                   301
#define NO_ES_CUERPO                    399
#define ES_CUERPO                       400
#define NOMBRE_ARCHIVO_AUX_ASM          "auxiliar.asm"
#define NOMBRE_ARCHIVO_ASM              "final.asm"
#define NOMBRE_ETIQUETA_COMPARACION     "ET_"
#define NOMBRE_ETIQUETA_ITERACION       "X_"
#define ES_SIMBOLO_ERRONEO              499
#define ES_SIMBOLO_ENTERO               500
#define ES_SIMBOLO_FLOAT                501
#define ES_SIMBOLO_STRING               502
#define BUFF_MAX                        256
#define NOMBRE_VARIABLES_AUX            "@aux_"
#define NO_ES_IO                        599
#define ES_DISPLAY                      600
#define ES_GET                          601



// VARIABLES GLOBALES
// ********************************************************************************************************
FILE* archivo_auxiliar_asm;
FILE* archivo_final_asm;
int contador_variables_aux = 1;
int contador_etiqueta_cmp = 0;
int contador_etiqueta_iter = 0;
char primer_etiqueta[100];
// ********************************************************************************************************
void generar_assembler();
void imprimir_header_assembler();
void imprimir_footer_assembler();
void imprimir_data_assembler();
void imprimir_code_assembler();
char* obtener_nombre_simbolo_ts(int posicion);
char* obtener_valor_ts(int posicion, char* tipo_dato);
char* obtener_tipo_simbolo_asm(char* tipo_dato);
char* obtener_tipo_simbolo_ts(int posicion);
int es_simbolo_constante(int posicion);
void agregar_aux_a_data();
void resolver_arbol(t_arbol root);
void escribir_if_asm();
void escribir_cuerpo_asm();
void escribir_while_asm();
void escribir_comparacion_asm(char* izq, char* der, int comparador);
char* obtener_salto(int comparador);
char* escribir_suma_asm(char* izq, char* der);
char* escribir_resta_asm(char* izq, char* der);
char* escribir_multiplicacion_asm(char* izq, char* der);
char* escribir_division_asm(char* izq, char* der);
void escribir_display_asm(char* info_nodo);
void escribir_get_asm(char* info_nodo);
int obtener_tipo_dato_ts(char* info_nodo);
int es_cuerpo(char* info_nodo);
int es_operador(char* info_nodo);
int es_iteracion(char* info_nodo);
int es_comparador(char* info_nodo);
int es_io(char* info_nodo);
void escribir_asignacion_asm(char* izq, char* der);
void escribir_asignacion_string_asm(char* izq, char* der);

void generar_assembler() {
    archivo_final_asm = fopen(NOMBRE_ARCHIVO_ASM, "w");
    sprintf(primer_etiqueta, "%s","START");

    imprimir_header_assembler();
    imprimir_data_assembler();
    imprimir_code_assembler();

    resolver_arbol(arbolPrograma);

    imprimir_footer_assembler();

    fclose(archivo_final_asm);
    agregar_aux_a_data();
}

void agregar_aux_a_data() {
    int flag = 0;
    char buffer[BUFF_MAX];
    char var_name[BUFF_MAX];
    FILE* archivo_auxiliar_asm = fopen(NOMBRE_ARCHIVO_AUX_ASM, "w");
    FILE* archivo_final_asm = fopen(NOMBRE_ARCHIVO_ASM, "r");
    int i = 0;
    // Lee el archivo
    while(fgets(buffer, BUFF_MAX, archivo_final_asm) != NULL) {

        if(strcmp(buffer, ".DATA\n") == 0 ) {

            flag = 1;
        }
       
        fprintf(archivo_auxiliar_asm, "%s", buffer);
        fflush(archivo_auxiliar_asm);

        if(flag) {
            for(i; i < contador_variables_aux; i++){
                sprintf(var_name, "%s%d", NOMBRE_VARIABLES_AUX, i);
                fprintf(archivo_auxiliar_asm, "\t%-15s \t %s \t\t\t %s\n", var_name, "dd", "?");
                fflush(archivo_auxiliar_asm);
            }
            fprintf(archivo_auxiliar_asm, "%s", "\n");
            flag = 0;
        }
    }

    fclose(archivo_auxiliar_asm);
    fclose(archivo_final_asm);

    archivo_auxiliar_asm = fopen(NOMBRE_ARCHIVO_AUX_ASM, "r");
    archivo_final_asm = fopen(NOMBRE_ARCHIVO_ASM, "w");    

    // Lee el archivo
    while(fgets(buffer, BUFF_MAX, archivo_auxiliar_asm) != NULL) {
        fprintf(archivo_final_asm, "%s", buffer);
    }
    fflush(archivo_final_asm);
    fclose(archivo_auxiliar_asm);
    fclose(archivo_final_asm);
}

void imprimir_code_assembler() {
    fprintf(archivo_final_asm, ".CODE\n");
	fprintf(archivo_final_asm, "%s:\n",primer_etiqueta);
	fprintf(archivo_final_asm, "MOV EAX, @DATA\n");
	fprintf(archivo_final_asm, "MOV DS, EAX\n");
    fprintf(archivo_final_asm, "MOV ES, EAX\n\n");
}

void imprimir_data_assembler() {
    fprintf(archivo_final_asm, "\n%s\n",".DATA");
    fprintf(archivo_final_asm, "\t%-15s \t %s \t\t\t %s\n", "NEW_LINE", "db", "0AH,0DH,'$'");
    if(existe_between){
        fprintf(archivo_final_asm, "\t%-15s \t %s \t\t\t %s\n", "@ID_AUX", "dd", "?");
    }
    int i = 0;
    char* tipo_dato;
    int es_constante;
    char* valor;
    char* nombre;
    char* tipo_dato_asm;
    for( i ; i <= cant_elem_ts ; i++ ){
        tipo_dato = obtener_tipo_simbolo_ts(i);
        tipo_dato_asm = obtener_tipo_simbolo_asm(tipo_dato);
        es_constante = es_simbolo_constante(i);
        valor = es_constante ? obtener_valor_ts(i, tipo_dato) : "?";
        nombre = obtener_nombre_simbolo_ts(i);
        if(!es_constante){
            tipo_dato_asm = "dd";
        }
        fprintf(archivo_final_asm, "\t%-15s \t %s \t\t\t %s\n", nombre, tipo_dato_asm, valor);
    }
}

char* obtener_nombre_simbolo_ts(int posicion) {
    return ts[posicion].nombre;
}

char* obtener_valor_ts(int posicion, char* tipo_dato) {
    char* return_value = malloc(sizeof(char[100])+1);
    if(strcmp(tipo_dato, "string") == 0) {
        sprintf(return_value,"%s,10,13,'$'", ts[posicion].valor);
    }
    else if(strcmp(tipo_dato, "int") == 0) {
        sprintf(return_value,"%s.0",ts[posicion].valor);
    }
    else {
        sprintf(return_value,ts[posicion].valor);
    }
    return return_value;
}

char* obtener_tipo_simbolo_ts(int posicion) {
    return  ts[posicion].tipo_dato;
}

char* obtener_tipo_simbolo_asm(char* tipo_dato) {
    char* return_value = malloc(sizeof(char[100])+1);
    if(strcmp(tipo_dato,"int") == 0 || strcmp(tipo_dato,"float") == 0) {
        sprintf(return_value,"dd");
    } 
    else if(strcmp(tipo_dato,"string") == 0) {
        sprintf(return_value,"db");
    }
    return return_value;
}

int es_simbolo_constante(int posicion) {
    return (strcmp(ts[posicion].valor,"-") != 0);
}

void imprimir_footer_assembler() {
    fprintf(archivo_final_asm, "\n\t%s\n","mov ah, 1 ; pausa, espera que oprima una tecla");
    fprintf(archivo_final_asm, "\t%s\n","int 21h ; AH=1 es el servicio de lectura");
    fprintf(archivo_final_asm, "\t%s\n","MOV AX, 4C00h ; Sale del Dos");
    fprintf(archivo_final_asm, "\t%s\n","INT 21h       ; Enviamos la interrupcion 21h");
    fprintf(archivo_final_asm, "\n%s\n","END START");
}

void imprimir_header_assembler() {
    fprintf(archivo_final_asm, "include macros2.asm\n");
	fprintf(archivo_final_asm, "include number.asm\n");
	fprintf(archivo_final_asm, ".MODEL LARGE\n\n");
	fprintf(archivo_final_asm, ".386\n");
	fprintf(archivo_final_asm, ".STACK 200h\n\n");
}
//*******************************************************************************************************
void resolver_arbol(t_arbol root) { 

    if (root == NULL) 
        return;

    if(!root->izq)
        return;

    int valor_switch;
    // A EVALUAR ANTES DE RECORRER CUALQUIERA DE LOS HIJOS
    //*************************************************************************************************************
    
    //*************************************************************************************************************
    resolver_arbol(root->izq); // LLAMO AL HIJO IZQUIERDO
    
    // A EVALUAR CUANDO TERMINO DE RECORRER EL HIJO IZQUIERDO
    //*************************************************************************************************************

    if((valor_switch = es_cuerpo(root->dato)) != NO_ES_CUERPO){
        escribir_cuerpo_asm();
    }

    //*************************************************************************************************************
    resolver_arbol(root->der); // LLAMO AL HIJO DERECHO

    // A EVALUAR CUANDO YA RECORRIO LOS DOS HIJOS
    //*************************************************************************************************************    
    
    if((valor_switch = es_operador(root->dato)) != NO_OPERADOR){
        char* variable_auxiliar = malloc(sizeof(char[100])+1);
        t_arbol hoja_derecha_modificada;
        int tipo_dato;
        switch (valor_switch)
        {
        case ES_SUMA:
            sprintf(variable_auxiliar,"%s",escribir_suma_asm(root->izq->dato, root->der->dato));
            (*root) = *(*crear_hoja(variable_auxiliar));
            break;
        case ES_RESTA:
            sprintf(variable_auxiliar,"%s",escribir_resta_asm(root->izq->dato, root->der->dato));
            (*root) = *(*crear_hoja(variable_auxiliar));
            break;
        case ES_MULTIPLICACION:
            sprintf(variable_auxiliar,"%s",escribir_multiplicacion_asm(root->izq->dato, root->der->dato));
            (*root) = *(*crear_hoja(variable_auxiliar));
            break;
        case ES_DIVISION:
            sprintf(variable_auxiliar,"%s",escribir_division_asm(root->izq->dato, root->der->dato));
            (*root) = *(*crear_hoja(variable_auxiliar));
            break;         
        case ES_ASIGNACION:
            tipo_dato = obtener_tipo_dato_ts(root->izq->dato);
            if(tipo_dato == ES_SIMBOLO_STRING) {
                printf("%s = %s\n", root->izq->dato, root->der->dato);
                escribir_asignacion_string_asm(root->izq->dato, root->der->dato);
            } else {
                escribir_asignacion_asm(root->izq->dato, root->der->dato);
            }            
            break;                  
        default:
            break;
        }
    } 
    else if((valor_switch = es_comparador(root->dato)) != NO_ES_COMPARADOR){
        escribir_comparacion_asm(root->izq->dato, root->der->dato, valor_switch);
    } 
    else if((valor_switch = es_iteracion(root->dato)) != NO_ES_ITERACION){
        switch (valor_switch){
            case ES_ITER_IF:
                escribir_if_asm();
                break;
            case ES_ITER_WHILE:
                escribir_while_asm();
                break;
            default:
                break;
        }
    } else if((valor_switch = es_io(root->dato)) != NO_ES_IO){
        switch (valor_switch) {
            case ES_DISPLAY:
                escribir_display_asm(root->izq->dato);
                break;
            case ES_GET:
                escribir_get_asm(root->izq->dato);
                break;
            default:
                break;
        }
    }
    //*************************************************************************************************************
}

void escribir_asignacion_string_asm(char* izq, char* der) {
    fprintf(archivo_final_asm, "\tLEA EAX, %s\n", der);
    fprintf(archivo_final_asm, "\tMOV %s, EAX\n", izq);
}

void escribir_display_asm(char* info_nodo) {
    int valor_switch;
    if( (valor_switch = obtener_tipo_dato_ts(info_nodo)) != ES_SIMBOLO_ERRONEO) {
        switch (valor_switch) {
            case ES_SIMBOLO_ENTERO:
                fprintf(archivo_final_asm,"\n\tDisplayFloat %s,0\n", info_nodo);
                break;
            case ES_SIMBOLO_FLOAT:
                fprintf(archivo_final_asm,"\n\tDisplayFloat %s,2\n", info_nodo);
                break;
            case ES_SIMBOLO_STRING:
                fprintf(archivo_final_asm,"\n\tDisplayString %s\n", info_nodo);
                break;
            default:
                break;
        }
        fprintf(archivo_final_asm,"\tDisplayString NEW_LINE\n");
    }
}

void escribir_get_asm(char* info_nodo) {
    int valor_switch;
    if( (valor_switch = obtener_tipo_dato_ts(info_nodo)) != ES_SIMBOLO_ERRONEO) {
        switch (valor_switch) {
            case ES_SIMBOLO_ENTERO:
                fprintf(archivo_final_asm,"\n\tGetFloat %s\n", info_nodo);
                break;
            case ES_SIMBOLO_FLOAT:
                fprintf(archivo_final_asm,"\n\tGetFloat %s\n", info_nodo);
                break;
            case ES_SIMBOLO_STRING:
                fprintf(archivo_final_asm,"\n\tGetString %s\n", info_nodo);
                break;
            default:
                break;
        }
    }
}

int obtener_tipo_dato_ts(char* info_nodo) {
    int i = 0;
    for( i ; i < cant_elem_ts ; i++ ){
        if( strcmp(info_nodo, ts[i].nombre) == 0 ){
            if(strcmp(ts[i].tipo_dato, "int") == 0)
                return ES_SIMBOLO_ENTERO;
            else if(strcmp(ts[i].tipo_dato, "float") == 0)
                return ES_SIMBOLO_FLOAT;
            else if(strcmp(ts[i].tipo_dato, "string") == 0)
                return ES_SIMBOLO_STRING;
            else
                return ES_SIMBOLO_ERRONEO;
        }
    }
    return ES_SIMBOLO_ERRONEO;
}

int es_io(char* info_nodo) {
    if(strcmp(info_nodo, "DISPLAY") == 0)
        return ES_DISPLAY;
    else if(strcmp(info_nodo, "GET") == 0)
        return ES_GET;
    else
        return NO_ES_IO;
}

void escribir_asignacion_asm(char* izq, char* der) {
    fprintf(archivo_final_asm,"\tFLD %s\n",der);
    fprintf(archivo_final_asm,"\tfstp %s\n",izq);
    fprintf(archivo_final_asm,"\t%s\n","ffree");
}

void escribir_if_asm() {
    desapilar_comparacion_asm();
    char* etiqueta_iteracion = desapilar_iteracion_asm();
    fprintf(archivo_final_asm,"%s:\n",etiqueta_iteracion);
}

void escribir_cuerpo_asm() {
    char* etiqueta_iteracion = desapilar_iteracion_asm();

    char etiqueta_iteracion_nueva[100];
    sprintf(etiqueta_iteracion_nueva, "%s%d", NOMBRE_ETIQUETA_ITERACION, contador_etiqueta_iter);
    apilar_itearacion_asm(etiqueta_iteracion_nueva);
    contador_etiqueta_iter++;

    fprintf(archivo_final_asm,"\tJMP %s\n",etiqueta_iteracion_nueva);
    fprintf(archivo_final_asm,"%s:\n",etiqueta_iteracion);
}

void escribir_while_asm() {
    char* etiqueta_comparacion = desapilar_comparacion_asm();
    char* etiqueta_iteracion = desapilar_iteracion_asm();
    fprintf(archivo_final_asm,"\tJMP %s\n", etiqueta_comparacion);
    fprintf(archivo_final_asm,"%s:\n",etiqueta_iteracion);
}


void escribir_comparacion_asm(char* izq, char* der, int comparador) {
    char* tipo_salto = obtener_salto(comparador);
    char etiqueta_comparacion[100];
    sprintf(etiqueta_comparacion, "%s%d", NOMBRE_ETIQUETA_COMPARACION, contador_etiqueta_cmp);
    apilar_comparacion_asm(etiqueta_comparacion);
    contador_etiqueta_cmp++;

    char etiqueta_iteracion[100];
    sprintf(etiqueta_iteracion, "%s%d", NOMBRE_ETIQUETA_ITERACION, contador_etiqueta_iter);
    apilar_itearacion_asm(etiqueta_iteracion);
    contador_etiqueta_iter++;

    
    fprintf(archivo_final_asm,"%s:\n",etiqueta_comparacion);
    fprintf(archivo_final_asm,"\tFLD %s\n",izq);
    fprintf(archivo_final_asm,"\tFCOMP %s\n",der);
    fprintf(archivo_final_asm,"\t%s\n","fstsw ax");
    fprintf(archivo_final_asm,"\t%s\n","sahf");
    fprintf(archivo_final_asm,"\t%s %s\n",tipo_salto, etiqueta_iteracion);
}

char* obtener_salto(int comparador) {
    switch (comparador) {
        case ES_COMP_DISTINTO:
            return "JE";
            break;
        case ES_COMP_IGUAL:
            return "JNE";
            break;
        case ES_COMP_MAYOR:
            return "JNA";
            break;
        case ES_COMP_MENOR:
            return "JNB";
            break;
        case ES_COMP_MAYOR_IGUAL:
            return "JNAE";
            break;
        case ES_COMP_MENOR_IGUAL:
            return "JNBE";
            break;
        default:
            break;
    }
}

char* escribir_suma_asm(char* izq, char* der) {
    char* return_value = malloc(sizeof(char[100]));
    fprintf(archivo_final_asm, "");
    fprintf(archivo_final_asm,"\tFLD %s\n",izq);
    fprintf(archivo_final_asm,"\tFLD %s\n",der);
    fprintf(archivo_final_asm,"\t%s\n","FADD");
    sprintf(return_value, "%s%d", NOMBRE_VARIABLES_AUX, contador_variables_aux);
    fprintf(archivo_final_asm,"\tfstp %s\n",return_value);
    fprintf(archivo_final_asm,"\tffree\n");
    contador_variables_aux++;
    return return_value;
}

char* escribir_resta_asm(char* izq, char* der) {
    char* return_value = malloc(sizeof(char[100]));
    fprintf(archivo_final_asm, "");
    fprintf(archivo_final_asm,"\tFLD %s\n",izq);
    fprintf(archivo_final_asm,"\tFLD %s\n",der);
    fprintf(archivo_final_asm,"\t%s\n","FSUB");
    sprintf(return_value, "%s%d", NOMBRE_VARIABLES_AUX , contador_variables_aux);
    fprintf(archivo_final_asm,"\tfstp %s\n",return_value);
    fprintf(archivo_final_asm,"\tffree\n");
    contador_variables_aux++;
    return return_value;
}

char* escribir_multiplicacion_asm(char* izq, char* der) {
    char* return_value = malloc(sizeof(char[100]));
    fprintf(archivo_final_asm, "");
    fprintf(archivo_final_asm,"\tFLD %s\n",izq);
    fprintf(archivo_final_asm,"\tFLD %s\n",der);
    fprintf(archivo_final_asm,"\t%s\n","FMUL");
    sprintf(return_value, "%s%d", NOMBRE_VARIABLES_AUX, contador_variables_aux);
    fprintf(archivo_final_asm,"\tfstp %s\n",return_value);
    fprintf(archivo_final_asm,"\tffree\n");
    contador_variables_aux++;
    return return_value;
}

char* escribir_division_asm(char* izq, char* der) {
    char* return_value = malloc(sizeof(char[100]));
    fprintf(archivo_final_asm, "");
    fprintf(archivo_final_asm,"\tFLD %s\n",izq);
    fprintf(archivo_final_asm,"\tFLD %s\n",der);
    fprintf(archivo_final_asm,"\t%s\n","FDIV");
    sprintf(return_value, "%s%d", NOMBRE_VARIABLES_AUX, contador_variables_aux);
    fprintf(archivo_final_asm,"\tfstp %s\n",return_value);
    fprintf(archivo_final_asm,"\tffree\n");
    contador_variables_aux++;
    return return_value;
}

int es_cuerpo(char* info_nodo) {
    if(strcmp(info_nodo,"CUERPO") == 0)
        return ES_CUERPO;
    else
        return NO_ES_CUERPO;
}

int es_operador(char* info_nodo) {
    if(strcmp(info_nodo,"=") == 0)
        return ES_ASIGNACION;
    if(strcmp(info_nodo,"/") == 0)
        return ES_DIVISION;
    else if(strcmp(info_nodo,"+") == 0)
        return ES_SUMA;
    else if(strcmp(info_nodo,"*") == 0)
        return ES_MULTIPLICACION;
    else if(strcmp(info_nodo,"-") == 0)
        return ES_RESTA;
    else 
        return NO_OPERADOR;
}

int es_iteracion(char* info_nodo) {
    if(strcmp(info_nodo,"IF") == 0)
        return ES_ITER_IF;
    else if(strcmp(info_nodo,"WHILE") == 0)
        return ES_ITER_WHILE;
    else 
        return NO_ES_ITERACION;
}

int es_comparador(char* info_nodo) {
    if(strcmp(info_nodo,"==") == 0)
        return ES_COMP_IGUAL;
    else if(strcmp(info_nodo,"!=") == 0)
        return ES_COMP_DISTINTO;
    else if(strcmp(info_nodo,">") == 0)
        return ES_COMP_MAYOR;
    else if(strcmp(info_nodo,"<") == 0)
        return ES_COMP_MENOR;
    else if(strcmp(info_nodo,">=") == 0)
        return ES_COMP_MAYOR_IGUAL;
    else if(strcmp(info_nodo,"<=") == 0)
        return ES_COMP_MENOR_IGUAL;
    else 
        return NO_ES_COMPARADOR;
}
//*******************************************************************************************************
