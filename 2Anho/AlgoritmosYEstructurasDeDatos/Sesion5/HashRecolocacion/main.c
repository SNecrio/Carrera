#include <stdio.h>
#include <stdlib.h>
#include "tabla_hash_recolocacion.h"

/// MODIFICACIONES EN main.c

void insercionArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nColisionesI, int* nPasosExtraI);
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraB);

int main(int argc, char** argv) {
    //variable que almacenan el tipo de función hash (1-2-3)
    unsigned int tipoFH; 
    unsigned int K=256; //factor de ponderación para hash por suma ponderada
    //variable que almacena el tipo de recolocación: lineal (1), o cuadrática (2)
    unsigned int tipoR;

    TablaHash t; //tabla hash

    int nColisionesI = 0, nPasosExtraI = 0, nPasosExtraB = 0;

    //Abrimos el fichero de datos de ejemplo, con 10000 jugadores
    FILE *fp = fopen("jugadores_got.txt", "rt");

    //Informo del tamaño de la tabla actual
    printf("Tamaño de la tabla hash: %d\n\n", N);

    //Pregunto al usuario que función hash quiere utilizar
    printf("\tFunción hash a utilizar:"
            "\n\t\t1: método división (suma ascii módulo N)"
            "\n\t\t2: suma ponderada con K=256"
            "\n\t\t3: suma ponderada con otro valor de K ");
    printf("\n\tTipo función hash: ");scanf("%d", &tipoFH);
    if(tipoFH==3){
        printf("\t\tValor de ponderación K (256): ");scanf("%d",&K);
    }
    printf("\nFUNCIÓN HASH %u", tipoFH);
    if (tipoFH==2 || tipoFH==3)
        printf(" con K=%u\n",K);

    //Pregunto al usuario que estrategia de recolocación quiere utilizar
    //Si es lineal, también pido el valor de a
    printf("\n\n\tEstrategia de recolocación (1: Lineal, 2: Cuadrática): ");
    scanf("%d", &tipoR);
    unsigned int a=0; //inicializo a
    if (tipoR == 1) {
        printf("\t\tValor de a (1 para recolocación simple): ");
        scanf("%u", &a);
        printf("\n\nRECOLOCACIÓN LINEAL con a=%u", a);
    } else
        printf("\n\nRECOLOCACIÓN CUADRÁTICA");

    //Inicializo la tabla hash por encadenamiento
    InicializarTablaHash(t);

    printf("\n\t----INSERCIÓN RECOLOCACIÓN----");
    //llamo a la función de inserción en t a partir del archivo

    insercionArchivo(fp, t, tipoFH, K, tipoR, a, &nColisionesI, &nPasosExtraI);

    printf("\n\nNumero de colisiones: %d\n", nColisionesI);
    printf("Numero de pasos extra: %d", nPasosExtraI);

    rewind(fp); //rebobino

    printf("\n\n\t----BÚSQUEDA RECOLOCACIÓN----");
    //Llamo a la función de búsqueda en t a partir del archivo 

    busquedaArchivo(fp, t, tipoFH, K, tipoR, a, &nPasosExtraB);
    printf("\n\nNumero de pasos extra: %d\n", nPasosExtraB);

    fclose(fp); //Cierro el archivo    

    return (EXIT_SUCCESS);
}

//Función para insertar usuarios en tabla hash, usando la función hash "tipo"
void insercionArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nColisionesI, int* nPasosExtraI) {
    TIPOELEMENTO jugador;
    if (fp) {
        fscanf(fp, "%[^-] - %s - %s", jugador.nombre, jugador.alias, jugador.correo);
        while (!feof(fp)) {
            *nColisionesI += InsertarHash(t, jugador, tipoFH, K, tipoR, a, nPasosExtraI);
            fscanf(fp, "%[^-] - %s - %s", jugador.nombre, jugador.alias, jugador.correo);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}

//Función para buscar un elemento leído del archivo en la tabla t, usando la función hash "tipo"
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraB) {
    TIPOELEMENTO jugador;
    if (fp) {
        fscanf(fp, "%[^-] - %s - %s", jugador.nombre, jugador.alias, jugador.correo);
        while (!feof(fp)) {
            //El número de colisiones es el mismo que en inserción, hay pasos adicionales al buscar en la lista
            BuscarHash(t, jugador.alias, &jugador, tipoFH, K, tipoR, a, nPasosExtraB);
            fscanf(fp, "%[^-] - %s - %s", jugador.nombre, jugador.alias, jugador.correo);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}