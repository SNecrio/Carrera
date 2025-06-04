#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAM 10

int global;
int arrayg[TAM];

int globalI = 30;
int arraygI[5] = {1, 2, 3, 4, 5};
int main(){
    int local = 3;
    int arrayl[TAM];
    printf("PID del proceso: %d.\n", getpid());
    printf("Dirección de var_local: %p\n", (void*)&local);
    printf("Dirección de array_local: %p\n", (void*)&arrayl);
    printf("Dirección de var_global: %p\n", (void*)&global);
    printf("Dirección de array_global: %p\n", (void*)&arrayg);
    
    printf("Dirección de var_global Inicializada: %p\n", (void*)&globalI);
    printf("Dirección de array_global Inicializada: %p\n", (void*)&arraygI);
    
    printf("La dirección de la función main es: %p\n", (void*)main);
    
    printf("Introduce un entero para continuar: ");
    scanf(" %d", &local);
    
    printf("\nFinalizando ejecución\n");
}
