#include <stdlib.h>
#include <stdio.h>
#include "backtracking.h"
#include "numeroN.h"

// Imprime los resultados obtenidos tras ejecutar el algoritmo.
void imprimirRes(int *s, int *numNodos, int *numCriterio, int *numGenerar, int *numSolucion, int *numHermanos, int *numRetroceder) {
    printf("Solucion:\n");
    for (int i = 0; i < N; i++)
        printf("%d, ", s[i]); // Muestra la solución obtenida.

    // Imprime estadísticas sobre el proceso de búsqueda.
    printf("\n\nNumero de nodos visitados: %d", *numNodos);
    printf("\nNumero de criterio visitados: %d", *numCriterio);
    printf("\nNumero de generar visitados: %d", *numGenerar);
    printf("\nNumero de solucion visitados: %d", *numSolucion);
    printf("\nNumero de hermanos visitados: %d", *numHermanos);
    printf("\nNumero de retroceder visitados: %d\n", *numRetroceder);
}

// Implementación básica de backtracking sin optimización con "usada".
void backtracking(int B[][N]) {
    
    int s[N]; // Vector de soluciones
    int nivel = 0; // Nivel actual del árbol de decisiones.

    // Inicialización de las variables de estado y solución.
    for (int i = 0; i < N; i++)
        s[i] = -1;

    int voa = -1000; // Valor óptimo actual.
    int soa[N]; // Solución óptima actual.
    int bact = 0; // Beneficio acumulado.

    // Variables de contadores para medir estadísticas.
    int numNodos = 1, numCriterio = 0, numGenerar = 0, numSolucion = 0, numHermanos = 0, numRetroceder = 0;

    // Bucle principal del algoritmo de backtracking.
    while (nivel != -1) {
        // Genera un nuevo candidato en el nivel actual.
        Generar(nivel, s, &bact, B, &numGenerar);

        // Verifica si es una solución válida y mejor que la actual.
        if (Solucion(nivel, s, &numSolucion, &numCriterio) && (bact > voa)) {
            voa = bact;
            for (int i = 0; i < N; i++)
                soa[i] = s[i];
        }

        // Comprueba si cumple con los criterios para seguir explorando.
        if (Criterio(nivel, s, &numCriterio)) {
            numNodos++;
            if (nivel < N - 1) {
                nivel++; // Avanza al siguiente nivel si es posible.
            }
        }

        // Retrocede si no hay más candidatos en el nivel actual.
        while (!MasHermanos(nivel, s, &numHermanos) && (nivel >= 0)) {
            Retroceder(&nivel, s, &bact, B, &numRetroceder);
        }
    }

    // Copia la mejor solución encontrada a `s`.
    for (int i = 0; i < N; i++)
        s[i] = soa[i];

    // Imprime los resultados y estadísticas del algoritmo.
    imprimirRes(s, &numNodos, &numCriterio, &numGenerar, &numSolucion, &numHermanos, &numRetroceder);
}

// Versión optimizada de backtracking utilizando el vector "usada".
void backtrackingU(int B[][N]) {
    int nivel = 0;

    int s[N]; // Vector de soluciones
    int usada[N]; // Vector para registrar elementos ya utilizados.
    for (int i = 0; i < N; i++) {
        s[i] = -1;
        usada[i] = 0;
    }

    int voa = -1000; // Valor óptimo actual.
    int soa[N]; // Solución óptima actual.
    int bact = 0; // Beneficio acumulado.

    // Variables de contadores para medir estadísticas.
    int numNodos = 1, numCriterio = 0, numGenerar = 0, numSolucion = 0, numHermanos = 0, numRetroceder = 0;

    // Bucle principal del algoritmo de backtracking optimizado.
    while (nivel != -1) {
        // Genera un nuevo candidato en el nivel actual.
        GenerarU(nivel, s, &bact, B, &numGenerar, usada);

        // Verifica si es una solución válida y mejor que la actual.
        if (SolucionU(nivel, s, &numSolucion, &numCriterio,usada) && (bact > voa)) {
            voa = bact;
            for (int i = 0; i < N; i++)
                soa[i] = s[i];
        }

        // Comprueba si cumple con los criterios para seguir explorando.
        if (CriterioU(nivel, s, &numCriterio, usada)) {
            numNodos++;
            if (nivel < N - 1) {
                nivel++; // Avanza al siguiente nivel si es posible.
            }
        }

        // Retrocede si no hay más candidatos en el nivel actual.
        while (!MasHermanos(nivel, s, &numHermanos) && (nivel >= 0)) {
            RetrocederU(&nivel, s, &bact, B, &numRetroceder, usada);
        }
    }

    // Copia la mejor solución encontrada a `s`.
    for (int i = 0; i < N; i++)
        s[i] = soa[i];

    // Imprime los resultados y estadísticas del algoritmo.
    imprimirRes(s, &numNodos, &numCriterio, &numGenerar, &numSolucion, &numHermanos, &numRetroceder);
}

