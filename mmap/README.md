| Name | Assignment |
| ---- | ---------- |
| cpmap.c  | Exercise 1 from chapter 49 |

## Ch 49, Ex 1.

According to the table 49.1 on the chapter, what we want (memory-mapped I/O) is achieved by a *shared file mapping*.

More information about the suggested functions `fstat()` and `ftruncate()` can be found on the book at chapters 15 and 5, respectively.

## Ch 49, Ex 3.

Files were created using `dd`:

```
$ dd if=/dev/zero of=case1.txt bs=1 count=6000
```