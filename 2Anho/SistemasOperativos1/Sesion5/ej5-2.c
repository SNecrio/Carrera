#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
Analiza un proceso con dos funciones f1 y f2. Cada una de ellas recibe como parámetro un número
entero y realiza la siguiente tarea: define una variable entera local y muestra en el terminal la
dirección del parámetro recibido y de la variable local. Identifica donde se encuentran los códigos
de las funciones, los parámetros y las variables locales de la función en el mapa de memoria.
*/


void func2(int numRec){

    int numLoc;

    printf("El numero recibido como parametro en la f2: %p\n",(void*)&numRec);
    printf("El numero definido localmente en la f2: %p\n",(void*)&numLoc);
}

void func1(int numRec){

    int numLoc;

    printf("El numero recibido como parametro en la f1: %p\n",(void*)&numRec);
    printf("El numero definido localmente en la f1: %p\n",(void*)&numLoc);
}

int main(){

    int numero = 5;

    printf("El PID de esta funcion es: %d\n", getpid());
    func1(numero);
    func2(6);
    scanf("%d", &numero);

    return 0;
}