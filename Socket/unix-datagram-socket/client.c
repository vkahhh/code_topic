#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#define NAME "meomeo"
#define BUF_SIZE 1024

int main (int argc, char *argv[])
{
    struct sockaddr_un svaddr;
    char buf [BUF_SIZE] = {0};

    // khởi tạo unix dtg socket
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd == -1) {
        printf (" loi");
        return 1;
    }


    // khởi tạo tên và thôn gitn 
    svaddr.sun_family = AF_UNIX;
    strcpy(svaddr.sun_path, NAME);


    // binding socket với địa chỉ đã cho
    if (bind (fd, (struct sockaddr *) &svaddr, sizeof (struct sockaddr_un)) == -1) {
        printf (" loi");
         return 1;
    }
       

    printf("socket -->%s\n", NAME);


    // đọc tin nhắn từ socket
    if (read(fd, buf, 1024) < 0){
        printf (" loi");
         return 1;
    }
  

    printf("-->%s\n", buf);

    close(fd); //giải phóng sc
    unlink(NAME);

}