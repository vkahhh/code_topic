#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])   
{
    /* code */
    pid_t child_pid;               
    int status, rv;

    child_pid = fork();         
    if (0 <= child_pid) {
        if (0 == child_pid) {       /*Child Process  */
            printf("Im the child process, my PID: %d\n", getpid());
	    sleep(5);


        } else {                     /* Parent Process */
            rv = wait(&status);
            if (-1 == rv) {
                printf("wait() unsuccessful\n");
            } else {
                printf("\nIm the parent process, PID child process: %d\n", rv);
            }
        }
    } else {
        printf("fork() unsuccessfully\n");      
    }

    return 0;
}
