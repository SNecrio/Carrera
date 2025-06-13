#include <iostream>
#include <ctype.h>

int main()
{
	char c;

	printf("Introduzca un caracter: ");
	scanf_s("%c", &c);

	c = toupper(c);

	printf("La mayuscula de su caracter es: %c", c);

	return(EXIT_SUCCESS);

}
