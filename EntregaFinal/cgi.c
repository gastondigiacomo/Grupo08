#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define COUNT 10

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

int numeroVariables = 1;
int anteriorAuxFlag = 0;
int numeroEtiqueta = 1;
char ultimoJump[100];

// NUEVAS VARIABLES
//******************************
int primerEtiqueta = 1;
int jumpFlag = 0;
int whileFlag = 0;
int numeroEtiquetaSaved = 0;
int returnWhile = 0;
int dobleWhile = 0;
typedef struct s_nodo_pila_etiquetas{
    char dato[100];
    struct s_nodo_pila_etiquetas * sig;
}p_nodo_pila_etiqueta;
typedef p_nodo_pila_etiqueta* t_pila_etiqueta;
t_pila_etiqueta pila_etiqueta;


void apilarEtiqueta(char* string);
char* desapilarEtiqueta();
char* verTopeEtiqueta();
//******************************

typedef struct s_nodo_pila_ASM{
    char dato[100];
    struct s_nodo_pila_ASM * sig;
}p_nodo_ASM;

typedef p_nodo_ASM* t_pila_ASM;

t_pila_ASM pila_ASM;
t_pila_ASM pila_ASM_while;

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
void print2DUtil(t_arbol root, int space);
void print2D(t_arbol root);
t_arbol desacolar();
t_cola cola;
t_cola fin_cola;

void imprimirAssembler(t_arbol root);
int recorrerArbol(t_arbol root);


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

int cant_pila_polaca = 0;
int cant_pila_aux = 0;

char* pila_polaca[5000];
int pila_auxiliar[100];

char* desapilarASM();
void vaciarPilaASM(t_pila_ASM *p);
void apilarASM(char* string);
int esPilaVacia(t_pila_ASM *p);
void apilarASM_while(char* string);
char* desapilarASM_while();


//*******************************************************************************************************
char* desapilarEtiqueta(){
    p_nodo_pila_etiqueta * aux;
    char* auxDato;
    auxDato = malloc(sizeof(strlen(aux->dato))+1);
    
    aux = pila_etiqueta;
    pila_etiqueta = aux->sig;
    strcpy(auxDato, aux->dato);
    free(aux);
    // printf("SE DESAPILO WHILE %s\n", auxDato);
    return auxDato;
}

void apilarEtiqueta(char* string) {
    p_nodo_pila_etiqueta* nue = (p_nodo_pila_etiqueta*) malloc(sizeof(p_nodo_pila_etiqueta));
    strcpy(nue->dato,string);
    nue->sig = pila_etiqueta;
    pila_etiqueta = nue;
    // printf("SE APILO WHILE %s\n", string);
}

char* verTopeEtiqueta() {
    char* auxDato;
    auxDato = malloc(sizeof(char[100])+1);
    strcpy(auxDato, pila_etiqueta->dato);
    return auxDato;
}
//*******************************************************************************************************

char* desapilarASM_while(){
    p_nodo_ASM * aux;
    char* auxDato;
    auxDato = malloc(sizeof(strlen(aux->dato))+1);


    aux = pila_ASM_while;
    pila_ASM_while = aux->sig;
    strcpy(auxDato, aux->dato);
    free(aux);
    // printf("SE DESAPILO WHILE %s\n", auxDato);
    return auxDato;
}

void apilarASM_while(char* string) {
    p_nodo_ASM* nue = (p_nodo_ASM*) malloc(sizeof(p_nodo_ASM));
    strcpy(nue->dato,string);
    nue->sig = pila_ASM_while;
    pila_ASM_while = nue;
    // printf("SE APILO WHILE %s\n", string);
}

int esPilaVacia(t_pila_ASM *p){
    if(*p){
        return 0;
    }
    return 1;
}

char* desapilarASM(){
    p_nodo_ASM * aux;
    char* auxDato;
    auxDato = malloc(sizeof(strlen(aux->dato))+1);


    aux = pila_ASM;
    pila_ASM = aux->sig;
    strcpy(auxDato, aux->dato);
    free(aux);
    // printf("SE DESAPILO %s\n", auxDato);
    return auxDato;
}

void vaciarPilaASM(t_pila_ASM *p){
    p_nodo_ASM* aux;
    while(*p){
        aux = *p;
        *p = aux->sig;
        free(aux);
    }
}

void apilarASM(char* string) {
    p_nodo_ASM* nue = (p_nodo_ASM*) malloc(sizeof(p_nodo_ASM));
    strcpy(nue->dato,string);
    nue->sig = pila_ASM;
    pila_ASM = nue;
    // printf("SE APILO %s\n", string);
}

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





