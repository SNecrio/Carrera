#include <iostream>

int main()
{
	int a, b;

	std::cin >> a;	std::cin >> b;

	if (a % b == 0) {
		std::cout << "El primer numero es multiplo del segundo";
	}
	else {
		std::cout << "El primer numero no es multiplo del segundo";
	}
}
