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
#define NUM_ITEMS 50

mqd_t almacenP; // Cola productor
mqd_t almacenC; // Cola consumidor
int T, fd;

int aleatorio(){
    return rand() % (T + 1);
}

void consumidor() {
    char item;
    char mensaxe = 'A';

    //Le decimos al productor que ya estamos esperando
    mq_send(almacenP, &mensaxe, sizeof(char), 0);
    while (1) {
        
        usleep(aleatorio());

        //Recibe el mensaje del productor
        mq_receive(almacenC, &item, sizeof(char), NULL);

        //Le devuelve un mensaje para confirmar que le llego
        mq_send(almacenP, &mensaxe, sizeof(char), 0);

        //Si el mensaje era el últim del archivo, se cierra
        if(item == '0'){
            return;
        }
        printf("Item consumido: %c\n", item);
        //Si no, se escribe en el archivo del consumidor
        if (write(fd, &item, 1) == -1) {
            perror("Error al escribir");
        }
    }
}

int main(int argc, char** argv){

    struct mq_attr attr;

    attr.mq_flags = 0; // 0 => bloqueante
    attr.mq_maxmsg = TAMANO_BUFFER; // Máximo número de mensaxes
    attr.mq_msgsize = sizeof(char); // Tamaño de cada mensaxe

    almacenP = mq_open("/almacenP", O_CREAT | O_WRONLY, 0777, &attr);
    almacenC = mq_open("/almacenC", O_CREAT | O_RDONLY, 0777, &attr);

    if (almacenP == -1 || almacenC == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    //Comrpobamos el numero de argumentos, si es el correcto, usamos el primero para abrir el archivo y el segundo
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
    
    consumidor();
    
    mq_close(almacenC);
    
    return 0;
}