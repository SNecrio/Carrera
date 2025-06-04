#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#define CORES 4 

/* Observaciones apartado h:
  Probamos a modificar nuestro código para que el código cree 5 hilos en vez de 4, los correspondientes con el número de cores del equipo.
  Observamos que el programa tardaba más en ejecutarse pese al incremento en hilos. Deducimos qu esto se debe a que, al tener menos cores
  que hilos, el sistema operativo tenía que ejecutar constantes cambios de contexto para que todos puedieran usar la CPU, ralentizando
  el proceso.
  Lo mismo ocurrió cuando creamos 6 hilos.
*/

//Inicio del programa

//Definimos una estructura para pasarle los argumentos necesarios a la función
typedef struct
{
    long comienzo;
    double* resFin;
}estruct;

//Hace los cálculos de 10 iteraciones consecutivas de los valores que le correspondan
void* calcular_diez(void* straux){

    //Dependiendo del número de cores, hace los cálculos en un intervalo diferente de valores
    int sumar = (CORES-1)*10;

    estruct* str = (estruct*)straux;

    long start = str->comienzo;
    
    long final = start + 9;
    double resultado = 0;
    //Realiza los cálculos en el intervalo
    while(start < 1000000000){
        while(start <= final){
            resultado += (double)1/(start * start);
            start++;
        }
        start += sumar;
        final = start + 9;
    }
    
    /*Apartado c, punto 2: Al descomentar este código y comentar el bucle anterior, se imprimirá el número de iteraciones realizadas como resultado final de los hilos
    while(start < 1000000000){
        while(start <= final){
            //printf("Iteracion %ld\n", start);
            resultado ++;
            start++;
        }
        start += sumar;
        final = start + 9;
    }*/
    
    *(str->resFin) = resultado;
    //Al acabar los cálculos, mata el hilo
    pthread_exit(NULL);
}

//Función para hacer todos los cálculos secuencialmente
double calcular_todo(){

    long start = 1;
    double resultado = 0;

    while(start < 1000000000){
        resultado += (double)1/(start * start);
        start++;
    }

    return resultado;
}

int main(){
    pthread_t pro1;
    pthread_t pro2;
    pthread_t pro3;
    pthread_t pro4;
    
    //Inicializa las estructuras para cada hilo
    estruct str1;
    str1.comienzo = 1;
    str1.resFin = (double*)malloc(sizeof(double));

    estruct str2;
    str2.comienzo = 11;
    str2.resFin = (double*)malloc(sizeof(double));

    estruct str3;
    str3.comienzo = 21;
    str3.resFin = (double*)malloc(sizeof(double));

    estruct str4;
    str4.comienzo = 31;
    str4.resFin = (double*)malloc(sizeof(double));

    //Crea los hilos, ejecutando la función de cálculo
    pthread_create(&pro1,NULL,calcular_diez,&str1);
    pthread_create(&pro2,NULL,calcular_diez,&str2);
    pthread_create(&pro3,NULL,calcular_diez,&str3);
    pthread_create(&pro4,NULL,calcular_diez,&str4);

    //El hilo principal espera a la vuelta de el resto de hilos
    pthread_join(pro1, NULL);
    pthread_join(pro2, NULL);
    pthread_join(pro3, NULL);
    pthread_join(pro4, NULL);
    
    //Cálculo del resultado final sumando los resultados individuales de todos los hilos
    double resHilos = *(str1.resFin) + *(str2.resFin) + *(str3.resFin) + *(str4.resFin);
    //Valor real de la expresión
    double real = 1.64493406685;
    double resPadre = calcular_todo();

    printf("\nValor real: %lf\n",real);
    //Cálculo del resultado, calculado únicamente por el hilo principal
    printf("\nResultado final del principal: %lf\n",resPadre);
    printf("\nResultado final de los hilos: %lf\n",resHilos);

    printf("\n\nDiferencia entre principal e hilos: %lf\n", fabs(resHilos - resPadre));
    printf("\nDiferencia entre principal y real: %lf\n", fabs(real - resPadre));
    printf("\nDiferencia entre hilos y real: %lf\n", fabs(resHilos - real));

    free(str1.resFin);
    free(str2.resFin);
    free(str3.resFin);
    free(str4.resFin);

    return 0;
}
