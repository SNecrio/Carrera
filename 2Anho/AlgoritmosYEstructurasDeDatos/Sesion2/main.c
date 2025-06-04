#include <stdio.h>
#include <stdlib.h>
#include "abin.h"
#include "FuncionesExpresion.h"
#include "recorridos.h"

int main(){

    abin arbol;
    char expresion[100];
    printf("\nIntroduzca una expresion: ");
    scanf("%s",expresion);

    arbol = arbolExpresion(expresion);

    printf("\nRecorrido postorden: ");
    postorden(arbol);
    printf("\nRecorrido preorden: ");
    preorden(arbol);
    printf("\n");
    

    return 0;
}