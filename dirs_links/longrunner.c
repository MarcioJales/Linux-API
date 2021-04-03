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

#include <sys/time.h>

int main() {
  int x = 0;
  struct timeval start, end;
  gettimeofday(&start, ((void *) 0));
  gettimeofday(&end, ((void *) 0));

  while((double)end.tv_sec - (double)start.tv_sec < 20) {
    x = x++;
    x = x--;
    gettimeofday(&end, ((void *) 0));
  }

  return 0;
}