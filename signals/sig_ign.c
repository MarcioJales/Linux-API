/* Chapter 20, exercise 2
 *
 * Write a program that shows that when the disposition of a pending signal is changed to be SIG_IGN, the program never sees (catches) the signal.
 */

#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"           /* Declaration of printSigset() */
#include "tlpi_hdr.h"

static int sigCnt = 0; 

static void
handler(int sig)
{
    sigCnt++;
}

int
main(int argc, char *argv[])
{
    const int sigNumber = 10; /* Hardcoded SIGUSR1 */
    const int numSecs = 20;
    struct sigaction sa;

    printf("%s: PID is %ld\n", argv[0], (long) getpid());

    /* Install the disposition that counts number of signals caught
    */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;          
    sa.sa_handler = handler;

    (void) sigaction(sigNumber, &sa, NULL); 

    /* Add signal to the set, block it and sleep while blocked
    */

    sigemptyset(&blockSIGUSR1);
    sigaddset(&blockSIGUSR1, sigNumber);
    if (sigprocmask(SIG_BLOCK, &blockSIGUSR1, NULL) == -1)
        errExit("sigprocmask");

    printf("sleeping for %d seconds with SIGUSR1 blocked\n", numSecs);
    sleep(numSecs);

    /* Show that signal is pending and, then, change its disposition to SIG_IGN 
    */

    if (sigpending(&pendingMask) == -1)
        errExit("sigpending");

    printf("pending signals are: \n");
    printSigset(stdout, "\t\t", &pendingMask);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_IGN;

    (void) sigaction(sigNumber, &sa, NULL);

    printf("sleeping for %d seconds with SIGUSR1 (hopefully) pending\n", numSecs);
    sleep(numSecs); 

    /* Unblock the signal and print how many times it was caught
    ** We expect that none
    */

    sigdelset(&blockSIGUSR1, sigNumber);
    if (sigprocmask(SIG_UNBLOCK, &blockSIGUSR1, NULL) == -1)
        errExit("sigprocmask");

    printf("Signal %d caught %d time%s\n", sigNumber,
                    sigCnt, (sigCnt == 1) ? "" : "s");

    exit(EXIT_SUCCESS); 
}