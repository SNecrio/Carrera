
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    float val1, val2, media;

    printf("Introduzca o valor 1: ");
    scanf_s("%f", &val1); // El %f sirve para decirle que tipo de variable quiero meter
    printf("Introduzca o valor 2: ");
    scanf_s("%f", &val2); // El & antes de la variable le dice que vaya al lugar donde esta guardada en el disco duro

    media = (val1 + val2) / 2.0;

    printf("Valor medio: %f\n", media);

    return(EXIT_SUCCESS);
}
