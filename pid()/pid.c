#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])   /*  main() */
{
    pid_t child_pid;                
    int counter = 2;             

    printf("Counter = %d\n", counter);

    child_pid = fork();         
    if (child_pid >= 0) {
        if (0 == child_pid) {       /* Child process */
	   
            printf("\nIm the child process, counter: %d, %d\n", ++counter, &counter);
            printf("My PID is: %d, my parent PID is: %d\n", getpid(), getppid()); 
        } else {   
              /* Parent Process */
            printf("\nIm the parent process, counter: %d, %d\n", ++counter, &counter);
            printf("My PID is: %d\n", getpid());
	    sleep (3);
        }
    } else {
        printf("fork() unsuccessfully\n");      /* fork() return -1 if error.*/
    }

    return 0;
}
