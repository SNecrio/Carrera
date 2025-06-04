#include <stdlib.h>
#include <stdio.h>
#include "lista.h"
#include "RyP.h"
#include "numeroN.h"

int numNodo = 1;

void AsignacionTrivial(int B[N][N]){
    TLISTA LNV;
    NODO raiz, x, y, s;
    float C = 0.0;

    raiz.bact = 0.0;
    raiz.nivel = -1;
    for(int i = 0; i < N; i++){
        raiz.tupla[i] = -1;
        raiz.usadas[i] = 0;
    }


    RCI_trivial(&raiz);
    RCS_trivial(&raiz,B);
    RBE(&raiz);
    raiz.n = 1;

    int max = B[0][0];
    for (int i = 0; i < N; i++) {     
        for (int j = 0; j < N; j++) { 
            if (B[i][j] > max) {
                max = B[i][j];          
            }
        }
    }

    C = raiz.CI;
    s.bact = -1;
    crearLista(&LNV);

    insertarElementoLista(&LNV, primeroLista(LNV), raiz);
    printf("\nAsignacion trivial:\n");
    while(!esListaVacia(LNV)){
        x = SeleccionarBM(LNV);
        if(x.CS > C){
            for(int i = 0; i < N; i++){
                y.nivel = x.nivel + 1;
                for(int a = 0; a < N; a++){
                    y.tupla[a] = x.tupla[a];
                    y.usadas[a] = x.usadas[a];
                }
                    
                if(!x.usadas[i]){
                    y.tupla[y.nivel] = i;
                    y.usadas[i] = 1;
                    y.bact = x.bact + B[y.nivel][i];

                    RCI_trivial(&y);
                    RCS_trivial(&y,B);
                    RBE(&y);

                    numNodo++;

                    y.n = numNodo;

                    if(ramSolucion(y) && y.bact > s.bact){
                        s = y;
                        C = compFloat(C,y.bact);
                    }else if(!ramSolucion(y) && y.CS > C){
                        insertarElementoLista(&LNV, primeroLista(LNV), y);
                        C = compFloat(C, y.CI);
                    }
                }
            }
        }
    }
    printf("Solucion:\n");
    for (int i = 0; i < N; i++)
        printf("%d, ", s.tupla[i]);

    printf("\nSolución optima: %f\n", C);
    printf("\nNumero de nodos visitados: %d\n",numNodo);
    numNodo = 1;

    destruirLista(&LNV);
}

void AsignacionPrecisa(int B[N][N]){
    TLISTA LNV;
    NODO raiz, x, y, s;
    float C = 0.0;

    raiz.bact = 0.0;
    raiz.nivel = -1;
    for(int i = 0; i < N; i++){
        raiz.tupla[i] = -1;
        raiz.usadas[i] = 0;
    }


    RCI_precisa(&raiz,B);
    RCS_precisa(&raiz,B);
    RBE(&raiz);
    raiz.n = 1;


    C = raiz.CI;
    s.bact = -1;
    crearLista(&LNV);

    insertarElementoLista(&LNV, primeroLista(LNV), raiz);

    printf("\nAsignacion precisa:\n");
    while(!esListaVacia(LNV)){
        x = SeleccionarBM(LNV);
        if(x.CS > C){
            for(int i = 0; i < N; i++){
                y.nivel = x.nivel + 1;
                for(int a = 0; a < N; a++){
                    y.tupla[a] = x.tupla[a];
                    y.usadas[a] = x.usadas[a];
                }
                    
                if(!x.usadas[i]){
                    y.tupla[y.nivel] = i;
                    y.usadas[i] = 1;
                    y.bact = x.bact + B[y.nivel][i];

                    numNodo++;

                    RCI_precisa(&y,B);
                    RCS_precisa(&y,B);
                    RBE(&y);

                    numNodo++;

                    y.n = numNodo;

                    if(!ramSolucion(y) && y.CS >= C && y.CS == y.CI){
                        y = SolAsignacionVoraz(y,B);
                        s = y;
                        C = compFloat(C,y.bact);
                        continue;
                    }

                    if(ramSolucion(y) && y.bact > s.bact){
                        s = y;
                        C = compFloat(C,y.bact);
                    }else if(!ramSolucion(y) && y.CS > C){
                        insertarElementoLista(&LNV, primeroLista(LNV), y);
                        C = compFloat(C, y.CI);
                    }
                }
            }
        }else if (x.CS == C && x.CS == x.CI) {
            s = SolAsignacionVoraz(x,B);
        }
    }
    printf("Solucion:\n");
    for (int i = 0; i < N; i++)
        printf("%d, ", s.tupla[i]);

    printf("\nSolución optima: %f\n", C);
    printf("\nNumero de nodos visitados: %d\n",numNodo);
    numNodo = 1;

    destruirLista(&LNV);
}

