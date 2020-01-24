#include <stdio.h>
#include <libgen.h>
#include <pthread.h>
#include "/home/marciojales/Estudo/Geral/Linux-SO/The-Linux-Programming-Interface/lib/tlpi_hdr.h"

char *p;

static void *
threadFunc(void *arg)
{
    char *path = "/var/log/log.txt";
    char *str, *str2;

    p = strdup(path);
    printf("Other thread about to call functions\n");
    str = basename(p);
    str2 = dirname(p);
    printf("Other thread: str (%p) = %s\n", str, str);
    printf("Other thread: str (%p) = %s\n", str2, str2);

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t;
    int s;
    char *path = "/etc/hosts";
    char *str, *str2;

    p = strdup(path);
    str = basename(p);
    str2 = dirname(p);
    printf("Main thread has called functions\n");

    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    /* If strerror() is not thread-safe, then the output of this printf() be
       the same as that produced by the analogous printf() in threadFunc() */

    printf("Main thread:  str (%p) = %s\n", str, str);
    printf("Main thread:  str (%p) = %s\n", str2, str2);

    exit(EXIT_SUCCESS);
}
