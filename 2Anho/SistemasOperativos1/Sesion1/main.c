#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

int CANT = 70000000;

double calcularMedia(int iterador, int hijo, int paso, FILE* fp){
    double suma = 0.0;
    int maximo = CANT;

    struct timeval inicio, final;
    double tiempo;

    gettimeofday(&inicio, NULL);
    
    while (iterador <= maximo){
        double angulo = sqrt(iterador); //raíz do numero enteiro
        suma += tan(angulo); //tangente do angulo
        iterador = iterador + paso;
    }

    //calcular a media
    double media = suma /(iterador);

    gettimeofday(&final, NULL); 

    //calculamos o tempo que lle levou
    tiempo= (final.tv_sec-inicio.tv_sec + (final.tv_usec-inicio.tv_usec)/1.e6);

    fprintf(fp, "Soy el hijo %d con pid %d, el resultado es %f y el tiempo que me ha llevado es %f\n", hijo, getpid(), media, tiempo);
    printf("Soy el hijo %d con pid %d, el resultado es %f y el tiempo que me ha llevado es %f\n", hijo, getpid(), media, tiempo);
    fclose(fp);
    return media; 
}

double mediaCombinada (FILE* fp){
    double media = 0.0;
    double tiempo1 = 0.0;
    double tiempo2 = 0.0;

    rewind(fp);
    //variables para almacenar os valores extraidos
    int hijo, pid;
    double resultado=0.0; 
    
    //definicions para calcular o tempo
    struct timeval inicio, final;
    double tiempo=0.0;

    //abrimos arquivo en modo lectura

    char linea[256];
    
    gettimeofday(&inicio, NULL);

    //leemos linea por linea
    while(fgets(linea, sizeof(linea), fp)!= NULL){
        
        int leidos = sscanf(linea, "Soy el hijo %d con pid %d, el resultado es %lf y el tiempo que me ha llevado es %lf", &hijo, &pid, &resultado, &tiempo);
        if (leidos ==4){
            if((hijo == 1)||(hijo == 2)){
                media = media + resultado;
            }
        }
        else{
            fprintf(fp, "ERROR NON LEOs CATRO");
        }

    }
    gettimeofday(&final, NULL);

    //calculamos o tempo que lle levou
    tiempo= (final.tv_sec-inicio.tv_sec + (final.tv_usec-inicio.tv_usec)/1.e6);
    
    
    fprintf(fp, "Soy el hijo 4 con pid %d, el resultado es %lf y el tiempo que me ha llevado es %lf\n", getpid(), media, tiempo);
    printf("Soy el hijo 4 con pid %d, el resultado es %lf y el tiempo que me ha llevado es %lf\n", getpid(), media, tiempo);
    fclose(fp);
    return media;
}


int main(){

    pid_t pid0= getpid();
    pid_t pid1= 0;
    pid_t pid2= 0;
    pid_t pid3= 0;
    pid_t pid4= 0;

    FILE *fp = fopen("resultados.txt","a+");

    printf("Yo soy el padre y mi pid es %d\n", pid0);
    fork();
    if(pid0 == getpid()){
        fork();
        if(pid0 == getpid()){
            fork();
            if(pid0 == getpid()){
            }else{
                pid3 = getpid();
                printf("Yo soy el hijo3 y mi pid es %d\n", pid3);
            }
        }else{
            pid2 = getpid();
            printf("Yo soy el hijo2 y mi pid es %d\n", pid2);
        }
    }
    else{
        pid1 = getpid();
        printf("Yo soy el hijo1 y mi pid es %d\n", pid1);
    }

    if(pid1== getpid()){
        double result_pid1 = calcularMedia(1, 1, 2,fp);
        exit;
    }
    
    if (pid2== getpid()){
        double result_pid2 = calcularMedia(0,2,2,fp);
        exit;
    }
    
    if(pid3==getpid()){
        double result_pid3 = calcularMedia(0, 3, 1,fp);
        exit;
    }
    
    //esperar a que os fillos 1 e 2 terminen
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0); 


    if(pid0 == getpid()){
        fork();
        if(pid0 != getpid()){
            printf("Yo soy el hijo4 y mi pid es %d\n", pid4);
            double result_pid4 = mediaCombinada(fp);
            exit;
        }
    }
    
    waitpid(pid3, NULL, 0);
    waitpid(pid4, NULL, 0);

    char linea[256];
    int hijo, pid;
    double tiempo, tiempo3, tiempo4, resultado, resultado3, resultado4;

    rewind(fp);
    if(getpid() == pid0){
        while(fgets(linea, sizeof(linea), fp)!= NULL){
        
            int leidos = sscanf(linea, "Soy el hijo %d con pid %d, el resultado es %lf y el tiempo que me ha llevado es %lf", &hijo, &pid, &resultado, &tiempo);
            if (leidos ==4){
                if(hijo == 3){
                    tiempo3 = tiempo;
                    resultado3 = resultado;
                }
                if(hijo == 4){
                    tiempo4 = tiempo;
                    resultado4 = resultado;
                }
            }
            else{
                fprintf(fp, "ERROR NON LEO CATRO");
            }

        }

        printf("\nLa diferencia de valor entre el hijo 3 y el hijo 4 es de %lf\n", resultado3 - resultado4);
    }

    fclose(fp);
    
    return 0;
}