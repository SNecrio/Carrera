#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

void * proceso_pausa(){
    printf("%d\n", getpid());
    sleep(10);
    printf("acabo\n");
    pthread_exit(NULL);
}

void * proceso_hijo(){
    fork();
    printf("%d\n", getpid());
    sleep(10);
    pthread_exit(NULL);
}

int main(){

    pthread_t pro1;
    pthread_t pro2;
    pthread_t pro3;
    printf("Hola%d\n", getpid());

    pthread_create(&pro1,NULL,proceso_pausa,NULL);
    pthread_create(&pro2,NULL,proceso_pausa,NULL);
    pthread_create(&pro3,NULL,proceso_hijo,NULL);

    pthread_join(pro1, NULL);
    pthread_join(pro2, NULL);
    pthread_join(pro3, NULL);

    sleep(100);


    return 0;
}