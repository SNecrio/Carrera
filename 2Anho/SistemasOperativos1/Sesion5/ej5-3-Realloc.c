#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("El PID de esta funcion es: %d\n", getpid());
    int num;
    // Reservar memoria con malloc (inicialmente para 5 enteros)
    int *arr = (int *)malloc(15 * sizeof(int));

    // Comprobar si malloc fue exitoso
    if (arr == NULL) {
        printf("Error al reservar memoria con malloc\n");
        return 1;  // Salimos si malloc falla
    }

    // Mostrar la dirección inicial de la memoria
    printf("Dirección inicial al reservar con malloc de 15: %p\n", (void *)arr);
    scanf(" %d", &num);
    
    printf("Realizando realloc de 12\n");

    // Realizar realloc para aumentar el tamaño del arreglo a 10 enteros
    arr = (int *)realloc(arr, 11 * sizeof(int));
    

    // Comprobar si realloc fue exitoso
    if (arr == NULL) {
        printf("Error al cambiar el tamaño de la memoria con realloc\n");
        return 1;
    }

    // Mostrar la nueva dirección de la memoria
    printf("Dirección después de realloc: %p\n", (void *)arr);
    scanf(" %d", &num);

    // Realizar un segundo realloc para disminuir el tamaño del arreglo a 3 enteros
    
    printf("Realizando realloc de 5\n");
    arr = (int *)realloc(arr, 5 * sizeof(int));

    // Comprobar si realloc fue exitoso
    if (arr == NULL) {
        printf("Error al cambiar el tamaño de la memoria con realloc\n");
        return 1;
    }


    // Mostrar la dirección después del segundo realloc
    printf("Dirección después del segundo realloc: %p\n", (void *)arr);
    scanf(" %d", &num);

    // Liberar la memoria reservada
    free(arr);
    printf("Memoria liberada.\n");

    return 0;
}

