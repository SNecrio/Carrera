#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>

using namespace std;

int main()
{
	float x;

	cout << "Introduzca x: ";cin >> x;

	if (x < -1 || x>4) {
		cout << "El numero no tiene una funcion asociada";
	}
	else if (x < 1 && x >= -1) {
		cout <<"El numero es " << (x * x);
	}
	else if (x < 2 && x>= 1) {
		cout << "El numero es " << (2*x - 1);
	}
	else {
		cout << "El numero es " << (-2 * (pow(x - 2.5,2)) + 3.5);
	}
}
