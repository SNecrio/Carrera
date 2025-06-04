#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include <string.h>
#include "GoTMap.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

#define INFINITY 999999

//Opción a del menú, introducir un vertice en el grafo

void introducir_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce nombre de ciudad: ");
    getchar();
    scanf("%[^\n]", v1.nombre);
    printf("\nIntroduce nombre de region: ");
    getchar();
    scanf("%[^\n]", v1.region);
    if (existe_vertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        insertar_vertice(G, v1);
}

//Opción b del menú, eliminar un vértice del grafo

void eliminar_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce nombre de ciudad: ");
    getchar();
    scanf("%[^\n]", v1.nombre);
    if (existe_vertice(*G, v1))
        borrar_vertice(G, v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú, crear una relación entre dos vértices

void nuevo_arco(grafo *G) {

    tipovertice v1, v2;
    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce nombre de ciudad de origen: ");
    getchar();
    scanf("%[^\n]", v1.nombre);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.nombre);
        return;
    }
    //Vértice destino del arco
    printf("Introduce nombre de ciudad de destino: ");
    getchar();
    scanf("%[^\n]", v2.nombre);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.nombre);
        return;
    }

    float distancia;
    char tipo;

    printf("\nIntroduzca distancia: ");
    scanf("%f",&distancia);
    do{
        printf("\nIntroduzca tipo de conexion (t/m): ");
        getchar();
        scanf("%c",&tipo);
    }while(tipo != 't' && tipo != 'm');

    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    //Creación del arco
    if (!son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2))){
        crear_arco(G, posicion(*G, v1), posicion(*G, v2), distancia, tipo);
    }else{
        printf("\nYa existe un camino entre estas dos ciudades\n");
    }
        
}

//Opción d del menú, eliminar una relación entre dos vértices

void eliminar_arco(grafo *G) {
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce nombre de ciudad de origen: ");
    getchar();
    scanf("%[^\n]", v1.nombre);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.nombre);
        return;
    }
    //Vértice destino del arco
    printf("Introduce nombre de ciudad de destino: ");
    getchar();
    scanf("%[^\n]", v2.nombre);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.nombre);
        return;
    }
    //Eliminación del arco
    if (son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2))){
        borrar_arco(G, posicion(*G, v1), posicion(*G, v2));
    }else{
        printf("\nNo existe un camino entre estas dos ciudades\n");
    }
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo

void imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf("Vertice(%d): %s\n", i, VECTOR[i].nombre);
        //Chequeo sus arcos
        for (j = 0; j < N; j++)
            if (son_adyacentes(G, i, j)){
                if(devolver_tipo(G,i,j) == 't'){
                    printf("\t%s----->%s\n\t\tDistancia: %.2f\n", VECTOR[i].nombre,VECTOR[j].nombre,devolver_distancia(G, i, j));
                }else{
                    printf("\t%s~~~~~>%s\n\t\tDistancia: %.2f\n", VECTOR[i].nombre,VECTOR[j].nombre,devolver_distancia(G, i, j));
                }                
            }
    }
}

void leer_archivo(grafo* G, char* archivo){

    FILE* fp = fopen(archivo, "r");
    if(fp == NULL){
        printf("Archivo no encontrado\n");
        return;
    }
    char linea[200];
    int size = sizeof(linea);

    while(fgets(linea,size,fp) != NULL && strcmp(linea,"+\n") != 0){
        tipovertice v1;

        strcpy(v1.nombre,strtok(linea,"-"));
        strcpy(v1.region,strtok(NULL,"\n"));
        if (!existe_vertice(*G, v1))
            insertar_vertice(G, v1);
    }

    while(fgets(linea,size,fp) != NULL){
        tipovertice verOr;
        tipovertice verDest;

        strcpy(verOr.nombre,strtok(linea,"-"));
        strcpy(verDest.nombre,strtok(NULL,"-"));


        if (existe_vertice(*G, verOr) && existe_vertice(*G, verDest)){

            if (!son_adyacentes(*G, posicion(*G, verOr), posicion(*G, verDest))){
                float distancia;
                char tipo;

                distancia = atof(strtok(NULL,"-"));

                if(strcmp(strtok(NULL,"\n"),"sea") == 0){
                    tipo = 'm';
                }else{
                    tipo = 't';
                }
                
                crear_arco(G, posicion(*G, verOr), posicion(*G, verDest), distancia, tipo);
            }
        }
    }

    fclose(fp);
}

