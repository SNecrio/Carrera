
#include<iostream>
#include <stdio.h>

#define PI 3.1416

int main(int argc, char** argv)
{
	float r, a, p;

	printf("Introduzca o radio do circulo: ");
	scanf_s("%f", &r);

	p = 2 * PI * r;
	a = PI * r * r;

	printf("Area do circulo = %f\n", a);
	printf("Perimetro do circulo = %f\n", p);

	return(EXIT_SUCCESS);

}
