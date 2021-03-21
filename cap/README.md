| Name | Assignment |
| ---- | ---------- |
| shed_set_cap.c  | Exercise 1 from chapter 39 |

## Ch 39, Ex 1.

The command used to compile the program on the book's source code is:

```
$ cc -std=c99 -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE -g -I../lib -pedantic -Wall -W -Wmissing-prototypes -Wno-sign-compare -Wno-unused-parameter    sched_set.c ../libtlpi.a  ../libtlpi.a -lm  -o sched_set
```

Based on this, the command we want to compile the program is:

```
$ cc -std=c99 -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE -g -I../../The-Linux-Programming-Interface/lib -pedantic -Wall -W -Wmissing-prototypes -Wno-sign-compare -Wno-unused-parameter    sched_set_cap.c ../../The-Linux-Programming-Interface/libtlpi.a -lm  -o sched_set_cap -lcap
```

The capability we need is the `CAP_SYS_NICE`, since the program uses the `sched_setscheduler()` function.

We may show that using the binary from the book, with the same example it uses, and the `setcap` utility:

```
sleep 600 &
[1] 6837
./sched_view 6837
6837: OTHER  0
$ sudo setcap "cap_sys_nice=pe" sched_set
password:
$ ./sched_set f 25 6837
$ ./sched_view 6837
6837: FIFO  25
```