/* Chapter 29, exercise 1
 *
 * What possible outcomes might there be if a thread executes the following code:
 *
 * pthread_join(pthread_self(), NULL);
 *
 * Write a program to see what actually happens on Linux. If we have a variable, tid, containing a thread ID,
 * how can a thread prevent itself from making a call, pthread_join(tid, NULL), that is equivalent to the above statement?
 */

#include <pthread.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int ret;

  ret = pthread_join(pthread_self(), NULL);
  if(ret == EDEADLK)
    printf("return %d = DEADLOCK\n", ret);

  return 0;
}
