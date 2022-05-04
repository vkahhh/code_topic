#include <sys/un.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdio.h>

#define BUF_SIZE 10 
#define SOCK_PATH "./sock_stream"

int main(int argc, char *argv[])
{    
	struct sockaddr_un svaddr;
    int fd;
    size_t msgLen;
    ssize_t numBytes;
    char buff [BUF_SIZE]={0};

// Tạo unix stream socket	
    fd = socket (AF_UNIX, SOCK_STREAM, 0);      
    if (fd == -1)
		return 1;
	
    //khởi tạo các thông tin địa chỉ
    memset (&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy (svaddr.sun_path, SOCK_PATH, sizeof (svaddr.sun_path) - 1);
	
    // connect socket với địa chỉ
	if (connect (fd, (struct sockaddr*)&svaddr, sizeof (struct sockaddr)) != 0) 
	{ 
        printf ("Ket noi bi loi...\n"); 
        return 1; 
    } 
    else
	{
        printf ("Da ket noi.\n"); 
	}
	printf ("Tin gui : %s\n", argv[1]);  // ghi vào tham số 1
	write (fd, argv[1], BUF_SIZE);
	
    //chat nè
	while (1)
	{      
        //đọc tin nhắn từ sv
        if (read(fd, buff, sizeof (buff)) > 0) 
			printf ("Tin da nhan : %s\n", buff); 
    } 
    
    return 0;
}

