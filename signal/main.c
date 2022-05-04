    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <signal.h>
static void signal_handler1(int signo) {
    
    if (signo == SIGINT) {
        printf("OK.\n");
        exit(EXIT_SUCCESS);
  }
}

int main(void) {
    signal (SIGINT, signal_handler1);
    while (1);
    if (signal(SIGINT, signal_handler1) == SIG_ERR) {
        printf("error.\n");
        exit(EXIT_SUCCESS);
    }

   return 0;
   }
