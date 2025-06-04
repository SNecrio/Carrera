#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "grafo.h"

typedef struct{
    char nombre[100];
    float distancia;
    char tipo;
}camino;

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS
//Opción a del menú, introducir un vertice en el grafo
void introducir_vertice(grafo *G);

//Opción b del menú, eliminar un vértice del grafo
void eliminar_vertice(grafo *G);

//Opción c del menú, crear una relación entre dos vértices
void nuevo_arco(grafo *G);

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G);

//Opción i del menú, imprimir el grafo
void imprimir_grafo(grafo G);

//Usada para leer un archivo y cargar sus datos si este fue proporcionado
void leer_archivo(grafo* G, char* archivo);

//Usado para encontrar el camino mas rapido entre 2 ciudades
void buscar_camino(grafo G);

//Calcula el arbol de expansion de coste minimo y lo imprime por pantalla
void expansion(grafo G);

//Usada pra actualizar o crear el archivo pasado como argumento con los datos actuales
void escribir_archivo(grafo G, char* archivo);

#endif	/* FUNCIONES_H */

