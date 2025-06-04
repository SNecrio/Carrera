/*
* PRACTICA 3: USO DE SEÑALES
* ENTREGABLE 5
* VALERIA ITZEL CONTRERAS MIRANDA
* SERGIO SOUTO MOURELLE
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

int ultimo_primo = 2; //Último número primo calculado (inicializamos en 2)
int continuar = 1; //Control de ejecución del programa

//Función para verificar si un número es primo
int es_primo(int numero) {
    if (numero < 2) return 0;
    for (int i = 2; i <= sqrt(numero); i++) {
        if (numero % i == 0) {
            return 0;
        }
    }
    return 1;
}

//Manejador para la señal SIGALRM
void manejador_alarm(int signum) {
    printf("Último número primo: %d, Raíz cuadrada: %.4f\n", ultimo_primo, sqrt(ultimo_primo));
    alarm(1); //Volver a activar la alarma
}

//Manejador para la señal SIGUSR1
void manejador_sigusr1(int signum) {
    printf("Señal SIGUSR1 recibida. Finalizando el programa...\n");
    continuar = 0; //Cambiar la bandera para finalizar el programa
}

int main() {
    //Configurar el manejador de la señal SIGALRM
    signal(SIGALRM, manejador_alarm);
    //Configurar el manejador de la señal SIGUSR1
    signal(SIGUSR1, manejador_sigusr1);
    printf("\nEl pid del proceso es %d\n",getpid());
    sleep(5);
    //Activar la primera alarma
    alarm(1);

    int numero = 2; //Comenzamos en el primer número primo

    //Bucle para calcular números primos hasta que se reciba SIGUSR1
    while (continuar == 1) {
        if (es_primo(numero) == 1) {
            ultimo_primo = numero;
        }
        numero++;
    }

    printf("Programa finalizado.\n");
    return 0;
}
