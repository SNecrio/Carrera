#ifndef PILAOPERANDO_H
#define PILAOPERANDO_H
#include "abin.h"

//Interfaz TAD pila
typedef void *pilaOperando; /*tipo opaco*/

//CONTENIDO DE CADA ELEMENTO DE LA PILA
//MODIFICAR: PARA LA PILA DE OPERADORES: char
//MODIFICAR: PARA LA PILA DE OPERANDOS: abin
typedef abin tipoelemPilaOperando;

//Funciones de creacion y destruccion
/**
 * Crea la pila vacia. 
 * @param P Puntero a la pila. Debe estar inicializada.
 */
void crearPilaOperando(pilaOperando *P);

/**
 * Destruye la pila
 * @param P puntero a la pila
 */
void destruirPilaOperando(pilaOperando *P);

//Funciones de informacion
/**
 * Comprueba si la pila esta vacia
 * @param P pila
 */
unsigned esVaciaPilaOperando(pilaOperando P);

/*
 * Recupera la informacion del tope de la pila
 * @param P pila
 * 
*/
tipoelemPilaOperando topeOperando(pilaOperando P);

//Funciones de insercion/eliminacion
/**
 * Inserta un nuevo nodo en la pila para el elemento E
 * en el tope de la pila
 * @param P puntero a la pila
 * @param E Informacion del nuevo nodo. 
 */
void pushOperando(pilaOperando *P, tipoelemPilaOperando E);

/**
 * Suprime el elemento en el tope de la pila
 * @param P puntero a la pila
 */
void popOperando(pilaOperando *P);



#endif