
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char c;

	printf("Introduzca un caracter: ");
	scanf_s("%c", &c);
	printf("El valor ASCII de su caracter es: %d", c);

	return(EXIT_SUCCESS);
}

