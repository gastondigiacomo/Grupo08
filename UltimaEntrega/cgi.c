#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define COUNT                           10



// ESTRUCTURAS 
// ********************************************************************************************************
typedef struct a_nodo{
    char dato[100];
    struct a_nodo *izq,
                    *der;
}arb_nodo;
typedef arb_nodo* t_arbol;
typedef struct s_nodo_cola{
    t_arbol dato;
    struct s_nodo_cola *sig;
}c_nodo;
typedef struct s_nodo_pila{
    t_arbol dato;
    struct s_nodo_pila *sig;
}p_nodo;
typedef struct s_nodo_pila_ASM{
    char dato[100];
    struct s_nodo_pila_ASM * sig;
}p_nodo_ASM;
typedef struct s_nodo_pila_etiquetas{
    char dato[100];
    struct s_nodo_pila_etiquetas * sig;
}p_nodo_pila_etiqueta;

typedef c_nodo* t_cola;

typedef p_nodo* t_pila;

typedef p_nodo_pila_etiqueta* t_pila_etiqueta;

typedef p_nodo_ASM* t_pila_ASM;

t_pila_etiqueta pila_etiqueta;

t_pila_ASM pila_comparacion_asm;
t_pila_ASM pila_iteracion_asm;

t_pila pila;
t_pila pilaBloque;

t_cola cola;
t_cola fin_cola;
// ********************************************************************************************************

// ********************************************************************************************************



//DECLARACION DE FUNCIONES
// ********************************************************************************************************
t_arbol* crear_hoja(char* hoja);
t_arbol* crear_nodo(char* raiz, t_arbol* hojaIzq, t_arbol* hojaDer);

void vaciar_cola(t_cola *p);
void crear_cola(t_cola *p);
void acolar(t_arbol* nodo);
t_arbol desacolar();

void guardarArbolIRD(t_arbol* pa,FILE* pf);

t_arbol negarComparacion(t_arbol * a);

t_arbol desapilarBloque();
void apilarBloque(t_arbol* nodo);

void vaciarPila(t_pila *p);
void crearPila(t_pila *p);
void apilar(t_arbol* nodo);

void print2DUtil(t_arbol root, int space);
void print2D(t_arbol root);


void vaciar_pila_asm(t_pila_ASM *p);

char* desapilar_comparacion_asm();
void apilar_comparacion_asm(char* string);

int esPilaVacia(t_pila_ASM *p);
char* ver_tope_asm(t_pila_ASM *p);


void apilar_itearacion_asm(char* string);
char* desapilar_iteracion_asm();


// ********************************************************************************************************

// PUNTEROS PARA CREAR LA INTERMEDIA
// ********************************************************************************************************
t_arbol arbolFactor; 
t_arbol arbolExpresion; 
t_arbol arbolTermino;
t_arbol arbolAsignacion;
t_arbol arbolAsignacionCadena;
t_arbol arbolAsignacionEspecial;
t_arbol arbolComparacion;
t_arbol arbolCondicion;
t_arbol arbolDecision;
t_arbol arbolSentencia;
t_arbol arbolBloque;
t_arbol arbolBetween;
t_arbol arbolIteracion;
t_arbol arbolCuerpoVerdadero;
t_arbol arbolCuerpoFalso;
t_arbol arbolCuerpoOR;
t_arbol arbolCuerpoAND;
t_arbol arbolAux;
t_arbol arbolBetween;
t_arbol id_aux;
t_arbol arbolDesapilado;
t_arbol arbolPrograma;
t_arbol arbolDisplay;
t_arbol arbolGet;
// ********************************************************************************************************

// ********************************************************************************************************
//*******************************************************************************************************

char* desapilar_iteracion_asm(){
    p_nodo_ASM * aux;
    char* auxDato;
    auxDato = malloc(sizeof(strlen(aux->dato))+1);
    aux = pila_iteracion_asm;
    pila_iteracion_asm = aux->sig;
    strcpy(auxDato, aux->dato);
    free(aux);
    return auxDato;
}


char* ver_tope_asm(t_pila_ASM* p){
    char* auxDato;
    auxDato = malloc(sizeof(strlen(pila_iteracion_asm->dato))+1);
    strcpy(auxDato, (*p)->dato);
    return auxDato;
}
void apilar_itearacion_asm(char* string) {
    p_nodo_ASM* nue = (p_nodo_ASM*) malloc(sizeof(p_nodo_ASM));
    strcpy(nue->dato,string);
    nue->sig = pila_iteracion_asm;
    pila_iteracion_asm = nue;
}

int esPilaVacia(t_pila_ASM *p){
    if(*p){
        return 0;
    }
    return 1;
}
//*******************************************************************************************************

//*******************************************************************************************************
char* desapilar_comparacion_asm(){
    p_nodo_ASM * aux;
    char* auxDato;
    auxDato = malloc(sizeof(strlen(aux->dato))+1);


    aux = pila_comparacion_asm;
    pila_comparacion_asm = aux->sig;
    strcpy(auxDato, aux->dato);
    free(aux);
    return auxDato;
}

void vaciar_pila_asm(t_pila_ASM *p){
    p_nodo_ASM* aux;
    while(*p){
        aux = *p;
        *p = aux->sig;
        free(aux);
    }
}

void apilar_comparacion_asm(char* string) {
    p_nodo_ASM* nue = (p_nodo_ASM*) malloc(sizeof(p_nodo_ASM));
    strcpy(nue->dato,string);
    nue->sig = pila_comparacion_asm;
    pila_comparacion_asm = nue;
}
//*******************************************************************************************************