void escribir_archivo(grafo G, char* archivo){

    FILE* fp = fopen(archivo, "w+");
    if(fp == NULL){
        printf("Archivo no encontrado\n");
        return;
    }

    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;

    //Imprimo los vértices
    for (i = 0; i < N; i++)
        fprintf(fp,"%s-%s\n", VECTOR[i].nombre, VECTOR[i].region);

    
    //Usamos un + de separador entre los vertices y los arcos en el archivo
    fprintf(fp,"+\n");

    for (i = 0; i < N; i++) {
        //Chequeo sus arcos
        for (j = i; j < N; j++)
            if (son_adyacentes(G, i, j)){
                if(devolver_tipo(G,i,j) == 't'){
                    fprintf(fp, "%s-%s-%.0f-land\n", VECTOR[i].nombre,VECTOR[j].nombre,devolver_distancia(G, i, j));
                }else{
                    fprintf(fp, "%s-%s-%.0f-sea\n", VECTOR[i].nombre,VECTOR[j].nombre,devolver_distancia(G, i, j));
                }
            }
    }

    fclose(fp);
}

void encontrar_camino(grafo G, float distancia[][MAXVERTICES], int previa[][MAXVERTICES], int origen, int destino){
    
    tipovertice *array = array_vertices(G);
    if(origen != destino){
        encontrar_camino(G, distancia, previa, origen, previa[origen][destino]);
    }

    if(devolver_tipo(G, destino, previa[origen][destino]) == 't'){
        printf(" --> ");
    }else if(devolver_tipo(G, destino, previa[origen][destino]) == 'm'){
        printf(" ~~> ");
    }
    printf("%s",array[destino].nombre);
}

void buscar_camino(grafo G){

    int N = num_vertices(G);
    tipovertice v1, v2;
    int origen, destino;
    printf("Encontrar camino vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce nombre de ciudad de origen: ");
    getchar();
    scanf("%[^\n]", v1.nombre);
    if (!existe_vertice(G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.nombre);
        return;
    }
    origen = posicion(G, v1);
    //Vértice destino del arco
    printf("Introduce nombre de ciudad de destino: ");
    getchar();
    scanf("%[^\n]", v2.nombre);
    if (!existe_vertice(G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.nombre);
        return;
    }

    destino = posicion (G,v2);
    //Elegir modo de movimiento
    char modo;
    do{
        printf("Desea encontrar:\n\tLa ruta mas corta? (c)\n\tLa ruta mas rapida a caballo? (p)\n\tO la ruta mas rapida a dragon? (d)\n\nOpcion: ");
        getchar();
        scanf("%c",&modo);
    }while(modo != 'c' && modo != 'p' && modo != 'd');

    float tierra, mar;
    switch (modo)
    {
        case 'c': //Ruta más rápida normal
            tierra = 1.0;
            mar = 1.0;
            break;
        case 'p': //Ruta más rápida a caballo
            tierra = 5.5;
            mar = 11.25;
            break;
        case 'd': //Ruta más rápida en dragón
            tierra = 80.0;
            mar = 80.0;
            break;
    }
    
    //Creamos las matrices
    int i,j,k;

    float distancia[MAXVERTICES][MAXVERTICES];
    int previa[MAXVERTICES][MAXVERTICES];

    //Inicializamos sus valores
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            
            //Distancia
            if(devolver_distancia(G, i, j) == INFINITY){
                distancia[i][j] = INFINITY;

            }else if(i == j){
                distancia[i][j] = 0;

            }else if(devolver_tipo(G, i, j) == 't'){
                distancia[i][j] = devolver_distancia(G, i, j) / tierra;
            }else{
                distancia[i][j] = devolver_distancia(G, i, j) / mar;
            }

            //Vertices previos
            if (devolver_distancia(G, i, j)==INFINITY || devolver_distancia(G, i, j)==0){
                previa[i][j] = -1;
            }
            else{
                previa[i][j] = i;
            }
        }
    }
    
    //Hacemos Floyd Wharshall
    for(k = 0; k < N; k++){
        for(i = 0; i < N; i++){
            for(j = 0; j < N; j++){
                if(distancia[i][j] > distancia[i][k] + distancia[k][j]){
                    distancia[i][j] = distancia[i][k] + distancia[k][j];
                    previa[i][j] = previa[k][j];
                }
            }
        }
    }

