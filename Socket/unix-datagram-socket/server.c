#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#define DATA "troi oi"

int main (int argc, char *argv[])
{
    struct sockaddr_un svaddr;

    // khởi tạo unix dtg socket
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd == -1) 
        return 1;

    //khởi tạo thông tin địa chỉ
    svaddr.sun_family = AF_UNIX;
    strcpy (svaddr.sun_path, argv[1]); //ts 1

    //gửi tin nhắn cho client
    if (sendto (fd, DATA, sizeof(DATA), 0, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un)) < 0) {
        return 1;
    }
    
    //giải phóng socket
    close (fd);
}