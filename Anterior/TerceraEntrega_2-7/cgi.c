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
int ultimoComparadorParaAssembler = 0;
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

char ultimoComparador[10];

char ultimaEtiqueta[100];

int numeroVariables = 1;

int anteriorAuxFlag = 1;

int flagJump = 1;

int numeroEtiqueta = 0;

typedef struct s_nodo_pila_ASM{
    char dato[100];
    struct s_nodo_pila_ASM * sig;
}p_nodo_ASM;

typedef p_nodo_ASM* t_pila_ASM;

t_pila_ASM pila_ASM;

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

FILE* pFinal;
int generarASM(t_arbol root, char* etiq);
void imprimirAssembler(t_arbol root, char* etiq);
int recorrerArbol(t_arbol root, char* etiq);





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

char* desapilarASM(){
    p_nodo_ASM * aux;
    char* auxDato;
    auxDato = malloc(sizeof(strlen(aux->dato))+1);


    aux = pila_ASM;
    pila_ASM = aux->sig;
    strcpy(auxDato, aux->dato);
    free(aux);
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



int generarASM(t_arbol root, char* etiq)
{
	recorrerArbol(arbolPrograma, "ET_0");
}
























int recorrerArbol(t_arbol root, char* etiq) 
{ 
    // printf("-----RECORRIENDO: %s\n",root->dato);
    // Base case 
    
    if (root == NULL) 
        return 1; 
    char etiquetaIzq[100];
    char etiquetaDer[100];
    // Increase distance between levels 
    int count = 0;
    int izq = 0;
    int der = 0;
    if(strcmp(root->dato,"BLOQUE") == 0){
        sprintf(etiquetaIzq,"ET_%d",numeroEtiqueta);
        numeroEtiqueta++;
        sprintf(etiquetaDer,"ET_%d",numeroEtiqueta);
        numeroEtiqueta++;
        apilarASM(etiquetaDer);
    }
    else if(strcmp(root->dato,"CUERPO") == 0){
        strcpy(etiquetaIzq, etiq);
        sprintf(etiquetaDer,"ET_ELSE_%d",numeroEtiqueta);
        printf("\t%s ET_ELSE_%d\n",ultimoComparador, numeroEtiqueta);
        numeroEtiqueta++;
        flagJump = 0;
    }
    else {
        strcpy(etiquetaIzq, etiq);
        strcpy(etiquetaDer, etiq);
    }


    izq = recorrerArbol(root->izq, etiquetaIzq); 
    der = recorrerArbol(root->der, etiquetaDer);

    count = izq + der;

    if(count != 2){
        if(strcmp(root->dato,"BLOQUE") != 0 && strcmp(root->dato,"IF") != 0 && strcmp(root->dato,"CUERPO") != 0) {
            // printf("\nINGRESO : %s\n\n",root->dato);
            imprimirAssembler(root, etiq);
        }

    }

} 

void imprimirAssembler(t_arbol root, char* etiq) {

    FILE* pFinal = fopen("Final.asm","wt");


    if (pFinal == NULL){
        printf("\r\nNo se pudo abrir el archivo Final\r\n");
        return;
    }
    fprintf(pFinal,"include macros2.asm\ninclude number.asm\n ");

    fprintf(pFinal,".MODEL LARGE\n.386\n.STACK 200h\n \n.DATA\n");


    //imprimirVariables(pFinal, &list);


    fprintf(pFinal, "\n.CODE\nSTART:\n");

    int antAux;
    if(strcmp(ultimaEtiqueta, "") == 0){
        strcpy(ultimaEtiqueta, etiq);
        antAux = 0;
        printf("\n%s:\n", etiq);
    }
    else if(strcmp(ultimaEtiqueta, etiq) != 0 && flagJump != 0){
        antAux = 0;
        strcpy(ultimaEtiqueta, etiq);
        char aux[100];
        strcpy(aux,desapilarASM());
        if(strstr(etiq,"ELSE")){
            apilarASM(aux);
        }
        fprintf(pFinal,"\tJMP %s\n",aux);
        fprintf(pFinal,"\n%s:\n", etiq);
    }
    else if(anteriorAuxFlag == 0){
        antAux = 0;
    }
    else {
        antAux = 1;
    }
    if(flagJump == 0){
        if(strcmp(ultimaEtiqueta, etiq) != 0){
            fprintf(pFinal,"\n%s:\n", etiq);
            strcpy(ultimaEtiqueta, etiq);
        }
        flagJump = 1;
    }
    // printf("%s %s %s\n", root->izq->dato, root->dato, root->der->dato);
    
    if(esComparador(root->dato)){
        anteriorAuxFlag = 0;
        if(strcmp(root->dato, "==") == 0){
            fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
            fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
            fprintf(pFinal,"\tfstsw ax\n");
            fprintf(pFinal,"\tsahf\n");
            strcpy(ultimoComparador,"JNE");
        }
        if(strcmp(root->dato, "!=") == 0){
            fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
            fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
            fprintf(pFinal,"\tfstsw ax\n");
            fprintf(pFinal,"\tsahf\n");
            strcpy(ultimoComparador,"JE");
        }
        if(strcmp(root->dato, ">") == 0){
            fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
            fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
            fprintf(pFinal,"\tfstsw ax\n");
            fprintf(pFinal,"\tsahf\n");
            strcpy(ultimoComparador,"JNA");
        }
        if(strcmp(root->dato, "<") == 0){
            fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
            fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
            fprintf(pFinal,"\tfstsw ax\n");
            fprintf(pFinal,"\tsahf\n");
            strcpy(ultimoComparador,"JNB");
        }
        if(strcmp(root->dato, ">=") == 0){
            fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
            fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
            fprintf(pFinal,"\tfstsw ax\n");
            fprintf(pFinal,"\tsahf\n");
            strcpy(ultimoComparador,"JNAE");
        }
        if(strcmp(root->dato, "<=") == 0){
            fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
            fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
            fprintf(pFinal,"\tfstsw ax\n");
            fprintf(pFinal,"\tsahf\n");
            strcpy(ultimoComparador,"JNBE");
        }
    }
    else if(strcmp(root->dato,"=")==0){
        if(antAux == 1) {
            fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
            fprintf(pFinal,"\tfstp %s\n",root->izq->dato);
        }
        else{
            fprintf(pFinal,"\tFLD %s\n",root->der->dato);
            fprintf(pFinal,"\tfstp %s\n",root->izq->dato);
        }
    }
    else{
        anteriorAuxFlag = 1;
        fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
        if(strcmp(root->dato,"+")==0){
            if(antAux == 1){
                fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
                fprintf(pFinal,"\tFADD\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
            else{
                fprintf(pFinal,"\tFLD %s\n",root->der->dato);
                fprintf(pFinal,"\tFADD\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
        }
        else if(strcmp(root->dato,"-")==0){
            if(antAux == 1){
                fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
                fprintf(pFinal,"\tFSUB\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
            else{
                fprintf(pFinal,"\tFLD %s\n",root->der->dato);
                fprintf(pFinal,"\tFSUB\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }

        }
        else if(strcmp(root->dato,"*")==0){
            if(antAux == 1){
                fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
                fprintf(pFinal,"\tFMUL\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
            else{
                fprintf(pFinal,"\tFLD %s\n",root->der->dato);
                fprintf(pFinal,"\tFMUL\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
        }
        else if(strcmp(root->dato,"/")==0){
            if(antAux == 1){
                fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
                fprintf(pFinal,"\tFDIV\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
            else{
                fprintf(pFinal,"\tFLD %s\n",root->der->dato);
                fprintf(pFinal,"\tFDIV\n");
                fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
                fprintf(pFinal,"\tffree\n");
            }
        }
        numeroVariables++;
    }
    
    printf("\n");


   


	fprintf(pFinal, "mov ax, 4C00h\n");
    fprintf(pFinal, "int 21h \n");
    fprintf(pFinal, "\nEND START"); 

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