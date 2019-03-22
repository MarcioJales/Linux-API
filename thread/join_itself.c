/* Chapter 29, Exercise 1
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
#include <stdlib.h>
#include <stdio.h>

void join_itself()
{
  int ret;

  ret = pthread_join(pthread_self(), NULL);
  if(ret == EDEADLK)
    printf("return %d = DEADLOCK\n", ret);
}

void prevent_join_itself(void *tid)
{
  pthread_t *thread = (pthread_t *) tid;

  if(pthread_equal(pthread_self(), *thread) == 0) {
    printf("Join from forked thread\n");
    pthread_join(*thread, NULL);
  }
}

int main(int argc, char **argv)
{
  int ret;
  pthread_t thread;
  //join_itself();

  ret = pthread_create(&thread, NULL, (void *) prevent_join_itself, (void *) &thread);
  if (ret != 0)
    exit(EXIT_FAILURE);

  if(pthread_equal(pthread_self(), thread) == 0) {
    printf("Join from main thread\n");
    pthread_join(thread, NULL);
  }

  return 0;
}

/* According to the "pthread_join" manual:

   EDEADLK
             A deadlock was detected (e.g., two threads tried to join with each other); or thread specifies the calling thread.

   As we've checked on the code, the return value was a deadlock

   To avoid this behavior, we need to assert the aforementioned tid value and pthread_self(), using pthread_equal()
*/

 /* BONUS: Chapter 29, Exercise 2

    Aside from the absence of error checking and various variable and structure declarations, what is the problem with the following program?

    static void *threadFunc(void *arg)
    {
        struct someStruct *pbuf = (struct someStruct *) arg;

        // Do some work with structure pointed to by 'pbuf'
    }
    int main(int argc, char *argv[])
    {
        struct someStruct buf;
        pthread_create(&thr, NULL, threadFunc, (void *) &buf);

        pthread_exit(NULL);
    }

********************************************************************************

    According to the book, "typically, arg points to a global or heap variable".

    Since "buf" is on the main thread's stack, its value can become undefined if the main threads exits while the other thread executes.

*/
