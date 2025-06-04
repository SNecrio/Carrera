#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(){

    //Creamos las varibales, hacemos la operacion para asegurarase de que se usa la biblioteca
    float resp;
    float inicial = 54.23;
    int a;

    resp = sin(inicial);

    printf("\nPID del proceso dinamico: %d\n\n", getpid());
    scanf("%d",&a);
}
