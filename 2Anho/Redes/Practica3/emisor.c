#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MESSAGE_LEN 1024

int main(int argc, char **argv){
    
    printf("Iniciando cliente...\n");

    // Check if the args recieved from the command line are correct
    if(argc < 3){
        perror("\nIncorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }

    // Declare the variables
    char *serverIP = argv[1];
    int portNum = atoi(argv[2]), clientSock;
    struct sockaddr_in clientSocketAddress;
    socklen_t socketSize = sizeof(struct sockaddr_in);

    // Create the socket
    clientSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientSock < 0){
        perror("\nUnable to create socket\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the sockaddr_in struct
    if(inet_pton(AF_INET, serverIP, &clientSocketAddress.sin_addr) != 1){
        perror("\nError in IP conversion\n");
        exit(EXIT_FAILURE);
    }
    clientSocketAddress.sin_port = htons(portNum);
    clientSocketAddress.sin_family = AF_INET;

    // Send message to server
    char message[MESSAGE_LEN] = "Hola, soy el cliente, me recibes?";
    ssize_t totalBytesSent = 0;
    
    ssize_t bytesSent = sendto(clientSock, message, strlen(message) + 1, 0, (struct sockaddr *)&clientSocketAddress, sizeof(clientSocketAddress));
    if(bytesSent < 0){
        perror("Error sending message to server\n");
        exit(EXIT_FAILURE);
    }

    /*
    float numArray[] = {1.2f, 1.4f, 2.6f, 76.341f, 69.69f};
    char numMessage[sizeof(float)];

    int i = 0;
    while(i < 5){
        memcpy(numMessage,&numArray[i],sizeof(float));

        ssize_t bytesSent = sendto(clientSock, numMessage, strlen(numMessage) + 1, 0, (struct sockaddr *)&clientSocketAddress, sizeof(clientSocketAddress));
        if(bytesSent < 0){
            perror("Error sending message to server\n");
            exit(EXIT_FAILURE);
        }
        printf("Bytes enviados: %ld\n", bytesSent);

        i++;
    }
    */

    printf("Bytes enviados: %ld\n", bytesSent);
    close(clientSock);
    return 0;
}
