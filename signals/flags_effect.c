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
handler()
{
    fprintf(stdout, "SIGINT brought us here\n");;
}

int main(int argc, char *argv[]) 
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);          
    sa.sa_handler = handler;

    if(argc != 2) {
        fprintf(stderr, "Provide 1 argument, either SA_RESETHAND or SA_NODEFER\n");
        exit(EXIT_FAILURE);
    }

    if(!strcmp(argv[1], "SA_RESETHAND")) {
        /* To test the flag, we first install a handler that simply prints a message 
        ** We use SIGINT in this example
        */
        sa.sa_flags = SA_RESETHAND;

        (void) sigaction(SIGINT, &sa, NULL); 

        printf("%ld: Paused. Use 'kill' to send SIGINT\n", (long) getpid());
        pause();

        /* Now, we expect that no message is printed */
        printf("%ld: Paused. If you send SIGINT now, I'm going to exit!\n", (long) getpid());
        pause();
    }
    else if(!strcmp(argv[1], "SA_NODEFER")) {
        fprintf(stdout, "SA_NODEFER\n");
    }
    else {
        fprintf(stderr, "Please, provide either SA_RESETHAND or SA_NODEFER\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}