int recorrerArbol(t_arbol root) 
{ 
    // printf("-----RECORRIENDO: %s\n",root->dato);
    // Base case 
    if (root == NULL) 
        return 1; 
    char etiquetaIzq[100];
    char etiquetaDer[100];
    numeroEtiquetaSaved = numeroEtiqueta;
    int numeroEtiquetaSavedLocal = numeroEtiqueta;
    // Increase distance between levels 
    int count = 0;
    int izq = 0;
    int der = 0;
    // LLAMAR A LA FUNCION RECORRERARBOL EN SINTACTICO CON PARAMETROS ET_0 COMO ETIQ y ET_-1 COMO ETIQUEPADRE
    if(primerEtiqueta) {
        primerEtiqueta = 0;
        printf("START:\n");
    }
    // A EVALUAR ANTES DE RECORRER CUALQUIERA DE LOS HIJOS
    //*************************************************************************************************************
    if(jumpFlag) {
        printf("ET_%d:\n",numeroEtiqueta);
        printf("--ET POR %s Y IF FLAG\n", root->dato);
        numeroEtiqueta++;
        jumpFlag = 0;
    } 
    else if(strcmp(root->dato,"WHILE") == 0){ // CREO LA ETIQUETA DEL WHILE PARA DESPUES PODER REALIZAR EL JMP A ELLA  
        if(!whileFlag) { // SOLO LA CREO SI NO HUBO UN WHILE ANTERIOR, XQ SI HUBO LA CREE EN EL NODO BLOQUE PADRE DE ESE WHILE
            printf("ET_%d:\n",numeroEtiqueta);
            printf("--ET POR WHILE Y NO WHILE FLAG\n");
            numeroEtiqueta++;
            whileFlag = 0;
        } 
    }
    else if(strcmp(root->dato,"BLOQUE") != 0){ // CREO LA ETIQUETA DEL WHILE PARA DESPUES PODER REALIZAR EL JMP A ELLA  
        if(whileFlag) { // SOLO LA CREO SI NO HUBO UN WHILE ANTERIOR, XQ SI HUBO LA CREE EN EL NODO BLOQUE PADRE DE ESE WHILE
            printf("%s:\n",desapilarEtiqueta());
            printf("--ET POR %s Y WHILE FLAG\n", root->dato);
            numeroEtiqueta++;
            whileFlag = 0;
        }
        else if(jumpFlag) {
            printf("ET_%d:\n",numeroEtiqueta);
            printf("--ET POR %s Y IF FLAG\n", root->dato);
            numeroEtiqueta++;
            jumpFlag = 0;
        }
    }
    //*************************************************************************************************************

    izq = recorrerArbol(root->izq); // LLAMO AL HIJO IZQUIERDO

    // A EVALUAR CUANDO RECORRIO EL HIJO IZQUIERDO
    //*************************************************************************************************************
    if(strcmp(root->dato,"CUERPO") == 0){ // IF CON ELSE
        // if(dobleWhile){
        //       printf("\tJMP ET_%d\n\n",verTopeEtiqueta());
        //       dobleWhile = 0;
        // }
        // else {
            printf("\tJMP ET_%d\n\n",numeroEtiqueta+1);
            printf("ET_%d:\n",numeroEtiqueta);
            printf("--ET POR CUERPO Y JMP FLAG\n");
            numeroEtiqueta++;
            jumpFlag = 0;
        // }

    }
    else if(strcmp(root->dato,"WHILE") == 0){ // SUMO UNO EN LA ETIQUETA PARA LA RAMA DERECHA
        numeroEtiqueta++;
        returnWhile = 1;
    } 
    else if(strcmp(root->dato,"BLOQUE") == 0 && strcmp(root->der->dato,"IF") == 0){
        dobleWhile = 1;
    }
    //*************************************************************************************************************
    der = recorrerArbol(root->der); // LLAMO AL HIJO DERECHO

    count = izq + der;

    if(count != 2){
        if(strcmp(root->dato,"BLOQUE") != 0 && strcmp(root->dato,"IF") != 0 && strcmp(root->dato,"CUERPO") != 0 && strcmp(root->dato,"WHILE") != 0 ) {
            imprimirAssembler(root);
        }
    }

    // A EVALUAR CUANDO YA RECORRIO LOS DOS HIJOS
    //*************************************************************************************************************
    if(strcmp(root->dato,"WHILE") == 0){ // CREO LA ETIQUETA DEL WHILE PARA DESPUES PODER REALIZAR EL JMP A ELLA
        printf("\tJUMP ET_%d\n",numeroEtiquetaSavedLocal);
        // printf("NUMERO ETIQUETA SAVED LOCAL : %d\n",numeroEtiquetaSavedLocal);
        // printf("NUMERO ETIQUETA  : %d\n",numeroEtiqueta);
        char aux[100];
        sprintf(aux,"ET_%d",numeroEtiquetaSavedLocal+1);
        printf("ETIQUETA RECIEN CREADA %s \n", aux);
        apilarEtiqueta(aux);
        if(numeroEtiquetaSavedLocal == numeroEtiqueta -2) {
            numeroEtiqueta--;
        }
        // numeroEtiqueta++;
        whileFlag = 1;
    }
    else if(strcmp(root->dato,"IF") == 0 && strcmp(root->der->dato,"CUERPO") != 0){ // IF SIN ELSE
        // printf("ET_%d:\n",numeroEtiqueta);
        // printf("--ET POR IF Y JMP FLAG\n");
        // numeroEtiqueta++;
        jumpFlag = 1;
    }
    //*************************************************************************************************************
} 

