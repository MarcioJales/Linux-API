#include <stdio.h>
#include <libgen.h>
#include <pthread.h>
#include "/home/marciojales/Estudo/Geral/Linux-SO/The-Linux-Programming-Interface/lib/tlpi_hdr.h"

static void *
threadFunc(void *arg)
{
    char *path = "/var/log/log.txt";
    char *str, *p;

    p = strdup(path);
    printf("Other thread about to call dirname\n");
    str = dirname(p);
    printf("Other thread: str (%p) = %s\n", str, str);

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t;
    int s;
    char *path = "/etc/nginx/nginx.conf";
    char *str, *p;

    p = strdup(path);
    str = dirname(p);
    printf("Main thread has called dirname\n");
    printf("Main thread: str (%p) = %s\n", str, str);

    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("Main thread: str (%p) = %s\n", str, str);

    exit(EXIT_SUCCESS);
}
