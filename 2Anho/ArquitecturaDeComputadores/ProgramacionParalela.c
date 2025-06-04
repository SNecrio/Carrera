//Programacion paralela

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "counter.h"

#define max_iter 20000
#define tol 1e-8
#define MAX_RAND 1000

int main(int argc, char** argv){
    //Comprobamos que se de el argumento obligatorio
    if(argc<2){
        printf("Debe introducirse el tamaño de la matriz por línea de comandos");
        exit(1);
    }

    int n=atoi(argv[1]);
    time_t n_random = (long)n;

    srand(time(&n_random));
    int i;
   
    //Declaramos: a = Matriz de coeficientes, b = Vector de term. independientes, x = Vector solucion
    float *a, *x, *b, *x_new;
    //Reeservamos memoria
    a = malloc(n*n*sizeof(float));
    x = malloc(n*sizeof(float));
    b = malloc(n*sizeof(float));
    x_new = malloc(n*sizeof(float));

    //Se rellena a con valores aleatorios
    float random;
    for(i=0;i< n;i++){
        float sum = 0.0f;
        for(int j = 0; j<n; j++){
            random= (float) rand() / MAX_RAND;
            a[i * n + j] = random;
            sum += random; //Mantenemos en la variable sum la suma de los valores de la línea
        }
        a[i * n + i] +=  sum;
    }

    //Se rellena b con valores aleatorios
    for(i=0;i< n;i++){
        b[i] = (float) rand() / MAX_RAND;
    }

    //Se rellena x con ceros
    for(i=0;i< n;i++){
        x[i] = 0;
    }

    float norm2 = 0.0f;
    float sigma = 0.0f;
    int fin = 0;

    //Comenzamos el temporizador
    double ck;
    start_counter();

    //Hacemos el cómputo
    #pragma omp parallel private(i, sigma) //Comenzamos la seccion y hacemos que las variables i y sigma sean privadas para cada hilo
    {
        for (int iter = 0; iter < max_iter; iter++) {

            #pragma omp for reduction(+:norm2) //Hacemos el bucle for paralelo y al final sumamos todos los resultados en norm2
            for (i = 0; i < n; i++) {
                sigma = 0.0f;
                for (int j = 0; j < n; j++) {
                    if (i != j) {
                        sigma += a[i * n + j] * x[j];
                    }
                }
                x_new[i] = (b[i] - sigma) / a[i * n + i];
                norm2 += (x_new[i] - x[i]) * (x_new[i] - x[i]);
            }

            #pragma omp for //Paralelizamos la asignacion de los nuevos valores de x
            for (int l = 0; l < n; l++) {
                x[l] = x_new[l];
            }

            #pragma omp single //Hacemos que solo 1 hilo compruebe si debemos salir del bucle
            {
                if (sqrt(norm2) < tol) {
                    fin = 1;
                }
            }

            #pragma omp barrier //Obligamos a todos los hilos a parar antes de seguir con una nueva interacion
            if(fin==1)
                break;
        }
    }


    //Conseguimos el tiempo que tardaron los bucles e imprimimos los ciclos medios
    ck=get_counter();
    printf("\nCodigo terminado\n\nValor de norm2: %e\n", norm2);
    double ciclosMedios = ck / (max_iter*n);
    printf("\n Ciclos promedio=%1.10lf \n", ciclosMedios);

    //Liberamos la memoria de los arrays
    free(a);
    free(b);
    free(x);
    free(x_new);
    exit(0);
}
