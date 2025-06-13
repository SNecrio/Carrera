#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
	string palabra;
	int c = 0;
	int a = 0, e = 0, i = 0, o = 0, u = 0;

	cout<<"Introduzca una palabra: ";
	getline(cin, palabra);

	while (c < palabra.length()) {

		switch (palabra[c]) {
		case 'A':
		case 'a': a++; break;
		case 'E':
		case 'e': e++; break;
		case 'I':
		case 'i': i++; break;
		case 'O':
		case 'o': o++; break;
		case 'U':
		case 'u': u++; break;
		}
		c++;
	}
	cout << "Hay " << a << " letras 'a'\n";
	cout << "Hay " << e << " letras 'e'\n";
	cout << "Hay " << i << " letras 'i'\n";
	cout << "Hay " << o << " letras 'o'\n";
	cout << "Hay " << u << " letras 'u'\n";


}
