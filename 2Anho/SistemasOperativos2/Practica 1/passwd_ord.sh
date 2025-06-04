#!/bin/bash

#Copiamos el archivo dos veces
cat /etc/passwd > ~/passwd_copia
cat /etc/passwd >> ~/passwd_copia

#Ordena el archivo y copia las lineas originales en el archivo de destino
sort ~/passwd_copia | uniq > /tmp/passwd_original

#Contamos la diferencia de lineas
echo $[$(wc -l < ~/passwd_copia) - $(wc -l < /tmp/passwd_original)]

exit 0