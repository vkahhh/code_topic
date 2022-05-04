#include <sys/types.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

int main (int argc, char *argv[])
{
 	printf ("Hello\n");
	int child_pid = fork();
 	if (-1 == child_pid)
		printf("fork() unsuccessfully\n"); 
 	else if (0 == child_pid) {
		write (STDOUT_FILENO, "Vkah\n", 5);
 		_exit (EXIT_SUCCESS);
	}
	else 
		exit (EXIT_SUCCESS);
}
