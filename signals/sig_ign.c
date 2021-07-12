/* Chapter 20, exercise 2
 *
 * Write a program that shows that when the disposition of a pending signal is changed to be SIG_IGN, the program never sees (catches) the signal.
 */

#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"           /* Declaration of printSigset() */
#include "tlpi_hdr.h"

static void
handler(int sig)
{
}

int
main(int argc, char *argv[])
{
    const int sigNumber = 10; /* Hardcoded SIGUSR1 */
    const int numSecs = 20;   
}