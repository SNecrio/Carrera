#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void hola(){}

int main(){

    pid_t padre = getpid();
    pid_t hijo = fork();

    signal(SIGUSR1,&hola);
    if(hijo == 0){
        pause();
        printf("\nSaliendo del pause\n");
        exit(0);
    }else{
        printf("PID del padre: %d\n", padre);
        printf("PID del hijo: %d\n", hijo);

        sleep(4);
        kill(hijo,SIGUSR1);
    }

    waitpid(hijo,NULL,0);
    printf("\nSaliendo del programa\n");

    return 0;
}