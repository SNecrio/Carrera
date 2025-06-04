#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#define PETITIONS_NUM 10
#define MESSAGE_LEN 1024

int main(int argc, char **argv){
    // Check if the args received from the command line are correct
    if(argc < 2){
        perror("\nIncorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }

    // Declare the variables
    int portNum = atoi(argv[1]), serverSock, connectionSock;
    struct sockaddr_in serverSocketAddress; // Server address
    serverSocketAddress.sin_family = AF_INET;
    serverSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverSocketAddress.sin_port = htons(portNum);
    socklen_t socketSize = sizeof(struct sockaddr_in);
    struct sockaddr_in clientSocketAddress;

    // Create the server socket
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock < 0){
        perror("\nUnable to create socket\n");
        exit(EXIT_FAILURE);
    }

    // Assing address and port to the socket
    if(bind(serverSock, (struct sockaddr *) &serverSocketAddress, socketSize) != 0){
        perror("\nUnable to assign address to socket\n");
        exit(EXIT_FAILURE);
    }
    char serverIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &serverSocketAddress.sin_addr, serverIP, INET_ADDRSTRLEN);


    // Set socket as passive so it can listen petitions
    if(listen(serverSock, PETITIONS_NUM) != 0){
        perror("\nUnable to set socket as passive\n");
        exit(EXIT_FAILURE);
    }

    // Accept connection
    connectionSock = accept(serverSock, (struct sockaddr *)&clientSocketAddress, &socketSize);
    if(connectionSock == -1){
        perror("\nUnable to accept connection\n");
        exit(EXIT_FAILURE);
    }

    char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientSocketAddress.sin_addr, clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientSocketAddress.sin_port);
        printf("\n~~ Client IP: %s", clientIP);
        printf("\n~~ Client Port: %d\n", clientPort);

     while(1){
        // Receive message from client
        char receivedMessage[MESSAGE_LEN];
        ssize_t totalBytesReceived = 0;
        ssize_t bytesReceived = recv(connectionSock, receivedMessage, sizeof(receivedMessage), 0);
        if(bytesReceived < 0){
            perror("Error receiving message from client\n");
            exit(EXIT_FAILURE);
        } else if (bytesReceived == 0){
            perror("Client Disconnected\n");
        }
        receivedMessage[bytesReceived] = '\0';

        totalBytesReceived += bytesReceived;
        // Transform to uppercase
        for(int i = 0; i < bytesReceived; i++)
            receivedMessage[i] = toupper(receivedMessage[i]);

        // Send the message to the client
        ssize_t totalBytesSent = 0;
        ssize_t bytesSent = send(connectionSock, receivedMessage, bytesReceived, 0);
        if(bytesSent < 0){
            perror("Error sending message to client\n");
            exit(EXIT_FAILURE);
        }
        totalBytesSent += bytesSent;

    }
    close(serverSock);

    return 1;
}