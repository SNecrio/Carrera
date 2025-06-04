#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abin.h"
#include "pilaOperadores.h"
#include "pilaOperando.h"

//Devuelve la prioridad del operador dentro de la pila.
//Prioridad('(')=0
int prioridadDentro(char op) {
    int prior;
    switch (op) {
        case '^': prior = 3;
            break;
        case '*': case '/': prior = 2;
            break;
        case '+': case '-': prior = 1;
            break;
        case '(': prior = 0; //nunca va a entrar en la pila, provoca vaciado
            break;
    }
    return prior;
}

//Devuelve la prioridad del operador fuera de la pila.
//Prioridad('(')=4
int prioridadFuera(char op) {
    int prior;
    switch (op) {
        case '^': prior = 3;
            break;
        case '*': case '/': prior = 2;
            break;
        case '+': case '-': prior = 1;
            break;
        case '(': prior = 4; //para que SIEMPRE entre en la pila
            break;
    }
    return prior;
}

//Devuelve 1 si c es un operador: +, -, /, *, ^, ()
unsigned esOperador(char c) {
    unsigned ope;
    switch (c) {
        case '^': case '*': case '/': case '+': case '-': case '(':
            ope = 1;
            break;
        default:
            ope = 0;
    }
    return ope;
}

//Devuelve 1 si c es un operando: mayúsculas y minúsculas
//completar para caracteres 0 a 9
unsigned esOperando(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))
        return 1;
    else
        return 0;
}

void desapilar(pilaOperadores* pOpes, pilaOperando* pOpando){
    abin auxbol; //Creamos el arbol
    crear(&auxbol);

    tipoelemPilaOperadores aux1 = topeOperadores(*pOpes); //Obtenemos el primer elemento de la pila y lo eliminamos de esta
    popOperadores(pOpes);
    insder(&auxbol,aux1); //Insertamos el elemento

    tipoelemPilaOperando aux2 = topeOperando(*pOpando); //Guardamos los 2 primeros operandos en el arbol y los eliminamos
    popOperando(pOpando);
    insArbolder(&auxbol,aux2);

    aux2 = topeOperando(*pOpando);
    popOperando(pOpando);
    insArbolizq(&auxbol,aux2);

    pushOperando(pOpando,auxbol); //Guardamos el arbol que contiene los datos

}

//Recibe una cadena de caracteres y devuelve el arbol de expresion
//DEBES ESCRIBIR ESTA FUNCIÓN
abin arbolExpresion(char *expr_infija) {

    abin arbol, auxArbol;
    crear(&arbol);

    pilaOperadores pOpes;
    pilaOperando pOpando;
    crearPilaOperadores(&pOpes);
    crearPilaOperando(&pOpando);

    char c = 'a', caux;
    int i = 0;
    while(c != '\0'){
        c = expr_infija[i];
        if(esOperando(c) == 1){
            crear(&auxArbol);
            insder(&auxArbol,c);
            pushOperando(&pOpando,auxArbol);
        }else if(esOperador(c)==1){
            while(!esVaciaPilaOperadores(pOpes) && prioridadFuera(c)<= prioridadDentro(topeOperadores(pOpes))){
                desapilar(&pOpes,&pOpando);
            }
            pushOperadores(&pOpes,c);
        }else if(c == ')'){
            while(topeOperadores(pOpes) != '('){
                desapilar(&pOpes, &pOpando);
            }
            popOperadores(&pOpes);
        }
        i++;
    }

    while(!esVaciaPilaOperadores(pOpes)){
        desapilar(&pOpes, &pOpando);
    }

    destruirPilaOperadores(&pOpes);
    arbol = topeOperando(pOpando);
    destruirPilaOperando(&pOpando);
    return arbol;
   
}