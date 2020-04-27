#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>


#define INITIAL_CAPACITY 1
#define MAX_STRING_LENGTH 30
#define TABLA_SIMBOLOS "ts.txt"
#define TAM_TABLA 350
#define TAM_NOMBRE 32
#define TRUE 1
#define FALSE 0
#define SUCCESS 99
#define NOT_SUCCESS -99

//funciones tabla simbolos
void crearTabla();
int crear_lista_variable(char * variable);
void guardar_variables_ts();
int guardar_cte_int(int valor);
int guardar_cte_string(char * valor);
int guardar_cte_float(float valor);
void guardar_ts();
int existe_simbolo(char * comp);
int verificar_asignacion(char * valor);

//funciones complementarias
char* concat(const char *s1, const char *s2);

typedef struct {
	char nombre[TAM_NOMBRE];
	char tipo_dato[TAM_NOMBRE];
	char valor[TAM_NOMBRE];
	int longitud;
} simbolo;

simbolo ts[TAM_TABLA];
simbolo simbolo_busqueda;

FILE * file;

int cant_elem_ts = 0;
int index_array = 0;
int array_size = 0;
char** array_nombres_variables;
char tipo_dato[30];
char * ultima_expresion;


int crear_lista_variable(char * variable){
  *(array_nombres_variables+index_array) = variable;
  if(sizeof(array_nombres_variables) <= index_array){
    char** tmp = realloc(array_nombres_variables, array_size * 2 * sizeof(*tmp));
    if(tmp){
      array_nombres_variables = tmp;
      array_size *= 2;
      return SUCCESS;
    }
    else{
      return NOT_SUCCESS;
    }
  }
  index_array++;
}

void guardar_variables_ts(){
  int i = 0;
  for(i; i<index_array; i++){
    if(cant_elem_ts<=TAM_TABLA && !existe_simbolo(array_nombres_variables[i])){
      strcpy(ts[cant_elem_ts].nombre,array_nombres_variables[i]);
      ts[cant_elem_ts].longitud = 0;
      strcpy(ts[cant_elem_ts].tipo_dato,tipo_dato);
      strcpy(ts[cant_elem_ts].valor,"-");
      cant_elem_ts++;
    }
    else{
      if(cant_elem_ts>TAM_TABLA){
        printf("TABLA DE SIMBOLOS LLENA.\n");
        exit(1);
      }
      else{
        printf("VARIABLE %s DECLARADA PREVIAMENTE\n", array_nombres_variables[i]);
        exit(1);
      }
    }
  }
  index_array=0;
}

void crearTabla(){
  file = fopen(TABLA_SIMBOLOS, "w");
  fprintf(file,"%s\n","NOMBRE|TIPODATO|VALOR|LONGITUD");
  fclose(file);
}

int guardar_cte_int(int valor) {
      char  nombre_variable[] = "_";
      char constante_string[100];
      sprintf(constante_string,"%d",valor);

      char* nombre_constante = concat(nombre_variable, constante_string);
      if(existe_simbolo(nombre_constante) == FALSE && cant_elem_ts <= TAM_TABLA){
        strcpy(ts[cant_elem_ts].nombre,nombre_constante);
        ts[cant_elem_ts].longitud = 0;
        strcpy(ts[cant_elem_ts].tipo_dato,"int");
        strcpy(ts[cant_elem_ts].valor,constante_string);
        cant_elem_ts++;
        free(nombre_constante); 
        return TRUE;
      }
      free(nombre_constante); 
      return FALSE;
}

int guardar_cte_string(char * valor) {
      char nombre_variable[32] = "_";

      //saca las comillas
      char * variable_sin_comillas = malloc(sizeof(char)*100);
      strcpy(variable_sin_comillas, valor);
      variable_sin_comillas++;
      variable_sin_comillas[strlen(variable_sin_comillas)-1] = 0;
      //saca las comillas
      char * nombre_constante = concat(nombre_variable, variable_sin_comillas);

      if(existe_simbolo(nombre_constante) == FALSE && cant_elem_ts <= TAM_TABLA){
        strcpy(ts[cant_elem_ts].nombre,nombre_constante);
        ts[cant_elem_ts].longitud = strlen(variable_sin_comillas);
        strcpy(ts[cant_elem_ts].tipo_dato,"string");
        strcpy(ts[cant_elem_ts].valor,variable_sin_comillas);
        cant_elem_ts++;
        free(nombre_constante); 
        return TRUE;
      }
      free(nombre_constante); 
      return FALSE;
      
}

int guardar_cte_float(float valor) {
      
      float constante = valor;
      char  nombre_variable[] = "_";
      char constante_string[100];
      sprintf(constante_string,"%f",constante);
      char* nombre_constante = concat(nombre_variable, constante_string);
      if(existe_simbolo(nombre_constante) == FALSE && cant_elem_ts <= TAM_TABLA){
        strcpy(ts[cant_elem_ts].nombre,nombre_constante);
        ts[cant_elem_ts].longitud = 0;
        strcpy(ts[cant_elem_ts].tipo_dato,"float");
        strcpy(ts[cant_elem_ts].valor,constante_string);
        cant_elem_ts++;
        free(nombre_constante);
        return TRUE;
      }
      free(nombre_constante); // deallocate the string
      return FALSE;
}

void guardar_ts(){
  int i = 0;
  char longitud[3];
  file = fopen(TABLA_SIMBOLOS,"a");
  for(i;i<cant_elem_ts;i++){
    if(ts[i].longitud == 0){
      strcpy(longitud,"-");
    }
    else{
      sprintf(longitud,"%d",ts[i].longitud);
      strcpy(longitud,longitud);
    }
    fprintf(file,"%s|%s|%s|%s\n",ts[i].nombre,ts[i].tipo_dato,ts[i].valor,longitud);

    //%-35s%-20s%-35s%-5s
  }
  fclose(file);
}

int existe_simbolo(char * comp) {
  char * aux;
  aux = malloc(sizeof(char)*strlen(comp));
  strcpy(aux,comp);
  int i = 0;
  for( i ; i < cant_elem_ts ; i++ ){
    if( strcmp(aux, ts[i].nombre) == 0 ){
      strcpy(simbolo_busqueda.nombre, ts[i].nombre);
      simbolo_busqueda.longitud = ts[i].longitud;
      strcpy(simbolo_busqueda.tipo_dato, ts[i].tipo_dato);
      strcpy(simbolo_busqueda.valor, ts[i].valor);
      free(aux);
      return TRUE;
      }
  }
  free(aux);
  return FALSE;
}

int verificar_asignacion(char * valor) {
  if(!existe_simbolo(valor)){
        return 1;
  } else {
        if(strcmp(ultima_expresion, simbolo_busqueda.tipo_dato) == 0 || (strcmp(simbolo_busqueda.tipo_dato, "float") == 0 && strcmp(ultima_expresion, "int") == 0)){ //float
              return 2;
        }
        else {
              return 3;
        }
  }
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}