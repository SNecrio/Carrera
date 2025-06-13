#include <iostream>

int main()
{
    int a, b, c;

    std::cin >> a >> b;

    c = a;
    a = b;
    b = c;

    std::cout << a << b;
}
