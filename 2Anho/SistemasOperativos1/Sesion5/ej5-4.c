#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int a;
    
    printf("PID del proceso: %d.\n", getpid());
    pid = fork();

    if (pid == -1) {
        // Error en fork
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Proceso hijo
        int *array;
        printf("Proceso hijo: %d\n", getpid());
        // Mostrar la dirección inicial de la memoria
        printf("Dirección inicial: %p\n", (void*)array);
        scanf(" %d", &a);
        // Realizar un malloc
        array = (int *)malloc(10 * sizeof(int));
        printf("Malloc realizando\n");

        printf("Dirección post-malloc: %p\n", (void *)array);
        scanf(" %d", &a);

        printf("Procediendo a realizar execv\n");
        // El programa a ejecutar
        char *path = "./ejemplo.out";

        // Argumentos para el programa
        char *args[] = { "./ejemplo.out", NULL };
        // Usar execv para cambiar la imagen del proceso hijo
        // Ejecutar el programa
        if (execv(path, args) == -1) {
            perror("Error al ejecutar execv");
            exit(1);
        }
        
        scanf(" %d", &a);
        

        exit(EXIT_FAILURE);
    } 
    
    // Esperamos a que el hijo termine
    wait(NULL);

    

    return 0;
}