NODO SolAsignacionVoraz(NODO x, int B[][N]){
    int Bmax; // Para cada fila, beneficio máximo entre las ciudades no usadas
    int tmax; // Para cada nivel, ciudad no usada de mayor beneficio
    
    for(int i=x.nivel+1; i<N; i++){
        Bmax = -1;
        tmax = 0;
        for (int j = 0; j<N; j++){
            if (!x.usadas[j] && B[i][j] > Bmax){
                Bmax = B[i][j];
                tmax = j;
            }
        }
        x.tupla[i] = tmax;
        x.usadas[tmax] = 1;
        x.bact = x.bact + Bmax;
        numNodo++;
    }

    x.nivel = N-1;
    return x;
}


float compFloat(float x, float y){
    if(x > y){
        return x;
    }else{
        return y;
    }
}

int maxBeneficio(NODO x, int B[][N]) {
    
    // Beneficio acumulado
    int bMax = 0; 

    // Encuentra el maximo beneficio de cada fila, aunque no sea posible ese camino
    for (int i = x.nivel + 1; i < N; i++) {
        int max = -1;
        for (int j = 0; j < N; j++) {
            if (B[i][j] > max) {
                max = B[i][j];
            }
        }
        bMax += max;
    }
    return bMax;
}

int Voraz(NODO x, int B[][N]){

    int usadas[N];
    for(int i = 0; i < N; i++){
        usadas[i] = x.usadas[i];
    }

    int bMax = 0;

    // Recorre la matriz y encuentra la tarea libre con mayor beneficio en cada fila, para eso, tambien comprueba que no la haya pillado ya

    for (int i = x.nivel + 1; i < N; i++) {
        int max = -1;
        int tarea = -1;

        for (int j = 0; j < N; j++) {
            if (!usadas[j] && B[i][j] > max) {
                max = B[i][j];
                tarea = j;
            }
        }
        if (tarea != -1) {
            usadas[tarea] = 1;
            bMax += max;
        }
    }
    return bMax;
}

void RBE(NODO *x){

    // El beneficio estimado es la media de ambas cotas
    x->BE = (x->CI + x->CS)/2.0;
}

void RCI_trivial(NODO *x){
    
    // Para calcular la cota inferior es lo mismo que el beneficio actual
    x->CI = x->bact;
}

void RCI_precisa(NODO *x, int B[][N]){

    // Se le suma al beneficio ya acumulado el que se obtiene mediante asignacion voraz
    x->CI = x->bact + Voraz(*x, B);
}

void RCS_trivial(NODO *x, int B[][N]){
    
    // Se ve cual es el valor mas grande de la matriz de beneficios

    int max = -1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (B[i][j] > max) {
                max = B[i][j];
            }
        }
    }

    // Se calcula la cota superior usando ese valor

    x->CS = x->bact + ((N - x->nivel - 1) * max);
}

void RCS_precisa(NODO *x, int B[][N]){
    
    // Se calcula con el beneficio ya acumulado mas el maximo de las tareas
    x->CS = x->bact + maxBeneficio(*x, B);
}

int ramSolucion(NODO x){
    return (x.nivel == N-1);
}

NODO SeleccionarBM(TLISTA LNV){
    TPOSICION posActual = primeroLista(LNV);
    TPOSICION posMayor;  // Obtener la primera posición de la lista
    NODO nodoActual;//nodo actual
    NODO nodoMayor;//nodo mayor

    posMayor=posActual;
    recuperarElementoLista(LNV,posActual,&nodoMayor);

    while (posActual != finLista(LNV)) {
        recuperarElementoLista(LNV, posActual, &nodoActual);
        if (nodoActual.BE > nodoMayor.BE) {
            nodoMayor = nodoActual;
            posMayor = posActual;
        }
        posActual = siguienteLista(LNV, posActual);
    }
    suprimirElementoLista(&LNV,posMayor);


    
    return nodoMayor;  // Retornar el nodo con el mayor valor en BE
}