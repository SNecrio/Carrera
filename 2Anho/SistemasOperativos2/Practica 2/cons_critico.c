#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define TAMANO_BUFFER 8
#define NUM_ITEMS 50

typedef struct{
    char buffer[TAMANO_BUFFER]; //bufer compartido
    volatile int cuenta; //número elementos no buffer
    volatile int wakeConsum;
    volatile int wakeProduc;
} SharedMemory;


int main(int argc, char** argv){

    //Se abre y crea el archivo si hace falta
    int fd = open("archivo", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    // Ajustar el tamaño del archivo
    ftruncate(fd, sizeof(SharedMemory));

    //Mapeamos una instancia del struct sobre el archivo para guardar ahi los datos en ambos programas
    SharedMemory* compartido = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (compartido == MAP_FAILED) {
        perror("Error en mmap");
        exit(1);
    }

    //Cerramos el puntero al archivo ya que al estar mapeado ya no lo necesitamos mas y asi evitamos
    //fugas de memoria
    close(fd);

    char actual;
    for(int i = 0; i < NUM_ITEMS; i++){

        //Lee el valor de la cuenta del buffer y la guarda localmente
        int cuentalocal = compartido->cuenta;
                
        //Comprueba el valor leido para ver si el buffer esta vacio
        //Si es asi, espera a que lo despierte el productor
        if(cuentalocal == 0){
            compartido->wakeConsum = 0;
            while(compartido->wakeConsum == 0){}
        }

        //Hacemos una espera para asegurarnos de que el valor leido al principio y el usado posteriormente
        //esten desincronizados la mayoria del tiempo
        sleep(2);

        //Leemos el item del buffer y actualizamos la cuenta local
        actual = compartido->buffer[cuentalocal-1]; //recoger el item
        compartido->cuenta = cuentalocal - 1; //Cambiar el valor de cuenta

        //Lo imprimimos
        printf("\tItem consumido en espacio %d -> %c\n", cuentalocal-1, actual);

        //Si el buffer estaba lleno, al consumir uno se debe despertar al productor
        if(cuentalocal <= TAMANO_BUFFER -1){
            compartido->wakeProduc = 1;
        }

    }

    munmap(compartido, sizeof(SharedMemory));

    return 0;
}