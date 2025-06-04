#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//Creamos la variable global
int glob = 7;

void* func2(void* var){

    //Creamos las variables, imprimimos sus direcciones y hacemos el malloc
    int *varIn = (int*)var;
    int *varMalloc;

    int locl2 = (*varIn) * glob;

    printf("Direccion en la func2 de la variable recibida: %p\n",var);
    printf("Direccion en la func2 de la variable local: %p\n", (void*)&locl2);

    varMalloc = (int *)malloc(4 * sizeof(int));
    printf("Direccion en la func2 de la variable con malloc: %p\n",(void*)varMalloc);

}

void* func1(void* var){

    //Creamos las variables, imprimimos sus direcciones y hacemos el malloc
    int *varIn = (int*)var;
    int *varMalloc;

    int locl1 = (*varIn) * glob;

    printf("Direccion en la func1 de la variable recibida: %p\n",var);
    printf("Direccion en la func1 de la variable local: %p\n", (void*)&locl1);

    varMalloc = (int *)malloc(4 * sizeof(int));
    printf("Direccion en la func1 de la variable con malloc: %p\n",(void*)varMalloc);

}

int main(){

    //Creamos las variables y imprimimos sus direcciones
    int locl = 5;
    int a;

    pthread_t hil1;
    pthread_t hil2;

    printf("PID del proceso: %d\n\n", getpid());

    printf("Direccion en main de la variable global: %p\n",(void*)&glob);
    printf("Direccion en main de la variable local: %p\n", (void*)&locl);

    //Creamos los hilos
    pthread_create(&hil1,NULL,func1,&locl);
    pthread_create(&hil2,NULL,func2,&locl);

    scanf("%d",&a);

    return 0;
}