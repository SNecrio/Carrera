#include <stdlib.h>
#include <stdio.h>
#include "lista.h"
#include "backtracking.h"
#include "RyP.h"
#include "numeroN.h"

//int B[N][N]= {{11,17,8},{9,7,6},{13,15,16}};

int B[N][N]= {{11,17,8 ,16,20,14},
                {9 ,7 ,6 ,12,15,18},
                {13,15,16,12,16,18},
                {21,24,28,17,26,20},
                {10,14,12,11,15,13},
                {12,20,19,13,22,17}};//*/

//Para cambiar el valor de N se cambia en numeroN.h


int main(int argc, char const *argv[])
{
    int a = -1;
    do{
        printf("\nBacktracking = 0\nBacktracking con vector de usadas = 1\nA. Trivial = 2\nA. Precisa = 3\nElija que quiere ejecutar: ");
        scanf("%d",&a);
    }while(a > 3 || a < 0);
    
    switch(a){
        case 0:
            backtracking(B);
        break;
        case 1:
            
            backtrackingU(B);
        break;
        case 2:
            AsignacionTrivial(B);
        break;
        case 3:
            AsignacionPrecisa(B);
        break;
    }

    return 0;
}