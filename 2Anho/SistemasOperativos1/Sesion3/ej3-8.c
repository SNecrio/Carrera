/*
* PRACTICA 3: USO DE SEÑALES
* ENTREGABLE 4
* VALERIA ITZEL CONTRERAS MIRANDA
* SERGIO SOUTO MOURELLE
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

FILE* fp;

//Esta funcion solo sirve de auxiliar para poder cambiar con kill()

void cambio(int sig){}

//Al arbitro solo se llama cuando se acaba un punto, asi que sus señales siempre indican un punto y siempre escribe un -1 para servir o un -3 para acabar la partida

void arbitro(pid_t jugador, pid_t maquina){

    //Espera a que lo llame el jugador

    pause();
    int puntosJugador = 0, puntosMaquina = 0;

    //Abre el archivo y escribe el codigo -1, que significa que el jugador que lo reciba tiene que servir

    fp = fopen("Ping.txt","w+");
    fprintf(fp,"-1");
    fclose(fp);

    //Elije aleatoriamente que jugador comienza y le cede el turno

    int opcion = rand() % 2;
    if(opcion == 1){
        kill(jugador,SIGHUP);
    }else{
        kill(maquina,SIGHUP);
    }
    pause();

    while(1){

        //Lee el archivo

        fp = fopen("Ping.txt","a+");
        fscanf(fp,"%d",&opcion);
        fclose(fp);

        //Si la opcion es -2, significa que gano el jugador, si es -1, gano la maquina

        if(opcion == -2){
            puntosJugador++;
            printf("\nPunto para el jugador!\n");
        }else{
            puntosMaquina++;
            printf("\nPunto para la maquina!\n");
        }

        //Escribe en el archivo un -1, que para los jugadores significa servir

        fp = fopen("Ping.txt","w+");
        fprintf(fp,"-1");
        fclose(fp);

        //Escribe la puntuacion actual y revisa si alguien gano, cuando alguien gana, escribe -3 en el archivo y llama a ambos jugadores para que acaben
        
        printf("\nResultado actual:\n\tJugador: %d puntos\n\tMaquina: %d puntos\n",puntosJugador,puntosMaquina);
        if(puntosJugador >= 10){
            printf("\nHa ganado el jugador!\n");

            fp = fopen("Ping.txt","w+");
            fprintf(fp,"-3");
            fclose(fp);
            
            kill(jugador,SIGHUP);
            kill(maquina,SIGHUP);
            return;
        }
        if(puntosMaquina >= 10){
            printf("\nHa ganado la maquina...\n");

            fp = fopen("Ping.txt","w+");
            fprintf(fp,"-3");
            fclose(fp);

            kill(jugador,SIGHUP);
            kill(maquina,SIGHUP);
            return;
        }

        //Si nadie gano, elije aleatoriamente quien sigue, lo llama, y se pausa

        opcion = rand() % 2;
        if(opcion == 1){
            kill(jugador,SIGHUP);
        }else{
            kill(maquina,SIGHUP);
        }
        pause();
    }
}

void player(pid_t maquina){

    int playerPos;
    int newPos;
    int posPelota;
    int servir = 0;

    //Conseguimos el pid del padre, que usaremos para mandar señales al arbitro
    pid_t arbitro = getppid();

    //Leemos la posicion del jugador
    do{
        printf("\nIntroduzca posicion de comienzo: ");
        scanf("%d", &playerPos);
    }while(playerPos < 0 || playerPos > 9);

    //Se pasa el turno al arbitro para que decida quien saca
    kill(arbitro,SIGHUP);
    pause();

    while(1){

        //Lee el archivo

        printf("\nTurno del jugador\n");
        fp = fopen("Ping.txt","a+");
        fscanf(fp,"%d",&posPelota);
        fclose(fp);

        //Si en el archivo pone 0--9, es una posicion de pelota, si pone -1 es que es un nuevo punto y toca servir,
        //si pone -3 es que acaba la partida y sale de la funcion

        if(posPelota == -3){
            return;
        }

        //Si es -1, sirve, donde pide posicion a donde servir y lo escribe en el archivo, luego llama a la maquina y se pausa

        if(posPelota == -1){
            printf("\nSirve el jugador, introduzca posicion a la que enviar la pelota: ");
            scanf("%d",&posPelota);
            fp = fopen("Ping.txt","w+");
            fprintf(fp,"%d",posPelota);
            fclose(fp);

            kill(maquina,SIGHUP);
            pause();
        }
        
        //Si es una posicion normal

        else{
            printf("\nLa pelota esta en la posicion %d\n", posPelota);

            //Primero se comprueba si se logra golpear la pelota, si no, se escribe -1 en el archivo, se mueve el jugador y se llama de vuelta al arbitro

            if(posPelota - playerPos < -3 || posPelota - playerPos > 3){
                printf("\nEl jugador no logro darle a la pelota\n");
                fp = fopen("Ping.txt","w+");
                fprintf(fp,"-1");
                fclose(fp);

                //Deja moverse despues de fallar, para moverse, se introduce una posicion y se comprueba que es valida

                do{
                    printf("\nSu posicion actual es %d\nIntroduzca su nueva posicion (puede moverse 2 pasos a cada lado): ",playerPos);
                    scanf("%d",&newPos);
                }while((playerPos - newPos < -2 || playerPos - newPos > 2 ) || (newPos > 9 || newPos < 0));
                playerPos = newPos;

                kill(arbitro,SIGHUP);
                pause();
            }
            
            //Si se logra golpear, se pide a que posicion se quiere mandar, se escribe en el archivo y se llama a la maquina, tambien deja moverse.
            
            else{
                printf("\nEl jugador le dio a la pelota\n");

                printf("Introduzca posicion a la que enviar la pelota: ");
                scanf("%d",&posPelota);
                fp = fopen("Ping.txt","w+");
                fprintf(fp,"%d",posPelota);
                fclose(fp);

                do{
                    printf("\nSu posicion actual es %d\nIntroduzca su nueva posicion (puede moverse 2 pasos a cada lado): ",playerPos);
                    scanf("%d",&newPos);
                }while((playerPos - newPos < -2 || playerPos - newPos > 2 ) && (newPos <= 9 && newPos >=0));
                playerPos = newPos;

                kill(maquina,SIGHUP);
                pause();
            }
        }
    }
}

void cpu(pid_t jugador){

    //Se calcula la posicion inicial de la maquina de manear aleatoria entre 0 y 9

    int posMaquina = rand() % 10;

    int posPelota;

    //Conseguimos el pid del padre, que usaremos para mandar señales al arbitro,
    //luego se pausa para esperar al jugador y al arbitro

    pid_t arbitro = getppid();

    pause();
    
    while(1){

        //Lee el archivo

        printf("\nTurno de la maquina\n");
        fp = fopen("Ping.txt","a+");
        fscanf(fp,"%d",&posPelota);
        fclose(fp);

        //Si en el archivo pone 0--9, es una posicion de pelota, si pone -1 es que es un nuevo punto y toca servir,
        //si pone -3 es que acaba la partida y sale de la funcion

        if(posPelota == -3){
            return;
        }

        //Si es -1, sirve, luego llama al jugador y se pausa

        if(posPelota == -1){
            printf("\nSirve la maquina\n");
            posPelota = rand() % 10;
            fp = fopen("Ping.txt","w+");
            fprintf(fp,"%d",posPelota);
            fclose(fp);

            kill(jugador,SIGHUP);
            pause();
        }
        
        //Si es una posicion, comprueba si la golpea
        
        else{
            printf("\nLa pelota esta en la posicion %d\n", posPelota);

            //Si no golpea a la pelota, escribe -2 en el archivo y llama al arbitro, no sin antes calcular una nueva posicion

            if(posPelota - posMaquina < -3 || posPelota - posMaquina > 3){
                printf("\nLa maquina no logro darle a la pelota\n");
                fp = fopen("Ping.txt","w+");
                fprintf(fp,"-2");
                fclose(fp);
                
                posMaquina = (posMaquina-2) + rand() % ((posMaquina + 2) - (posMaquina-2) + 1);
                if(posMaquina > 9){
                    posMaquina = 9;
                }
                if(posMaquina < 0){
                    posMaquina < 0;
                }

                kill(arbitro,SIGHUP);
                pause();
            }
            
            //Si la golpea, calcula la posicion a la que mandar la pelota, la escribe en el archivo,
            //se mueve y llama al jugador
            
            else{
                printf("\nLa maquina le dio a la pelota\n");

                posPelota = rand() % 10;
                fp = fopen("Ping.txt","w+");
                fprintf(fp,"%d",posPelota);
                fclose(fp);
                
                posMaquina = (posMaquina-2) + rand() % ((posMaquina + 2) - (posMaquina-2) + 1);
                if(posMaquina > 9){
                    posMaquina = 9;
                }
                if(posMaquina < 0){
                    posMaquina < 0;
                }

                kill(jugador,SIGHUP);
                pause();
            }
        }
    }
}


int main(){

    //Definimos los pids y definimos la semilla del generador de numeros aleatorios

    pid_t padre = getpid();
    pid_t hijo1;
    pid_t hijo2;
    srand(time(0));

    //Creamos la señal

    signal(SIGHUP, &cambio);

    //Hacemos los 2 forks y mandamos a cada hijo por un camino y a una funcion

    hijo1 = fork();

    if(getpid() == padre){
        printf("\nSoy el padre: %d\n",padre);

        hijo2 = fork();
        if(getpid() != padre){
            //El hijo2 sera la maquina, obtiene su pid y lo escribe en el archivo, luego duerme un poco
            hijo2 = getpid();
            printf("\nSoy el hijo 2, sere la maquina: %d\n",hijo2);

            fp = fopen("Ping.txt","w+");
            fprintf(fp,"%d",getpid());
            fclose(fp);
            sleep(2);

            //La maquina lee el archivo para obtener el PID del jugador y llama a la funcion para comenzar
            fp = fopen("Ping.txt","a+");
            rewind(fp);
            pid_t pidJugador;
            fscanf(fp,"%d",&pidJugador);
            fclose(fp);

            cpu(pidJugador);

            //Acaba el proceso
            exit(0);
        }
    }else{
        //El jugador espera un poco para leer el archivo, que tendra el PID de la maquina
        sleep(1);

        hijo1 = getpid();
        printf("\nSoy el hijo 1, sere el jugador: %d\n",hijo1);

        fp = fopen("Ping.txt","a+");
        rewind(fp);
        pid_t pidMaquina;
        fscanf(fp,"%d",&pidMaquina);
        fclose(fp);
        
        //Luego escribe su propia posicion para que la pueda leer la maquina y luego comienza
        fp = fopen("Ping.txt","w+");
        fprintf(fp,"%d",getpid());
        fclose(fp);

        player(pidMaquina);

        //Acaba el proceso
        exit(0);
    }
    if(getpid() == padre){
        //El padre va a la funcion de arbitrar
        arbitro(hijo1,hijo2);
    }

    //Cuando todo acaba, el padre espera a que acaben sus hijos y luego acaba el programa
    waitpid(hijo1, NULL, 0);
    waitpid(hijo2, NULL, 0);

    return 0;
}