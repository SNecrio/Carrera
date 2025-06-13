
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int Valor_1 = 0, Valor_2 = 0;
	float Cociente, Resto;

	printf("Introduzca el primer numero: ");

	while (Valor_1 <= 0) {
		scanf_s("%d", &Valor_1);
		if (Valor_1 <= 0) {
			printf("No es un valor positivo, vuelva a introducir un valor entero y positivo: ");
		}
	}

	printf("Introduzca el segudo numero: ");

	while (Valor_2 <= 0) {
		scanf_s("%d", &Valor_2);
		if (Valor_2 <= 0) {
			printf("No es un valor positivo, vuelva a introducir un valor entero y positivo: ");
		}
	}

	Cociente = Valor_1 / Valor_2;
	Resto = Valor_1 % Valor_2;

	printf("El cociente de la operacion es: %f", Cociente);
	printf("\nEl resto de la operacion es: %f", Resto);

	return(EXIT_SUCCESS);
}
