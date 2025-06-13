#include <iostream>

using namespace std;

int main()
{
    int a, b;

    cin >> a >> b;

    if (a > b) {
        cout << "La primera es mayor";
    }
    else if(b>a){
        cout << "La segunda es mayor";
    }
    else {
        cout << "Son iguales";
    }
}