///ESTO ESTA DISTINTO---------------------REVISAR A VER SI

    printf("La ruta buscada desde %s hasta %s es: ", v1.nombre, v2.nombre);
    encontrar_camino(G, distancia, previa, origen, destino);
    if(modo = 'c'){
        printf(", y dura %.2f kilometros\n",distancia[origen][destino]);
    }else{
        printf(", y dura %.2f horas\n",distancia[origen][destino]);
    }
    
}

void _printMatrix(grafo G, int N){
   int i,j;
   for(i=0;i<N;i++){
      for(j=0;j<N;j++){
         if (devolver_distancia(G, i, j)==INFINITY)
            printf("%10s","INF");
         else
            printf("%10.2f",devolver_distancia(G, i, j));
      }
      printf("\n");
   }
}

void _printMatrix2(grafo G, int N){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if (devolver_distancia(G, i, j)==INFINITY || devolver_distancia(G, i, j)==0)
                printf("%10d",0);
            else    
                printf("%10d",i);
        }
        printf("\n");
    }
}

void expansion(grafo G){
    
    int i,j,N;
    N = num_vertices(G);

    //Creamos una matriz con los valores de las distancias y la inicializamos
    camino distancia[MAXVERTICES][MAXVERTICES];
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){

            if(i == j){

                distancia[i][j].distancia = 0;
                
            }else if(son_adyacentes(G,i,j) == 1){
                
                if(devolver_tipo(G,i,j) == 't'){
                    distancia[i][j].distancia = devolver_distancia(G, i, j) / 5.5;
                }else{
                    distancia[i][j].distancia = devolver_distancia(G, i, j) / 11.25;
                }

            }else{
                distancia[i][j].distancia = INFINITY;
            }
        }
    }

    //Creamos un array para guardar los nodos que ya hayan sido visitados
    int* array = malloc(N * sizeof(int));
    
    //Lo inicializamos a 0 (Nodo no visitado) y ponemos el primero que visitamos a 1
    for(i = 0; i < N; i++){
        array[i] = 0;
    }
    array[0] = 1;

    double longitud = 0, min;
    int arcos = 0, v1, v2;

    //El bucle recorre todos los arcos
    while(arcos<N-1){
        min = INFINITY;
        v1 = 0;
        v2 = 0;
        for(i=0;i<N;i++){
            
            //Se comprueban los nodos incluidos en el arbol de expansion
            if(array[i] == 1){
                for(j=0;j<N;j++){

                    //Si los nodos ya visitados son adyacentes a uno no visitado, se comprueba so valor, si es el minimo encontrado para los arcos
                    //de se nodo, se guarda
                    if(array[j] != 1 && distancia[i][j].distancia > 0){
                        if(min > distancia[i][j].distancia){
                            min = distancia[i][j].distancia;
                            v1 = i;
                            v2 = j;                            
                        }
                    }
                }
            }
        }

        //Se incluye el nodo que acabamos de visitar en el arbol de expansion
        array[v2] = 1;
        arcos++;

        //Se imprime el nodo que acabamos de visitar con su arco al nodo con el que se unio
        if(devolver_tipo(G,v1,v2) ==  't'){
            printf("%15s ---- %-15s : %.2f horas\n", array_vertices(G)[v1].nombre, array_vertices(G)[v2].nombre, distancia[v1][v2].distancia);    
        }else{
            printf("%15s ~~~~ %-15s : %.2f horas\n", array_vertices(G)[v1].nombre, array_vertices(G)[v2].nombre, distancia[v1][v2].distancia);    
        }

        //Se suma la longitud del arco a la longitud total y al final se imprime
        longitud += distancia[v1][v2].distancia;
    }
    printf("\nSe necesitan %.2f horas para recorrer todo el arbol de expansion\n",longitud);
}
