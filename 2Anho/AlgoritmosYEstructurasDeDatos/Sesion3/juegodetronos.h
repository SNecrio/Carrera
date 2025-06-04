#ifndef JUEGODETRONOS_h
#define JUEGODETRONOS_h
#include "abb.h"
/*Funcion para añadir personajes
@param abb Arbol a añadir
*/
int anhadirPersonaje(TABB* abb);

/*Funcion para listar los personajes en orden alfabetico
@param abb Arbol que listar
*/
void listarPersonajes(TABB abb);

/*Funcion que elimina un personaje do arbol
@param abb Arbol de donde borrar
*/
void eliminarPersonaje(TABB* abb);

/*Funcion para añadir el personaje de un archivo a un arbol, devuelve 1 si encuentra el archivo y 0 si no
@param abb Arbol al que añadir el personaje
@param argc Numero de argumentos dados por linea de comandos
@param fichero Nombre del fichero, dado por linea de comandos y recogido como argv
*/
int anhadirArchivo(TABB* abb, char* fichero);

/*Funcion que escribe todos los personajes del arbol de vuelta en el archivo
@param abb Arbol que escribir
@param fichero Nombre del fichero donde imprimir
*/
void escribirArchivo(TABB abb, char* fichero);

/*Funcion que busca los asesinos de un personaje
@param abb Arbol donde buscar
*/
void buscarAsesino(TABB abb);

//void inOrdenAsesino(TABB abb, char* nombre, int* num);
int inOrdenAsesino(TABB abb, char* nombre);
int compAsesino(TIPOELEMENTOABB elem, char* nombre);

/*Funcion que busca los padres de un personaje
@param abb Arbol donde buscar
*/
void buscarHijos(TABB abb);

/*Funcion que busca a los personajes con mayor numero de asesinatos
@param abb Arbol donde buscar
*/
void buscarKiller(TABB abb);

/*Funcion que modifica un personaje existente en el arbol
@param abb Arbol donde modificar
*/
void modificar(TABB *abb);


#endif