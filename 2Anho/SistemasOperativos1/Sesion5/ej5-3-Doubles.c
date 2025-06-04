#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    //Definimos las variables
    double *array;
    int num;

    printf("El PID de esta funcion es: %d\n", getpid());
    printf("Malloc aun no ejecutado, su direccion es: %p\n",(void*)array);
    scanf("%d",&num);

    //Hacemos el malloc de 1000 doubles
    array = (double *)malloc(1000 * sizeof(double));
    if (array == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;  // Terminar el programa si falla malloc
    }
    printf("Malloc numero 1 de tamano 1000 ejecutado, su direccion es: %p\n",(void*)array);
    printf("El tamaño del puntero devuelto de este malloc es: %ld\n",sizeof(array));
    scanf("%d",&num);

    
    //Antes de liberar memoria, hacemos un segundo malloc con tamaño distnto y volvemos a comprobar
    array = (double *)malloc(500 * sizeof(double));
    
    if (array == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;  // Terminar el programa si falla malloc
    }
    printf("Malloc numero 2 de tamano 500 ejecutado, su direccion es: %p\n",(void*)array);
    printf("El tamaño del puntero devuelto de este malloc es: %ld\n",sizeof(array));
    scanf("%d",&num);

    free(array); //Liberamos memoria

    return 0;
}