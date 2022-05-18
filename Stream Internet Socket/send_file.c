#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "transfer.h"

void sendfile(FILE *fp, int sockfd);

int main(int argc, char* argv[])
{
    /*form: ./send_file <path> <IP address>*/
    if (argc != 3) 
    {
        perror("Type: ./send_file <path> <IP address>");
        exit(1);
    }

    /*Create a Stream Internet Socket*/
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("Can't allocate sockfd");
        exit(1);
    }

    /*Config socket*/
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, argv[2], &serveraddr.sin_addr) < 0) /*Convert IP address format from string to binary*/
    {
        perror("IPaddress Convert Error");
        exit(1);
    }

    /*Connect*/
    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("Connect Error");
        exit(1);
    }
    
    /*get filename*/
    char *filename = basename(argv[1]); 
    if (filename == NULL)
    {
        perror("Can't get filename");
        exit(1);
    }
    
/* send file name
       In order to send the file name at one time, instead of temporarily storing it in the TCP send buffer, to avoid the other party from receiving redundant data, it is difficult to parse the correct file name.
       The size of the data to be sent needs to be set to the buffer size */
    char buff[BUFFSIZE] = {0};
    strncpy(buff, filename, strlen(filename));
    if (send(sockfd, buff, BUFFSIZE, 0) == -1)
    {
        perror("Can't send filename");
        exit(1);
    }
    
    /*open file */
    FILE *fp = fopen(argv[1], "rb"); /*rb: read and binary*/
    if (fp == NULL) 
    {
        perror("Can't open file");
        exit(1);
    }

    /*read and send file*/
    sendfile(fp, sockfd);
    puts("Send Success");

    /*close file and close socket*/
    fclose(fp);
    close(sockfd);
    return 0;
}

void sendfile(FILE *fp, int sockfd) 
{
    int n; /*read n data each time */
    char sendline[MAX_LINE] = {0}; /*Temporarily store the data read each time*/
    while ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0) 
    {
        if (n != MAX_LINE && ferror(fp)) 
        {
            perror("Read File Error");
            exit(1);
        }
        
        /*Send the read data to the TCP send buffer*/
        if (send(sockfd, sendline, n, 0) == -1)
        {
            perror("Can't send file");
            exit(1);
        }
        memset(sendline, 0, MAX_LINE); /*Clear the temporary string*/
    }
}
