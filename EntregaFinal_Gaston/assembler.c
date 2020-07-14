#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#define C_STRING "c_string"
#define C_FLOAT "c_float"
#define C_INT "c_int"

//void escribirPrograma(FILE *, char*, t_arbol);
void escribirPuntodata(FILE *);
char * verSiVaInterrogacion(char *);

FILE* pFinal;
void generarArchivoAssembler(t_arbol root, char* etiq);



void generarArchivoAssembler(t_arbol root, char* etiq) {

    FILE* pFinal = fopen("Final.asm","wt");


    if (pFinal == NULL){
        printf("\r\nNo se pudo abrir el archivo Final\r\n");
        return;
    }
	
    // Definiciones iniciales
	fprintf(pFinal, "include macros2.asm\n");
	fprintf(pFinal, "include number.asm\n");
	fprintf(pFinal, ".MODEL LARGE\n");
	fprintf(pFinal, ".386\n");
	fprintf(pFinal, ".STACK 200h\n\n");

	// Inicio de seccion codigo
	fprintf(pFinal, ".CODE\n");
	fprintf(pFinal, "\nMAIN:\n\n");
	fprintf(pFinal, "\tMOV EAX, @DATA\n");
	fprintf(pFinal, "\tMOV DS, EAX\n");
	fprintf(pFinal, "\tMOV ES, EAX\n\n");
	
	//escribirPrograma(pFinal,etiq, root);

    recorrerArbol(pFinal,arbolPrograma);

    escribirPuntodata(pFinal);   

}

void escribirPuntodata(FILE *pFinal)
{
	// Escribimos el final del archivo
	fprintf(pFinal, "\n\nMOV EAX, 4C00h\n");
	fprintf(pFinal, "INT 21h\n\n");
	fprintf(pFinal, ".DATA\n");

	// Tabla de datos a partir de tabla de simbolos
    int i;
    for (i = 0; i < cant_elem_ts; i++) {
		
		if(strcmp(ts[i].tipo_dato,C_INT) == 0){
			fprintf(pFinal, "\t%-10s \tDD \t\t\t%s\n", &(ts[i].nombre), strcat(ts[i].valor,".0"));
		}else{
			if(strcmp(ts[i].tipo_dato,C_FLOAT) == 0){
				fprintf(pFinal, "\t%-10s \tDD \t\t\t%s\n", &(ts[i].nombre), ts[i].valor);
			}else{
				if(strcmp(ts[i].tipo_dato,C_STRING) == 0){
					fprintf(pFinal, "\t%-10s \tDB \t\t\t%s, '$', %d dup(?)\n", &(ts[i].nombre), &(ts[i].valor), ts[i].longitud);
				}else{
					fprintf(pFinal, "\t%-10s \tDD \t\t\t?\n", &(ts[i].nombre));
				}
			}
		}
    }
	
	fprintf(pFinal, "\tNEW_LINE \tDB \t\t\t0AH,0DH,'$'\n");
	fprintf(pFinal, "\nEND START"); 
	fclose(pFinal);
}
// void escribirPrograma(FILE *pFinal, char*etiq, t_arbol root)
// {
// 	int antAux;
//     if(strcmp(ultimaEtiqueta, "") == 0){
//         strcpy(ultimaEtiqueta, etiq);
//         antAux = 0;
//         printf("\n%s:\n", etiq);
//     }
//     else if(strcmp(ultimaEtiqueta, etiq) != 0 && flagJump != 0){
//         antAux = 0;
//         strcpy(ultimaEtiqueta, etiq);
//         char aux[100];
//         strcpy(aux,desapilarASM());
//         if(strstr(etiq,"ELSE")){
//             apilarASM(aux);
//         }
//         fprintf(pFinal,"\tJMP %s\n",aux);
//         fprintf(pFinal,"\n%s:\n", etiq);
//     }
//     else if(anteriorAuxFlag == 0){
//         antAux = 0;
//     }
//     else {
//         antAux = 1;
//     }
//     if(flagJump == 0){
//         if(strcmp(ultimaEtiqueta, etiq) != 0){
//             fprintf(pFinal,"\n%s:\n", etiq);
//             strcpy(ultimaEtiqueta, etiq);
//         }
//         flagJump = 1;
//     }
//     // printf("%s %s %s\n", root->izq->dato, root->dato, root->der->dato);
    
