#include "juegodetronos.h"
#include "abb.h"
#include "lista.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int anhadirPersonaje(TABB* abb){

    //Creamos las variables y las listas
    TIPOELEMENTOABB auxelem;
    crearLista(&auxelem.parents);
    crearLista(&auxelem.siblings);
    crearLista(&auxelem.killed);
    TPOSICION pos;
    TIPOELEMENTOLISTA auxlista;
    strcpy(auxlista.nameP, "");

    int auxint;
    
    //Introducimos por teclado el nombre, casa y realeza
    printf("\nName: ");
    getchar();
    scanf("%[^\n]", auxelem.name);
    printf("\nHouse (? si desconocido):");
    getchar();
    scanf("%[^\n]", auxelem.house);
    printf("\nRoyal (0/1): ");
    scanf("%d",&auxint);

    while(auxint != 0 && auxint != 1){
        printf("\nNumero no valido, introduzca de nuevo: ");
        getchar();
        scanf("%d",&auxint);
    }
    auxelem.royal = auxint;

    //Hacemos el msimo proceso en las 3 listas, ponemos el TPOSICION en la primera posicion y vamos capturando en la variable auxLista cada
    //elemento nuevo hasta que veamos que es igual a "fin", cada elemento lo vamos insertando en la lista correspondiente

    pos = primeroLista(auxelem.parents);

    while(strcmp(auxlista.nameP, "fin") != 0){
        printf("\nParents (fin para terminar): ");
        getchar();
        scanf("%[^\n]", auxlista.nameP);

        if(strcmp(auxlista.nameP, "fin") != 0){
            insertarElementoLista(&auxelem.parents,pos,auxlista);
        }
    }

    pos = primeroLista(auxelem.siblings);
    
    strcpy(auxlista.nameP, "");
    while(strcmp(auxlista.nameP, "fin") != 0){
        printf("\nSiblings (fin para terminar): ");
        getchar();
        scanf("%[^\n]", auxlista.nameP);

        if(strcmp(auxlista.nameP, "fin") != 0){
            insertarElementoLista(&auxelem.siblings,pos,auxlista);
        }
    }

    pos = primeroLista(auxelem.killed);

    strcpy(auxlista.nameP, "");
    while(strcmp(auxlista.nameP, "fin") != 0){
        printf("\nKilled (fin para terminar): ");
        getchar();
        scanf("%[^\n]", auxlista.nameP);

        if(strcmp(auxlista.nameP, "fin") != 0){
            insertarElementoLista(&auxelem.killed,pos,auxlista);
        }
    }

    //Finalmente insertamos el elemento en la tabla
    
    insertarElementoAbb(abb,auxelem);
}

void imprimirLista(TLISTA E){
    
    //Si la lista no es vacia, creamos las variables necesarias

    if(!esListaVacia(E)){

        TPOSICION pos;
        pos = primeroLista(E);
        TIPOELEMENTOLISTA elem;

        //Hacemos un bucle donde se recorre toda la lista hasta el final, se recupera cada elemento, se imprime y se pasa al siguiente
        
        while(pos != finLista(E)){
            recuperarElementoLista(E,pos,&elem);
            printf("%s, ",elem.nameP);
            pos = siguienteLista(E,pos);
        }
    }
}

void listarPersonajes(TABB abb){
    
    //Creamos las variables y vemos si el arbol esta vacio o no

	TIPOELEMENTOABB E;
	if(!esAbbVacio(abb)){

        //Llamamos recursivamente a la funcion con el arbol de la izquierda
	    listarPersonajes(izqAbb(abb));

        //Leemos el elemento de la lista e imprimimos todos los valores, para las listas utilizamso al funcion auxiliar imprimirLista()
	    leerElementoAbb(abb,&E); 

        printf("\n%s\n",E.name);
        printf("\tHouse: %s\n",E.house);
        printf("\tRoyal: ");
        if(E.royal == 0){
            printf("No\n");
        }else{
            printf("Yes\n");
        }

        printf("\tParents: ");
        imprimirLista(E.parents);

        printf("\n\tSiblings: ");
        imprimirLista(E.siblings);

        printf("\n\tKilled: ");
        imprimirLista(E.killed);

        //Llamamos recursivamente a la funcion con el arbol de la derecha
	    listarPersonajes(derAbb(abb));
	}	
}

