#include <stdio.h>
#include <stdlib.h>
#include "GoTMap.h"
#include <string.h>

/*
 * Programa que muestra el uso del TAD grafo de números enteros
 */

int main(int argc, char** argv) {
    //Grafo de números enteros
    grafo G; //grafo
    char opcion;
    
    //Creo el grafo
    crear_grafo(&G);
    leer_archivo(&G,argv[1]);

    do {
        printf("\n\na. Insertar nuevo vertice\n");
        printf("b. Eliminar vertice\n");
        printf("c. Crear arco\n");
        printf("d. Eliminar arco\n");
        printf("i. Imprimir grafo\n");
        printf("e. Encontrar camino\n");
        printf("f. Mostrar arbol de expansion\n");
        printf("s. Salir\n");

        printf("Opcion: ");

        scanf(" %c", &opcion); 

        switch (opcion) {
            case 'a':case'A':
                introducir_vertice(&G);
                break;
            case 'b':case 'B':
                eliminar_vertice(&G);
                break;
            case 'c': case 'C':
                nuevo_arco(&G);
                break;
            case 'd': case 'D':
                eliminar_arco(&G);
                break;
            case 'i': case 'I':
                imprimir_grafo(G);
                break;
            case 'e':case 'E':
                buscar_camino(G);
                break;
            case 'f': case 'F':
                expansion(G);
                break;

            case 's': case 'S':
                opcion='s';
                break;
            default:
                printf("Opción equivocada\n");
        }
    } while (opcion != 's');

    //Al salir, liberamos la memoria del TAD, lo destruimos
    escribir_archivo(G,argv[1]);
    borrar_grafo(&G);

    return (EXIT_SUCCESS);
}