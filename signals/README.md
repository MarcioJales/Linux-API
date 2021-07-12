| Name | Assignment |
| ---- | ---------- |
| sig_receiver.c  | Exercise 1 from chapter 20 |
| sig_ign.c  | Exercise 2 from chapter 20 |
| flags_effect.c  | Exercise 3 from chapter 20 |

## Chapter 20, exercise 1

Tested with the same examples from the book.

## Chapter 20, exercise 2

We use much of the structure from `sig_receiver.c` to create the program. Thus, in order to compile it, I've added `sig_ign.c` to directory where the book's applications are and its Makefile.

To test what as asked, I install a handler that, if executed, would count number of signal caught, then block it and sleep. On another terminal, the same blocked signal is sent with `kill -s $SIG`. The application will show that this signal is pending.

After that, we change its disposition to `SIG_IGN` and sleep again.

In the end, program will show that no signal was caught, despite the previous pending state:

```bash
$ ./sig_ign 
./sig_ign: PID is 10189
sleeping for 20 seconds with SIGUSR1 blocked  /* Send signal 10 with kill on another terminal */
pending signals are: 
                10 (User defined signal 1)
sleeping for 20 seconds with SIGUSR1 (hopefully) pending
Signal 10 caught 0 times
```

## Chapter 20, exercise 3

We must provide `SA_NODEFER` or `SA_RESETHAND` as an argument