void eliminarPersonaje(TABB* abb){

    //Creamos las variables y pedimso por teclado el nombre del personaje a eliminar
    char nombre[100];
    TIPOELEMENTOABB nodo;
    strcpy(nodo.name,"");
    printf("\nIntroduzca nombre del personaje a eliminar: ");
    getchar();
    scanf("%[^\n]", nombre);

    //Se busca en el arbol, y si se encuentra se elimina
    buscarNodoAbb(*abb,nombre,&nodo);
    suprimirElementoAbb(abb,nodo);
    if(strcmp(nodo.name,"") == 0){
        printf("\nNo se encontro ningun personaje con ese nombre\n");
    }

}

//Leer archivo

int anhadirArchivo(TABB* abb, char* fichero){
    
    //Abrimos el archivo, si no lo encuentra, devuelve un 0.
    FILE* fp = fopen(fichero,"r");
    if(fp == NULL){
        printf("\nArchivo no encontrado\n");
        return 0;
    }

    //Creamos las variables
    char linea[400];
    char* lineaExtra;
    char lineaAux[400];
    int size = sizeof(linea);
    TPOSICION pos;
    TIPOELEMENTOLISTA auxlista;
    strcpy(auxlista.nameP, "");

    int auxint;

    //Repetimos el proceso de añadir personajes hasta que se acaben las lineas de la lista
    while(fgets(linea,size,fp) != NULL){

        //Creamos un elemento del arbol y creamos sus listas
        TIPOELEMENTOABB auxelem;
        crearLista(&auxelem.parents);
        crearLista(&auxelem.siblings);
        crearLista(&auxelem.killed);

        //Usando strtok_r, dividmos la linea en partes y metemos los valores en los campos correspondientes, guardando la linea restante en lineaExtra
        strcpy(auxelem.name,strtok_r(linea, "|",&lineaExtra));
        strcpy(auxelem.house,strtok_r(NULL, "|",&lineaExtra));
        auxelem.royal = atoi(strtok_r(NULL, "|",&lineaExtra));

        //Para las listas, usamos una lineaAux con strtok y guardamos el elemento el elemLista, si este es nulo, guardamos un espacio vacio en la variable
        pos = primeroLista(auxelem.parents);
        strcpy(lineaAux,strtok_r(NULL, "|",&lineaExtra));

        char* elemLista = strtok(lineaAux, ",");
        if(elemLista != NULL){
            strcpy(auxlista.nameP,elemLista);
        }else{
            strcpy(auxlista.nameP, "");
        }

        //Hasta que el elemento sea un espacio vacio, repetimos el proceso y vamos insertando elementos en la lista, repetimos para las demas listas
        while(strcmp(auxlista.nameP,"") != 0){

            insertarElementoLista(&auxelem.parents,pos,auxlista);
            elemLista = strtok(NULL, ",");
            if(elemLista != NULL){
                strcpy(auxlista.nameP,elemLista);
            }else{
                strcpy(auxlista.nameP, "");
            }
        }

        //Hermanos
        pos = primeroLista(auxelem.siblings);
        strcpy(lineaAux,strtok_r(NULL, "|",&lineaExtra));

        elemLista = strtok(lineaAux, ",");
        if(elemLista != NULL){
            strcpy(auxlista.nameP,elemLista);
        }else{
            strcpy(auxlista.nameP, "");
        }

        while(strcmp(auxlista.nameP,"") != 0){

            insertarElementoLista(&auxelem.siblings,pos,auxlista);
            elemLista = strtok(NULL, ",");
            if(elemLista != NULL){
                strcpy(auxlista.nameP,elemLista);
            }else{
                strcpy(auxlista.nameP, "");
            }
        }


        //Matados
        pos = primeroLista(auxelem.killed);
        strcpy(lineaAux,strtok_r(NULL, "|",&lineaExtra));

        elemLista = strtok(lineaAux, ",");
        if(elemLista != NULL){
            strcpy(auxlista.nameP,elemLista);
        }else{
            strcpy(auxlista.nameP, "");
        }

        while(strcmp(auxlista.nameP,"") != 0){

            insertarElementoLista(&auxelem.killed,pos,auxlista);
            elemLista = strtok(NULL, ",");
            if(elemLista != NULL){
                strcpy(auxlista.nameP,elemLista);
            }else{
                strcpy(auxlista.nameP, "");
            }
        }

        //Insertamos el elemento en el arbol
        insertarElementoAbb(abb,auxelem);
    }

    //Cerramos el archivo y devolvemos un 1
    fclose(fp);
    return 1;
}

