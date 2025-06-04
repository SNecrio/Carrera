#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MESSAGE_LEN 1000

int main(int argc, char **argv){
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
    char recievedMessage[MESSAGE_LEN];

    // Create the socket
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSock < 0){
        perror("\nUnable to create socket\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the sockaddr_in struct
    inet_pton(AF_INET, serverIP, &clientSocketAddress.sin_addr);
    clientSocketAddress.sin_port = htons(portNum);
    clientSocketAddress.sin_family = AF_INET;

    // Solicit connection
    if(connect(clientSock, (struct sockaddr *) &clientSocketAddress, socketSize) == -1){
        perror("\nUnable to connect\n");
        exit(EXIT_FAILURE);
    }

    // Recieve message
    ssize_t bytesReceived = recv(clientSock, recievedMessage, MESSAGE_LEN, 0);
    if(bytesReceived == -1){
        perror("\nUnable to receive message\n");
        exit(EXIT_FAILURE);
    }
    if(bytesReceived == 0){
        perror("\nConnection socket closed\n");
        exit(EXIT_FAILURE);
    }
    printf("\nReceived message: %s\n", recievedMessage);
    printf("\nBytes received: %ld\n", bytesReceived);

    // Close socket
    close(clientSock);
    
    return 0;
}
