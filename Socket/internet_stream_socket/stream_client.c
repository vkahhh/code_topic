#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc socket. 
#include <netinet/in.h>     //  Thư viện internet
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 256
// hàm trả về lỗi
#define handle_error (msg) \
    do { perror (msg); exit (EXIT_FAILURE); } while (0)
		
//chat
void chat_func (int server_fd)
{
    int numb_write, numb_read; 
    char recvbuff [BUFF_SIZE]; 
    char sendbuff [BUFF_SIZE];

    while (1) {
        memset (sendbuff, '0', BUFF_SIZE); //set sendbuff = 0
        memset (recvbuff, '0', BUFF_SIZE); //set recvbuff = 0
        
	printf ("Nhan tin di : ");
	fgets  (sendbuff, BUFF_SIZE, stdin);

        //gửi tin nhắn đến server
        numb_write = write (server_fd, sendbuff, sizeof (sendbuff));
        
	if (numb_write == -1)  handle_error ("write()");
        if (strncmp ("exit", sendbuff, 4) == 0) { 
	//Nhận về exit thì thoát (4: số kí tự tối đa)
            printf ("Thoat ...\n");
            break;
        }
		
        /* đọc thông tin từ server */
        numb_read = read (server_fd, recvbuff, sizeof (recvbuff));
        
	if (numb_read < 0) 
            handle_error ("read()");
        // đọc được chuỗi exit -> thoát
        if (strncmp ("exit", recvbuff, 4) == 0) {
            printf ("Thoat ...\n");
            break;
        }

        printf ("\nTin nhan da nhan: %s\n", recvbuff);   
    }
    
    close (server_fd); // giải phóng sc
}

int main (int argc, char *argv[])
{
    int portno;
    int server_fd;
    struct sockaddr_in serv_addr;

    memset (&serv_addr, '0', sizeof(serv_addr)); //đặt serv_addr = 0
	
    //đọc portnum
    if (argc < 3) { //thiếu một trong các tham số thì thông báo 
        printf ("command : ./client <server address> <port number>\n");
        exit (1);
    }
    
    portno = atoi (argv[2]); //atoi: chuyển chuỗi thành số nguyên, lấy từ tham số t2
    //int atoi (const char *str)
	
    //khởi tạo server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons (portno); 
    //htons: chuyen int thanh host byte order (big-endian)
    if (inet_pton (AF_INET, argv[1], &serv_addr.sin_addr) == -1) 
        handle_error ("inet_pton()");
	//inet_pton: convert IPv4 and IPv6 addresses from text to binary form
    // khởi tạo socket
    server_fd = socket (AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error ("socket()");
	
    // kết nối
    if (connect (server_fd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) == -1)
        handle_error ("connect()");
	
    chat_func (server_fd);

    return 0;
}