//Escribir archivo

void escribirElemento(TIPOELEMENTOABB E, FILE* fp){

    TLISTA listaAux;

    //Imprime directamente el nombre, casa y realeza
    fprintf(fp,"%s|%s|%d|",E.name,E.house,E.royal);

	TPOSICION pos;
    TIPOELEMENTOLISTA elem;

    //Recorre cada lista, la imprime poco a poco recuperando los elementos y poniendo , y | donde tienen que estar, si la lista esta vacia, pone un -|
    if(!esListaVacia(E.parents)){

        pos = primeroLista(E.parents);
        
        while(pos != finLista(E.parents)){
            recuperarElementoLista(E.parents,pos,&elem);
            fprintf(fp,"%s",elem.nameP);
            pos = siguienteLista(E.parents,pos);
            if(pos != finLista(E.parents)){
                fprintf(fp,",");
            }
        }
        fprintf(fp,"|");
    }else{
        fprintf(fp,"-|");
    }

    if(!esListaVacia(E.siblings)){

        pos = primeroLista(E.siblings);
        
        while(pos != finLista(E.siblings)){
            recuperarElementoLista(E.siblings,pos,&elem);
            fprintf(fp,"%s",elem.nameP);
            pos = siguienteLista(E.siblings,pos);
            if(pos != finLista(E.siblings)){
                fprintf(fp,",");
            }
        }
        fprintf(fp,"|");
    }else{
        fprintf(fp,"-|");
    }

    if(!esListaVacia(E.killed)){

        pos = primeroLista(E.killed);
        
        while(pos != finLista(E.killed)){
            recuperarElementoLista(E.killed,pos,&elem);
            fprintf(fp,"%s",elem.nameP);
            pos = siguienteLista(E.killed,pos);
            if(pos != finLista(E.killed)){
                fprintf(fp,",");
            }
        }
        fprintf(fp,"|");
    }else{
        fprintf(fp,"-|");
    }

    //Al final imprime un salto de linea
    fprintf(fp,"\n");
}

void preordenArchivo(TABB abb, FILE* fp){

    //Esta funcion se llama recursivamente hasta que acaba el arbol, lo que ahce es leer cada elemento y mandarlo a la funcion de escribirlo
    TIPOELEMENTOABB arbolAux;

    if(!esAbbVacio(abb)){
	    leerElementoAbb(abb,&arbolAux);
        escribirElemento(arbolAux,fp);

	    preordenArchivo(izqAbb(abb), fp);	
	    preordenArchivo(derAbb(abb), fp);
	}
}

void escribirArchivo(TABB abb, char* fichero){

    //La funcion solo abre el archivo, llama a la funcion recursiva y cuando acaba cierra el archivo
    FILE* fp = fopen(fichero,"w+");
    preordenArchivo(abb,fp);
    fclose(fp);
}

//Asesino

