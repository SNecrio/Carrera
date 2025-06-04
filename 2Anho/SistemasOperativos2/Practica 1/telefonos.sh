#!/bin/bash

#Comprobamos que tenemos un argumetno, que es la ruta al archivo de agenda
if [ "$#" -ne 1 ]; then
    echo "Error: Se requiere exactamente 1 argumento, el archivo a utilizar como agenda"
    echo "Ejemplo:"
    echo "./telefonos.sh agenda"
    exit 1
fi

agenda=$1

#Comprobamos que es un archivo y que tiene los permisos necesarios
if [ ! -f "$agenda" ]; then
    echo "Error: '$agenda' no es un archivo valido"
    echo "Ejemplo:"
    echo "./telefonos.sh agenda"
    exit 1
fi
if [ ! -w "$agenda" ]; then
    echo "Error: El archivo '$agenda' no tene permisos de escritura"
    echo "Ejemplo:"
    echo "./telefonos.sh agenda"
    exit 1
fi
if [ ! -r "$agenda" ]; then
    echo "Error: El archivo '$agenda' no tene permisos de lectura"
    echo "Ejemplo:"
    echo "./telefonos.sh agenda"
    exit 1
fi


#Hacemos un bucle infinito hasta romperlo con el comando de salida
while true; do

#Imprimimos por pantalla las opciones disponibles y leemos la seleccionada por el usuario
    cat <<EOF 
Opciones disponibles:
    [a]: Crear nueva agenda
    [r]: Registrar nueva entrada
    [n]: Buscar por nombre
    [t]: Buscar por numero de telefono
    [m]: Modificar una entrada
    [e]: Eliminar una entrada
    [s]: Salir
EOF
    
    read entrada

    #Vemos que opcion eligio el usuario
    case "$entrada" in
    #Crear agenda
        "a") 
        #Borramos el contenido del archivo a utilizar para poder usarlo como una agenda nueva desde 0
            > "$agenda"
            echo "Nueva agenda creada";;

    #Introducir un nombre con un numero
        "r")
        #Leemos el nombre y numeros a introducir
            echo -n "Introduzca nombre del dueño del numero: "
            read nombre
            echo -n "Introduzca numero: "
            read numero

        #Introducimos al final de la agenda la nueva entrada
            echo "$nombre@$numero" >> "$agenda"
            echo "Numero introducido correctamente" ;;

    #Buscar por nombre
        "n")
            echo -n "Introduzca nombre del dueño del numero: "
            read nombre

    #Con un bucle comprobamos todas las lineas del archivo
            encontrado=0
            while IFS= read -r linea; do

    #Cortamos la linea para tener solo el nombre
                nombreLeido=$(echo "$linea" | cut -d'@' -f1)

    #Si el nombre coincide, imprimimos solo su numero, para lo cual tenemos que cortar la linea y romper del bucle
                if [ "$nombreLeido" = "$nombre" ]; then
                    
                    numeroLeido=$(echo "$linea" | cut -d'@' -f2)
                    echo "Numero encontrado: $numeroLeido"
                    encontrado=1
                    break;
                fi
    
    #Si no encontramos el numero, imprimimos un mensaje avisando al usuario
            done < "$agenda"
            if [ "$encontrado" -eq 0 ]; then
                echo "Numero no encontrado"
            fi ;;

    #Buscar por numero
        "t")

    #Buscamos igual que cuando buscamos por nombre, pero comprobamos la segunda parte de la linea e imprimimos la primera
            echo -n "Introduzca numero: "
            read numero

            encontrado=0
            while IFS= read -r linea; do
                numeroLeido=$(echo "$linea" | cut -d'@' -f2)

                if [ "$numeroLeido" = "$numero" ]; then
                    
                    nombreLeido=$(echo "$linea" | cut -d'@' -f1)
                    echo "Nombre encontrado: $nombreLeido"
                    encontrado=1
                    break;
                fi

            done < "$agenda"
            if [ "$encontrado" -eq 0 ]; then
                echo "Nombre no encontrado"
            fi ;;

    #Modificar una entrada
        "m") 

            echo -n "Introduzca nombre del dueño del numero: "
            read nombre
            echo -n "Introduzca nuevo nombre: "
            read nombreNuevo
            echo -n "Introduzca nuevo numero: "
            read numeroNuevo

    #Buscamos en todo el archivo la linea
            encontrado=0
            while IFS= read -r linea; do
                nombreLeido=$(echo "$linea" | cut -d'@' -f1)
                if [ "$nombreLeido" = "$nombre" ]; then

    #Modificamos la linea que contenga el nombre y le ponemos el nombre y numero nuevos
                    sed -i "/$nombre/s/.*/$nombreNuevo@$numeroNuevo/" "$agenda"

                    echo "Entrada modificada"
                    encontrado=1
                    break;
                fi
            done < "$agenda"
            if [ "$encontrado" -eq 0 ]; then
                echo "Numero no encontrado"
            fi ;;

    #Eliminar una entrada
        "e")
            echo -n "Introduzca nombre del dueño del numero: "
            read nombre

    #Buscamos en todo el archivo y contamos las lineas para saber cual eliminar
            encontrado=0
            n=0
            while IFS= read -r linea; do
                nombreLeido=$(echo "$linea" | cut -d'@' -f1)
                ((n++))
                if [ "$nombreLeido" = "$nombre" ]; then
    #Si se encuentra, se elimina la linea por su numero
                    sed -i "${n}d" "$agenda"

                    echo "Entrada eliminada"
                    encontrado=1
                    break;
                fi
            done < "$agenda"
            if [ "$encontrado" -eq 0 ]; then
                echo "Numero no encontrado"
            fi ;;

        "s") 
            echo "Saliendo..."
            exit 0
            ;;
        *) echo "Opción no válida. Inténtalo de nuevo." ;;
    esac
done

exit 0