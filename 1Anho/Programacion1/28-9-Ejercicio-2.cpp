#include <iostream>
using namespace std;

int main()
{
	int i = 0;
	int Suma = 0;
	float Vector[] = { 1,2,3,4,5 }; //1+2+3+4+5 = 15

	while (i < (sizeof(Vector) / sizeof(int)))
	{
		Suma += Vector[i];
		i++;
	}

	cout << Suma;
}