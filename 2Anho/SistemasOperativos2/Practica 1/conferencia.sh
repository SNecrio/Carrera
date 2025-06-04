#!/bin/bash

#Comprobamos que se nos pasaron 2 argumetnos exactamente

if [ "$#" -ne 2 ]; then
    echo "Error: Se requieren exactamente 2 argumentos, el directorio de origen y el directorio destino."
    echo "Ejemplo:"
    echo "./conferencia.sh dirInput dirOutput"
    exit 1
fi

origen=$1
destino=$2

#Comprobamos que los directorios dados existen y que tienen los permisos de lectura y escritura necesarios
if [ ! -d "$origen" ]; then
    echo "Error: '$origen' no es un directorio valido, ambos directorios tienen que existir"
    echo "Ejemplo:"
    echo "./conferencia.sh dirInput dirOutput"
    exit 1
fi
if [ ! -r "$origen" ]; then
    echo "Error: No tienes permisos de lectura en '$origen', el directorio destino tiene que tener permisos de escritura y el directorio de origen permisos de lectura."
    echo "Ejemplo:"
    echo "./conferencia.sh dirInput dirOutput"
    exit 1
fi

if [ ! -d "$destino" ]; then
    echo "Error: '$destino' no es un directorio valido, ambos directorios tienen que existir"
    echo "Ejemplo:"
    echo "./conferencia.sh dirInput dirOutput"
    exit 1
fi
if [ ! -w "$destino" ]; then
    echo "Error: No tienes permisos de escritura en '$destino', el directorio destino tiene que tener permisos de escritura y el directorio de origen permisos de lectura."
    echo "Ejemplo:"
    echo "./conferencia.sh dirInput dirOutput"
    exit 1
fi


#Creamos un array al que añadiremos todas las fechas distintas encontradas entre los archivos
todaFecha=()

for archivoBase in $origen/*; do

    encontrado=0
    archivo=$(basename "$archivoBase")
    fecha=$((echo "$archivo" | cut -d'_' -f3) | cut -d'@' -f1)

    for elemento in "${todaFecha[@]}"; do
        if [[ "$elemento" == "$fecha" ]]; then
            encontrado=1
            break
        fi
    done

    if [[ $encontrado -eq 0 ]]; then
        todaFecha+=("$fecha")
    fi

done

#Creamos directorios para todas las salas y todas las fechas
for i in {20..50}; do
    for a in "${todaFecha[@]}"; do
        mkdir -p "$destino/sala$i/$a"
    done
done


#Hacemos un for que comprueba todos los archivos en la carpeta de conference

for archivoBase in $origen/*; do

    #Cortamos la ruta al archivo
    archivo=$(basename "$archivoBase")

    #Cortamos cada parte del nombre del archivo y la guardamos en distintas variables
    sala=$(echo "$archivo" | cut -d'_' -f2)
    fecha=$((echo "$archivo" | cut -d'_' -f3) | cut -d'@' -f1)
    hora=$((echo "$archivo" | cut -d'@' -f2) | cut -d'.' -f1)
    calidad=$(echo "$archivo" | cut -d'.' -f2)

    #Usamos esas variables para crear el directorio correspondiente y mover el archivo a dentro suya
    mkdir -p "$destino/sala$sala/$fecha/$calidad"

    #Usamos cp en vez de mv para copiar los archivos en caso de ncesitar ejecutarlo mas veces
    cp "$archivoBase" "$destino/sala$sala/$fecha/$calidad/charla_$hora"
    #mv "$archivoBase" "$destino/sala$sala/$fecha/$calidad/charla_$hora"

done

exit 0