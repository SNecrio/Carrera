#include <stdlib.h>
#include <stdio.h>
#include "abb.h"
#include "juegodetronos.h"
#include "lista.h"
#include <string.h>

int main(int argc , char** argv ){

    //Creamos las variables y el arbol
    char option;
    TABB arbol;
    crearAbb(&arbol);
    int found = 0;

    //Si nota que hay mas de un argumento, mira si este es -f, y si es asi, hace la funcion de añadir archivo
    if(argc > 1){
        if(strcmp(argv[1], "-f") == 0)
            found = anhadirArchivo(&arbol,argv[2]);
    }

    //Hacemos un switch para ver que opcion quiere el usuario hasta que este quiera salir
    do{
        printf("\nElija una opcion:\n\n\tA: Anhadir personaje\n\tL: Listado de personajes\n\tE: Eliminar personaje\n\tB: Buscar asesino\n\tH: Buscar hijos\n\tK: Buscar killer\n\tM: Modificar personaje\n\tS: Salir\nOpcion: ");
        scanf(" %c",&option);

        switch(option){
            case 'A':
            case 'a':
            anhadirPersonaje(&arbol);
            break;

            case 'L':
            case 'l':
            listarPersonajes(arbol);
            break;

            case 'E':
            case 'e':
            eliminarPersonaje(&arbol);
            break;

            case 'B':
            case 'b':
            buscarAsesino(arbol);
            break;

            case 'H':
            case 'h':
            buscarHijos(arbol);
            break;

            case 'K':
            case 'k':
            buscarKiller(arbol);
            break;

            case 'M':
            case 'm':
            modificar(&arbol);
            break;

            default:
            break;
        }

    }while(option != 's' && option != 'S');

    //Si los argumentos son correctos, escribe en el archivo de vuelta los personajes
    
    if(found == 1)
        escribirArchivo(arbol,argv[2]);
    
    
    //Destruimos el arbol y salimos del programa
    destruirAbb(&arbol);
    printf("\nSaliendo del programa...\n\n");

    return 0;
}