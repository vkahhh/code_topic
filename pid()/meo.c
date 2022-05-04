#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])   
{
    /* code */
    pid_t child_pid;               
    int status;
    int counter = 0;
 
        child_pid = fork();         
	if (child_pid >= 0) {
		if (0 == child_pid) {     /*Child Process*/  
			printf("Child process termination, PID = %d\n", getpid());
        	        exit (EXIT_SUCCESS);  
	        } else {		/*Parent Process*/
			wait (&status);
			while (1);
			
				
		}

	} else {                       
	        printf("fork() unsuccessfully\n"); 
	}

    return 0;
}
