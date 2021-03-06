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
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

int main() {
  pid_t pidfork;

  /* fork() is explained on chapter 24 */
  pidfork = fork();
  if(pidfork == -1)
    exit(EXIT_FAILURE);

  if(pidfork == 0) {
    char *argVec[] = { NULL };
    char *envVec[] = { NULL };

    printf("Executing longrunner...\n");
    /* execve() is explained on chapter 27 */
    execve("./longrunner", argVec, envVec);
  }
  else {
    sleep(10);

    printf("Opening longrunner...\n");
    int fd = open("./longrunner", O_WRONLY);
    if(fd == -1) {
      if(errno == ETXTBSY)
        fprintf(stderr, "Error: ETXTBSY\n");
    }
  }

  return 0;
}