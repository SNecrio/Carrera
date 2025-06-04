#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    int a;
    int fp = open("hola.txt",O_RDWR);
    
    struct stat estats;
    fstat(fp,&estats);

    printf("El PID es: %d", getpid());
    scanf("%d",&a);

    char *map = mmap(NULL,estats.st_size,PROT_READ | PROT_WRITE, MAP_SHARED,fp, 0);

    scanf("%d",&a);

    close(fp);

    printf("Contenido del archivo: \n\n");
    fwrite(map, 1, estats.st_size, stdout);
    printf("\n\n");

    munmap(map,estats.st_size);

    return 0;
}
