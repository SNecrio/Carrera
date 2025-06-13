#include <stdio.h>
#include <stdlib.h>

int main() {

    int i = 1;
    int vector[] = { 3,2,1,6,5,6,1,2 };
    int max, min, maxtimes = 1, mintimes = 1;

    max = vector[0]; min = vector[0];

    while (i < sizeof(vector) / sizeof(vector[0])) {

        if (vector[i] == max && vector[i] == min) {
            mintimes++; maxtimes++;
        }
        else if (vector[i] > max) {
            max = vector[i]; maxtimes = 1;
        }
        else if (vector[i] == max) {
            maxtimes++;
        }
        else if (vector[i] < min) {
            min = vector[i]; mintimes = 1;
        }
        else if (vector[i] == min) {
            mintimes++;
        }

    }

    printf("El numero mas grande es el %d", max, " y se repite %d", maxtimes, " veces.\n");
    printf("El numero mas pequeño es el %d", min, " y se repite %d", mintimes, " veces.");

    return (EXIT_SUCCESS);
}