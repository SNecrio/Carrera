#include <iostream>

int main()
{
	float Vec[5] = { 1,2,3,4,5 };
	int Num = 8, i=0;
	bool Enc = false;

	while (i < sizeof(Vec) / sizeof(int) && !Enc) {

		if (Vec[i] == Num) {
			Enc = true;
		}
		i++;
	}

	if (Enc == true) {
		printf("El numero esta en la lista");

	}
	else {
		printf("El numero no esta en la lista");
	}
}
