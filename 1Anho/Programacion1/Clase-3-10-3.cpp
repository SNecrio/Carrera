#include <iostream>
#include <math.h>

using namespace std;

int main()
{
	int n = 1;
	double num = 0;
	while (n < 100) {
		num += 1/pow(2,n);
		n++;
	}
	cout << num;
}