//*******************************************************************************************************
t_arbol* crear_hoja(char* raiz){
	// printf("----crearHoja : %s\n", raiz);
	t_arbol hoja = (arb_nodo*) malloc(sizeof(arb_nodo));	
	t_arbol* aux;
	if(hoja == 0){
	 	puts("memoria llena");
        return NULL;
    }
    if(raiz != NULL) {
        strcpy(hoja->dato,raiz);
    }
	else
    {
        strcpy(hoja->dato,"");
    }
	hoja->izq=NULL;
    hoja->der=NULL;
    aux= &hoja;
    
	return  aux;
}

t_arbol* crear_nodo(char* raiz, t_arbol* hojaIzq, t_arbol* hojaDer){
	t_arbol nodo = (arb_nodo*) malloc(sizeof(arb_nodo));
	t_arbol* aux;
	
	 if(!nodo){
	 	puts("memoria llena");
        return NULL;
    }

	strcpy(nodo->dato,raiz);
	if(hojaIzq){
		nodo->izq = *hojaIzq;
	}else{
		nodo->izq = NULL;
	}
	if(hojaDer){
		nodo->der = *hojaDer;
	}else{
		nodo->der = NULL;
	}
	
    aux = &nodo;
	return  aux;
}
//*******************************************************************************************************

//*******************************************************************************************************
void acolar(t_arbol* d)
{
    c_nodo* nue = (c_nodo*)malloc(sizeof(c_nodo));
    nue->dato = (t_arbol) (*d);
    nue->sig = NULL;
    if(cola == NULL) {
        cola = nue;
        fin_cola = nue;
    }
    else{
        fin_cola->sig = nue;
        fin_cola = nue;
    }
}

t_arbol desacolar()
{
    if(cola != NULL) {
        t_arbol auxDato = cola->dato;
        c_nodo *aux = cola;

        if (cola == fin_cola) {
            cola = NULL;
            fin_cola = NULL;
        }
        else {
            cola = cola->sig;
        }
        free(aux);
        return auxDato;
    }
    else {
        return NULL;
    }
}

void vaciar_cola(t_cola *p)
{
    c_nodo* aux;

    while(*p)
    {
        aux=*p;
        *p=aux->sig;
        free(aux);
    }
}

void crear_cola(t_cola *p)
{
    *p=NULL;
}
//*******************************************************************************************************

//*******************************************************************************************************
void guardarArbolIRD(t_arbol* pa,FILE* pf)
{
    if(!(*pa)){
        return; 
    }
    guardarArbolIRD(&(*pa)->izq,pf);
    fprintf(pf,"%-33s\n",(*pa)->dato);
    guardarArbolIRD(&(*pa)->der,pf);
}
//*******************************************************************************************************

//*******************************************************************************************************
void apilar(t_arbol* d)
{
    p_nodo* nue=(p_nodo*)malloc(sizeof(p_nodo));
    nue->dato =(t_arbol) (*d);
    nue->sig = pila;
    pila = nue;
}

t_arbol desapilar()
{
    p_nodo *aux;
	t_arbol auxDato;
    aux=pila;
    pila=aux->sig;
    auxDato = aux->dato;
	free(aux);
    return auxDato;
}

void vaciarPila(t_pila *p)
{
    p_nodo* aux;

    while(*p)
    {
        aux=*p;
        *p=aux->sig;
        free(aux);
    }
}
//*******************************************************************************************************

//*******************************************************************************************************
t_arbol negarComparacion(t_arbol * a) {
    t_arbol arbolAux;
    char* simboloNegado;
    if(strcmp((*a)->dato,">") == 0){
        simboloNegado = "<";
    }
    else if(strcmp((*a)->dato,"<") == 0){
        simboloNegado = ">";
    }
    else if(strcmp((*a)->dato,">=") == 0){
        simboloNegado = "<=";
    }
    else if(strcmp((*a)->dato,"<=") == 0){
        simboloNegado = ">=";
    }
    else if(strcmp((*a)->dato,"==") == 0){
        simboloNegado = "!=";
    }
    else if(strcmp((*a)->dato,"!=") == 0){
        simboloNegado = "==";
    }
    arbolAux = (*crear_nodo(simboloNegado, &(*a)->izq,&(*a)->der));
    return arbolAux;
}
//*******************************************************************************************************

//*******************************************************************************************************
t_arbol desapilarBloque() {
	p_nodo *aux;
	t_arbol auxDato;
    aux=pilaBloque;
    pilaBloque=aux->sig;
    auxDato = aux->dato;
	free(aux);
    return auxDato;
}

void apilarBloque(t_arbol* nodo){
	p_nodo* nue=(p_nodo*)malloc(sizeof(p_nodo));

    nue->dato =(t_arbol) (*nodo);
    nue->sig = pilaBloque;
    pilaBloque = nue;
}
//*******************************************************************************************************

//*******************************************************************************************************
void print2DUtil(t_arbol root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
    // Increase distance between levels 
    space += COUNT; 
    // Process der child first 
    print2DUtil(root->der, space); 
    // Print current node after space 
    // count 
    printf("\n"); 
    int i = COUNT;
    for (i; i < space; i++) 
        printf(" "); 
    printf("%s\n", root->dato); 
    // Process izq child 
    print2DUtil(root->izq, space); 
} 
  
// Wrapper over print2DUtil() 
void print2D(t_arbol root) 
{ 
   // Pass initial space count as 0 
   print2DUtil(root, 0); 
} 
//*******************************************************************************************************