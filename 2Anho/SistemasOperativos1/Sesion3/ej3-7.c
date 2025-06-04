/*
* PRACTICA 3: USO DE SEÑALES
* ENTREGABLE 3
* VALERIA ITZEL CONTRERAS MIRANDA
* SERGIO SOUTO MOURELLE
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

//Función para imprimir la hora
void print_time() {
    time_t now;
    time(&now);
    printf("Hora: %s", ctime(&now));
}
//Función que maneja las señales recibidas
void gestion(int sig){

    if(sig == SIGUSR1){
        printf("\nSIGUSR1 recibida\n");
        print_time();
    }else if(sig == SIGUSR2){
        printf("\nSIGUSR2 recibida\n");
        print_time();
    }

     return;
}

int main(){

    pid_t padre = getpid();
    pid_t hijo1;
    pid_t hijo2;

    sigset_t bloqueadas, pendientes;

    fork();

    if(getpid() == padre){
        printf("\nSoy el padre: %d\n",padre);

        struct sigaction ignorar;
        struct sigaction sigur;

        memset(&ignorar, 0, sizeof(ignorar));
        memset(&sigur, 0, sizeof(sigur));
       
       //Manejador de Señales
        ignorar.sa_handler = SIG_IGN;
        ignorar.sa_flags = SA_RESTART; 
        sigur.sa_handler = gestion;
        sigur.sa_flags = SA_RESTART;
        
        sigaction(SIGINT, &ignorar, NULL);
        sigaction(SIGUSR1, &sigur, NULL);
        sigaction(SIGUSR2, &sigur, NULL);

        fork();
        if(getpid() != padre){
            hijo2 = getpid(); //Envio de señal SGINT por el hijo 2
            printf("\nSoy el hijo 2: %d\n",hijo2);
            sleep(5);
            printf("\nEnviando señal SIGINT desde el hijo 2\n");
            print_time();
            kill(padre,SIGINT);
            exit(0);
        }
    }else{ //Envio de señales SIGUSR1 Y SIGUSR2 por el hijo 1
        hijo1 = getpid();
        printf("\nSoy el hijo 1: %d\n",hijo1);
        sleep(1);
        printf("\nEnviando señal SIGUSR1\n");
        print_time();
        kill(padre,SIGUSR1);
        sleep(20);
        printf("\nEnviando señal SIGUSR2\n");
        print_time();
        kill(padre,SIGUSR2);
        sleep(20);
        exit(0);
    }
    
    //Bloqueo de la señal 1
    sigemptyset(&bloqueadas);
    sigaddset(&bloqueadas, SIGUSR1);
    sigprocmask(SIG_BLOCK, &bloqueadas, NULL);
    
    sleep(25);

    sigpending(&pendientes);
    if (sigismember(&pendientes, SIGUSR1)) {
        printf("\nPadre: SIGUSR1 está pendiente.\n");
        print_time();
    } else {
        printf("\nPadre: SIGUSR1 no está pendiente.\n");
    }
    printf("\nPadre: Señal SIGUSR1 desbloqueada.\n");
    print_time();
    sigprocmask(SIG_UNBLOCK, &bloqueadas, NULL); //Desbloqueo de la señal
    
    waitpid(hijo1,NULL,0);

    return 0;
}
