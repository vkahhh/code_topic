#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024 

int main (int argc, char *argv[])
{
   struct sockaddr_in svaddr;
   char buf [BUF_SIZE] = {0};
   int len = sizeof (svaddr);
 
   // khởi tạo internet dtg socket
   int fd = socket (AF_INET, SOCK_DGRAM, 0);
   if (fd == -1) 
        return 1;

   // khởi tạo các thông tin
   svaddr.sin_family = AF_INET;
   svaddr.sin_addr.s_addr = INADDR_ANY; //server chấp nhận mọi thay đổi từ ip
   svaddr.sin_port = 0;
 
 //binding socket với các thông tin trên
    if (bind (fd, (struct sockaddr *) &svaddr, sizeof (struct sockaddr_in)) == -1) 
        return 1;
   
   //getsockname: truy cập local name của socket
   if (getsockname (fd, (struct sockaddr *) &svaddr, &len) == -1) 
      exit (1);
   printf ("Socket port: %d\n", ntohs (svaddr.sin_port));
   //htons: chuyen int thanh host byte order (big-endian)
   // bên server là hton -> client: ntohs
   
   
   // đọc tin nhắn từ sc
   if ( read(fd, buf, 1024) == -1 )
      return 1;
   printf("-->%s\n", buf);
   
   close (fd);
   exit (0);
}