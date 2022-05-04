#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  socket. 
#include <netinet/in.h>     //  Thư viện internet
#include <arpa/inet.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50  //lắng nghe tối đa 50
#define BUFF_SIZE 256      
#define handle_error (msg) \
    do { perror (msg); exit (EXIT_FAILURE); } while (0)



void chat_func(int new_socket_fd)
{       
    int numb_read, numb_write;
    char sendbuff[BUFF_SIZE];
    char recvbuff[BUFF_SIZE];
	
    while (1) {  

        memset (sendbuff, '0', BUFF_SIZE); //set senbuff = 0
        memset (recvbuff, '0', BUFF_SIZE); //tương tự

// đọc tin nhắn đến
        numb_read = read (new_socket_fd, recvbuff, BUFF_SIZE);

        if (numb_read == -1)
            handle_error ("read()");
            // Nhận được chuỗi exit -> thoát
        if (strncmp ("exit", recvbuff, 4) == 0) {
            system ("clear");
            break;
        
	}

        printf ("\nTin nhan da nhan: %s\n", recvbuff);

        //gửi tin nhắn, dùng write
        printf ("Tra loi: ");
        fgets (sendbuff, BUFF_SIZE, stdin);
        numb_write = write (new_socket_fd, sendbuff, sizeof (sendbuff));
        
	if (numb_write == -1)
            handle_error ("write()");
        
        if (strncmp ("exit", sendbuff, 4) == 0) { //nhan duoc chuoi ki tu exit
            system ("clear");
            break;
        }

        sleep (1);
    }
    close (new_socket_fd); // giải phóng
}

int main (int argc, char *argv[])
{
    int    port_no, len;
    int    server_fd, new_socket_fd;
    struct sockaddr_in serv_addr, client_addr;
    char   temp [BUFF_SIZE];

    //đọc portnum
    if (argc < 2) { //người dùng truyền thiêu tham số -> trả về lỗi
        printf ("No port provided\ncommand: ./server <port number>\n");
        exit (EXIT_FAILURE);
    } else  port_no = atoi (argv[1]); // atoi: chuyen str từ tham số 1 thanh int

    memset (&serv_addr, 0, sizeof (struct sockaddr_in));
    memset (&client_addr, 0, sizeof (struct sockaddr_in));
    
    // khởi tạo socket
    server_fd = socket (AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) handle_error ("socket()");
    

    /* ngăn thông báo : “address already in use”  ???
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        handle_error("setsockopt()");  */

    // khởi tạo các thông tin về sv
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (port_no);  //htons: chuyen int thanh host byte order (big-endian)

    serv_addr.sin_addr.s_addr =  INADDR_ANY; //chap nhan moi ip neu co su thay doi

    // binding: kết nối socket đã khởi tạo với addr cho trước
    if (bind (server_fd, (struct sockaddr*) &serv_addr, sizeof (serv_addr)) == -1)
        handle_error ("bind()");

    // tối đa 50 
    if (listen (server_fd, LISTEN_BACKLOG) == -1)
        handle_error ("listen()");

    /* Lay thong tin client */
	len = sizeof (client_addr);

    while (1) {
        printf("Dang lang nghe tu : %d \n....\n", port_no);
	
	new_socket_fd  = accept (server_fd, (struct sockaddr*) &client_addr, (socklen_t *) &len); 
	if (new_socket_fd == -1)  handle_error("accept()");
	system ("clear");

	inet_ntop(client_addr.sin_family, (struct sockaddr*) &client_addr, temp, sizeof (temp));
	//inet_ntop: function converts an IPv4 or IPv6 Internet network address into a string in Internet standard format.
	
	printf ("Ket noi tu %s\n", temp);
	chat_func (new_socket_fd);
    }
    close (server_fd);
    return 0;
}

