#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int asteriscosleidos = 0,P,C,N,puntero = 0;

char *buffer;
pthread_barrier_t barrier;
int prodActual = 0;

pthread_mutex_t lock;
pthread_mutexattr_t attrlock;
pthread_cond_t cond_no_vacio = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_no_lleno = PTHREAD_COND_INITIALIZER;


void produceItem(int fd, char* bufLocal){   
    read(fd, bufLocal, 1); //Leemos un caracter

}
void consumeItem(int fd,char actual){
    if(actual != '*') write(fd, &actual, 1); //Escribimos un item en el archivo de output
}

void insertItem(int fd, char bufLocal){
    
    if((bufLocal >= '0' && bufLocal <= '9') || 
       (bufLocal >= 'A' && bufLocal <= 'Z') || 
       (bufLocal >= 'a' && bufLocal <= 'z') || 
       bufLocal == '*') { //Comprobamos que lo que leimos fue un caracter alfanumerico
        
        //Movemos toda la cola un espacio atras e introducimos el nuevo elemento en el buffer
        for(int i = N -1; i > 0 ; i--){
            buffer[i] = buffer[i-1];
        }
        buffer[0] = bufLocal;
        puntero++;
        
        //Si es un asterisco, aumentamos el numero de asteriscos leids
        if(bufLocal == '*'){
            asteriscosleidos++;
        }
    }    
}

char removeItem(int fd){
    //Leemos el caracter actual y actualizamos la posicion del puntero
    char actual = buffer[puntero-1];
    puntero--;
    return actual;
}


void* productor(void *vnproductor){

    //Convertimos los argumentos y declaramos las variables
    int nproductor=*((int *)(vnproductor));
    free(vnproductor);

    //Definimos el buffer
    char bufLocal;

    char nombreArchivo[50];
    int T;
    int fd; //Puntero al archivo

    while(prodActual < nproductor); //Espera activa de cada productor

    printf("\nEscriba el nombre del archivo para el hilo %d: ",nproductor);
    scanf("%s",nombreArchivo);
    printf("\nIntroduzca el retardo maximo para el hilo %d: ", nproductor);
    scanf("%d",&T);
    
    if((fd=open(nombreArchivo,O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO)) < 0){ //Leemos el archivo y comprobamos que se lee bien
		perror("Error abriendo el archivo de productor\n");
		exit(EXIT_FAILURE);
	}

    prodActual++; //Se aumenta el numero para dejar pasar al siguiente productor
    srand(time(0) + nproductor); //Se aleatoriza la semilla para que no todos los productores tengan los mismos valores, por si acaso

    pthread_barrier_wait(&barrier); //Con una barrera conseguimos que todos los productores esperen antes de ponerse a producir
    
    do{
        sleep(rand() % T); //Retardo antes de leer
        
        pthread_mutex_lock(&lock);
        
        while(puntero == N){
            pthread_cond_wait(&cond_no_lleno, &lock);
        }

        produceItem(fd, &bufLocal); //Producimos (leemos del archivo) un item
        insertItem(fd, bufLocal); //Insertamos (ponemos en el archivo) un item
        pthread_cond_signal(&cond_no_vacio);

        pthread_mutex_unlock(&lock);
        
    }while(bufLocal != '*'); //Al leer un asterisco, se acaba el bucle de producir, ya que señaliza el final del archivo

    printf("\nAcabo hilo %d\n", nproductor);
    printf("Se han leido %d asteriscos\n",asteriscosleidos);

    pthread_cond_broadcast(&cond_no_vacio);
    return NULL;
}


void* consumidor(void *nConsumidor){
    
    //Convertimos los argumentos y declaramos las variables
    int n = *((int*)(nConsumidor));
    free(nConsumidor);
    int fd;

    char nombreArchivo[30];
    char actual;
    sprintf(nombreArchivo, "archivoCons_%d.txt", n);

    //Abrimos el archivo
    if((fd=open(nombreArchivo,O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO)) < 0){
        perror("Error abriendo el archivo de consumidor\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        
        //Cerramos el mutex ya que vamos a modificar variables criticas
        pthread_mutex_lock(&lock);

        //Esperar hasta que haya elementos o todos los productores hayan terminado
        while (puntero == 0 && asteriscosleidos < P) {
            pthread_cond_wait(&cond_no_vacio, &lock);
        }

        //Si el buffer esta vacio y se han leido todos los asteriscos, se acaba el bucle de los consumidores
        if (puntero == 0 && asteriscosleidos >= P) {
            pthread_mutex_unlock(&lock);
            break;
        }

        //Consumimos el item en la siguiente posicion del buffer
        actual = removeItem(fd);
        consumeItem(fd, actual);

        //Señalizamos que el buffer no esta lleno y abrimos el mutex
        pthread_cond_signal(&cond_no_lleno);
        pthread_mutex_unlock(&lock);
    }

    //Liberamos memoria
    close(fd);
    return NULL;
}


int main(int argc,char **argv){
    int fd,*nhilo,i;
    void *buffervoid;

    if(argc!=4){
        printf("Error: el programa debe ejecutarse escribiendo en la línea de comandos \"%s [número de productores] [número de consumidores] [número de carácteres que caben en el buffer]\".\n",argv[0]);
        perror("Número de argumentos incorrecto.\n");
        exit(EXIT_FAILURE);
    }
    P=atoi(argv[1]);    //Numero de productores
    C=atoi(argv[2]);    //Numero de consumidores
    N=atoi(argv[3]);    //Tamaño del buffer

    pthread_t productores[P];
    pthread_t consumidores[C];

    //Inicializamos la barrera
    pthread_barrier_init(&barrier, NULL, P);

    //Inicializamos los mutexes y comprobamos que se crean exitosamente
    if(pthread_mutex_init(&lock,NULL) != 0){
        perror("Error creando el mutex\n");
        exit(EXIT_FAILURE);
    }
    
    if((fd=open("buffer.txt",O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO)) < 0){ //Abrimos el archivo.
		perror("Error abriendo el archivo.\n");
		exit(EXIT_FAILURE);
	}
    if(ftruncate(fd, N) == -1){   //Truncamos el archivo.
		perror("Error al hacer ftruncate.\n");
        exit(EXIT_FAILURE);
	}
    if((buffervoid=mmap(NULL,N,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){   //Hacemos el mapeo de memoria.
        perror("Error mapeando el archivo.\n");
        exit(EXIT_FAILURE);
    }

    buffer=(char *)buffervoid;  //Hacemos cast a buffervoid.

    //Creamos los hilos productores
    for(i=0;i<P;i++){
        nhilo=malloc(sizeof(int));
        *nhilo=i;
        pthread_create(&productores[i],NULL,productor,nhilo);
    }
    //Creamos los hilos consumidores
    for(i=0;i<C;i++){
        nhilo=malloc(sizeof(int));
        *nhilo=i;
        pthread_create(&consumidores[i],NULL,consumidor,nhilo);
    }

    //Esperamos a los hilos
    for(i=0;i<P;i++){
        pthread_join(productores[i],NULL);
    }
    for(i=0;i<C;i++){
        pthread_join(consumidores[i],NULL);
    }

    munmap(buffer, N);
    close(fd);

    return(EXIT_SUCCESS);
}