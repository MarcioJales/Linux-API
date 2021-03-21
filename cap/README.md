| Name | Assignment |
| ---- | ---------- |
| shed_set_cap.c  | Exercise 1 from chapter 39 |

## Ch 39, Ex 1.

The command used to compile the program on the book's source code is:

```
$ cc -std=c99 -D_XOPEN_SOURCE=600 -D_DEFAULT_SOURCE -g -I../lib -pedantic -Wall -W -Wmissing-prototypes -Wno-sign-compare -Wno-unused-parameter    sched_set.c ../libtlpi.a  ../libtlpi.a -lm  -o sched_set
```

The capability we need is the `CAP_SYS_NICE`, since the program uses the `sched_setscheduler()` function.