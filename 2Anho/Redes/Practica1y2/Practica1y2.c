#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){

    uint16_t puertoH = 5005;
    uint16_t puertoN;

    puertoN = htons(puertoH);

    printf("Primer valor: %x\nConvertido a Net: %x\nConvertido de vuelta: %x\n",puertoH,puertoN,ntohs(puertoN));

    char ipv4t[] = "104.22.70.253";
    int ipv4n;
    struct in_addr ipstruct;

    inet_pton(AF_INET,ipv4t,&ipstruct);

    printf("\nIP original por texto: %s\nIP en hexadecimal: %x\n",ipv4t,ipstruct.s_addr);
    
}