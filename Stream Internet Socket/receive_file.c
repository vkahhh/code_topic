#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "transfer.h"

void writefile(int sockfd, FILE *fp);

int main(int argc, char *argv[]) 
{
/*Stream Internet socket*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("Error");
        exit(1);
    }
    
    /*Config stream socket*/
    struct sockaddr_in clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr)); 
    serveraddr.sin_family = AF_INET; 
/*via IPv4*/
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
/*INADDR_ANY: This is an IP address that is used when we don't want to bind a socket to any specific IP.*/
    serveraddr.sin_port = htons(SERVERPORT);
/*The htons() function translates a short integer from host byte order to network byte order.*/

    /*Binding*/
    if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) 
    {
        perror("Bind Error");
        exit(1);
    }

    /*Listen*/
    if (listen(sockfd, LINSTENPORT) == -1) 
    {
        perror("Listen Error");
        exit(1);
    }
/*wait for the connection to complete*/
    socklen_t addrlen = sizeof(clientaddr);
    int connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen);
    if (connfd == -1) 
    {
        perror("Connect Error");
        exit(1);
    }
    close(sockfd);
/*Close Socket*/

    /*get Filename*/
    char filename[BUFFSIZE] = {0}; 
    if (recv(connfd, filename, BUFFSIZE, 0) == -1) 
    {
        perror("Can't receive filename");
        exit(1);
    }

    /*open file*/
    FILE *fp = fopen(filename, "wb"); /*wb: File mode, write and binary. */
    if (fp == NULL) 
    {
        perror("Can't open file");
        exit(1);
    }
    
    /*Write data to file*/
    char addr[INET_ADDRSTRLEN];
    printf("Start receive file: %s from %s\n", filename, inet_ntop(AF_INET, &clientaddr.sin_addr, addr, INET_ADDRSTRLEN));
/*This function converts the network address structure src in the
       af address family into a character string.*/
    writefile(connfd, fp);
    puts("Receive Success");

 
    fclose(fp);
    close(connfd);
    return 0;
}

void writefile(int sockfd, FILE *fp)
{
    ssize_t n; 
    char buff[MAX_LINE] = {0}; 
    while ((n = recv(sockfd, buff, MAX_LINE, 0)) > 0) 
    {
        if (n == -1)
        {
            perror("Receive File Error");
            exit(1);
        }
        
       
        if (fwrite(buff, sizeof(char), n, fp) != n)
        {
            perror("Write File Error");
            exit(1);
        }
        memset(buff, 0, MAX_LINE); 
    }
}
