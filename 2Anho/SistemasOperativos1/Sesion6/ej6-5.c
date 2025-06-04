#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
    int fp = open("hola.txt",O_RDWR);
    
    struct stat estats;
    fstat(fp,&estats);

    printf("El PID es: %d", getpid());

    char *map = mmap(NULL,estats.st_size,PROT_READ | PROT_WRITE, MAP_SHARED,fp, 0);

    printf("Contenido del archivo: \n\n");
    fwrite(map, 1, estats.st_size, stdout);
    printf("\n\n");


    printf("Contenido del archivo (desde la proyección en memoria y en menorculas):\n");

    char *nuevo = malloc(estats.st_size * 10);
    char c;
    int a = 0;

    for (size_t i = 0; i < estats.st_size; i++) {
        c = map[i];
        if(c >= 65 && c <= 90){
            c = c + 32;
        }
        printf("%c",c);

        if(isdigit(c)){
            int dig = c -'0';
            for(int j = 0; j < dig; j++){
                nuevo[a] = '*';
                a++;
            }
        }else{
            nuevo[a] = c;
            a++;
        }
        

    }printf("\n\n");

    munmap(map,estats.st_size);

    ftruncate(fp,a);

    char *nmap = mmap(NULL,a,PROT_READ | PROT_WRITE, MAP_SHARED,fp, 0);

    for (size_t i = 0; i < a; i++)
        nmap[i] = nuevo[i];

    munmap(nmap,a);

    return 0;
}