int compAsesino(TIPOELEMENTOABB elem, char* nombre){

    TPOSICION pos;
    TIPOELEMENTOLISTA asesino;

    pos = primeroLista(elem.killed);

    //Recorre la lista de killed de cada personaje, si encuentra a un asesinado con el mismo nombre que el dado, lo imprime por pantalla y devuelve 1
    do{
        recuperarElementoLista(elem.killed,pos,&asesino);
        
        if(strcmp(asesino.nameP,nombre) == 0){
            printf("%s, ",elem.name);
            return 1;
        }
        
        pos = siguienteLista(elem.killed,pos);
    }while(pos != finLista(elem.killed));

    return 0;
}

int inOrdenAsesino(TABB abb, char* nombre){

    TIPOELEMENTOABB arbolAux;

    int found = 0;
    int func;

    //La funcion se llama recursivamente para buscar asesinos, si alguna devuelve 1, la que la llamo tambien lo devuelve

    if(!esAbbVacio(abb)){
        leerElementoAbb(abb,&arbolAux);
        if(esListaVacia(arbolAux.killed) == 0){
            if(func = compAsesino(arbolAux, nombre) == 1){
                found = 1;
            }
        }
	    if(func = inOrdenAsesino(izqAbb(abb), nombre) == 1){
            found = 1;
        }
        if(func = inOrdenAsesino(derAbb(abb), nombre) == 1){
            found = 1;
        }
    }
    if(found == 1){
        return 1;
    }else{
        return 0;
    }
}

void buscarAsesino(TABB abb){

    char nombre[100];
    int num = 0;
    
    //Pide el nombre del asesino
    printf("\nIntroduzca nombre: ");
    getchar();
    scanf("%[^\n]", nombre);
    getchar();
    
    //Llama a la funcion recursiva de busqueda y si devuelve un 0, es que no encontro al asesino
    printf("\nLos asesinos de %s son:\n\t", nombre);
    num = inOrdenAsesino(abb,nombre);
    if(num == 0){
        printf("El personoaje aun no esta muerto o su asesino no esta en la base de datos\n");
    }
}

//Hijos, funciona igual que la de buscar asesinos, solo cambiando en que lista busca

int compHijos(TIPOELEMENTOABB elem, char* nombre){

    TPOSICION pos;
    TIPOELEMENTOLISTA hijos;

    pos = primeroLista(elem.siblings);

    do{
        recuperarElementoLista(elem.siblings,pos,&hijos);
        
        if(strcmp(hijos.nameP,nombre) == 0){
            printf("%s, ",elem.name);
            return 1;
        }
        
        pos = siguienteLista(elem.siblings,pos);
    }while(pos != finLista(elem.siblings));

    return 0;
}

int inOrdenHijos(TABB abb, char* nombre){

    TIPOELEMENTOABB arbolAux;

    int found = 0;
    int func;

    if(!esAbbVacio(abb)){
        leerElementoAbb(abb,&arbolAux);
        if(esListaVacia(arbolAux.siblings) == 0){
            if(func = compHijos(arbolAux, nombre) == 1){
                found = 1;
            }
        }
	    if(func = inOrdenHijos(izqAbb(abb), nombre) == 1){
            found = 1;
        }
        if(func = inOrdenHijos(derAbb(abb), nombre) == 1){
            found = 1;
        }
    }
    if(found == 1){
        return 1;
    }else{
        return 0;
    }
}

void buscarHijos(TABB abb){

    char nombre[100];
    int num = 0;
    

    printf("\nIntroduzca nombre: ");
    getchar();
    scanf("%[^\n]", nombre);
    getchar();
    
    printf("\nLos hijos de %s son:\n\t", nombre);
    num = inOrdenHijos(abb,nombre);
    if(num == 0){
        printf("Los hijos de este personaje no estan en las bases de datos\n");
    }
}

//Killer

