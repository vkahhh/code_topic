#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MSG_SIZE    16

char *msg1 = "hello, world #1";
char *msg2 = "hello, world #2";
char *msg3 = "hello, world #3";

static void func(int signum)
{
    wait(NULL);
}

int main(int argc, char const *argv[])
{

    char in_buff [MSG_SIZE];
    int num_read = 0;
    int fds1[2], fds2[2]; //tao 2 pipe 
    pid_t child_pid;

    //fd[0]: quá trình đọc (read)
    // fd[1]: quá trình ghi (write)

    /* Tạo 2 pipe để truyền dữ liệu 2 chiều.
     phía cha và con đều phải block một chiều 
     nào đó: tiến trình cha hoặc block read 
     hoặc block write, và tiến trình con block 
     chiều ngược lại.
    */
    //Nếu 1 trong 2 pipe bị lỗi
    //Phải tạo pipe lần lượt nên không thể dùng dấu ||

    if (pipe (fds1) < 0) {
        printf ("Loi");
        exit (1);
    }

    if ( (pipe (fds2) < 0)) {
        printf ("Loi");
        exit (1);
    }

    child_pid = fork (); //tạo process
    if (child_pid < 0) { //In log nếu gặp lỗi
        printf ("Loi\n");
        exit (1);
    }
    else if (child_pid >= 0) {
        if (child_pid > 0) { //tiến trình cha
            
            printf("I am Parent\n"); 
            if (close(fds2[1]) == -1) //block phía ghi ở pipe 2 
                printf("close(fds2[1]) failed\n");
            
            if (close(fds1[0]) == -1) //ở phía pipe 1, block phía đọc
                printf("close(fds1[0]) failed\n");


            // Block cho đến khi có dữ liệu đi vào pipe: quá trình read bị block cho đến khi đọc được ít nhất 1 byte từ pipe (LT)
            while (1 ) {
                num_read = read(fds2[0], in_buff, MSG_SIZE); //đọc DL 
                if (num_read == -1) {
                    printf("read() failed\n");
                    exit(0);
                } else if (num_read == 0) { // đọc được giá trị end-of-pipe: giá trị cuối mà nó phải đọc ra từ pipe
                    printf("pipe end-of-pipe\n");
                    break;
                } else {
                    printf("msg2: %s\n", in_buff); //in tin nhắn đọc được
                }
             };
                write(fds1[1], "HELLO", sizeof ("HELLO"));   // viết chuỗi HELLO vào pipe

        } else { //tien trinh con


            printf("I am Children\n"); 
            if (close(fds2[0]) == -1) //tạm block quá trình đọc ở pipe 2
                printf("close(fds2[0]) failed\n");

            if (close(fds1[1]) == -1) // tạm block quá trình ghi ở pipe 1
                printf("close(fds1[1]) failed\n");


            // viết tin nhắn vào pipe 2
            write(fds2[1], msg1, MSG_SIZE);
            write(fds2[1], msg2, MSG_SIZE);
            write(fds2[1], msg3, MSG_SIZE);


            if (close(fds2[1]) == -1) 
                printf("close(fds[0]) failed\n");
            
            //đọc tin nhắn từ pipe 1
            read(fds1[0], in_buff, MSG_SIZE);
            printf ("msg: %s\n", in_buff);

            while(1);
        }
    } else {
        printf("fork() unsuccessfully\n"); // tạo process con không thành công
    }
    return 0;
}
