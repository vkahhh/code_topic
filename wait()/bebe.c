#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

 
int main ()
{
    int i, status;
    pid_t pid[5];
    for (i = 0; i < 5; i++)
    {
        if (0 == (pid[i] = fork()))
        {
            sleep (1);
            exit (100 + i);
        }
    }
 

    for (i = 0; i < 5; i++)
    {
        pid_t childpid = waitpid (pid[i], &status, 0);
        if (WIFEXITED(status))
            printf("Child %d terminated with status: %d\n", childpid, WEXITSTATUS(status));
    }
    return 0;
}
