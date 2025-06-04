#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    
    int fp = open("hola.txt",O_RDWR);
    
    struct stat estats;
    fstat(fp,&estats);

    char *map = mmap(NULL,estats.st_size,PROT_READ | PROT_WRITE, MAP_SHARED,fp, 0);

    close(fp);

    printf("Contenido del archivo: \n\n");
    fwrite(map, 1, estats.st_size, stdout);
    printf("\n\n");

    map[0] = 'S';

    fwrite(map, 1, estats.st_size, stdout);
    printf("\n\n");

    munmap(map,estats.st_size);

    return 0;
}
