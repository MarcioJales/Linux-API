// Chapter 18, exercise 1
// 
// In Section 4.3.2, we noted that it is not possible to open a file for writing if it is currently being executed (open() returns –1, with errno set to ETXTBSY). 
// Nevertheless, it is possible to do the following from the shell:
// $ cc -o longrunner longrunner.c
// $ ./longrunner &                       Leave running in background
// $ vi longrunner.c                      Make some changes to the source code
// $ cc -o longrunner longrunner.c
// The last command overwrites the existing executable of the same name. How is this possible? 
// (For a clue, use ls –li to look at the i-node number of the executable file after each compilation.)

#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t pidfork;

  /* Fork is explained at chapter 24 */
  pidfork = fork();
  if(pidfork == -1)
    exit(EXIT_FAILURE);

  if(pidfork == 0) {
  }
  else {

  }
}