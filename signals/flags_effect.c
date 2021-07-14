/* Chapter 20, exercise 3
**
** Write programs that verify the effect of the SA_RESETHAND and SA_NODEFER flags when establishing a signal handler with sigaction().
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static void
handler_resethand()
{
    fprintf(stdout, "SIGINT brought us here\n");;
}

static void
handler_nodefer()
{
    fprintf(stdout, "SIGUSR1 brought us here\nAnd now I sleep 10 seconds\n");
    sleep(10);
}

int main(int argc, char *argv[]) 
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);          

    if(argc != 2) {
        fprintf(stderr, "Provide 1 argument, either SA_RESETHAND or SA_NODEFER\n");
        exit(EXIT_FAILURE);
    }

    if(!strcmp(argv[1], "SA_RESETHAND")) {
        /* To test the flag, we first install a handler that simply prints a message 
        ** We use SIGINT in this example
        */
        sa.sa_handler = handler_resethand;
        sa.sa_flags = SA_RESETHAND;

        (void) sigaction(SIGINT, &sa, NULL); 

        printf("%ld: Paused. Use 'kill' to send SIGINT\n", (long) getpid());
        pause();

        /* Now, we expect that no message is printed */
        printf("%ld: Paused. If you send SIGINT now, I'm going to exit!\n", (long) getpid());
        pause();
    }
    else if(!strcmp(argv[1], "SA_NODEFER")) {
        /* We use SIGUSR1 in this example 
        ** First, we show that signal is blocked indeed
        */
        sa.sa_handler = handler_nodefer;
        sa.sa_flags = 0;
        
        (void) sigaction(SIGUSR1, &sa, NULL);
        
        printf("%ld: Paused. Use 'kill' to send SIGUSR1\n", (long) getpid());
        pause();

        /* Provide the flag*
        ** If we send multiple SIGUSR1, message will be immediately printed, instead of the signal being pending
        */
        sa.sa_flags = SA_NODEFER;
        
        (void) sigaction(SIGUSR1, &sa, NULL);
        
        printf("%ld: Paused again. Use 'kill' to send SIGUSR1\n", (long) getpid());
        pause();
    }
    else {
        fprintf(stderr, "Please, provide either SA_RESETHAND or SA_NODEFER\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}