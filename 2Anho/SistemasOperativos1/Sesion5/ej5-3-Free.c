#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){

    //Definimos las variables
    int *array;
    int num;

    printf("El PID de esta funcion es: %d\n", getpid());
    printf("Malloc aun no ejecutado, su direccion es: %p\n",(void*)array);
    scanf("%d",&num);

    //Hacemos un primer malloc de tamaño 5
    array = (int *)malloc(5 * sizeof(int));
    
    if (array == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;  // Terminar el programa si falla malloc
    }
    printf("Malloc numero 1 de tamano 5 ejecutado, su direccion es: %p\n",(void*)array);
    scanf("%d",&num);
    free(array);
    
    //Despues de liberar memoria, hacemos otro malloc mas grande, y vemos que no utiliza el mismo espacio, ya que no cabe
    array = (int *)malloc(500 * sizeof(int));
    
    if (array == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;  // Terminar el programa si falla malloc
    }
    printf("Malloc numero 2 de tamano 500 ejecutado, su direccion es: %p\n",(void*)array);
    scanf("%d",&num);

    //Finalmente hacemos un tercer malloc para comprobar que utiliza el espacio liberado por free
    array = (int *)malloc(5 * sizeof(int));
    
    if (array == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;  // Terminar el programa si falla malloc
    }
    printf("Malloc numero 3 de tamano 5 ejecutado, su direccion es: %p\n",(void*)array);
    scanf("%d",&num);

    free(array); //Liberamos memoria

    return 0;
}