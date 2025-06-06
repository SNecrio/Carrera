#include "tabla_hash_recolocacion.h"

/* TABLA HASH CON RECOLOCACION */

void InicializarTablaHash(TablaHash t) {
    for (int i = 0; i < N; i++)
        t[i].alias[0] = VACIO;
}

/******* FUNCIONES HASH *******/

int FuncionHash(char *cad, unsigned int tipoFH, unsigned int K) {
    int suma = 0, posicion = 0;
    //int K = 500; /* FUNCION HASH 3: Probad al menos dos valores para la constante K */
    switch (tipoFH) {
        case 1:
            for (int i = strlen(cad) - 1; i >= 0; i--)
                suma += cad[i];
            posicion = suma % N;
            break;
        case 2: case 3: /* FUNCION HASH 3: Probad al menos dos valores para la constante K */
            for (int i = strlen(cad) - 1; i >= 0; i--) {
                suma = (suma * K + cad[i]) % N;
            }
            posicion = suma;
            break;
    }
    return posicion;
}

/* Funcion que localiza la posicion del elemento cuando buscamos*/
int _PosicionBuscar(TablaHash t, char *cad, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int *nPasosExtraB) {
    /* Devuelve el sitio donde esta la clave cad, o donde deberia estar. */
    /* No tiene en cuenta los borrados para avanzar.                     */
    int posicion;

    int ini = FuncionHash(cad, tipoFH, K);

    for (int i = 0; i < N; i++) {
        switch (tipoR) {
            case 1: //lineal
                posicion = (ini + a * i) % N;
                break;
            case 2: //cuadrática
                posicion = (ini + i * i) % N;
                break;
        }

        if (t[posicion].alias[0] == VACIO) { //si está vacío, terminé de buscar

            *nPasosExtraB += i;
            return posicion;
        }
        if (!strcmp(t[posicion].alias, cad)) {//si encontré cad, terminé de buscar

            *nPasosExtraB += i;
            return posicion;
        }
    }
    return ini; //devuelvo la posición dada por la función hash
}

/*Funcion que localiza la posicion para insertar un elemento */
int _PosicionInsertar(TablaHash t, char *cad, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a,int* hayColisionI, int* nPasosExtraI) {
    // Devuelve el sitio donde podriamos poner el elemento de clave cad
    int posicion;
    int ini = FuncionHash(cad, tipoFH, K); //calculo la posición mediante la función hash "tipo"

    for (int i = 0; i < N; i++) {

        if(i != 0){
            *hayColisionI = 1;
        }

        //Intento recolocar en aux. Cuando i=0, aux=ini, pruebo en la posición dada por la función hash
        switch (tipoR) {
            case 1: //recolocación lineal
                posicion = (ini + a * i) % N;
                break;
            case 2: //Recolocación cuadrática
                posicion = (ini + i * i) % N;
                break;
        }

        //Busco hueco en aux
        if (t[posicion].alias[0] == VACIO || t[posicion].alias[0] == BORRADO) {
            //Hueco encontrado, se han necesitado i intentos para ubicar el dato

            *nPasosExtraI += i;
            return posicion;
        }
        //Si el elemento a insertar ya estaba en la tabla
        if (!strcmp(t[posicion].alias, cad)) {

            *nPasosExtraI += i;
            return posicion;
        }
    }
    return ini;
}

/* Funcion que indica si un elemento esta o no en la tabla */
/* Devuelve 1 si el elemento de clave cad esta en la tabla, 0 si no esta */
int EsMiembroHash(TablaHash t, char *cad, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraB) {

    int posicion = _PosicionBuscar(t, cad, tipoFH, K, tipoR, a, nPasosExtraB);

    if (t[posicion].alias[0] == VACIO)
        return 0;
    else
        return (!strcmp(t[posicion].alias, cad));
}

/* BUSCA UN ELEMENTO CON LA CLAVE INDICADA EN LA TABLA HASH, Y LO DEVUELVE, 
 * ADEMAS DE INDICAR CON 1 QUE EXISTE EL ELEMENTO, Y CON 0 QUE NO ESTA EN LA TABLA */
int BuscarHash(TablaHash t, char *clavebuscar, TIPOELEMENTO *e, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraB) {

    int posicion = _PosicionBuscar(t, clavebuscar, tipoFH, K, tipoR, a, nPasosExtraB);
    if (t[posicion].alias[0] == VACIO)
        return 0;
    else {
        if (strcmp(t[posicion].alias, clavebuscar) == 0) {
            *e = t[posicion];
            return 1;
        } else return 0;
    }
}

/* Funcion que inserta un elemento en la tabla */
int InsertarHash(TablaHash t, TIPOELEMENTO e, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraI) {
    int posicion;
    int hayColisionI = 0;
    posicion = _PosicionInsertar(t, e.alias, tipoFH, K, tipoR,a, &hayColisionI, nPasosExtraI);

    if (t[posicion].alias[0] == VACIO || t[posicion].alias[0] == BORRADO) {
        t[posicion] = e;
    }
    
    return hayColisionI;
}

/* Funcion que elimina un elemento de la tabla */
void BorrarHash(TablaHash t, char *cad, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraE) {

    int posicion = _PosicionBuscar(t, cad, tipoFH, K, tipoR, a, nPasosExtraE);

    if (t[posicion].alias[0] != VACIO && t[posicion].alias[0] != BORRADO) {
        if (!strcmp(t[posicion].alias, cad)) {
            t[posicion].alias[0] = BORRADO;
            t[posicion].alias[1] = VACIO;
        }
    }
}

