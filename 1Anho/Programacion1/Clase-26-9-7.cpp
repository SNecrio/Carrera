#include <iostream>
#include <math.h>

int main()
{
	float n;

	printf("Introduzca un numero: ");
	scanf_s("%f", &n);

	n = round(n);

	printf("Redondeando su numero nos da: %f", n);

	return(EXIT_SUCCESS);

}

