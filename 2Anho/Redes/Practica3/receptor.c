#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    struct sockaddr_in direccion_receptor, direccion_emisor;
    int socket_receptor, socket_emisor, puerto;
    socklen_t tamanho = sizeof(direccion_emisor);
    socklen_t addr_len = sizeof(direccion_receptor);
    char mensaje[1024];

    if (argc != 2) {
        printf("Por favor, introduzca por linea de comandos <puerto>\n");
        exit(EXIT_FAILURE);
    }

    char *ip = "127.0.0.1";
    //Asignación del puerto con el primer argumento
    puerto = atoi(argv[1]);

    direccion_receptor.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip, &direccion_receptor.sin_addr) != 1) {
        fprintf(stderr, "Formato de direccion incorrecto\n");
        exit(EXIT_FAILURE);
    }
    direccion_receptor.sin_addr.s_addr = INADDR_ANY;  // Permite conexión entre ordenadores distintos
    direccion_receptor.sin_port = htons(puerto);

    // Creación del socket
    socket_receptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_receptor < 0) {
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
    // Asignación de dirección  
    if (bind(socket_receptor, (struct sockaddr *) &direccion_receptor, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo asignar direccion");
        exit(EXIT_FAILURE);
    }
    
    // Bucle infinito para que el receptor se mantenga abierto
    while (1) {
         printf("Esperando mensajes...\n");
        
        // Recibe el mensaje
        // Si se descomentan las modificaciones del apartado 1.c, se debe comentar este bloque
        ssize_t numerobytes = recvfrom(socket_receptor, mensaje, sizeof(mensaje), 0, (struct sockaddr *)&direccion_emisor, &addr_len);
        if (numerobytes < 0) {
            perror("Error al recibir el mensaje");
            exit(EXIT_FAILURE);
        }
        
        // Imprime el puerto e IP del emisor
        char ip_emisor[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &direccion_emisor.sin_addr, ip_emisor, sizeof(ip_emisor));
        int puerto_emisor = ntohs(direccion_emisor.sin_port);
        printf("IP del emisor: %s\n", ip_emisor);
        printf("Puerto del emisor: %d\n\n", puerto_emisor);
        
        // Imprime el mensaje recbido y los bytes que ocupa
        printf("El mensaje recibido es: %s\n", mensaje);
        printf("El numero de bytes es: %ld\n\n", numerobytes);


        for(int i = 0; i < numerobytes; i++)
            mensaje[i] = toupper(mensaje[i]);
        
        numerobytes = sendto(socket_receptor, mensaje, strlen(mensaje) + 1, 0, (struct sockaddr *)&socket_receptor, sizeof(socket_receptor));
        if(numerobytes < 0){
            perror("Error sending message to client\n");
            exit(EXIT_FAILURE);
        }

        printf("Bytes enviados: %ld\n\n", numerobytes);
        /*
        // MODIFICACIONES DEL APARTADO 1.D
        char mensajeNum[sizeof(float)];
        ssize_t numerobytes = recvfrom(socket_receptor, mensajeNum, sizeof(mensajeNum), 0, (struct sockaddr *)&direccion_emisor, &addr_len);

        if (numerobytes < 0) {
            perror("Error receiving message from client\n");
            exit(EXIT_FAILURE);
        }

        float receivedNum;
        memcpy(&receivedNum, mensajeNum, sizeof(float));
        printf("Número recibido: %f\n", receivedNum);
        printf("Nuero de bytes recibidos: %ld\n",numerobytes);
        */

        /*
        // MODIFICACIONES DEL APARTADO 1.C
        ssize_t numerobytes = recvfrom(socket_receptor, mensaje, 5, 0, (struct sockaddr *)&direccion_emisor, &addr_len);
        
        // Imprime el puerto e IP del emisor
        char ip_emisor[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &direccion_emisor.sin_addr, ip_emisor, sizeof(ip_emisor));
        int puerto_emisor = ntohs(direccion_emisor.sin_port);
        printf("IP del emisor: %s\n", ip_emisor);
        printf("Puerto del emisor: %d\n\n", puerto_emisor);
        
        if (numerobytes < 0) {
            perror("Error al recibir el mensaje");
            exit(EXIT_FAILURE);}
        // Imprime el mensaje recbido y los bytes que ocupa
        printf("El mensaje recibido es: %s\n", mensaje);
        printf("El numero de bytes es: %ld\n\n", numerobytes);
        
        numerobytes = recvfrom(socket_receptor, mensaje, 5, 0, (struct sockaddr *)&direccion_emisor, &addr_len);
        
        // Imprime el mensaje recbido y los bytes que ocupa
        printf("El mensaje recibido es: %s\n", mensaje);
        printf("El numero de bytes es: %ld\n\n", numerobytes);
        */
        /*
        // Imprime el puerto e IP del emisor
        char ip_emisor[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &direccion_emisor.sin_addr, ip_emisor, sizeof(ip_emisor));
        int puerto_emisor = ntohs(direccion_emisor.sin_port);
        printf("IP del emisor: %s\n", ip_emisor);
        printf("Puerto del emisor: %d\n\n", puerto_emisor);
        
        // Imprime el mensaje recbido y los bytes que ocupa
        printf("El mensaje recibido es: %s\n", mensaje);
        printf("El numero de bytes es: %ld\n\n", numerobytes);
        */
    }
    return 0;
}
