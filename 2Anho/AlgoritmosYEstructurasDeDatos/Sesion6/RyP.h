#ifndef RYP_H
#define RYP_H
#include "lista.h"
#include "numeroN.h"

//#define N 3
//#define N 6

void RCI_trivial(NODO *x);
void RCS_trivial(NODO *x, int B[][N]);
void RCI_precisa(NODO *x, int B[][N]);
void RCS_precisa(NODO *x, int B[][N]);
void RBE(NODO *x);
int Voraz(NODO x, int B[][N]);
int maxBeneficio(NODO x, int B[][N]);
float compFloat(float x, float y);
NODO SolAsignacionVoraz(NODO x, int B[][N]);
NODO SeleccionarBM(TLISTA LNV);
int ramSolucion(NODO x);
void AsignacionTrivial(int B[N][N]);
void AsignacionPrecisa(int B[N][N]);

#endif