void inOrdenKiller(TABB abb, TLISTA* asesinos, TPOSICION* pos, int* max){

    TIPOELEMENTOABB arbolAux;
    TIPOELEMENTOLISTA elemLista;

    //La funcion se llama recursivamente a si misma

    if(!esAbbVacio(abb)){
        leerElementoAbb(abb,&arbolAux);

        //Si encuentra una lista de asesinados mas grande que el maximo actual, vuelve el maximo ese numero, destruye la lista de asesinos
        //actual y crea una neuva, y mete al individuo en esa lista nueva
        if(longitudLista(arbolAux.killed) > *max){
            *max = longitudLista(arbolAux.killed);

            destruirLista(asesinos);
            crearLista(asesinos);
            *pos = primeroLista(*asesinos);

            strcpy(elemLista.nameP,arbolAux.name);
            insertarElementoLista(asesinos,*pos,elemLista);
            *pos = siguienteLista(*asesinos,*pos);

        //Si encuentra una lista de asesinados igual que el maximo actual, introduce al individuo en la lista
        }else if(longitudLista(arbolAux.killed) == *max){
            
            strcpy(elemLista.nameP,arbolAux.name);
            insertarElementoLista(asesinos,*pos,elemLista);
            *pos = siguienteLista(*asesinos,*pos);
        }
        
	    inOrdenKiller(izqAbb(abb),asesinos,pos,max);
        inOrdenKiller(derAbb(abb),asesinos,pos,max);
    }
}

void buscarKiller(TABB abb){

    int asesinatos = 0;

    //Crea las variables y listas
    TLISTA maxAsesinos;
    crearLista(&maxAsesinos);
    TPOSICION pos = primeroLista(maxAsesinos);

    //Llama a la funcion recursiva con la lista y el numero de asesinatos
    inOrdenKiller(abb, &maxAsesinos, &pos, &asesinatos);

    pos = primeroLista(maxAsesinos);
    TIPOELEMENTOLISTA elemLista;

    printf("\nEl mayor asesino es: \n\t");

    //Recorre la lista imprimiendo a los persoanjes con mayor numero de asesinatos y su numero
    while(pos != finLista(maxAsesinos)){
        recuperarElementoLista(maxAsesinos,pos,&elemLista);
        printf("%s, ",elemLista.nameP);
        pos = siguienteLista(maxAsesinos,pos);
    }

    printf("con %d asesinatos.\n", asesinatos);

}

//Modificar

