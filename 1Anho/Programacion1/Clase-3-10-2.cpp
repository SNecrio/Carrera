#include <iostream>
using namespace std;
int main()
{
	int ano;

	cout << "Introduzca un año: ";cin >> ano;

	if (((ano % 4 == 0) && (ano % 100 != 0)) || ano % 400 == 0) {
		cout << "El año que introdujo es bisiesto\n";
	}
	else {
		cout << "El año que introdujo no es bisiesto\n";
	}
}
