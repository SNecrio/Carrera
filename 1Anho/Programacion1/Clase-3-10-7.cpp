#include <iostream>
using namespace std;

int main()
{
	int a[] = { 4,1,4,4,3,1,4,5 };
	int i = 0;
	int max=0,min=99;
	int vecmax = 0, vecmin = 0;

	while (i < sizeof(a)/sizeof(a[0])) {

		if (a[i] > max) {
			max = a[i];
			vecmax = 1;
		}
		else if (a[i] == max) {
			vecmax++;
		}
		else if (a[i] == min) {
			vecmin++;
		}
		else if (a[i] < min) {
			min = a[i];
			vecmin = 1;
		}

		i++;
	}

	cout << "El numero que mas veces se repite es " << max << " y se repite " << vecmax << " veces\n";
	cout << "El numero que menos veces se repite es " << min << " y se repite " << vecmin << " veces\n";

}
