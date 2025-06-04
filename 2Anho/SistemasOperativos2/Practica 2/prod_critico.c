#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define TAMANO_BUFFER 8
#define NUM_ITEMS 50 //Numero de iteraciones maximas del bucle

typedef struct{
    char buffer[TAMANO_BUFFER]; //bufer compartido
    volatile int cuenta; //Numero de elementos actuales del buffer
    volatile int wakeConsum; //Flag para despertar al consumidor (0=espera, 1=despierta)
    volatile int wakeProduc; //Flag para despertar al productor (0=espera, 1=despierta)
} SharedMemory;

//Funcion que genera y devuelve una letra aleatoria
char produceItem(){

    srand(time(NULL)); // Inicializa la semilla aleatoria
    char letra = 'A' + (rand() % 26); 
    return letra;
}

int main(int argc, char** argv){

    //Buffer que llenamos de todos los items generados
    char bufferLocal[NUM_ITEMS];

    //Abrimos el archivo, creandolo si hace falta y con permisos de leer y escribir
    int fd = open("archivo", O_CREAT | O_RDWR, 0666);
    if (fd == -1) { //Comprobacion de error al abrir el archivo
        perror("Error al abrir el archivo");
        exit(1);
    }

    //Ajusta el tamaño del archivo
    ftruncate(fd, sizeof(SharedMemory));

    //Mapeamos una instancia del struct sobre el archivo para guardar ahi los datos en ambos programas
    SharedMemory* compartido = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (compartido == MAP_FAILED) { //Comprobacion de error al mapear
        perror("Error en mmap");
        exit(1);
    }

    //Cerramos el puntero al archivo ya que al estar mapeado ya no lo necesitamos mas y asi evitamos
    //fugas de memoria
    close(fd);

    //Ponemos la cuenta a 0 al principio
    compartido->cuenta = 0;

    //Comenzamos el bucle, que itera el numero maximo de items
    for(int i=0; i<NUM_ITEMS; i++){
        
        char item = produceItem(); //Genera un item

        int cuentalocal = compartido-> cuenta; //Lee el valor de la cuenta del buffer y la guarda localmente
        
        //Comprueba el valor leido para ver si el buffer esta lleno
        //Si es asi, espera a que lo despierte el consumidor
        if(cuentalocal == TAMANO_BUFFER){ 
            compartido->wakeProduc = 0;
            while(compartido->wakeProduc== 0){}
        }

        //sleep(1); Soluciona en gran parte a carreira critica
        //cuentalocal = compartido->cuenta;
        
        //Se hace un sleep para asegurarse de que ocurre la carrera critica
        //Al hacerlo, el valor leido de cuenta se queda obsoleto para cuando el consumidor lo cambia
        sleep(3);

        //Coloca el item en el siguiente lugar (obsoleto, ya que seguramente el consumidor ya consumio el anterior)
        compartido->buffer[cuentalocal]=item;
        //Colocamos el item tambien en el buffer local para el final
        bufferLocal[i]=item;

        //Actualiza el valor de la cuenta en la memoria compartida
        compartido->cuenta = cuentalocal + 1;

        printf("\tItem producido en espacio %d -> %c\n", cuentalocal, item);
        
        //Si la cantidad de objetos en la memoria es 1 (antes 0) se despierta al consumidor
        if(cuentalocal >=  0){
            compartido->wakeConsum = 1;
        }
    }

    //Se cierra la memoria compartida
    munmap(compartido, sizeof(SharedMemory));

    return 0;
}