// Funciones auxiliares:

// Genera un nuevo candidato en el nivel actual (versión básica).
void Generar(int nivel, int s[N], int *bact, int B[][N], int *numGenerar) {
    (*numGenerar)++;
    s[nivel] += 1;
    if (s[nivel] == 0) {
        *bact += B[nivel][s[nivel]];
    } else {
        *bact += B[nivel][s[nivel]] - B[nivel][s[nivel] - 1];
    }
}

// Genera un nuevo candidato en el nivel actual (versión optimizada).
void GenerarU(int nivel, int s[N], int *bact, int B[][N], int *numGenerar, int *usada) {
    (*numGenerar)++;
    if (s[nivel] != -1) usada[s[nivel]]--; // Desmarca el elemento anterior.

    s[nivel] += 1; // Genera el siguiente candidato.
    usada[s[nivel]]++; // Marca el nuevo elemento.

    if (s[nivel] == 0) {
        *bact += B[nivel][s[nivel]];
    } else {
        *bact += B[nivel][s[nivel]] - B[nivel][s[nivel] - 1];
    }
}

// Verifica si la asignación actual cumple los criterios (básico).
int Criterio(int nivel, int s[N], int *numCriterio) {
    for (int i = 0; i < nivel; i++) {
        (*numCriterio)++;
        if (s[nivel] == s[i]) return 0; // Criterio incumplido: repetición.
    }
    return 1; // Criterio cumplido.
}

// Verifica si la asignación actual cumple los criterios (optimizado).
int CriterioU(int nivel, int s[N], int *numCriterio, int *usada) {
    (*numCriterio)++;
    return (usada[s[nivel]] == 1); // El elemento no debe repetirse.
}

// Verifica si la asignación actual es una solución completa.
int Solucion(int nivel, int s[N], int *numSolucion, int *numCriterio) {
    (*numSolucion)++;
    return (nivel == N - 1 && Criterio(nivel, s, numCriterio));
}

int SolucionU(int nivel, int s[N], int *numSolucion, int *numCriterio, int *usada) {
    (*numSolucion)++;
    return (nivel == N - 1 && CriterioU(nivel, s, numCriterio,usada));
}

// Comprueba si hay más candidatos en el nivel actual.
int MasHermanos(int nivel, int s[N], int *numHermanos) {
    (*numHermanos)++;
    return (s[nivel] < N - 1);
}

// Retrocede al nivel anterior (básico).
void Retroceder(int *nivel, int s[N], int *bact, int B[][N], int *numRetroceder) {
    (*numRetroceder)++;
    *bact -= B[*nivel][s[*nivel]];
    s[*nivel] = -1;
    (*nivel)--;
}

// Retrocede al nivel anterior (optimizado).
void RetrocederU(int *nivel, int s[N], int *bact, int B[][N], int *numRetroceder, int *usada) {
    (*numRetroceder)++;
    usada[s[*nivel]]--;
    *bact -= B[*nivel][s[*nivel]];
    s[*nivel] = -1;
    (*nivel)--;
}
