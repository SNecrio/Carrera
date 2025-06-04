#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>   
#include <mqueue.h>  

#define TAMANO_BUFFER 5
#define NUM_ITEMS 50 //Numero de iteraciones maximas del bucle

int T, fd;
mqd_t almacenP; // Cola productor
mqd_t almacenC; // Cola consumidor


int aleatorio(){
    return rand() % (T + 1);
}

void productor() {
    char item;
    char mensaxe;
    
    while (1) {
        
        usleep(aleatorio());
        //Se produce un item leyendolo del archivo
        if (read(fd, &item, 1) != 1){
            char c = '0';
            mq_send(almacenC, &c, sizeof(char), 0);
            return;
        }

        printf("Item producido: %c\n", item);

        mq_receive(almacenP, &mensaxe, sizeof(char), NULL);
        mq_send(almacenC, &item, sizeof(char), 0);
    }
}

int main(int argc, char** argv){
    struct mq_attr attr;

    attr.mq_flags = 0; // 0 => bloqueante
    attr.mq_maxmsg = TAMANO_BUFFER; // Máximo número de mensaxes
    attr.mq_msgsize = sizeof(char); // Tamaño de cada mensaxe

    mq_unlink("/almacenP");

    almacenP = mq_open("/almacenP", O_CREAT | O_RDONLY, 0777, &attr);
    almacenC = mq_open("/almacenC", O_CREAT | O_WRONLY, 0777, &attr);

    if (almacenP == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    //Comprobamos el numero de argumentos, si es el correcto, usamos el primero para abrir el archivo y el segundo
    //como tiempo máximo de retardo
    if(argc < 2){
        perror("Hace falta un archivo que se pase como argumento y el tiempo de retardo en microsegundos\n");
        perror("Ejemplo de ejecucion:\n ./prod fichero.txt 10000\n");
        exit(1);
    }

    fd = open(argv[1], O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    srand(time(NULL));

    T = atoi(argv[2]);

    productor();

    mq_close(almacenP);

    return 0;
}