//     if(esComparador(root->dato)){
//         anteriorAuxFlag = 0;
//         if(strcmp(root->dato, "==") == 0){
//             fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//             fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstsw ax\n");
//             fprintf(pFinal,"\tsahf\n");
//             strcpy(ultimoComparador,"JNE");
//         }
//         if(strcmp(root->dato, "!=") == 0){
//             fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//             fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstsw ax\n");
//             fprintf(pFinal,"\tsahf\n");
//             strcpy(ultimoComparador,"JE");
//         }
//         if(strcmp(root->dato, ">") == 0){
//             fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//             fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstsw ax\n");
//             fprintf(pFinal,"\tsahf\n");
//             strcpy(ultimoComparador,"JNA");
//         }
//         if(strcmp(root->dato, "<") == 0){
//             fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//             fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstsw ax\n");
//             fprintf(pFinal,"\tsahf\n");
//             strcpy(ultimoComparador,"JNB");
//         }
//         if(strcmp(root->dato, ">=") == 0){
//             fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//             fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstsw ax\n");
//             fprintf(pFinal,"\tsahf\n");
//             strcpy(ultimoComparador,"JNAE");
//         }
//         if(strcmp(root->dato, "<=") == 0){
//             fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//             fprintf(pFinal,"\tFCOMP %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstsw ax\n");
//             fprintf(pFinal,"\tsahf\n");
//             strcpy(ultimoComparador,"JNBE");
//         }
//     }
//     else if(strcmp(root->dato,"=")==0){
//         if(antAux == 1) {
//             fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
//             fprintf(pFinal,"\tfstp %s\n",root->izq->dato);
//         }
//         else{
//             fprintf(pFinal,"\tFLD %s\n",root->der->dato);
//             fprintf(pFinal,"\tfstp %s\n",root->izq->dato);
//         }
//     }
//     else{
//         anteriorAuxFlag = 1;
//         fprintf(pFinal,"\tFLD %s\n",root->izq->dato);
//         if(strcmp(root->dato,"+")==0){
//             if(antAux == 1){
//                 fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
//                 fprintf(pFinal,"\tFADD\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//             else{
//                 fprintf(pFinal,"\tFLD %s\n",root->der->dato);
//                 fprintf(pFinal,"\tFADD\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//         }
//         else if(strcmp(root->dato,"-")==0){
//             if(antAux == 1){
//                 fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
//                 fprintf(pFinal,"\tFSUB\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//             else{
//                 fprintf(pFinal,"\tFLD %s\n",root->der->dato);
//                 fprintf(pFinal,"\tFSUB\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }

//         }
//         else if(strcmp(root->dato,"*")==0){
//             if(antAux == 1){
//                 fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
//                 fprintf(pFinal,"\tFMUL\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//             else{
//                 fprintf(pFinal,"\tFLD %s\n",root->der->dato);
//                 fprintf(pFinal,"\tFMUL\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//         }
//         else if(strcmp(root->dato,"/")==0){
//             if(antAux == 1){
//                 fprintf(pFinal,"\tFLD @aux%d\n",numeroVariables-1);
//                 fprintf(pFinal,"\tFDIV\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//             else{
//                 fprintf(pFinal,"\tFLD %s\n",root->der->dato);
//                 fprintf(pFinal,"\tFDIV\n");
//                 fprintf(pFinal,"\tfstp @aux%d\n",numeroVariables);
//                 fprintf(pFinal,"\tffree\n");
//             }
//         }
//         numeroVariables++;
//     }
    
//     printf("\n");
   
// }

char * verSiVaInterrogacion(char *valor) {
    if (strcmp(valor, "") == 0) {
        return "?";
    }
    return valor;
}