void modificar(TABB *abb){


    //Crea las variables
    char nombre[100];
    char opcion;
    TIPOELEMENTOABB nodo;

    //Pide al usuario el nombre del personaje a modificar
    strcpy(nodo.name,"");
    printf("\nIntroduzca nombre del personaje a modificar: ");
    getchar();
    scanf("%[^\n]", nombre);
    TPOSICION pos;
    TIPOELEMENTOLISTA aux;

    //Si no encuentra el nombre, acaba la funcion
    buscarNodoAbb(*abb,nombre,&nodo);
    if(strcmp(nodo.name,"") == 0){
        printf("\nNo se encontro ningun personaje con ese nombre\n");
        return;
    }

    //Si lo encuentra, pregunta por el campo a modificar
    printf("\nPersonaje encontrado, elija campo a modificar:\n\tN: Nombre\n\tC: Casa\n\tR: Realeza\n\tP: Añadir Padres\n\tH: Añadir Hermanos\n\tA: Añadir Asesinados\n");;
    scanf(" %c", &opcion);

    switch (opcion)
    {
    case 'N':
    case 'n':

        //Si modifica el nombre, hace falta crear un nodo auxiliar al que duplicar todos los datos antes de borrarlo de la lista
        //Primero preguntamos el nuevo nombre y pasamos facilmente los primeros parametros
        TIPOELEMENTOABB nodoAux;

        printf("\nName: ");
        getchar();
        scanf("%[^\n]", nodoAux.name);
        strcpy(nodoAux.house,nodo.house);
        nodoAux.royal = nodo.royal;

        //Creamos las listas
        crearLista(&nodoAux.parents);
        crearLista(&nodoAux.siblings);
        crearLista(&nodoAux.killed);

        //Recorremos toda la lista del nodo original y copiamos todos los elementos en el nodo auxiliar, hacemos esto con todas las listas
        pos = primeroLista(nodo.parents);
        TPOSICION auxPos = primeroLista(nodoAux.parents);
        TIPOELEMENTOLISTA auxLista;
        while(pos != finLista(nodo.parents)){
            recuperarElementoLista(nodo.parents,pos,&auxLista);
            insertarElementoLista(&nodoAux.parents,auxPos,auxLista);
            pos = siguienteLista(nodo.parents,pos);
            auxPos = siguienteLista(nodoAux.parents,auxPos);
        }

        pos = primeroLista(nodo.siblings);
        auxPos = primeroLista(nodoAux.siblings);
        while(pos != finLista(nodo.siblings)){
            recuperarElementoLista(nodo.siblings,pos,&auxLista);
            insertarElementoLista(&nodoAux.siblings,auxPos,auxLista);
            pos = siguienteLista(nodo.siblings,pos);
            auxPos = siguienteLista(nodoAux.siblings,auxPos);
        }

        pos = primeroLista(nodo.killed);
        auxPos = primeroLista(nodoAux.killed);
        while(pos != finLista(nodo.killed)){
            recuperarElementoLista(nodo.killed,pos,&auxLista);
            insertarElementoLista(&nodoAux.killed,auxPos,auxLista);
            pos = siguienteLista(nodo.killed,pos);
            auxPos = siguienteLista(nodoAux.killed,auxPos);
        }

        //Al final suprimimos el nodo original del arbol e introducimos el auxiliar
        suprimirElementoAbb(abb,nodo);
        insertarElementoAbb(abb,nodoAux);
        break;
    case 'C':
    case 'c':

        //Preguntamos la nueva casa y cambiamos el campo del nodo, luego lo modificamos
        strcpy(nodo.house,"");
        printf("\nHouse (? si desconocido):");
        getchar();
        scanf("%[^\n]", nodo.house);
        modificarElementoAbb(*abb,nodo);
        break;
    case 'R':
    case 'r':

        //Preguntamos la nueva realeza y cambiamos el campo del nodo, luego lo modificamos
        printf("\nRoyal (0/1): ");
        scanf("%d", &nodo.royal);
        while(nodo.royal != 0 && nodo.royal != 1){
            printf("\nNumero no valido, introduzca de nuevo: ");
            getchar();
            scanf("%d", &nodo.royal);
        }
        modificarElementoAbb(*abb,nodo);
        break;
    case 'P':
    case 'p':

        pos = finLista(nodo.parents);

        //Introducimos elementos nuevos en la lista de parents hasta que veamos fin, luego modificamos el elemento
        while(strcmp(aux.nameP, "fin") != 0){
            printf("\nParents (fin para terminar): ");
            getchar();
            scanf("%[^\n]", aux.nameP);

            if(strcmp(aux.nameP, "fin") != 0){
                insertarElementoLista(&nodo.parents,pos,aux);
            }
        }
        modificarElementoAbb(*abb,nodo);
        
        break;
    case 'H':
    case 'h':

        pos = finLista(nodo.siblings);

        //Introducimos elementos nuevos en la lista de siblings hasta que veamos fin, luego modificamos el elemento
        while(strcmp(aux.nameP, "fin") != 0){
            printf("\nSiblings (fin para terminar): ");
            getchar();
            scanf("%[^\n]", aux.nameP);

            if(strcmp(aux.nameP, "fin") != 0){
                insertarElementoLista(&nodo.siblings,pos,aux);
            }
        }
        modificarElementoAbb(*abb,nodo);

        break;
    case 'A':
    case 'a':

        pos = finLista(nodo.killed);

        //Introducimos elementos nuevos en la lista de killed hasta que veamos fin, luego modificamos el elemento
        while(strcmp(aux.nameP, "fin") != 0){
            printf("\nKilled (fin para terminar): ");
            getchar();
            scanf("%[^\n]", aux.nameP);

            if(strcmp(aux.nameP, "fin") != 0){
                insertarElementoLista(&nodo.killed,pos,aux);
            }
        }
        modificarElementoAbb(*abb,nodo);

        break;
    default:
        printf("\nEsa no es una opcion valida\n");
        break;
    }
}