#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define CORES 4

typedef struct {
    long comienzo;
    double* resFin;
} estruct;

void* calcular_diez(void* straux) {
    // Cambia `sumar` para que corresponda al próximo bloque exacto de trabajo
    int sumar = CORES * 10;

    estruct* str = (estruct*)straux;

    long start = str->comienzo;
    long final = start + 9;
    double resultado = 0;
    long iter = 0;

    // Bucle de iteraciones
    while (start < 100000000) {
        while (start <= final && start < 100000000) {
            resultado += 1.0 / (start * start);
            iter++;
            start++;
        }
        start += sumar;
        final = start + 9;
    }

    *(str->resFin) = resultado;

    printf("\nEste hilo hizo %ld iteraciones\n", iter);
    printf("\nstrar: %ld\n", start);

    pthread_exit(NULL);
}

double calcular_todo() {
    long start = 1;
    double resultado = 0;

    while (start < 1000000000) {
        resultado += 1.0 / (start * start);
        start++;
    }

    return resultado;
}

int main() {
    pthread_t pro1, pro2, pro3, pro4;

    estruct str1, str2, str3, str4;

    str1.comienzo = 1;
    str1.resFin = (double*)malloc(sizeof(double));

    str2.comienzo = 11;
    str2.resFin = (double*)malloc(sizeof(double));

    str3.comienzo = 21;
    str3.resFin = (double*)malloc(sizeof(double));

    str4.comienzo = 31;
    str4.resFin = (double*)malloc(sizeof(double));

    pthread_create(&pro1, NULL, calcular_diez, &str1);
    pthread_create(&pro2, NULL, calcular_diez, &str2);
    pthread_create(&pro3, NULL, calcular_diez, &str3);
    pthread_create(&pro4, NULL, calcular_diez, &str4);

    double resPadre = calcular_todo();

    pthread_join(pro1, NULL);
    pthread_join(pro2, NULL);
    pthread_join(pro3, NULL);
    pthread_join(pro4, NULL);

    double resHilos = *(str1.resFin) + *(str2.resFin) + *(str3.resFin) + *(str4.resFin);
    double real = 1.64493406685;

    printf("\nValor real: %lf\n", real);
    printf("\nResultado final del principal: %lf\n", resPadre);
    printf("\nResultado final de los hilos: %lf\n", resHilos);

    printf("\n\nDiferencia entre ambos: %lf\n", fabs(resHilos - resPadre));
    printf("\nDiferencia entre principal y real: %lf\n", fabs(real - resPadre));
    printf("\nDiferencia entre hilos y real: %lf\n", fabs(resHilos - real));

    free(str1.resFin);
    free(str2.resFin);
    free(str3.resFin);
    free(str4.resFin);

    return 0;
}