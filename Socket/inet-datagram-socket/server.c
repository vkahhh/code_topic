#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
 
#define DATA "troi dat oi"

//cmd: ./server 192.168.192.141 [port] 
int main (int argc, char *argv[])
{
   struct sockaddr_in svaddr;
   struct hostent *host, *gethostbyname();
 
   // tạo internet dtg socket
   int fd = socket (AF_INET, SOCK_DGRAM, 0);
   if (fd == -1) 
        return 1;

    // lấy địa chỉ host từ command line
   host = gethostbyname (argv[1]);
   if (host == (struct hostent *) 0) { //không nhập đúng hostname -> tin thông báo hostname ko hợp lệ
      fprintf(stderr, "%s: unknown host\n", argv[1]);
      exit(2);
   }

   // khởi tạo thông tin cho sc
   memcpy ((char *) &svaddr.sin_addr, (char *) host->h_addr, host->h_length);
   svaddr.sin_family = AF_INET;
   svaddr.sin_port = htons (atoi (argv[2])); //chuyển tham số nhập t2 thành dạng số, rồi chuyển thành host byte order

   //gửi tin nhắn
   if (sendto (fd, DATA, sizeof DATA, 0, (struct sockaddr *) &svaddr, sizeof svaddr) == -1) {
       perror ("Loi gi rui\n");
   }

   close(fd); //giải phóng sc
   exit(0);
}