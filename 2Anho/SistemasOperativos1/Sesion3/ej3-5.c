/*
* PRACTICA 3: USO DE SEÑALES
* ENTREGABLE 2
* VALERIA ITZEL CONTRERAS MIRANDA
* SERGIO SOUTO MOURELLE
* AL EJECUTAR, ABRIR OTRA TERMINAL Y ESCRIBIR EL COMANDO:
* kill -SIGUSR2 [PID-HIJO] PARA COMPROBAR EL FUNCIONAMIENTO
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int hijoMatar;
pid_t arrayPID[25];
int maxHijos = 0;

//Función que recibe la señal SIGUSR2
static void gestion(int sig) {
    printf("Proceso con PID: %d ha recibido SIGUSR2\n",getpid());
    int i = 1;

    while(i < maxHijos){
        if(getpid() == arrayPID[i]){
            printf("\nProceso numero %d de PID %d ha acabado con el proceso de PID %d\n",i,getpid(),arrayPID[i-1]);
            kill(arrayPID[i - 1], SIGKILL);
            exit(0);
        }
        i++;
    }
}

int main(){

    pid_t padre = getpid();
    pid_t pidHijo;

    printf("\nIntroduzca numero de hijos: ");
    scanf("%d",&maxHijos);
    //Creación de hijos 
    for(int i = 0; i < maxHijos; i++){
        if(padre == getpid()){
            pidHijo = fork();
            if(getpid() != padre){
                printf("\nHijo numero %d, PID: %d\n", i,getpid());
                arrayPID[i] = getpid();
                if (signal(SIGUSR2, gestion) == SIG_ERR){
                perror("Error al crear gestor");
                }
                while(1){ //Espera la señal SIGUSR2
                    pause();
                }
            }
            arrayPID[i] = pidHijo;
            sleep(1);
            
        }

    }
    
    printf("Proceso padre esperando la finalización de H[1]...\n");
    waitpid(arrayPID[0], NULL, 0); // Espera la finalización de H[1]
    printf("El proceso hijo H[1] ha finalizado.\n");
   

    return 0;
}
