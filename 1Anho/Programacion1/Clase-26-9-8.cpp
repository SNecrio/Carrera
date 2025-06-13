#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
	string d;

	printf("Introduzca una palabra: ");
	getline(cin, d);

	cout << "Su palabra es: " << d << " y tiene " << d.length() << " letras.";

	return(EXIT_SUCCESS);
}
