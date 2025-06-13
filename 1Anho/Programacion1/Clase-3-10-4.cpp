#include <iostream>
#include <math.h>

using namespace std;

int main()
{
	int n = 1;
	float prev = 1;
	float num = 0;
	while (n < 100) {

		prev = (prev/2);
		num += prev;
		n++;
	}
	cout << num;
}
