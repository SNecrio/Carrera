#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{

    // Comprobamos que el número de argumentos coincide con archivo e IP
    if (argc != 3)
    {
        perror("Se ha iniciado el programa con un número incorrecto de argumentos\n");
        return 1;
    }

    // Abrimos el archivo y comprobamos que se abrio correctamente
    FILE *archivo;

    archivo = fopen(argv[1], "r");
    if (archivo == NULL)
    {   
        perror("Error al abrir el archivo:\n");
        return 1;
    }

    struct in_addr ipEntradaStruct;
    struct in_addr ipArchivoStruct;

    // Inicializamos a 0 para evitar datos previos en esa dirección de memoria
    ipEntradaStruct.s_addr = 0;
    ipArchivoStruct.s_addr = 0;

    char IPEntrada[INET_ADDRSTRLEN + 3];
        char IPEntrada2[INET_ADDRSTRLEN + 3];

    strcpy(IPEntrada,argv[2]);
    char IPArchivo[INET_ADDRSTRLEN + 3];
    char IPAux[INET_ADDRSTRLEN + 3];

    // Inicializamos a 0 para evitar datos previos en esa dirección de memoria
    int sufijo = 0;
    int sufijoAux = 0;
    // La interfaz por defecto es 0 por eso se inicializa así
    int interfaz = 0;
    int interfazAux = 0;

    // Hacemos la conversion de IP a red
    if(inet_pton(AF_INET, IPEntrada, &ipEntradaStruct.s_addr) != 1 ){
        perror("Error al convertir la IP\n");
        return 1;
    } 

    // Hacemos un while para leer el archivo hasta que llegue al final
    while (fscanf(archivo, "%[^,],%d\n", IPArchivo, &interfaz) > 0)
    {

        ipArchivoStruct.s_addr = 0;

        // Hacemos una conversion de IP con mascara y guardamos ambos campos por separado
        if ((sufijo = inet_net_pton(AF_INET, IPArchivo, (void *)&ipArchivoStruct, sizeof(struct in_addr))) < 0)
        {
            perror("No se pudo pasar la direccion a binario\n");
            return 1;
        }

        //Transformamos la ip de entrada a texto para aplicarle la máscara 
        if ((inet_net_ntop(AF_INET, (void *)&ipEntradaStruct, sufijo, IPEntrada2, INET_ADDRSTRLEN + 3)) == NULL)
        {
            perror("No se pudo pasar la direccion de vuelta a texto\n");
            return 1;
        }
        
        ipEntradaStruct.s_addr = 0;

        // Transformamos a binario para auxiliar en la comparacion
        if ((inet_net_pton(AF_INET, IPEntrada2, (void *)&ipEntradaStruct, sizeof(struct in_addr))) < 0)
        {
            perror("No se pudo pasar la direccion a binario\n");
            return 1;
        }
        printf("%s  tabla %s   %d\t%X\t%X\n", IPEntrada2, IPArchivo, sufijo, ipArchivoStruct.s_addr , ipEntradaStruct.s_addr);
        //Comprobamos si coinciden las direcciones base
        if (ipArchivoStruct.s_addr == ipEntradaStruct.s_addr)
        {
            //En caso de que coincidan comprobamos si el sufijo es mayor
            if (sufijoAux < sufijo)
            {
                sufijoAux = sufijo;
                interfazAux = interfaz;
                strcpy(IPAux, IPArchivo);
                printf("%s\t%d\n", IPAux, sufijo);
            }
        }
    }

    printf("IP de salida: %s, Interfaz de salida: %d, Nº de bits del prefijo: %d\n",IPAux, interfazAux, sufijoAux);

    return 0;
}
