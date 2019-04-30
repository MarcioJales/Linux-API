| Name | Assignment |
| ---- | ---------- |
| join_itself.c | Exercise 1 from chapter 29 |

## Ch 29, Ex 1.

According to the "pthread_join" manual:

```
EDEADLK
             A deadlock was detected (e.g., two threads tried to join with each other); or thread specifies the calling thread.
```

As we've checked on the code, the return value was a deadlock

To avoid this behavior, we need to assert the aforementioned tid value and pthread_self(), using pthread_equal().

## Ch 29, Ex 2.

**Statement**:

Aside from the absence of error checking and various variable and structure declarations, what is the problem with the following program?

```
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
```

---

**Answer**:

According to the book: *"typically, arg points to a global or heap variable".*

Since `buf` is on the main thread's stack, its value can become undefined if the main threads exits while the other thread executes.

## Ch 30, Ex 2

The functions are compiled to a shared library. The process to execute a program using it is below:

```
$ gcc -c -Wall -Werror -fpic thrsafebt.c -I. [-DDEBUG] -lpthread
$ gcc -shared -o libthrsafebt.so thrsafebt.o
$ gcc -L. -I. -Wall -o thrsafebt thrsafebt_main.c -lthrsafebt -lpthread
$ LD_LIBRARY_PATH=. ./thrsafebt [ARGUMENTS]
```

The `thrsafebt` program receives three arguments, which are three keys to be searched on a binary tree. It starts three threasd to do multiple inserts, deletes and lookups on a tree.
