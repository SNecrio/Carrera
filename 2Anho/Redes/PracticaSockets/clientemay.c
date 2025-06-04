#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#define MESSAGE_LEN 1024

int main(int argc, char **argv){
    // Check if the args recieved from the command line are correct
    if(argc < 4){
        perror("\nIncorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }

    // Declare the variables
    char *serverIP = argv[2];
    int portNum = atoi(argv[3]), clientSock;
    struct sockaddr_in clientSocketAddress;
    socklen_t socketSize = sizeof(struct sockaddr_in);

    // Create the socket
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
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

    // Solicit connection
    if(connect(clientSock, (struct sockaddr *) &clientSocketAddress, socketSize) == -1){
        perror("\nUnable to connect\n");
        exit(EXIT_FAILURE);
    }

    //Open files
    FILE* lowecaseFile = fopen(argv[1], "r");
    if(lowecaseFile == NULL){
        perror("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }
    char *uFileName = malloc(strlen(argv[1]) * 4);
    for(int i = 0; i < strlen(argv[1]); i++)
            uFileName[i] = toupper(argv[1][i]);

    FILE* uppercaseFile = fopen(uFileName, "w");
    if(uppercaseFile == NULL){
        perror("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    // Send message to server
    char buffer[MESSAGE_LEN];
    ssize_t totalBytesSent = 0;
    ssize_t totalBytesReceived = 0;
    while(fgets(buffer, sizeof(buffer), lowecaseFile) != NULL){
        ssize_t bytesSent = send(clientSock, buffer, strlen(buffer) + 1, 0);
        if(bytesSent < 0){
            perror("Error sending message to server\n");
            exit(EXIT_FAILURE);
        }
        totalBytesSent += bytesSent;

        sleep(1);

        //Receive message from server (in uppercase)
        char recvMessage[MESSAGE_LEN];
        ssize_t bytesReceived = recv(clientSock, recvMessage, sizeof(recvMessage), 0);
        if(bytesReceived < 0){
            perror("Error receiving message from server\n");
            exit(EXIT_FAILURE);
        }
        recvMessage[bytesReceived] = '\0';
        totalBytesReceived += bytesReceived;
        fprintf(uppercaseFile, "%s", recvMessage);
    }


    free(uFileName);
    fclose(lowecaseFile);
    fclose(uppercaseFile);
    close(clientSock);
    return 0;
}
