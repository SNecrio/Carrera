#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

void *hablar(void* hola){

    printf("\n%s\n",hola);

    pthread_exit(NULL);
}


int main(){

    char* hola = "Mecago";
    pthread_t fernando;
    pthread_create(&fernando,NULL,hablar,(void *)hola);


    pthread_join(fernando, NULL);

    return 0;
}