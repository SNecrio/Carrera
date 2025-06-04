#ifndef BACKTRACKING_H
#define BACKTRACKING_H
#include "numeroN.h"

void imprimirRes(int *s, int *numNodos, int *numCriterio, int *numGenerar, int *numSolucion, int *numHermanos, int *numRetroceder);
void Generar(int nivel, int s[N], int *bact, int B[][N], int *intGenerar);
int Criterio(int nivel, int s[N], int *numCriterio);
int Solucion(int nivel, int s[N], int *numSolucion, int *numCriterio);
int MasHermanos(int nivel, int s[N], int *numHermanos);
void Retroceder(int *nivel, int s[N], int *bact, int B[][N], int *numRetroceder);
void backtracking(int B[][N]);

int SolucionU(int nivel, int s[N], int *numSolucion, int *numCriterio, int *usada);
void GenerarU(int nivel, int s[N], int *bact, int B[][N], int *intGenerar, int *usada);
int CriterioU(int nivel, int s[N], int *numCriterio, int *usada);
void RetrocederU(int *nivel, int s[N], int *bact, int B[][N], int *numRetroceder, int *usada);
void backtrackingU(int B[][N]);

#endif