void imprimirAssembler(t_arbol root) {
    
    if(esComparador(root->dato)){
        anteriorAuxFlag = 0;
        if(strcmp(root->dato, "==") == 0){
            printf("\tFLD %s\n",root->izq->dato);
            printf("\tFCOMP %s\n",root->der->dato);
            printf("\tfstsw ax\n");
            printf("\tsahf\n");
            strcpy(ultimoJump,"JNE");
            printf("\t%s ET_%d\n", ultimoJump, numeroEtiqueta);
        }
        if(strcmp(root->dato, "!=") == 0){
            printf("\tFLD %s\n",root->izq->dato);
            printf("\tFCOMP %s\n",root->der->dato);
            printf("\tfstsw ax\n");
            printf("\tsahf\n");
            strcpy(ultimoJump,"JE");
            printf("\t%s ET_%d\n", ultimoJump, numeroEtiqueta);
        }
        if(strcmp(root->dato, ">") == 0){
            printf("\tFLD %s\n",root->izq->dato);
            printf("\tFCOMP %s\n",root->der->dato);
            printf("\tfstsw ax\n");
            printf("\tsahf\n");
            strcpy(ultimoJump,"JNA");
            printf("\t%s ET_%d\n", ultimoJump, numeroEtiqueta);
        }
        if(strcmp(root->dato, "<") == 0){
            printf("\tFLD %s\n",root->izq->dato);
            printf("\tFCOMP %s\n",root->der->dato);
            printf("\tfstsw ax\n");
            printf("\tsahf\n");
            strcpy(ultimoJump,"JNB");
            printf("\t%s ET_%d\n", ultimoJump, numeroEtiqueta);
        }
        if(strcmp(root->dato, ">=") == 0){
            printf("\tFLD %s\n",root->izq->dato);
            printf("\tFCOMP %s\n",root->der->dato);
            printf("\tfstsw ax\n");
            printf("\tsahf\n");
            strcpy(ultimoJump,"JNAE");
            printf("\t%s ET_%d\n", ultimoJump, numeroEtiqueta);
        }
        if(strcmp(root->dato, "<=") == 0){
            printf("\tFLD %s\n",root->izq->dato);
            printf("\tFCOMP %s\n",root->der->dato);
            printf("\tfstsw ax\n");
            printf("\tsahf\n");
            strcpy(ultimoJump,"JNBE");
            printf("\t%s ET_%d\n", ultimoJump, numeroEtiqueta);
        }
    }
    else if(strcmp(root->dato,"=")==0){
        if(anteriorAuxFlag == 1) {
            printf("\tFLD @aux%d\n",numeroVariables-1);
            printf("\tfstp %s\n",root->izq->dato);
            printf("\tffree\n");
        }
        else{
            printf("\tFLD %s\n",root->der->dato);
            printf("\tfstp %s\n",root->izq->dato);
            printf("\tffree\n");
        }
        anteriorAuxFlag = 0;
    }
    else{
        printf("\tFLD %s\n",root->izq->dato);
        if(strcmp(root->dato,"+")==0){
            if(anteriorAuxFlag == 1){
                printf("\tFLD @aux%d\n",numeroVariables-1);
                printf("\tFADD\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            else{
                printf("\tFLD %s\n",root->der->dato);
                printf("\tFADD\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            anteriorAuxFlag = 1;
        }
        else if(strcmp(root->dato,"-")==0){
            if(anteriorAuxFlag == 1){
                printf("\tFLD @aux%d\n",numeroVariables-1);
                printf("\tFSUB\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            else{
                printf("\tFLD %s\n",root->der->dato);
                printf("\tFSUB\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            anteriorAuxFlag = 1;

        }
        else if(strcmp(root->dato,"*")==0){
            if(anteriorAuxFlag == 1){
                printf("\tFLD @aux%d\n",numeroVariables-1);
                printf("\tFMUL\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            else{
                printf("\tFLD %s\n",root->der->dato);
                printf("\tFMUL\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            anteriorAuxFlag = 1;
        }
        else if(strcmp(root->dato,"/")==0){
            if(anteriorAuxFlag == 1){
                printf("\tFLD @aux%d\n",numeroVariables-1);
                printf("\tFDIV\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            else{
                printf("\tFLD %s\n",root->der->dato);
                printf("\tFDIV\n");
                printf("\tfstp @aux%d\n",numeroVariables);
                printf("\tffree\n");
            }
            anteriorAuxFlag = 1;
        }
        numeroVariables++;
    }
    printf("\n");
}

int isOperador(char* string){
    if(strcmp(string,"/") == 0 || strcmp(string,"+") == 0 || strcmp(string,"*") == 0 || strcmp(string,"-") == 0){
        return 1;
    }
    return 0;
}

int esComparador(char* string){
    if(strcmp(string, "==") == 0 || strcmp(string, "!=") == 0 || strcmp(string, ">") == 0 || strcmp(string, "<") == 0 || strcmp(string, ">=") == 0 || strcmp(string, "<=") == 0){
        return 1;
    }
    return 0;
}