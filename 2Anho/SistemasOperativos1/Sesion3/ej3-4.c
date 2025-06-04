/*
* PRACTICA 3: USO DE SEÑALES
* ENTREGABLE 1
* VALERIA ITZEL CONTRERAS MIRANDA
* SERGIO SOUTO MOURELLE
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int i = 0;

//Función que recibe la señal "CtrlC" y la regresa a su funcionamiento habitual
void imprimir(){
    printf("\nSeñal recibida!\n");
    i++;

    if(i >= 3){
        int a;
        printf("\nQuiere restaurar el comportamiento por defecto? 0 = no, 1 = si\n");
        scanf("%d",&a);
        if(a == 1){
            signal(SIGINT,SIG_DFL);
        }
    }
}

int main(){
    
    signal(SIGINT,&imprimir);
    while(1){}

    return 0;
}
