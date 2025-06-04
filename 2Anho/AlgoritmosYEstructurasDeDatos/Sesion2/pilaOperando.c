#include <stdlib.h>
#include "abin.h"

//Implementacion TAD pila

//CONTENIDO DE CADA ELEMENTO DE LA PILA
//MODIFICAR: PARA LA PILA DE OPERADORES: char
//MODIFICAR: PARA LA PILA DE OPERANDOS: abin
typedef abin tipoelemPilaOperando;
///////////////////////////////////////////////////////

//Definicion del tipo de datos pila
struct tipo_celdaOperando {
    tipoelemPilaOperando elemento;
    struct tipo_celdaOperando * sig;
};

typedef struct tipo_celdaOperando * pilaOperando;
/////////////////////////////////////////////////////////

void crearPilaOperando(pilaOperando *P) {
    *P = NULL;
}

void destruirPilaOperando(pilaOperando *P) {
    pilaOperando aux;
    aux = *P;
    while (aux != NULL) {
        aux = aux->sig;
        free(*P);
        *P = aux;
    }
}

unsigned esVaciaPilaOperando(pilaOperando P) {
    return P == NULL;
}

tipoelemPilaOperando topeOperando(pilaOperando P) {
    if (!esVaciaPilaOperando(P)) /*si pila no vacia*/
        return P->elemento;
}

void pushOperando(pilaOperando *P, tipoelemPilaOperando E) {
    pilaOperando aux;
    aux = (pilaOperando) malloc(sizeof (struct tipo_celdaOperando));
    aux->elemento = E;
    aux->sig = *P;
    *P = aux;
}

void popOperando(pilaOperando *P) {
    pilaOperando aux;
    if (!esVaciaPilaOperando(*P)) /*si pila no vacia*/ {
        aux = *P;
        *P = (*P)->sig;
        free(aux);
    }
}
