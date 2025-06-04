#!/bin/bash

#Comprobar que se pasaron dous parámetros
if [ $# -ne 2 ]; then
	echo "Parámetros incorrectos. Exemplo de uso: accesos.sh [-c, GET/POST, -s] ruta/al/archivo/de/log/access.log"
	exit 1
fi

# Obter o arquivo
ruta=$2

# Verificar que o arquivo existe
if [ ! -f "$ruta" ] || [ ! -r $ruta ]; then
    	echo "Error: O arquivo indicado non é válido."
    	echo "Exemplo de uso: accesos.sh [-c, GET/POST, -s] ruta/al/archivo/de/log/access.log"
    	exit 1
fi

case $1 in 
#Opción -c: Muestra los diferentes codigos de respuesta sin repeticion. Indicara, ademas, el numero de veces que se produce cada uno de dichos codigos.
-c)
	#Collemos o penúltimo argumento, agrupamos os códigos iguais e imprimimos as veces que se producen
	echo "Nº aparicions  |  Codigo"
	awk '{print $(NF-1)}' "$ruta" | sort | uniq -c
    	exit 0;;

# Opción -t: Muestra el numero de dıas para los que no hay ningun acceso al servidor, desde la fecha del primer acceso hasta la fecha del ultimo
-t)
   	# Extraer as datas en formato día/mes/ano e ordenalas (segundo ano, mes e día)
	fechas=$(awk -F'[][]' '{print $2}' "$ruta" | sort -t/ -k3,3n -k2,2M -k1,1n)
    
    	# Collemos as datas ordenadas
    	primeira=$(echo "$fechas" | head -n 1) #head colle a primeira
    	ultima=$(echo "$fechas" | tail -n 1) #tail colle a última
    
    	#Convertimos as datas a timestamp (en segundos dende 1970)
	seg1=$(echo "$primeira" | awk -F'[/:]' '{print $1, $2, $3, $4":"$5":"$6}' | xargs -I{} date -d "{}" +%s)
	seg2=$(echo "$ultima" | awk -F'[/:]' '{print $1, $2, $3, $4":"$5":"$6}' | xargs -I{} date -d "{}" +%s)
	
	#Quitamos a hora de fechas (para cando contemos días de acceso sen repetición)
	fechas=$(echo "$fechas" | awk -F':' '{print $1}' | sort -u)
   
    	#Restamos as datas e dividimos para obter días
    	diferencia=$(( (seg2 - seg1) / 86400 ))
    	#Contamos as liñas de fechas (xa serán sen repetición)
    	dias_acceso=$(echo "$fechas" | wc -l)
	
    	sinacceso=$((diferencia - dias_acceso))
    
    	#Como a división para pasar a días non é exacta, se obtemos un número negativo significa que houbo accesos todos os días
    	if [ $sinacceso -lt 0 ]; then
    		sinacceso=0
	fi

    	echo "Días sin acceso: $sinacceso"
    	exit 0;;
    
# Cuenta el total de accesos con una petici´on tipo GET/POST con una respuesta 200
GET|POST)
	#Contamos cantas veces aparecen accesos tipo "GET/"POST con código de resposta 200
	numero=$(grep "\"$1" "$ruta" | grep ' 200 ' | wc -l)
	#Poñemos a data actual no echo
	echo "$(date). Registrados $numero accesos tipo $1 con respuesta 200"
	exit 0;;

#Resume el total de Datos enviados en KiB por cada mes.
-s)
	awk '{ 
		#Collemos as datas (4º columna) e separamos os campos por "/"
    		split($4, data, "/")    		
    		mes = data[2] #Collemos o mes (segundo argumento)
    		# Tomamos o nº de bytes ao final da liña
    		bytes = $NF 
    		#SUmamos os bytes de cada entrada ao mes correspondente e o nº de accesos
    		total_bytes[mes]+=bytes
    		accesos[mes]++
    		
	} END {
	
    	# Imprimimos os resultados por mes
    		for (m in total_bytes) {
        	printf "%d KiB sent in %s by %d accesses\n", total_bytes[m]/1024, m, accesos[m]
    		}
}' $ruta
	exit 0;;

#Ordena las lıneas del fichero access.log en orde decreciente del numero de bytes enviados
-o)
	awk '{print $NF}' "$ruta" | sort -nr > access_ord.log
	echo "Ficheiro creado con exito."
	exit 0;;

esac

exit 0
