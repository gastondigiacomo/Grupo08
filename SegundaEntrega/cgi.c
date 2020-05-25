#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>


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


typedef c_nodo* t_cola;

typedef struct s_nodo_pila{
    t_arbol dato;
    struct s_nodo_pila *sig;
}p_nodo;


typedef p_nodo* t_pila;


t_pila pila;
t_pila pilaBloque;

t_arbol* crear_hoja(char* hoja);
t_arbol* crear_nodo(char* raiz, t_arbol* hojaIzq, t_arbol* hojaDer);
void vaciar_cola(t_cola *p);
void crear_cola(t_cola *p);
void acolar(t_arbol* nodo);
void guardarArbolIRD(t_arbol* pa,FILE* pf);
t_arbol negarComparacion(t_arbol * a);
t_arbol desapilarBloque();
void apilarBloque(t_arbol* nodo);
void vaciarPila(t_pila *p);
void crearPila(t_pila *p);
void apilar(t_arbol* nodo);

t_arbol desacolar();
t_cola cola;
t_cola fin_cola;

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

int cant_pila_polaca = 0;
int cant_pila_aux = 0;

char* pila_polaca[5000];
int pila_auxiliar[100];


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
    printf("------crearNodo : %s\n", nodo->dato);
	if(hojaIzq){
		nodo->izq = *hojaIzq;
        printf("izq nodo: %s\n",(nodo->izq)->dato);
        printf("izq nodo izq: %s\n",(nodo->izq)->izq->dato);
        printf("izq nodo der: %s\n",(nodo->izq)->der->dato);
	}else{
		nodo->izq = NULL;
	}
	if(hojaDer){
		nodo->der = *hojaDer;
        printf("der nodo: %s\n",(nodo->der)->dato);
        printf("der nodo izq: %s\n",(nodo->der)->izq->dato);
        printf("der nodo der: %s\n",(nodo->der)->der->dato);
	}else{
		nodo->der = NULL;
	}
	
    aux = &nodo;
	return  aux;
}



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


void guardarArbolIRD(t_arbol* pa,FILE* pf)
{
    
    if(!(*pa)){
        return; 
    }
    guardarArbolIRD(&(*pa)->izq,pf);
    fprintf(pf,"%-33s\n",(*pa)->dato);

    guardarArbolIRD(&(*pa)->der,pf);
}


void apilar(t_arbol* d)
{
    p_nodo* nue=(p_nodo*)malloc(sizeof(p_nodo));

    // printf("DATO APILADO %s\n", (*d)->dato);
    nue->dato =(t_arbol) (*d);
    nue->sig = pila;
    pila = nue;
    // printf("DATO APILADO %s\n", (*d)->dato);
    // printf("DATO APILADO IZQ %s\n", (*d)->izq->dato);
    // printf("DATO APILADO DER %s\n", (*d)->der->dato);
}

t_arbol desapilar()
{
    p_nodo *aux;
	t_arbol auxDato;
	
    aux=pila;
    pila=aux->sig;
    auxDato = aux->dato;
	free(aux);
    // printf("DATO DESAPILADO %s\n", (auxDato)->dato);
    // printf("DATO DESAPILADO IZQ %s\n", (auxDato)->izq->dato);
    // printf("DATO DESAPILADO DER %s\n", (auxDato)->der->dato);
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

t_arbol desapilarBloque(){
	p_nodo *aux;
	t_arbol auxDato;
	
    aux=pilaBloque;
    pilaBloque=aux->sig;
    auxDato = aux->dato;
    // printf("DATO DESAPILADO %s\n", (auxDato)->dato);
    // printf("DATO DESAPILADO IZQ %s\n", (auxDato)->izq->dato);
    // printf("DATO DESAPILADO DER %s\n", (auxDato)->der->dato);
	free(aux);
    return auxDato;
}
void apilarBloque(t_arbol* nodo){
	p_nodo* nue=(p_nodo*)malloc(sizeof(p_nodo));

    nue->dato =(t_arbol) (*nodo);
    nue->sig = pilaBloque;
    pilaBloque = nue;
}