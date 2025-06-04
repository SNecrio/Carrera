#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    // Declaración de variables
    uint16_t puerto = atoi(argv[1]);
    int socketServidor, socketConexion;
    struct sockaddr_in direccion, ipportcli;
    socklen_t tamano = sizeof(struct sockaddr_in);
    char mensaje[60] = "Hey listen, watch out\n";
    ssize_t bytes;

    // Creamos el socket de conexion
    socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor < 0)
    {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = htonl(INADDR_ANY);
    direccion.sin_port = htons(puerto);

    if (bind(socketServidor, (struct sockaddr *)&direccion, sizeof(struct sockaddr_in)) < 0)
    {
        perror("No se pudo asignar direccion");
        exit(EXIT_FAILURE);
    }

    if (listen(socketServidor, 1) != 0)
    {
        perror("No se pudo escuchar la conexion");
        exit(EXIT_FAILURE);
    }

    printf("\n~~ Server Port: %d\n", puerto);

    while (1)
    {
        if ((socketConexion = accept(socketServidor, (struct sockaddr *)&ipportcli, &tamano)) < 0)
        {
            perror("No se pudo aceptar la conexion");
            exit(EXIT_FAILURE);
        }
        char miiptext[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, (const void *) &ipportcli.sin_addr.s_addr, miiptext , INET_ADDRSTRLEN) != NULL) {
            printf ("Se ha conectado el cliente con ip %s\n" , miiptext);
        }
        if((bytes=send(socketConexion, mensaje, strlen(mensaje)+1, 0))!=-1){
            printf("Se han enviado %li bytes\n",bytes);
        }
        close(socketConexion);
    }
    close(socketServidor);

    return 0;
}
