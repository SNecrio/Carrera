#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    
    int fp = open(argv[1],O_RDWR);
    if(fp ==-1){
        perror("Error abriendo archivo\n");
        return 1;
    }

    struct stat estad;
    if(fstat(fp, &estad) == -1){
        perror("Error obteniendo estadisticas\n");
        close(fp);
        return 1;
    }

    printf("\nLongitud del archivo: %ld\n",estad.st_size);


    printf("Contenido del archivo (leído directamente):\n");
    for (long i = 0; i < estad.st_size; i++) {
        char c;
        if (read(fp, &c, 1) == -1) {
            perror("Error al leer el archivo");
            close(fp);
            return 1;
        }
        putchar(c);
    }
    putchar('\n');putchar('\n');

    //Mapear el archivo en la memoria
    void *map = mmap(NULL, estad.st_size, PROT_READ, MAP_PRIVATE, fp, 0);
    if (map == MAP_FAILED) {
        perror("Error al proyectar el archivo en memoria");
        close(fp);
        return 1;
    }

    //Cerrar el archivo
    if (close(fp) == -1) {
        perror("Error al cerrar el archivo");
        munmap(map, estad.st_size);
        return 1;
    }

    //Imprimir desde la proyeccion
    printf("Contenido del archivo (desde la proyección en memoria):\n");
    char *contenido = (char *)map;
    for (size_t i = 0; i < estad.st_size; i++) {
        putchar(contenido[i]);
    }
    putchar('\n');putchar('\n');

    // Desmapear la memoria
    if (munmap(map, estad.st_size) == -1) {
        perror("Error al desmapear la memoria");
        return 1;
    }

    return 0;
}