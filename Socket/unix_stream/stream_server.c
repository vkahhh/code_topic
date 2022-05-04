#include <sys/un.h>
#include <sys/socket.h>
#include <stddef.h>
#include <ctype.h>
#include <stdio.h>

#define BUF_SIZE 10
//file socket
#define SOCK_PATH "./sock_stream"

int main (int argc, char *argv[])
{
    struct sockaddr_un svaddr;
    int fd, datafd;
    ssize_t numBytes;
    socklen_t len;
    char buf [BUF_SIZE] = {0};

//khởi tạo unix stream socket
    fd = socket (AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) 
        return 1;

//khởi tạo thông tin địa chỉ
    memset (&svaddr, 0, sizeof(struct sockaddr_un)); 
    svaddr.sun_family = AF_UNIX;
    strncpy (svaddr.sun_path, SOCK_PATH, sizeof (svaddr.sun_path));

//binding socket với các thông tin trên
    if (bind (fd, (struct sockaddr *) &svaddr, sizeof (struct sockaddr_un)) == -1) 
        return 1;
 
 // lắng nghe tối đa 5 kết nối
    if ((listen (fd, 5)) == -1)
	return 1; 
    else
	printf ("Lang nghe ket noi tu server\n");
	//
 
    if ((datafd = accept (fd, (struct sockaddr*)&svaddr, &len)) == -1)
	return 1;
    else
	printf ("accept connect\n");

	// chat nè
      while (1) {
        if (read(datafd, buf, BUF_SIZE) > 0)
		{
			for (j = 0; j < BUF_SIZE; j++) 
				buf[j] = toupper ((unsigned char) buf[j]); // chuyển tin nhắn thành in hoa
			printf ("start response %s\n", buf);
			write (datafd, buf, BUF_SIZE-1);
		}
    }
	
    close (fd); // giải phóng socket
	remove (SOCK_PATH); // xóa file socket
}

